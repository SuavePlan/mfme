#include <classes.hpp>
#pragma hdrstop
//
//
//	m6376.cpp
//
//	(C) C.J.Wren 1998, 1999, 2000
//
#include "system.h"
//#include "typedefs.h"
#include "m6376.h"
#include "sample.h"
#include "shared.h"

//#define LOG_SOUND
#ifdef LOG_SOUND
    FILE             *stream;
#endif
msm6376::msm6376()
{
	reset();
}

msm6376::~msm6376()
{
#ifdef LOG_SOUND
    fclose(stream);
#endif
}

void msm6376::reset()
{
	nar = 1;
    busy = 1;
    st = 1;
    latch = 0;
    playing[0] = playing[1] = -1;
#ifdef LOG_SOUND
    stream = fopen("sound.txt", "w+");
#endif
}

void msm6376::update(void)
{
  if ( pause > 0 ) {
    pause--;
    if ( pause == 0 ) {
      nar = 1;
    }
  }
}
bool msm6376::replay()
{
  if ( next1 && HasSampledSound ) {
    busy = 0;
    tune = next1;
    next1 = 0;
    playing[0] = tune;
    channel = 0;
    nar = 0;
    pause = 45;
    return true;
  }
  if ( next2 && HasSampledSound ) {
    busy = 0;
    tune = next2;
    next2 = 0;
    playing[1] = tune;
    channel = 1;
    return true;
  }
  return false;
}

void msm6376::finished(Byte Channel)
{
  if ( Channel == 1 && next2 )
    again = true;
  if ( Channel == 0 && (((lastval & 3) == 2 ) || next1) )  {
    next1 = latch1;
    again = true;
  }
  if ( playing[0] == playing[1] )
    playing[1-Channel] = -1;

  playing[Channel] = -1;

  if ( playing[0] == - 1 ) {
    nar = 1;
    if ( playing[1] == -1 ) {
      busy = 1;
    }
  }
//  ((TMPU4 *)Fruit)->board.pia7.set_cb1(nar);
//  ((TMPU4 *)Fruit)->board.pia7.set_ca1(busy);
}

void msm6376::write_latch(Byte val)
{
  latch = val & 0x7f;
}

void msm6376::write_control(Byte val)
{
#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

#endif
  st_changed = false;
  ch2_changed = false;
  clk_changed = false;
  dir_changed = false;

  if ( (lastval & 0x1 ) != ( val & 0x1 ) ) {
    st = val & 1;
    st_changed = true;
#ifdef LOG_SOUND
    fprintf( stream, "%08d ST  %d Latch %d Nar %d Busy %d\n", tickcount, st, latch, nar, busy);
    if ( st == 1 && ch2 == 1 )
      fprintf( stream, "\n");
#endif
  }
  if ( (lastval & 0x2 ) != ( val & 0x2 ) ) {
    ch2 = (val & 2) >> 1;
    ch2_changed = true;
#ifdef LOG_SOUND
    fprintf( stream, "%08d CH2 %d Latch %d Nar %d Busy %d\n", tickcount, ch2, latch, nar, busy);
    if ( ch2 == 1 )
      fprintf( stream, "\n");
#endif
  }
  if ( (lastval & 0x20 ) != ( val & 0x20 ) ) {
    clk = (val & 0x20) >> 5;
    clk_changed = true;
  }
  if ( (lastval & 0x10 ) != ( val & 0x10 ) ) {
    dir = (val & 0x10) >> 4;
    dir_changed = true;
  }


  if ( clk_changed && !clk ) {
    if ( dir ) {
      if ( volume < 32 )
        volume++;
    } else {
      if ( volume > 0 )
        volume--;
    }
  }

  if ( ch2 && !ch2_changed && st_changed ) {
    if ( !st ) {
      latch1 = latch;
      if ( HasSampledSound ) {
        if ( Sound->Details(latch1) ) {
          channel = 0;
//          if ( busy ) {
//            nar = 0;
//            pause = 200;
//          }
          if ( latch1 == 0 ) {
            next1 = 0;
            next2 = 0;
//            busy = 1;
//            nar = 1;
          } else
            busy = 0;
          if ( playing[channel] == -1 || latch1 == 0 || Sound_Type == 2) {
//            ((TMPU4 *)Fruit)->board.pia7.set_ca1(busy);
//            nar = 0;
//            ((TMPU4 *)Fruit)->board.pia7.set_cb1(nar);
//            pause = 100;
            tune = latch1;
            playing[channel] = tune;
            Fruit->do_wave();
          } else {
            if ( nar ) {
              next1 = latch1;
              if ( Sound_Type == 0 )
                nar = 0;
//              ((TMPU4 *)Fruit)->board.pia7.set_cb1(nar);
            }
          }
        }
      }
    } else if ( nar == 1 ) {
        nar = 0;
        pause = 45;
    }
  }

  if ( !ch2 && ch2_changed )
    pulses = 0;

  if ( !ch2 && st_changed ) {
    if ( !st ) {
      if ( !pulses )
        latch2 = latch;
    } else {
      pulses++;
      atten++;
      if ( pulses == 1 )
        atten = 0;
    }
  }

  if ( ch2 && ch2_changed ) {
    tune = latch2;
    if ( HasSampledSound ) {
      if ( Sound->Details(tune) ) {
        busy = 0;
        channel = 1;
        if ( latch2 == 0 ) {
            next1 = 0;
            next2 = 0;
            busy = 1;
        }
        if ( playing[channel] == -1 || latch2 == 0 || Sound_Type != 3 ) {
//          ((TMPU4 *)Fruit)->board.pia7.set_ca1(busy);
          playing[channel] = tune;
          Fruit->do_wave();
        } else {
          next2 = latch2;
        }
      }
    }
  }
  lastval = val;
}

