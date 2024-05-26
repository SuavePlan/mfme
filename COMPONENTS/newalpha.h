//---------------------------------------------------------------------------
#ifndef BFMAlphaH
#define BFMAlphaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class PACKAGE TBFMAlpha : public TImage32
{
private:
  TBitmap32 *Src;
  TBitmap32 *Trans;
  TBitmap32 *OffMask;
  TBitmap32 *Charmap;
  Graphics::TBitmap *Fbmp;
  int FColumns;
  char buffer[40];
  char oldbuffer[40];
  int buff[16];
  int oldbuff[16];
  int FIntensity;
  int FOff;
  TColor FColour;
  TColor FOffColour;
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
  void __fastcall DrawCharMap(void);
  void __fastcall SetTrans(TBitmap32 *ptr, TColor32 Colour);
protected:
  virtual void __fastcall Paint(void);
  void __fastcall Loaded(void);
public:
    __fastcall TBFMAlpha(TComponent* Owner);
    __fastcall ~TBFMAlpha();
  void __fastcall SetBitmap(Graphics::TBitmap *ptr);
  void __fastcall SetColumns(int AColumns);
  void __fastcall SetDigitWidth(int AWidth);
  void __fastcall SetDisplay2(int *ptr);
  void __fastcall SetColour(TColor AColour);
  void __fastcall SetOffColour(TColor AColour);
  void __fastcall SetIntensity(int AIntensity);
  void __fastcall SetReversed(bool AReversed);
  void __fastcall DrawCustom(TBitmap32 * ptr, int pos, int value);
  Graphics::TBitmap * __fastcall TBFMAlpha::GetBitmap(void);
__published:
  __property Graphics::TBitmap *Bitmap = { read = GetBitmap, write = SetBitmap, nodefault};
  __property int Columns = { read = FColumns, write = SetColumns};
  __property int DigitWidth = { read = FDigitWidth, write = SetDigitWidth, default = 17};
  __property TColor Colour = { read = FColour, write = SetColour};
  __property TColor OffColour = { read = FOffColour, write = SetOffColour};
  __property int Intensity = { read = FIntensity, write = SetIntensity};
  __property bool Reversed = { read = FReversed, write = SetReversed, default = false};
  __property OnMouseDown;
  __property OnMouseMove;
  __property OnMouseUp;
};
//---------------------------------------------------------------------------
#endif
