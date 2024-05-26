//
//	mc6809.h
//
//	Class definition for Motorola MC6809 microprocessor
//

#ifndef __mc6809_h__
#define __mc6809_h__

#include "machdep.h"

#define M6809_INT_NONE  0   /* No interrupt required */
#define M6809_INT_IRQ	1	/* Standard IRQ interrupt */
#define M6809_INT_FIRQ	2	/* Fast IRQ */
#define M6809_INT_NMI	4	/* NMI */	/* NS 970909 */
#define M6809_IRQ_LINE	0	/* IRQ line number */
#define M6809_FIRQ_LINE 1   /* FIRQ line number */
#define M6809_NMI_LINE  2   /* NMI line number */

#define M6809_CWAI		8	/* set when CWAI is waiting for an interrupt */
#define M6809_SYNC		16	/* set when SYNC is waiting for an interrupt */
#define M6809_LDS		32	/* set when LDS occured at least once */

#define CHECK_IRQ_LINES

#define M6809_CLEAR_LINE		0		/* clear (a fired, held or pulsed) line */
#define M6809_ASSERT_LINE     1       /* assert an interrupt immediately */
#define M6809_HOLD_LINE       2       /* hold interrupt line until enable is true */
#define M6809_PULSE_LINE		3		/* pulse interrupt line for one instruction */

class mc6809 {

// Processor addressing modes
protected:

// Processor registers
public:

	PAIR	pc; 		/* Program counter */
	PAIR	ppc;		/* Previous program counter */
	PAIR	dd;			/* Accumulator a and b */
	PAIR	dp; 		/* Direct Page register (page in MSB) */
	PAIR	u, s;		/* Stack pointers */
	PAIR	x, y;		/* Index registers */
    UINT8   cc;
	UINT8	ireg;		/* First opcode */
	UINT8	irq_state[3];
    int     extra_cycles; /* cycles used up by interrupts */
//    int     (*irq_callback)(int irqline);
    UINT8   int_state;  /* SYNC and CWAI flags */
    UINT8   nmi_state;
    PAIR ea;         /* effective address */

	int		cycles;			// Number of cycles executed
private:


	void			do_br(int);
	void			do_lbr(int);

	void			do_nmi(void);
	void			do_firq(void);
    void			do_irq(void);

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
    void addd_im(void);
    void addd_ix(void);
    void anda_di(void);
    void anda_ex(void);
    void anda_im(void);
    void anda_ix(void);
    void andb_di(void);
    void andb_ex(void);
    void andb_im(void);
    void andb_ix(void);
    void andcc(void);
    void asl_di(void);
    void asl_ex(void);
    void asl_ix(void);
    void asla(void);
    void aslb(void);
    void asr_di(void);
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
    void clr_di(void);
    void clr_ex(void);
    void clr_ix(void);
    void clra(void);
    void clrb(void);
    void cmpa_di(void);
    void cmpa_ex(void);
    void cmpa_im(void);
    void cmpa_ix(void);
    void cmpb_di(void);
    void cmpb_ex(void);
    void cmpb_im(void);
    void cmpb_ix(void);
    void cmpd_di(void);
    void cmpd_ex(void);
    void cmpd_im(void);
    void cmpd_ix(void);
    void cmps_di(void);
    void cmps_ex(void);
    void cmps_im(void);
    void cmps_ix(void);
    void cmpu_di(void);
    void cmpu_ex(void);
    void cmpu_im(void);
    void cmpu_ix(void);
    void cmpx_di(void);
    void cmpx_ex(void);
    void cmpx_im(void);
    void cmpx_ix(void);
    void cmpy_di(void);
    void cmpy_ex(void);
    void cmpy_im(void);
    void cmpy_ix(void);
    void com_di(void);
    void com_ex(void);
    void com_ix(void);
    void coma(void);
    void comb(void);
    void cwai(void);
    void daa(void);
    void dec_di(void);
    void dec_ex(void);
    void dec_ix(void);
    void deca(void);
    void decb(void);
    void eora_di(void);
    void eora_ex(void);
    void eora_im(void);
    void eora_ix(void);
    void eorb_di(void);
    void eorb_ex(void);
    void eorb_im(void);
    void eorb_ix(void);
    void exg(void);
    void illegal(void);
    void inc_di(void);
    void inc_ex(void);
    void inc_ix(void);
    void inca(void);
    void incb(void);
    void jmp_di(void);
    void jmp_ex(void);
    void jmp_ix(void);
    void jsr_di(void);
    void jsr_ex(void);
    void jsr_ix(void);
    void lbcc(void);
    void lbcs(void);
    void lbeq(void);
    void lbge(void);
    void lbgt(void);
    void lbhi(void);
    void lble(void);
    void lbls(void);
    void lblt(void);
    void lbmi(void);
    void lbne(void);
    void lbpl(void);
    void lbra(void);
    void lbrn(void);
    void lbsr(void);
    void lbvc(void);
    void lbvs(void);
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
    void ldu_di(void);
    void ldu_ex(void);
    void ldu_im(void);
    void ldu_ix(void);
    void ldx_di(void);
    void ldx_ex(void);
    void ldx_im(void);
    void ldx_ix(void);
    void ldy_di(void);
    void ldy_ex(void);
    void ldy_im(void);
    void ldy_ix(void);
    void leas(void);
    void leau(void);
    void leax(void);
    void leay(void);
    void lsr_di(void);
    void lsr_ex(void);
    void lsr_ix(void);
    void lsra(void);
    void lsrb(void);
    void mul(void);
    void neg_di(void);
    void neg_ex(void);
    void neg_ix(void);
    void nega(void);
    void negb(void);
    void nop(void);
    void ora_di(void);
    void ora_ex(void);
    void ora_im(void);
    void ora_ix(void);
    void orb_di(void);
    void orb_ex(void);
    void orb_im(void);
    void orb_ix(void);
    void orcc(void);
    void pshs(void);
    void pshu(void);
    void puls(void);
    void pulu(void);
    void rol_di(void);
    void rol_ex(void);
    void rol_ix(void);
    void rola(void);
    void rolb(void);
    void ror_di(void);
    void ror_ex(void);
    void ror_ix(void);
    void rora(void);
    void rorb(void);
    void rti(void);
    void rts(void);
    void sbca_di(void);
    void sbca_ex(void);
    void sbca_im(void);
    void sbca_ix(void);
    void sbcb_di(void);
    void sbcb_ex(void);
    void sbcb_im(void);
    void sbcb_ix(void);
    void sex(void);
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
    void stu_di(void);
    void stu_ex(void);
    void stu_im(void);
    void stu_ix(void);
    void stx_di(void);
    void stx_ex(void);
    void stx_im(void);
    void stx_ix(void);
    void sty_di(void);
    void sty_ex(void);
    void sty_im(void);
    void sty_ix(void);
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
    void swi2(void);
    void swi3(void);
    void sync(void);
    void tfr(void);
    void tst_di(void);
    void tst_ex(void);
    void tst_ix(void);
    void tsta(void);
    void tstb(void);

    void pref10(void);
    void pref11(void);

    void fetch_effective_address(void);
        void irq(void);
    UINT32 RM16(UINT32);
    void WM16(UINT32, PAIR *);

protected:
	virtual int	execute(void);

public:
    mc6809();
    ~mc6809();

    int        dasm(int addr, int page, char *p);

	void	   reset(void);		// CPU reset
    virtual    Byte        read(Word) = 0;
    virtual    void        write(Word,Byte) = 0;
    Byte       fetch();
    void interrupt(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};

#endif // __mc6809_h__
