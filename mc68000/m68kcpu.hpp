#ifndef M68KCPU__HEADER
#define M68KCPU__HEADER


#include "m68k.h"
#include <limits.h>

class sim40_module;
class timer_module;
class serial_module;
class dma_module;

/* ======================================================================== */
/* ==================== ARCHITECTURE-DEPENDANT DEFINES ==================== */
/* ======================================================================== */

/* Check if we have certain storage sizes */

#if UCHAR_MAX == 0xff
#define M68K_HAS_8_BIT_SIZE  1
#else
#define M68K_HAS_8_BIT_SIZE  0
#endif /* UCHAR_MAX == 0xff */

#if USHRT_MAX == 0xffff
#define M68K_HAS_16_BIT_SIZE 1
#else
#define M68K_HAS_16_BIT_SIZE 0
#endif /* USHRT_MAX == 0xffff */

#if ULONG_MAX == 0xffffffff
#define M68K_HAS_32_BIT_SIZE 1
#else
#define M68K_HAS_32_BIT_SIZE 0
#endif /* ULONG_MAX == 0xffffffff */

#if UINT_MAX > 0xffffffff
#define M68K_OVER_32_BIT     1
#else
#define M68K_OVER_32_BIT     0
#endif /* UINT_MAX > 0xffffffff */

/* Data types used in this emulation core */
#undef int8
#undef int16
#undef int32
#undef uint
#undef uint8
#undef uint16
#undef uint

#define int8   signed char			/* ASG: changed from char to signed char */
#define uint8  unsigned char
#define int16  short
#define uint16 unsigned short
#define int32  long

/* int and unsigned int must be at least 32 bits wide */
#define uint   unsigned int


/* Allow for architectures that don't have 8-bit sizes */
#if M68K_HAS_8_BIT_SIZE
#define MAKE_INT_8(A) (int8)((A)&0xff)
#else
#undef  int8
#define int8   int
#undef  uint8
#define uint8  unsigned int
int MAKE_INT_8(int value)
{
   /* Will this work for 1's complement machines? */
   return (value & 0x80) ? value | ~0xff : value & 0xff;
}
#endif /* M68K_HAS_8_BIT_SIZE */


/* Allow for architectures that don't have 16-bit sizes */
#if M68K_HAS_16_BIT_SIZE
#define MAKE_INT_16(A) (int16)((A)&0xffff)
#else
#undef  int16
#define int16  int
#undef  uint16
#define uint16 unsigned int
int MAKE_INT_16(int value)
{
   /* Will this work for 1's complement machines? */
   return (value & 0x8000) ? value | ~0xffff : value & 0xffff;
}
#endif /* M68K_HAS_16_BIT_SIZE */


/* Allow for architectures that don't have 32-bit sizes */
#if M68K_HAS_32_BIT_SIZE
#if M68K_OVER_32_BIT
#define MAKE_INT_32(A) (int32)((A)&0xffffffff)
#else
#define MAKE_INT_32(A) (int32)(A)
#endif /* M68K_OVER_32_BIT */
#else
#undef  int32
#define int32  int
int MAKE_INT_32(int value)
{
   /* Will this work for 1's complement machines? */
   return (value & 0x80000000) ? value | ~0xffffffff : value & 0xffffffff;
}
#endif /* M68K_HAS_32_BIT_SIZE */



/* ======================================================================== */
/* ============================ GENERAL DEFINES =========================== */
/* ======================================================================== */

/* Exception Vectors handled by emulation */
#define EXCEPTION_ILLEGAL_INSTRUCTION      4
#define EXCEPTION_ZERO_DIVIDE              5
#define EXCEPTION_CHK                      6
#define EXCEPTION_TRAPV                    7
#define EXCEPTION_PRIVILEGE_VIOLATION      8
#define EXCEPTION_TRACE                    9
#define EXCEPTION_1010                    10
#define EXCEPTION_1111                    11
#define EXCEPTION_FORMAT_ERROR            14
#define EXCEPTION_UNINITIALIZED_INTERRUPT 15
#define EXCEPTION_SPURIOUS_INTERRUPT      24
#define EXCEPTION_INTERRUPT_AUTOVECTOR    24
#define EXCEPTION_TRAP_BASE               32

/* Function codes set by CPU during data/address bus activity */
#define FUNCTION_CODE_USER_DATA          1
#define FUNCTION_CODE_USER_PROGRAM       2
#define FUNCTION_CODE_SUPERVISOR_DATA    5
#define FUNCTION_CODE_SUPERVISOR_PROGRAM 6
#define FUNCTION_CODE_CPU_SPACE          7

/* CPU modes for deciding what to emulate */
#define CPU_MODE_000   M68K_CPU_MODE_68000
#define CPU_MODE_010   M68K_CPU_MODE_68010
#define CPU_MODE_EC020 M68K_CPU_MODE_68EC020
#define CPU_MODE_020   M68K_CPU_MODE_68020

#define CPU_MODE_ALL       (CPU_MODE_000 | CPU_MODE_010 | CPU_MODE_EC020 | CPU_MODE_020)
#define CPU_MODE_010_PLUS  (CPU_MODE_010 | CPU_MODE_EC020 | CPU_MODE_020)
#define CPU_MODE_010_LESS  (CPU_MODE_000 | CPU_MODE_010)
#define CPU_MODE_EC020_PLUS  (CPU_MODE_EC020 | CPU_MODE_020)
#define CPU_MODE_EC020_LESS  (CPU_MODE_000 | CPU_MODE_010 | CPU_MODE_EC020)
#define CPU_MODE_020_PLUS  CPU_MODE_020
#define CPU_MODE_020_LESS  (CPU_MODE_000 | CPU_MODE_010 | CPU_MODE_EC020 | CPU_MODE_020)


/* ======================================================================== */
/* ================================ MACROS ================================ */
/* ======================================================================== */

/* Bit Isolation Macros */
#define BIT_0(A)  ((A) & 0x00000001)
#define BIT_1(A)  ((A) & 0x00000002)
#define BIT_2(A)  ((A) & 0x00000004)
#define BIT_3(A)  ((A) & 0x00000008)
#define BIT_4(A)  ((A) & 0x00000010)
#define BIT_5(A)  ((A) & 0x00000020)
#define BIT_6(A)  ((A) & 0x00000040)
#define BIT_7(A)  ((A) & 0x00000080)
#define BIT_8(A)  ((A) & 0x00000100)
#define BIT_9(A)  ((A) & 0x00000200)
#define BIT_A(A)  ((A) & 0x00000400)
#define BIT_B(A)  ((A) & 0x00000800)
#define BIT_C(A)  ((A) & 0x00001000)
#define BIT_D(A)  ((A) & 0x00002000)
#define BIT_E(A)  ((A) & 0x00004000)
#define BIT_F(A)  ((A) & 0x00008000)
#define BIT_10(A) ((A) & 0x00010000)
#define BIT_11(A) ((A) & 0x00020000)
#define BIT_12(A) ((A) & 0x00040000)
#define BIT_13(A) ((A) & 0x00080000)
#define BIT_14(A) ((A) & 0x00100000)
#define BIT_15(A) ((A) & 0x00200000)
#define BIT_16(A) ((A) & 0x00400000)
#define BIT_17(A) ((A) & 0x00800000)
#define BIT_18(A) ((A) & 0x01000000)
#define BIT_19(A) ((A) & 0x02000000)
#define BIT_1A(A) ((A) & 0x04000000)
#define BIT_1B(A) ((A) & 0x08000000)
#define BIT_1C(A) ((A) & 0x10000000)
#define BIT_1D(A) ((A) & 0x20000000)
#define BIT_1E(A) ((A) & 0x40000000)
#define BIT_1F(A) ((A) & 0x80000000)

/* Get the most significant bit for specific sizes */
#define GET_MSB_8(A)  ((A) & 0x80)
#define GET_MSB_9(A)  ((A) & 0x100)
#define GET_MSB_16(A) ((A) & 0x8000)
#define GET_MSB_17(A) ((A) & 0x10000)
#define GET_MSB_32(A) ((A) & 0x80000000)

/* Isolate nibbles */
#define LOW_NIBBLE(A) ((A) & 0x0f)
#define HIGH_NIBBLE(A) ((A) & 0xf0)

/* These are used to isolate 8, 16, and 32 bit sizes */
#define MASK_OUT_ABOVE_2(A)  ((A) & 3)
#define MASK_OUT_ABOVE_8(A)  ((A) & 0xff)
#define MASK_OUT_ABOVE_16(A) ((A) & 0xffff)
#define MASK_OUT_BELOW_2(A)  ((A) & ~3)
#define MASK_OUT_BELOW_8(A)  ((A) & ~0xff)
#define MASK_OUT_BELOW_16(A) ((A) & ~0xffff)

/* No need for useless masking if we're 32-bit */
#if M68K_OVER_32_BIT
#define MASK_OUT_ABOVE_32(A) ((A) & 0xffffffff)
#define MASK_OUT_BELOW_32(A) ((A) & ~0xffffffff)
#else
#define MASK_OUT_ABOVE_32(A) (A)
#define MASK_OUT_BELOW_32(A) 0
#endif /* M68K_OVER_32_BIT */


/* Simulate address lines of 68k family */
#define ADDRESS_68K(A) (CPU_MODE & CPU_MODE_020_PLUS ? A : (A)&0xffffff)


/* Instruction extension word information for indexed addressing modes */
#define EXT_INDEX_LONG(A)         BIT_B(A)
#define EXT_INDEX_AR(A)           BIT_F(A)
#define EXT_INDEX_REGISTER(A)     (((A)>>12)&7)
#define EXT_INDEX_SCALE(A)        (((A)>>9)&3)
#define EXT_BRIEF_FORMAT(A)       !BIT_8(A)
#define EXT_IX_SUPPRESSED(A)      BIT_6(A)
#define EXT_BR_SUPPRESSED(A)      BIT_7(A)
#define EXT_BD_PRESENT(A)         BIT_5(A)
#define EXT_BD_LONG(A)            BIT_4(A)
#define EXT_NO_MEMORY_INDIRECT(A) !((A)&7)
#define EXT_OD_PRESENT(A)         BIT_1(A)
#define EXT_OD_LONG(A)            BIT_0(A)
#define EXT_POSTINDEX(A)          BIT_2(A)


/* Shift & Rotate Macros.
 * 32-bit shifts defined in architecture-dependant section.
 */

#define LSL(A, C) ((A) << (C))
#define LSR(A, C) ((A) >> (C))

/* Some > 32-bit optimizations */
#if M68K_OVER_32_BIT
/* Shift left and right */
#define LSR_32(A, C) ((A) >> (C))
#define LSL_32(A, C) ((A) << (C))
#else
/* We have to do this because the morons at ANSI decided that shifts
 * by >= data size are undefined.
 */
#define LSR_32(A, C) ((C) < 32 ? (A) >> (C) : 0)
#define LSL_32(A, C) ((C) < 32 ? (A) << (C) : 0)
#endif /* M68K_OVER_32_BIT */

#define ROL_8(A, C)      MASK_OUT_ABOVE_8(LSL(A, C) | LSR(A, 8-(C)))
#define ROL_9(A, C)                       LSL(A, C) | LSR(A, 9-(C))
#define ROL_16(A, C)    MASK_OUT_ABOVE_16(LSL(A, C) | LSR(A, 16-(C)))
#define ROL_17(A, C)                      LSL(A, C) | LSR(A, 17-(C))
#define ROL_32(A, C) MASK_OUT_ABOVE_32(LSL_32(A, C) | LSR_32(A, 32-(C)))
#define ROL_33(A, C)                  (LSL_32(A, C) | LSR_32(A, 33-(C)))

#define ROR_8(A, C)      MASK_OUT_ABOVE_8(LSR(A, C) | LSL(A, 8-(C)))
#define ROR_9(A, C)                       LSR(A, C) | LSL(A, 9-(C))
#define ROR_16(A, C)    MASK_OUT_ABOVE_16(LSR(A, C) | LSL(A, 16-(C)))
#define ROR_17(A, C)                      LSR(A, C) | LSL(A, 17-(C))
#define ROR_32(A, C) MASK_OUT_ABOVE_32(LSR_32(A, C) | LSL_32(A, 32-(C)))
#define ROR_33(A, C)                  (LSR_32(A, C) | LSL_32(A, 33-(C)))


/* Access the CPU registers */
#define CPU_MODE         m68k_cpu.mode
#define CPU_D            m68k_cpu.dr
#define CPU_A            m68k_cpu.ar
#define CPU_PPC 		 m68k_cpu.ppc
#define CPU_PC           m68k_cpu.pc
#define CPU_SP           m68k_cpu.sp
#define CPU_USP          m68k_cpu.sp[0]
#define CPU_ISP          m68k_cpu.sp[1]
#define CPU_MSP          m68k_cpu.sp[3]
#define CPU_VBR          m68k_cpu.vbr
#define CPU_SFC          m68k_cpu.sfc
#define CPU_DFC          m68k_cpu.dfc
#define CPU_CACR         m68k_cpu.cacr
#define CPU_CAAR         m68k_cpu.caar
#define CPU_IR           m68k_cpu.ir
#define CPU_T1           m68k_cpu.t1_flag
#define CPU_T0           m68k_cpu.t0_flag
#define CPU_S            m68k_cpu.s_flag
#define CPU_M            m68k_cpu.m_flag
#define CPU_X            m68k_cpu.x_flag
#define CPU_N            m68k_cpu.n_flag
#define CPU_NOT_Z        m68k_cpu.not_z_flag
#define CPU_V            m68k_cpu.v_flag
#define CPU_C            m68k_cpu.c_flag
#define CPU_INT_MASK     m68k_cpu.int_mask
#define CPU_INT_STATE    m68k_cpu.int_state /* ASG: changed from CPU_INTS_PENDING */
#define CPU_STOPPED      m68k_cpu.stopped
#define CPU_HALTED       m68k_cpu.halted
#define CPU_INT_CYCLES   m68k_cpu.int_cycles /* ASG */
#define CPU_PREF_ADDR    m68k_cpu.pref_addr
#define CPU_PREF_DATA    m68k_cpu.pref_data

#define CPU_INT_ACK_CALLBACK     m68k_cpu.int_ack_callback
#define CPU_BKPT_ACK_CALLBACK    m68k_cpu.bkpt_ack_callback
#define CPU_RESET_INSTR_CALLBACK m68k_cpu.reset_instr_callback
#define CPU_PC_CHANGED_CALLBACK  m68k_cpu.pc_changed_callback
#define CPU_SET_FC_CALLBACK      m68k_cpu.set_fc_callback
#define CPU_INSTR_HOOK_CALLBACK  m68k_cpu.instr_hook_callback


/*
 * The general instruction format follows this pattern:
 * .... XXX. .... .YYY
 * where XXX is register X and YYY is register Y
 */
/* Data Register Isolation */
#define DX (CPU_D[(CPU_IR >> 9) & 7])
#define DY (CPU_D[CPU_IR & 7])
/* Address Register Isolation */
#define AX (CPU_A[(CPU_IR >> 9) & 7])
#define AY (CPU_A[CPU_IR & 7])


