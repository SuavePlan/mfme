//---------------------------------------------------------------------------
#ifndef BFMLEDH
#define BFMLEDH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TBFMLED : public TImage32
{
private:
  TBitmap32 *Src;
  TColor FColour;
  int FSizeX;
  int FSizeY;
  int FIntensity;
  int FDigitSpacing;
  int map[20];
  void __fastcall Draw(void);
  void __fastcall DrawColumn(int i);
  void __fastcall Init(void);
protected:
  virtual void __fastcall Paint(void);
  void __fastcall Loaded(void);
public:
    __fastcall TBFMLED (TComponent* Owner);
    __fastcall ~TBFMLED ();
  void __fastcall SetDot(int nbr, int value);
  void __fastcall SetSizeX(int Asize);
  void __fastcall SetSizeY(int Asize);
  void __fastcall SetSpacing(int ASpacing);
  void __fastcall SetColour(TColor AColour);
__published:
  __property int SizeX = { read = FSizeX, write = SetSizeX};
  __property int SizeY = { read = FSizeY, write = SetSizeY};
  __property int Spacing = { read = FDigitSpacing, write = SetSpacing};
  __property TColor Colour = { read = FColour, write = SetColour};
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
