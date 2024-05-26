//
//
//	lamp.h
//
//	(C) C.J.Wren 2000
//

#ifndef __lamps__
#define __lamps__

class lamps {

// Internal registers

protected:

// Initialisation functions

protected:
	void			 reset(void);

// Read and write functions
public:
    Byte             lamp[320];
    Byte             updated[320];
    int			     colmn;
    Byte			 data;
    int              on;
    bool             sense;
    bool             toggle;
    Byte             last_b7;
    Byte             last_b6;
    bool             On[2];
    bool             update(int size = 320);
    bool             write(Byte data, Byte column, Byte bank, Byte persistance = 18);
    void             write_extender1(Byte data);
    void             write_extender2(Byte data, Byte column, bool enabled);
    void             write_extender3(Byte data, Byte column, bool enabled);
    void             write_extender4(Byte data, Byte column, bool enabled);
    bool             writejpm(short x, Byte position, Byte persistance);
    int              writebuf(Byte *ptr, int base, int mux);
    void             write_maygay(Byte x, Byte column, Byte bank);
    bool             writebit(Word x, Byte persistance);
    bool             write_static(Byte x, Byte number);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 lamps();

};

#endif // __lamps_h__
