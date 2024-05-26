//
//	mc6800.cpp
//

#include <stdlib.h>
#include <stdio.h>
#include "machdep.h"
#include "mc6800.h"

#define	pPPC	ppc
#define pPC 	pc
#define pS		s
#define pX		x
#define pD		d

#define PC		pc.w.l
#define PCD		pc.d
#define S		s.w.l
#define SD		s.d
#define X		x.w.l
#define D		d.w.l
#define A		d.b.h
#define B		d.b.l
#define CC		cc

#define CT		counter.w.l
#define CTH		counter.w.h
#define CTD		counter.d
#define OC		output_compare.w.l
#define OCH		output_compare.w.h
#define OCD		output_compare.d
#define TOH		timer_over.w.l
#define TOD		timer_over.d

static PAIR ea; 		/* effective address */
#define EAD ea.d
#define EA	ea.w.l

/* public globals */
int m6800_ICount=50000;

/* point of next timer event */
static UINT32 timer_next;

/* DS -- THESE ARE RE-DEFINED IN h TO RAM, ROM or FUNCTIONS IN cpuintrf.c */
#define RM(Addr)		read2(Addr)
#define WM(Addr,Value)	write2(Addr,Value)
#define M_RDOP(Addr)		read2(Addr)
#define M_RDOP_ARG(Addr)	read2(Addr)

/* macros to access memory */
#define IMMBYTE(b)	b = M_RDOP_ARG(PCD); PC++
#define IMMWORD(w)	w.d = (M_RDOP_ARG(PCD)<<8) | M_RDOP_ARG((PCD+1)&0xffff); PC+=2

#define PUSHBYTE(b) WM(SD,b); --S
#define PUSHWORD(w) WM(SD,w.b.l); --S; WM(SD,w.b.h); --S
#define PULLBYTE(b) S++; b = RM(SD)
#define PULLWORD(w) S++; w.d = RM(SD)<<8; S++; w.d |= RM(SD)

#define MODIFIED_tcsr {	\
	irq2 = (tcsr&(tcsr<<3))&(TCSR_ICF|TCSR_OCF|TCSR_TOF); \
}

#define SET_TIMRE_EVENT {					\
	timer_next = (OCD < TOD) ? OCD : TOD;	\
}

/* cleanup high-word of counters */
#define CLEANUP_conters {						\
	OCH -= CTH;									\
	TOH -= CTH;									\
	CTH = 0;									\
	SET_TIMRE_EVENT;							\
}

/* when change freerunningcounter or outputcapture */
#define MODIFIED_counters {						\
	OCH = (OC >= CT) ? CTH : CTH+1;				\
	SET_TIMRE_EVENT;							\
}

/* take interrupt */
#define TAKE_ICI ENTER_INTERRUPT(0xfff6)
#define TAKE_OCI ENTER_INTERRUPT(0xfff4)
#define TAKE_TOI ENTER_INTERRUPT(0xfff2)
#define TAKE_SCI ENTER_INTERRUPT(0xfff0)
#define TAKE_TRAP ENTER_INTERRUPT(0xffee)

/* check IRQ2 (internal irq) */
#define CHECK_IRQ2 {											\
	if(irq2&(TCSR_ICF|TCSR_OCF|TCSR_TOF))					\
	{															\
		if(irq2&TCSR_ICF)									\
		{														\
			TAKE_ICI;											\
			if( irq_callback )							\
				(void)(*irq_callback)(M6800_TIN_LINE);	\
		}														\
		else if(irq2&TCSR_OCF)							\
		{														\
			TAKE_OCI;											\
		}														\
		else if(irq2&TCSR_TOF)							\
		{														\
			TAKE_TOI;											\
		}														\
	}															\
}

/* operate one instruction for */
#define ONE_MORE_INSN() {		\
	UINT8 ireg; 							\
	pPPC = pPC; 							\
	ireg=M_RDOP(PCD);						\
	PC++;									\
	(*insn[ireg])();					\
	INCREMENT_COUNTER(cycles[ireg]);	\
}

/* check the IRQ lines for pending interrupts */
#define CHECK_IRQ_LINES() {										\
	if( !(CC & 0x10) )											\
	{															\
		if( irq_state[M6800_IRQ_LINE] != M6800_CLEAR_LINE )		\
		{	/* stanadrd IRQ */									\
			ENTER_INTERRUPT(0xfff8);		\
			if( irq_callback )							\
				(void)(*irq_callback)(M6800_IRQ_LINE);	\
		}														\
		else													\
			CHECK_IRQ2;											\
	}															\
}

/* CC masks                       HI NZVC
								7654 3210	*/
#define CLR_HNZVC	CC&=0xd0
#define CLR_NZV 	CC&=0xf1
#define CLR_HNZC	CC&=0xd2
#define CLR_NZVC	CC&=0xf0
#define CLR_Z		CC&=0xfb
#define CLR_NZC 	CC&=0xf2
#define CLR_ZC		CC&=0xfa
#define CLR_C		CC&=0xfe

/*  macros for CC --
    CC bits affected should be reset before calling */
#define SET_Z(a)		if(!a)SEZ
#define SET_Z8(a)		SET_Z((UINT8)a)
#define SET_Z16(a)		SET_Z((UINT16)a)
#define SET_N8(a)		CC|=((a&0x80)>>4)
#define SET_N16(a)		CC|=((a&0x8000)>>12)
#define SET_H(a,b,r)	CC|=(((a^b^r)&0x10)<<1)
#define SET_C8(a)		CC|=((a&0x100)>>8)
#define SET_C16(a)		CC|=((a&0x10000)>>16)
#define SET_V8(a,b,r)	CC|=(((a^b^r^(r>>1))&0x80)>>6)
#define SET_V16(a,b,r)	CC|=(((a^b^r^(r>>1))&0x8000)>>14)

static const UINT8 flags8i[256]=	 /* increment */
{
0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x0a,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08
};
static const UINT8 flags8d[256]= /* decrement */
{
0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08
};
#define SET_FLAGS8I(a)		{CC|=flags8i[(a)&0xff];}
#define SET_FLAGS8D(a)		{CC|=flags8d[(a)&0xff];}

/* combos */
#define SET_NZ8(a)			{SET_N8(a);SET_Z(a);}
#define SET_NZ16(a)			{SET_N16(a);SET_Z(a);}
#define SET_FLAGS8(a,b,r)	{SET_N8(r);SET_Z8(r);SET_V8(a,b,r);SET_C8(r);}
#define SET_FLAGS16(a,b,r)	{SET_N16(r);SET_Z16(r);SET_V16(a,b,r);SET_C16(r);}

/* for treating an UINT8 as a signed INT16 */
#define SIGNED(b) ((INT16)(b&0x80?b|0xff00:b))

/* Macros for addressing modes */
#define DIRECT IMMBYTE(EAD)
#define IMM8 EA=PC++
#define IMM16 {EA=PC;PC+=2;}
#define EXTENDED IMMWORD(ea)
#define INDEXED {EA=X+(UINT8)M_RDOP_ARG(PCD);PC++;}

/* macros to set status flags */
#define SEC CC|=0x01
#define CLC CC&=0xfe
#define SEZ CC|=0x04
#define CLZ CC&=0xfb
#define SEN CC|=0x08
#define CLN CC&=0xf7
#define SEV CC|=0x02
#define CLV CC&=0xfd
#define SEH CC|=0x20
#define CLH CC&=0xdf
#define SEI CC|=0x10
#define CLI CC&=~0x10

/* mnemonics for the Timer Control and Status Register bits */
#define TCSR_OLVL 0x01
#define TCSR_IEDG 0x02
#define TCSR_ETOI 0x04
#define TCSR_EOCI 0x08
#define TCSR_EICI 0x10
#define TCSR_TOF  0x20
#define TCSR_OCF  0x40
#define TCSR_ICF  0x80

#define INCREMENT_COUNTER(amount)	\
{									\
	m6800_ICount += amount;			\
	CTD += amount;					\
	if( CTD >= timer_next)			\
		check_timer_event();		\
}

#define EAT_CYCLES													\
{																	\
	int cycles_to_eat;												\
																	\
	cycles_to_eat = timer_next - CTD;								\
	if( cycles_to_eat > m6800_ICount) cycles_to_eat = m6800_ICount;	\
	if (cycles_to_eat > 0)											\
	{																\
		INCREMENT_COUNTER(cycles_to_eat);							\
	}																\
}

/* macros for convenience */
#define DIRBYTE(b) {DIRECT;b=RM(EAD);}
#define DIRWORD(w) {DIRECT;w.d=RM16(EAD);}
#define EXTBYTE(b) {EXTENDED;b=RM(EAD);}
#define EXTWORD(w) {EXTENDED;w.d=RM16(EAD);}

#define IDXBYTE(b) {INDEXED;b=RM(EAD);}
#define IDXWORD(w) {INDEXED;w.d=RM16(EAD);}

/* Macros for branch instructions */
#define CHANGE_PC()
#define BRANCH(f) {IMMBYTE(t);if(f){PC+=SIGNED(t);CHANGE_PC();}}
#define NXORV  ((CC&0x08)^((CC&0x02)<<2))

static const UINT8 cycles_6800[] =
{
		/* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
	/*0*/  0, 2, 0, 0, 0, 0, 2, 2, 4, 4, 2, 2, 2, 2, 2, 2,
	/*1*/  2, 2, 0, 0, 0, 0, 2, 2, 0, 2, 0, 2, 0, 0, 0, 0,
	/*2*/  4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	/*3*/  4, 4, 4, 4, 4, 4, 4, 4, 0, 5, 0,10, 0, 0, 9,12,
	/*4*/  2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2,
	/*5*/  2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2,
	/*6*/  7, 0, 0, 7, 7, 0, 7, 7, 7, 7, 7, 0, 7, 7, 4, 7,
	/*7*/  6, 0, 0, 6, 6, 0, 6, 6, 6, 6, 6, 0, 6, 6, 3, 6,
	/*8*/  2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 3, 8, 3, 0,
	/*9*/  3, 3, 3, 0, 3, 3, 3, 4, 3, 3, 3, 3, 4, 0, 4, 5,
	/*A*/  5, 5, 5, 0, 5, 5, 5, 6, 5, 5, 5, 5, 6, 8, 6, 7,
	/*B*/  4, 4, 4, 0, 4, 4, 4, 5, 4, 4, 4, 4, 5, 9, 5, 6,
	/*C*/  2, 2, 2, 0, 2, 2, 2, 0, 2, 2, 2, 2, 0, 0, 3, 0,
	/*D*/  3, 3, 3, 0, 3, 3, 3, 4, 3, 3, 3, 3, 0, 0, 4, 5,
	/*E*/  5, 5, 5, 0, 5, 5, 5, 6, 5, 5, 5, 5, 0, 0, 6, 7,
	/*F*/  4, 4, 4, 0, 4, 4, 4, 5, 4, 4, 4, 4, 0, 0, 5, 6
};

static const UINT8 cycles_6303[] =
{
		  /* 0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F */
	/*0*/  0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	/*1*/  1, 1, 0, 0, 0, 0, 1, 1, 2, 2, 4, 1, 0, 0, 0, 0,
	/*2*/  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	/*3*/  1, 1, 3, 3, 1, 1, 4, 4, 4, 5, 1,10, 5, 7, 9,12,
	/*4*/  1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 9, 1,
	/*5*/  1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1,
	/*6*/  6, 7, 7, 6, 6, 7, 6, 6, 6, 6, 6, 5, 6, 4, 3, 5,
	/*7*/  6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 4, 6, 4, 3, 5,
	/*8*/  2, 2, 2, 3, 2, 2, 2, 0, 2, 2, 2, 2, 3, 5, 3, 0,
	/*9*/  3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 5, 4, 4,
	/*A*/  4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
	/*B*/  4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 6, 5, 5,
	/*C*/  2, 2, 2, 3, 2, 2, 2, 0, 2, 2, 2, 2, 3, 0, 3, 0,
	/*D*/  3, 3, 3, 4, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4,
	/*E*/  4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5,
	/*F*/  4, 4, 4, 5, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5
};

/*

HNZVC

? = undefined
* = affected
- = unaffected
0 = cleared
1 = set
# = ccr directly affected by instruction
@ = special - carry set if bit 7 is set

*/

