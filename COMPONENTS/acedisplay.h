//---------------------------------------------------------------------------
#ifndef aceledH
#define aceledH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TACELED: public TImage32
{
private:
  Byte map[192];
  Byte newmap[192];
  TBitmap32 *Src;
  int FSize;
  int Spacing;
  bool FRotated;
  void __fastcall Draw(void);
  void __fastcall Init(void);
protected:
  virtual void __fastcall Paint(void);
  void __fastcall Loaded(void);
public:
    __fastcall TACELED(TComponent* Owner);
    __fastcall ~TACELED();
  void __fastcall SetPixel(int nbr, bool state);
  void __fastcall SetMap(Byte *ptr);
  void __fastcall SetSize(int Asize);
  void __fastcall SetRotated(bool ARotated);
  Graphics::TBitmap * __fastcall GetBitmap(void);
__published:
  __property int Size = { read = FSize, write = SetSize, default = 7};
  __property bool Rotated = { read = FRotated, write = SetRotated, default = false};
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
