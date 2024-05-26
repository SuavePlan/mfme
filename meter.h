//
//
//	meter.h
//
//	(C) C.J.Wren 2000
//

#ifndef __meters__
#define __meters__

class meters {

// Internal registers

public:
	Byte			meter_on;
    Byte            state;
    int             delay;
    bool            changed;
// Initialisation functions

protected:

// Read and write functions
public:
	void			 reset(void);
	bool			 update(void);
	bool			 write(Byte x);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 meters();

};

#endif // __meters_h__
