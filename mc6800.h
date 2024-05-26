//
//	mc6800.h
//

#ifndef __mc6800_h__
#define __mc6800_h__

#include "machdep.h"

enum {
	M6800_PC=1, M6800_S, M6800_A, M6800_B, M6800_X, M6800_CC,
	M6800_WAI_STATE, M6800_NMI_STATE, M6800_IRQ_STATE };

/*
 * This value is passed to cpu_get_reg to retrieve the previous
 * program counter value, ie. before a CPU emulation started
 * to fetch opcodes and arguments for the current instrution.
 */
#define REG_PREVIOUSPC	-1

/*
 * This value is passed to cpu_get_reg/cpu_set_reg, instead of one of
 * the names from the enum a CPU core defines for it's registers,
 * to get or set the contents of the memory pointed to by a stack pointer.
 * You can specify the n'th element on the stack by (REG_SP_CONTENTS-n),
 * ie. lower negative values. The actual element size (UINT16 or UINT32)
 * depends on the CPU core.
 * This is also used to replace the cpu_geturnpc() function.
 */
#define REG_SP_CONTENTS -2

#define M6800_INT_NONE    0     // No interrupt required
#define M6800_INT_IRQ	    1			// Standard IRQ interrupt
#define M6800_INT_NMI	    2			// NMI interrupt
#define M6800_WAI		      8			// set when WAI is waiting for an interrupt
#define M6800_SLP		      0x10	// HD63701 only


#define M6800_IRQ_LINE	  0			// IRQ line number
#define M6800_TIN_LINE	  1			// P20/Tin Input Capture line (eddge sense)
									              // Active edge is selecrable by internal reg.
									              // raise edge : CLEAR_LINE  -> ASSERT_LINE
									              // fall  edge : ASSERT_LINE -> CLEAR_LINE
									              // it is usual to use PULSE_LINE state
#define M6800_CLEAR_LINE		0		// clear (a fired, held or pulsed) line
#define M6800_ASSERT_LINE   1   // assert an interrupt immediately
#define M6800_HOLD_LINE     2   // hold interrupt line until enable is true
#define M6800_PULSE_LINE		3		// pulse interrupt line for one instruction

#define M6808_A 					M6800_A
#define M6808_B 					M6800_B
#define M6808_PC					M6800_PC
#define M6808_S 					M6800_S
#define M6808_X 					M6800_X
#define M6808_CC					M6800_CC
#define M6808_WAI_STATE 			M6800_WAI_STATE
#define M6808_NMI_STATE 			M6800_NMI_STATE
#define M6808_IRQ_STATE 			M6800_IRQ_STATE

#define M6808_INT_NONE              M6800_INT_NONE
#define M6808_INT_IRQ               M6800_INT_IRQ
#define M6808_INT_NMI               M6800_INT_NMI
#define M6808_WAI                   M6800_WAI
#define M6808_IRQ_LINE              M6800_IRQ_LINE

#define M6800_TYPE_6800     0
#define M6800_TYPE_6303     1

/* PUBLIC GLOBALS */
extern int m6800_ICount;

class mc6800 {

// Processor addressing modes
protected:

// Processor registers
public:

	PAIR	ppc;			/* Previous program counter */
	PAIR	pc; 			/* Program counter */
	PAIR	s;				/* Stack pointer */
	PAIR	x;				/* Index register */
	PAIR	d;				/* Accumulators */
	UINT8	cc; 			/* Condition codes */
	UINT8	wai_state;		/* WAI opcode state ,(or sleep opcode state) */
	UINT8	nmi_state;		/* NMI line state */
	UINT8	irq_state[2];	/* IRQ line state [IRQ1,TIN] */
	UINT8	ic_eddge;		/* InputCapture eddge , b.0=fall,b.1=raise */

	int		(*irq_callback)(int irqline);
	//void	(** insn)(void);	/* instruction table */
	int 	extra_cycles;	/* cycles used for interrupts */
	const UINT8 *cycles;			/* clock cycle of instruction table */
	/* internal registers */
	UINT8	port1_ddr;
	UINT8	port2_ddr;
	UINT8	port1_data;
	UINT8	port2_data;
	UINT8	tcsr;			/* Timer Control and Status Register */
	UINT8	pending_tcsr;	/* pending IRQ flag for clear IRQflag process */
	UINT8	irq2;			/* IRQ2 flags */
	UINT8	ram_ctrl;
	PAIR	counter;		/* free running counter */
	PAIR	output_compare;	/* output compare       */
	UINT16	input_capture;	/* input capture        */

