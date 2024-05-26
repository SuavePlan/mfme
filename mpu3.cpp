//---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
#include "configuration.h"
#include "debug.h"
#include "iomonitor.h"
#include "mpu3.h"
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
//      void __fastcall TMPU3::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMPU3::TMPU3(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
  board_type = BARCREST_MPU3;
  MainDebug->SetProcess(this,board_type);
  board.parent = this;
//  Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TMPU3::Run()
{
int cycles = 0;

  SetReels(&board.reel, 5, 0);

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

void __fastcall TMPU3::Run2()
{
int cycles = 0;

  SetReels(&board.reel, 5, 0);

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

void __fastcall TMPU3::saveram()
{
  TSystem::saveram(board.memory,0x400);
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::load_intelhex()
{
FILE		*fp;
int		addr;
struct stat statbuf;
String filename;
int size;
int count;
bool is_ihex = false;
char ch;

  addr = 0;
  size = 0;
  for ( count = 0; count < ROMList->Count; count++ ) {
	fp = fopen(ROMList->Strings[count].c_str(), "rb");
    if ( fp != NULL ) {
      fread( &ch, 1, 1, fp);
      if ( ch == ':' )
        is_ihex = true;
      stat( ROMList->Strings[count].c_str(), &statbuf);
      size += statbuf.st_size;
      fclose(fp);
    }
  }

  if ( is_ihex ) {
    IntelHexRead( board.memory, ROMList->Strings[0] );
  } else {
    if ( size < 0x10000 )
      addr = 0x10000 - size;

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
  }

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    fread( &board.memory[0], 0x400, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::hard_reset(void)
{
  board.refresh = 0;
  board.reset();
  board.ptm.reset();
  board.pia1.reset();
  board.pia2.reset();
  board.pia3.reset();
  board.pia4.reset();
  board.misc.reset(SampleRate);
  board.meter.reset();
  board.display.reset();
  board.decoder.reset();
  board.current_sense = 0;
}

//---------------------------------------------------------------------------
int MPU3::execute(void)
{
	Byte	do_int;
    int     cycs;

    do_int = 0;
    refresh++;
    if ( refresh == 100000 )
      refresh = 0;
    switch ( parent->state ) {
      case 0:
        misc.update();
        if ( misc.zero_changed ) {
          if ( !misc.zero ) {
            ptm.clock1();
            pia1.set_cb1(0);
            pia1.ora_in &= ~2;
          } else {
            pia1.set_cb1(1);
            pia1.ora_in |= 2;
          }
        }
        if ( misc.timer_changed ) {
            if ( misc.timer )
              pia2.set_ca1(1);
            else {
              pia2.set_ca1(0);
              ptm.clock2();
            }
        }
        if ( (refresh % 1000) == 0 ) {
          lamp.update();
          seg7.update();
          decoder.update();
          if ( meter.update() )
            parent->do_meter();
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        cycs = mc6800::execute();
        sound_updated += cycs;
        cycles += cycs;
        parent->Total_cycles += cycs;
        ptm.tick(cycs);
        misc.tick(cycs);
        if ( !(cc & 0x10) ) {
          if ( ptm.sr & 0x80 )
            do_int |= 1;
          if ( (( pia2.cra & 0x81 ) == 0x81) || (( pia1.crb & 0x81 ) == 0x81 ) )
            do_int |= 2;
          if ( (( pia4.cra & 0x81 ) == 0x81) || (( pia4.cra & 0x68 ) == 0x48 ) )
            do_int |= 4;
        }
        if ( parent->state == MS_READ_BP )
          break;
        if ( do_int )
          ENTER_INTERRUPT(0xfff8);
        if ( pc.w.l == parent->bp )
          parent->state = MS_READ_BP;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        cycs = mc6800::execute();
        cycles += cycs;
        parent->Total_cycles += cycs;
        ptm.tick(cycs);
        misc.tick(cycs);
        if ( ptm.sr & 0x80 )
          do_int |= 1;
        if ( do_int )
          ENTER_INTERRUPT(0xfff8);
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
        break;
    }
    if ( sound_updated > 5000 ) {
      if ( !NoSound )
        UpdateSoundBuffermpu3(cycles);
      sound_updated -= 5000;
    }

  return cycs;
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::UpdateTrace(void)
{
char buffer[80];
int count;
int addr;
Word pos;

  MainDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos++ % TRACE_SIZE].pc;
    addr &= 0xFFFF;
    board.dasm( addr, buffer);
    MainDebug->Code->Items->Add(buffer);
  }

}
//---------------------------------------------------------------------------

Byte MPU3::read(Word addr)
{
Byte		ret = 0;

  if ( addr < 0x800 || ( addr >= 0x8000 && addr < 0x8800 ) )
    ret = memory[addr & 0x7ff];
  else if ( (addr & 0xc000) == 0x8000 ) {
	switch ( addr & 0xf800 ) {
		case 0x8800:
          ret = ptm.read((addr & 0x1f)>>2);
          break;
        case 0x9000:		// IC 3
          switch ( decoder.column ) {
            case 0:
            case 1:
            case 2:
            case 3:
              pia1.ora_in = matrix[decoder.column] << 2;
              break;
            case 4:
              pia1.ora_in = ((DIP1 & 1) << 7) + ((DIP1 & 2) << 5) +
                            ((DIP1 & 4) << 3) + ((DIP1 & 8) << 1) +
                            ((DIP1 & 16) >> 1) + ((DIP1 & 32 ) >> 3);
              break;
            case 5:
              pia1.ora_in = ((DIP1 & 0x80) >> 1) + ((DIP1 & 0x40) << 1);
              break;
            case 6:
              pia1.ora_in = ((DIP2 & 1) << 7) + ((DIP2 & 2) << 5) +
                            ((DIP2 & 4) << 3) + ((DIP2 & 8) << 1) +
                            ((DIP2 & 16) >> 1) + ((DIP2 & 32 ) >> 3);
              break;
            case 7:
              pia1.ora_in = ((DIP2 & 0x80) >> 1) + ((DIP2 & 0x40) << 1);
              break;
          }
          pia1.ora_in += misc.zero;
          ret = pia1.read(addr & 3);
          break;
        case 0x9800:		// IC 4
          pia2.ora_in = decoder.enabled << 7;
          ret = pia2.read(addr & 3);
          if ( addr == 0x9802 )
            ret = 1;
          break;
        case 0xa000:		// IC 5
          pia3.orb_in = (pia3.orb & 0x80 ) ? reel.optos : 0;
          ret = pia3.read(addr & 3);
          break;
        case 0xa800:		// IC 6
          pia4.ora_in = matrix[4];
          pia4.orb_in = matrix[5];
          ret = pia4.read(addr & 3);
          break;
        default:
          if ( parent->errors )
            parent->state = MS_EXECUTE_BP ;
          break;
	}
  } else if ( addr == CharAddr )
     ret = memory[0x8001 + (x.w.l & 0x7fff)];
    else
     ret = memory[0x8000 + (addr & 0x7fff)];

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


void MPU3::write(Word addr, Byte x)
{

  if ( addr < 0x800 || ( addr >= 0x8000 && addr < 0x8800 ))
    memory[addr & 0x7ff] = x;
  else if ( (addr & 0xc000) == 0x8000 ) {
	switch ( addr & 0xf800 ) {
	  case 0x8800:			// IC2
        ptm.write((addr & 0x1f)>>2, x);
        if ( ptm.lat1_changed || ptm.t1_changed ) {
          misc.SetPeriod(0, ptm.lat1, ptm.cr1, ptm.lat1_changed, ptm.t1_changed, cycles );
          ptm.lat1_changed = false;
          ptm.t1_changed = false;
        }
        if ( ptm.lat2_changed || ptm.t2_changed ) {
          misc.SetPeriod(1, ptm.lat2, ptm.cr2, ptm.lat2_changed, ptm.t2_changed, cycles);
          ptm.lat2_changed = false;
          ptm.t2_changed = false;
        }
        if ( ptm.lat3_changed || ptm.t3_changed ) {
          misc.SetPeriod(2, ptm.lat3, ptm.cr3, ptm.lat3_changed, ptm.t3_changed, cycles);
          ptm.lat3_changed = false;
          ptm.t3_changed = false;
        }
		break;
	  case 0x9000:			// IC3 pa switches pb triacs
		pia1.write(addr & 3, x);
        if ( pia1.cb2_changed )
          printf("It's changed\n");
        if ( pia1.orb_changed) {
          if ( MonitorActive )
            IOMonitor->UpdateTriacs(pia1.orb + ( pia2.cb2 << 8) + ( pia3.cb2 << 9));
          triac.write(pia1.orb + ( pia2.cb2 << 8) + ( pia3.cb2 << 9));
          if ( triac.changed )
            parent->do_triac();
        }
		if ( pia1.ca2_changed ) {
          decoder.write_A0(pia1.ca2);
          if ( decoder.column == 0 ) {
            parent->do_lamp();
            parent->do_led_display();
          }
          if ( MonitorActive )
            IOMonitor->UpdateLampSelects(decoder.column);
        }
		break;
      case 0x9800:			// IC4 pa 7seg  pb lamps
        pia2.write(addr & 3, x);
        if ( pia2.ora_changed ) {
          switch ( MPU3_AUX ) {
            case 0:
              seg7.write_data_mpu3(pia2.ora, decoder.column);
              break;
            case 1:
              if ( MonitorActive )
                IOMonitor->UpdateMeters(pia2.ora);
              meter.write((pia2.ora ^ 0xff) & 0x3f);
              parent->MeterSensed(&meter, pia2.ora ^ 0xff);
              break;
            case 2:
              seg7.write_data_bwb(pia2.ora, decoder.column);
              break;
          }
        }
		if ( pia2.orb_changed ) {
          if ( MonitorActive )
            IOMonitor->UpdateLampDrives1(pia2.orb);
          lamp.write(pia2.orb,decoder.column,0, 18);
        }
		if ( pia2.ca2_changed ) {
          decoder.write_A1(pia2.ca2);
          if ( decoder.column == 0 ) {
            parent->do_lamp();
            parent->do_led_display();
          }
          if ( MonitorActive )
            IOMonitor->UpdateLampSelects(decoder.column);
        }
        if ( pia2.cb2_changed ) {
          if ( MonitorActive )
            IOMonitor->UpdateTriacs(pia1.orb + ( pia2.cb2 << 8) + ( pia3.cb2 << 9));
          triac.write(pia1.orb + ( pia2.cb2 << 8) + ( pia3.cb2 << 9));
          if ( triac.changed ) {
            parent->do_triac();
          }
        }
		break;
	  case 0xa000:			// IC5 pa steppers pb optos
		pia3.write(addr & 3, x);
        if ( pia3.ora_changed ) {
          if ( MonitorActive )
            IOMonitor->UpdateReel1(pia3.ora, 0, 0);
          reel.write(pia3.ora);
          if ( reel.changed[0] || reel.changed[1] ||
               reel.changed[2] || reel.changed[3] )
            parent->do_reel();
        }
        if ( pia3.orb_changed )
          ;
        if ( pia3.ca2_changed ) {
          decoder.write_A2(pia3.ca2);
          if ( decoder.column == 0 ) {
            parent->do_lamp();
            parent->do_led_display();
          }
          if ( MonitorActive )
            IOMonitor->UpdateLampSelects(decoder.column);
        }
        if ( pia3.cb2_changed ) {
          if ( MonitorActive )
            IOMonitor->UpdateTriacs(pia1.orb + ( pia2.cb2 << 8) + ( pia3.cb2 << 9));
          triac.write(pia1.orb + ( pia2.cb2 << 8) + ( pia3.cb2 << 9));
          if ( triac.changed ) {
            parent->do_triac();
          }
        }
        break;
	  case 0xa800:			// IC6 pa aux1 pb aux2
		pia4.write(addr & 3, x);
        if ( pia4.ora_changed ) {
          display.write(pia4.ora);
          if ( display.display_changed || display.intensity_changed )
            parent->do_display();
          }
//        if ( pia4.orb_changed )
//          printf("ORB changed to %d",pia4.orb);
		break;
      case 0xb000:
        break;
	  default:
        if ( parent->errors )
          parent->state = MS_EXECUTE_BP;
		break;
	}
  }
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

void __fastcall TMPU3::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 1);
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 64);
}

//---------------------------------------------------------------------------

void __fastcall TMPU3::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TMPU3::UpdateTriac(void)
{
  UpdateSenseTriacs(&board.triac);
}
//---------------------------------------------------------------------------
int __fastcall TMPU3::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TMPU3::getmemory(int address)
{
  if ( address >= 0 && address < 0x10000 ) {
    if ( address >= 0x800 ) {
      address &= 0x7fff;
      address += 0x8000;
    }
    return board.memory[address];
  } else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TMPU3::setmemory(int address, Byte value)
{
  board.memory[address] = value;
}

//---------------------------------------------------------------------------
int __fastcall TMPU3::dasm(int pc, char *buffer)
{

  return board.dasm(pc,buffer);

}
//---------------------------------------------------------------------------
void __fastcall TMPU3::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x100; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------

