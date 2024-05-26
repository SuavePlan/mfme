//
//
//	Triac.cpp
//
//	(C) C.J.Wren 2000
//

#include "typedefs.h"
#include "triac.h"

triacs::triacs()
{
	reset();
}

triacs::~triacs()
{
}

void triacs::reset()
{
	value = 0;
}

void triacs::write(Word x)
{
    changed = value ^ x;
	if ( x != value )
		value = x;
}

void __fastcall triacs::SaveToStream(Byte *&ptr)
{
  savestate(ptr,value);
  savestate(ptr,changed);
}

void __fastcall triacs::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,value);
  readstate(ptr,changed);
}


