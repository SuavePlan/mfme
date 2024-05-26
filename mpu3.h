//---------------------------------------------------------------------------
#ifndef mpu3H
#define mpu3H

#include "mc6800.h"
#include "mc6821.h"
#include "mc6840.h"
#include "alpha.h"
#include "led.h"
#include "reels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "ls138.h"
#include "rs232.h"
#include "various.h"

class TMPU3;

class MPU3 : public mc6800 {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
	int			     execute(void);
    mc6821			 pia1;
    mc6821			 pia2;
    mc6821			 pia3;
    mc6821			 pia4;
    mc6840			 ptm;
    alpha			 display;
    led              seg7;
    reels			 reel;
    meters			 meter;
    lamps			 lamp;
    triacs			 triac;
    ls138			 decoder;
    various          misc;
    int              refresh;
    int              current_sense;
    int              patch;
    int              charpos;
    unsigned int     cycles;
    Byte             memory[0x10000];
    TMPU3            *parent;
    int              sound_updated;

};

class TMPU3 : public TSystem
{
public:
    MPU3 board;

protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TMPU3(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall	load_intelhex();
    void __fastcall UpdateDisplay(void);
    void __fastcall UpdateReel(void);
    void __fastcall UpdateLamp(void);
    void __fastcall UpdateMeter(void);
    void __fastcall UpdateTriac(void);
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateLedDisplay(void);
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void  __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void  __fastcall ClearRAM(void);
};
//---------------------------------------------------------------------------
#endif
