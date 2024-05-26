//---------------------------------------------------------------------------
#ifndef sys80H
#define sys80H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
//---------------------------------------------------------------------------

//#include "system.h"
#include "TMS/tms99xx.h"
#include "JPMreels.h"
#include "meter.h"
#include "lamp.h"
#include "led.h"
#include "triac.h"
#include "ay8910.h"
#include "tms9902.h"
#include "jpmhopper.h"
#include "ppi8255.h"

class TSYS80;

class SYS80 : public tms99xx {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
    Byte             readport(Word);
    void             writeport(Word,Byte);
    int			     execute(void);
    jpmreels		 reel;
    meters			 meter;
    lamps			 lamp;
    led              seg7;
    triacs			 triac;
    ay8910           gi;
    TMS9902          duart1;
    Word             reelport;
    Word             old_reel;
    Byte             soundport;
    Byte             ledport;
    Byte             extport;
    Byte             bcd;
    int              refresh;
    int cyccount, cyccount2;
    Byte             lev1_int;
    Byte             lev2_int;
    Byte             int_pending;
    TSYS80 *         parent;
    Byte             memory[0x10000];
};

class TSYS80 : public TSystem
{
public:
  SYS80 board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();

  public:
	__fastcall TSYS80(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall load_intelhex();
    void __fastcall UpdateReel(void);
    void __fastcall UpdateLamp(void);
    void __fastcall UpdateMeter(void);
    void __fastcall UpdateTriac(void);
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateLedDisplay(void);
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
//---------------------------------------------------------------------------
#endif
