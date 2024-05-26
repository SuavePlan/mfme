//---------------------------------------------------------------------------
#ifndef mps2H
#define mps2H

#include "TMS/tms99xx.h"
#include "JPMreels.h"
#include "meter.h"
#include "lamp.h"
#include "led.h"
#include "triac.h"
#include "sn76496.h"
#include "tms9902.h"
#include "jpmhopper.h"
#include "ppi8255.h"

class TMPS2;

class MPS2 : public tms99xx {

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
    sn76496          sn;
    TMS9902          duart1;
    TMS9902          duart2;
    PPI8255          ppi21;
    PPI8255          ppi22;
    PPI8255          ppi25;
    PPI8255          ppi26;
    jpmhopper        hopper;
    int              refresh;
    int              last_cycles;
    Byte             duart1_int;
    Byte             duart2_int;
    Byte             int4_int;
    Byte             watchdog_strobe;
    Byte             ram_enable;
    Byte             alarm;
    Byte             nmi_en;
    Byte             reel_drive_enable;
    Byte             io_enable;
    Byte             bb_ptr;
    Byte             diag_led;
    Byte             power_fail;
    Byte             watchdog_timeout;
    Byte             invalid_access;
    Byte             clear_down;
    Byte             nmi_clear;
    Byte             memory[0x10000];
    Byte             last_mux;
    TMPS2            *parent;
};

class TMPS2 : public TSystem
{
public:
  MPS2 board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();

  public:
	__fastcall TMPS2(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall	load_intelhex(void);
    void __fastcall saveram(void);
    void __fastcall hard_reset(void);
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