//void illegal( void )
void mc6800::illegal( void )
{
}

/* HD63701 only */
//void trap( void )
void mc6800::trap( void )
{
	TAKE_TRAP;
}

/* $00 ILLEGAL */

/* $01 NOP */
void mc6800::nop( void )
{
}

/* $02 ILLEGAL */

/* $03 ILLEGAL */

/* $04 LSRD inherent -0*-* */
void mc6800::lsrd (void)
{
	UINT16 t;
	CLR_NZC; t = D; CC|=(t&0x0001);
	t>>=1; SET_Z16(t); D=t;
}

/* $05 ASLD inherent ?**** */
void mc6800::asld (void)
{
	int r;
	UINT16 t;
	t = D; r=t<<1;
	CLR_NZVC; SET_FLAGS16(t,t,r);
	D=r;
}

/* $06 TAP inherent ##### */
void mc6800::tap (void)
{
	CC=A;
//	ONE_MORE_INSN();
	CHECK_IRQ_LINES(); /* HJB 990417 */
}

/* $07 TPA inherent ----- */
void mc6800::tpa (void)
{
	A=CC;
}

/* $08 INX inherent --*-- */
void mc6800::inx (void)
{
	++X;
	CLR_Z; SET_Z16(X);
}

/* $09 DEX inherent --*-- */
void mc6800::dex (void)
{
	--X;
	CLR_Z; SET_Z16(X);
}

/* $0a CLV */
void mc6800::clv (void)
{
	CLV;
}

/* $0b SEV */
void mc6800::sev (void)
{
	SEV;
}

/* $0c CLC */
void mc6800::clc (void)
{
	CLC;
}

/* $0d SEC */
void mc6800::sec (void)
{
	SEC;
}

/* $0e CLI */
void mc6800::cli (void)
{
	CLI;
//	ONE_MORE_INSN();
	CHECK_IRQ_LINES(); /* HJB 990417 */
}

/* $0f SEI */
void mc6800::sei (void)
{
	SEI;
//	ONE_MORE_INSN();
	CHECK_IRQ_LINES(); /* HJB 990417 */
}

/* $10 SBA inherent -**** */
void mc6800::sba (void)
{
	UINT16 t;
	t=A-B;
	CLR_NZVC; SET_FLAGS8(A,B,t);
	A=t;
}

/* $11 CBA inherent -**** */
void mc6800::cba (void)
{
	UINT16 t;
	t=A-B;
	CLR_NZVC; SET_FLAGS8(A,B,t);
}

/* $12 ILLEGAL */
void mc6800::undoc1 (void)
{
	X += RM( S + 1 );
}

/* $13 ILLEGAL */
void mc6800::undoc2 (void)
{
	X += RM( S + 1 );
}


/* $14 ILLEGAL */

/* $15 ILLEGAL */

/* $16 TAB inherent -**0- */
void mc6800::tab (void)
{
	B=A;
	CLR_NZV; SET_NZ8(B);
}

/* $17 TBA inherent -**0- */
void mc6800::tba (void)
{
	A=B;
	CLR_NZV; SET_NZ8(A);
}

/* $18 XGDX inherent ----- */ /* HD63701YO only */
void mc6800::xgdx( void )
{
	UINT16 t = X;
	X = D;
	D=t;
}

/* $19 DAA inherent (A) -**0* */
void mc6800::daa( void )
{
	UINT8 msn, lsn;
	UINT16 t, cf = 0;
	msn=A & 0xf0; lsn=A & 0x0f;
	if( lsn>0x09 || CC&0x20 ) cf |= 0x06;
	if( msn>0x80 && lsn>0x09 ) cf |= 0x60;
	if( msn>0x90 || CC&0x01 ) cf |= 0x60;
	t = cf + A;
	CLR_NZV; /* keep carry from previous operation */
	SET_NZ8((UINT8)t); SET_C8(t);
	A = t;
}

/* $1a ILLEGAL */

#if (HAS_HD63701)
/* $1a SLP */ /* HD63701YO only */
void mc6800::slp (void)
{
	/* wait for next IRQ (same as waiting of wai) */
	wai_state |= HD63701_SLP;
	EAT_CYCLES;
}
#endif

/* $1b ABA inherent ***** */
void mc6800::aba (void)
{
	UINT16 t;
	t=A+B;
	CLR_HNZVC; SET_FLAGS8(A,B,t); SET_H(A,B,t);
	A=t;
}

/* $1c ILLEGAL */

/* $1d ILLEGAL */

/* $1e ILLEGAL */

/* $1f ILLEGAL */

/* $20 BRA relative ----- */
void mc6800::bra( void )
{
	UINT8 t;
	IMMBYTE(t);PC+=SIGNED(t);CHANGE_PC();
	/* speed up busy loops */
	if (t==0xfe) EAT_CYCLES;
}

/* $21 BRN relative ----- */
void mc6800::brn( void )
{
	UINT8 t;
	IMMBYTE(t);
}

/* $22 BHI relative ----- */
void mc6800::bhi( void )
{
	UINT8 t;
	BRANCH(!(CC&0x05));
}

/* $23 BLS relative ----- */
void mc6800::bls( void )
{
	UINT8 t;
	BRANCH(CC&0x05);
}

/* $24 BCC relative ----- */
void mc6800::bcc( void )
{
	UINT8 t;
	BRANCH(!(CC&0x01));
}

/* $25 BCS relative ----- */
void mc6800::bcs( void )
{
	UINT8 t;
	BRANCH(CC&0x01);
}

/* $26 BNE relative ----- */
void mc6800::bne( void )
{
	UINT8 t;
	BRANCH(!(CC&0x04));
}

/* $27 BEQ relative ----- */
void mc6800::beq( void )
{
	UINT8 t;
	BRANCH(CC&0x04);
}

/* $28 BVC relative ----- */
void mc6800::bvc( void )
{
	UINT8 t;
	BRANCH(!(CC&0x02));
}

/* $29 BVS relative ----- */
void mc6800::bvs( void )
{
	UINT8 t;
	BRANCH(CC&0x02);
}

/* $2a BPL relative ----- */
void mc6800::bpl( void )
{
	UINT8 t;
	BRANCH(!(CC&0x08));
}

/* $2b BMI relative ----- */
void mc6800::bmi( void )
{
	UINT8 t;
	BRANCH(CC&0x08);
}

/* $2c BGE relative ----- */
void mc6800::bge( void )
{
	UINT8 t;
	BRANCH(!NXORV);
}

/* $2d BLT relative ----- */
void mc6800::blt( void )
{
	UINT8 t;
	BRANCH(NXORV);
}

/* $2e BGT relative ----- */
void mc6800::bgt( void )
{
	UINT8 t;
	BRANCH(!(NXORV||CC&0x04));
}

/* $2f BLE relative ----- */
void mc6800::ble( void )
{
	UINT8 t;
	BRANCH(NXORV||CC&0x04);
}

/* $30 TSX inherent ----- */
void mc6800::tsx (void)
{
	X = ( S + 1 );
}

/* $31 INS inherent ----- */
void mc6800::ins (void)
{
	++S;
}

/* $32 PULA inherent ----- */
void mc6800::pula (void)
{
	PULLBYTE(d.b.h);
}

/* $33 PULB inherent ----- */
void mc6800::pulb (void)
{
	PULLBYTE(d.b.l);
}

/* $34 DES inherent ----- */
void mc6800::des (void)
{
	--S;
}

/* $35 TXS inherent ----- */
void mc6800::txs (void)
{
	S = ( X - 1 );
}

/* $36 PSHA inherent ----- */
void mc6800::psha (void)
{
	PUSHBYTE(d.b.h);
}

/* $37 PSHB inherent ----- */
void mc6800::pshb (void)
{
	PUSHBYTE(d.b.l);
}

/* $38 PULX inherent ----- */
void mc6800::pulx (void)
{
	PULLWORD(pX);
}

/* $39 RTS inherent ----- */
void mc6800::rts( void )
{
	PULLWORD(pPC);
	CHANGE_PC();
}

/* $3a ABX inherent ----- */
void mc6800::abx( void )
{
	X += B;
}

/* $3b RTI inherent ##### */
void mc6800::rti( void )
{
	PULLBYTE(CC);
	PULLBYTE(B);
	PULLBYTE(A);
	PULLWORD(pX);
	PULLWORD(pPC);
	CHANGE_PC();
	CHECK_IRQ_LINES(); /* HJB 990417 */
}

/* $3c PSHX inherent ----- */
void mc6800::pshx (void)
{
	PUSHWORD(pX);
}

/* $3d MUL inherent --*-@ */
void mc6800::mul( void )
{
	UINT16 t;
	t=A*B;
	CLR_C; if(t&0x80) SEC;
	D=t;
}

/* $3e WAI inherent ----- */
void mc6800::wai( void )
{
	/*
	 * WAI stacks the entire machine state on the
	 * hardware stack, then waits for an interrupt.
	 */
	wai_state |= M6808_WAI;
	PUSHWORD(pPC);
	PUSHWORD(pX);
	PUSHBYTE(A);
	PUSHBYTE(B);
	PUSHBYTE(CC);
	CHECK_IRQ_LINES();
	if (wai_state & M6808_WAI) EAT_CYCLES;
}

/* $3f SWI absolute indirect ----- */
void mc6800::swi( void )
{
	PUSHWORD(pPC);
	PUSHWORD(pX);
	PUSHBYTE(A);
	PUSHBYTE(B);
    PUSHBYTE(CC);
    SEI;
	PCD = RM16(0xfffa);
	CHANGE_PC();
}

/* $40 NEGA inherent ?**** */
void mc6800::nega( void )
{
	UINT16 r;
	r=-A;
	CLR_NZVC; SET_FLAGS8(0,A,r);
	A=r;
}

/* $41 ILLEGAL */

/* $42 ILLEGAL */

/* $43 COMA inherent -**01 */
void mc6800::coma( void )
{
	A = ~A;
	CLR_NZV; SET_NZ8(A); SEC;
}

/* $44 LSRA inherent -0*-* */
void mc6800::lsra( void )
{
	CLR_NZC; CC|=(A&0x01);
	A>>=1; SET_Z8(A);
}

/* $45 ILLEGAL */

/* $46 RORA inherent -**-* */
void mc6800::rora( void )
{
	UINT8 r;
	r=(CC&0x01)<<7;
	CLR_NZC; CC|=(A&0x01);
	r |= A>>1; SET_NZ8(r);
	A=r;
}

/* $47 ASRA inherent ?**-* */
void mc6800::asra( void )
{
	CLR_NZC; CC|=(A&0x01);
	A>>=1; A|=((A&0x40)<<1);
	SET_NZ8(A);
}

/* $48 ASLA inherent ?**** */
void mc6800::asla( void )
{
	UINT16 r;
	r=A<<1;
	CLR_NZVC; SET_FLAGS8(A,A,r);
	A=r;
}

/* $49 ROLA inherent -**** */
void mc6800::rola( void )
{
	UINT16 t,r;
	t = A; r = CC&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	A=r;
}

/* $4a DECA inherent -***- */
void mc6800::deca( void )
{
	--A;
	CLR_NZV; SET_FLAGS8D(A);
}

/* $4b ILLEGAL */

/* $4c INCA inherent -***- */
void mc6800::inca( void )
{
	++A;
	CLR_NZV; SET_FLAGS8I(A);
}

/* $4d TSTA inherent -**0- */
void mc6800::tsta( void )
{
	CLR_NZVC; SET_NZ8(A);
}

/* $4e ILLEGAL */

/* $4f CLRA inherent -0100 */
void mc6800::clra( void )
{
	A=0;
	CLR_NZVC; SEZ;
}

/* $50 NEGB inherent ?**** */
void mc6800::negb( void )
{
	UINT16 r;
	r=-B;
	CLR_NZVC; SET_FLAGS8(0,B,r);
	B=r;
}

/* $51 ILLEGAL */

/* $52 ILLEGAL */

/* $53 COMB inherent -**01 */
void mc6800::comb( void )
{
	B = ~B;
	CLR_NZV; SET_NZ8(B); SEC;
}

/* $54 LSRB inherent -0*-* */
void mc6800::lsrb( void )
{
	CLR_NZC; CC|=(B&0x01);
	B>>=1; SET_Z8(B);
}

