/*
	99xxcore.h : generic tms99xx emulation

	The TMS99XX_MODEL switch tell which emulator we want to build.  Set the switch, then include
	99xxcore.h, and you will have an emulator for this processor.

	Only tms9900, tms9980a/9981, and tms9995 work OK for now.  Note that tms9995 has not been tested
	extensively.

	I think all software aspects of tms9940, tms9985 and tms9989 are implemented (though there
	must be some mistakes, particularily in tms9940 BCD support).  You'll just have to implement
	bus interfaces, provided you know them.  (I don't...)

	Original tms9900 emulator by Edward Swartz
	Smoothed out by Raphael Nabet
	Originally converted for Mame by M.Coates
	Processor timing, support for tms9980 and tms9995, and many bug fixes by R Nabet
*/

/*
	tms9900 is derived from the TI990/x minicomputer series (with multi-chip processors, except with
	cheaper or later models, which used microprocessors).  However, tms99xx (and even extension-less
	tms99xxx) only implement a reduced subset of the huge instruction set available on big TI990
	systems.

	AFAIK, tms9900, tms9980, and tms9985 have exactly the same programming model, and are actually
	the same CPU with different bus interfaces.  The former affirmation is (almost) true with
	tms9940, so I guess the later is true, too.  (The only problem with tms9940 is I have no idea
	what its bus interfaces are - I have a pinout, but it does not help much, although it is obvious
	that the beast is a microcontroller.)  tms9985 had on-chip RAM and timer, like tms9995, although
	I don't know how the memory and CRU were mapped exactly.

	tms9989 is mostly alien to me.  I assumed it was more related to tms9900 than tms9995, although
	it has most of the additionnal features tms9995 has.

	tms9995 belongs to another generation.  As a matter of fact, it quite faster than tms9900.

	tms99xxx include all tms9995 advances, and add some advances of their own.  I know they support
	many extra opcodes (84 instructions types on tms99105a, vs. 69 on tms9900), and support
	privileges (i.e. supervisor mode), flags in multiprocessor environment, 32-bit operations...
	There was yet other features (Macrostore to define custom instructions, op code compression...),
	which are completely alien to me.

	I have written code to recognize every tms99xxx opcode, and all other TI990 instructions I have
	heard of.  I cannot complete this code, since I have no documentation on tms99xxx.  I am not
	even sure the instruction set I guessed for tms99xxx is correct.  Also, the proposed meaning for
	these extra memnonics could be wrong.

References :
* 9900 family systems design, chapter 6, 7, 8
* TMS 9980A/ TMS 9981 Product Data Book
* TMS 9995 16-Bit Microcomputer Data Manual

Tons of thanks to the guy who posted these, whoever he is...
<http://www.stormaster.com/Spies/arcade/simulation/processors/index.html>
*/
//---------------------------------------------------------------------------

#pragma hdrstop

#include <stdio.h>
#include <math.h>
#include "typedefs.h"
#include "tms99xx.h"

#define RDOP(A) (read(A) << 8) + (read((A+1) & 0xffff))
#define RDWORD(A) (read(A) << 8) + (read((A+1) & 0xffff))

#define BITS_0to3	((OP>>12) & 0xf)
#define BITS_2to5	((OP>>10) & 0xf)
#define BITS_5to9	((OP>>6) & 0x1f)
#define BITS_3to7	((OP>>8) & 0x1f)
#define BITS_6to10	((OP>>5) & 0x1f)

#define BITS_0to1	((OP>>14) & 0x3)
#define BITS_0to4	((OP>>11) & 0x1f)
#define BITS_0to2	((OP>>13) & 0x7)
#define BITS_0to5	((OP>>10) & 0x3f)

#define	MASK	0x0000ffff
#define OPBITS(n1,n2)	((OP>>(15-(n2))) & (MASK>>(15-((n2)-(n1)))))

#define cpu_readmem16 read
#define cpu_writemem16 write
#define READREG(reg)          readword(WP+reg)
#define WRITEREG(reg,data)    writeword(WP+reg,data)

#define BYTE_XOR_BE(a)  	((a) ^ 1)				/* read/write a byte to a 16-bit space. Would just be (a) on MSB arch */
//#define BYTE_XOR_BE(a)  	(a)     				/* read/write a byte to a 16-bit space. Would just be (a) on MSB arch */

#define wadd(addr,expr) { int lval = setst_add_laeco(readword(addr), (expr)); writeword((addr),lval); }
#define wsub(addr,expr) { int lval = setst_sub_laeco(readword(addr), (expr)); writeword((addr),lval); }

static const char *token[]=
{
	"a",	"ab",	"c",	"cb",	"s",	"sb",	"soc",	"socb",	"szc",	"szcb",
	"mov",	"movb",	"coc",	"czc",	"xor",	"mpy",	"div",	"xop",	"b",	"bl",
	"blwp",	"clr",	"seto",	"inv",	"neg",	"abs",	"swpb",	"inc",	"inct",	"dec",
	"dect",	"x",	"ldcr",	"stcr",	"sbo",	"sbz",	"tb",	"jeq",	"jgt",	"jh",
	"jhe",	"jl",	"jle",	"jlt",	"jmp",	"jnc",	"jne",	"jno",	"joc",	"jop",
	"sla",	"sra",	"src",	"srl",	"ai",	"andi",	"ci",	"li",	"ori",	"lwpi",
	"limi",	"stst",	"stwp",	"rtwp",	"idle",	"rset",	"ckof",	"ckon",	"lrex",	"ill"
};

static const enum opcodes ops0to3[16]=
{
	_ill,	_ill,	_ill,	_ill,	_szc,	_szcb,	_s,		_sb,	/*0000-0111*/
	_c,		_cb,	_a,		_ab,	_mov,	_movb,	_soc,	_socb	/*1000-1111*/
};

static const enum opcodes ops2to5[16]=
{
	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	/*0000-0111*/
	_coc,	_czc,	_xor,	_xop,	_ldcr,	_stcr,	_mpy,	_div	/*1000-1111*/
};

static const enum opcodes ops5to9[32]=
{
	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	/*00000-00111*/
	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	/*01000-01111*/
	_blwp,	_b,		_x,		_clr,	_neg,	_inv,	_inc,	_inct,	/*10000-10111*/
	_dec,	_dect,	_bl,	_swpb,	_seto,	_abs,	_ill,	_ill	/*11000-11111*/
};

static const enum opcodes ops3to7[32]=
{
	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	/*00000-00111*/
	_sra,	_srl,	_sla,	_src,	_ill,	_ill,	_ill,	_ill,	/*01000-01111*/
	_jmp,	_jlt,	_jle,	_jeq,	_jhe,	_jgt,	_jne,	_jnc,	/*10000-10111*/
	_joc,	_jno,	_jl,	_jh,	_jop,	_sbo,	_sbz,	_tb		/*11000-11111*/
};

static const enum opcodes ops6to10[32]=
{
	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	/*00000-00111*/
	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	_ill,	/*01000-01111*/
	_li,	_ai,	_andi,	_ori,	_ci,	_stwp,	_stst,	_lwpi,	/*10000-10111*/
	_limi,	_ill,	_idle,	_rset,	_rtwp,	_ckon,	_ckof,	_lrex	/*11000-11111*/
};

/*
	TMS9900 hard reset
*/
tms99xx::tms99xx(void/*int Type*/)
{
  WP = PC = STATUS = IR = 0; /* dont care */
  interrupt_pending = 0; /* no pending interrupt */
  load_state = 0; /* LOAD* inactive */
  irq_level = 16; /* INTREQ* inactive */
  irq_state = 0;
  TMS99XX_ICOUNT = 0;
  disable_interrupt_recognition=0;
  if (CpuType == TMS9995_ID) {
	  //void decrementer_callback(int ignored);
	  //timer = timer_alloc(decrementer_callback);
  }
}

tms99xx::~tms99xx()
{
}

void tms99xx::reset(void *param, int Type)
{
  CpuType = Type & 0x01;
  
    WP = PC = STATUS = IR = 0; /* dont care */
	contextswitch(0x0000);

	STATUS = 0; /* TMS9980 and TMS9995 Data Book say so */
	setstat();

	IDLE = 0;   /* clear IDLE condition */

  if (CpuType == TMS9995_ID) {
    /* we can ask at reset time that the CPU always generates one wait state automatically */
    if (param == NULL)
    {	/* if no param, the default is currently "wait state added" */
      memory_wait_states_byte = 4;
      memory_wait_states_word = 12;
    }
    else
    {
      memory_wait_states_byte = (((tms9995reset_param *) param)->auto_wait_state) ? 4 : 0;
      memory_wait_states_word = (((tms9995reset_param *) param)->auto_wait_state) ? 12 : 4;
    }

    MID_flag = 0;

    /* Clear flag bits 0 & 1 */
    set_flag0(0);
    set_flag1(0);

    /* Clear internal interupt latches */
    int_latch = 0;
    flag &= 0xFFE3;
  }

	/* The ST register and interrupt latches changed, didn't it ? */
	field_interrupt();

	CYCLES(26, 14);
}

