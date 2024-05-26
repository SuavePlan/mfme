//
//
//	scn68681.hpp
//
//	C.J.Wren 2000
//

#ifndef __scn68681__
#define __scn68681__

class scn68681 {

// Internal registers

protected:
    Byte conf;
    Word timer;
    Word timer2;
    Byte coin;
    Byte coin2;
    Byte motor;
    Byte opto;
    Byte motor2;
    Byte opto2;

public:
    Byte ip_port;
    Byte op_port;

// Initialisation functions

protected:

// Read and write functions
public:
	void			 reset(void);
	void			 write(Byte addr, Byte x);
	void			 opto_drive(Byte opto, Byte motor, Byte opto2, Byte motor2);
    Byte             read(Byte addr);
    void             hoppermotor(Byte addr);
    void             update(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 scn68681 ();
				~scn68681 ();

};

#endif // __scn68681_hpp__
