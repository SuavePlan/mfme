#ifndef SN76496_H
#define SN76496_H

#define SN_MAX_OUTPUT 0x7fff

#define SN_STEP 0x10000


/* Formulas for noise generator */
/* bit0 = output */

/* noise feedback for white noise mode (verified on real SN76489 by John Kortink) */
#define SN_FB_WNOISE 0x14002	/* (16bits) bit16 = bit0(out) ^ bit2 ^ bit15 */

/* noise feedback for periodic noise mode */
#define SN_FB_PNOISE 0x10000 /* 16bit rorate */
//#define SN_FB_PNOISE 0x08000   /* JH 981127 - fixes Do Run Run */

/*
0x08000 is definitely wrong. The Master System conversion of Marble Madness
uses periodic noise as a baseline. With a 15-bit rotate, the bassline is
out of tune.
The 16-bit rotate has been confirmed against a real PAL Sega Master System 2.
Hope that helps the System E stuff, more news on the PSG as and when!
*/

/* noise generator start preset (for periodic noise) */
#define SN_NG_PRESET 0x0f35

class sn76496 {

// Internal registers

protected:
  int baseclock;
  int volume;
	int Channel;
	int SampleRate;
	unsigned int UpdateStep;
	int VolTable[16];	/* volume table         */
	int Register[8];	/* registers */
	int LastRegister;	/* last register written */
	int Volume[4];		/* volume of voice 0-2 and noise */
	unsigned int RNG;		/* noise generator      */
	int NoiseFB;		/* noise feedback mask */
	int Period[4];
	int Count[4];
	int Output[4];

// Initialisation functions
protected:
	void			 por(void);

// Read and write functions
public:
    void SN76496Write(int data);
    void SN76496Update(short *buffer,int length, unsigned int cycles);
    int SN76496_init(int clock,int volume,int sample_rate);
    void SN76496_set_clock(int clock);
    void SN76496_set_gain(int gain);
//    FILE    *fp;
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);

// Public constructor and destructor

    sn76496();
		~sn76496();
};

#endif
