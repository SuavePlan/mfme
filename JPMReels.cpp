//
//
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NO_REEL_DEBUG

#ifdef REEL_DEBUG
#include "system.h"
#else
#include "typedefs.h"
#endif
#include "JPMReels.h"


static const int newjpm[8][16] = {
  { 0,  0,  0,  0,  2,  1,  3,  0, -2, -1, -3,  0,  0,  0,  0,  0 },
  { 0, -1,  3,  0,  1,  0,  2,  0, -3, -2,  0,  0,  0,  0,  0,  0 },
  { 0, -2,  2,  0,  0, -1,  1,  0,  0, -3,  3,  0,  0,  0,  0,  0 },
  { 0, -3,  1,  0, -1, -2,  0,  0,  3,  0,  2,  0,  0,  0,  0,  0 },
  { 0,  0,  0,  0, -2, -3, -1,  0,  2,  3,  1,  0,  0,  0,  0,  0 },
  { 0,  3, -1,  0, -3,  0, -2,  0,  1,  2,  0,  0,  0,  0,  0,  0 },
  { 0,  2, -2,  0,  0,  3, -3,  0,  0,  1, -1,  0,  0,  0,  0,  0 },
  { 0,  1, -3,  0,  3,  2,  0,  0, -1,  0, -2,  0,  0,  0,  0,  0 },
};

static const Byte flip03[16] = {
  0, 8, 2, 10, 4, 12, 6, 14, 1, 9, 3, 11, 5, 13, 7
};

jpmreels::jpmreels()
{
rstruct *reel = &reels[0];
int count;

  for ( count = 0; count < 8; count++ ) {
    reel->steps = 96;
    reel->offset = 0;
    reel->inverted = false;
    reel->flag = 0;
//  steps[count] = 96;
//  offset[count] = 0;
//  inverted[count] = false;
//  flag[count] = 0;
    reel++;
  }
  reset();
}

void jpmreels::reset()
{
int count;
rstruct *reel = &reels[0];

  for ( count = 0; count < 8; count++ ) {
    reel->pos = 0;
    changed[count] = false;
    lamps[count] = 0;
    lamp_changed[count] = false;
    reel->inverted = false;
    reel->steps_remaining = 0;
    reel->stepsperstop = 1;
    reel->present = false;
    reel->pat = 0;
    reel++;
  }
  proc[0].rxcount = 0;
  proc[0].unit = 0x40;
  proc[0].bank = 0;
  proc[1].rxcount = 0;
  proc[1].unit = 0x80;
  proc[1].bank = 4;
//  scaler = 0;
//  speed = 0;
  optos = 0x00;
}

void jpmreels::clear()
{
int count;

  for ( count = 0; count < 8; count++ ) {
    reels[count].pat = 0;
  }
}

void jpmreels::write(Byte x, Byte nbr)
{
  if ( x != reels[nbr].pat ) {
    state( x, nbr);
    reels[nbr].pat = x;
  }
}

void jpmreels::writex(Byte x)
{
  write( x & 0xf, 0);
  write( x >> 4, 1);
}

void jpmreels::writey(Byte x)
{
  write( x & 0xf, 2);
  write( x >> 4, 3);
}

void jpmreels::writez(Byte x)
{
  write( x & 0xf, 4);
  write( x >> 4, 5);
}

void jpmreels::writexr(Byte x)
{
  write( x >> 4, 0);
  write( x & 0xf, 1);
}

void jpmreels::writeyr(Byte x)
{
  write( x >> 4, 2);
  write( x & 0xf, 3);
}

void jpmreels::writezr(Byte x)
{
  write( x >> 4, 4);
  write( x & 0xf, 5);
}

void jpmreels::write1(Word x)
{
  write( x & 0xf, 0);
  write( ( x >> 4 ) & 0x0F, 1);
  write( ( x >> 8 ) & 0x0F, 2);
  write( ( x >> 12 ) & 0x0F, 3);
}

void jpmreels::writeflip03(Word x)
{
  write( flip03[x & 0xf], 0);
  write( flip03[( x >> 4 ) & 0x0F], 1);
  write( flip03[( x >> 8 ) & 0x0F], 2);
  write( flip03[( x >> 12 ) & 0x0F], 3);
}

void jpmreels::write2(Word x)
{
  write( x & 0xf, 4);
  write( ( x >> 4 ) & 0x0F, 5);
}

void jpmreels::write3(int x)
{
  write( ( x >> 16 ) & 0x0F, 0);
  write( ( x >> 20 ) & 0x0F, 1);
  write( ( x >> 24 ) & 0x0F, 2);
  write( ( x >> 28 ) & 0x0F, 3);
  write( ( x >> 0 ) & 0x0F, 4);
  write( ( x >> 4 ) & 0x0F, 5);
  write( ( x >> 8 ) & 0x0F, 6);
  write( ( x >> 12 ) & 0x0F, 7);
}

void jpmreels::writesys5(Byte x, Byte nbr)
{
int pat[4] = { 10, 6, 9, 5};

  write( pat[x & 3], nbr);
  write( pat[(x >> 4) & 3], nbr + 1);
}