/* Effective Address Calculations */
#define EA_AI    AY                                    /* address register indirect */
#define EA_PI_8  (AY++)                                /* postincrement (size = byte) */
#define EA_PI7_8 ((CPU_A[7]+=2)-2)                     /* postincrement (size = byte & AR = 7) */
#define EA_PI_16 ((AY+=2)-2)                           /* postincrement (size = word) */
#define EA_PI_32 ((AY+=4)-4)                           /* postincrement (size = long) */
#define EA_PD_8  (--AY)                                /* predecrement (size = byte) */
#define EA_PD7_8 (CPU_A[7]-=2)                         /* predecrement (size = byte & AR = 7) */
#define EA_PD_16 (AY-=2)                               /* predecrement (size = word) */
#define EA_PD_32 (AY-=4)                               /* predecrement (size = long) */
#define EA_DI    (AY+MAKE_INT_16(m68ki_read_imm_16())) /* displacement */
#define EA_IX    m68ki_get_ea_ix()                     /* indirect + index */
#define EA_AW    MAKE_INT_16(m68ki_read_imm_16())      /* absolute word */
#define EA_AL    m68ki_read_imm_32()                   /* absolute long */
#define EA_PCIX  m68ki_get_ea_pcix()                   /* pc indirect + index */


/* Add and Subtract Flag Calculation Macros */
#define VFLAG_ADD_8(S, D, R) GET_MSB_8((S & D & ~R) | (~S & ~D & R))
#define VFLAG_ADD_16(S, D, R) GET_MSB_16((S & D & ~R) | (~S & ~D & R))
#define VFLAG_ADD_32(S, D, R) GET_MSB_32((S & D & ~R) | (~S & ~D & R))

#define CFLAG_ADD_8(S, D, R) GET_MSB_8((S & D) | (~R & D) | (S & ~R))
#define CFLAG_ADD_16(S, D, R) GET_MSB_16((S & D) | (~R & D) | (S & ~R))
#define CFLAG_ADD_32(S, D, R) GET_MSB_32((S & D) | (~R & D) | (S & ~R))


#define VFLAG_SUB_8(S, D, R) GET_MSB_8((~S & D & ~R) | (S & ~D & R))
#define VFLAG_SUB_16(S, D, R) GET_MSB_16((~S & D & ~R) | (S & ~D & R))
#define VFLAG_SUB_32(S, D, R) GET_MSB_32((~S & D & ~R) | (S & ~D & R))

#define CFLAG_SUB_8(S, D, R) GET_MSB_8((S & ~D) | (R & ~D) | (S & R))
#define CFLAG_SUB_16(S, D, R) GET_MSB_16((S & ~D) | (R & ~D) | (S & R))
#define CFLAG_SUB_32(S, D, R) GET_MSB_32((S & ~D) | (R & ~D) | (S & R))


/* Conditions */
#define CONDITION_HI     (CPU_C == 0 && CPU_NOT_Z != 0)
#define CONDITION_NOT_HI (CPU_C != 0 || CPU_NOT_Z == 0)
#define CONDITION_LS     (CPU_C != 0 || CPU_NOT_Z == 0)
#define CONDITION_NOT_LS (CPU_C == 0 && CPU_NOT_Z != 0)
#define CONDITION_CC     (CPU_C == 0)
#define CONDITION_NOT_CC (CPU_C != 0)
#define CONDITION_CS     (CPU_C != 0)
#define CONDITION_NOT_CS (CPU_C == 0)
#define CONDITION_NE     (CPU_NOT_Z != 0)
#define CONDITION_NOT_NE (CPU_NOT_Z == 0)
#define CONDITION_EQ     (CPU_NOT_Z == 0)
#define CONDITION_NOT_EQ (CPU_NOT_Z != 0)
#define CONDITION_VC     (CPU_V == 0)
#define CONDITION_NOT_VC (CPU_V != 0)
#define CONDITION_VS     (CPU_V != 0)
#define CONDITION_NOT_VS (CPU_V == 0)
#define CONDITION_PL     (CPU_N == 0)
#define CONDITION_NOT_PL (CPU_N != 0)
#define CONDITION_MI     (CPU_N != 0)
#define CONDITION_NOT_MI (CPU_N == 0)
#define CONDITION_GE     ((CPU_N == 0) == (CPU_V == 0))
#define CONDITION_NOT_GE ((CPU_N == 0) != (CPU_V == 0))
#define CONDITION_LT     ((CPU_N == 0) != (CPU_V == 0))
#define CONDITION_NOT_LT ((CPU_N == 0) == (CPU_V == 0))
#define CONDITION_GT     (CPU_NOT_Z != 0 && (CPU_N == 0) == (CPU_V == 0))
#define CONDITION_NOT_GT (CPU_NOT_Z == 0 || (CPU_N == 0) != (CPU_V == 0))
#define CONDITION_LE     (CPU_NOT_Z == 0 || (CPU_N == 0) != (CPU_V == 0))
#define CONDITION_NOT_LE (CPU_NOT_Z != 0 && (CPU_N == 0) == (CPU_V == 0))


/* Use up clock cycles.
 * NOTE: clock cycles used in here are 99.9% correct for a 68000, not for the
 * higher processors.
 */
#define USE_CLKS(A) num_clks += (A)


/* Push/pull data to/from the stack */
#define m68ki_push_16(A) m68ki_write_16(CPU_A[7]-=2, A)
#define m68ki_push_32(A) m68ki_write_32(CPU_A[7]-=4, A)
#define m68ki_pull_16()  m68ki_read_16((CPU_A[7]+=2) - 2)
#define m68ki_pull_32()  m68ki_read_32((CPU_A[7]+=4) - 4)


/* branch byte and word are for branches, while long is for jumps.
 * So far it's been safe to not call set_pc() for branch word.
 */
#define m68ki_branch_byte(A) CPU_PC += MAKE_INT_8(A)
#define m68ki_branch_word(A) CPU_PC += MAKE_INT_16(A)
#define m68ki_branch_dword(A) CPU_PC += (A)
#define m68ki_branch_long(A) m68ki_set_pc(A)


/* Get the condition code register */
#define m68ki_get_ccr() (((CPU_X != 0)     << 4) | \
                         ((CPU_N != 0)     << 3) | \
                         ((CPU_NOT_Z == 0) << 2) | \
                         ((CPU_V != 0)     << 1) | \
                          (CPU_C != 0))

/* Get the status register */
#define m68ki_get_sr() (((CPU_T1 != 0)    << 15) | \
                        ((CPU_T0 != 0)    << 14) | \
                        ((CPU_S != 0)     << 13) | \
                        ((CPU_M != 0)     << 12) | \
                         (CPU_INT_MASK    <<  8) | \
                        ((CPU_X != 0)     <<  4) | \
                        ((CPU_N != 0)     <<  3) | \
                        ((CPU_NOT_Z == 0) <<  2) | \
                        ((CPU_V != 0)     <<  1) | \
                         (CPU_C != 0))



/* ======================================================================== */
/* ========================= CONFIGURATION DEFINES ======================== */
/* ======================================================================== */

/* Act on values in m68kconf.h */
#if M68K_INT_ACK
#define m68ki_int_ack(A) CPU_INT_ACK_CALLBACK(A)
#else
/* Default action is to used autovector mode, which is most common */
#define m68ki_int_ack(A) M68K_INT_ACK_AUTOVECTOR
#endif /* M68K_INT_ACK */

#if M68K_BKPT_ACK
#define m68ki_bkpt_ack(A) CPU_BKPT_ACK_CALLBACK(A)
#else
#define m68ki_bkpt_ack(A)
#endif /* M68K_BKPT_ACK */

#if M68K_OUTPUT_RESET
#define m68ki_output_reset() CPU_RESET_INSTR_CALLBACK()
#else
#define m68ki_output_reset()
#endif /* M68K_OUTPUT_RESET */

#if M68K_PC_CHANGED
#define m68ki_pc_changed(A) CPU_PC_CHANGED_CALLBACK(A)
#else
#define m68ki_pc_changed(A)
#endif /* M68K_PC_CHANGED */

//#if M68K_SET_FC
//#define m68ki_set_fc(A) CPU_SET_FC_CALLBACK(A)
//#else
//#define m68ki_set_fc(A)
//#endif /* M68K_SET_FC */
#define m68ki_set_fc(A) m68k_cpu.fc = A

#if M68K_INSTR_HOOK
#define m68ki_instr_hook() CPU_INSTR_HOOK_CALLBACK()
#else
#define m68ki_instr_hook()
#endif /* M68K_INSTR_HOOK */

#if M68K_TRACE
/* Initiates trace checking before each instruction (t1) */
#define m68ki_set_trace() m68k_tracing = CPU_T1
/* adds t0 to trace checking if we encounter change of flow */
#define m68ki_add_trace() m68k_tracing |= CPU_T0
/* Clear all tracing */
#define m68ki_clear_trace() m68k_tracing = 0
/* Cause a trace exception if we are tracing */
#define m68ki_exception_if_trace() if(m68k_tracing) m68ki_exception(EXCEPTION_TRACE)
#else
#define m68ki_set_trace()
#define m68ki_add_trace()
#define m68ki_clear_trace()
#define m68ki_exception_if_trace()
#endif /* M68K_TRACE */


#ifdef M68K_LOG

extern char* m68k_disassemble_quick(uint pc);
extern uint  m68k_pc_offset;
extern char* m68k_cpu_names[];

#define M68K_DO_LOG(A) if(M68K_LOG) fprintf A
#if M68K_LOG_EMULATED_INSTRUCTIONS
#define M68K_DO_LOG_EMU(A) if(M68K_LOG) fprintf A
#else
#define M68K_DO_LOG_EMU(A)
#endif

#else
#define M68K_DO_LOG(A)
#define M68K_DO_LOG_EMU(A)

#endif

/* ======================================================================== */
/* =============================== PROTOTYPES ============================= */
/* ======================================================================== */
class mc68000 {

// Processor addressing modes
protected:

// Processor registers
public:
void (mc68000::*m68k_instruction_jump_table[0x10000])(void); /* opcode handler jump table */
void (mc68000::*g_instruction_table[0x10000])(void);
typedef struct
{
	void (mc68000::*opcode_handler)(void); /* handler function */
	uint bits;			/* number of bits set in mask */
	uint mask;			/* mask on opcode */
	uint match;			/* what to match after masking */
} opcode_handler_struct;
typedef struct
{
   void (mc68000::*opcode_handler)(void); /* handler function */
   uint mask;                    /* mask on opcode */
   uint match;                   /* what to match after masking */
   uint ea_mask;                 /* what ea modes are allowed */
} opcode_struct;

static opcode_handler_struct m68k_opcode_handler_table[];
static opcode_struct g_opcode_info[];
uint *m68k_cpu_dar[2];
uint* m68k_movem_pi_table[16];
uint* m68k_movem_pd_table[16];


