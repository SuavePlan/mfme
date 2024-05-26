//
//
//	scn2674.h
//
//	(C) C.J.Wren 2004
//

#ifndef __ef9369_h__
#define __ef9369_h__

class ef9369 {

// Internal registers

private:
    Byte pos;
protected:

// Read and write functions
public:
    Byte regs[32];
    void reset();
    Byte read(int addr);
    void write(int addr, Byte data);

// Public constructor and destructor

    ef9369();

};

#endif // __ef9369_h__
