//---------------------------------------------------------------------------

#ifndef grbitmapH
#define grbitmapH
//---------------------------------------------------------------------------
#include "grbase.h"
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRBitmap : public TGRBase
{
private:
    TBitmap32 * bmp;
    bool Transparent;
    void __fastcall Draw(void);
public:
    __fastcall TGRBitmap(int Number, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRBitmap(void);
    void __fastcall SetBitmap(Graphics::TBitmap *bmp);
    void __fastcall SetTransparent(bool trans);
    void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif