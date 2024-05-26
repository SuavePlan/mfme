//
//
//	triac.h
//
//	(C) C.J.Wren 2000
//

#ifndef __triacs__
#define __triacs__

class triacs {

// Internal registers

protected:

// Initialisation functions

protected:
	void			 reset(void);

// Read and write functions
public:
    Word			value;
    Word            changed;
	void			write(Word x);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 triacs();
				~triacs();

};

#endif // __triacs_h__
