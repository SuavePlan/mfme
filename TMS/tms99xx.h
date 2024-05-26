//---------------------------------------------------------------------------

#ifndef tms99xxH
#define tms99xxH

#include "machdep.h"

/* tms9900 ST register bits. */

/* These bits are set by every compare, move and arithmetic or logical operation : */
/* (Well, COC, CZC and TB only set the E bit, but these are kind of exceptions.) */
#define ST_LGT 0x8000 /* Logical Greater Than (strictly) */
#define ST_AGT 0x4000 /* Arithmetical Greater Than (strictly) */
#define ST_EQ  0x2000 /* Equal */

/* These bits are set by arithmetic operations, when it makes sense to update them. */
#define ST_C   0x1000 /* Carry */
#define ST_OV  0x0800 /* OVerflow (overflow with operations on signed integers, */
                      /* and when the result of a 32bits:16bits division cannot fit in a 16-bit word.) */

/* This bit is set by move and arithmetic operations WHEN THEY USE BYTE OPERANDS. */
#define ST_OP  0x0400 /* Odd Parity */

/* This bit is set by the XOP instruction. */
#define ST_X   0x0200 /* Xop */

/* This bit is set in TMS9995 and later chips to generate a level-2 interrupt when the Overflow
status bit is set */
#define ST_OV_EN 0x0020 /* OVerflow interrupt ENable */

/* On models before TMS9995 (TMS9989 ?), unused ST bits are always forced to 0, so we define
a ST_MASK */
#define ST_MASK 0xFE0F

/* Interrupt mask */
#define IMASK       (STATUS & 0x0F)

/* Offsets for registers. */
#define R0   0
#define R1   2
#define R2   4
#define R3   6
#define R4   8
#define R5  10
#define R6  12
#define R7  14
#define R8  16
#define R9  18
#define R10 20
#define R11 22
#define R12 24
#define R13 26
#define R14 28
#define R15 30

/*************************************
 *
 *	Interrupt line constants
 *
 *************************************/

enum
{
	/* line states */
	CLEAR_LINE = 0,				/* clear (a fired, held or pulsed) line */
	ASSERT_LINE,				/* assert an interrupt immediately */
	HOLD_LINE,					/* hold interrupt line until acknowledged */
	PULSE_LINE,					/* pulse interrupt line for one instruction */

	/* internal flags (not for use by drivers!) */
	INTERNAL_CLEAR_LINE = 100 + CLEAR_LINE,
	INTERNAL_ASSERT_LINE = 100 + ASSERT_LINE,

	/* interrupt parameters */
	MAX_IRQ_LINES =	16,			/* maximum number of IRQ lines per CPU */
	IRQ_LINE_NMI = 127			/* IRQ line for NMIs */
};

enum {
	TMS9900_PC=1, TMS9900_WP, TMS9900_STATUS, TMS9900_IR,
	TMS9900_R0, TMS9900_R1, TMS9900_R2, TMS9900_R3,
	TMS9900_R4, TMS9900_R5, TMS9900_R6, TMS9900_R7,
	TMS9900_R8, TMS9900_R9, TMS9900_R10, TMS9900_R11,
	TMS9900_R12, TMS9900_R13, TMS9900_R14, TMS9900_R15
};

enum opcodes {
	_a=0,	_ab,	_c,		_cb,	_s,		_sb,	_soc,	_socb,	_szc,	_szcb,
	_mov,	_movb,	_coc,	_czc,	_xor,	_mpy,	_div,	_xop,	_b,		_bl,
	_blwp,	_clr,	_seto,	_inv,	_neg,	_abs,	_swpb,	_inc,	_inct,	_dec,
	_dect,	_x,		_ldcr,	_stcr,	_sbo,	_sbz,	_tb,	_jeq,	_jgt,	_jh,
	_jhe,	_jl,	_jle,	_jlt,	_jmp,	_jnc,	_jne,	_jno,	_joc,	_jop,
	_sla,	_sra,	_src,	_srl,	_ai,	_andi,	_ci,	_li,	_ori,	_lwpi,
	_limi,	_stst,	_stwp,	_rtwp,	_idle,	_rset,	_ckof,	_ckon,	_lrex,	_ill
};


// structure with the parameters tms9995_reset wants.
typedef struct tms9995reset_param
{
	/*
      auto_wait_state :
      a non-zero value makes tms9995 generate a wait state automatically on each
	    memory access
  */
	int auto_wait_state;
} tms9995reset_param;

enum {
  TMS9980_ID=0,   /* 8-bit variant of tms9900.  Two distinct chips actually : tms9980a, */
                  /* and tms9981 with an extra clock and simplified power supply */
  TMS9995_ID      /* tms9985-like, with many improvements */
};

