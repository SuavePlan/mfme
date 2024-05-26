//---------------------------------------------------------------------------

#ifndef sharedH
#define sharedH

#include <System.hpp>

//---------------------------------------------------------------------------

enum sys80type {
  S80_LAMP,
  S80_UNUSED,
  S80_METER,
  S80_TRIAC,
  S80_SEG,
  S80_BCD,
  S80_SEG_DIGIT,
  S80_BCD_DIGIT
};

struct sys80_io {
  enum sys80type type;
  System::Byte value;
};

extern struct sys80_io sys80io[512];
extern System::Byte matrix[16];
extern System::Byte Character[72];
extern bool HasSampledSound;
extern bool CharLoaded;
extern bool loading;
extern System::Byte Stake;
extern System::Byte Jackpot;
extern System::Byte Percent;
extern int M1SampType;
extern int Speed;
extern int Speed2;
extern int Throttle;
extern int CharAddr;
extern int Hopper_Type;
extern int Extender_Type;
extern int Program_Type;
extern int Reel_Type;
extern int LED_Type;
extern int Sound_Type;
extern int Volume_Type;
extern int Char_Type;
extern int LVD_Type;
extern int DoorSwitch;
extern int CashSwitch;
extern int RefillSwitch;
extern int MPU3_AUX;
extern int SYS5_SND;
extern int MPU4_TYPE;
extern int TotalIn;
extern int TotalOut;
extern int Percentage;
extern int CashDrift;
extern int STotalIn;
extern int STotalOut;
extern int SCashDrift;
extern int VTP;
extern int ReelLampX;
extern int ReelLampY;
extern int ReelLamp2;
extern int Auxinvert;
extern int DoorInvert;
extern int IRQType;
extern int Rotary;
extern int TotalInTriacMult[10];
extern int TotalOutTriacMult[10];
extern int TotalInMeterMult[10];
extern int TotalOutMeterMult[10];
extern int TotalInSECMult[10];
extern int TotalOutSECMult[10];
extern int MeterSense;
extern int TriacSense;
extern int MeterMask;
extern int TriacMask;
extern int SampleFreq;
extern bool MonitorActive;
extern bool NoSound;
extern System::Byte DIP1;
extern System::Byte DIP2;
extern System::Byte DIP3;
extern System::Byte PCkey;
extern System::Byte STkey;
extern System::Byte JPkey;
extern int Hoppers;
extern int MechType;
extern int SampleRate;
extern int SECFitted;
extern int Stereo;
extern int cointimer;
extern int coinTag;
extern char SEC_Strings[31][8];
extern int SEC_Counters[31];
extern char GameName[101];
extern char RealName[101];
extern char GameVersion[8];
extern bool MonitorActive;
extern int CursorX;
extern int CursorY;
extern void LayoutChanged(void);
extern void UpdateMeters(void);
extern System::Byte StrToBin(char * s);
extern int StrToHex(char * s);
extern void __fastcall UpdateSoundBuffermpu4(unsigned int cycles);
extern void __fastcall UpdateSoundBuffermpu3(unsigned int cycles);
extern void __fastcall UpdateSoundBufferm1a(unsigned int cycles);
extern void __fastcall UpdateSoundBuffermps2(unsigned int cycles);
extern void __fastcall UpdateSoundBuffersys1(unsigned int cycles);
extern void __fastcall UpdateSoundBufferscorp1(unsigned int cycles);
extern void __fastcall UpdateSoundBufferscorp2(unsigned int cycles);
extern void __fastcall UpdateSoundBuffersys80(unsigned int cycles);
extern void __fastcall UpdateSoundBuffersys5(unsigned int cycles);
extern void __fastcall UpdateSoundBuffermpu4video(unsigned int cycles);
extern void __fastcall UpdateSoundBufferspace(unsigned int cycles);
extern void __fastcall UpdateSoundBufferproconn(unsigned int cycles);

#undef NO_IMPLICIT_NAMESPACE_USE

#endif
