//---------------------------------------------------------------------------
#ifndef SYS5H
#define SYS5H

#include "m68kcpu.h"
#include "alpha.h"
#include "led.h"
#include "jpmreels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "mc6821.h"
#include "mc6840.h"
#include "mc6850.h"
#include "jpmhopper.h"
#include "saa1099.h"
#include "ym2413.h"

class TSYS5;

class SYS5 : public mc68000 {

public:
  int  m68k_read_immediate_8(uint address, int cs);
  int  m68k_read_immediate_16(uint address, int cs);
  int  m68k_read_immediate_32(uint address, int cs);

/* Read an instruction (16-bit word immeditately after PC) */
  int  m68k_read_instruction(uint address, int cs);

/* Write to anywhere */
  void m68k_write_memory_8(uint address, int value, int cs);
  void m68k_write_memory_16(uint address, int value, int cs);
  void m68k_write_memory_32(uint address, int value, int cs);
  int m68k_read_memory_8(uint address, int cs);
  int m68k_read_memory_16(uint address, int cs);
  int m68k_read_memory_32(uint address, int cs);
  int			 execute(void);
  alpha			 display;
  led            seg7;
  jpmreels		 reel;
  meters		 meter;
  lamps			 lamp;
  triacs		 triac;
  mc6821         pia;
  mc6840		 ptm;
  saa1099        saa;
  ym2413         yamaha;
  mc6850         acia;
  int            refresh;
  Byte           status;
  Byte           payslide;
  bool           busy;
  Byte           tune;
  Byte           page;
  int            nbr_reels;
  int            last_cycles;
  Byte           reel12;
  Byte           reel34;
  Byte           reel56;
  Byte           reel78;
  unsigned char  mux_array[256];
  unsigned char  serial[32];
  unsigned char  rom[0x100000];
  unsigned char  ram[0x10000];
  TSYS5          *parent;
};

class TSYS5 : public TSystem
{
public:
  SYS5 board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TSYS5(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall load_intelhex();
    void __fastcall UpdateStatus(void);
    void __fastcall UpdateDisplay(void);
    void __fastcall UpdateReel(void);
    void __fastcall UpdateReelLamp(void);
    void __fastcall UpdateLamp(void);
    void __fastcall UpdateMeter(void);
    void __fastcall UpdateTriac(void);
    void __fastcall do_sound(void);
    void __fastcall UpdateSound(void);
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateLedDisplay(void);
    void __fastcall UpdateWave(void);
    void __fastcall UpdateIOReels(void);
    void __fastcall UpdateIOMeters(void);
    void __fastcall UpdateIOTriacs(void);
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
//---------------------------------------------------------------------------
#endif
