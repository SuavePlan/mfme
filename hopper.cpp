//
//
//	hopper.cpp
//
//	(C) C.J.Wren 2000
//

#include "typedefs.h"
#include "hopper.h"

hopper::hopper(int Astart_delay, int Acoin_on_delay, int Acoin_off_delay)
{
    start_delay = Astart_delay;
    coin_on_period = Acoin_on_delay;
    coin_off_period = Acoin_off_delay;
	reset();
}

hopper::~hopper()
{
}

void hopper::reset()
{
  coin = 0;
  timer = 0;
  motor = 0;
  coin_on_period = 5;
  coin_off_period = 15;
  start_delay = 50;
  opto_drive = false;
  state = 0;
}

void hopper::update()
{
  if ( timer != 0 ) {
    timer--;
    if ( timer == 0 ) {
      switch ( state ) {
        case 0:
          coin = 1;
          timer = 10;
          state = 1;
          break;
        case 1:
          coin2 = 1;
          timer = coin_on_period;
          state = 2;
          break;
        case 2:
          coin = 0;
          timer = 10;
          state = 3;
          break;
        case 3:
          coin2 = 0;
          if ( motor )
            timer = coin_off_period;
          state = 0;
          break;
      }
//      if ( coin == 0 && motor ) {
//        coin = 1;
//        timer = coin_on_period;
//      } else {
//        coin = 0;
//        timer = coin_off_period;
//      }
    }
  }
}

void hopper::motordrive(Byte x)
{
  if ( x ) {
    if ( motor == 0 ) {
      timer = start_delay;
      motor = 1;
      state = 0;
    }
  } else {
//    timer = 0;
//    coin = 0;
    motor = 0;
//    opto_drive = false;
  }
}

void hopper::optodrive(Byte x)
{
  if ( x )
    opto_drive = true;
  else
    opto_drive = false;
}

Byte hopper::optoread(Byte x)
{
Byte ret;


  if ( opto_drive )
    ret = x ^ (coin * x);
  else
    ret = coin * x;

  return ret;
}

Byte hopper::optoread(Byte x, Byte opto)
{
Byte ret;


  if ( opto )
    ret = x ^ (coin * x);
  else
    ret = coin * x;

  return ret;
}

Byte hopper::optoread2(Byte x, Byte opto)
{
Byte ret;


  if ( opto )
    ret = x ^ (coin2 * x);
  else
    ret = coin2 * x;

  return ret;
}

Byte hopper::motorread(Byte x)
{
  if ( motor )
    return x;
  else
    return 0;
}

void hopper::stopcoin(void)
{
  coin = 0;
  coin2 = 0;
  timer = coin_off_period;
  state = 3;
}

void hopper::SaveToStream(Byte *&ptr)
{
  savestate(ptr,timer);
  savestate(ptr,motor);
  savestate(ptr,coin);
  savestate(ptr,opto_drive);
}

void hopper::LoadFromStream(Byte *&ptr)
{
  readstate(ptr, timer);
  readstate(ptr, motor);
  readstate(ptr, coin);
  readstate(ptr, opto_drive);
}

