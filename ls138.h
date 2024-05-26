//
//
//	ls138.h
//
//	(C) C.J.Wren 2000
//

#ifndef __ls138__
#define __ls138__

class ls138 {

// Internal registers

protected:
public:
	Byte	column;
    bool    enabled;
    int     timer;
// Initialisation functions

protected:

// Read and write functions
public:
	void			 reset(void);
	void			 write_A0(Byte x);
	void			 write_A1(Byte x);
	void			 write_A2(Byte x);
    void             update(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 ls138();
				~ls138();

};

#endif // __ls138_h__