/* $55 ILLEGAL */

/* $56 RORB inherent -**-* */
void mc6800::rorb( void )
{
	UINT8 r;
	r=(CC&0x01)<<7;
	CLR_NZC; CC|=(B&0x01);
	r |= B>>1; SET_NZ8(r);
	B=r;
}

/* $57 ASRB inherent ?**-* */
void mc6800::asrb( void )
{
	CLR_NZC; CC|=(B&0x01);
	B>>=1; B|=((B&0x40)<<1);
	SET_NZ8(B);
}

/* $58 ASLB inherent ?**** */
void mc6800::aslb( void )
{
	UINT16 r;
	r=B<<1;
	CLR_NZVC; SET_FLAGS8(B,B,r);
	B=r;
}

/* $59 ROLB inherent -**** */
void mc6800::rolb( void )
{
	UINT16 t,r;
	t = B; r = CC&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	B=r;
}

/* $5a DECB inherent -***- */
void mc6800::decb( void )
{
	--B;
	CLR_NZV; SET_FLAGS8D(B);
}

/* $5b ILLEGAL */

/* $5c INCB inherent -***- */
void mc6800::incb( void )
{
	++B;
	CLR_NZV; SET_FLAGS8I(B);
}

/* $5d TSTB inherent -**0- */
void mc6800::tstb( void )
{
	CLR_NZVC; SET_NZ8(B);
}

/* $5e ILLEGAL */

/* $5f CLRB inherent -0100 */
void mc6800::clrb( void )
{
	B=0;
	CLR_NZVC; SEZ;
}

/* $60 NEG indexed ?**** */
void mc6800::neg_ix( void )
{
	UINT16 r,t;
	IDXBYTE(t); r=-t;
	CLR_NZVC; SET_FLAGS8(0,t,r);
	WM(EAD,r);
}

/* $61 AIM --**0- */ /* HD63701YO only */
void mc6800::aim_ix( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	IDXBYTE(r);
	r &= t;
	CLR_NZV; SET_NZ8(r);
	WM(EAD,r);
}

/* $62 OIM --**0- */ /* HD63701YO only */
void mc6800::oim_ix( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	IDXBYTE(r);
	r |= t;
	CLR_NZV; SET_NZ8(r);
	WM(EAD,r);
}

/* $63 COM indexed -**01 */
void mc6800::com_ix( void )
{
	UINT8 t;
	IDXBYTE(t); t = ~t;
	CLR_NZV; SET_NZ8(t); SEC;
	WM(EAD,t);
}

/* $64 LSR indexed -0*-* */
void mc6800::lsr_ix( void )
{
	UINT8 t;
	IDXBYTE(t); CLR_NZC; CC|=(t&0x01);
	t>>=1; SET_Z8(t);
	WM(EAD,t);
}

/* $65 EIM --**0- */ /* HD63701YO only */
void mc6800::eim_ix( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	IDXBYTE(r);
	r ^= t;
	CLR_NZV; SET_NZ8(r);
	WM(EAD,r);
}

/* $66 ROR indexed -**-* */
void mc6800::ror_ix( void )
{
	UINT8 t,r;
	IDXBYTE(t); r=(CC&0x01)<<7;
	CLR_NZC; CC|=(t&0x01);
	r |= t>>1; SET_NZ8(r);
	WM(EAD,r);
}

/* $67 ASR indexed ?**-* */
void mc6800::asr_ix( void )
{
	UINT8 t;
	IDXBYTE(t); CLR_NZC; CC|=(t&0x01);
	t>>=1; t|=((t&0x40)<<1);
	SET_NZ8(t);
	WM(EAD,t);
}

/* $68 ASL indexed ?**** */
void mc6800::asl_ix( void )
{
	UINT16 t,r;
	IDXBYTE(t); r=t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $69 ROL indexed -**** */
void mc6800::rol_ix( void )
{
	UINT16 t,r;
	IDXBYTE(t); r = CC&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $6a DEC indexed -***- */
void mc6800::dec_ix( void )
{
	UINT8 t;
	IDXBYTE(t); --t;
	CLR_NZV; SET_FLAGS8D(t);
	WM(EAD,t);
}

/* $6b TIM --**0- */ /* HD63701YO only */
void mc6800::tim_ix( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	IDXBYTE(r);
	r &= t;
	CLR_NZV; SET_NZ8(r);
}

/* $6c INC indexed -***- */
void mc6800::inc_ix( void )
{
	UINT8 t;
	IDXBYTE(t); ++t;
	CLR_NZV; SET_FLAGS8I(t);
	WM(EAD,t);
}

/* $6d TST indexed -**0- */
void mc6800::tst_ix( void )
{
	UINT8 t;
	IDXBYTE(t); CLR_NZVC; SET_NZ8(t);
}

/* $6e JMP indexed ----- */
void mc6800::jmp_ix( void )
{
	INDEXED; PC=EA; CHANGE_PC();
}

/* $6f CLR indexed -0100 */
void mc6800::clr_ix( void )
{
	INDEXED; WM(EAD,0);
	CLR_NZVC; SEZ;
}

/* $70 NEG extended ?**** */
void mc6800::neg_ex( void )
{
	UINT16 r,t;
	EXTBYTE(t); r=-t;
	CLR_NZVC; SET_FLAGS8(0,t,r);
	WM(EAD,r);
}

/* $71 AIM --**0- */ /* HD63701YO only */
void mc6800::aim_di( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	DIRBYTE(r);
	r &= t;
	CLR_NZV; SET_NZ8(r);
	WM(EAD,r);
}

/* $72 OIM --**0- */ /* HD63701YO only */
void mc6800::oim_di( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	DIRBYTE(r);
	r |= t;
	CLR_NZV; SET_NZ8(r);
	WM(EAD,r);
}

/* $73 COM extended -**01 */
void mc6800::com_ex( void )
{
	UINT8 t;
	EXTBYTE(t); t = ~t;
	CLR_NZV; SET_NZ8(t); SEC;
	WM(EAD,t);
}

/* $74 LSR extended -0*-* */
void mc6800::lsr_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	CLR_NZC;
	CC|=(t&0x01);
	t>>=1;
	SET_Z8(t);
	WM(EAD,t);
}

/* $75 EIM --**0- */ /* HD63701YO only */
void mc6800::eim_di( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	DIRBYTE(r);
	r ^= t;
	CLR_NZV; SET_NZ8(r);
	WM(EAD,r);
}

/* $76 ROR extended -**-* */
void mc6800::ror_ex( void )
{
	UINT8 t,r;
	EXTBYTE(t); r=(CC&0x01)<<7;
	CLR_NZC; CC|=(t&0x01);
	r |= t>>1; SET_NZ8(r);
	WM(EAD,r);
}

/* $77 ASR extended ?**-* */
void mc6800::asr_ex( void )
{
	UINT8 t;
	EXTBYTE(t); CLR_NZC; CC|=(t&0x01);
	t>>=1; t|=((t&0x40)<<1);
	SET_NZ8(t);
	WM(EAD,t);
}

/* $78 ASL extended ?**** */
void mc6800::asl_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r=t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $79 ROL extended -**** */
void mc6800::rol_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r = CC&0x01; r |= t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $7a DEC extended -***- */
void mc6800::dec_ex( void )
{
	UINT8 t;
	EXTBYTE(t); --t;
	CLR_NZV; SET_FLAGS8D(t);
	WM(EAD,t);
}

/* $7b TIM --**0- */ /* HD63701YO only */
void mc6800::tim_di( void )
{
	UINT8 t, r;
	IMMBYTE(t);
	DIRBYTE(r);
	r &= t;
	CLR_NZV; SET_NZ8(r);
}

/* $7c INC extended -***- */
void mc6800::inc_ex( void )
{
	UINT8 t;
	EXTBYTE(t); ++t;
	CLR_NZV; SET_FLAGS8I(t);
	WM(EAD,t);
}

/* $7d TST extended -**0- */
void mc6800::tst_ex( void )
{
	UINT8 t;
	EXTBYTE(t); CLR_NZVC; SET_NZ8(t);
}

/* $7e JMP extended ----- */
void mc6800::jmp_ex( void )
{
	EXTENDED; PC=EA; CHANGE_PC(); /* TS 971002 */
}

/* $7f CLR extended -0100 */
void mc6800::clr_ex( void )
{
    UINT8 t;
    EXTBYTE(t);
    WM(EAD,0);
	CLR_NZVC; SEZ;
}

/* $80 SUBA immediate ?**** */
void mc6800::suba_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $81 CMPA immediate ?**** */
void mc6800::cmpa_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
}

/* $82 SBCA immediate ?**** */
void mc6800::sbca_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t); r = A-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $83 SUBD immediate -**** */
void mc6800::subd_im( void )
{
	UINT32 r,dst;
	PAIR b;
	IMMWORD(b);
	dst = D;
	r = dst - b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D = r;
}

/* $84 ANDA immediate -**0- */
void mc6800::anda_im( void )
{
	UINT8 t;
	IMMBYTE(t); A &= t;
	CLR_NZV; SET_NZ8(A);
}

/* $85 BITA immediate -**0- */
void mc6800::bita_im( void )
{
	UINT8 t,r;
	IMMBYTE(t); r = A&t;
	CLR_NZV; SET_NZ8(r);
}

/* $86 LDA immediate -**0- */
void mc6800::lda_im( void )
{
	IMMBYTE(A);
	CLR_NZV; SET_NZ8(A);
}

/* is this a legal instruction? */
/* $87 STA immediate -**0- */
void mc6800::sta_im( void )
{
	CLR_NZV; SET_NZ8(A);
	IMM8; WM(EAD,A);
}

/* $88 EORA immediate -**0- */
void mc6800::eora_im( void )
{
	UINT8 t;
	IMMBYTE(t); A ^= t;
	CLR_NZV; SET_NZ8(A);
}

/* $89 ADCA immediate ***** */
void mc6800::adca_im( void )
{
	UINT16 t,r;
	IMMBYTE(t); r = A+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $8a ORA immediate -**0- */
void mc6800::ora_im( void )
{
	UINT8 t;
	IMMBYTE(t); A |= t;
	CLR_NZV; SET_NZ8(A);
}

/* $8b ADDA immediate ***** */
void mc6800::adda_im( void )
{
	UINT16 t,r;
	IMMBYTE(t); r = A+t;
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $8c CMPX immediate -***- */
void mc6800::cmpx_im( void )
{
	UINT32 r,dst;
	PAIR b;
	IMMWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZV;
	SET_NZ16(r); SET_V16(dst,b.d,r);
}

/* $8c CPX immediate -**** (6803) */
void mc6800::cpx_im( void )
{
	UINT32 r,dst;
	PAIR b;
	IMMWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZVC; SET_FLAGS16(dst,b.d,r);
}


/* $8d BSR ----- */
void mc6800::bsr( void )
{
	UINT8 t;
	IMMBYTE(t);
	PUSHWORD(pPC);
	PC += SIGNED(t);
	CHANGE_PC();	 /* TS 971002 */
}

/* $8e LDS immediate -**0- */
void mc6800::lds_im( void )
{
	IMMWORD(s);
	CLR_NZV;
	SET_NZ16(S);
}

/* $8f STS immediate -**0- */
void mc6800::sts_im( void )
{
	CLR_NZV;
	SET_NZ16(S);
	IMM16;
	WM16(EAD,&s);
}

/* $90 SUBA direct ?**** */
void mc6800::suba_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $91 CMPA direct ?**** */
void mc6800::cmpa_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
}

/* $92 SBCA direct ?**** */
void mc6800::sbca_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t); r = A-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $93 SUBD direct -**** */
void mc6800::subd_di( void )
{
	UINT32 r,dst;
	PAIR b;
	DIRWORD(b);
	dst = D;
	r = dst - b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D=r;
}

/* $94 ANDA direct -**0- */
void mc6800::anda_di( void )
{
	UINT8 t;
	DIRBYTE(t); A &= t;
	CLR_NZV; SET_NZ8(A);
}

/* $95 BITA direct -**0- */
void mc6800::bita_di( void )
{
	UINT8 t,r;
	DIRBYTE(t); r = A&t;
	CLR_NZV; SET_NZ8(r);
}

/* $96 LDA direct -**0- */
void mc6800::lda_di( void )
{
	DIRBYTE(A);
	CLR_NZV; SET_NZ8(A);
}

/* $97 STA direct -**0- */
void mc6800::sta_di( void )
{
	CLR_NZV; SET_NZ8(A);
	DIRECT; WM(EAD,A);
}

/* $98 EORA direct -**0- */
void mc6800::eora_di( void )
{
	UINT8 t;
	DIRBYTE(t); A ^= t;
	CLR_NZV; SET_NZ8(A);
}

/* $99 ADCA direct ***** */
void mc6800::adca_di( void )
{
	UINT16 t,r;
	DIRBYTE(t); r = A+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $9a ORA direct -**0- */
void mc6800::ora_di( void )
{
	UINT8 t;
	DIRBYTE(t); A |= t;
	CLR_NZV; SET_NZ8(A);
}

/* $9b ADDA direct ***** */
void mc6800::adda_di( void )
{
	UINT16 t,r;
	DIRBYTE(t); r = A+t;
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $9c CMPX direct -***- */
void mc6800::cmpx_di( void )
{
	UINT32 r,dst;
	PAIR b;
	DIRWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZV;
	SET_NZ16(r); SET_V16(dst,b.d,r);
}

/* $9c CPX direct -**** (6803) */
void mc6800::cpx_di( void )
{
	UINT32 r,dst;
	PAIR b;
	DIRWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZVC; SET_FLAGS16(dst,b.d,r);
}

/* $9d JSR direct ----- */
void mc6800::jsr_di( void )
{
	DIRECT;
	PUSHWORD(pPC);
    PC = EA;
	CHANGE_PC();
}

/* $9e LDS direct -**0- */
void mc6800::lds_di( void )
{
	DIRWORD(s);
	CLR_NZV;
	SET_NZ16(S);
}

/* $9f STS direct -**0- */
void mc6800::sts_di( void )
{
	CLR_NZV;
	SET_NZ16(S);
	DIRECT;
	WM16(EAD,&s);
}

/* $a0 SUBA indexed ?**** */
void mc6800::suba_ix( void )
{
	UINT16	  t,r;
	IDXBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $a1 CMPA indexed ?**** */
void mc6800::cmpa_ix( void )
{
	UINT16	  t,r;
	IDXBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
}

/* $a2 SBCA indexed ?**** */
void mc6800::sbca_ix( void )
{
	UINT16	  t,r;
	IDXBYTE(t); r = A-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $a3 SUBD indexed -**** */
void mc6800::subd_ix( void )
{
	UINT32 r,dst;
	PAIR b;
	IDXWORD(b);
	dst = D;
	r = dst - b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D = r;
}

/* $a4 ANDA indexed -**0- */
void mc6800::anda_ix( void )
{
	UINT8 t;
	IDXBYTE(t); A &= t;
	CLR_NZV; SET_NZ8(A);
}

/* $a5 BITA indexed -**0- */
void mc6800::bita_ix( void )
{
	UINT8 t,r;
	IDXBYTE(t); r = A&t;
	CLR_NZV; SET_NZ8(r);
}

/* $a6 LDA indexed -**0- */
void mc6800::lda_ix( void )
{
	IDXBYTE(A);
	CLR_NZV; SET_NZ8(A);
}

/* $a7 STA indexed -**0- */
void mc6800::sta_ix( void )
{
	CLR_NZV; SET_NZ8(A);
	INDEXED; WM(EAD,A);
}

/* $a8 EORA indexed -**0- */
void mc6800::eora_ix( void )
{
	UINT8 t;
	IDXBYTE(t); A ^= t;
	CLR_NZV; SET_NZ8(A);
}

/* $a9 ADCA indexed ***** */
void mc6800::adca_ix( void )
{
	UINT16 t,r;
	IDXBYTE(t); r = A+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $aa ORA indexed -**0- */
void mc6800::ora_ix( void )
{
	UINT8 t;
	IDXBYTE(t); A |= t;
	CLR_NZV; SET_NZ8(A);
}

/* $ab ADDA indexed ***** */
void mc6800::adda_ix( void )
{
	UINT16 t,r;
	IDXBYTE(t); r = A+t;
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $ac CMPX indexed -***- */
void mc6800::cmpx_ix( void )
{
	UINT32 r,dst;
	PAIR b;
	IDXWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZV;
	SET_NZ16(r); SET_V16(dst,b.d,r);
}

/* $ac CPX indexed -**** (6803)*/
void mc6800::cpx_ix( void )
{
	UINT32 r,dst;
	PAIR b;
	IDXWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZVC; SET_FLAGS16(dst,b.d,r);
}

/* $ad JSR indexed ----- */
void mc6800::jsr_ix( void )
{
	INDEXED;
	PUSHWORD(pPC);
    PC = EA;
	CHANGE_PC();
}

/* $ae LDS indexed -**0- */
void mc6800::lds_ix( void )
{
	IDXWORD(s);
	CLR_NZV;
	SET_NZ16(S);
}

/* $af STS indexed -**0- */
void mc6800::sts_ix( void )
{
	CLR_NZV;
	SET_NZ16(S);
	INDEXED;
	WM16(EAD,&s);
}

/* $b0 SUBA extended ?**** */
void mc6800::suba_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $b1 CMPA extended ?**** */
void mc6800::cmpa_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t); r = A-t;
	CLR_NZVC; SET_FLAGS8(A,t,r);
}

/* $b2 SBCA extended ?**** */
void mc6800::sbca_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t); r = A-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(A,t,r);
	A = r;
}

/* $b3 SUBD extended -**** */
void mc6800::subd_ex( void )
{
	UINT32 r,dst;
	PAIR b;
	EXTWORD(b);
	dst = D;
	r = dst - b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D=r;
}

/* $b4 ANDA extended -**0- */
void mc6800::anda_ex( void )
{
	UINT8 t;
	EXTBYTE(t); A &= t;
	CLR_NZV; SET_NZ8(A);
}

/* $b5 BITA extended -**0- */
void mc6800::bita_ex( void )
{
	UINT8 t,r;
	EXTBYTE(t); r = A&t;
	CLR_NZV; SET_NZ8(r);
}

/* $b6 LDA extended -**0- */
void mc6800::lda_ex( void )
{
	EXTBYTE(A);
	CLR_NZV; SET_NZ8(A);
}

/* $b7 STA extended -**0- */
void mc6800::sta_ex( void )
{
	CLR_NZV; SET_NZ8(A);
	EXTENDED; WM(EAD,A);
}

/* $b8 EORA extended -**0- */
void mc6800::eora_ex( void )
{
	UINT8 t;
	EXTBYTE(t); A ^= t;
	CLR_NZV; SET_NZ8(A);
}

/* $b9 ADCA extended ***** */
void mc6800::adca_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r = A+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $ba ORA extended -**0- */
void mc6800::ora_ex( void )
{
	UINT8 t;
	EXTBYTE(t); A |= t;
	CLR_NZV; SET_NZ8(A);
}

/* $bb ADDA extended ***** */
void mc6800::adda_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r = A+t;
	CLR_HNZVC; SET_FLAGS8(A,t,r); SET_H(A,t,r);
	A = r;
}

/* $bc CMPX extended -***- */
void mc6800::cmpx_ex( void )
{
	UINT32 r,dst;
	PAIR b;
	EXTWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZV;
	SET_NZ16(r); SET_V16(dst,b.d,r);
}

/* $bc CPX extended -**** (6803) */
void mc6800::cpx_ex( void )
{
	UINT32 r,dst;
	PAIR b;
	EXTWORD(b);
	dst = X;
	r = dst - b.d;
	CLR_NZVC; SET_FLAGS16(dst,b.d,r);
}

/* $bd JSR extended ----- */
void mc6800::jsr_ex( void )
{
	EXTENDED;
	PUSHWORD(pPC);
    PC = EA;
	CHANGE_PC();
}

/* $be LDS extended -**0- */
void mc6800::lds_ex( void )
{
	EXTWORD(s);
	CLR_NZV;
	SET_NZ16(S);
}

/* $bf STS extended -**0- */
void mc6800::sts_ex( void )
{
	CLR_NZV;
	SET_NZ16(S);
	EXTENDED;
	WM16(EAD,&s);
}

/* $c0 SUBB immediate ?**** */
void mc6800::subb_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $c1 CMPB immediate ?**** */
void mc6800::cmpb_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
}

/* $c2 SBCB immediate ?**** */
void mc6800::sbcb_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t); r = B-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $c3 ADDD immediate -**** */
void mc6800::addd_im( void )
{
	UINT32 r,dst;
	PAIR b;
	IMMWORD(b);
	dst = D;
	r = dst + b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D = r;
}

/* $c4 ANDB immediate -**0- */
void mc6800::andb_im( void )
{
	UINT8 t;
	IMMBYTE(t); B &= t;
	CLR_NZV; SET_NZ8(B);
}

/* $c5 BITB immediate -**0- */
void mc6800::bitb_im( void )
{
	UINT8 t,r;
	IMMBYTE(t); r = B&t;
	CLR_NZV; SET_NZ8(r);
}

/* $c6 LDB immediate -**0- */
void mc6800::ldb_im( void )
{
	IMMBYTE(B);
	CLR_NZV; SET_NZ8(B);
}

/* is this a legal instruction? */
/* $c7 STB immediate -**0- */
void mc6800::stb_im( void )
{
	CLR_NZV; SET_NZ8(B);
	IMM8; WM(EAD,B);
}

/* $c8 EORB immediate -**0- */
void mc6800::eorb_im( void )
{
	UINT8 t;
	IMMBYTE(t); B ^= t;
	CLR_NZV; SET_NZ8(B);
}

/* $c9 ADCB immediate ***** */
void mc6800::adcb_im( void )
{
	UINT16 t,r;
	IMMBYTE(t); r = B+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $ca ORB immediate -**0- */
void mc6800::orb_im( void )
{
	UINT8 t;
	IMMBYTE(t); B |= t;
	CLR_NZV; SET_NZ8(B);
}

/* $cb ADDB immediate ***** */
void mc6800::addb_im( void )
{
	UINT16 t,r;
	IMMBYTE(t); r = B+t;
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $CC LDD immediate -**0- */
void mc6800::ldd_im( void )
{
	IMMWORD(d);
	CLR_NZV;
	SET_NZ16(D);
}

/* is this a legal instruction? */
/* $cd STD immediate -**0- */
void mc6800::std_im( void )
{
	IMM16;
	CLR_NZV;
	SET_NZ16(D);
	WM16(EAD,&d);
}

/* $ce LDX immediate -**0- */
void mc6800::ldx_im( void )
{
	IMMWORD(x);
	CLR_NZV;
	SET_NZ16(X);
}

/* $cf STX immediate -**0- */
void mc6800::stx_im( void )
{
	CLR_NZV;
	SET_NZ16(X);
	IMM16;
	WM16(EAD,&x);
}

/* $d0 SUBB direct ?**** */
void mc6800::subb_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $d1 CMPB direct ?**** */
void mc6800::cmpb_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
}

/* $d2 SBCB direct ?**** */
void mc6800::sbcb_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t); r = B-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $d3 ADDD direct -**** */
void mc6800::addd_di( void )
{
	UINT32 r,dst;
	PAIR b;
	DIRWORD(b);
	dst = D;
	r = dst + b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D = r;
}

/* $d4 ANDB direct -**0- */
void mc6800::andb_di( void )
{
	UINT8 t;
	DIRBYTE(t); B &= t;
	CLR_NZV; SET_NZ8(B);
}

/* $d5 BITB direct -**0- */
void mc6800::bitb_di( void )
{
	UINT8 t,r;
	DIRBYTE(t); r = B&t;
	CLR_NZV; SET_NZ8(r);
}

/* $d6 LDB direct -**0- */
void mc6800::ldb_di( void )
{
	DIRBYTE(B);
	CLR_NZV; SET_NZ8(B);
}

/* $d7 STB direct -**0- */
void mc6800::stb_di( void )
{
	CLR_NZV; SET_NZ8(B);
	DIRECT; WM(EAD,B);
}

