//
//
//	sec.cpp
//
//	(C) C.J.Wren 2003
//

#include <stdio.h>
//#include "typedefs.h"
#include "secmeter.h"
//#include "system.h"
//#include "diag.h"
#include "sec.h"
#include "shared.h"

extern char buffer[];

static unsigned char *version = "02E";

SEC::SEC()
{
  reset();
//  for ( int i = 0; i < 31; i++ )
//    counter[i].value = Form1->SEC_Counters[i];
}

void SEC::reset(void)
{
	clk = 1;		// Clear Clk bit
	clks = 0;
    ch = 0;
    pos = 0;
    status = 32;
    rx_clk = 0;
    rx_length = 0;
    rx_pos = 0;
    fingerprint = 0x11010000;
}

void SEC::enable(Byte e)
{
   if ( e ) {
    if ( !enabled ) {
      enabled = true;
      rx_data = 0;
    }
   } else {
    if ( enabled ) {
//      sprintf(buffer, "SEC reset");
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      rx_data = 1;
    }
    enabled = false;
	reset();
   }
}

void SEC::setdata(Byte d)
{
  data1 = d ? 1 : 0;
}

Byte SEC::readdata(void)
{
//    sprintf(buffer, "SEC bit read %02x", value);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  return rx_data;
}

void SEC::setclock(Byte c)
{
  c = c ? 1 : 0;

  if ( (clk ^ c) & 1 ) {
//    sprintf(buffer, "SEC clock %d", c);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
    if ( !c ) {
      ch = ( ch << 1 ) | data1;
      if ( rx_clk == 8 ) {
        rx_clk = 0;
        rx_pos++;
        rx_length--;
//        sprintf(buffer, "SEC char read %02x remaining", rx_length);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }
      if ( rx_length )
        rx_data = (response[rx_pos] & 0x80) >> 7;
      else
        rx_data = enabled ? 0 : 1;
    } else {
      clks++;
      if ( rx_length ) {
        response[rx_pos] <<= 1;
        rx_clk++;
//        sprintf(buffer, "SEC bit read");
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      }
      if ( clks == 8 ) {
        clks = 0;
        if ( !rx_length )
          do_char(ch);
        else {
//          sprintf(buffer, "SEC data in %02x", ch);
//          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
        }
      }
    }
    clk = c;
  }
}

void SEC::do_char(Byte x)
{
//  sprintf(buffer, "SEC %02x", x);
//  DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  command[pos++] = x;
  if ( bytes_left ) {
    bytes_left--;
    if ( !bytes_left ) {
      pos--;
      Processcommand();
      pos = 0;
    }
  }
  if ( pos == 3 )
    bytes_left = x + 1;
}

void SEC::Processcommand(void)
{
Byte csum = 0;
char valbuf[10];
Byte val[4];

  for ( int i = 0; i < pos; i++ )
    csum += command[i];
  if ( csum != command[pos] ) {
//    sprintf(buffer, "SEC Bad Checksum %02x v %02x", csum, command[pos]);
//    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
  }
  last_id = command[1];
  switch ( command[0] ) {
    case 0x20: // Request Status
      Buildresponse(SEC_DATA, last_id, 1, &status);
      break;
    case 0x21: // Request Market Type
      Buildresponse(SEC_DATA, last_id, 1, &market_type);
      break;
    case 0x22: // Request Last Error
      Buildresponse(SEC_DATA, last_id, 1, &last_error);
      break;
    case 0x23: // Request Version
      Buildresponse(SEC_DATA, last_id, 3, version);
      break;
    case 0x24: // Request Counter Value
      sprintf(valbuf,"%07d0", SEC_Counters[command[3]]);
      for ( int i = 0; i < 4; i++ )
        val[i] = (valbuf[i*2] - '0' << 4) | valbuf[1+i*2] - '0';
      Buildresponse(SEC_DATA, last_id, 4, &val[0]);
      break;
    case 0x25: // Request Last Command ID
      Buildresponse(SEC_DATA, last_id, 4, &last_id);
      break;
    case 0x26: // Request Fingerprint
      Buildresponse(SEC_DATA, last_id, 4, (Byte *)&fingerprint);
      break;
    case 0x30: // Set Number of Counters
      nbr_of_counters = command[3];
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x31: // Set Market Type
      market_type = command[3];
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x32: // Set Counter Text
      meter = command[3];
//      memcpy( &counter[meter].text[0], &command[4], 7);
      memcpy( valbuf, &command[4], 7);
      valbuf[7] = 0;
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      strcpy(SEC_Strings[meter], valbuf);
      SECForm->SetText(meter, valbuf);
      SECForm->SetValue(meter, SEC_Counters[meter]);
      break;
    case 0x40: // Show Text
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x41: // Show Counter Value
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x42: // Show Counter Text
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x43: // Show Bit Pattern
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x50: // Counter Increment (Small)
      meter = command[3];
      value = command[4] & 0xf;
      SEC_Counters[meter] += value;
//      counter[meter].value += value;
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      SECForm->SetValue(meter, SEC_Counters[meter]);
      updated = true;
      break;
    case 0x51: // Counter Increment (Medium)
      meter = command[3];
      value = command[4];
      SEC_Counters[meter] += value;
//      counter[meter].value += value;
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      SECForm->SetValue(meter, SEC_Counters[meter]);
      updated = true;
      break;
    case 0x52: // Counter Increment (Large)
      meter = command[3];
      value = command[4] + 256 * command[5];
      SEC_Counters[meter] += value;
//      counter[meter].value += value;
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      SECForm->SetValue(meter, SEC_Counters[meter]);
      updated = true;
      break;
    case 0x54: // Cycle Counter Display
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x55: // Stop Cycle
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
    case 0x5C: // Self Test
      Buildresponse(SEC_ACK, last_id, 0, NULL);
      break;
  }
}

void SEC::Buildresponse(Byte code, Byte id, Byte len, Byte *data)
{
Byte csum = 0;

  response[0] = code;
  response[1] = id;
  response[2] = len;
  if ( len )
    memcpy( &response[3], data, len);
  for ( int i = 0; i < 3 + len; i++ )
    csum += response[i];
  response[3+len] = csum;
  rx_pos = 0;
  rx_clk = 0;
  rx_length = 4 + len;
//  p += sprintf(p,"SEC Response");
//  for ( int i = 0; i < rx_length; i++ )
//    p += sprintf(p," %02x", response[i]);
//  DataWindow->AddMessage((AnsiString)buffer, 0xb0);
}

void __fastcall SEC::SaveToStream(Byte *&ptr)
{
/*  ptr->WriteBuffer(&clk, 1);
  ptr->WriteBuffer(&ch_pos, 1);
  ptr->WriteBuffer(&ch, 1);
  ptr->WriteBuffer(&clks, 1);
  ptr->WriteBuffer(&nbr_digits, 1);
  ptr->WriteBuffer(&chs, 32);
  ptr->WriteBuffer(&reversed, 1);
  ptr->WriteBuffer(&enabled, 1);
  ptr->WriteBuffer(&display, 33);
  ptr->WriteBuffer(&intensity, 1); */
}
void __fastcall SEC::LoadFromStream(Byte *&ptr)
{
/*  ptr->ReadBuffer(&clk, 1);
  ptr->ReadBuffer(&ch_pos, 1);
  ptr->ReadBuffer(&ch, 1);
  ptr->ReadBuffer(&clks, 1);
  ptr->ReadBuffer(&nbr_digits, 1);
  ptr->ReadBuffer(&chs, 32);
  ptr->ReadBuffer(&reversed, 1);
  ptr->ReadBuffer(&enabled, 1);
  ptr->ReadBuffer(&display, 33);
  ptr->ReadBuffer(&intensity, 1); */
}