class tms99xx {
private:
  char *print_arg (int mode, int arg);
  void illegal(UINT16 opcode);
  void xop(UINT16 opcode);
  void ldcr_stcr(UINT16 opcode);
  void h0040(UINT16 opcode);
  void h0100(UINT16 opcode);
  void h0200(UINT16 opcode);
  void h0400(UINT16 opcode);
  void h0800(UINT16 opcode);
  void h1000(UINT16 opcode);
  void h2000(UINT16 opcode);
  void h4000b(UINT16 opcode);
  void h4000w(UINT16 opcode);
  int CpuType;
  __inline void exec(UINT16 opcode);
  void contextswitch(UINT16 addr);
  UINT16 decipheraddr(UINT16 opcode);
  UINT16 decipheraddrbyte(UINT16 opcode);
  void field_interrupt(void);
  void CYCLES(int a, int b);
  void reset_decrementer(void);
  void set_flag0(int val);
  void set_flag1(int val);
  void getstat(void);
  void setstat(void);
  UINT16 logical_right_shift(UINT16 val, int c);
  INT16 arithmetic_right_shift(INT16 val, int c);
  __inline void setst_lae(INT16 val);
  __inline void setst_byte_laep(INT8 val);
  __inline void setst_e(UINT16 val, UINT16 to);
  __inline void setst_c_lae(UINT16 to, UINT16 val);
  __inline INT16 setst_add_laeco(int a, int b);
  __inline INT16 setst_sub_laeco(int a, int b);
  __inline INT8 setst_addbyte_laecop(int a, int b);
  __inline INT8 setst_subbyte_laecop(int a, int b);
  __inline void setst_laeo(INT16 val);
  __inline UINT16 setst_sra_laec(INT16 a, UINT16 c);
  __inline UINT16 setst_srl_laec(UINT16 a,UINT16 c);
  __inline UINT16 setst_src_laec(UINT16 a,UINT16 c);
  __inline UINT16 setst_sla_laeco(UINT16 a, UINT16 c);
  UINT16 fetch(void);
  void external_instruction_notify(int ext_op_ID);
  void writeCRU(int CRUAddr, int Number, UINT16 Value);
  int READPORT(int Port);
  UINT16 readCRU(int CRUAddr, int Number);
  void writeword(int addr, int data);
  void decrementer_callback(int ignored);
protected:
	virtual int		execute(void);
public:
  tms99xx();
  ~tms99xx();
  void reset(void *param, int Type);
//  int execute(void);
  void set_irq_callback(int (*callback)(int irqline));
  unsigned short get_reg(int regnum);
  void set_reg(int regnum, unsigned val);
  virtual Byte read(Word) = 0;
  virtual void write(Word,Byte) = 0;
  virtual Byte readport(Word) = 0;
  virtual void writeport(Word,Byte) = 0;
  void set_irq_line(int irqline, int state);
  int dasm(int addr, char *p);
  int readbyte(int addr);
  int readword(int addr);
  void writebyte(int addr, int data);
  unsigned TMS99XX_ICOUNT;
	int		cycles;			// Number of cycles executed

  UINT8 lastparity;  /* rather than handling ST_OP directly, we copy the last value which
                               would set it here */
  /*  Some instructions (i.e. XOP, BLWP, and MID) disable interrupt recognition until another
      instruction is executed : so they set this flag */
  int disable_interrupt_recognition;

  void __fastcall SaveToStream(Byte *&ptr);
  void __fastcall LoadFromStream(Byte *&ptr);

  /* "actual" tms9900 registers : */
	UINT16 WP;  /* Workspace pointer */
	UINT16 PC;  /* Program counter */
	UINT16 STATUS;  /* STatus register */

  /* Now, data used for emulation */
	UINT16 IR;  /* Instruction register, with the currently parsed opcode */

	int interrupt_pending;  /* true if an interrupt must be honored... */

	int load_state; /* nonzero if the LOAD* line is active (low) */

	/* On tms9900, we cache the state of INTREQ* and IC0-IC3 here */
	/* On tms9980/9981, we translate the state of IC0-IC2 to the equivalent state for a tms9900,
	and store the result here */
	/* tms9995 is quite different : it latches the interrupt inputs */
	int irq_level;    /* We store the level of the request with the highest level here */
	int irq_state;    /* interrupt lines state */
	int int_latch;	  /* interrupt latches state */

	/* interrupt callback */
	/* note that this callback is used by tms9900_set_irq_line() and tms9980a_set_irq_line() to
	retreive the value on IC0-IC3 (non-standard behaviour) */
	int (*irq_callback)(int irq_line);

	int IDLE;       /* nonzero if processor is IDLE - i.e waiting for interrupt while writing
	                    special data on CRU bus */

//	UINT16 FR[16];  /* contains a copy of the workspace for the needs of the debugger */

    UINT16 OLD_PC; /* Dasm PC */
    
	unsigned char RAM[256]; /* on-chip RAM (yes, sir !) */

	/* on-chip event counter/timer*/
	int decrementer_enabled;
	UINT16 decrementer_interval;
	int decrementer;
	UINT16 decrementer_count;	/* used in event counter mode*/
	void *timer;  /* used in timer mode */

	/* additionnal registers */
	UINT16 flag; 	  /* flag register */
	int MID_flag;   /* MID flag register */

	/* chip config, which can be set on reset */
	int memory_wait_states_byte;
	int memory_wait_states_word;
};

typedef void (tms99xx::*tmsopcode)(UINT16 opcode);
#define callMemberFunction(object,ptrToMember)  ((object)->*(ptrToMember))

//---------------------------------------------------------------------------
#endif
