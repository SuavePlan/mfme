//---------------------------------------------------------------------------
#ifndef spaceH
#define spaceH
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
//---------------------------------------------------------------------------

#include "mc6800.h"
#include "mc6821.h"
#include "mc6840.h"
#include "jpmreels.h"
#include "meter.h"
#include "lamp.h"
#include "led.h"
#include "triac.h"
#include "ay8910.h"
#include "alpha.h"

class TSPACE;

class SPACE : public mc6803 {
public:
    Byte	      read(Word);
    void	      write(Word, Byte);
    int	          execute(void);
    Byte          readport(Byte port);
    void          writeport(Byte port, Byte value, Byte changed);
    void          writeinternal(Byte port, Byte value);
    Byte          readinternal(Byte addr, Byte value);
    void          RxReelCommand(Byte cmd);
    Byte          TxReelResponse(void);
    alpha         display;
    mc6821        pia;
    ay8910        gi1;
    ay8910        gi2;
    jpmreels      reel;
    meters        meter;
    lamps         lamp;
    triacs        triac;
    led           seg7;
    int           refresh;
    int           irqcount;
    int           nmicount;
    Byte          aydata;
    unsigned int  cycles;
    bool          reelnmi;
    bool          nmilatch;
    bool          irqlatch;
    bool          reelen;
    int           strobecount;
    Word          outputsA, outputsB;
    Word          inputsA, inputsB;
    Byte          inputsC;
    Byte          strobe;
    Byte          lamphi;
    Byte          lamplo;
    bool          Rx;
    Byte          Rxcount;
    Byte          Rxcmd[20];
    Byte          Txcmd[20];
    Byte          *Rxptr;
    Byte          RxLength;
    Byte          TxLength;
    Byte          *Txptr;
    Byte          Txcount;
    Byte          Tx;
    int           matrixrefresh;
    int           reelspeed;
    bool          serdata;
    Byte          sercommand;
    Byte          firstbyte;
    Byte          secondbyte;
    Byte          tune;
    Byte          tune1;
    Byte          tune2;
    Byte          channel;
    bool          ioenabled;
    Byte          tunelatch;
    Byte          nar;
    Byte          busy;
    bool          tune1playing;
    bool          tune2playing;
    Byte          nartimer;
    Byte          extlamp;
    Byte          memory[0x10000];
    Byte          ram[0x2000];
    TSPACE        *parent;
};
// InputsA = IP0+IP1+IP2+IP3+IP4+IP5+IP6+IP7 +
//           24vcheck+jam+busy+credit2+credit1+vend2+vend1 +
//           inhibit2+inhibit1+credtaken+retnnote+solD+solC+solB+solA +
//           MeterH+MeterG+MeterF+MeterE+MeterD+MeterC+MeterB+MeterA
//
// InputsB = IP8+IP9+IP10+IP11+IP12+IP13+IP14+IP15 +
//           Backdoor+cashdoor+Refill+coinE+coinD+coinC+coinB+coinA +
//           volB+volA+U/A+inhibE+inhibD+inhibC+inhibB+inhibA
//
// InputsC = T/SlowerC+T/SlowerB+T/SlowerA+T/SUpperD+T/SUpperC+T/SUpperB+T/SUpperA
//
// OutputsA = volB+volA+U/A+inhibE+inhibD+inhibC+inhibB+inhibA;
//
// OutputsB = inhibit2+inhibit1+credtaken+retnnote+solD+solC+solB+solA +
//            MeterH+MeterG+MeterF+MeterE+MeterD+MeterC+MeterB+MeterA

class TSPACE : public TSystem
{
public:
    SPACE board;
protected:
	  void __fastcall Run();
public:
	  __fastcall TSPACE(String AGame, String AGamePath, TStringList *AROM_list);
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
    void __fastcall UpdateWave(void);
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void  __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void  __fastcall ClearRAM(void);
    bool __fastcall AutoLamp(int i, int &reel, int &symbol);
};

//---------------------------------------------------------------------------
#endif
