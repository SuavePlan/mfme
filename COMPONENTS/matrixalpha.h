//---------------------------------------------------------------------------
#ifndef DotAlphaH
#define DotAlphaH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TDotAlpha : public TImage32
{
private:
  TBitmap32 *Src;
  TColor FColour;
  TBitmap32 *chars;
  int FSizeX;
  int FSizeY;
  int FIntensity;
  int FDigitSpacing;
  int buff[16], oldbuff[16];
  void __fastcall Draw(void);
  void __fastcall Init(void);
  void __fastcall GenerateCharSet(void);
  void __fastcall GenerateChar(int i);
  void __fastcall GenerateCustomChar(int i);
  void __fastcall GetChar(TBitmap32 * ptr, int value, int pos);
protected:
  virtual void __fastcall Paint(void);
  void __fastcall Loaded(void);
public:
    __fastcall TDotAlpha (TComponent* Owner);
    __fastcall ~TDotAlpha ();
  void __fastcall SetDisplay(int *ptr);
  void __fastcall SetChar(Byte *ptr);
  void __fastcall SetSizeX(int Asize);
  void __fastcall SetSizeY(int Asize);
  void __fastcall SetIntensity(int AIntensity);
  void __fastcall SetColour(TColor AColour);
__published:
  __property int SizeX = { read = FSizeX, write = SetSizeX};
  __property int SizeY = { read = FSizeY, write = SetSizeY};
  __property int Intensity = { read = FIntensity, write = SetIntensity};
  __property TColor Colour = { read = FColour, write = SetColour};
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
