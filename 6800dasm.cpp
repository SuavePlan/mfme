/*
 *   A quick-hack 6803/6808 disassembler
 *
 *   Note: this is not the good and proper way to disassemble anything, but it works
 *
 *   I'm afraid to put my name on it, but I feel obligated:
 *   This code written by Aaron Giles (agiles@sirius.com) for the MAME project
 *
 * History:
 * 990327 HJB
 * Added code to support the debugger set_ea_info function
 * 990314 HJB
 * The disassembler knows about valid opcodes for M6800/1/2/3/8 and HD63701.
 * 990302 HJB
 * Changed the string array into a table of opcode names (or tokens) and
 * argument types. This second try should give somewhat better results.
 * Named the undocumented HD63701YO opcodes $12 and $13 'asx1' and 'asx2',
 * since 'add contents of stack to x register' is what they do.
 *
 */

#include <string.h>
#include <stdio.h>
#include "mc6800.h"

enum addr_mode {
	inh,	/* inherent */
	rel,	/* relative */
	imm,	/* immediate (byte or word) */
	dir,	/* direct address */
	imd,	/* HD63701YO: immediate, direct address */
	ext,	/* extended address */
	idx,	/* x + byte offset */
	imx,	/* HD63701YO: immediate, x + byte offset */
	sx1 	/* HD63701YO: undocumented opcodes: byte from (s+1) */
};

enum op_names {
	aba=0,	abx,	adca,	adcb,	adda,	addb,	addd,	aim,
	anda,	andb,	asl,	asla,	aslb,	asld,	asr,	asra,
	asrb,	bcc,	bcs,	beq,	bge,	bgt,	bhi,	bita,
	bitb,	ble,	bls,	blt,	bmi,	bne,	bpl,	bra,
	brn,	bsr,	bvc,	bvs,	cba,	clc,	cli,	clr,
	clra,	clrb,	clv,	cmpa,	cmpb,	cmpx,	com,	coma,
	comb,	daa,	dec,	deca,	decb,	des,	dex,	eim,
	eora,	eorb,	ill,	inc,	inca,	incb,	ins,	inx,
	jmp,	jsr,	lda,	ldb,	ldd,	lds,	ldx,	lsr,
	lsra,	lsrb,	lsrd,	mul,	neg,	nega,	negb,	nop,
	oim,	ora,	orb,	psha,	pshb,	pshx,	pula,	pulb,
	pulx,	rol,	rola,	rolb,	ror,	rora,	rorb,	rti,
	rts,	sba,	sbca,	sbcb,	sec,	sev,	sta,	stb,
	std1,	sei,	sts,	stx,	suba,	subb,	subd,	swi,
	wai,	tab,	tap,	tba,	tim,	tpa,	tst,	tsta,
	tstb,	tsx,	txs,	asx1,	asx2,	xgdx,	addx, adcx
};

static const char *op_name_str[] = {
	"ABA",   "ABX",   "ADCA",  "ADCB",  "ADDA",  "ADDB",  "ADDD",  "AIM",
	"ANDA",  "ANDB",  "ASL",   "ASLA",  "ASLB",  "ASLD",  "ASR",   "ASRA",
	"ASRB",  "BCC",   "BCS",   "BEQ",   "BGE",   "BGT",   "BHI",   "BITA",
	"BITB",  "BLE",   "BLS",   "BLT",   "BMI",   "BNE",   "BPL",   "BRA",
	"BRN",   "BSR",   "BVC",   "BVS",   "CBA",   "CLC",   "CLI",   "CLR",
	"CLRA",  "CLRB",  "CLV",   "CMPA",  "CMPB",  "CMPX",  "COM",   "COMA",
	"COMB",  "DAA",   "DEC",   "DECA",  "DECB",  "DES",   "DEX",   "EIM",
	"EORA",  "EORB",  "??",    "INC",   "INCA",  "INCB",  "INS",   "INX",
	"JMP",   "JSR",   "LDA",   "LDB",   "LDD",   "LDS",   "LDX",   "LSR",
	"LSRA",  "LSRB",  "LSRD",  "MUL",   "NEG",   "NEGA",  "NEGB",  "NOP",
	"OIM",   "ORA",   "ORB",   "PSHA",  "PSHB",  "PSHX",  "PULA",  "PULB",
	"PULX",  "ROL",   "ROLA",  "ROLB",  "ROR",   "RORA",  "RORB",  "RTI",
	"RTS",   "SBA",   "SBCA",  "SBCB",  "SEC",   "SEV",   "STA",   "STB",
	"STD",   "SEI",   "STS",   "STX",   "SUBA",  "SUBB",  "SUBD",  "SWI",
	"WAI",   "TAB",   "TAP",   "TBA",   "TIM",   "TPA",   "TST",   "TSTA",
	"TSTB",  "TSX",   "TXS",   "ASX1",  "ASX2",  "XGDX",  "ADDX",  "ADCX"
};

enum {
  EA_NONE,        /* no EA mode */
  EA_VALUE,       /* immediate value */
  EA_ABS_PC,      /* change PC absolute (JMP or CALL type opcodes) */
  EA_REL_PC,      /* change PC relative (BRA or JR type opcodes) */
	EA_ZPG_RD,		  /* read zero page memory */
  EA_ZPG_WR,		  /* write zero page memory */
	EA_ZPG_RDWR,	  /* read then write zero page memory */
  EA_MEM_RD,      /* read memory */
  EA_MEM_WR,      /* write memory */
  EA_MEM_RDWR,    /* read then write memory */
  EA_PORT_RD,     /* read i/o port */
  EA_PORT_WR,     /* write i/o port */
  EA_COUNT
};
/* Size of the data element accessed (or the immediate value) */
enum {
  EA_DEFAULT,
  EA_INT8,
  EA_UINT8,
  EA_INT16,
  EA_UINT16,
  EA_INT32,
  EA_UINT32,
  EA_SIZE
};
enum {
  EA_DST,
  EA_SRC
};


/* Some really short names for the EA access modes */
#define VAL EA_VALUE
#define JMP EA_ABS_PC
#define BRA EA_REL_PC
#define MRD EA_MEM_RD
#define MWR EA_MEM_WR
#define MRW EA_MEM_RDWR
#define ZRD EA_ZPG_RD
#define ZWR EA_ZPG_WR
#define ZRW EA_ZPG_RDWR

/* and the data element sizes */
#define B	EA_UINT8
#define W	EA_UINT16

/*
 * This table defines the opcodes:
 * byte meaning
 * 0	token (menmonic)
 * 1	addressing mode
 * 2	EA access mode
 * 3	EA access size
 * 4	invalid opcode for 1:6800/6802/6808, 2:6801/6803, 4:HD63701
 */

static UINT8 table[0x102][5] = {
	{ill, inh,0  ,0,7},{nop, inh,0	,0,0},{ill, inh,0  ,0,7},{ill, inh,0  ,0,7},/* 00 */
	{lsrd,inh,0  ,0,1},{asld,inh,0	,0,1},{tap, inh,0  ,0,0},{tpa, inh,0  ,0,0},
	{inx, inh,0  ,0,0},{dex, inh,0	,0,0},{clv, inh,0  ,0,0},{sev, inh,0  ,0,0},
	{clc, inh,0  ,0,0},{sec, inh,0	,0,0},{cli, inh,0  ,0,0},{sei, inh,0  ,0,0},
	{sba, inh,0  ,0,0},{cba, inh,0	,0,0},{asx1,sx1,MRD,B,0},{asx2,sx1,MRD,B,0},/* 10 */
	{ill, inh,0  ,0,7},{ill, inh,0	,0,7},{tab, inh,0  ,0,0},{tba, inh,0  ,0,0},
	{xgdx,inh,0  ,0,3},{daa, inh,0	,0,0},{ill, inh,0  ,0,7},{aba, inh,0  ,0,0},
	{ill, inh,0  ,0,7},{ill, inh,0	,0,7},{ill, inh,0  ,0,7},{ill, inh,0  ,0,7},
	{bra, rel,BRA,0,0},{brn, rel,BRA,0,0},{bhi, rel,BRA,0,0},{bls, rel,BRA,0,0},/* 20 */
	{bcc, rel,BRA,0,0},{bcs, rel,BRA,0,0},{bne, rel,BRA,0,0},{beq, rel,BRA,0,0},
	{bvc, rel,BRA,0,0},{bvs, rel,BRA,0,0},{bpl, rel,BRA,0,0},{bmi, rel,BRA,0,0},
	{bge, rel,BRA,0,0},{blt, rel,BRA,0,0},{bgt, rel,BRA,0,0},{ble, rel,BRA,0,0},
	{tsx, inh,0  ,0,0},{ins, inh,0	,0,0},{pula,inh,0  ,0,0},{pulb,inh,0  ,0,0},/* 30 */
	{des, inh,0  ,0,0},{txs, inh,0	,0,0},{psha,inh,0  ,0,0},{pshb,inh,0  ,0,0},
	{pulx,inh,0  ,0,1},{rts, inh,0	,0,0},{abx, inh,0  ,0,1},{rti, inh,0  ,0,0},
	{pshx,inh,0  ,0,1},{mul, inh,0	,0,1},{wai, inh,0  ,0,0},{swi, inh,0  ,0,0},
	{nega,inh,0  ,0,0},{ill, inh,0	,0,7},{ill, inh,0  ,0,7},{coma,inh,0  ,0,0},/* 40 */
	{lsra,inh,0  ,0,0},{ill, inh,0	,0,7},{rora,inh,0  ,0,0},{asra,inh,0  ,0,0},
	{asla,inh,0  ,0,0},{rola,inh,0	,0,0},{deca,inh,0  ,0,0},{ill, inh,0  ,0,7},
	{inca,inh,0  ,0,0},{tsta,inh,0	,0,0},{ill, inh,0  ,0,7},{clra,inh,0  ,0,0},
	{negb,inh,0  ,0,0},{ill, inh,0	,0,7},{ill, inh,0  ,0,7},{comb,inh,0  ,0,0},/* 50 */
	{lsrb,inh,0  ,0,0},{ill, inh,0	,0,7},{rorb,inh,0  ,0,0},{asrb,inh,0  ,0,0},
	{aslb,inh,0  ,0,0},{rolb,inh,0	,0,0},{decb,inh,0  ,0,0},{ill, inh,0  ,0,7},
	{incb,inh,0  ,0,0},{tstb,inh,0	,0,0},{ill, inh,0  ,0,7},{clrb,inh,0  ,0,0},
	{neg, idx,MRW,B,0},{aim, imx,MRW,B,3},{oim, imx,MRW,B,3},{com, idx,MRW,B,0},/* 60 */
	{lsr, idx,MRW,B,0},{eim, imx,MRW,B,3},{ror, idx,MRW,B,0},{asr, idx,MRW,B,0},
	{asl, idx,MRW,B,0},{rol, idx,MRW,B,0},{dec, idx,MRW,B,0},{tim, imx,MRD,B,3},
	{inc, idx,MRW,B,0},{tst, idx,MRD,B,0},{jmp, idx,JMP,0,0},{clr, idx,MWR,B,0},
	{neg, ext,MRW,B,0},{aim, imd,ZRW,B,3},{oim, imd,ZRW,B,3},{com, ext,MRW,B,0},/* 70 */
	{lsr, ext,MRW,B,0},{eim, imd,ZRW,B,3},{ror, ext,MRW,B,0},{asr, ext,MRW,B,0},
	{asl, ext,MRW,B,0},{rol, ext,MRW,B,0},{dec, ext,MRW,B,0},{tim, imd,ZRD,B,3},
	{inc, ext,MRW,B,0},{tst, ext,MRD,B,0},{jmp, ext,JMP,0,0},{clr, ext,MWR,B,0},
	{suba,imm,VAL,B,0},{cmpa,imm,VAL,B,0},{sbca,imm,VAL,B,0},{subd,imm,VAL,W,1},/* 80 */
	{anda,imm,VAL,B,0},{bita,imm,VAL,B,0},{lda, imm,VAL,B,0},{sta, imm,VAL,B,0},
	{eora,imm,VAL,B,0},{adca,imm,VAL,B,0},{ora, imm,VAL,B,0},{adda,imm,VAL,B,0},
	{cmpx,imm,VAL,W,0},{bsr, rel,BRA,0,0},{lds, imm,VAL,W,0},{sts, imm,VAL,W,0},
	{suba,dir,ZRD,B,0},{cmpa,dir,ZRD,B,0},{sbca,dir,ZRD,B,0},{subd,dir,ZRD,W,1},/* 90 */
	{anda,dir,ZRD,B,0},{bita,dir,ZRD,B,0},{lda, dir,ZRD,B,0},{sta, dir,ZWR,B,0},
	{eora,dir,ZRD,B,0},{adca,dir,ZRD,B,0},{ora, dir,ZRD,B,0},{adda,dir,ZRD,B,0},
	{cmpx,dir,ZRD,W,0},{jsr, dir,JMP,0,0},{lds, dir,ZRD,W,0},{sts, dir,ZWR,W,0},
	{suba,idx,MRD,B,0},{cmpa,idx,MRD,B,0},{sbca,idx,MRD,B,0},{subd,idx,MRD,W,1},/* a0 */
	{anda,idx,MRD,B,0},{bita,idx,MRD,B,0},{lda, idx,MRD,B,0},{sta, idx,MWR,B,0},
	{eora,idx,MRD,B,0},{adca,idx,MRD,B,0},{ora, idx,MRD,B,0},{adda,idx,MRD,B,0},
	{cmpx,idx,MRD,W,0},{jsr, idx,JMP,0,0},{lds, idx,MRD,W,0},{sts, idx,MWR,W,0},
	{suba,ext,MRD,B,0},{cmpa,ext,MRD,B,0},{sbca,ext,MRD,B,0},{subd,ext,MRD,W,1},/* b0 */
	{anda,ext,MRD,B,0},{bita,ext,MRD,B,0},{lda, ext,MRD,B,0},{sta, ext,MWR,B,0},
	{eora,ext,MRD,B,0},{adca,ext,MRD,B,0},{ora, ext,MRD,B,0},{adda,ext,MRD,B,0},
	{cmpx,ext,MRD,W,0},{jsr, ext,JMP,0,0},{lds, ext,MRD,W,0},{sts, ext,MWR,W,0},
	{subb,imm,VAL,B,0},{cmpb,imm,VAL,B,0},{sbcb,imm,VAL,B,0},{addd,imm,VAL,W,1},/* c0 */
	{andb,imm,VAL,B,0},{bitb,imm,VAL,B,0},{ldb, imm,VAL,B,0},{stb, imm,VAL,B,0},
	{eorb,imm,VAL,B,0},{adcb,imm,VAL,B,0},{orb, imm,VAL,B,0},{addb,imm,VAL,B,0},
	{ldd, imm,VAL,W,1},{std1, imm,VAL,W,1},{ldx, imm,VAL,W,0},{stx, imm,VAL,W,0},
	{subb,dir,ZRD,B,0},{cmpb,dir,ZRD,B,0},{sbcb,dir,ZRD,B,0},{addd,dir,ZRD,W,1},/* d0 */
	{andb,dir,ZRD,B,0},{bitb,dir,ZRD,B,0},{ldb, dir,ZRD,B,0},{stb, dir,ZWR,B,0},
	{eorb,dir,ZRD,B,0},{adcb,dir,ZRD,B,0},{orb, dir,ZRD,B,0},{addb,dir,ZRD,B,0},
	{ldd, dir,ZRD,W,1},{std1, dir,ZRD,W,1},{ldx, dir,ZRD,W,0},{stx, dir,ZWR,W,0},
	{subb,idx,MRD,B,0},{cmpb,idx,MRD,B,0},{sbcb,idx,MRD,B,0},{addd,idx,MRD,W,1},/* e0 */
	{andb,idx,MRD,B,0},{bitb,idx,MRD,B,0},{ldb, idx,MRD,B,0},{stb, idx,MWR,B,0},
	{eorb,idx,MRD,B,0},{adcb,idx,MRD,B,0},{orb, idx,MRD,B,0},{addb,idx,MRD,B,0},
	{ldd, idx,MRD,W,1},{std1, idx,MRD,W,1},{ldx, idx,MRD,W,0},{stx, idx,MWR,W,0},
	{subb,ext,MRD,B,0},{cmpb,ext,MRD,B,0},{sbcb,ext,MRD,B,0},{addd,ext,MRD,W,1},/* f0 */
	{andb,ext,MRD,B,0},{bitb,ext,MRD,B,0},{ldb, ext,MRD,B,0},{stb, ext,MWR,B,0},
	{eorb,ext,MRD,B,0},{adcb,ext,MRD,B,0},{orb, ext,MRD,B,0},{addb,ext,MRD,B,0},
	{ldd, ext,MRD,W,1},{std1, ext,MRD,W,1},{ldx, ext,MRD,W,0},{stx, ext,MWR,W,0},

	/* extra instruction $fc for NSC-8105 */
	{addx,ext,MRD,W,0},
	/* extra instruction $ec for NSC-8105 */
	{adcx,imm,MRD,B,0}
};

