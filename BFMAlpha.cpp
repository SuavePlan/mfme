//
//
//	BFMalpha.cpp
//
//	(C) C.J.Wren 2003
//

#define NO_ALPHA_DEBUG

#include <stdio.h>
#ifdef ALPHA_DEBUG
#include "system.h"
#else
#include "typedefs.h"
#endif
#include "BFMalpha.h"

extern char buffer[];

BFMalpha::BFMalpha()
{
	reset();
}

BFMalpha::~BFMalpha()
{
}

void BFMalpha::update()
{
int i;

  if ( flashtimer ) {
    flashtimer--;
    if ( !flashtimer ) {
      flashtimer = 20;
      if ( !flash ) {
        switch ( flashcontrol ) {
          case 1:    // Flash Inside Window
            for ( i = 0; i < 16; i++ ) {
              if ( (i >= window_start) && (i <= window_end) )
                flashchs[i] = 0x20;
              else
                flashchs[i] = chs[i];
            }
            flash = true;
            display_changed++;
            break;
          case 2:    // Flash Outside Window
            for ( i = 0; i < 16; i++ ) {
              if ( (i < window_start) || (i > window_end) )
                flashchs[i] = 0x20;
              else
                flashchs[i] = chs[i];
            }
            flash = true;
            display_changed++;
            break;
          case 3:    // Flash All
            for ( i = 0; i < 16; i++ )
              flashchs[i] = 0x20;
            flash = true;
            display_changed++;
            break;
        }
      } else {
        flashrate--;
        if ( !flashrate ) {
          flashtimer = 0;
        }
        if ( flashcontrol ) {
          flash = false;
          display_changed++;
        }
      }
    }
  }
}

void BFMalpha::reset()
{
	Byte	count;

	clk = 0;		// Clear Clk bit
	ch_pos = 0;
    scroll_pos = 0;
	clks = 0;
    nbr_digits = 16;

  // Reset display RAM
	for ( count = 0; count < 16; count++ )
		chs[count] = 0x20;


//    intensity = 31;
    ch = 0;
//    intensity_next = false;
    clearmode = 0;
    window_start = 0;
    window_end = 15;
    mode = 0;
    flashrate = 0;
    flashcontrol = 0;
    flash = false;
    flashtimer = 0;
    flash_changed = 0;
    enabled = false;
}

void BFMalpha::enable(Byte e)
{
   if ( e ) {
    enabled = true;
   } else {
    enabled = false;
	reset();
#ifdef ALPHA_DEBUG
    sprintf(buffer, "Alpha Reset");
    DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
    display_changed++;
//    intensity_changed++;
   }
}

void BFMalpha::write_all(int Reset, int Clock, int Data)
{
  Data = Data ? 0 : 1;
  Clock = Clock ? 1 : 0;
  Reset = Reset ? 1 : 0;

//#ifdef ALPHA_DEBUG
//  sprintf(buffer, "Alpha Write %d %d %d ", Reset, Clock, Data);
//  DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//#endif

  if ( Reset ) {
    if ( !enabled ) {
      reset();
      enabled = true;
#ifdef ALPHA_DEBUG
      sprintf(buffer, "Alpha Reset");
      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
/*      if ( enabled ) {
        reset();  */
        display_changed++;
//        intensity_changed++;
//      }
//      enabled = false;
      clk = Clock;
//      return;
    }
  } else {
    enabled = false;
    return;
  }
  if ( Clock != clk ) {
    write_clock(Clock, Data);
    clk = Clock;
  }
}

void BFMalpha::write_clock(int c, int data)
{

  if ( clk != c ) {
    if ( !c ) {
      ch = ( ch << 1 ) | data;
      clks++;
      if ( clks == 8 ) {
        clks = 0;
        do_char((int)ch);
      }
    }
    clk = c;
  }
}

void BFMalpha::do_char(int x)
{
int count;
bool redraw = false;
int redraw_pos;

#ifdef ALPHA_DEBUG
  sprintf(buffer, "Alpha %02X", x);
  DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif

  updated = false;
  if ( enabled ) {
    if ( (x & 0x80) && !skip ) {
      DoCommand(x);
      dp_next = false;
    } else {
      if ( skip ) {
        custom = (custom >> 8) | (x << 8);
        skip--;
        if ( skip )
          return;
      else
          x = custom |= 0x2000000;
      }
      if ( x == 37 || x == 38 || x == 59 )
        x = -1;
      else if ( x == 58 ) { // Custom char !
        skip = 2;
        custom = 0;
      return;
    }
      if ( x >= 0 ) {
        if ( x == '.' || x == ',' ) {
          if ( !(chs[last_pos] & 0xff0000) )
            updated = true;
          chs[last_pos] |= x << 16;
          dp_next = false;
        } else {
          if ( dp_next && (chs[last_pos] & 0xff0000) ) {
            chs[last_pos] &= 0x300ffff;
            updated = true;
          }
          last_pos = ch_pos;
          switch ( mode ) {
            case 0: // Left rotate
              if ( (chs[ch_pos] & 0xffff) != x )
                updated = true;
              chs[ch_pos] &= 0x1ff0000;
              chs[ch_pos] |= x;
              redraw_pos = ch_pos;
              dp_next = true;
              ch_pos++;
              ch_pos &= 0xf;
              break;
            case 1: // Right rotate
              if ( (chs[ch_pos] & 0xffff) != x )
                updated = true;
              chs[ch_pos] &= 0x1ff0000;
              chs[ch_pos] |= x;
              redraw_pos = ch_pos;
              dp_next = true;
              ch_pos--;
              ch_pos &= 0xf;
              break;
            case 2: // Left Scroll
              dp_next = true;
              if ( scroll_pos < window_end ) {
                scroll_pos = ch_pos;
                if ( (chs[ch_pos] & 0xffff) != x )
                  updated = true;
                chs[ch_pos] &= 0x1ff0000;
                chs[ch_pos] |= x;
                redraw_pos = ch_pos;
                ch_pos++;
              } else {
                for ( count = window_start; count < window_end; count++ )
                  chs[count] = chs[count+1];
                chs[window_end] &= 0x1ff0000;
                redraw_pos = window_end;
                chs[window_end] |= x;
                last_pos = window_end;
                updated = true;
              }
              break;
            case 3: // Right Scroll
              dp_next = true;
              if ( scroll_pos > window_start ) {
                scroll_pos = ch_pos;
                if ( (chs[ch_pos] & 0xffff) != x )
                  updated = true;
                chs[ch_pos] &= 0x1ff0000;
                chs[ch_pos] |= x;
                redraw_pos = ch_pos;
                ch_pos--;
              } else {
                for ( count = window_end; count > window_start; count-- )
                  chs[count] = chs[count-1];
                chs[window_start] &= 0x1ff0000;
                chs[window_start] |= x;
                last_pos = window_start;
                redraw_pos = window_start;
                updated = true;
              }
          break;
      }
    }
    }
      }
    if ( updated )
      display_changed++;
  }
    }

