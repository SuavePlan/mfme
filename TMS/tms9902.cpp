//---------------------------------------------------------------------------

#define NO_DUART_DEBUG

#include <stdio.h>
#include <string.h>
#include "tms9902.h"
//---------------------------------------------------------------------------

// Equivalent to Baud Rate of Channel

//#define TX_SPEED 600
//#define RX_SPEED 600

// Interupt - masks & pending

#define RX_INT 1
#define TX_INT 2
#define TIM_INT 4
#define DSR_CTS_INT 8

// Events
#define PARITY_ERROR 0x1
#define OVERRUN_ERROR 0x2
#define FRAMING_ERROR 0x4
#define FIRST_DATA_BIT 0x8
#define START_BIT 0x10
#define RECEIVE_BUFFER_LOADED 0x20
#define TRANSMIT_BUFFER_EMPTY 0x40
#define TRANSMIT_SHIFT_REG_EMPTY 0x80
#define TIMER_OVERRUN 0x100
#define TIMER_TIMEOUT 0x200
#define DSR_CTS_CHANGED 0x400

void TMS9902::write(int offset, int x)
{
  if ( offset <= 10 )
    if ( x )
      value |= (1<<offset);
    else
      value &= ~(1<<offset);

  switch ( offset ) {
    case 7:
      if ( control & 8 ) {
        parameter = value;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Parameter (%d) set to %04X", nbr, value);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
        if ( parameter & 8 ) {
          clk = 2 * (64 * 4 * 4);// * 10;
          divider = 4;
        } else {
          clk = 2 * (64 * 3 * 4);// * 10;
          divider = 3;
        }
        control &= ~8;
      } else if ( control & 4 ) {
        timer_interval = value;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Timer (%d) set to %02X", nbr, value);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
        timer = value;
        control &= ~4;
      } else if ( !control ) {
#ifdef DUART_DEBUG
         {
          char buffer[50];
          sprintf( buffer, "TX Char (%d) %02X", nbr, value);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
        tx_char = value;
        pending &= ~TX_INT;
        event &= ~TRANSMIT_BUFFER_EMPTY;
        if ( !RTS && (tx_delay <= 0) )
          tx_delay = tx_speed;
      }
      break;
    case 10:
      if ( (control & 0xe) == 2) {
        rx_rate = value;
        rx_speed = divider * 4 * 2 * rx_rate * 8; //8
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "RX Rate (%d) set to %04X rx speed %d", nbr, value, rx_speed);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
        control &= ~2;
      }
      if ( (control & 0xd) == 1) {
        tx_rate = value;
        tx_speed = divider * 4 * 2 * tx_rate * 8; //8
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "TX Rate (%d) set to %04X tx_speed %d", nbr, value, tx_speed);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
//        control &= ~1;
      }
      break;
    case 11:
    case 12:
    case 13:
    case 14:
      if ( x )
        control |= 1<<(offset-11);
      else
        control &= ~(1<<(offset-11));
      break;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Control (%d) %02X",nbr,control);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
    case 15:
      test_mode = x;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Enable (%d) Test %02X",nbr,x);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 16:
      RTS = !x;
      if ( nbr == 1 )
        CTS = RTS;
      if ( !RTS && !(event & TRANSMIT_BUFFER_EMPTY) && (tx_delay <= 0) )
        tx_delay = tx_speed;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Set RTS (%d) %02X",nbr,RTS);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 17:
      tx_break = x;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "TX Break (%d) %02X",nbr,x);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 18:
      if ( x )
        mask |= RX_INT;
      else
        mask &= ~RX_INT;
      pending &= ~RX_INT;
      event &= ~(RECEIVE_BUFFER_LOADED);
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "RX Int (%d) %02X",nbr,x);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 19:
      if ( x )
        mask |= TX_INT;
      else
        mask &= ~TX_INT;
      pending &= ~TX_INT;
      if ( (mask & TX_INT ) && (event & TRANSMIT_BUFFER_EMPTY) )
        pending |= TX_INT; 
//          {
//          char buffer[50];
//          sprintf( buffer, "TX Int (%d) %02X",nbr,x);
//          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
//          }
      break;
    case 20:
      if ( x )
        mask |= TIM_INT;
      else
        mask &= ~TIM_INT;
      pending &= ~TIM_INT;
      event &= ~(TIMER_TIMEOUT | TIMER_OVERRUN);
      break;
    case 21:
      if ( x )
        mask |= DSR_CTS_INT;
      else
        mask &= ~DSR_CTS_INT;
      pending &= ~DSR_CTS_INT;
      event &= ~DSR_CTS_CHANGED;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "DSR CTS Int (%d) %02X",nbr, x);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 31:
      reset(nbr);
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "Reset (%d)",nbr);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    default:
      break;
  }
}

