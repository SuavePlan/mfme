//---------------------------------------------------------------------------

#ifndef grbackgroundH
#define grbackgroundH
//---------------------------------------------------------------------------
#include "grbase.h"
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRBackground : TGRBase
{
private:
    TBitmap32 *background;
    int FDarkness;
    TColor Colour;
    void __fastcall Draw(void);
    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
//    void __fastcall DoAlpha(TBitmap32 *ptr);
public:
    __fastcall TGRBackground(int Number, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRBackground(void);
    void __fastcall SetColour(TColor colour);
    void __fastcall SetDarkness(int FValue);
    void __fastcall SetBitmap(Graphics::TBitmap *ptr);
    void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif