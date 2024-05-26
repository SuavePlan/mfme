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
#include "impact.h"
#include "shared.h"
#include "sample.h"

extern Byte lookup[8][256];

#define RAMTOP 0x410000

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TIMPACT::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TIMPACT::TIMPACT(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
  board_type = JPM_IMPACT;
  MainDebug->SetProcess(this,board_type);
  board.parent = this;
//  Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::Run()
{
int cycles = 0;

  SetReels(&board.reel, 10, 7);
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
void __fastcall TIMPACT::Run2()
{
int cycles = 0;

  SetReels(&board.reel, 10, 7);
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

void __fastcall TIMPACT::saveram(void)
{
  TSystem::saveram(board.ram,0x10000);
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::load_intelhex(void)
{
FILE		*fp;
int		addr;
struct stat statbuf;
Byte	n;
String filename;
int size, count, total_size;

  addr = 0;
  total_size = 0;
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
    total_size += 0x80000;
    switch ( count ) {
      case 0:
        addr = 1;
        break;
      case 1:
        addr = 0x100000;
        break;
      case 2:
        addr = 0x100001;
        break;
      case 3:
        addr = 0x200000;
        break;
      case 4:
        addr = 0x200001;
        break;
    }
  }

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    for ( addr = 0; addr < 0x10000; addr++ ) {
      n = fgetc(fp);
      board.ram[addr] = n;
    }
    fclose(fp);
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

void __fastcall TIMPACT::hard_reset(void)
{
  board.refresh = 0;
  board.reset(M68K_CPU_MODE_68000);
  board.meter.reset();
  board.duart.reset();
  board.sec.reset();
  board.display.reset();
  board.ppi8255.reset();
  board.ppi8255.portc_in = 0xf0;
  board.hopper1.reset();
  board.hopper2.reset();
//  board.cycles = 0;
//  board.current_sense = 0;
  board.busy = 0;
}

//---------------------------------------------------------------------------
int IMPACT::execute(void)
{
int cycles;

    switch ( parent->state ) {
      case 0:
        refresh++;
        if ( refresh == 1000  ) {
          if ( lamp.update() )
            parent->do_lamp();
          if ( seg7.update() )
            parent->do_led_display();
          hopper1.update();
          hopper2.update();
          if ( meter.update() )
            parent->do_meter();
          refresh = 0;
          if ( cointimer ) {
            cointimer--;
            if ( cointimer == 15 ) {
              matrix[9] &= ~coinTag;
            }
          }
        }
//        if ( MainDebug->Traceon ) {
          parent->sts = ++parent->sts % TRACE_SIZE;
          parent->trace[parent->sts].pc = m68k_cpu.pc;
//        }
        cycles = mc68000::execute();
        last_cycles += cycles;
        parent->Total_cycles += cycles;
        while ( last_cycles > 4 ) {
          duart.tick(1);
          last_cycles -= 4;
        }
        if ( duart.isr & duart.imr )
          signal_interrupt( 5, duart.ivr );
        if ( parent->state == MS_READ_BP )
          break;
        if ( m68k_cpu.pc == parent->bp )
          parent->state = MS_EXECUTE_BP;
        break;
      case 1:
//        if ( MainDebug->Traceon ) {
          parent->sts = ++parent->sts % TRACE_SIZE;
          parent->trace[parent->sts].pc = m68k_cpu.pc;
//        }
        cycles = mc68000::execute();
        last_cycles += cycles;
        parent->Total_cycles += cycles;
        while ( last_cycles > 4 ) {
          duart.tick(1);
          last_cycles -= 4;
        }
//        duart.tick(2);
        if ( duart.isr & duart.imr )
          signal_interrupt( 5, duart.ivr );
        if ( parent->state == MS_STEPPING )
          parent->state = MS_EXECUTE_BP;
      case 2:  // Read Break
      case 3:  // Write Break
      case 4:  // Execute break
        parent->do_debug();
        MainDebug->Halt();
        parent->state = MS_HALTED;
        break;
      case 5:  // Reset
        parent->hard_reset();
        parent->state = MS_RUNNING;
        break;
      case 6:  // Resume
        MainDebug->RunClick(NULL);
        break;
      default: // Halted
        break;
    }

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateTrace(void)
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
int IMPACT::m68k_read_immediate_8(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 )
    value = rom[address];
  else
    printf("not from rom\n");
  return value;
}
//---------------------------------------------------------------------------

int  IMPACT::m68k_read_immediate_16(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else
    printf("not from rom\n");
  return value;
}
//---------------------------------------------------------------------------

int  IMPACT::m68k_read_immediate_32(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 ) {
      value = rom[address];
      value = rom[address+1] + (value << 8);
      value = rom[address+2] + (value << 8);
      value = rom[address+3] + (value << 8);
  } else
    printf("not from rom\n");
  return value;
}

/* Read an instruction (16-bit word immeditately after PC) */
int  IMPACT::m68k_read_instruction(uint address, int cs)
{
uint value = 0;

  if ( address < 0x100000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else
    printf("not from rom\n");
  return value;
}

/* Write to anywhere */
void IMPACT::m68k_write_memory_8(uint address, int value, int cs)
{

  if ( address >= 0x400000 && address < RAMTOP )
    ram[address-0x400000] = value;
  else if ( address >= 0x480000 && address < 0x480020 ) {
    duart.write( (address - 0x480000) >> 1, value);
  } else if ( address >= 0x480060 && address < 0x480068 ) {
    ppi8255.write( (address - 0x480060) >> 1, value);
    if ( ppi8255.portc_changed ) {
      display.writejpm(ppi8255.portc);
      if ( display.display_changed || display.intensity_changed )
        parent->do_display();
    }
    if ( ppi8255.porta_changed )  {
      if ( ppi8255.porta & 0x10 )
        payen = 1;
      else
        payen = 0;
      hopper1.enable( (ppi8255.porta & 0x11) == 0x11, ppi8255.porta & 0x80, ppi8255.porta & 0x20);
      hopper2.enable( (ppi8255.porta & 0x50) == 0x50, ppi8255.porta & 0x80, ppi8255.porta & 0x20);
    }
  } else if ( address == 0x480081 ) {
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
  } else if ( address == 0x480083 ) {
    page = (value >> 1) & 7;
    if ( value & 1 ) { // Reset
      tune = 0;
      parent->do_wave();
    }
  } else if ( address == 0x4800ab ) {
    if ( seg7.writejpm(value, source) )
      parent->do_led_display();
  } else if ( address == 0x4800ad ) {
    ;
  } else if ( address == 0x4800af ) {
    if ( value & 0x10 ) {
      source = (value + 1) & 0xf;
//      if ( source == 0 )
//        parent->do_lamp();
    }
  } else if ( parent->errors ) {
    parent->write_addr = address;
    parent->write_value = value;
    parent->state = MS_WRITE_BP;
  }

//  if ( MainDebug->Traceon ) {
    if ( (address & parent->write_mask) == parent->write_bp ) {
      parent->write_addr = address;
      parent->write_value = value;
      parent->write_count++;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
//  }
}
//---------------------------------------------------------------------------

void IMPACT::m68k_write_memory_16(uint address, int value, int cs)
{
  if ( address & 1 )
    printf("Address exception");
  if ( address >= 0x400000 && address < RAMTOP ) {
    address -= 0x400000;
    ram[address] = value >> 8;
    ram[address+1] = value & 0xff;
  } else if ( address == 0x4800a0  ) {
    value = (value & 0x200) >> 9;
    if ( value != status ) {
      status = value;
      parent->do_status();
//      Form1->UpdateStatus( status * 0x10 );
    }
  } else if ( address == 0x4800a2  ) {
    reel1 = value;
    reel.write1( value );
    if ( reel.changed[0] || reel.changed[1] ||
         reel.changed[2] || reel.changed[3] )
      parent->do_reel();
    if ( MonitorActive )
      parent->do_io_reels();
  } else if ( address == 0x4800a4 ) {
    reel2 = value;
    reel.write2( value );
    if ( reel.changed[4] || reel.changed[5] )
      parent->do_reel();
    if ( MonitorActive )
      parent->do_io_reels();
  } else if ( address == 0x4800a6 ) {
    if ( value & 0x10 ) {   // PAYEN ?
      if ( value & 0xf )
        payslide = 1;
      else
        payslide = 0;
    } else
      payslide = 0;
    if ( SECFitted ) {
      sec.enable((~value >> 10) & 0x20);
      sec.setdata((~value >> 10) & 2);
      sec.setclock((~value >> 10) & 1);
      if ( sec.updated )
        parent->do_sec();
    } else {
      meter.write(value >> 10);
      if ( meter.meter_on )
        duart.ip &= ~0x10;
      else
        duart.ip |= 0x10;
      if ( MonitorActive ) {
        parent->do_io_triacs();
       parent->do_io_meters();
      }
    }
  } else if ( address == 0x4800a8 ) {
    if ( lamp.writejpm(value,source,30) )
      parent->do_lamp();
  } else if ( parent->errors ) {
    parent->write_addr = address;
    parent->write_value = value;
    parent->state = MS_WRITE_BP;
  }
//  if ( MainDebug->Traceon ) {
    if ( (address & parent->write_mask) == parent->write_bp ) {
      parent->write_addr = address;
      parent->write_value = value;
      parent->write_count++;
      if ( parent->writebp )
        parent->state = MS_WRITE_BP;
    }
//  }
}
//---------------------------------------------------------------------------

void IMPACT::m68k_write_memory_32(uint address, int value, int cs)
{
  if ( address & 3 )
    printf("Address exception");
  if ( address >= 0x400000 && address < RAMTOP ) {
    address -= 0x400000;
    ram[address] = value >> 24;
    ram[address+1] = (value >> 16 ) & 0xff;
    ram[address+2] = (value >> 8 ) & 0xff;
    ram[address+3] = value & 0xff;
  } else if ( address == 0x4800a2  ) {
    reel.write3( value );
    if ( reel.changed[0] || reel.changed[1] ||
         reel.changed[2] || reel.changed[3] ||
         reel.changed[4] || reel.changed[5] ||
         reel.changed[6] || reel.changed[7] )
      parent->do_reel();
    if ( MonitorActive )
      parent->do_io_reels();
  } else if ( parent->errors ) {
    parent->write_addr = address;
    parent->write_value = value;
    parent->state = MS_WRITE_BP;
  }
  if ( (address & parent->write_mask) == parent->write_bp ) {
    parent->write_addr = address;
    parent->write_value = value;
    parent->write_count++;
    if ( parent->writebp )
      parent->state = MS_WRITE_BP;
  }
}
//---------------------------------------------------------------------------

int IMPACT::m68k_read_memory_8(uint address, int cs)
{
uint value, init_addr = address;

  if ( address < 0x100000 ) {
    value = rom[address];
  } else if ( address >= 0x400000 && address < RAMTOP ) {
    value = ram[address-0x400000];
  } else if ( address >= 0x480000 && address < 0x480020 ) {
    value = duart.read( (address - 0x480000) >> 1);
  } else if ( address >= 0x480060 && address < 0x480068 ) {
    if ( SECFitted )
      ppi8255.portb_in = 0xe0 + hopper1.opto() + (hopper2.opto() << 3) + ( sec.readdata() ? 4 : 0);
    else
      ppi8255.portb_in = 0xe4 + hopper1.opto() + (hopper2.opto() << 3);
    ppi8255.portc_in = (ppi8255.portc_in & 0x20) | Hoppers;
    value = ppi8255.read( (address - 0x480060) >> 1);
  } else if ( address >= 0x480020 && address < 0x480034 ) {
    switch (address) {
      case 0x480021:
        value = ~( DIP1 | matrix[0]);
        break;
      case 0x480023:
        value = ~(PCkey | matrix[1]);
        break;
      case 0x480025:
        value = ~((STkey << 4) | JPkey | matrix[2]) & 0xff;
        break;
      case 0x480033:
        value = ~(matrix[9] | (MechType << 5));
        break;
      default:
        value = ~matrix[(address-0x480020)>>1];
        break;
    }
  } else if ( address == 0x480041 ) {
    value = reel.optos;
  } else if ( address == 0x480085 ) {
//    value = 1 - busy;
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
  } else if ( address == 0x480035 ) {
    value = 0xff;
  } else if ( address >= 0x480085 ) {
    value = 1;
  } else if ( address >= 0x480000 ) {
    value = 0x0;
  } else if ( parent->errors ) {
    parent->read_addr = init_addr;
    parent->read_value = value;
    parent->state = MS_READ_BP;
  }
//  if ( MainDebug->Traceon ) {
    if ( (init_addr & parent->read_mask) == parent->read_bp ) {
      if ( parent->readbp )
        parent->state = MS_READ_BP;
      parent->read_addr = init_addr;
      parent->read_value = value;
      parent->read_count++;
    }
//  }
  return value;
}
//---------------------------------------------------------------------------

int IMPACT::m68k_read_memory_16(uint address, int cs)
{
uint value = 0, init_addr = address;

  if ( address < 0x100000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
  } else if ( address >= 0x400000 && address < RAMTOP ) {
    address -= 0x400000;
    value = ram[address];
    value = ram[address+1] + (value << 8);
  } else if ( parent->errors ) {
    parent->read_addr = init_addr;
    parent->read_value = value;
    parent->state = MS_READ_BP;
  }
//  if ( MainDebug->Traceon ) {
    if ( (init_addr & parent->read_mask) == parent->read_bp ) {
      if ( parent->readbp )
        parent->state = MS_READ_BP;
      parent->read_addr = init_addr;
      parent->read_value = value;
      parent->read_count++;
    }
//  }
  return value;
}
//---------------------------------------------------------------------------

int IMPACT::m68k_read_memory_32(uint address, int cs)
{
uint value = 0, init_addr = address;

  if ( address < 0x100000 ) {
    value = rom[address];
    value = rom[address+1] + (value << 8);
    value = rom[address+2] + (value << 8);
    value = rom[address+3] + (value << 8);
  } else if ( address >= 0x400000 && address < RAMTOP ) {
    address -= 0x400000;
    value = ram[address];
    value = ram[address+1] + (value << 8);
    value = ram[address+2] + (value << 8);
    value = ram[address+3] + (value << 8);
  } else if ( parent->errors ) {
    parent->read_addr = init_addr;
    parent->read_value = value;
    parent->state = MS_READ_BP;
  }
  if ( (init_addr & parent->read_mask) == parent->read_bp ) {
    if ( parent->readbp )
      parent->state = MS_READ_BP;
    parent->read_addr = init_addr;
    parent->read_value = value;
    parent->read_count++;
  }
  return value;
}

//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateStatus(void)
{
  UpdateStatuss(board.status * 0x10);
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 1);
}
//---------------------------------------------------------------------------


void __fastcall TIMPACT::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateWave(void)
{
  UpdateWaves(board.tune,0);
//  Form1->play2(board.tune, 0);
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 256);
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

bool __fastcall TIMPACT::AutoLamp(int i, int &reel, int &symbol)
{
  if ( (i / 16) < 3 && ( i % 16 ) < 6 ) {
    reel = ((i / 16) * 2) + (( i % 16) / 3);
    symbol = 2 - ((i % 16) % 3);
    if ( symbol < 3 )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TIMPACT::UpdateSEC(void)
{
  UpdateSECs(&board.sec);
}
//---------------------------------------------------------------------------

int __fastcall TIMPACT::getpc(void)
{
  return board.m68k_cpu.pc;
}
//---------------------------------------------------------------------------
int __fastcall TIMPACT::getmemory(int address)
{
  return board.m68k_read_memory_8(address,0);
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::setmemory(int address, Byte value)
{
  board.m68k_write_memory_8(address,value,0);
}
//---------------------------------------------------------------------------
int __fastcall TIMPACT::dasm(int pc, char *buffer)
{
  return board.dasm( pc, buffer);
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x4000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::UpdateIOReels(void)
{
  IOMonitor->UpdateReel2(board.reel1 & 0xff,board.reel.reels[0].pos,board.reel.reels[1].pos);
  IOMonitor->UpdateReel1(board.reel1 >> 8,board.reel.reels[2].pos,board.reel.reels[3].pos);
  IOMonitor->UpdateReel3(board.reel2 & 0xff,board.reel.reels[4].pos,board.reel.reels[5].pos);
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::UpdateIOMeters(void)
{
  IOMonitor->UpdateMeters(board.meter.state);
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::UpdateIOTriacs(void)
{
  IOMonitor->UpdateTriacs(board.triac.value);
}
//---------------------------------------------------------------------------
void __fastcall TIMPACT::SaveToStream(Byte *&ptr)
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
void __fastcall TIMPACT::LoadFromStream(Byte *&ptr)
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

