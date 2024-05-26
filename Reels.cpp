//
//
//
#include "typedefs.h"
#include "reels.h"
#include "shared.h"

static const int steps[8][16] = {
  { 0,  1,  3,  2, -3,  0,  0,  0, -1,  0,  0,  0, -2,  0,  0,  0 },
  { 0,  0,  2,  1,  0,  0,  3,  0, -2, -1,  0,  0, -3,  0,  0,  0 },
  { 0, -1,  1,  0,  3,  0,  2,  0, -3, -2,  0,  0,  0,  0,  0,  0 },
  { 0, -2,  0, -1,  2,  0,  1,  0,  0, -3,  0,  0,  3,  0,  0,  0 },
  { 0, -3, -1, -2,  1,  0,  0,  0,  3,  0,  0,  0,  2,  0,  0,  0 },
  { 0,  0, -2, -3,  0,  0, -1,  0,  2,  3,  0,  0,  1,  0,  0,  0 },
  { 0,  3, -3,  0, -1,  0, -2,  0,  1,  2,  0,  0,  0,  0,  0,  0 },
  { 0,  2,  0,  3, -2,  0, -3,  0,  0,  1,  0,  0, -1,  0,  0,  0 },
};

static const int steps2[8][4] = {
 2,  0,  0, -2,
 0,  0,  0,  0,
 0, -2,  2,  0,
 0,  0,  0,  0,
-2,  0,  0,  2,
 0,  0,  0,  0,
 0,  2, -2,  0,
 0,  0,  0,  0
 };

reels::reels()
{
	reset();
}

reels::~reels()
{
}

void reels::reset()
{
System::Byte count;

  for ( count = 0; count < 10; count++ ) {
    step_size[count] = 24;
    pos[count] = 2;
    reelpos[count] = 0;
    changed[count] = false;
  }
  optos = 0;
}

void reels::write_x(System::Byte x)
{
int loop;

  for ( loop = 2; loop < 4; loop++ ) {
    if ( (x & 0xf) != old[loop] && ( x & 0xf ) != 0 ) {
      state( x & 0x0F, loop);
      old[loop] = x & 0xf;
    }
    x = x >> 4;
  }
}

void reels::write_y(System::Byte x)
{
int loop;

  for ( loop = 0; loop < 2; loop++ ) {
    if ( (x & 0xf) != old[loop] && ( x & 0xf ) != 0 ) {
      state( x & 0x0F, loop);
      old[loop] = x & 0xf;
    }
    x = x >> 4;
  }
}

void reels::write_z(System::Byte x)
{
int loop;

  for ( loop = 4; loop < 6; loop++ ) {
    if ( (x & 0xf) != old[loop] && ( x & 0xf ) != 0 ) {
      state( x & 0x0F, loop);
      old[loop] = x & 0xf;
    }
    x = x >> 4;
  }
}

void reels::write_5a(System::Byte x)
{
  state( (((x & 0x80) >> 4) + ((x & 0x20) >> 3) + ((x & 0x8) >> 2) + (x & 1)) & 0xf, 4);
}

void reels::write_5b(System::Byte x)
{
  state( x >> 4, 4);
}

void reels::write_5c(System::Byte x)
{
  state( (x >> 2) & 0xf, 4);
}

void reels::write_6(System::Byte x)
{
  state( x & 0xf, 4);
  state( (x >> 4) & 0xf, 5);
}

void reels::write_6a(System::Byte x)
{
  state( x & 0xf, 4);
  state( (x >> 4) & 0xf, 5);
}

void reels::write_7(System::Byte x)
{
  state( (((x & 0x80) >> 4) + ((x & 0x20) >> 3) + ((x & 0x8) >> 2) + (x & 1)) & 0xf, 6);
}

void reels::state(System::Byte x, System::Byte nbr)
{
System::Byte step;
int old_pos;
System::Byte opto;
int adj;

    old_pos = pos[nbr];
    step = pos[nbr] % 8;
//    if ( steps[step][x] == 0 )
//      printf("Stepped 0\r\n");
    pos[nbr] = (pos[nbr] + steps[step][x] + 96) % 96;
//    pos[nbr] = (pos[nbr] + 96 ) % 96;
    adj = ( 96 + pos[nbr] - offset[nbr]) % 96;
    if ( old_pos != pos[nbr] /*&& !(pos[nbr] % (step_size[nbr] / 3))*/ ) {
      reelpos[nbr] = (96 - pos[nbr]) % 96;
      changed[nbr] = true;
    }
    if ( Reel_Type == 0 ) {
      opto = 8 << (3-nbr);
//      if ( pos[nbr] > 4 && pos[nbr] < 12 ) {
      if ( (flag[nbr] == 0 && (pos[nbr] < 4 || pos[nbr] > 92)) ||
           (flag[nbr] == 3 && (pos[nbr] > 93 || pos[nbr] < 2) ) ||
           (flag[nbr] == 1 && pos[nbr] > 4 && pos[nbr] < 12 ) ||
           (flag[nbr] == 2 && (pos[nbr] < 3 || pos[nbr] > 92) ) ) {
          optos = optos | opto;
      } else {
          optos = optos & ~opto;
      }
    }
}

void reels::write(System::Byte x)
{
int old_pos;
System::Byte opto;
System::Byte count;
System::Byte value;
System::Byte step;

  for ( count = 0; count < 4; count++ ) {
    value = x & 3;
    x = x >> 2;
	old_pos = pos[count];
    step = pos[count] % 8;
//    if ( steps[step][value] > 0 )
//      printf("stepping");
	pos[count] = pos[count] + steps2[step][value];
    pos[count] = (pos[count] + 96 ) % 96;
    if ( old_pos != pos[count] /*&& !(pos[count] % (step_size[count] / 2)) */) {
      reelpos[count] = (95 - pos[count] + 1) % 96;
      changed[count] = true;
    }
    opto = 1 << count;
    if ( pos[count] < 2 || pos[count] > 92 ) {
      optos = optos | opto;
    } else {
      optos = optos & ~opto;
    }
  }
}

void __fastcall reels::SaveToStream(System::Byte *&ptr)
{
  savestate(ptr,optos);
  savestate(ptr,mux);
  savestate(ptr,pos);
  savestate(ptr,old);
  savestate(ptr,reelpos);
}

void __fastcall reels::LoadFromStream(System::Byte *&ptr)
{
int i;

  readstate(ptr,optos);
  readstate(ptr,mux);
  readstate(ptr,pos);
  readstate(ptr,old);
  readstate(ptr,reelpos);
  for ( i = 0; i < 10; i++ )
    changed[i] = true;
}


