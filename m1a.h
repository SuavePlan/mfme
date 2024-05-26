//---------------------------------------------------------------------------
#ifndef m1AH
#define m1AH

#include "mc6809.h"
#include "mc6821.h"
#include "alpha.h"
#include "led.h"
#include "JPMreels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "ay8910.h"
#include "rs232.h"
#include "scn68681b.h"
#include "jpmhopper.h"
#include "ym2413.h"
#include "I8279.h"
#include "m1a6376.h"

class TM1A;

class M1A : public mc6809 {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
    int			     execute(void);
    mc6821			 pia;
    alpha			 display;
    led              seg7;
    jpmreels		 reel;
    meters			 meter;
    lamps			 lamp;
    triacs			 triac;
    ay8910           gi;
    scn68681b        duart;
    jpmhopper        hoppr;
    ym2413           yamaha;
    msm6376a         sample;
    bool             busy;
    bool             oldbusy;
    Byte             tune;
    I8279            io1;
    I8279            io2;
    Byte             page;
    int              refresh;
    int              alarmen;
    int              enable;
    int              wdog;
    int              psurelay;
    int              srsel;
    int              ramen;
    bool             firq;
    bool             nmi;
    bool             Vmm;
    int              sndcntl;
    int              vmmctr;
    Byte             memory[0x80000];
    TM1A             *parent;
};

class TM1A : public TSystem
{
public:
  M1A board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TM1A(String AGame, String AGamePath, TStringList *AROM_list);
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
    bool __fastcall AutoLamp(int lamp, int &reel, int &symbol);
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void  __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void  __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
//---------------------------------------------------------------------------
#endif
