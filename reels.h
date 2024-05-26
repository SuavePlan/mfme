//
//
//	reels.h
//
//	(C) C.J.Wren 2000
//

#ifndef __reels__
#define __reels__

class reels {

// Internal registers

public:
    Byte            step_size[10];
    int             offset[10];
    Byte            flag[10];
    Byte            old[10];

public:
	Byte			optos;
	int 			pos[10];
    bool            changed[10];
    Byte            reelpos[10];
    Byte            mux;

// Initialisation functions

protected:

// Read and write functions
public:
	void			 state(Byte x, Byte nbr);
	void			 write_x(Byte x);
	void			 write_y(Byte x);
	void			 write_z(Byte x);
	void			 write_5a(Byte x);
	void			 write_5b(Byte x);
	void			 write_5c(Byte x);
	void			 write_6(Byte x);
	void			 write_6a(Byte x);
	void			 write_7(Byte x);
	void			 write(Byte x);  // MPU 3
	void			 reset(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				 reels();
				~reels();

};

#endif // __reels_h__
