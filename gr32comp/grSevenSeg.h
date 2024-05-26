//---------------------------------------------------------------------------
#ifndef grSevenSegH
#define grSevenSegH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TGRSevenSeg : public TGRBase
{
private:
//  TBitmap32 *Src;
//  TBitmap32 *Mask;
//  TBitmap32 *OffMask;
//  TBitmap32 *Trans;
  TBitmap32 *dst;
  int FThickness;
  int FSpace;
  int FHorzSpace;
  int FVertSpace;
  TColor FBackColour;
  TColor FOnColour;
  TColor FOffColour;
  bool FdpRight;
  bool FdpOn;
  bool Fseg14;
  bool FProgrammable;
//  int FMask[16];
//  Graphics::TBitmap *Fbmp;
//  int FColumns;
//  TBitmap32 * dest;
  void __fastcall IncRect(TRect &Rect, int Delta);
  void __fastcall Draw(void);
  void __fastcall DrawSegment(int digit, TCanvas * ptr, TRect rect);
public:
  int FValue;
    __fastcall TGRSevenSeg(int ANumber, int aX, int aY, int AHeight, int AWidth, TImage32 * dest);
    __fastcall ~TGRSevenSeg();
  void __fastcall SetOnColour(TColor AColour);
  void __fastcall SetOffColour(TColor AColour);
  void __fastcall SetBackColour(TColor AColour);
  void __fastcall SetThickness(int FValue);
  void __fastcall SetHorzSpace(int FValue);
  void __fastcall SetVertSpace(int FValue);
  void __fastcall SetSpace(int FValue);
  void __fastcall SetDP(bool FValue);
  void __fastcall SetDPOn(bool FValue);
  void __fastcall Set14(bool FValue);
  void __fastcall SetLamp(int nbr, int FValue);
  void __fastcall SetLamps(int FValue);
  void __fastcall SetProgrammable(bool FValue);
  Graphics::TBitmap * __fastcall GetBitmap(void);
  void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif
