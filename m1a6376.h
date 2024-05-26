//
//
//	mc6376.h
//
//	(C) C.J.Wren 2000
//

#ifndef __msm6376a_h__
#define __msm6376a_h__

#include <stdio.h>

class msm6376a {

// Internal registers

private:
    int              playing[2];
    bool             st_changed;
    bool             ch2_changed;
    Byte             st, ch2, clk, dir;
    Byte             volume;
    Byte             latch;
    Byte             latch1;
    Byte             latch2;
    Byte             lastval;
    Byte             atten;
    Byte             pulses;
public:
	Byte			 nar, busy;
    Word             pause;
    Byte             channel;
    Byte             tune;
    Byte             next1;
    Byte             next2;
    bool             again;
// Initialisation functions

protected:

// Read and write functions
public:
	void			 write_latch(Byte val);
	void			 write_control(Byte val);
	void			 reset(void);
    bool             update(void);
    void             finished(Byte Channel);
    void             replay(int chan);

// Public constructor and destructor

				 msm6376a();
				~msm6376a();

};

#endif // __msm6376a_h__
