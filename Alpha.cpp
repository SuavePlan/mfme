//
//
//	alpha.cpp
//
//	(C) C.J.Wren 2000
//
#include <classes.hpp>
#pragma hdrstop

//#include "typedefs.h"
#include <stdio.h>
#include "system.h"
#include "alpha.h"

//#define LOG_ALPHA

#ifdef LOG_ALPHA
#include <stdio.h>
FILE *stream;
#endif

extern char buffer[];

alpha::alpha()
{
	reset();
#ifdef LOG_ALPHA
    stream = fopen("alpha.txt", "w+");
#endif
}

alpha::~alpha()
{
#ifdef LOG_ALPHA
    fclose(stream);
#endif
}

void alpha::reset(void)
{
	Byte	count;

	clk = 0;		// Clear Clk bit
	ch_pos = 15;
	clks = 0;
    nbr_digits = 0;

  // Reset display RAM
	for ( count = 0; count < 16; count++ )
      chs[count] = 0x20;

    intensity = 0;
    ch = 0;
}

void alpha::write(Byte data)
{
  if ( data & 0x08) {
    if ( !enabled ) {
      enabled = true;
    }
  } else {
    if ( enabled ) {
      reset();
      display_changed++;
      intensity_changed++;
    }
    enabled = false;
    return;
  }
  if ( (data & 0x10) != clk ) {
    write_clock(data & 0x10, (data & 0x20) >> 5);
    clk = data & 0x10;
  }
}

void alpha::write_all(Byte Reset, Byte Clock, Byte Data)
{
  Data = Data ? 1 : 0;
  Clock = Clock ? 1 : 0;
  
  if ( Reset ) {
    if ( !enabled ) {
      enabled = true;
    }
  } else {
    if ( enabled ) {
      reset();
      display_changed++;
      intensity_changed++;
    }
    enabled = false;
    clk = Clock;
    return;
  }
  if ( Clock != clk ) {
    write_clock(Clock, Data);
    clk = Clock;
  }
}

void alpha::write_clock(Byte c, Byte data)
{

  if ( clk != c && enabled ) {
    if ( !c ) {
      ch = ( ch << 1 ) | data;
      clks++;
      if ( clks == 8 ) {
        clks = 0;
        do_char(ch);
      }
    }
    clk = c;
  }
}

void alpha::writebuf(Byte *ptr, Byte length)
{
int i;

  for ( i = 0; i < length; i++)
    do_char(*ptr++);

}

void alpha::writejpm(Byte data)
{
  if ( !(data & 0x04) ) {
    if ( enabled ) {
      reset();
      enabled = false;
    }
  } else {
    enabled = true;
    write_clock(data & 1, (data >> 1) & 1);
  }
}

void alpha::enable(Byte e)
{
   if ( e ) {
#ifdef LOG_ALPHA
    fprintf( stream, "Reset\n");
#endif
    enabled = true;
   } else {
    enabled = false;
	reset();
    display_changed++;
    intensity_changed++;
   }
}

void alpha::do_char(Byte x)
{
Byte command;
Byte ch;
//Byte count;
bool update = false;

  if ( enabled ) {
    command = x & 0xe0;
    switch ( command ) {
		  case 0xE0:  // Set Duty Period
            ch = x & 0x1f;
#ifdef LOG_ALPHA
            fprintf( stream, "Intensity %d\n", ch);
#endif
            if ( intensity != ch ) {
              intensity = ch;
              intensity_changed++;
            }
			break;
		  case 0xA0:  // Set RAM Position
            ch_pos = x & 0x0f;
#ifdef LOG_ALPHA
            fprintf( stream, "Position %d\n", new_pos);
#endif
			break;
        case 0x80:      // Test Mode
            break;
        case 0xC0:      // Number of digits
            nbr_digits = x & 0x1F;
#ifdef LOG_ALPHA
            fprintf( stream, "Nbr digits %d\n", nbr_digits);
#endif
            if ( nbr_digits == 0 )
              nbr_digits = 16;
            break;
		default:
          x = x & 0x3f;
#ifdef LOG_ALPHA
          fprintf( stream, "Character %d\n", x);
#endif
          if ( !x )
            x = 64;
          if ( x == '.' || x == ',' ) {
#ifdef LOG_ALPHA
              fprintf( stream, "dp\n");
#endif
            if ( !(chs[ch_pos] & 0xff00) )
              update = true;
            chs[ch_pos] |= x << 8;
          } else {
            ch_pos = (ch_pos + 1) & 15;
            if ( (chs[ch_pos] & 0xff ) != x )
              update = true;
            chs[ch_pos] = x;

#ifdef LOG_ALPHA
              fprintf( stream, "Position now %d\n", ch_pos);
#endif
          }
          break;
	}
    if ( update ) {
      display_changed++;
//      ch = 0;
//      if ( !reversed ) {
//        for ( count = 0; count < 16; count++ ) {
//          display[ch++] = chs[15-count] & 0xff;
//          if ( chs[15-count] & 0xff00 )
//            display[ch++] = chs[15-count] >> 8;
//        }
//      } else {
//        for ( count = 0; count < 16; count++ ) {
//          display[ch++] = chs[count] & 0xff;
//          if ( chs[count] & 0xff00 )
//            display[ch++] = chs[count] >> 8;
//        }
//      }
//      display[ch] = 0;
    }
  }// else
//    printf("char while disabled\r\n");
}

void __fastcall alpha::SaveToStream(Byte *&ptr)
{
  savestate(ptr, clk);
  savestate(ptr, ch_pos);
  savestate(ptr, ch);
  savestate(ptr, clks);
  savestate(ptr, nbr_digits);
  savestate(ptr, chs);
  savestate(ptr, enabled);
  savestate(ptr, intensity);
}
void __fastcall alpha::LoadFromStream(Byte *&ptr)
{
  readstate(ptr, clk);
  readstate(ptr, ch_pos);
  readstate(ptr, ch);
  readstate(ptr, clks);
  readstate(ptr, nbr_digits);
  readstate(ptr, chs);
  readstate(ptr, enabled);
  readstate(ptr, intensity);
  display_changed++;
  intensity_changed++;
//  Fruit->do_display();
}

