//
//
//	scn2674.h
//
//	(C) C.J.Wren 2004
//

#ifndef __scn2674_h__
#define __scn2674_h__

#define SCN_SPLIT2   0x01
#define SCN_READY    0x02
#define SCN_SPLIT1   0x04
#define SCN_LINEZERO 0x08
#define SCN_VBLANK   0x10
#define SCN_RDFLG    0x20

class scn2674 {

// Internal registers

private:
    bool cursor;
    bool next_field;
    Byte intflag;
    bool delayed;
    int  timer;
    int  timer1;
    int  timer2;
    int  rowtimer;
    int  interval;
    int  interval1;
    int  interval2;
    int  drawtimer;
protected:

// Read and write functions
public:
    Byte pos;
    bool graphics;
    bool display;
    Byte ir[15];
    Byte regs[8];
    Byte intmask;
    Byte maskchanged;
    Byte rowcounter;
    bool vblank;
    bool draw;
    bool row;
    void reset();
    Byte read(int addr);
    void write(int addr, Byte data);
    void tick(int cycles);

// Public constructor and destructor

    scn2674();

};

#endif // __scn2674_h__
