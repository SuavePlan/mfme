//
//
//	various.cpp
//
//	(C) C.J.Wren 2000
//
#include <math.h>
#include "typedefs.h"
#include "various.h"
#include "shared.h"

various::various()
{
	reset(0);
}

various::~various()
{
}

void various::reset(int Rate)
{
  zero = 0;
  timer = 0;
  delay = 3000;
  ne555 = 600;
  Step[0] = Step[1] = Step[2] = 0;
  Period[0] = Period[1] = Period[2] = 0;
  Output[0] = Output[1] = Output[2] = 0;
  set_RC_filter(820,500,0,10000);
  SampleRate = Rate;
}

void various::update(void)
{
  zero_changed = false;
  timer_changed =false;
  delay--;
  if ( !delay ) {
    zero ^= 2;
    zero_changed = true;
    delay = 1800;  // 3000
  }
  ne555--;
  if ( !ne555 ) {
    timer ^= 1;
    timer_changed = true;
    ne555 = 600;
  }
}

void various::tick(unsigned int cycles, bool toggle)
{
int diff;
int i;

  for ( i = 0; i < 3; i++) {
    if ( Running[i] && ( updated[i] || toggle ) ) {
      if ( dual_mode[i] == false ) {
        if ( Step[i] > cycles )
          Step[i] -= cycles;
        else {
          if ( Period[i] > 0 ) {
            diff = (cycles - Step[i]) & 0xffff;
            if ( !single_shot[i] )
              Step[i] = (Period[i] - diff) & 0xffff;
            if ( toggle )
              Output[i] ^= 1;
          }
          else {
            Output[i] = 0;
          }
        }
      } else {
        int lo = Step[i] & 0xff;
        int hi = Step[i] >> 8;
        if ( lo > cycles )
          lo -= cycles;
        else {
          if ( hi == 0 ) {
            Output[i] = 0;
            hi = Period[i] >> 8;
          }
          diff = cycles - lo;
          lo = (Period[i] & 0xff ) - diff;
          hi--;
          if ( hi == 0 )
            Output[i] = 1;
        }
        Step[i] = lo + hi * 256;
      }
    }
  }
}

void various::SetPeriod(int channel, short latch, System::Byte ctrl, bool latch_changed, bool o_changed, int cycles)
{
  prescaler[channel] = 0;

  if ( ctrl & 2 )
    Running[channel] = true;
  else
    Running[channel] = false;

  if ( latch_changed ) {
    Period[channel] = latch;
    if ( (ctrl & 0x10) == 0 ) {
      updated[channel] = true;
      Step[channel] = Period[channel];
      if ( ctrl & 0x20 )
        Output[channel] = 1;
      else
        Output[channel] = 0;
    }
  }
  if ( ctrl & 0x04 ) {
    dual_mode[channel] = true;
  } else {
    dual_mode[channel] = false;
  }
  if ( ctrl & 0x20 ) {
    single_shot[channel] = true;
  } else {
    single_shot[channel] = false;
  }
  if ( ctrl & 0x80 ) {
    enabled[channel] = true;
  } else {
    enabled[channel] = false;
  }
}

void various::SoundUpdate(short *ptr, int len, unsigned int cycles)
{
int length = len;
short *buffptr = ptr;
int o1, o2, o3;

  updated[0] = updated[1] = updated[2] = false;
  int rate = 675000/SampleRate;

  while ( length ) {

    tick(rate, true);
    if ( enabled[0] && Volume_Type < 2 )
      o1 = Output[0];
    else
      o1 = 0;

    if ( enabled[1] && Volume_Type < 2 )
      o2 = Output[1];
    else
      o2 = 0;

    if ( enabled[2] && Volume_Type < 2 )
      o3 = Output[2];
    else
      o3 = 0;

    *buffptr++ = 0x2000 * ( o1 ^ o2 ^ o3 );
    length--;
  };
//  apply_RC_filter( ptr, len, SampleRate);
}


/*
signal >--R1--+--R2--+
              |      |
              C      R3---> amp
              |      |
             GND    GND

R1, R2, R3 in Ohm; C in pF
set C = 0 to disable the filter
*/

void various::set_RC_filter(int R1,int R2,int R3,int C)
{
	r1 = R1;
	r2 = R2;
	r3 = R3;
    c = C;
}

void various::apply_RC_filter(short *buf,int len,int sample_rate)
{
	float R1,R2,R3,C;
	float Req;
	int K;
	int i;


	if (c == 0) return;	/* filter disabled */

	R1 = r1;
	R2 = r2;
	R3 = r3;
	C = (float)c * 1E-12;	/* convert pF to F */

	/* Cut Frequency = 1/(2*Pi*Req*C) */

	Req = (R1*(R2+R3))/(R1+R2+R3);

	K = 0x10000 * exp(-1 / (Req * C) / sample_rate);

	buf[0] = buf[0] + (memory - buf[0]) * K / 0x10000;

	for (i = 1;i < len;i++)
		buf[i] = buf[i] + (buf[i-1] - buf[i]) * K / 0x10000;

	memory = buf[len-1];
}

