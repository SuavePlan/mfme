//
//
//	jpmreels.h
//
//	(C) C.J.Wren 2000
//

#ifndef __jpmreels__
#define __jpmreels__

class jpmreels {
public:
// Internal registers
struct rstruct {
  int offset;
  int flag;
  int steps;
  int stops;
  int stepsperstop;
  int pos;
  int reelpos;
  int startopto;
  int endopto;
  int steps_remaining;
  int direction;
  int adjust;
  bool inverted;
  bool present;
  Byte pat;
};

struct mps2reel {
  int rxcount;
  Byte msg[20];
  Byte *rxp;
  Byte unit;
  Byte bank;
  int speed;
  int scaler;

};

protected:
    Word            x1, x2, x3;
//    int             scaler,scaler2;

public:
	Byte			optos;
//    int             speed,speed2;
    bool            changed[8];
    rstruct         reels[8];
    Byte            lamps[8];
    Byte            lamp_changed[8];
    mps2reel        proc[2];
    char            reply[50];

protected:
	void			state(Byte x, Byte nbr);
    void            writelamp(Byte value, Byte nbr);

// Read and write functions
public:
    void            write(Byte x, Byte nbr);
    void            write1(Word x);
    void            write2(Word x);
    void            write3(int x);
    void            writex(Byte x);
    void            writey(Byte x);
    void            writez(Byte x);
    void            writexr(Byte x);
    void            writeyr(Byte x);
    void            writezr(Byte x);
    void            writeflip03(Word x);
    void            writesys5(Byte x, Byte nbr);
    void            writeproconn(Byte x);
    void            writereellamp(Byte value, Byte nbr);
	void		    reset(void);
	void		    clear(void);
    bool            tick(int cycles);
    void            spin(int bank);
    void            write_sys1(Byte x);
    void            spinace(void);
    void            setace(Byte reel, int steps, int direction);
    bool            mps2Rxchar( Byte ch );
    void            RxMessage(mps2reel *p );
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);

// Public constructor and destructor

				    jpmreels();
};

#endif // __reels_h__
