//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#include "Reelunit.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

/*
static int size5[5][6]     = {{ 81, 95, 100, 95, 81, 0 },
                               { 77, 93, 100, 97, 84, 64 },
                               { 73, 90, 99, 98, 88, 68 },
                               { 68, 88, 98, 99, 90, 73 },
                               { 64, 84, 97, 100, 93, 77 }};

static int position5[5][6] = {{ 8,  28, 50, 72, 92, 107 },
                               { 5,  24, 46, 68, 88, 104 },
                               { 2,  20, 41, 63, 84, 102 },
                               { -2,  16, 37, 59, 80, 98 },
                               { -4,  12, 32, 54, 76, 95}};
*/
static int size5[5][6]     = {{ 50, 85, 100, 85, 50, 0 },
                               { 38, 79, 99, 92, 60, 13 },
                               { 34, 76, 98, 94, 64, 18 },
                               { 18, 64, 94, 98, 76, 34 },
                               { 13, 60, 92, 99, 79, 38 }};
static int position5[5][6] = {{ 8,  25, 50, 75, 93, 0 },
                               { 5,  20, 44, 69, 89, 99 },
                               { 4,  18, 42, 67, 88, 98 },
                               { 1,  12, 33, 58, 82, 95 },
                               { 1,  10, 30, 56, 80, 96}};
static int size4[4][6]     = {{ 50, 85, 100, 85, 50, 0 },
                               { 38, 79, 99, 92, 60, 13 },
                               { 26, 71, 96, 96, 71, 26 },
                               { 13, 60, 92, 99, 79, 38 }};
static int position4[4][6] = {{ 8,  25, 50, 75, 93, 0 },
                               { 5,  20, 44, 69, 89, 99 },
                               { 3,  16, 38, 63, 85, 98 },
                               { 1,  10, 30, 56, 80, 96}};
static int size3[3][6]     = {{ 50, 85, 100, 85, 50, 0 },
                               { 34, 76, 98, 94, 64, 18 },
                               { 18, 64, 94, 98, 76, 34 }};
static int position3[3][6] = {{ 8,  25, 50, 75, 93, 0 },
                               { 4,  18, 42, 67, 88, 98 },
                               { 1,  12, 33, 58, 82, 95}};
static int size2[2][6]     = {{ 50, 86, 100, 86, 50, 0 },
                               { 26, 71, 96, 96, 71, 26 }};
static int position2[2][6] = {{ 8,  25, 50, 75, 93, 0 },
                               { 2,  15, 37, 63, 86, 98 }};

