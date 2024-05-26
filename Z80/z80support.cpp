/***************************************************************************

  Z80 FMLY.C   Z80 FAMILY CHIP EMURATOR for MAME Ver.0.1 alpha

  Support chip :  Z80PIO , Z80CTC

  Copyright(C) 1997 Tatsuyuki Satoh.

  This version are tested starforce driver.

  8/21/97 -- Heavily modified by Aaron Giles to be much more accurate for the MCR games
  8/27/97 -- Rewritten a second time by Aaron Giles, with the datasheets in hand

pending:
	Z80CTC , Counter mode & Timer with Trigrt start :not support Triger level

***************************************************************************/

#include <stdio.h>
#include "z80support.h"
#include "z80.h"

typedef struct
{
	int vector;                 /* interrupt vector */
	int clock;                  /* system clock */
	double invclock16;          /* 16/system clock */
	double invclock256;         /* 256/system clock */
	void (*intr)(int which);    /* interrupt callback */
//	mem_write_handler zc[4];    /* zero crossing callbacks */
	int notimer;                /* no timer masks */
	int mask[4];                /* masked channel flags */
	int mode[4];                /* current mode */
	int tconst[4];              /* time constant */
	int down[4];                /* down counter (clock mode only) */
	int extclk[4];              /* current signal from the external clock */
	void *timer[4];             /* array of active timers */

	int int_state[4];           /* interrupt status (for daisy chain) */
} z80ctc;


/* these are the bits of the incoming commands to the CTC */
#define INTERRUPT			0x80
#define PRESCALER			0x20
#define CONSTANT			0x04
#define RESET				0x02
#define CONTROL			0x01

/* interrupt request callback with daisy-chain circuit */
int __fastcall Z80CTC::interrupt_check()
{
	int state = 0;
	int ch;

	for( ch = 0; ch <= 3; ch++ )
	{
		/* if IEO disable , same and lower IRQ is masking */
/* ASG: changed this line because this state could have an interrupt pending as well! */
/*		if( int_state[ch] & Z80_INT_IEO ) state  = Z80_INT_IEO;*/
		if( int_state[ch] ) {
		  if( int_state[ch] & Z80_INT_REQ ) {
//            if (  int_state[ch] & Z80_INT_IEO )
//              state = int_state[ch];
		    if( (int_state[ch] & (Z80_INT_REQ | Z80_INT_IEO)) == Z80_INT_REQ )
        state = int_state[ch];
	}
      break;
    }
	}

  return state;
}


void __fastcall Z80CTC::reset ()
{
	/* set up defaults */
	for (int i = 0; i < 4; i++)
	{
		mode[i] = RESET;
		tconst[i] = 0x100;
		int_state[i] = 0;
                prescaler[i] = 0;
	}
}

void __fastcall Z80CTC::w (int offset, int data)
{
	int mde, ch;

	/* keep channel within range, and get the current mode */
	ch = offset & 3;
	mde = mode[ch];

	/* if we're waiting for a time constant, this is it */
	if (mde & CONSTANT)	{
		/* set the time constant (0 -> 0x100) */
		tconst[ch] = data ? data : 0x100;

		/* clear the internal mode -- we're no longer waiting */
		mode[ch] &= ~CONSTANT;

		/* also clear the reset, since the constant gets it going again */
		mode[ch] &= ~RESET;

		/* set the down counter cause we're clocking externally */
		down[ch] = tconst[ch];

		/* all done here */
		return;
	}

	/* if we're writing the interrupt vector, handle it specially */
#if 0	/* Tatsuyuki Satoh changes */
	/* The 'Z80family handbook' wrote,                            */
	/* interrupt vector is able to set for even channel (0 or 2)  */
	if (!(data & CONTROL) && (ch&1) == 0)
#else
	if (!(data & CONTROL) && ch == 0)
#endif
	{
		vector = data & 0xf8;
		return;
	}

	/* this must be a control word */
	if (data & CONTROL)	{
		/* set the new mode */
		mode[ch] = data;

		/* if we're being reset, clear out any pending timers for this channel */
		if (data & RESET) {
			if (int_state[ch] != 0) {
				/* clear interrupt service , request */
				int_state[ch] = 0;
//				interrupt_check();
			}
		}

		/* all done here */
		return;
	}
}

