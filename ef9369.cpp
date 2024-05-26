//
//
//	scn2674.cpp
//
//	(C) C.J.Wren 2004
//

#include "typedefs.h"
#include "ef9369.h"

ef9369::ef9369()
{
	reset();
}

void ef9369::reset()
{
  pos = 0;
  for ( int i = 0; i < 32; i++ )
    regs[i] = 0;
}

Byte ef9369::read(int addr)
{
  return regs[addr];
}

void ef9369::write(int addr, Byte data)
{
bool value;

  switch ( addr ) {
    case 0:
      regs[pos++] = data;
      if ( pos > 31 )
        pos = 0;
      break;
    case 1:
      pos = data;
      break;
  }
}

