//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "grBase.h"
#include "grDiscReel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall TGRDiscReel::TGRDiscReel(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * ADest)
    : TGRBase(ANumber,"TGRDiscReel",AX,AY,AHeight,AWidth, ADest)
{
  lampmask = new TBitmap32();
  lampmask->DrawMode = dmCustom;
  lampmask->OnPixelCombine=GetBlendMode(1);
  lampmask->MasterAlpha = 0xb0;
  mask = new TBitmap32();
  mask->DrawMode=dmBlend;
  mask->Clear();
  FDarkness = 0xb0;
  Dst = new TBitmap32();
  FSrc = new TBitmap32();
  FSrc->DrawMode = dmBlend;
  FOverlay = new TBitmap32();
  FOverlay->DrawMode = dmBlend;
  FLampMask = new Graphics::TBitmap();
  FLampMask->Height = 40;
  FLampMask->Width = 40;
  FSrc2 = new Graphics::TBitmap();
  FOverlay2 = new Graphics::TBitmap();
  FOffset = 0;
  FDrawn = false;
  FReversed = false;
  for ( int i = 0; i < 24; i++ )
    lamps[i] = 256;
  for ( int i = 0; i < 40; i++ )
    on[i] = NULL;
  FSteps = 96;
  FStops = 12;
  bg2 = new TBitmap32();
  bg2->DrawMode = dmBlend;
  Dst->SetSize(AWidth,AHeight);
  Dst->Clear(clWhite32);
  Dst->DrawMode = dmBlend;
  FPosition = 0;
  FOuterH = FOuterL = 0;
  FInnerH = FInnerL = 0;
  FOutline = 0;
  FLampsize = 53;
  FState = 0;
  FLampOffset = 0;
  Bitmap->DrawMode = dmCustom;
  Bitmap->OnPixelCombine = Modulate;
}
//---------------------------------------------------------------------------
__fastcall TGRDiscReel::~TGRDiscReel(void)
{
  delete mask;
  delete lampmask;
  delete Dst;
  delete FSrc;
  delete FOverlay;
  delete FLampMask;
  delete tmp;
  delete FSrc2;
  delete FOverlay2;
  delete bg2;
  for ( int i = 0; i < 40; i++ ) {
    if ( on[i] )
      delete on[i];
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::Draw(void)
{
  image->BeginUpdate();
  Dst->DrawTo(image->Bitmap,Dstrect);
  lampmask->DrawTo(image->Bitmap,Dstrect);
  FOverlay->DrawTo(image->Bitmap,Dstrect);
  image->EndUpdate();
  if ( Flush )
    image->Flush(Dstrect);
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetOuterH(int Value)
{
  if ( Value != FOuterH ) {
    FOuterH = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetOuterL(int Value)
{
  if ( Value != FOuterL ) {
    FOuterL = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetInnerH(int Value)
{
  if ( Value != FInnerH ) {
    FInnerH = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetInnerL(int Value)
{
  if ( Value != FInnerL ) {
    FInnerL = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetLampsize(int Value)
{
  if ( Value != FLampsize ) {
    FLampsize = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetOutline(int Value)
{
  if ( Value != FOutline ) {
    FOutline = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetPos(int Value)
{
  if ( Value != FPosition ) {
    FPosition = Value;
    if ( !FReversed )
      pos = FOffset + ((360 * FPosition)/FSteps);
    else
      pos = FOffset + ((360 * (FSteps - FPosition))/FSteps);
    ScaleRot(pos);
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetLamp(int nbr, int Value)
{
  if ( Value )
    Value = FState | nbr;
  else
    Value = FState & ~nbr;

  if ( Value != FState ) {
    FState = Value;
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetLamps(int Value)
{
  if ( Value != FState ) {
    FState = Value;
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetOffset(int Value)
{
  if ( Value != FOffset ) {
    FOffset = Value;
//    pos = FOffset + (3.75 * FPosition);
    if ( !FReversed )
      pos = FOffset + ((360 * FPosition)/FSteps);
    else
      pos = FOffset + ((360 * (FSteps - FPosition))/FSteps);
    ScaleRot(pos);
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetLampOffset(int Value)
{
  if ( Value != FLampOffset ) {
    FLampOffset = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetDarkness(int Value)
{
  if ( Value != FDarkness ) {
    FDarkness = Value;
    lampmask->MasterAlpha = FDarkness;
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetSteps(int Value)
{
  if ( Value != FSteps ) {
    FSteps = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetStops(int Value)
{
  if ( Value != FStops ) {
    FStops = Value;
    Render();
    DoLamps();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetInverted(bool Value)
{
  if ( Value != FInverted ) {
    FInverted = Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetReversed(bool Value)
{
  if ( Value != FReversed ) {
    FReversed = Value;
    if ( !FReversed )
      pos = FOffset + ((360 * FPosition)/FSteps);
    else
      pos = FOffset + ((360 * (FSteps - FPosition))/FSteps);
    ScaleRot(pos);
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetSrc(Graphics::TBitmap *ptr)
{
  FSrc->Assign(ptr);
  FSrc2->Assign(ptr);
  mask->Width = FSrc->Width;
  mask->Height = FSrc->Height;
  if ( FSrc->Width && FSrc->Height )
    SetMaskAlpha(FSrc, *FSrc->PixelPtr[0][0]);
  Dst->Width = FSrc->Width;
  Dst->Height = FSrc->Height;
  Dst->Clear(clWhite32);
  lampmask->Width = FSrc->Width;
  lampmask->Height = FSrc->Height;
//  Bitmap->SetSize(FSrc->Width,FSrc->Height);
  SetAlpha( Dst, clWhite32);
  bg2->Width = FSrc->Width;
  bg2->Height = FSrc->Height;
  pos = FOffset + ((360 * FPosition)/FSteps);
  ScaleRot(0);
  Render();
  DoLamps();
  Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRDiscReel::SetOverlay(Graphics::TBitmap *ptr)
{
  FOverlay->Assign(ptr);
  FOverlay2->Assign(ptr);
  if ( FOverlay->Width && FOverlay->Height )
    SetAlpha(FOverlay, *FOverlay->PixelPtr[0][0]);
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetMask(Graphics::TBitmap *ptr)
{
  FLampMask->Assign(ptr);
  Render();
  DoLamps();
  Draw();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRDiscReel::GetSrc(void)
{
  return FSrc2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRDiscReel::GetOverlay(void)
{
  return FOverlay2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRDiscReel::GetMask(void)
{
  return FLampMask;
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::ScaleRot(Single Alpha)
{
int SrcR;
int SrcB;
TAffineTransformation *T;

  SrcR = FSrc->Width - 1;
  SrcB = FSrc->Height - 1;
  T = new TAffineTransformation;
  T->SrcRect = FloatRect(0, 0, SrcR + 1, SrcB + 1);

    // shift the origin
    T->Clear();

    // move the origin to a center for scaling and rotation
    T->Translate(-SrcR / 2, -SrcB / 2);
    T->Rotate(0, 0, Alpha);

    // move the origin back
    T->Translate(SrcR / 2, SrcB / 2);

    // transform the bitmap
    Transform(Dst, FSrc, T);

    delete T;
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::Modulate(TColor32 F, TColor32 &B, TColor32 M)
{
  if ( F & 0xff000000 )
    B = F;
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::DoLamps(void)
{
int msk = 1;

  bg2->Assign(mask);
  for ( int i = 0; i < (FStops * 2); i++ ) {
    if ( ( (FState & msk ) && on[i]) ) {
      bg2->Draw(0,0,on[i]);
    }
    msk <<= 1;
  }
  lampmask->Draw(0,0,bg2);
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I, J;

  ptr->ResetAlpha();
  Colour = Colour & 0x00ffffff;

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
void __fastcall TGRDiscReel::SetMaskAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P,Q;
TColor32 C;
int I, J;

  ptr->ResetAlpha();
  Colour = Colour & 0x00ffffff;
  mask->Clear();

  for ( I = 0; I < ptr->Height; I++ ) {
    for ( J = 0; J < ptr->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      Q = mask->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour ) {
        *P = C;
        *Q = *Q & 0xffffff;
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::Render(void)
{
int loop, lampx, lampy;
float x, y, p, q;
float start;
int dix, diy;
Graphics::TBitmap *ptr = new Graphics::TBitmap();
Graphics::TBitmap *ptr2 = new Graphics::TBitmap();
TBitmap32 *new_ptr;

  ptr->Width = FSrc->Width;
  ptr->Height = FSrc->Height;
  ptr2->Width = FSrc->Width;
  ptr2->Height = FSrc->Height;
  ptr2->Canvas->Brush->Color = clBlack;
  dix = FSrc->Width/2;
  diy = FSrc->Height/2;
  ptr->Canvas->Pen->Width = FOutline;
  ptr->Canvas->FillRect(Rect(0,0,ptr->Width,ptr->Height));
  for ( int i = 0; i < 40; i++ ) {
    if ( on[i] ) {
      delete on[i];
      on[i] = NULL;
    }
  }
  start = ((FLampOffset * 2 * M_PI)/360) + ((M_PI/(FStops*2)) * 2);
  for ( loop = 0; loop < FStops; loop++ ) {
    x = dix + (sin(start) * dix);
    y = diy - (cos(start) * diy);
    p = dix + (sin(start - (M_PI/(FStops/2))) * dix);
    q = diy - (cos(start - (M_PI/(FStops/2))) * diy);
    ptr->Canvas->Brush->Color = clBlack;
    ptr->Canvas->FillRect(Rect(0,0,ptr->Width,ptr->Height));
    ptr2->Canvas->FillRect(Rect(0,0,ptr2->Width,ptr2->Height));
    ptr->Canvas->Brush->Color = clWhite;
    ptr->Canvas->Pie(dix-FOuterH,diy-FOuterH,dix+FOuterH,diy+FOuterH,x,y,p,q);
    ptr->Canvas->Brush->Color = clBlack;
    ptr->Canvas->Ellipse(dix-FOuterL,diy-FOuterL,dix+FOuterL,diy+FOuterL);
    lampx = dix + (sin(start - (M_PI/FStops)) * ( (FOuterH + FOuterL) / 2));
    lampy = diy - (cos(start - (M_PI/FStops)) * ( (FOuterH + FOuterL) / 2));
    ptr->Canvas->Brush->Color = clBlack;
    TRect rect = Rect(lampx-FLampsize,lampy-FLampsize,lampx+FLampsize,lampy+FLampsize);
    ptr2->Canvas->CopyMode = cmSrcCopy;
    ptr2->Canvas->StretchDraw(rect, FLampMask);
    ptr->Canvas->CopyMode = cmSrcAnd;
    ptr->Canvas->Draw(0,0,ptr2);
    new_ptr = new TBitmap32();
    new_ptr->DrawMode = dmBlend;
    new_ptr->Width = FSrc->Width;
    new_ptr->Height = FSrc->Height;
    new_ptr->Assign(ptr);
    SetAlpha(new_ptr, clBlack);
    on[loop] = new_ptr;
//    new_ptr->SaveToFile("Outer"+IntToStr(loop)+".bmp");
    start += (M_PI/(FStops/2));
  }
  if ( FInnerH ) {
    start = ((FLampOffset * 2 * M_PI)/360) + ((M_PI/(FStops*2)) * 2);
    for ( loop = 0; loop < FStops; loop++ ) {
      x = dix + (sin(start) * dix);
      y = diy - (cos(start) * diy);
      p = dix + (sin(start - (M_PI/(FStops/2))) * dix);
      q = diy - (cos(start - (M_PI/(FStops/2))) * diy);
      ptr->Canvas->Brush->Color = clBlack;
      ptr->Canvas->FillRect(Rect(0,0,ptr->Width,ptr->Height));
      ptr2->Canvas->FillRect(Rect(0,0,ptr2->Width,ptr2->Height));
      ptr->Canvas->Brush->Color = clWhite;
      ptr->Canvas->Pie(dix-FInnerH,diy-FInnerH,dix+FInnerH,diy+FInnerH,x,y,p,q);
      ptr->Canvas->Brush->Color = clBlack;
      ptr->Canvas->Ellipse(dix-FInnerL,diy-FInnerL,dix+FInnerL,diy+FInnerL);
      lampx = dix + (sin(start - (M_PI/FStops)) * ( (FInnerH + FInnerL) / 2));
      lampy = diy - (cos(start - (M_PI/FStops)) * ( (FInnerH + FInnerL) / 2));
      ptr->Canvas->Brush->Color = clBlack;
      TRect rect = Rect(lampx-FLampsize,lampy-FLampsize,lampx+FLampsize,lampy+FLampsize);
      ptr2->Canvas->CopyMode = cmSrcCopy;
      ptr2->Canvas->StretchDraw(rect, FLampMask);
      ptr->Canvas->CopyMode = cmSrcAnd;
      ptr->Canvas->Draw(0,0,ptr2);
      new_ptr = new TBitmap32();
      new_ptr->DrawMode = dmBlend;
      new_ptr->Width = FSrc->Width;
      new_ptr->Height = FSrc->Height;
      new_ptr->Assign(ptr);
      SetAlpha(new_ptr, clBlack);
      on[FStops + loop] = new_ptr;
//      new_ptr->SaveToFile("Inner"+IntToStr(loop)+".bmp");
      start += (M_PI/(FStops/2));
    }
  }
  delete ptr;
  delete ptr2;
}
//---------------------------------------------------------------------------
void __fastcall TGRDiscReel::SetLampNumber(int index, int value)
{
  lamps[index] = value;
}
//---------------------------------------------------------------------------
int __fastcall TGRDiscReel::GetLamp(int index)
{
  return lamps[index];
}
//---------------------------------------------------------------------------

