//
//
//	jpmhopper.h
//
//	C.J.Wren 2002
//

#ifndef __jpmhopper__
#define __jpmhopper__

class jpmhopper{

// Internal registers

protected:
    Word timer;
    Byte coin;
    Byte motor;
    Byte hz;

public:
    bool opto_test;

// Initialisation functions

protected:

// Read and write functions
public:
	void			 reset(void);
    Byte             opto(void);
    void             enable(Byte enable, Byte opto, Byte Hz50 );
    void             update(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);

// Public constructor

    jpmhopper();

};

#endif // __jpmhopper_h__
