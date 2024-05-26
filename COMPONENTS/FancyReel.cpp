//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "FancyReel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TFancyReel *)
{
    new TFancyReel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFancyReel::TFancyReel(TComponent* Owner)
    : TGraphicControl(Owner)
{
  mask = new TBitmap32();
  mask->DrawMode=dmCustom;
  mask->OnPixelCombine=Darken;
//  mask->MasterAlpha = 100;
  mask->Clear(clBlack32);
  FDarkness = 100;
  bg = new TBitmap32();
  bg->StretchFilter = sfLinear;
  FLampMask = new TBitmap32();
//  FLampMask->DrawMode=dmCustom;
//  FLampMask->OnPixelCombine=Darken;
//  FLampMask->MasterAlpha = 255;
  FBand = new TBitmap32();
  FBand->DrawMode=dmCustom;
  FBand->OnPixelCombine=Darken;
  FOverlay = new TBitmap32();
  FOverlay->DrawMode = dmBlend;
  FReelHeight = 180;
  Height = 140;
  Width = 80;
  FBorderWidth = 1;
  FLampMask2 = new Graphics::TBitmap();
  FLampMask2->Height = 40;
  FLampMask2->Width = 40;
  FBand2 = new Graphics::TBitmap();
  FOverlay2 = new Graphics::TBitmap();
  FOffset = 0;
  FStops = 16;
  FSteps = 96;
  FStepsPerStop = 1;
  FBand2->Width = 80;
  FBand2->Height = FStops * 56;
  FReversed = false;
  FBorderColour = clBlack;
  FWinLines = 1;
  FWinLineWidth = 1;
  for ( int i = 0; i < 5; i++ )
    lamps[i] = -2;
  TGraphicControl::OnResize = Resized;
  tmp = new TBitmap32();
  tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
  tmp->Clear(clWhite32);
  lines = new TBitmap32();
  lines->DrawMode = dmBlend;
  FToggle = false;
  FExpand = false;
  FFlag = 0;
  ControlStyle << csOpaque;
}
//---------------------------------------------------------------------------
__fastcall TFancyReel::~TFancyReel(void)
{
  delete mask;
  delete bg;
  delete FBand;
  delete FOverlay;
  delete FLampMask;
  delete FBand2;
  delete FOverlay2;
  delete FLampMask2;
  delete tmp;
  delete lines;
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::Loaded(void)
{
    TGraphicControl::Loaded();
    FBand->Assign(FBand2);
    FOverlay->Assign(FOverlay2);
    if ( !FOverlay->Empty() )
      SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
    else
      FOverlay->SetSize(Width,Height);
    FOverlay->PenColor = clBlack32;
    BandHeight = FBand->Height / M_PI;
    bg->SetSize(FBand->Width,BandHeight);
    tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
    mask->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
    FLampMask->Assign(FLampMask2);
    if ( !FLampMask->Empty() )
      Dark = *FLampMask->PixelPtr[0][0];
    TGraphicControl::OnResize = Resized;
    InitSize();
    DrawWinLines();
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::Paint(void)
{
int offset, offset2;
  if ( FBorderWidth )
  {
    offset = FBorderWidth/2;
    offset2 = (FBorderWidth+1)/2;
    Canvas->Pen->Width = FBorderWidth;
    Canvas->Pen->Color = FBorderColour;
    Canvas->MoveTo( offset, offset);
    Canvas->LineTo(Width-offset2,offset);
    Canvas->LineTo(Width-offset2,Height-offset2);
    Canvas->LineTo(offset,Height-offset2);
    Canvas->LineTo(offset,offset);
  }
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::Draw(void)
{
int j,k;
TColor32Array *big2, *reel2;
float *array;
int copy = bg->Width * sizeof(TColor32Array);

  if ( FBand->Height ) {
    j = windowrect.Top;
    array = &line_array[j];
//    reel2 = (TColor32Array *)bg->ScanLine[j];
    for ( ; j < windowrect.Bottom; j++ ) {
      k = *array+pos;
      while ( k >= FBand->Height )
        k -= FBand->Height;
//      k = int(*array+pos) % FBand->Height;
      big2 = (TColor32Array *)FBand->ScanLine[k];
      reel2 = (TColor32Array *)bg->ScanLine[j];
      memcpy( reel2, big2, copy );
      array++;
//      reel2+= sizeof(TColor32Array) * bg->Width;
    }
//    bg->SaveToFile("reel.bmp");
    tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),windowrect,bg);
    if ( FLamps )
      tmp->Draw(0,0,mask);
  }
  if ( WinLines )
    tmp->Draw(0,0,lines);
  if ( FOverlay->Height )
    tmp->Draw(0,0,FOverlay);
//  tmp->SaveToFile("reel2.bmp");
  BitBlt(Canvas->Handle, FBorderWidth, FBorderWidth, Width-2*FBorderWidth, Height-2*FBorderWidth, tmp->Handle, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetPosition(int Value)
{
  if ( Value != FPosition ) {
    FPosition = (Value + FSteps) % FSteps;
    if ( FPosition < 0 || FPosition > FSteps )
      FPosition = 0;
    Setpos();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::Setpos(void)
{
int newpos = FPosition;

  if ( FReversed )
    newpos = (FSteps - newpos) % FSteps;
  newpos += FOffset;
  pos = (symbol / 2) + ((newpos * FBand->Height) / FSteps) - (symbol*FStops/4) + FBand->Height;
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetLamp(int nbr, bool state)
{
  if ( state )
    FState |= (1 << nbr);
  else
    FState &= ~(1 << nbr);
  ConstructLampMask();
  tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),windowrect,bg);
  if ( FLamps )
    tmp->Draw(0,0,mask);
  if ( WinLines )
    tmp->Draw(0,0,lines);
  if ( FOverlay->Height )
    tmp->Draw(0,0,FOverlay);
  BitBlt(Canvas->Handle, FBorderWidth, FBorderWidth, Width, Height, tmp->Handle, 0, 0, SRCCOPY);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetLamps(int state)
{
  if ( FState != state ) {
    if ( FReversed )
      FState = (state & 4) + ((state & 2) << 2) + ((state & 8) >> 2);
    else
      FState = state;
    ConstructLampMask();
    tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),windowrect,bg);
    if ( FLamps )
      tmp->Draw(0,0,mask);
    if ( WinLines )
      tmp->Draw(0,0,lines);
    if ( FOverlay->Height )
      tmp->Draw(0,0,FOverlay);
    BitBlt(Canvas->Handle, FBorderWidth, FBorderWidth, Width, Height, tmp->Handle, 0, 0, SRCCOPY);
//    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::SetOffset(int Value)
{
  if ( Value != FOffset ) {
    FOffset = Value;
    Setpos();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetDarkness(int Value)
{
  if ( Value != FDarkness ) {
    FDarkness = Value;
    mask->MasterAlpha = FDarkness;
    ConstructLampMask();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetReelHeight(int Value)
{
  if ( Value != FReelHeight && Value ) {
    if ( Value > BandHeight && BandHeight )
      Value = BandHeight;
    FReelHeight = Value;
    InitSize();
    DrawWinLines();
    ConstructLampMask();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetStops(int Value)
{
  if ( Value != FStops ) {
    FStops = Value;
    InitArray();
    ConstructLampMask();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetSteps(int Value)
{
  if ( Value != FSteps ) {
    FSteps = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetBorderWidth(int Value)
{
  if ( Value != FBorderWidth ) {
    FBorderWidth = Value;
    tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
    tmp->Clear(clWhite32);
    mask->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
    DrawWinLines();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetBorderColour(TColor FColour)
{
  if ( FColour != FBorderColour ) {
    FBorderColour = FColour;
    DrawWinLines();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetNumber(int Value)
{
  if ( Value != FNumber ) {
    FNumber = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetWinLines(int Value)
{
  if ( Value != FWinLines ) {
    FWinLines = Value;
    DrawWinLines();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetWinLineWidth(int Value)
{
  if ( Value != FWinLineWidth ) {
    FWinLineWidth = Value;
    DrawWinLines();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetReversed(bool Value)
{
  if ( Value != FReversed ) {
    FReversed = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetLamps(bool Value)
{
  if ( Value != FLamps ) {
    FLamps = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetCustom(bool Value)
{
  if ( Value != FCustom ) {
    FCustom = Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetExpand(bool Value)
{
  if ( FToggle && Value != FExpand ) {
    FExpand = Value;
    if ( Value ) {
      Expanded = FReelHeight;
      FReelHeight = BandHeight;
    } else
      FReelHeight = Expanded;
    InitSize();
    DrawWinLines();
    ConstructLampMask();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetToggle(bool Value)
{
  if ( FToggle != Value ) {
    FToggle = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetInverted(bool Value)
{
  if ( FInverted != Value ) {
    FInverted = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetHorizontal(bool Value)
{
  if ( FHorizontal != Value ) {
    FHorizontal = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::SetBand(Graphics::TBitmap *ptr)
{
  FBand->Assign(ptr);
  FBand2->Assign(ptr);
  BandHeight = FBand->Height / M_PI;
  InitSize();
  Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::SetOverlay(Graphics::TBitmap *ptr)
{
  if ( ptr->Width && ptr->Height ) {
    FOverlay->Assign(ptr);
    FOverlay2->Assign(ptr);
    SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
//  DrawWinLines();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetMask(Graphics::TBitmap *ptr)
{
  FLampMask->Assign(ptr);
  FLampMask2->Assign(ptr);
  if ( !FLampMask->Empty() )
    Dark = *FLampMask->PixelPtr[0][0];
  ConstructLampMask();
  Invalidate();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TFancyReel::GetBand(void)
{
  return FBand2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TFancyReel::GetOverlay(void)
{
  return FOverlay2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TFancyReel::GetMask(void)
{
  return FLampMask2;
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::ConstructLampMask(void)
{
int i, msk = 1;

  mask->Clear(Dark);
  for ( i = 0; i < 6; i++, msk *= 2 )
    if ( (FState & msk) || lamps[i] == -1 )
      mask->Draw( lamprect[i], Rect(0,0, FLampMask->Width, FLampMask->Height), FLampMask);
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I, J;

  ptr->ResetAlpha();
  Colour = Colour & 0x00FFFFFF;
  for ( I = 0; I < ptr->Height; I++ ) {
    for ( J = 0; J < ptr->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour )
        *P = C;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::SetLampNumber(int index, int value)
{
  lamps[index] = value;
}
//---------------------------------------------------------------------------
int __fastcall TFancyReel::GetLampNumber(int index)
{
  return lamps[index];
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::InitSize(void)
{
  bg->SetSize(FBand->Width,BandHeight);
  InitArray();
  ConstructLampMask();
}
//---------------------------------------------------------------------------

void __fastcall TFancyReel::InitArray(void)
{
int j;

  if ( line_array )
    delete[] line_array;
  line_array = new float[BandHeight];

  for ( j = 0; j < 6; j++ )
    lamp_pos[j] = 0;

  symbol = FBand->Height / FStops;
  int mark = (FBand->Height / 4) - ( symbol / 2) - (2 * symbol);
  int pos = 0;

  for ( j = 0; j < BandHeight; j++ ) {
    double angle = acos(((BandHeight/2) - j) / double(BandHeight/2));
    line_array[j] = (angle / M_PI) * (FBand->Height / 2) + 0.5;
    if ( line_array[j] >= mark && !lamp_pos[pos] && pos < 6 ) {
      mark += symbol;
      lamp_pos[pos++] = j;
    }
  }
  for ( j = 0; j < 6; j++ )
    rect[j] = Rect( 0, lamp_pos[j], mask->Width, lamp_pos[j+1]);

  windowrect = Rect( 0, (BandHeight - FReelHeight)/2, bg->Width, (BandHeight + FReelHeight)/2);
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::Resized(TObject *Sender)
{
  tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
  tmp->Clear(clWhite32);
  mask->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
  DrawWinLines();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::Darken(TColor32 F, TColor32 &B, TColor32 M)
{
int fR, fG, fB, bR, bG, bB;
//  Channel separation
  bR = (B >> 16) & 0xFF;
  bG = (B >> 8)  & 0xFF;
  bB = B         & 0xFF;
  fR = (F >> 16) & 0xFF;
  fG = (F >> 8)  & 0xFF;
  fB = F         & 0xFF;
//  Darken
  bR = (bR * fR) >> 8;
  bG = (bG * fG) >> 8;
  bB = (bB * fB) >> 8;
//  Reset - keeping B alpha
  B = (B & 0xff000000) | (bR << 16) | (bG << 8) | bB;
}
//---------------------------------------------------------------------------
void __fastcall TFancyReel::DrawWinLines(void)
{
int i,j;
int height = Height-FBorderWidth*2;
int width = Width-FBorderWidth*2;
TCanvas *Canvas = new TCanvas();


  lines->SetSize(width,height);
  Canvas->Handle = lines->Handle;
  
  for ( j = 0; j < 6; j++ )
    lamprect[j] = Rect(0, (((lamp_pos[j] - (BandHeight/2)) * height) / FReelHeight) + (height/2),
                       width, (((lamp_pos[j+1] - (BandHeight/2)) * height) / FReelHeight) + (height/2));

    lines->Clear(clWhite32);
//    lines->PenColor = Color32(FBorderColour);
    Canvas->Pen->Color = FBorderColour;
    Canvas->Pen->Width = FWinLineWidth;

    for ( i = 0; i < 3; i++ ) {
      if ( i == 1 || ( i == 0 && FWinLines > 2) || ( i == 2 && FWinLines > 1) ) {
        Canvas->MoveTo(0, (lamprect[i+1].Bottom + lamprect[i+1].Top) / 2);
        Canvas->LineTo(lines->Width,(lamprect[i+1].Bottom + lamprect[i+1].Top) / 2);
      }
      if ( FWinLines > 3 ) {
        if ( (i == 0 && FNumber == 0) || ( i == 2 && FNumber == 2) || ( i == 1 && FNumber == 1) ||
             (i == 0 && FNumber == 3) || ( i == 2 && FNumber == 5) || ( i == 1 && FNumber == 4) ) {
          Canvas->MoveTo(0, lamprect[i+1].Top+2);    //  Top to Bottom
          Canvas->LineTo(lines->Width,lamprect[i+1].Bottom-2);
        }
        if ( FWinLines > 5 ) {
          if ( (i == 0 && FNumber == 4) || ( i == 1 && FNumber == 5) || ( i == 2 && FNumber == 6) ) {
            Canvas->MoveTo(0, lamprect[i+1].Top+2);    //  Top to Bottom
            Canvas->LineTo(lines->Width,lamprect[i+1].Bottom-2);
          }
        }
        if ( (i == 2 && FNumber == 0) || ( i == 0 && FNumber == 2) || ( i == 1 && FNumber == 1) ||
             (i == 2 && FNumber == 3) || ( i == 0 && FNumber == 5) || ( i == 1 && FNumber == 4) ) {
          Canvas->MoveTo(0, lamprect[i+1].Bottom-2); // Bottom to Top
          Canvas->LineTo(lines->Width,lamprect[i+1].Top+2);
        }
        if ( FWinLines > 5 ) {
          if ( (i == 2 && FNumber == 4) || ( i == 1 && FNumber == 5) || ( i == 0 && FNumber == 6) ) {
            Canvas->MoveTo(0, lamprect[i+1].Bottom-2); // Bottom to Top
            Canvas->LineTo(lines->Width,lamprect[i+1].Top+2);
          }
        }
      }
    }
    lines->ResetAlpha();
    SetAlpha(lines,clWhite32);
//    lines->SaveToFile("lines.bmp");
    delete Canvas;
}
//---------------------------------------------------------------------------

