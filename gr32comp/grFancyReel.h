//---------------------------------------------------------------------------

#ifndef grFancyReelH
#define grFancyReelH
//---------------------------------------------------------------------------
#include "grbase.h"
//---------------------------------------------------------------------------
class TGRFancyReel : public TGRBase
{
private:
    TBitmap32 *mask;
    TBitmap32 *FBand;
    TBitmap32 *FOverlay;
    TBitmap32 *FLampMask;
    TBitmap32 *sized_original;
    TBitmap32 *destination;
    Graphics::TBitmap *FLampMask2;
    Graphics::TBitmap *FBand2;
    Graphics::TBitmap *FOverlay2;
    int lamps[5];
//    int FState;
    int FPosition;
    int FDarkness;
    int FReelHeight;
    int FBorderWidth;
    float *line_array;
    float size;
    float pos;
    int lamp_pos[6];
    TRect rect[6], windowrect, lamprect[6], symrect;
    int symbol;
    bool FReversed;
    int BandHeight;
    int FWinLines;
    int FWinLineWidth;
    TColor FBorderColour;
    TColor32 Dark;
    TBitmap32 *lines;
    bool FExpand;
    int Expanded;
    bool FToggle;
    bool FHorizontal;
    void __fastcall SetAlpha(TBitmap32 *ptr, TColor32 Colour);
    void __fastcall ConstructLampMask(void);
    void __fastcall InitArray(void);
    void __fastcall InitSize(void);
    void __fastcall Setpos2(void);
    void __fastcall Draw(void);
    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
    void __fastcall DrawWinLines(void);
public:
    int FFlag;
    bool FInverted;
    int FOffset;
    int FStops;
    int FSteps;
    bool FLamps;
    bool FCustom;
    __fastcall TGRFancyReel(int ANumber, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRFancyReel(void);
    void __fastcall SetSize(int AWidth, int AHeight);
    void __fastcall SetPos(int FValue);
    void __fastcall SetOffset(int FValue);
    void __fastcall SetDarkness(int FValue);
    void __fastcall SetReelHeight(int FValue);
    void __fastcall SetStops(int FValue);
    void __fastcall SetSteps(int FValue);
    void __fastcall SetBorderWidth(int FValue);
    void __fastcall SetBorderColour(TColor FColour);
    void __fastcall SetWinLines(int FValue);
    void __fastcall SetWinLineWidth(int FValue);
    void __fastcall SetReversed(bool FValue);
    void __fastcall SetLamps(bool FValue);
    void __fastcall SetCustom(bool FValue);
    void __fastcall SetExpand(bool FValue);
    void __fastcall SetToggle(bool FValue);
    void __fastcall SetInverted(bool FValue);
    void __fastcall SetHorizontal(bool FValue);
    void __fastcall SetBand(Graphics::TBitmap *ptr);
    void __fastcall SetOverlay(Graphics::TBitmap *ptr);
    void __fastcall SetMask(Graphics::TBitmap *ptr);
    void __fastcall SetRHeight(int Value);
    Graphics::TBitmap * __fastcall GetBand(void);
    Graphics::TBitmap * __fastcall GetOverlay(void);
    Graphics::TBitmap * __fastcall GetMask(void);
    int __fastcall GetLampNumber(int index);
    void __fastcall SetLampNumber(int index, int value);
    void __fastcall SetLamp(int nbr, int state);
    void __fastcall SetLamps(int state);
    void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif
