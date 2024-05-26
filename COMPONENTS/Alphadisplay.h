//---------------------------------------------------------------------------
#ifndef AlphaH
#define AlphaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TAlpha : public TImage32
{
private:
  TBitmap32 *Src;
  TBitmap32 *Mask;
  TBitmap32 *OffMask;
  TBitmap32 *Trans;
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
  void __fastcall Draw(void);
  bool FEnabled;
  bool FReversed;
  int FPos;
  int FDigitWidth;
  int bmpwidth;
  void __fastcall SetAlpha(TBitmap32 *ptr, TColor32 Colour);
  void __fastcall SetMask(TBitmap32 *ptr, TColor32 Colour);
  void __fastcall SetColor(TBitmap32 *ptr, TColor32 Colour);
  void __fastcall GetChar(TBitmap32 * ptr, int value, int pos);
  void __fastcall Setup(void);
  void __fastcall GenerateOff(TBitmap32 * ptr);
protected:
  virtual void __fastcall Paint(void);
  void __fastcall Loaded(void);
public:
    __fastcall TAlpha(TComponent* Owner);
    __fastcall ~TAlpha();
  void __fastcall SetBitmap(Graphics::TBitmap *ptr);
  void __fastcall SetColumns(int AColumns);
  void __fastcall SetDigitWidth(int AWidth);
  void __fastcall SetDisplay(String ADisplay);
  void __fastcall SetDisplay2(int *ptr);
  void __fastcall SetColour(TColor AColour);
  void __fastcall SetIntensity(int AIntensity);
  void __fastcall SetOffBrightness(int AOff);
  void __fastcall SetReversed(bool AReversed);
  void __fastcall DrawCustom(int pos, int value);
  Graphics::TBitmap * __fastcall TAlpha::GetBitmap(void);
__published:
  __property Graphics::TBitmap *Bitmap = { read = GetBitmap, write = SetBitmap, nodefault};
  __property int Columns = { read = FColumns, write = SetColumns};
  __property int DigitWidth = { read = FDigitWidth, write = SetDigitWidth, default = 17};
  __property TColor Colour = { read = FColour, write = SetColour};
  __property String Display = { read = FDisplay, write = SetDisplay};
  __property int Intensity = { read = FIntensity, write = SetIntensity};
  __property int OffBrightness = { read = FOff, write = SetOffBrightness};
  __property bool Reversed = { read = FReversed, write = SetReversed, default = false};
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