/* $d8 EORB direct -**0- */
void mc6800::eorb_di( void )
{
	UINT8 t;
	DIRBYTE(t); B ^= t;
	CLR_NZV; SET_NZ8(B);
}

/* $d9 ADCB direct ***** */
void mc6800::adcb_di( void )
{
	UINT16 t,r;
	DIRBYTE(t); r = B+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $da ORB direct -**0- */
void mc6800::orb_di( void )
{
	UINT8 t;
	DIRBYTE(t); B |= t;
	CLR_NZV; SET_NZ8(B);
}

/* $db ADDB direct ***** */
void mc6800::addb_di( void )
{
	UINT16 t,r;
	DIRBYTE(t); r = B+t;
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $dc LDD direct -**0- */
void mc6800::ldd_di( void )
{
	DIRWORD(d);
	CLR_NZV;
	SET_NZ16(D);
}

/* $dd STD direct -**0- */
void mc6800::std_di( void )
{
	DIRECT;
	CLR_NZV;
	SET_NZ16(D);
	WM16(EAD,&d);
}

/* $de LDX direct -**0- */
void mc6800::ldx_di( void )
{
	DIRWORD(x);
	CLR_NZV;
	SET_NZ16(X);
}

/* $dF STX direct -**0- */
void mc6800::stx_di( void )
{
	CLR_NZV;
	SET_NZ16(X);
	DIRECT;
	WM16(EAD,&x);
}

/* $e0 SUBB indexed ?**** */
void mc6800::subb_ix( void )
{
	UINT16	  t,r;
	IDXBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $e1 CMPB indexed ?**** */
void mc6800::cmpb_ix( void )
{
	UINT16	  t,r;
	IDXBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
}

/* $e2 SBCB indexed ?**** */
void mc6800::sbcb_ix( void )
{
	UINT16	  t,r;
	IDXBYTE(t); r = B-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $e3 ADDD indexed -**** */
void mc6800::addd_ix( void )
{
	UINT32 r,dst;
	PAIR b;
	IDXWORD(b);
	dst = D;
	r = dst + b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D = r;
}

/* $e4 ANDB indexed -**0- */
void mc6800::andb_ix( void )
{
	UINT8 t;
	IDXBYTE(t); B &= t;
	CLR_NZV; SET_NZ8(B);
}

/* $e5 BITB indexed -**0- */
void mc6800::bitb_ix( void )
{
	UINT8 t,r;
	IDXBYTE(t); r = B&t;
	CLR_NZV; SET_NZ8(r);
}

/* $e6 LDB indexed -**0- */
void mc6800::ldb_ix( void )
{
	IDXBYTE(B);
	CLR_NZV; SET_NZ8(B);
}

/* $e7 STB indexed -**0- */
void mc6800::stb_ix( void )
{
	CLR_NZV; SET_NZ8(B);
	INDEXED; WM(EAD,B);
}

/* $e8 EORB indexed -**0- */
void mc6800::eorb_ix( void )
{
	UINT8 t;
	IDXBYTE(t); B ^= t;
	CLR_NZV; SET_NZ8(B);
}

/* $e9 ADCB indexed ***** */
void mc6800::adcb_ix( void )
{
	UINT16 t,r;
	IDXBYTE(t); r = B+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $ea ORB indexed -**0- */
void mc6800::orb_ix( void )
{
	UINT8 t;
	IDXBYTE(t); B |= t;
	CLR_NZV; SET_NZ8(B);
}

/* $eb ADDB indexed ***** */
void mc6800::addb_ix( void )
{
	UINT16 t,r;
	IDXBYTE(t); r = B+t;
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $ec LDD indexed -**0- */
void mc6800::ldd_ix( void )
{
	IDXWORD(d);
	CLR_NZV;
	SET_NZ16(D);
}

/* $ed STD indexed -**0- */
void mc6800::std_ix( void )
{
	INDEXED;
	CLR_NZV;
	SET_NZ16(D);
	WM16(EAD,&d);
}

/* $ee LDX indexed -**0- */
void mc6800::ldx_ix( void )
{
	IDXWORD(x);
	CLR_NZV;
	SET_NZ16(X);
}

/* $ef STX indexed -**0- */
void mc6800::stx_ix( void )
{
	CLR_NZV;
	SET_NZ16(X);
	INDEXED;
	WM16(EAD,&x);
}

/* $f0 SUBB extended ?**** */
void mc6800::subb_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $f1 CMPB extended ?**** */
void mc6800::cmpb_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t); r = B-t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
}

/* $f2 SBCB extended ?**** */
void mc6800::sbcb_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t); r = B-t-(CC&0x01);
	CLR_NZVC; SET_FLAGS8(B,t,r);
	B = r;
}

/* $f3 ADDD extended -**** */
void mc6800::addd_ex( void )
{
	UINT32 r,dst;
	PAIR b;
	EXTWORD(b);
	dst = D;
	r = dst + b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	D = r;
}

/* $f4 ANDB extended -**0- */
void mc6800::andb_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	B &= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $f5 BITB extended -**0- */
void mc6800::bitb_ex( void )
{
	UINT8 t,r;
	EXTBYTE(t);
	r = B & t;
	CLR_NZV;
	SET_NZ8(r);
}

/* $f6 LDB extended -**0- */
void mc6800::ldb_ex( void )
{
	EXTBYTE(B);
	CLR_NZV;
	SET_NZ8(B);
}

/* $f7 STB extended -**0- */
void mc6800::stb_ex( void )
{
	CLR_NZV; SET_NZ8(B);
	EXTENDED; WM(EAD,B);
}

/* $f8 EORB extended -**0- */
void mc6800::eorb_ex( void )
{
	UINT8 t;
	EXTBYTE(t); B ^= t;
	CLR_NZV; SET_NZ8(B);
}

/* $f9 ADCB extended ***** */
void mc6800::adcb_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r = B+t+(CC&0x01);
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $fa ORB extended -**0- */
void mc6800::orb_ex( void )
{
	UINT8 t;
	EXTBYTE(t); B |= t;
	CLR_NZV; SET_NZ8(B);
}

/* $fb ADDB extended ***** */
void mc6800::addb_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r = B+t;
	CLR_HNZVC; SET_FLAGS8(B,t,r); SET_H(B,t,r);
	B = r;
}

/* $fc LDD extended -**0- */
void mc6800::ldd_ex( void )
{
	EXTWORD(d);
	CLR_NZV;
	SET_NZ16(D);
}

/* $fc ADDX extended -****    NSC8105 only.  Flags are a guess */
void mc6800::addx_ex( void )
{
	UINT32 r,dst;
	PAIR b;
	EXTWORD(b);
	dst = X;
	r = dst + b.d;
	CLR_NZVC;
	SET_FLAGS16(dst,b.d,r);
	X = r;
}

/* $fd STD extended -**0- */
void mc6800::std_ex( void )
{
	EXTENDED;
	CLR_NZV;
	SET_NZ16(D);
	WM16(EAD,&d);
}

/* $fe LDX extended -**0- */
void mc6800::ldx_ex( void )
{
	EXTWORD(x);
	CLR_NZV;
	SET_NZ16(X);
}

/* $ff STX extended -**0- */
void mc6800::stx_ex( void )
{
	CLR_NZV;
	SET_NZ16(X);
	EXTENDED;
	WM16(EAD,&x);
}

//
// End of Opcodes
// Now on to utility functions
//

UINT32 mc6800::RM16( UINT32 Addr )
{
	UINT32 result = RM(Addr) << 8;
	return result | RM((Addr+1)&0xffff);
}

void mc6800::WM16( UINT32 Addr, PAIR *p )
{
	WM( Addr, p->b.h );
	WM( (Addr+1)&0xffff, p->b.l );
}

/* IRQ enter */
void mc6800::ENTER_INTERRUPT(UINT16 irq_vector)
{
	if( wai_state & (M6800_WAI|M6800_SLP) )
	{
		if( wai_state & M6800_WAI )
			extra_cycles += 4;
		wai_state &= ~(M6800_WAI|M6800_SLP);
	}
	else
	{
		PUSHWORD(pPC);
		PUSHWORD(pX);
		PUSHBYTE(A);
		PUSHBYTE(B);
		PUSHBYTE(CC);
		extra_cycles += 12;
	}
	SEI;
	PCD = RM16( irq_vector );
	CHANGE_PC();
}

/* check OCI or TOI */
void mc6800::check_timer_event(void)
{
	/* OCI */
	if( CTD >= OCD)
	{
		OCH++;	// next IRQ point
		tcsr |= TCSR_OCF;
		pending_tcsr |= TCSR_OCF;
		MODIFIED_tcsr;
		if ( !(CC & 0x10) && (tcsr & TCSR_EOCI))
			TAKE_OCI;
	}
	/* TOI */
	if( CTD >= TOD)
	{
		TOH++;	// next IRQ point
#if 0
		CLEANUP_conters;
#endif
		tcsr |= TCSR_TOF;
		pending_tcsr |= TCSR_TOF;
		MODIFIED_tcsr;
		if ( !(CC & 0x10) && (tcsr & TCSR_ETOI))
			TAKE_TOI;
	}
	/* set next event */
	SET_TIMRE_EVENT;
}

/****************************************************************************
 * Reset registers to their initial values
 ****************************************************************************/
void mc6800::reset(void)
{
	SEI;				/* IRQ disabled */
	PCD = RM16( 0xfffe );
	CHANGE_PC();

	/* HJB 990417 set CPU subtype (other reset functions override this) */
//	subtype   = SUBTYPE_M6800;
//	m6800.insn = m6800_insn;
	cycles = cycles_6800;

	wai_state = 0;
	nmi_state = 0;
	irq_state[M6800_IRQ_LINE] = 0;
	irq_state[M6800_TIN_LINE] = 0;
	ic_eddge = 0;

	port1_ddr = 0x00;
	port2_ddr = 0x00;
	/* TODO: on reset port 2 should be read to determine the operating mode (bits 0-2) */
	tcsr = 0x00;
	pending_tcsr = 0x00;
	irq2 = 0;
	CTD = 0x0000;
	OCD = 0xffff;
	TOD = 0xffff;
	ram_ctrl |= 0x40;
    m6800_subtype = M6800_TYPE_6800;
}


/****************************************************************************
 * Execute cycles CPU cycles. Return number of cycles really executed
 ****************************************************************************/
int mc6800::execute(void)
{
int cycles = 0;

	UINT8 ireg;
	m6800_ICount = 0;

	CLEANUP_conters;
	INCREMENT_COUNTER(extra_cycles);
	extra_cycles = 0;


//	do
//	{
	if( wai_state & M6800_WAI )
	{
		EAT_CYCLES;
        ireg = 1; // Dummy
		//goto getout;
	} else {
		pPPC = pPC;
		ireg=M_RDOP(PCD);
		PC++;

		switch( ireg )
		{
			case 0x00: illegal(); break;
			case 0x01: nop(); break;
			case 0x02: illegal(); break;
			case 0x03: illegal(); break;
			case 0x04: illegal(); break;
			case 0x05: illegal(); break;
			case 0x06: tap(); break;
			case 0x07: tpa(); break;
			case 0x08: inx(); break;
			case 0x09: dex(); break;
			case 0x0A: CLV; break;
			case 0x0B: SEV; break;
			case 0x0C: CLC; break;
			case 0x0D: SEC; break;
			case 0x0E: cli(); break;
			case 0x0F: sei(); break;
			case 0x10: sba(); break;
			case 0x11: cba(); break;
			case 0x12: illegal(); break;
			case 0x13: illegal(); break;
			case 0x14: illegal(); break;
			case 0x15: illegal(); break;
			case 0x16: tab(); break;
			case 0x17: tba(); break;
			case 0x18: illegal(); break;
			case 0x19: daa(); break;
			case 0x1a: illegal(); break;
			case 0x1b: aba(); break;
			case 0x1c: illegal(); break;
			case 0x1d: illegal(); break;
			case 0x1e: illegal(); break;
			case 0x1f: illegal(); break;
			case 0x20: bra(); break;
			case 0x21: brn(); break;
			case 0x22: bhi(); break;
			case 0x23: bls(); break;
			case 0x24: bcc(); break;
			case 0x25: bcs(); break;
			case 0x26: bne(); break;
			case 0x27: beq(); break;
			case 0x28: bvc(); break;
			case 0x29: bvs(); break;
			case 0x2a: bpl(); break;
			case 0x2b: bmi(); break;
			case 0x2c: bge(); break;
			case 0x2d: blt(); break;
			case 0x2e: bgt(); break;
			case 0x2f: ble(); break;
			case 0x30: tsx(); break;
			case 0x31: ins(); break;
			case 0x32: pula(); break;
			case 0x33: pulb(); break;
			case 0x34: des(); break;
			case 0x35: txs(); break;
			case 0x36: psha(); break;
			case 0x37: pshb(); break;
			case 0x38: illegal(); break;
			case 0x39: rts(); break;
			case 0x3a: illegal(); break;
			case 0x3b: rti(); break;
			case 0x3c: illegal(); break;
			case 0x3d: illegal(); break;
			case 0x3e: wai(); break;
			case 0x3f: swi(); break;
			case 0x40: nega(); break;
			case 0x41: illegal(); break;
			case 0x42: illegal(); break;
			case 0x43: coma(); break;
			case 0x44: lsra(); break;
			case 0x45: illegal(); break;
			case 0x46: rora(); break;
			case 0x47: asra(); break;
			case 0x48: asla(); break;
			case 0x49: rola(); break;
			case 0x4a: deca(); break;
			case 0x4b: illegal(); break;
			case 0x4c: inca(); break;
			case 0x4d: tsta(); break;
			case 0x4e: illegal(); break;
			case 0x4f: clra(); break;
			case 0x50: negb(); break;
			case 0x51: illegal(); break;
			case 0x52: illegal(); break;
			case 0x53: comb(); break;
			case 0x54: lsrb(); break;
			case 0x55: illegal(); break;
			case 0x56: rorb(); break;
			case 0x57: asrb(); break;
			case 0x58: aslb(); break;
			case 0x59: rolb(); break;
			case 0x5a: decb(); break;
			case 0x5b: illegal(); break;
			case 0x5c: incb(); break;
			case 0x5d: tstb(); break;
			case 0x5e: illegal(); break;
			case 0x5f: clrb(); break;
			case 0x60: neg_ix(); break;
			case 0x61: illegal(); break;
			case 0x62: illegal(); break;
			case 0x63: com_ix(); break;
			case 0x64: lsr_ix(); break;
			case 0x65: illegal(); break;
			case 0x66: ror_ix(); break;
			case 0x67: asr_ix(); break;
			case 0x68: asl_ix(); break;
			case 0x69: rol_ix(); break;
			case 0x6a: dec_ix(); break;
			case 0x6b: illegal(); break;
			case 0x6c: inc_ix(); break;
			case 0x6d: tst_ix(); break;
			case 0x6e: jmp_ix(); break;
			case 0x6f: clr_ix(); break;
			case 0x70: neg_ex(); break;
			case 0x71: illegal(); break;
			case 0x72: illegal(); break;
			case 0x73: com_ex(); break;
			case 0x74: lsr_ex(); break;
			case 0x75: illegal(); break;
			case 0x76: ror_ex(); break;
			case 0x77: asr_ex(); break;
			case 0x78: asl_ex(); break;
			case 0x79: rol_ex(); break;
			case 0x7a: dec_ex(); break;
			case 0x7b: illegal(); break;
			case 0x7c: inc_ex(); break;
			case 0x7d: tst_ex(); break;
			case 0x7e: jmp_ex(); break;
			case 0x7f: clr_ex(); break;
			case 0x80: suba_im(); break;
			case 0x81: cmpa_im(); break;
			case 0x82: sbca_im(); break;
			case 0x83: illegal(); break;
			case 0x84: anda_im(); break;
			case 0x85: bita_im(); break;
			case 0x86: lda_im(); break;
			case 0x87: sta_im(); break;
			case 0x88: eora_im(); break;
			case 0x89: adca_im(); break;
			case 0x8a: ora_im(); break;
			case 0x8b: adda_im(); break;
			case 0x8c: cmpx_im(); break;
			case 0x8d: bsr(); break;
			case 0x8e: lds_im(); break;
			case 0x8f: sts_im(); /* orthogonality */ break;
			case 0x90: suba_di(); break;
			case 0x91: cmpa_di(); break;
			case 0x92: sbca_di(); break;
			case 0x93: illegal(); break;
			case 0x94: anda_di(); break;
			case 0x95: bita_di(); break;
			case 0x96: lda_di(); break;
			case 0x97: sta_di(); break;
			case 0x98: eora_di(); break;
			case 0x99: adca_di(); break;
			case 0x9a: ora_di(); break;
			case 0x9b: adda_di(); break;
			case 0x9c: cmpx_di(); break;
			case 0x9d: jsr_di(); break;
			case 0x9e: lds_di(); break;
			case 0x9f: sts_di(); break;
			case 0xa0: suba_ix(); break;
			case 0xa1: cmpa_ix(); break;
			case 0xa2: sbca_ix(); break;
			case 0xa3: illegal(); break;
			case 0xa4: anda_ix(); break;
			case 0xa5: bita_ix(); break;
			case 0xa6: lda_ix(); break;
			case 0xa7: sta_ix(); break;
			case 0xa8: eora_ix(); break;
			case 0xa9: adca_ix(); break;
			case 0xaa: ora_ix(); break;
			case 0xab: adda_ix(); break;
			case 0xac: cmpx_ix(); break;
			case 0xad: jsr_ix(); break;
			case 0xae: lds_ix(); break;
			case 0xaf: sts_ix(); break;
			case 0xb0: suba_ex(); break;
			case 0xb1: cmpa_ex(); break;
			case 0xb2: sbca_ex(); break;
			case 0xb3: illegal(); break;
			case 0xb4: anda_ex(); break;
			case 0xb5: bita_ex(); break;
			case 0xb6: lda_ex(); break;
			case 0xb7: sta_ex(); break;
			case 0xb8: eora_ex(); break;
			case 0xb9: adca_ex(); break;
			case 0xba: ora_ex(); break;
			case 0xbb: adda_ex(); break;
			case 0xbc: cmpx_ex(); break;
			case 0xbd: jsr_ex(); break;
			case 0xbe: lds_ex(); break;
			case 0xbf: sts_ex(); break;
			case 0xc0: subb_im(); break;
			case 0xc1: cmpb_im(); break;
			case 0xc2: sbcb_im(); break;
			case 0xc3: illegal(); break;
			case 0xc4: andb_im(); break;
			case 0xc5: bitb_im(); break;
			case 0xc6: ldb_im(); break;
			case 0xc7: stb_im(); break;
			case 0xc8: eorb_im(); break;
			case 0xc9: adcb_im(); break;
			case 0xca: orb_im(); break;
			case 0xcb: addb_im(); break;
			case 0xcc: illegal(); break;
			case 0xcd: illegal(); break;
			case 0xce: ldx_im(); break;
			case 0xcf: stx_im(); break;
			case 0xd0: subb_di(); break;
			case 0xd1: cmpb_di(); break;
			case 0xd2: sbcb_di(); break;
			case 0xd3: illegal(); break;
			case 0xd4: andb_di(); break;
			case 0xd5: bitb_di(); break;
			case 0xd6: ldb_di(); break;
			case 0xd7: stb_di(); break;
			case 0xd8: eorb_di(); break;
			case 0xd9: adcb_di(); break;
			case 0xda: orb_di(); break;
			case 0xdb: addb_di(); break;
			case 0xdc: illegal(); break;
			case 0xdd: illegal(); break;
			case 0xde: ldx_di(); break;
			case 0xdf: stx_di(); break;
			case 0xe0: subb_ix(); break;
			case 0xe1: cmpb_ix(); break;
			case 0xe2: sbcb_ix(); break;
			case 0xe3: illegal(); break;
			case 0xe4: andb_ix(); break;
			case 0xe5: bitb_ix(); break;
			case 0xe6: ldb_ix(); break;
			case 0xe7: stb_ix(); break;
			case 0xe8: eorb_ix(); break;
			case 0xe9: adcb_ix(); break;
			case 0xea: orb_ix(); break;
			case 0xeb: addb_ix(); break;
			case 0xec: illegal(); break;
			case 0xed: illegal(); break;
			case 0xee: ldx_ix(); break;
			case 0xef: stx_ix(); break;
			case 0xf0: subb_ex(); break;
			case 0xf1: cmpb_ex(); break;
			case 0xf2: sbcb_ex(); break;
			case 0xf3: illegal(); break;
			case 0xf4: andb_ex(); break;
			case 0xf5: bitb_ex(); break;
			case 0xf6: ldb_ex(); break;
			case 0xf7: stb_ex(); break;
			case 0xf8: eorb_ex(); break;
			case 0xf9: adcb_ex(); break;
			case 0xfa: orb_ex(); break;
			case 0xfb: addb_ex(); break;
			case 0xfc: addx_ex(); break;
			case 0xfd: illegal(); break;
			case 0xfe: ldx_ex(); break;
			case 0xff: stx_ex(); break;
		}
		INCREMENT_COUNTER(cycles_6800[ireg]);

  }
//	} while( m6800_ICount>0 );

getout:
	INCREMENT_COUNTER(extra_cycles);
	extra_cycles = 0;

	return m6800_ICount;
}

mc6800::mc6800(void)
{
  m6800_subtype = M6800_TYPE_6800;
}
mc6800::~mc6800(void)
{
}

UINT8 mc6800::read2(UINT16 address)
{
  return read(address);
}

void mc6800::write2(UINT16 address,UINT8 data)
{
  write( address, data);
}

/****************************************************************************
 * Return a specific register
 ****************************************************************************/
unsigned mc6800::m6800_get_reg(int regnum)
{
	switch( regnum )
	{
		case M6800_PC: return pc.w.l;
		case M6800_S: return s.w.l;
		case M6800_CC: return cc;
		case M6800_A: return d.b.h;
		case M6800_B: return d.b.l;
		case M6800_X: return x.w.l;
		case M6800_NMI_STATE: return nmi_state;
		case M6800_IRQ_STATE: return irq_state[M6800_IRQ_LINE];
		case REG_PREVIOUSPC: return ppc.w.l;
		default:
			if( regnum <= REG_SP_CONTENTS )
			{
				unsigned offset = S + 2 * (REG_SP_CONTENTS - regnum);
				if( offset < 0xffff )
					return ( RM( offset ) << 8 ) | RM( offset+1 );
			}
	}
	return 0;
}





/*

READ_HANDLER( m6803_internal_registers_r )
{
	switch (offset)
	{
		case 0x00:
			return m6800.port1_ddr;
		case 0x01:
			return m6800.port2_ddr;
		case 0x02:
			return (cpu_readport16(M6803_PORT1) & (m6800.port1_ddr ^ 0xff))
					| (m6800.port1_data & m6800.port1_ddr);
		case 0x03:
			return (cpu_readport16(M6803_PORT2) & (m6800.port2_ddr ^ 0xff))
					| (m6800.port2_data & m6800.port2_ddr);
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			logerror("CPU #%d PC %04x: warning - read from unsupported internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),offset);
			return 0;
		case 0x08:
			m6800.pending_tcsr = 0;
//logerror("CPU #%d PC %04x: warning - read TCSR register\n",cpu_getactivecpu(),activecpu_get_pc());
			return m6800.tcsr;
		case 0x09:
			if(!(m6800.pending_tcsr&TCSR_TOF))
			{
				m6800.tcsr &= ~TCSR_TOF;
				MODIFIED_tcsr;
			}
			return m6800.counter.b.h;
		case 0x0a:
			return m6800.counter.b.l;
		case 0x0b:
			if(!(m6800.pending_tcsr&TCSR_OCF))
			{
				m6800.tcsr &= ~TCSR_OCF;
				MODIFIED_tcsr;
			}
			return m6800.output_compare.b.h;
		case 0x0c:
			if(!(m6800.pending_tcsr&TCSR_OCF))
			{
				m6800.tcsr &= ~TCSR_OCF;
				MODIFIED_tcsr;
			}
			return m6800.output_compare.b.l;
		case 0x0d:
			if(!(m6800.pending_tcsr&TCSR_ICF))
			{
				m6800.tcsr &= ~TCSR_ICF;
				MODIFIED_tcsr;
			}
			return (m6800.input_capture >> 0) & 0xff;
		case 0x0e:
			return (m6800.input_capture >> 8) & 0xff;
		case 0x0f:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
			logerror("CPU #%d PC %04x: warning - read from unsupported internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),offset);
			return 0;
		case 0x14:
			logerror("CPU #%d PC %04x: read RAM control register\n",cpu_getactivecpu(),activecpu_get_pc());
			return m6800.ram_ctrl;
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1a:
		case 0x1b:
		case 0x1c:
		case 0x1d:
		case 0x1e:
		case 0x1f:
		default:
			logerror("CPU #%d PC %04x: warning - read from reserved internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),offset);
			return 0;
	}
}

WRITE_HANDLER( m6803_internal_registers_w )
{
	static int latch09;

	switch (offset)
	{
		case 0x00:
			if (m6800.port1_ddr != data)
			{
				m6800.port1_ddr = data;
				if(m6800.port1_ddr == 0xff)
					cpu_writeport16(M6803_PORT1,m6800.port1_data);
				else
					cpu_writeport16(M6803_PORT1,(m6800.port1_data & m6800.port1_ddr)
						| (cpu_readport16(M6803_PORT1) & (m6800.port1_ddr ^ 0xff)));
			}
			break;
		case 0x01:
			if (m6800.port2_ddr != data)
			{
				m6800.port2_ddr = data;
				if(m6800.port2_ddr == 0xff)
					cpu_writeport16(M6803_PORT2,m6800.port2_data);
				else
					cpu_writeport16(M6803_PORT2,(m6800.port2_data & m6800.port2_ddr)
						| (cpu_readport16(M6803_PORT2) & (m6800.port2_ddr ^ 0xff)));

				if (m6800.port2_ddr & 2)
					logerror("CPU #%d PC %04x: warning - port 2 bit 1 set as output (OLVL) - not supported\n",cpu_getactivecpu(),activecpu_get_pc());
			}
			break;
		case 0x02:
			m6800.port1_data = data;
			if(m6800.port1_ddr == 0xff)
				cpu_writeport16(M6803_PORT1,m6800.port1_data);
			else
				cpu_writeport16(M6803_PORT1,(m6800.port1_data & m6800.port1_ddr)
					| (cpu_readport16(M6803_PORT1) & (m6800.port1_ddr ^ 0xff)));
			break;
		case 0x03:
			m6800.port2_data = data;
			m6800.port2_ddr = data;
			if(m6800.port2_ddr == 0xff)
				cpu_writeport16(M6803_PORT2,m6800.port2_data);
			else
				cpu_writeport16(M6803_PORT2,(m6800.port2_data & m6800.port2_ddr)
					| (cpu_readport16(M6803_PORT2) & (m6800.port2_ddr ^ 0xff)));
			break;
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
			logerror("CPU #%d PC %04x: warning - write %02x to unsupported internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),data,offset);
			break;
		case 0x08:
			m6800.tcsr = data;
			m6800.pending_tcsr &= m6800.tcsr;
			MODIFIED_tcsr;
			if( !(CC & 0x10) )
				CHECK_IRQ2;
//logerror("CPU #%d PC %04x: TCSR = %02x\n",cpu_getactivecpu(),activecpu_get_pc(),data);
			break;
		case 0x09:
			latch09 = data & 0xff;	// 6301 only
			CT  = 0xfff8;
			TOH = CTH;
			MODIFIED_counters;
			break;
		case 0x0a:	// 6301 only
			CT = (latch09 << 8) | (data & 0xff);
			TOH = CTH;
			MODIFIED_counters;
			break;
		case 0x0b:
			if( m6800.output_compare.b.h != data)
			{
				m6800.output_compare.b.h = data;
				MODIFIED_counters;
			}
			break;
		case 0x0c:
			if( m6800.output_compare.b.l != data)
			{
				m6800.output_compare.b.l = data;
				MODIFIED_counters;
			}
			break;
		case 0x0d:
		case 0x0e:
			logerror("CPU #%d PC %04x: warning - write %02x to read only internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),data,offset);
			break;
		case 0x0f:
		case 0x10:
		case 0x11:
		case 0x12:
		case 0x13:
			logerror("CPU #%d PC %04x: warning - write %02x to unsupported internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),data,offset);
			break;
		case 0x14:
			logerror("CPU #%d PC %04x: write %02x to RAM control register\n",cpu_getactivecpu(),activecpu_get_pc(),data);
			m6800.ram_ctrl = data;
			break;
		case 0x15:
		case 0x16:
		case 0x17:
		case 0x18:
		case 0x19:
		case 0x1a:
		case 0x1b:
		case 0x1c:
		case 0x1d:
		case 0x1e:
		case 0x1f:
		default:
			logerror("CPU #%d PC %04x: warning - write %02x to reserved internal register %02x\n",cpu_getactivecpu(),activecpu_get_pc(),data,offset);
			break;
	}
}

*/
void mc6803::reset(void)
{
	SEI;				/* IRQ disabled */
	PCD = RM16( 0xfffe );
	CHANGE_PC();

	/* HJB 990417 set CPU subtype (other reset functions override this) */
//	subtype   = SUBTYPE_M6800;
//	m6800.insn = m6800_insn;
	cycles = cycles_6303;

	wai_state = 0;
	nmi_state = 0;
	irq_state[M6800_IRQ_LINE] = 0;
	irq_state[M6800_TIN_LINE] = 0;
	ic_eddge = 0;

	port1_ddr = 0x00;
	port2_ddr = 0x00;
	/* TODO: on reset port 2 should be read to determine the operating mode (bits 0-2) */
	tcsr = 0x00;
	pending_tcsr = 0x00;
	irq2 = 0;
	CTD = 0x0000;
	OCD = 0xffff;
	TOD = 0xffff;
	ram_ctrl |= 0x40;
    m6800_subtype = M6800_TYPE_6303;
    p1ddr = 0xfe;
    p2ddr = 0;
//    port1 = ;
//    port2 = ;
    p3ddr = 0xfe;
    p4ddr = 0;
//    port3 = ;
//    port4 = ;
    tcsr1 = 0;
    frch = 0;
    frcl = 0;
    ocr1h = 0xff;
    ocr1l = 0xff;
    icrh = 0;
    icrl = 0;
    tcsr2 = 0x10;
    rmcr = 0;
    trcsr1 = 0x20;
    rdr = 0;
    tdr;
    rp5cr = 0xf8;
//    port5 = ;
    p6ddr = 0;
//    port6 = ;
//    port7 = ;
    ocr2h = 0xff;
    ocr2l = 0xff;
    tcsr3 = 0x00;
    tconr = 0xff;
    t2cnt = 0;
    trcsr2 = 0x28;
//    tstreg = ;
    p5ddr = 0;
    p6csr = 7;
    TxDelay = 0;
}

int mc6803::execute(void)
{
int cycles = 0;

	UINT8 ireg;
	m6800_ICount = 0;

	CLEANUP_conters;
	INCREMENT_COUNTER(extra_cycles);
	extra_cycles = 0;


//	do
//	{
	if( wai_state & M6800_WAI )
	{
//		EAT_CYCLES;
        ireg = 1; // Dummy
		//goto getout;
    } else
    {
		pPPC = pPC;
		ireg=M_RDOP(PCD);
		PC++;

			switch( ireg )
			{
				case 0x00: illegal(); break;
				case 0x01: nop(); break;
				case 0x02: illegal(); break;
				case 0x03: illegal(); break;
				case 0x04: lsrd(); /* 6803 only */; break;
				case 0x05: asld(); /* 6803 only */; break;
				case 0x06: tap(); break;
				case 0x07: tpa(); break;
				case 0x08: inx(); break;
				case 0x09: dex(); break;
				case 0x0A: CLV; break;
				case 0x0B: SEV; break;
				case 0x0C: CLC; break;
				case 0x0D: SEC; break;
				case 0x0E: cli(); break;
				case 0x0F: sei(); break;
				case 0x10: sba(); break;
				case 0x11: cba(); break;
				case 0x12: illegal(); break;
				case 0x13: illegal(); break;
				case 0x14: illegal(); break;
				case 0x15: illegal(); break;
				case 0x16: tab(); break;
				case 0x17: tba(); break;
				case 0x18: xgdx(); break;
				case 0x19: daa(); break;
				case 0x1a: illegal(); break;  // should be slp();
				case 0x1b: aba(); break;
				case 0x1c: illegal(); break;
				case 0x1d: illegal(); break;
				case 0x1e: illegal(); break;
				case 0x1f: illegal(); break;
				case 0x20: bra(); break;
				case 0x21: brn(); break;
				case 0x22: bhi(); break;
				case 0x23: bls(); break;
				case 0x24: bcc(); break;
				case 0x25: bcs(); break;
				case 0x26: bne(); break;
				case 0x27: beq(); break;
				case 0x28: bvc(); break;
				case 0x29: bvs(); break;
				case 0x2a: bpl(); break;
				case 0x2b: bmi(); break;
				case 0x2c: bge(); break;
				case 0x2d: blt(); break;
				case 0x2e: bgt(); break;
				case 0x2f: ble(); break;
				case 0x30: tsx(); break;
				case 0x31: ins(); break;
				case 0x32: pula(); break;
				case 0x33: pulb(); break;
				case 0x34: des(); break;
				case 0x35: txs(); break;
				case 0x36: psha(); break;
				case 0x37: pshb(); break;
				case 0x38: pulx(); /* 6803 only */ break;
				case 0x39: rts(); break;
				case 0x3a: abx(); /* 6803 only */ break;
				case 0x3b: rti(); break;
				case 0x3c: pshx(); /* 6803 only */ break;
				case 0x3d: mul(); /* 6803 only */ break;
				case 0x3e: wai(); break;
				case 0x3f: swi(); break;
				case 0x40: nega(); break;
				case 0x41: illegal(); break;
				case 0x42: illegal(); break;
				case 0x43: coma(); break;
				case 0x44: lsra(); break;
				case 0x45: illegal(); break;
				case 0x46: rora(); break;
				case 0x47: asra(); break;
				case 0x48: asla(); break;
				case 0x49: rola(); break;
				case 0x4a: deca(); break;
				case 0x4b: illegal(); break;
				case 0x4c: inca(); break;
				case 0x4d: tsta(); break;
				case 0x4e: illegal(); break;
				case 0x4f: clra(); break;
				case 0x50: negb(); break;
				case 0x51: illegal(); break;
				case 0x52: illegal(); break;
				case 0x53: comb(); break;
				case 0x54: lsrb(); break;
				case 0x55: illegal(); break;
				case 0x56: rorb(); break;
				case 0x57: asrb(); break;
				case 0x58: aslb(); break;
				case 0x59: rolb(); break;
				case 0x5a: decb(); break;
				case 0x5b: illegal(); break;
				case 0x5c: incb(); break;
				case 0x5d: tstb(); break;
				case 0x5e: illegal(); break;
				case 0x5f: clrb(); break;
				case 0x60: neg_ix(); break;
				case 0x61: aim_ix(); break;
				case 0x62: oim_ix(); break;
				case 0x63: com_ix(); break;
				case 0x64: lsr_ix(); break;
				case 0x65: eim_ix(); break;
				case 0x66: ror_ix(); break;
				case 0x67: asr_ix(); break;
				case 0x68: asl_ix(); break;
				case 0x69: rol_ix(); break;
				case 0x6a: dec_ix(); break;
				case 0x6b: tim_ix(); break;
				case 0x6c: inc_ix(); break;
				case 0x6d: tst_ix(); break;
				case 0x6e: jmp_ix(); break;
				case 0x6f: clr_ix(); break;
				case 0x70: neg_ex(); break;
				case 0x71: aim_di(); break;
				case 0x72: oim_di(); break;
				case 0x73: com_ex(); break;
				case 0x74: lsr_ex(); break;
				case 0x75: eim_di(); break;
				case 0x76: ror_ex(); break;
				case 0x77: asr_ex(); break;
				case 0x78: asl_ex(); break;
				case 0x79: rol_ex(); break;
				case 0x7a: dec_ex(); break;
				case 0x7b: tim_di(); break;
				case 0x7c: inc_ex(); break;
				case 0x7d: tst_ex(); break;
				case 0x7e: jmp_ex(); break;
				case 0x7f: clr_ex(); break;
				case 0x80: suba_im(); break;
				case 0x81: cmpa_im(); break;
				case 0x82: sbca_im(); break;
				case 0x83: subd_im(); /* 6803 only */ break;
				case 0x84: anda_im(); break;
				case 0x85: bita_im(); break;
				case 0x86: lda_im(); break;
				case 0x87: sta_im(); break;
				case 0x88: eora_im(); break;
				case 0x89: adca_im(); break;
				case 0x8a: ora_im(); break;
				case 0x8b: adda_im(); break;
				case 0x8c: cpx_im(); /* 6803 difference */ break;
				case 0x8d: bsr(); break;
				case 0x8e: lds_im(); break;
				case 0x8f: sts_im(); /* orthogonality */ break;
				case 0x90: suba_di(); break;
				case 0x91: cmpa_di(); break;
				case 0x92: sbca_di(); break;
				case 0x93: subd_di(); /* 6803 only */ break;
				case 0x94: anda_di(); break;
				case 0x95: bita_di(); break;
				case 0x96: lda_di(); break;
				case 0x97: sta_di(); break;
				case 0x98: eora_di(); break;
				case 0x99: adca_di(); break;
				case 0x9a: ora_di(); break;
				case 0x9b: adda_di(); break;
				case 0x9c: cpx_di(); /* 6803 difference */ break;
				case 0x9d: jsr_di(); break;
				case 0x9e: lds_di(); break;
				case 0x9f: sts_di(); break;
				case 0xa0: suba_ix(); break;
				case 0xa1: cmpa_ix(); break;
				case 0xa2: sbca_ix(); break;
				case 0xa3: subd_ix(); /* 6803 only */ break;
				case 0xa4: anda_ix(); break;
				case 0xa5: bita_ix(); break;
				case 0xa6: lda_ix(); break;
				case 0xa7: sta_ix(); break;
				case 0xa8: eora_ix(); break;
				case 0xa9: adca_ix(); break;
				case 0xaa: ora_ix(); break;
				case 0xab: adda_ix(); break;
				case 0xac: cpx_ix(); /* 6803 difference */ break;
				case 0xad: jsr_ix(); break;
				case 0xae: lds_ix(); break;
				case 0xaf: sts_ix(); break;
				case 0xb0: suba_ex(); break;
				case 0xb1: cmpa_ex(); break;
				case 0xb2: sbca_ex(); break;
				case 0xb3: subd_ex(); /* 6803 only */ break;
				case 0xb4: anda_ex(); break;
				case 0xb5: bita_ex(); break;
				case 0xb6: lda_ex(); break;
				case 0xb7: sta_ex(); break;
				case 0xb8: eora_ex(); break;
				case 0xb9: adca_ex(); break;
				case 0xba: ora_ex(); break;
				case 0xbb: adda_ex(); break;
				case 0xbc: cpx_ex(); /* 6803 difference */ break;
				case 0xbd: jsr_ex(); break;
				case 0xbe: lds_ex(); break;
				case 0xbf: sts_ex(); break;
				case 0xc0: subb_im(); break;
				case 0xc1: cmpb_im(); break;
				case 0xc2: sbcb_im(); break;
				case 0xc3: addd_im(); /* 6803 only */ break;
				case 0xc4: andb_im(); break;
				case 0xc5: bitb_im(); break;
				case 0xc6: ldb_im(); break;
				case 0xc7: stb_im(); break;
				case 0xc8: eorb_im(); break;
				case 0xc9: adcb_im(); break;
				case 0xca: orb_im(); break;
				case 0xcb: addb_im(); break;
				case 0xcc: ldd_im(); /* 6803 only */ break;
				case 0xcd: std_im(); /* 6803 only -- orthogonality */ break;
				case 0xce: ldx_im(); break;
				case 0xcf: stx_im(); break;
				case 0xd0: subb_di(); break;
				case 0xd1: cmpb_di(); break;
				case 0xd2: sbcb_di(); break;
				case 0xd3: addd_di(); /* 6803 only */ break;
				case 0xd4: andb_di(); break;
				case 0xd5: bitb_di(); break;
				case 0xd6: ldb_di(); break;
				case 0xd7: stb_di(); break;
				case 0xd8: eorb_di(); break;
				case 0xd9: adcb_di(); break;
				case 0xda: orb_di(); break;
				case 0xdb: addb_di(); break;
				case 0xdc: ldd_di(); /* 6803 only */ break;
				case 0xdd: std_di(); /* 6803 only */ break;
				case 0xde: ldx_di(); break;
				case 0xdf: stx_di(); break;
				case 0xe0: subb_ix(); break;
				case 0xe1: cmpb_ix(); break;
				case 0xe2: sbcb_ix(); break;
				case 0xe3: addd_ix(); /* 6803 only */ break;
				case 0xe4: andb_ix(); break;
				case 0xe5: bitb_ix(); break;
				case 0xe6: ldb_ix(); break;
				case 0xe7: stb_ix(); break;
				case 0xe8: eorb_ix(); break;
				case 0xe9: adcb_ix(); break;
				case 0xea: orb_ix(); break;
				case 0xeb: addb_ix(); break;
				case 0xec: ldd_ix(); /* 6803 only */ break;
				case 0xed: std_ix(); /* 6803 only */ break;
				case 0xee: ldx_ix(); break;
				case 0xef: stx_ix(); break;
				case 0xf0: subb_ex(); break;
				case 0xf1: cmpb_ex(); break;
				case 0xf2: sbcb_ex(); break;
				case 0xf3: addd_ex(); /* 6803 only */ break;
				case 0xf4: andb_ex(); break;
				case 0xf5: bitb_ex(); break;
				case 0xf6: ldb_ex(); break;
				case 0xf7: stb_ex(); break;
				case 0xf8: eorb_ex(); break;
				case 0xf9: adcb_ex(); break;
				case 0xfa: orb_ex(); break;
				case 0xfb: addb_ex(); break;
				case 0xfc: ldd_ex(); /* 6803 only */ break;
				case 0xfd: std_ex(); /* 6803 only */ break;
				case 0xfe: ldx_ex(); break;
				case 0xff: stx_ex(); break;
			}
			INCREMENT_COUNTER(cycles_6303[ireg]);
    }
//	} while( m6800_ICount>0 );

getout:
	INCREMENT_COUNTER(extra_cycles);
    doserial(m6800_ICount);
	extra_cycles = 0;

    return m6800_ICount;
}

UINT8 mc6803::read2(UINT16 address)
{
UINT8 value;

  if ( address < 0x28 ) {
    switch ( address ) {
      case 0:
      case 2:
      case 4:
      case 5:
      case 6:
      case 7:
      case 0x18:
        value = read( address );
        break;
      case 3:
        value = readport(2);
        break;
      case 8:
        value = tcsr1;
        break;
      case 9:
        value = frch;
        break;
      case 0xa:
        value = frcl;
        break;
      case 0xb:
        value = ocr1h;
        break;
      case 0xc:
        value = ocr1l;
        break;
      case 0xd:
        value = icrh;
        break;
      case 0xe:
        value = icrl;
        break;
      case 0xf:
        value = tcsr2;
        break;
		case 0x10:
        value = rmcr;
        break;
		case 0x11:
        value = trcsr1;
        break;
		case 0x12:
        value = rdr;
        break;
		case 0x14:
        value = rp5cr;
        break;
		case 0x15:
        value = readport(5);
        break;
		case 0x17:
        value = readport(6);
        break;
		case 0x19:
        value = ocr2h;
        break;
		case 0x1a:
        value = ocr2l;
        break;
		case 0x1b:
        value = tcsr3;
        break;
		case 0x1c:
        value = tconr;
        break;
		case 0x1d:
        value = t2cnt;
        break;
		case 0x1e:
        value = trcsr2;
        break;
      case 0x21:
        value = p6csr;
        break;
      case 0x22:
      case 0x23:
      case 0x24:
      case 0x25:
      case 0x26:
      case 0x27:
        break;
	}
    value = readinternal(address, value);
  } else if ( address >= 0x40 && address <= 0x13f && (rp5cr & 0x40) )
      value = ram[address-0x40];
    else
      value = read( address );

  return value;
}

void mc6803::write2(UINT16 address,UINT8 data)
{
  if ( address < 0x28 ) {
    switch ( address ) {
      case 0:
      case 2:
      case 4:
      case 5:
      case 6:
      case 7:
      case 0x18:
        write(address,data);
        return;
			break;
      case 1:
        p2ddr = data;
			break;
      case 3:
        writeport( 2, data, data ^ port2);
        port2 = data;
			break;
      case 8:
        tcsr1 = (tcsr1 & 0xe0) | ( data & 0x1f);
			break;
      case 9:
        tmpfrch = data;
        frch = 0xff;
        frcl = 0xf8;
        tcsr1 &= ~0x20; // TOF
			break;
      case 0xa:
        frch = tmpfrch;
        frcl = data;
			break;
      case 0xb:
        ocr1h = data;
        tcsr1 &= ~0x40; // OCF1
        tcsr2 &= ~0x40; // OCF1
			break;
      case 0xc:
        ocr1l = data;
        tcsr1 &= ~0x40; // OCF1
        tcsr2 &= ~0x40; // OCF1
			break;
      case 0xf:
        tcsr2 = (tcsr2 & 0xf0) | ( data & 0xf);
			break;
      case 0x10:
        rmcr = data;
			break;
      case 0x11:
        trcsr1 = (trcsr1 & 0xe0 ) | ( data & 0x1f);;
			break;
		case 0x13:
        tdr = data;
        trcsr1 &= ~0x20;
        TxDelay = 2000;
        tdsr = tdr;
			break;
		case 0x14:
        rp5cr = data;
			break;
		case 0x15:
        writeport( 5, data, data ^ port5);
        port5 = data;
        break;
		case 0x16:
        p6ddr = data;
        break;
		case 0x17:
        writeport( 6, data, data ^ port6);
        port6 = data;
        break;
		case 0x19:
        ocr2h = data;
        tcsr2 &= ~0x20; // OCF2
        break;
		case 0x1a:
        ocr2l = data;
        tcsr2 &= ~0x20; // OCF2
        break;
		case 0x1b:
        tcsr3 = (tcsr3 & 0x80 ) | ( data & 0x7f);
        break;
		case 0x1c:
        tconr = data;
        break;
		case 0x1d:
        t2cnt = data;
        break;
		case 0x1e:
        trcsr2 = (trcsr2 & 0xe0 ) | ( data & 0x1f);
        break;
      case 0x20:
        p5ddr = data;
        break;
      case 0x21:
        p6csr = data;
        break;
      case 0x22:
      case 0x23:
      case 0x24:
      case 0x25:
      case 0x26:
      case 0x27:
			break;
	}
    writeinternal( address, data);
  } else if ( address >= 0x40 && address <= 0x13f && (rp5cr & 0x40) )
      ram[address-0x40] = data;
    else
      write(address,data);
}

void mc6803::writeport(UINT8 port,UINT8 data,UINT8 changed)
{
}

void mc6803::writeinternal(UINT8 port,UINT8 data)
{
}

UINT8 mc6803::readport(UINT8 port)
{
}

UINT8 mc6803::readinternal(UINT8 addr, UINT8 value)
{
  return value;
}

void mc6803::doserial(int clks)
{
  if ( TxDelay ) {
    TxDelay -= clks;
    if ( TxDelay <= 0 ) {
      TxDelay = 0;
      trcsr1 |= 0x20;
    }
  }
  while ( clks-- ) {
    frcl++;
    if ( frcl == 0 ) {
      frch++;
      if ( frch == 0 )
        tcsr1 |= 0x20; // TOF
    }
    if ( frcl == ocr1l && frch == ocr1h ) {
      tcsr1 |= 0x40; // OCF1
      tcsr2 |= 0x40; // OCF1
    }
    if ( frcl == ocr2l && frch == ocr2h )
      tcsr2 |= 0x20; // OCF2
    if ( tcsr3 & 0x10 ) { // Timer 2 enable
      t2cnt++;
      if ( t2cnt == tconr ) {
        t2cnt = 0;
        tcsr3 |= 0x80; // CMF
      }
    }
  }
}




