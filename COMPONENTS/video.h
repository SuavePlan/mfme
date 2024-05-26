//---------------------------------------------------------------------------

#ifndef VideoH
#define VideoH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Transforms.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TVideo : public TImage32
{
private:
    TBitmap32 *shadow;
    Byte *memory;
    Byte palette[32];
    TColor32  colours[16];
    Word *rowtable;
    void __fastcall DisplayChar(int x, int y, int pos, Byte dbl);
protected:
    void __fastcall Paint(void);
    void __fastcall Loaded(void);
public:
    void __fastcall WriteMemory(Byte *ptr);
    void __fastcall WriteRowTable(Word *ptr);
    void __fastcall WritePalette(Byte *ptr);
    void __fastcall SetColour(int nbr, TColor colour);
    void __fastcall DrawRow(int row);
    TColor  __fastcall GetColour(int nbr);
    void __fastcall Dump();
    __fastcall TVideo(TComponent* Owner);
    __fastcall ~TVideo(void);
__published:
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnMouseDown;
};
//---------------------------------------------------------------------------
#endif
