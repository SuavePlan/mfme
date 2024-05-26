//---------------------------------------------------------------------------
#ifndef ReelH
#define ReelH
//---------------------------------------------------------------------------
//#include <SysUtils.hpp>
//#include <Controls.hpp>
//#include <Classes.hpp>
//#include <Forms.hpp>
#include "GR32_Image.hpp"
#include "GR32_Transforms.hpp"
#include "GR32_Add_Blendmodes.hpp"
//---------------------------------------------------------------------------
class TReel : public TGraphicControl
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
  bool FOverlay;
  bool FReversed;
  TColor FBorderColor;
  bool init;
  TRect rects[3];
  int FBWidth;
  int FReel;
  int FPosition2;
  Graphics::TBitmap *FMask;
  Graphics::TBitmap *FBackground;
  Graphics::TBitmap *dest;
  TRect destRect, srcRect;
  TRect area;
  int OHeight, OWidth;
  Graphics::TBitmap *FExtReels[24];
  int FTotalSteps;
  int FLampNumber[3];
  bool FCustomLamps;
  TBitmap32 *band;
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
  void __fastcall SetReelLamps(bool ALamps);
  void __fastcall SetSmooth(bool ASmooth);
  void __fastcall SetOverlay(bool AOverlay);
  void __fastcall SetHeight(int AHeight);
  void __fastcall SetTotalSteps(int ATotalSteps);
  void __fastcall SetHorizontal(bool AHorizontal);
  void __fastcall SetReversed(bool AHorizontal);
  void __fastcall SetCustomLamps(bool ACustom);
  void __fastcall SetOffset(int AOffset);
  void __fastcall Darken(Graphics::TBitmap *ptr);
  void __fastcall Brighten(void);
  void __fastcall DrawWinLines(void);
  void __fastcall Loaded(void);
public:
  __fastcall TReel(TComponent* Owner);
  __fastcall ~TReel(void);
  void __fastcall SetBitmap(int index, Graphics::TBitmap *ptr);
  void __fastcall SetMask(Graphics::TBitmap *ptr);
  Graphics::TBitmap * __fastcall GetMask(void);
  void __fastcall SetBackground(Graphics::TBitmap *ptr);
  Graphics::TBitmap * __fastcall GetBackground(void);
  Graphics::TBitmap * __fastcall GetBitmap(int index);
  int __fastcall GetLampNumber(int index);
  void __fastcall DrawReels(void);
  void __fastcall Draw(void);
  void __fastcall SetLamp(int index, bool state);
  void __fastcall SetLamps(int state);
  void __fastcall SetLampNumber(int index, int number);
  __property Graphics::TBitmap *reels[int index] = { read = GetBitmap, write = SetBitmap };
  __property int LampNumbers[int index] = { read = GetLampNumber, write = SetLampNumber };
