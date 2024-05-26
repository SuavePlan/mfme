//
//
//	bfmhopper.cpp
//
//	(C) C.J.Wren 2003
//

#include "typedefs.h"
#include "shared.h"
#include "bfmhopper.h"

bfmhopper::bfmhopper(void)
{
    start_delay = 10;
    coin_on_period = 2;
    coin_off_period = 15;
	reset();
}

void bfmhopper::reset()
{
  coin1 = 0;
  timer1 = 0;
  motor1 = 0;
  state1 = 0;
  coin2 = 0;
  timer2 = 0;
  motor2 = 0;
  state2 = 0;
  coin_on_period = 5;
  coin_off_period = 15;
  start_delay = 200;
  opto_drive = false;
  SW24 = false;
  TREN = false;
}

void bfmhopper::update()
{
  if ( timer1 ) {
    timer1--;
    if ( !timer1 ) {
      switch ( state1 ) {
        case 0: // Coin On
          coin1 = 1;
          timer1 = 30; // Coin on delay
          state1 = 1;
          break;
        case 1: // Coin Off
          coin1 = 0;
          timer1 = 10; // Between coin delay
          state1 = 0;
          break;
      }
    }
  }
  if ( timer2 ) {
    timer2--;
    if ( !timer2 ) {
      switch ( state2 ) {
        case 0:  // Coin On
          coin2 = 1;
          timer2 = 30; // Coin on delay
          state2 = 1;
          break;
        case 1: // Coin Off
          coin2 = 0;
          timer2 = 10; // Between coin delay
          state2 = 0;
          break;
      }
    }
  }
}

void bfmhopper::writeP03(System::Byte x)
{
  p03 = x;
  statechange();
}

void bfmhopper::writepayen1(System::Byte x)
{
  if ( x == 0x4d )
    TREN = true;
  else
    TREN = false;
  statechange();
}

void bfmhopper::writepayen2(System::Byte x)
{
  if ( x == 0x4d )
    SW24 = true;
  else
    SW24 = false;
  statechange();
}

void bfmhopper::statechange(void)
{

  if ( (TREN && ((p03 & 9) == 9)) )
    opto1 = 1;
  else
    opto1 = 0;

  if ( (TREN && ((p03 & 10) == 10)) )
    opto2 = 1;
  else
    opto2 = 0;

  if ( TREN && (p03 & 1) && SW24 ) {
    if ( !motor1 ) {
      motor1 = 1;
      timer1 = 30;
      coin1 = 0;
    }
  } else {
    motor1 = 0;
    timer1 = 0;
    coin1 = 0;
  }

  if ( TREN && (p03 & 2) && SW24 ) {
    if ( !motor2 ) {
      motor2 = 1;
      timer2 = 30;
      coin2 = 0;
    }
  } else {
    motor2 = 0;
    timer2 = 0;
    coin2 = 0;
  }
}

System::Byte bfmhopper::readpay(void)
{
System::Byte value = 0;

  if ( (Hoppers & 0x50) == 0x10 ) // 1
    value |= ( coin1 ^ opto1 );
  else if ( (Hoppers & 0x50) == 0x0 ) { // 2
    value |= ( (2 * (opto2 ^ coin2) ) | ( coin1 ^ opto1 ) );
    if ( SW24 )
      value ^= 3;
  } else
    value |= ( (2 * (opto1 ^ coin1) ) | ( coin2 ^ opto2 ) );

  if ( SW24 )
    value |= 0x40;

  return value | 0x20 | 0x4;
}

void bfmhopper::SaveToStream(System::Byte *&ptr)
{
  savestate(ptr,timer1);
  savestate(ptr,motor1);
  savestate(ptr,coin1);
}

void bfmhopper::LoadFromStream(System::Byte *&ptr)
{
  readstate(ptr,timer1);
  readstate(ptr,motor1);
  readstate(ptr,coin1);
}

