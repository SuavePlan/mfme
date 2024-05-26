 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
#include "debug.h"
#include "iomonitor.h"
#include "proconn.h"
#include "sample.h"
#include "shared.h"

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
//      void __fastcall TProconn::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------
__fastcall TProconn::TProconn(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = PROJECT_PROCONN;
    MainDebug->SetProcess(this,board_type);
    board.parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TProconn::Run()
{
int cycles = 0;

  SetReels(&board.reel, 4, 1);
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

void __fastcall TProconn::hard_reset(void)
{
  board.refresh = 0;
  board.Z80_Reset();
  board.gi.AY8910_reset();
  board.gi.AY8910_set_clock(1500000/*765000*/);
  board.ctc.reset();
  board.pio1.reset();
  board.pio2.reset();
  board.pio3.reset();
  board.pio4.reset();
  board.pio5.reset();
  board.meter.reset();
//  board.reel.reset();
  board.cycles = 0;
  board.depth = 0;
  board.tune = 0;
  board.chars = 0;
  board.busy = false;
}
//---------------------------------------------------------------------------
void __fastcall TProconn::saveram(void)
{
  TSystem::saveram(&board.memory[0xf000], 0x800);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::load_intelhex(void)
{
FILE		*fp;
int		addr, count;
struct stat statbuf;
String filename;
int size;

  addr = 0;
  size = 0;
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
    fread( &board.memory[0xf000], 0x800, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------
int Proconn::execute(void)
{
int cycles;

  refresh++;
  if ( refresh == 800000 ) {
    refresh = 0;
  }

  switch ( parent->state ) {
    case 0:
      parent->Stopped = false;

      if ( (refresh % 2000) == 0 ) {
        if ( seg7.update() )
          parent->do_led_display();
        if ( meter.update() )
          parent->do_meter();
      }
      if ((refresh % 1000) == 0) {
        if (lamp.update())
          parent->do_lamp();
      }

      parent->sts = ++parent->sts % TRACE_SIZE;
      parent->trace[parent->sts].pc = R.PC.W.l;
      cycles = z80cpu::Z80_Step();
      ctc.tick(cycles);
      parent->Total_cycles += cycles;
      parent->trace[parent->sts].a  = R.AF.B.l;
      parent->trace[parent->sts].b  = R.AF.B.h;
      parent->trace[parent->sts].x  = R.BC.W.l;
      parent->trace[parent->sts].y  = R.DE.W.l;
      parent->trace[parent->sts].u  = R.HL.W.l;
      parent->trace[parent->sts].s  = R.SP.W.l;
      if ( parent->state == 2 )
        break;
      if ( R.PC.W.l == parent->bp )
        parent->state = 2;
      break;
    case 1:
      parent->sts = ++parent->sts % TRACE_SIZE;
      parent->trace[parent->sts].pc = R.PC.W.l;
      cycles = z80cpu::Z80_Step();
      ctc.tick(cycles);
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
      parent->state = 10;
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

  // And update sound
  if ( (refresh % 1200) == 0 && !NoSound )
    UpdateSoundBufferproconn(0);

  return 0;
}
//---------------------------------------------------------------------------

Byte Proconn::Z80_In (word Port)
{
Byte RetV;
Byte optos;
int mask;
Byte value;

  Port = Port ^ 0xff;
  switch (Port & 0xf8) {
    case 0x80:
      mask = 1 << strobe;
      value = 0;
      for ( int i = 0; i < 5; i++ ) {
        if ( i == 0 ) {
          if ( DIP1 & mask )
            value |= 1;
        } else if ( matrix[i] & mask )
          value |= 1;
        value <<= 1;
      }
      pio1.in[0] = (~value) & 0x3f;
      if ( tune && !Sound->Playing(tune) ) {
        if ( (bank & 0x40) == 0 ) {
          busy = true;
          parent->do_wave();
        } else
          busy = false;
      }
      if ( !busy )
        pio1.in[0] |= 0x80;
      if (Port & 0x100)
        RetV = pio1.c_r((Port & 0x200) >> 9);
      else
        RetV = pio1.d_r((Port & 0x200) >> 9);
//      if ( (Port & 0x300) == 0x200 ) {
//        sprintf(buffer, "Read pio1 %02x @ %04x", RetV, R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//      }
      break;
    case 0x40:
      if ( Port & 0x100)
        RetV = pio2.c_r((Port & 0x200) >> 9);
      else
        RetV = pio2.d_r((Port & 0x200) >> 9);
//      if ( (Port & 0x100) == 0 ) {
//        sprintf(buffer, "Read pio2 %02x @ %04x", RetV, R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//      }
      break;
    case 0x20:
      pio3.in[1] = ~matrix[6]; // Coin Mech
      optos = reel.optos ^ 0xff;
      pio3.in[0] = ((optos & 1) << 4) | ((optos & 2) << 4) |
                   ((optos & 4) << 4) | ((optos & 8) << 4) | 8;
//      pio3.in[0] = ((reel.optos ^ 0xff) << 3);// ^ 0xff;
      if ( Port & 0x100)
        RetV = pio3.c_r((Port & 0x200) >> 9);
      else
        RetV = pio3.d_r((Port & 0x200) >> 9);
//      if ( (Port & 0x300) == 0 ) {
//        sprintf(buffer, "Opto Read %02x (Optos are %02x) @ %04x", RetV, pio3.in[0], R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//      }
      break;
    case 0x10:
      if ( Port & 0x100)
        RetV = pio4.c_r((Port & 0x200) >> 9);
      else
        RetV = pio4.d_r((Port & 0x200) >> 9);
//      if ( (Port & 0x100) == 0 ) {
//        sprintf(buffer, "Read pio4 (%02x) %02x @ %04x", Port & 0x200, RetV, R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//      }
      break;
    case 0x08:
      if ( Port & 0x100)
        RetV = pio5.c_r((Port & 0x200) >> 9);
      else
        RetV = pio5.d_r((Port & 0x200) >> 9);
      break;
    default:
      switch (Port & 0x07) {
        case 0x06:
          // VFD Reset
          break;
        case 0x04:
          // Sound
          RetV = gi.AY8910Read();
          break;
        case 0x01:
          RetV = ctc.r(((Port & 0x200) >> 9) | ((Port & 0x100) >> 7));
          break;
        case 0x00:
          // SIO
          RetV = 0x04;
          break;
        default:
          break;
      }
      break;
  }
  return RetV;
}
//---------------------------------------------------------------------------
void Proconn::Z80_Out (word Port,byte Value)
{
byte reelval;

  Port = Port ^ 0xff;
  switch (Port & 0xf8) {
    case 0x80:
      if (Port & 0x100)
        pio1.c_w((Port & 0x200) >> 9, Value);
      else
        pio1.d_w((Port & 0x200) >> 9, Value);
/*      if ( (Port & 0x300) == 0x200 ) {
        sprintf(buffer, "PIO1b Write %02x @ %04x", Value, R.PC.W.l);
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }   */
/*
      reelval = ((pio1.out[1] & 0x01) << 3) | ((pio1.out[1] & 0x02));
      reelval |= (reelval >> 1);
      reelval ^= 0x05;
      reel.write( reelval, 3 );
      reelval = ((pio1.out[1] & 0x04) << 1) | ((pio1.out[1] & 0x08) >> 2);
      reelval |= (reelval >> 1);
      reelval ^= 0x05;
      reel.write( reelval, 2 );
      reelval = ((pio1.out[1] & 0x10) >> 1) | ((pio1.out[1] & 0x20) >> 4);
      reelval |= (reelval >> 1);
      reelval ^= 0x05;
      reel.write( reelval, 1 );
      reelval = ((pio1.out[1] & 0x40) >> 3) | ((pio1.out[1] & 0x80) >> 6);
      reelval |= (reelval >> 1);
      reelval ^= 0x05;
      reel.write( reelval, 0 );
*/
//      reel.writeproconn(pio1.out[1]);
/*      reelval = (pio1.out[1] & 0x03);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 0);
      reelval = ((pio1.out[1] & 0x0c) >> 2);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 1);
      reelval = ((pio1.out[1] & 0x30) >> 4);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 2);
      reelval = ((pio1.out[1] & 0xc0) >> 6);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 3);*/

/*      if (reel.changed[0] || reel.changed[1] || reel.changed[2] || reel.changed[3])
        parent->do_reel();
      if (MonitorActive) {
        IOMonitor->UpdateReel2(reel.reels[2].pat | (reel.reels[3].pat << 4), reel.reels[0].pos, reel.reels[1].pos);
        IOMonitor->UpdateReel1(reel.reels[0].pat | (reel.reels[1].pat << 4), reel.reels[2].pos, reel.reels[3].pos); // bogus
        IOMonitor->UpdateAux1(pio2.out[0]);
        IOMonitor->UpdateAux2(pio2.out[1]);
      }  */
      break;
    case 0x40:
      if ( Port & 0x100)
        pio2.c_w((Port & 0x200) >> 9, Value);
      else
        pio2.d_w((Port & 0x200) >> 9, Value);
      if ( (Port & 0x300) == 0x200 ) {
        display.write_all(1, ~pio2.out[1] & 8, ~pio2.out[1] & 4);
        if ( display.display_changed || display.intensity_changed )
          parent->do_display();
//        sprintf(buffer, "PIO2b Write %02x @ %04x", Value, R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }

/*      if ( (Port & 0x300) == 0 ) {
        sprintf(buffer, "PIO2a Write %02x @ %04x", Value, R.PC.W.l);
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        sprintf(buffer, "Reel0 %02x Reel1 %02x Reel2 %02x Reel3 %02x",
               ((pio1.out[1] & 0x0c) >> 2) | (pio2.out[0] & 0x0c),
               ((pio1.out[1] & 0xc0) >> 6) | ((pio2.out[0] & 0xc0) >> 4),
               ((pio1.out[1] & 0x30) >> 4) | ((pio2.out[0] & 0x30) >> 2),
               (pio1.out[1] & 0x03) | ((pio2.out[0] & 0x03) << 2) );
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }   */
//      if ( (Port & 0x300) == 0x200 ) {
//        sprintf(buffer, "PIO2b Write %02x @ %04x", Value, R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//      }
      reelval = ((pio1.out[1] & 0xc0) >> 6) | ((pio2.out[0] & 0xc0) >> 4);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 0);
      reelval = ((pio1.out[1] & 0x30) >> 4) | ((pio2.out[0] & 0x30) >> 2);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 1);
      reelval = ((pio1.out[1] & 0x0c) >> 2) | (pio2.out[0] & 0x0c);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 2);
      reelval = (pio1.out[1] & 0x03) | ((pio2.out[0] & 0x03) << 2);
      reel.write((reelval & 0x09) | ((reelval & 0x04) >> 1) | ((reelval & 0x02) << 1), 3);

      if (reel.changed[0] || reel.changed[1] || reel.changed[2] || reel.changed[3])
        parent->do_reel();
      if (MonitorActive) {
        IOMonitor->UpdateReel2(reel.reels[2].pat | (reel.reels[3].pat << 4), reel.reels[0].pos, reel.reels[1].pos);
        IOMonitor->UpdateReel1(reel.reels[0].pat | (reel.reels[1].pat << 4), reel.reels[2].pos, reel.reels[3].pos); // bogus
        IOMonitor->UpdateAux1(pio2.out[0]);
        IOMonitor->UpdateAux2(pio2.out[1]);
      }
      break;
    case 0x20:
      if ( Port & 0x100) {
        pio3.c_w((Port & 0x200) >> 9, Value);
//        sprintf(buffer, "pio3 dir_a %02x @ %04x", pio3.dir[0], R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      } else
        pio3.d_w((Port & 0x200) >> 9, Value);
      strobe = pio3.out[0] & 0x07;
      if (MonitorActive) {
        IOMonitor->UpdateLampSelects(pio3.out[0] & 0x07);
        IOMonitor->UpdateAux3(pio3.out[1] & 0x0f);
      }
      break;
    case 0x10:
      if ( Port & 0x100)
        pio4.c_w((Port & 0x200) >> 9, Value);
      else
        pio4.d_w((Port & 0x200) >> 9, Value);
//      if ( (Port & 0x300) == 0 ) {
//        sprintf(buffer, "Triacs %02x @ %04x", pio4.out[0], R.PC.W.l);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//      }
      if (MonitorActive) {
        IOMonitor->UpdateTriacs(pio4.out[0]);
        IOMonitor->Update7segs(pio4.out[1]);
      }
      if ( (Port & 0x300) == 0x200 && seg7.writejpm(pio4.out[1], 7-strobe) )
        parent->do_led_display();
      break;
    case 0x08:
      if ( Port & 0x100)
        pio5.c_w((Port & 0x200) >> 9, Value);
      else
        pio5.d_w((Port & 0x200) >> 9, Value);
      if (MonitorActive) {
        IOMonitor->UpdateLampDrives1(pio5.out[0] | (pio5.out[1] << 8));
      }
      if ((lamp.write(pio5.out[0], strobe<<1, 0)) || (lamp.write(pio5.out[1], strobe<< 1, 8)))
        parent->do_lamp();
      break;
    default:
      switch (Port & 0x07) {
        case 0x06:
          // VFD Reset
//          sprintf(buffer, "VFD Reset %02x @ %04x", Value, R.PC.W.l);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
          display.enable(Value);
          if ( display.display_changed || display.intensity_changed )
            parent->do_display();
          break;
        case 0x03:
        case 0x02:
          // Sound
          gi.AY8910Write(Port & 0x01, Value);
          if ( gi.porta_changed ) {
            meter.write( gi.porta );
          }
          break;
        case 0x01:
          ctc.w(((Port & 0x200) >> 9) | ((Port & 0x100) >> 7), Value);
          break;
        case 0x00:
          // SIO
          if (Port & 0x100)
            ; // sprintf(buffer, "SIO Control Write %02x @ %04x", Value, R.PC.W.l);
          else {
            if ( Value == 0xc ) {// Reset ?
              if ( chars == 80 ) {
                if ( alpha_update )
                  parent->do_matrix();
                alpha_update = false;
              }
              chars = 0;
            } else if ( chars < 80 ) {
              switch ( Value ) {
                case 0x2f:
                  break;
                default:
                  if ( alpha[chars] != Value ) {
                    alpha_update = true;
                  }
                  alpha[chars++] = Value;
                  break;
              }
            }
          }
          break;
        case 0x05: // NEC
//          sprintf(buffer, "Sound Write %02x %04x @ %04x", Value, Port, R.PC.W.l);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
          bank = Value;
          if ( Value == 0xc0 ) {  // Reset
//            sprintf(buffer, "Sound Reset %02x @ %04x", Value, R.PC.W.l);
//            DataWindow->AddMessage((AnsiString)buffer, 0xb0);
            tune = 0;
            busy = false;
            parent->do_wave();
          } else {
            if ( (Value & 0x80) == 0 ) { // Latch
              latch = (Value & 0x1f);
//              sprintf(buffer, "Sound Latch %02x @ %04x", Value, R.PC.W.l);
//              DataWindow->AddMessage((AnsiString)buffer, 0xb0);
            }
            if ( Value == 0xb0 ) {  // Play
              tune = lookup[1][latch];
//              sprintf(buffer, "Sound Play1 %d @ %04x", tune, R.PC.W.l);
//              DataWindow->AddMessage((AnsiString)buffer, 0xb0);
              busy = true;
              parent->do_wave();
            } else if ( Value == 0xa0 ) {
              tune = lookup[0][latch];
//              sprintf(buffer, "Sound Play2 %d @ %04x", tune, R.PC.W.l);
//              DataWindow->AddMessage((AnsiString)buffer, 0xb0);
              busy = true;
              parent->do_wave();
            }
          }
          break;
        default:
          break;
      }
      break;
    }
  }
//---------------------------------------------------------------------------
unsigned Proconn::Z80_RDMEM(word A)
{
Byte ret = 0;

  switch (A & 0xf800) {
    case 0xf000:
      ret = memory[A & 0xf7ff];
      break;
    default:
      ret = memory[A];
      break;
  }
  if ( (A & parent->read_mask) == parent->read_bp ) {
    if ( (ret & parent->read_value_mask) == parent->read_value_bp ) {
      parent->read_addr = A;
      parent->read_value = ret;
      if ( parent->readbp )
        parent->state = MS_READ_BP;
    }
  }
  return ret;
}
//---------------------------------------------------------------------------
void Proconn::Z80_WRMEM(word A,byte V)
{
  switch (A & 0xf800) {
    case 0xf000:      // Write To RAM
      memory[A & 0xf7ff] = V;
      break;
    default:         // Write To ROM!!
      break;
  }
  if ( (A & parent->write_mask) == parent->write_bp ) {
    if ((V & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = A;
      parent->write_value = V;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------
unsigned Proconn::Z80_RDSTACK(word A)
{
  return Z80_RDMEM(A);
}
//---------------------------------------------------------------------------
void Proconn::Z80_WRSTACK(word A,byte V)
{
  Z80_WRMEM(A,V);
}
//---------------------------------------------------------------------------
void Proconn::Z80_Patch (Z80_Regs *Regs)
{
  // Nothing required here for now
}
//---------------------------------------------------------------------------
void Proconn::Z80_Reti (void)
{
  ctc.reti();
  if ( depth )
    depth--;
}
//---------------------------------------------------------------------------
void Proconn::Z80_Retn (void)
{
}
//---------------------------------------------------------------------------
int Proconn::Z80_Interrupt(void)
{
int rv = Z80_IGNORE_INT;

  if ( ctc.interrupt_check() && R.IFF1 == 1 ) {
    rv = ctc.interrupt();
    depth++;
  }

  return rv;
}
//---------------------------------------------------------------------------
bool Proconn::Z80_InstHook(int PC)
{
}
//---------------------------------------------------------------------------

int Proconn::dasm(int pc, char *buffer)
{
  return Z80_Dasm(&memory[pc], buffer, pc);
}

//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 1);
}
//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 256);
  UpdateLedDisplays(&board.seg7);
}

//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateWave(void)
{
  UpdateWaves(board.tune,0);
}
//---------------------------------------------------------------------------

void __fastcall TProconn::UpdateMatrix(void)
{
  UpdatePROCONNMatrix(board.alpha);
}
//---------------------------------------------------------------------------

bool __fastcall TProconn::AutoLamp(int i, int &reel, int &symbol)
{
  if ( i < 9 ) {
    reel = (i / 3);
    symbol = (i % 3);
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------

int __fastcall TProconn::getpc(void)
{
  return board.R.PC.W.l;
}
//---------------------------------------------------------------------------
int __fastcall TProconn::getmemory(int address)
{
  if ( address >= 0 && address < 0x10000 )
    return board.memory[address];
  else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TProconn::setmemory(int address, Byte value)
{
  board.memory[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TProconn::dasm(int pc, char *buffer)
{
  return board.dasm(pc,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::ClearRAM(void)
{
  for (int i = 0xf000; i < 0xf800; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TProconn::UpdateIOMeters(void)
{
//  IOMonitor->UpdateMeters(board.pia5.orb);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::UpdateIOTriacs(void)
{
//  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::UpdateIOSinks(void)
{
//  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::UpdateIOSelects(void)
{
//  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
//  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TProconn::SaveToStream(Byte *&ptr)
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
void __fastcall TProconn::LoadFromStream(Byte *&ptr)
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
void __fastcall TProconn::UpdateTrace(void)
{
int count;
int addr;
Word pos;
char args[150];


  MainDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
    addr &= 0xFFFF;
    board.dasm( addr, buffer);
    strncat( buffer, "                                             ", 46 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }
}
//---------------------------------------------------------------------------


