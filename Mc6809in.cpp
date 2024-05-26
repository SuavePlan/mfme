//
//	mc6809in.cpp
//

#include "typedefs.h"
#include "mc6809.h"

 /* flag bits in the cc register */
#define CC_C    0x01        /* Carry */
#define CC_V    0x02        /* Overflow */
#define CC_Z    0x04        /* Zero */
#define CC_N    0x08        /* Negative */
#define CC_II   0x10        /* Inhibit IRQ */
#define CC_H    0x20        /* Half (auxiliary) carry */
#define CC_IF   0x40        /* Inhibit FIRQ */
#define CC_E    0x80        /* entire state pushed */


#define pPPC    ppc
#define pPC 	pc
#define pU		u
#define pS		s
#define pX		x
#define pY		y
#define pD		dd

#define	PPC		ppc.w.l
#define PC  	pc.w.l
#define PCD 	pc.d
#define U		u.w.l
#define UD		u.d
#define S		s.w.l
#define SD		s.d
#define X		x.w.l
#define XD		x.d
#define Y		y.w.l
#define YD		y.d
#define D   	dd.w.l
#define A   	dd.b.h
#define B		dd.b.l
#define DP		dp.b.h
#define DPD 	dp.d
#define CC  	cc

#define EA	ea.w.l
#define EAD ea.d

#define CHANGE_PC


//#define CLEAR_LINE		0		/* clear (a fired, held or pulsed) line */
//#define ASSERT_LINE     1       /* assert an interrupt immediately */
//#define HOLD_LINE       2       /* hold interrupt line until enable is true */
//#define PULSE_LINE		3		/* pulse interrupt line for one instruction */


/* these are re-defined in m6809.h TO RAM, ROM or functions in cpuintrf.c */
#define RM(Addr)		read(Addr)
#define WM(Addr,Value)	write(Addr,Value)
#define ROP(Addr)		read(Addr)
#define ROP_ARG(Addr)	read(Addr)

/* macros to access memory */
#define IMMBYTE(b)	b = ROP_ARG(PCD); PC++
#define IMMWORD(w)	w.d = (ROP_ARG(PCD)<<8) | ROP_ARG((PCD+1)&0xffff); PC+=2

#define PUSHBYTE(b) --S; WM(SD,b)
#define PUSHWORD(w) --S; WM(SD,w.b.l); --S; WM(SD,w.b.h)
#define PULLBYTE(b) b = RM(SD); S++
#define PULLWORD(w) w = RM(SD)<<8; S++; w |= RM(SD); S++

#define PSHUBYTE(b) --U; WM(UD,b);
#define PSHUWORD(w) --U; WM(UD,w.b.l); --U; WM(UD,w.b.h)
#define PULUBYTE(b) b = RM(UD); U++
#define PULUWORD(w) w = RM(UD)<<8; U++; w |= RM(UD); U++

#define CLR_HNZVC   CC&=~(CC_H|CC_N|CC_Z|CC_V|CC_C)
#define CLR_NZV 	CC&=~(CC_N|CC_Z|CC_V)
#define CLR_HNZC	CC&=~(CC_H|CC_N|CC_Z|CC_C)
#define CLR_NZVC	CC&=~(CC_N|CC_Z|CC_V|CC_C)
#define CLR_Z		CC&=~(CC_Z)
#define CLR_NZC 	CC&=~(CC_N|CC_Z|CC_C)
#define CLR_ZC		CC&=~(CC_Z|CC_C)


#define SET_FLAGS8I(a)		{CC|=flags8i[(a)&0xff];}
#define SET_FLAGS8D(a)		{CC|=flags8d[(a)&0xff];}


/* macros for CC -- CC bits affected should be reset before calling */
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

/* combos */
#define SET_NZ8(a)			{SET_N8(a);SET_Z(a);}
#define SET_NZ16(a)			{SET_N16(a);SET_Z(a);}
#define SET_FLAGS8(a,b,r)	{SET_N8(r);SET_Z8(r);SET_V8(a,b,r);SET_C8(r);}
#define SET_FLAGS16(a,b,r)	{SET_N16(r);SET_Z16(r);SET_V16(a,b,r);SET_C16(r);}

/* for treating an unsigned byte as a signed word */
#define SIGNED(b) ((UINT16)(b&0x80?b|0xff00:b))

/* macros for addressing modes (postbytes have their own code) */
#define DIRECT	EAD = DPD; IMMBYTE(ea.b.l)
#define IMM8	EAD = PCD; PC++
#define IMM16	EAD = PCD; PC+=2
#define EXTENDED IMMWORD(ea)

/* macros to set status flags */
#define SEC CC|=CC_C
#define CLC CC&=~CC_C
#define SEZ CC|=CC_Z
#define CLZ CC&=~CC_Z
#define SEN CC|=CC_N
#define CLN CC&=~CC_N
#define SEV CC|=CC_V
#define CLV CC&=~CC_V
#define SEH CC|=CC_H
#define CLH CC&=~CC_H

/* macros for convenience */
#define DIRBYTE(b) {DIRECT;b=RM(EAD);}
#define DIRWORD(w) {DIRECT;w.d=RM16(EAD);}
#define EXTBYTE(b) {EXTENDED;b=RM(EAD);}
#define EXTWORD(w) {EXTENDED;w.d=RM16(EAD);}

/* macros for branch instructions */
#define BRANCH(f) { 					\
	UINT8 t;							\
	IMMBYTE(t); 						\
	if( f ) 							\
	{									\
		PC += SIGNED(t);				\
		CHANGE_PC;						\
	}									\
}

#define LBRANCH(f) {                    \
	PAIR t; 							\
	IMMWORD(t); 						\
	if( f ) 							\
	{									\
		cycles += 1;				\
		PC += t.w.l;					\
		CHANGE_PC;						\
	}									\
}

#define NXORV  ((CC&CC_N)^((CC&CC_V)<<2))

static UINT8 flags8i[256]=	 /* increment */
{
CC_Z,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
CC_N|CC_V,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N
};
static UINT8 flags8d[256]= /* decrement */
{
CC_Z,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,CC_V,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,
CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N,CC_N
};

/*

HNZVC

? = undefined
* = affected
- = unaffected
0 = cleared
1 = set
# = CCr directly affected by instruction
@ = special - carry set if bit 7 is set

*/

void mc6809::illegal(void)
{
}

/* $00 NEG direct ?**** */
void mc6809::neg_di( void )
{
	UINT16 r,t;
	DIRBYTE(t);
	r = -t;
	CLR_NZVC;
	SET_FLAGS8(0,t,r);
	WM(EAD,r);
}

/* $01 ILLEGAL */

/* $02 ILLEGAL */

/* $03 COM direct -**01 */
void mc6809::com_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	t = ~t;
	CLR_NZV;
	SET_NZ8(t);
	SEC;
	WM(EAD,t);
}

/* $04 LSR direct -0*-* */
void mc6809::lsr_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	CLR_NZC;
	CC |= (t & CC_C);
	t >>= 1;
	SET_Z8(t);
	WM(EAD,t);
}

/* $05 ILLEGAL */

/* $06 ROR direct -**-* */
void mc6809::ror_di( void )
{
	UINT8 t,r;
	DIRBYTE(t);
	r= (CC & CC_C) << 7;
	CLR_NZC;
	CC |= (t & CC_C);
	r |= t>>1;
	SET_NZ8(r);
	WM(EAD,r);
}

/* $07 ASR direct ?**-* */
void mc6809::asr_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	CLR_NZC;
	CC |= (t & CC_C);
	t = (t & 0x80) | (t >> 1);
	SET_NZ8(t);
	WM(EAD,t);
}

/* $08 ASL direct ?**** */
void mc6809::asl_di( void )
{
	UINT16 t,r;
	DIRBYTE(t);
	r = t << 1;
	CLR_NZVC;
	SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $09 ROL direct -**** */
void mc6809::rol_di( void )
{
	UINT16 t,r;
	DIRBYTE(t);
	r = (CC & CC_C) | (t << 1);
	CLR_NZVC;
	SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $0A DEC direct -***- */
void mc6809::dec_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	--t;
	CLR_NZV;
	SET_FLAGS8D(t);
	WM(EAD,t);
}

/* $0B ILLEGAL */

/* $OC INC direct -***- */
void mc6809::inc_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	++t;
	CLR_NZV;
	SET_FLAGS8I(t);
	WM(EAD,t);
}

/* $OD TST direct -**0- */
void mc6809::tst_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	CLR_NZV;
	SET_NZ8(t);
}

