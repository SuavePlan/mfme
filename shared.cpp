//---------------------------------------------------------------------------


#include "typedefs.h"
#include "shared.h"
#include <ctype.h>

//---------------------------------------------------------------------------

System::Byte matrix[16];
System::Byte Character[72];
bool HasSampledSound;
bool CharLoaded;
bool loading;
System::Byte Stake;
System::Byte Jackpot;
System::Byte Percent;
int M1SampType;
int Speed;
int Speed2;
int Throttle;
int CharAddr;
int Hopper_Type;
int Extender_Type;
int Program_Type;
int Reel_Type;
int LED_Type;
int Sound_Type;
int Volume_Type;
int Char_Type;
int LVD_Type;
int DoorSwitch;
int CashSwitch;
int RefillSwitch;
int MPU3_AUX;
int TotalIn;
int TotalOut;
int Percentage;
int CashDrift;
int STotalIn;
int STotalOut;
int SCashDrift;
int VTP;
int ReelLampX;
int ReelLampY;
int ReelLamp2;
int Auxinvert;
int DoorInvert;
int IRQType;
int Rotary;
int TotalInTriacMult[10];
int TotalOutTriacMult[10];
int TotalInMeterMult[10];
int TotalOutMeterMult[10];
int TotalInSECMult[10];
int TotalOutSECMult[10];
int MeterSense;
int TriacSense;
int MeterMask;
int TriacMask;
int SampleFreq;
int SYS5_SND;
int MPU4_TYPE;
bool MonitorActive;
bool NoSound;
System::Byte DIP1;
System::Byte DIP2;
System::Byte DIP3;
System::Byte PCkey;
System::Byte STkey;
System::Byte JPkey;
int Hoppers;
int MechType;
int SampleRate;
int SECFitted;
int Stereo;
int cointimer;
int coinTag;
char SEC_Strings[31][8];
int SEC_Counters[31];
char GameName[101];
char RealName[101];
char GameVersion[8];
struct sys80_io sys80io[512];
//int jump_lo;
//int jump_hi;
int CursorX;
int CursorY;

//---------------------------------------------------------------------------
System::Byte StrToBin(char * s)
{
System::Byte value;
char *ch_ptr;
System::Byte mask;

  value = 0;
  mask = 1;
  ch_ptr = s;
  while ( *ch_ptr ) {
    if ( *ch_ptr == '1' || *ch_ptr == '0' )
        value += mask * (*ch_ptr - '0');
        mask = mask << 1;
    ch_ptr++;
  }
  return value;
}
//---------------------------------------------------------------------------
int StrToHex(char * s)
{
int count;
char *ch_ptr;

  count = 0;
  ch_ptr = s;
  if ( *ch_ptr ) {
    while ( *ch_ptr ) {
      if ( isxdigit(*ch_ptr) )
        count = count * 16 + ( *ch_ptr >= '0' &&
                               *ch_ptr <= '9' ? *ch_ptr-'0' :
                               toupper(*ch_ptr)-'A' + 10);
      ch_ptr++;
    }
    return count;
  } else
    return -1;
}
//---------------------------------------------------------------------------

