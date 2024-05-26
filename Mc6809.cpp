//
//	mc6809.cpp
//

#include "typedefs.h"
#include "machdep.h"
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
		m6809_ICount -= 1;				\
		PC += t.w.l;					\
		CHANGE_PC;						\
	}									\
}

#define NXORV  ((CC&CC_N)^((CC&CC_V)<<2))

mc6809::mc6809()
{
  ;
}

mc6809::~mc6809()
{
  ;
}

void mc6809::reset(void)
{
	int_state = 0;
	irq_state[0] = M6809_CLEAR_LINE;
	irq_state[1] = M6809_CLEAR_LINE;
	irq_state[2] = M6809_CLEAR_LINE;

	DPD = 0;			/* Reset direct page register */

    CC |= CC_II;        /* IRQ disabled */
    CC |= CC_IF;        /* FIRQ disabled */

	PCD = RM16(0xfffe);
	CHANGE_PC;

}

int mc6809::execute(void)
{
    cycles = 0;

    if( int_state & M6809_CWAI ) {
      cycles += 2;
      return cycles;
    }
    
	ireg = read(pc.w.l++);

            switch( ireg )
			{
			case 0x00: neg_di();   cycles+= 6; break;
			case 0x01: illegal();  cycles+= 2; break;
			case 0x02: illegal();  cycles+= 2; break;
			case 0x03: com_di();   cycles+= 6; break;
			case 0x04: lsr_di();   cycles+= 6; break;
			case 0x05: illegal();  cycles+= 2; break;
			case 0x06: ror_di();   cycles+= 6; break;
			case 0x07: asr_di();   cycles+= 6; break;
			case 0x08: asl_di();   cycles+= 6; break;
			case 0x09: rol_di();   cycles+= 6; break;
			case 0x0a: dec_di();   cycles+= 6; break;
			case 0x0b: illegal();  cycles+= 2; break;
			case 0x0c: inc_di();   cycles+= 6; break;
			case 0x0d: tst_di();   cycles+= 6; break;
			case 0x0e: jmp_di();   cycles+= 3; break;
			case 0x0f: clr_di();   cycles+= 6; break;
			case 0x10: pref10();			   break;
			case 0x11: pref11();			   break;
			case 0x12: nop();	   cycles+= 2; break;
			case 0x13: sync();	   cycles+= 4; break;
			case 0x14: illegal();  cycles+= 2; break;
			case 0x15: illegal();  cycles+= 2; break;
			case 0x16: lbra();	   cycles+= 5; break;
			case 0x17: lbsr();	   cycles+= 9; break;
			case 0x18: illegal();  cycles+= 2; break;
			case 0x19: daa();	   cycles+= 2; break;
			case 0x1a: orcc();	   cycles+= 3; break;
			case 0x1b: illegal();  cycles+= 2; break;
			case 0x1c: andcc();    cycles+= 3; break;
			case 0x1d: sex();	   cycles+= 2; break;
			case 0x1e: exg();	   cycles+= 8; break;
			case 0x1f: tfr();	   cycles+= 6; break;
			case 0x20: bra();	   cycles+= 3; break;
			case 0x21: brn();	   cycles+= 3; break;
			case 0x22: bhi();	   cycles+= 3; break;
			case 0x23: bls();	   cycles+= 3; break;
			case 0x24: bcc();	   cycles+= 3; break;
			case 0x25: bcs();	   cycles+= 3; break;
			case 0x26: bne();	   cycles+= 3; break;
			case 0x27: beq();	   cycles+= 3; break;
			case 0x28: bvc();	   cycles+= 3; break;
			case 0x29: bvs();	   cycles+= 3; break;
			case 0x2a: bpl();	   cycles+= 3; break;
			case 0x2b: bmi();	   cycles+= 3; break;
			case 0x2c: bge();	   cycles+= 3; break;
			case 0x2d: blt();	   cycles+= 3; break;
			case 0x2e: bgt();	   cycles+= 3; break;
			case 0x2f: ble();	   cycles+= 3; break;
			case 0x30: leax();	   cycles+= 4; break;
			case 0x31: leay();	   cycles+= 4; break;
			case 0x32: leas();	   cycles+= 4; break;
			case 0x33: leau();	   cycles+= 4; break;
			case 0x34: pshs();	   cycles+= 5; break;
			case 0x35: puls();	   cycles+= 5; break;
			case 0x36: pshu();	   cycles+= 5; break;
			case 0x37: pulu();	   cycles+= 5; break;
			case 0x38: illegal();  cycles+= 2; break;
			case 0x39: rts();	   cycles+= 5; break;
			case 0x3a: abx();	   cycles+= 3; break;
			case 0x3b: rti();	   cycles+= 6; break;
			case 0x3c: cwai();	   cycles+=20; break;
			case 0x3d: mul();	   cycles+=11; break;
			case 0x3e: illegal();  cycles+= 2; break;
			case 0x3f: swi();	   cycles+=19; break;
			case 0x40: nega();	   cycles+= 2; break;
			case 0x41: illegal();  cycles+= 2; break;
			case 0x42: illegal();  cycles+= 2; break;
			case 0x43: coma();	   cycles+= 2; break;
			case 0x44: lsra();	   cycles+= 2; break;
			case 0x45: illegal();  cycles+= 2; break;
			case 0x46: rora();	   cycles+= 2; break;
			case 0x47: asra();	   cycles+= 2; break;
			case 0x48: asla();	   cycles+= 2; break;
			case 0x49: rola();	   cycles+= 2; break;
			case 0x4a: deca();	   cycles+= 2; break;
			case 0x4b: illegal();  cycles+= 2; break;
			case 0x4c: inca();	   cycles+= 2; break;
			case 0x4d: tsta();	   cycles+= 2; break;
			case 0x4e: illegal();  cycles+= 2; break;
			case 0x4f: clra();	   cycles+= 2; break;
			case 0x50: negb();	   cycles+= 2; break;
			case 0x51: illegal();  cycles+= 2; break;
			case 0x52: illegal();  cycles+= 2; break;
			case 0x53: comb();	   cycles+= 2; break;
			case 0x54: lsrb();	   cycles+= 2; break;
			case 0x55: illegal();  cycles+= 2; break;
			case 0x56: rorb();	   cycles+= 2; break;
			case 0x57: asrb();	   cycles+= 2; break;
			case 0x58: aslb();	   cycles+= 2; break;
			case 0x59: rolb();	   cycles+= 2; break;
			case 0x5a: decb();	   cycles+= 2; break;
			case 0x5b: illegal();  cycles+= 2; break;
			case 0x5c: incb();	   cycles+= 2; break;
			case 0x5d: tstb();	   cycles+= 2; break;
			case 0x5e: illegal();  cycles+= 2; break;
			case 0x5f: clrb();	   cycles+= 2; break;
			case 0x60: neg_ix();   cycles+= 6; break;
			case 0x61: illegal();  cycles+= 2; break;
			case 0x62: illegal();  cycles+= 2; break;
			case 0x63: com_ix();   cycles+= 6; break;
			case 0x64: lsr_ix();   cycles+= 6; break;
			case 0x65: illegal();  cycles+= 2; break;
			case 0x66: ror_ix();   cycles+= 6; break;
			case 0x67: asr_ix();   cycles+= 6; break;
			case 0x68: asl_ix();   cycles+= 6; break;
			case 0x69: rol_ix();   cycles+= 6; break;
			case 0x6a: dec_ix();   cycles+= 6; break;
			case 0x6b: illegal();  cycles+= 2; break;
			case 0x6c: inc_ix();   cycles+= 6; break;
			case 0x6d: tst_ix();   cycles+= 6; break;
			case 0x6e: jmp_ix();   cycles+= 3; break;
			case 0x6f: clr_ix();   cycles+= 6; break;
			case 0x70: neg_ex();   cycles+= 7; break;
			case 0x71: illegal();  cycles+= 2; break;
			case 0x72: illegal();  cycles+= 2; break;
			case 0x73: com_ex();   cycles+= 7; break;
			case 0x74: lsr_ex();   cycles+= 7; break;
			case 0x75: illegal();  cycles+= 2; break;
			case 0x76: ror_ex();   cycles+= 7; break;
			case 0x77: asr_ex();   cycles+= 7; break;
			case 0x78: asl_ex();   cycles+= 7; break;
			case 0x79: rol_ex();   cycles+= 7; break;
			case 0x7a: dec_ex();   cycles+= 7; break;
			case 0x7b: illegal();  cycles+= 2; break;
			case 0x7c: inc_ex();   cycles+= 7; break;
			case 0x7d: tst_ex();   cycles+= 7; break;
			case 0x7e: jmp_ex();   cycles+= 4; break;
			case 0x7f: clr_ex();   cycles+= 7; break;
			case 0x80: suba_im();  cycles+= 2; break;
			case 0x81: cmpa_im();  cycles+= 2; break;
			case 0x82: sbca_im();  cycles+= 2; break;
			case 0x83: subd_im();  cycles+= 4; break;
			case 0x84: anda_im();  cycles+= 2; break;
			case 0x85: bita_im();  cycles+= 2; break;
			case 0x86: lda_im();   cycles+= 2; break;
			case 0x87: sta_im();   cycles+= 2; break;
			case 0x88: eora_im();  cycles+= 2; break;
			case 0x89: adca_im();  cycles+= 2; break;
			case 0x8a: ora_im();   cycles+= 2; break;
			case 0x8b: adda_im();  cycles+= 2; break;
			case 0x8c: cmpx_im();  cycles+= 4; break;
			case 0x8d: bsr();	   cycles+= 7; break;
			case 0x8e: ldx_im();   cycles+= 3; break;
			case 0x8f: stx_im();   cycles+= 2; break;
			case 0x90: suba_di();  cycles+= 4; break;
			case 0x91: cmpa_di();  cycles+= 4; break;
			case 0x92: sbca_di();  cycles+= 4; break;
			case 0x93: subd_di();  cycles+= 6; break;
			case 0x94: anda_di();  cycles+= 4; break;
			case 0x95: bita_di();  cycles+= 4; break;
			case 0x96: lda_di();   cycles+= 4; break;
			case 0x97: sta_di();   cycles+= 4; break;
			case 0x98: eora_di();  cycles+= 4; break;
			case 0x99: adca_di();  cycles+= 4; break;
			case 0x9a: ora_di();   cycles+= 4; break;
			case 0x9b: adda_di();  cycles+= 4; break;
			case 0x9c: cmpx_di();  cycles+= 6; break;
			case 0x9d: jsr_di();   cycles+= 7; break;
			case 0x9e: ldx_di();   cycles+= 5; break;
			case 0x9f: stx_di();   cycles+= 5; break;
			case 0xa0: suba_ix();  cycles+= 4; break;
			case 0xa1: cmpa_ix();  cycles+= 4; break;
			case 0xa2: sbca_ix();  cycles+= 4; break;
			case 0xa3: subd_ix();  cycles+= 6; break;
			case 0xa4: anda_ix();  cycles+= 4; break;
			case 0xa5: bita_ix();  cycles+= 4; break;
			case 0xa6: lda_ix();   cycles+= 4; break;
			case 0xa7: sta_ix();   cycles+= 4; break;
			case 0xa8: eora_ix();  cycles+= 4; break;
			case 0xa9: adca_ix();  cycles+= 4; break;
			case 0xaa: ora_ix();   cycles+= 4; break;
			case 0xab: adda_ix();  cycles+= 4; break;
			case 0xac: cmpx_ix();  cycles+= 6; break;
			case 0xad: jsr_ix();   cycles+= 7; break;
			case 0xae: ldx_ix();   cycles+= 5; break;
			case 0xaf: stx_ix();   cycles+= 5; break;
			case 0xb0: suba_ex();  cycles+= 5; break;
			case 0xb1: cmpa_ex();  cycles+= 5; break;
			case 0xb2: sbca_ex();  cycles+= 5; break;
			case 0xb3: subd_ex();  cycles+= 7; break;
			case 0xb4: anda_ex();  cycles+= 5; break;
			case 0xb5: bita_ex();  cycles+= 5; break;
			case 0xb6: lda_ex();   cycles+= 5; break;
			case 0xb7: sta_ex();   cycles+= 5; break;
			case 0xb8: eora_ex();  cycles+= 5; break;
			case 0xb9: adca_ex();  cycles+= 5; break;
			case 0xba: ora_ex();   cycles+= 5; break;
			case 0xbb: adda_ex();  cycles+= 5; break;
			case 0xbc: cmpx_ex();  cycles+= 7; break;
			case 0xbd: jsr_ex();   cycles+= 8; break;
			case 0xbe: ldx_ex();   cycles+= 6; break;
			case 0xbf: stx_ex();   cycles+= 6; break;
			case 0xc0: subb_im();  cycles+= 2; break;
			case 0xc1: cmpb_im();  cycles+= 2; break;
			case 0xc2: sbcb_im();  cycles+= 2; break;
			case 0xc3: addd_im();  cycles+= 4; break;
			case 0xc4: andb_im();  cycles+= 2; break;
			case 0xc5: bitb_im();  cycles+= 2; break;
			case 0xc6: ldb_im();   cycles+= 2; break;
			case 0xc7: stb_im();   cycles+= 2; break;
			case 0xc8: eorb_im();  cycles+= 2; break;
			case 0xc9: adcb_im();  cycles+= 2; break;
			case 0xca: orb_im();   cycles+= 2; break;
			case 0xcb: addb_im();  cycles+= 2; break;
			case 0xcc: ldd_im();   cycles+= 3; break;
			case 0xcd: std_im();   cycles+= 2; break;
			case 0xce: ldu_im();   cycles+= 3; break;
			case 0xcf: stu_im();   cycles+= 3; break;
			case 0xd0: subb_di();  cycles+= 4; break;
			case 0xd1: cmpb_di();  cycles+= 4; break;
			case 0xd2: sbcb_di();  cycles+= 4; break;
			case 0xd3: addd_di();  cycles+= 6; break;
			case 0xd4: andb_di();  cycles+= 4; break;
			case 0xd5: bitb_di();  cycles+= 4; break;
			case 0xd6: ldb_di();   cycles+= 4; break;
			case 0xd7: stb_di();   cycles+= 4; break;
			case 0xd8: eorb_di();  cycles+= 4; break;
			case 0xd9: adcb_di();  cycles+= 4; break;
			case 0xda: orb_di();   cycles+= 4; break;
			case 0xdb: addb_di();  cycles+= 4; break;
			case 0xdc: ldd_di();   cycles+= 5; break;
			case 0xdd: std_di();   cycles+= 5; break;
			case 0xde: ldu_di();   cycles+= 5; break;
			case 0xdf: stu_di();   cycles+= 5; break;
			case 0xe0: subb_ix();  cycles+= 4; break;
			case 0xe1: cmpb_ix();  cycles+= 4; break;
			case 0xe2: sbcb_ix();  cycles+= 4; break;
			case 0xe3: addd_ix();  cycles+= 6; break;
			case 0xe4: andb_ix();  cycles+= 4; break;
			case 0xe5: bitb_ix();  cycles+= 4; break;
			case 0xe6: ldb_ix();   cycles+= 4; break;
			case 0xe7: stb_ix();   cycles+= 4; break;
			case 0xe8: eorb_ix();  cycles+= 4; break;
			case 0xe9: adcb_ix();  cycles+= 4; break;
			case 0xea: orb_ix();   cycles+= 4; break;
			case 0xeb: addb_ix();  cycles+= 4; break;
			case 0xec: ldd_ix();   cycles+= 5; break;
			case 0xed: std_ix();   cycles+= 5; break;
			case 0xee: ldu_ix();   cycles+= 5; break;
			case 0xef: stu_ix();   cycles+= 5; break;
			case 0xf0: subb_ex();  cycles+= 5; break;
			case 0xf1: cmpb_ex();  cycles+= 5; break;
			case 0xf2: sbcb_ex();  cycles+= 5; break;
			case 0xf3: addd_ex();  cycles+= 7; break;
			case 0xf4: andb_ex();  cycles+= 5; break;
			case 0xf5: bitb_ex();  cycles+= 5; break;
			case 0xf6: ldb_ex();   cycles+= 5; break;
			case 0xf7: stb_ex();   cycles+= 5; break;
			case 0xf8: eorb_ex();  cycles+= 5; break;
			case 0xf9: adcb_ex();  cycles+= 5; break;
			case 0xfa: orb_ex();   cycles+= 5; break;
			case 0xfb: addb_ex();  cycles+= 5; break;
			case 0xfc: ldd_ex();   cycles+= 6; break;
			case 0xfd: std_ex();   cycles+= 6; break;
			case 0xfe: ldu_ex();   cycles+= 6; break;
			case 0xff: stu_ex();   cycles+= 6; break;
			}

	return cycles;
}