int __fastcall Z80CTC::r (int ch)
{
	/* keep channel within range */
	ch &= 3;

		return down[ch];
	}

int __fastcall Z80CTC::interrupt()
{
	int ch;

	for (ch = 0 ; ch < 4 ; ch++) {
		if (int_state[ch]) {
      // Current timer is either pending or being processed
			if( int_state[ch] & Z80_INT_REQ)
				int_state[ch] = Z80_INT_IEO;
			break;
		}
	}
	if( ch > 3 )
		ch = 0;

	return vector + ch * 2;
}

// when RETI occurs, this function is called to clear down the current and permit the next pending interrupt
void __fastcall Z80CTC::reti()
{
	int ch;

	for( ch = 0 ; ch < 4 ; ch++ ) {
		if( int_state[ch] & Z80_INT_IEO ) {
			// Highest served interrupt found
			int_state[ch] &= ~Z80_INT_IEO; // clear the "being serviced flag"
			break; // only clear one flag
		}
	}
}

void __fastcall Z80CTC::tick(unsigned int cycles)
{
  int i;
  int scaler;

  for (i = 0; i < 4; i++) {
    if (!tconst[i])
      continue;
    prescaler[i] += cycles;
    scaler = (mode[i] & PRESCALER) ? 256 : 16;
    down[i] -= (prescaler[i] / scaler);
    prescaler[i] %= scaler;
    if (down[i] <= 0) {
	/* down counter has reached zero - see if we should interrupt */
	    if (mode[i] & INTERRUPT) {
//		    if( !(int_state[i] & Z80_INT_REQ) )	{
			    int_state[i] |= Z80_INT_REQ;
//	}
			}

	    /* reset the down counter */
	    down[i] += tconst[i];
		}
	}
}
/*---------------------- Z80 PIO ---------------------------------*/

/* starforce emurate Z80PIO subset */
/* ch.A mode 1 input handshake mode from sound command */
/* ch.b not use */


#define PIO_MODE0 0x00		/* output mode */
#define PIO_MODE1 0x01		/* input  mode */
#define PIO_MODE2 0x02		/* i/o    mode */
#define PIO_MODE3 0x03		/* bit    mode */
/* pio controll port operation (bit 0-3) */
#define PIO_OP_MODE 0x0f	/* mode select        */
#define PIO_OP_INTC 0x07	/* interrupt controll */
#define PIO_OP_INTE 0x03	/* interrupt enable   */
#define PIO_OP_INTE 0x03	/* interrupt enable   */
/* pio interrupt controll nit */
#define PIO_INT_ENABLE 0x80  /* ENABLE : 0=disable , 1=enable */
#define PIO_INT_AND    0x40  /* LOGIC  : 0=OR      , 1=AND    */
#define PIO_INT_HIGH   0x20  /* LEVEL  : 0=low     , 1=high   */
#define PIO_INT_MASK   0x10  /* MASK   : 0=off     , 1=on     */

void __fastcall Z80PIO::set_rdy( int ch, int state)
{
//	/* set state */
//	rdy[ch] = state;
//
//	/* call callback with state */
//	if (rdyr[ch]!=0)
//		rdyr[ch](rdy[ch]);
}

void __fastcall Z80PIO::interrupt_check()
{
//	int state;
//
//	if( int_state[1] & Z80_INT_IEO ) state  = Z80_INT_IEO;
//	else                                  state  = int_state[1];
//	if( int_state[0] & Z80_INT_IEO ) state  = Z80_INT_IEO;
//	else                                  state |= int_state[0];
//	/* change daisy chain status */
//	if (intr) (*intr)(state);
}

void __fastcall Z80PIO::check_irq( int ch )
{
	int irq = 0;
	int data;
	int old_state;

	if( enable[ch] & PIO_INT_ENABLE )
	{
		if( mode[ch] == PIO_MODE3 )
		{
			data  =  in[ch] & dir[ch]; /* input data only */
			data &= ~mask[ch];              /* mask follow     */
			if( !(enable[ch]&PIO_INT_HIGH) )/* active level    */
				data ^= mask[ch];             /* active low  */
			if( enable[ch]&PIO_INT_AND )    /* logic      */
			     { if( data == mask[ch] ) irq = 1; }
			else { if( data == 0             ) irq = 1; }
			/* if portB , portA mode 2 check */
			if( ch && (mode[0]==PIO_MODE2) )
			{
				if( rdy[ch] == 0 ) irq = 1;
			}
		}
		else if( rdy[ch] == 0 ) irq = 1;
	}
	old_state = int_state[ch];
	if( irq ) int_state[ch] |=  Z80_INT_REQ;
	else      int_state[ch] &= ~Z80_INT_REQ;

	if( old_state != int_state[ch] )
		interrupt_check();
}

