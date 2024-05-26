//
//
//	Ls138.cpp
//
//	(C) C.J.Wren 2000
//
#include "typedefs.h"
#include "ls138.h"

ls138::ls138()
{
	reset();
}

ls138::~ls138()
{
}

void ls138::reset()
{
	column = 0;
    timer = 0;
    enabled = false;
}

void ls138::write_A0(Byte x)
{
	if ( x ) {
        enabled = true;
        timer = 5;
		column |= 1;
	} else
		column &= 6;
}

void ls138::write_A1(Byte x)
{
	if ( x )
		column |= 2;
	else
		column &= 5;
}

void ls138::write_A2(Byte x)
{
	if ( x )
		column |= 4;
	else
		column &= 3;
}

void ls138::update(void)
{
  if ( timer ) {
    timer--;
    if ( timer == 0 )
      enabled = false;
  }
}

void __fastcall ls138::SaveToStream(Byte *&ptr)
{
  savestate(ptr,timer);
  savestate(ptr,column);
  savestate(ptr,enabled);
}
void __fastcall ls138::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,timer);
  readstate(ptr,column);
  readstate(ptr,enabled);
}