int tms99xx::execute(void)
{
int cycles = 2000;

	TMS99XX_ICOUNT = cycles;

//	do
//	{
    // From here on is MAME Debug code
    setstat();

/*    FR[ 0] = READREG(R0);
    FR[ 1] = READREG(R1);
    FR[ 2] = READREG(R2);
    FR[ 3] = READREG(R3);
    FR[ 4] = READREG(R4);
    FR[ 5] = READREG(R5);
    FR[ 6] = READREG(R6);
    FR[ 7] = READREG(R7);
    FR[ 8] = READREG(R8);
    FR[ 9] = READREG(R9);
    FR[10] = READREG(R10);
    FR[11] = READREG(R11);
    FR[12] = READREG(R12);
    FR[13] = READREG(R13);
    FR[14] = READREG(R14);
    FR[15] = READREG(R15);  */
		// Up to here

		if (IDLE)
		{	/* IDLE instruction has halted execution */
			external_instruction_notify(2);
			CYCLES(2, 2); /* 2 cycles per CRU write */
		}
		else
		{	/* we execute an instruction */
			disable_interrupt_recognition = 0;  /* default value */
			IR = fetch();
			exec(IR);

            if (CpuType == TMS9995_ID) {
        /* Note that TI had some problem implementing this...  I don't know if this feature works on
        a real-world TMS9995. */
              if ((STATUS & ST_OV_EN) && (STATUS & ST_OV) && (irq_level > 2))
                irq_level = 2;  /* interrupt request */
            }
		}

		/*
		  We handle interrupts here because :
		  a) LOAD and level-0 (reset) interrupts are non-maskable, so, AFAIK, if the LOAD* line or
		     INTREQ* line (with IC0-3 == 0) remain active, we will execute repeatedly the first
		     instruction of the interrupt routine.
		  b) if we did otherwise, we could have weird, buggy behavior if IC0-IC3 changed more than
		     once in too short a while (i.e. before tms9900 executes another instruction).  Yes, this
		     is rather pedantic, the probability is really small.
		*/
		if (interrupt_pending)
		{
			int level;

			level = irq_level;

			if (load_state)
			{	/* LOAD has the highest priority */

				contextswitch(0xFFFC);  /* load vector, save PC, WP and ST */

				STATUS &= 0xFFF0;     /* clear mask */

				/* clear IDLE status if necessary */
				IDLE = 0;

				CYCLES(22, 14);
                load_state = 0;
			}
			/* all TMS9900 chips I know do not honor interrupts after XOP, BLWP or MID (after any
			  interrupt-like instruction, actually) */
			else if (! disable_interrupt_recognition)
			{
				if (level <= IMASK)
				{	/* a maskable interrupt is honored only if its level isn't greater than IMASK */

					contextswitch(level*4); /* load vector, save PC, WP and ST */

					/* change interrupt mask */
					if (level)
					{
						STATUS = (STATUS & 0xFFF0) | (level -1);  /* decrement mask */
						interrupt_pending = 0;  /* as a consequence, the interrupt request will be subsequently ignored */
					}
					else
						STATUS &= 0xFFF0; /* clear mask (is this correct ???) */

                    if (CpuType == TMS9995_ID)
					  STATUS &= 0xFE0F;

					/* clear IDLE status if necessary */
					IDLE = 0;

                    if (CpuType == TMS9995_ID) {
                          /* Clear bit in latch */
                          /* I think tms9989 does this, too */
                      if (level != 2)
                      {	/* Only do this on level 1, 3, 4 interrupts */
                        int mask = 1 << level;
                        int flag_mask = (level == 1) ? 4 : mask;

                        int_latch &= ~ mask;
                        flag &= ~ flag_mask;

                      }
                    }

					CYCLES(22, 14);
				}
				else
				{
					interrupt_pending = 0;
				}
            }
		}

//	} while (TMS99XX_ICOUNT > 0);
    if ( decrementer ) {
      decrementer -= (cycles - TMS99XX_ICOUNT);
      if ( decrementer <= 0 ) {
        decrementer += (decrementer_interval * 34);  // 32
        decrementer_callback(0);
      }
    }

	return cycles - TMS99XX_ICOUNT;
}

/*
  this call-back is called by MESS timer system when the timer reaches 0.
*/
void tms99xx::decrementer_callback(int ignored)
{
	/* request decrementer interrupt */
	int_latch |= 0x8;
	flag |= 0x8;

	field_interrupt();
}

void tms99xx::set_irq_callback(int (*callback)(int irqline))
{
	irq_callback = callback;
}

void tms99xx::set_irq_line(int irqline, int state)
{
  if (CpuType < TMS9995_ID) {
    /*
	    interrupt system similar to tms9900, but only 3 interrupt pins (IC0-IC2)
    */
    if (state == CLEAR_LINE)
    {
      load_state = 0;
      irq_state = 0;
      irq_level = 16;
      /* trick : 16 will always be bigger than the IM (0-15), so there will never be interrupts */
    }
    else
    {
      int level;

//      level = (* irq_callback)(0);
      level = irqline;

      switch (level)
      {
      case 0:
      case 1:
        load_state = 0;
        irq_state = 0;
        irq_level = 16;
        reset(NULL, CpuType);
        break;
      case 2:
        load_state = 1;
        irq_state = 0;
        irq_level = 16;
        break;
      case 7:
        load_state = 0;
        irq_state = 0;
        irq_level = 16;
        break;
      default:  /* external levels 1, 2, 3, 4 */
        load_state = 0;
        irq_state = 1;
        irq_level = level - 2;
        break;
      }
    }

    field_interrupt();  /* interrupt state is likely to have changed */
  } else {
    /*
      You have two interrupt line : one triggers level-1 interrupts, the other triggers level-4
      interrupts (or decrements the decrementer register).

      According to the hardware, you may use PULSE_LINE (edge-triggered interrupts), or ASSERT_LINE
      (level-triggered interrupts).  Edge-triggered interrupts are way simpler, but if multiple devices
      share the same line, they must use level-triggered interrupts.
    */
    if (irqline == IRQ_LINE_NMI)
    {
      load_state = state;   /* save new state */

      field_interrupt();  /* interrupt status changed */
    }
    else
    {
      int mask = (irqline == 0) ? 0x2 : 0x10;
      int flag_mask = (irqline == 0) ? 0x4 : 0x10;

      if (((irq_state & mask) != 0) ^ (state != 0))
      {	/* only if state changes */
        if (state)
        {
          irq_state |= mask;

          if ((irqline == 1) && (flag & 1))
          {	/* event counter mode : INT4* triggers no interrupt... */
            if (decrementer_enabled)
            {	/* decrement, then interrupt if reach 0 */
              if ((-- decrementer_count) == 0)
              {
                decrementer_callback(0);
                decrementer_count = decrementer_interval * 32;	/* reload */
              }
            }
          }
          else
          {	/* plain interrupt mode */
            int_latch |= mask;
            flag |= flag_mask;
          }
        }
        else
        {
          irq_state &= ~ mask;
        }

        field_interrupt();  /* interrupt status changed */
      }
    }
  }
}

/*
	reset and load the timer/decrementer

	Note that I don't know whether toggling flag0/flag1 causes the decrementer to be reloaded or not
*/
void tms99xx::reset_decrementer(void)
{
//	timer_adjust(timer, TIME_NEVER, 0, 0);

    decrementer = 0;

	/* decrementer / timer enabled ? */
	decrementer_enabled = ((flag & 2) && (decrementer_interval));

	if (decrementer_enabled)
	{
		if (flag & 1)
		{	/* decrementer */
			decrementer_count = decrementer_interval;
		}
		else
		{	/* timer */
            decrementer = decrementer_interval * 34;
//			timer_adjust(timer, TIME_IN_CYCLES(decrementer_interval * 16L, cpu_getactivecpu()), 0, 0);
		}
	}
}

/* set decrementer mode flag */
void tms99xx::set_flag0(int val)
{
	if (val)
		flag |= 1;
	else
		flag &= ~ 1;

	reset_decrementer();
}

/* set decrementer enable flag */
void tms99xx::set_flag1(int val)
{
	if (val)
		flag |= 2;
	else
		flag &= ~ 2;

	reset_decrementer();
}

void tms99xx::CYCLES(int a, int b)
{
  if (CpuType < TMS9995_ID)
    TMS99XX_ICOUNT -= a;
  else
    TMS99XX_ICOUNT -= b*4;
}

/* fetch : read one word at * PC, and increment PC. */

UINT16 tms99xx::fetch(void)
{
	register UINT16 value = readword(PC);
	PC += 2;
	return value;
}

/*
 * field_interrupt
 *
 * Determines whether if an interrupt is pending, and sets the revelant flag.
 *
 * Called when an interrupt pin (LOAD*, INTREQ*, IC0-IC3) is changed, and when the interrupt mask
 * is modified.
 *
 * By using this flag, we save some compares in the execution loop.  Subtle, isn't it ;-) ?
 *
 * R Nabet.
 */
void tms99xx::field_interrupt(void)
{
  if (CpuType < TMS9995_ID) {
	  interrupt_pending = ((irq_level <= IMASK) || (load_state));
  } else {
    if (load_state)
    {
      interrupt_pending = 1;
    }
    else
    {
      int current_int;
      int level;

      if (flag & 1)
        /* event counter mode : ignore int4* line... */
        current_int = (irq_state & ~0x10) | int_latch;
      else
        /* normal behavior */
        current_int = irq_state | int_latch;

      if (current_int)
        /* find first bit to 1 */
        /* possible values : 1, 3, 4 */
        for (level=0; ! (current_int & 1); current_int >>= 1, level++)
          ;
      else
        level=16;

      irq_level = level;

      interrupt_pending = (level <= IMASK);
    }
  }
}

unsigned short tms99xx::get_reg(int regnum)
{
	switch( regnum )
	{
		case TMS9900_PC: return PC;
		case TMS9900_IR: return IR;
		case TMS9900_WP: return WP;
		case TMS9900_STATUS: return STATUS;
		case TMS9900_R0: return READREG(R0);
		case TMS9900_R1: return READREG(R1);
		case TMS9900_R2: return READREG(R2);
		case TMS9900_R3: return READREG(R3);
		case TMS9900_R4: return READREG(R4);
		case TMS9900_R5: return READREG(R5);
		case TMS9900_R6: return READREG(R6);
		case TMS9900_R7: return READREG(R7);
		case TMS9900_R8: return READREG(R8);
		case TMS9900_R9: return READREG(R9);
		case TMS9900_R10: return READREG(R10);
		case TMS9900_R11: return READREG(R11);
		case TMS9900_R12: return READREG(R12);
		case TMS9900_R13: return READREG(R13);
		case TMS9900_R14: return READREG(R14);
		case TMS9900_R15: return READREG(R15);
	}
	return 0;
}

void tms99xx::set_reg(int regnum, unsigned val)
{
	switch( regnum )
	{
		case TMS9900_PC: PC = val; break;
		case TMS9900_IR: IR = val; break;
		case TMS9900_WP: WP = val; break;
		case TMS9900_STATUS: STATUS = val; break;
		case TMS9900_R0: WRITEREG(R0, val); break;
		case TMS9900_R1: WRITEREG(R1, val); break;
		case TMS9900_R2: WRITEREG(R2, val); break;
		case TMS9900_R3: WRITEREG(R3, val); break;
		case TMS9900_R4: WRITEREG(R4, val); break;
		case TMS9900_R5: WRITEREG(R5, val); break;
		case TMS9900_R6: WRITEREG(R6, val); break;
		case TMS9900_R7: WRITEREG(R7, val); break;
		case TMS9900_R8: WRITEREG(R8, val); break;
		case TMS9900_R9: WRITEREG(R9, val); break;
		case TMS9900_R10: WRITEREG(R10, val); break;
		case TMS9900_R11: WRITEREG(R11, val); break;
		case TMS9900_R12: WRITEREG(R12, val); break;
		case TMS9900_R13: WRITEREG(R13, val); break;
		case TMS9900_R14: WRITEREG(R14, val); break;
		case TMS9900_R15: WRITEREG(R15, val); break;
	}
}

