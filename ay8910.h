//
//
//	ay8910.h
//

#ifndef __ay8910__
#define __ay8910__

/* register id's */
#define AY_AFINE	(0)
#define AY_ACOARSE	(1)
#define AY_BFINE	(2)
#define AY_BCOARSE	(3)
#define AY_CFINE	(4)
#define AY_CCOARSE	(5)
#define AY_NOISEPER	(6)
#define AY_ENABLE	(7)
#define AY_AVOL		(8)
#define AY_BVOL		(9)
#define AY_CVOL		(10)
#define AY_EFINE	(11)
#define AY_ECOARSE	(12)
#define AY_ESHAPE	(13)

#define AY_PORTA	(14)
#define AY_PORTB	(15)
#define STEP 0x8000
#define MAX_OUTPUT 0x4fff

class ay8910 {

// Internal registers

protected:
    Byte address;
//  byte bc1;
//  byte bdir;

    int SampleRate;
	int register_latch;
	unsigned char Regs[16];
	unsigned int UpdateStep;
	int PeriodA,PeriodB,PeriodC,PeriodN,PeriodE;
	int CountA,CountB,CountC,CountN,CountE;
	unsigned int VolA,VolB,VolC,VolE;
	unsigned char EnvelopeA,EnvelopeB,EnvelopeC;
	unsigned char OutputA,OutputB,OutputC,OutputN;
	signed char CountEnv;
	unsigned char Hold,Alternate,Attack,Holding;
	int RNG;
	unsigned int VolTable[32];
    int r1[1];
    int r2[1];
    int r3[1];
    int c[1];
    int memory[1];
// Initialisation functions

// Read and write functions
public:
    Byte    muted;
    int     porta, portb, porta_in, portb_in;
    Byte    porta_changed, portb_changed;
    void    write_data(Byte d, Byte cs, Byte bdir, Byte bc1);
    void    _AYWriteReg(int r, int v);
    void    AYWriteReg(int r, int v);
    unsigned char AYReadReg(int r);
    void    AY8910Write(int a,int data);
    int     AY8910Read(void);
    void    Update(short *buff_ptr1, int length, unsigned int clks);
    void    AY8910_set_clock(int clock);
    void    AY8910_set_volume(int channel,int volume);
    void    build_mixer_table(void);
    void    AY8910_reset(void);
    void    mute(Byte muted);
    void    set_RC_filter(int channel,int r1,int r2,int r3,int C);
    void    apply_RC_filter(int channel,short *buf,int len,int sample_rate);
//    FILE    *fp;
	void	reset(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 ay8910();
				~ay8910();

};

#endif // __ay8910_h__

