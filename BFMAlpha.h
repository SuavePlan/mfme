//
//
//	BFMalpha.h
//

#ifndef __bfmalpha__
#define __bfmalpha__

class BFMalpha {

// Internal registers

protected:

	Byte			 clk;
	Byte			 ch_pos;
    Byte             last_pos;
	Byte			 ch;
	Byte			 clks;
    Byte             nbr_digits;
//    bool             intensity_next;
    bool             dp_next;
    Byte             clearmode;
    Byte             skip;
    int              custom;
//    Byte             user_defined;
    Byte             scroll_pos;
// Initialisation functions

protected:

// Read and write functions
public:
	int 	         chs[16];
	int 	         flashchs[16];
//    Byte             user_def[7];
//    Byte             user_def_pos;
    Byte             window_start;
    Byte             window_end;
    Byte             window_size;
    Byte             mode;
    bool             enabled;
//    char             intensity;
    Byte             display_changed;
    Byte             flash_changed;
//    Byte             intensity_changed;
//    bool             char_changed;
    int              flashrate;
    int              flashcontrol;
    int              flashtimer;
    bool             flash;
    bool             updated;
//    FILE             *stream;
	void			 do_char(int x);
    void             DoCommand(int x);
	void			 enable(Byte e);
	void			 reset(void);
	void			 update();
    void             write_all(int Reset, int Clock, int Data);
    void             write_clock(int c, int data);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);
// Public constructor and destructor

    BFMalpha();
    ~BFMalpha();
};

#endif // __bfmalpha_h__
