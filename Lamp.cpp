//
//
//	lamp.cpp
//
//  (C) C.J.Wren 2000
//
//#include <classes.hpp>

//#include <stdlib.h>
//#include <stdio.h>
#include "typedefs.h"
#include <string.h>

#include "lamp.h"

lamps::lamps()
{
	reset();
}

void lamps::reset()
{
int count;

    for ( count = 0; count < 320; count++ ) {
      lamp[count] = 0;
      updated[count] = 0;
    }
    last_b7 = 0;
    last_b6 = 0;
    sense = 0;
    toggle = false;
    on = 0;
}

bool lamps::update(int nbr)
{
int count;
bool changed = false;

  for ( count = 0; count < nbr; count++ )
    if ( lamp[count] > 0 && lamp[count] != 255 ) {
      lamp[count]--;
      if ( lamp[count] == 0 ) {
        updated[count]++;
        changed = true;
        on--;
      }
    }
  return changed;
}
bool lamps::write(Byte x, Byte column, Byte bank, Byte persistance)
{
Byte count;
int lampbase;
Byte mask;
bool changed = false;

  lampbase = bank + column * 8;
  mask = 1;

  for ( count = 0; count < 8; count++ ) {
    colmn = lampbase + count;
    if ( x & mask ) {
      if ( lamp[colmn] == 0 ) {
        updated[colmn]++;
        changed = true;
        on++;
      }
      lamp[colmn] = persistance;
    }
    mask *= 2;
  }
  return changed;
}

void lamps::write_maygay(Byte x, Byte column, Byte bank)
{
Byte count;
Byte masks[8] = { 16, 32, 64, 128, 1, 2, 4, 8 };

  colmn = bank + column * 8;

  for ( count = 0; count < 8; count++ ) {
    if ( x & masks[count] ) {
      if ( lamp[colmn] == 0 )
        updated[colmn] = 1;
      lamp[colmn] = 1;
    } else {
      if ( lamp[colmn] )
        updated[colmn] = 1;
      lamp[colmn] = 0;
    }
    colmn++;
  }
}

void lamps::write_extender1(Byte x)
{
Byte mask;
Byte count;
Byte value;
Byte column;

  column = x & 0x7;

  value = 0x1f - ((x & 0xf8) >> 3);
  if ( value ) {
    mask = 1;
    for ( count = 0; count < 5; count++ ) {
      if ( value & mask ) {
        colmn = 128 + (column * 8) + count;
        if ( lamp[colmn] == 0 )
          updated[colmn]++;
        lamp[colmn] = 18;
      }
      mask *= 2;
    }
  }


}

void lamps::write_extender2(Byte x, Byte column, bool enabled)
{
int lampbase;
Byte mask;
Byte count;
Byte new_b7;

  sense = false;
  new_b7 = x & 0x80;
  if ( new_b7 != last_b7 ) {
    if ( new_b7 ) {
      lampbase = 128 + column * 8;
      if ( x & 0x3f )
        sense = true;
    } else {
      lampbase = 192 + column * 8;
      if ( x & 0x3f )
        sense = true;
    }
    if ( enabled ) {
      mask = 1;
      for ( count = 0; count < 6; count++ ) {
        colmn = lampbase + count;
        if ( x & mask ) {
          if ( lamp[colmn] == 0 )
            updated[colmn]++;
          lamp[colmn] = 18;
        }
        mask *= 2;
      }
    }
    last_b7 = new_b7;
  }
}

void lamps::write_extender3(Byte x, Byte column, bool enabled)
{
int lampbase;
Byte mask;
Byte count;
Byte new_b7;

  sense = false;
  new_b7 = x & 0x80;
  if ( new_b7 != last_b7 ) {
    toggle = true;
    if ( x & 0x3f )
      sense = true;
    if ( new_b7 )
      lampbase = 128 + column * 8;
    else
      lampbase = 192 + column * 8;
      
    if ( enabled ) {
      mask = 1;
      for ( count = 0; count < 6; count++ ) {
        colmn = lampbase + count;
        if ( x & mask ) {
          if ( lamp[colmn] == 0 )
            updated[colmn]++;
          lamp[colmn] = 10;
        }
        mask *= 2;
      }
    }
    last_b7 = new_b7;
  } else
    toggle = false;
}

