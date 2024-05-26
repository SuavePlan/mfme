//
//
//	mc6821.h
//
//	(C) C.J.Wren 2000
//

#ifndef __mc6821_h__
#define __mc6821_h__

class mc6821 {

// Internal registers

private:
    bool             ca1_transition, cb1_transition, ca2_transition, cb2_transition;
    
public:
	Byte			 cra, crb;
	Byte			 ora, orb;
	Byte			 ora_in, orb_in;
	Byte			 ora_changed, orb_changed;
	Byte			 Dda, Ddb;
	Byte			 ca1, ca2, cb1, cb2, ca2_in, cb2_in;
	bool			 ca2_changed, cb2_changed;
    bool             ca2_updated, cb2_updated;
    bool             ora_updated, orb_updated;
    bool             ca2_strobed, cb2_strobed;

// Initialisation functions

protected:


// Read and write functions
public:

	Byte			 read(Word offset);
	void			 write(Word offset, Byte val);
    void             set_ca1(Byte val);
    void             set_ca2(Byte val);
    void             set_cb1(Byte val);
    void             set_cb2(Byte val);
	void			 reset(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 mc6821();
				~mc6821();

};

#endif // __mc6821_h__
