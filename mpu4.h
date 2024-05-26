//---------------------------------------------------------------------------
#ifndef mpu4H
#define mpu4H

#include "mc6809.h"
#include "mc6821.h"
#include "mc6840.h"
#include "mc6850.h"
#include "alpha.h"
#include "led.h"
#include "reels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "ls138.h"
#include "rs232.h"
#include "m6376.h"
#include "scn68681.h"
#include "hopper.h"
#include "various.h"
#include "ay8910.h"
#include "ym2413.h"
#include "bwbchr.h"

class TMPU4;

class MPU4 : public mc6809 {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
    int			     execute(void);
    void             writemeterport(void);
    mc6821			 pia1;
    mc6821			 pia2;
    mc6821			 pia3;
    mc6821			 pia4;
    mc6821			 pia5;
    mc6821			 pia6;
    mc6821           pia7;
    mc6840			 ptm;
    mc6840           ptm2;
    alpha			 display;
    led              seg7;
    reels			 reel;
    meters			 meter;
    lamps			 lamp;
    triacs			 triac;
    ls138			 decoder;
    ay8910           gi;
    rs232            serial;
    msm6376          sample;
    scn68681         duart;
    hopper           hoppr1;
    hopper           hoppr2;
    ym2413           yamaha;
    mc6850           acia;
    bwbchr           bwb_chr;
    Byte             page;
    int              refresh;
    int              fiftyHz;
    int              current_sense;
    int              patch;
    int              charpos;
    Byte             bwbcharpos;
    Byte             meterport;
    int              metertimer;
    Byte             trackX;
    Byte             trackXcnt;
    Byte             trackY;
    Byte             trackYcnt;
    bool             trackXdir;
    bool             trackYdir;
    Byte             speedup1, speedup2;
    Byte             trackdelay;
    int              LastX, LastY;
    Byte             memory[0x80000];
    bool             acia_int;
    TMPU4            *parent;
};

class TMPU4 : public TSystem
{
public:
  MPU4 board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TMPU4(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall load_intelhex();
    void __fastcall UpdateDisplay(void);
    void __fastcall UpdateReel(void);
    void __fastcall UpdateLamp(void);
    void __fastcall UpdateMeter(void);
    void __fastcall UpdateTriac(void);
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateLedDisplay(void);
    void __fastcall UpdateWave(void);
    void __fastcall UpdateIOMeters(void);
    void __fastcall UpdateIOTriacs(void);
    void __fastcall UpdateIOSinks(void);
    void __fastcall UpdateIOSelects(void);
    void __fastcall UpdateIOReels(void);
    bool __fastcall AutoLamp(int i, int &reel, int &symbol);
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void  __fastcall setmemory(int address, Byte value);
    int  __fastcall getstack(void);
    int  __fastcall getstacktop(void);
    int  __fastcall dasm(int pc, char *buffer);
    void __fastcall samplefinished(int Channel);
    void  __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
//---------------------------------------------------------------------------
#endif
