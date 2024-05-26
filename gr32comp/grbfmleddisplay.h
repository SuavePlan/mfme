//---------------------------------------------------------------------------
#ifndef grBFMLEDH
#define grBFMLEDH
//---------------------------------------------------------------------------
#include "GR32_Image.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TGRBFMLED : public TGRBase
{
private:
  TBitmap32 *Src;
  TColor FColour;
  int FSizeX;
  int FSizeY;
  int FIntensity;
  int FDigitSpacing;
  int map[20];
  void __fastcall Render(void);
  void __fastcall DrawColumn(int i);
  void __fastcall Init(void);
//protected:
  void __fastcall Draw(void);
//  void __fastcall Loaded(void);
public:
    __fastcall TGRBFMLED (int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * ADest);
    __fastcall ~TGRBFMLED ();
  void __fastcall SetDot(int nbr, int value);
  void __fastcall SetSizeX(int Asize);
  void __fastcall SetSizeY(int Asize);
  void __fastcall SetSpacing(int ASpacing);
  void __fastcall SetColour(TColor AColour);
};
//---------------------------------------------------------------------------
#endif
