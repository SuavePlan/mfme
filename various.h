//
//
//	various.h
//
//	(C) C.J.Wren 2000
//

#ifndef __various__
#define __various__

class various {
// Internal registers

public:
	int		     	delay, ne555;
    Byte			zero;
    Byte            timer;
    bool            zero_changed;
    bool            timer_changed;
    int             Period[3];
    int             Step[3];
    int             Step_hi[3];
    int             Step_lo[3];
    int             Output[3];
    int             Running[3];
    bool            enabled[3];
    bool            updated[3];
    bool            dual_mode[3];
    bool            prescaler[3];
    bool            single_shot[3];
    int             SampleRate;
    int             cycle;
// Initialisation functions

protected:
    int r1;
    int r2;
    int r3;
    int c;
    int memory;

// Read and write functions
public:
	void			 reset(int Rate);
	void			 update(void);
    void             tick(unsigned int cycles, bool toggle = false);
    void             SoundUpdate(short *ptr, int len, unsigned int cycles);
    void             set_RC_filter(int R1,int R2,int R3,int C);
    void             apply_RC_filter(short *buf,int len,int sample_rate);
    void             SetPeriod(int channel, short latch, Byte ctrl, bool latch_changed, bool o_changed, int cycles);

// Public constructor and destructor

				 various();
				~various();

};

#endif // __various_h__
