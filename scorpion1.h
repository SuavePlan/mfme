//---------------------------------------------------------------------------
#ifndef scorpion1H
#define scorpion1H

#include "mc6809.h"
#include "mc6850.h"
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

class TSCORPION1;

class SCORPION1 : public mc6809 {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
    int			     execute(void);
    BFMalpha	     display;
    jpmreels		 reel;
    meters			 meter;
    lamps			 lamp;
    triacs			 triac;
    ay8910           gi;
    mc6850           acia;
    hopper           hoppr1;
    hopper           hoppr2;
//    ym2413           yamaha;
    Byte             page;
    bool             busy;
    int              tune;
    int              wdog;
    int              refresh;
    int              fiftyHz;
    int              mux1, mux2;
    int              locked;
    bool             irq;
    bool             firq;
    Byte             coin;
    Byte             memory[0x10000];
    Byte             ram[0x2000];
    TSCORPION1 *     parent;
};

class TSCORPION1 : public TSystem
{
public:
  SCORPION1 board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TSCORPION1(String AGame, String AGamePath, TStringList *AROM_list);
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
    int  __fastcall dasm(int pc, char *buffer);
    void  __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
//---------------------------------------------------------------------------
#endif
