//
//
//	mc6850.cpp
//
//	(C) C.J.Wren 2004
//

#include "typedefs.h"
#include "mc6850.h"

#define TX_SPEED 6000

mc6850::mc6850()
{
    control = 0;
	reset();
    tx_speed = 30000;
    tx_char = false;
}

void mc6850::reset()
{
  tdr = rdr = 0;
  status = ACIA_TDRE;
  timer = 0;
  timer2 = 0;
  status2 = 0;
  status_read = false;
  tx_empty = true;
}

Byte mc6850::read(int rs)
{
Byte ret;

  if ( rs ) {
    status &= ~(ACIA_RDRF);
    if ( status_read )
      status2 &= ~ACIA_DCD;
    check_irq();
    ret = rdr;
    rdr = 0;
    rx_char = true;
  } else {
    status_read = true;
    ret = status;
  }
  return ret;
}

void mc6850::write(int rs, Byte data)
{
  Byte old_rts = rts;

  if ( rs ) {
    if ( status & ACIA_TDRE ) {
      tdr = data;
//      timer = tx_speed;
      timer2 = 160;
//      tx_char = true;
      tx_empty = false;
    }
    status &= ~ACIA_TDRE;
  } else {
    control = data;
    cnt_divide = control & 3;
    word_sel = (control & 0x1c) >> 2;
    rx_int = control >> 7;
    if ( cnt_divide == 3 )
      reset();
    tx_control = (control & 0x60) >> 5;
    switch ( tx_control ) {
      case 0x0:
        rts = 0;
        break;
      case 0x1:
        rts = 0;
        break;
      case 0x2:
        rts = 1;
        break;
      case 0x3:
        rts = 0;
        break;
    }
  }
  rts_changed = rts ^ old_rts;
  check_irq();
}

void mc6850::receive_char(Byte value)
{
  status |= ACIA_RDRF;
  check_irq();
  rdr = value;
}

void mc6850::char_received()
{
//  timer = 2000;
  if ( (status & ACIA_CTS) == 0 )
    status |= ACIA_TDRE;
  tx_empty = true;
  check_irq();
}

void mc6850::tick(int cycles)
{
  if ( timer ) {
    if ( timer >= cycles )
      timer -= cycles;
    else
      timer = 0;
    if ( timer == 0 ) {
      if ( (status & ACIA_CTS) == 0 )
        status |= ACIA_TDRE;
      tx_empty = true;
      check_irq();
    }
  }
  if ( timer2 ) {
    if ( timer2 >= cycles )
      timer2 -= cycles;
    else
      timer2 = 0;
    if ( timer2 == 0 ) {
      tx_char = true;
/*      if ( (status & ACIA_CTS) == 0 )
        status |= ACIA_TDRE;
      tx_empty = true;
      check_irq();*/
    }
  }
}

void mc6850::set_cts(Byte value)
{
  if ( value ) {
    status |= ACIA_CTS;
    status &= ~ACIA_TDRE;
  } else {
    status &= ~ACIA_CTS;
    if ( tx_empty )
      status |= ACIA_TDRE;
  }
  check_irq();
}

void mc6850::set_dcd(Byte value)
{
  if ( value ) {
    if ( (status & ACIA_DCD) == 0 ) {
      status2 |= ACIA_DCD;
      status_read = false;
    }
    status |= ACIA_DCD;
  } else {
    status &= ~ACIA_DCD;
  }
  check_irq();
}

void mc6850::check_irq()
{
  if ( ((control & 0x80) && ( (status & (ACIA_RDRF+ACIA_OVRN)) || (status2 & ACIA_DCD)) ) ||
       ( ((control & 0x60) == 0x20) && (status & ACIA_TDRE) ) )
    status |= ACIA_IRQ;
  else
    status &= ~ACIA_IRQ;
}

/*void __fastcall mc6850::SaveToStream(Byte *&ptr)
{
  savestate(ptr,cra);
}

void __fastcall mc6850::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,cra);
}        */