/* $0E JMP direct ----- */
void mc6809::jmp_di( void )
{
	DIRECT;
	PCD = EAD;
	CHANGE_PC;
}

/* $0F CLR direct -0100 */
void mc6809::clr_di( void )
{
	DIRECT;
	WM(EAD,0);
	CLR_NZVC;
	SEZ;
}

/* $10 FLAG */

/* $11 FLAG */

/* $12 NOP inherent ----- */
void mc6809::nop( void )
{
	;
}

/* $13 SYNC inherent ----- */
void mc6809::sync( void )
{
	/* SYNC stops processing instructions until an interrupt request happens. */
	/* This doesn't require the corresponding interrupt to be enabled: if it */
	/* is disabled, execution continues with the next instruction. */
	int_state |= M6809_SYNC;	 /* HJB 990227 */
	CHECK_IRQ_LINES;
	/* if mc6809_SYNC has not been cleared by CHECK_IRQ_LINES,
	 * stop execution until the interrupt lines change. */
//	if( int_state & M6809_SYNC )
//		if (m6809_ICount > 0) m6809_ICount = 0;
}

/* $14 ILLEGAL */

/* $15 ILLEGAL */

/* $16 LBRA relative ----- */
void mc6809::lbra( void )
{
	IMMWORD(ea);
	PC += EA;
	CHANGE_PC;

//	if ( EA == 0xfffd )  /* EHC 980508 speed up busy loop */
//		if ( m6809_ICount > 0)
//			m6809_ICount = 0;
}

/* $17 LBSR relative ----- */
void mc6809::lbsr( void )
{
	IMMWORD(ea);
	PUSHWORD(pPC);
	PC += EA;
	CHANGE_PC;
}

/* $18 ILLEGAL */

#if 1
/* $19 DAA inherent (A) -**0* */
void mc6809::daa( void )
{
	UINT8 msn, lsn;
	UINT16 t, cf = 0;
	msn = A & 0xf0; lsn = A & 0x0f;
	if( lsn>0x09 || CC & CC_H) cf |= 0x06;
	if( msn>0x80 && lsn>0x09 ) cf |= 0x60;
	if( msn>0x90 || CC & CC_C) cf |= 0x60;
	t = cf + A;
	CLR_NZV; /* keep carry from previous operation */
	SET_NZ8((UINT8)t); SET_C8(t);
	A = t;
}
#else
/* $19 DAA inherent (A) -**0* */
void mc6809::daa( void )
{
	UINT16 t;
	t = A;
	if (CC & CC_H) t+=0x06;
	if ((t&0x0f)>9) t+=0x06;		/* ASG -- this code is broken! $66+$99=$FF -> DAA should = $65, we get $05! */
	if (CC & CC_C) t+=0x60;
	if ((t&0xf0)>0x90) t+=0x60;
	if (t&0x100) SEC;
	A = t;
}
#endif

/* $1A ORCC immediate ##### */
void mc6809::orcc( void )
{
	UINT8 t;
	IMMBYTE(t);
	CC |= t;
	CHECK_IRQ_LINES;	/* HJB 990116 */
}

/* $1B ILLEGAL */

/* $1C ANDCC immediate ##### */
void mc6809::andcc( void )
{
	UINT8 t;
	IMMBYTE(t);
	CC &= t;
	CHECK_IRQ_LINES;	/* HJB 990116 */
}

/* $1D SEX inherent -**0- */
void mc6809::sex( void )
{
	UINT16 t;
	t = SIGNED(B);
	D = t;
	CLR_NZV;
	SET_NZ16(t);
}

/* $1E EXG inherent ----- */
void mc6809::exg( void )
{
	UINT16 t1,t2;
	UINT8 tb;

	IMMBYTE(tb);
	if( (tb^(tb>>4)) & 0x08 )	/* HJB 990225: mixed 8/16 bit case? */
	{
		/* transfer $ff to both registers */
		t1 = t2 = 0xff;
	}
	else
	{
		switch(tb>>4) {
			case  0: t1 = D;  break;
			case  1: t1 = X;  break;
			case  2: t1 = Y;  break;
			case  3: t1 = U;  break;
			case  4: t1 = S;  break;
			case  5: t1 = PC; break;
			case  8: t1 = A;  break;
			case  9: t1 = B;  break;
			case 10: t1 = CC; break;
			case 11: t1 = DP; break;
			default: t1 = 0xff;
		}
		switch(tb&15) {
			case  0: t2 = D;  break;
			case  1: t2 = X;  break;
			case  2: t2 = Y;  break;
			case  3: t2 = U;  break;
			case  4: t2 = S;  break;
			case  5: t2 = PC; break;
			case  8: t2 = A;  break;
			case  9: t2 = B;  break;
			case 10: t2 = CC; break;
			case 11: t2 = DP; break;
			default: t2 = 0xff;
        }
	}
	switch(tb>>4) {
		case  0: D = t2;  break;
		case  1: X = t2;  break;
		case  2: Y = t2;  break;
		case  3: U = t2;  break;
		case  4: S = t2;  break;
		case  5: PC = t2; CHANGE_PC; break;
		case  8: A = t2;  break;
		case  9: B = t2;  break;
		case 10: CC = t2; break;
		case 11: DP = t2; break;
	}
	switch(tb&15) {
		case  0: D = t1;  break;
		case  1: X = t1;  break;
		case  2: Y = t1;  break;
		case  3: U = t1;  break;
		case  4: S = t1;  break;
		case  5: PC = t1; CHANGE_PC; break;
		case  8: A = t1;  break;
		case  9: B = t1;  break;
		case 10: CC = t1; break;
		case 11: DP = t1; break;
	}
}

/* $1F TFR inherent ----- */
void mc6809::tfr( void )
{
	UINT8 tb;
	UINT16 t;

	IMMBYTE(tb);
	if( (tb^(tb>>4)) & 0x08 )	/* HJB 990225: mixed 8/16 bit case? */
	{
		/* transfer $ff to register */
		t = 0xff;
    }
	else
	{
		switch(tb>>4) {
			case  0: t = D;  break;
			case  1: t = X;  break;
			case  2: t = Y;  break;
			case  3: t = U;  break;
			case  4: t = S;  break;
			case  5: t = PC; break;
			case  8: t = A;  break;
			case  9: t = B;  break;
			case 10: t = CC; break;
			case 11: t = DP; break;
			default: t = 0xff;
        }
	}
	switch(tb&15) {
		case  0: D = t;  break;
		case  1: X = t;  break;
		case  2: Y = t;  break;
		case  3: U = t;  break;
		case  4: S = t;  break;
		case  5: PC = t; CHANGE_PC; break;
		case  8: A = t;  break;
		case  9: B = t;  break;
		case 10: CC = t; break;
		case 11: DP = t; break;
    }
}

#if macintosh
#pragma mark ____2x____
#endif

/* $20 BRA relative ----- */
void mc6809::bra( void )
{
	UINT8 t;
	IMMBYTE(t);
	PC += SIGNED(t);
    CHANGE_PC;
	/* JB 970823 - speed up busy loops */
//	if( t == 0xfe )
//		if( m6809_ICount > 0 ) m6809_ICount = 0;
}

/* $21 BRN relative ----- */
void mc6809::brn( void )
{
	UINT8 t;
	IMMBYTE(t);
}

/* $1021 LBRN relative ----- */
void mc6809::lbrn( void )
{
	IMMWORD(ea);
}

/* $22 BHI relative ----- */
void mc6809::bhi( void )
{
	BRANCH( !(CC & (CC_Z|CC_C)) );
}

/* $1022 LBHI relative ----- */
void mc6809::lbhi( void )
{
	LBRANCH( !(CC & (CC_Z|CC_C)) );
}

/* $23 BLS relative ----- */
void mc6809::bls( void )
{
	BRANCH( (CC & (CC_Z|CC_C)) );
}

/* $1023 LBLS relative ----- */
void mc6809::lbls( void )
{
	LBRANCH( (CC&(CC_Z|CC_C)) );
}

/* $24 BCC relative ----- */
void mc6809::bcc( void )
{
	BRANCH( !(CC&CC_C) );
}

/* $1024 LBCC relative ----- */
void mc6809::lbcc( void )
{
	LBRANCH( !(CC&CC_C) );
}

/* $25 BCS relative ----- */
void mc6809::bcs( void )
{
	BRANCH( (CC&CC_C) );
}

/* $1025 LBCS relative ----- */
void mc6809::lbcs( void )
{
	LBRANCH( (CC&CC_C) );
}

