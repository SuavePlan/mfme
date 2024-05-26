//
//
//	alpha.h
//

#ifndef __alpha__
#define __alpha__

class alpha {

// Internal registers

protected:

	Byte			 clk;
	Byte			 ch_pos;
	Byte			 ch;
	Byte			 clks;
    Byte             nbr_digits;

// Initialisation functions

protected:

// Read and write functions
public:
	int	             chs[16];
    bool             enabled;
    char             intensity;
    Byte             display_changed;
    Byte             intensity_changed;
//    FILE             *stream;
    void             write(Byte data);
    void             writebuf(Byte *ptr, Byte length);
    void             writejpm(Byte data);
	void			 write_clock(Byte c,Byte data);
    void             write_all(Byte Reset, Byte Clock, Byte Data);
	void			 do_char(Byte ch);
	void			 enable(Byte e);
	void			 reset(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);
// Public constructor and destructor

    alpha();
    ~alpha();
};

#endif // __alpha_h__
