#include <classes.hpp>
#pragma hdrstop
//
//
//	m6376.cpp
//
//	(C) C.J.Wren 1998, 1999, 2000
//
#include <stdio.h>
//#include "typedefs.h"
#include "system.h"
#include "m1a6376.h"
#include "shared.h"
#include "sample.h"

//#define LOG_SOUND
#ifdef LOG_SOUND
    FILE             *stream;
#endif
msm6376a::msm6376a()
{
	reset();
}

msm6376a::~msm6376a()
{
#ifdef LOG_SOUND
    fclose(stream);
#endif
}

void msm6376a::reset()
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

bool msm6376a::update(void)
{
int i;
int old_nar;
#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

#endif

  old_nar = nar;

  if ( pause > 0 ) {
    pause--;
    if ( pause == 0 ) {
      nar = 1;
#ifdef LOG_SOUND
        fprintf( stream, "%08d Nar gone high\n", tickcount);
#endif
    }
  }
  for ( i = 0; i < 2; i++ ) {
    if ( playing[i] != -1 ) {
      if ( Sound->Playing(playing[i]) == 0 ) {
#ifdef LOG_SOUND
        fprintf( stream, "%08d Finished %d chan %d\n", tickcount, playing[i], i);
#endif
        playing[i] = -1;
        finished(i);
        if ( again ) {
          replay(i);
          again = false;
        }
      }
    }
  }
  if ( old_nar == 0 && nar == 1 )
    return true;
  else
    return false;
}
void msm6376a::replay(int chan)
{
#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

#endif
  if ( chan == 0 && next1 && HasSampledSound ) {
    busy = 0;
    tune = next1;
    next1 = 0;
    playing[0] = tune;
    channel = 0;
    nar = 0;
    pause = 45;
#ifdef LOG_SOUND
        fprintf( stream, "%08d Playing next %d chan %d\n", tickcount, tune, chan);
#endif
    Fruit->do_wave();
  }
  if ( chan == 1 && next2 && HasSampledSound ) {
    busy = 0;
    tune = next2;
    next2 = 0;
    playing[1] = tune;
    channel = 1;
#ifdef LOG_SOUND
        fprintf( stream, "%08d Playing next %d chan %d\n", tickcount, tune, chan);
#endif
    Fruit->do_wave();
  }
}

void msm6376a::finished(Byte Channel)
{
#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

#endif
  if ( Channel == 1 && next2 )
    again = true;
  if ( Channel == 0 && latch1 && (((lastval & 3) == 2 ) || next1) )  {
    next1 = latch1;
    again = true;
  }
//  if ( playing[0] == playing[1] )
//    playing[1-Channel] = -1;

  playing[Channel] = -1;

  if ( playing[0] == -1 ) {
    nar = 1;
#ifdef LOG_SOUND
        fprintf( stream, "%08d Nar gone high2\n", tickcount);
#endif
    if ( playing[1] == -1 ) {
      busy = 1;
    }
  }
}

void msm6376a::write_latch(Byte val)
{
#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

#endif
  latch = val & 0x7f;
#ifdef LOG_SOUND
    fprintf( stream, "%08d Latch %d\n", tickcount, latch);
#endif
}

void msm6376a::write_control(Byte val)
{
#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

#endif
  st_changed = false;
  ch2_changed = false;

  if ( (lastval & 0x1 ) != ( val & 0x1 ) ) {
    st = val & 1;
    st_changed = true;
#ifdef LOG_SOUND
    fprintf( stream, "%08d ST %d\n", tickcount, st);
#endif
  }
  if ( (lastval & 0x2 ) != ( val & 0x2 ) ) {
    ch2 = (val & 2) >> 1;
    ch2_changed = true;
#ifdef LOG_SOUND
    fprintf( stream, "%08d CH2 %d\n", tickcount, ch2);
#endif
  }

  if ( ch2 && !ch2_changed && st_changed ) {
    if ( !st ) {
      latch1 = latch;
      if ( HasSampledSound ) {
        if ( Sound->Details(latch1) ) {
          channel = 0;
          if ( latch1 == 0 ) {
            next1 = 0;
            next2 = 0;
            busy = 1;
          } else
            busy = 0;
          if ( playing[channel] == -1 || latch1 == 0 ) {
            nar = 0;
            pause = 50;
            tune = latch1;
            playing[channel] = tune;
#ifdef LOG_SOUND
            fprintf( stream, "%08d Playing %d chan %d\n", tickcount, tune, channel);
#endif
            Fruit->do_wave();
          } else {
            if ( nar ) {
              next1 = latch1;
              nar = 0;
#ifdef LOG_SOUND
              fprintf( stream, "%08d Latching %d chan %d\n", tickcount, next1, channel);
#endif
            }
          }
        }
      }
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
        if ( playing[channel] == -1 || latch2 == 0 ) {
#ifdef LOG_SOUND
            fprintf( stream, "%08d Playing %d chan %d\n", tickcount, tune, channel);
#endif
          playing[channel] = tune;
          Fruit->do_wave();
        } else {
          next2 = latch2;
#ifdef LOG_SOUND
              fprintf( stream, "%08d Latching %d chan %d\n", tickcount, next2, channel);
#endif
        }
      }
    }
  }
  lastval = val;
}

