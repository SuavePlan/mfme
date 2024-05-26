//---------------------------------------------------------------------------

#ifndef grBandReelH
#define grBandReelH
//---------------------------------------------------------------------------
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRBandReel : public TGRBase
{
private:
    int pos;
    TBitmap32 *mask;
    TBitmap32 *FBand;
    TBitmap32 *FOverlay;
    TBitmap32 *FLampMask;
    Graphics::TBitmap *FLampMask2;
    Graphics::TBitmap *FBand2;
    Graphics::TBitmap *FOverlay2;
    int lamps[5];
    int FPosition;
    int FDarkness;
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
    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
    void __fastcall DrawWinLines(void);
    void __fastcall DrawBand(void);
    void __fastcall Flip(void);
public:
    int FOffset;
    int FStops;
    int FSteps;
    __fastcall TGRBandReel(int ANumber, int aX, int aY, int AHeight, int AWidth, TImage32 * ADest);
    __fastcall ~TGRBandReel(void);
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
    void __fastcall SetLamp(int nbr, int state);
    void __fastcall SetLamps(int state);
};
//---------------------------------------------------------------------------
#endif
