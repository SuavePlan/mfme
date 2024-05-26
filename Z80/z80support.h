/*  Z80 FMLY.H   Z80 FAMILY IC EMULATION */

#ifndef __z80support_h__
#define __z80support_h__

class Z80CTC {
public:
	int vector;                 /* interrupt vector */
	int notimer;                /* no timer masks */
	int mask[4];                /* masked channel flags */
	int mode[4];                /* current mode */
	int tconst[4];              /* time constant */
	int down[4];                /* down counter (clock mode only) */
	int int_state[4];           /* interrupt status (for daisy chain) */
        int prescaler[4];
        int __fastcall      interrupt_check();

public:
  void __fastcall reset ();
  void __fastcall w (int offset, int data);
  int __fastcall r (int offset);
  void __fastcall     tick(unsigned int cycles);

/* Z80 DaisyChain control */
  int __fastcall interrupt();
  void __fastcall reti();
};

/*--------------------------------------------------------------------*/

class Z80PIO {
public:
	int vector[2];                        /* interrupt vector               */
	void (*intr)(int which);              /* interrupt callbacks            */
	void (*rdyr[2])(int data);            /* RDY active callback            */
	int mode[2];                          /* mode 00=in,01=out,02=i/o,03=bit*/
	int enable[2];                        /* interrupt enable               */
	int mask[2];                          /* mask followers                 */
	int dir[2];                           /* direction (bit mode)           */
	int rdy[2];                           /* ready pin level                */
	int in[2];                            /* input port data                */
	int out[2];                           /* output port                    */
	int strobe[2];                        /* strobe inputs                  */
	int int_state[2];                     /* interrupt status (daisy chain) */
public:
  void __fastcall init ();
  void __fastcall reset ();
  void __fastcall d_w(int ch , int data );
  void __fastcall c_w(int ch , int data );
  int  __fastcall c_r(int ch );
  int  __fastcall d_r(int ch );

/* set/clear /astb input */
  void	__fastcall astb_w(int state);
/* set/clear /bstb input */
  void	__fastcall bstb_w(int state);

  void __fastcall p_w(int ch , int data );
  int  __fastcall p_r(int ch );

/* Z80 DaisyChain controll */
  int  __fastcall interrupt();
  void __fastcall reti();

/* mame interface */
  void __fastcall check_irq( int ch );
  void __fastcall interrupt_check();
  void __fastcall set_rdy( int ch, int state);
  void __fastcall w(int offset, int data);
  int  __fastcall r(int offset );
  void __fastcall update_strobe(int ch, int state);
  void __fastcall astb_w(int which, int state);
  void __fastcall bstb_w(int which, int state);
};

#endif
