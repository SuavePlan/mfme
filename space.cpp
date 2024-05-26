//---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
#include "debug.h"
#include "iomonitor.h"
#include "space.h"
#include "sample.h"
#include "shared.h"

extern char buffer[];

const char *spec6303regs[] = {
  "P1DDR(*)", "P2DDR", "PORT1(*)", "PORT2", "P3DDR(*)", "P4DDR(*)", "PORT3(*)", "PORT4(*)",
  "TCSR1", "FRCH", "FRCL", "OCR1H", "OCR1L", "ICRH", "ICRL", "TCSR2", "RMCR",
  "TRCSR1", "RDR", "TDR", "RP5CR", "PORT5", "P6DDR", "PORT6", "PORT7", "OCR2H",
  "OCR2L", "TCSR3", "TCONR", "T2CNT", "TRCSR2", "TSTREG(**)", "P5DDR", "P6CSR",
  "RES22", "RES23", "RES24", "RES25", "RES26", "RES27"
};

#define NO_DEBUG  1
#define NO_SOUND_DEBUG 1

#define NAR 0x80
#define BUSY 0x40

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TSPACE::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSPACE::TSPACE(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
  board_type = ACE_SPACE;
  MainDebug->SetProcess(this,board_type);
  board.parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TSPACE::Run()
{
int cycles = 0;

  SetReels(&board.reel, 4, 0);
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

void __fastcall TSPACE::saveram(void)
{
FILE		*fp;
String      filename;

  filename = CurrGamePath + Game.SubString(1, Game.Pos(".")) + "ram";
  chmod(filename.c_str(),S_IWRITE);
  fp = fopen(filename.c_str(), "wb");
  if (fp) {
    fwrite( &board.ram[0], 0x2000, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::load_intelhex(void)
{
FILE		*fp;
int		addr;
struct stat statbuf;
String filename;
int size;
int count;

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
    fread( &board.ram[0], 0x2000, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::hard_reset(void)
{
  board.refresh = 0;
//  board.m6800_subtype = M6800_TYPE_6303;
  board.reset();
  board.pia.reset();
  board.display.reset();
  board.seg7.reset();
  board.display.enable(1);
  board.gi1.AY8910_reset();
  board.gi2.AY8910_reset();
  board.gi1.AY8910_set_clock(2000000);
  board.gi2.AY8910_set_clock(2000000);
  board.meter.reset();
  board.nmicount = 0;
  board.irqcount = 0;
  board.outputsA = 0;
  board.outputsB = 0;
  board.inputsA = 0;
  board.inputsB = 0;
  board.inputsC = 0;
  board.tune = 0;
  board.tune1 = 0;
  board.tune2 = 0;
  board.nar = NAR;
  board.nartimer = 0;
  board.reelen = false;
  board.reelnmi = false;
  board.nmilatch = false;
  board.serdata = false;
  board.ioenabled = false;
  board.pia.set_cb1(~board.nmilatch);
  board.irqlatch = false;
  board.strobecount = 0;
  board.Rx = false;
  board.Rxcount = 0;
  board.Tx = 0;
  board.Txcount = 0;
  board.reelspeed = 0;
}

//---------------------------------------------------------------------------
int SPACE::execute(void)
{
    Byte	do_int;
    int     cycs;

    do_int = 0;
    refresh++;
    if ( refresh == 100000 )
      refresh = 0;
    switch ( parent->state ) {
      case 0:
        if ( nartimer ) {
          nartimer--;
          if ( !nartimer ) {
            nar = NAR;
          }
        }
        if ( matrixrefresh ) {
          matrixrefresh--;
          if ( !matrixrefresh )
            parent->do_matrix();
        }
        if ( (refresh % 2000) == 0 ) {
          if ( lamp.update() )
            parent->do_lamp();
          if ( meter.update() )
            parent->do_meter();
        }
        if ( reelspeed > 15500 ) {
          reelspeed -= 15500;
          reel.spinace();
          if ( reel.changed[0] | reel.changed[1] | reel.changed[2] | reel.changed[3] )
            parent->do_reel();
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        parent->trace[parent->sts].a = d.b.h;
        parent->trace[parent->sts].b = d.b.l;
        parent->trace[parent->sts].x = x.w.l;
        parent->trace[parent->sts].s = s.w.l;
        cycs = mc6803::execute();
        cycles += cycs;
        reelspeed += cycs;
        parent->Total_cycles += cycs;
        if ( reelen ) {
          nmicount += cycs;
          if ( nmicount > 3000 ) {
            nmicount -= 3000;
            nmilatch = true;
            reelnmi = true;
            pia.set_cb1(~nmilatch);
          }
        }
        irqcount += cycs;
        if ( irqcount > 21000 ) {
          irqcount -= 21000;
          irqlatch = true;
        }
        if ( reelnmi ) {
          ENTER_INTERRUPT( 0xfffc );
          reelnmi = false;
        } else if ( !(cc & 0x10) ) {
          if ( ((tcsr1 & 0x48) == 0x48 || (tcsr2 & 0x28) == 0x28 ) ) {
            ENTER_INTERRUPT(0xfff4);
          } else if ( (tcsr3 & 0xc0) == 0xc0 ) {
            ENTER_INTERRUPT(0xffec); // CMI
          } else if ( irqlatch && (rp5cr & 0x2) ) {
            ENTER_INTERRUPT(0xffea);
          } else if ( (trcsr1 & 0x24) == 0x24 ) {
            ENTER_INTERRUPT(0xfff0);
          }
        }
        if ( parent->state == 2 )
          break;
        if ( pc.w.l == parent->bp )
          parent->state = 2;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        parent->trace[parent->sts].a = d.b.h;
        parent->trace[parent->sts].b = d.b.l;
        parent->trace[parent->sts].x = x.w.l;
        parent->trace[parent->sts].s = s.w.l;
        cycs = mc6803::execute();
        cycles += cycs;
        parent->Total_cycles += cycs;
        irqcount += cycs;
        if ( irqcount > 200000 ) {
          irqcount -= 200000;
          if ( rp5cr & 0x2 )
            ENTER_INTERRUPT(0xffea);
        }
        if ( parent->in_int )
          break;
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
        break;
    }
    if ( (refresh % 1000) == 0 && !NoSound )
      UpdateSoundBufferspace(cycles);

  return 0;
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateTrace(void)
{
char buffer[200];
int count;
int addr;
Word pos;
char args[50];

  MainDebug->Code->Items->Clear();
  pos = (sts + 1) % TRACE_SIZE;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
    addr &= 0xFFFF;
    board.dasm( addr, buffer);
    strncat( buffer, "                                        ", 40 - strlen(buffer));
    sprintf( args, "   %02x %02x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].s);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }

}
//---------------------------------------------------------------------------

Byte SPACE::read(Word addr)
{
Byte		ret = 0;

  if ( addr >= 0x30 && addr < 0x38 ) {
    switch ( addr & 0x7 ) {
      case 0: // Reel
        ret = TxReelResponse();
        break;
      case 1: // STBHI
        break;
      case 2: // STBLO
        break;
      case 3: // LampSTB
        break;
      case 4: // Shift STB
        break;
      case 5: // Shift CLK
        break;
      case 6: // SIOCOM
        ret = firstbyte;
        serdata = false;
        switch ( sercommand ) {
          case 0x8b:
            busy = BUSY;
            if ( tune1 && Sound->Playing(tune1) )
              busy = 0x0;
            else if ( tune2 && Sound->Playing(tune2) )
              busy = 0x0;
            ret = nar | busy | ( ret & 0x3f );
            if ( !nar )
              ret &= ~BUSY;
            break;
          case 0x8a:
            ret = tunelatch;
            break;
        }
        break;
      case 7: // Watchdog ?
        break;
    }
  } else if ( addr >= 0x38 && addr <= 0x3b ) {
    pia.ora_in = 0xc0; // INP4 & INP5
    pia.orb_in = 0x0;  // optos 0 - 15
    if ( inputsA & 1 ) // INP0
      pia.orb_in |= 0x10;
    if ( inputsB & 1 ) // INP1
      pia.orb_in |= 0x20;
    if ( inputsC & 1 )   // INP3
      pia.orb_in |= 0x80;
    ret = pia.read(addr & 0x3);
  } else if ( addr < 0x2000 ) {
    if ( ioenabled )
      ret = ram[addr];
  else
      ret = 0;
  } else
    ret = memory[addr];

  if ( (addr & parent->read_mask) == parent->read_bp ) {
    if ( (ret & parent->read_value_mask) == parent->read_value_bp ) {
      parent->read_addr = addr;
      parent->read_value = ret;
      if ( parent->readbp )
        parent->state = 2;
    }
  }
  return ret;
}
//---------------------------------------------------------------------------


void SPACE::write(Word addr, Byte x)
{

  if ( addr >= 0x30 && addr < 0x38 ) {
    switch ( addr & 0x7 ) {
      case 0: // Reel
        if ( Rx )
          RxReelCommand(x);
        break;
      case 1: // STBHI
        lamphi = x;
        break;
      case 2: // STBLO
        lamplo = x;
        break;
      case 3: // LampSTB
        strobe = x & 0x7;
        break;
      case 4: // Shift STB
        strobecount = 0;
        meter.write(outputsB & 0xff);

        inputsA = ( ~matrix[2] & 0xff ) | ( matrix[4] << 8  );
        inputsB = ( ~matrix[1] & 0xff ) | ( (~matrix[0] & 0xff) << 8 );
        inputsC = ( matrix[3] );
        if ( meter.meter_on )
          inputsB &= ~0x100;
        else
          inputsB |= 0x100;
        break;
      case 5: // Shift CLK
        strobecount++;
        inputsA >>= 1;
        if ( !(outputsB & 0x8000) )
          inputsA |= 0x8000;
        inputsB >>= 1;
        if ( !(outputsA & 0x8000) )
          inputsB |= 0x8000;
        inputsC >>= 1;
        outputsA <<= 1;
        outputsB <<= 1;
        if ( port2 & 1 ) {
          outputsA |= 1;
        }
        if ( port2 & 2 ) {
          outputsB |= 1;
        } else {
          inputsC |= 0x80;
        }
        break;
      case 6: // SIOCOM
        if ( serdata ) {
          if ( (sercommand & 0x90) == 0x90 )
            firstbyte = x;
          else if ( (sercommand & 0x80) == 0x80 )
            secondbyte = x;
          switch ( sercommand ) {
            case 0x9a:
              tunelatch = x;
              break;
            case 0x9b:
              extlamp = x;
              break;
            case 0x9c:
              break;
            case 0x8a:
            case 0x8b:
            case 0x8c:
            case 0x88:
            case 0x89:  // ??
              break;
            case 0x99:
              break;
            case 0x98:   // Alpha
              display.write_all(x & 4,x & 1,x & 2);
              if ( display.display_changed || display.intensity_changed )
                parent->do_display();
              break;
            case 0x00:  // Reset
              break;
            case 0xa0:
            case 0x80:
              break;
            default:
              break;
          }
          serdata = false;
        } else {
          sercommand = x;
          switch ( x ) {
            case 0xa0: // ST ?
              nar = 0;
              nartimer = 2000;
              if ( !tunelatch || !Sound->Playing(tune1) ) {
                tune1 = tunelatch;
                tune = tune1;
                channel = 0;
                tune1playing = true;
                parent->do_wave();
              }
              break;
            case 0xc0:
              if ( !tunelatch || !Sound->Playing(tune2) ) {
                tune2 = tunelatch;
                tune = tune2;
                channel = 1;
                tune2playing = true;
                parent->do_wave();
              }
              break;
            case 0xe0:
              break;
            case 0x80:
              break;
            default:
              sercommand = x;
              serdata = true;
              break;
          }
        }
//#endif
        break;
      case 7: // Watchdog ?
        break;
    }
  } else  if ( addr >= 0x38 && addr <= 0x3b ) {
    pia.write(addr & 0x3, x);
    if ( pia.orb_changed ) {
      if ( (pia.orb_changed & 0x40) && ((pia.orb & 0x40) == 0) )
        irqlatch = false;
    }
    if ( pia.ca2_changed ) {
      if ( pia.ca2 == 0 ) {
        if ( (strobe == 5 || strobe == 6) && extlamp == 0x1a ) {
          if ( lamp.writejpm( lamphi << 8 | lamplo, strobe + 3, 12) )
            parent->do_lamp();
        } else if ( lamp.writejpm( lamphi << 8 | lamplo, strobe, 12) )
          parent->do_lamp();
      }
    }
    if ( pia.cb2_changed ) {
      if ( pia.cb2 == 0 ) { // Reel Processor enabled :-)
        reelen = true;
      }
    }
  } else if ( addr < 0x2000 ) {
    ram[addr] = x;
    if ( addr >= 0x1f00 && addr < 0x1fc0 )
      matrixrefresh = 1600; //800;
    else if ( addr >= 0x1fc0 && addr < 0x1fe0 ) {
      if ( seg7.write_static(x, addr - 0x1fc0) )
        parent->do_led_display();
    }
  }

  if ( (addr & parent->write_mask) == parent->write_bp ) {
    if ((x & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = addr;
      parent->write_value = x;
      if ( parent->writebp )
        parent->state = 3;
    }
  }
}
//---------------------------------------------------------------------------

void SPACE::writeport(Byte port, Byte value, Byte changed)
{
  switch ( port ) {
    case 6: // 4-7 GI's
      aydata = value;
      break;
    case 5: // 4-7 GI's
      if ( changed & 0xf0 ) {
        value &= 0xf0;
        if ( value == 0x70 || value == 0x50 )
          gi1.write_data( aydata, value & 0x40, value & 0x10, value & 0x20);
        else if ( value == 0xb0 || value == 0x90 )
          gi2.write_data( aydata, value & 0x80, value & 0x10, value & 0x20);
      }
      if ( changed & 4 ) {
        if ( (value & 4) == 0 ) {
          Rx = true;
          Rxcount = 0;
          Rxptr = Rxcmd;
          RxLength = 0;
        }
      }
      break;
    case 2:
      if ( changed & 0x20 ) {
        if ( (value & 0x20) == 0 ) {
          ioenabled = true;
          serdata = false;
        } else
          ioenabled = false;
      }
      if ( changed & 4 ) {
        if ( (value & 4) == 0 ) {
          nmilatch = false;
          pia.set_cb1(~nmilatch);
        }
      }
      break;
  }
}
//---------------------------------------------------------------------------

void SPACE::writeinternal(Byte addr, Byte value)
{
  switch ( addr ) {
    case 0x15: // Port5
    case 0x17: // Port6
    case 0x16: // p6ddr
    case 3:    // Port2
    case 0x8:  // TCSR1
    case 0x9:  // FRCH
    case 0xa:  // FRCL
    case 0xb:  // OCR1H
    case 0xc:  // OCR1L
    case 0x19:  // OCR2H
    case 0x1a:  // OCR2L
    case 0xf:  // TCSR2
      break;
    case 0x14: // RP5CR
#ifdef DEBUG
      if ( value & 1 ) {
        sprintf(buffer, "-- IRQ1 --");
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }
#endif
      break;
    case 0x13: // TDR
#ifdef DEBUG
      sprintf(buffer, "Transmit char %02x", tdr);
      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
      break;
    case 0x1b: // TCSR3
#ifdef DEBUG
      if ( value & 0x40 ) {
        sprintf(buffer, "Int enabled %02x", value);
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }
#endif
      break;
    default:
#ifdef DEBUG
      sprintf(buffer, "Write %02x to %s @ %04x", value, spec6303regs[addr], pc.w.l);
      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
      break;
  }
}
//---------------------------------------------------------------------------

Byte SPACE::readinternal(Byte addr, Byte value)
{
  switch ( addr ) {
    case 0x15: // Port5
    case 0x17: // Port6
    case 0x16: // p6ddr
    case 3:    // Port2
    case 0x8:  // TCSR1
    case 0x9:  // FRCH
    case 0xa:  // FRCL
    case 0xb:  // OCR1H
    case 0xc:  // OCR1L
    case 0xf:  // TCSR2
    case 0x14: // RP5CR
      break;
    default:
#ifdef DEBUG
      sprintf(buffer, "Read %02x from %s @ %04x", value, spec6303regs[addr], pc.w.l);
      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
      break;
  }
  return value;
}
//---------------------------------------------------------------------------

Byte SPACE::readport(Byte port)
{
Byte ret = 0;

  switch ( port ) {
    case 6:
      if ( (port5 & 0xf0) == 0x60 ) {
        ret = ~DIP2;
#ifdef DEBUG
        sprintf(buffer, "Read DIP2 %02x @ %04x", ret, pc.w.l);
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
      } else if ( (port5 & 0xf0) == 0xa0 ) {
        ret = ~DIP1;
#ifdef DEBUG
        sprintf(buffer, "Read DIP1 %02x @ %04x", ret, pc.w.l);
        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
      }
      break;
    case 5:
      ret = port5;
//      sprintf(buffer, "Read Port5 %02x @ %04x", ret, pc.w.l);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      break;
    case 2:
      ret = port2;// | 0x80;
      break;
    default:
#ifdef DEBUG
      sprintf(buffer, "read port %02x", port);
      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
      break;
  }
  return ret;
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 160);
}
//---------------------------------------------------------------------------

bool __fastcall TSPACE::AutoLamp(int i, int &reel, int &symbol)
{
  if ( (i / 16) < 3 && (i % 16) < 3 ) {
    reel = (i / 16);
    symbol = (i % 16);
    if ( symbol < 3 )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 1);
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateMatrix(void)
{
  UpdateACEMatrix(&board.ram[0x1f00]);
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

void __fastcall TSPACE::UpdateWave(void)
{
#ifdef SOUND_DEBUG
  sprintf(buffer, "Play %d channel %d", board.tune, board.channel);
  DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
  UpdateWaves(board.tune,board.channel);
}
//---------------------------------------------------------------------------

int __fastcall TSPACE::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------

int __fastcall TSPACE::getmemory(int address)
{
  if ( address == 0 || address == 2 || address == 4 || address == 5 ||
       address == 6 || address == 7 || address == 24 )
    return board.ram[address];
  else if ( address >= 0x40 && address < 0x2000 )
    return board.ram[address];
  else if ( address >= 2000 && address < 0x10000 ) {
    return board.memory[address];
  } else
    return -1;
}
//---------------------------------------------------------------------------
void __fastcall TSPACE::setmemory(int address, Byte value)
{
  board.memory[address] = value;
}

//---------------------------------------------------------------------------
int __fastcall TSPACE::dasm(int pc, char *buffer)
{
  return board.dasm(pc,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TSPACE::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x2000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------

void SPACE::RxReelCommand(Byte cmd )
{
Byte csum = 0;
Byte *ptr;
int i;

  if ( Rxcount & 1 ) {
    *Rxptr = ( *Rxptr & 0xf ) | ( cmd & 0xf0 );
    Rxptr++;
    RxLength--;
  } else {
    *Rxptr = cmd >> 4;
    if ( Rxcount == 0 )
      RxLength = *Rxptr + 2;
  }
  Rxcount++;

  if ( RxLength == 0 ) {
    Rx = false;
    Rxcount = Rxcount / 2;
#ifdef DEBUG
    char * p = buffer;
    p += sprintf(p, "**** Rx Command: ");
    for ( i = 0; i < Rxcount; i++ )
      p += sprintf(p, "%02x ", Rxcmd[i]);
    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
    for ( i = 0; i < Rxcount - 1; i++ )
      csum += Rxcmd[i];
    if ( csum == 0 )
      csum = 0xff;
    if ( csum == Rxcmd[Rxcount - 1] ) {  // Checksum OK
      Txcmd[0] = csum;
      Txcmd[1] = (Rxcmd[0] & 0xf0) + 1;
      Txcmd[2] = 0;
      Txcmd[3] = Txcmd[1];
      Txptr = Txcmd;
      Txcount = 8;
      switch ( Rxcmd[0] >> 4 ) {
        case 0xf:
          if ( Rxcmd[0] == 0xf1 && Rxcmd[1] == 0 )
            Txcmd[2] = 0x10 ; //0x07;   // Version ?
          break;
        case 0x2:
          break;
        case 0x1: // Spin
          ptr = &Rxcmd[1];
          for ( i = 0; i < 4; i++ ) {
            int steps = ptr[1] + 256 * (ptr[0] & 0xf );
            if ( ptr[0] & 0x80 )
              reel.setace(i,steps*2,-1);
            else
              reel.setace(i,steps*2,1);
            ptr += 2;
          }
          break;
        case 0x8: // Go to Position
          ptr = &Rxcmd[1];
          for ( i = 0; i < 4; i++ ) {
            int pos = ptr[1] + 256 * ptr[0];
            if ( ptr[0] & 0x80 )
              reel.setace(i,(((((reel.reels[i].pos/2) + 3) - pos) + 48) % 48) << 1,-1);
            else
              reel.setace(i,(((48 - ((reel.reels[i].pos/2)+3)) + pos) % 48) << 1 ,1);
            ptr += 2;
          }
          break;
        case 0x9: // Busy
          for ( i = 0; i < 4; i++ ) {
            if ( reel.reels[i].steps_remaining )
              Txcmd[2] |= (1 << i);
          }
          break;
        case 0x7: // opto
          break;
        case 0x4: // Positions
          ptr = &Txcmd[2];
          for ( i = 0; i < 4; i++ )
            *ptr++ = (reel.reels[i].pos >> 1) + 3;
          Txcount = 14;
          break;
        default:  // Unknown
          break;
      }
      Txcmd[1] = (Rxcmd[0] & 0xf0) + ((Txcount-6) >> 1);
      csum = 0;
      for ( i = 1; i < (Txcount-2)/2; i++ )
        csum += Txcmd[i];
      Txcmd[i] = csum;
    }
#ifdef DEBUG
    p = buffer;
    p += sprintf(p, "**** Tx Command: ");
    for ( i = 0; i < Txcount/2; i++ )
      p += sprintf(p, "%02x ", Txcmd[i]);
    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
  }
}

Byte SPACE::TxReelResponse(void)
{
Byte ret = 0;

  if ( Txcount ) {
    if ( Txcount & 1 ) {
      ret = *Txptr & 0xf0;
      Txptr++;
    } else {
      ret = (*Txptr & 0xf) << 4;
    }
    Txcount--;
  }
  return ret;
}


