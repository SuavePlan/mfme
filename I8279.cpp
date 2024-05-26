//---------------------------------------------------------------------------

#include <stdio.h>
#include "typedefs.h"
#include "I8279.h"
#include "shared.h"

//---------------------------------------------------------------------------

void I8279::write(uint8 offset, uint8 value)
{
  if ( offset == 1 ) {  // Command
    write_display = false;
    switch ( value & 0xe0 ) {
      case 0x0:   // keyboard/Display mode set
        mode = value & 0x1f;
        break;
      case 0x20:  // program clock
        clock = value & 0x1f;
        break;
      case 0x40:  // read FIFO/sensor RAM
        sense_address = value & 0x7;
        sense_auto_inc = value & 0x10;
        read_sensor = true;
        break;
      case 0x60:  // read display RAM
        display_address = value & 0xf;
        display_auto_inc = value & 0x10;
        read_sensor = false;
        break;
      case 0x80:  // write display RAM
        display_address = value & 0xf;
        display_auto_inc = value & 0x10;
        write_display = true;
        break;
      case 0xa0:  // display write inhibit/blanking
        break;
      case 0xc0:  // clear
        break;
      case 0xe0:  // end interupt/error mode set
        break;
    }
  } else if ( write_display ) {  // Data
    lamps_changed = false;
    if ( display_ram[display_address] != value ) {
      lamps_changed = true;
      changed = display_address;
    }
    display_ram[display_address] = value;
    if ( display_auto_inc )
      display_address++;
  }
}

uint8 I8279::read(uint8 offset, bool keys)
{
uint8 value = 0;

  if ( offset )
    return value;//fifo_status;
  else {
    if ( read_sensor && keys ) {
      switch ( sense_address ) {
        case 0:
          value = ~DIP1;
          break;
        case 1:
          value = ~matrix[3];
          break;
        case 2:
          value = ~matrix[5];
          break;
        case 3:
          value = ~matrix[1];
          break;
        case 4:
          value = ~DIP2;
          break;
        case 5:
          value = ~(matrix[2] | (PCkey & 3) | ((PCkey & 12 )<< 4));
          break;
        case 6:
          value = ~matrix[4];
          break;
        case 7:
          value = ~matrix[0];
          break;
      }
      if ( sense_auto_inc )
        sense_address = (sense_address + 1 ) & 7;
    } else {
      value = display_ram[display_address];
      if ( display_auto_inc )
        display_address++;
    }
  }
  return value;
}

void I8279::reset(void)
{
  scan = 0;
  sense_auto_inc = 0;
  display_auto_inc = 0;
  sense_address = 0;
  display_address = 0;
}

void I8279::tick(int cycles)
{
}