/*
	Some opcodes perform a dummy write to a special CRU address, so that an external function may be
	triggered.

	Only the first 3 MSBs of the address matter : other address bits and the written value itself
	are undefined.

	How should we support this ? With callback functions ? Actually, as long as we do not support
	hardware which makes use of this feature, it does not really matter :-) .
*/
void tms99xx::external_instruction_notify(int ext_op_ID)
{
	/* I guess we can support this like normal CRU operations */
  if (CpuType < TMS9995_ID) {
	  writeport((ext_op_ID & 3) << 11, (ext_op_ID & 4) ? 1 : 0);
  } else {
	  writeport(ext_op_ID << 15, 0); /* or is it 1 ??? */
  }
}

#define WRITEPORT(Port, data) writeport(Port, data)

/*
	performs a normal write to CRU bus (used by SBZ, SBO, LDCR : address range 0 -> 0xFFF)
*/
void tms99xx::writeCRU(int CRUAddr, int Number, UINT16 Value)
{
  UINT16 wCRUAddrMask;

  if (CpuType < TMS9995_ID) {
	  /* 2 bits unused, and 2 MSBs are always 0 to support external instructions */
	  wCRUAddrMask = 0x7FF;
  } else {
	  /* no such problem here : data bus lines D0-D2 provide the external instruction code */
	  wCRUAddrMask = 0x7FFF;
  }

	int count;

	CRUAddr &= wCRUAddrMask;

	/* Write Number bits from CRUAddr */

	for(count=0; count<Number; count++)
	{
    if (CpuType == TMS9995_ID) {
      if (CRUAddr == 0xF70)
        set_flag0(Value & 0x01);
      else if (CRUAddr == 0xF71)
        set_flag1(Value & 0x01);
      else if ((CRUAddr >= 0xF72) && (CRUAddr < 0xF75))
        ;     /* ignored */
      else if ((CRUAddr >= 0xF75) && (CRUAddr < 0xF80))
      {	/* user defined flags */
        int mask = 1 << (CRUAddr - 0xF70);
        if (Value & 0x01)
          flag |= mask;
        else
          flag &= ~ mask;
      }
      else if (CRUAddr == 0x0FED)
        /* MID flag */
        MID_flag = Value & 0x01;
      else
        /* External CRU */
		    WRITEPORT(CRUAddr, (Value & 0x01));
    } else {
		  WRITEPORT(CRUAddr, (Value & 0x01));
    }
		Value >>= 1;
		CRUAddr = (CRUAddr + 1) & wCRUAddrMask;
	}
}

/*
	performs a normal read to CRU bus (used by TB, STCR : address range 0->0xFFF)

	Note that on some hardware, e.g. TI99, all normal memory operations cause unwanted CRU
	read at the same address.
	This seems to be impossible to emulate efficiently.
*/
/* on tms9995, we have to handle internal CRU port */
int tms99xx::READPORT(int Port)
{
  if (CpuType < TMS9995_ID) {
    return readport(Port);
  } else {
    if (Port >= 0xf70 && Port <= 0xf7f )
      /* flag, bits 0-15 */
      return (flag >> (Port - 0xf70)) & 1;
    else if (Port == 0xFED)
      /* MID flag */
      return MID_flag;
    else
      /* external devices */
      return readport(Port);
  }
}

UINT16 tms99xx::readCRU(int CRUAddr, int Number)
{
  UINT16 rCRUAddrMask;

  if (CpuType < TMS9995_ID) {
	  rCRUAddrMask = 0x0FF;
  } else {
	  /* no such problem here : data bus lines D0-D2 provide the external instruction code */
	  rCRUAddrMask = 0xFFF;
  }
  int count;
  int value = 0;

  for ( count = 0; count < Number; count++, CRUAddr++ )
    value = value | ((READPORT(CRUAddr & rCRUAddrMask) & 1) << count);

  return value;
}

int tms99xx::readbyte(int addr)
{
	static int extra_byte;

  if (CpuType < TMS9995_ID)
  {
	  /*8-bit data bus, 14-bit address*/
	  /*Note that tms9980 never accesses a single byte (however crazy it may seem).  Although this
	  makes memory access slower, I have emulated this feature, because if I did otherwise,
	  there would be some implementation problems in some driver sooner or later.*/
    /*This is how it really works*/
    /*Note that every writebyte must match a readbyte (which is the case on a real-world tms9980)*/

		TMS99XX_ICOUNT -= 2;
		if (addr & 1)
		{
			extra_byte = cpu_readmem16((addr-1) & 0x3fff);
			return cpu_readmem16(addr & 0x3fff);
		}
		else
		{
			int val = cpu_readmem16(addr & 0x3fff);
			extra_byte = cpu_readmem16((addr+1) & 0x3fff);
			return val;
		}
  } else
  {
		if (addr < 0xf000)
		{
			TMS99XX_ICOUNT -= memory_wait_states_byte;
			return cpu_readmem16(addr);
		}
		else if (addr < 0xf0fc)
		{
//			return RAM[BYTE_XOR_BE(addr - 0xf000)];
			return cpu_readmem16(addr);
		}
		else if (addr < 0xfffa)
		{
			TMS99XX_ICOUNT -= memory_wait_states_byte;
			return cpu_readmem16(addr);
		}
		else if (addr < 0xfffc)
		{
			/* read decrementer */
			int value;

			if (flag & 1)
				/* event counter mode */
				value = decrementer_count;
			else if (decrementer_enabled && !(flag & 1))
				/* timer mode, timer enabled */
				value = decrementer / 4;
			else
				/* timer mode, timer disabled */
				value = 0;

			if (addr & 1)
				return (value & 0xFF);
			else
				return (value >> 8);
		}
		else
		{
//			return RAM[BYTE_XOR_BE(addr - 0xff00)];
			return cpu_readmem16(addr);
		}
  }
}

void tms99xx::writebyte(int addr, int data)
{
	static int extra_byte;

  if (CpuType < TMS9995_ID)
  {
		TMS99XX_ICOUNT -= 2;
		if (addr & 1)
		{
			extra_byte = cpu_readmem16((addr-1) & 0x3fff);

			cpu_writemem16((addr-1) & 0x3fff, extra_byte);
			cpu_writemem16(addr & 0x3fff, data);
		}
		else
		{
			extra_byte = cpu_readmem16((addr+1) & 0x3fff);

			cpu_writemem16(addr & 0x3fff, data);
			cpu_writemem16((addr+1) & 0x3fff, extra_byte);
		}
  } else
  {
		if (addr < 0xf000)
		{
			TMS99XX_ICOUNT -= memory_wait_states_byte;
			cpu_writemem16(addr, data);
		}
		else if (addr < 0xf0fc)
		{
//			RAM[BYTE_XOR_BE(addr - 0xf000)] = data;
			cpu_writemem16(addr, data);
		}
		else if (addr < 0xfffa)
		{
			TMS99XX_ICOUNT -= memory_wait_states_byte;
			cpu_writemem16(addr, data);
		}
		else if (addr < 0xfffc)
		{
			/* write decrementer */
			/* Note that a byte write to tms9995 timer messes everything up. */
			decrementer_interval = (data << 8) | data;
			reset_decrementer();
		}
		else
		{
//			RAM[BYTE_XOR_BE(addr - 0xff00)] = data;
			cpu_writemem16(addr, data);
		}
  }
}

int tms99xx::readword(int addr)
{
  if (CpuType < TMS9995_ID)
  {
    TMS99XX_ICOUNT -= 2;
    return (cpu_readmem16(addr & 0x3fff) << 8) + cpu_readmem16((addr+1) & 0x3fff);
  } else
  {
		if (addr < 0xf000)
		{
			TMS99XX_ICOUNT -= memory_wait_states_word;
			return (cpu_readmem16(addr) << 8) + cpu_readmem16(addr + 1);
		}
		else if (addr < 0xf0fc)
		{
//			return *(UINT16 *)(& RAM[addr - 0xf000]);
			return (cpu_readmem16(addr) << 8) + cpu_readmem16(addr + 1);
		}
		else if (addr < 0xfffa)
		{
			TMS99XX_ICOUNT -= memory_wait_states_word;
			return (cpu_readmem16(addr) << 8) + cpu_readmem16(addr + 1);
		}
		else if (addr < 0xfffc)
		{
			/* read decrementer */
			if (flag & 1)
				/* event counter mode */
				return decrementer_count;
			else if (decrementer_enabled && !(flag & 1))
				/* timer mode, timer enabled */
				return decrementer / 4;
			else
				/* timer mode, timer disabled */
				return 0;
		}
		else
		{
//			return *(UINT16 *)(& RAM[addr - 0xff00]);
			return (cpu_readmem16(addr) << 8) + cpu_readmem16(addr + 1);
		}
  }
}

void tms99xx::writeword(int addr, int data)
{
  if (CpuType < TMS9995_ID)
  {
    TMS99XX_ICOUNT -= 2;
    cpu_writemem16((addr & 0x3fff), (data) >> 8);
    cpu_writemem16((addr+1) & 0x3fff, (data) & 0xff);
  } else
  {
		if (addr < 0xf000)
		{
			TMS99XX_ICOUNT -= memory_wait_states_word;
			cpu_writemem16(addr, data >> 8);
			cpu_writemem16(addr + 1, data & 0xff);
		}
		else if (addr < 0xf0fc)
		{
//			*(UINT16 *)(& RAM[addr - 0xf000]) = data;
			cpu_writemem16(addr, data >> 8);
			cpu_writemem16(addr + 1, data & 0xff);
		}
		else if (addr < 0xfffa)
		{
			TMS99XX_ICOUNT -= memory_wait_states_word;
			cpu_writemem16(addr, data >> 8);
			cpu_writemem16(addr + 1, data & 0xff);
		}
		else if (addr < 0xfffc)
		{
			/* write decrementer */
			decrementer_interval = data;
			reset_decrementer();
		}
		else
		{
//			*(UINT16 *)(& RAM[addr - 0xff00]) = data;
			cpu_writemem16(addr, data >> 8);
			cpu_writemem16(addr + 1, data & 0xff);
		}
  }
}

/* contextswitch : performs a BLWP, ie change PC, WP, and save PC, WP and ST... */
void tms99xx::contextswitch(UINT16 addr)
{
	UINT16 oldWP, oldpc;

	/* save old state */
	oldWP = WP;
	oldpc = PC;

	/* load vector */
	WP = readword(addr) & ~1;
	PC = readword(addr+2) & ~1;

	/* write old state to regs */
	WRITEREG(R13, oldWP);
	WRITEREG(R14, oldpc);
	setstat();
	WRITEREG(R15, STATUS);
}

/*
 * decipheraddr : compute and return the effective adress in word instructions.
 *
 * NOTA : the LSB is always ignored in word adresses,
 * but we do not set to 0 because of XOP...
 */
UINT16 tms99xx::decipheraddr(UINT16 opcode)
{
	register UINT16 ts = opcode & 0x30;
	register UINT16 reg = opcode & 0xF;

	reg += reg;

	if (ts == 0)
		/* Rx */
		return(reg + WP);
	else if (ts == 0x10)
	{	/* *Rx */
		CYCLES(4, 1);
		return(readword(reg + WP));
	}
	else if (ts == 0x20)
	{
		register UINT16 imm;

		imm = fetch();

		if (reg)
		{	/* @>xxxx(Rx) */
			CYCLES(8, 3);
			return(readword(reg + WP) + imm);
		}
		else
		{	/* @>xxxx */
			CYCLES(8, 1);
			return(imm);
		}
	}
	else /*if (ts == 0x30)*/
	{	/* *Rx+ */
		register UINT16 response;

		reg += WP;    /* reg now contains effective address */

		CYCLES(8, 3);

		response = readword(reg);
		writeword(reg, response+2); /* we increment register content */
		return(response);
	}
}

/* decipheraddrbyte : compute and return the effective adress in byte instructions. */
UINT16 tms99xx::decipheraddrbyte(UINT16 opcode)
{
	register UINT16 ts = opcode & 0x30;
	register UINT16 reg = opcode & 0xF;

	reg += reg;

	if (ts == 0)
		/* Rx */
		return(reg + WP);
	else if (ts == 0x10)
	{	/* *Rx */
		CYCLES(4, 1);
		return(readword(reg + WP));
	}
	else if (ts == 0x20)
	{
		register UINT16 imm;

		imm = fetch();

		if (reg)
		{	/* @>xxxx(Rx) */
			CYCLES(8, 3);
			return(readword(reg + WP) + imm);
		}
		else
		{	/* @>xxxx */
			CYCLES(8, 1);
			return(imm);
		}
	}
	else /*if (ts == 0x30)*/
	{	/* *Rx+ */
		register UINT16 response;

		reg += WP;    /* reg now contains effective address */

		CYCLES(6, 3);

		response = readword(reg);
		writeword(reg, response+1); /* we increment register content */
		return(response);
	}
}

/*==========================================================================
   Illegal instructions                                        >0000->01FF (not for 9989 and later)
                                                               >0C00->0FFF (not for 99xxx)
============================================================================*/

void tms99xx::illegal(UINT16 opcode)
{
  if (CpuType < TMS9995_ID)
  {
	  /* TMS9900/TMS9980 merely ignore the instruction */
	  TMS99XX_ICOUNT -= 6;
  } else
  {
	  /* TMS9995 generates a MID interrupt */
		MID_flag = 1;
		contextswitch(0x0008); 
		STATUS = (STATUS & 0xFE00) | 0x1;
		disable_interrupt_recognition = 1;
	}
}

/*==========================================================================
   Additionnal single-register instructions,                   >0040->00FF
 ---------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    |     o p c o d e       | reg # |
    ---------------------------------

tms9989 and later : LST, LWP
tms99xxx : BLSK
============================================================================*/
void tms99xx::h0040(UINT16 opcode)
{
	register UINT16 addr;

	addr = opcode & 0xF;
	addr = ((addr + addr) + WP) & ~1;

	switch ((opcode & 0xF0) >> 4)
	{
	case 8:   /* LST */
		/* LST --- Load STatus register */
		/* ST = *Reg */
		STATUS = readword(addr);
        CYCLES( 12, 7);
		field_interrupt();  /*IM has been modified.*/
		break;
	case 9:   /* LWP */
		/* LWP --- Load Workspace Pointer register */
		/* WP = *Reg */
		WP = readword(addr);
        CYCLES( 12, 6);
		break;
	default:
		illegal(opcode);
		break;
	}
}


/*==========================================================================
   Additionnal single-operand instructions,                    >0100->01FF
 ---------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    |    o p c o d e    |TS |   S   |
    ---------------------------------

tms9989 and later : DIVS, MPYS
tms99xxx : BIND
============================================================================*/
void tms99xx::h0100(UINT16 opcode)
{
	register UINT16 src;

	src = decipheraddr(opcode) & ~1;

  switch ((opcode & 0xC0) >> 6)
  {
  case 2:   /* DIVS */
		/* DIVS -- DIVide Signed */
		/* R0 = (R0:R1)/S   R1 = (R0:R1)%S */
		{
			INT16 d = readword(src);
			long divq = (READREG(R0) << 16) | READREG(R1);
			long q = divq/d;

			if ((q < -32768L) || (q > 32767L))
			{
				STATUS |= ST_OV;
				CYCLES(24 /*don't know*/, 10);
			}
			else
			{
				STATUS &= ~ST_OV;
				setst_lae(q);
				WRITEREG(R0, q);
				WRITEREG(R1, divq%d);
				/* tms9995 : 33 is the worst case */
				CYCLES(102 /*don't know*/, 33);
			}
		}
		break;

  case 3:   /* MPYS */
		/* MPYS -- MultiPlY Signed */
		/* Results:  R0:R1 = R0*S */
		{
			long prod = ((long) (INT16) READREG(R0)) * ((long) (INT16) readword(src));

			STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ);
			if (prod > 0)
				STATUS |= (ST_LGT | ST_AGT);
			else if (prod < 0)
				STATUS |= ST_LGT;
			else
				STATUS |= ST_EQ;

			WRITEREG(R0, prod >> 16);
			WRITEREG(R1, prod);
		}
		CYCLES(56 /*don't know*/, 25);
		break;
	default:
		illegal(opcode);
		break;
  }
}

/*==========================================================================
   Immediate, Control instructions,                            >0200->03FF
 ---------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    |     o p c o d e     |0| reg # |
    ---------------------------------

  LI, AI, ANDI, ORI, CI, STWP, STST, LIMI, LWPI, IDLE, RSET, RTWP, CKON, CKOF, LREX
============================================================================*/
void tms99xx::h0200(UINT16 opcode)
{
	register UINT16 addr;
	register UINT16 value;	/* used for anything */

	addr = opcode & 0xF;
	addr = ((addr + addr) + WP) & ~1;

  if (CpuType == TMS9995_ID) {
    /* better instruction decoding on tms9995 */
    if (((opcode < 0x2E0) && (opcode & 0x10)) || ((opcode >= 0x2E0) && (opcode & 0x1F)))
    {
      illegal(opcode);
      return;
    }
  }

	switch ((opcode & 0x1e0) >> 5)
	{
	case 0:   /* LI */
		/* LI ---- Load Immediate */
		/* *Reg = *PC+ */
		value = fetch();
		writeword(addr, value);
		setst_lae(value);
		CYCLES(12, 3);
		break;
	case 1:   /* AI */
		/* AI ---- Add Immediate */
		/* *Reg += *PC+ */
		value = fetch();
		wadd(addr, value);
		CYCLES(14, 4);
		break;
	case 2:   /* ANDI */
		/* ANDI -- AND Immediate */
		/* *Reg &= *PC+ */
		value = fetch();
		value = readword(addr) & value;
		writeword(addr, value);
		setst_lae(value);
		CYCLES(14, 4);
		break;
	case 3:   /* ORI */
		/* ORI --- OR Immediate */
		/* *Reg |= *PC+ */
		value = fetch();
		value = readword(addr) | value;
		writeword(addr, value);
		setst_lae(value);
		CYCLES(14, 4);
		break;
	case 4:   /* CI */
		/* CI ---- Compare Immediate */
		/* status = (*Reg-*PC+) */
		value = fetch();
		setst_c_lae(value, readword(addr));
		CYCLES(14, 4);
		break;
	case 5:   /* STWP */
		/* STWP -- STore Workspace Pointer */
		/* *Reg = WP */
		writeword(addr, WP);
		CYCLES(8, 3);
		break;
	case 6:   /* STST */
		/* STST -- STore STatus register */
		/* *Reg = ST */
		setstat();
		writeword(addr, STATUS);
		CYCLES(8, 3);
		break;
	case 7:   /* LWPI */
		/* LWPI -- Load Workspace Pointer Immediate */
		/* WP = *PC+ */
        {int oldWP = WP;
		WP = fetch();
        if ( WP == oldWP )
          printf("DOH !!");
		CYCLES(10, 4);
        }
		break;
	case 8:   /* LIMI */
		/* LIMI -- Load Interrupt Mask Immediate */
		/* ST&15 |= (*PC+)&15 */
		value = fetch();
		STATUS = (STATUS & ~ 0xF) | (value & 0xF);
		field_interrupt();  /*IM has been modified.*/
		CYCLES(16, 5);
		break;
	case 9:   /* LMF is implemented elsewhere - when it is implemented */
		illegal(opcode);
		break;
	case 10:  /* IDLE */
		/* IDLE -- IDLE until a reset, interrupt, load */
		/* The TMS99000 locks until an interrupt happen (like with 68k STOP instruction),
		   and continuously performs a special CRU write (code 2). */
		IDLE = 1;
		external_instruction_notify(2);
		CYCLES(12, 7);
		/* we take care of further external_instruction_notify(2); in execute() */
		break;
	case 11:  /* RSET */
		/* RSET -- ReSET */
		/* Reset the Interrupt Mask, and perform a special CRU write (code 3). */
		/* Does not actually cause a reset, but an external circuitery could trigger one. */
		STATUS &= 0xFFF0; /*clear IM.*/
		field_interrupt();  /*IM has been modified.*/
		external_instruction_notify(3);
		CYCLES(12, 7);
		break;
	case 12:  /* RTWP */
		/* RTWP -- Return with Workspace Pointer */
		/* WP = R13, PC = R14, ST = R15 */
		STATUS = READREG(R15);
		getstat();  /* set last_parity */
		PC = READREG(R14);
		WP = READREG(R13);
		field_interrupt();  /*IM has been modified.*/
		CYCLES(14, 6);
		break;
	case 13:  /* CKON */
	case 14:  /* CKOF */
	case 15:  /* LREX */
		/* CKON -- ClocK ON */
		/* Perform a special CRU write (code 5). */
		/* An external circuitery could, for instance, enable a "decrement-and-interrupt" timer. */
		/* CKOF -- ClocK OFf */
		/* Perform a special CRU write (code 6). */
		/* An external circuitery could, for instance, disable a "decrement-and-interrupt" timer. */
		/* LREX -- Load or REstart eXecution */
		/* Perform a special CRU write (code 7). */
		/* An external circuitery could, for instance, activate the LOAD* line,
		   causing a non-maskable LOAD interrupt (vector -1). */
		external_instruction_notify((opcode & 0x00e0) >> 5);
		CYCLES(12, 7);
		break;
	}
}


