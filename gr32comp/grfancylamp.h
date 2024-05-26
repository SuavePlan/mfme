//---------------------------------------------------------------------------

#ifndef grfancylampH
#define grfancylampH
//---------------------------------------------------------------------------
#include "grbase.h"
#include "GR32_Image.hpp"
//#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRFancyLamp : public TGRBase
{
private:
    TBitmap32 *overlay;
    TBitmap32 *on1;
    TBitmap32 *on2;
    TBitmap32 *off;
    TBitmap32 *on1image;
    TBitmap32 *on2image;
    TBitmap32 *offimage;
    TBitmap32 *FLampMask;
    TFont * Font;
    TColor On1Colour;
    TColor On2Colour;
    TColor OffColour;
    bool Graphic;
    bool Transparent;
    bool Outline;
    String Text;
//    int FState;
    int Shape;
    bool Down;
    void __fastcall Darken(TColor32 F, TColor32 &B, TColor32 M);
//    void __fastcall DoAlpha(TBitmap32 *ptr, TColor32 Colour);
    void __fastcall Render(int State, TBitmap32 * bmp);
public:
    __fastcall TGRFancyLamp(int Number, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRFancyLamp(void);
    void __fastcall SetDown(bool value);
    void __fastcall SetLamps(int FValue);
    void __fastcall SetLamp(int nbr, int FValue);
    void __fastcall SetGraphic(bool value);
    void __fastcall SetOutline(bool value);
    void __fastcall SetTransparent(bool value);
    void __fastcall SetFont(TFont *font);
    void __fastcall SetText(String text);
    void __fastcall SetShape(int shape);
    void __fastcall SetOn1Colour(TColor colour);
    void __fastcall SetOn2Colour(TColor colour);
    void __fastcall SetOffColour(TColor colour);
    void __fastcall SetOn1(Graphics::TBitmap *ptr);
    void __fastcall SetOn2(Graphics::TBitmap *ptr);
    void __fastcall SetOff(Graphics::TBitmap *ptr);
    void __fastcall SetMask(Graphics::TBitmap *ptr);
    void __fastcall SaveToStream(TStream *ptr);
    void __fastcall Draw(void);
};
//---------------------------------------------------------------------------
#endif