void mc6809::fetch_effective_address( void )
{
	UINT8 postbyte = ROP_ARG(PCD);
	PC++;

	switch(postbyte)
	{
	case 0x00: EA=X;												cycles+=1;   break;
	case 0x01: EA=X+1;												cycles+=1;   break;
	case 0x02: EA=X+2;												cycles+=1;   break;
	case 0x03: EA=X+3;												cycles+=1;   break;
	case 0x04: EA=X+4;												cycles+=1;   break;
	case 0x05: EA=X+5;												cycles+=1;   break;
	case 0x06: EA=X+6;												cycles+=1;   break;
	case 0x07: EA=X+7;												cycles+=1;   break;
	case 0x08: EA=X+8;												cycles+=1;   break;
	case 0x09: EA=X+9;												cycles+=1;   break;
	case 0x0a: EA=X+10; 											cycles+=1;   break;
	case 0x0b: EA=X+11; 											cycles+=1;   break;
	case 0x0c: EA=X+12; 											cycles+=1;   break;
	case 0x0d: EA=X+13; 											cycles+=1;   break;
	case 0x0e: EA=X+14; 											cycles+=1;   break;
	case 0x0f: EA=X+15; 											cycles+=1;   break;

	case 0x10: EA=X-16; 											cycles+=1;   break;
	case 0x11: EA=X-15; 											cycles+=1;   break;
	case 0x12: EA=X-14; 											cycles+=1;   break;
	case 0x13: EA=X-13; 											cycles+=1;   break;
	case 0x14: EA=X-12; 											cycles+=1;   break;
	case 0x15: EA=X-11; 											cycles+=1;   break;
	case 0x16: EA=X-10; 											cycles+=1;   break;
	case 0x17: EA=X-9;												cycles+=1;   break;
	case 0x18: EA=X-8;												cycles+=1;   break;
	case 0x19: EA=X-7;												cycles+=1;   break;
	case 0x1a: EA=X-6;												cycles+=1;   break;
	case 0x1b: EA=X-5;												cycles+=1;   break;
	case 0x1c: EA=X-4;												cycles+=1;   break;
	case 0x1d: EA=X-3;												cycles+=1;   break;
	case 0x1e: EA=X-2;												cycles+=1;   break;
	case 0x1f: EA=X-1;												cycles+=1;   break;

	case 0x20: EA=Y;												cycles+=1;   break;
	case 0x21: EA=Y+1;												cycles+=1;   break;
	case 0x22: EA=Y+2;												cycles+=1;   break;
	case 0x23: EA=Y+3;												cycles+=1;   break;
	case 0x24: EA=Y+4;												cycles+=1;   break;
	case 0x25: EA=Y+5;												cycles+=1;   break;
	case 0x26: EA=Y+6;												cycles+=1;   break;
	case 0x27: EA=Y+7;												cycles+=1;   break;
	case 0x28: EA=Y+8;												cycles+=1;   break;
	case 0x29: EA=Y+9;												cycles+=1;   break;
	case 0x2a: EA=Y+10; 											cycles+=1;   break;
	case 0x2b: EA=Y+11; 											cycles+=1;   break;
	case 0x2c: EA=Y+12; 											cycles+=1;   break;
	case 0x2d: EA=Y+13; 											cycles+=1;   break;
	case 0x2e: EA=Y+14; 											cycles+=1;   break;
	case 0x2f: EA=Y+15; 											cycles+=1;   break;

	case 0x30: EA=Y-16; 											cycles+=1;   break;
	case 0x31: EA=Y-15; 											cycles+=1;   break;
	case 0x32: EA=Y-14; 											cycles+=1;   break;
	case 0x33: EA=Y-13; 											cycles+=1;   break;
	case 0x34: EA=Y-12; 											cycles+=1;   break;
	case 0x35: EA=Y-11; 											cycles+=1;   break;
	case 0x36: EA=Y-10; 											cycles+=1;   break;
	case 0x37: EA=Y-9;												cycles+=1;   break;
	case 0x38: EA=Y-8;												cycles+=1;   break;
	case 0x39: EA=Y-7;												cycles+=1;   break;
	case 0x3a: EA=Y-6;												cycles+=1;   break;
	case 0x3b: EA=Y-5;												cycles+=1;   break;
	case 0x3c: EA=Y-4;												cycles+=1;   break;
	case 0x3d: EA=Y-3;												cycles+=1;   break;
	case 0x3e: EA=Y-2;												cycles+=1;   break;
	case 0x3f: EA=Y-1;												cycles+=1;   break;

	case 0x40: EA=U;												cycles+=1;   break;
	case 0x41: EA=U+1;												cycles+=1;   break;
	case 0x42: EA=U+2;												cycles+=1;   break;
	case 0x43: EA=U+3;												cycles+=1;   break;
	case 0x44: EA=U+4;												cycles+=1;   break;
	case 0x45: EA=U+5;												cycles+=1;   break;
	case 0x46: EA=U+6;												cycles+=1;   break;
	case 0x47: EA=U+7;												cycles+=1;   break;
	case 0x48: EA=U+8;												cycles+=1;   break;
	case 0x49: EA=U+9;												cycles+=1;   break;
	case 0x4a: EA=U+10; 											cycles+=1;   break;
	case 0x4b: EA=U+11; 											cycles+=1;   break;
	case 0x4c: EA=U+12; 											cycles+=1;   break;
	case 0x4d: EA=U+13; 											cycles+=1;   break;
	case 0x4e: EA=U+14; 											cycles+=1;   break;
	case 0x4f: EA=U+15; 											cycles+=1;   break;

	case 0x50: EA=U-16; 											cycles+=1;   break;
	case 0x51: EA=U-15; 											cycles+=1;   break;
	case 0x52: EA=U-14; 											cycles+=1;   break;
	case 0x53: EA=U-13; 											cycles+=1;   break;
	case 0x54: EA=U-12; 											cycles+=1;   break;
	case 0x55: EA=U-11; 											cycles+=1;   break;
	case 0x56: EA=U-10; 											cycles+=1;   break;
	case 0x57: EA=U-9;												cycles+=1;   break;
	case 0x58: EA=U-8;												cycles+=1;   break;
	case 0x59: EA=U-7;												cycles+=1;   break;
	case 0x5a: EA=U-6;												cycles+=1;   break;
	case 0x5b: EA=U-5;												cycles+=1;   break;
	case 0x5c: EA=U-4;												cycles+=1;   break;
	case 0x5d: EA=U-3;												cycles+=1;   break;
	case 0x5e: EA=U-2;												cycles+=1;   break;
	case 0x5f: EA=U-1;												cycles+=1;   break;

	case 0x60: EA=S;												cycles+=1;   break;
	case 0x61: EA=S+1;												cycles+=1;   break;
	case 0x62: EA=S+2;												cycles+=1;   break;
	case 0x63: EA=S+3;												cycles+=1;   break;
	case 0x64: EA=S+4;												cycles+=1;   break;
	case 0x65: EA=S+5;												cycles+=1;   break;
	case 0x66: EA=S+6;												cycles+=1;   break;
	case 0x67: EA=S+7;												cycles+=1;   break;
	case 0x68: EA=S+8;												cycles+=1;   break;
	case 0x69: EA=S+9;												cycles+=1;   break;
	case 0x6a: EA=S+10; 											cycles+=1;   break;
	case 0x6b: EA=S+11; 											cycles+=1;   break;
	case 0x6c: EA=S+12; 											cycles+=1;   break;
	case 0x6d: EA=S+13; 											cycles+=1;   break;
	case 0x6e: EA=S+14; 											cycles+=1;   break;
	case 0x6f: EA=S+15; 											cycles+=1;   break;

	case 0x70: EA=S-16; 											cycles+=1;   break;
	case 0x71: EA=S-15; 											cycles+=1;   break;
	case 0x72: EA=S-14; 											cycles+=1;   break;
	case 0x73: EA=S-13; 											cycles+=1;   break;
	case 0x74: EA=S-12; 											cycles+=1;   break;
	case 0x75: EA=S-11; 											cycles+=1;   break;
	case 0x76: EA=S-10; 											cycles+=1;   break;
	case 0x77: EA=S-9;												cycles+=1;   break;
	case 0x78: EA=S-8;												cycles+=1;   break;
	case 0x79: EA=S-7;												cycles+=1;   break;
	case 0x7a: EA=S-6;												cycles+=1;   break;
	case 0x7b: EA=S-5;												cycles+=1;   break;
	case 0x7c: EA=S-4;												cycles+=1;   break;
	case 0x7d: EA=S-3;												cycles+=1;   break;
	case 0x7e: EA=S-2;												cycles+=1;   break;
	case 0x7f: EA=S-1;												cycles+=1;   break;

	case 0x80: EA=X;	X++;										cycles+=2;   break;
	case 0x81: EA=X;	X+=2;										cycles+=3;   break;
	case 0x82: X--; 	EA=X;										cycles+=2;   break;
	case 0x83: X-=2;	EA=X;										cycles+=3;   break;
	case 0x84: EA=X;																   break;
	case 0x85: EA=X+SIGNED(B);										cycles+=1;   break;
	case 0x86: EA=X+SIGNED(A);										cycles+=1;   break;
	case 0x87: EA=0;																   break; /*   ILLEGAL*/
	case 0x88: IMMBYTE(EA); 	EA=X+SIGNED(EA);					cycles+=1;   break; /* this is a hack to make Vectrex work. It should be cycles+=1. Dunno where the cycle was lost :( */
	case 0x89: IMMWORD(ea); 	EA+=X;								cycles+=4;   break;
	case 0x8a: EA=0;																   break; /*   ILLEGAL*/
	case 0x8b: EA=X+D;												cycles+=4;   break;
	case 0x8c: IMMBYTE(EA); 	EA=PC+SIGNED(EA);					cycles+=1;   break;
	case 0x8d: IMMWORD(ea); 	EA+=PC; 							cycles+=5;   break;
	case 0x8e: EA=0;																   break; /*   ILLEGAL*/
	case 0x8f: IMMWORD(ea); 										cycles+=5;   break;

	case 0x90: EA=X;	X++;						EAD=RM16(EAD);	cycles+=5;   break; /* Indirect ,R+ not in my specs */
	case 0x91: EA=X;	X+=2;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0x92: X--; 	EA=X;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0x93: X-=2;	EA=X;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0x94: EA=X;								EAD=RM16(EAD);	cycles+=3;   break;
	case 0x95: EA=X+SIGNED(B);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0x96: EA=X+SIGNED(A);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0x97: EA=0;																   break; /*   ILLEGAL*/
	case 0x98: IMMBYTE(EA); 	EA=X+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0x99: IMMWORD(ea); 	EA+=X;				EAD=RM16(EAD);	cycles+=7;   break;
	case 0x9a: EA=0;																   break; /*   ILLEGAL*/
	case 0x9b: EA=X+D;								EAD=RM16(EAD);	cycles+=7;   break;
	case 0x9c: IMMBYTE(EA); 	EA=PC+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0x9d: IMMWORD(ea); 	EA+=PC; 			EAD=RM16(EAD);	cycles+=8;   break;
	case 0x9e: EA=0;																   break; /*   ILLEGAL*/
	case 0x9f: IMMWORD(ea); 						EAD=RM16(EAD);	cycles+=8;   break;

	case 0xa0: EA=Y;	Y++;										cycles+=2;   break;
	case 0xa1: EA=Y;	Y+=2;										cycles+=3;   break;
	case 0xa2: Y--; 	EA=Y;										cycles+=2;   break;
	case 0xa3: Y-=2;	EA=Y;										cycles+=3;   break;
	case 0xa4: EA=Y;																   break;
	case 0xa5: EA=Y+SIGNED(B);										cycles+=1;   break;
	case 0xa6: EA=Y+SIGNED(A);										cycles+=1;   break;
	case 0xa7: EA=0;																   break; /*   ILLEGAL*/
	case 0xa8: IMMBYTE(EA); 	EA=Y+SIGNED(EA);					cycles+=1;   break;
	case 0xa9: IMMWORD(ea); 	EA+=Y;								cycles+=4;   break;
	case 0xaa: EA=0;																   break; /*   ILLEGAL*/
	case 0xab: EA=Y+D;												cycles+=4;   break;
	case 0xac: IMMBYTE(EA); 	EA=PC+SIGNED(EA);					cycles+=1;   break;
	case 0xad: IMMWORD(ea); 	EA+=PC; 							cycles+=5;   break;
	case 0xae: EA=0;																   break; /*   ILLEGAL*/
	case 0xaf: IMMWORD(ea); 										cycles+=5;   break;

	case 0xb0: EA=Y;	Y++;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0xb1: EA=Y;	Y+=2;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0xb2: Y--; 	EA=Y;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0xb3: Y-=2;	EA=Y;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0xb4: EA=Y;								EAD=RM16(EAD);	cycles+=3;   break;
	case 0xb5: EA=Y+SIGNED(B);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0xb6: EA=Y+SIGNED(A);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0xb7: EA=0;																   break; /*   ILLEGAL*/
	case 0xb8: IMMBYTE(EA); 	EA=Y+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0xb9: IMMWORD(ea); 	EA+=Y;				EAD=RM16(EAD);	cycles+=7;   break;
	case 0xba: EA=0;																   break; /*   ILLEGAL*/
	case 0xbb: EA=Y+D;								EAD=RM16(EAD);	cycles+=7;   break;
	case 0xbc: IMMBYTE(EA); 	EA=PC+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0xbd: IMMWORD(ea); 	EA+=PC; 			EAD=RM16(EAD);	cycles+=8;   break;
	case 0xbe: EA=0;																   break; /*   ILLEGAL*/
	case 0xbf: IMMWORD(ea); 						EAD=RM16(EAD);	cycles+=8;   break;

	case 0xc0: EA=U;			U++;								cycles+=2;   break;
	case 0xc1: EA=U;			U+=2;								cycles+=3;   break;
	case 0xc2: U--; 			EA=U;								cycles+=2;   break;
	case 0xc3: U-=2;			EA=U;								cycles+=3;   break;
	case 0xc4: EA=U;																   break;
	case 0xc5: EA=U+SIGNED(B);										cycles+=1;   break;
	case 0xc6: EA=U+SIGNED(A);										cycles+=1;   break;
	case 0xc7: EA=0;																   break; /*ILLEGAL*/
	case 0xc8: IMMBYTE(EA); 	EA=U+SIGNED(EA);					cycles+=1;   break;
	case 0xc9: IMMWORD(ea); 	EA+=U;								cycles+=4;   break;
	case 0xca: EA=0;																   break; /*ILLEGAL*/
	case 0xcb: EA=U+D;												cycles+=4;   break;
	case 0xcc: IMMBYTE(EA); 	EA=PC+SIGNED(EA);					cycles+=1;   break;
	case 0xcd: IMMWORD(ea); 	EA+=PC; 							cycles+=5;   break;
	case 0xce: EA=0;																   break; /*ILLEGAL*/
	case 0xcf: IMMWORD(ea); 										cycles+=5;   break;

	case 0xd0: EA=U;	U++;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0xd1: EA=U;	U+=2;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0xd2: U--; 	EA=U;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0xd3: U-=2;	EA=U;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0xd4: EA=U;								EAD=RM16(EAD);	cycles+=3;   break;
	case 0xd5: EA=U+SIGNED(B);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0xd6: EA=U+SIGNED(A);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0xd7: EA=0;																   break; /*ILLEGAL*/
	case 0xd8: IMMBYTE(EA); 	EA=U+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0xd9: IMMWORD(ea); 	EA+=U;				EAD=RM16(EAD);	cycles+=7;   break;
	case 0xda: EA=0;																   break; /*ILLEGAL*/
	case 0xdb: EA=U+D;								EAD=RM16(EAD);	cycles+=7;   break;
	case 0xdc: IMMBYTE(EA); 	EA=PC+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0xdd: IMMWORD(ea); 	EA+=PC; 			EAD=RM16(EAD);	cycles+=8;   break;
	case 0xde: EA=0;																   break; /*ILLEGAL*/
	case 0xdf: IMMWORD(ea); 						EAD=RM16(EAD);	cycles+=8;   break;

	case 0xe0: EA=S;	S++;										cycles+=2;   break;
	case 0xe1: EA=S;	S+=2;										cycles+=3;   break;
	case 0xe2: S--; 	EA=S;										cycles+=2;   break;
	case 0xe3: S-=2;	EA=S;										cycles+=3;   break;
	case 0xe4: EA=S;																   break;
	case 0xe5: EA=S+SIGNED(B);										cycles+=1;   break;
	case 0xe6: EA=S+SIGNED(A);										cycles+=1;   break;
	case 0xe7: EA=0;																   break; /*ILLEGAL*/
	case 0xe8: IMMBYTE(EA); 	EA=S+SIGNED(EA);					cycles+=1;   break;
	case 0xe9: IMMWORD(ea); 	EA+=S;								cycles+=4;   break;
	case 0xea: EA=0;																   break; /*ILLEGAL*/
	case 0xeb: EA=S+D;												cycles+=4;   break;
	case 0xec: IMMBYTE(EA); 	EA=PC+SIGNED(EA);					cycles+=1;   break;
	case 0xed: IMMWORD(ea); 	EA+=PC; 							cycles+=5;   break;
	case 0xee: EA=0;																   break;  /*ILLEGAL*/
	case 0xef: IMMWORD(ea); 										cycles+=5;   break;

	case 0xf0: EA=S;	S++;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0xf1: EA=S;	S+=2;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0xf2: S--; 	EA=S;						EAD=RM16(EAD);	cycles+=5;   break;
	case 0xf3: S-=2;	EA=S;						EAD=RM16(EAD);	cycles+=6;   break;
	case 0xf4: EA=S;								EAD=RM16(EAD);	cycles+=3;   break;
	case 0xf5: EA=S+SIGNED(B);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0xf6: EA=S+SIGNED(A);						EAD=RM16(EAD);	cycles+=4;   break;
	case 0xf7: EA=0;																   break; /*ILLEGAL*/
	case 0xf8: IMMBYTE(EA); 	EA=S+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0xf9: IMMWORD(ea); 	EA+=S;				EAD=RM16(EAD);	cycles+=7;   break;
	case 0xfa: EA=0;																   break; /*ILLEGAL*/
	case 0xfb: EA=S+D;								EAD=RM16(EAD);	cycles+=7;   break;
	case 0xfc: IMMBYTE(EA); 	EA=PC+SIGNED(EA);	EAD=RM16(EAD);	cycles+=4;   break;
	case 0xfd: IMMWORD(ea); 	EA+=PC; 			EAD=RM16(EAD);	cycles+=8;   break;
	case 0xfe: EA=0;																   break; /*ILLEGAL*/
	case 0xff: IMMWORD(ea); 						EAD=RM16(EAD);	cycles+=8;   break;
	}
}