int TMS9902::read(int offset)
{
int ret = 0;

  if ( offset < 8 )
    ret = receive_buffer & (1 << offset);
  else
  switch ( offset ) {
    case 9:
      ret = event & (PARITY_ERROR|OVERRUN_ERROR|FRAMING_ERROR);
      break;
    case 10:
      ret = event & PARITY_ERROR;
      break;
    case 11:
      ret = event & OVERRUN_ERROR;
      break;
    case 12:
      ret = event & FRAMING_ERROR;
      break;
    case 13:
      ret = event & FIRST_DATA_BIT;
      break;
    case 14:
      ret = event & START_BIT;
      break;
    case 15:
      ret = RxD;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading RxD (%d)",nbr);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 16:
      ret = pending & RX_INT;
      break;
    case 17:
      ret = pending & TX_INT;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading TRANSMIT INT PENDING (%d)",nbr);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 18: // Not Used
      break;
    case 19:
      ret = pending & TIM_INT;
      break;
    case 20:
      ret = pending & DSR_CTS_INT;
      break;
    case 21:
      ret = event & RECEIVE_BUFFER_LOADED;
      break;
    case 22:
      ret = event & TRANSMIT_BUFFER_EMPTY;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading TRANSMIT_BUFFER_EMPTY (%d)",nbr);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 23:
      ret = event & TRANSMIT_SHIFT_REG_EMPTY;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading TRANSMIT_SHIFT_REG_EMPTY (%d)",nbr);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 24:
      ret = event & TIMER_OVERRUN;
      break;
    case 25:
      ret = event & TIMER_TIMEOUT;
      break;
    case 26:
      ret = !RTS;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading RTS (%d) %02X",nbr, ret);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 27:
      ret = !DSR;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading DSR %02X",ret);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 28:
      ret = !CTS;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading CTS %02X",ret);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 29:
      ret = event & DSR_CTS_CHANGED;
#ifdef DUART_DEBUG
          {
          char buffer[50];
          sprintf( buffer, "*********** Reading DSR CTS Changed %02X",ret);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
          }
#endif
      break;
    case 30:
      ret = control | tx_break;
      break;
    case 31:
      ret = pending;
      break;
  }
  return ret ? 1 : 0;
}

void TMS9902::rx_char(uint8 chr)
{
  event &= ~(PARITY_ERROR | FRAMING_ERROR | FIRST_DATA_BIT | START_BIT | OVERRUN_ERROR);
  if ( event & RECEIVE_BUFFER_LOADED )
    event |= OVERRUN_ERROR;
  receive_buffer = chr;
  event |= RECEIVE_BUFFER_LOADED;
  if ( mask & RX_INT )
    pending |= RX_INT;
}

void TMS9902::reset(int Anbr)
{
  nbr = Anbr;
  control = 0xf;
  value = 0;
  pending = 0;
  mask = 0;
  event = TRANSMIT_BUFFER_EMPTY;
  timer_interval = 0;
  timer = 0;
  prescaler = 0;
  clk = 64 * 3 * 4 * 10;
  RTS = 1;
  tx_delay = 0;
  tx_break = 0;
  receive_buffer = 0;
  rx_buffer[0] = 0x76;
  rx_count = 0;
}

void TMS9902::tick(int cycles)
{
  if ( timer && ( (control & 4) == 0) ) {
    prescaler += cycles;
    while ( prescaler >= clk ) {
      timer--;
      prescaler -= clk;
      if ( !timer ) {
        if ( event & TIMER_TIMEOUT )
          event |= TIMER_OVERRUN;
//        else
        event |= TIMER_TIMEOUT;
        if ( mask & TIM_INT )
          pending |= TIM_INT;
        timer = timer_interval;
      }
    }
  }
  if ( tx_delay > 0) {
    if ( tx_delay == tx_speed ) {
      tx_register = tx_char;
      event |= TRANSMIT_BUFFER_EMPTY;
      event &= ~TRANSMIT_SHIFT_REG_EMPTY;
      if ( mask & TX_INT )
        pending |= TX_INT;
    }
    tx_delay-=cycles;
    if ( tx_delay <= 0 ) {
#ifdef DUART_DEBUG
        {
          char buffer[50];
          sprintf(buffer,"tx timeout (%d) %02x", nbr, tx_register);
          DataWindow->AddMessage(AnsiString(buffer), 0xb0);
        }
#endif
      RxChar = true;
      if ( event & TRANSMIT_BUFFER_EMPTY ) {
        event |= TRANSMIT_SHIFT_REG_EMPTY;
        rx_char(tx_register);
      } else if ( !RTS )
        tx_delay = tx_speed;
    }
  }
  if ( rx_delay > 0 )
    rx_delay-=cycles;
  if ( rx_delay <= 0 && rx_count ) {
    rx_count--;
#ifdef DUART_DEBUG
    {
      char buffer[50];
      sprintf( buffer, "RX Char (%d) %02X",nbr,*rx_ptr);
      DataWindow->AddMessage(AnsiString(buffer), 0xb0);
    }
#endif
    rx_char(*rx_ptr++);
    rx_delay = rx_speed;
  }
}

void TMS9902::TxReply(char *ptr)
{
  strcpy( rx_buffer, ptr );
  rx_ptr = rx_buffer;
  rx_count = strlen(rx_buffer);
  rx_delay = rx_speed;
}

