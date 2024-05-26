//---------------------------------------------------------------------------
#ifndef ReelH
#define ReelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class PACKAGE TReel : public TGraphicControl
{
private:
  TImageList *all;
  TImageList *lit[3];
  Graphics::TBitmap *FReels[24];
  int FStops;
  int FView;
  int FPosition;
  bool FWinLine;
  bool FInverted;
  int FWinLineWidth;
  int FWinLines;
  int FCoverage;
  int FDarkness;
  int FHeight;
  bool FSmooth;
  bool Lamps[3];
  bool FToggle;
  bool FLamps;
  int step;
  int FOffset;
  int FFlag;
  bool FDrawn;
  bool FHorizontal;
  TColor FBorderColor;
  bool init;
  TRect rects[3];
  int FBWidth;
  int FReel;
  Graphics::TBitmap *FMask;
  Graphics::TBitmap *FBackground;
  Graphics::TBitmap *dest;
  TRect destRect, srcRect;
  TRect area;
  int OHeight, OWidth;
protected:
  virtual void __fastcall Paint(void);
  void __fastcall SetBorderWidth(int ABorderWidth);
  void __fastcall SetBorderColor(TColor ABorderColor);
  void __fastcall SetStops(int Astops);
  void __fastcall SetPosition(int APosition);
  void __fastcall SetView(int AView);
  void __fastcall SetLine(bool ALine);
  void __fastcall SetWinLines(int ALines);
  void __fastcall SetLineWidth(int ALine);
  void __fastcall SetCoverage(int ACoverage);
  void __fastcall SetDarkness(int ADarkness);
  void __fastcall SetReel(int AReel);
  void __fastcall SetLamps(bool ALamps);
  void __fastcall SetSmooth(bool ASmooth);
  void __fastcall SetHeight(int AHeight);
  void __fastcall SetHorizontal(bool AHorizontal);
  void __fastcall Darken(Graphics::TBitmap *ptr);
  void __fastcall Brighten(void);
  void __fastcall DrawWinLines(void);
public:
  __fastcall TReel(TComponent* Owner);
  __fastcall ~TReel(void);
  void __fastcall SetBitmap(int index, Graphics::TBitmap *ptr);
  void __fastcall SetMask(Graphics::TBitmap *ptr);
  Graphics::TBitmap * __fastcall GetMask(void);
  void __fastcall SetBackground(Graphics::TBitmap *ptr);
  Graphics::TBitmap * __fastcall GetBackground(void);
  Graphics::TBitmap * __fastcall GetBitmap(int index);
  void __fastcall DrawReels(void);
  void __fastcall Draw(void);
  void __fastcall SetLamp(int index, bool state);
  __property Graphics::TBitmap *reels[int index] = { read = GetBitmap, write = SetBitmap };
__published:
  __property int BorderWidth = { read = FBWidth, write = SetBorderWidth, default = 1 };
  __property TColor BorderColor = { read = FBorderColor, write = SetBorderColor, default = clBlack };
  __property int Darkness = { read = FDarkness, write = SetDarkness, default = 50 };
  __property int Stops = { read = FStops, write = SetStops, default = 16 };
  __property bool ReelLamps = { read = FLamps, write = SetLamps, default = false };
  __property int Position = { read = FPosition, write = SetPosition, default = 0 };
  __property int View = { read = FView, write = SetView, default = 3 };
  __property bool WinLine = { read = FWinLine, write = SetLine, default = true };
  __property int WinLineWidth = { read = FWinLineWidth, write = SetLineWidth, default = 1 };
  __property int WinLines = { read = FWinLines, write = SetWinLines, default = 1 };
  __property int Coverage = { read = FCoverage, write = SetCoverage, default = 100 };
  __property bool Toggle = { read = FToggle, write = FToggle, default = false };
  __property bool Inverted = { read = FInverted, write = FInverted, default = false };
  __property int Offset = { read = FOffset, write = FOffset, default = 0 };
  __property int Flag = { read = FFlag, write = FFlag, default = 0 };
  __property int Number = { read = FReel, write = SetReel, default = 0};
  __property int RHeight = { read = FHeight, write = SetHeight, default = 0};
  __property bool Horizontal = { read = FHorizontal, write = SetHorizontal, default = false};
  __property bool Smooth = { read = FSmooth, write = SetSmooth, default = true};
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnMouseDown;
  __property Graphics::TBitmap *Background = { read = GetBackground, write = SetBackground};
  __property Graphics::TBitmap *Mask = { read = GetMask, write = SetMask};
  __property Graphics::TBitmap *reel1 = { read = GetBitmap, write = SetBitmap, index = 0 };
  __property Graphics::TBitmap *reel2 = { read = GetBitmap, write = SetBitmap, index = 1 };
  __property Graphics::TBitmap *reel3 = { read = GetBitmap, write = SetBitmap, index = 2 };
  __property Graphics::TBitmap *reel4 = { read = GetBitmap, write = SetBitmap, index = 3 };
  __property Graphics::TBitmap *reel5 = { read = GetBitmap, write = SetBitmap, index = 4 };
  __property Graphics::TBitmap *reel6 = { read = GetBitmap, write = SetBitmap, index = 5 };
  __property Graphics::TBitmap *reel7 = { read = GetBitmap, write = SetBitmap, index = 6 };
  __property Graphics::TBitmap *reel8 = { read = GetBitmap, write = SetBitmap, index = 7 };
  __property Graphics::TBitmap *reel9 = { read = GetBitmap, write = SetBitmap, index = 8 };
  __property Graphics::TBitmap *reel10 = { read = GetBitmap, write = SetBitmap, index = 9 };
  __property Graphics::TBitmap *reel11 = { read = GetBitmap, write = SetBitmap, index = 10 };
  __property Graphics::TBitmap *reel12 = { read = GetBitmap, write = SetBitmap, index = 11 };
  __property Graphics::TBitmap *reel13 = { read = GetBitmap, write = SetBitmap, index = 12 };
  __property Graphics::TBitmap *reel14 = { read = GetBitmap, write = SetBitmap, index = 13 };
  __property Graphics::TBitmap *reel15 = { read = GetBitmap, write = SetBitmap, index = 14 };
  __property Graphics::TBitmap *reel16 = { read = GetBitmap, write = SetBitmap, index = 15 };
  __property Graphics::TBitmap *reel17 = { read = GetBitmap, write = SetBitmap, index = 16 };
  __property Graphics::TBitmap *reel18 = { read = GetBitmap, write = SetBitmap, index = 17 };
  __property Graphics::TBitmap *reel19 = { read = GetBitmap, write = SetBitmap, index = 18 };
  __property Graphics::TBitmap *reel20 = { read = GetBitmap, write = SetBitmap, index = 19 };
  __property Graphics::TBitmap *reel21 = { read = GetBitmap, write = SetBitmap, index = 20 };
  __property Graphics::TBitmap *reel22 = { read = GetBitmap, write = SetBitmap, index = 21 };
  __property Graphics::TBitmap *reel23 = { read = GetBitmap, write = SetBitmap, index = 22 };
  __property Graphics::TBitmap *reel24 = { read = GetBitmap, write = SetBitmap, index = 23 };
};
//---------------------------------------------------------------------------
#endif