/* $26 BNE relative ----- */
void mc6809::bne( void )
{
	BRANCH( !(CC&CC_Z) );
}

/* $1026 LBNE relative ----- */
void mc6809::lbne( void )
{
	LBRANCH( !(CC&CC_Z) );
}

/* $27 BEQ relative ----- */
void mc6809::beq( void )
{
	BRANCH( (CC&CC_Z) );
}

/* $1027 LBEQ relative ----- */
void mc6809::lbeq( void )
{
	LBRANCH( (CC&CC_Z) );
}

/* $28 BVC relative ----- */
void mc6809::bvc( void )
{
	BRANCH( !(CC&CC_V) );
}

/* $1028 LBVC relative ----- */
void mc6809::lbvc( void )
{
	LBRANCH( !(CC&CC_V) );
}

/* $29 BVS relative ----- */
void mc6809::bvs( void )
{
	BRANCH( (CC&CC_V) );
}

/* $1029 LBVS relative ----- */
void mc6809::lbvs( void )
{
	LBRANCH( (CC&CC_V) );
}

/* $2A BPL relative ----- */
void mc6809::bpl( void )
{
	BRANCH( !(CC&CC_N) );
}

/* $102A LBPL relative ----- */
void mc6809::lbpl( void )
{
	LBRANCH( !(CC&CC_N) );
}

/* $2B BMI relative ----- */
void mc6809::bmi( void )
{
	BRANCH( (CC&CC_N) );
}

/* $102B LBMI relative ----- */
void mc6809::lbmi( void )
{
	LBRANCH( (CC&CC_N) );
}

/* $2C BGE relative ----- */
void mc6809::bge( void )
{
	BRANCH( !NXORV );
}

/* $102C LBGE relative ----- */
void mc6809::lbge( void )
{
	LBRANCH( !NXORV );
}

/* $2D BLT relative ----- */
void mc6809::blt( void )
{
	BRANCH( NXORV );
}

/* $102D LBLT relative ----- */
void mc6809::lblt( void )
{
	LBRANCH( NXORV );
}

/* $2E BGT relative ----- */
void mc6809::bgt( void )
{
	BRANCH( !(NXORV || (CC&CC_Z)) );
}

/* $102E LBGT relative ----- */
void mc6809::lbgt( void )
{
	LBRANCH( !(NXORV || (CC&CC_Z)) );
}

/* $2F BLE relative ----- */
void mc6809::ble( void )
{
	BRANCH( (NXORV || (CC&CC_Z)) );
}

/* $102F LBLE relative ----- */
void mc6809::lble( void )
{
	LBRANCH( (NXORV || (CC&CC_Z)) );
}

#if macintosh
#pragma mark ____3x____
#endif

/* $30 LEAX indexed --*-- */
void mc6809::leax( void )
{
	fetch_effective_address();
    X = EA;
	CLR_Z;
	SET_Z(X);
}

/* $31 LEAY indexed --*-- */
void mc6809::leay( void )
{
	fetch_effective_address();
    Y = EA;
	CLR_Z;
	SET_Z(Y);
}

/* $32 LEAS indexed ----- */
void mc6809::leas( void )
{
	fetch_effective_address();
    S = EA;
	int_state |= M6809_LDS;
}

/* $33 LEAU indexed ----- */
void mc6809::leau( void )
{
	fetch_effective_address();
    U = EA;
}

/* $34 PSHS inherent ----- */
void mc6809::pshs( void )
{
	UINT8 t;
	IMMBYTE(t);
	if( t&0x80 ) { PUSHWORD(pPC); cycles += 2; }
	if( t&0x40 ) { PUSHWORD(pU);  cycles += 2; }
	if( t&0x20 ) { PUSHWORD(pY);  cycles += 2; }
	if( t&0x10 ) { PUSHWORD(pX);  cycles += 2; }
	if( t&0x08 ) { PUSHBYTE(DP);  cycles += 1; }
	if( t&0x04 ) { PUSHBYTE(B);   cycles += 1; }
	if( t&0x02 ) { PUSHBYTE(A);   cycles += 1; }
	if( t&0x01 ) { PUSHBYTE(CC);  cycles += 1; }
}

/* 35 PULS inherent ----- */
void mc6809::puls( void )
{
	UINT8 t;
	IMMBYTE(t);
	if( t&0x01 ) { PULLBYTE(CC); cycles += 1; }
	if( t&0x02 ) { PULLBYTE(A);  cycles += 1; }
	if( t&0x04 ) { PULLBYTE(B);  cycles += 1; }
	if( t&0x08 ) { PULLBYTE(DP); cycles += 1; }
	if( t&0x10 ) { PULLWORD(XD); cycles += 2; }
	if( t&0x20 ) { PULLWORD(YD); cycles += 2; }
	if( t&0x40 ) { PULLWORD(UD); cycles += 2; }
	if( t&0x80 ) { PULLWORD(PCD); CHANGE_PC; cycles += 2; }

	/* HJB 990225: moved check after all PULLs */
	if( t&0x01 ) { CHECK_IRQ_LINES; }
}

/* $36 PSHU inherent ----- */
void mc6809::pshu( void )
{
	UINT8 t;
	IMMBYTE(t);
	if( t&0x80 ) { PSHUWORD(pPC); cycles += 2; }
	if( t&0x40 ) { PSHUWORD(pS);  cycles += 2; }
	if( t&0x20 ) { PSHUWORD(pY);  cycles += 2; }
	if( t&0x10 ) { PSHUWORD(pX);  cycles += 2; }
	if( t&0x08 ) { PSHUBYTE(DP);  cycles += 1; }
	if( t&0x04 ) { PSHUBYTE(B);   cycles += 1; }
	if( t&0x02 ) { PSHUBYTE(A);   cycles += 1; }
	if( t&0x01 ) { PSHUBYTE(CC);  cycles += 1; }
}

/* 37 PULU inherent ----- */
void mc6809::pulu( void )
{
	UINT8 t;
	IMMBYTE(t);
	if( t&0x01 ) { PULUBYTE(CC); cycles += 1; }
	if( t&0x02 ) { PULUBYTE(A);  cycles += 1; }
	if( t&0x04 ) { PULUBYTE(B);  cycles += 1; }
	if( t&0x08 ) { PULUBYTE(DP); cycles += 1; }
	if( t&0x10 ) { PULUWORD(XD); cycles += 2; }
	if( t&0x20 ) { PULUWORD(YD); cycles += 2; }
	if( t&0x40 ) { PULUWORD(SD); cycles += 2; }
	if( t&0x80 ) { PULUWORD(PCD); CHANGE_PC; cycles += 2; }

	/* HJB 990225: moved check after all PULLs */
	if( t&0x01 ) { CHECK_IRQ_LINES; }
}

/* $38 ILLEGAL */

/* $39 RTS inherent ----- */
void mc6809::rts( void )
{
	PULLWORD(PCD);
	CHANGE_PC;
}

/* $3A ABX inherent ----- */
void mc6809::abx( void )
{
	X += B;
}

/* $3B RTI inherent ##### */
void mc6809::rti( void )
{
	UINT8 t;
	PULLBYTE(CC);
	t = CC & CC_E;		/* HJB 990225: entire state saved? */
	if(t)
	{
        cycles += 9;
		PULLBYTE(A);
		PULLBYTE(B);
		PULLBYTE(DP);
		PULLWORD(XD);
		PULLWORD(YD);
		PULLWORD(UD);
	}
	PULLWORD(PCD);
	CHANGE_PC;
	CHECK_IRQ_LINES;	/* HJB 990116 */
}

/* $3C CWAI inherent ----1 */
void mc6809::cwai( void )
{
	UINT8 t;
	IMMBYTE(t);
	CC &= t;
	/*
     * CWAI stacks the entire machine state on the hardware stack,
     * then waits for an interrupt; when the interrupt is taken
     * later, the state is *not* saved again after CWAI.
     */
	CC |= CC_E; 		/* HJB 990225: save entire state */
	PUSHWORD(pPC);
	PUSHWORD(pU);
	PUSHWORD(pY);
	PUSHWORD(pX);
	PUSHBYTE(DP);
	PUSHBYTE(B);
	PUSHBYTE(A);
	PUSHBYTE(CC);
	int_state |= M6809_CWAI;	 /* HJB 990228 */
    CHECK_IRQ_LINES;    /* HJB 990116 */
//	if( int_state & M6809_CWAI )
//		if( m6809_ICount > 0 )
//			m6809_ICount = 0;
}

