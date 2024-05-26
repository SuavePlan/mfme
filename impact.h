//---------------------------------------------------------------------------
#ifndef impactH
#define impactH

#include "m68kcpu.h"
#include "alpha.h"
#include "led.h"
#include "jpmreels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "scn68681b.h"
#include "ppi8255.h"
#include "jpmhopper.h"
#include "sec.h"

class TIMPACT;

class IMPACT : public mc68000 {

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
  scn68681b      duart;
  jpmhopper      hopper1;
  jpmhopper      hopper2;
  PPI8255        ppi8255;
  SEC            sec;
  int            source;
  int            refresh;
  int            last_cycles;
  Byte           status;
  Byte           payen;
  Byte           payslide;
//  int            current_sense;
  bool           busy;
  Byte           tune;
  Byte           page;
  Word           reel1;
  Word           reel2;
  unsigned char  rom[0x100000];
  unsigned char  ram[0x10000];
  TIMPACT        *parent;
};

class TIMPACT : public TSystem
{
public:
  IMPACT board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
//	int            execute(void);
//    void        step(void);
//    void        halt(void);
public:
	__fastcall TIMPACT(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall load_intelhex();
    void __fastcall UpdateStatus(void);
    void __fastcall UpdateDisplay(void);
    void __fastcall UpdateReel(void);
    void __fastcall UpdateLamp(void);
    void __fastcall UpdateMeter(void);
    void __fastcall UpdateSEC(void);
    void __fastcall do_sound(void);
    void __fastcall UpdateSound(void);
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateLedDisplay(void);
    void __fastcall UpdateWave(void);
    void __fastcall UpdateIOReels(void);
    void __fastcall UpdateIOMeters(void);
    void __fastcall UpdateIOTriacs(void);
    bool __fastcall AutoLamp(int i, int &reel, int &symbol);
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
