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
#include "mpu4video.h"
#include "shared.h"
#include "sample.h"

extern char buffer[];
#define NO_DEBUG 1

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMPU4VIDEO::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMPU4VIDEO::TMPU4VIDEO(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
  board_type = BARCREST_MPU4VIDEO;
  VideoDebug->SetProcess(this,board_type);
  board.parent = this;
//  Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::Run()
{
int cycles = 0;

  while ( !Terminated ) {
    board.execute();
    cycles += 2;
    if ( cycles > Speed2 ) {
      Sleep(1);
      cycles = 0;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMPU4VIDEO::Run2()
{
int cycles = 0;

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

void __fastcall TMPU4VIDEO::saveram(void)
{
  //TSystem::saveram(board.video,0x20000);
}
//---------------------------------------------------------------------------

void __fastcall TMPU4VIDEO::load_intelhex(void)
{
FILE		*fp;
unsigned int addr;
struct stat statbuf;
Byte	n;
String filename;
int size, count, total_size;
int romsize;

  memset( board.rom, 0xff, sizeof(board.rom));
  addr = 0;
  total_size = 0;
  for ( count = 0; count < ROMList->Count; count++ ) {
	fp = fopen(ROMList->Strings[count].c_str(), "rb");
    if ( fp != NULL ) {
      stat( ROMList->Strings[count].c_str(), &statbuf);
      size = statbuf.st_size;
      romsize = size * 2;

      for ( ; size != 0; size-- ) {
		n = fgetc(fp);
		board.rom[addr] = n;
        addr+=2;
	  }
      fclose(fp);
    }
    total_size += romsize;
    addr = romsize * ( (count+1)/2);
    if ( (count & 1) == 0 )
      addr++;
  }

/*
  if ( !FileExists("rom.hex") ) {
    fp = fopen("rom.hex", "wb");
    for ( count = 0; count < total_size; count++ )
     fputc( rom[count], fp);
     fclose(fp);
  }
*/
}
//---------------------------------------------------------------------------

void __fastcall TMPU4VIDEO::hard_reset(void)
{
  board.refresh = 0;
  board.reset(M68K_CPU_MODE_68000);
  board.ptm.reset();
  board.vdc.reset();
  board.acia.reset();
  board.sound.reset();
}

//---------------------------------------------------------------------------
int MPU4VIDEO::execute(void)
{
int cycles, save;

    switch ( parent->state ) {
      case 0:
        refresh++;
        if ( refresh == 1000 ) {
        }
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = m68k_cpu.pc;
//        save = m68k_read_memory_32(0x8033ce,0);
        cycles = mc68000::execute();
        parent->Total_cycles += cycles;
        divider += cycles;
        while ( divider > 8 ) {
          divider -= 8;
	      ptm.tick(1);
          if ( ptm.t3_timedout )
            ptm.clock1();
          if ( ptm.t1_timedout )
            ptm.clock2();
        }
        vdc.tick(cycles);
        acia.tick(cycles);
        if ( acia.tx_char ) {
          acia.tx_char = false;
          parent->WriteCharToMPU4(acia.tdr);
        }
#ifdef DEBUG
        if ( vdc.row ) {
          sprintf(buffer, "row %d", vdc.rowcounter);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
        }
#endif
        if ( vdc.vblank ) {
#ifdef DEBUG
          sprintf(buffer, "vdc vblank ir[12] = %02x, ir[13] = %02x, mask = %02x", vdc.ir[12], vdc.ir[13], vdc.intmask);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
#endif
          vdc.vblank = false;
        }
#ifdef DEBUG
        if ( vdc.row ) {
          sprintf(buffer, "do row");
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
        }
#endif
        if ( ptm.sr & 0x80 )
          signal_interrupt(1,25);
        else if ( acia.status & 0x80 ) {
          if ( !acia_int ) {
            parent->SetMPU4CTS(true);
            acia_int = true;
          }
#ifdef DEBUG
//          sprintf(buffer, "Video ACIA IRQ Rx %02x ST %02x", acia.rdr, acia.status);
//          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
#endif
          signal_interrupt(2,26);
        } else {
          if ( (acia.status & 0x80) == 0 && acia_int ) {
            parent->SetMPU4CTS(false);
            acia_int = false;
          }
          if ( vdc.regs[0] & vdc.intmask )
            signal_interrupt(3,27);
        }
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
        divider += cycles;
        while ( divider > 16 ) {
          divider -= 16;
	      ptm.tick(1);
          if ( ptm.t3_timedout )
            ptm.clock1();
          if ( ptm.t1_timedout )
            ptm.clock2();
        }
        if ( ptm.sr & 0x80 )
          signal_interrupt(1,25);
        acia.tick(cycles);
        if ( acia.status & 0x80 )
          signal_interrupt(2,26);
        if ( parent->state == MS_STEPPING )
          parent->state = MS_EXECUTE_BP;
      case 2: // Read Break
      case 3: // Write Break
      case 4: // Execute break
        parent->do_debug();
        VideoDebug->Halt();
        parent->state = MS_HALTED;
        break;
      case 5:
        parent->hard_reset();
        parent->state = MS_RUNNING;
        break;
      case 6:
        VideoDebug->RunClick(NULL);
        break;
      default:
        break;
    }

  if ( (refresh % 6000) == 0 && !NoSound )
    UpdateSoundBuffermpu4video(0);

  if ( vdc.row ) {
    gfx_written = false;
    vdc.row = false;
    buildrowtable();
    if ( vdc.rowcounter < 37 )
      parent->do_row();
  }

  if ( vdc.draw ) {
    vdc.draw = 0;
  }

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TMPU4VIDEO::UpdateTrace(void)
{
char buffer[80];
int count;
int addr;
Word pos;

  VideoDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos++ % TRACE_SIZE].pc;
    board.dasm( addr, buffer);
    VideoDebug->Code->Items->Add(buffer);
  }

}
//---------------------------------------------------------------------------
int MPU4VIDEO::m68k_read_immediate_8(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 )
    value = rom[address];
  else if ( address >= 0x800000 && address < 0x810000 )
    value = ram[address-0x800000];
  return value;
}
//---------------------------------------------------------------------------

int  MPU4VIDEO::m68k_read_immediate_16(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else if ( address >= 0x800000 && address < 0x810000 ) {
    address -= 0x800000;
    value = ram[address];
    value = ram[address+1] + (value << 8);
  }
  return value;
}
//---------------------------------------------------------------------------

int  MPU4VIDEO::m68k_read_immediate_32(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 ) {
      value = rom[address];
      value = rom[address+1] + (value << 8);
      value = rom[address+2] + (value << 8);
      value = rom[address+3] + (value << 8);
  } else if ( address >= 0x800000 && address < 0x810000 ) {
      address -= 0x800000;
      value = ram[address];
      value = ram[address+1] + (value << 8);
      value = ram[address+2] + (value << 8);
      value = ram[address+3] + (value << 8);
  }
  return value;
}
//---------------------------------------------------------------------------
/* Read an instruction (16-bit word immediately after PC) */
int  MPU4VIDEO::m68k_read_instruction(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else if ( address >= 0x800000 && address < 0x810000 ) {
    address -= 0x800000;
    value = ram[address];
    value = ram[address+1] + (value << 8);
  }
  return value;
}
//---------------------------------------------------------------------------

/* Write to anywhere */
void MPU4VIDEO::m68k_write_memory_8(uint address, int value, int cs)
{
uint addr = address;

  if ( addr >= 0x800000 && addr <= 0x900000 )
    ram[addr & 0xffff] = value;
  else if ( addr > 0xa00000 && addr < 0xa000ff ) {
    if ( addr == 0xa00003 )
      palettepos = value;
    else {
      palette[palettepos] = value;
      if ( ++palettepos >= 32 ) {
#ifdef DEBUG
        sprintf(buffer, "Palette written");
        DataWindow->AddMessage(AnsiString(buffer), 0xb0);
#endif
        paletteupdated = true;
        palettepos = 0;
      }
    }
  } else if ( addr >= 0xb00000 && addr <= 0xb000ff ) {
      vdc.write( (address & 0xff) >> 1, value);
#ifdef DEBUG
      if ( (address & 2) == 0 && vdc.pos == 14 ) {
        sprintf(buffer, "ir[13] now %02x", vdc.ir[13]);
        DataWindow->AddMessage(AnsiString(buffer), 0xb0);
      }
      if ( vdc.maskchanged ) {
        sprintf(buffer, "intmask now %02x", vdc.intmask);
        DataWindow->AddMessage(AnsiString(buffer), 0xb0);
      }
#endif
  } else if ( addr >= 0xc00000 && addr < 0xc20000 )
    video[address & 0x1ffff] = value;
  else {
    switch ( address & 0xffffff00 ) {
      case 0x00900000:
        if ( address & 2 )
          sound.control(value);
        else
          sound.data(value);
        break;
      case 0xffff8000:
        acia.write( (address & 0xf) >> 1, value);
        acia.set_dcd(acia.rts);
        if ( acia.rts_changed ) {
          parent->SetMPU4DCD(acia.rts);
        }
        if ( acia.tx_char ) {
          acia.tx_char = false;
          parent->WriteCharToMPU4(acia.tdr);
        }
        break;
      case 0xffff9000:
        ptm.write( (address & 0xf) >> 1, value);
        break;
      case 0xffffd000: // CHR ?
        break;
      case 0x00ffd000: // Q chr ?
        break;
    }
  }

  if ( (address & parent->write_mask) == parent->write_bp ) {
    if ((value & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = address;
      parent->write_value = value;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

void MPU4VIDEO::m68k_write_memory_16(uint address, int value, int cs)
{
int addr = address;

  if ( addr >= 0xc00000 && addr < 0xc20000 ) {
    address &= 0x1ffff;
    video[address] = value >> 8;
    video[address+1] = value & 0xff;
  } else if ( addr >= 0x800000 && addr < 0x900000 ) {
    address &= 0xffff;
    ram[address] = value >> 8;
    ram[address+1] = value & 0xff;
  }
  if ( (addr & parent->write_mask) == parent->write_bp ) {
    if ((value & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = addr;
      parent->write_value = value;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

void MPU4VIDEO::m68k_write_memory_32(uint address, int value, int cs)
{
int addr = address;

  if ( addr >= 0xc00000 && addr < 0xc20000 ) {
    address &= 0x1ffff;
    video[address] = value >> 24;
    video[address+1] = (value >> 16 ) & 0xff;
    video[address+2] = (value >> 8 ) & 0xff;
    video[address+3] = value & 0xff;
#if DEBUG
    if ( !gfx_written ) {
      gfx_written = true;
      sprintf(buffer, "gfx written");
      DataWindow->AddMessage(AnsiString(buffer), 0xb0);
    }
#endif
  } else if ( addr >= 0x800000 && addr < 0x900000 ) {
    address &= 0xffff;
    ram[address] = value >> 24;
    ram[address+1] = (value >> 16 ) & 0xff;
    ram[address+2] = (value >> 8 ) & 0xff;
    ram[address+3] = value & 0xff;
  }
  if ( (addr & parent->write_mask) == parent->write_bp ) {
    if ((value & parent->write_value_mask) == parent->write_value_bp ) {
      parent->write_addr = addr;
      parent->write_value = value;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
  }
}
//---------------------------------------------------------------------------

int MPU4VIDEO::m68k_read_memory_8(uint address, int cs)
{
uint value = 0, addr = address;

  if ( addr < 0x800000 )
    value = rom[address];
  else if ( addr < 0x900000 )
    value = ram[address & 0xffff];
  else if ( addr >= 0xc00000 && addr < 0xc20000 )
    value = video[address & 0x1ffff];
  else if ( addr >= 0xb00000 && addr <= 0xb000ff ) {
    value = vdc.read( (address & 0xf) >> 1);
  } else {
    switch ( address & 0xffffff00 ) {
      case 0xffff8000:
        value = acia.read( (address & 0xf) >> 1);
        if ( acia.rx_char ) {
          parent->VideoCharReceived();
          acia.rx_char = false;
        }
        break;
      case 0xffff9000:
        value = ptm.read( (address & 0xf) >> 1);
        break;
      case 0xffffd000: // CHR ?
        value = m68k_cpu.dr[0] & 0xff;
        break;
      case 0x00ffd000:
        break;
      default:
        break;
    }
  }
  if ( (addr & parent->read_mask) == parent->read_bp ) {
    if ( parent->readbp )
      parent->state = MS_READ_BP;
    parent->read_addr = addr;
    parent->read_value = value;
    parent->read_count++;
  }
  return value;
}
//---------------------------------------------------------------------------

int MPU4VIDEO::m68k_read_memory_16(uint address, int cs)
{
uint value = 0, addr = address;

  if ( addr < 0x800000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else if ( addr < 0x900000 ) {
    address &= 0xffff;
    value = ram[address];
    value = ram[address+1] + (value << 8);
  } else if ( addr >= 0xc00000 && addr < 0xc20000 ) {
    address &= 0x1ffff;
    value = video[address];
    value = video[address+1] + (value << 8);
  }
  if ( (addr & parent->read_mask) == parent->read_bp ) {
    if ( parent->readbp )
      parent->state = MS_READ_BP;
    parent->read_addr = addr;
    parent->read_value = value;
    parent->read_count++;
  }
  return value;
}
//---------------------------------------------------------------------------

int MPU4VIDEO::m68k_read_memory_32(uint address, int cs)
{
uint value = 0, addr = address;

  if ( addr < 0x800000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
    value = rom[address+2] + (value << 8);
    value = rom[address+3] + (value << 8);
  } else if ( addr < 0x900000 ) {
    address &= 0xffff;
    value = ram[address];
    value = ram[address+1] + (value << 8);
    value = ram[address+2] + (value << 8);
    value = ram[address+3] + (value << 8);
  } else if ( addr >= 0xc00000 && addr < 0xc20000 ) {
    address &= 0x1ffff;
    value = video[address];
    value = video[address+1] + (value << 8);
    value = video[address+2] + (value << 8);
    value = video[address+3] + (value << 8);
  }
  if ( (addr & parent->read_mask) == parent->read_bp ) {
    if ( (value & parent->read_value_mask) == parent->read_value_bp ) {
      parent->read_addr = addr;
      parent->read_value = value;
      if ( parent->readbp )
        parent->state = MS_READ_BP;
    }
  }
  return value;
}
//---------------------------------------------------------------------------
void MPU4VIDEO::buildrowtable()
{
uint display_start = (vdc.regs[6] + 256 * vdc.regs[7]) << 1;
Byte lo_byte, hi_byte;
uint row;
int pos = 0;

  for ( int i = 0; i < 37; i++ ) {
    lo_byte = ram[display_start + 1];
    hi_byte = ram[display_start + 3];
    rowtable[pos++] = lo_byte + 256 * hi_byte;
    hi_byte &= 0x3f;
    display_start += 4;
    row = (lo_byte + 256 * hi_byte) << 1;
    for ( int k = 0; k < 63; k++ ) {
      Word addr = ram[row+1] + 256 * ram[row];
      row += 2;
      rowtable[pos++] = addr;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::UpdateDebug(void)
{
  VideoDebug->Debug_Display();
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::UpdateScreen(void)
{
  UpdateScreens(board.video, board.rowtable);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::UpdateRow(void)
{
  UpdateRows(board.palette, board.video, board.rowtable, board.paletteupdated, board.vdc.rowcounter);
  board.paletteupdated = false;
}
//---------------------------------------------------------------------------


void __fastcall TMPU4VIDEO::UpdateWave(void)
{
}
//---------------------------------------------------------------------------

int __fastcall TMPU4VIDEO::getpc(void)
{
  return board.m68k_cpu.pc;
}
//---------------------------------------------------------------------------
int __fastcall TMPU4VIDEO::getstack(void)
{
  return board.m68k_cpu.ar[7];
}
//---------------------------------------------------------------------------
int __fastcall TMPU4VIDEO::getstacktop(void)
{
  return 0x803bd2;
}
//---------------------------------------------------------------------------
int __fastcall TMPU4VIDEO::getmemory(int address)
{
  return board.m68k_read_memory_8(address,0);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::setmemory(int address, Byte value)
{
  board.m68k_write_memory_8(address,value,0);
}
//---------------------------------------------------------------------------
int __fastcall TMPU4VIDEO::dasm(int pc, char *buffer)
{
  return board.dasm( pc, buffer);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x10000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.ram);
//  board.SaveToStream(ptr);
//  board.duart.SaveToStream(ptr);
//  board.hoppr1.SaveToStream(ptr);
//  board.hoppr2.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4VIDEO::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.ram);
//  board.LoadFromStream(ptr);
//  board.duart.LoadFromStream(ptr);
//  board.hoppr1.LoadFromStream(ptr);
//  board.hoppr2.LoadFromStream(ptr);
}