	PAIR	timer_over;

private:


    void fetch_effective_address(void);
    void interrupt(void);
    void trap( void );

protected:
	virtual int		execute(void);
    void check_timer_event(void);
    UINT32 RM16(UINT32);
    void WM16(UINT32, PAIR *);

public:
	virtual    void	       reset(void);		// CPU reset
    int m6800_subtype;

                        mc6800();
                        ~mc6800();


void illegal(void);
void aba(void);
void abx(void);
void adca_di(void);
void adca_ex(void);
void adca_im(void);
void adca_ix(void);
void adcb_di(void);
void adcb_ex(void);
void adcb_im(void);
void adcb_ix(void);
void adda_di(void);
void adda_ex(void);
void adda_im(void);
void adda_ix(void);
void addb_di(void);
void addb_ex(void);
void addb_im(void);
void addb_ix(void);
void addd_di(void);
void addd_ex(void);
void addx_ex(void);
void addd_im(void);
void addd_ix(void);
void aim_di(void);
void aim_ix(void);
void anda_di(void);
void anda_ex(void);
void anda_im(void);
void anda_ix(void);
void andb_di(void);
void andb_ex(void);
void andb_im(void);
void andb_ix(void);
void asl_ex(void);
void asl_ix(void);
void asla(void);
void aslb(void);
void asld(void);
void asr_ex(void);
void asr_ix(void);
void asra(void);
void asrb(void);
void bcc(void);
void bcs(void);
void beq(void);
void bge(void);
void bgt(void);
void bhi(void);
void bita_di(void);
void bita_ex(void);
void bita_im(void);
void bita_ix(void);
void bitb_di(void);
void bitb_ex(void);
void bitb_im(void);
void bitb_ix(void);
void ble(void);
void bls(void);
void blt(void);
void bmi(void);
void bne(void);
void bpl(void);
void bra(void);
void brn(void);
void bsr(void);
void bvc(void);
void bvs(void);
void cba(void);
void clc(void);
void cli(void);
void clr_ex(void);
void clr_ix(void);
void clra(void);
void clrb(void);
void clv(void);
void cmpa_di(void);
void cmpa_ex(void);
void cmpa_im(void);
void cmpa_ix(void);
void cmpb_di(void);
void cmpb_ex(void);
void cmpb_im(void);
void cmpb_ix(void);
void cmpx_di(void);
void cmpx_ex(void);
void cmpx_im(void);
void cmpx_ix(void);
void com_ex(void);
void com_ix(void);
void coma(void);
void comb(void);
void daa(void);
void dec_ex(void);
void dec_ix(void);
void deca(void);
void decb(void);
void des(void);
void dex(void);
void eim_di(void);
void eim_ix(void);
void eora_di(void);
void eora_ex(void);
void eora_im(void);
void eora_ix(void);
void eorb_di(void);
void eorb_ex(void);
void eorb_im(void);
void eorb_ix(void);
void inc_ex(void);
void inc_ix(void);
void inca(void);
void incb(void);
void ins(void);
void inx(void);
void jmp_ex(void);
void jmp_ix(void);
void jsr_di(void);
void jsr_ex(void);
void jsr_ix(void);
void lda_di(void);
void lda_ex(void);
void lda_im(void);
void lda_ix(void);
void ldb_di(void);
void ldb_ex(void);
void ldb_im(void);
void ldb_ix(void);
void ldd_di(void);
void ldd_ex(void);
void ldd_im(void);
void ldd_ix(void);
void lds_di(void);
void lds_ex(void);
void lds_im(void);
void lds_ix(void);
void ldx_di(void);
void ldx_ex(void);
void ldx_im(void);
void ldx_ix(void);
void lsr_ex(void);
void lsr_ix(void);
void lsra(void);
void lsrb(void);
void lsrd(void);
void mul(void);
void neg_ex(void);
void neg_ix(void);
void nega(void);
void negb(void);
void nop(void);
void oim_di(void);
void oim_ix(void);
void ora_di(void);
void ora_ex(void);
void ora_im(void);
void ora_ix(void);
void orb_di(void);
void orb_ex(void);
void orb_im(void);
void orb_ix(void);
void psha(void);
void pshb(void);
void pshx(void);
void pula(void);
void pulb(void);
void pulx(void);
void rol_ex(void);
void rol_ix(void);
void rola(void);
void rolb(void);
void ror_ex(void);
void ror_ix(void);
void rora(void);
void rorb(void);
void rti(void);
void rts(void);
void sba(void);
void sbca_di(void);
void sbca_ex(void);
void sbca_im(void);
void sbca_ix(void);
void sbcb_di(void);
void sbcb_ex(void);
void sbcb_im(void);
void sbcb_ix(void);
void sec(void);
void sei(void);
void sev(void);
void sta_di(void);
void sta_ex(void);
void sta_im(void);
void sta_ix(void);
void stb_di(void);
void stb_ex(void);
void stb_im(void);
void stb_ix(void);
void std_di(void);
void std_ex(void);
void std_im(void);
void std_ix(void);
void sts_di(void);
void sts_ex(void);
void sts_im(void);
void sts_ix(void);
void stx_di(void);
void stx_ex(void);
void stx_im(void);
void stx_ix(void);
void suba_di(void);
void suba_ex(void);
void suba_im(void);
void suba_ix(void);
void subb_di(void);
void subb_ex(void);
void subb_im(void);
void subb_ix(void);
void subd_di(void);
void subd_ex(void);
void subd_im(void);
void subd_ix(void);
void swi(void);
void tab(void);
void tap(void);
void tba(void);
void tim_di(void);
void tim_ix(void);
void tpa(void);
void tst_ex(void);
void tst_ix(void);
void tsta(void);
void tstb(void);
void tsx(void);
void txs(void);
void undoc1(void);
void undoc2(void);
void wai(void);
void xgdx(void);

void cpx_di(void);
void cpx_ex(void);
void cpx_im(void);
void cpx_ix(void);

