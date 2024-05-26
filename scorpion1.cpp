 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "interface.h"
#include "debug.h"
#include "iomonitor.h"
#include "scorpion1.h"
#include "shared.h"

extern int addrscram[16];
extern int datascram[8];
extern char buffer[];
extern Byte lookup[8][256];


//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TSCORPION1::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSCORPION1::TSCORPION1(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = BFM_SCORPION1;
    MainDebug->SetProcess(this,board_type);
    board.parent = this;
//    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::Run()
{
int cycles = 0;

  SetReels(&board.reel, NBR_REELS, 7);
  CreateAutoLamp();

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

void __fastcall TSCORPION1::Run2()
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

void __fastcall TSCORPION1::saveram(void)
{
  TSystem::saveram(board.ram,0x2000);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION1::load_intelhex(void)
{
FILE		*fp;
int		addr, count;
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
  Scramble();

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    fread( board.ram, 0x2000, 1, fp);
    fclose(fp);
  }

//  if ( !FileExists("rom.hex") ) {
//    fp = fopen("rom.hex", "wb");
//    fwrite(board.memory, addr, 1, fp);
//    fclose(fp);
//  }
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION1::hard_reset(void)
{
  board.refresh = 0;
  board.reset();
  board.meter.reset();
  board.display.reset();
  board.gi.reset();
  board.gi.AY8910_set_clock(1000000);
  board.acia.reset();
  board.hoppr1.reset();
  board.hoppr2.reset();
  board.irq = false;
  board.page = 3;
  board.cycles = 0;
  board.busy = 0;
  board.locked = 7;
}

//---------------------------------------------------------------------------
int SCORPION1::execute(void)
{
	int cycles = 0, ret;

    refresh++;
    if ( refresh == 100000 )
      refresh = 0;
    switch ( parent->state ) {
      case MS_RUNNING:
        parent->Stopped = false;
        if ( (refresh % 1000) == 0 ) {
//          if ( tune != 0 ) {
//            ret = 1 - Sound->Playing(tune);
//            if ( ret == 1 ) {
//              gi.mute(0);
//            }
//          }
          if ( wdog ) {
            wdog--;
            if ( !wdog ) {
              parent->state = MS_RESET;
              break;
            }
          }
          if ( lamp.update() )
            parent->do_lamp();
          if ( acia.tx_char ) {
            acia.char_received();
            acia.tx_char = false;
          }
          if ( meter.update() )
            parent->do_meter();
          display.update();
          if ( display.display_changed )
            parent->do_display();
        }
        if ( parent->reeltimer ) {
          parent->reeltimer--;
          if ( !parent->reeltimer )
            parent->do_reel();
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        parent->trace[parent->sts].page = page;
        cycles = mc6809::execute();
        parent->Total_cycles += cycles;
        fiftyHz += cycles;
        acia.tick(cycles);
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
        if ( fiftyHz >= 2000 ) {  // 2000
          fiftyHz -= 2000;
          irq = true;
        }
        if ( parent->state == MS_READ_BP )
          break;
	    if ( irq ) {
          irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
	    if ( firq ) {
          irq_state[M6809_FIRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
          irq_state[M6809_FIRQ_LINE] = M6809_CLEAR_LINE;
          firq = false;
        }
        if ( pc.w.l == parent->bp && page == parent->page_bp )
          parent->state = MS_EXECUTE_BP;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        parent->trace[parent->sts].page = page;
	    cycles = mc6809::execute();
        parent->Total_cycles += cycles;
        fiftyHz += cycles;
        acia.tick(cycles);
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
        if ( fiftyHz >= 2000 ) {
          fiftyHz -= 2000;
          irq = true;
        }
	    if ( irq ) {
          irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
	    if ( firq ) {
          irq_state[M6809_FIRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
          irq_state[M6809_FIRQ_LINE] = M6809_CLEAR_LINE;
          firq = false;
        }
      case 2:
      case 3:
      case 4:
        parent->do_debug();
        MainDebug->Halt();
        parent->state = MS_HALTED;
        break;
      case 5:
        parent->hard_reset();
        parent->state = MS_RUNNING;
        break;
      case 6:
        MainDebug->RunClick(NULL);
        break;
      default:
        parent->Stopped = true;
        break;
    }
    if ( (refresh % 1000) == 0 && !NoSound )
      UpdateSoundBufferscorp1(0);

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION1::UpdateTrace(void)
{
char buffer[200];
int count;
int addr;
Word pos;
Byte old_page;
char args[200];

  old_page = board.page;
  MainDebug->Code->Items->Clear();
  pos = (sts + 1) % TRACE_SIZE;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
    board.page = trace[pos].page;
    board.dasm( addr, board.page, buffer);
    strncat( buffer, "                                   ", 35 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X %02X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s, trace[pos].page);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }

  board.page = old_page;

}
//---------------------------------------------------------------------------

Byte SCORPION1::read(Word addr)
{
Byte ret = 0;

  if ( addr < 0x2000 )
    ret = ram[addr];
  else if ( addr >= 0x6000 && addr < 0x8000 )
    ret = memory[(addr&0x1fff) | ((page & 3) << 13)];
  else if ( addr >= 0x4000 )
    ret = memory[addr];
  else {
	switch ( addr ) {
      case 0x2600:
        ret = meter.state;
        break;
      case 0x3406: // Uart
        ret = acia.read(0);
        break;
      case 0x3407: // Uart
        ret = acia.read(1);
        break;
      case 0x2a00: // Mux Inputs
        switch ( mux1 & 0x7 ) {
          case 5: // %
            ret = (matrix[5] & 0xf0) | PCkey;
            break;
          case 6:
            ret = DIP2;
            break;
          case 7:
            ret = DIP1;
            break;
          default:
            ret = matrix[mux1 & 0x7];
            break;
        }
        break;
      case 0x3408: // reel optos
        ret = ~reel.optos;
        break;
      case 0x2e00:
        ret = 3;
        break;
      case 0x3801: // NEC
        if ( tune != 0 ) {
          ret = 1 - Sound->Playing(tune);
        } else
          ret = 1;
        if ( ret == 1 )
          busy = false;
        if ( HasSampledSound )
          ret |= 2;
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

void SCORPION1::write(Word addr, Byte x)
{
  if ( addr < 0x2000 ) {
    ram[addr] = x;
  } else if ( addr >= 0x8000 ) {
    irq_state[M6809_IRQ_LINE] = M6809_CLEAR_LINE;
    irq = false;
    wdog = 500;
  } else {
    switch ( addr ) {
      case 0x3600: // page latch
        page = x;
        break;
      case 0x3001:
        gi.write_data(x, 1, 1, 0);
        break;
      case 0x3201:
        gi.write_data(x, 1, 1, 1);
        break;
      case 0x2a00:
        mux1 = x;
        if ( (mux1 & 8) == 8 )
          parent->do_lamp();
        break;
      case 0x2a01: // Mux1 Lo
        if ( mux1 & 8 )
          if ( lamp.write( x, (mux1 & 0x7) << 1, 0, 10) )
            ;//parent->do_lamp();
        break;
      case 0x2a02: // Mux1 Hi
        if ( mux1 & 8 )
          if ( lamp.write( x, ( mux2 & 0x7) << 1, 128, 10) )
            ;//parent->do_lamp();
        break;
      case 0x3406:
        acia.write(0, x);
        break;
      case 0x3407:
        acia.write(1, x);
        break;
      case 0x3408:
        mux2 = x;
        break;
      case 0x3409: // Mux2 Lo
        if ( mux2 & 8 )
          if ( lamp.write( x, (mux1 & 0x7) << 1, 8, 10) )
            ;//parent->do_lamp();
        break;
      case 0x340a: // Mux2 Hi
        if ( mux2 & 8 )
          if ( lamp.write( x, ( mux2 & 0x7) << 1, 136, 10) )
            ;//parent->do_lamp();
        break;
      case 0x2200: // reels12
        if ( locked & 1 ) {
          if ( x == 0x46 )
            locked &= ~0x01;
        } else {
          reel.writexr(x);
          if ( reel.changed[0] || reel.changed[1] )
            if ( !parent->reeltimer )
              parent->reeltimer = 200;
//            parent->do_reel();
          if ( MonitorActive )
            IOMonitor->UpdateReel1(x, reel.reels[0].pos, reel.reels[1].pos);
        }
        break;
      case 0x2000: // reels34
        if ( locked & 2 ) {
          if ( x == 0x42 )
            locked &= ~0x02;
        } else {
          reel.writeyr(x);
          if ( reel.changed[2] || reel.changed[3] )
            if ( !parent->reeltimer )
              parent->reeltimer = 200;
//            parent->do_reel();
          if ( MonitorActive )
            IOMonitor->UpdateReel2(x, reel.reels[2].pos, reel.reels[3].pos);
        }
        break;
      case 0x3800: // reels56 & NEC
        reel.writezr(x);
        if ( reel.changed[4] || reel.changed[5] )
            if ( !parent->reeltimer )
              parent->reeltimer = 200;
//          parent->do_reel();
        break;
      case 0x3900: // reels56
        reel.writezr(x);
        if ( reel.changed[4] || reel.changed[5] )
            if ( !parent->reeltimer )
              parent->reeltimer = 200;
//          parent->do_reel();
        break;
      case 0x3801: // NEC
        if ( (x & 0xc0) == 0x80 ) {
          if ( (x & 0x3f) == 0x3f ) {
            tune = 0;
            gi.reset();
            parent->do_wave();
            busy = false;
          } else {
            tune = lookup[0][x & 0x3f];
            busy = true;
            parent->do_wave();
          }
        }
        break;
      case 0x2600: // Meters
        if ( locked & 4 ) {
            locked &= ~0x04;
        } else {
          if ( x ) {
            firq = true;
          }
          meter.write(x);
        }
        break;
      case 0x2400: // Alpha data
        display.write_all(x & 0x20, x & 0x80, x & 0x40);
        if ( display.display_changed )
          parent->do_display();
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

void __fastcall TSCORPION1::UpdateDisplay(void)
{
  UpdateDisplays(&board.display,0);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION1::UpdateWave(void)
{
  UpdateWaves(board.tune,0);
}

//---------------------------------------------------------------------------
void __fastcall TSCORPION1::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION1::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, NBR_LAMPS);
}

//---------------------------------------------------------------------------

void __fastcall TSCORPION1::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION1::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

bool __fastcall TSCORPION1::AutoLamp(int i, int &reel, int &symbol)
{
  if ( (i / 16) > 1 && ( i / 16) < 4 && ( i % 16 ) < 6 ) {
    reel = ((i / 16) * 2) - 4 + (( i % 16) / 3);
    symbol = 2 - ((i % 16) % 3);
    if ( symbol < 3 )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION1::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION1::getmemory(int address)
{
  if ( address >= 0x2000 )
    return board.memory[address];
  else
    return board.ram[address];
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::setmemory(int address, Byte value)
{
  if ( address < 0x2000 )
    board.ram[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION1::dasm(int pc, char *buffer)
{
  return board.dasm(pc,board.page,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x2000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::UpdateIOMeters(void)
{
//  IOMonitor->UpdateMeters(board.pia5.orb);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.ram);
  savestate(ptr,board.page);
  savestate(ptr,board.refresh);
  savestate(ptr,board.fiftyHz);
  savestate(ptr,board.irq);
  savestate(ptr,board.firq);
  board.SaveToStream(ptr);
  board.display.SaveToStream(ptr);
  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.ram);
  readstate(ptr,board.page);
  readstate(ptr,board.refresh);
  readstate(ptr,board.fiftyHz);
  readstate(ptr,board.irq);
  readstate(ptr,board.firq);
  board.LoadFromStream(ptr);
  board.display.LoadFromStream(ptr);
  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION1::Scramble()
{
int transaddr, transdata;
Byte * tmp = new Byte[0x10000];
Byte * ptr = board.memory;

  for ( int i = 0; i < 0x10000; i++ ) {
    transaddr = TranslateAddr(i);
    transdata = TranslateData(*ptr);
    tmp[transaddr] = transdata;
    ptr++;
  }
  memcpy( board.memory, tmp, 0x10000);
  delete tmp;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION1::TranslateAddr(int addr)
{
int mask = 1;
int result = 0;

    for ( int i = 0; i < 16; i++ ) {
      if ( addr & mask )
        result += addrscram[i];
      mask <<= 1;
    }
    return result;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION1::TranslateData(int addr)
{
int mask = 1;
int result = 0;

    for ( int i = 0; i < 8; i++ ) {
      if ( addr & mask )
        result += datascram[i];
      mask <<= 1;
    }
    return result;
}
//---------------------------------------------------------------------------

