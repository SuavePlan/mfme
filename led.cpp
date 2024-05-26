//
//
//	led.cpp
//

#include "typedefs.h"
#include "led.h"

static const Byte decoder[16] = {
  63, 6, 91, 79, 102, 109, 125, 7, 127, 111, // 0 - 9
//  119, 124, 57, 94, 122, 0                   // A - F
  0, 0, 0, 0, 0, 0
};

led::led()
{
	reset();
}

led::~led()
{
}

void led::reset()
{
Byte	count;

  for ( count = 0; count < 40; count++ ) {
    digits[count] = 0;
    time[count] = 10;
    updated[count] = true;
  }
  for ( count = 0; count < 64; count++ )
    segcount[count] = 0;
}

void led::write_latch(Byte d, Byte value, Byte pos)
{
Byte diff;

  diff = (d ^ old_latch) & d;
  pos = 7 - pos;
  value = ~value;

//if ( diff < 7 ) {
  if ( diff & 1 )
    if ( value ) {
      time[pos] = 20;
      if ( digits[pos] != value ) {
        digits[pos] = value;
        updated[pos] = true;
      }
    }
  if ( diff & 2 )
    if ( value ) {
      time[pos+8] = 20;
      if ( digits[pos+8] != value ) {
        digits[pos+8] = value;
        updated[pos+8] = true;
      }
    }
  if ( diff & 4 )
    if ( value ) {
      time[pos+16] = 20;
      if ( digits[pos+16] != value ) {
        digits[pos+16] = value;
        updated[pos+16] = true;
      }
    }
  if ( diff & 8 )
    if ( value ) {
      time[pos+24] = 20;
      if ( digits[pos+24] != value ) {
        digits[pos+24] = value;
        updated[pos+24] = true;
      }
    }
  if ( diff & 16 )
    if ( value ) {
      time[pos+32] = 20;
      if ( digits[pos+32] != value ) {
        digits[pos+32] = value;
        updated[pos+32] = value;
      }
    }
//}
old_latch = d;
}

int led::writebuf(Byte *ptr, int base, int mux)
{
Byte pos, value;
int mask;
int mask2 = 0;
int mask3;
Byte segs1 = ptr[0];
Byte segs2 = ptr[1];

    base = base + 8 * mux;
    ptr += 2;
    for ( mask = 0; mask < 8; mask++ ) {
      mask3 = 128 >> mask;
      if ( segs1 & mask3 ) {
        value = 1 << mask;
        for ( pos = 0; pos < 8; pos++ ) {
          if ( ptr[mask2] & (1 << pos) )
            digits[base + pos] |= value;
          else
            digits[base + pos] &= ~value;
        }
        mask2++;
      }
      if ( segs2 & mask3 ) {
        value = 1 << mask;
//        for ( pos = 0; pos < 8; pos++ ) {
//          if ( ptr[mask2] & (1 << pos) )
//            digits[base + pos] |= value;
//          else
//            digits[base + pos] &= ~value;
//        }
        mask2++;
      }
      updated[base + mask]++;
    }
    return mask2 + 2;
}

bool led::writejpm(Byte d, Byte position)
{
bool changed = false;

  if ( d ) {
    time[position] = 50;
    if ( digits[position] != d ) {
      digits[position] = d;
      updated[position] = true;
      changed = true;
    }
  }
  return changed;
}

bool led::writebfm(Byte d, Byte column, Byte bank)
{
int base;
bool changed = false;
Byte val;

  base = column + bank * 16;
//  val = 0;
  if ( d ) {
    time[base] = 12;
    if ( d & 1 )
      val = (d >> 1) | 128;
    else
      val = d >> 1;
    if ( digits[base] != val ) {
      digits[base] = val;
      updated[base] = true;
      changed = true;
    }
  }
  return changed;
}

bool led::writebcd(Byte d, Byte position)
{
  d = decoder[d];
  if ( d ) {
    time[position] = 30;
    if ( digits[position] != d ) {
      digits[position] = d;
      updated[position] = true;
      return true;
    }
  }
  return false;
}

bool led::write_data(Byte d, Byte position, Byte nbr)
{
Byte pos;
bool changed = false;

  pos = ( nbr * 8 ) + 7 - position;
  if ( d ) {
    time[pos] = 15;
    if ( digits[pos] != d ) {
      digits[pos] = d;
      updated[pos] = true;
      changed = true;
    }
  }
  return changed;
}

bool led::writeca(Byte segs, Byte segment, Byte segnum, Byte nbr)
{
Byte pos;
bool changed = false;

  for ( pos = 0; pos < 8; pos++ ) {
    if ( segs & (1 << pos) ) {
      segcount[segnum+pos*8] = 35;
      if ( ( digits[pos] & segment ) == 0 ) {
        digits[pos] |= segment;
        updated[pos] = true;
        changed = true;
      }
    }
  }
  return changed;
}

void led::write_data_mpu3(Byte d, Byte position)
{

    d = (( d & 0x1) << 2) + (d & 2) + ((d & 0x4)>>2) + (d & 0x8) +
        (( d & 0x10) << 2) + ( d & 0x20 ) + (( d & 0x40) >> 2);
    write_data(d,position, 0);
}

void led::write_data_bwb(Byte d, Byte position)
{

    d = (( d & 0x1) << 2) + ((d & 2) >> 1) + ((d & 0x4) << 3) + (d & 8) +
        ((d & 0x10) >> 3) + ((d & 0x20) >> 1) + (d & 0x40);
    write_data(d,position, 0);
}


void led::write_data2(Byte nbr, Byte d, Byte position)
{
Byte pos;
int offset;

//    if ( nbr )
//      printf("A Value!");
    d = ~d;
    offset = 8 * nbr;
    pos = offset + 7 - position;
	if ( d ) {
      time[pos] = 15;
      if ( digits[pos] != d ) {
        digits[pos] = d;
        updated[pos] = true;
      }
	}

}

bool led::update(void)
{
Byte count, j, mask;
bool changed = false;

  for ( count = 0; count < 40; count++ ) {
    if ( time[count] > 0 ) {
      time[count]--;
      if ( time[count] == 0 ) {
        digits[count] = 0;
        updated[count] = true;
        changed = true;
      }
    }
  }
  for ( count = 0; count < 64; count++ ) {
    if ( segcount[count] ) {
      segcount[count]--;
      if ( segcount[count] == 0 ) {
        j = count / 8;
        mask = 1 << (count % 8);
        if ( digits[j] & mask ) {
            updated[j] = true;
            digits[j] &= ~mask;
        }
        changed = true;
      }
    }
  }
  return changed;
}

bool led::write_static(Byte d, Byte position)
{
bool changed = false;

  if ( digits[position] != d ) {
    digits[position] = d;
    updated[position] = true;
    changed = true;
  }
  return changed;
}

void __fastcall led::SaveToStream(Byte *&ptr)
{
  savestate(ptr,time);
  savestate(ptr,digits);
}

void __fastcall led::LoadFromStream(Byte *&ptr)
{
int i;

  readstate(ptr,time);
  readstate(ptr,digits);
  for ( i = 0; i < 40; i++ )
    updated[i] = true;
}