/* $3D MUL inherent --*-@ */
void mc6809::mul( void )
{
	UINT16 t;
	t = A * B;
	CLR_ZC; SET_Z16(t); if(t&0x80) SEC;
	D = t;
}

/* $3E ILLEGAL */

/* $3F SWI (SWI2 SWI3) absolute indirect ----- */
void mc6809::swi( void )
{
	CC |= CC_E; 			/* HJB 980225: save entire state */
	PUSHWORD(pPC);
	PUSHWORD(pU);
	PUSHWORD(pY);
	PUSHWORD(pX);
	PUSHBYTE(DP);
	PUSHBYTE(B);
	PUSHBYTE(A);
	PUSHBYTE(CC);
	CC |= CC_IF | CC_II;	/* inhibit FIRQ and IRQ */
	PCD=RM16(0xfffa);
	CHANGE_PC;
}

/* $103F SWI2 absolute indirect ----- */
void mc6809::swi2( void )
{
	CC |= CC_E; 			/* HJB 980225: save entire state */
	PUSHWORD(pPC);
	PUSHWORD(pU);
	PUSHWORD(pY);
	PUSHWORD(pX);
	PUSHBYTE(DP);
	PUSHBYTE(B);
	PUSHBYTE(A);
    PUSHBYTE(CC);
	PCD = RM16(0xfff4);
	CHANGE_PC;
}

/* $113F SWI3 absolute indirect ----- */
void mc6809::swi3( void )
{
	CC |= CC_E; 			/* HJB 980225: save entire state */
	PUSHWORD(pPC);
	PUSHWORD(pU);
	PUSHWORD(pY);
	PUSHWORD(pX);
	PUSHBYTE(DP);
	PUSHBYTE(B);
	PUSHBYTE(A);
    PUSHBYTE(CC);
	PCD = RM16(0xfff2);
	CHANGE_PC;
}

void mc6809::irq( void )
{
  if ( !(CC & CC_II) ) {
    CC |= CC_E; 				/* save entire state */
    PUSHWORD(pPC);
    PUSHWORD(pU);
    PUSHWORD(pY);
    PUSHWORD(pX);
    PUSHBYTE(DP);
    PUSHBYTE(B);
    PUSHBYTE(A);
    PUSHBYTE(CC);
    extra_cycles += 19;	 /* subtract +19 cycles */
    CC |= CC_II;					/* inhibit IRQ */
    PCD=RM16(0xfff8);
    CHANGE_PC;
  }
}


/* $40 NEGA inherent ?**** */
void mc6809::nega( void )
{
	UINT16 r;
	r = -A;
	CLR_NZVC;
	SET_FLAGS8(0,A,r);
	A = r;
}

/* $41 ILLEGAL */

/* $42 ILLEGAL */

/* $43 COMA inherent -**01 */
void mc6809::coma( void )
{
	A = ~A;
	CLR_NZV;
	SET_NZ8(A);
	SEC;
}

/* $44 LSRA inherent -0*-* */
void mc6809::lsra( void )
{
	CLR_NZC;
	CC |= (A & CC_C);
	A >>= 1;
	SET_Z8(A);
}

/* $45 ILLEGAL */

/* $46 RORA inherent -**-* */
void mc6809::rora( void )
{
	UINT8 r;
	r = (CC & CC_C) << 7;
	CLR_NZC;
	CC |= (A & CC_C);
	r |= A >> 1;
	SET_NZ8(r);
	A = r;
}

/* $47 ASRA inherent ?**-* */
void mc6809::asra( void )
{
	CLR_NZC;
	CC |= (A & CC_C);
	A = (A & 0x80) | (A >> 1);
	SET_NZ8(A);
}

/* $48 ASLA inherent ?**** */
void mc6809::asla( void )
{
	UINT16 r;
	r = A << 1;
	CLR_NZVC;
	SET_FLAGS8(A,A,r);
	A = r;
}

/* $49 ROLA inherent -**** */
void mc6809::rola( void )
{
	UINT16 t,r;
	t = A;
	r = (CC & CC_C) | (t<<1);
	CLR_NZVC; SET_FLAGS8(t,t,r);
	A = r;
}

/* $4A DECA inherent -***- */
void mc6809::deca( void )
{
	--A;
	CLR_NZV;
	SET_FLAGS8D(A);
}

/* $4B ILLEGAL */

/* $4C INCA inherent -***- */
void mc6809::inca( void )
{
	++A;
	CLR_NZV;
	SET_FLAGS8I(A);
}

/* $4D TSTA inherent -**0- */
void mc6809::tsta( void )
{
	CLR_NZV;
	SET_NZ8(A);
}

/* $4E ILLEGAL */

/* $4F CLRA inherent -0100 */
void mc6809::clra( void )
{
	A = 0;
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____5x____
#endif

/* $50 NEGB inherent ?**** */
void mc6809::negb( void )
{
	UINT16 r;
	r = -B;
	CLR_NZVC;
	SET_FLAGS8(0,B,r);
	B = r;
}

/* $51 ILLEGAL */

/* $52 ILLEGAL */

/* $53 COMB inherent -**01 */
void mc6809::comb( void )
{
	B = ~B;
	CLR_NZV;
	SET_NZ8(B);
	SEC;
}

/* $54 LSRB inherent -0*-* */
void mc6809::lsrb( void )
{
	CLR_NZC;
	CC |= (B & CC_C);
	B >>= 1;
	SET_Z8(B);
}

/* $55 ILLEGAL */

/* $56 RORB inherent -**-* */
void mc6809::rorb( void )
{
	UINT8 r;
	r = (CC & CC_C) << 7;
	CLR_NZC;
	CC |= (B & CC_C);
	r |= B >> 1;
	SET_NZ8(r);
	B = r;
}

/* $57 ASRB inherent ?**-* */
void mc6809::asrb( void )
{
	CLR_NZC;
	CC |= (B & CC_C);
	B= (B & 0x80) | (B >> 1);
	SET_NZ8(B);
}

/* $58 ASLB inherent ?**** */
void mc6809::aslb( void )
{
	UINT16 r;
	r = B << 1;
	CLR_NZVC;
	SET_FLAGS8(B,B,r);
	B = r;
}

/* $59 ROLB inherent -**** */
void mc6809::rolb( void )
{
	UINT16 t,r;
	t = B;
	r = CC & CC_C;
	r |= t << 1;
	CLR_NZVC;
	SET_FLAGS8(t,t,r);
	B = r;
}

/* $5A DECB inherent -***- */
void mc6809::decb( void )
{
	--B;
	CLR_NZV;
	SET_FLAGS8D(B);
}

/* $5B ILLEGAL */

/* $5C INCB inherent -***- */
void mc6809::incb( void )
{
	++B;
	CLR_NZV;
	SET_FLAGS8I(B);
}

/* $5D TSTB inherent -**0- */
void mc6809::tstb( void )
{
	CLR_NZV;
	SET_NZ8(B);
}

/* $5E ILLEGAL */

/* $5F CLRB inherent -0100 */
void mc6809::clrb( void )
{
	B = 0;
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____6x____
#endif

/* $60 NEG indexed ?**** */
void mc6809::neg_ix( void )
{
	UINT16 r,t;
	fetch_effective_address();
	t = RM(EAD);
	r=-t;
	CLR_NZVC;
	SET_FLAGS8(0,t,r);
	WM(EAD,r);
}

/* $61 ILLEGAL */

/* $62 ILLEGAL */

/* $63 COM indexed -**01 */
void mc6809::com_ix( void )
{
	UINT8 t;
	fetch_effective_address();
	t = ~RM(EAD);
	CLR_NZV;
	SET_NZ8(t);
	SEC;
	WM(EAD,t);
}

/* $64 LSR indexed -0*-* */
void mc6809::lsr_ix( void )
{
	UINT8 t;
	fetch_effective_address();
	t=RM(EAD);
	CLR_NZC;
	CC |= (t & CC_C);
	t>>=1; SET_Z8(t);
	WM(EAD,t);
}

/* $65 ILLEGAL */

/* $66 ROR indexed -**-* */
void mc6809::ror_ix( void )
{
	UINT8 t,r;
	fetch_effective_address();
	t=RM(EAD);
	r = (CC & CC_C) << 7;
	CLR_NZC;
	CC |= (t & CC_C);
	r |= t>>1; SET_NZ8(r);
	WM(EAD,r);
}

/* $67 ASR indexed ?**-* */
void mc6809::asr_ix( void )
{
	UINT8 t;
	fetch_effective_address();
	t=RM(EAD);
	CLR_NZC;
	CC |= (t & CC_C);
	t=(t&0x80)|(t>>1);
	SET_NZ8(t);
	WM(EAD,t);
}

/* $68 ASL indexed ?**** */
void mc6809::asl_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t=RM(EAD);
	r = t << 1;
	CLR_NZVC;
	SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $69 ROL indexed -**** */
void mc6809::rol_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t=RM(EAD);
	r = CC & CC_C;
	r |= t << 1;
	CLR_NZVC;
	SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $6A DEC indexed -***- */
void mc6809::dec_ix( void )
{
	UINT8 t;
	fetch_effective_address();
	t = RM(EAD) - 1;
	CLR_NZV; SET_FLAGS8D(t);
	WM(EAD,t);
}

/* $6B ILLEGAL */

/* $6C INC indexed -***- */
void mc6809::inc_ix( void )
{
	UINT8 t;
	fetch_effective_address();
	t = RM(EAD) + 1;
	CLR_NZV; SET_FLAGS8I(t);
	WM(EAD,t);
}

/* $6D TST indexed -**0- */
void mc6809::tst_ix( void )
{
	UINT8 t;
	fetch_effective_address();
	t = RM(EAD);
	CLR_NZV;
	SET_NZ8(t);
}

/* $6E JMP indexed ----- */
void mc6809::jmp_ix( void )
{
	fetch_effective_address();
	PCD = EAD;
	CHANGE_PC;
}

/* $6F CLR indexed -0100 */
void mc6809::clr_ix( void )
{
	fetch_effective_address();
    WM(EAD,0);
	CLR_NZVC; SEZ;
}

#if macintosh
#pragma mark ____7x____
#endif

/* $70 NEG extended ?**** */
void mc6809::neg_ex( void )
{
	UINT16 r,t;
	EXTBYTE(t); r=-t;
	CLR_NZVC; SET_FLAGS8(0,t,r);
	WM(EAD,r);
}

/* $71 ILLEGAL */

/* $72 ILLEGAL */

/* $73 COM extended -**01 */
void mc6809::com_ex( void )
{
	UINT8 t;
	EXTBYTE(t); t = ~t;
	CLR_NZV; SET_NZ8(t); SEC;
	WM(EAD,t);
}

/* $74 LSR extended -0*-* */
void mc6809::lsr_ex( void )
{
	UINT8 t;
	EXTBYTE(t); CLR_NZC; CC |= (t & CC_C);
	t>>=1; SET_Z8(t);
	WM(EAD,t);
}

/* $75 ILLEGAL */

/* $76 ROR extended -**-* */
void mc6809::ror_ex( void )
{
	UINT8 t,r;
	EXTBYTE(t); r=(CC & CC_C) << 7;
	CLR_NZC; CC |= (t & CC_C);
	r |= t>>1; SET_NZ8(r);
	WM(EAD,r);
}

/* $77 ASR extended ?**-* */
void mc6809::asr_ex( void )
{
	UINT8 t;
	EXTBYTE(t); CLR_NZC; CC |= (t & CC_C);
	t=(t&0x80)|(t>>1);
	SET_NZ8(t);
	WM(EAD,t);
}

/* $78 ASL extended ?**** */
void mc6809::asl_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r=t<<1;
	CLR_NZVC; SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $79 ROL extended -**** */
void mc6809::rol_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t); r = (CC & CC_C) | (t << 1);
	CLR_NZVC; SET_FLAGS8(t,t,r);
	WM(EAD,r);
}