/*==========================================================================
   Single-operand instructions,                                >0400->07FF
 ---------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    |    o p c o d e    |TS |   S   |
    ---------------------------------

  BLWP, B, X, CLR, NEG, INV, INC, INCT, DEC, DECT, BL, SWPB, SETO, ABS
tms99xxx : LDD, LDS
============================================================================*/
void tms99xx::h0400(UINT16 opcode)
{
	register UINT16 addr = decipheraddr(opcode) & ~1;
	register UINT16 value;  /* used for anything */

	switch ((opcode & 0x3C0) >> 6)
	{
	case 0:   /* BLWP */
		/* BLWP -- Branch and Link with Workspace Pointer */
		/* Result: WP = *S+, PC = *S */
		/*         New R13=old WP, New R14=Old PC, New R15=Old ST */
		contextswitch(addr);
		CYCLES(26, 11);
		disable_interrupt_recognition = 1;
		break;
	case 1:   /* B */
		/* B ----- Branch */
		/* PC = S */
		PC = addr;
		CYCLES(8, 3);
		break;
	case 2:   /* X */
		/* X ----- eXecute */
		/* Executes instruction *S */
		exec(readword(addr));
		/* On tms9900, the X instruction actually takes 8 cycles, but we gain 4 cycles on the next
		instruction, as we don't need to fetch it. */
		CYCLES(4, 2);
		break;
	case 3:   /* CLR */
		/* CLR --- CLeaR */
		/* *S = 0 */
		writeword(addr, 0);
		CYCLES(10, 3);
		break;
	case 4:   /* NEG */
		/* NEG --- NEGate */
		/* *S = -*S */
		value = - (INT16) readword(addr);
		if (value)
			STATUS &= ~ ST_C;
		else
			STATUS |= ST_C;
		setst_laeo(value);
		writeword(addr, value);
		CYCLES(12, 3);
		break;
	case 5:   /* INV */
		/* INV --- INVert */
		/* *S = ~*S */
		value = ~ readword(addr);
		writeword(addr, value);
		setst_lae(value);
		CYCLES(10, 3);
		break;
	case 6:   /* INC */
		/* INC --- INCrement */
		/* (*S)++ */
		wadd(addr, 1);
		CYCLES(10, 3);
		break;
	case 7:   /* INCT */
		/* INCT -- INCrement by Two */
		/* (*S) +=2 */
		wadd(addr, 2);
		CYCLES(10, 3);
		break;
	case 8:   /* DEC */
		/* DEC --- DECrement */
		/* (*S)-- */
		wsub(addr, 1);
		CYCLES(10, 3);
		break;
	case 9:   /* DECT */
		/* DECT -- DECrement by Two */
		/* (*S) -= 2 */
		wsub(addr, 2);
		CYCLES(10, 3);
		break;
	case 10:  /* BL */
		/* BL ---- Branch and Link */
		/* IP=S, R11=old IP */
		WRITEREG(R11, PC);
		PC = addr;
		CYCLES(12, 5);
		break;
	case 11:  /* SWPB */
		/* SWPB -- SWaP Bytes */
		/* *S = swab(*S) */
		value = readword(addr);
		value = logical_right_shift(value, 8) | (value << 8);
		writeword(addr, value);
		CYCLES(10, 13);
		break;
	case 12:  /* SETO */
		/* SETO -- SET Ones */
		/* *S = #$FFFF */
		writeword(addr, 0xFFFF);
		CYCLES(10, 3);
		break;
	case 13:  /* ABS */
		/* ABS --- ABSolute value */
		/* *S = |*S| */
		/* clearing ST_C seems to be necessary, although ABS will never set it. */
    if (CpuType < TMS9995_ID) {
      /* tms9900/tms9980 only write the result if it has changed */
      STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV);
      value = readword(addr);

      CYCLES(12, 0); // 0 = Moof

      if (((INT16) value) > 0)
        STATUS |= ST_LGT | ST_AGT;
      else if (((INT16) value) < 0)
      {
        STATUS |= ST_LGT;
        if (value == 0x8000)
          STATUS |= ST_OV;
        writeword(addr, - ((INT16) value));
        CYCLES(2, 0); // 0 = Moof
      }
      else
        STATUS |= ST_EQ;
    } else {
      /* tms9995 always write the result */
      STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV);
      value = readword(addr);

      CYCLES(12, 3);
      if (((INT16) value) > 0)
        STATUS |= ST_LGT | ST_AGT;
      else if (((INT16) value) < 0)
      {
        STATUS |= ST_LGT;
        if (value == 0x8000)
          STATUS |= ST_OV;
        value = - ((INT16) value);
      }
      else
        STATUS |= ST_EQ;

      writeword(addr, value);
    }
		break;
	default:
		/* illegal instructions */
		illegal(opcode);
		break;
	}
}


/*==========================================================================
   Shift instructions,                                         >0800->0BFF
  --------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    | o p c o d e   |   C   |   W   |
    ---------------------------------

  SRA, SRL, SLA, SRC
============================================================================*/
void tms99xx::h0800(UINT16 opcode)
{
	register UINT16 addr;
	register UINT16 cnt = (opcode & 0xF0) >> 4;
	register UINT16 value;

	addr = (opcode & 0xF);
	addr = ((addr+addr) + WP) & ~1;

	CYCLES(12, 5);

	if (cnt == 0)
	{
		CYCLES(8, 2);

		cnt = READREG(0) & 0xF;

		if (cnt == 0)
			cnt = 16;
	}

	CYCLES(cnt+cnt, cnt);

	switch ((opcode & 0x300) >> 8)
	{
	case 0:   /* SRA */
		/* SRA --- Shift Right Arithmetic */
		/* *W >>= C   (*W is filled on the left with a copy of the sign bit) */
		value = setst_sra_laec(readword(addr), cnt);
		writeword(addr, value);
		break;
	case 1:   /* SRL */
		/* SRL --- Shift Right Logical */
		/* *W >>= C   (*W is filled on the left with 0) */
		value = setst_srl_laec(readword(addr), cnt);
		writeword(addr, value);
		break;
	case 2:   /* SLA */
		/* SLA --- Shift Left Arithmetic */
		/* *W <<= C */
		value = setst_sla_laeco(readword(addr), cnt);
		writeword(addr, value);
		break;
	case 3:   /* SRC */
		/* SRC --- Shift Right Circular */
		/* *W = rightcircularshift(*W, C) */
		value = setst_src_laec(readword(addr), cnt);
		writeword(addr, value);
		break;
	}
}

/*==========================================================================
   Jump, CRU bit instructions,                                 >1000->1FFF
 ---------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    |  o p c o d e  | signed offset |
    ---------------------------------

  JMP, JLT, JLE, JEQ, JHE, JGT, JNE, JNC, JOC, JNO, JL, JH, JOP
  SBO, SBZ, TB
============================================================================*/
void tms99xx::h1000(UINT16 opcode)
{
	/* we convert 8 bit signed word offset to a 16 bit effective word offset. */
	register INT16 offset = ((INT8) opcode);


	switch ((opcode & 0xF00) >> 8)
	{
	case 0:   /* JMP */
		/* JMP --- unconditional JuMP */
		/* PC += offset */
		PC += (offset + offset);
		CYCLES(10, 3);
		break;
	case 1:   /* JLT */
		/* JLT --- Jump if Less Than (arithmetic) */
		/* if (A==0 && EQ==0), PC += offset */
		if (! (STATUS & (ST_AGT | ST_EQ)))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 2:   /* JLE */
		/* JLE --- Jump if Lower or Equal (logical) */
		/* if (L==0 || EQ==1), PC += offset */
		if ((! (STATUS & ST_LGT)) || (STATUS & ST_EQ))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 3:   /* JEQ */
		/* JEQ --- Jump if EQual */
		/* if (EQ==1), PC += offset */
		if (STATUS & ST_EQ)
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 4:   /* JHE */
		/* JHE --- Jump if Higher or Equal (logical) */
		/* if (L==1 || EQ==1), PC += offset */
		if (STATUS & (ST_LGT | ST_EQ))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 5:   /* JGT */
		/* JGT --- Jump if Greater Than (arithmetic) */
		/* if (A==1), PC += offset */
		if (STATUS & ST_AGT)
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 6:   /* JNE */
		/* JNE --- Jump if Not Equal */
		/* if (EQ==0), PC += offset */
		if (! (STATUS & ST_EQ))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 7:   /* JNC */
		/* JNC --- Jump if No Carry */
		/* if (C==0), PC += offset */
		if (! (STATUS & ST_C))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 8:   /* JOC */
		/* JOC --- Jump On Carry */
		/* if (C==1), PC += offset */
		if (STATUS & ST_C)
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 9:   /* JNO */
		/* JNO --- Jump if No Overflow */
		/* if (OV==0), PC += offset */
		if (! (STATUS & ST_OV))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 10:  /* JL */
		/* JL ---- Jump if Lower (logical) */
		/* if (L==0 && EQ==0), PC += offset */
		if (! (STATUS & (ST_LGT | ST_EQ)))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 11:  /* JH */
		/* JH ---- Jump if Higher (logical) */
		/* if (L==1 && EQ==0), PC += offset */
		if ((STATUS & ST_LGT) && ! (STATUS & ST_EQ))
		{
			PC += (offset + offset);
			CYCLES(10, 3);
		}
		else
			CYCLES(8, 3);
		break;
	case 12:  /* JOP */
		/* JOP --- Jump On (odd) Parity */
		/* if (P==1), PC += offset */
		{
			/* Let's set ST_OP. */
			int i;
			UINT8 a;
				a = lastparity;
			i = 0;

			while (a != 0)
			{
				if (a & 1)  /* If current bit is set, */
					i++;      /* increment bit count. */
				a >>= 1U;   /* Next bit. */
			}

			/* Set ST_OP bit. */
			/*if (i & 1)
				STATUS |= ST_OP;
			else
				STATUS &= ~ ST_OP;*/

			/* Jump accordingly. */
			if (i & 1)  /*(STATUS & ST_OP)*/
			{
				PC += (offset + offset);
				CYCLES(10, 3);
			}
			else
				CYCLES(8, 3);
		}

		break;
	case 13:  /* SBO */
		/* SBO --- Set Bit to One */
		/* CRU Bit = 1 */
		writeCRU((READREG(R12) >> 1) + offset, 1, 1);
		CYCLES(12, 8);
		break;
	case 14:  /* SBZ */
		/* SBZ --- Set Bit to Zero */
		/* CRU Bit = 0 */
		writeCRU((READREG(R12) >> 1) + offset, 1, 0);
		CYCLES(12, 8);
		break;
	case 15:  /* TB */
		/* TB ---- Test Bit */
		/* EQ = (CRU Bit == 1) */
		setst_e(readCRU((READREG(R12)>> 1) + offset, 1) & 1, 1);
		CYCLES(12, 8);
		break;
	}
}


