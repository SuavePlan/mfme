//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "grBase.h"
#include "grBandReel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TGRBandReel::TGRBandReel(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * ADest)
    : TGRBase(ANumber,"TGRBandReel",AX,AY,AHeight,AWidth, ADest)
{
//  dest = ADest->Bitmap;
  mask = new TBitmap32();
  mask->DrawMode=dmCustom;
  mask->OnPixelCombine=Darken;
  mask->Clear(clBlack32);
  FDarkness = 100;
  FLampMask = new TBitmap32();
  FBand = new TBitmap32();
  FOverlay = new TBitmap32();
  FOverlay->DrawMode = dmBlend;
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
  FVertical = false;
  FBorderColour = clBlack;
  FPosition = 0;
  FLamps = false;
  for ( int i = 0; i < 5; i++ )
    lamps[i] = -2;
  pos = 0;
}
//---------------------------------------------------------------------------
__fastcall TGRBandReel::~TGRBandReel(void)
{
  delete mask;
  delete FBand;
  delete FOverlay;
  delete FLampMask;
  delete FBand2;
  delete FOverlay2;
  delete FLampMask2;
}
//---------------------------------------------------------------------------

void __fastcall TGRBandReel::Draw(void)
{
  if ( FBand->Height ) {
    int start = (bandwidth * (pos % FSteps)) / FSteps;
    if ( !FVertical ) {
      windowrect = Rect( start, 0, start+viewwidth, FBand->Height);
      Bitmap->Draw(Rect(0,0,Width,Height),windowrect,FBand);
      if ( FLamps )
        Bitmap->Draw(Rect(0,0,Width,Height),Rect(0,0,viewwidth,FBand->Height),mask);
    } else {
      windowrect = Rect( 0, start, FBand->Width, start+viewwidth);
      Bitmap->Draw(Rect(0,0,Width,Height),windowrect,FBand);
      if ( FLamps )
        Bitmap->Draw(Rect(0,0,Width,Height),Rect(0,0,FBand->Width,viewwidth),mask);
    }
  }
  if ( FOverlay->Height )
    Bitmap->Draw(0,0,FOverlay);
  TGRBase::Draw();
//  Buffer->DrawTo(dest,X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetPosition(int Value)
{
  if ( Value != FPosition ) {
    FPosition = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRBandReel::Setpos(void)
{
int newpos = FPosition;

  newpos -= FOffset;
  if ( FReversed )
    newpos = FSteps - newpos;
  pos = (newpos + FSteps) % FSteps;
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetLamp(int nbr, int state)
{
  if ( state )
    State |= (1 << nbr);
  else
    State &= ~(1 << nbr);
  ConstructLampMask();
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetLamps(int state)
{
}
//---------------------------------------------------------------------------

void __fastcall TGRBandReel::SetOffset(int Value)
{
  if ( Value != FOffset ) {
    FOffset = Value;
    Setpos();
//    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetDarkness(int Value)
{
  if ( Value != FDarkness ) {
    FDarkness = Value;
    mask->MasterAlpha = FDarkness;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetStops(int Value)
{
  if ( Value != FStops ) {
    FStops = Value;
    DrawBand();
//    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetSteps(int Value)
{
  if ( Value != FSteps ) {
    FSteps = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetBorderWidth(int Value)
{
  if ( Value != FBorderWidth ) {
    FBorderWidth = Value;
//    tmp->SetSize(Width-FBorderWidth*2,Height-FBorderWidth*2);
//    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetBorderColour(TColor FColour)
{
  if ( FColour != FBorderColour ) {
    FBorderColour = FColour;
//    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetNumber(int Value)
{
  if ( Value != FNumber ) {
    FNumber = Value;
//    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetReversed(bool Value)
{
  if ( Value != FReversed ) {
    FReversed = Value;
    Setpos();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetVertical(bool Value)
{
  if ( Value != FVertical ) {
    FVertical = Value;
    Flip();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetLamps(bool Value)
{
  if ( Value != FLamps ) {
    FLamps = Value;
    ConstructLampMask();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetBand(Graphics::TBitmap *ptr)
{
  FBand2->Assign(ptr);
  DrawBand();
  Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRBandReel::SetOverlay(Graphics::TBitmap *ptr)
{
  FOverlay->Assign(ptr);
  FOverlay2->Assign(ptr);
  if ( FOverlay->Width && FOverlay->Height )
    SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetMask(Graphics::TBitmap *ptr)
{
  FLampMask->Assign(ptr);
  FLampMask2->Assign(ptr);
  if ( !FLampMask->Empty() )
    Dark = *FLampMask->PixelPtr[0][0];
  ConstructLampMask();
  Draw();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRBandReel::GetBand(void)
{
  return FBand2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRBandReel::GetOverlay(void)
{
  return FOverlay2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRBandReel::GetMask(void)
{
  return FLampMask2;
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::ConstructLampMask(void)
{
int i, msk = 1;

  mask->Clear(Dark);
  for ( i = 0; i < 5; i++, msk *= 2 )
    if ( (State & msk) || lamps[i] == -1 )
      mask->Draw( lamprect[i], Rect(0,0, FLampMask->Width, FLampMask->Height), FLampMask);
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
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
void __fastcall TGRBandReel::SetLampNumber(int index, int value)
{
  lamps[index] = value;
}
//---------------------------------------------------------------------------
int __fastcall TGRBandReel::GetLampNumber(int index)
{
  return lamps[index];
}
//---------------------------------------------------------------------------
void __fastcall TGRBandReel::Darken(TColor32 F, TColor32 &B, TColor32 M)
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
void __fastcall TGRBandReel::DrawBand(void)
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
void __fastcall TGRBandReel::Flip(void)
{
  DrawBand();
//  if ( (FVertical && Width > Height) || (!FVertical && Height > Width) )
//    SetBounds(Left, Top, Height, Width);
}
