 //---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <sys/stat.h>
#include <stdio.h>
#include <io.h>
#include "system.h"
#include "debug.h"
#include "iomonitor.h"
#include "dotmatrix.h"
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
//      void __fastcall TDOTMATRIX::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TDOTMATRIX::TDOTMATRIX(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = BFM_DOTMATRIX;
    MatrixDebug->SetProcess(this,board_type);
    board.parent = this;
//    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::Run()
{
int cycles = 0;

  board.refresh = 0;
  hard_reset();

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

//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::Run2()
{
int cycles = 0;

  board.refresh = 0;
  hard_reset();

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

void __fastcall TDOTMATRIX::load_intelhex(void)
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
}
//---------------------------------------------------------------------------

void __fastcall TDOTMATRIX::hard_reset(void)
{
  board.reset();
  board.display.reset();
  board.irq = false;
  board.firq = false;
  board.blocks = 0;
  board.bcount = 0;
  
  board.cycles = 0;
  board.chr = 0;
  board.latch = 0;
  for ( int i = 0; i < 9*21; i++ )
    board.map[i] = 0;
}

//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::sendchar(Byte chr)
{
  board.chr = chr;
  board.irq = true;
}

//---------------------------------------------------------------------------
int DOTMATRIX::execute(void)
{
	int cycles;

    refresh++;
    if ( refresh == 100000 )
      refresh = 0;
    switch ( parent->state ) {
      case 0:
        parent->Stopped = false;
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
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
          firq = true;
        }
        if ( parent->state == MS_READ_BP )
          break;
	    if ( firq ) {
          irq_state[M6809_NMI_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
	    if ( irq ) {
          irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
        if ( pc.w.l == parent->bp )
          parent->state = MS_EXECUTE_BP;
        break;
      case 1:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
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
          firq = true;
        }
	    if ( firq ) {
          irq_state[M6809_NMI_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
	    if ( irq ) {
          irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        }
      case 2:
      case 3:
      case 4:
        parent->do_debug();
        MatrixDebug->Halt();
        parent->state = MS_HALTED;
        break;
      case 5:
        parent->hard_reset();
        parent->state = MS_RUNNING;
        break;
      case 6:
        MatrixDebug->RunClick(NULL);
        break;
      default:
        parent->Stopped = true;
        break;
    }

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TDOTMATRIX::UpdateDebug(void)
{
  MatrixDebug->Debug_Display();
}
//---------------------------------------------------------------------------

void __fastcall TDOTMATRIX::UpdateTrace(void)
{
char buffer[80];
int count;
int addr;
Word pos;
char args[50];

  MatrixDebug->Code->Items->Clear();
  pos = sts + 1;
  for ( count = 0; count < TRACE_SIZE; count++ ) {
    addr = trace[pos].pc;
    board.dasm( addr, 0, buffer);
    strncat( buffer, "                                   ", 35 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s);
    strcat( buffer, args);
    MatrixDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }
}
//---------------------------------------------------------------------------

Byte DOTMATRIX::read(Word addr)
{
Byte		ret = 0;
int value;

  if ( addr < 0x2000 )
    ret = ram[addr];
  else if ( addr >= 0x4000 )
    ret = memory[addr];
  else {
    switch ( addr ) {
        case 0x3000:
          ret = queue[readpos++];
          if ( readpos >= 500 )
            readpos = 0;
          if ( readpos == writepos ) {
            irq_state[M6809_IRQ_LINE] = M6809_CLEAR_LINE;
            irq = false;
          }
  //        latch = 0;
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

void DOTMATRIX::write(Word addr, Byte x)
{
  if ( addr < 0x2000 )
    ram[addr] = x;
  else if ( addr < 0x4000 ) {
    switch ( addr ) {
      case 0x2000:
//        if ( ( x & 8 ) != ( status & 8 ) )
//          Form1->UpdateStatus( ((x & 8) << 1) ^ 0x10 );
        status = x;
        break;
      case 0x2800:
        if ( map[pos] != x )
          updated = true;
        map[pos++] = x;
//        segdata[bcount] = x;
        bcount++;
        if ( bcount == 9 ) {
          bcount = 0;
          blocks++;
//          sprintf(buffer, "%02x %02x %02x %02x %02x %02x %02x %02x %02x",
//                  segdata[0],segdata[1],segdata[2],segdata[3],segdata[4],
//                  segdata[5],segdata[6],segdata[7],segdata[8]);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//          sprintf(buffer, "pos = %d blocks = %d Data %02x", pos, blocks, x);
 //         DataWindow->AddMessage((AnsiString)buffer, 0xb0);
          if ( blocks == 21 ) {
            blocks = 0;
            pos = 0;
            if ( updated )
              parent->do_matrix();
            updated = false;
          }
        }
        break;
      case 0x3800:
        irq_state[M6809_NMI_LINE] = M6809_CLEAR_LINE;
        firq = false;
        break;
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
void __fastcall TDOTMATRIX::UpdateMatrix(void)
{
  UpdateMatrixs(board.map);
}
//---------------------------------------------------------------------------
int __fastcall TDOTMATRIX::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TDOTMATRIX::getmemory(int address)
{
  if ( address >= 0x4000 )
    return board.memory[address];
  else
    return board.ram[address];
}
//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::setmemory(int address, Byte value)
{
  if ( address < 0x2000 )
    board.ram[address] = value;
}
//---------------------------------------------------------------------------
int __fastcall TDOTMATRIX::dasm(int pc, char *buffer)
{
  return board.dasm(pc,0,buffer);
}
//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::ClearRAM(void)
{
  for ( int i = 0; i < 0x2000; i++ )
    board.ram[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::SaveToStream(Byte *&ptr)
{
  savestate(ptr,board.memory);
  savestate(ptr,board.refresh);
  savestate(ptr,board.fiftyHz);
  board.SaveToStream(ptr);
  board.display.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,board.memory);
  readstate(ptr,board.refresh);
  readstate(ptr,board.fiftyHz);
  board.LoadFromStream(ptr);
  board.display.LoadFromStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TDOTMATRIX::QueueChar(Byte ch)
{
  board.queue[board.writepos++] = ch;
  if ( board.writepos >= 500 )
    board.writepos = 0;
  board.irq = true;
}
//---------------------------------------------------------------------------

