//---------------------------------------------------------------------------
#ifndef ProconnH
#define ProconnH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
//---------------------------------------------------------------------------

#include "system.h"
#include "z80.h"
#include "led.h"
#include "jpmreels.h"
#include "meter.h"
#include "lamp.h"
#include "triac.h"
#include "ay8910.h"
#include "alpha.h"
#include "z80support.h"

class TProconn;

class Proconn : public z80cpu {

public:
  Byte      Z80_In(word Port);              // Input byte from Port
  void      Z80_Out(word Port,byte Value);  // Output byte to Port
  unsigned  Z80_RDMEM(word A);              // Read byte from given Mem Location
  void      Z80_WRMEM(word A,byte V);       // Write byte to Memory
  unsigned  Z80_RDSTACK(word A);            // Read byte from given Mem Location
  void      Z80_WRSTACK(word A,byte V);     // Write byte to Memory
  void      Z80_Patch(Z80_Regs *Regs);      // Called when ED FE occurs. Can be used
                                            // to emulate disk access etc.
  void      Z80_Reti(void);                 // Called when RETI occurs
  void      Z80_Retn(void);                 // Called when RETN occurs
  int       Z80_Interrupt(void);
  bool      Z80_InstHook(int PC);           // Called prior each instruction

  int       execute(void);
  int       dasm(int pc, char *buffer);
  Byte      reelpat;
  led       seg7;
  alpha		display;
  jpmreels	reel;
  meters    meter;
  lamps		lamp;
  triacs	triac;
  ay8910    gi;
  Z80PIO    pio1;
  Z80PIO    pio2;
  Z80PIO    pio3;
  Z80PIO    pio4;
  Z80PIO    pio5;
  Z80CTC    ctc;
  int       refresh;
  int       strobe;
  int       depth;
  bool      busy;
  int       bank;
  Byte      tune;
  Byte      latch;
  Byte      alpha[81];
  bool      alpha_update;
  Byte      chars;
  unsigned long cycles;
  Byte      memory[0x10000];
  TProconn  *parent;
};

class TProconn : public TSystem
{
public:
  Proconn board;
protected:
  void __fastcall Run();
public:
       __fastcall TProconn(String AGame, String AGamePath, TStringList *AROM_list);
  void __fastcall hard_reset(void);
  void __fastcall saveram();
  void __fastcall load_intelhex();
  void __fastcall UpdateReel(void);
  void __fastcall UpdateLamp(void);
  void __fastcall UpdateMeter(void);
  void __fastcall UpdateTriac(void);
  void __fastcall UpdateTrace(void);
  void __fastcall UpdateDisplay(void);
  void __fastcall UpdateMatrix(void);
  void __fastcall UpdateLedDisplay(void);
  void __fastcall UpdatePROCONMatrix(void);
  void __fastcall UpdateIOMeters(void);
  void __fastcall UpdateIOTriacs(void);
  void __fastcall UpdateIOSinks(void);
  void __fastcall UpdateIOSelects(void);
  void __fastcall UpdateIOReels(void);
  void __fastcall UpdateWave(void);
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
