//---------------------------------------------------------------------------

#ifndef grDiscReelH
#define grDiscReelH
//---------------------------------------------------------------------------
#include "GR32_Image.hpp"
#include "GR32_Transforms.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRDiscReel : public TGRBase
{
private:
    int step;
    int angle;
    bool col;
    Single pos;
    TBitmap32 *mask;
    TBitmap32 *lampmask;
    TBitmap32 *Dst;
    TBitmap32 *FSrc;
    TBitmap32 *FOverlay;
    Graphics::TBitmap *FLampMask;
    Graphics::TBitmap *tmp;
    Graphics::TBitmap *FSrc2;
    Graphics::TBitmap *FOverlay2;
    TBitmap32 *bg2;
    TBitmap32 *on[40];
    int lamps[24];
    int FState;
 //   TColor background;
    int FOuterH;
    int FOuterL;
    int FInnerH;
    int FInnerL;
    int FLampsize;
    int FOutline;
    int FPosition;
    int FLampOffset;
    int FDarkness;
    bool FDrawn;
    bool FReversed;
//    TBitmap32 * dest;
    void __fastcall ScaleRot(Single Alpha);
    void __fastcall Modulate(TColor32 F, TColor32 &B, TColor32 M);
    void __fastcall DoLamps(void);
    void __fastcall SetAlpha(TBitmap32 *ptr, TColor32 Colour);
    void __fastcall Render(void);
    void __fastcall SetMaskAlpha(TBitmap32 *ptr, TColor32 Colour);
//protected:
//    void __fastcall Loaded(void);
public:
    int FOffset;
    bool FInverted;
    int FSteps;
    int FStops;
    __fastcall TGRDiscReel(int Number, int AX, int AY, int AHeight, int AWidth, TImage32 * ADest);
    __fastcall ~TGRDiscReel(void);
    void __fastcall Draw(void);
    void __fastcall SetOuterH(int FValue);
    void __fastcall SetOuterL(int FValue);
    void __fastcall SetInnerH(int FValue);
    void __fastcall SetInnerL(int FValue);
    void __fastcall SetLampsize(int FValue);
    void __fastcall SetOutline(int FValue);
    void __fastcall SetPos(int FValue);
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
    void __fastcall SetLampNumber(int index, int value);
    void __fastcall SetLamp(int index, int value);
    void __fastcall SetLamps(int value);
};
//---------------------------------------------------------------------------
#endif