/*==========================================================================
   General and One-Register instructions                       >2000->3FFF
 ---------------------------------------------------------------------------

     0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ---------------------------------
    |  opcode   |   D   |TS |   S   |
    ---------------------------------

  COC, CZC, XOR, LDCR, STCR, XOP, MPY, DIV
tms9940 : DCA, DCS, LIIM
==========================================================================*/

/* xop, ldcr and stcr are handled elsewhere */
void tms99xx::h2000(UINT16 opcode)
{
	register UINT16 dest = (opcode & 0x3C0) >> 6;
	register UINT16 src;
	register UINT16 value;

	src = decipheraddr(opcode);

	switch ((opcode & 0x1C00) >> 10)
	{
	case 0:   /* COC */
		/* COC --- Compare Ones Corresponding */
		/* status E bit = (S&D == S) */
		dest = ((dest+dest) + WP) & ~1;
		src &= ~1;
		value = readword(src);
		setst_e(value & readword(dest), value);
		CYCLES(14, 4);
		break;
	case 1:   /* CZC */
		/* CZC --- Compare Zeroes Corresponding */
		/* status E bit = (S&~D == S) */
		dest = ((dest+dest) + WP) & ~1;
		src &= ~1;
		value = readword(src);
		setst_e(value & (~ readword(dest)), value);
		CYCLES(14, 4);
		break;
	case 2:   /* XOR */
		/* XOR --- eXclusive OR */
		/* D ^= S */
		dest = ((dest+dest) + WP) & ~1;
		src &= ~1;
		value = readword(dest) ^ readword(src);
		setst_lae(value);
		writeword(dest,value);
		CYCLES(14, 4);
		break;
	/*case 3:*/   /* XOP is implemented elsewhere */
	/*case 4:*/   /* LDCR is implemented elsewhere */
	/*case 5:*/   /* STCR is implemented elsewhere */
	case 6:   /* MPY */
		/* MPY --- MultiPlY  (unsigned) */
		/* Results:  D:D+1 = D*S */
		/* Note that early TMS9995 reportedly perform an extra dummy read in PC space */
		dest = ((dest+dest) + WP) & ~1;
		src &= ~1;
		{
			unsigned long prod = ((unsigned long) readword(dest)) * ((unsigned long) readword(src));
			writeword(dest, prod >> 16);
			writeword(dest+2, prod);
		}
		CYCLES(52, 23);
		break;
	case 7:   /* DIV */
		/* DIV --- DIVide    (unsigned) */
		/* D = D/S    D+1 = D%S */
		dest = ((dest+dest) + WP) & ~1;
		src &= ~1;
		{
			UINT16 d = readword(src);
			UINT16 hi = readword(dest);
			unsigned long divq = (((unsigned long) hi) << 16) | readword(dest+2);

			if (d <= hi)
			{
				STATUS |= ST_OV;
				CYCLES(16, 6);
			}
			else
			{
				STATUS &= ~ST_OV;
				writeword(dest, divq/d);
				writeword(dest+2, divq%d);
				/* tms9900 : from 92 to 124, possibly 92 + 2*(number of bits to 1 (or 0?) in quotient) */
				/* tms9995 : 28 is the worst case */
				CYCLES(92, 28);
			}
		}
		break;
	}
}

void tms99xx::xop(UINT16 opcode)
{	/* XOP */
	/* XOP --- eXtended OPeration */
	/* WP = *(40h+D), PC = *(42h+D) */
	/* New R13=old WP, New R14=Old IP, New R15=Old ST */
	/* New R11=S */
	/* Xop bit set */

	register UINT16 immediate = (opcode & 0x3C0) >> 6;
	register UINT16 operand;

	operand = decipheraddr(opcode);

  if (CpuType < TMS9995_ID) {
    (void)readword(operand & ~1); /*dummy read (personnal guess)*/
  }

  contextswitch(0x40 + (immediate << 2));
  /* The bit is not set on tms9940 */
	STATUS |= ST_X;
	WRITEREG(R11, operand);
	CYCLES(36, 15);
	disable_interrupt_recognition = 1;
}

/* LDCR and STCR */
void tms99xx::ldcr_stcr(UINT16 opcode)
{
	register UINT16 cnt = (opcode & 0x3C0) >> 6;
	register UINT16 addr;
	register UINT16 value;

	if (cnt == 0)
		cnt = 16;

	if (cnt <= 8)
		addr = decipheraddrbyte(opcode);
	else
		addr = decipheraddr(opcode) & ~1;

	if (opcode < 0x3400)
	{	/* LDCR */
		/* LDCR -- LoaD into CRu */
		/* CRU R12--CRU R12+D-1 set to S */
		if (cnt <= 8)
		{
#if (TMS99XX_MODEL != TMS9995_ID)
			value = readbyte(addr);
#else
			/* just for once, tms9995 behaves like earlier 8-bit tms99xx chips */
			/* this must be because instruction decoding is too complex */
			value = readword(addr);
			if (addr & 1)
				value &= 0xFF;
			else
				value = (value >> 8) & 0xFF;
#endif
			(void)READREG(cnt+cnt); /*dummy read (reasonnable guess, cf TMS9995)*/
			setst_byte_laep(value);
			writeCRU((READREG(R12) >> 1), cnt, value);
		}
		else
		{
			value = readword(addr);
			(void)READREG(cnt+cnt); /*dummy read (reasonnable guess, cf TMS9995)*/
			setst_lae(value);
			writeCRU((READREG(R12) >> 1), cnt, value);
		}
		CYCLES(20 + cnt+cnt, 9 + cnt+cnt);
	}
	else
	{	/* STCR */
		/* STCR -- STore from CRu */
		/* S = CRU R12--CRU R12+D-1 */
		if (cnt <= 8)
		{
#if (TMS99XX_MODEL != TMS9995_ID)
			(void)readbyte(addr); /*dummy read*/
			(void)READREG(cnt+cnt); /*dummy read (reasonnable guess, cf TMS9995)*/
			value = readCRU((READREG(R12) >> 1), cnt);
			setst_byte_laep(value);
			writebyte(addr, value);
			CYCLES((cnt != 8) ? 42 : 44, 19 + cnt);
#else
			/* just for once, tms9995 behaves like earlier 8-bit tms99xx chips */
			/* this must be because instruction decoding is too complex */
			int value2 = readword(addr);

			READREG(cnt+cnt); /*dummy read (guessed from timing table)*/
			value = readCRU((READREG(R12) >> 1), cnt);
			setst_byte_laep(value);

			if (addr & 1)
				writeword(addr, (value & 0x00FF) | (value2 & 0xFF00));
			else
				writeword(addr, (value2 & 0x00FF) | ((value << 8) & 0xFF00));

			CYCLES(Mooof!, 19 + cnt);
#endif
		}
		else
		{
			(void)readword(addr); /*dummy read*/
			(void)READREG(cnt+cnt); /*dummy read (reasonnable guess, cf TMS9995)*/
			value = readCRU((READREG(R12) >> 1), cnt);
			setst_lae(value);
			writeword(addr, value);
			CYCLES((cnt != 16) ? 58 : 60, 27 + cnt);
		}
	}
}


/*==========================================================================
   Two-Operand instructions                                    >4000->FFFF
 ---------------------------------------------------------------------------

      0 1 2 3-4 5 6 7+8 9 A B-C D E F
    ----------------------------------
    |opcode|B|TD |   D   |TS |   S   |
    ----------------------------------

  SZC, SZCB, S, SB, C, CB, A, AB, MOV, MOVB, SOC, SOCB
============================================================================*/

/* word instructions */
void tms99xx::h4000w(UINT16 opcode)
{
	register UINT16 src;
	register UINT16 dest;
	register UINT16 value;
	int a,b;

	src = decipheraddr(opcode) & ~1;
	dest = decipheraddr(opcode >> 6) & ~1;

	switch ((opcode >> 13) & 0x0007)    /* ((opcode & 0xE000) >> 13) */
	{
	case 2:   /* SZC */
		/* SZC --- Set Zeros Corresponding */
		/* D &= ~S */
		value = readword(dest) & (~ readword(src));
		setst_lae(value);
		writeword(dest, value);
		CYCLES(14, 4);
		break;
	case 3:   /* S */
		/* S ----- Subtract */
		/* D -= S */
		a = readword(dest);
		b = readword(src);
		value = setst_sub_laeco(a, b);
		writeword(dest, value);
		CYCLES(14, 4);
		break;
	case 4:   /* C */
		/* C ----- Compare */
		/* ST = (D - S) */
		a = readword(dest);
		b = readword(src);
		setst_c_lae(a, b);
		CYCLES(14, 4);
		break;
	case 5:   /* A */
		/* A ----- Add */
		/* D += S */
		a = readword(dest);
		b = readword(src);
		value = setst_add_laeco(a, b);
		writeword(dest, value);
		CYCLES(14, 4);
		break;
	case 6:   /* MOV */
		/* MOV --- MOVe */
		/* D = S */
		value = readword(src);
		setst_lae(value);
#if (TMS99XX_MODEL <= TMS9989_ID)
		/* MOV performs a dummy read... */
		(void)readword(dest);
#endif
		writeword(dest, value);
		CYCLES(14, 3);
		break;
	case 7:   /* SOC */
		/* SOC --- Set Ones Corresponding */
		/* D |= S */
		value = readword(dest) | readword(src);
		setst_lae(value);
		writeword(dest, value);
		CYCLES(14, 4);
		break;
	}
}

