//---------------------------------------------------------------------------

#ifndef VideoH
#define VideoH
//---------------------------------------------------------------------------
//#include <SysUtils.hpp>
//#include <Controls.hpp>
//#include <Classes.hpp>
//#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Transforms.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRVideo : public TGRBase
{
private:
    TBitmap32 *shadow;
    Byte *memory;
    Byte palette[32];
    TColor32  colours[16];
    Word *rowtable;
    void __fastcall DisplayChar(int x, int y, int pos, Byte dbl);
//protected:
    void __fastcall Draw(void);
//    void __fastcall Loaded(void);
public:
    void __fastcall WriteMemory(Byte *ptr);
    void __fastcall WriteRowTable(Word *ptr);
    void __fastcall WritePalette(Byte *ptr);
    void __fastcall SetColour(int nbr, TColor colour);
    void __fastcall DrawRow(int row);
    TColor  __fastcall GetColour(int nbr);
    void __fastcall Dump();
    __fastcall TGRVideo(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * ADest);
    __fastcall ~TGRVideo(void);
};
//---------------------------------------------------------------------------
#endif