void BFMalpha::DoCommand(int x)
{
int count;

      switch ( x & 0xf0 ) {
        case 0x80: // Set Display Blanking
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Set Blanking %02X", x);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          switch ( x ) {
            case 0x80:
              for ( count = 0; count < 16; count++ )
                chs[count] |= 0x1000000;
              updated = true;
              break;
            case 0x81:
              for ( count = 0; count < window_start; count++ )
                chs[count] |= 0x1000000;
              for ( count = window_end + 1; count < 16; count++ )
                chs[count] |= 0x1000000;
              updated = true;
              break;
            case 0x82:
              for ( count = window_start; count < window_end; count++ )
                chs[count] |= 0x1000000;
              updated = true;
              break;
            case 0x83:
              for ( count = 0; count < 16; count++ )
                chs[count] &= 0x2FFFFFF;
              updated = true;
              break;
            case 0x85:
              break;
            default:
#ifdef ALPHA_DEBUG
              sprintf(buffer, "Unknown Blanking %02X", x);
              DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
              break;
          }
          break;
        case 0x90:  // Set Buffer pointer
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Buffer Pos %02X", x);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          ch_pos = x & 0x0f;
          scroll_pos = ch_pos;
          break;
        case 0xa0: // Set Display Mode
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Set Mode %02X", x);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          switch ( x ) {
            case 0xa0:
            case 0xa1:
            case 0xa2:
            case 0xa3:
              mode = x & 3;
              break;
            default:
//              sprintf(buffer, "Unknown Mode %02X", x);
//              DataWindow->AddMessage((AnsiString)buffer, 0xb0);
              break;
          }
          break;
        case 0xb0: // Clear Display
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Clear Display %02X", x);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          switch ( x ) {
            case 0xb0:
              break;
            case 0xb1:
              for ( count = window_start; count < window_end; count++ ) {
	            chs[count] = 0x20;
              }
              updated = true;
              break;
            case 0xb2:
              for ( count = 0; count < window_start; count++ ) {
	            chs[count] = 0x20;
              }
              for ( count = window_end + 1; count < 16; count++ ) {
	            chs[count] = 0x20;
              }
              updated = true;
              break;
            case 0xb3:
              for ( count = 0; count < 16; count++ ) {
	            chs[count] = 0x20;
              }
              updated = true;
              break;
            case 0xb8:   // Clear to char outside window
              clearmode = x;
              break;
            default:
#ifdef ALPHA_DEBUG
              sprintf(buffer, "Unknown Clear %02X", x);
              DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
              break;
          }
          break;
        case 0xC0: // Set Flash Rate
          flashrate = x & 0xf;
          if ( !flashrate && flash ) {
            flashtimer = 0;
            flash = false;
            display_changed++;
          }
          flashtimer = 20;
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Flash Rate %02X", x);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          break;
        case 0xd0: // Flash Control
          flashcontrol = x & 0xf;
          if ( flashcontrol == 0 && flash ) {
            flashtimer = 0;
            flash = false;
            display_changed++;
          }
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Flash Control %02X", x);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          break;
        case 0xE0: // Start of Window
          window_start = x & 0xf;
          window_size = (window_end - window_start) + 1;
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Window Start %02X", window_start);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          break;
        case 0xF0: // End of window
          window_end = x & 0xf;
          window_size = (window_end - window_start) + 1;
#ifdef ALPHA_DEBUG
          sprintf(buffer, "Window End %02X", window_end);
          DataWindow->AddMessage((AnsiString)buffer, 0xb0);
#endif
          break;
      }
}

void __fastcall BFMalpha::SaveToStream(Byte *&ptr)
{
  savestate(ptr,clk);
  savestate(ptr,ch_pos);
  savestate(ptr,ch);
  savestate(ptr,clks);
  savestate(ptr,nbr_digits);
  savestate(ptr,chs);
  savestate(ptr,enabled);
}
void __fastcall BFMalpha::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,clk);
  readstate(ptr,ch_pos);
  readstate(ptr,ch);
  readstate(ptr,clks);
  readstate(ptr,nbr_digits);
  readstate(ptr,chs);
  readstate(ptr,enabled);
  display_changed++;
//  Fruit->do_display();
}

