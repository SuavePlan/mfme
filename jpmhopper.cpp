//
//
//	jpmhopper.cpp
//
//	(C) C.J.Wren 2002
//

#include "typedefs.h"
#include "jpmhopper.h"

jpmhopper::jpmhopper()
{
	reset();
}

void jpmhopper::reset()
{
  coin = 0;
  timer = 0;
  motor = 0;
  opto_test = false;
  hz = 0;
}

void jpmhopper::update()
{
  if ( timer != 0 ) {
    timer--;
    if ( timer == 0 ) {
      if ( coin == 0 ) {
        coin = 1;
        timer = 2;
      } else {
        coin = 0;
        timer = 6;
      }
    }
  }
}

void jpmhopper::enable(Byte enable, Byte opto, Byte Hz50 )
{
  opto_test = opto;
  hz = Hz50;
  if ( enable ) {
    if ( motor == 0 ) {
      timer = 20;
      motor = 1;
    }
  } else {
    timer = 0;
    coin = 0;
    motor = 0;
  }
}

Byte jpmhopper::opto(void)
{

  if ( opto_test )
    return 0;

  if ( hz )
    return coin;

  return 1;

}

void __fastcall jpmhopper::SaveToStream(Byte *&ptr)
{
  savestate(ptr, timer);
  savestate(ptr, coin);
  savestate(ptr, motor);
  savestate(ptr, hz);
  savestate(ptr, opto_test);
}

void __fastcall jpmhopper::LoadFromStream(Byte *&ptr)
{
  readstate(ptr, timer);
  readstate(ptr, coin);
  readstate(ptr, motor);
  readstate(ptr, hz);
  readstate(ptr, opto_test);
}