    int      dasm(int pc, char *buff);
    unsigned m6800_get_reg(int regnum);
    void ENTER_INTERRUPT(UINT16 irq_vector);

    virtual UINT8       read2(UINT16);
    virtual void        write2(UINT16,UINT8);

    virtual    UINT8       read(UINT16) = 0;
    virtual    void        write(UINT16,UINT8) = 0;
//    Byte        fetch();

};

class mc6803 : public mc6800
{
public:
    UINT8          p1ddr;
    UINT8          p2ddr;
    UINT8          port1;
    UINT8          port2;
    UINT8          p3ddr;
    UINT8          p4ddr;
    UINT8          port3;
    UINT8          port4;
    UINT8          tcsr1;
    UINT8          frch;
    UINT8          frcl;
    UINT8          ocr1h;
    UINT8          ocr1l;
    UINT8          icrh;
    UINT8          icrl;
    UINT8          tcsr2;
    UINT8          rmcr;
    UINT8          trcsr1;
    UINT8          rdr;
    UINT8          tdr;
    UINT8          rp5cr;
    UINT8          port5;
    UINT8          p6ddr;
    UINT8          port6;
    UINT8          port7;
    UINT8          ocr2h;
    UINT8          ocr2l;
    UINT8          tcsr3;
    UINT8          tconr;
    UINT8          t2cnt;
    UINT8          trcsr2;
    UINT8          tstreg;
    UINT8          p5ddr;
    UINT8          p6csr;
    UINT8          ram[256];
    int            TxDelay;
    UINT8          tmpfrch;
    UINT8          tdsr;
	virtual void   reset(void);		// CPU reset
    UINT8          read2(UINT16);
    void           write2(UINT16,UINT8);
    virtual void   writeport(UINT8, UINT8, UINT8);
    virtual UINT8  readport(UINT8);
    virtual void   writeinternal(UINT8, UINT8);
    virtual UINT8  readinternal(UINT8,UINT8);
    void           doserial(int clocks);

protected:
	virtual int		execute(void);
};
#endif // __mc6800_h__