/* byte instruction */
void tms99xx::h4000b(UINT16 opcode)
{
	register UINT16 src;
	register UINT16 dest;
	register UINT16 value;

	src = decipheraddrbyte(opcode);
	dest = decipheraddrbyte(opcode >> 6);

	switch ((opcode >> 13) & 0x0007)    /* ((opcode & 0xE000) >> 13) */
	{
	case 2:   /* SZCB */
		/* SZCB -- Set Zeros Corresponding, Byte */
		/* D &= ~S */
		value = readbyte(dest) & (~ readbyte(src));
		setst_byte_laep(value);
		writebyte(dest, value);
		CYCLES(14, 4);
		break;
	case 3:   /* SB */
		/* SB ---- Subtract, Byte */
		/* D -= S */
		value = setst_subbyte_laecop(readbyte(dest), readbyte(src));
		writebyte(dest, value);
		CYCLES(14, 4);
		break;
	case 4:   /* CB */
		/* CB ---- Compare Bytes */
		/* ST = (D - S) */
		value = readbyte(src);
		setst_c_lae(readbyte(dest)<<8, value<<8);
		lastparity = value;
		CYCLES(14, 4);
		break;
	case 5:   /* AB */
		/* AB ---- Add, Byte */
		/* D += S */
		value = setst_addbyte_laecop(readbyte(dest), readbyte(src));
		writebyte(dest, value);
		break;
	case 6:   /* MOVB */
		/* MOVB -- MOVe Bytes */
		/* D = S */
		value = readbyte(src);
		setst_byte_laep(value);
#if (TMS99XX_MODEL <= TMS9989_ID)
		/* on tms9900, MOVB needs to read destination, because it cannot actually read one single byte.
		  It reads a word, replaces the revelant byte, then write the result */
		/* A tms9980 theorically does not need to do so, but still does... */
		readbyte(dest);
#endif
		writebyte(dest, value);
		CYCLES(14, 3);
		break;
	case 7:   /* SOCB */
		/* SOCB -- Set Ones Corresponding, Byte */
		/* D |= S */
		value = readbyte(dest) | readbyte(src);
		setst_byte_laep(value);
		writebyte(dest, value);
		CYCLES(14, 4);
		break;
	}
}

__inline void tms99xx::exec(UINT16 opcode)
{
  if (CpuType < TMS9995_ID)
  {
    /* tms9900-like instruction set*/
    tmsopcode jumptable[128] =
    {
      &tms99xx::illegal,&tms99xx::h0200,&tms99xx::h0400,&tms99xx::h0400,
      &tms99xx::h0800,&tms99xx::h0800,&tms99xx::illegal,&tms99xx::illegal,
      &tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,
      &tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::xop,&tms99xx::xop,
      &tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b
    };

    callMemberFunction(this, jumptable[opcode >> 9]) (opcode);
  } else
  {
    /* tms9989 and tms9995 include 4 extra instructions, and one additionnal instruction type */
    static void (tms99xx::* jumptable[256])(UINT16) =
    {
      &tms99xx::h0040,&tms99xx::h0100,&tms99xx::h0200,&tms99xx::h0200,
      &tms99xx::h0400,&tms99xx::h0400,&tms99xx::h0400,&tms99xx::h0400,
      &tms99xx::h0800,&tms99xx::h0800,&tms99xx::h0800,&tms99xx::h0800,
      &tms99xx::illegal,&tms99xx::illegal,&tms99xx::illegal,&tms99xx::illegal,
      &tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,
      &tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,
      &tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,
      &tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,&tms99xx::h1000,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::xop,&tms99xx::xop,&tms99xx::xop,&tms99xx::xop,
      &tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,
      &tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,&tms99xx::ldcr_stcr,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,&tms99xx::h2000,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,&tms99xx::h4000w,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,
      &tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b,&tms99xx::h4000b
    };

    callMemberFunction(this, jumptable[opcode >> 8]) (opcode);
  }
}

/************************************************************************
 * Status register functions
 ************************************************************************/
/************************************************************************

	ST register functions

************************************************************************/

/*
	remember that the OP ST bit is maintained in lastparity
*/

/*
	setstat sets the ST_OP bit according to lastparity

	It must be called before reading the ST register.
*/

void tms99xx::setstat(void)
{
	int i;
	UINT8 a;

	STATUS &= ~ ST_OP;

	/* We set the parity bit. */
	a = lastparity;

	for (i=0; i<8; i++)     /* 8 bits to test */
	{
		if (a & 1)  /* If current bit is set */
			STATUS ^= ST_OP;  /* we toggle the ST_OP bit */

		a >>= 1;    /* Next bit. */
	}
}

/*
	getstat sets emulator's lastparity variable according to 9900's STATUS bits.
	It must be called on interrupt return, or when, for some reason,
	the emulated program sets the STATUS register directly.
*/
void tms99xx::getstat(void)
{
  if (CpuType < TMS9995_ID)
	  STATUS &= ST_MASK;  /* unused bits are forced to 0 */

	if (STATUS & ST_OP)
		lastparity = 1;
	else
		lastparity = 0;
}

/*
	A few words about the following functions.

	A big portability issue is the behavior of the ">>" instruction with the sign bit, which has
	not been normalised.  Every compiler does whatever it thinks smartest.
	My code assumed that when shifting right signed numbers, the operand is left-filled with a
	copy of sign bit, and that when shifting unsigned variables, it is left-filled with 0s.
	This is probably the most logical behaviour, and it is the behavior of CW PRO3 - most time
	(the exception is that ">>=" instructions always copy the sign bit (!)).  But some compilers
	are bound to disagree.

	So, I had to create special functions with predefined tables included, so that this code work
	on every compiler.  BUT this is a real slow-down.
	So, you might have to include a few lines in assembly to make this work better.
	Sorry about this, this problem is really unpleasant and absurd, but it is not my fault.
*/

static const UINT16 right_shift_mask_table[17] =
{
	0xFFFF,
	0x7FFF,
	0x3FFF,
	0x1FFF,
	0x0FFF,
	0x07FF,
	0x03FF,
	0x01FF,
	0x00FF,
	0x007F,
	0x003F,
	0x001F,
	0x000F,
	0x0007,
	0x0003,
	0x0001,
	0x0000
};

static const UINT16 inverted_right_shift_mask_table[17] =
{
	0x0000,
	0x8000,
	0xC000,
	0xE000,
	0xF000,
	0xF800,
	0xFC00,
	0xFE00,
	0xFF00,
	0xFF80,
	0xFFC0,
	0xFFE0,
	0xFFF0,
	0xFFF8,
	0xFFFC,
	0xFFFE,
	0xFFFF
};

UINT16 tms99xx::logical_right_shift(UINT16 val, int c)
{
	return((val>>c) & right_shift_mask_table[c]);
}

INT16 tms99xx::arithmetic_right_shift(INT16 val, int c)
{
	if (val < 0)
		return((val>>c) | inverted_right_shift_mask_table[c]);
	else
		return((val>>c) & right_shift_mask_table[c]);
}

/*
	Set lae
*/
__inline void tms99xx::setst_lae(INT16 val)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ);

	if (val > 0)
		STATUS |= (ST_LGT | ST_AGT);
	else if (val < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;
}

/*
	Set laep (BYTE)
*/
__inline void tms99xx::setst_byte_laep(INT8 val)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ);

	if (val > 0)
		STATUS |= (ST_LGT | ST_AGT);
	else if (val < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	lastparity = val;
}

/*
	For COC, CZC, and TB
*/
__inline void tms99xx::setst_e(UINT16 val, UINT16 to)
{
	if (val == to)
		STATUS |= ST_EQ;
	else
		STATUS &= ~ ST_EQ;
}

/*
	For CI, C, CB
*/
__inline void tms99xx::setst_c_lae(UINT16 to, UINT16 val)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ);

	if (val == to)
		STATUS |= ST_EQ;
	else
	{
		if ( ((INT16) val) > ((INT16) to) )
			STATUS |= ST_AGT;
		if ( ((UINT16) val) > ((UINT16) to) )
		STATUS |= ST_LGT;
	}
}

