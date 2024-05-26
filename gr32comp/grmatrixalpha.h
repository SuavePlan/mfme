//---------------------------------------------------------------------------
#ifndef grDotAlphaH
#define grDotAlphaH
//---------------------------------------------------------------------------
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRDotAlpha : public TGRBase
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
  void __fastcall Render(void);
  void __fastcall Init(void);
  void __fastcall GenerateCharSet(void);
  void __fastcall GenerateChar(int i);
  void __fastcall GenerateCustomChar(int i);
  void __fastcall GetChar(TBitmap32 * ptr, int value, int pos);
//protected:
  void __fastcall Draw(void);
//  void __fastcall Loaded(void);
public:
    __fastcall TGRDotAlpha (int ANumber, int aX, int aY, int AHeight, int AWidth, TImage32 * ADest);
    __fastcall ~TGRDotAlpha ();
  void __fastcall SetDisplay(int *ptr);
  void __fastcall SetChar(Byte *ptr);
  void __fastcall SetSizeX(int Asize);
  void __fastcall SetSizeY(int Asize);
  void __fastcall SetIntensity(int AIntensity);
  void __fastcall SetColour(TColor AColour);
};
//---------------------------------------------------------------------------
#endif