void lamps::write_extender4(Byte x, Byte column, bool enabled)
{
int lampbase;
Byte mask;
Byte count;
Byte new_b7;

  sense = false;
  new_b7 = x & 0x80;
  if ( new_b7 != last_b7 ) {
    toggle = true;
    if ( new_b7 ) {
      lampbase = 128 + column * 8;
      if ( x & 0x3f )
        sense = true;
    } else {
      lampbase = 192 + column * 8;
      if ( x & 0x3f )
        sense = true;
    }
    if ( enabled ) {
      mask = 1;
      for ( count = 0; count < 7; count++ ) {
        colmn = lampbase + count;
        if ( x & mask ) {
          if ( lamp[colmn] == 0 )
            updated[colmn]++;
          lamp[colmn] = 10;
        }
        mask *= 2;
      }
    }
    last_b7 = new_b7;
  } else
    toggle = false;
}

bool lamps::writejpm(short x, Byte position, Byte persistance)
{
int loop;
bool changed = false;

  for ( loop = 0; loop < 16; loop++ ) {
    colmn = loop+16*position;
    if ( x & 1 ) {
      if ( lamp[colmn] == 0 ) {
        updated[colmn]++;
        changed = true;
      }
      lamp[colmn] = persistance;
    }
    x >>= 1;
  }
  return changed;
}

int lamps::writebuf(Byte *ptr, int base, int mux)
{
Byte pos, value;
int mask;
int mask2 = 0;
int mask3;
Byte segs1 = ptr[0];
Byte segs2 = ptr[1];

    On[mux] = false;
    base = base + 128 * mux;
    ptr += 2;
    for ( mask = 0; mask < 8; mask++ ) {
      mask3 = 128 >> mask;
      if ( segs1 & mask3 ) {
        value = 1 << mask;
        for ( pos = 0; pos < 8; pos++ ) {
          if ( ptr[mask2] & (1 << pos) ) {
            if ( !lamp[base+mask+pos*8] )
              updated[base+mask+pos*8] = 1;
            lamp[base+mask+pos*8] = 40;
            On[mux] = true;
          } else {
            if ( lamp[base+mask+pos*8] == 40 )
              updated[base+mask+pos*8] = 1;
            lamp[base+mask+pos*8] = 0;
          }
        }
        mask2++;
      }
      if ( segs2 & mask3 ) {
        value = 1 << mask;
        for ( pos = 0; pos < 8; pos++ ) {
//          if ( ptr[mask2] & (1 << pos) ) {
//            if ( !lamp[64+mask+pos*8] )
//              updated[64+mask+pos*8] = 1;
//            lamp[64+mask+pos*8] = 40;
//          } else {
//            if ( lamp[64+mask+pos*8] == 40 )
//              updated[64+mask+pos*8] = 1;
//            lamp[64+mask+pos*8] = 0;
//          }
        }
        mask2++;
      }
    }
    return mask2 + 2;
}

bool lamps::writebit(Word x, Byte value)
{
bool changed = false;

  value = value ? 255 : 0;
  if ( lamp[x] != value ) {
    lamp[x] = value;
    changed = true;
    if ( value )
      on++;
    else
      on--;
    updated[x] = 1;
  }
  return changed;
}

bool lamps::write_static(Byte x, Byte number)
{
Byte count;
Byte mask = 1;
bool changed = false;

  for ( count = 0; count < 8; count++ ) {
    colmn = number + count;
    if ( x & mask ) {
      if ( lamp[colmn] == 0 ) {
        updated[colmn] = 1;
        changed = true;
        on++;
        lamp[colmn] = 255;
      }
    } else {
      if ( lamp[colmn] == 255 ) {
        updated[colmn] = 1;
        changed = true;
        on--;
        lamp[colmn] = 0;
      }
    }
    mask *= 2;
  }
  return changed;
}

void __fastcall lamps::SaveToStream(Byte *&ptr)
{
  savestate(ptr,lamp);
  savestate(ptr, colmn);
  savestate(ptr, data);
  savestate(ptr, sense);
  savestate(ptr, toggle);
  savestate(ptr, last_b7);
  savestate(ptr, last_b6);
}

void __fastcall lamps::LoadFromStream(Byte *&ptr)
{
int i;

  readstate(ptr, lamp);
  readstate(ptr, colmn);
  readstate(ptr, data);
  readstate(ptr, sense);
  readstate(ptr, toggle);
  readstate(ptr, last_b7);
  readstate(ptr, last_b6);
  for ( i = 0; i < 320; i++ )
    updated[i] = 1;
}



