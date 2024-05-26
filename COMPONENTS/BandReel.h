//---------------------------------------------------------------------------

#ifndef BandReelH
#define BandReelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TBandReel : public TGraphicControl
{
private:
    int pos;
    TBitmap32 *mask;
    TBitmap32 *tmp;
    TBitmap32 *FBand;
    TBitmap32 *FOverlay;
    TBitmap32 *FLampMask;
    Graphics::TBitmap *FLampMask2;
    Graphics::TBitmap *FBand2;
    Graphics::TBitmap *FOverlay2;
    int lamps[5];
    int FState;
    int FPosition;
    int FOffset;
    int FDarkness;
    int FStops;
    int FSteps;
    int FBorderWidth;
    int lamp_pos[5];
    TRect windowrect, lamprect[5];
    int symbol;
    bool FReversed;
    int FNumber;
    bool FLamps;
    bool FVertical;
    TColor FBorderColour;
    TColor32 Dark;
    int symwidth;
    int viewwidth;
    int bandwidth;
    void __fastcall SetAlpha(TBitmap32 *ptr, TColor32 Colour);
    void __fastcall ConstructLampMask(void);
    void __fastcall Setpos(void);
    void __fastcall Draw(void);
    void __fastcall Resized(TObject *Sender);
    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
    void __fastcall DrawWinLines(void);
    void __fastcall DrawBand(void);
    void __fastcall Flip(void);
protected:
    void __fastcall Paint(void);
    void __fastcall Loaded(void);
public:
    __fastcall TBandReel(TComponent* Owner);
    __fastcall ~TBandReel(void);
    void __fastcall SetPosition(int FValue);
    void __fastcall SetOffset(int FValue);
    void __fastcall SetDarkness(int FValue);
    void __fastcall SetStops(int FValue);
    void __fastcall SetSteps(int FValue);
    void __fastcall SetBorderWidth(int FValue);
    void __fastcall SetBorderColour(TColor FColour);
    void __fastcall SetNumber(int FValue);
    void __fastcall SetReversed(bool FValue);
    void __fastcall SetLamps(bool FValue);
    void __fastcall SetVertical(bool FValue);
    void __fastcall SetBand(Graphics::TBitmap *ptr);
    void __fastcall SetOverlay(Graphics::TBitmap *ptr);
    void __fastcall SetMask(Graphics::TBitmap *ptr);
    Graphics::TBitmap * __fastcall GetBand(void);
    Graphics::TBitmap * __fastcall GetOverlay(void);
    Graphics::TBitmap * __fastcall GetMask(void);
    int __fastcall GetLampNumber(int index);
    void __fastcall SetLampNumber(int index, int value);
    void __fastcall SetLamp(int nbr, bool state);
__published:
  __property int Position = { read = FPosition, write = SetPosition };
  __property int Offset = { read = FOffset, write = SetOffset };
  __property int Stops = { read = FStops, write = SetStops };
  __property int TotalSteps = { read = FSteps, write = SetSteps };
  __property bool Reversed = { read = FReversed, write = SetReversed };
  __property int BorderWidth = { read = FBorderWidth, write = SetBorderWidth, default = 1 };
  __property TColor BorderColour = { read = FBorderColour, write = SetBorderColour };
  __property int Number = { read = FNumber, write = SetNumber };
  __property bool Lamps = { read = FLamps, write = SetLamps };
  __property bool Vertical = { read = FVertical, write = SetVertical };
  __property Graphics::TBitmap *Band = { read = GetBand, write = SetBand };
  __property Graphics::TBitmap *Overlay = { read = GetOverlay, write = SetOverlay };
  __property Graphics::TBitmap *LampMask = { read = GetMask, write = SetMask };
  __property int Lamp1 = { read = GetLampNumber, write = SetLampNumber, index = 0 };
  __property int Lamp2 = { read = GetLampNumber, write = SetLampNumber, index = 1 };
  __property int Lamp3 = { read = GetLampNumber, write = SetLampNumber, index = 2 };
  __property int Lamp4 = { read = GetLampNumber, write = SetLampNumber, index = 3 };
  __property int Lamp5 = { read = GetLampNumber, write = SetLampNumber, index = 4 };
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnMouseDown;
};
//---------------------------------------------------------------------------
#endif
