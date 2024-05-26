//---------------------------------------------------------------------------

#ifndef grledH
#define grledH
//---------------------------------------------------------------------------
#include "grbase.h"
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRLed : public TGRBase
{
private:
    TBitmap32 * on;
    TBitmap32 * off;
    int FState;
    int FType;
    TColor32 FColour;
    void __fastcall Render(void);
    void __fastcall Draw(void);
//    void __fastcall DoAlpha(TBitmap32 *ptr);
public:
    __fastcall TGRLed(int Number, int X, int Y, int Height, int Width, TImage32 *dest);
    __fastcall ~TGRLed(void);
    void __fastcall SetType(int type);
    void __fastcall SetLamp(int nbr, int state);
    void __fastcall SetColour(TColor colour);
    void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif