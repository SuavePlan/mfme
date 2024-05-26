 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "interface.h"
#include "configuration.h"
#include "debug.h"
#include "iomonitor.h"
#include "m1a.h"
#include "shared.h"

extern Byte lookup[8][256];
extern char buffer[];

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TM1A::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TM1A::TM1A(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
  board_type = MAYGAY_M1A;
  MainDebug->SetProcess(this,board_type);
  board.parent = this;
//  Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TM1A::Run()
{
int cycles = 0;

  SetReels(&board.reel, NBR_REELS, 7);
  CreateAutoLamp();

  while ( !Terminated ) {
    board.execute();
    cycles += 2;
    if ( cycles > Speed ) {
      Sleep(1);
//      do_bitmap();
      cycles = 0;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TM1A::Run2()
{
int cycles = 0;

  SetReels(&board.reel, NBR_REELS, 7);
  CreateAutoLamp();

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

void __fastcall TM1A::saveram(void)
{
  TSystem::saveram(board.memory, 0x2000);
}
//---------------------------------------------------------------------------

void __fastcall TM1A::load_intelhex(void)
{
FILE *fp;
int	addr, count;
struct stat statbuf;
String filename;
int size;

  addr = 0;
  size = 0;
  for ( count = 0; count < ROMList->Count; count++ ) {
	fp = fopen(ROMList->Strings[count].c_str(), "rb");
    if ( fp != NULL ) {
      stat( ROMList->Strings[count].c_str(), &statbuf);
      size += statbuf.st_size;
      fclose(fp);
    }
  }
  
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

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    fread( &board.memory[0], 0x2000, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TM1A::hard_reset(void)
{
  board.refresh = 0;
  board.reset();
  board.pia.reset();
  board.meter.reset();
  board.display.reset();
  board.duart.reset();
  board.yamaha.reset();
  board.gi.reset();
  board.hoppr.reset();
  board.ramen = 0;
  board.alarmen = 0;
  board.psurelay = 0;
  board.enable = 0;
  board.srsel = 0;
  board.wdog = 0;
  board.busy = false;
  board.oldbusy = false;
  board.sndcntl = 0;
  board.cycles = 0;
  board.vmmctr = 0;
  board.refresh = 0;
  board.firq = 0;
  board.nmi = 0;
  board.page = 0;
}

//---------------------------------------------------------------------------
int M1A::execute(void)
{
	int cycles;
	Byte	do_irq, do_firq, do_nmi;

    do_irq = do_firq = do_nmi = 0;
    refresh++;
    if ( refresh == 100000 ) {
      refresh = 0;
    }
    switch ( parent->state ) {
      case MS_RUNNING:
        parent->Stopped = false;
        if ( (refresh % 50) == 0 ) {
          if (M1SampType == 0) {
            if ( sample.update() )
              firq = true;
          } else {
            if ( tune != 0 ) {
              if ( !Sound->Playing(tune) ) {
                tune = 0;
                busy = false;
                firq = true;
              }
            } else
              busy = false;
          }
        }
//        if ( (refresh % 30000) == 0 ) {
//          parent->do_bitmap();
//        }
        if ( (refresh % 1000) == 0 ) {
          if ( wdog ) {
            wdog--;
            if ( !wdog ) {
              parent->hard_reset();
              break;
            }
          }
          seg7.update();
          hoppr.update();
//          if ( meter.update() )
//            parent->do_meter();
          if ( cointimer ) {
            cointimer--;
            if ( cointimer == 15 ) {
              matrix[0] &= ~coinTag;
            }
          }
        }
        if ( parent->reeltimer ) {
          parent->reeltimer--;
          if ( !parent->reeltimer )
            parent->do_reel();
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        if ( pc.w.l < 0x2000 )
          parent->trace[parent->sts].pc = pc.w.l;
        else if ( pc.w.l >= 0x2800 ) {
          if ( !srsel )
            parent->trace[parent->sts].pc = pc.w.l;
          else if ( pc.w.l >= 0xc400 + 0xd00 )
            parent->trace[parent->sts].pc = pc.w.l;
          else
            parent->trace[parent->sts].pc = pc.w.l + 0x10000;
        }
        cycles = mc6809::execute();
        parent->Total_cycles += cycles;
        vmmctr += cycles;
        if ( vmmctr > 1800 && Vmm) {
          vmmctr -= 1800;
          Vmm = false;
        } else if ( vmmctr > 18000 && !Vmm) {
          vmmctr -= 18000;
          Vmm = true;
          if ( enable )
            nmi = true;
        }
        duart.tick(cycles);
        if ( nmi ) {
          nmi = false;
          do_nmi = 1;
        } else
          irq_state[M6809_NMI_LINE] = M6809_CLEAR_LINE;
        if ( firq )
          do_firq = 1;
        else
          irq_state[M6809_FIRQ_LINE] = M6809_CLEAR_LINE;
        if ( duart.isr & duart.imr )
          do_irq = 1;
        else
          irq_state[M6809_IRQ_LINE] = M6809_CLEAR_LINE;
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
        if ( parent->state == MS_READ_BP )
          break;
	    if ( do_nmi ) {
          irq_state[M6809_NMI_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
	    if ( do_firq ) {
          irq_state[M6809_FIRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
	    if ( do_irq ) {
          irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
        if ( pc.w.l == parent->bp )
          parent->state = MS_READ_BP;
        break;
      case MS_STEPPING:
        parent->sts = ++parent->sts % TRACE_SIZE;
        if ( pc.w.l < 0x2000 )
          parent->trace[parent->sts].pc = pc.w.l;
        else if ( pc.w.l >= 0x2800 ) {
          if ( !srsel )
            parent->trace[parent->sts].pc = pc.w.l;
          else if ( pc.w.l >= 0xc400 + 0xd00 )
            parent->trace[parent->sts].pc = pc.w.l;
          else
            parent->trace[parent->sts].pc = pc.w.l + 0x10000;
        }
	    cycles = mc6809::execute();
        duart.tick(cycles);
        parent->Total_cycles += cycles;
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
	    if ( do_irq )
		  interrupt();
        if ( parent->in_int )
          break;
      case MS_READ_BP:
      case MS_WRITE_BP:
      case MS_EXECUTE_BP:
        parent->do_debug();
        MainDebug->Halt();
        parent->state = MS_HALTED;
        break;
      case MS_RESET:
        parent->hard_reset();
      case MS_RESUME:
        MainDebug->RunClick(NULL);
        break;
      default:
        parent->Stopped = true;
        break;
    }
    if ( (refresh % 1000) == 0 && !NoSound )
      UpdateSoundBufferm1a(0);

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TM1A::UpdateTrace(void)
{
char buffer[150];
int count;
int addr;
Word pos;
Byte old_page;
char args[50];

//  old_page = board.page;
  MainDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
//    board.page = (addr & 0x30000) >> 16;
//    addr &= 0xFFFF;
    board.dasm( addr, 0, buffer);
    strncat( buffer, "                                        ", 40 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }
//  board.page = old_page;

}
//---------------------------------------------------------------------------

Byte M1A::read(Word addr)
{
Byte		ret = 0;

  if ( addr < 0x2000 )
    ret = memory[addr];
  else if ( addr >= 0x2800 ) {
    if ( !srsel )
      ret = memory[addr];
//    else if ( addr >= 0xc400 + 0xd00 )
//      ret = memory[addr];
    else if ( addr >= 0xe000 )
      ret = memory[addr];
    else
      ret = memory[addr + 0x10000];
  } else {
	switch ( addr & 0xfff0 ) {
        case 0x2000: // RDS 0
          break;
        case 0x2010: // RDS 1
          break;
        case 0x2020: // RDS 2
          break;
        case 0x2030: // 8279
          if ( (Hoppers & 0x50) != 0x50 ) {
            if ( hoppr.opto() ) {
              if ( (Hoppers & 0x50) == 0x40 )
                matrix[3] &= ~0x80;
              else
                matrix[3] &= ~0x14;
            } else {
              if ( (Hoppers & 0x50) == 0x40 )
                matrix[3] |= 0x80;
              else
                matrix[3] |= 0x14;
            }
          }
          ret = io1.read(addr & 1, true);
          break;
        case 0x2040: // 8051
          ret = io2.read(addr & 1, false);
          break;
		case 0x2050: // Scan on M1B
          break;
        case 0x2060: // RTC
          break;
		case 0x2070: // 68681
          duart.ip = ~reel.optos;
          ret = duart.read(addr & 0xf);
          break;
        case 0x2080: // 68681 IntAck
          break;
        case 0x2090: // PSG
          break;
        case 0x20a0: // PIA
          ret = pia.read( addr & 0x3 );
          break;
        case 0x20b0: // Chk Meter
          ret = ~gi.porta;
          break;
        case 0x20c0: // Latch
          break;
        case 0x20d0:
          break;
        case 0x20e0:
          break;
        case 0x20f0:
          break;
        case 0x2400: // Clear FIRQ
        case 0x2410: // Clear FIRQ
          switch ( addr ) {
            case 0x2404: // st low
              sndcntl = sndcntl & ~1;
              sample.write_control( sndcntl );
              break;
            case 0x2406: // st high
              sndcntl = sndcntl | 1;
              sample.write_control( sndcntl );
              break;
            case 0x2408:
              if (M1SampType == 1 ) {
                tune = 0;
                busy = false;
                parent->do_wave();
              }
              break;
            case 0x240c:
              if (M1SampType == 1 )
                firq = false;
              break;
            case 0x2410:
              firq = false;
              break;
            case 0x240e:
              if ( M1SampType == 1 && !busy )
                firq = true;
              break;
            case 0x2412: // Assert FIRQ
              if ( sample.nar )
                firq = true;
              break;
          }
          break;
        default:
          if ( parent->errors )
            parent->state = MS_EXECUTE_BP;
        break;
	}
  }
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

void M1A::write(Word addr, Byte x)
{
    if ( addr < 0x2000 )
      memory[addr] = x;
    else {
	  switch ( addr & 0xfff0 ) {
        case 0x2000: // Reel 1 & 2
          reel.writex(x);
          if ( reel.changed[0] || reel.changed[1] ) {
            if ( MonitorActive )
              IOMonitor->UpdateReel1(0, reel.reels[0].pos, reel.reels[1].pos);
//            if ( !parent->reeltimer )
//              parent->reeltimer = 50;
            parent->do_reel();
          }
          break;
        case 0x2010: // Reel 3 & 4
          reel.writey(x);
          if ( reel.changed[2] || reel.changed[3] ) {
            if ( MonitorActive )
              IOMonitor->UpdateReel2(0, reel.reels[2].pos, reel.reels[3].pos);
//            if ( !parent->reeltimer )
//              parent->reeltimer = 50;
            parent->do_reel();
          }
          break;
        case 0x2020: // Reel 5 & 6
          reel.writez(x);
          if ( reel.changed[4] || reel.changed[5] ) {
            if ( MonitorActive )
              IOMonitor->UpdateReel3(0, reel.reels[4].pos, reel.reels[5].pos);
//            if ( !parent->reeltimer )
//              parent->reeltimer = 50;
            parent->do_reel();
          }
          break;
        case 0x2030: // 8279
          io1.write( addr & 1, x);
          if ( io1.lamps_changed ) {
            lamp.write_maygay(io1.display_ram[io1.changed], io1.changed, 0);
            parent->do_lamp();
          }
          break;
        case 0x2040: // 8051
          io2.write( addr & 1, x);
          if ( io2.lamps_changed ) {
            lamp.write_maygay(io2.display_ram[io2.changed], io2.changed, 128);
            parent->do_lamp();
          }
          break;
		case 0x2050: // Scan on M1B
          break;
        case 0x2060: // RTC
          break;
		case 0x2070: // 68681
          duart.write( addr & 0xf, x);
          if ( duart.op_changed & 0x30 ) {
            if ( (Hoppers & 0x50) == 0x40 )
              hoppr.enable(duart.op & 0x10, duart.op & 0x20, 0);
          }
          break;
        case 0x2080: // 68681 IntAck
          break;
        case 0x2090: // PSG
          gi.write_data(x, 1, 1, addr & 1);
          if ( gi.porta_changed ) {
            if ( (gi.porta_changed & 0x44) && ((Hoppers & 0x50) == 0x10) ) {
              hoppr.enable(gi.porta & 0x40, gi.porta & 0x4, 1);
            }
            if ( meter.write(gi.porta) )
              parent->do_meter();
          }
          break;
        case 0x20a0: // PIA
          pia.write( addr & 0x3, x);
          if ( pia.ora_changed & 0x70 ) {
            display.write_all(pia.ora & 0x40, pia.ora & 0x20, pia.ora & 0x10);
            if ( display.display_changed || display.intensity_changed )
              parent->do_display();
          }
          if ( pia.orb_changed ) {
            triac.write(pia.orb);
            if ( triac.changed )
              parent->do_triac();
          }
          break;
        case 0x20b0: // Chk Meter
          break;
        case 0x20c0: // Latch
          switch ( addr & 7 ) {
            case 0: // RAMEN
              ramen = x & 1;
              break;
            case 1: // AlarmEn
              alarmen = x & 1;
              break;
            case 2: // Enable
              if ( enable == 0 && ( x & 1 ) && Vmm )
                nmi = true;
              enable = x & 1;
              break;
            case 3: // RTS
              break;
            case 4: // PSURelay
              psurelay = x & 1;
              break;
            case 5: // WDog
              wdog = 50;
//              wdog = x & 1;
              break;
            case 6: // Srsel
              srsel = x & 1;
              break;
          }
          break;
        case 0x20d0:
          break;
        case 0x20e0:
          break;
        case 0x20f0:
          break;
        case 0x2400: // Clear FIRQ
        case 0x2410: // Clear FIRQ
        case 0x2420: // Clear FIRQ
          switch ( addr ) {
            case 0x2400:
              yamaha.write_reg(x);
              break;
            case 0x2401:
              yamaha.write_data(x);
              break;
            case 0x2404: // NEC Sample
            case 0x2406: // NEC Sample
              if (M1SampType == 1 ) {
                if ( addr == 0x2404 )
                  tune = lookup[0][x];
                else
                  tune = lookup[1][x];
                busy = true;
                parent->do_wave();
              }
              break;
            case 0x2420: // Sample and ch2
              sample.write_latch( x & 0x7f );
              sndcntl = (sndcntl & 1) + ((x & 0x80) >> 6);
              sample.write_control( sndcntl );
              break;
          }
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

void __fastcall TM1A::UpdateDisplay(void)
{
  UpdateDisplays(&board.display,1);
}
//---------------------------------------------------------------------------

void __fastcall TM1A::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TM1A::UpdateWave(void)
{
  if (M1SampType == 0 )
    Form1->play(board.sample.tune, board.sample.channel);
  else
    Form1->play2(board.tune, 0);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TM1A::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 256);
  UpdateLedDisplays(&board.seg7);
}

//---------------------------------------------------------------------------
bool __fastcall TM1A::AutoLamp(int i, int &reel, int &symbol)
{
  if ( ( i > 9) && (i < 30) ) {
    reel = (i<22) ? (i - 10) / 3 : (i>23) ? (i - 12) / 3 : 0;
    symbol = (i<22) ? 2 - ( (i - 10) % 3 ) : (i>23) ? 2 - ( (i - 24) % 3 ) : 4;
    if ( symbol < 3 )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------

void __fastcall TM1A::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TM1A::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

int __fastcall TM1A::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TM1A::getmemory(int address)
{
  if ( address >= 0 && address < 0x10000 )
  return board.memory[address];
  else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TM1A::setmemory(int address, Byte value)
{
  if ( address > 0x800 )
    address = address + 0x10000 * board.page;

  board.memory[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TM1A::dasm(int pc, char *buffer)
{
  return board.dasm(pc,0,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x800; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TM1A::UpdateIOMeters(void)
{
//  IOMonitor->UpdateMeters(board.pia5.orb);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::SaveToStream(Byte *&ptr)
{
  savestatesize(ptr,board.memory,0x2000);
  savestate(ptr,board.page);
  savestate(ptr,board.refresh);
  savestate(ptr,board.alarmen);
  savestate(ptr,board.enable);
  savestate(ptr,board.wdog);
  savestate(ptr,board.psurelay);
  savestate(ptr,board.srsel);
  savestate(ptr,board.ramen);
  savestate(ptr,board.firq);
  savestate(ptr,board.nmi);
  savestate(ptr,board.Vmm);
  savestate(ptr,board.sndcntl);
  savestate(ptr,board.vmmctr);
  board.SaveToStream(ptr);
  board.pia.SaveToStream(ptr);
  board.display.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
  board.duart.SaveToStream(ptr);
  board.hoppr.SaveToStream(ptr);
  board.gi.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TM1A::LoadFromStream(Byte *&ptr)
{
  readstatesize(ptr,board.memory,0x2000);
  readstate(ptr,board.page);
  readstate(ptr,board.refresh);
  readstate(ptr,board.alarmen);
  readstate(ptr,board.enable);
  readstate(ptr,board.wdog);
  readstate(ptr,board.psurelay);
  readstate(ptr,board.srsel);
  readstate(ptr,board.ramen);
  readstate(ptr,board.firq);
  readstate(ptr,board.nmi);
  readstate(ptr,board.Vmm);
  readstate(ptr,board.sndcntl);
  readstate(ptr,board.vmmctr);
  board.LoadFromStream(ptr);
  board.pia.LoadFromStream(ptr);
  board.display.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
  board.duart.LoadFromStream(ptr);
  board.hoppr.LoadFromStream(ptr);
  board.gi.LoadFromStream(ptr);
  do_reel();
  do_lamp();
  do_display();
  do_led_display();
}

