//
//
//	bwbchr.h
//
//	(C) C.J.Wren 2000
//

#ifndef __bwbchr__
#define __bwbchr__

class bwbchr {
// Internal registers

public:
	Byte		    previous;
    int             count;
    bool            identifying;
public:
    void __fastcall  write(Byte value);
    Byte __fastcall  read(void);

// Public constructor and destructor

				 bwbchr();
				~bwbchr();

};

#endif // __bwbchr_h__
