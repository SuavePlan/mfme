//
//
//	scn68681.cpp
//
//	(C) C.J.Wren 2000
//
#include "typedefs.h"
#include "scn68681.h"

scn68681::scn68681()
{
	reset();
}

scn68681::~scn68681()
{
}

void scn68681::reset()
{
  coin = 0;
  coin2 = 0;
  timer = 0;
  timer2 = 0;
  motor = 0;
  opto = 0;
  motor2 = 0;
  opto2 = 0;
}

void scn68681::update()
{
  if ( timer != 0 ) {
    timer--;
    if ( timer == 0 ) {
      if ( coin == 0 ) {
        coin = 1;
        timer = 5;
      } else {
        coin = 0;
        timer = 15;
      }
    }
  }
  if ( timer2 != 0 ) {
    timer2--;
    if ( timer2 == 0 ) {
      if ( coin2 == 0 ) {
        coin2 = 1;
        timer2 = 5;
      } else {
        coin2 = 0;
        timer2 = 15;
      }
    }
  }
}

void scn68681::hoppermotor(Byte x)
{
  if ( x & 0x10 ) {
    if ( motor == 0 ) {
      timer = 10;
      motor = 1;
    }
  } else {
    timer = 0;
    coin = 0;
    motor = 0;
  }
}

void scn68681::write(Byte addr, Byte x)
{

  switch ( addr ) {
    case 0x0d:
      conf = x;
      break;
    case 0x0e:
      op_port |= x;
      break;
    case 0x0f:
      op_port &= ~x;
      break;
    default:
//      printf("Hmm, Whats Happened\n");
      break;
  }
  if ( op_port & 0x8 )
    opto = 1;
  else
    opto = 0;
}

void scn68681::opto_drive(Byte o, Byte m, Byte o2, Byte m2)
{
  if ( o )
    opto = 0;
  else
    opto = 1;
  if ( m ) {
    if ( motor == 0 ) {
      motor = 1;
      timer = 10;
    }
  } else {
    motor = 0;
    timer = 0;
    coin = 0;
  }
  if ( o2 )
    opto2 = 0;
  else
    opto2 = 1;
  if ( m2 ) {
    if ( motor2 == 0 ) {
      motor2 = 1;
      timer2 = 10;
    }
  } else {
    motor2 = 0;
    timer2 = 0;
    coin2 = 0;
  }

}

Byte scn68681::read(Byte addr)
{
Byte ret, ret2;

  switch ( addr ) {
    case 0xd:
      if ( opto )
        ret = 0x10 ^ (coin << 4);
      else
        ret = coin * 0x10;
      if ( opto2 )
        ret2 = 0x20 ^ (coin2 << 5);
      else
        ret2 = coin2 * 0x20;
      break;
    default:
//      printf("Hmm, Whats Happened\n");
      break;
  }

  return ret + ret2;
}

void __fastcall scn68681::SaveToStream(Byte *&ptr)
{
  savestate(ptr,conf);
  savestate(ptr,timer);
  savestate(ptr,timer2);
  savestate(ptr,coin);
  savestate(ptr,coin2);
  savestate(ptr,motor);
  savestate(ptr,opto);
  savestate(ptr,motor2);
  savestate(ptr,opto2);
  savestate(ptr,ip_port);
  savestate(ptr,op_port);
}
void __fastcall scn68681::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,conf);
  readstate(ptr,timer);
  readstate(ptr,timer2);
  readstate(ptr,coin);
  readstate(ptr,coin2);
  readstate(ptr,motor);
  readstate(ptr,opto);
  readstate(ptr,motor2);
  readstate(ptr,opto2);
  readstate(ptr,ip_port);
  readstate(ptr,op_port);
}


