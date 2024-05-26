 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
#include "debug.h"
#include "iomonitor.h"
#include "sys80.h"
#include "shared.h"


extern char buffer[];

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMPS2::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSYS80::TSYS80(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = JPM_SYS80;
    MainDebug->SetProcess(this,board_type);
    board.parent = this;
//    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::Run()
{
int cycles = 0;

  SetReels(&board.reel, 10, 7);

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
void __fastcall TSYS80::Run2()
{
int cycles = 0;

  SetReels(&board.reel, 10, 7);

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

void __fastcall TSYS80::saveram(void)
{
  TSystem::saveram(&board.memory[0x3000], 0x1000);
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::load_intelhex(void)
{
FILE		*fp;
int		addr, count;
struct stat statbuf;
String filename;
int size;

  addr = 0;
  for ( count = 0; count < ROMList->Count; count++ ) {
	fp = fopen(ROMList->Strings[count].c_str(), "rb");
    if ( fp != NULL ) {
      stat( ROMList->Strings[count].c_str(), &statbuf);
      size = statbuf.st_size;
      fread( &board.memory[addr], size, 1, fp);
      addr += size;
      fclose(fp);
    }
  }

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    fread( &board.memory[0x3000], 0x1000, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::hard_reset(void)
{
  board.refresh = 0;
  board.reset(NULL, 0);
  board.meter.reset();
  board.seg7.reset();
  board.cycles = 0;
  board.refresh = 0;
  board.cyccount = 0;
  board.cyccount2 = 0;
  board.lev1_int = 0;
  board.lev2_int = 0;
}


//---------------------------------------------------------------------------
int SYS80::execute(void)
{
	int cycles;

    refresh++;
    switch ( Fruit->state ) {
      case 0:
        Fruit->Stopped = false;
        if ( (refresh % 800) == 0 ) {
          if ( seg7.update() )
            parent->do_led_display();
        }
        Fruit->sts = ++Fruit->sts % TRACE_SIZE;
        Fruit->trace[Fruit->sts].pc = PC;
        if ( PC >= 0x3000 ) {
          parent->state = MS_EXECUTE_BP;
          break;
        }
        cycles = tms99xx::execute();
        cyccount += cycles;
        cyccount2 += cycles;
        Fruit->Total_cycles += cycles;
        if ( duart1.pending ) {
          int_pending |= 4;
        } else {
          int_pending &= ~4;
          set_irq_line(5, CLEAR_LINE);
        }
        if (cyccount > 1042) {
          cyccount -= 1042;
          int_pending |= 1;
        }
        if (cyccount2 > 25000) {
          cyccount2 -= 25000;
          int_pending |= 2;
        }
        if ( int_pending & 1 ) {
          set_irq_line(3, ASSERT_LINE); // 2.44 KHz Interrupt highest priority
        } else if ( int_pending & 2 ) {
          set_irq_line(4, ASSERT_LINE); // 100Hz Interrupt 2nd  Highest
        } else if ( int_pending & 4 ) {
          set_irq_line(5, ASSERT_LINE); // duart lowest
        }
        if ( Fruit->state == 2 )
          break;
        if ( PC == Fruit->bp )
          Fruit->state = 2;
        break;
      case 1:
        Fruit->sts = ++Fruit->sts % TRACE_SIZE;
        Fruit->trace[Fruit->sts].pc = PC;
	    cycles = tms99xx::execute();
        Fruit->Total_cycles += cycles;
        if ( Fruit->in_int )
          break;
      case 2:
      case 3:
      case 4:
        Fruit->do_debug();
        MainDebug->Halt();
        Fruit->state = 10;
        break;
      case 5:
        Fruit->hard_reset();
      case 6:
        MainDebug->RunClick(NULL);
        break;
      default:
        Fruit->Stopped = true;
        break;
    }

    if ( (refresh % 700) == 0 && !NoSound )
      UpdateSoundBuffersys80(0);

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::UpdateTrace(void)
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
Byte SYS80::readport(Word addr)
{
int ret = 0;

  switch (addr & 0x1e0) {
    case 0x000: // i/o card
      matrix[0] = (matrix[0] & 0xf0) | (reel.optos ^ 0xf);
      ret = matrix[addr / 8];
      ret >>= (addr % 8);
      ret &= 0x01;
      break;
    case 0x140:
      break;
    case 0x1e0:
      ret = duart1.read(addr & 0x1f);
      break;
    default:
      ret = 1;
      break;
  }
  return ret;
}
//---------------------------------------------------------------------------
void SYS80::writeport(Word addr,Byte value)
{
Word mask;

  switch (addr & 0x1f0) {
    case 0x000: // reels
      mask = 1 << addr;
      reelport = ( reelport & ~mask ) | ( mask * value);
      if ( addr == 0xf && reelport != old_reel && reelport ) {
        reel.writeflip03(reelport);
        if ( MonitorActive ) {
          IOMonitor->UpdateReel1(0, reel.reels[2].pos, reel.reels[3].pos);
          IOMonitor->UpdateReel2(0, reel.reels[0].pos, reel.reels[1].pos);
        }
        if ( reel.changed[0] || reel.changed[1] ||
             reel.changed[2] || reel.changed[3] )
          parent->do_reel();
        old_reel = reelport;
      }
      break;
    case 0x050:
      switch ( addr ) {
        case 0x50:  // Int1 En
          if ( lev1_int != value )
          {
            if ( !lev1_int ) {
              set_irq_line(3, CLEAR_LINE); // 2.44 KHz Interrupt
              int_pending &= ~1;
            }
          }
          lev1_int = value;
          break;
        case 0x51:  // Int2 En
          if ( lev2_int != value )
          {
            if ( !lev2_int ) {
              set_irq_line(4, CLEAR_LINE); // 100Hz Interrupt
              int_pending &= ~2;
            }
          }
          lev2_int = value;
          break;
        case 0x52:  // Watchdog
          break;
        case 0x53:  // I/O Enable
          break;
      }
      break;
    case 0x140: // CSG
    case 0x150:
      if ( addr < 0x148 ) {
        Byte mask = 1 << (addr - 0x140);
        soundport = ( soundport & ~mask ) | ( mask * value);
      } else if ( addr == 0x149 && value == 1 ) {
        gi.write_data( soundport, 1, 1, 0);
      } else if ( addr == 0x14a && value == 1 ) {
        gi.write_data( soundport, 1, 0, 1);
      }
      break;
    case 0x1e0: // rs232
    case 0x1f0:
      duart1.write(addr & 0x1f, value);
      break;
    default:
      addr &= 0x1ff;
      switch ( sys80io[addr].type ) {
        case S80_LAMP:
          if ( addr < 320 && lamp.writebit(addr, value) )
            Fruit->do_lamp();
          break;
        case S80_METER:
          if ( sys80io[addr].value )
            if ( meter.write(value << (sys80io[addr].value - 1)) )
              Fruit->do_meter();
          break;
        case S80_TRIAC:
          break;
        case S80_SEG:
          {
            Byte mask = 1 << sys80io[addr].value;
            extport = ( extport & ~mask ) | ( mask * value);
          }
          break;
        case S80_BCD:
          {
            Byte mask = 1 << (sys80io[addr].value & 7);
            if ( sys80io[addr].value & 8 )
              value = 1 - value;
            bcd = ( bcd & ~mask ) | ( mask * value);
          }
          break;
        case S80_SEG_DIGIT:
          {
            Byte pos = sys80io[addr].value % 8;
            Byte row = sys80io[addr].value / 8;
            if ( value && seg7.write_data(~extport, 7 - pos, row) )
              parent->do_led_display();
          }
          break;
        case S80_BCD_DIGIT:
          {
            if ( value && seg7.writebcd(bcd, sys80io[addr].value) )
              parent->do_led_display();
          }
          break;
      }
      break;
  }
}
//---------------------------------------------------------------------------
Byte SYS80::read(Word addr)
{
Byte ret = 0;

  if ( addr < 0x3000 || ( addr >= 0x3400 && addr < 0x4000 ) )
    ret = memory[addr];
  else if ( parent->errors )
    parent->state = MS_EXECUTE_BP;

  if ( (addr & Fruit->read_mask) == Fruit->read_bp ) {
    if ( (ret & Fruit->read_value_mask) == Fruit->read_value_bp ) {
      Fruit->read_addr = addr;
      Fruit->read_value = ret;
      if ( Fruit->readbp )
        Fruit->state = 2;
    }
  }
  return ret;
}
//---------------------------------------------------------------------------

void SYS80::write(Word addr, Byte x)
{
  if ( addr >= 0x3000 && addr < 0x4000 )
    memory[addr] = x;
  if ( (addr & Fruit->write_mask) == Fruit->write_bp ) {
    if ((x & Fruit->write_value_mask) == Fruit->write_value_bp ) {
      Fruit->write_addr = addr;
      Fruit->write_value = x;
      if ( Fruit->writebp )
        Fruit->state = 3;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 320);
}

//---------------------------------------------------------------------------

void __fastcall TSYS80::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TSYS80::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

int __fastcall TSYS80::getpc(void)
{
  return board.PC;
}
//---------------------------------------------------------------------------
int __fastcall TSYS80::getmemory(int address)
{
  if ( address >= 0 && address < 0x10000 )
    return board.memory[address];
  else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::setmemory(int address, Byte value)
{
  board.memory[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TSYS80::dasm(int pc, char *buffer)
{
  return board.dasm(pc,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x800; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::UpdateIOMeters(void)
{
//  IOMonitor->UpdateMeters(board.pia5.orb);
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::SaveToStream(Byte *&ptr)
{
/*  ptr->WriteBuffer(&board.memory, 0x800);
  ptr->WriteBuffer(&board.refresh, 4);
  board.SaveToStream(ptr);
//  board.display.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
//  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
//  board.duart.SaveToStream(ptr); */
}
//---------------------------------------------------------------------------
void __fastcall TSYS80::LoadFromStream(Byte *&ptr)
{
/*  ptr->ReadBuffer(&board.memory, 0x800);
  ptr->ReadBuffer(&board.refresh, 4);
  board.LoadFromStream(ptr);
//  board.display.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
//  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
//  board.duart.LoadFromStream(ptr); */
}

