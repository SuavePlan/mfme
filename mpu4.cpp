#include <classes.hpp>
#pragma hdrstop
//---------------------------------------------------------------------------

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "system.h"
//#include "interface.h"
#include "debug.h"
#include "iomonitor.h"
#include "configuration.h"
#include "mpu4.h"
#include "shared.h"

Byte trackballX[4] = {
  0x0,
  0x40,
  0x50,
  0x10
};

Byte trackballY[4] = {
  0,
  0x20,
  0xa0,
  0x80
};

extern char buffer[];

//---------------------------------------------------------------------------
//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TMPU4::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall TMPU4::TMPU4(String AGame, String AGamePath, TStringList *AROM_list)
	: TSystem(AGame, AGamePath, AROM_list)
{
    board_type = BARCREST_MPU4;
    MainDebug->SetProcess(this, board_type);
    board.parent = this;
//    Suspended = false;
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::Run()
{
int cycles = 0;

  SetReels(&board.reel, 10, 0);
  CreateAutoLamp();

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

void __fastcall TMPU4::Run2()
{
int cycles = 0;

  SetReels(&board.reel, 10, 0);
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

void __fastcall TMPU4::saveram(void)
{
  TSystem::saveram(board.memory, 0x800);
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::load_intelhex(void)
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

  for ( addr = 0x800; addr < 0x1000; addr++ )
    board.memory[addr] = 0;

  filename = Game.SubString(1, Game.Pos(".")) + "ram";

  fp = fopen(filename.c_str(), "rb");
  if (fp) {
    fread( &board.memory[0], 0x800, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::hard_reset(void)
{
  board.refresh = 0;
  board.reset();
  board.ptm.reset();
  board.ptm2.reset();
  board.pia1.reset();
  board.pia2.reset();
  board.pia3.reset();
  board.pia4.reset();
  board.pia4.set_cb1(1);
  board.pia5.reset();
  board.pia6.reset();
  board.pia7.reset();
  board.meter.reset();
  board.display.reset();
  board.decoder.reset();
  board.duart.reset();
  board.hoppr1.reset();
  board.hoppr2.reset();
  board.sample.reset();
  board.acia.reset();
  board.acia.tx_speed = 4000;
  
  board.cycles = 0;
  board.current_sense = 0;
  board.charpos = 0;
  board.page = 0;
  board.LastX = 0;
  board.LastY = 0;
  if (Hopper_Type == 6) {
    board.hoppr1.coin_on_period = 1;
    board.hoppr2.coin_on_period = 1;
  }
}

//---------------------------------------------------------------------------
int MPU4::execute(void)
{
	int cycles;
	Byte	do_int;

    do_int = 0;
    refresh++;
    if ( refresh == 100000 )
      refresh = 0;
    switch ( parent->state ) {
      case MS_RUNNING:
        parent->Stopped = false;
        if ( sample.again ) {
          sample.again = false;
          if ( sample.replay() )
            parent->do_wave();
        }
          sample.update();
        if ( (refresh % 1000) == 0 ) {
          lamp.update();
          seg7.update();
          decoder.update();
//          sample.update();
          serial.update();
          duart.update();
          hoppr1.update();
          hoppr2.update();
          if ( meter.update() )
            parent->do_meter();
          if ( (refresh % 4000) == 0 && LVD_Type == 0 ) {
            fiftyHz = fiftyHz ^ 4;
            pia2.set_ca1(fiftyHz == 4);
          }
          if ( cointimer ) {
            cointimer--;
            if ( cointimer == 15 ) {
              matrix[5] &= ~coinTag;
            }
          }
        }
/*        if ( parent->reeltimer ) {
          parent->reeltimer--;
          if ( !parent->reeltimer )
            parent->do_reel();
        }  */
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l;
        parent->trace[parent->sts].page = page;
        cycles = mc6809::execute();
        parent->Total_cycles += cycles;
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
        if ( metertimer ) {
          if ( metertimer >= cycles )
            metertimer -= cycles;
          else
            metertimer = 0;
          if ( !metertimer )
            writemeterport();
        }
	    ptm.tick(cycles);
        acia.tick(cycles);
        if ( acia.tx_char ) {
          acia.tx_char = false;
          parent->WriteCharToVideo(acia.tdr);
        }
        if ( MPU4_TYPE == 1 ) {
          if ( acia.status & 0x80 ) {
            if ( !acia_int ) {
              acia_int = true;
            }
            irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
            interrupt();
          } else {
            irq_state[M6809_IRQ_LINE] = M6809_CLEAR_LINE;
            if ( acia_int ) {
              acia_int = false;
            }
          }
        }
        if ( ptm.sr & 0x80 )
          do_int |= 1;
        if ( ((ptm.sr & 2) && do_int) && !(cc & 0x10) && Program_Type == 1 ) {
          serial.shiftbit();
          pia2.set_cb1(serial.in);
        }
        if ( (( pia2.cra & 0x81 ) == 0x81) | (( pia2.cra & 0x68 ) == 0x48 ) )
          do_int |= 2;
        if ( (( pia2.crb & 0x81 ) == 0x81) | (( pia2.crb & 0x68 ) == 0x48 ) )
          do_int |= 4;
//        if ( ( pia2.cra & 0xc0 ) | ( pia2.crb & 0xc0 ) )
//          do_int |= 1;
        if ( parent->state == MS_READ_BP )
          break;
	    if ( do_int ) {
          if ( MPU4_TYPE == 0 )
            irq_state[M6809_IRQ_LINE] = M6809_ASSERT_LINE;
          else
            irq_state[M6809_FIRQ_LINE] = M6809_ASSERT_LINE;
		  interrupt();
        } else {
          irq_state[M6809_IRQ_LINE] = M6809_CLEAR_LINE;
          irq_state[M6809_FIRQ_LINE] = M6809_CLEAR_LINE;
        }
        if ( pc.w.l == parent->bp && page == parent->page_bp )
          parent->state = MS_EXECUTE_BP;
        break;
      case MS_STEPPING:
        parent->sts = ++parent->sts % TRACE_SIZE;
        parent->trace[parent->sts].pc = pc.w.l + page * 0x10000;
	    cycles = mc6809::execute();
        parent->Total_cycles += cycles;
        parent->trace[parent->sts].a  = dd.b.h;
        parent->trace[parent->sts].b  = dd.b.l;
        parent->trace[parent->sts].x  = x.w.l;
        parent->trace[parent->sts].y  = y.w.l;
        parent->trace[parent->sts].u  = u.w.l;
        parent->trace[parent->sts].s  = s.w.l;
	    ptm.tick(cycles);
        if ( ptm.sr & 0x80 )
          do_int |= 1;
	    if ( do_int )
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
      UpdateSoundBuffermpu4(0);

  return cycles;
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateTrace(void)
{
char buffer[200];
int count;
int addr;
int page;
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
    strncat( buffer, "                                        ", 40 - strlen(buffer));
    sprintf( args, "   %02x %02x %04x %04x %04X %04X",
             trace[pos].a, trace[pos].b,trace[pos].x,trace[pos].y,trace[pos].u,trace[pos].s);
    strcat( buffer, args);
    MainDebug->Code->Items->Add(buffer);
    pos = ( pos + 1 ) % TRACE_SIZE;
  }

  board.page = old_page;

}
//---------------------------------------------------------------------------

Byte MPU4::read(Word addr)
{
Byte ret = 0;
int chr;

  if ( addr < 0x800 )
    ret = memory[addr];
  else if ( addr >= 0x1000 )
    ret = memory[addr + page * 0x10000];
  else
	switch ( addr & 0xfff0 ) {
        case 0x0800:
          if ( MPU4_TYPE == 1 ) {
            ret = acia.read(addr - 0x800);
            if ( acia.rx_char ) {
              parent->MPU4CharReceived();
              acia.rx_char = false;
            }
          } else {
            if ( Config->CharType->ItemIndex == 1 )
              ret = bwb_chr.read();
            else {
              switch ( addr & 3 ) {
                case 0:  /* Character PROM */
                  if ( CharLoaded ) {
                    ret = Character[charpos];
                  } else {
                    ret = memory[x.w.l + page * 0x10000];
                    Character[charpos] = ret;
                  }
//                charpos = (charpos + 1 ) % 64;
                  break;
                case 1:
                  ret = memory[u.w.l + page * 0x10000]; // JNE empire
                  break;
                case 3:
                  switch ( patch ) {
                    case 0:
                      ret = Character[64] & 0xfc;
                      break;
                    case 1:
                      ret = Character[65] & 0xfc;
                      break;
                    case 4:
                      ret = Character[66] & 0xfc;
                      break;
                    case 9:
                      ret = Character[67] & 0xfc;
                      break;
                    case 16:
                      ret = Character[68] & 0xfc;
                      break;
                    case 25:
                      ret = Character[69] & 0xfc;
                      break;
                    case 36:
                      ret = Character[70] & 0xfc;
                      break;
                    case 49:
                      ret = Character[71] & 0xfc;
                      break;
                  }
                  break;
                }
             }
          }
          break;
        case 0x0810:
          switch ( addr & 7 ) {
            case 4:  /* Character PROM */
              if ( CharLoaded ) {
                ret = Character[charpos];
              } else {
                ret = memory[x.w.l];
                Character[charpos] = memory[x.w.l];
              }
              charpos = (charpos + 1 ) % 64;
              break;
            case 1:
              ;
              break;
            case 2:
              ;
              break;
            case 3:
              switch ( patch ) {
                case 0:
                  ret = Character[64];
                  break;
                case 1:
                  ret = Character[65];
                  break;
                case 4:
                  ret = Character[66];
                  break;
                case 9:
                  ret = Character[67];
                  break;
                case 16:
                  ret = Character[68];
                  break;
                case 25:
                  ret = Character[69];
                  break;
                case 36:
                  ret = Character[70];
                  break;
                case 49:
                  ret = Character[71];
                  break;
              }
              break;
          }
          break;
        case 0x850:
          ret = memory[addr];// | page;
          break;
        case 0x858:
          printf("??\n");
          break;
        case 0x880:
          pia7.orb_in = (sample.nar << 7) + (sample.busy << 6);
          ret = pia7.read(addr & 3);
          break;
		case 0x08c0:
          ret = ptm2.read(addr & 7);
          break;
        case 0x8e0:
          if ( Hopper_Type == 1 || Hopper_Type == 3 || Hopper_Type == 5 )
            ret = duart.read(addr & 15);
//          if ( addr == 0x8ed )
//            ret = 0x10;
          break;
		case 0x0900:
          ret = ptm.read(addr & 7);
          break;
        case 0x0A00:		// IC 3
          ret = pia1.read(addr & 3);
          break;
        case 0x0B00:		// IC 4
                   // CB1 goes to IC25 p8
/*          if ( decoder.enabled ) {
            if ( pia1.ora | pia1.orb )
              current_sense = 2;
            else
              current_sense = 0;
          } else
            current_sense = 0;    */

          pia2.orb_in = (/*pia2.cb2 ? */reel.optos/* : 0x78*/ ) + current_sense + fiftyHz + (serial.in << 7); //| 2;  PB3 -PB6
          pia2.set_cb1(serial.in); // *****
          ret = pia2.read(addr & 3);
          break;
        case 0x0C00:		// IC 5
          if ( !MPU4_TYPE ) {
            if ( !(Auxinvert & 2) )
              pia3.ora_in = matrix[4];
            else
              pia3.ora_in = ~matrix[4];
          } else {
            pia3.ora_in = trackballX[trackX & 3] | trackballY[trackY & 3] | speedup1 | speedup2;
            if ( !trackdelay ) {
              trackdelay = 1;
              int xmoves = 0;
              int ymoves = 0;
              parent->UpdateMouse();
              if ( CursorX > LastX ) {
                if ( CursorX - LastX > 2 )
                  speedup1 = 8;
                xmoves++;
                ymoves++;
                LastX++;
              } else if ( CursorX < LastX ) {
                if ( LastX - CursorX > 2 )
                  speedup1 = 4;
                xmoves--;
                ymoves--;
                LastX--;
              } else {
                speedup1 = 0;
              }
              if ( CursorY > LastY ) {
                if ( CursorY - LastY > 2 )
                  speedup2 = 4;
                xmoves++;
                ymoves--;
                LastY++;
              } else if ( CursorY < LastY ) {
                if ( LastY - CursorY > 2 )
                  speedup2 = 8;
                xmoves--;
                ymoves++;
                LastY--;
              } else {
                speedup2 = 0;
              }
              if ( xmoves > 0 )
                trackX++;
              else if ( xmoves < 0 )
                trackX--;
              if ( ymoves > 0 )
                trackY++;
              else if ( ymoves < 0 )
                trackY--;
            } else
              trackdelay--;
          }
          if ( Extender_Type == 2 ) {
            if ( lamp.sense && decoder.enabled )
              pia3.ora_in = 0x40;
            else
              pia3.ora_in = 0;
          }
          if ( Hopper_Type == 4 )
            pia3.ora_in = pia3.ora_in + hoppr1.optoread(0x4);
          else if (Hopper_Type == 6) {
            pia3.ora_in = pia3.ora_in + hoppr1.optoread(0x08) | hoppr2.optoread(0x04);
          }
          if ( !(Auxinvert & 1) )
            pia3.orb_in = matrix[5];
          else
            pia3.orb_in = ~matrix[5];
          if ( Hopper_Type == 2 )
            pia3.orb_in |= hoppr1.optoread(0x8);
          ret = pia3.read(addr & 3);
          if ( (addr & 3) == 0 && Extender_Type == 2 && ( pia3.cra & 4) ) {
            if ( lamp.sense && decoder.enabled )
              ret |= 0x40;
            else
              ret &= ~0x40;
          }
          break;
        case 0x0D00:		// IC 6
          ret = pia4.read(addr & 3);
          break;
        case 0x0E00:		// IC 7
          if ( meter.meter_on )
            pia5.orb_in = 0x80;
          else
            pia5.orb_in = 0;
          ret = pia5.read(addr & 3);
          break;
        case 0x0F00:		// IC 8
          switch ( decoder.column ) {
            case 0:
              pia6.ora_in = matrix[decoder.column] | (STkey << 5);
              break;
            case 1:
              pia6.ora_in = matrix[decoder.column] | JPkey | (PCkey << 4);
              break;
            case 2:
              pia6.ora_in = matrix[decoder.column] ^ DoorInvert;
              break;
            case 4:
              pia6.ora_in = matrix[decoder.column-4] | (STkey << 5);
              break;
            case 5:
              pia6.ora_in = matrix[decoder.column-4] | JPkey | (PCkey << 4);
              break;
            case 6:
              pia6.ora_in = DIP1;
              break;
            case 7:
              pia6.ora_in = DIP2;
              break;
            default:
              pia6.ora_in = matrix[decoder.column];
              break;
          }
          ret = pia6.read(addr & 3);
          break;
        default:
          if ( parent->errors )
            parent->state = MS_EXECUTE_BP;
        break;
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

void MPU4::write(Word addr, Byte x)
{
const int back[8] = {
  0, 4, 2, 6, 1, 5, 3, 7
//    0, 4, 2, 3, 1, 5, 6, 7
};
const int back7[8] = {
  3, 1, 5, 6, 4, 2, 0, 7
};

    if ( addr < 0x800 )
      memory[addr] = x;
    else if ( (addr >= 0xb800 && addr < 0xc000) || ( addr >= 0x4000 && addr < 0x5fff ) )  // MPU4 video
      memory[addr] = x;
    else
	  switch ( addr & 0xfff0 ) {
      case 0x800:
        if ( MPU4_TYPE == 1 ) {
          bool send = false;
          if ( (addr - 0x800) ) {
            send = true;
          }
          acia.write(addr - 0x800, x);
        } else {
          if (  Config->CharType->ItemIndex == 1 ) {
            bwb_chr.write(x);
          } else {
          switch ( addr & 7) {
            case 0:   /* Character prom */
              if ( x == 0 )
                charpos = 0;
              else
                charpos = (charpos + 1 ) % 64;
              break;
            case 2:
              patch = x;
              break;
            default:
              printf("??");
              break;
          }
         }
        }
        break;
      case 0x810:
        switch ( addr & 3 ) {
          case 2:   /* Character prom */
            if ( x == 0 )
              charpos = 0;
            break;
          case 1:
            ;
            break;
          case 3:
            ;
            break;
          default:
            printf("??");
            break;
        }
        break;
//      case 0x0858:
//        break;
      case 0x850:
        if ( addr == 0x850 || addr == 0x858) {
//          if ( addr == 0x850 )
            memory[0x850] = x;
          switch ( x & 3 ) {
            case 0:
              page = 0;
              break;
            case 1:
              page = 1;
              break;
            case 2:
              page = 2;
              break;
            case 3:
              page = 3;
              break;
          }
        }
        break;
      case 0x0880:
        if ( Sound_Type != 4 ) {
          pia7.write(addr & 7, x);
          if ( pia7.ora_changed )
            sample.write_latch(pia7.ora & 0x7f);
          if ( pia7.orb_changed )
            sample.write_control(pia7.orb);
          if ( pia7.ca2_changed && !pia7.ca2 )
            sample.reset();
        } else {
          if ( addr & 1 )
            yamaha.write_data(x);
          else
            yamaha.write_reg(x);
        }
        break;
      case 0x08c0:
        ptm2.write(addr & 7, x);
        break;
      case 0x08e0:
        if ( Hopper_Type == 1 )
          duart.write(addr & 15, x);
        break;
	  case 0x0900:			// IC2
        ptm.write(addr & 7, x);
		break;
	  case 0x0A00:			// IC3
		pia1.write(addr & 3, x);
		if ( pia1.ora_updated && decoder.enabled ) {
          if ( MonitorActive )
            parent->do_io_selects();
		  lamp.write( pia1.ora, decoder.column, 0, 10);
        }
        if ( pia1.orb_updated && decoder.enabled ) {
          if ( MonitorActive )
            parent->do_io_selects();
          lamp.write( pia1.orb, decoder.column, 64, 10);
        }
//		if ( pia1.ca2_changed )
//		  ;//	display.write_data( pia1.ca2);
		if ( pia1.cb2_changed ) {
          display.enable(pia1.cb2);
          if ( display.display_changed || display.intensity_changed )
            parent->do_display();
        }
		break;
	  case 0x0B00:			// IC4
		pia2.write(addr & 3, x);
		if ( pia2.ca2_changed ) {
			decoder.write_A1(pia2.ca2);
            if ( MonitorActive )
              parent->do_io_sinks();
            if ( decoder.column == 0 )
              parent->do_lamp();
        }
        if ( pia2.ora_updated && decoder.enabled ) {
          if ( MonitorActive )
            IOMonitor->Update7segs(~pia2.ora);
          if ( (Extender_Type < 2 || Extender_Type == 4) &&
                LED_Type == 0 )
            seg7.write_data( pia2.ora, decoder.column, 0);
        }
        if ( pia2.orb_changed ) {
          if ( Reel_Type != 0 ) {
            if ( Reel_Type != 5 )
              reel.mux = back[(pia2.orb >> 4) & 7];
            else
              reel.mux = back7[(pia2.orb >> 4) & 7];
            if ( reel.pos[reel.mux] == 0 )
              reel.optos = 0x8;
            else
              reel.optos = 0x0;
          }
        }
		break;
	  case 0x0C00:			// IC5
		pia3.write(addr & 3, x);
        if ( pia3.ora_changed ) {   // aux1
          if ( MonitorActive )
            IOMonitor->UpdateAux1(pia3.ora);
          if ( Hopper_Type == 2 )
            hoppr1.optodrive(pia3.ora & 0x10);
          if ( Reel_Type == 5 ) {
            reel.state( pia3.ora & 0xf, 1);
            reel.state( (pia3.ora >> 4) & 0xf, 2);
            if ( reel.changed[1] || reel.changed[2] )
//              if ( !parent->reeltimer )
//                parent->reeltimer = 200;
              parent->do_reel();
          } else if ( Reel_Type == 6 ) {
            reel.state( pia3.ora & 0xf, 4);
            reel.state( (pia3.ora >> 4) & 0xf, 5);
            if ( reel.changed[4] || reel.changed[5] )
//              if ( !parent->reeltimer )
//                parent->reeltimer = 200;
              parent->do_reel();
          }
        }
        if ( pia3.ora_updated ) {
          switch ( Extender_Type ) {
            case 0:
              if ( LED_Type == 2 )
                seg7.write_latch(pia3.ora & 0x1f, pia2.ora, decoder.column);
              else if ( LED_Type != 1 )
                seg7.write_data( pia3.ora, decoder.column, 1);
              break;
            case 1:
              if ( decoder.enabled )
                lamp.write_extender1( pia3.ora);
              break;
            case 2:
              lamp.write_extender2( pia3.ora, decoder.column, decoder.enabled);
              break;
            case 3:
              lamp.write_extender3( pia3.ora, decoder.column, decoder.enabled);
              if ( lamp.toggle && decoder.enabled )
                seg7.write_data2(lamp.last_b7 >> 7, pia2.ora, decoder.column);
              break;
            case 4:
              lamp.write_extender3( pia3.ora, decoder.column, decoder.enabled);
              break;
          }
        }
        if ( pia3.orb_changed ) {  // aux2
          if ( MonitorActive )
            IOMonitor->UpdateAux2( matrix[7] | pia3.orb);
          if ( Hopper_Type == 4 ) {
            hoppr1.motordrive(pia3.orb & 1);
            hoppr1.optodrive(pia3.orb & 8);
          } //else if ( Hopper_Type == 5 )
//            hoppr1.optodrive(pia3.orb & 4);
        }
        if ( pia3.orb_updated ) {
          if ( LED_Type == 1 )
            seg7.write_latch(pia3.orb & 0x7, pia2.ora, decoder.column);
        }
        if ( pia3.ca2_updated && Program_Type == 1 ) { // RS232 Send
          serial.write(pia3.ca2,pia3.ca2_changed);
//          char buf[100];
          if (serial.RXComplete) {
            switch (serial.data) {
              case 0x80:
                serial.in = 0x59;
                break;
            }
          }
        } else
          serial.in = pia3.ca2;
        if ( pia3.cb2_changed && !pia3.cb2 ) // AY-3-8913 CS
          gi.write_data(pia4.ora,!pia3.cb2,pia4.cb2,pia4.ca2);
        break;
	  case 0x0D00:			// IC6
		pia4.write(addr & 3, x);
        if ( pia4.cb2_strobed )
          gi.write_data(pia4.ora, !pia3.cb2, 1, pia4.ca2);
        if ( pia4.ca2_changed )   // AY-3-8913 BC1
          gi.write_data(pia4.ora,!pia3.cb2,pia4.cb2,pia4.ca2);
        if ( pia4.cb2_changed )   // AY-3-8913 BDIR
          gi.write_data(pia4.ora,!pia3.cb2,pia4.cb2,pia4.ca2);
		if ( pia4.orb_changed ) {          // Reels
          if ( MonitorActive )
            parent->do_io_reels();
          if ( Reel_Type != 5 ) {
            if ( pia4.orb )
	          reel.write_y(pia4.orb);
            if ( reel.changed[0] || reel.changed[1] )
//              if ( !parent->reeltimer )
//                parent->reeltimer = 200;
              parent->do_reel();
          } else {
	          reel.state(pia4.orb & 0xf, 3);
	          reel.state((pia4.orb >> 4) & 0xf, 4);
            if ( reel.changed[3] || reel.changed[4] )
//              if ( !parent->reeltimer )
//                parent->reeltimer = 200;
              parent->do_reel();
          }
        }
		break;
	  case 0x0E00:			// IC7
		pia5.write(addr & 3, x);
		if ( pia5.ca2_changed ) {
			decoder.write_A0(pia5.ca2);
            if ( MonitorActive )
              parent->do_io_sinks();
          if ( decoder.column == 0 )
              parent->do_lamp();
        }
		if ( pia5.ora_changed ) {           // Reels
          if ( Reel_Type != 5 ) {
            if ( pia5.ora )
			  reel.write_x(pia5.ora);
            if ( reel.changed[2] || reel.changed[3] )
//              if ( !parent->reeltimer )
//                parent->reeltimer = 200;
              parent->do_reel();
            if ( MonitorActive )
              parent->do_io_reels();
          } else {
	          reel.state(pia5.ora & 0xf, 5);
	          reel.state((pia5.ora >> 4) & 0xf, 6);
            if ( reel.changed[5] || reel.changed[6] )
//              if ( !parent->reeltimer )
//                parent->reeltimer = 200;
              parent->do_reel();
          }
        }
//        if ( pia5.cb2_changed ) {
//          if ( MonitorActive )
//            parent->do_io_meters();
//        }
        if ( pia5.orb_changed || pia5.cb2_changed ) {            // Meters
          meterport = (pia5.orb & 0x7f) | (pia5.cb2 << 7);
          metertimer = 300;
        }
		break;
	  case 0x0F00:			// IC8
		pia6.write(addr & 3, x);
        if ( pia6.ca2_changed ) {
			decoder.write_A2( pia6.ca2);
            if ( MonitorActive )
              parent->do_io_sinks();
          if ( decoder.column == 0 )
              parent->do_lamp();
        }
		if ( pia6.orb_changed ) {  // Triacs
          if ( MonitorActive )
            parent->do_io_triacs();
          if ( Hopper_Type == 3 )
            duart.opto_drive(pia6.orb & 4, pia6.orb & 1, 0, 0);
          else if ( Hopper_Type == 5 )
            duart.opto_drive(pia6.orb & 4, pia6.orb & 1, pia6.orb & 4, pia6.orb & 2);
          triac.write(pia6.orb);
          if ( triac.changed )
            parent->do_triac();
        }
		if ( pia6.cb2_changed ) {
          display.write_clock(pia6.cb2,pia1.ca2);
          if ( display.display_changed || display.intensity_changed )
            parent->do_display();
        }
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

void MPU4::writemeterport()
{
  if ( MonitorActive )
    parent->do_io_meters();
  if ( Hopper_Type == 1 )
    duart.hoppermotor(meterport);
  else if ( Hopper_Type == 2 )
    hoppr1.motordrive(meterport & 0x10);
    //      else if ( Hopper_Type == 5 )
    //        duart.opto_drive(1, pia5.orb & 0x10, 0, 0);
//          Byte value = pia5.orb;//(pia5.orb & 0x7f) | (pia5.cb2 << 7);
          else if ( Hopper_Type == 6) {
            hoppr1.motordrive(pia5.orb & 0x10);
            hoppr2.motordrive(pia5.orb & 0x20);
/*
            if (!hoppr1.motorread(1))
              hoppr1.stopcoin();
            if (!hoppr2.motorread(1))
              hoppr2.stopcoin();
*/
}
  switch ( Reel_Type ) {
    case 0:
      if ( Hopper_Type == 0 )
        meter.write(meterport);
      else
        meter.write(meterport & 0xf);
      break;
    case 1:
      if ( MonitorActive )
        IOMonitor->UpdateReel3(meterport, reel.pos[4], 0);
      if ( meterport )
        reel.write_5a(meterport);
      break;
    case 2:
      if ( MonitorActive )
        IOMonitor->UpdateReel3(meterport, reel.pos[4], reel.pos[5]);
      if ( meterport )
        reel.write_6(meterport);
      break;
    case 3:
      if ( MonitorActive )
        IOMonitor->UpdateReel3(meterport, 0, reel.pos[4]);
      if ( meterport & 0xf0 )
        reel.state(meterport >> 4, 4);
      if ( meterport )
        meter.write(meterport & 0x0f);
      break;
    case 4:
      if ( MonitorActive )
        IOMonitor->UpdateReel3(meterport, 0, reel.pos[4]);
      if ( meterport )
        reel.state((meterport>>2)&0xf,4);
      break;
    case 5:
      if ( meterport ) {
        reel.state( (((meterport & 0x80) >> 4) + ((meterport & 0x20) >> 3) + ((meterport & 0x8) >> 2) + (meterport & 1)) & 0xf, 0);
      }
      break;
    case 6:
      if ( meterport ) {
        reel.state( meterport >> 4, 4);
      }
  }
  if ( reel.changed[4] || reel.changed[5] || reel.changed[0] )
//    if ( !parent->reeltimer )
//      parent->reeltimer = 200;
    parent->do_reel();
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateDisplay(void)
{
  UpdateDisplays(&board.display, 1);
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateLedDisplay(void)
{
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateWave(void)
{
  UpdateWaves(board.sample.tune, board.sample.channel);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::UpdateReel(void)
{
  UpdateReels(&board.reel);
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateLamp(void)
{
  UpdateLamps(&board.lamp, 256);
  UpdateLedDisplays(&board.seg7);
}
//---------------------------------------------------------------------------

bool __fastcall TMPU4::AutoLamp(int i, int &reel, int &symbol)
{
  if ( ((i / 8) - ReelLampX) < 3 && ((i % 8) - ReelLampY) < 7 ) {
    reel = (i % 8) - ReelLampY;
    symbol = (i / 8) - ReelLampX + ReelLamp2;
    if ( symbol < 3 )
      return true;
  }
  return false;
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateMeter(void)
{
  UpdateMeters(&board.meter);
}
//---------------------------------------------------------------------------

void __fastcall TMPU4::UpdateTriac(void)
{
  UpdateTriacs(&board.triac);
}
//---------------------------------------------------------------------------
int __fastcall TMPU4::getpc(void)
{
  return board.pc.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TMPU4::getmemory(int address)
{
  if ( address > 0x800 )
    address = address + 0x10000 * board.page;

  return board.memory[address];
}
//---------------------------------------------------------------------------
int __fastcall TMPU4::getstack(void)
{
  return board.s.w.l;
}
//---------------------------------------------------------------------------
int __fastcall TMPU4::getstacktop(void)
{
  return 0x7ff;
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::setmemory(int address, Byte value)
{
  if ( address > 0x800 )
    address = address + 0x10000 * board.page;

  board.memory[address] = value;

}
//---------------------------------------------------------------------------
int __fastcall TMPU4::dasm(int pc, char *buffer)
{

  return board.dasm(pc,board.page,buffer);

}
//---------------------------------------------------------------------------
void __fastcall TMPU4::samplefinished(int Channel)
{
  board.sample.finished(Channel);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::ClearRAM(void)
{
int i;

  for ( i = 0; i < 0x800; i++ )
    board.memory[i] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::UpdateIOMeters(void)
{
  IOMonitor->UpdateMeters((board.pia5.orb & 0x7f) | (board.pia5.cb2 << 7));
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::UpdateIOTriacs(void)
{
  IOMonitor->UpdateTriacs(board.pia6.orb);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::UpdateIOSinks(void)
{
  IOMonitor->UpdateLampSelects(board.decoder.column);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::UpdateIOSelects(void)
{
  IOMonitor->UpdateLampDrives1((board.pia1.orb << 8) + board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::UpdateIOReels(void)
{
  IOMonitor->UpdateReel1(board.pia5.ora, board.reel.pos[2], board.reel.pos[3]);
  IOMonitor->UpdateReel2(board.pia4.orb, board.reel.pos[0], board.reel.pos[1]);
//  IOMonitor->UpdateReel3(board.pia1.ora);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::SaveToStream(Byte *&ptr)
{
  savestatesize(ptr,board.memory,0x800);
  savestate(ptr,board.page);
  savestate(ptr,board.refresh);
  savestate(ptr,board.fiftyHz);
  savestate(ptr,board.current_sense);
  savestate(ptr,board.patch);
  savestate(ptr,board.charpos);
  savestate(ptr,board.bwbcharpos);
  board.SaveToStream(ptr);
  board.pia1.SaveToStream(ptr);
  board.pia2.SaveToStream(ptr);
  board.pia3.SaveToStream(ptr);
  board.pia4.SaveToStream(ptr);
  board.pia5.SaveToStream(ptr);
  board.pia6.SaveToStream(ptr);
  board.pia7.SaveToStream(ptr);
  board.ptm.SaveToStream(ptr);
  board.ptm2.SaveToStream(ptr);
  board.display.SaveToStream(ptr);
  board.seg7.SaveToStream(ptr);
  board.reel.SaveToStream(ptr);
  board.meter.SaveToStream(ptr);
  board.lamp.SaveToStream(ptr);
  board.triac.SaveToStream(ptr);
  board.duart.SaveToStream(ptr);
  board.decoder.SaveToStream(ptr);
  board.hoppr1.SaveToStream(ptr);
  board.hoppr2.SaveToStream(ptr);
  board.gi.SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TMPU4::LoadFromStream(Byte *&ptr)
{
  readstatesize(ptr,board.memory,0x800);
  readstate(ptr,board.page);
  readstate(ptr,board.refresh);
  readstate(ptr,board.fiftyHz);
  readstate(ptr,board.current_sense);
  readstate(ptr,board.patch);
  readstate(ptr,board.charpos);
  readstate(ptr,board.bwbcharpos);
  board.LoadFromStream(ptr);
  board.pia1.LoadFromStream(ptr);
  board.pia2.LoadFromStream(ptr);
  board.pia3.LoadFromStream(ptr);
  board.pia4.LoadFromStream(ptr);
  board.pia5.LoadFromStream(ptr);
  board.pia6.LoadFromStream(ptr);
  board.pia7.LoadFromStream(ptr);
  board.ptm.LoadFromStream(ptr);
  board.ptm2.LoadFromStream(ptr);
  board.display.LoadFromStream(ptr);
  board.seg7.LoadFromStream(ptr);
  board.reel.LoadFromStream(ptr);
  board.meter.LoadFromStream(ptr);
  board.lamp.LoadFromStream(ptr);
  board.triac.LoadFromStream(ptr);
  board.duart.LoadFromStream(ptr);
  board.decoder.LoadFromStream(ptr);
  board.hoppr1.LoadFromStream(ptr);
  board.hoppr2.LoadFromStream(ptr);
  board.gi.LoadFromStream(ptr);
}