  typedef struct
{
   uint mode;        /* CPU Operation Mode: 68000, 68010, or 68020 */
   uint dr[8];       /* Data Registers */
   uint ar[8];       /* Address Registers */
   uint ppc;		 /* Previous program counter */
   uint pc;          /* Program Counter */
   uint sp[4];       /* User, Interrupt, and Master Stack Pointers */
   uint vbr;         /* Vector Base Register (68010+) */
   uint sfc;         /* Source Function Code Register (m68010+) */
   uint dfc;         /* Destination Function Code Register (m68010+) */
   uint cacr;        /* Cache Control Register (m68020+) */
   uint caar;        /* Cacge Address Register (m68020+) */
   uint ir;          /* Instruction Register */
   uint t1_flag;     /* Trace 1 */
   uint t0_flag;     /* Trace 0 */
   uint s_flag;      /* Supervisor */
   uint m_flag;      /* Master/Interrupt state */
   uint x_flag;      /* Extend */
   uint n_flag;      /* Negative */
   uint not_z_flag;  /* Zero, inverted for speedups */
   uint v_flag;      /* Overflow */
   uint c_flag;      /* Carry */
   uint int_mask;    /* I0-I2 */
   uint int_state;   /* Current interrupt state -- ASG: changed from ints_pending */
   uint stopped;     /* Stopped state */
   uint halted;      /* Halted state */
   uint int_cycles;  /* ASG: extra cycles from generated interrupts */
   uint pref_addr;   /* Last prefetch address */
   uint pref_data;   /* Data in the prefetch queue */
   uint mbar;        /* Simm40 Module Base Address */
   bool mbar_valid;
   uint amask;
   uint fc;
   
   /* Callbacks to host */
   int  (*int_ack_callback)(int int_line);  /* Interrupt Acknowledge */
   void (*bkpt_ack_callback)(int data);     /* Breakpoint Acknowledge */
   void (*reset_instr_callback)(void);      /* Called when a RESET instruction is encountered */
   void (*pc_changed_callback)(int new_pc); /* Called when the PC changes by a large amount */
   void (*set_fc_callback)(int new_fc);     /* Called when the CPU function code changes */
   void (*instr_hook_callback)(void);       /* Called every instruction cycle prior to execution */

} m68k_cpu_core;

m68k_cpu_core m68k_cpu;
sim40_module *sim40;
timer_module *timer;
serial_module *serial;
dma_module *dma;
uint num_clks;

private:


protected:
	virtual int		execute(void);

public:
    mc68000();
    ~mc68000();
    int     dasm(int pc,char* str_buff);
    uint    getcs(uint address,uint fc);
    uint    getccr(void);

virtual int  m68k_read_immediate_8(int address, int cs) =0;
virtual int  m68k_read_immediate_16(int address, int cs) =0;
virtual int  m68k_read_immediate_32(int address, int cs) =0;

/* Read an instruction (16-bit word immeditately after PC) */
virtual int  m68k_read_instruction(int address, int cs) =0;

/* Write to anywhere */
virtual void m68k_write_memory_8(int address, int value, int cs)=0;
virtual void m68k_write_memory_16(int address, int value, int cs)=0;
virtual void m68k_write_memory_32(int address, int value, int cs)=0;

virtual int m68k_read_memory_8(int address, int cs)=0;
virtual int m68k_read_memory_16(int address, int cs)=0;
virtual int m68k_read_memory_32(int address, int cs)=0;

uint m68ki_read_8  (uint address);
uint m68ki_read_16 (uint address);
uint m68ki_read_32 (uint address);

/* Write to memory */
void m68ki_write_8 (uint address, uint value);
void m68ki_write_16(uint address, uint value);
void m68ki_write_32(uint address, uint value);

/* Read data immediately after the program counter */
uint m68ki_read_imm_8(void);
uint m68ki_read_imm_16(void);
uint m68ki_read_imm_32(void);

/* Reads the next word after the program counter */
uint m68ki_read_instruction(void);

/* Read data with specific function code */
uint m68ki_read_8_fc  (uint address, uint fc);
uint m68ki_read_16_fc (uint address, uint fc);
uint m68ki_read_32_fc (uint address, uint fc);

/* Write data with specific function code */
void m68ki_write_8_fc (uint address, uint fc, uint value);
void m68ki_write_16_fc(uint address, uint fc, uint value);
void m68ki_write_32_fc(uint address, uint fc, uint value);

void m68k_set_int_ack_callback(int  (*callback)(int int_level));
void m68k_set_bkpt_ack_callback(void  (*callback)(int data));
void m68k_set_reset_instr_callback(void  (*callback)(void));
void m68k_set_pc_changed_callback(void  (*callback)(int new_pc));
void m68k_set_fc_callback(void  (*callback)(int new_fc));
void m68k_set_instr_hook_callback(void  (*callback)(void));
void m68k_set_cpu_mode(int cpu_mode);
void m68k_assert_irq(int int_line);
void m68k_clear_irq(int int_line);
void reset(void);
void m68k_pulse_halt(void);
unsigned m68k_get_context(void* dst);
void m68k_set_context(void* src);
int m68k_is_valid_instruction(int instruction, int cpu_mode);

void m68ki_build_opcode_table(void);
uint m68ki_get_ea_ix(void);     /* Get ea for address register indirect + index */
uint m68ki_get_ea_pcix(void);   /* Get ea for program counter indirect + index */
uint m68ki_get_ea_ix_dst(void); /* Get ea ix for destination of move instruction */

void m68ki_set_s_flag(int value);                 /* Set the S flag */
void m68ki_set_m_flag(int value);                 /* Set the M flag */
void m68ki_set_sm_flag(int s_value, int m_value); /* Set the S and M flags */
void m68ki_set_ccr(uint value);                   /* set the condition code register */
void m68ki_set_sr(uint value);                    /* set the status register */
void m68ki_set_sr_no_int(uint value);             /* ASG: set the status register, but don't check interrupts */
void m68ki_set_pc(uint address);                  /* set the program counter */
void m68ki_service_interrupt(uint pending_mask);  /* service a pending interrupt -- ASG: added parameter */
void m68ki_exception(uint vector);                /* process an exception */
void m68ki_interrupt(uint vector);				 /* process an interrupt */
void m68ki_check_interrupts(void);
void signal_interrupt(uint mask, uint vector);


char *m68k_disassemble_quick(int pc);
char* make_signed_hex_str_8(uint val);
char* make_signed_hex_str_16(uint val);
char* make_signed_hex_str_32(uint val);

/* make string of ea mode */
char* get_ea_mode_str(uint instruction, uint size);

char* get_ea_mode_str_8(uint instruction);
char* get_ea_mode_str_16(uint instruction);
char* get_ea_mode_str_32(uint instruction);

/* make string of immediate value */
char* get_imm_str_s(uint size);
char* get_imm_str_u(uint size);

char* get_imm_str_s8(void);
char* get_imm_str_s16(void);
char* get_imm_str_s32(void);
int make_int_8(int value);
int make_int_16(int value);

int d68k_is_valid_instruction(int instruction);
void build_opcode_table(void);
void d68000_1010(void);
void d68000_1111(void);
void d68000_abcd_rr(void);
void d68000_abcd_mm(void);
void d68000_add_er_8(void);
void d68000_add_er_16(void);
void d68000_add_er_32(void);
void d68000_add_re_8(void);
void d68000_add_re_16(void);
void d68000_add_re_32(void);
void d68000_adda_16(void);
void d68000_adda_32(void);
void d68000_addi_8(void);
void d68000_addi_16(void);
void d68000_addi_32(void);
void d68000_addq_8(void);
void d68000_addq_16(void);
void d68000_addq_32(void);
void d68000_addx_rr_8(void);
void d68000_addx_rr_16(void);
void d68000_addx_rr_32(void);
void d68000_addx_mm_8(void);
void d68000_addx_mm_16(void);
void d68000_addx_mm_32(void);
void d68000_and_er_8(void);
void d68000_and_er_16(void);
void d68000_and_er_32(void);
void d68000_and_re_8(void);
void d68000_and_re_16(void);
void d68000_and_re_32(void);
void d68000_andi_8(void);
void d68000_andi_16(void);
void d68000_andi_32(void);
void d68000_andi_to_ccr(void);
void d68000_andi_to_sr(void);
void d68000_asr_s_8(void);
void d68000_asr_s_16(void);
void d68000_asr_s_32(void);
void d68000_asr_r_8(void);
void d68000_asr_r_16(void);
void d68000_asr_r_32(void);
void d68000_asr_ea(void);
void d68000_asl_s_8(void);
void d68000_asl_s_16(void);
void d68000_asl_s_32(void);
void d68000_asl_r_8(void);
void d68000_asl_r_16(void);
void d68000_asl_r_32(void);
void d68000_asl_ea(void);
void d68000_bcc_8(void);
void d68000_bcc_16(void);
void d68020_bcc_32(void);
void d68000_bchg_r(void);
void d68000_bchg_s(void);
void d68000_bclr_r(void);
void d68000_bclr_s(void);
void d68010_bkpt(void);
void d68020_bfchg(void);
void d68020_bfclr(void);
void d68020_bfexts(void);
void d68020_bfextu(void);
void d68020_bfffo(void);
void d68020_bfins(void);
void d68020_bfset(void);
void d68020_bftst(void);
void d68000_bra_8(void);
void d68000_bra_16(void);
void d68020_bra_32(void);
void d68000_bset_r(void);
void d68000_bset_s(void);
void d68000_bsr_8(void);
void d68000_bsr_16(void);
void d68020_bsr_32(void);
void d68000_btst_r(void);
void d68000_btst_s(void);
void d68020_callm(void);
void d68020_cas_8(void);
void d68020_cas_16(void);
void d68020_cas_32(void);
void d68020_cas2_16(void);
void d68020_cas2_32(void);
void d68000_chk_16(void);
void d68020_chk_32(void);
void d68020_chk2_cmp2_8(void);
void d68020_chk2_cmp2_16(void);
void d68020_chk2_cmp2_32(void);
void d68040_cinv(void);
void d68000_clr_8(void);
void d68000_clr_16(void);
void d68000_clr_32(void);
void d68000_cmp_8(void);
void d68000_cmp_16(void);
void d68000_cmp_32(void);
void d68000_cmpa_16(void);
void d68000_cmpa_32(void);
void d68000_cmpi_8(void);
void d68020_cmpi_pcdi_8(void);
void d68020_cmpi_pcix_8(void);
void d68000_cmpi_16(void);
void d68020_cmpi_pcdi_16(void);
void d68020_cmpi_pcix_16(void);
void d68000_cmpi_32(void);
void d68020_cmpi_pcdi_32(void);
void d68020_cmpi_pcix_32(void);
void d68000_cmpm_8(void);
void d68000_cmpm_16(void);
void d68000_cmpm_32(void);
void d68020_cpbcc_16(void);
void d68020_cpbcc_32(void);
void d68020_cpdbcc(void);
void d68020_cpgen(void);
void d68020_cprestore(void);
void d68020_cpsave(void);
void d68020_cpscc(void);
void d68020_cptrapcc_0(void);
void d68020_cptrapcc_16(void);
void d68020_cptrapcc_32(void);
void d68040_cpush(void);
void d68000_dbra(void);
void d68000_dbcc(void);
void d68000_divs(void);
void d68000_divu(void);
void d68020_divl(void);
void d68000_eor_8(void);
void d68000_eor_16(void);
void d68000_eor_32(void);
void d68000_eori_8(void);
void d68000_eori_16(void);
void d68000_eori_32(void);
void d68000_eori_to_ccr(void);
void d68000_eori_to_sr(void);
void d68000_exg_dd(void);
void d68000_exg_aa(void);
void d68000_exg_da(void);
void d68000_ext_16(void);
void d68000_ext_32(void);
void d68020_extb_32(void);
void d68000_jmp(void);
void d68000_jsr(void);
void d68000_lea(void);
void d68000_link_16(void);
void d68020_link_32(void);
void d68000_lsr_s_8(void);
void d68000_lsr_s_16(void);
void d68000_lsr_s_32(void);
void d68000_lsr_r_8(void);
void d68000_lsr_r_16(void);
void d68000_lsr_r_32(void);
void d68000_lsr_ea(void);
void d68000_lsl_s_8(void);
void d68000_lsl_s_16(void);
void d68000_lsl_s_32(void);
void d68000_lsl_r_8(void);
void d68000_lsl_r_16(void);
void d68000_lsl_r_32(void);
void d68000_lsl_ea(void);
void d68000_move_8(void);
void d68000_move_16(void);
void d68000_move_32(void);
void d68000_movea_16(void);
void d68000_movea_32(void);
void d68000_move_to_ccr(void);
void d68010_move_fr_ccr(void);
void d68000_move_fr_sr(void);
void d68000_move_to_sr(void);
void d68000_move_fr_usp(void);
void d68000_move_to_usp(void);
void d68010_movec(void);
void d68000_movem_pd_16(void);
void d68000_movem_pd_32(void);
void d68000_movem_er_16(void);
void d68000_movem_er_32(void);
void d68000_movem_re_16(void);
void d68000_movem_re_32(void);
void d68000_movep_re_16(void);
void d68000_movep_re_32(void);
void d68000_movep_er_16(void);
void d68000_movep_er_32(void);
void d68010_moves_8(void);
void d68010_moves_16(void);
void d68010_moves_32(void);
void d68000_moveq(void);
void d68040_move16_pi_pi(void);
void d68040_move16_pi_al(void);
void d68040_move16_al_pi(void);
void d68040_move16_ai_al(void);
void d68040_move16_al_ai(void);
void d68000_muls(void);
void d68000_mulu(void);
void d68020_mull(void);
void d68000_nbcd(void);
void d68000_neg_8(void);
void d68000_neg_16(void);
void d68000_neg_32(void);
void d68000_negx_8(void);
void d68000_negx_16(void);
void d68000_negx_32(void);
void d68000_nop(void);
void d68000_not_8(void);
void d68000_not_16(void);
void d68000_not_32(void);
void d68000_or_er_8(void);
void d68000_or_er_16(void);
void d68000_or_er_32(void);
void d68000_or_re_8(void);
void d68000_or_re_16(void);
void d68000_or_re_32(void);
void d68000_ori_8(void);
void d68000_ori_16(void);
void d68000_ori_32(void);
void d68000_ori_to_ccr(void);
void d68000_ori_to_sr(void);
void d68020_pack_rr(void);
void d68020_pack_mm(void);
void d68000_pea(void);
void d68000_reset(void);
void d68000_ror_s_8(void);
void d68000_ror_s_16(void);
void d68000_ror_s_32(void);
void d68000_ror_r_8(void);
void d68000_ror_r_16(void);
void d68000_ror_r_32(void);
void d68000_ror_ea(void);
void d68000_rol_s_8(void);
void d68000_rol_s_16(void);
void d68000_rol_s_32(void);
void d68000_rol_r_8(void);
void d68000_rol_r_16(void);
void d68000_rol_r_32(void);
void d68000_rol_ea(void);
void d68000_roxr_s_8(void);
void d68000_roxr_s_16(void);
void d68000_roxr_s_32(void);
void d68000_roxr_r_8(void);
void d68000_roxr_r_16(void);
void d68000_roxr_r_32(void);
void d68000_roxr_ea(void);
void d68000_roxl_s_8(void);
void d68000_roxl_s_16(void);
void d68000_roxl_s_32(void);
void d68000_roxl_r_8(void);
void d68000_roxl_r_16(void);
void d68000_roxl_r_32(void);
void d68000_roxl_ea(void) ;
void d68010_rtd(void);
void d68000_rte(void);
void d68020_rtm(void);
void d68000_rtr(void);
void d68000_rts(void);
void d68000_sbcd_rr(void);
void d68000_sbcd_mm(void);
void d68000_scc(void);
void d68000_stop(void);
void d68000_sub_er_8(void);
void d68000_sub_er_16(void);
void d68000_sub_er_32(void);
void d68000_sub_re_8(void);
void d68000_sub_re_16(void);
void d68000_sub_re_32(void);
void d68000_suba_16(void);
void d68000_suba_32(void);
void d68000_subi_8(void);
void d68000_subi_16(void);
void d68000_subi_32(void);
void d68000_subq_8(void);
void d68000_subq_16(void);
void d68000_subq_32(void);
void d68000_subx_rr_8(void);
void d68000_subx_rr_16(void);
void d68000_subx_rr_32(void);
void d68000_subx_mm_8(void);
void d68000_subx_mm_16(void);
void d68000_subx_mm_32(void);
void d68000_swap(void);
void d68000_tas(void);
void d68000_trap(void);
void d68020_trapcc_0(void);
void d68020_trapcc_16(void);
void d68020_trapcc_32(void);
void d68000_trapv(void);
void d68000_tst_8(void);
void d68020_tst_pcdi_8(void);
void d68020_tst_pcix_8(void);
void d68020_tst_i_8(void);
void d68000_tst_16(void);
void d68020_tst_a_16(void);
void d68020_tst_pcdi_16(void);
void d68020_tst_pcix_16(void);
void d68020_tst_i_16(void);
void d68000_tst_32(void);
void d68020_tst_a_32(void);
void d68020_tst_pcdi_32(void);
void d68020_tst_pcix_32(void);
void d68020_tst_i_32(void);
void d68000_unlk(void);
void d68020_unpk_rr(void);
void d68020_unpk_mm(void);
void d68000_illegal(void);



void m68000_1010(void);
void m68000_1111(void);
void m68000_abcd_rr(void);
void m68000_abcd_mm_ax7(void);
void m68000_abcd_mm_ay7(void);
void m68000_abcd_mm_axy7(void);
void m68000_abcd_mm(void);
void m68000_add_er_d_8(void);
void m68000_add_er_ai_8(void);
void m68000_add_er_pi_8(void);
void m68000_add_er_pi7_8(void);
void m68000_add_er_pd_8(void);
void m68000_add_er_pd7_8(void);
void m68000_add_er_di_8(void);
void m68000_add_er_ix_8(void);
void m68000_add_er_aw_8(void);
void m68000_add_er_al_8(void);
void m68000_add_er_pcdi_8(void);
void m68000_add_er_pcix_8(void);
void m68000_add_er_i_8(void);
void m68000_add_er_d_16(void);
void m68000_add_er_a_16(void);
void m68000_add_er_ai_16(void);
void m68000_add_er_pi_16(void);
void m68000_add_er_pd_16(void);
void m68000_add_er_di_16(void);
void m68000_add_er_ix_16(void);
void m68000_add_er_aw_16(void);
void m68000_add_er_al_16(void);
void m68000_add_er_pcdi_16(void);
void m68000_add_er_pcix_16(void);
void m68000_add_er_i_16(void);
void m68000_add_er_d_32(void);
void m68000_add_er_a_32(void);
void m68000_add_er_ai_32(void);
void m68000_add_er_pi_32(void);
void m68000_add_er_pd_32(void);
void m68000_add_er_di_32(void);
void m68000_add_er_ix_32(void);
void m68000_add_er_aw_32(void);
void m68000_add_er_al_32(void);
void m68000_add_er_pcdi_32(void);
void m68000_add_er_pcix_32(void);
void m68000_add_er_i_32(void);
void m68000_add_re_ai_8(void);
void m68000_add_re_pi_8(void);
void m68000_add_re_pi7_8(void);
void m68000_add_re_pd_8(void);
void m68000_add_re_pd7_8(void);
void m68000_add_re_di_8(void);
void m68000_add_re_ix_8(void);
void m68000_add_re_aw_8(void);
void m68000_add_re_al_8(void);
void m68000_add_re_ai_16(void);
void m68000_add_re_pi_16(void);
void m68000_add_re_pd_16(void);
void m68000_add_re_di_16(void);
void m68000_add_re_ix_16(void);
void m68000_add_re_aw_16(void);
void m68000_add_re_al_16(void);
void m68000_add_re_ai_32(void);
void m68000_add_re_pi_32(void);
void m68000_add_re_pd_32(void);
void m68000_add_re_di_32(void);
void m68000_add_re_ix_32(void);
void m68000_add_re_aw_32(void);
void m68000_add_re_al_32(void);
void m68000_adda_d_16(void);
void m68000_adda_a_16(void);
void m68000_adda_ai_16(void);
void m68000_adda_pi_16(void);
void m68000_adda_pd_16(void);
void m68000_adda_di_16(void);
void m68000_adda_ix_16(void);
void m68000_adda_aw_16(void);
void m68000_adda_al_16(void);
void m68000_adda_pcdi_16(void);
void m68000_adda_pcix_16(void);
void m68000_adda_i_16(void);
void m68000_adda_d_32(void);
void m68000_adda_a_32(void);
void m68000_adda_ai_32(void);
void m68000_adda_pi_32(void);
void m68000_adda_pd_32(void);
void m68000_adda_di_32(void);
void m68000_adda_ix_32(void);
void m68000_adda_aw_32(void);
void m68000_adda_al_32(void);
void m68000_adda_pcdi_32(void);
void m68000_adda_pcix_32(void);
void m68000_adda_i_32(void);
void m68000_addi_d_8(void);
void m68000_addi_ai_8(void);
void m68000_addi_pi_8(void);
void m68000_addi_pi7_8(void);
void m68000_addi_pd_8(void);
void m68000_addi_pd7_8(void);
void m68000_addi_di_8(void);
void m68000_addi_ix_8(void);
void m68000_addi_aw_8(void);
void m68000_addi_al_8(void);
void m68000_addi_d_16(void);
void m68000_addi_ai_16(void);
void m68000_addi_pi_16(void);
void m68000_addi_pd_16(void);
void m68000_addi_di_16(void);
void m68000_addi_ix_16(void);
void m68000_addi_aw_16(void);
void m68000_addi_al_16(void);
void m68000_addi_d_32(void);
void m68000_addi_ai_32(void);
void m68000_addi_pi_32(void);
void m68000_addi_pd_32(void);
void m68000_addi_di_32(void);
void m68000_addi_ix_32(void);
void m68000_addi_aw_32(void);
void m68000_addi_al_32(void);
void m68000_addq_d_8(void);
void m68000_addq_ai_8(void);
void m68000_addq_pi_8(void);
void m68000_addq_pi7_8(void);
void m68000_addq_pd_8(void);
void m68000_addq_pd7_8(void);
void m68000_addq_di_8(void);
void m68000_addq_ix_8(void);
void m68000_addq_aw_8(void);
void m68000_addq_al_8(void);
void m68000_addq_d_16(void);
void m68000_addq_a_16(void);
void m68000_addq_ai_16(void);
void m68000_addq_pi_16(void);
void m68000_addq_pd_16(void);
void m68000_addq_di_16(void);
void m68000_addq_ix_16(void);
void m68000_addq_aw_16(void);
void m68000_addq_al_16(void);
void m68000_addq_d_32(void);
void m68000_addq_a_32(void);
void m68000_addq_ai_32(void);
void m68000_addq_pi_32(void);
void m68000_addq_pd_32(void);
void m68000_addq_di_32(void);
void m68000_addq_ix_32(void);
void m68000_addq_aw_32(void);
void m68000_addq_al_32(void);
void m68000_addx_rr_8(void);
void m68000_addx_rr_16(void);
void m68000_addx_rr_32(void);
void m68000_addx_mm_8_ax7(void);
void m68000_addx_mm_8_ay7(void);
void m68000_addx_mm_8_axy7(void);
void m68000_addx_mm_8(void);
void m68000_addx_mm_16(void);
void m68000_addx_mm_32(void);
void m68000_and_er_d_8(void);
void m68000_and_er_ai_8(void);
void m68000_and_er_pi_8(void);
void m68000_and_er_pi7_8(void);
void m68000_and_er_pd_8(void);
void m68000_and_er_pd7_8(void);
void m68000_and_er_di_8(void);
void m68000_and_er_ix_8(void);
void m68000_and_er_aw_8(void);
void m68000_and_er_al_8(void);
void m68000_and_er_pcdi_8(void);
void m68000_and_er_pcix_8(void);
void m68000_and_er_i_8(void);
void m68000_and_er_d_16(void);
void m68000_and_er_ai_16(void);
void m68000_and_er_pi_16(void);
void m68000_and_er_pd_16(void);
void m68000_and_er_di_16(void);
void m68000_and_er_ix_16(void);
void m68000_and_er_aw_16(void);
void m68000_and_er_al_16(void);
void m68000_and_er_pcdi_16(void);
void m68000_and_er_pcix_16(void);
void m68000_and_er_i_16(void);
void m68000_and_er_d_32(void);
void m68000_and_er_ai_32(void);
void m68000_and_er_pi_32(void);
void m68000_and_er_pd_32(void);
void m68000_and_er_di_32(void);
void m68000_and_er_ix_32(void);
void m68000_and_er_aw_32(void);
void m68000_and_er_al_32(void);
void m68000_and_er_pcdi_32(void);
void m68000_and_er_pcix_32(void);
void m68000_and_er_i_32(void);
void m68000_and_re_ai_8(void);
void m68000_and_re_pi_8(void);
void m68000_and_re_pi7_8(void);
void m68000_and_re_pd_8(void);
void m68000_and_re_pd7_8(void);
void m68000_and_re_di_8(void);
void m68000_and_re_ix_8(void);
void m68000_and_re_aw_8(void);
void m68000_and_re_al_8(void);
void m68000_and_re_ai_16(void);
void m68000_and_re_pi_16(void);
void m68000_and_re_pd_16(void);
void m68000_and_re_di_16(void);
void m68000_and_re_ix_16(void);
void m68000_and_re_aw_16(void);
void m68000_and_re_al_16(void);
void m68000_and_re_ai_32(void);
void m68000_and_re_pi_32(void);
void m68000_and_re_pd_32(void);
void m68000_and_re_di_32(void);
void m68000_and_re_ix_32(void);
void m68000_and_re_aw_32(void);
void m68000_and_re_al_32(void);
void m68000_andi_to_ccr(void);
void m68000_andi_to_sr(void);
void m68000_andi_d_8(void);
void m68000_andi_ai_8(void);
void m68000_andi_pi_8(void);
void m68000_andi_pi7_8(void);
void m68000_andi_pd_8(void);
void m68000_andi_pd7_8(void);
void m68000_andi_di_8(void);
void m68000_andi_ix_8(void);
void m68000_andi_aw_8(void);
void m68000_andi_al_8(void);
void m68000_andi_d_16(void);
void m68000_andi_ai_16(void);
void m68000_andi_pi_16(void);
void m68000_andi_pd_16(void);
void m68000_andi_di_16(void);
void m68000_andi_ix_16(void);
void m68000_andi_aw_16(void);
void m68000_andi_al_16(void);
void m68000_andi_d_32(void);
void m68000_andi_ai_32(void);
void m68000_andi_pi_32(void);
void m68000_andi_pd_32(void);
void m68000_andi_di_32(void);
void m68000_andi_ix_32(void);
void m68000_andi_aw_32(void);
void m68000_andi_al_32(void);
void m68000_asr_s_8(void);
void m68000_asr_s_16(void);
void m68000_asr_s_32(void);
void m68000_asr_r_8(void);
void m68000_asr_r_16(void);
void m68000_asr_r_32(void);
void m68000_asr_ea_ai(void);
void m68000_asr_ea_pi(void);
void m68000_asr_ea_pd(void);
void m68000_asr_ea_di(void);
void m68000_asr_ea_ix(void);
void m68000_asr_ea_aw(void);
void m68000_asr_ea_al(void);
void m68000_asl_s_8(void);
void m68000_asl_s_16(void);
void m68000_asl_s_32(void);
void m68000_asl_r_8(void);
void m68000_asl_r_16(void);
void m68000_asl_r_32(void);
void m68000_asl_ea_ai(void);
void m68000_asl_ea_pi(void);
void m68000_asl_ea_pd(void);
void m68000_asl_ea_di(void);
void m68000_asl_ea_ix(void);
void m68000_asl_ea_aw(void);
void m68000_asl_ea_al(void);
void m68000_bhi_16(void);
void m68020_bhi_32(void);
void m68000_bhi_8(void);
void m68000_bls_16(void);
void m68020_bls_32(void);
void m68000_bls_8(void);
void m68000_bcc_16(void);
void m68020_bcc_32(void);
void m68000_bcc_8(void);
void m68000_bcs_16(void);
void m68020_bcs_32(void);
void m68000_bcs_8(void);
void m68000_bne_16(void);
void m68020_bne_32(void);
void m68000_bne_8(void);
void m68000_beq_16(void);
void m68020_beq_32(void);
void m68000_beq_8(void);
void m68000_bvc_16(void);
void m68020_bvc_32(void);
void m68000_bvc_8(void);
void m68000_bvs_16(void);
void m68020_bvs_32(void);
void m68000_bvs_8(void);
void m68000_bpl_16(void);
void m68020_bpl_32(void);
void m68000_bpl_8(void);
void m68000_bmi_16(void);
void m68020_bmi_32(void);
void m68000_bmi_8(void);
void m68000_bge_16(void);
void m68020_bge_32(void);
void m68000_bge_8(void);
void m68000_blt_16(void);
void m68020_blt_32(void);
void m68000_blt_8(void);
void m68000_bgt_16(void);
void m68020_bgt_32(void);
void m68000_bgt_8(void);
void m68000_ble_16(void);
void m68020_ble_32(void);
void m68000_ble_8(void);
void m68000_bchg_r_d(void);
void m68000_bchg_r_ai(void);
void m68000_bchg_r_pi(void);
void m68000_bchg_r_pi7(void);
void m68000_bchg_r_pd(void);
void m68000_bchg_r_pd7(void);
void m68000_bchg_r_di(void);
void m68000_bchg_r_ix(void);
void m68000_bchg_r_aw(void);
void m68000_bchg_r_al(void);
void m68000_bchg_s_d(void);
void m68000_bchg_s_ai(void);
void m68000_bchg_s_pi(void);
void m68000_bchg_s_pi7(void);
void m68000_bchg_s_pd(void);
void m68000_bchg_s_pd7(void);
void m68000_bchg_s_di(void);
void m68000_bchg_s_ix(void);
void m68000_bchg_s_aw(void);
void m68000_bchg_s_al(void);
void m68000_bclr_r_d(void);
void m68000_bclr_r_ai(void);
void m68000_bclr_r_pi(void);
void m68000_bclr_r_pi7(void);
void m68000_bclr_r_pd(void);
void m68000_bclr_r_pd7(void);
void m68000_bclr_r_di(void);
void m68000_bclr_r_ix(void);
void m68000_bclr_r_aw(void);
void m68000_bclr_r_al(void);
void m68000_bclr_s_d(void);
void m68000_bclr_s_ai(void);
void m68000_bclr_s_pi(void);
void m68000_bclr_s_pi7(void);
void m68000_bclr_s_pd(void);
void m68000_bclr_s_pd7(void);
void m68000_bclr_s_di(void);
void m68000_bclr_s_ix(void);
void m68000_bclr_s_aw(void);
void m68000_bclr_s_al(void);
void m68020_bfchg_d(void);
void m68020_bfchg_ai(void);
void m68020_bfchg_di(void);
void m68020_bfchg_ix(void);
void m68020_bfchg_aw(void);
void m68020_bfchg_al(void);
void m68020_bfclr_d(void);
void m68020_bfclr_ai(void);
void m68020_bfclr_di(void);
void m68020_bfclr_ix(void);
void m68020_bfclr_aw(void);
void m68020_bfclr_al(void);
void m68020_bfexts_d(void);
void m68020_bfexts_ai(void);
void m68020_bfexts_di(void);
void m68020_bfexts_ix(void);
void m68020_bfexts_aw(void);
void m68020_bfexts_al(void);
void m68020_bfexts_pcdi(void);
void m68020_bfexts_pcix(void);
void m68020_bfextu_d(void);
void m68020_bfextu_ai(void);
void m68020_bfextu_di(void);
void m68020_bfextu_ix(void);
void m68020_bfextu_aw(void);
void m68020_bfextu_al(void);
void m68020_bfextu_pcdi(void);
void m68020_bfextu_pcix(void);
void m68020_bfffo_d(void);
void m68020_bfffo_ai(void);
void m68020_bfffo_di(void);
void m68020_bfffo_ix(void);
void m68020_bfffo_aw(void);
void m68020_bfffo_al(void);
void m68020_bfffo_pcdi(void);
void m68020_bfffo_pcix(void);
void m68020_bfins_d(void);
void m68020_bfins_ai(void);
void m68020_bfins_di(void);
void m68020_bfins_ix(void);
void m68020_bfins_aw(void);
void m68020_bfins_al(void);
void m68020_bfset_d(void);
void m68020_bfset_ai(void);
void m68020_bfset_di(void);
void m68020_bfset_ix(void);
void m68020_bfset_aw(void);
void m68020_bfset_al(void);
void m68020_bftst_d(void);
void m68020_bftst_ai(void);
void m68020_bftst_di(void);
void m68020_bftst_ix(void);
void m68020_bftst_aw(void);
void m68020_bftst_al(void);
void m68020_bftst_pcdi(void);
void m68020_bftst_pcix(void);
void m68010_bkpt(void);
void m68000_bra_16(void);
void m68020_bra_32(void);
void m68000_bra_8(void);
void m68000_bset_r_d(void);
void m68000_bset_r_ai(void);
void m68000_bset_r_pi(void);
void m68000_bset_r_pi7(void);
void m68000_bset_r_pd(void);
void m68000_bset_r_pd7(void);
void m68000_bset_r_di(void);
void m68000_bset_r_ix(void);
void m68000_bset_r_aw(void);
void m68000_bset_r_al(void);
void m68000_bset_s_d(void);
void m68000_bset_s_ai(void);
void m68000_bset_s_pi(void);
void m68000_bset_s_pi7(void);
void m68000_bset_s_pd(void);
void m68000_bset_s_pd7(void);
void m68000_bset_s_di(void);
void m68000_bset_s_ix(void);
void m68000_bset_s_aw(void);
void m68000_bset_s_al(void);
void m68000_bsr_16(void);
void m68020_bsr_32(void);
void m68000_bsr_8(void);
void m68000_btst_r_d(void);
void m68000_btst_r_ai(void);
void m68000_btst_r_pi(void);
void m68000_btst_r_pi7(void);
void m68000_btst_r_pd(void);
void m68000_btst_r_pd7(void);
void m68000_btst_r_di(void);
void m68000_btst_r_ix(void);
void m68000_btst_r_aw(void);
void m68000_btst_r_al(void);
void m68000_btst_r_pcdi(void);
void m68000_btst_r_pcix(void);
void m68000_btst_r_i(void);
void m68000_btst_s_d(void);
void m68000_btst_s_ai(void);
void m68000_btst_s_pi(void);
void m68000_btst_s_pi7(void);
void m68000_btst_s_pd(void);
void m68000_btst_s_pd7(void);
void m68000_btst_s_di(void);
void m68000_btst_s_ix(void);
void m68000_btst_s_aw(void);
void m68000_btst_s_al(void);
void m68000_btst_s_pcdi(void);
void m68000_btst_s_pcix(void);
void m68020_callm_ai(void);
void m68020_callm_di(void);
void m68020_callm_ix(void);
void m68020_callm_aw(void);
void m68020_callm_al(void);
void m68020_callm_pcdi(void);
void m68020_callm_pcix(void);
void m68020_cas_ai_8(void);
void m68020_cas_pi_8(void);
void m68020_cas_pi7_8(void);
void m68020_cas_pd_8(void);
void m68020_cas_pd7_8(void);
void m68020_cas_di_8(void);
void m68020_cas_ix_8(void);
void m68020_cas_aw_8(void);
void m68020_cas_al_8(void);
void m68020_cas_ai_16(void);
void m68020_cas_pi_16(void);
void m68020_cas_pd_16(void);
void m68020_cas_di_16(void);
void m68020_cas_ix_16(void);
void m68020_cas_aw_16(void);
void m68020_cas_al_16(void);
void m68020_cas_ai_32(void);
void m68020_cas_pi_32(void);
void m68020_cas_pd_32(void);
void m68020_cas_di_32(void);
void m68020_cas_ix_32(void);
void m68020_cas_aw_32(void);
void m68020_cas_al_32(void);
void m68020_cas2_16(void);
void m68020_cas2_32(void);
void m68000_chk_d_16(void);
void m68000_chk_ai_16(void);
void m68000_chk_pi_16(void);
void m68000_chk_pd_16(void);
void m68000_chk_di_16(void);
void m68000_chk_ix_16(void);
void m68000_chk_aw_16(void);
void m68000_chk_al_16(void);
void m68000_chk_pcdi_16(void);
void m68000_chk_pcix_16(void);
void m68000_chk_i_16(void);
void m68020_chk_d_32(void);
void m68020_chk_ai_32(void);
void m68020_chk_pi_32(void);
void m68020_chk_pd_32(void);
void m68020_chk_di_32(void);
void m68020_chk_ix_32(void);
void m68020_chk_aw_32(void);
void m68020_chk_al_32(void);
void m68020_chk_pcdi_32(void);
void m68020_chk_pcix_32(void);
void m68020_chk_i_32(void);
void m68020_chk2_cmp2_ai_8(void);
void m68020_chk2_cmp2_di_8(void);
void m68020_chk2_cmp2_ix_8(void);
void m68020_chk2_cmp2_aw_8(void);
void m68020_chk2_cmp2_al_8(void);
void m68020_chk2_cmp2_pcdi_8(void);
void m68020_chk2_cmp2_pcix_8(void);
void m68020_chk2_cmp2_ai_16(void);
void m68020_chk2_cmp2_di_16(void);
void m68020_chk2_cmp2_ix_16(void);
void m68020_chk2_cmp2_aw_16(void);
void m68020_chk2_cmp2_al_16(void);
void m68020_chk2_cmp2_pcdi_16(void);
void m68020_chk2_cmp2_pcix_16(void);
void m68020_chk2_cmp2_ai_32(void);
void m68020_chk2_cmp2_di_32(void);
void m68020_chk2_cmp2_ix_32(void);
void m68020_chk2_cmp2_aw_32(void);
void m68020_chk2_cmp2_al_32(void);
void m68020_chk2_cmp2_pcdi_32(void);
void m68020_chk2_cmp2_pcix_32(void);
void m68000_clr_d_8(void);
void m68000_clr_ai_8(void);
void m68000_clr_pi_8(void);
void m68000_clr_pi7_8(void);
void m68000_clr_pd_8(void);
void m68000_clr_pd7_8(void);
void m68000_clr_di_8(void);
void m68000_clr_ix_8(void);
void m68000_clr_aw_8(void);
void m68000_clr_al_8(void);
void m68000_clr_d_16(void);
void m68000_clr_ai_16(void);
void m68000_clr_pi_16(void);
void m68000_clr_pd_16(void);
void m68000_clr_di_16(void);
void m68000_clr_ix_16(void);
void m68000_clr_aw_16(void);
void m68000_clr_al_16(void);
void m68000_clr_d_32(void);
void m68000_clr_ai_32(void);
void m68000_clr_pi_32(void);
void m68000_clr_pd_32(void);
void m68000_clr_di_32(void);
void m68000_clr_ix_32(void);
void m68000_clr_aw_32(void);
void m68000_clr_al_32(void);
void m68000_cmp_d_8(void);
void m68000_cmp_ai_8(void);
void m68000_cmp_pi_8(void);
void m68000_cmp_pi7_8(void);
void m68000_cmp_pd_8(void);
void m68000_cmp_pd7_8(void);
void m68000_cmp_di_8(void);
void m68000_cmp_ix_8(void);
void m68000_cmp_aw_8(void);
void m68000_cmp_al_8(void);
void m68000_cmp_pcdi_8(void);
void m68000_cmp_pcix_8(void);
void m68000_cmp_i_8(void);
void m68000_cmp_d_16(void);
void m68000_cmp_a_16(void);
void m68000_cmp_ai_16(void);
void m68000_cmp_pi_16(void);
void m68000_cmp_pd_16(void);
void m68000_cmp_di_16(void);
void m68000_cmp_ix_16(void);
void m68000_cmp_aw_16(void);
void m68000_cmp_al_16(void);
void m68000_cmp_pcdi_16(void);
void m68000_cmp_pcix_16(void);
void m68000_cmp_i_16(void);
void m68000_cmp_d_32(void);
void m68000_cmp_a_32(void);
void m68000_cmp_ai_32(void);
void m68000_cmp_pi_32(void);
void m68000_cmp_pd_32(void);
void m68000_cmp_di_32(void);
void m68000_cmp_ix_32(void);
void m68000_cmp_aw_32(void);
void m68000_cmp_al_32(void);
void m68000_cmp_pcdi_32(void);
void m68000_cmp_pcix_32(void);
void m68000_cmp_i_32(void);
void m68000_cmpa_d_16(void);
void m68000_cmpa_a_16(void);
void m68000_cmpa_ai_16(void);
void m68000_cmpa_pi_16(void);
void m68000_cmpa_pd_16(void);
void m68000_cmpa_di_16(void);
void m68000_cmpa_ix_16(void);
void m68000_cmpa_aw_16(void);
void m68000_cmpa_al_16(void);
void m68000_cmpa_pcdi_16(void);
void m68000_cmpa_pcix_16(void);
void m68000_cmpa_i_16(void);
void m68000_cmpa_d_32(void);
void m68000_cmpa_a_32(void);
void m68000_cmpa_ai_32(void);
void m68000_cmpa_pi_32(void);
void m68000_cmpa_pd_32(void);
void m68000_cmpa_di_32(void);
void m68000_cmpa_ix_32(void);
void m68000_cmpa_aw_32(void);
void m68000_cmpa_al_32(void);
void m68000_cmpa_pcdi_32(void);
void m68000_cmpa_pcix_32(void);
void m68000_cmpa_i_32(void);
void m68000_cmpi_d_8(void);
void m68000_cmpi_ai_8(void);
void m68000_cmpi_pi_8(void);
void m68000_cmpi_pi7_8(void);
void m68000_cmpi_pd_8(void);
void m68000_cmpi_pd7_8(void);
void m68000_cmpi_di_8(void);
void m68000_cmpi_ix_8(void);
void m68000_cmpi_aw_8(void);
void m68000_cmpi_al_8(void);
void m68020_cmpi_pcdi_8(void);
void m68020_cmpi_pcix_8(void);
void m68000_cmpi_d_16(void);
void m68000_cmpi_ai_16(void);
void m68000_cmpi_pi_16(void);
void m68000_cmpi_pd_16(void);
void m68000_cmpi_di_16(void);
void m68000_cmpi_ix_16(void);
void m68000_cmpi_aw_16(void);
void m68000_cmpi_al_16(void);
void m68020_cmpi_pcdi_16(void);
void m68020_cmpi_pcix_16(void);
void m68000_cmpi_d_32(void);
void m68000_cmpi_ai_32(void);
void m68000_cmpi_pi_32(void);
void m68000_cmpi_pd_32(void);
void m68000_cmpi_di_32(void);
void m68000_cmpi_ix_32(void);
void m68000_cmpi_aw_32(void);
void m68000_cmpi_al_32(void);
void m68020_cmpi_pcdi_32(void);
void m68020_cmpi_pcix_32(void);
void m68000_cmpm_8_ax7(void);
void m68000_cmpm_8_ay7(void);
void m68000_cmpm_8_axy7(void);
void m68000_cmpm_8(void);
void m68000_cmpm_16(void);
void m68000_cmpm_32(void);
void m68020_cpbcc(void);
void m68020_cpdbcc(void);
void m68020_cpgen(void);
void m68020_cpscc(void);
void m68020_cptrapcc(void);
void m68000_dbt(void);
void m68000_dbf(void);
void m68000_dbhi(void);
void m68000_dbls(void);
void m68000_dbcc(void);
void m68000_dbcs(void);
void m68000_dbne(void);
void m68000_dbeq(void);
void m68000_dbvc(void);
void m68000_dbvs(void);
void m68000_dbpl(void);
void m68000_dbmi(void);
void m68000_dbge(void);
void m68000_dblt(void);
void m68000_dbgt(void);
void m68000_dble(void);
void m68000_divs_d_16(void);
void m68000_divs_ai_16(void);
void m68000_divs_pi_16(void);
void m68000_divs_pd_16(void);
void m68000_divs_di_16(void);
void m68000_divs_ix_16(void);
void m68000_divs_aw_16(void);
void m68000_divs_al_16(void);
void m68000_divs_pcdi_16(void);
void m68000_divs_pcix_16(void);
void m68000_divs_i_16(void);
void m68000_divu_d_16(void);
void m68000_divu_ai_16(void);
void m68000_divu_pi_16(void);
void m68000_divu_pd_16(void);
void m68000_divu_di_16(void);
void m68000_divu_ix_16(void);
void m68000_divu_aw_16(void);
void m68000_divu_al_16(void);
void m68000_divu_pcdi_16(void);
void m68000_divu_pcix_16(void);
void m68000_divu_i_16(void);
void m68020_divl_d_32(void);
void m68020_divl_ai_32(void);
void m68020_divl_pi_32(void);
void m68020_divl_pd_32(void);
void m68020_divl_di_32(void);
void m68020_divl_ix_32(void);
void m68020_divl_aw_32(void);
void m68020_divl_al_32(void);
void m68020_divl_pcdi_32(void);
void m68020_divl_pcix_32(void);
void m68020_divl_i_32(void);
void m68000_eor_d_8(void);
void m68000_eor_ai_8(void);
void m68000_eor_pi_8(void);
void m68000_eor_pi7_8(void);
void m68000_eor_pd_8(void);
void m68000_eor_pd7_8(void);
void m68000_eor_di_8(void);
void m68000_eor_ix_8(void);
void m68000_eor_aw_8(void);
void m68000_eor_al_8(void);
void m68000_eor_d_16(void);
void m68000_eor_ai_16(void);
void m68000_eor_pi_16(void);
void m68000_eor_pd_16(void);
void m68000_eor_di_16(void);
void m68000_eor_ix_16(void);
void m68000_eor_aw_16(void);
void m68000_eor_al_16(void);
void m68000_eor_d_32(void);
void m68000_eor_ai_32(void);
void m68000_eor_pi_32(void);
void m68000_eor_pd_32(void);
void m68000_eor_di_32(void);
void m68000_eor_ix_32(void);
void m68000_eor_aw_32(void);
void m68000_eor_al_32(void);
void m68000_eori_to_ccr(void);
void m68000_eori_to_sr(void);
void m68000_eori_d_8(void);
void m68000_eori_ai_8(void);
void m68000_eori_pi_8(void);
void m68000_eori_pi7_8(void);
void m68000_eori_pd_8(void);
void m68000_eori_pd7_8(void);
void m68000_eori_di_8(void);
void m68000_eori_ix_8(void);
void m68000_eori_aw_8(void);
void m68000_eori_al_8(void);
void m68000_eori_d_16(void);
void m68000_eori_ai_16(void);
void m68000_eori_pi_16(void);
void m68000_eori_pd_16(void);
void m68000_eori_di_16(void);
void m68000_eori_ix_16(void);
void m68000_eori_aw_16(void);
void m68000_eori_al_16(void);
void m68000_eori_d_32(void);
void m68000_eori_ai_32(void);
void m68000_eori_pi_32(void);
void m68000_eori_pd_32(void);
void m68000_eori_di_32(void);
void m68000_eori_ix_32(void);
void m68000_eori_aw_32(void);
void m68000_eori_al_32(void);
void m68000_exg_dd(void);
void m68000_exg_aa(void);
void m68000_exg_da(void);
void m68000_ext_16(void);
void m68000_ext_32(void);
void m68020_extb(void);
void m68000_illegal(void);
void m68000_jmp_ai(void);
void m68000_jmp_di(void);
void m68000_jmp_ix(void);
void m68000_jmp_aw(void);
void m68000_jmp_al(void);
void m68000_jmp_pcdi(void);
void m68000_jmp_pcix(void);
void m68000_jsr_ai(void);
void m68000_jsr_di(void);
void m68000_jsr_ix(void);
void m68000_jsr_aw(void);
void m68000_jsr_al(void);
void m68000_jsr_pcdi(void);
void m68000_jsr_pcix(void);
void m68000_lea_ai(void);
void m68000_lea_di(void);
void m68000_lea_ix(void);
void m68000_lea_aw(void);
void m68000_lea_al(void);
void m68000_lea_pcdi(void);
void m68000_lea_pcix(void);
void m68000_link_16_a7(void);
void m68000_link_16(void);
void m68020_link_32_a7(void);
void m68020_link_32(void);
void m68000_lsr_s_8(void);
void m68000_lsr_s_16(void);
void m68000_lsr_s_32(void);
void m68000_lsr_r_8(void);
void m68000_lsr_r_16(void);
void m68000_lsr_r_32(void);
void m68000_lsr_ea_ai(void);
void m68000_lsr_ea_pi(void);
void m68000_lsr_ea_pd(void);
void m68000_lsr_ea_di(void);
void m68000_lsr_ea_ix(void);
void m68000_lsr_ea_aw(void);
void m68000_lsr_ea_al(void);
void m68000_lsl_s_8(void);
void m68000_lsl_s_16(void);
void m68000_lsl_s_32(void);
void m68000_lsl_r_8(void);
void m68000_lsl_r_16(void);
void m68000_lsl_r_32(void);
void m68000_lsl_ea_ai(void);
void m68000_lsl_ea_pi(void);
void m68000_lsl_ea_pd(void);
void m68000_lsl_ea_di(void);
void m68000_lsl_ea_ix(void);
void m68000_lsl_ea_aw(void);
void m68000_lsl_ea_al(void);
void m68000_move_dd_d_8(void);
void m68000_move_dd_ai_8(void);
void m68000_move_dd_pi_8(void);
void m68000_move_dd_pi7_8(void);
void m68000_move_dd_pd_8(void);
void m68000_move_dd_pd7_8(void);
void m68000_move_dd_di_8(void);
void m68000_move_dd_ix_8(void);
void m68000_move_dd_aw_8(void);
void m68000_move_dd_al_8(void);
void m68000_move_dd_pcdi_8(void);
void m68000_move_dd_pcix_8(void);
void m68000_move_dd_i_8(void);
void m68000_move_ai_d_8(void);
void m68000_move_ai_ai_8(void);
void m68000_move_ai_pi_8(void);
void m68000_move_ai_pi7_8(void);
void m68000_move_ai_pd_8(void);
void m68000_move_ai_pd7_8(void);
void m68000_move_ai_di_8(void);
void m68000_move_ai_ix_8(void);
void m68000_move_ai_aw_8(void);
void m68000_move_ai_al_8(void);
void m68000_move_ai_pcdi_8(void);
void m68000_move_ai_pcix_8(void);
void m68000_move_ai_i_8(void);
void m68000_move_pi_d_8(void);
void m68000_move_pi_ai_8(void);
void m68000_move_pi_pi_8(void);
void m68000_move_pi_pi7_8(void);
void m68000_move_pi_pd_8(void);
void m68000_move_pi_pd7_8(void);
void m68000_move_pi_di_8(void);
void m68000_move_pi_ix_8(void);
void m68000_move_pi_aw_8(void);
void m68000_move_pi_al_8(void);
void m68000_move_pi_pcdi_8(void);
void m68000_move_pi_pcix_8(void);
void m68000_move_pi_i_8(void);
void m68000_move_pi7_d_8(void);
void m68000_move_pi7_ai_8(void);
void m68000_move_pi7_pi_8(void);
void m68000_move_pi7_pi7_8(void);
void m68000_move_pi7_pd_8(void);
void m68000_move_pi7_pd7_8(void);
void m68000_move_pi7_di_8(void);
void m68000_move_pi7_ix_8(void);
void m68000_move_pi7_aw_8(void);
void m68000_move_pi7_al_8(void);
void m68000_move_pi7_pcdi_8(void);
void m68000_move_pi7_pcix_8(void);
void m68000_move_pi7_i_8(void);
void m68000_move_pd_d_8(void);
void m68000_move_pd_ai_8(void);
void m68000_move_pd_pi_8(void);
void m68000_move_pd_pi7_8(void);
void m68000_move_pd_pd_8(void);
void m68000_move_pd_pd7_8(void);
void m68000_move_pd_di_8(void);
void m68000_move_pd_ix_8(void);
void m68000_move_pd_aw_8(void);
void m68000_move_pd_al_8(void);
void m68000_move_pd_pcdi_8(void);
void m68000_move_pd_pcix_8(void);
void m68000_move_pd_i_8(void);
void m68000_move_pd7_d_8(void);
void m68000_move_pd7_ai_8(void);
void m68000_move_pd7_pi_8(void);
void m68000_move_pd7_pi7_8(void);
void m68000_move_pd7_pd_8(void);
void m68000_move_pd7_pd7_8(void);
void m68000_move_pd7_di_8(void);
void m68000_move_pd7_ix_8(void);
void m68000_move_pd7_aw_8(void);
void m68000_move_pd7_al_8(void);
void m68000_move_pd7_pcdi_8(void);
void m68000_move_pd7_pcix_8(void);
void m68000_move_pd7_i_8(void);
void m68000_move_di_d_8(void);
void m68000_move_di_ai_8(void);
void m68000_move_di_pi_8(void);
void m68000_move_di_pi7_8(void);
void m68000_move_di_pd_8(void);
void m68000_move_di_pd7_8(void);
void m68000_move_di_di_8(void);
void m68000_move_di_ix_8(void);
void m68000_move_di_aw_8(void);
void m68000_move_di_al_8(void);
void m68000_move_di_pcdi_8(void);
void m68000_move_di_pcix_8(void);
void m68000_move_di_i_8(void);
void m68000_move_ix_d_8(void);
void m68000_move_ix_ai_8(void);
void m68000_move_ix_pi_8(void);
void m68000_move_ix_pi7_8(void);
void m68000_move_ix_pd_8(void);
void m68000_move_ix_pd7_8(void);
void m68000_move_ix_di_8(void);
void m68000_move_ix_ix_8(void);
void m68000_move_ix_aw_8(void);
void m68000_move_ix_al_8(void);
void m68000_move_ix_pcdi_8(void);
void m68000_move_ix_pcix_8(void);
void m68000_move_ix_i_8(void);
void m68000_move_aw_d_8(void);
void m68000_move_aw_ai_8(void);
void m68000_move_aw_pi_8(void);
void m68000_move_aw_pi7_8(void);
void m68000_move_aw_pd_8(void);
void m68000_move_aw_pd7_8(void);
void m68000_move_aw_di_8(void);
void m68000_move_aw_ix_8(void);
void m68000_move_aw_aw_8(void);
void m68000_move_aw_al_8(void);
void m68000_move_aw_pcdi_8(void);
void m68000_move_aw_pcix_8(void);
void m68000_move_aw_i_8(void);
void m68000_move_al_d_8(void);
void m68000_move_al_ai_8(void);
void m68000_move_al_pi_8(void);
void m68000_move_al_pi7_8(void);
void m68000_move_al_pd_8(void);
void m68000_move_al_pd7_8(void);
void m68000_move_al_di_8(void);
void m68000_move_al_ix_8(void);
void m68000_move_al_aw_8(void);
void m68000_move_al_al_8(void);
void m68000_move_al_pcdi_8(void);
void m68000_move_al_pcix_8(void);
void m68000_move_al_i_8(void);
void m68000_move_dd_d_16(void);
void m68000_move_dd_a_16(void);
void m68000_move_dd_ai_16(void);
void m68000_move_dd_pi_16(void);
void m68000_move_dd_pd_16(void);
void m68000_move_dd_di_16(void);
void m68000_move_dd_ix_16(void);
void m68000_move_dd_aw_16(void);
void m68000_move_dd_al_16(void);
void m68000_move_dd_pcdi_16(void);
void m68000_move_dd_pcix_16(void);
void m68000_move_dd_i_16(void);
void m68000_move_ai_d_16(void);
void m68000_move_ai_a_16(void);
void m68000_move_ai_ai_16(void);
void m68000_move_ai_pi_16(void);
void m68000_move_ai_pd_16(void);
void m68000_move_ai_di_16(void);
void m68000_move_ai_ix_16(void);
void m68000_move_ai_aw_16(void);
void m68000_move_ai_al_16(void);
void m68000_move_ai_pcdi_16(void);
void m68000_move_ai_pcix_16(void);
void m68000_move_ai_i_16(void);
void m68000_move_pi_d_16(void);
void m68000_move_pi_a_16(void);
void m68000_move_pi_ai_16(void);
void m68000_move_pi_pi_16(void);
void m68000_move_pi_pd_16(void);
void m68000_move_pi_di_16(void);
void m68000_move_pi_ix_16(void);
void m68000_move_pi_aw_16(void);
void m68000_move_pi_al_16(void);
void m68000_move_pi_pcdi_16(void);
void m68000_move_pi_pcix_16(void);
void m68000_move_pi_i_16(void);
void m68000_move_pd_d_16(void);
void m68000_move_pd_a_16(void);
void m68000_move_pd_ai_16(void);
void m68000_move_pd_pi_16(void);
void m68000_move_pd_pd_16(void);
void m68000_move_pd_di_16(void);
void m68000_move_pd_ix_16(void);
void m68000_move_pd_aw_16(void);
void m68000_move_pd_al_16(void);
void m68000_move_pd_pcdi_16(void);
void m68000_move_pd_pcix_16(void);
void m68000_move_pd_i_16(void);
void m68000_move_di_d_16(void);
void m68000_move_di_a_16(void);
void m68000_move_di_ai_16(void);
void m68000_move_di_pi_16(void);
void m68000_move_di_pd_16(void);
void m68000_move_di_di_16(void);
void m68000_move_di_ix_16(void);
void m68000_move_di_aw_16(void);
void m68000_move_di_al_16(void);
void m68000_move_di_pcdi_16(void);
void m68000_move_di_pcix_16(void);
void m68000_move_di_i_16(void);
void m68000_move_ix_d_16(void);
void m68000_move_ix_a_16(void);
void m68000_move_ix_ai_16(void);
void m68000_move_ix_pi_16(void);
void m68000_move_ix_pd_16(void);
void m68000_move_ix_di_16(void);
void m68000_move_ix_ix_16(void);
void m68000_move_ix_aw_16(void);
void m68000_move_ix_al_16(void);
void m68000_move_ix_pcdi_16(void);
void m68000_move_ix_pcix_16(void);
void m68000_move_ix_i_16(void);
void m68000_move_aw_d_16(void);
void m68000_move_aw_a_16(void);
void m68000_move_aw_ai_16(void);
void m68000_move_aw_pi_16(void);
void m68000_move_aw_pd_16(void);
void m68000_move_aw_di_16(void);
void m68000_move_aw_ix_16(void);
void m68000_move_aw_aw_16(void);
void m68000_move_aw_al_16(void);
void m68000_move_aw_pcdi_16(void);
void m68000_move_aw_pcix_16(void);
void m68000_move_aw_i_16(void);
void m68000_move_al_d_16(void);
void m68000_move_al_a_16(void);
void m68000_move_al_ai_16(void);
void m68000_move_al_pi_16(void);
void m68000_move_al_pd_16(void);
void m68000_move_al_di_16(void);
void m68000_move_al_ix_16(void);
void m68000_move_al_aw_16(void);
void m68000_move_al_al_16(void);
void m68000_move_al_pcdi_16(void);
void m68000_move_al_pcix_16(void);
void m68000_move_al_i_16(void);
void m68000_move_dd_d_32(void);
void m68000_move_dd_a_32(void);
void m68000_move_dd_ai_32(void);
void m68000_move_dd_pi_32(void);
void m68000_move_dd_pd_32(void);
void m68000_move_dd_di_32(void);
void m68000_move_dd_ix_32(void);
void m68000_move_dd_aw_32(void);
void m68000_move_dd_al_32(void);
void m68000_move_dd_pcdi_32(void);
void m68000_move_dd_pcix_32(void);
void m68000_move_dd_i_32(void);
void m68000_move_ai_d_32(void);
void m68000_move_ai_a_32(void);
void m68000_move_ai_ai_32(void);
void m68000_move_ai_pi_32(void);
void m68000_move_ai_pd_32(void);
void m68000_move_ai_di_32(void);
void m68000_move_ai_ix_32(void);
void m68000_move_ai_aw_32(void);
void m68000_move_ai_al_32(void);
void m68000_move_ai_pcdi_32(void);
void m68000_move_ai_pcix_32(void);
void m68000_move_ai_i_32(void);
void m68000_move_pi_d_32(void);
void m68000_move_pi_a_32(void);
void m68000_move_pi_ai_32(void);
void m68000_move_pi_pi_32(void);
void m68000_move_pi_pd_32(void);
void m68000_move_pi_di_32(void);
void m68000_move_pi_ix_32(void);
void m68000_move_pi_aw_32(void);
void m68000_move_pi_al_32(void);
void m68000_move_pi_pcdi_32(void);
void m68000_move_pi_pcix_32(void);
void m68000_move_pi_i_32(void);
void m68000_move_pd_d_32(void);
void m68000_move_pd_a_32(void);
void m68000_move_pd_ai_32(void);
void m68000_move_pd_pi_32(void);
void m68000_move_pd_pd_32(void);
void m68000_move_pd_di_32(void);
void m68000_move_pd_ix_32(void);
void m68000_move_pd_aw_32(void);
void m68000_move_pd_al_32(void);
void m68000_move_pd_pcdi_32(void);
void m68000_move_pd_pcix_32(void);
void m68000_move_pd_i_32(void);
void m68000_move_di_d_32(void);
void m68000_move_di_a_32(void);
void m68000_move_di_ai_32(void);
void m68000_move_di_pi_32(void);
void m68000_move_di_pd_32(void);
void m68000_move_di_di_32(void);
void m68000_move_di_ix_32(void);
void m68000_move_di_aw_32(void);
void m68000_move_di_al_32(void);
void m68000_move_di_pcdi_32(void);
void m68000_move_di_pcix_32(void);
void m68000_move_di_i_32(void);
void m68000_move_ix_d_32(void);
void m68000_move_ix_a_32(void);
void m68000_move_ix_ai_32(void);
void m68000_move_ix_pi_32(void);
void m68000_move_ix_pd_32(void);
void m68000_move_ix_di_32(void);
void m68000_move_ix_ix_32(void);
void m68000_move_ix_aw_32(void);
void m68000_move_ix_al_32(void);
void m68000_move_ix_pcdi_32(void);
void m68000_move_ix_pcix_32(void);
void m68000_move_ix_i_32(void);
void m68000_move_aw_d_32(void);
void m68000_move_aw_a_32(void);
void m68000_move_aw_ai_32(void);
void m68000_move_aw_pi_32(void);
void m68000_move_aw_pd_32(void);
void m68000_move_aw_di_32(void);
void m68000_move_aw_ix_32(void);
void m68000_move_aw_aw_32(void);
void m68000_move_aw_al_32(void);
void m68000_move_aw_pcdi_32(void);
void m68000_move_aw_pcix_32(void);
void m68000_move_aw_i_32(void);
void m68000_move_al_d_32(void);
void m68000_move_al_a_32(void);
void m68000_move_al_ai_32(void);
void m68000_move_al_pi_32(void);
void m68000_move_al_pd_32(void);
void m68000_move_al_di_32(void);
void m68000_move_al_ix_32(void);
void m68000_move_al_aw_32(void);
void m68000_move_al_al_32(void);
void m68000_move_al_pcdi_32(void);
void m68000_move_al_pcix_32(void);
void m68000_move_al_i_32(void);
void m68000_movea_d_16(void);
void m68000_movea_a_16(void);
void m68000_movea_ai_16(void);
void m68000_movea_pi_16(void);
void m68000_movea_pd_16(void);
void m68000_movea_di_16(void);
void m68000_movea_ix_16(void);
void m68000_movea_aw_16(void);
void m68000_movea_al_16(void);
void m68000_movea_pcdi_16(void);
void m68000_movea_pcix_16(void);
void m68000_movea_i_16(void);
void m68000_movea_d_32(void);
void m68000_movea_a_32(void);
void m68000_movea_ai_32(void);
void m68000_movea_pi_32(void);
void m68000_movea_pd_32(void);
void m68000_movea_di_32(void);
void m68000_movea_ix_32(void);
void m68000_movea_aw_32(void);
void m68000_movea_al_32(void);
void m68000_movea_pcdi_32(void);
void m68000_movea_pcix_32(void);
void m68000_movea_i_32(void);
void m68010_move_fr_ccr_d(void);
void m68010_move_fr_ccr_ai(void);
void m68010_move_fr_ccr_pi(void);
void m68010_move_fr_ccr_pd(void);
void m68010_move_fr_ccr_di(void);
void m68010_move_fr_ccr_ix(void);
void m68010_move_fr_ccr_aw(void);
void m68010_move_fr_ccr_al(void);
void m68000_move_to_ccr_d(void);
void m68000_move_to_ccr_ai(void);
void m68000_move_to_ccr_pi(void);
void m68000_move_to_ccr_pd(void);
void m68000_move_to_ccr_di(void);
void m68000_move_to_ccr_ix(void);
void m68000_move_to_ccr_aw(void);
void m68000_move_to_ccr_al(void);
void m68000_move_to_ccr_pcdi(void);
void m68000_move_to_ccr_pcix(void);
void m68000_move_to_ccr_i(void);
void m68000_move_fr_sr_d(void);
void m68000_move_fr_sr_ai(void);
void m68000_move_fr_sr_pi(void);
void m68000_move_fr_sr_pd(void);
void m68000_move_fr_sr_di(void);
void m68000_move_fr_sr_ix(void);
void m68000_move_fr_sr_aw(void);
void m68000_move_fr_sr_al(void);
void m68000_move_to_sr_d(void);
void m68000_move_to_sr_ai(void);
void m68000_move_to_sr_pi(void);
void m68000_move_to_sr_pd(void);
void m68000_move_to_sr_di(void);
void m68000_move_to_sr_ix(void);
void m68000_move_to_sr_aw(void);
void m68000_move_to_sr_al(void);
void m68000_move_to_sr_pcdi(void);
void m68000_move_to_sr_pcix(void);
void m68000_move_to_sr_i(void);
void m68000_move_fr_usp(void);
void m68000_move_to_usp(void);
void m68010_movec_cr(void);
void m68010_movec_rc(void);
void m68000_movem_pd_16(void);
void m68000_movem_pd_32(void);
void m68000_movem_pi_16(void);
void m68000_movem_pi_32(void);
void m68000_movem_re_ai_16(void);
void m68000_movem_re_di_16(void);
void m68000_movem_re_ix_16(void);
void m68000_movem_re_aw_16(void);
void m68000_movem_re_al_16(void);
void m68000_movem_re_ai_32(void);
void m68000_movem_re_di_32(void);
void m68000_movem_re_ix_32(void);
void m68000_movem_re_aw_32(void);
void m68000_movem_re_al_32(void);
void m68000_movem_er_ai_16(void);
void m68000_movem_er_di_16(void);
void m68000_movem_er_ix_16(void);
void m68000_movem_er_aw_16(void);
void m68000_movem_er_al_16(void);
void m68000_movem_er_pcdi_16(void);
void m68000_movem_er_pcix_16(void);
void m68000_movem_er_ai_32(void);
void m68000_movem_er_di_32(void);
void m68000_movem_er_ix_32(void);
void m68000_movem_er_aw_32(void);
void m68000_movem_er_al_32(void);
void m68000_movem_er_pcdi_32(void);
void m68000_movem_er_pcix_32(void);
void m68000_movep_er_16(void);
void m68000_movep_er_32(void);
void m68000_movep_re_16(void);
void m68000_movep_re_32(void);
void m68010_moves_ai_8(void);
void m68010_moves_pi_8(void);
void m68010_moves_pi7_8(void);
void m68010_moves_pd_8(void);
void m68010_moves_pd7_8(void);
void m68010_moves_di_8(void);
void m68010_moves_ix_8(void);
void m68010_moves_aw_8(void);
void m68010_moves_al_8(void);
void m68010_moves_ai_16(void);
void m68010_moves_pi_16(void);
void m68010_moves_pd_16(void);
void m68010_moves_di_16(void);
void m68010_moves_ix_16(void);
void m68010_moves_aw_16(void);
void m68010_moves_al_16(void);
void m68010_moves_ai_32(void);
void m68010_moves_pi_32(void);
void m68010_moves_pd_32(void);
void m68010_moves_di_32(void);
void m68010_moves_ix_32(void);
void m68010_moves_aw_32(void);
void m68010_moves_al_32(void);
void m68000_moveq(void);
void m68000_muls_d_16(void);
void m68000_muls_ai_16(void);
void m68000_muls_pi_16(void);
void m68000_muls_pd_16(void);
void m68000_muls_di_16(void);
void m68000_muls_ix_16(void);
void m68000_muls_aw_16(void);
void m68000_muls_al_16(void);
void m68000_muls_pcdi_16(void);
void m68000_muls_pcix_16(void);
void m68000_muls_i_16(void);
void m68000_mulu_d_16(void);
void m68000_mulu_ai_16(void);
void m68000_mulu_pi_16(void);
void m68000_mulu_pd_16(void);
void m68000_mulu_di_16(void);
void m68000_mulu_ix_16(void);
void m68000_mulu_aw_16(void);
void m68000_mulu_al_16(void);
void m68000_mulu_pcdi_16(void);
void m68000_mulu_pcix_16(void);
void m68000_mulu_i_16(void);
void m68020_mull_d_32(void);
void m68020_mull_ai_32(void);
void m68020_mull_pi_32(void);
void m68020_mull_pd_32(void);
void m68020_mull_di_32(void);
void m68020_mull_ix_32(void);
void m68020_mull_aw_32(void);
void m68020_mull_al_32(void);
void m68020_mull_pcdi_32(void);
void m68020_mull_pcix_32(void);
void m68020_mull_i_32(void);
void m68000_nbcd_d(void);
void m68000_nbcd_ai(void);
void m68000_nbcd_pi(void);
void m68000_nbcd_pi7(void);
void m68000_nbcd_pd(void);
void m68000_nbcd_pd7(void);
void m68000_nbcd_di(void);
void m68000_nbcd_ix(void);
void m68000_nbcd_aw(void);
void m68000_nbcd_al(void);
void m68000_neg_d_8(void);
void m68000_neg_ai_8(void);
void m68000_neg_pi_8(void);
void m68000_neg_pi7_8(void);
void m68000_neg_pd_8(void);
void m68000_neg_pd7_8(void);
void m68000_neg_di_8(void);
void m68000_neg_ix_8(void);
void m68000_neg_aw_8(void);
void m68000_neg_al_8(void);
void m68000_neg_d_16(void);
void m68000_neg_ai_16(void);
void m68000_neg_pi_16(void);
void m68000_neg_pd_16(void);
void m68000_neg_di_16(void);
void m68000_neg_ix_16(void);
void m68000_neg_aw_16(void);
void m68000_neg_al_16(void);
void m68000_neg_d_32(void);
void m68000_neg_ai_32(void);
void m68000_neg_pi_32(void);
void m68000_neg_pd_32(void);
void m68000_neg_di_32(void);
void m68000_neg_ix_32(void);
void m68000_neg_aw_32(void);
void m68000_neg_al_32(void);
void m68000_negx_d_8(void);
void m68000_negx_ai_8(void);
void m68000_negx_pi_8(void);
void m68000_negx_pi7_8(void);
void m68000_negx_pd_8(void);
void m68000_negx_pd7_8(void);
void m68000_negx_di_8(void);
void m68000_negx_ix_8(void);
void m68000_negx_aw_8(void);
void m68000_negx_al_8(void);
void m68000_negx_d_16(void);
void m68000_negx_ai_16(void);
void m68000_negx_pi_16(void);
void m68000_negx_pd_16(void);
void m68000_negx_di_16(void);
void m68000_negx_ix_16(void);
void m68000_negx_aw_16(void);
void m68000_negx_al_16(void);
void m68000_negx_d_32(void);
void m68000_negx_ai_32(void);
void m68000_negx_pi_32(void);
void m68000_negx_pd_32(void);
void m68000_negx_di_32(void);
void m68000_negx_ix_32(void);
void m68000_negx_aw_32(void);
void m68000_negx_al_32(void);
void m68000_nop(void);
void m68000_not_d_8(void);
void m68000_not_ai_8(void);
void m68000_not_pi_8(void);
void m68000_not_pi7_8(void);
void m68000_not_pd_8(void);
void m68000_not_pd7_8(void);
void m68000_not_di_8(void);
void m68000_not_ix_8(void);
void m68000_not_aw_8(void);
void m68000_not_al_8(void);
void m68000_not_d_16(void);
void m68000_not_ai_16(void);
void m68000_not_pi_16(void);
void m68000_not_pd_16(void);
void m68000_not_di_16(void);
void m68000_not_ix_16(void);
void m68000_not_aw_16(void);
void m68000_not_al_16(void);
void m68000_not_d_32(void);
void m68000_not_ai_32(void);
void m68000_not_pi_32(void);
void m68000_not_pd_32(void);
void m68000_not_di_32(void);
void m68000_not_ix_32(void);
void m68000_not_aw_32(void);
void m68000_not_al_32(void);
void m68000_or_er_d_8(void);
void m68000_or_er_ai_8(void);
void m68000_or_er_pi_8(void);
void m68000_or_er_pi7_8(void);
void m68000_or_er_pd_8(void);
void m68000_or_er_pd7_8(void);
void m68000_or_er_di_8(void);
void m68000_or_er_ix_8(void);
void m68000_or_er_aw_8(void);
void m68000_or_er_al_8(void);
void m68000_or_er_pcdi_8(void);
void m68000_or_er_pcix_8(void);
void m68000_or_er_i_8(void);
void m68000_or_er_d_16(void);
void m68000_or_er_ai_16(void);
void m68000_or_er_pi_16(void);
void m68000_or_er_pd_16(void);
void m68000_or_er_di_16(void);
void m68000_or_er_ix_16(void);
void m68000_or_er_aw_16(void);
void m68000_or_er_al_16(void);
void m68000_or_er_pcdi_16(void);
void m68000_or_er_pcix_16(void);
void m68000_or_er_i_16(void);
void m68000_or_er_d_32(void);
void m68000_or_er_ai_32(void);
void m68000_or_er_pi_32(void);
void m68000_or_er_pd_32(void);
void m68000_or_er_di_32(void);
void m68000_or_er_ix_32(void);
void m68000_or_er_aw_32(void);
void m68000_or_er_al_32(void);
void m68000_or_er_pcdi_32(void);
void m68000_or_er_pcix_32(void);
void m68000_or_er_i_32(void);
void m68000_or_re_ai_8(void);
void m68000_or_re_pi_8(void);
void m68000_or_re_pi7_8(void);
void m68000_or_re_pd_8(void);
void m68000_or_re_pd7_8(void);
void m68000_or_re_di_8(void);
void m68000_or_re_ix_8(void);
void m68000_or_re_aw_8(void);
void m68000_or_re_al_8(void);
void m68000_or_re_ai_16(void);
void m68000_or_re_pi_16(void);
void m68000_or_re_pd_16(void);
void m68000_or_re_di_16(void);
void m68000_or_re_ix_16(void);
void m68000_or_re_aw_16(void);
void m68000_or_re_al_16(void);
void m68000_or_re_ai_32(void);
void m68000_or_re_pi_32(void);
void m68000_or_re_pd_32(void);
void m68000_or_re_di_32(void);
void m68000_or_re_ix_32(void);
void m68000_or_re_aw_32(void);
void m68000_or_re_al_32(void);
void m68000_ori_to_ccr(void);
void m68000_ori_to_sr(void);
void m68000_ori_d_8(void);
void m68000_ori_ai_8(void);
void m68000_ori_pi_8(void);
void m68000_ori_pi7_8(void);
void m68000_ori_pd_8(void);
void m68000_ori_pd7_8(void);
void m68000_ori_di_8(void);
void m68000_ori_ix_8(void);
void m68000_ori_aw_8(void);
void m68000_ori_al_8(void);
void m68000_ori_d_16(void);
void m68000_ori_ai_16(void);
void m68000_ori_pi_16(void);
void m68000_ori_pd_16(void);
void m68000_ori_di_16(void);
void m68000_ori_ix_16(void);
void m68000_ori_aw_16(void);
void m68000_ori_al_16(void);
void m68000_ori_d_32(void);
void m68000_ori_ai_32(void);
void m68000_ori_pi_32(void);
void m68000_ori_pd_32(void);
void m68000_ori_di_32(void);
void m68000_ori_ix_32(void);
void m68000_ori_aw_32(void);
void m68000_ori_al_32(void);
void m68020_pack_rr(void);
void m68020_pack_mm_ax7(void);
void m68020_pack_mm_ay7(void);
void m68020_pack_mm_axy7(void);
void m68020_pack_mm(void);
void m68000_pea_ai(void);
void m68000_pea_di(void);
void m68000_pea_ix(void);
void m68000_pea_aw(void);
void m68000_pea_al(void);
void m68000_pea_pcdi(void);
void m68000_pea_pcix(void);
void m68000_rst(void);
void m68000_ror_s_8(void);
void m68000_ror_s_16(void);
void m68000_ror_s_32(void);
void m68000_ror_r_8(void);
void m68000_ror_r_16(void);
void m68000_ror_r_32(void);
void m68000_ror_ea_ai(void);
void m68000_ror_ea_pi(void);
void m68000_ror_ea_pd(void);
void m68000_ror_ea_di(void);
void m68000_ror_ea_ix(void);
void m68000_ror_ea_aw(void);
void m68000_ror_ea_al(void);
void m68000_rol_s_8(void);
void m68000_rol_s_16(void);
void m68000_rol_s_32(void);
void m68000_rol_r_8(void);
void m68000_rol_r_16(void);
void m68000_rol_r_32(void);
void m68000_rol_ea_ai(void);
void m68000_rol_ea_pi(void);
void m68000_rol_ea_pd(void);
void m68000_rol_ea_di(void);
void m68000_rol_ea_ix(void);
void m68000_rol_ea_aw(void);
void m68000_rol_ea_al(void);
void m68000_roxr_s_8(void);
void m68000_roxr_s_16(void);
void m68000_roxr_s_32(void);
void m68000_roxr_r_8(void);
void m68000_roxr_r_16(void);
void m68000_roxr_r_32(void);
void m68000_roxr_ea_ai(void);
void m68000_roxr_ea_pi(void);
void m68000_roxr_ea_pd(void);
void m68000_roxr_ea_di(void);
void m68000_roxr_ea_ix(void);
void m68000_roxr_ea_aw(void);
void m68000_roxr_ea_al(void);
void m68000_roxl_s_8(void);
void m68000_roxl_s_16(void);
void m68000_roxl_s_32(void);
void m68000_roxl_r_8(void);
void m68000_roxl_r_16(void);
void m68000_roxl_r_32(void);
void m68000_roxl_ea_ai(void);
void m68000_roxl_ea_pi(void);
void m68000_roxl_ea_pd(void);
void m68000_roxl_ea_di(void);
void m68000_roxl_ea_ix(void);
void m68000_roxl_ea_aw(void);
void m68000_roxl_ea_al(void);
void m68010_rtd(void);
void m68000_rte(void);
void m68020_rtm(void);
void m68000_rtr(void);
void m68000_rts(void);
void m68000_sbcd_rr(void);
void m68000_sbcd_mm_ax7(void);
void m68000_sbcd_mm_ay7(void);
void m68000_sbcd_mm_axy7(void);
void m68000_sbcd_mm(void);
void m68000_st_d(void);
void m68000_st_ai(void);
void m68000_st_pi(void);
void m68000_st_pi7(void);
void m68000_st_pd(void);
void m68000_st_pd7(void);
void m68000_st_di(void);
void m68000_st_ix(void);
void m68000_st_aw(void);
void m68000_st_al(void);
void m68000_sf_d(void);
void m68000_sf_ai(void);
void m68000_sf_pi(void);
void m68000_sf_pi7(void);
void m68000_sf_pd(void);
void m68000_sf_pd7(void);
void m68000_sf_di(void);
void m68000_sf_ix(void);
void m68000_sf_aw(void);
void m68000_sf_al(void);
void m68000_shi_d(void);
void m68000_shi_ai(void);
void m68000_shi_pi(void);
void m68000_shi_pi7(void);
void m68000_shi_pd(void);
void m68000_shi_pd7(void);
void m68000_shi_di(void);
void m68000_shi_ix(void);
void m68000_shi_aw(void);
void m68000_shi_al(void);
void m68000_sls_d(void);
void m68000_sls_ai(void);
void m68000_sls_pi(void);
void m68000_sls_pi7(void);
void m68000_sls_pd(void);
void m68000_sls_pd7(void);
void m68000_sls_di(void);
void m68000_sls_ix(void);
void m68000_sls_aw(void);
void m68000_sls_al(void);
void m68000_scc_d(void);
void m68000_scc_ai(void);
void m68000_scc_pi(void);
void m68000_scc_pi7(void);
void m68000_scc_pd(void);
void m68000_scc_pd7(void);
void m68000_scc_di(void);
void m68000_scc_ix(void);
void m68000_scc_aw(void);
void m68000_scc_al(void);
void m68000_scs_d(void);
void m68000_scs_ai(void);
void m68000_scs_pi(void);
void m68000_scs_pi7(void);
void m68000_scs_pd(void);
void m68000_scs_pd7(void);
void m68000_scs_di(void);
void m68000_scs_ix(void);
void m68000_scs_aw(void);
void m68000_scs_al(void);
void m68000_sne_d(void);
void m68000_sne_ai(void);
void m68000_sne_pi(void);
void m68000_sne_pi7(void);
void m68000_sne_pd(void);
void m68000_sne_pd7(void);
void m68000_sne_di(void);
void m68000_sne_ix(void);
void m68000_sne_aw(void);
void m68000_sne_al(void);
void m68000_seq_d(void);
void m68000_seq_ai(void);
void m68000_seq_pi(void);
void m68000_seq_pi7(void);
void m68000_seq_pd(void);
void m68000_seq_pd7(void);
void m68000_seq_di(void);
void m68000_seq_ix(void);
void m68000_seq_aw(void);
void m68000_seq_al(void);
void m68000_svc_d(void);
void m68000_svc_ai(void);
void m68000_svc_pi(void);
void m68000_svc_pi7(void);
void m68000_svc_pd(void);
void m68000_svc_pd7(void);
void m68000_svc_di(void);
void m68000_svc_ix(void);
void m68000_svc_aw(void);
void m68000_svc_al(void);
void m68000_svs_d(void);
void m68000_svs_ai(void);
void m68000_svs_pi(void);
void m68000_svs_pi7(void);
void m68000_svs_pd(void);
void m68000_svs_pd7(void);
void m68000_svs_di(void);
void m68000_svs_ix(void);
void m68000_svs_aw(void);
void m68000_svs_al(void);
void m68000_spl_d(void);
void m68000_spl_ai(void);
void m68000_spl_pi(void);
void m68000_spl_pi7(void);
void m68000_spl_pd(void);
void m68000_spl_pd7(void);
void m68000_spl_di(void);
void m68000_spl_ix(void);
void m68000_spl_aw(void);
void m68000_spl_al(void);
void m68000_smi_d(void);
void m68000_smi_ai(void);
void m68000_smi_pi(void);
void m68000_smi_pi7(void);
void m68000_smi_pd(void);
void m68000_smi_pd7(void);
void m68000_smi_di(void);
void m68000_smi_ix(void);
void m68000_smi_aw(void);
void m68000_smi_al(void);
void m68000_sge_d(void);
void m68000_sge_ai(void);
void m68000_sge_pi(void);
void m68000_sge_pi7(void);
void m68000_sge_pd(void);
void m68000_sge_pd7(void);
void m68000_sge_di(void);
void m68000_sge_ix(void);
void m68000_sge_aw(void);
void m68000_sge_al(void);
void m68000_slt_d(void);
void m68000_slt_ai(void);
void m68000_slt_pi(void);
void m68000_slt_pi7(void);
void m68000_slt_pd(void);
void m68000_slt_pd7(void);
void m68000_slt_di(void);
void m68000_slt_ix(void);
void m68000_slt_aw(void);
void m68000_slt_al(void);
void m68000_sgt_d(void);
void m68000_sgt_ai(void);
void m68000_sgt_pi(void);
void m68000_sgt_pi7(void);
void m68000_sgt_pd(void);
void m68000_sgt_pd7(void);
void m68000_sgt_di(void);
void m68000_sgt_ix(void);
void m68000_sgt_aw(void);
void m68000_sgt_al(void);
void m68000_sle_d(void);
void m68000_sle_ai(void);
void m68000_sle_pi(void);
void m68000_sle_pi7(void);
void m68000_sle_pd(void);
void m68000_sle_pd7(void);
void m68000_sle_di(void);
void m68000_sle_ix(void);
void m68000_sle_aw(void);
void m68000_sle_al(void);
void m68000_stop(void);
void m68000_sub_er_d_8(void);
void m68000_sub_er_ai_8(void);
void m68000_sub_er_pi_8(void);
void m68000_sub_er_pi7_8(void);
void m68000_sub_er_pd_8(void);
void m68000_sub_er_pd7_8(void);
void m68000_sub_er_di_8(void);
void m68000_sub_er_ix_8(void);
void m68000_sub_er_aw_8(void);
void m68000_sub_er_al_8(void);
void m68000_sub_er_pcdi_8(void);
void m68000_sub_er_pcix_8(void);
void m68000_sub_er_i_8(void);
void m68000_sub_er_d_16(void);
void m68000_sub_er_a_16(void);
void m68000_sub_er_ai_16(void);
void m68000_sub_er_pi_16(void);
void m68000_sub_er_pd_16(void);
void m68000_sub_er_di_16(void);
void m68000_sub_er_ix_16(void);
void m68000_sub_er_aw_16(void);
void m68000_sub_er_al_16(void);
void m68000_sub_er_pcdi_16(void);
void m68000_sub_er_pcix_16(void);
void m68000_sub_er_i_16(void);
void m68000_sub_er_d_32(void);
void m68000_sub_er_a_32(void);
void m68000_sub_er_ai_32(void);
void m68000_sub_er_pi_32(void);
void m68000_sub_er_pd_32(void);
void m68000_sub_er_di_32(void);
void m68000_sub_er_ix_32(void);
void m68000_sub_er_aw_32(void);
void m68000_sub_er_al_32(void);
void m68000_sub_er_pcdi_32(void);
void m68000_sub_er_pcix_32(void);
void m68000_sub_er_i_32(void);
void m68000_sub_re_ai_8(void);
void m68000_sub_re_pi_8(void);
void m68000_sub_re_pi7_8(void);
void m68000_sub_re_pd_8(void);
void m68000_sub_re_pd7_8(void);
void m68000_sub_re_di_8(void);
void m68000_sub_re_ix_8(void);
void m68000_sub_re_aw_8(void);
void m68000_sub_re_al_8(void);
void m68000_sub_re_ai_16(void);
void m68000_sub_re_pi_16(void);
void m68000_sub_re_pd_16(void);
void m68000_sub_re_di_16(void);
void m68000_sub_re_ix_16(void);
void m68000_sub_re_aw_16(void);
void m68000_sub_re_al_16(void);
void m68000_sub_re_ai_32(void);
void m68000_sub_re_pi_32(void);
void m68000_sub_re_pd_32(void);
void m68000_sub_re_di_32(void);
void m68000_sub_re_ix_32(void);
void m68000_sub_re_aw_32(void);
void m68000_sub_re_al_32(void);
void m68000_suba_d_16(void);
void m68000_suba_a_16(void);
void m68000_suba_ai_16(void);
void m68000_suba_pi_16(void);
void m68000_suba_pd_16(void);
void m68000_suba_di_16(void);
void m68000_suba_ix_16(void);
void m68000_suba_aw_16(void);
void m68000_suba_al_16(void);
void m68000_suba_pcdi_16(void);
void m68000_suba_pcix_16(void);
void m68000_suba_i_16(void);
void m68000_suba_d_32(void);
void m68000_suba_a_32(void);
void m68000_suba_ai_32(void);
void m68000_suba_pi_32(void);
void m68000_suba_pd_32(void);
void m68000_suba_di_32(void);
void m68000_suba_ix_32(void);
void m68000_suba_aw_32(void);
void m68000_suba_al_32(void);
void m68000_suba_pcdi_32(void);
void m68000_suba_pcix_32(void);
void m68000_suba_i_32(void);
void m68000_subi_d_8(void);
void m68000_subi_ai_8(void);
void m68000_subi_pi_8(void);
void m68000_subi_pi7_8(void);
void m68000_subi_pd_8(void);
void m68000_subi_pd7_8(void);
void m68000_subi_di_8(void);
void m68000_subi_ix_8(void);
void m68000_subi_aw_8(void);
void m68000_subi_al_8(void);
void m68000_subi_d_16(void);
void m68000_subi_ai_16(void);
void m68000_subi_pi_16(void);
void m68000_subi_pd_16(void);
void m68000_subi_di_16(void);
void m68000_subi_ix_16(void);
void m68000_subi_aw_16(void);
void m68000_subi_al_16(void);
void m68000_subi_d_32(void);
void m68000_subi_ai_32(void);
void m68000_subi_pi_32(void);
void m68000_subi_pd_32(void);
void m68000_subi_di_32(void);
void m68000_subi_ix_32(void);
void m68000_subi_aw_32(void);
void m68000_subi_al_32(void);
void m68000_subq_d_8(void);
void m68000_subq_ai_8(void);
void m68000_subq_pi_8(void);
void m68000_subq_pi7_8(void);
void m68000_subq_pd_8(void);
void m68000_subq_pd7_8(void);
void m68000_subq_di_8(void);
void m68000_subq_ix_8(void);
void m68000_subq_aw_8(void);
void m68000_subq_al_8(void);
void m68000_subq_d_16(void);
void m68000_subq_a_16(void);
void m68000_subq_ai_16(void);
void m68000_subq_pi_16(void);
void m68000_subq_pd_16(void);
void m68000_subq_di_16(void);
void m68000_subq_ix_16(void);
void m68000_subq_aw_16(void);
void m68000_subq_al_16(void);
void m68000_subq_d_32(void);
void m68000_subq_a_32(void);
void m68000_subq_ai_32(void);
void m68000_subq_pi_32(void);
void m68000_subq_pd_32(void);
void m68000_subq_di_32(void);
void m68000_subq_ix_32(void);
void m68000_subq_aw_32(void);
void m68000_subq_al_32(void);
void m68000_subx_rr_8(void);
void m68000_subx_rr_16(void);
void m68000_subx_rr_32(void);
void m68000_subx_mm_8_ax7(void);
void m68000_subx_mm_8_ay7(void);
void m68000_subx_mm_8_axy7(void);
void m68000_subx_mm_8(void);
void m68000_subx_mm_16(void);
void m68000_subx_mm_32(void);
void m68000_swap(void);
void m68000_tas_d(void);
void m68000_tas_ai(void);
void m68000_tas_pi(void);
void m68000_tas_pi7(void);
void m68000_tas_pd(void);
void m68000_tas_pd7(void);
void m68000_tas_di(void);
void m68000_tas_ix(void);
void m68000_tas_aw(void);
void m68000_tas_al(void);
void m68000_trap(void);
void m68020_trapt_0(void);
void m68020_trapt_16(void);
void m68020_trapt_32(void);
void m68020_trapf_0(void);
void m68020_trapf_16(void);
void m68020_trapf_32(void);
void m68020_traphi_0(void);
void m68020_traphi_16(void);
void m68020_traphi_32(void);
void m68020_trapls_0(void);
void m68020_trapls_16(void);
void m68020_trapls_32(void);
void m68020_trapcc_0(void);
void m68020_trapcc_16(void);
void m68020_trapcc_32(void);
void m68020_trapcs_0(void);
void m68020_trapcs_16(void);
void m68020_trapcs_32(void);
void m68020_trapne_0(void);
void m68020_trapne_16(void);
void m68020_trapne_32(void);
void m68020_trapeq_0(void);
void m68020_trapeq_16(void);
void m68020_trapeq_32(void);
void m68020_trapvc_0(void);
void m68020_trapvc_16(void);
void m68020_trapvc_32(void);
void m68020_trapvs_0(void);
void m68020_trapvs_16(void);
void m68020_trapvs_32(void);
void m68020_trappl_0(void);
void m68020_trappl_16(void);
void m68020_trappl_32(void);
void m68020_trapmi_0(void);
void m68020_trapmi_16(void);
void m68020_trapmi_32(void);
void m68020_trapge_0(void);
void m68020_trapge_16(void);
void m68020_trapge_32(void);
void m68020_traplt_0(void);
void m68020_traplt_16(void);
void m68020_traplt_32(void);
void m68020_trapgt_0(void);
void m68020_trapgt_16(void);
void m68020_trapgt_32(void);
void m68020_traple_0(void);
void m68020_traple_16(void);
void m68020_traple_32(void);
void m68000_trapv(void);
void m68000_tst_d_8(void);
void m68000_tst_ai_8(void);
void m68000_tst_pi_8(void);
void m68000_tst_pi7_8(void);
void m68000_tst_pd_8(void);
void m68000_tst_pd7_8(void);
void m68000_tst_di_8(void);
void m68000_tst_ix_8(void);
void m68000_tst_aw_8(void);
void m68000_tst_al_8(void);
void m68020_tst_pcdi_8(void);
void m68020_tst_pcix_8(void);
void m68020_tst_imm_8(void);
void m68000_tst_d_16(void);
void m68020_tst_a_16(void);
void m68000_tst_ai_16(void);
void m68000_tst_pi_16(void);
void m68000_tst_pd_16(void);
void m68000_tst_di_16(void);
void m68000_tst_ix_16(void);
void m68000_tst_aw_16(void);
void m68000_tst_al_16(void);
void m68020_tst_pcdi_16(void);
void m68020_tst_pcix_16(void);
void m68020_tst_imm_16(void);
void m68000_tst_d_32(void);
void m68020_tst_a_32(void);
void m68000_tst_ai_32(void);
void m68000_tst_pi_32(void);
void m68000_tst_pd_32(void);
void m68000_tst_di_32(void);
void m68000_tst_ix_32(void);
void m68000_tst_aw_32(void);
void m68000_tst_al_32(void);
void m68020_tst_pcdi_32(void);
void m68020_tst_pcix_32(void);
void m68020_tst_imm_32(void);
void m68000_unlk_a7(void);
void m68000_unlk(void);
void m68020_unpk_rr(void);
void m68020_unpk_mm_ax7(void);
void m68020_unpk_mm_ay7(void);
void m68020_unpk_mm_axy7(void);
void m68020_unpk_mm(void);
};


extern int           m68k_clks_left;
extern uint          m68k_tracing;
extern uint          m68k_sr_implemented_bits[];
extern mc68000::m68k_cpu_core m68k_cpu;
extern uint*         m68k_cpu_dar[];
extern uint*         m68k_movem_pi_table[];
extern uint*         m68k_movem_pd_table[];
extern uint8         m68k_int_masks[];
extern uint8         m68k_shift_8_table[];
extern uint16        m68k_shift_16_table[];
extern uint          m68k_shift_32_table[];
extern uint8         m68k_exception_cycle_table[];

#endif /* M68KCPU__HEADER */