static inline void ValidCtrCheck(TReel *)
{
    new TReel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TReel::TReel(TComponent* Owner)
    : TGraphicControl(Owner)
{
int i;

  all = new TImageList(this);
  FMask = new Graphics::TBitmap();
  FMask->Width = 190;
  FMask->Height = 190;
  FMask->Canvas->Brush->Color = clWhite;
  FMask->Canvas->FillRect(FMask->Canvas->ClipRect);
  dest = new Graphics::TBitmap();
  dest->PixelFormat = pf24bit;
  FBackground = new Graphics::TBitmap();
//  FBackground->PixelFormat = pf24bit;
  FBWidth = 1;
  FBorderColor = clBlack;
  FView = 5;
  FStops = 16;
  FTotalSteps = 96;
  step = FTotalSteps/FStops;
  FToggle = false;
  FWinLine = true;
  FHorizontal = false;
  FDarkness = 50;
  FLamps = false;
  FOverlay = false;
  FCoverage = 100;
  FWinLines = 1;
  FWinLineWidth = 1;
  FReel = 0;
  FHeight = 140;
  FSmooth = true;
  FReversed = false;
  FCustomLamps = false;
  for ( i = 0; i < 24; i++ ) {
    FReels[i] = new Graphics::TBitmap();
    FExtReels[i] = new Graphics::TBitmap();
  }
  for ( i = 0; i < 3; i++ ) {
    lit[i] = new TImageList(this);
    Lamps[i] = false;
    FLampNumber[i] = -2;
  }
  FPosition = FPosition2 = 0;
  ControlStyle << csOpaque;
}
//---------------------------------------------------------------------------
__fastcall TReel::~TReel(void)
{
int i;

  delete all;
  delete lit[0];
  delete lit[1];
  delete lit[2];
  delete FMask;
  delete dest;
  delete FBackground;
  for ( i = 0; i< 24; i++ ) {
    if ( FReels[i] )
      delete FReels[i];
    if ( FExtReels[i] )
      delete FExtReels[i];
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::Loaded(void)
{
  TGraphicControl::Loaded();
  DrawReels();
  FDrawn = true;
  OHeight = Height;
  OWidth = Width;
  if ( FOverlay ) {
    FBackground->Transparent = true;
    FBackground->TransparentColor = FBackground->Canvas->Pixels[FBackground->Width/2][FBackground->Height/2];
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::Paint(void)
{
int offset, offset2;

  if ( OHeight != Height || OWidth != Width ) {
    if ( FHeight < Height )
      FHeight = Height;
    DrawReels();
    OHeight = Height;
    OWidth = Width;
  }
  if ( FBWidth )
  {
    offset = FBWidth/2;
    offset2 = (FBWidth+1)/2;
    Canvas->Pen->Width = FBWidth;
    Canvas->Pen->Color = FBorderColor;
    Canvas->MoveTo( offset, offset);
    Canvas->LineTo(Width-offset2,offset);
    Canvas->LineTo(Width-offset2,Height-offset2);
    Canvas->LineTo(offset,Height-offset2);
    Canvas->LineTo(offset,offset);
  }
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TReel::Draw(void)
{
Graphics::TBitmap *ptr = new Graphics::TBitmap();
Graphics::TBitmap *ptr2;
int i;
int pos;

  pos = (FPosition + FOffset + FTotalSteps) % FTotalSteps;
  if ( FHorizontal )
    pos = ((FTotalSteps-pos)/2) % (FTotalSteps/2);
  else
    pos = pos/2;
  all->GetBitmap(pos, ptr);
  if ( FLamps ) {
    ptr2 = new Graphics::TBitmap();
    for ( i = 0; i < 3; i++ ) {
      if ( Lamps[i] ) {
        if ( FHorizontal ) {
          lit[2-i]->GetBitmap(pos, ptr2);
          ptr->Canvas->Draw(rects[2-i].Left, 0, ptr2);
        }
        else {
          lit[i]->GetBitmap(pos, ptr2);
          ptr->Canvas->Draw(0, rects[i].Top, ptr2);
        }
      }
    }
    delete ptr2;
  }
  if ( FOverlay )
    ptr->Canvas->Draw(0,0,FBackground);
//  Canvas->CopyRect(ClientRect, ptr->Canvas, ClientRect);
  Canvas->Draw(FBWidth,FBWidth,ptr);
//  BitBlt(Canvas->Handle, FBWidth, FBWidth, Width, Height, ptr->Handle, 0, 0, SRCCOPY);
  delete ptr;
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetBorderWidth(int ABorderWidth)
{
  if ( ABorderWidth != FBWidth )
  {
    FBWidth = ABorderWidth;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetMask(Graphics::TBitmap *ptr)
{
  FMask->Assign(ptr);
  if ( FDrawn ) {
    DrawReels();
    Refresh();
  }
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TReel::GetMask(void)
{
  return FMask;
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetBackground(Graphics::TBitmap *ptr)
{
  FBackground->Assign(ptr);
  FBackground->Transparent = true;
  FBackground->TransparentMode = tmFixed;
  FOverlay = true;
  FBackground->TransparentColor = FBackground->Canvas->Pixels[FBackground->Width/2][FBackground->Height/2];
  if ( FDrawn )
    Draw();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TReel::GetBackground(void)
{
  return FBackground;
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetBorderColor(TColor ABorderColor)
{
  if ( ABorderColor != FBorderColor )
  {
    FBorderColor = ABorderColor;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetReel(int AReel)
{
  if ( AReel != FReel )
  {
    FReel = AReel;
    if ( FDrawn ) {
      DrawReels();
      Draw();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetStops(int AStops)
{
  if ( AStops != FStops )
  {
    if ( AStops != 12 && AStops != 16 && AStops != 20 && AStops != 24 && AStops != 25 )
      return;
    FStops = AStops;
    step = FTotalSteps/FStops;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetTotalSteps(int ATotalSteps)
{
  if ( ATotalSteps != FTotalSteps )
  {
    if ( ATotalSteps != 96 && ATotalSteps != 144 &&
         ATotalSteps != 100 && ATotalSteps != 200 &&
         ATotalSteps != 400 )
      return;
    FTotalSteps = ATotalSteps;
    step = FTotalSteps/FStops;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetOffset(int AOffset)
{
  if ( AOffset != FOffset )
  {
    FOffset = AOffset;
    if ( FDrawn )
      Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetPosition(int APosition)
{
  FPosition2 = APosition;
  if ( !FSmooth )
    APosition = (APosition / step) * step;
  if ( FReversed )
      APosition = (FTotalSteps - APosition) % FTotalSteps;
  if ( APosition != FPosition && APosition < FTotalSteps )
  {
    FPosition = APosition;
//    Invalidate();
    if ( FDrawn )
      Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetView(int AView)
{
//  if ( AView != FView )
//  {
//    FView = AView;
//    if ( FDrawn ) {
//      DrawReels();
//      Draw();
//    }
//  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetLine(bool ALine)
{
  if ( ALine != FWinLine )
  {
    FWinLine = ALine;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetLineWidth(int ALine)
{
  if ( ALine != FWinLineWidth )
  {
    FWinLineWidth = ALine;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetWinLines(int ALines)
{
  if ( ALines != FWinLines )
  {
    FWinLines = ALines;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetCoverage(int ACoverage)
{
  if ( ACoverage != FCoverage )
  {
    FCoverage = ACoverage;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetDarkness(int ADarkness)
{
  if ( ADarkness != FDarkness )
  {
    FDarkness = ADarkness;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetReelLamps(bool ALamps)
{
  if ( ALamps != FLamps )
  {
    FLamps = ALamps;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetSmooth(bool ASmooth)
{
  if ( ASmooth != FSmooth )
  {
    FSmooth = ASmooth;
    if ( FDrawn ) {
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetReversed(bool AReversed)
{
  if ( AReversed != FReversed )
  {
    FReversed = AReversed;
    if ( FDrawn )
      Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetOverlay(bool AOverlay)
{
  if ( AOverlay != FOverlay )
  {
    FOverlay = AOverlay;
//    if ( FOverlay ) {
//      FBackground->Transparent = true;
//      FBackground->TransparentMode = tmFixed;
//      FBackground->TransparentColor = FBackground->Canvas->Pixels[FBackground->Width/2][FBackground->Height/2];
//    }
  } 
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetHeight(int AHeight)
{
  if ( AHeight != FHeight && AHeight >= (Height - 2 * FBWidth))
  {
    FHeight = AHeight;
    if ( FDrawn ) {
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetHorizontal(bool AHorizontal)
{
  if ( AHorizontal != FHorizontal )
  {
    FHorizontal = AHorizontal;
    if ( FDrawn ) {
      int temp = Height;
      Height = Width;
      Width = temp;
      DrawReels();
      Refresh();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetCustomLamps(bool ACustomLamps)
{
  if ( FCustomLamps != ACustomLamps )
    FCustomLamps = ACustomLamps;
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetBitmap(int index, Graphics::TBitmap *ptr)
{
  if ( index < 24 )
    FReels[index]->Assign(ptr);
  else
    FExtReels[index-24]->Assign(ptr);
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TReel::GetBitmap(int index)
{
  if ( index < 24 )
    return FReels[index];
  else
    return FExtReels[index-24];
}
//---------------------------------------------------------------------------
int __fastcall TReel::GetLampNumber(int index)
{
  return FLampNumber[index];
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetLampNumber(int index, int number)
{
  FLampNumber[index] = number;
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetLamp(int index, bool state)
{
  Lamps[index] = state;
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TReel::SetLamps(int state)
{
bool changed = false;

  if ( FReversed )
    state = (state & 2) + ((state & 1) << 2) + ((state & 4) >> 2);
  for ( int i = 0; i < 3; i++ ) {
    if ( state & (1 << i) ) {
      if ( !Lamps[i] )
        changed = true;
      Lamps[i] = true;
    } else {
      if ( Lamps[i] )
        changed = true;
      Lamps[i] = false;
    }
  }
  if ( changed )
    Draw();
}
//---------------------------------------------------------------------------
void __fastcall TReel::DrawReels(void)
{
int i, centre, height, width;
TRect rect;
int symbol;
int offset;
int *sz;
int *ps;
int pos;
Graphics::TBitmap *window, *sym;

  window = new Graphics::TBitmap();
  sym = new Graphics::TBitmap();
  sym->PixelFormat = pf24bit;
  if ( FReels[0]->Width > 0 )
    window->Canvas->Brush->Color = FReels[0]->Canvas->Pixels[0][0];
  else
    window->Canvas->Brush->Color = clWhite;
  FDarkness = FMask->Canvas->Pixels[0][0] & 0xff;

  if ( FHorizontal ) {
    window->Width = FHeight;
    window->Height = Height - 2 * FBWidth;
  } else {
    window->Width = Width - 2 * FBWidth;
    window->Height = FHeight;// - 2 * FBWidth;
  }
  area = Rect(0,0,window->Width,window->Height);
  dest->Width = Width - 2 * FBWidth;
  dest->Height = Height - 2 * FBWidth;
  dest->Canvas->Pen->Width = FWinLineWidth;
  dest->Canvas->Pen->Color = FBorderColor;
  destRect = Rect(0,0,dest->Width,dest->Height);
  all->Width = Width - 2 * FBWidth;
  all->Height = Height - 2 * FBWidth;

  if ( FHorizontal ) {
    srcRect = Rect((window->Width/2) - (dest->Width/2), 0,
                 (window->Width/2) + (dest->Width/2),window->Height);
  } else {
    srcRect = Rect(0, (window->Height/2) - (dest->Height/2),
                 window->Width,(window->Height/2) + (dest->Height/2));
  }
  for ( int i = 0; i < 3; i++ ) {
    lit[i]->Clear();
    if ( FHorizontal ) {
      width = ( (size3[0][i+1]+14) * (window->Width / 4 ) ) / 100;
      centre = (window->Width * position3[0][i+1]) / 100;
      centre = centre - (( window->Width - Width )/2) - FBWidth;
      rects[i] = Rect(centre - (width/2), 0, centre + (width/2), Height - 2 * FBWidth );
      lit[i]->Height = Height - 2 * FBWidth;
      lit[i]->Width = rects[i].Right - rects[i].Left;
    } else {
      height = ( (size3[0][i+1]+14) * (window->Height / 4 ) ) / 100;
      centre = (window->Height * position3[0][i+1]) / 100;
      centre = centre - (( window->Height - Height )/2) - FBWidth;
      rects[i] = Rect( 0, centre - (height/2), Width - 2 * FBWidth, centre + (height/2));
      lit[i]->Height = rects[i].Bottom - rects[i].Top;;
      lit[i]->Width = Width - 2 * FBWidth;
    }
  }
  all->Clear();

  for ( pos = 0; pos < FTotalSteps; pos+=2 ) {
    symbol = (((pos / (FTotalSteps / FStops)) - 2 ) + FStops ) % FStops;
    offset = (pos % (FTotalSteps / FStops)) / 2;
    window->Canvas->FillRect(area);
    switch ( FStops ) {
      case 12:
        sz = &size4[offset][0];
        ps = &position4[offset][0];
        break;
      case 16:
        sz = &size3[offset][0];
        ps = &position3[offset][0];
        break;
      case 20:
        sz = &size5[offset][0];
        ps = &position5[offset][0];
        break;
      case 24:
        sz = &size2[offset][0];
        ps = &position2[offset][0];
        break;
      case 25:
        sz = &size4[offset][0];
        ps = &position4[offset][0];
        break;
    }

    for ( i = 0; i < 6; i++ ) {
      if ( FHorizontal ) {
        width = ( *sz++ * (FHeight / 4 ) ) / 90;
        if ( width ) {
          centre = (FHeight * *ps++) / 100;
          rect = Rect( centre - (width/2), 0, centre + (width/2), Height - 2 * FBWidth);
          if ( symbol < 24 )
          window->Canvas->StretchDraw( rect, FReels[symbol]);
          else
            window->Canvas->StretchDraw( rect, FExtReels[symbol-24]);
        }
      } else {
        height = ( *sz++ * (FHeight / 4 ) ) / 90;
        if ( height ) {
          centre = (FHeight * *ps++) / 100;
          rect = Rect( 0, centre - (height/2), Width - 2 * FBWidth, centre + (height/2));
          if ( symbol < 24 )
          window->Canvas->StretchDraw( rect, FReels[symbol]);
          else
            window->Canvas->StretchDraw( rect, FExtReels[symbol-24]);
        }
      }
      symbol = (symbol + 1) % FStops;
    }
    dest->Canvas->CopyRect(destRect,window->Canvas,srcRect);
    if ( FLamps )
      Darken(dest);
    if ( FWinLine )
      DrawWinLines();
    if ( FLamps )
      Brighten();
    all->Add(dest,NULL);
  }
  delete sym;
  delete window;
  FDrawn = true;
}
//---------------------------------------------------------------------------
void __fastcall TReel::DrawWinLines(void)
{
int i,j;

  if ( FHorizontal ) {
    if ( FWinLines == 1 ) {
      j = 1;
      i = (rects[j].Right + rects[j].Left) / 2;
      dest->Canvas->MoveTo(i, 0);
      dest->Canvas->LineTo(i, dest->Height);
    }
  } else {
    for ( i = 0; i < 3; i++ ) {
      if ( i == 1 || ( i == 0 && FWinLines > 2) || ( i == 2 && FWinLines > 1) ) {
        dest->Canvas->MoveTo(0, (rects[i].Bottom + rects[i].Top) / 2);
        dest->Canvas->LineTo(dest->Width,(rects[i].Bottom + rects[i].Top) / 2);
      }
      if ( FWinLines > 3 ) {
        if ( (i == 0 && FReel == 0) || ( i == 2 && FReel == 2) || ( i == 1 && FReel == 1) ||
             (i == 0 && FReel == 3) || ( i == 2 && FReel == 5) || ( i == 1 && FReel == 4) ) {
          dest->Canvas->MoveTo(0, rects[i].Top+2);    //  Top to Bottom
          dest->Canvas->LineTo(dest->Width,rects[i].Bottom-2);
        }
        if ( FWinLines > 5 ) {
          if ( (i == 0 && FReel == 4) || ( i == 1 && FReel == 5) || ( i == 2 && FReel == 6) ) {
            dest->Canvas->MoveTo(0, rects[i].Top+2);    //  Top to Bottom
            dest->Canvas->LineTo(dest->Width,rects[i].Bottom-2);
          }
        }
        if ( (i == 2 && FReel == 0) || ( i == 0 && FReel == 2) || ( i == 1 && FReel == 1) ||
             (i == 2 && FReel == 3) || ( i == 0 && FReel == 5) || ( i == 1 && FReel == 4) ) {
          dest->Canvas->MoveTo(0, rects[i].Bottom-2); // Bottom to Top
          dest->Canvas->LineTo(dest->Width,rects[i].Top+2);
        }
        if ( FWinLines > 5 ) {
          if ( (i == 2 && FReel == 4) || ( i == 1 && FReel == 5) || ( i == 0 && FReel == 6) ) {
            dest->Canvas->MoveTo(0, rects[i].Bottom-2); // Bottom to Top
            dest->Canvas->LineTo(dest->Width,rects[i].Top+2);
          }
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TReel::Darken(Graphics::TBitmap *ptr)
{
int i,j;
RGBTRIPLE *old;
Graphics::TBitmap *newmask = new Graphics::TBitmap();

  newmask->PixelFormat = pf24bit;
  newmask->Assign(FMask);

  for ( j = 0; j < ptr->Height; j++ ) {
    old = (RGBTRIPLE *)ptr->ScanLine[j];

    for ( i = 0; i < ptr->Width; i++ ) {
      old->rgbtRed   =  (old->rgbtRed * FDarkness  ) / 255;
      old->rgbtGreen =  (old->rgbtGreen * FDarkness ) / 255;
      old->rgbtBlue  =  (old->rgbtBlue * FDarkness ) / 255;
      old++;
    }
  }
  delete newmask;
}
//---------------------------------------------------------------------------
void __fastcall TReel::Brighten(void)
{
int i,j,k;
Graphics::TBitmap *sym, *newmask;
Graphics::TBitmap *newwindow = new Graphics::TBitmap();
RGBTRIPLE *MaskBase, *old;

  newwindow->Assign(dest);
  sym = new Graphics::TBitmap();
  newmask = new Graphics::TBitmap();
  if ( FHorizontal ) {
    newmask->Height = Height - 2 * FBWidth;
    sym->Height = Height - 2 * FBWidth;
  } else {
    newmask->Width = Width - 2 * FBWidth;
    sym->Width = Width - 2 * FBWidth;
  }
  sym->PixelFormat = pf24bit;
  newmask->PixelFormat = pf24bit;

  for ( k = 0; k < 3; k++ ) {
    if ( FHorizontal ) {
      sym->Width = rects[k].Right - rects[k].Left;
      newmask->Width = sym->Width;
    } else {
      sym->Height = rects[k].Bottom - rects[k].Top;
      newmask->Height = sym->Height;
    }
    sym->Canvas->CopyRect(sym->Canvas->ClipRect, dest->Canvas, rects[k]);
    newmask->Canvas->StretchDraw(newmask->Canvas->ClipRect, FMask);
    for ( j = 0; j < sym->Height; j++ ) {
      MaskBase = (RGBTRIPLE *)newmask->ScanLine[j];
      old      = (RGBTRIPLE *)sym->ScanLine[j];
      for ( i = 0; i < sym->Width; i++ ) {
        old->rgbtRed   =  (((old->rgbtRed * 255  ) / FDarkness) * MaskBase->rgbtRed) / 255;
        old->rgbtGreen =  (((old->rgbtGreen * 255 ) / FDarkness)* MaskBase->rgbtGreen) / 255;
        old->rgbtBlue  =  (((old->rgbtBlue * 255 ) / FDarkness)* MaskBase->rgbtBlue) / 255;
        old++;
        MaskBase++;
      }
    }
    lit[k]->Add(sym,NULL);
  }
  delete sym;
  delete newmask;
  delete newwindow;
}
 
