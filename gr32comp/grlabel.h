//---------------------------------------------------------------------------

#ifndef grlabelH
#define grlabelH
//---------------------------------------------------------------------------
#include "grbase.h"
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRLabel : TGRBase
{
private:
    TBitmap32 * buffer;
    String text;
    TColor32 FColour;
    bool FTransparent;
    void __fastcall Draw(void);
//    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
//    void __fastcall DoAlpha(TBitmap32 *ptr);
public:
    __fastcall TGRLabel(int Number, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRLabel(void);
    void __fastcall SetText(String txt);
    void __fastcall SetFont(TFont *font);
    void __fastcall SetColour(TColor colour);
    void __fastcall SetTransparent(bool trans);
    void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif