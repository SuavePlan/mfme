//
//
//	meter.cpp
//
//	(C) C.J.Wren 2000
//
#include "typedefs.h"
#include "meter.h"

meters::meters()
{
	reset();
}

void meters::reset()
{
  meter_on = 0;
  state = 0;
  delay = 0;
  changed = false;
}

bool meters::write(Byte x)
{

  if ( x != state ) {
    state = x;
    if ( state ) {
      meter_on = 1;
      delay = 5;
//      Fruit->do_meter();
    } else {
      meter_on = 0;
      delay = 0;
    }
    return true;
  }
  return false;
}

bool meters::update(void)
{
  if ( delay ) {
    delay--;
    if ( !delay )
      return true;
  }
  return false;
}

void __fastcall meters::SaveToStream(Byte *&ptr)
{
  savestate(ptr,state);
  savestate(ptr,meter_on);
}
void __fastcall meters::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,state);
  readstate(ptr,meter_on);
}

