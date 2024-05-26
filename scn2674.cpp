//
//
//	scn2674.cpp
//
//	(C) C.J.Wren 2004
//

#include "typedefs.h"
#include "scn2674.h"

#define ROWTIMER 4500

scn2674::scn2674()
{
    timer = 0;
    rowcounter = 36;
	reset();
}

void scn2674::reset()
{                                                                                                  
  pos = 0;
  for ( int i = 0; i < 15; i++ )
    ir[i] = 0;
  regs[0] = 0;
  regs[1] = SCN_RDFLG;
  delayed = false;
  rowtimer = 0;
  graphics = false;
  display  = false;
}

Byte scn2674::read(int addr)
{
  return regs[addr];
}

void scn2674::write(int addr, Byte data)
{
bool value;

  Byte oldmask = intmask;
  switch ( addr ) {
    case 0:
      ir[pos++] = data;
      if ( pos > 14 )
        pos = 14;
      break;
    case 1:
      switch ( data ) {
        case 0:
          reset();
          break;
        case 0xa4:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xa2:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xa9:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xac:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xaa:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xad:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xab:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xbb:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        case 0xbd:
          delayed = true;
          regs[1] &= ~SCN_RDFLG;
          break;
        default:
          if ( (data & 0xf0) == 0x10 ) {
            pos = data & 0xf;
          } else {
            switch ( data & 0xe0 ) {
              case 0x20:
                if ( data & 1 )
                  value = true;
                else
                  value = false;
                if ( data & 2 )
                  graphics = value;
                if ( data & 0x10 )
                  cursor = value;
                if ( data & 8 ) {
                  display = value;
                  if ( data & 4 )
                    next_field = true;
                  else
                    next_field = false;
                }
                break;
              case 0x40: // Reset Interrupt/Status
                regs[0] &= ~( data & 0x1f);
                regs[1] &= ~( data & 0x1f);
                break;
              case 0x80: // Disable Interrupt
                intmask &= ~( data & 0x1f);
                break;
              case 0x60: // Enable Interrupt
                intmask |= ( data & 0x1f);
                break;
            }
          }
          break;
      }
      break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
      regs[addr] = data;
      break;
  }
  maskchanged = oldmask ^ intmask;
}

void scn2674::tick(int cycles)
{
  rowtimer -= cycles;
  if ( rowtimer <= 0 && (intmask & 0x10) ) {
    rowtimer += ROWTIMER;
    rowcounter++;
    regs[1] |= SCN_LINEZERO;
    if ( rowcounter == ir[12] ) {
      regs[1] |= SCN_SPLIT1;
      if ( intmask & SCN_SPLIT1 )
        regs[0] |= SCN_SPLIT1;
    } else if ( rowcounter == ir[13] ) {
      regs[1] |= SCN_SPLIT2;
      if ( intmask & SCN_SPLIT2 )
        regs[0] |= SCN_SPLIT2;
    }
    timer = 200;
    if ( rowcounter > 36 ) {
      draw = true;
      vblank = true;
      regs[1] |= SCN_VBLANK;
      if ( intmask & SCN_VBLANK )
        regs[0] |= SCN_VBLANK;
      if ( delayed ) {
        delayed = false;
        regs[1] |= (SCN_RDFLG + SCN_READY);
        regs[0] |= SCN_READY;
      }
      rowcounter = 0;
    }
  }
  if ( timer > 0 ) {
    timer -= cycles;
    if ( timer <= 0 )
      row = true;
  }
}