/*
	Set laeco for add
*/
__inline INT16 tms99xx::setst_add_laeco(int a, int b)
{
	UINT32 res;
	INT16 res2;

	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV);

	res = (a & 0xffff) + (b & 0xffff);

	if (res & 0x10000)
		STATUS |= ST_C;

	if ((res ^ b) & (res ^ a) & 0x8000)
		STATUS |= ST_OV;

	res2 = (INT16) res;

	if (res2 > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (res2 < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	return res2;
}

/*
	Set laeco for subtract
*/
__inline INT16 tms99xx::setst_sub_laeco(int a, int b)
{
	UINT32 res;
	INT16 res2;

	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV);

	res = (a & 0xffff) - (b & 0xffff);

	if (! (res & 0x10000))
		STATUS |= ST_C;

	if ((a ^ b) & (a ^ res) & 0x8000)
		STATUS |= ST_OV;

	res2 = (INT16) res;

	if (res2 > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (res2 < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	return res2;
}

/*
	Set laecop for add (BYTE)
*/
__inline INT8 tms99xx::setst_addbyte_laecop(int a, int b)
{
	unsigned int res;
	INT8 res2;

	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV | ST_OP);

	res = (a & 0xff) + (b & 0xff);

	if (res & 0x100)
		STATUS |= ST_C;

	if ((res ^ b) & (res ^ a) & 0x80)
		STATUS |= ST_OV;

	res2 = (INT8) res;

	if (res2 > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (res2 < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	lastparity = res2;

	return res2;
}

/*
	Set laecop for subtract (BYTE)
*/
__inline INT8 tms99xx::setst_subbyte_laecop(int a, int b)
{
	unsigned int res;
	INT8 res2;

	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV | ST_OP);

	res = (a & 0xff) - (b & 0xff);

	if (! (res & 0x100))
		STATUS |= ST_C;

	if ((a ^ b) & (a ^ res) & 0x80)
		STATUS |= ST_OV;

	res2 = (INT8) res;

	if (res2 > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (res2 < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	lastparity = res2;

	return res2;
}

/*
	For NEG
*/
__inline void tms99xx::setst_laeo(INT16 val)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_OV);

	if (val > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (val < 0)
	{
	  STATUS |= ST_LGT;
	  if (((UINT16) val) == 0x8000)
		  STATUS |= ST_OV;
	}
	else
		STATUS |= ST_EQ;
}

/*
	Meat of SRA
*/
__inline UINT16 tms99xx::setst_sra_laec(INT16 a, UINT16 c)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C);

	if (c != 0)
	{
		a = arithmetic_right_shift(a, c-1);
		if (a & 1)  // The carry bit equals the last bit that is shifted out
			STATUS |= ST_C;
		a = arithmetic_right_shift(a, 1);
	}

	if (a > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (a < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	return a;
}

/*
	Meat of SRL.  Same algorithm as SRA, except that we fills in with 0s.
*/
__inline UINT16 tms99xx::setst_srl_laec(UINT16 a,UINT16 c)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C);

	if (c != 0)
	{
		a = logical_right_shift(a, c-1);
		if (a & 1)
			STATUS |= ST_C;
		a = logical_right_shift(a, 1);
	}

	if (((INT16) a) > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (((INT16) a) < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	return a;
}

//
// Meat of SRC
//
__inline UINT16 tms99xx::setst_src_laec(UINT16 a,UINT16 c)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C);

	if (c != 0)
	{
		a = logical_right_shift(a, c) | (a << (16-c));
		if (a & 0x8000) // The carry bit equals the last bit that is shifted out
			STATUS |= ST_C;
	}

	if (((INT16) a) > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (((INT16) a) < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	return a;
}

//
// Meat of SLA
//
__inline UINT16 tms99xx::setst_sla_laeco(UINT16 a, UINT16 c)
{
	STATUS &= ~ (ST_LGT | ST_AGT | ST_EQ | ST_C | ST_OV);

	if (c != 0)
	{
		{
			register UINT16 mask;
			register UINT16 ousted_bits;

			mask = 0xFFFF << (16-c-1);
			ousted_bits = a & mask;

			if (ousted_bits)        // If ousted_bits is neither all 0s
				if (ousted_bits ^ mask)   // nor all 1s,
					STATUS |= ST_OV;  // we set overflow
		}

		a <<= c-1;
		if (a & 0x8000) // The carry bit equals the last bit that is shifted out
			STATUS |= ST_C;

		a <<= 1;
	}

	if (((INT16) a) > 0)
		STATUS |= ST_LGT | ST_AGT;
	else if (((INT16) a) < 0)
		STATUS |= ST_LGT;
	else
		STATUS |= ST_EQ;

	return a;
}

/*****************************************************************************
 *	Disassemble a single command and return the number of bytes it uses.
 *****************************************************************************/
char *tms99xx::print_arg (int mode, int arg)
{
	static char temp[20];
	int	base;

	switch (mode)
	{
		case 0x0:	/* workspace register */
			sprintf (temp, "R%d", arg);
			break;
		case 0x1:	/* workspace register indirect */
			sprintf (temp, "*R%d", arg);
			break;
		case 0x2:	/* symbolic|indexed */
			base = RDWORD(OLD_PC); OLD_PC+=2;
			if (arg) 	/* indexed */
				sprintf (temp, "@$%04x(R%d)", base, arg);
			else		/* symbolic (direct) */
				sprintf (temp, "@$%04x", base);
			break;
		case 0x3:	/* workspace register indirect auto increment */
			sprintf (temp, "*R%d+", arg);
			break;
	}
	return temp;
}

int tms99xx::dasm(int pc, char *ptr)
{
	int	OP, opc;
	int sarg, darg, smode, dmode;
    char op[40], *p = op;

	OLD_PC = pc;
	OP = RDOP(OLD_PC); OLD_PC+=2;

	if ((opc = ops0to3[BITS_0to3]) != _ill)
	{
		smode = OPBITS(10,11);
		sarg = OPBITS(12,15);
		dmode = OPBITS(4,5);
		darg = OPBITS(6,9);

 		p += sprintf (p, "%-4s ", token[opc]);
		strcat (p, print_arg (smode, sarg));
		strcat (p, ",");
		strcat (p, print_arg (dmode, darg));
	}
	else if (BITS_0to1==0 && (opc = ops2to5[BITS_2to5]) != _ill)
	{
		smode = OPBITS(10,11);
		sarg = OPBITS(12,15);
		darg = OPBITS(6,9);

		if (darg==0 && (opc==_ldcr || opc==_stcr))
			darg = 16;

		if (opc==_xop || opc==_ldcr || opc==_stcr)
			p += sprintf (p, "%-4s %s,%d", token[opc], print_arg (smode, sarg), darg);
		else	/* _coc, _czc, _xor, _mpy, _div */
			p += sprintf (p, "%-4s %s,R%d", token[opc], print_arg (smode, sarg), darg);
	}
	else if (BITS_0to2==0 && (opc = ops3to7[BITS_3to7]) != _ill)
	{
		switch (opc)
		{
			case _sra: case _srl: case _sla: case _src:
				sarg = OPBITS(12,15);
				darg = OPBITS(8,11);

				p += sprintf (p, "%-4s R%d,%d", token[opc], sarg, darg);
				break;
			case _jmp: case _jlt: case _jle: case _jeq: case _jhe: case _jgt:
			case _jne: case _jnc: case _joc: case _jno: case _jl:  case _jh: case _jop:
				{
					signed char displacement;

					displacement = (signed char)OPBITS(8,15);
					p += sprintf (p, "%-4s $%04x", token[opc], 0xffff & (OLD_PC + displacement * 2));
				}
				break;
			case _sbo: case _sbz: case _tb:
				{
					signed char displacement;

					displacement = (signed char)OPBITS(8,15);
					p += sprintf (p, "%-4s $%04x", token[opc], 0xffff & displacement);
				}
				break;
		}
	}
	else if (BITS_0to4==0 && (opc = ops5to9[BITS_5to9]) != _ill)
	{
		smode = OPBITS(10,11);
		sarg = OPBITS(12,15);

		p += sprintf (p, "%-4s %s", token[opc], print_arg (smode, sarg));
	}
	else if (BITS_0to5==0 && (opc = ops6to10[BITS_6to10]) != _ill)
	{
		switch (opc)
		{
			case _li:   case _ai:   case _andi: case _ori:  case _ci:
				darg = OPBITS(12,15);
				sarg = RDWORD(OLD_PC); OLD_PC+=2;

				p += sprintf (p, "%-4s R%d,$%04x", token[opc], darg, sarg);
				break;
			case _lwpi: case _limi:
				sarg = RDWORD(OLD_PC); OLD_PC+=2;

				p += sprintf (p, "%-4s $%04x", token[opc], sarg);
				break;
			case _stwp: case _stst:
				sarg = OPBITS(12,15);

				p += sprintf (p, "%-4s R%d", token[opc], sarg);
				break;
			case _idle: case _rset: case _rtwp: case _ckon: case _ckof: case _lrex:
				p += sprintf (p, "%-4s", token[opc]);
				break;
		}
	}
	else
		p += sprintf (p, "data $%04x", OP);

    ptr += sprintf( ptr, "%04X   ", pc);

    for ( int i = 0; i <= 5; i++ )
      if ( i < OLD_PC - pc )
        ptr += sprintf( ptr, "%02x ", read(pc+i));
      else
        ptr += sprintf( ptr, "   ");

    sprintf(ptr, "%s", op);

	return OLD_PC - pc;
}

void __fastcall tms99xx::SaveToStream(Byte *&ptr)
{
/*
  ptr->WriteBuffer(&pc, sizeof(PAIR));
  ptr->WriteBuffer(&ppc, sizeof(PAIR));
  ptr->WriteBuffer(&dd, sizeof(PAIR));
  ptr->WriteBuffer(&dp, sizeof(PAIR));
  ptr->WriteBuffer(&u, sizeof(PAIR));
  ptr->WriteBuffer(&s, sizeof(PAIR));
  ptr->WriteBuffer(&x, sizeof(PAIR));
  ptr->WriteBuffer(&y, sizeof(PAIR));
  ptr->WriteBuffer(&ea, sizeof(PAIR));
  ptr->WriteBuffer(&cc, 1);
  ptr->WriteBuffer(&ireg, 1);
  ptr->WriteBuffer(&irq_state, 2);
//  ptr->WriteBuffer(&extra_cycles, 4);
  ptr->WriteBuffer(&irq_state, 1);
//  ptr->WriteBuffer(&nmi_state, 1);
//  ptr->WriteBuffer(&cycles, 4);
*/
}

void __fastcall tms99xx::LoadFromStream(Byte *&ptr)
{
/*
  ptr->ReadBuffer(&pc, sizeof(PAIR));
  ptr->ReadBuffer(&ppc, sizeof(PAIR));
  ptr->ReadBuffer(&dd, sizeof(PAIR));
  ptr->ReadBuffer(&dp, sizeof(PAIR));
  ptr->ReadBuffer(&u, sizeof(PAIR));
  ptr->ReadBuffer(&s, sizeof(PAIR));
  ptr->ReadBuffer(&x, sizeof(PAIR));
  ptr->ReadBuffer(&y, sizeof(PAIR));
  ptr->ReadBuffer(&ea, sizeof(PAIR));
  ptr->ReadBuffer(&cc, 1);
  ptr->ReadBuffer(&ireg, 1);
  ptr->ReadBuffer(&irq_state, 2);
//  ptr->ReadBuffer(&extra_cycles, 4);
  ptr->ReadBuffer(&irq_state, 1);
//  ptr->ReadBuffer(&nmi_state, 1);
//  ptr->ReadBuffer(&cycles, 4);
*/
}

//---------------------------------------------------------------------------
#pragma package(smart_init)

