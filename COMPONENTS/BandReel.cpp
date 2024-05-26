//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "BandReel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TBandReel *)
{
    new TBandReel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TBandReel::TBandReel(TComponent* Owner)
    : TGraphicControl(Owner)
{
  mask = new TBitmap32();
  mask->DrawMode=dmCustom;
  mask->OnPixelCombine=Darken;
  mask->Clear(clBlack32);
  FDarkness = 100;
  FLampMask = new TBitmap32();
  FBand = new TBitmap32();
  FOverlay = new TBitmap32();
  FOverlay->DrawMode = dmBlend;
  Height = 80;
  Width = 200;
  FBorderWidth = 1;
  FLampMask2 = new Graphics::TBitmap();
  FLampMask2->Height = 190;
  FLampMask2->Width = 190;
  FLampMask2->Canvas->Brush->Color = clWhite;
  FBand2 = new Graphics::TBitmap();
  FOverlay2 = new Graphics::TBitmap();
  FOffset = 0;
  FStops = 16;
  FSteps = 320;
  FBand2->Width = 80;
  FBand2->Height = FStops * 56;
  FReversed = false;
  FState = 0;
  FVertical = false;
  FBorderColour = clBlack;
  for ( int i = 0; i < 5; i++ )
    lamps[i] = -2;
  TGraphicControl::OnResize = Resized;
  tmp = new TBitmap32();
  tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
  tmp->Clear(clWhite32);
}
//---------------------------------------------------------------------------
__fastcall TBandReel::~TBandReel(void)
{
  delete mask;
  delete FBand;
  delete FOverlay;
  delete FLampMask;
  delete FBand2;
  delete FOverlay2;
  delete FLampMask2;
  delete tmp;
}
//---------------------------------------------------------------------------

void __fastcall TBandReel::Loaded(void)
{
    TGraphicControl::Loaded();
    DrawBand();
    FOverlay->Assign(FOverlay2);
    if ( !FOverlay->Empty() )
      SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
    else
      FOverlay->SetSize(Width,Height);
    FOverlay->PenColor = clBlack32;
    tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
    FLampMask->Assign(FLampMask2);
    if ( !FLampMask->Empty() )
      Dark = *FLampMask->PixelPtr[0][0];
    TGraphicControl::OnResize = Resized;
    ConstructLampMask();
}
//---------------------------------------------------------------------------

void __fastcall TBandReel::Paint(void)
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
void __fastcall TBandReel::Draw(void)
{
int j;

  if ( FBand->Height ) {
    int start = (bandwidth * (pos % FSteps)) / FSteps;
    if ( !Vertical ) {
      windowrect = Rect( start, 0, start+viewwidth, FBand->Height);
      tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),windowrect,FBand);
      if ( FLamps )
        tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),Rect(0,0,viewwidth,FBand->Height),mask);
    } else {
      windowrect = Rect( 0, start, FBand->Width, start+viewwidth);
      tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),windowrect,FBand);
      if ( FLamps )
        tmp->Draw(Rect(0,0,tmp->Width,tmp->Height),Rect(0,0,FBand->Width,viewwidth),mask);
    }
  }
  if ( FOverlay->Height )
    tmp->Draw(0,0,FOverlay);
  BitBlt(Canvas->Handle, FBorderWidth, FBorderWidth, Width, Height, tmp->Handle, 0, 0, SRCCOPY);
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetPosition(int Value)
{
  if ( Value != FPosition ) {
    FPosition = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------

void __fastcall TBandReel::Setpos(void)
{
int newpos = FPosition;

  newpos -= FOffset;
  if ( FReversed )
    newpos = FSteps - newpos;
  pos = (newpos + FSteps) % FSteps;
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TBandReel::SetLamp(int nbr, bool state)
{
  if ( state )
    FState |= (1 << nbr);
  else
    FState &= ~(1 << nbr);
  ConstructLampMask();
  Draw();
}
//---------------------------------------------------------------------------

void __fastcall TBandReel::SetOffset(int Value)
{
  if ( Value != FOffset ) {
    FOffset = Value;
    Setpos();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetDarkness(int Value)
{
  if ( Value != FDarkness ) {
    FDarkness = Value;
    mask->MasterAlpha = FDarkness;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetStops(int Value)
{
  if ( Value != FStops ) {
    FStops = Value;
    DrawBand();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetSteps(int Value)
{
  if ( Value != FSteps ) {
    FSteps = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetBorderWidth(int Value)
{
  if ( Value != FBorderWidth ) {
    FBorderWidth = Value;
    tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetBorderColour(TColor FColour)
{
  if ( FColour != FBorderColour ) {
    FBorderColour = FColour;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetNumber(int Value)
{
  if ( Value != FNumber ) {
    FNumber = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetReversed(bool Value)
{
  if ( Value != FReversed ) {
    FReversed = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetVertical(bool Value)
{
  if ( Value != FVertical ) {
    FVertical = Value;
    Flip();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetLamps(bool Value)
{
  if ( Value != FLamps ) {
    FLamps = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetBand(Graphics::TBitmap *ptr)
{
  FBand2->Assign(ptr);
  DrawBand();
  Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TBandReel::SetOverlay(Graphics::TBitmap *ptr)
{
  FOverlay->Assign(ptr);
  FOverlay2->Assign(ptr);
  if ( FOverlay->Width && FOverlay->Height )
    SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetMask(Graphics::TBitmap *ptr)
{
  FLampMask->Assign(ptr);
  FLampMask2->Assign(ptr);
  if ( !FLampMask->Empty() )
    Dark = *FLampMask->PixelPtr[0][0];
  ConstructLampMask();
  Invalidate();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TBandReel::GetBand(void)
{
  return FBand2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TBandReel::GetOverlay(void)
{
  return FOverlay2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TBandReel::GetMask(void)
{
  return FLampMask2;
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::ConstructLampMask(void)
{
int i, msk = 1;

  mask->Clear(Dark);
  for ( i = 0; i < 5; i++, msk *= 2 )
    if ( (FState & msk) || lamps[i] == -1 )
      mask->Draw( lamprect[i], Rect(0,0, FLampMask->Width, FLampMask->Height), FLampMask);
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I, J;

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
void __fastcall TBandReel::SetLampNumber(int index, int value)
{
  lamps[index] = value;
}
//---------------------------------------------------------------------------
int __fastcall TBandReel::GetLampNumber(int index)
{
  return lamps[index];
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::Resized(TObject *Sender)
{
  tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
  tmp->Clear(clWhite32);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::Darken(TColor32 F, TColor32 &B, TColor32 M)
{
int fR, fG, fB, bR, bG, bB;
//  Channel separation
  bR = (B >> 16) & 0xFF;
  bG = (B >> 8)  & 0xFF;
  bB = B        & 0xFF;
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
void __fastcall TBandReel::DrawBand(void)
{
int width, height, i;
TRect src, dst;
TBitmap32 *cpy = new TBitmap32();

  cpy->Assign(FBand2);
  width = FBand2->Width;
  height = FBand2->Height / FStops;

  if ( !FVertical ) {
    FBand->Width = width * ( FStops + 5 );
    FBand->Height = height;

    for ( i = 0; i < FStops; i++ ) {
      src = Rect(0, i*height, width, height+i*height);
      dst = Rect(i*width, 0, width+i*width, height);
      FBand->Draw(dst, src, cpy);
    }
    for ( i = 0; i < 5; i++ ) {
      src = Rect(0, i*height, width, height+i*height);
      dst = Rect((i+FStops)*width, 0, width+(i+FStops)*width, height);
      FBand->Draw(dst, src, cpy);
    }
    viewwidth = 5 * width;
    bandwidth = FStops * width;
    mask->SetSize(viewwidth,height);
    for ( i = 0; i < 5; i++ )
      lamprect[i] = Rect(i*width,0,width+i*width,height);
  } else {
    FBand->Width = width;
    FBand->Height = height * ( FStops + 5 );;

    for ( i = 0; i < FStops; i++ ) {
      src = Rect(0, i*height, width, height+i*height);
      dst = Rect(0, i*height, width, height+i*height);
      FBand->Draw(dst, src, cpy);
    }
    for ( i = 0; i < 5; i++ ) {
      src = Rect(0, i*height, width, height+i*height);
      dst = Rect(0, (i+FStops)*height, width, height+(i+FStops)*height);
      FBand->Draw(dst, src, cpy);
    }
    viewwidth = 5 * height;
    bandwidth = FStops * height;
    mask->SetSize(width,viewwidth);
    for ( i = 0; i < 5; i++ )
      lamprect[i] = Rect(0,i*height,width,height+i*height);
  }

  delete cpy;
}
//---------------------------------------------------------------------------
void __fastcall TBandReel::Flip(void)
{
  DrawBand();
  if ( (FVertical && Width > Height) || (!FVertical && Height > Width) )
    SetBounds(Left, Top, Height, Width);
}
