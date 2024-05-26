//
//
//	mc6840.h
//
//	(C) C.J.Wren 2000
//

#ifndef __mc6840_h__
#define __mc6840_h__

class mc6840 {

// Internal registers

protected:
    Byte         MSB;
    Byte         LSB;
	int			 clk;
    Byte         status_read;
    Byte         prescaler;

// Initialisation functions

protected:


// Read and write functions
public:
	unsigned short	 lat1, lat2, lat3;
	Byte		     cr1, cr2, cr3;
    bool             t1_changed, t2_changed, t3_changed;
    bool             lat1_changed, lat2_changed, lat3_changed;
    bool             t1_timedout, t2_timedout, t3_timedout;
    Byte             g1, g2, g3, c1, c2, c3, o1, o2, o3;
    Byte             sr;
    unsigned short   cnt1, cnt2, cnt3;
	Byte			 read(Word offset);
	void			 write(Word offset, Byte val);
	void			 tick(int);
	void			 reset(void);
    void             clock1(void);
    void             clock2(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 mc6840();
				~mc6840();

};

#endif // __mc6821_h__