void jpmreels::writeproconn(Byte x)
{
int pat[4] = { 10, 6, 9, 5};

  write( pat[x & 3]       , 3);
  write( pat[(x >> 2) & 3], 2);
  write( pat[(x >> 4) & 3], 1);
  write( pat[(x >> 6) & 3], 0);
}

void jpmreels::writereellamp(Byte value, Byte nbr)
{
  writelamp( value & 0xf, nbr);
  writelamp( value >> 4, nbr + 1);
}

void jpmreels::writelamp(Byte value, Byte nbr)
{
  value &= 7;
  value = (value & 2) | ((value & 4)>>2) | ((value & 1)<<2);
  if ( lamps[nbr] != value ) {
    lamps[nbr] = value;
    lamp_changed[nbr] = true;
  }
}

bool jpmreels::tick(int cycles)
{
mps2reel *ptr = &proc[0];

  for ( int i = 0; i < 2; i++, ptr++ ) {
    if ( ptr->speed ) {
      ptr->scaler += cycles;
      if ( ptr->scaler >= ptr->speed ) {
        ptr->scaler -= ptr->speed;
        spin(i);
        if ( changed[ptr->bank] || changed[ptr->bank+1] || changed[ptr->bank+2] || changed[ptr->bank+3] )
        return true;
      }
    }
  }
  return false;
}

void jpmreels::state(Byte x, Byte nbr)
{
Byte phase;
int old_pos;
Byte opto;
rstruct *reel = &reels[nbr];

  old_pos = reel->pos;
  phase = reel->pos % 8;
#ifdef REEL_DEBUG
  if ( newjpm[phase][x] ) {
          char buffer[50];
          sprintf( buffer, "%d Added %d phase %d drive %d",nbr,newjpm[phase][x], phase, x);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
  reel->pos = (reel->pos + newjpm[phase][x] + reel->steps ) % reel->steps;
  if ( old_pos != reel->pos && (reel->pos % reel->stepsperstop) == 0 ) {
    changed[nbr] = true;
  }
  reel->reelpos = (reel->steps + reel->adjust - reel->pos ) % reel->steps;
  opto = 1 << nbr;
  if ( reel->pos > reel->startopto && reel->pos < reel->endopto ) {
    if ( reel->inverted )
      optos &= ~opto;
    else
      optos |= opto;
  } else {
    if ( reel->inverted )
      optos |= opto;
    else
      optos &= ~opto;
  }
}
void jpmreels::spin(int bank)
{
int i, direction;
int start = 4 * bank;
rstruct *reel = &reels[start];

  for ( i = start; i < start+4; i++ ) {
    if ( reel->steps_remaining != 0 && reel->present ) {
      direction = reel->steps_remaining < 0 ? -1 : 1;
      reel->steps_remaining -= direction;
      reel->pos = (reel->pos + direction + reel->steps) % reel->steps;
      reel->reelpos = (reel->pos + reel->adjust ) % reel->steps;
      if ( (reel->steps_remaining % reel->stepsperstop) == 0 )
        changed[i] = true;
    }
    reel++;
  }
}

void jpmreels::write_sys1(Byte x)
{
int i, direction;
rstruct *reel = &reels[0];

  for (i=0; i<4; i++) {
    direction=( ((x & (1 << (4+i)) ) == 0) ? -1 : 1);
    if (x&(1<<i)) {
      reel->pos = (reel->pos + direction + 120) % 120 ;
      reel->reelpos = ((reel->pos - reel->adjust) + 120 ) % 120;
      changed[i] = true;
    }
    if ( reel->pos > 118 || reel->pos < 2 )
      optos |= (1 << i);
    else
      optos &= ~(1 << i);
    reel++;
  }
}

bool jpmreels::mps2Rxchar( Byte ch )
{
mps2reel *ptr;

  if ( ch & 0x40 )
    ptr = &proc[0];
  else
    ptr = &proc[1];

  if ( ptr->rxcount ) {
    *ptr->rxp++ = ch & 0x3f;
    ptr->rxcount--;
    if ( !ptr->rxcount ) {
      *ptr->rxp = 0;
      RxMessage(ptr);
      return true;
    }
  }
  if ( ((ch & 0xf0) == 0x40) || ((ch & 0xf0) == 0x80) ) {
    ptr->rxcount = ( ch & 0xf ) << 1;
    ptr->rxcount++;
    ptr->msg[0] = ch;
    ptr->rxp = &ptr->msg[1];
  } else if ( ch == 0x67 || ch == 0x68 || ch == 0x69 || ch == 0xa7 || ch == 0xa8 || ch == 0xa9 ) {
    ptr->msg[0] = ch & 0x3f;
    ptr->msg[1] = 0;
    RxMessage(ptr);
    return true;
  }

  return false;
}

void jpmreels::RxMessage(mps2reel * p)
{
Byte code = p->msg[strlen(p->msg)-1];
char *ptr = reply, *ptr2;
char busy = 0;
int i, direction, value;
int spd;

  switch ( code ) {
    case 0x26:
      *ptr++ = 0x36 | p->unit;
      ptr2 = &p->msg[1];
      for ( i = p->bank; i < p->bank + 4; i++, ptr2+=2 ) {
        value = (ptr2[0] & 0xf ) + 16 * (ptr2[1] & 0xf);
        if ( value == 1 )
          reels[i].present = true;
        else
          reels[i].present = false;
      }
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "(%d) present %d %d %d %d",p->bank, reels[0].present,reels[1].present,
                                                reels[2].present,reels[3].present);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 0x2a:
      *ptr++ = 0x36 | p->unit;
      ptr2 = &p->msg[1];
      spd = (ptr2[0] & 0xf) + 16 * (ptr2[1] & 0xf);
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "(%d) speed %d",p->bank, spd);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      switch ( spd ) {
        case 134:
          p->speed = 45000;
          break;
        case 135:
          p->speed = 52000; // 24000
          break;
        case 136:
          p->speed = 60000;
          break;
        case 137:
          p->speed = 72000;
          break;
        case 138:
          p->speed = 84000;
          break;
        case 139:
          p->speed = 150000;
          break;
        default:
          p->speed = 60000;
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Unknown speed! %d",spd);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
          break;
      }
      break;
    case 0x27:
      *ptr++ = 0x01 | p->unit;
      for ( i = p->bank; i < p->bank + 4; i++ ) {
        if ( reels[i].steps_remaining != 0 )
          busy |= ( 1 << (i - p->bank) );
      }
      *ptr++ = 0x10 | p->unit | busy;
      *ptr++ = 0x10 | p->unit;
/*#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "busy %d",busy);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif */
      break;
    case 0x28:
      *ptr++ = 0x04 | p->unit;
      for ( i = p->bank; i < p->bank + 4; i++ ) {
        *ptr++ = 0x10 | p->unit | ( reels[i].pos & 0xf );
        *ptr++ = 0x10 | p->unit | ( reels[i].pos >> 4 );
      }
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "(%d) pos %d %d %d %d", p->bank, reels[p->bank].pos, reels[p->bank+1].pos, reels[p->bank+2].pos, reels[p->bank+3].pos);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 0x29:
      *ptr++ = 0x01 | p->unit;
      *ptr++ = 0x17 | p->unit;
      *ptr++ = 0x10 | p->unit;
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "(%d) ?", p->bank);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 0x22:
      *ptr++ = 0x36 | p->unit;
      ptr2 = &p->msg[1];
      for ( i = p->bank; i < p->bank + 4; i++, ptr2+=4 ) {
        direction = (ptr2[1] & 0x8) ? -1 : 1;
        reels[i].steps_remaining = (ptr2[2] & 0xf) + 16 * (ptr2[3] & 0xf) + 256 * (ptr2[0] & 0xf);
        reels[i].steps_remaining *= direction;
      }
      p->scaler = 0;
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "(%d) Steps %d %d %d %d", p->bank, reels[p->bank].steps_remaining,
                                               reels[p->bank+1].steps_remaining,
                                               reels[p->bank+2].steps_remaining,
                                               reels[p->bank+3].steps_remaining);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