/* some macros to keep things short */
#define OP      read(pc)
#define ARG1    read(pc+1)
#define ARG2    read(pc+2)
#define ARGW	(read(pc+1)<<8) + read(pc+2)


int mc6800::dasm(int pc,char *buf)
{
	int invalid_mask;
	int code = read(pc);
	UINT8 opcode, args, access, size, invalid;
  char buff2[30];
  char * buf2 = buff2;
  int nbr_bytes;
  int i;

  if (m6800_subtype == M6800_TYPE_6303)
    invalid_mask = 0;
  else
    invalid_mask = 1;

	opcode = table[code][0];
	args = table[code][1];
	access = table[code][2];
	size = table[code][3];
	invalid = table[code][4];

	if ( invalid & invalid_mask )	/* invalid for this cpu type ? */
	{
        buf += sprintf(buf, "%04X    ??", pc);
		return 1;
	}

	buf += sprintf(buf, "%04X  ", pc);
	buf2 += sprintf(buf2, "%-5s", op_name_str[opcode]);

	switch( args )
	{
		case rel:  /* relative */
			sprintf (buf2, "$%04X", pc + (INT8)ARG1 + 2);
			nbr_bytes = 2;
      break;
		case imm:  /* immediate (byte or word) */
			if( size == B )
			{
				sprintf (buf2, "#%02X", ARG1);
                nbr_bytes = 2;
			}
      else
      {
		  	sprintf (buf2, "#%04X", ARGW);
		   	nbr_bytes = 3;
      }
      break;
		case idx:  /* indexed + byte offset */
			sprintf (buf2, "[x+%02X]", ARG1 );
      nbr_bytes = 2;
      break;
		case imx:  /* immediate, indexed + byte offset */
			sprintf (buf2, "#%02X,[x+$%02X]", ARG1, ARG2 );
			nbr_bytes = 3;
      break;
		case dir:  /* direct address */
			sprintf (buf2, "$%04X", ARG1 );
			nbr_bytes = 2;
      break;
		case imd:  /* immediate, direct address */
			sprintf (buf2, "#%02X,%02X", ARG1, ARG2);
      nbr_bytes = 3;
      break;
		case ext:  /* extended address */
			sprintf (buf2, "$%04X", ARGW);
			nbr_bytes = 3;
      break;
		case sx1:  /* byte from address (s + 1) */
      sprintf (buf2, "[s+1]");
      nbr_bytes = 1;
      break;
    default:
			nbr_bytes = 1;
      break;
	}

  for ( i = 0; i < nbr_bytes; i++ )
    buf += sprintf(buf, "%02X ", read(pc+i));

  for ( ; i < 4; i++ )
    buf += sprintf(buf, "   ");

  buf += sprintf(buf, "%s",buff2);

  return nbr_bytes;
}

