//---------------------------------------------------------------------------

#ifndef grbuttonH
#define grbuttonH
//---------------------------------------------------------------------------
#include "grbase.h"
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRButton : public TGRBase
{
private:
    String Text;
    TBitmap32 *on;
    TBitmap32 *off;
    TBitmap32 *FLampMask;
    TBitmap32 *bitmap;
    TColor OnColour;
    TColor OffColour;
    bool Down;
    bool Graphic;
//    int State;
    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
//    void __fastcall DoAlpha(TBitmap32 *ptr);
public:
    __fastcall TGRButton(int Number, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRButton(void);
    void __fastcall SetLamps(int FValue);
    void __fastcall SetLamp(int nbr, int FValue);
    void __fastcall SetText(String text);
    void __fastcall SetFont(TFont *font);
    void __fastcall SetDown(bool value);
    void __fastcall SetGraphic(bool value);
    void __fastcall SetOnColour(TColor Color);
    void __fastcall SetOffColour(TColor Color);
    void __fastcall SetOn(Graphics::TBitmap *ptr);
    void __fastcall SetOff(Graphics::TBitmap *ptr);
    void __fastcall SetMask(Graphics::TBitmap *ptr);
    void __fastcall SaveToStream(TStream *ptr);
    void __fastcall Draw(void);
};
//---------------------------------------------------------------------------
#endif