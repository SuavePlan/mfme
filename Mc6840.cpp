//
//
//	mc6840.cpp
//
//	(C) C.J.Wren 1998
//
#include "typedefs.h"
#include "mc6840.h"

#define updatecf 		if ( (( sr & 1 ) && ( cr1 & 0x40 )) || \
                             (( sr & 2 ) && ( cr2 & 0x40 )) || \
                             (( sr & 4 ) && ( cr3 & 0x40 )) )  { \
                          sr |= 0x80; \
                        } else { \
                          sr &= ~0x80; \
                        }

// O1 > C2, O2 > C3, O3 > C1

mc6840::mc6840()
{
	reset();
}

mc6840::~mc6840()
{
}

void mc6840::reset()
{
	lat1 = lat2 = lat3 = 0xFFFF;
	cr1 = 1;
	cr2 = 0;
	cr3 = 0;
	cnt1 = cnt2 = cnt3 = 0xFFFF;
	sr = 0;
	clk = 0;
    status_read = 0;
    t1_changed = t2_changed = t3_changed = false;
    lat1_changed = lat2_changed = lat3_changed = false;
    t1_timedout = t2_timedout = t3_timedout = false;
    prescaler = 8;
}

Byte mc6840::read(Word offset)
{
Byte ret;

	switch ( offset ) {
	case 0:
		break;
	case 1:
        status_read = sr;
		ret = sr;
		break;
	case 2:
        if ( status_read & 1 ) {
          sr = sr & 0xfe;
          updatecf
        }
        status_read &= ~1;
		ret = lat1 >> 8;
		break;
	case 3:
		ret = lat1 & 0xff;
		break;
	case 4:
        if ( status_read & 2 ) {
	  	  sr = sr & 0xfd;
          updatecf
        }
        status_read &= ~2;
		ret = lat2 >> 8;
		break;
	case 5:
		ret = lat2 & 0xff;
		break;
	case 6:
        if ( status_read & 4 ) {
		  sr = sr & 0xfb;
          updatecf
        }
        status_read &= ~4;
		ret = lat3 >> 8;
		break;
	case 7:
		ret = lat3 & 0xff;
		break;
	}
	return ret;
}

void mc6840::write(Word offset, Byte val)
{

  switch ( offset ) {
    case 0:
      if ( cr2 & 1 ) {
        if ( ( cr1 & 0x80) != ( val & 0x80 ) )
          t1_changed = 1;
        cr1 = val;
        updatecf
        if ( cr1 & 1 )
          sr = 0;
      } else {
        if ( ( cr3 & 0x80) != ( val & 0x80 ) )
          t3_changed = 1;
        cr3 = val;
        updatecf
//        if ( val & 0x40 )
//          printf("Interrupt enabled");
//        else
//          printf("Interrupt dis-abled");
      }
      break;
    case 1:
      if ( ( cr2 & 0x80) != ( val & 0x80 ) )
        t2_changed = 1;
      cr2 = val;
      updatecf
//        if ( val & 0x40 )
//          printf("Interrupt enabled");
//        else
//          printf("Interrupt dis-abled");
      break;
    case 2:
    case 4:
    case 6:
      MSB = val;
      break;
    case 3:
      lat1 = (MSB << 8) + val;
      lat1_changed = true;
      if ( (cr1 & 0x10) == 0 )
        cnt1 = lat1;
      sr = sr & 0xfe;
      updatecf
      break;
    case 5:
      lat2 = (MSB << 8) + val;
      lat2_changed = true;
      if ( (cr2 & 0x10) == 0 )
        cnt2 = lat2;
      sr = sr & 0xfd;
      updatecf
      break;
    case 7:
      lat3 = (MSB << 8) + val;
      lat3_changed = true;
      if ( (cr3 & 0x10) == 0 )
        cnt3 = lat3;
      sr = sr & 0xfb;
      updatecf
      break;
  }
}

void mc6840::tick(int clocks_elapsed)
{
//int	clocks_elapsed;

//	clocks_elapsed = cycles - clk;
//    clk = cycles;
  t1_timedout = t2_timedout = t3_timedout = false;

  for ( ; clocks_elapsed; clocks_elapsed-- ) {
	if ( ( cr1 & 1 ) == 0 ) {
		if ( ( cr1 & 2 ) == 2 ) {                 // Internal clock
			if ( ( cr1 & 0x8 ) == 0 ) {           // Continous Mode
				if ( !cnt1 ) {
                    t1_timedout = true;
					sr |= 1;
					if ( cr1 & 0x40 ) {
                        sr |= 0x80;
					}
                    cnt1 = lat1;
				} else
			     cnt1--;
			}
		}
		if ( ( cr2 & 2 ) == 2 ) {                 // Internal clock
			if ( ( cr2 & 0x8 ) == 0 ) {
				if ( !cnt2 ) {
                    t2_timedout = true;
					sr |= 2;
					if ( cr2 & 0x40 ) {
                        sr |= 0x80;
					}
                    cnt2 = lat2;
				} else
			     cnt2--;
			}
		}
		if ( ( cr3 & 2 ) == 2 ) {                 // Internal clock
			if ( ( cr3 & 0x8 ) == 0 ) {
				if ( !cnt3 ) {
                    t3_timedout = true;
                    sr |= 4;
					if ( cr3 & 0x40 ) {
                        sr |= 0x80;
					}
                    cnt3 = lat3;
				}
                if ( cr3 & 1 ) { // / 8 prescaler
                  prescaler++;
                  if ( prescaler == 8 ) {
                    prescaler = 0;
                    cnt3--;
                  }
                } else
                  cnt3--;
			}
		}
	}
  }
}

void mc6840::clock1(void)
{
  t1_timedout = false;

  if ( ~cr1 & 3 ) {
    if ( cnt1  ) {
      cnt1--;
      if ( !cnt1 ) {
        t1_timedout = true;
        sr |= 1;
        if ( cr1 & 0x40 ) {
          sr |= 0x80;
          status_read &= ~sr;
        }
        cnt1 = lat1;
      }
    }
  }
}

void mc6840::clock2(void)
{
  t2_timedout =false;
  if ( (~cr1 & 1) && (~cr2 & 2) ) {
    if ( cnt2 ) {
      cnt2--;
      if ( !cnt2 ) {
        t2_timedout = true;
        sr |= 2;
        if ( cr2 & 0x40 ) {
          sr |= 0x80;
          status_read &= ~sr;
        }
        if ( ( cr2 & 0x38 ) == 0 )
          cnt2 = lat2;
      }
    }
  }
}

void __fastcall mc6840::SaveToStream(Byte *&ptr)
{
  savestate(ptr,MSB);
  savestate(ptr,LSB);
  savestate(ptr,lat1);
  savestate(ptr,lat2);
  savestate(ptr,lat3);
  savestate(ptr,cnt1);
  savestate(ptr,cnt2);
  savestate(ptr,cnt3);
  savestate(ptr,cr1);
  savestate(ptr,cr2);
  savestate(ptr,cr3);
  //->WriteBuffer(&clk, 4);
  savestate(ptr,status_read);
  savestate(ptr,sr);
}

void __fastcall mc6840::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,MSB);
  readstate(ptr,LSB);
  readstate(ptr,lat1);
  readstate(ptr,lat2);
  readstate(ptr,lat3);
  readstate(ptr,cnt1);
  readstate(ptr,cnt2);
  readstate(ptr,cnt3);
  readstate(ptr,cr1);
  readstate(ptr,cr2);
  readstate(ptr,cr3);
//  readstate(ptr,clk, 4);
  readstate(ptr,status_read);
  readstate(ptr,sr);
}

