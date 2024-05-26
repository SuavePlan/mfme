 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
#include "debug.h"
#include "iomonitor.h"
#include "ace1.h"
#include "shared.h"

// addresses of possible interest
// fa00
// e000
// d720?
// d46c?
// d00c?
// cde7
// cdfa
// c96d
// c000
// abf2
// abf3

extern char buffer[];

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TACE1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TACE1::TACE1(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = ACE_SYSTEM1;
    MainDebug->SetProcess(this,board_type);
    board.parent = this;
//    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TACE1::Run()
{
int cycles = 0;

  SetReels(&board.reel, 4, 0);

  while ( !Terminated ) {
    cycles += 2;
    board.execute();
    if ( cycles > Speed ) {
      Sleep(1);
      cycles = 0;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TACE1::Run2()
{
int cycles = 0;

  SetReels(&board.reel, 4, 0);

  while ( !Terminated ) {
    board.execute();
    cycles += 2;
    if ( cycles > Speed ) {
      for ( int i = 0; i < 1500000; i++ )
        ;
      cycles = 0;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TACE1::hard_reset(void)
{
  board.refresh = 0;
  board.nmi = board.irq = false;
  board.innmi = board.inirq = false;
  board.irqcnt = board.nmicnt = 0;
  board.Z80_Reset();
  board.gi.AY8910_reset();
  board.gi.AY8910_set_clock(1000000);
  board.ppi24.reset();
  board.ppi25.reset();
  board.ppi37.reset();
  board.meter.reset();
  board.cycles = 0;
  board.refresh = 0;
}
//---------------------------------------------------------------------------
void __fastcall TACE1::saveram(void)
{
  TSystem::saveram(&board.memory[0x8000], 0x800);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::load_intelhex(void)
{
FILE		*fp;
int		addr, count;
struct stat statbuf;
String filename;
int size;

  addr = 0;
  for ( count = ROMList->Count; count ; count-- ) {
	fp = fopen(ROMList->Strings[count-1].c_str(), "rb");
    if ( fp != NULL ) {
      stat( ROMList->Strings[count-1].c_str(), &statbuf);
      size = statbuf.st_size;
      fread( &board.memory[addr], size, 1, fp);
      addr += size;
      fclose(fp);
    }
  }

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    fread( &board.memory[0x8000], 0x800, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------
int ACE1::execute(void)
{
  int cycles;

  refresh++;
  if ( refresh == 800000 ) {
    refresh = 0;
  }

  switch ( parent->state ) {
    case 0:
      parent->Stopped = false;
      if ( (refresh % 1000) == 0 ) {
        if ( meter.update() )
          parent->do_meter();
      }
      if ((refresh % 2000) == 0) {
        if (lamp.update())
          parent->do_lamp();
        if ( seg7.update() )
          parent->do_led_display();
      }
      if ( parent->reeltimer ) {
        parent->reeltimer--;
        if ( !parent->reeltimer )
          parent->do_reel();
      }
      if (irqcnt >= 160000) {
        irq=true;
        irqcnt -= 160000;
      }
      if ((nmicnt >= 3072) /*&& (R.IM==1)*/) {
        nmi=true;
        nmicnt -= 3072;
      }

      parent->sts = ++parent->sts % TRACE_SIZE;
      parent->trace[parent->sts].pc = R.PC.W.l;
      cycles = z80cpu::Z80_Step();
      parent->Total_cycles += cycles;
      nmicnt += cycles;
      irqcnt += cycles;

      parent->trace[parent->sts].a  = R.AF.B.l;
      parent->trace[parent->sts].b  = R.AF.B.h;
      parent->trace[parent->sts].x  = R.BC.W.l;
      parent->trace[parent->sts].y  = R.DE.W.l;
      parent->trace[parent->sts].u  = R.HL.W.l;
      parent->trace[parent->sts].s  = R.SP.W.l;
      if ( parent->state == MS_READ_BP )
        break;
      if ( R.PC.W.l == parent->bp )
        parent->state = MS_READ_BP;
      break;
    case 1:
      parent->sts = ++parent->sts % TRACE_SIZE;
      parent->trace[parent->sts].pc = R.PC.W.l;
      cycles = z80cpu::Z80_Step();
      nmicnt += cycles;
      irqcnt += cycles;
      parent->Total_cycles += cycles;
      parent->trace[parent->sts].a  = R.AF.B.l;
      parent->trace[parent->sts].b  = R.AF.B.h;
      parent->trace[parent->sts].x  = R.BC.W.l;
      parent->trace[parent->sts].y  = R.DE.W.l;
      parent->trace[parent->sts].u  = R.HL.W.l;
      parent->trace[parent->sts].s  = R.SP.W.l;
    case 2:
    case 3:
    case 4:
      parent->do_debug();
      MainDebug->Halt();
      parent->state = MS_HALTED;
      break;
    case 5:
      parent->hard_reset();
    case 6:
      MainDebug->RunClick(NULL);
      break;
    default:
      parent->Stopped = true;
      break;
  }
  if ( (refresh % 2000) == 0 && !NoSound )
    UpdateSoundBuffersys1(0);

  return cycles;
}
//---------------------------------------------------------------------------

byte ACE1::Z80_In (word Port)
{
}

void ACE1::Z80_Out (word Port,byte Value)
{
}

unsigned ACE1::Z80_RDMEM(word A)
{
char buffer[150];
Byte ret = 0;
Byte reelpulse = 0;

  switch (A & 0xe000) {
    // Read from ROM
    case 0x0000:
    case 0x2000:
    case 0x4000:
    case 0x6000:
    // Read from RAM
    case 0x8000:
      ret = memory[A];
      break;
    // MMIO
    case 0xa000:
      switch (A & 0xfff0) {
        case 0xadf0:
          gi.porta_in = ~DIP1;
          gi.portb_in = ~DIP2;
          ret = gi.AY8910Read();
          break;
        case 0xafb0:
          ppi24.porta_in = 0xd0;
          if (strobe == 0x0a)
            ppi24.porta_in ^= 0x40;
          if (ppi24.portb > 0)
            ppi24.porta_in ^= 0x80;
          ret = ppi24.read(A & 0x03);
          break;
        case 0xafd0:
          ret = ppi25.read(A & 0x03);
          break;
        case 0xafe0:
          switch ( A & 0x3 ) {
            case 0: // door & coin switches
              ppi37.porta_in = (~matrix[0] & 0x7f);// | ((ppi25.portb & 0xf5) ? 0x80 : 0x00);
              break;
            case 1:  // main switches
              ppi37.portb_in = 0xff;
              if ((strobe > 0) && (strobe < 7)) {
                ppi37.portb_in = ~matrix[strobe];
              }
              break;
            case 2:
              for (int reelcnt = 0; reelcnt < 4; reelcnt++) {
                if ( reel.reels[reelcnt].stops )
                  if (((reel.reels[reelcnt].pos % (reel.reels[reelcnt].steps / reel.reels[reelcnt].stops)) == 0) )
                    reelpulse |= (1 << reelcnt);
              }
              ppi37.portc_in = ((reel.optos & 0x0f) << 4) | reelpulse;
              break;
          }
          ret = ppi37.read(A & 0x03);
          break;
      }
      break;
    // ILLEGAL! NOT USED
    case 0xc000:
      irq = false;
      break;
    // NMI COUNTDOWN RESET
    case 0xe000:
      nmicnt = 0;
      break;
  }

  return ret;
}

void ACE1::Z80_WRMEM(word A,byte V)
{
char buffer[150];

  switch (A & 0xe000) {
    // Write To ROM!!
    case 0x0000:
    case 0x2000:
    case 0x4000:
    case 0x6000:
      break;
    // Write To RAM
    case 0x8000:
      if (A < 0x8800) {
        memory[A] = V;
      }
      break;
    // MMIO
    case 0xa000:
      // MMIO or Illegal
      switch (A & 0xfff0) {
        case 0xadf0:
          gi.AY8910Write(A & 0x1, V);
          break;
        case 0xafb0:
          ppi24.write(A & 0x03, V);
          strobe = ppi24.portc & 0x0f;
          if (MonitorActive) {
            IOMonitor->UpdateLampSelects(strobe);
            IOMonitor->UpdateAux1(ppi24.portc & 0xf0);
          }
          if ((ppi24.porta_updated & 0x7f) && ppi24.porta ) {
            if (MonitorActive)
              IOMonitor->Update7segs(ppi24.porta);
            seg7.writejpm(ppi24.porta & 0x7f, strobe);
            if (seg7.updated[strobe])
              parent->do_led_display();
          }
          if (ppi24.portb_updated) {
            if (MonitorActive)
              IOMonitor->UpdateLampDrives1(ppi24.portb);
            if ((strobe>0) && (strobe<11)) {
              if (lamp.write(ppi24.portb, strobe - 1, 0, 8))
                parent->do_lamp();
            }
          }
          break;
        case 0xafd0:
          ppi25.write(A & 0x03, V);
          if (MonitorActive) {
            IOMonitor->UpdateTriacs(ppi25.porta);
            IOMonitor->UpdateMeters(ppi25.portb & 0xf5);
            IOMonitor->UpdateAux2(ppi25.portb & 0x0a);
          }
          if ( (ppi25.portb_changed & 0xa) && ((~ppi25.portb) & 0xa) ) {
            Byte extlmp = (ppi25.portc & 0x70) >> 4;
            if ( !(ppi25.portb & 0x02)) {
              // Extender 2
              if ( lamp.writebit(87 - extlmp, ppi25.portc & 0x80) )
                parent->do_lamp();
            }
            if ( !(ppi25.portb & 0x08)) {
              // Extender 1
              if ( lamp.writebit(95 - extlmp, ppi25.portc & 0x80) )
                parent->do_lamp();
            }
          }
          if ( ppi25.portb_changed & 0xf5 ) {
            // Meters bits 0, 2 and 4. 5 thru 7 are spare meters
            // cash in, refill, cash out respectively
            meter.write((ppi25.portb & 0x05) | ((ppi25.portb & 0x10) >> 3) | ((ppi25.portb & 0xe0) >> 2));
          }
          if ((ppi25.portc_changed & 0xf) & (ppi25.portc & 0xf)) {
            if (MonitorActive)
              IOMonitor->UpdateAux3(ppi25.portc);
            reel.write_sys1((ppi25.portc & 0xf0)|((ppi25.portc_changed & 0xf) & (ppi25.portc & 0xf)));
            if ( reel.changed[0] || reel.changed[1] ||
                 reel.changed[2] || reel.changed[3] ) {
              if ( !parent->reeltimer )
                parent->reeltimer = 200;
//              parent->do_reel();
            }
          }
          break;
        case 0xafe0:
          ppi37.write(A & 0x03, V);
          break;
      }
      break;
    // ILLEGAL! NOT USED
    case 0xc000:
      break;
    // NMI COUNTDOWN RESET
    case 0xe000:
      nmicnt = 0;
      break;
  }
}

unsigned ACE1::Z80_RDSTACK(word A)
{
  return Z80_RDMEM(A);
}

void ACE1::Z80_WRSTACK(word A,byte V)
{
  Z80_WRMEM(A,V);
}

void ACE1::Z80_Patch (Z80_Regs *Regs)
{
  // Nothing required here for now
}

void ACE1::Z80_Reti (void)
{
    inirq = false;
}

void ACE1::Z80_Retn (void)
{
    innmi = false;
}

int ACE1::Z80_Interrupt(void)
{
int rv;

  if ( nmi ) {
    rv = Z80_NMI_INT;
  } else if ( irq ) {
    rv = Z80_INT_REQ;
  } else
    rv = Z80_IGNORE_INT;

//  irq = false;
  nmi = false;
  return rv;
}

bool ACE1::Z80_InstHook(int PC)
{
}
//---------------------------------------------------------------------------

int ACE1::dasm(int pc, char *buffer)
{
  return Z80_Dasm(&memory[pc], buffer, pc);
}

//---------------------------------------------------------------------------

void __fastcall TACE1::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TACE1::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TACE1::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 256);
/*      if ( ( i > 9) && (i < 30) ) {
        reel = (i<22) ? (i - 10) / 3 : (i>23) ? (i - 12) / 3 : 0;
        symbol = (i<22) ? 2 - ( (i - 10) % 3 ) : (i>23) ? 2 - ( (i - 24) % 3 ) : 4;
        if ( symbol < 3 ) {    */
  UpdateLedDisplays(&board.seg7);
}

//---------------------------------------------------------------------------

void __fastcall TACE1::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TACE1::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

int __fastcall TACE1::getpc(void)
{
  return board.R.PC.W.l;
}
//---------------------------------------------------------------------------
int __fastcall TACE1::getmemory(int address)
{
  if ( address >= 0 && address < 0x10000 )
    return board.memory[address];
  else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TACE1::setmemory(int address, Byte value)
{
  board.memory[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TACE1::dasm(int pc, char *buffer)
{
  return board.dasm(pc,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::ClearRAM(void)
{
  for (int i = 0x8000; i < 0x8800; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TACE1::UpdateIOMeters(void)
{
//  IOMonitor->UpdateMeters(board.pia5.orb);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.memory);
  savestate(ptr,board.refresh);
//  board.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
//  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
  board.gi.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TACE1::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.memory);
  readstate(ptr,board.refresh);
//  board.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
//  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
  board.gi.LoadFromStream(ptr);
}

//---------------------------------------------------------------------------
void __fastcall TACE1::UpdateTrace(void)
{
char buffer[150];
int count;
int addr;
Word pos;
char args[50];

  MainDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
    addr &= 0xFFFF;
    board.dasm( addr, buffer);
    strncat( buffer, "                                        ", 40 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }
}
//---------------------------------------------------------------------------