__published:
  __property int BorderWidth = { read = FBWidth, write = SetBorderWidth, default = 1 };
  __property TColor BorderColor = { read = FBorderColor, write = SetBorderColor, default = clBlack };
  __property int Darkness = { read = FDarkness, write = SetDarkness, default = 50 };
  __property int Stops = { read = FStops, write = SetStops, default = 16 };
  __property bool ReelLamps = { read = FLamps, write = SetReelLamps, default = false };
  __property int Position = { read = FPosition2, write = SetPosition, default = 0 };
  __property int View = { read = FView, write = SetView, default = 3 };
  __property bool WinLine = { read = FWinLine, write = SetLine, default = true };
  __property int WinLineWidth = { read = FWinLineWidth, write = SetLineWidth, default = 1 };
  __property int WinLines = { read = FWinLines, write = SetWinLines, default = 1 };
  __property int Coverage = { read = FCoverage, write = SetCoverage, default = 100 };
  __property bool Toggle = { read = FToggle, write = FToggle, default = false };
  __property bool Inverted = { read = FInverted, write = FInverted, default = false };
  __property int Offset = { read = FOffset, write = SetOffset, default = 0 };
  __property int Flag = { read = FFlag, write = FFlag, default = 0 };
  __property int Number = { read = FReel, write = SetReel, default = 0};
  __property int RHeight = { read = FHeight, write = SetHeight, default = 0};
  __property bool Horizontal = { read = FHorizontal, write = SetHorizontal, default = false};
  __property bool Smooth = { read = FSmooth, write = SetSmooth, default = true};
  __property bool Reversed = { read = FReversed, write = SetReversed, default = false};
  __property bool Overlay = { read = FOverlay, write = SetOverlay, default = false};
  __property OnMouseMove;
  __property OnMouseUp;
  __property OnMouseDown;
  __property Graphics::TBitmap *Background = { read = GetBackground, write = SetBackground, nodefault};
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
  __property Graphics::TBitmap *reel25 = { read = GetBitmap, write = SetBitmap, index = 24 };
  __property Graphics::TBitmap *reel26 = { read = GetBitmap, write = SetBitmap, index = 25 };
  __property Graphics::TBitmap *reel27 = { read = GetBitmap, write = SetBitmap, index = 26 };
  __property Graphics::TBitmap *reel28 = { read = GetBitmap, write = SetBitmap, index = 27 };
  __property Graphics::TBitmap *reel29 = { read = GetBitmap, write = SetBitmap, index = 28 };
  __property Graphics::TBitmap *reel30 = { read = GetBitmap, write = SetBitmap, index = 29 };
  __property Graphics::TBitmap *reel31 = { read = GetBitmap, write = SetBitmap, index = 30 };
  __property Graphics::TBitmap *reel32 = { read = GetBitmap, write = SetBitmap, index = 31 };
  __property Graphics::TBitmap *reel33 = { read = GetBitmap, write = SetBitmap, index = 32 };
  __property Graphics::TBitmap *reel34 = { read = GetBitmap, write = SetBitmap, index = 33 };
  __property Graphics::TBitmap *reel35 = { read = GetBitmap, write = SetBitmap, index = 34 };
  __property Graphics::TBitmap *reel36 = { read = GetBitmap, write = SetBitmap, index = 35 };
  __property Graphics::TBitmap *reel37 = { read = GetBitmap, write = SetBitmap, index = 36 };
  __property Graphics::TBitmap *reel38 = { read = GetBitmap, write = SetBitmap, index = 37 };
  __property Graphics::TBitmap *reel39 = { read = GetBitmap, write = SetBitmap, index = 38 };
  __property Graphics::TBitmap *reel40 = { read = GetBitmap, write = SetBitmap, index = 39 };
  __property Graphics::TBitmap *reel41 = { read = GetBitmap, write = SetBitmap, index = 40 };
  __property Graphics::TBitmap *reel42 = { read = GetBitmap, write = SetBitmap, index = 41 };
  __property Graphics::TBitmap *reel43 = { read = GetBitmap, write = SetBitmap, index = 42 };
  __property Graphics::TBitmap *reel44 = { read = GetBitmap, write = SetBitmap, index = 43 };
  __property Graphics::TBitmap *reel45 = { read = GetBitmap, write = SetBitmap, index = 44 };
  __property Graphics::TBitmap *reel46 = { read = GetBitmap, write = SetBitmap, index = 45 };
  __property Graphics::TBitmap *reel47 = { read = GetBitmap, write = SetBitmap, index = 46 };
  __property Graphics::TBitmap *reel48 = { read = GetBitmap, write = SetBitmap, index = 47 };
  __property int TotalSteps = { read = FTotalSteps, write = SetTotalSteps, default = 96};
  __property int Lamp1 = { read = GetLampNumber, write = SetLampNumber, index = 0};
  __property int Lamp2 = { read = GetLampNumber, write = SetLampNumber, index = 1};
  __property int Lamp3 = { read = GetLampNumber, write = SetLampNumber, index = 2};
  __property bool CustomLamps = { read = FCustomLamps, write = SetCustomLamps, default = false};
};
//---------------------------------------------------------------------------
#endif

