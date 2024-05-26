//
//
//	rs232.cpp
//
//  (C) C.J.Wren 2000
//

#include "typedefs.h"
#include "rs232.h"

rs232::rs232()
{
	reset();
}

rs232::~rs232()
{
}

void rs232::reset()
{
  frame = 0;
  data = 0;
  in = 1;
  state = 255;
}

bool rs232::update()
{
  if ( timer ) {
    timer--;
    if ( timer == 0 ) {
      in = 0;
      state = 0;
      return true;
    }
  }
  return false;
}

void rs232::shiftbit()
{

  switch ( state ) {
    case 0:
      state++;
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      in = in_data & 1;
      in_data = in_data >> 1;
      state++;
      break;
    default:
      in = 1;
      break;
  }

}

void rs232::write(int bit, int changed)
{
   RXComplete = false;
  switch ( frame ) {
    case 0:
      data = 0;
      if ( bit == 1 ) {
//        printf("Bad Start bit");
        frame = 0;
      } else
        frame++;
      break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
      data = ( data >> 1 ) | ( bit * 0x80 );
      frame++;
      break;
    case 9:
      parity = bit;
      frame++;
      break;
    case 10:
      if ( bit == 0 ) {
//        printf("Bad Stop bit");
        frame = 0;
        return;
      } else
        frame++;
      break;
    case 11:
      if ( bit == 0 ) {
//        printf("Bad Stop bit");
      } else {
//        Fruit->do_pak();
        RXComplete = true;
        timer = 1;
        in_data = 6;
      }
      frame = 0;
      break;
  }
}
