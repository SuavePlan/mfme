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
#include "sys5.h"
#include "shared.h"
#include "sample.h"

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
//      void __fastcall TSYS5::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TSYS5::TSYS5(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
  board_type = JPM_SYS5;
  MainDebug->SetProcess(this,board_type);
  board.parent = this;
//  Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::Run()
{
int cycles = 0;

  board.nbr_reels = SetReels(&board.reel, 8, 9);
//  CreateAutoLamp();

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
void __fastcall TSYS5::Run2()
{
int cycles = 0;

  board.nbr_reels = SetReels(&board.reel, 8, 9);
//  CreateAutoLamp();

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

void __fastcall TSYS5::saveram(void)
{
  TSystem::saveram(board.ram,0x4000);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::load_intelhex(void)
{
FILE *fp;
int addr;
struct stat statbuf;
Byte	n;
String filename;
int size, count;

  addr = 0;
  for ( count = 0; count < ROMList->Count; count++ ) {
	fp = fopen(ROMList->Strings[count].c_str(), "rb");
    if ( fp != NULL ) {
      stat( ROMList->Strings[count].c_str(), &statbuf);
      size = statbuf.st_size;

      for ( ; size != 0; size-- ) {
		n = fgetc(fp);
		board.rom[addr] = n;
        addr+=2;
	  }
      fclose(fp);
    }
    switch ( count ) {
      case 0:
        addr = 1;
        break;
      case 1:
        addr = 0x10000;
        break;
      case 2:
        addr = 0x10001;
        break;
      case 3:
        addr = 0x20000;
        break;
      case 4:
        addr = 0x20001;
        break;
    }
  }

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    for ( addr = 0; addr < 0x4000; addr++ ) {
      n = fgetc(fp);
      board.ram[addr] = n;
    }
    fclose(fp);
  }

}
//---------------------------------------------------------------------------

void __fastcall TSYS5::hard_reset(void)
{
  board.refresh = 0;
  board.reset(M68K_CPU_MODE_68000);
  board.meter.reset();
  board.pia.reset();
  board.ptm.reset();
  board.acia.reset();
  board.yamaha.reset();
//  board.cycles = 0;
  board.busy = 0;
}

//---------------------------------------------------------------------------
int SYS5::execute(void)
{
int cycles;

    switch ( parent->state ) {
      case 0:
        refresh++;
        if ( refresh == 1000 ) {
          if ( meter.update() )
            parent->do_meter();
          refresh = 0;
          if ( cointimer ) {
            cointimer--;
            if ( cointimer == 15 ) {
              matrix[7] &= ~coinTag;
            }
          }
        }
        if ( parent->reeltimer ) {
          parent->reeltimer--;
          if ( !parent->reeltimer )
            parent->do_reel();
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = m68k_cpu.pc;
        cycles = mc68000::execute();
        parent->Total_cycles += cycles;
        last_cycles += cycles;
        while ( last_cycles > 8 ) {
          last_cycles -= 8;
	      ptm.tick(1);
        }
        acia.tick(cycles);
        if ( ptm.sr & 0x80 )
          signal_interrupt(3,27);
        else if ( acia.status & 0x80 )
          signal_interrupt(4,28);
        if ( parent->state == MS_READ_BP )
          break;
        if ( m68k_cpu.pc == parent->bp )
          parent->state = MS_EXECUTE_BP;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = m68k_cpu.pc;
        cycles = mc68000::execute();
        parent->Total_cycles += cycles;
        last_cycles += cycles;
        if ( last_cycles > 10 ) {
          last_cycles -= 10;
	      ptm.tick(1);
        }
        if ( acia.status & 0x80 )
          signal_interrupt(4,28);
        else if ( ptm.sr & 0x80 )
          signal_interrupt(3,27);
        if ( parent->state == MS_STEPPING )
          parent->state = MS_EXECUTE_BP;
      case 2: // Read Break
      case 3: // Write Break
      case 4: // Execute break
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
        break;
    }

    if ( (refresh % 1000) == 0 && !NoSound )
      UpdateSoundBuffersys5(0);

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateTrace(void)
{
char buffer[80];
int count;
int addr;
Word pos;

  MainDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos++ % TRACE_SIZE].pc;
    board.dasm( addr, buffer);
    MainDebug->Code->Items->Add(buffer);
  }

}
//---------------------------------------------------------------------------
int SYS5::m68k_read_immediate_8(uint address, int cs)
{
uint value = 0;

  if ( address < 0x40000 )
    value = rom[address];
  else
    printf("not from rom\n");
  return value;
}
//---------------------------------------------------------------------------

int  SYS5::m68k_read_immediate_16(uint address, int cs)
{
uint value = 0;

  if ( address < 0x40000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else
    printf("not from rom\n");
  return value;
}
//---------------------------------------------------------------------------

int  SYS5::m68k_read_immediate_32(uint address, int cs)
{
uint value = 0;

  if ( address < 0x40000 ) {
      value = rom[address];
      value = rom[address+1] + (value << 8);
      value = rom[address+2] + (value << 8);
      value = rom[address+3] + (value << 8);
  } else
    printf("not from rom\n");
  return value;
}
//---------------------------------------------------------------------------

/* Read an instruction (16-bit word immeditately after PC) */
int  SYS5::m68k_read_instruction(uint address, int cs)
{
uint value = 0;

  if ( address < 0x40000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else
    printf("not from rom\n");
  return value;
}
//---------------------------------------------------------------------------

/* Write to anywhere */
void SYS5::m68k_write_memory_8(uint address, int value, int cs)
{
int orig_address = address;

  if ( address >= 0x40000 && address < 0x44000 )
    ram[address-0x40000] = value;
  else if ( address >= 0x46020 && address < 0x46040 ) {
    acia.write( address & 2, value);
//    if ( address == 0x46023 ) {
//      sprintf(buffer, "acia chr '%c' @ %08x", value, m68k_cpu.pc);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//    }
  } else if ( address >= 0x46040 && address < 0x46060 ) {
    ptm.write( (address - 0x46040) >> 1, value);
//    sprintf(buffer, "ptm data %02x to %08x(%02x) @ %08x", value, address, (address - 0x46040) >> 1, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x46060 && address < 0x46080 ) {
    pia.write( (address - 0x46060) >> 1, value);
    if ( pia.ca2_changed ) {
//      sprintf(buffer, "chop %02x", pia.ca2);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
    }
//    if ( pia.cb2_changed ) {
//      sprintf(buffer, "volume override %02x", pia.cb2);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//    }
    if ( pia.orb_changed ) {
      if ( pia.orb_changed & 0x7f ) {
        meter.write(pia.orb);
      }
    }
  } else if ( address >= 0x46080 && address < 0x460a0 ) {
//    sprintf(buffer, "uart3 %02x to %08x @ %08x", value, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x4c000 && address < 0x4c100 ) {
    mux_array[address-0x4c000] = value;
    address = ((address - 0x4c000) >> 1);
    int column = address / 4;
    int type = address % 4;
    if ( column < 16 ) {
      switch ( type ) {
        case 0: // Lamps 0
          if ( lamp.write_static( value, column * 16) )
            parent->do_lamp();
          break;
        case 1: // Lamps 1
          if ( lamp.write_static( value, 8 + column * 16) )
            parent->do_lamp();
          break;
        case 2: // LEDs
          if ( seg7.write_static( value, column) )
            parent->do_led_display();
//          sprintf(buffer, "led data %02x(%d - %d) to %08x @ %08x", value, column, type, address, m68k_cpu.pc);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
          break;
      }
    }
//    sprintf(buffer, "mux data %02x(%d - %d) to %08x @ %08x", value, column, type, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x48000 && address < 0x48020 ) {
    switch ( address ) {
      case 0x48008:
        reel34 = value;
        reel.writesys5( value, 2);
        if ( reel.changed[2] || reel.changed[3] )
          parent->do_reel();
        break;
      case 0x48009:
        reel12 = value;
        reel.writesys5( value, 0);
        if ( reel.changed[0] || reel.changed[1] )
          parent->do_reel();
        break;
      case 0x4800a:
        reel78 = value;
        reel.writesys5( value, 6);
        if ( reel.changed[6] || reel.changed[7] )
          parent->do_reel();
        break;
      case 0x4800b:
        reel56 = value;
        reel.writesys5( value, 4);
        if ( reel.changed[4] || reel.changed[5] )
          parent->do_reel();
        break;
      case 0x4800c:
        reel.writereellamp( value, 2);
        if ( reel.lamp_changed[2] || reel.lamp_changed[3] )
          parent->do_reel_lamp();
        break;
      case 0x4800d:
        reel.writereellamp( value, 0);
        if ( reel.lamp_changed[0] || reel.lamp_changed[1] )
          parent->do_reel_lamp();
        break;
      case 0x4800e:
        reel.writereellamp( value, 6);
        if ( reel.lamp_changed[6] || reel.lamp_changed[7] )
          parent->do_reel_lamp();
        break;
      case 0x4800f:
        reel.writereellamp( value, 4);
        if ( reel.lamp_changed[4] || reel.lamp_changed[5] )
          parent->do_reel_lamp();
        break;
    }
    serial[address-0x48000] = value;
//    if ( address == 0x48008 ) {
//      sprintf(buffer, "serial write %02x to %08x @ %08x", value, address, m68k_cpu.pc);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//    }
  } else if ( address >= 0x460a0 && address < 0x460c0 ) {
    if ( address & 2 ) {
      if ( SYS5_SND )
        yamaha.write_data(value);
      else
        saa.control(value);
//      sprintf(buffer, "yamaha data %02x(%02x) to %08x @ %08x", value, value & 0x1f, address, m68k_cpu.pc);
    } else {
      if ( SYS5_SND )
        yamaha.write_reg(value);
      else
        saa.data(value);
//      sprintf(buffer, "yamaha control %02x to %08x @ %08x", value, address, m68k_cpu.pc);
    }
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x4c100 && address < 0x4c120 ) {
    switch ( address ) {
      case 0x4c101:
        if ( tune != 0 ) {
          if ( !Sound->Playing(tune) ) {
            tune = 0;
            busy = false;
          }
        } else
          busy = false;
        if ( !busy ) {
          tune = lookup[page][value & 0xff];
          busy = true;
          parent->do_wave();
        }
//        sprintf(buffer, "write8 %02x to %08x @ %08x", value, address, m68k_cpu.pc);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        break;
      case 0x4c103:
        display.write_all( value & 4, value & 1, value & 2);
        if ( display.display_changed || display.intensity_changed )
          parent->do_display();
        break;
      case 0x4c105:
        page = (value >> 1) & 7;
        if ( !value ) { // Reset
          tune = 0;
          parent->do_wave();
        }
//        sprintf(buffer, "write8 %02x to %08x @ %08x", value, address, m68k_cpu.pc);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        break;
//      default:
//        sprintf(buffer, "write8 %02x to %08x @ %08x", value, address, m68k_cpu.pc);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//        break;
    }
  } 
  if ( (orig_address & parent->write_mask) == parent->write_bp ) {
    if ((value & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = orig_address;
      parent->write_value = value;
      parent->write_count++;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

void SYS5::m68k_write_memory_16(uint address, int value, int cs)
{
int orig_address = address;

  if ( address >= 0x40000 && address < 0x44000 ) {
    address -= 0x40000;
    ram[address] = value >> 8;
    ram[address+1] = value & 0xff;
  } else if ( address >= 0x48000 && address < 0x48020 ) {
    switch ( address ) {
      case 0x48008: // reels 1234
        reel12 = value & 0xff;
        reel34 = value >> 8;
        reel.writesys5( reel12, 0);
        reel.writesys5( reel34, 2);
        if ( reel.changed[0] || reel.changed[1] || reel.changed[2] || reel.changed[3] )
//          if ( !parent->reeltimer )
//            parent->reeltimer = 200;
          parent->do_reel();
        break;
      case 0x4800a: // reels 5678
        reel56 = value & 0xff;
        reel78 = value >> 8;
        reel.writesys5( reel56, 4);
        reel.writesys5( reel78, 6);
        if ( reel.changed[4] || reel.changed[5] || reel.changed[6] || reel.changed[7] )
//          if ( !parent->reeltimer )
//            parent->reeltimer = 200;
          parent->do_reel();
        break;
      case 0x4800c:
        reel.writereellamp( value & 0xff, 0);
        reel.writereellamp( value >> 8, 2);
        if ( reel.lamp_changed[0] || reel.lamp_changed[1] || reel.lamp_changed[2] || reel.lamp_changed[3] )
          parent->do_reel_lamp();
        break;
      case 0x4800e:
        reel.writereellamp( value & 0xff, 4);
        reel.writereellamp( value >> 8, 6);
        if ( reel.lamp_changed[4] || reel.lamp_changed[5] || reel.lamp_changed[6] || reel.lamp_changed[7] )
          parent->do_reel_lamp();
        break;
      case 0x48006:
        if ( (value & 0xf) == 0xc ) {
          triac.write(1 << ((value & 0x30) >> 4));
        } else
          triac.write(0);
        if ( triac.changed ) {
          parent->do_triac();
        }
        break;
    }
//    if ( address == 0x48008 /*|| address == 0x4800a || address == 0x4800c || address == 0x4800e*/ ) {
//      sprintf(buffer, "reel pos %d write %04x to %08x @ %08x", reel.reels[3].pos, value, address, m68k_cpu.pc);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//    }
    address -= 0x48000;
    serial[address] = value >> 8;
    serial[address+1] = value & 0xff;
  } else if ( address >= 0x4c000 && address < 0x4c100 ) {
    address -= 0x4c000;
    mux_array[address] = value >> 8;
    mux_array[address+1] = value & 0xff;
//    sprintf(buffer, "mux data %04x to %08x @ %08x", value, address + 0x4c000, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } 
  if ( (orig_address & parent->write_mask) == parent->write_bp ) {
    if ((value & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = orig_address;
      parent->write_value = value;
      parent->write_count++;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

void SYS5::m68k_write_memory_32(uint address, int value, int cs)
{
int orig_address = address;

  if ( address >= 0x40000 && address < 0x44000 ) {
    address -= 0x40000;
    ram[address] = value >> 24;
    ram[address+1] = (value >> 16 ) & 0xff;
    ram[address+2] = (value >> 8 ) & 0xff;
    ram[address+3] = value & 0xff;
  } else if ( address >= 0x48000 && address < 0x48020 ) {
//    sprintf(buffer, "serial write %08x to %08x @ %08x", value, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
    address -= 0x48000;
    serial[address] = value >> 24;
    serial[address+1] = (value >> 16 ) & 0xff;
    serial[address+2] = (value >> 8 ) & 0xff;
    serial[address+3] = value & 0xff;
  } else if ( address >= 0x4c000 && address < 0x4c100 ) {
    sprintf(buffer, "mux data %08x to %08x @ %08x", value, address, m68k_cpu.pc);
    address -= 0x4c000;
    mux_array[address] = value >> 24;
    mux_array[address+1] = (value >> 16 ) & 0xff;
    mux_array[address+2] = (value >> 8 ) & 0xff;
    mux_array[address+3] = value & 0xff;
  } else if ( address == 0x4800c ) {
    sprintf(buffer, "reel lamp data %08x to %08x @ %08x", value, address, m68k_cpu.pc);
  }
  if ( (orig_address & parent->write_mask) == parent->write_bp ) {
    if ((value & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = orig_address;
      parent->write_value = value;
      parent->write_count++;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

int SYS5::m68k_read_memory_8(uint address, int cs)
{
uint value = 0, orig_address = address;

  if ( address < 0x40000 ) {
    value = rom[address];
  } else if ( address >= 0x40000 && address < 0x44000 ) {
    value = ram[address-0x40000];
  } else if ( address >= 0x46020 && address < 0x46040 ) {
    value = acia.read( (address - 0x46020) >> 1);
//    sprintf(buffer, "acia read8 %02x from %08x @ %08x", value, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x46040 && address < 0x46060 ) {
    value = ptm.read( (address - 0x46040) >> 1);
  } else if ( address >= 0x46060 && address < 0x46080 ) {
    pia.ora_in = ~(matrix[5] | (meter.meter_on << 7));
    value = pia.read( (address - 0x46060) >> 1);
//    sprintf(buffer, "pia read8 %02x from %08x @ %08x", value, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x46080 && address < 0x460a0 ) {
    value = 0xff;
//    sprintf(buffer, "uart3 read8 %02x from %08x @ %08x", value, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x48000 && address < 0x48020 ) {
    if ( address == 0x48013 )
      value = reel.optos << (8 - nbr_reels);
    else
      value = serial[address-0x48000];
//    if ( address == 0x48013 ) {
//      sprintf(buffer, "serial read %02x from %08x @ %08x", value, address, m68k_cpu.pc);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//    }
  } else if ( address >= 0x4c000 && address < 0x4c080 ) {
    value = mux_array[address - 0x4c000];
//    sprintf(buffer, "mux %02x from %08x @ %08x", value, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x4c080 && address < 0x4c100 ) {
    int column = (address - 0x4c080) >> 1;
    switch ( column ) {
      case 0:
        value = ~DIP1;
        break;
      case 1:
        value = ~DIP2;
        break;
      case 2:
        value = ~Rotary;
        break;
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
        value = ~matrix[column-3];
        break;
      default:
        value = 0xff;
        break;
    }
//    sprintf(buffer, "switch %02x(%d) from %08x @ %08x", value & 0xff, column, address, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x4c100 && address < 0x4c120 ) {
    switch ( address ) {
      case 0x4c101:
        if ( tune != 0 ) {
          value = 1 - Sound->Playing(tune);
          if ( value == 1 ) {
            tune = 0;
            busy = false;
          }
        } else
          value = 1;
        if ( value == 1 )
          busy = false;
        value = 0x14 + value;
        break;
//      default:
//        sprintf(buffer, "read8 from %08x @ %08x", address, m68k_cpu.pc);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//        break;
    }
  }

  if ( (orig_address & parent->read_mask) == parent->read_bp ) {
    if ( parent->readbp )
      parent->state = MS_READ_BP;
    parent->read_addr = orig_address;
    parent->read_value = value;
    parent->read_count++;
  }
  return value;
}
//---------------------------------------------------------------------------

int SYS5::m68k_read_memory_16(uint address, int cs)
{
uint value = 0, orig_address = address;

  if ( address < 0x40000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else if ( address >= 0x40000 && address < 0x44000 ) {
    address -= 0x40000;
    value = ram[address];
    value = ram[address+1] + (value << 8);
  } else if ( address >= 0x48000 && address < 0x48020 ) {
    address -= 0x48000;
    switch ( address ) {
      case 0: // Optos ?
        value = serial[0xc];
        value = serial[0xd] + (value << 8);
        break;
      case 2:
        value = serial[0xe];
        value = serial[0xf] + (value << 8);
        break;
      case 4:
        value = ~((matrix[7] << 8) | matrix[6]); // Coin In
        break;
      default:
        value = serial[address];
        value = serial[address+1] + (value << 8);
        break;
    }
//    if ( address == 4 ) {
//      sprintf(buffer, "serial read %04x from %08x @ %08x", value, address + 0x48000, m68k_cpu.pc);
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//    }
  } else if ( address >= 0x4c000 && address < 0x4c100 ) {
    address -= 0x4c000;
    value = mux_array[address];
    value = mux_array[address+1] + (value << 8);
//    sprintf(buffer, "mux %04x from %08x @ %08x", value, address + 0x4c000, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  }
  if ( (orig_address & parent->read_mask) == parent->read_bp ) {
    if ( parent->readbp )
      parent->state = MS_READ_BP;
    parent->read_addr = orig_address;
    parent->read_value = value;
    parent->read_count++;
  }
  return value;
}
//---------------------------------------------------------------------------

int SYS5::m68k_read_memory_32(uint address, int cs)
{
uint value = 0, orig_address = address;

  if ( address < 0x40000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
    value = rom[address+2] + (value << 8);
    value = rom[address+3] + (value << 8);
  } else if ( address >= 0x40000 && address < 0x44000 ) {
    address -= 0x40000;
    value = ram[address];
    value = ram[address+1] + (value << 8);
    value = ram[address+2] + (value << 8);
    value = ram[address+3] + (value << 8);
  } else if ( address >= 0x48000 && address < 0x48020 ) {
    address -= 0x48000;
    if ( address == 0 ) {
      value = serial[0xc];
      value = serial[0xd] + (value << 8);
      value = serial[0xe] + (value << 8);
      value = serial[0xf] + (value << 8);
      int reel_bits = 4 * nbr_reels;
      value = ( value << (32 - reel_bits) ) | ( value >> reel_bits );
    } else {
      value = serial[address];
      value = serial[address+1] + (value << 8);
      value = serial[address+2] + (value << 8);
      value = serial[address+3] + (value << 8);
    }
//    sprintf(buffer, "serial read %08x from %08x @ %08x", value, address + 0x48000, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } else if ( address >= 0x4c000 && address < 0x4c100 ) {
    address -= 0x4c000;
    value = mux_array[address];
    value = mux_array[address+1] + (value << 8);
    value = mux_array[address+2] + (value << 8);
    value = mux_array[address+3] + (value << 8);
//    sprintf(buffer, "mux %08x from %08x @ %08x", value, address + 0x4c000, m68k_cpu.pc);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  } 
/*  } else if ( parent->errors ) {
    parent->read_addr = init_addr;
    parent->read_value = value;
    parent->state = MS_READ_BP;
  }  */
  if ( (orig_address & parent->read_mask) == parent->read_bp ) {
    if ( parent->readbp )
      parent->state = MS_READ_BP;
    parent->read_addr = orig_address;
    parent->read_value = value;
    parent->read_count++;
  }
  return value;
}

//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateStatus(void)
{
  UpdateStatuss(board.status * 0x10);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 1);
}
//---------------------------------------------------------------------------


void __fastcall TSYS5::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateWave(void)
{
  UpdateWaves(board.tune,0);
//  Form1->play2(board.tune, 0);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateReelLamp(void)
{
  UpdateReelLamps(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 256);
}
//---------------------------------------------------------------------------

void __fastcall TSYS5::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------

int __fastcall TSYS5::getpc(void)
{
  return board.m68k_cpu.pc;
}
//---------------------------------------------------------------------------
int __fastcall TSYS5::getmemory(int address)
{
  return board.m68k_read_memory_8(address,0);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::setmemory(int address, Byte value)
{
  board.m68k_write_memory_8(address,value,0);
}
//---------------------------------------------------------------------------
int __fastcall TSYS5::dasm(int pc, char *buffer)
{
  return board.dasm( pc, buffer);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x4000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::UpdateIOReels(void)
{
//  IOMonitor->UpdateReel2(board.reel1 & 0xff,board.reel.reels[0].pos,board.reel.reels[1].pos);
//  IOMonitor->UpdateReel1(board.reel1 >> 8,board.reel.reels[2].pos,board.reel.reels[3].pos);
//  IOMonitor->UpdateReel3(board.reel2 & 0xff,board.reel.reels[4].pos,board.reel.reels[5].pos);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::UpdateIOMeters(void)
{
  IOMonitor->UpdateMeters(board.meter.state);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::UpdateIOTriacs(void)
{
  IOMonitor->UpdateTriacs(board.triac.value);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.ram);
//  board.SaveToStream(ptr);
  board.display.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
//  board.duart.SaveToStream(ptr);
//  board.hoppr1.SaveToStream(ptr);
//  board.hoppr2.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TSYS5::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.ram);
//  board.LoadFromStream(ptr);
  board.display.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
//  board.duart.LoadFromStream(ptr);
//  board.hoppr1.LoadFromStream(ptr);
//  board.hoppr2.LoadFromStream(ptr);
}

