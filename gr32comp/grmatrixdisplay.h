//---------------------------------------------------------------------------
#ifndef grDotMatrixH
#define grDotMatrixH
//---------------------------------------------------------------------------
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRDotMatrix : public TGRBase
{
private:
  Byte map[9*21];
  Byte newmap[9*21];
  TBitmap32 *Src;
  int FSize;
  int Spacing;
  void __fastcall Render(void);
  void __fastcall Init(void);
//protected:
  void __fastcall Draw(void);
//  void __fastcall Loaded(void);
public:
    __fastcall TGRDotMatrix (int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * ADest);
    __fastcall ~TGRDotMatrix ();
  void __fastcall SetPixel(int nbr, int state);
  void __fastcall SetMap(Byte *ptr);
  void __fastcall SetSize(int Asize);
  Graphics::TBitmap * __fastcall GetBitmap(void);
};
//---------------------------------------------------------------------------
#endif
