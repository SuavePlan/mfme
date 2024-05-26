//---------------------------------------------------------------------------
#ifndef ace1H
#define ace1H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
//---------------------------------------------------------------------------

#include "system.h"
#include "z80.h"
#include "led.h"
#include "JPMreels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "ay8910.h"
//#include "rs232.h"
#include "ppi8255.h"

class TACE1;

class ACE1 : public z80cpu {

public:
  byte Z80_In (word Port);              // Input byte from Port
  void Z80_Out (word Port,byte Value);  // Output byte to Port
  unsigned Z80_RDMEM(word A);          // Read byte from given Mem Location
  void Z80_WRMEM(word A,byte V);       // Write byte to Memory
  unsigned Z80_RDSTACK(word A);          // Read byte from given Mem Location
  void Z80_WRSTACK(word A,byte V);       // Write byte to Memory
  void Z80_Patch (Z80_Regs *Regs);   /* Called when ED FE occurs. Can be used */
                                     /* to emulate disk access etc.           */
  void Z80_Reti (void);              /* Called when RETI occurs               */
  void Z80_Retn (void);              /* Called when RETN occurs               */
  int Z80_Interrupt(void);
  bool Z80_InstHook(int PC);      /* Called prior each instruction */

  int       execute(void);
  int       dasm(int pc, char *buffer);
  led       seg7;
  jpmreels	reel;
  meters    meter;
  lamps		lamp;
  triacs	triac;
  ay8910    gi;
  PPI8255   ppi24;
  PPI8255   ppi25;
  PPI8255   ppi37;  // inputs (p1)
  int       refresh;
  int       strobe;
  bool      nmi;
  bool      irq;
  bool      innmi;
  bool      inirq;
  int       nmicnt;
  int       irqcnt;
  unsigned long cycles;
  Byte      memory[0x10000];
  TACE1     *parent;
};

class TACE1 : public TSystem
{
public:
  ACE1 board;
protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TACE1(String AGame, String AGamePath, TStringList *AROM_list);
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