void __fastcall Z80PIO::reset ()
{
	int i;

	for( i = 0 ; i <= 1 ; i++){
		mask[i]   = 0xff;	/* mask all on */
		enable[i] = 0x00;	/* disable     */
		mode[i]   = 0x01;	/* mode input  */
		dir[i]    = 0x01;	/* dir  input  */
		set_rdy(i,0);	/* RDY = low   */
		out[i]    = 0x00;	/* outdata = 0 */
		int_state[i] = 0;
		strobe[i] = 0;
	}
	interrupt_check( );
}

/* pio data register write */
void __fastcall Z80PIO::d_w(int ch , int data )
{
	if( ch ) ch = 1;

	out[ch] = data;	/* latch out data */
	switch( mode[ch] ){
	case PIO_MODE0:			/* mode 0 output */
	case PIO_MODE2:			/* mode 2 i/o */
		set_rdy( ch,1); /* ready = H */
		check_irq(ch );
		return;
	case PIO_MODE1:			/* mode 1 intput */
	case PIO_MODE3:			/* mode 0 bit */
		return;
	default:
		;//logerror("PIO-%c data write,bad mode\n",'A'+ch );
	}
}

/* pio controll register write */
void __fastcall Z80PIO::c_w(int ch , int data )
{
	if( ch ) ch = 1;

	/* load direction phase ? */
	if( mode[ch] == 0x13 ){
		dir[ch] = data;
		mode[ch] = 0x03;
		return;
	}
	/* load mask folows phase ? */
	if( enable[ch] & PIO_INT_MASK ){	/* load mask follows */
		mask[ch] = data;
		enable[ch] &= ~PIO_INT_MASK;
//		logerror("PIO-%c interrupt mask %02x\n",'A'+ch,data );
		return;
	}
	switch( data & 0x0f ){
	case PIO_OP_MODE:	/* mode select 0=out,1=in,2=i/o,3=bit */
		mode[ch] = (data >> 6 );
		if( mode[ch] == 0x03 ) mode[ch] = 0x13;
//		logerror("PIO-%c Mode %x\n",'A'+ch,mode[ch] );
		break;
	case PIO_OP_INTC:		/* interrupt control */
		enable[ch] = data & 0xf0;
		mask[ch]   = 0x00;
		/* when interrupt enable , set vector request flag */
//		logerror("PIO-%c Controll %02x\n",'A'+ch,data );
		break;
	case PIO_OP_INTE:		/* interrupt enable control */
		enable[ch] &= ~PIO_INT_ENABLE;
		enable[ch] |= (data & PIO_INT_ENABLE);
//		logerror("PIO-%c enable %02x\n",'A'+ch,data&0x80 );
		break;
	default:
        if( !(data&1) )
        {
            vector[ch] = data;
//				logerror("PIO-%c vector %02x\n",'A'+ch,data);
        }
//        else logerror("PIO-%c illegal command %02x\n",'A'+ch,data );
	}
	/* interrupt check */
	check_irq(ch );
}

/* pio controll register read */
int __fastcall Z80PIO::c_r(int ch )
{
	if( ch ) ch = 1;

//	logerror("PIO-%c controll read\n",'A'+ch );
	return 0;
}

/* pio data register read */
int __fastcall Z80PIO::d_r(int ch )
{
	if( ch ) ch = 1;

	switch( mode[ch] ){
	case PIO_MODE0:			/* mode 0 output */
		return out[ch];
	case PIO_MODE1:			/* mode 1 intput */
		set_rdy(ch, 1);	/* ready = H */
		check_irq(ch );
		return in[ch];
	case PIO_MODE2:			/* mode 2 i/o */
//		if( ch ) logerror("PIO-B mode 2 \n");
		set_rdy(1, 1); /* brdy = H */
		check_irq(ch );
		return in[ch];
	case PIO_MODE3:			/* mode 3 bit */
		return (in[ch]&dir[ch])|(out[ch]&~dir[ch]);
	}
//	logerror("PIO-%c data read,bad mode\n",'A'+ch );
	return 0;
}

