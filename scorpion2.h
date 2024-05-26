//---------------------------------------------------------------------------
#ifndef scorpion2H
#define scorpion2H

#include "mc6809.h"
#include "alpha.h"
#include "BFMalpha.h"
#include "led.h"
#include "jpmreels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "hopper.h"
#include "ay8910.h"
#include "ym2413.h"

class TSCORPION2;

class SCORPION2 : public mc6809 {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
    int			     execute(void);
    void __fastcall Write7Seg(void);
    BFMalpha	     display;
    BFMalpha	     leddisplay;
    led              seg7;
    jpmreels		 reel;
    meters			 meter;
    lamps			 lamp;
    triacs			 triac;
    ay8910           gi;
    hopper           hoppr1;
    hopper           hoppr2;
    ym2413           yamaha;
    Byte             page;
    bool             busy;
    int              tune;
    int              sound_page;
    int              refresh;
    int              fiftyHz;
    int              wdog;
    bool             irq;
    bool             firq;
    Byte             spcs;
    Byte             coin;
    Byte             extledcount;
    Byte             firstled;
    Byte             memory[0x10000];
    Byte             ram[0x2000];
    TSCORPION2 *     parent;
};

class TSCORPION2 : public TSystem
{
public:
  SCORPION2 board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TSCORPION2(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall load_intelhex();
    void __fastcall Scramble();
    int __fastcall TranslateAddr(int addr);
    int __fastcall TranslateData(int addr);
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
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void  __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void  __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
extern TSCORPION2 *scorpion2;
//---------------------------------------------------------------------------
#endif
