//---------------------------------------------------------------------------

#ifndef z80H
#define z80H
//---------------------------------------------------------------------------

/****************************************************************************/
/*** Machine dependent definitions                                        ***/
/****************************************************************************/
#define LSB_FIRST                /* Compile for low-endian CPU         */

/****************************************************************************/
/* sizeof(byte)=1, sizeof(word)=2, sizeof(dword)>=4                         */
/****************************************************************************/
typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned       dword;
typedef signed char    offset;

/****************************************************************************/
/* Define a Z80 word. Upper bytes are always zero                           */
/****************************************************************************/
typedef union
{
 #ifdef LSB_FIRST
   struct { byte l,h,h2,h3; } B;
   struct { word l,h; } W;
   dword D;
 #else
   struct { byte h3,h2,h,l; } B;
   struct { word h,l; } W;
   dword D;
 #endif
} pair;

/****************************************************************************/
/*** End of machine dependent definitions                                 ***/
/****************************************************************************/

/****************************************************************************/
/* The Z80 registers. HALT is set to 1 when the CPU is halted, the refresh  */
/* register is calculated as follows: refresh=(Regs.R&127)|(Regs.R2&128)    */
/****************************************************************************/
#define S_FLAG          0x80
#define Z_FLAG          0x40
#define U1_FLAG         0x20
#define H_FLAG          0x10
#define U2_FLAG         0x08
#define V_FLAG          0x04
#define N_FLAG          0x02
#define C_FLAG          0x01

typedef struct
{
  pair AF,BC,DE,HL,IX,IY,PC,SP;
  pair AF2,BC2,DE2,HL2;
  unsigned IFF1,IFF2,HALT,IM,I,R,R_2;
} Z80_Regs;

#define Z80_IGNORE_INT  -1   /* Ignore interrupt                            */
#define Z80_NMI_INT     -2   /* Execute NMI                                 */
#define Z80_INT_REQ 	0x01	/* interrupt request mask		*/
#define Z80_INT_IEO 	0x02	/* interrupt disable mask(IEO)	*/

class z80cpu {
private:
  unsigned M_RDMEM_OPCODE (void);       // Read Opcode
  unsigned M_RDMEM_WORD (dword A);      // Read Word
  void M_WRMEM_WORD (dword A,word V);   // Write Word
  unsigned M_RDMEM_OPCODE_WORD (void);  // Read Word Opcode
  unsigned M_RD_XIX(void);
  unsigned M_RD_XIY(void);
  void M_WR_XIX(byte a);
  void M_WR_XIY(byte a);
  void InitTables (void);
  void Interrupt (int j);
public:
  z80cpu();
  ~z80cpu();
  virtual byte Z80_In (word Port) = 0;              // Input byte from Port
  virtual void Z80_Out (word Port,byte Value) = 0;  // Output byte to Port
  virtual unsigned Z80_RDMEM(word A) = 0;          // Read byte from given Mem Location
  virtual void Z80_WRMEM(word A,byte V) = 0;       // Write byte to Memory
  virtual unsigned Z80_RDSTACK(word A) = 0;          // Read byte from given Mem Location
  virtual void Z80_WRSTACK(word A,byte V) = 0;       // Write byte to Memory
  virtual void Z80_Patch (Z80_Regs *Regs) = 0;   /* Called when ED FE occurs. Can be used */
                                     /* to emulate disk access etc.           */
  virtual void Z80_Reti (void) = 0;              /* Called when RETI occurs               */
  virtual void Z80_Retn (void) = 0;              /* Called when RETN occurs               */
  virtual bool Z80_InstHook(int PC) = 0;      /* Called prior each instruction */
  unsigned Z80_GetPC (void);         /* Get program counter                   */
  void Z80_GetRegs (Z80_Regs *Regs); /* Get registers                         */
  void Z80_SetRegs (Z80_Regs *Regs); /* Set registers                         */
  void Z80_Reset (void);             /* Reset registers to the initial values */
  int  Z80_Step (void);              /* Execute Single Instruction              */
  int  Z80_Execute (void);           /* Execute IPeriod T-States              */
  word Z80 (void);                   /* Execute until Z80_Running==0          */
  void Z80_SetWaitStates (int n);    /* Set number of memory wait states.     */
                                     /* This only affects opcode fetching, so */
                                     /* wait state adjustment is still        */
                                     /* necessary in Z80_RDMEM, Z80_RDOP_ARG, */
                                     /* Z80_RDSTACK and Z80_WRSTACK           */
  virtual int Z80_Interrupt(void) = 0;           /* This is called after IPeriod T-States */
                                     /* have been executed. It should return  */
                                     /* Z80_IGNORE_INT, Z80_NMI_INT or a byte */
                                     /* identifying the device (most often    */
                                     /* 0xFF)                                 */
  static int Z80_Dasm (unsigned char *buffer,char *dest,unsigned PC);

