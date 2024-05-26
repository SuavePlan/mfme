//---------------------------------------------------------------------------
#ifndef grAlphaH
#define grAlphaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TGRAlpha : public TGRBase
{
private:
  TBitmap32 *Src;
  TBitmap32 *Mask;
  TBitmap32 *OffMask;
  TBitmap32 *Trans;
//  TBitmap32 *tmp;
  Graphics::TBitmap *Fbmp;
  int FColumns;
  char buffer[40];
  char oldbuffer[40];
  int buff[16];
  int oldbuff[16];
  String FDisplay;
  int FIntensity;
  int FOff;
  TColor FColour;
  TColor Color;
  bool FEnabled;
  bool FReversed;
  int FPos;
  int FDigitWidth;
  int bmpwidth;
//  TBitmap32 * dest;
  void __fastcall SetAlpha(TBitmap32 *ptr, TColor32 Colour);
  void __fastcall SetMask(TBitmap32 *ptr, TColor32 Colour);
  void __fastcall SetColor(TBitmap32 *ptr, TColor32 Colour);
  void __fastcall GetChar(TBitmap32 * ptr, int value, int pos);
  void __fastcall Setup(void);
  void __fastcall GenerateOff(TBitmap32 * ptr);
  void __fastcall Render(void);
  void __fastcall Draw(void);
public:
    __fastcall TGRAlpha(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * dest);
    __fastcall ~TGRAlpha();
  void __fastcall SetBitmap(Graphics::TBitmap *ptr);
  void __fastcall SetColumns(int AColumns);
  void __fastcall SetDigitWidth(int AWidth);
  void __fastcall SetDisplay2(int *ptr);
  void __fastcall SetColour(TColor AColour);
  void __fastcall SetIntensity(int AIntensity);
  void __fastcall SetOffBrightness(int AOff);
  void __fastcall SetReversed(bool AReversed);
  void __fastcall DrawCustom(int pos, int value);
  Graphics::TBitmap * __fastcall GetBitmap(void);
  void __fastcall SaveToStream(TStream *ptr);
};
//---------------------------------------------------------------------------
#endif
