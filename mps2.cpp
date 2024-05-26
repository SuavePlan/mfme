 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
#include "debug.h"
#include "iomonitor.h"
#include "mps2.h"
#include "shared.h"

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
//        Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMPS2::TMPS2(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = JPM_MPS2;
    MainDebug->SetProcess(this,board_type);
    board.parent = this;
//    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::Run()
{
int cycles = 0;

  SetReels(&board.reel, 6, -3);

  while ( !Terminated ) {
    board.execute();
    cycles += 2;
    if ( cycles > Speed ) {
      Sleep(1);
      cycles = 0;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::Run2()
{
int cycles = 0;

  SetReels(&board.reel, 6, -3);

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

void __fastcall TMPS2::saveram(void)
{
  TSystem::saveram(&board.memory[0xe800],0x800);
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::load_intelhex(void)
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
    fread( &board.memory[0xe800], 0x800, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::hard_reset(void)
{
  board.refresh = 0;
  board.reset(NULL, 1);
  board.meter.reset();
  board.duart1.reset(1);
  board.duart2.reset(2);
  board.seg7.reset();
  board.cycles = 0;
  board.refresh = 0;
  board.power_fail = 1;
  board.watchdog_timeout = 1;
  board.invalid_access = 1;
  board.clear_down = 1;
  board.hopper.reset();
}

//---------------------------------------------------------------------------
int MPS2::execute(void)
{
int cycles;

    refresh++;
    if ( refresh == 100000 ) {
      refresh = 0;
    }
    switch ( parent->state ) {
      case 0:
        parent->Stopped = false;
        if ( (refresh % 500) == 0 ) {
          if ( seg7.update() )
            parent->do_led_display();
          if ( lamp.update() )
            parent->do_lamp();
          if ( meter.update() )
            parent->do_meter();
        }
        if ( (refresh % 25000) == 0 ) {
          hopper.update();
        }
        if ( reel.tick(last_cycles) ) {
          if ( MonitorActive ) {
            IOMonitor->UpdateReel1(0, reel.reels[2].pos, reel.reels[3].pos);
            IOMonitor->UpdateReel2(0, reel.reels[0].pos, reel.reels[1].pos);
          }
//          if ( !parent->reeltimer )
//            parent->reeltimer = 400;
          parent->do_reel();
        }
        if ( parent->reeltimer ) {
          parent->reeltimer--;
          if ( !parent->reeltimer )
            parent->do_reel();
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = PC;
        parent->trace[parent->sts].x = STATUS;
        parent->trace[parent->sts].y = WP;
        parent->trace[parent->sts].u = readword(WP+26);
        cycles = tms99xx::execute();
        last_cycles = cycles;
        parent->Total_cycles += cycles;
        duart1.tick(cycles);
        duart2.tick(cycles);
        if ( duart1.pending || duart2.pending ) {
          if ( duart1.pending )
            duart1_int = 0;
          else
            duart1_int = 1;
          if ( duart2.pending )
            duart2_int = 0;
          else
            duart2_int = 1;
          set_irq_line(0, ASSERT_LINE);
        } else {
          duart1_int = 1;
          duart2_int = 1;
          set_irq_line(0, CLEAR_LINE);
        }
        if ( duart1.RxChar ) {
          duart1.RxChar = false;
          if ( reel.mps2Rxchar(duart1.tx_register) )
            duart1.TxReply(reel.reply);
        }
/*
        if ( duart1.RxMessage ) {
          char *reply;
          duart1.RxMessage = false;
          reply = reel.RxMessage(duart1.tx_buffer);
          duart1.TxReply(reply);
        }
*/
        if ( parent->state == MS_READ_BP )
          break;
        if ( PC == parent->bp )
          parent->state = MS_READ_BP;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = PC;
        cycles = tms99xx::execute();
        parent->Total_cycles += cycles;
        duart1.tick(cycles);
        duart2.tick(cycles);
        if ( duart1.pending || duart2.pending ) {
          if ( duart1.pending )
            duart1_int = 0;
          else
            duart1_int = 1;
          if ( duart2.pending )
            duart2_int = 0;
          else
            duart2_int = 1;
          set_irq_line(0, ASSERT_LINE);
        } else {
          duart1_int = 1;
          duart2_int = 1;
          set_irq_line(0, CLEAR_LINE);
        }
        if ( parent->in_int )
          break;
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

    if ( (refresh % 1400) == 0 && !NoSound )
      UpdateSoundBuffermps2(0);


  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::UpdateTrace(void)
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
Byte MPS2::readport(Word addr)
{
int ret = 0;

  switch ( addr & 0xffe0 ) {
    case 0x0:
      ret = duart2.read(addr & 0x1f);
      break;
    case 0x20:
      switch ( addr & 0x1f ) {
        case 0:
          ret = power_fail;
          break;
        case 1:
          ret = watchdog_timeout;
          break;
        case 2:
          ret = invalid_access;
          break;
        case 3:
          ret = clear_down;
          break;
        case 6:
          ret = duart1_int;
          break;
        case 7:
          ret = duart2_int;
          break;
        default:
          if ( parent->errors )
            parent->state = MS_EXECUTE_BP;
          break;
      }
      break;
    case 0x40:
      ret = duart1.read(addr & 0x1f);
      break;
    case 0x60:
      ret = 1;
      break;
    default:
      if ( parent->errors )
        parent->state = MS_EXECUTE_BP;
      break;
  }
  return ret;
}
//---------------------------------------------------------------------------
void MPS2::writeport(Word addr,Byte value)
{
  switch ( addr & 0xffe0 ) {
    case 0x0:
      duart2.write(addr & 0x1f, value);
      break;
    case 0x20:
      switch ( addr & 0x7 ) {
        case 0:
          power_fail = 1;
          break;
        case 1:
          watchdog_timeout = 1;
          break;
        case 2:
          invalid_access = 1;
          break;
      }
      break;
    case 0x40:
      duart1.write(addr & 0x1f, value);
      break;
    case 0x60:
      switch ( addr & 0x7 ) {
        case 0:
          watchdog_strobe = value;
          break;
        case 1:
          ram_enable = value;
          break;
        case 2:
          alarm = value;
          break;
        case 3:
          nmi_en = value;
          break;
        case 4:
          reel_drive_enable = value;
          break;
        case 5:
          io_enable = value;
          break;
        case 6:
          bb_ptr = value;
          break;
        case 7:
          diag_led = value;
          break;
      }
      break;
    default:
      if ( parent->errors )
        parent->state = MS_EXECUTE_BP;
      break;
  }

}
//---------------------------------------------------------------------------
Byte MPS2::read(Word addr)
{
Byte ret = 0;

  if ( addr < 0xc000 || (addr >= 0xe800 && addr < 0xf0fc) )
    ret = memory[addr];
  else if ( (addr & 0xfff0) == 0xc000 ) {
    switch ( addr & 0xc ) {
      case 0x0:
        ret = ppi26.read(addr & 0x3); // Outputs
        break;
      case 0x4:
        switch ( addr & 0x3 ) {
          case 0:
            if ( (Hoppers & 0x40) == 0 )
              matrix[3] = (matrix[3] & 0x7f) + (hopper.opto() << 7);
            ret = ~matrix[3];
            break;
          case 1:
            ret = ~( ((matrix[2] & 0xf8) >> 3) | ((matrix[0] & 0x7) << 5) );
            break;
          case 2:
            ret = ~( ((matrix[0] & 0xf8) >> 3) | ((matrix[1] & 0x7) << 5) );
            break;
          default:
            ret = ppi21.read(addr & 0x3);
            break;
        }
        break;
      case 0x8:
        switch ( addr & 0x3 ) {
          case 0:
            ret = ~( ((matrix[1] & 0xf8) >> 3) | ((matrix[2] & 7) << 5) );
            break;
          case 1:
            if ( ( ppi22.portc & 2 ) == 0 )
              ret = ~DIP1;
            else if ( ( ppi22.portc & 1 ) == 0 )
              ret = ~DIP2;
            else if ( ( ppi22.portc & 4 ) == 0 )
              ret = 0xff;
            break;
          case 2:
            ret = (Rotary << 4) ^ 0xf0; // Hex switch
            break;
          default:
            ret = ppi22.read(addr & 0x3);
            break;
        }
        break;
      case 0xc:
        ret = ppi25.read(addr & 0x3); // Lamps
        break;
    }
  } else if ( parent->errors )
    parent->state = MS_EXECUTE_BP;

  if ( (addr & parent->read_mask) == parent->read_bp ) {
    if ( (ret & parent->read_value_mask) == parent->read_value_bp ) {
      parent->read_addr = addr;
      parent->read_value = ret;
      if ( parent->readbp )
        parent->state = MS_READ_BP;
    }
  }

  return ret;
}
//---------------------------------------------------------------------------

void MPS2::write(Word addr, Byte x)
{
  if ( addr >= 0xe800 && addr < 0xf0fc )
    memory[addr] = x;
  else if ( (addr & 0xfff0) == 0xc000 ) {
    switch ( addr & 0xc ) {
      case 0x0:
        ppi26.write(addr & 0x3, x); // Outputs
        if ( ( ppi26.portb_changed & 0x81 ) || ppi26.porta_changed ) {
          int hd10 = ppi26.porta & 0xf0;
          if ( (ppi26.porta & 0x8) == 0 ) {
            hd10 |= ((ppi26.portb & 0x80) >> 7) + ((ppi26.portb & 0x1) << 4);
            hd10 |= ((ppi26.porta & 0x1) << 3) + ((ppi26.porta & 0x2) << 1);
            hd10 |= ((ppi26.porta & 0x4) >> 1);
          }
          if ( (Hoppers & 0x40) == 0 )
            hopper.enable( hd10 & 4, 0, 1);
        }
        if ( ppi26.portb_updated )
          if ( seg7.writebcd( ((ppi26.portb & 0x3c) >> 2) ^ 0xf, ppi25.portc & 0xf) )
            parent->do_led_display();
        if ( ppi26.portc_changed ) {
          if ( MonitorActive )
            parent->do_io_meters();
          meter.write(ppi26.portc);
        }
        break;
      case 0x4:
        ppi21.write(addr & 0x3, x); // Switches
        break;
      case 0x8:
        ppi22.write(addr & 0x3, x); // Switches & Sound
        if ( (ppi22.portc_changed & 0x4 ) && (ppi22.portc & 4) == 4 )
          sn.SN76496Write(ppi22.portb);
        break;
      case 0xc:
        ppi25.write(addr & 0x3, x); // Lamps
//        if ( (ppi25.porta_updated && ppi25.porta) || (ppi25.portb_updated && ppi25.portb) )
//        if ( ppi25.porta_changed || ppi25.portb_changed )
        if ( ppi25.porta_updated || ppi25.portb_updated )
          if ( lamp.writejpm((ppi25.portb << 8) | ppi25.porta, ppi25.portc & 0xf, 12) )
            parent->do_lamp();
        if ( (ppi25.portc_changed & 0xf) && (ppi25.portc & 0xf) == 0 ) {
//        if ( ppi25.portc_changed ) {
//        if ( (ppi25.portc & 0xf) != last_mux ) {
//          last_mux = ppi25.portc & 0xf;
//          parent->do_lamp();
        }
        break;
    }
  } else if ( parent->errors )
    parent->state = MS_EXECUTE_BP;


  if ( (addr & parent->write_mask) == parent->write_bp ) {
    if ((x & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = addr;
      parent->write_value = x;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::UpdateLedDisplay(void)
{
  UpdateLedDisplays( &board.seg7 );
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::UpdateLamp(void)
{
  UpdateLamps( &board.lamp, 256);
}

//---------------------------------------------------------------------------

void __fastcall TMPS2::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TMPS2::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

int __fastcall TMPS2::getpc(void)
{
  return board.PC;
}
//---------------------------------------------------------------------------
int __fastcall TMPS2::getmemory(int address)
{
  if ( address >= 0 && address < 0x10000 )
    return board.readbyte(address);
  else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::setmemory(int address, Byte value)
{
  board.writebyte(address, value);
}
//---------------------------------------------------------------------------
int __fastcall TMPS2::dasm(int pc, char *buffer)
{
  return board.dasm(pc,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x800; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::UpdateIOMeters(void)
{
  IOMonitor->UpdateMeters(board.ppi26.portc);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.memory);
  savestate(ptr,board.refresh);
  board.SaveToStream(ptr);
//  board.display.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
//  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
//  board.duart.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TMPS2::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.memory);
  readstate(ptr,board.refresh);
  board.LoadFromStream(ptr);
//  board.display.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
//  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
//  board.duart.LoadFromStream(ptr);
}