/* $7A DEC extended -***- */
void mc6809::dec_ex( void )
{
	UINT8 t;
	EXTBYTE(t); --t;
	CLR_NZV; SET_FLAGS8D(t);
	WM(EAD,t);
}

/* $7B ILLEGAL */

/* $7C INC extended -***- */
void mc6809::inc_ex( void )
{
	UINT8 t;
	EXTBYTE(t); ++t;
	CLR_NZV; SET_FLAGS8I(t);
	WM(EAD,t);
}

/* $7D TST extended -**0- */
void mc6809::tst_ex( void )
{
	UINT8 t;
	EXTBYTE(t); CLR_NZV; SET_NZ8(t);
}

/* $7E JMP extended ----- */
void mc6809::jmp_ex( void )
{
	EXTENDED;
	PCD = EAD;
	CHANGE_PC;
}

/* $7F CLR extended -0100 */
void mc6809::clr_ex( void )
{
	EXTENDED;
	WM(EAD,0);
	CLR_NZVC; SEZ;
}


#if macintosh
#pragma mark ____8x____
#endif

/* $80 SUBA immediate ?**** */
void mc6809::suba_im( void )
{
	UINT16 t,r;
	IMMBYTE(t);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $81 CMPA immediate ?**** */
void mc6809::cmpa_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
}

/* $82 SBCA immediate ?**** */
void mc6809::sbca_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t);
	r = A - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $83 SUBD (CMPD CMPU) immediate -**** */
