//---------------------------------------------------------------------------
#ifndef proconnledH
#define proconnledH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TPROCONLED: public TImage32
{
private:
  Byte map[80];
  Byte newmap[80];
  TBitmap32 *Src;
  int FSize;
  int Spacing;
  void __fastcall Draw(void);
  void __fastcall Init(void);
protected:
  virtual void __fastcall Paint(void);
  void __fastcall Loaded(void);
public:
    __fastcall TPROCONLED(TComponent* Owner);
    __fastcall ~TPROCONLED();
  void __fastcall SetPixel(int nbr, bool state);
  void __fastcall SetMap(Byte *ptr);
  void __fastcall SetSize(int Asize);
  Graphics::TBitmap * __fastcall GetBitmap(void);
__published:
  __property int Size = { read = FSize, write = SetSize, default = 7};
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