/*          sprintf( buffer, "(%d) Stop @ %d %d %d %d", p->bank, (reels[0].pos + reels[0].steps_remaining) % 200,
                                                (reels[1].pos + reels[1].steps_remaining) % 200,
                                                (reels[2].pos + reels[2].steps_remaining) % 200,
                                                (reels[3].pos + reels[3].steps_remaining) % 200);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0); */
          }
#endif
      break;
    default:
#ifdef REEL_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "(%d) unknown reel cmd %02x",p->bank, code);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
          break;
  }
  *ptr = 0;
}

void __fastcall jpmreels::SaveToStream(Byte *&ptr)
{
  savestate(ptr,reels);
//  ptr->WriteBuffer(&pos, 24);
//  ptr->WriteBuffer(&reelpos, 24);
//  ptr->WriteBuffer(&inverted, 24);
}

void __fastcall jpmreels::LoadFromStream(Byte *&ptr)
{
int i;

  readstate(ptr,reels);
//  ptr->ReadBuffer(&pos, 24);
//  ptr->ReadBuffer(&reelpos, 24);
//  ptr->ReadBuffer(&inverted, 24);
  for ( i = 0; i < 8; i++ )
    changed[i] = true;
}

void jpmreels::spinace(void)
{
int i, direction;
rstruct *reel = &reels[0];

  for ( i = 0; i < 4; i++ ) {
    if ( reel->steps_remaining != 0 ) {
      reel->steps_remaining--;
      reel->pos = (reel->pos + reel->direction + reel->steps) % reel->steps;
      reel->reelpos = (reel->pos + reel->adjust ) % reel->steps;
      changed[i] = true;
    }
    reel++;
  }
}

void jpmreels::setace(Byte nbr, int steps, int direction)
{
rstruct *reel = &reels[nbr];

  reel->steps_remaining = steps;
  reel->direction = direction;

}

