//---------------------------------------------------------------------------

#ifndef DiscReelH
#define DiscReelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Transforms.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TDiscReel : public TImage32
{
private:
    int step;
    int angle;
    bool col;
    Single pos;
    bool drawn;
//    TImageList *onlist;
    TBitmap32 *mask;
    TBitmap32 *Dst;
    TBitmap32 *FSrc;
    TBitmap32 *FOverlay;
    Graphics::TBitmap *FLampMask;
    Graphics::TBitmap *tmp;
    Graphics::TBitmap *FSrc2;
    Graphics::TBitmap *FOverlay2;
    Graphics::TBitmap *bg;
    TBitmap32 *bg2;
    TBitmap32 *on[40];
    int lamps[24];
    int FState;
    TColor background;
    int FOuterH;
    int FOuterL;
    int FInnerH;
    int FInnerL;
    int FLampsize;
    int FOutline;
    int FPosition;
    int FOffset;
    int FLampOffset;
    int FDarkness;
    bool FDrawn;
    bool FInverted;
    bool FReversed;
    int FSteps;
    int FStops;
    void __fastcall ScaleRot(Single Alpha);
    void __fastcall Modulate(TColor32 F, TColor32 &B, TColor32 M);
    void __fastcall DoLamps(void);
    void __fastcall SetAlpha(TBitmap32 *ptr, TColor32 Colour);
    void __fastcall Draw(void);
protected:
    void __fastcall Paint(void);
    void __fastcall Loaded(void);
public:
    __fastcall TDiscReel(TComponent* Owner);
    __fastcall ~TDiscReel(void);
    void __fastcall SetOuterH(int FValue);
    void __fastcall SetOuterL(int FValue);
    void __fastcall SetInnerH(int FValue);
    void __fastcall SetInnerL(int FValue);
    void __fastcall SetLampsize(int FValue);
    void __fastcall SetOutline(int FValue);
    void __fastcall SetPosition(int FValue);
    void __fastcall SetOffset(int FValue);
    void __fastcall SetLampOffset(int FValue);
    void __fastcall SetDarkness(int FValue);
    void __fastcall SetSteps(int FValue);
    void __fastcall SetStops(int FValue);
    void __fastcall SetInverted(bool FValue);
    void __fastcall SetReversed(bool FValue);
    void __fastcall SetSrc(Graphics::TBitmap *ptr);
    void __fastcall SetOverlay(Graphics::TBitmap *ptr);
    void __fastcall SetMask(Graphics::TBitmap *ptr);
    Graphics::TBitmap * __fastcall GetSrc(void);
    Graphics::TBitmap * __fastcall GetOverlay(void);
    Graphics::TBitmap * __fastcall GetMask(void);
    int __fastcall GetLamp(int index);
    void __fastcall SetLamp(int index, int value);
    void __fastcall SetState(int value);
__published:
  __property int OuterH = { read = FOuterH, write = SetOuterH };
  __property int OuterL = { read = FOuterL, write = SetOuterL };
  __property int InnerH = { read = FInnerH, write = SetInnerH };
  __property int InnerL = { read = FInnerL, write = SetInnerL };
  __property int State = { read = FState, write = SetState };
  __property int Lampsize = { read = FLampsize, write = SetLampsize };
  __property int Outline = { read = FOutline, write = SetOutline };
  __property int Position = { read = FPosition, write = SetPosition };
  __property int Offset = { read = FOffset, write = SetOffset };
  __property int LampOffset = { read = FLampOffset, write = SetLampOffset, default = 0 };
  __property int Darkness = { read = FDarkness, write = SetDarkness };
  __property int Stops = { read = FStops, write = SetStops, default = 12 };
  __property int Steps = { read = FSteps, write = SetSteps, default = 96 };
  __property bool Inverted = { read = FInverted, write = SetInverted, default = false };
  __property bool Reversed = { read = FReversed, write = SetReversed, default = false };
  __property Graphics::TBitmap *Source = { read = GetSrc, write = SetSrc };
  __property Graphics::TBitmap *Overlay = { read = GetOverlay, write = SetOverlay };
  __property Graphics::TBitmap *LampMask = { read = GetMask, write = SetMask };
  __property int mask1 = { read = GetLamp, write = SetLamp, index = 0 };
  __property int mask2 = { read = GetLamp, write = SetLamp, index = 1 };
  __property int mask3 = { read = GetLamp, write = SetLamp, index = 2 };
  __property int mask4 = { read = GetLamp, write = SetLamp, index = 3 };
  __property int mask5 = { read = GetLamp, write = SetLamp, index = 4 };
  __property int mask6 = { read = GetLamp, write = SetLamp, index = 5 };
  __property int mask7 = { read = GetLamp, write = SetLamp, index = 6 };
  __property int mask8 = { read = GetLamp, write = SetLamp, index = 7 };
  __property int mask9 = { read = GetLamp, write = SetLamp, index = 8 };
  __property int mask10 = { read = GetLamp, write = SetLamp, index = 9 };
  __property int mask11 = { read = GetLamp, write = SetLamp, index = 10 };
  __property int mask12 = { read = GetLamp, write = SetLamp, index = 11 };
  __property int mask13 = { read = GetLamp, write = SetLamp, index = 12 };
  __property int mask14 = { read = GetLamp, write = SetLamp, index = 13 };
  __property int mask15 = { read = GetLamp, write = SetLamp, index = 14 };
  __property int mask16 = { read = GetLamp, write = SetLamp, index = 15 };
  __property int mask17 = { read = GetLamp, write = SetLamp, index = 16 };
  __property int mask18 = { read = GetLamp, write = SetLamp, index = 17 };
  __property int mask19 = { read = GetLamp, write = SetLamp, index = 18 };
  __property int mask20 = { read = GetLamp, write = SetLamp, index = 19 };
  __property int mask21 = { read = GetLamp, write = SetLamp, index = 20 };
  __property int mask22 = { read = GetLamp, write = SetLamp, index = 21 };
  __property int mask23 = { read = GetLamp, write = SetLamp, index = 22 };
  __property int mask24 = { read = GetLamp, write = SetLamp, index = 23 };
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnMouseDown;
};
//---------------------------------------------------------------------------
#endif
