//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "DiscReel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDiscReel *)
{
    new TDiscReel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDiscReel::TDiscReel(TComponent* Owner)
    : TImage32(Owner)
{
  drawn = false;
//  onlist = new TImageList(this);
  mask = new TBitmap32();
  mask->DrawMode=dmCustom;
  mask->OnPixelCombine=GetBlendMode(1);
  mask->MasterAlpha = 0xb0;
  FDarkness = 0xb0;
  Dst = new TBitmap32();
//  Dst->DrawMode = dmCustom;
//  Dst->OnPixelCombine = Modulate;
  FSrc = new TBitmap32();
  FSrc->DrawMode = dmBlend;
  FOverlay = new TBitmap32();
  FOverlay->DrawMode = dmBlend;
  Height = 160;
  Width = 160;
  ScaleMode = Gr32_image::smStretch;
  FLampMask = new Graphics::TBitmap();
  FLampMask->Height = 40;
  FLampMask->Width = 40;
  FSrc2 = new Graphics::TBitmap();
  FSrc2->Height = 160;
  FSrc2->Width = 160;
  FOverlay2 = new Graphics::TBitmap();
  FOverlay2->Height = 160;
  FOverlay2->Width = 160;
  FOffset = 0;
  FDrawn = false;
  FReversed = false;
  for ( int i = 0; i < 24; i++ )
    lamps[i] = 256;
  Color = clWhite;
  FSteps = 96;
  FStops = 12;
  bg = new Graphics::TBitmap();
  bg2 = new TBitmap32();
  bg2->DrawMode = dmBlend;
}
//---------------------------------------------------------------------------
__fastcall TDiscReel::~TDiscReel(void)
{
//  delete onlist;
  delete mask;
  delete Dst;
  delete FSrc;
  delete FOverlay;
  delete FLampMask;
  delete tmp;
  delete FSrc2;
  delete FOverlay2;
  delete bg;
  delete bg2;
  for ( int i = 0; i < 40; i++ ) {
    if ( on[i] )
      delete on[i];
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::Loaded(void)
{
    FSrc->Assign(FSrc2);
    bg->Width = FSrc->Width;
    bg->Height = FSrc->Height;
    bg->Canvas->CopyMode = cmSrcPaint;
    bg->Canvas->Brush->Color = clBlack;
    bg2->Width = FSrc->Width;
    bg2->Height = FSrc->Height;
    FOverlay->Assign(FOverlay2);
    background = FLampMask->Canvas->Pixels[0][0];
    SetAlpha(FSrc, *FSrc->PixelPtr[0][0]);
    SetAlpha(FOverlay, *FOverlay->PixelPtr[0][0]);
    Dst->Width = FSrc->Width;
    Dst->Height = FSrc->Height;
    mask->Width = FSrc->Width;
    mask->Height = FSrc->Height;
//    onlist->Width = FSrc->Width;
//    onlist->Height = FSrc->Height;
    pos = FOffset + ((360 * FPosition)/FSteps);
    ScaleRot(pos);
    Draw();
    DoLamps();
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::Paint(void)
{
  BeginUpdate();
  Bitmap->Assign(Dst);
  Bitmap->Draw(0,0,mask);
  Bitmap->Draw(0,0,FOverlay);
  EndUpdate();
  TImage32::Paint();
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetOuterH(int Value)
{
  if ( Value != FOuterH ) {
    FOuterH = Value;
    Draw();
    DoLamps();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetOuterL(int Value)
{
  if ( Value != FOuterL ) {
    FOuterL = Value;
    Draw();
    DoLamps();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetInnerH(int Value)
{
  if ( Value != FInnerH ) {
    FInnerH = Value;
    Draw();
    DoLamps();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetInnerL(int Value)
{
  if ( Value != FInnerL ) {
    FInnerL = Value;
    Draw();
    DoLamps();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetLampsize(int Value)
{
  if ( Value != FLampsize ) {
    FLampsize = Value;
//    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetOutline(int Value)
{
  if ( Value != FOutline ) {
    FOutline = Value;
//    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetPosition(int Value)
{
  if ( Value != FPosition ) {
    FPosition = Value;
    if ( !FReversed )
      pos = FOffset + ((360 * FPosition)/FSteps);
    else
      pos = FOffset + ((360 * (FSteps - FPosition))/FSteps);
    ScaleRot(pos);
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetState(int Value)
{
  if ( Value != FState ) {
    FState = Value;
    DoLamps();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetOffset(int Value)
{
  if ( Value != FOffset ) {
    FOffset = Value;
//    pos = FOffset + (3.75 * FPosition);
    if ( !FReversed )
      pos = FOffset + ((360 * FPosition)/FSteps);
    else
      pos = FOffset + ((360 * (FSteps - FPosition))/FSteps);
    ScaleRot(pos);
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetLampOffset(int Value)
{
  if ( Value != FLampOffset ) {
    FLampOffset = Value;
//    Draw();
//    DoLamps();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetDarkness(int Value)
{
  if ( Value != FDarkness ) {
    FDarkness = Value;
    mask->MasterAlpha = FDarkness;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetSteps(int Value)
{
  if ( Value != FSteps ) {
    FSteps = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetStops(int Value)
{
  if ( Value != FStops ) {
    FStops = Value;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetInverted(bool Value)
{
  if ( Value != FInverted ) {
    FInverted = Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetReversed(bool Value)
{
  if ( Value != FReversed ) {
    FReversed = Value;
    if ( !FReversed )
      pos = FOffset + ((360 * FPosition)/FSteps);
    else
      pos = FOffset + ((360 * (FSteps - FPosition))/FSteps);
    ScaleRot(pos);
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetSrc(Graphics::TBitmap *ptr)
{
  FSrc->Assign(ptr);
  FSrc2->Assign(ptr);
  if ( FSrc->Width && FSrc->Height )
    SetAlpha(FSrc, *FSrc->PixelPtr[0][0]);
  Dst->Width = FSrc->Width;
  Dst->Height = FSrc->Height;
  mask->Width = FSrc->Width;
  mask->Height = FSrc->Height;
//  onlist->Width = FSrc->Width;
//  onlist->Height = FSrc->Height;
  bg->Width = FSrc->Width;
  bg->Height = FSrc->Height;
  bg->Canvas->CopyMode = cmSrcPaint;
  bg->Canvas->Brush->Color = clBlack;
  bg2->Width = FSrc->Width;
  bg2->Height = FSrc->Height;
//  pos = FOffset + ((360 * FPosition)/FSteps);
//  ScaleRot(pos);
//  ScaleRot(0);
  Draw();
  DoLamps();
  Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TDiscReel::SetOverlay(Graphics::TBitmap *ptr)
{
  FOverlay->Assign(ptr);
  FOverlay2->Assign(ptr);
  if ( FOverlay->Width && FOverlay->Height )
    SetAlpha(FOverlay, *FOverlay->PixelPtr[0][0]);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetMask(Graphics::TBitmap *ptr)
{
  FLampMask->Assign(ptr);
  background = FLampMask->Canvas->Pixels[0][0];
  Draw();
  DoLamps();
  Invalidate();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TDiscReel::GetSrc(void)
{
  return FSrc2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TDiscReel::GetOverlay(void)
{
  return FOverlay2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TDiscReel::GetMask(void)
{
  return FLampMask;
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::ScaleRot(Single Alpha)
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
//    Alpha = Alpha * 3.14159265358979 / 180;


    // move the origin back
    T->Translate(SrcR / 2, SrcB / 2);

    // transform the bitmap
    Dst->Clear(clWhite32);
    Transform(Dst, FSrc, T);

    delete T;
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::Modulate(TColor32 F, TColor32 &B, TColor32 M)
{
  B = ColorModulate(F,B);
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::DoLamps(void)
{
Graphics::TBitmap *ptr = new Graphics::TBitmap();
int msk = 1;

//  dix = FSrc->Width/2;
//  diy = FSrc->Height/2;
//  bg->Width = FSrc->Width;
//  bg->Height = FSrc->Height;
//  bg->Canvas->CopyMode = cmSrcPaint;
//  bg->Canvas->Brush->Color = clBlack;
  bg->Canvas->FillRect(Rect(0,0,mask->Width,mask->Height));
  bg2->Clear();
//  bg->Canvas->Brush->Color = background;
//  bg->Canvas->Ellipse(Rect(dix-FOuterH,diy-FOuterH,dix+FOuterH,diy+FOuterH));
//  mask->FillRect(0,0,mask->Width,mask->Height, clBlack32);
  for ( int i = 0; i < (FStops * 2); i++ ) {
    if ( (FState & msk ) && on[i] ) {
//      onlist->GetBitmap(i, ptr);
//      bg->Canvas->Draw(0,0, ptr);
      bg2->Draw(0,0,on[i]);
    }
    msk <<= 1;
  }
  mask->Draw(0,0,bg2);
//  SetAlpha( mask, clBlack32);
  delete ptr;
//  delete bg;

}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I;

  Colour = Colour & 0x00FFFFFF;
  P = ptr->PixelPtr[0][0];
  for ( I = 0; I < ptr->Width * ptr->Height ; I++, P++) {
    C = *P & 0x00FFFFFF;
    if ( C == Colour )
      *P = C;
  }
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::Draw(void)
{
int loop, lampx, lampy;
float x, y, p, q;
float start;
int dix, diy;
Graphics::TBitmap *ptr = new Graphics::TBitmap();
Graphics::TBitmap *ptr2 = new Graphics::TBitmap();
TBitmap32 *new_ptr;

//  onlist->Clear();
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
//    onlist->Add(ptr,NULL);
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
//      onlist->Add(ptr,NULL);
      start += (M_PI/(FStops/2));
    }
  }
  delete ptr;
  delete ptr2;
}
//---------------------------------------------------------------------------
void __fastcall TDiscReel::SetLamp(int index, int value)
{
  lamps[index] = value;
}
//---------------------------------------------------------------------------
int __fastcall TDiscReel::GetLamp(int index)
{
  return lamps[index];
}
//---------------------------------------------------------------------------

