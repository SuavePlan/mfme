//
//
//	BWBchar.cpp
//
//	(C) C.J.Wren 2000
//
#include "shared.h"
#include "bwbchr.h"

static Byte reply[] = {
  0x00,
  0x04,
  0x04,
  0x0C,
  0x0C,
  0x1C,
  0x14,
  0x2C,
  0x5C,
  0x2C
};

static Byte lookup[] = {
  6, 0, 7, 0, 8, 0, 7, 0, 0, 8
};

bwbchr::bwbchr()
{
  count = 0;
  identifying = false;
}

bwbchr::~bwbchr()
{
}

void __fastcall bwbchr::write(Byte Value)
{
  previous = Value;
  identifying = false;
  if ( Value == Character[lookup[count]] )
    identifying = true;
  else
    count = 0;
}

Byte __fastcall bwbchr::read(void)
{
Byte ret = 0;

  if ( identifying ) {
    ret = reply[count++];
  } else {
    if ( previous == Character[1] )
      ret = ((Character[9] ^ 0xff) << 6) | 0x3f;
    else if ( previous == Character[2] )
      ret = (Character[9] ^ 0xff) & 0xfc;
    else if ( previous == Character[3] )
      ret = 0xff;
    else if ( previous == Character[4] )
      ret = 0xff;
    else if ( previous == Character[5] )
      ret = 0xff;
  }
  return ret;
}