int __fastcall Z80PIO::interrupt()
{
	int ch = 0;

	/* port A */
	if( int_state[0] == Z80_INT_REQ )
	{
		int_state[0] |= Z80_INT_IEO;
	} if( int_state[0] == 0 )
	{
		/* port B */
		ch = 1;
		if( int_state[1] == Z80_INT_REQ )
		{
			int_state[1] |= Z80_INT_IEO;
		}
		else
		{
//			logerror("PIO entry INT : non IRQ\n");
			ch = 0;
		}
	}
	interrupt_check( );
	return vector[ch];
}

void __fastcall Z80PIO::reti()
{

	if( int_state[0] & Z80_INT_IEO )
	{
		int_state[0] &= ~Z80_INT_IEO;
	} else if( int_state[1] & Z80_INT_IEO )
	{
		int_state[1] &= ~Z80_INT_IEO;
	}
	/* set next interrupt stattus */
	interrupt_check( );
}

/* z80pio port write */
void __fastcall Z80PIO::p_w(int ch , int data )
{
	if( ch ) ch = 1;

	in[ch]  = data;
	switch( mode[ch] ){
	case PIO_MODE0:
//		logerror("PIO-%c OUTPUT mode and data write\n",'A'+ch );
		break;
	case PIO_MODE2:	/* only port A */
		ch = 1;		/* handshake and IRQ is use portB */
	case PIO_MODE1:
		set_rdy( ch, 0);
		check_irq( ch );
		break;
	case PIO_MODE3:
		/* irq check */
		check_irq( ch );
		break;
	}
}

/* z80pio port read */
int __fastcall Z80PIO::p_r(int ch )
{
	if( ch ) ch = 1;

	switch( mode[ch] ){
	case PIO_MODE2:		/* port A only */
	case PIO_MODE0:
		set_rdy(ch, 0);
		check_irq(ch );
		break;
	case PIO_MODE1:
//		logerror("PIO-%c INPUT mode and data read\n",'A'+ch );
		break;
	case PIO_MODE3:
		/*     input bits                , output bits                */
		return (in[ch]&dir[ch])|(out[ch]&~dir[ch]);
	}
	return out[ch];
}

/* for mame interface */

//void __fastcall Z80PIO::0_reset (void) { reset (0); }

void __fastcall Z80PIO::w(int offset, int data)
{
	if(offset&1) c_w((offset/2)&1,data);
	else         d_w((offset/2)&1,data);
}

int __fastcall Z80PIO::r(int offset )
{
	return (offset&1) ? c_r((offset/2)&1) : d_r((offset/2)&1);
}

void __fastcall Z80PIO::update_strobe(int ch, int state)
{
	if (ch) ch=1;

	switch (mode[ch])
	{
		/* output mode */
		case PIO_MODE0:
		{
			/* ensure valid */
			state = state & 0x01;

			/* strobe changed state? */
			if ((strobe[ch]^state)!=0)
			{
				/* yes */
				if (state!=0)
				{
					/* positive edge */
//					logerror("PIO-%c positive strobe\n",'A'+ch );
					/* ready is now inactive */
					set_rdy( ch, 0);

					/* int enabled? */
					if (enable[ch] & PIO_INT_ENABLE)
					{
						/* trigger an int request */
						int_state[ch] |= Z80_INT_REQ;
					}
				}
			}

			/* store strobe state */
			strobe[ch] = state;

			/* check interrupt */
			interrupt_check( );
		}
		break;

		default:
			break;
	}
}

/* set /astb input */
/* /astb is active low */
/* output mode: a positive edge is used by peripheral to acknowledge
the receipt of data */
/* input mode: strobe is used by peripheral to load data from the peripheral
into port a input register, data loaded into pio when signal is active */
void __fastcall Z80PIO::astb_w(int which, int state)
{
	update_strobe(0, state);
}

/* set bstb input */
void __fastcall Z80PIO::bstb_w(int which, int state)
{
	update_strobe(1, state);
}