UINT32 mc6809::RM16( UINT32 Addr )
{
	UINT32 result = RM(Addr) << 8;
	return result | RM((Addr+1)&0xffff);
}

void mc6809::WM16( UINT32 Addr, PAIR *p )
{
	WM( Addr, p->b.h );
	WM( (Addr+1)&0xffff, p->b.l );
}

void mc6809::interrupt(void)
{
	if( irq_state[M6809_IRQ_LINE] != M6809_CLEAR_LINE ||
		irq_state[M6809_FIRQ_LINE] != M6809_CLEAR_LINE ||
		irq_state[M6809_NMI_LINE] != M6809_CLEAR_LINE )
		int_state &= ~M6809_SYNC; /* clear SYNC flag */
	if( irq_state[M6809_NMI_LINE]!=M6809_CLEAR_LINE )
	{
		if( int_state & M6809_CWAI )
		{
			int_state &= ~M6809_CWAI;  /* clear CWAI flag */
			/*extra_*/cycles += 7;		 /* subtract +7 cycles */
		}
		else
		{
			CC |= CC_E; 				/* save entire state */
			PUSHWORD(pPC);
			PUSHWORD(pU);
			PUSHWORD(pY);
			PUSHWORD(pX);
			PUSHBYTE(DP);
			PUSHBYTE(B);
			PUSHBYTE(A);
			PUSHBYTE(CC);
			/*extra_*/cycles += 19;	 /* subtract +19 cycles */
		}
		CC |= CC_IF |CC_II;					/* inhibit FIRQ and IRQ */
		PCD=RM16(0xfffc);
		CHANGE_PC;
    }
    else
	if( irq_state[M6809_FIRQ_LINE]!=M6809_CLEAR_LINE && !(CC & CC_IF) )
	{
		/* fast IRQ */
		/* HJB 990225: state already saved by CWAI? */
		if( int_state & M6809_CWAI )
		{
			int_state &= ~M6809_CWAI;  /* clear CWAI */
			/*extra_*/cycles += 7;		 /* subtract +7 cycles */
        }
		else
		{
			CC &= ~CC_E;				/* save 'short' state */
			PUSHWORD(pPC);
			PUSHBYTE(CC);
			/*extra_*/cycles += 10;	/* subtract +10 cycles */
		}
		CC |= CC_IF | CC_II;			/* inhibit FIRQ and IRQ */
		PCD=RM16(0xfff6);
		CHANGE_PC;
	}
	else
	if( irq_state[M6809_IRQ_LINE]!=M6809_CLEAR_LINE && !(CC & CC_II) )
	{
		/* standard IRQ */
		/* HJB 990225: state already saved by CWAI? */
		if( int_state & M6809_CWAI )
		{
			int_state &= ~M6809_CWAI;  /* clear CWAI flag */
			/*extra_*/cycles += 7;		 /* subtract +7 cycles */
		}
		else
		{
			CC |= CC_E; 				/* save entire state */
			PUSHWORD(pPC);
			PUSHWORD(pU);
			PUSHWORD(pY);
			PUSHWORD(pX);
			PUSHBYTE(DP);
			PUSHBYTE(B);
			PUSHBYTE(A);
			PUSHBYTE(CC);
			/*extra_*/cycles += 19;	 /* subtract +19 cycles */
		}
		CC |= CC_II;					/* inhibit IRQ */
		PCD=RM16(0xfff8);
		CHANGE_PC;
	}
}

void __fastcall mc6809::SaveToStream(Byte *&ptr)
{
  savestate(ptr,pc);
  savestate(ptr,ppc);
  savestate(ptr,dd);
  savestate(ptr,dp);
  savestate(ptr,u);
  savestate(ptr,s);
  savestate(ptr,x);
  savestate(ptr,y);
  savestate(ptr,ea);
  savestate(ptr,cc);
  savestate(ptr,ireg);
  savestate(ptr,irq_state);
//  savestate(ptr,extra_cycles, 4);
  savestate(ptr,int_state);
//  savestate(ptr,nmi_state, 1);
//  savestate(ptr,cycles, 4);
}

void __fastcall mc6809::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,pc);
  readstate(ptr,ppc);
  readstate(ptr,dd);
  readstate(ptr,dp);
  readstate(ptr,u);
  readstate(ptr,s);
  readstate(ptr,x);
  readstate(ptr,y);
  readstate(ptr,ea);
  readstate(ptr,cc);
  readstate(ptr,ireg);
  readstate(ptr,irq_state);
//  readstate(ptr,extra_cycles, 4);
  readstate(ptr,int_state);
//  readstate(ptr,nmi_state, 1);
//  readstate(ptr,cycles, 4);
}