void mc6809::subd_im( void )
{
	UINT32 r,d;
	PAIR b;
	IMMWORD(b);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $1083 CMPD immediate -**** */
void mc6809::cmpd_im( void )
{
	UINT32 r,d;
	PAIR b;
	IMMWORD(b);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $1183 CMPU immediate -**** */
void mc6809::cmpu_im( void )
{
	UINT32 r, d;
	PAIR b;
	IMMWORD(b);
	d = U;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $84 ANDA immediate -**0- */
void mc6809::anda_im( void )
{
	UINT8 t;
	IMMBYTE(t);
	A &= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $85 BITA immediate -**0- */
void mc6809::bita_im( void )
{
	UINT8 t,r;
	IMMBYTE(t);
	r = A & t;
	CLR_NZV;
	SET_NZ8(r);
}

/* $86 LDA immediate -**0- */
void mc6809::lda_im( void )
{
	IMMBYTE(A);
	CLR_NZV;
	SET_NZ8(A);
}

/* is this a legal instruction? */
/* $87 STA immediate -**0- */
void mc6809::sta_im( void )
{
	CLR_NZV;
	SET_NZ8(A);
	IMM8;
	WM(EAD,A);
}

/* $88 EORA immediate -**0- */
void mc6809::eora_im( void )
{
	UINT8 t;
	IMMBYTE(t);
	A ^= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $89 ADCA immediate ***** */
void mc6809::adca_im( void )
{
	UINT16 t,r;
	IMMBYTE(t);
	r = A + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $8A ORA immediate -**0- */
void mc6809::ora_im( void )
{
	UINT8 t;
	IMMBYTE(t);
	A |= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $8B ADDA immediate ***** */
void mc6809::adda_im( void )
{
	UINT16 t,r;
	IMMBYTE(t);
	r = A + t;
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $8C CMPX (CMPY CMPS) immediate -**** */
void mc6809::cmpx_im( void )
{
	UINT32 r,d;
	PAIR b;
	IMMWORD(b);
	d = X;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $108C CMPY immediate -**** */
void mc6809::cmpy_im( void )
{
	UINT32 r,d;
	PAIR b;
	IMMWORD(b);
	d = Y;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $118C CMPS immediate -**** */
void mc6809::cmps_im( void )
{
	UINT32 r,d;
	PAIR b;
	IMMWORD(b);
	d = S;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $8D BSR ----- */
void mc6809::bsr( void )
{
	UINT8 t;
	IMMBYTE(t);
	PUSHWORD(pPC);
	PC += SIGNED(t);
	CHANGE_PC;
}

/* $8E LDX (LDY) immediate -**0- */
void mc6809::ldx_im( void )
{
	IMMWORD(pX);
	CLR_NZV;
	SET_NZ16(X);
}

/* $108E LDY immediate -**0- */
void mc6809::ldy_im( void )
{
	IMMWORD(pY);
	CLR_NZV;
	SET_NZ16(Y);
}

/* is this a legal instruction? */
/* $8F STX (STY) immediate -**0- */
void mc6809::stx_im( void )
{
	CLR_NZV;
	SET_NZ16(X);
	IMM16;
	WM16(EAD,&pX);
}

/* is this a legal instruction? */
/* $108F STY immediate -**0- */
void mc6809::sty_im( void )
{
	CLR_NZV;
	SET_NZ16(Y);
	IMM16;
	WM16(EAD,&pY);
}

#if macintosh
#pragma mark ____9x____
#endif

/* $90 SUBA direct ?**** */
void mc6809::suba_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $91 CMPA direct ?**** */
void mc6809::cmpa_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
}

/* $92 SBCA direct ?**** */
void mc6809::sbca_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t);
	r = A - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $93 SUBD (CMPD CMPU) direct -**** */
void mc6809::subd_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $1093 CMPD direct -**** */
void mc6809::cmpd_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $1193 CMPU direct -**** */
void mc6809::cmpu_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = U;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(U,b.d,r);
}

/* $94 ANDA direct -**0- */
void mc6809::anda_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	A &= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $95 BITA direct -**0- */
void mc6809::bita_di( void )
{
	UINT8 t,r;
	DIRBYTE(t);
	r = A & t;
	CLR_NZV;
	SET_NZ8(r);
}

/* $96 LDA direct -**0- */
void mc6809::lda_di( void )
{
	DIRBYTE(A);
	CLR_NZV;
	SET_NZ8(A);
}

/* $97 STA direct -**0- */
void mc6809::sta_di( void )
{
	CLR_NZV;
	SET_NZ8(A);
	DIRECT;
	WM(EAD,A);
}

/* $98 EORA direct -**0- */
void mc6809::eora_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	A ^= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $99 ADCA direct ***** */
void mc6809::adca_di( void )
{
	UINT16 t,r;
	DIRBYTE(t);
	r = A + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $9A ORA direct -**0- */
void mc6809::ora_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	A |= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $9B ADDA direct ***** */
void mc6809::adda_di( void )
{
	UINT16 t,r;
	DIRBYTE(t);
	r = A + t;
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $9C CMPX (CMPY CMPS) direct -**** */
void mc6809::cmpx_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = X;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $109C CMPY direct -**** */
void mc6809::cmpy_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = Y;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $119C CMPS direct -**** */
void mc6809::cmps_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = S;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $9D JSR direct ----- */
void mc6809::jsr_di( void )
{
	DIRECT;
	PUSHWORD(pPC);
	PCD = EAD;
	CHANGE_PC;
}

/* $9E LDX (LDY) direct -**0- */
void mc6809::ldx_di( void )
{
	DIRWORD(pX);
	CLR_NZV;
	SET_NZ16(X);
}

/* $109E LDY direct -**0- */
void mc6809::ldy_di( void )
{
	DIRWORD(pY);
	CLR_NZV;
	SET_NZ16(Y);
}

/* $9F STX (STY) direct -**0- */
void mc6809::stx_di( void )
{
	CLR_NZV;
	SET_NZ16(X);
	DIRECT;
	WM16(EAD,&pX);
}

/* $109F STY direct -**0- */
void mc6809::sty_di( void )
{
	CLR_NZV;
	SET_NZ16(Y);
	DIRECT;
	WM16(EAD,&pY);
}

#if macintosh
#pragma mark ____Ax____
#endif


/* $a0 SUBA indexed ?**** */
void mc6809::suba_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $a1 CMPA indexed ?**** */
void mc6809::cmpa_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
}

/* $a2 SBCA indexed ?**** */
void mc6809::sbca_ix( void )
{
	UINT16	  t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = A - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $a3 SUBD (CMPD CMPU) indexed -**** */
void mc6809::subd_ix( void )
{
	UINT32 r,d;
	PAIR b;
	fetch_effective_address();
    b.d=RM16(EAD);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $10a3 CMPD indexed -**** */
void mc6809::cmpd_ix( void )
{
	UINT32 r,d;
	PAIR b;
	fetch_effective_address();
    b.d=RM16(EAD);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $11a3 CMPU indexed -**** */
void mc6809::cmpu_ix( void )
{
	UINT32 r;
	PAIR b;
	fetch_effective_address();
    b.d=RM16(EAD);
	r = U - b.d;
	CLR_NZVC;
	SET_FLAGS16(U,b.d,r);
}

/* $a4 ANDA indexed -**0- */
void mc6809::anda_ix( void )
{
	fetch_effective_address();
	A &= RM(EAD);
	CLR_NZV;
	SET_NZ8(A);
}

/* $a5 BITA indexed -**0- */
void mc6809::bita_ix( void )
{
	UINT8 r;
	fetch_effective_address();
	r = A & RM(EAD);
	CLR_NZV;
	SET_NZ8(r);
}

/* $a6 LDA indexed -**0- */
void mc6809::lda_ix( void )
{
	fetch_effective_address();
	A = RM(EAD);
	CLR_NZV;
	SET_NZ8(A);
}

/* $a7 STA indexed -**0- */
void mc6809::sta_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ8(A);
	WM(EAD,A);
}

/* $a8 EORA indexed -**0- */
void mc6809::eora_ix( void )
{
	fetch_effective_address();
	A ^= RM(EAD);
	CLR_NZV;
	SET_NZ8(A);
}

/* $a9 ADCA indexed ***** */
void mc6809::adca_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = A + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $aA ORA indexed -**0- */
void mc6809::ora_ix( void )
{
	fetch_effective_address();
	A |= RM(EAD);
	CLR_NZV;
	SET_NZ8(A);
}

/* $aB ADDA indexed ***** */
void mc6809::adda_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = A + t;
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $aC CMPX (CMPY CMPS) indexed -**** */
void mc6809::cmpx_ix( void )
{
	UINT32 r,d;
	PAIR b;
	fetch_effective_address();
    b.d=RM16(EAD);
	d = X;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $10aC CMPY indexed -**** */
void mc6809::cmpy_ix( void )
{
	UINT32 r,d;
	PAIR b;
	fetch_effective_address();
    b.d=RM16(EAD);
	d = Y;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $11aC CMPS indexed -**** */
void mc6809::cmps_ix( void )
{
	UINT32 r,d;
	PAIR b;
	fetch_effective_address();
    b.d=RM16(EAD);
	d = S;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $aD JSR indexed ----- */
void mc6809::jsr_ix( void )
{
	fetch_effective_address();
    PUSHWORD(pPC);
	PCD = EAD;
	CHANGE_PC;
}

/* $aE LDX (LDY) indexed -**0- */
void mc6809::ldx_ix( void )
{
	fetch_effective_address();
    X=RM16(EAD);
	CLR_NZV;
	SET_NZ16(X);
}

/* $10aE LDY indexed -**0- */
void mc6809::ldy_ix( void )
{
	fetch_effective_address();
    Y=RM16(EAD);
	CLR_NZV;
	SET_NZ16(Y);
}

/* $aF STX (STY) indexed -**0- */
void mc6809::stx_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ16(X);
	WM16(EAD,&pX);
}

/* $10aF STY indexed -**0- */
void mc6809::sty_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ16(Y);
	WM16(EAD,&pY);
}

#if macintosh
#pragma mark ____Bx____
#endif

/* $b0 SUBA extended ?**** */
void mc6809::suba_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $b1 CMPA extended ?**** */
void mc6809::cmpa_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t);
	r = A - t;
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
}

/* $b2 SBCA extended ?**** */
void mc6809::sbca_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t);
	r = A - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(A,t,r);
	A = r;
}

/* $b3 SUBD (CMPD CMPU) extended -**** */
void mc6809::subd_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $10b3 CMPD extended -**** */
void mc6809::cmpd_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = D;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $11b3 CMPU extended -**** */
void mc6809::cmpu_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = U;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $b4 ANDA extended -**0- */
void mc6809::anda_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	A &= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $b5 BITA extended -**0- */
void mc6809::bita_ex( void )
{
	UINT8 t,r;
	EXTBYTE(t);
	r = A & t;
	CLR_NZV; SET_NZ8(r);
}

/* $b6 LDA extended -**0- */
void mc6809::lda_ex( void )
{
	EXTBYTE(A);
	CLR_NZV;
	SET_NZ8(A);
}

/* $b7 STA extended -**0- */
void mc6809::sta_ex( void )
{
	CLR_NZV;
	SET_NZ8(A);
	EXTENDED;
	WM(EAD,A);
}

/* $b8 EORA extended -**0- */
void mc6809::eora_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	A ^= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $b9 ADCA extended ***** */
void mc6809::adca_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t);
	r = A + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $bA ORA extended -**0- */
void mc6809::ora_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	A |= t;
	CLR_NZV;
	SET_NZ8(A);
}

/* $bB ADDA extended ***** */
void mc6809::adda_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t);
	r = A + t;
	CLR_HNZVC;
	SET_FLAGS8(A,t,r);
	SET_H(A,t,r);
	A = r;
}

/* $bC CMPX (CMPY CMPS) extended -**** */
void mc6809::cmpx_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = X;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $10bC CMPY extended -**** */
void mc6809::cmpy_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = Y;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $11bC CMPS extended -**** */
void mc6809::cmps_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = S;
	r = d - b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
}

/* $bD JSR extended ----- */
void mc6809::jsr_ex( void )
{
	EXTENDED;
	PUSHWORD(pPC);
	PCD = EAD;
	CHANGE_PC;
}

/* $bE LDX (LDY) extended -**0- */
void mc6809::ldx_ex( void )
{
	EXTWORD(pX);
	CLR_NZV;
	SET_NZ16(X);
}

/* $10bE LDY extended -**0- */
void mc6809::ldy_ex( void )
{
	EXTWORD(pY);
	CLR_NZV;
	SET_NZ16(Y);
}

/* $bF STX (STY) extended -**0- */
void mc6809::stx_ex( void )
{
	CLR_NZV;
	SET_NZ16(X);
	EXTENDED;
	WM16(EAD,&pX);
}

/* $10bF STY extended -**0- */
void mc6809::sty_ex( void )
{
	CLR_NZV;
	SET_NZ16(Y);
	EXTENDED;
	WM16(EAD,&pY);
}


#if macintosh
#pragma mark ____Cx____
#endif

/* $c0 SUBB immediate ?**** */
void mc6809::subb_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $c1 CMPB immediate ?**** */
void mc6809::cmpb_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t);
	r = B - t;
	CLR_NZVC; SET_FLAGS8(B,t,r);
}

/* $c2 SBCB immediate ?**** */
void mc6809::sbcb_im( void )
{
	UINT16	  t,r;
	IMMBYTE(t);
	r = B - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $c3 ADDD immediate -**** */
void mc6809::addd_im( void )
{
	UINT32 r,d;
	PAIR b;
	IMMWORD(b);
	d = D;
	r = d + b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $c4 ANDB immediate -**0- */
void mc6809::andb_im( void )
{
	UINT8 t;
	IMMBYTE(t);
	B &= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $c5 BITB immediate -**0- */
void mc6809::bitb_im( void )
{
	UINT8 t,r;
	IMMBYTE(t);
	r = B & t;
	CLR_NZV;
	SET_NZ8(r);
}

/* $c6 LDB immediate -**0- */
void mc6809::ldb_im( void )
{
	IMMBYTE(B);
	CLR_NZV;
	SET_NZ8(B);
}

/* is this a legal instruction? */
/* $c7 STB immediate -**0- */
void mc6809::stb_im( void )
{
	CLR_NZV;
	SET_NZ8(B);
	IMM8;
	WM(EAD,B);
}

/* $c8 EORB immediate -**0- */
void mc6809::eorb_im( void )
{
	UINT8 t;
	IMMBYTE(t);
	B ^= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $c9 ADCB immediate ***** */
void mc6809::adcb_im( void )
{
	UINT16 t,r;
	IMMBYTE(t);
	r = B + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $cA ORB immediate -**0- */
void mc6809::orb_im( void )
{
	UINT8 t;
	IMMBYTE(t);
	B |= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $cB ADDB immediate ***** */
void mc6809::addb_im( void )
{
	UINT16 t,r;
	IMMBYTE(t);
	r = B + t;
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $cC LDD immediate -**0- */
void mc6809::ldd_im( void )
{
	IMMWORD(pD);
	CLR_NZV;
	SET_NZ16(D);
}

/* is this a legal instruction? */
/* $cD STD immediate -**0- */
void mc6809::std_im( void )
{
	CLR_NZV;
	SET_NZ16(D);
    IMM16;
	WM16(EAD,&pD);
}

/* $cE LDU (LDS) immediate -**0- */
void mc6809::ldu_im( void )
{
	IMMWORD(pU);
	CLR_NZV;
	SET_NZ16(U);
}

/* $10cE LDS immediate -**0- */
void mc6809::lds_im( void )
{
	IMMWORD(pS);
	CLR_NZV;
	SET_NZ16(S);
    int_state |= M6809_LDS;
}

/* is this a legal instruction? */
/* $cF STU (STS) immediate -**0- */
void mc6809::stu_im( void )
{
	CLR_NZV;
	SET_NZ16(U);
    IMM16;
	WM16(EAD,&pU);
}

/* is this a legal instruction? */
/* $10cF STS immediate -**0- */
void mc6809::sts_im( void )
{
	CLR_NZV;
	SET_NZ16(S);
    IMM16;
	WM16(EAD,&pS);
}


#if macintosh
#pragma mark ____Dx____
#endif

/* $d0 SUBB direct ?**** */
void mc6809::subb_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $d1 CMPB direct ?**** */
void mc6809::cmpb_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
}

/* $d2 SBCB direct ?**** */
void mc6809::sbcb_di( void )
{
	UINT16	  t,r;
	DIRBYTE(t);
	r = B - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $d3 ADDD direct -**** */
void mc6809::addd_di( void )
{
	UINT32 r,d;
	PAIR b;
	DIRWORD(b);
	d = D;
	r = d + b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $d4 ANDB direct -**0- */
void mc6809::andb_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	B &= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $d5 BITB direct -**0- */
void mc6809::bitb_di( void )
{
	UINT8 t,r;
	DIRBYTE(t);
	r = B & t;
	CLR_NZV;
	SET_NZ8(r);
}

/* $d6 LDB direct -**0- */
void mc6809::ldb_di( void )
{
	DIRBYTE(B);
	CLR_NZV;
	SET_NZ8(B);
}

/* $d7 STB direct -**0- */
void mc6809::stb_di( void )
{
	CLR_NZV;
	SET_NZ8(B);
	DIRECT;
	WM(EAD,B);
}

/* $d8 EORB direct -**0- */
void mc6809::eorb_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	B ^= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $d9 ADCB direct ***** */
void mc6809::adcb_di( void )
{
	UINT16 t,r;
	DIRBYTE(t);
	r = B + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $dA ORB direct -**0- */
void mc6809::orb_di( void )
{
	UINT8 t;
	DIRBYTE(t);
	B |= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $dB ADDB direct ***** */
void mc6809::addb_di( void )
{
	UINT16 t,r;
	DIRBYTE(t);
	r = B + t;
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $dC LDD direct -**0- */
void mc6809::ldd_di( void )
{
	DIRWORD(pD);
	CLR_NZV;
	SET_NZ16(D);
}

/* $dD STD direct -**0- */
void mc6809::std_di( void )
{
	CLR_NZV;
	SET_NZ16(D);
    DIRECT;
	WM16(EAD,&pD);
}

/* $dE LDU (LDS) direct -**0- */
void mc6809::ldu_di( void )
{
	DIRWORD(pU);
	CLR_NZV;
	SET_NZ16(U);
}

/* $10dE LDS direct -**0- */
void mc6809::lds_di( void )
{
	DIRWORD(pS);
	CLR_NZV;
	SET_NZ16(S);
	int_state |= M6809_LDS;
}

/* $dF STU (STS) direct -**0- */
void mc6809::stu_di( void )
{
	CLR_NZV;
	SET_NZ16(U);
	DIRECT;
	WM16(EAD,&pU);
}

/* $10dF STS direct -**0- */
void mc6809::sts_di( void )
{
	CLR_NZV;
	SET_NZ16(S);
	DIRECT;
	WM16(EAD,&pS);
}

#if macintosh
#pragma mark ____Ex____
#endif


/* $e0 SUBB indexed ?**** */
void mc6809::subb_ix( void )
{
	UINT16	  t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $e1 CMPB indexed ?**** */
void mc6809::cmpb_ix( void )
{
	UINT16	  t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
}

/* $e2 SBCB indexed ?**** */
void mc6809::sbcb_ix( void )
{
	UINT16	  t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = B - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $e3 ADDD indexed -**** */
void mc6809::addd_ix( void )
{
	UINT32 r,d;
    PAIR b;
    fetch_effective_address();
	b.d=RM16(EAD);
	d = D;
	r = d + b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $e4 ANDB indexed -**0- */
void mc6809::andb_ix( void )
{
	fetch_effective_address();
	B &= RM(EAD);
	CLR_NZV;
	SET_NZ8(B);
}

/* $e5 BITB indexed -**0- */
void mc6809::bitb_ix( void )
{
	UINT8 r;
	fetch_effective_address();
	r = B & RM(EAD);
	CLR_NZV;
	SET_NZ8(r);
}

/* $e6 LDB indexed -**0- */
void mc6809::ldb_ix( void )
{
	fetch_effective_address();
	B = RM(EAD);
	CLR_NZV;
	SET_NZ8(B);
}

/* $e7 STB indexed -**0- */
void mc6809::stb_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ8(B);
	WM(EAD,B);
}

/* $e8 EORB indexed -**0- */
void mc6809::eorb_ix( void )
{
	fetch_effective_address();
	B ^= RM(EAD);
	CLR_NZV;
	SET_NZ8(B);
}

/* $e9 ADCB indexed ***** */
void mc6809::adcb_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = B + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $eA ORB indexed -**0- */
void mc6809::orb_ix( void )
{
	fetch_effective_address();
	B |= RM(EAD);
	CLR_NZV;
	SET_NZ8(B);
}

/* $eB ADDB indexed ***** */
void mc6809::addb_ix( void )
{
	UINT16 t,r;
	fetch_effective_address();
	t = RM(EAD);
	r = B + t;
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $eC LDD indexed -**0- */
void mc6809::ldd_ix( void )
{
	fetch_effective_address();
    D=RM16(EAD);
	CLR_NZV; SET_NZ16(D);
}

/* $eD STD indexed -**0- */
void mc6809::std_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ16(D);
	WM16(EAD,&pD);
}

/* $eE LDU (LDS) indexed -**0- */
void mc6809::ldu_ix( void )
{
	fetch_effective_address();
    U=RM16(EAD);
	CLR_NZV;
	SET_NZ16(U);
}

/* $10eE LDS indexed -**0- */
void mc6809::lds_ix( void )
{
	fetch_effective_address();
    S=RM16(EAD);
	CLR_NZV;
	SET_NZ16(S);
	int_state |= M6809_LDS;
}

/* $eF STU (STS) indexed -**0- */
void mc6809::stu_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ16(U);
	WM16(EAD,&pU);
}

/* $10eF STS indexed -**0- */
void mc6809::sts_ix( void )
{
	fetch_effective_address();
    CLR_NZV;
	SET_NZ16(S);
	WM16(EAD,&pS);
}

#if macintosh
#pragma mark ____Fx____
#endif

/* $f0 SUBB extended ?**** */
void mc6809::subb_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $f1 CMPB extended ?**** */
void mc6809::cmpb_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t);
	r = B - t;
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
}

/* $f2 SBCB extended ?**** */
void mc6809::sbcb_ex( void )
{
	UINT16	  t,r;
	EXTBYTE(t);
	r = B - t - (CC & CC_C);
	CLR_NZVC;
	SET_FLAGS8(B,t,r);
	B = r;
}

/* $f3 ADDD extended -**** */
void mc6809::addd_ex( void )
{
	UINT32 r,d;
	PAIR b;
	EXTWORD(b);
	d = D;
	r = d + b.d;
	CLR_NZVC;
	SET_FLAGS16(d,b.d,r);
	D = r;
}

/* $f4 ANDB extended -**0- */
void mc6809::andb_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	B &= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $f5 BITB extended -**0- */
void mc6809::bitb_ex( void )
{
	UINT8 t,r;
	EXTBYTE(t);
	r = B & t;
	CLR_NZV;
	SET_NZ8(r);
}

/* $f6 LDB extended -**0- */
void mc6809::ldb_ex( void )
{
	EXTBYTE(B);
	CLR_NZV;
	SET_NZ8(B);
}

/* $f7 STB extended -**0- */
void mc6809::stb_ex( void )
{
	CLR_NZV;
	SET_NZ8(B);
	EXTENDED;
	WM(EAD,B);
}

/* $f8 EORB extended -**0- */
void mc6809::eorb_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	B ^= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $f9 ADCB extended ***** */
void mc6809::adcb_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t);
	r = B + t + (CC & CC_C);
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $fA ORB extended -**0- */
void mc6809::orb_ex( void )
{
	UINT8 t;
	EXTBYTE(t);
	B |= t;
	CLR_NZV;
	SET_NZ8(B);
}

/* $fB ADDB extended ***** */
void mc6809::addb_ex( void )
{
	UINT16 t,r;
	EXTBYTE(t);
	r = B + t;
	CLR_HNZVC;
	SET_FLAGS8(B,t,r);
	SET_H(B,t,r);
	B = r;
}

/* $fC LDD extended -**0- */
void mc6809::ldd_ex( void )
{
	EXTWORD(pD);
	CLR_NZV;
	SET_NZ16(D);
}

/* $fD STD extended -**0- */
void mc6809::std_ex( void )
{
	CLR_NZV;
	SET_NZ16(D);
    EXTENDED;
	WM16(EAD,&pD);
}

/* $fE LDU (LDS) extended -**0- */
void mc6809::ldu_ex( void )
{
	EXTWORD(pU);
	CLR_NZV;
	SET_NZ16(U);
}

/* $10fE LDS extended -**0- */
void mc6809::lds_ex( void )
{
	EXTWORD(pS);
	CLR_NZV;
	SET_NZ16(S);
	int_state |= M6809_LDS;
}

/* $fF STU (STS) extended -**0- */
void mc6809::stu_ex( void )
{
	CLR_NZV;
	SET_NZ16(U);
	EXTENDED;
	WM16(EAD,&pU);
}

/* $10fF STS extended -**0- */
void mc6809::sts_ex( void )
{
	CLR_NZV;
	SET_NZ16(S);
	EXTENDED;
	WM16(EAD,&pS);
}

/* $10xx opcodes */
void mc6809::pref10( void )
{
	UINT8 ireg2 = ROP(PCD);
	PC++;
	switch( ireg2 )
	{
		case 0x21: lbrn();		cycles +=5;	break;
		case 0x22: lbhi();		cycles +=5;	break;
		case 0x23: lbls();		cycles +=5;	break;
		case 0x24: lbcc();		cycles +=5;	break;
		case 0x25: lbcs();		cycles +=5;	break;
		case 0x26: lbne();		cycles +=5;	break;
		case 0x27: lbeq();		cycles +=5;	break;
		case 0x28: lbvc();		cycles +=5;	break;
		case 0x29: lbvs();		cycles +=5;	break;
		case 0x2a: lbpl();		cycles +=5;	break;
		case 0x2b: lbmi();		cycles +=5;	break;
		case 0x2c: lbge();		cycles +=5;	break;
		case 0x2d: lblt();		cycles +=5;	break;
		case 0x2e: lbgt();		cycles +=5;	break;
		case 0x2f: lble();		cycles +=5;	break;

		case 0x3f: swi2();		cycles +=20;	break;

		case 0x83: cmpd_im();	cycles +=5;	break;
		case 0x8c: cmpy_im();	cycles +=5;	break;
		case 0x8e: ldy_im();	cycles +=4;	break;
		case 0x8f: sty_im();	cycles +=4;	break;

		case 0x93: cmpd_di();	cycles +=7;	break;
		case 0x9c: cmpy_di();	cycles +=7;	break;
		case 0x9e: ldy_di();	cycles +=6;	break;
		case 0x9f: sty_di();	cycles +=6;	break;

		case 0xa3: cmpd_ix();	cycles +=7;	break;
		case 0xac: cmpy_ix();	cycles +=7;	break;
		case 0xae: ldy_ix();	cycles +=6;	break;
		case 0xaf: sty_ix();	cycles +=6;	break;

		case 0xb3: cmpd_ex();	cycles +=8;	break;
		case 0xbc: cmpy_ex();	cycles +=8;	break;
		case 0xbe: ldy_ex();	cycles +=7;	break;
		case 0xbf: sty_ex();	cycles +=7;	break;

		case 0xce: lds_im();	cycles +=4;	break;
		case 0xcf: sts_im();	cycles +=4;	break;

		case 0xde: lds_di();	cycles +=4;	break;
		case 0xdf: sts_di();	cycles +=4;	break;

		case 0xee: lds_ix();	cycles +=6;	break;
		case 0xef: sts_ix();	cycles +=6;	break;

		case 0xfe: lds_ex();	cycles +=7;	break;
		case 0xff: sts_ex();	cycles +=7;	break;

		default:   illegal();						break;
	}
}

/* $11xx opcodes */
void mc6809::pref11( void )
{
	UINT8 ireg2 = ROP(PCD);
	PC++;
	switch( ireg2 )
	{
		case 0x3f: swi3();		cycles +=20;	break;

		case 0x83: cmpu_im();	cycles +=5;	break;
		case 0x8c: cmps_im();	cycles +=5;	break;

		case 0x93: cmpu_di();	cycles +=7;	break;
		case 0x9c: cmps_di();	cycles +=7;	break;

		case 0xa3: cmpu_ix();	cycles +=7;	break;
		case 0xac: cmps_ix();	cycles +=7;	break;

		case 0xb3: cmpu_ex();	cycles +=8;	break;
		case 0xbc: cmps_ex();	cycles +=8;	break;

		default:   illegal();						break;
	}
}



