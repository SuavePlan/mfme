 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "interface.h"
#include "debug.h"
#include "iomonitor.h"
#include "scorpion2.h"
#include "shared.h"

/*
necrst=0x2b00
nec_data=0x2a00
unlckcs=0x2c00
mech_met=0x2333
watdog=0x2332
watcop=0x2329
muxstat=0x232e
platch=0x2e00
status=0x2000
reels12=0x2000
reels34=0x2100
reels56=0x2200
aciast=0x2400
aciadt=0x2500
volovr=0x2337
muxdata=0x2300
yamreg=0x2d00
yamdat=0x2d01
paylat=0x2330
payscs=0x2338
triacen=0x2331
*/

int addrscram[16] = {
 0x800,
 0x1000,
 0x1,
 0x4,
 0x8,
 0x20,
 0x80,
 0x200,
 0x100,
 0x40,
 0x2,
 0x10,
 0x400,
 0x2000,
 0x4000,
 0x8000
};

int datascram[8] = {
  0x2,
  0x8,
  0x20,
  0x40,
  0x10,
  0x4,
  0x1,
  0x80
};

static int charset[64] = {
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,  // 10
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,  // 20
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,  // 30
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,  // 40
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00,
0x3f,  // 48
0x06,
0x5b,
0x4f,
0x66,
0x6d,
0x7d,
0x07,
0x7f,
0x6f,
0x00,
0x00,
0x00,
0x00,
0x00,
0x00
};

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
//      void __fastcall TSCORPION2::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSCORPION2::TSCORPION2(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = BFM_SCORPION2;
    MainDebug->SetProcess(this,board_type);
    board.parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::Run()
{
int cycles = 0;

  SetReels(&board.reel, NBR_REELS, 5);
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

void __fastcall TSCORPION2::Run2()
{
int cycles = 0;

  SetReels(&board.reel, NBR_REELS, 5);
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

void __fastcall TSCORPION2::saveram(void)
{
  TSystem::saveram(board.ram,0x2000);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::load_intelhex(void)
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
//    fwrite(rom, total_size, 1, fp);
//    fclose(fp);
//  }
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::hard_reset(void)
{
  board.refresh = 0;
  board.reset();
  board.meter.reset();
  board.display.reset();
  board.leddisplay.reset();
  board.hoppr1.reset();
  board.hoppr2.reset();
  board.yamaha.reset();
  board.irq = false;
  if (Sound)
    Sound->ymz280b_reset();

  board.cycles = 0;
}

//---------------------------------------------------------------------------
int SCORPION2::execute(void)
{
	int cycles = 0;

    refresh++;
    if ( refresh == 100000 )
      refresh = 0;
    switch ( parent->state ) {
      case MS_RUNNING:
        parent->Stopped = false;
        if ( (refresh % 1000) == 0 ) {
          if ( wdog ) {
            wdog--;
            if ( !wdog ) {
              parent->state = MS_RESET;
              break;
            }
          }
          if ( lamp.update() )
            parent->do_lamp();
          if ( seg7.update() )
            parent->do_led_display();
          if ( meter.update() )
            parent->do_meter();
//          sample.update();
//          serial.update();
//          hoppr1.update();
//          hoppr2.update();
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
//        if ( s.w.l > 0x2000 )
//          parent->state = MS_EXECUTE_BP;
        parent->Total_cycles += cycles;
        fiftyHz += cycles;
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
        if ( fiftyHz >= 2500 ) {
          fiftyHz -= 2500;
          irq = true;
        }
        if ( parent->state == MS_READ_BP )
          break;
//      if (HasSampledSound) {
//        irq = irq ? irq : Sound->ymz280b_checkirq() == 0 ? false : true;
//      }
	if ( firq ) {
          irq_state[M6809_FIRQ_LINE] = M6809_ASSERT_LINE;
          interrupt();
          irq_state[M6809_FIRQ_LINE] = M6809_CLEAR_LINE;
          firq = false;
        } else if ( irq ) {
//          sprintf(buffer, "%d irq", parent->Total_cycles);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
          irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
        if ( pc.w.l == parent->bp )
          parent->state = MS_EXECUTE_BP;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        parent->trace[parent->sts].page = page;
	    cycles = mc6809::execute();
        parent->Total_cycles += cycles;
        fiftyHz += cycles;
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
        if ( fiftyHz >= 4000 ) {
          fiftyHz -= 4000;
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
      UpdateSoundBufferscorp2(0);

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::UpdateTrace(void)
{
char buffer[80];
int count;
int addr;
Word pos;
Byte old_page;
char args[50];

  old_page = board.page;
  MainDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
    board.page = trace[pos].page;
    board.dasm( addr, board.page, buffer);
    strncat( buffer, "                                   ", 35 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }

  board.page = old_page;

}
//---------------------------------------------------------------------------

Byte SCORPION2::read(Word addr)
{
Byte ret = 0;
int value;

  if ( addr < 0x2000 )
    ret = ram[addr];
  else if ( addr >= 0x6000 && addr < 0x8000 )
    ret = memory[(addr&0x1fff) | ((page & 3) << 13)];
  else if ( addr >= 0x4000 )
    ret = memory[addr];
  else {
	switch ( addr ) {
      case 0x2000:
        if ( tune != 0 & HasSampledSound ) {
          value = Sound->Playing(tune);
          if ( value == 0 ) {
            tune = 0;
            busy = false;
          }
        } else
          value = 0;
        if ( value == 0 )
          busy = false;
        ret = (reel.optos & 0x3f) + (value << 7) + parent->LatchState();
//        if ( ret & 0x40 ) {
//          sprintf(buffer, "%d DM busy", parent->Total_cycles);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//        }
        break;
      case 0x2329: // watcop
        ret = 0;
        break;
      case 0x2400:
//        sprintf(buffer, "aciast1 %04x", pc.w.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        ret = 2;
        break;
      case 0x2500:
//        sprintf(buffer, "aciast2 %04x", pc.w.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        ret = 2;
        break;
      case 0x232e: // muxstat
//        sprintf(buffer, "muxstat %04x", pc.w.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        ret = 0x81;
//        ret = 0x1;
        break;
      case 0x2300:
        ret = (matrix[addr-0x2300] & 0x1f) + (matrix[addr-0x2300+8] << 5);
        break;
      case 0x2301: //  Test & DIP1
        ret = (matrix[addr-0x2300] & 0x1f) + ((matrix[addr-0x2300+8] & 1) << 5) +
              ((DIP1 & 0x6) << 5);
        break;
      case 0x2302: // DIP 1
        ret = (matrix[addr-0x2300] & 0x1f) + (DIP1 & 0xe0);
        break;
      case 0x2303: // DIP 2
        ret = (matrix[addr-0x2300] & 0x1f) + ((DIP2 & 0x38) << 2);
        break;
      case 0x2304:
        ret = (matrix[addr-0x2300] & 0x1f) + ((matrix[addr-0x2300+4] & 0x18) << 2);
        break;
      case 0x2305: // DIP 1
        ret = (matrix[addr-0x2300] & 0x1f) + ((DIP1 & 0x18) << 2);
        break;
      case 0x2306: // DIP 2
        ret = (matrix[addr-0x2300] & 0x1f) + ((DIP2 & 0x6) << 4);
        break;
      case 0x2307: // DIP 2
        ret = (matrix[addr-0x2300] & 0x1f) + ((DIP2 & 0xc0) >> 1);
        break;
      case 0x30f0:
        if (addr & 0x0f)
          ret = Sound->ymz280b_readstatus();
        else
          ret = Sound->ymz280b_readdata();
        break;
      case 0x3c00: // Microphone
        ret = 0;
        break;
      case 0x3fff: // Coins
        ret = coin;
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

void SCORPION2::write(Word addr, Byte x)
{
  if ( addr < 0x2000 ) {
    ram[addr] = x;
  } else if ( addr >= 0x8000 ) {
    irq_state[M6809_IRQ_LINE] = M6809_CLEAR_LINE;
    irq = false;
    wdog = 500;
  } else
    switch ( addr & 0xfff0 ) {
      case 0x2e00: // page latch
        page = x;
        break;
      case 0x2b00: // nec reset
        if ( x == 0 ) {
          tune = 0;
          parent->do_wave();
        }
        break;
      case 0x2a00: // nec data
        if ( tune != 0 ) {
          if ( !Sound->Playing(tune) ) {
            tune = 0;
            busy = false;
          }
        } else
          busy = false;
        if ( !busy ) {
          if ( x & 0x80 )
            sound_page |= 1;
          else
            sound_page &= ~1;
          tune = lookup[sound_page][x & 0x7f];
          busy = true;
          parent->do_wave();
        }
        break;
      case 0x2c00: // unlock cs
        break;
      case 0x2300:
      case 0x2310:
        if ( (spcs & 1) == 0 )
          if ( lamp.write(x,addr & 0xf,128 * (addr >= 0x2310), 24) )
            parent->do_lamp();
        break;
      case 0x2320:
        switch ( addr & 0xff ) {
          case 0x20:
          case 0x21:
          case 0x22:
          case 0x23:
            break;
          case 0x24: // spcs
            spcs = x;
            sound_page = x & 2;
            break;
          case 0x28: // ??
            break;
          case 0x2c: // ??
            break;
          case 0x2f: // Coin Lockouts
            break;
          default:
            if ( parent->errors )
              parent->state = MS_EXECUTE_BP;
	        break;
        }
        break;
      case 0x2330:
        switch ( addr & 0xff ) {
          case 0x30: // paylat
            break;
          case 0x31: // triacen
            break;
          case 0x32: // watch dog
            break;
          case 0x33: // mech met
            if ( x ) {
              firq = true;
            }
            meter.write(x);
            break;
          case 0x36: // ??
            break;
          case 0x37: // volovr
            break;
          case 0x38: // payscs
            break;
          case 0x39: // ??
            break;
          default:
            if ( parent->errors )
              parent->state = MS_EXECUTE_BP;
	        break;
        }
        break;
      case 0x2000: // reels12
        reel.writex(x);
        if ( reel.changed[0] || reel.changed[1] )
          if ( !parent->reeltimer )
            parent->reeltimer = 200;
//          parent->do_reel();
        if ( MonitorActive )
          IOMonitor->UpdateReel1(x, reel.reels[0].pos, reel.reels[1].pos);
        break;
      case 0x2100: // reels34
        reel.writey(x);
        if ( reel.changed[2] || reel.changed[3] )
          if ( !parent->reeltimer )
            parent->reeltimer = 200;
//          parent->do_reel();
        if ( MonitorActive )
          IOMonitor->UpdateReel2(x, reel.reels[2].pos, reel.reels[3].pos);
        break;
      case 0x2200: // reels56
        reel.writez(x);
        if ( reel.changed[4] || reel.changed[5] )
          if ( !parent->reeltimer )
            parent->reeltimer = 200;
//          parent->do_reel();
        break;
      case 0x2400: // aciadt
        break;
      case 0x2500: // aciadt
        break;
      case 0x2d00: // yam reg
        if ( addr & 1 )
          yamaha.write_data(x);
        else
          yamaha.write_reg(x);
        break;
      case 0x2600: // ??
        break;
      case 0x2800: // Alpha data
        Form1->SendChar(x);
        display.do_char(x);
        if ( display.display_changed )
            parent->do_display();
        break;
      case 0x2900: // Alpha reset
        display.enable(x & 1);
        leddisplay.enable(x & 1);
        if ( display.display_changed )
            parent->do_display();
        if ( leddisplay.display_changed )
          Write7Seg();
        if ( !x )
          Form1->ResetMatrix();
        break;
      case 0x30f0:
        if (addr & 0x0f)
          Sound->ymz280b_writedata(x);
        else
          Sound->ymz280b_writereg(x);
      case 0x3c00: // Microphone
        break;
      case 0x4000:
      case 0x4010:
      case 0x4020:
        break;
      case 0xf000: // ??
        break;
      case 0x2f00: // Extender
        leddisplay.do_char(x);
        if ( leddisplay.display_changed )
          Write7Seg();
        break;
      default:
        if ( parent->errors )
          parent->state = MS_EXECUTE_BP;
	    break;
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

void __fastcall TSCORPION2::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 0);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::UpdateWave(void)
{
  UpdateWaves( board.tune, 0);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, NBR_LAMPS);
  UpdateLedDisplays(&board.seg7);
}

//---------------------------------------------------------------------------

void __fastcall TSCORPION2::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TSCORPION2::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION2::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION2::getmemory(int address)
{
  if ( address >= 0x2000 )
    return board.memory[address];
  else
    return board.ram[address];
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::setmemory(int address, Byte value)
{
  if ( address < 0x2000 )
    board.ram[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TSCORPION2::dasm(int pc, char *buffer)
{
  return board.dasm(pc,board.page,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x2000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::UpdateIOMeters(void)
{
//  IOMonitor->UpdateMeters(board.pia5.orb);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.ram);
  savestate(ptr,board.page);
  savestate(ptr,board.refresh);
  savestate(ptr,board.fiftyHz);
  savestate(ptr,board.spcs);
  savestate(ptr,board.irq);
  savestate(ptr,board.firq);
  board.SaveToStream(ptr);
  board.display.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.ram);
  readstate(ptr,board.page);
  readstate(ptr,board.refresh);
  readstate(ptr,board.fiftyHz);
  readstate(ptr,board.spcs);
  readstate(ptr,board.irq);
  readstate(ptr,board.firq);
  board.LoadFromStream(ptr);
  board.display.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TSCORPION2::Scramble()
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
int __fastcall TSCORPION2::TranslateAddr(int addr)
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
int __fastcall TSCORPION2::TranslateData(int addr)
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

void __fastcall SCORPION2::Write7Seg(void)
{
  Byte value;
  bool update = false;

  for ( int i = 0; i < 16; i++ ) {
    if ( leddisplay.chs[i] & 0x2000000 ) {
      int chs = leddisplay.chs[i] & 0xffff;
      value = (( chs & 1 ) << 5 )  | ( chs & 2 ) | (( chs & 4 ) >> 2) | (( chs & 32 ) >> 3 ) |
              (( chs & 0x200 ) >> 6) | (( chs & 0x2000 ) >> 9 ) | (( chs & 0x8000 ) >> 9 ) |
              (( chs & 0x1000 ) >> 5 );
    } else if ( leddisplay.chs[i] & 0xff0000 )
      value = charset[leddisplay.chs[i] & 0x3f] | 0x80;
    else
      value = charset[leddisplay.chs[i] & 0x3f];
    if ( seg7.write_static(value, i) )
      update = true;
  }
  if ( update )
    parent->do_led_display();
}