  void adc_a_xhl(void);
  void adc_a_xix(void);
  void adc_a_xiy(void);
  void adc_a_a(void);
  void adc_a_b(void);
  void adc_a_c(void);
  void adc_a_d(void);
  void adc_a_e(void);
  void adc_a_h(void);
  void adc_a_l(void);
  void adc_a_ixl(void);
  void adc_a_ixh(void);
  void adc_a_iyl(void);
  void adc_a_iyh(void);
  void adc_a_byte(void);
  void adc_hl_bc(void);
  void adc_hl_de(void);
  void adc_hl_hl(void);
  void adc_hl_sp(void);
  void add_a_xhl(void);
  void add_a_xix(void);
  void add_a_xiy(void);
  void add_a_a(void);
  void add_a_b(void);
  void add_a_c(void);
  void add_a_d(void);
  void add_a_e(void);
  void add_a_h(void);
  void add_a_l(void);
  void add_a_ixl(void);
  void add_a_ixh(void);
  void add_a_iyl(void);
  void add_a_iyh(void);
  void add_a_byte(void);
  void add_hl_bc(void);
  void add_hl_de(void);
  void add_hl_hl(void);
  void add_hl_sp(void);
  void add_ix_bc(void);
  void add_ix_de(void);
  void add_ix_ix(void);
  void add_ix_sp(void);
  void add_iy_bc(void);
  void add_iy_de(void);
  void add_iy_iy(void);
  void add_iy_sp(void);
  void and_xhl(void);
  void and_xix(void);
  void and_xiy(void);
  void and_a(void);
  void and_b(void);
  void and_c(void);
  void and_d(void);
  void and_e(void);
  void and_h(void);
  void and_l(void);
  void and_ixh(void);
  void and_ixl(void);
  void and_iyh(void);
  void and_iyl(void);
  void and_byte(void);
  void bit_0_xhl(void);
  void bit_0_xix(void);
  void bit_0_xiy(void);
  void bit_0_a(void);
  void bit_0_b(void);
  void bit_0_c(void);
  void bit_0_d(void);
  void bit_0_e(void);
  void bit_0_h(void);
  void bit_0_l(void);
  void bit_1_xhl(void);
  void bit_1_xix(void);
  void bit_1_xiy(void);
  void bit_1_a(void);
  void bit_1_b(void);
  void bit_1_c(void);
  void bit_1_d(void);
  void bit_1_e(void);
  void bit_1_h(void);
  void bit_1_l(void);
  void bit_2_xhl(void);
  void bit_2_xix(void);
  void bit_2_xiy(void);
  void bit_2_a(void);
  void bit_2_b(void);
  void bit_2_c(void);
  void bit_2_d(void);
  void bit_2_e(void);
  void bit_2_h(void);
  void bit_2_l(void);
  void bit_3_xhl(void);
  void bit_3_xix(void);
  void bit_3_xiy(void);
  void bit_3_a(void);
  void bit_3_b(void);
  void bit_3_c(void);
  void bit_3_d(void);
  void bit_3_e(void);
  void bit_3_h(void);
  void bit_3_l(void);
  void bit_4_xhl(void);
  void bit_4_xix(void);
  void bit_4_xiy(void);
  void bit_4_a(void);
  void bit_4_b(void);
  void bit_4_c(void);
  void bit_4_d(void);
  void bit_4_e(void);
  void bit_4_h(void);
  void bit_4_l(void);
  void bit_5_xhl(void);
  void bit_5_xix(void);
  void bit_5_xiy(void);
  void bit_5_a(void);
  void bit_5_b(void);
  void bit_5_c(void);
  void bit_5_d(void);
  void bit_5_e(void);
  void bit_5_h(void);
  void bit_5_l(void);
  void bit_6_xhl(void);
  void bit_6_xix(void);
  void bit_6_xiy(void);
  void bit_6_a(void);
  void bit_6_b(void);
  void bit_6_c(void);
  void bit_6_d(void);
  void bit_6_e(void);
  void bit_6_h(void);
  void bit_6_l(void);
  void bit_7_xhl(void);
  void bit_7_xix(void);
  void bit_7_xiy(void);
  void bit_7_a(void);
  void bit_7_b(void);
  void bit_7_c(void);
  void bit_7_d(void);
  void bit_7_e(void);
  void bit_7_h(void);
  void bit_7_l(void);
  void call_c(void);
  void call_m(void);
  void call_nc(void);
  void call_nz(void);
  void call_p(void);
  void call_pe(void);
  void call_po(void);
  void call_z(void);
  void call(void);
  void ccf(void);
  void cp_xhl(void);
  void cp_xix(void);
  void cp_xiy(void);
  void cp_a(void);
  void cp_b(void);
  void cp_c(void);
  void cp_d(void);
  void cp_e(void);
  void cp_h(void);
  void cp_l(void);
  void cp_ixh(void);
  void cp_ixl(void);
  void cp_iyh(void);
  void cp_iyl(void);
  void cp_byte(void);
  void cpd(void);
  void cpdr(void);
  void cpi(void);
  void cpir(void);
  void cpl(void);
  void daa(void);
  void dec_xhl(void);
  void dec_xix(void);
  void dec_xiy(void);
  void dec_a(void);
  void dec_b(void);
  void dec_c(void);
  void dec_d(void);
  void dec_e(void);
  void dec_h(void);
  void dec_l(void);
  void dec_ixh(void);
  void dec_ixl(void);
  void dec_iyh(void);
  void dec_iyl(void);
  void dec_bc(void);
  void dec_de(void);
  void dec_hl(void);
  void dec_ix(void);
  void dec_iy(void);
  void dec_sp(void);
  void di(void);
  void djnz(void);
  void ei(void);
  void ex_xsp_hl(void);
  void ex_xsp_ix(void);
  void ex_xsp_iy(void);
  void ex_af_af(void);
  void ex_de_hl(void);
  void exx(void);
  void halt(void);
  void im_0(void);
  void im_1(void);
  void im_2(void);
  void in_a_c(void);
  void in_b_c(void);
  void in_c_c(void);
  void in_d_c(void);
  void in_e_c(void);
  void in_h_c(void);
  void in_l_c(void);
  void in_0_c(void);
  void in_a_byte(void);
  void inc_xhl(void);
  void inc_xix(void);
  void inc_xiy(void);
  void inc_a(void);
  void inc_b(void);
  void inc_c(void);
  void inc_d(void);
  void inc_e(void);
  void inc_h(void);
  void inc_l(void);
  void inc_ixh(void);
  void inc_ixl(void);
  void inc_iyh(void);
  void inc_iyl(void);
  void inc_bc(void);
  void inc_de(void);
  void inc_hl(void);
  void inc_ix(void);
  void inc_iy(void);
  void inc_sp(void);
  void ind(void);
  void indr(void);
  void ini(void);
  void inir(void);
  void jp(void);
  void jp_hl(void);
  void jp_ix(void);
  void jp_iy(void);
  void jp_c(void);
  void jp_m(void);
  void jp_nc(void);
  void jp_nz(void);
  void jp_p(void);
  void jp_pe(void);
  void jp_po(void);
  void jp_z(void);
  void jr(void);
  void jr_c(void);
  void jr_nc(void);
  void jr_nz(void);
  void jr_z(void);
  void ld_xbc_a(void);
  void ld_xde_a(void);
  void ld_xhl_a(void);
  void ld_xhl_b(void);
  void ld_xhl_c(void);
  void ld_xhl_d(void);
  void ld_xhl_e(void);
  void ld_xhl_h(void);
  void ld_xhl_l(void);
  void ld_xhl_byte(void);
  void ld_xix_a(void);
  void ld_xix_b(void);
  void ld_xix_c(void);
  void ld_xix_d(void);
  void ld_xix_e(void);
  void ld_xix_h(void);
  void ld_xix_l(void);
  void ld_xix_byte(void);
  void ld_xiy_a(void);
  void ld_xiy_b(void);
  void ld_xiy_c(void);
  void ld_xiy_d(void);
  void ld_xiy_e(void);
  void ld_xiy_h(void);
  void ld_xiy_l(void);
  void ld_xiy_byte(void);
  void ld_xbyte_a(void);
  void ld_xword_bc(void);
  void ld_xword_de(void);
  void ld_xword_hl(void);
  void ld_xword_ix(void);
  void ld_xword_iy(void);
  void ld_xword_sp(void);
  void ld_a_xbc(void);
  void ld_a_xde(void);
  void ld_a_xhl(void);
  void ld_a_xix(void);
  void ld_a_xiy(void);
  void ld_a_xbyte(void);
  void ld_a_byte(void);
  void ld_b_byte(void);
  void ld_c_byte(void);
  void ld_d_byte(void);
  void ld_e_byte(void);
  void ld_h_byte(void);
  void ld_l_byte(void);
  void ld_ixh_byte(void);
  void ld_ixl_byte(void);
  void ld_iyh_byte(void);
  void ld_iyl_byte(void);
  void ld_b_xhl(void);
  void ld_c_xhl(void);
  void ld_d_xhl(void);
  void ld_e_xhl(void);
  void ld_h_xhl(void);
  void ld_l_xhl(void);
  void ld_b_xix(void);
  void ld_c_xix(void);
  void ld_d_xix(void);
  void ld_e_xix(void);
  void ld_h_xix(void);
  void ld_l_xix(void);
  void ld_b_xiy(void);
  void ld_c_xiy(void);
  void ld_d_xiy(void);
  void ld_e_xiy(void);
  void ld_h_xiy(void);
  void ld_l_xiy(void);
  void ld_a_a(void);
  void ld_a_b(void);
  void ld_a_c(void);
  void ld_a_d(void);
  void ld_a_e(void);
  void ld_a_h(void);
  void ld_a_l(void);
  void ld_a_ixh(void);
  void ld_a_ixl(void);
  void ld_a_iyh(void);
  void ld_a_iyl(void);
  void ld_b_b(void);
  void ld_b_a(void);
  void ld_b_c(void);
  void ld_b_d(void);
  void ld_b_e(void);
  void ld_b_h(void);
  void ld_b_l(void);
  void ld_b_ixh(void);
  void ld_b_ixl(void);
  void ld_b_iyh(void);
  void ld_b_iyl(void);
  void ld_c_c(void);
  void ld_c_a(void);
  void ld_c_b(void);
  void ld_c_d(void);
  void ld_c_e(void);
  void ld_c_h(void);
  void ld_c_l(void);
  void ld_c_ixh(void);
  void ld_c_ixl(void);
  void ld_c_iyh(void);
  void ld_c_iyl(void);
  void ld_d_d(void);
  void ld_d_a(void);
  void ld_d_c(void);
  void ld_d_b(void);
  void ld_d_e(void);
  void ld_d_h(void);
  void ld_d_l(void);
  void ld_d_ixh(void);
  void ld_d_ixl(void);
  void ld_d_iyh(void);
  void ld_d_iyl(void);
  void ld_e_e(void);
  void ld_e_a(void);
  void ld_e_c(void);
  void ld_e_b(void);
  void ld_e_d(void);
  void ld_e_h(void);
  void ld_e_l(void);
  void ld_e_ixh(void);
  void ld_e_ixl(void);
  void ld_e_iyh(void);
  void ld_e_iyl(void);
  void ld_h_h(void);
  void ld_h_a(void);
  void ld_h_c(void);
  void ld_h_b(void);
  void ld_h_e(void);
  void ld_h_d(void);
  void ld_h_l(void);
  void ld_l_l(void);
  void ld_l_a(void);
  void ld_l_c(void);
  void ld_l_b(void);
  void ld_l_e(void);
  void ld_l_d(void);
  void ld_l_h(void);
  void ld_ixh_a(void);
  void ld_ixh_b(void);
  void ld_ixh_c(void);
  void ld_ixh_d(void);
  void ld_ixh_e(void);
  void ld_ixh_ixh(void);
  void ld_ixh_ixl(void);
  void ld_ixl_a(void);
  void ld_ixl_b(void);
  void ld_ixl_c(void);
  void ld_ixl_d(void);
  void ld_ixl_e(void);
  void ld_ixl_ixh(void);
  void ld_ixl_ixl(void);
  void ld_iyh_a(void);
  void ld_iyh_b(void);
  void ld_iyh_c(void);
  void ld_iyh_d(void);
  void ld_iyh_e(void);
  void ld_iyh_iyh(void);
  void ld_iyh_iyl(void);
  void ld_iyl_a(void);
  void ld_iyl_b(void);
  void ld_iyl_c(void);
  void ld_iyl_d(void);
  void ld_iyl_e(void);
  void ld_iyl_iyh(void);
  void ld_iyl_iyl(void);
  void ld_bc_xword(void);
  void ld_bc_word(void);
  void ld_de_xword(void);
  void ld_de_word(void);
  void ld_hl_xword(void);
  void ld_hl_word(void);
  void ld_ix_xword(void);
  void ld_ix_word(void);
  void ld_iy_xword(void);
  void ld_iy_word(void);
  void ld_sp_xword(void);
  void ld_sp_word(void);
  void ld_sp_hl(void);
  void ld_sp_ix(void);
  void ld_sp_iy(void);
  void ld_a_i(void);
  void ld_i_a(void);
  void ld_a_r(void);
  void ld_r_a(void);
  void ldd(void);
  void lddr(void);
  void ldi(void);
  void ldir(void);
  void neg(void);
  void nop(void);
  void or_xhl(void);
  void or_xix(void);
  void or_xiy(void);
  void or_a(void);
  void or_b(void);
  void or_c(void);
  void or_d(void);
  void or_e(void);
  void or_h(void);
  void or_l(void);
  void or_ixh(void);
  void or_ixl(void);
  void or_iyh(void);
  void or_iyl(void);
  void or_byte(void);
  void outd(void);
  void otdr(void);
  void outi(void);
  void otir(void);
  void out_c_a(void);
  void out_c_b(void);
  void out_c_c(void);
  void out_c_d(void);
  void out_c_e(void);
  void out_c_h(void);
  void out_c_l(void);
  void out_c_0(void);
  void out_byte_a(void);
  void pop_af(void);
  void pop_bc(void);
  void pop_de(void);
  void pop_hl(void);
  void pop_ix(void);
  void pop_iy(void);
  void push_af(void);
  void push_bc(void);
  void push_de(void);
  void push_hl(void);
  void push_ix(void);
  void push_iy(void);
  void res_0_xhl(void);
  void res_0_xix(void);
  void res_0_xiy(void);
  void res_0_a(void);
  void res_0_b(void);
  void res_0_c(void);
  void res_0_d(void);
  void res_0_e(void);
  void res_0_h(void);
  void res_0_l(void);
  void res_1_xhl(void);
  void res_1_xix(void);
  void res_1_xiy(void);
  void res_1_a(void);
  void res_1_b(void);
  void res_1_c(void);
  void res_1_d(void);
  void res_1_e(void);
  void res_1_h(void);
  void res_1_l(void);
  void res_2_xhl(void);
  void res_2_xix(void);
  void res_2_xiy(void);
  void res_2_a(void);
  void res_2_b(void);
  void res_2_c(void);
  void res_2_d(void);
  void res_2_e(void);
  void res_2_h(void);
  void res_2_l(void);
  void res_3_xhl(void);
  void res_3_xix(void);
  void res_3_xiy(void);
  void res_3_a(void);
  void res_3_b(void);
  void res_3_c(void);
  void res_3_d(void);
  void res_3_e(void);
  void res_3_h(void);
  void res_3_l(void);
  void res_4_xhl(void);
  void res_4_xix(void);
  void res_4_xiy(void);
  void res_4_a(void);
  void res_4_b(void);
  void res_4_c(void);
  void res_4_d(void);
  void res_4_e(void);
  void res_4_h(void);
  void res_4_l(void);
  void res_5_xhl(void);
  void res_5_xix(void);
  void res_5_xiy(void);
  void res_5_a(void);
  void res_5_b(void);
  void res_5_c(void);
  void res_5_d(void);
  void res_5_e(void);
  void res_5_h(void);
  void res_5_l(void);
  void res_6_xhl(void);
  void res_6_xix(void);
  void res_6_xiy(void);
  void res_6_a(void);
  void res_6_b(void);
  void res_6_c(void);
  void res_6_d(void);
  void res_6_e(void);
  void res_6_h(void);
  void res_6_l(void);
  void res_7_xhl(void);
  void res_7_xix(void);
  void res_7_xiy(void);
  void res_7_a(void);
  void res_7_b(void);
  void res_7_c(void);
  void res_7_d(void);
  void res_7_e(void);
  void res_7_h(void);
  void res_7_l(void);
  void ret(void);
  void ret_c(void);
  void ret_m(void);
  void ret_nc(void);
  void ret_nz(void);
  void ret_p(void);
  void ret_pe(void);
  void ret_po(void);
  void ret_z(void);
  void reti(void);
  void retn(void);
  void rl_xhl(void);
  void rl_xix(void);
  void rl_xiy(void);
  void rl_a(void);
  void rl_b(void);
  void rl_c(void);
  void rl_d(void);
  void rl_e(void);
  void rl_h(void);
  void rl_l(void);
  void rla(void);
  void rlc_xhl(void);
  void rlc_xix(void);
  void rlc_xiy(void);
  void rlc_a(void);
  void rlc_b(void);
  void rlc_c(void);
  void rlc_d(void);
  void rlc_e(void);
  void rlc_h(void);
  void rlc_l(void);
  void rlca(void);
  void rld(void);
  void rr_xhl(void);
  void rr_xix(void);
  void rr_xiy(void);
  void rr_a(void);
  void rr_b(void);
  void rr_c(void);
  void rr_d(void);
  void rr_e(void);
  void rr_h(void);
  void rr_l(void);
  void rra(void);
  void rrc_xhl(void);
  void rrc_xix(void);
  void rrc_xiy(void);
  void rrc_a(void);
  void rrc_b(void);
  void rrc_c(void);
  void rrc_d(void);
  void rrc_e(void);
  void rrc_h(void);
  void rrc_l(void);
  void rrca(void);
  void rrd(void);
  void rst_00(void);
  void rst_08(void);
  void rst_10(void);
  void rst_18(void);
  void rst_20(void);
  void rst_28(void);
  void rst_30(void);
  void rst_38(void);
  void sbc_a_byte(void);
  void sbc_a_xhl(void);
  void sbc_a_xix(void);
  void sbc_a_xiy(void);
  void sbc_a_a(void);
  void sbc_a_b(void);
  void sbc_a_c(void);
  void sbc_a_d(void);
  void sbc_a_e(void);
  void sbc_a_h(void);
  void sbc_a_l(void);
  void sbc_a_ixh(void);
  void sbc_a_ixl(void);
  void sbc_a_iyh(void);
  void sbc_a_iyl(void);
  void sbc_hl_bc(void);
  void sbc_hl_de(void);
  void sbc_hl_hl(void);
  void sbc_hl_sp(void);
  void scf(void);
  void set_0_xhl(void);
  void set_0_xix(void);
  void set_0_xiy(void);
  void set_0_a(void);
  void set_0_b(void);
  void set_0_c(void);
  void set_0_d(void);
  void set_0_e(void);
  void set_0_h(void);
  void set_0_l(void);
  void set_1_xhl(void);
  void set_1_xix(void);
  void set_1_xiy(void);
  void set_1_a(void);
  void set_1_b(void);
  void set_1_c(void);
  void set_1_d(void);
  void set_1_e(void);
  void set_1_h(void);
  void set_1_l(void);
  void set_2_xhl(void);
  void set_2_xix(void);
  void set_2_xiy(void);
  void set_2_a(void);
  void set_2_b(void);
  void set_2_c(void);
  void set_2_d(void);
  void set_2_e(void);
  void set_2_h(void);
  void set_2_l(void);
  void set_3_xhl(void);
  void set_3_xix(void);
  void set_3_xiy(void);
  void set_3_a(void);
  void set_3_b(void);
  void set_3_c(void);
  void set_3_d(void);
  void set_3_e(void);
  void set_3_h(void);
  void set_3_l(void);
  void set_4_xhl(void);
  void set_4_xix(void);
  void set_4_xiy(void);
  void set_4_a(void);
  void set_4_b(void);
  void set_4_c(void);
  void set_4_d(void);
  void set_4_e(void);
  void set_4_h(void);
  void set_4_l(void);
  void set_5_xhl(void);
  void set_5_xix(void);
  void set_5_xiy(void);
  void set_5_a(void);
  void set_5_b(void);
  void set_5_c(void);
  void set_5_d(void);
  void set_5_e(void);
  void set_5_h(void);
  void set_5_l(void);
  void set_6_xhl(void);
  void set_6_xix(void);
  void set_6_xiy(void);
  void set_6_a(void);
  void set_6_b(void);
  void set_6_c(void);
  void set_6_d(void);
  void set_6_e(void);
  void set_6_h(void);
  void set_6_l(void);
  void set_7_xhl(void);
  void set_7_xix(void);
  void set_7_xiy(void);
  void set_7_a(void);
  void set_7_b(void);
  void set_7_c(void);
  void set_7_d(void);
  void set_7_e(void);
  void set_7_h(void);
  void set_7_l(void);
  void sla_xhl(void);
  void sla_xix(void);
  void sla_xiy(void);
  void sla_a(void);
  void sla_b(void);
  void sla_c(void);
  void sla_d(void);
  void sla_e(void);
  void sla_h(void);
  void sla_l(void);
  void sll_xhl(void);
  void sll_xix(void);
  void sll_xiy(void);
  void sll_a(void);
  void sll_b(void);
  void sll_c(void);
  void sll_d(void);
  void sll_e(void);
  void sll_h(void);
  void sll_l(void);
  void sra_xhl(void);
  void sra_xix(void);
  void sra_xiy(void);
  void sra_a(void);
  void sra_b(void);
  void sra_c(void);
  void sra_d(void);
  void sra_e(void);
  void sra_h(void);
  void sra_l(void);
  void srl_xhl(void);
  void srl_xix(void);
  void srl_xiy(void);
  void srl_a(void);
  void srl_b(void);
  void srl_c(void);
  void srl_d(void);
  void srl_e(void);
  void srl_h(void);
  void srl_l(void);
  void sub_xhl(void);
  void sub_xix(void);
  void sub_xiy(void);
  void sub_a(void);
  void sub_b(void);
  void sub_c(void);
  void sub_d(void);
  void sub_e(void);
  void sub_h(void);
  void sub_l(void);
  void sub_ixh(void);
  void sub_ixl(void);
  void sub_iyh(void);
  void sub_iyl(void);
  void sub_byte(void);
  void xor_xhl(void);
  void xor_xix(void);
  void xor_xiy(void);
  void xor_a(void);
  void xor_b(void);
  void xor_c(void);
  void xor_d(void);
  void xor_e(void);
  void xor_h(void);
  void xor_l(void);
  void xor_ixh(void);
  void xor_ixl(void);
  void xor_iyh(void);
  void xor_iyl(void);
  void xor_byte(void);
  void no_op(void);
  void patch(void);
  void no_op_xx(void);
  void dd_cb(void);
  void fd_cb(void);
  void cb(void);
  void dd(void);
  void ed(void);
  void fd(void);

  Z80_Regs  R;          // Registers
  int Z80_Running;      // When 0, emulation terminates
  int Z80_IPeriod;      // Number of T-states per interrupt
  int Z80_ICount;       // T-state count
  int Z80_IRQ;          // Current IRQ status. Checked after EI occurs
  int InitTables_virgin;
  byte PTable[512];
  byte ZSTable[512];
  byte ZSPTable[512];
};

typedef void (z80cpu::*z80opcode)(void);
#define callZ80Function(object,ptrToMember)  ((object)->*(ptrToMember))

#endif
