//---------------------------------------------------------------------------
//#include <classes.hpp>
//#pragma hdrstop

#include <stdio.h>
#include "Alphadisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TAlpha *)
{
    new TAlpha(NULL);
}
//---------------------------------------------------------------------------
__fastcall TAlpha::TAlpha(TComponent* Owner)
    : TImage32(Owner)
{
int i;

  Fbmp = new Graphics::TBitmap();
  Src = new TBitmap32();
  Src->DrawMode = dmBlend;
  Mask = new TBitmap32();
  Mask->DrawMode = dmBlend;
  OffMask = new TBitmap32();
  Trans = new TBitmap32();
  Trans->DrawMode = dmBlend;
  FIntensity = 0;
  FOff = 70;
  FColumns = 16;
  FColour = clLime;
  FDigitWidth = 17;
  for ( i = 0; i < 16; i++ ) {
    buffer[i] = 0x20;
    oldbuffer[i] = 0;
  }
  ScaleMode = Gr32_image::smStretch;
  TImage32::Bitmap->StretchFilter = sfLinear;
}
//---------------------------------------------------------------------------
__fastcall TAlpha::~TAlpha()
{
  delete Fbmp;
  delete Src;
  delete Mask;
  delete Trans;
  delete OffMask;
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::Loaded(void)
{
  Setup();
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetBitmap(Graphics::TBitmap *ptr)
{
    Fbmp->Assign(ptr);
    Trans->Assign(ptr);
    Width = FDigitWidth * FColumns;
    Height = Fbmp->Height;
    Setup();
    Draw();
    Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetColour(TColor AColour)
{

    if ( FColour != AColour ) {
      FColour = AColour;
      Setup();
      Draw();
      Invalidate();
    }
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TAlpha::GetBitmap(void)
{
    return Fbmp;
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetColumns(int AColumns)
{
  if ( AColumns != FColumns ) {
    FColumns = AColumns;
    Width = FDigitWidth * FColumns;
    Height = Fbmp->Height;
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetDigitWidth(int AWidth)
{
  if ( AWidth != FDigitWidth ) {
    FDigitWidth = AWidth;
    Setup();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetDisplay(String ADisplay)
{
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetDisplay2(int *ptr)
{
  if ( memcmp( ptr, oldbuff, 64) ) {
    memcpy( buff, ptr, 64);
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetIntensity(int AIntensity)
{
/*static int intensities[32] = {
7,   15,  23,  31,  39,  47,  55,  63,
71,  79,  87,  95,  103, 111, 119, 127,
135, 143, 151, 159, 167, 175, 183, 191,
199, 207, 215, 223, 231, 239, 247, 255
}; */

static int intensities[32] = {
15,   39,  70,  95, 110, 125, 135, 145,
169, 175, 181, 187, 193, 198, 203, 208,
213, 218, 222, 226, 230, 234, 237, 240,
243, 246, 248, 250, 252, 253, 254, 255
};

  if ( FIntensity != AIntensity ) {
    FIntensity = AIntensity;
    Src->MasterAlpha = intensities[FIntensity];//7 + FIntensity * 8;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetReversed(bool AReversed)
{
  if ( FReversed != AReversed ) {
    FReversed = AReversed;
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetOffBrightness(int AOff)
{
  if ( FOff != AOff ) {
    FOff = AOff;
    Mask->MasterAlpha = FOff;
    OffMask->Clear(clBlack32);
    OffMask->Draw(0,0,Mask);
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::Draw(void)
{
int i;
int chr;
int index;
int *ch_ptr;

  ch_ptr = buff;
  Src->Clear(0);
  for ( i = 0; i < FColumns; i++ ) {
    chr = *ch_ptr++;
    if ( FReversed )
      index = i;
    else
      index = FColumns - i - 1;
    if ( chr & 0xffff0000 )
      DrawCustom( chr >> 16, index);
    else {
      if ( chr && ((chr & 0xF0000) == 0) )
        chr--;
      else
        chr = 0x1f;
      GetChar(Src, chr & 0x3f, index);
      if ( (chr & 0xff00) == 0x2c00 || (chr & 0xff00) == 0x2e00 )
        GetChar(Src, ((chr & 0xff00) >> 8) - 1, index);
    }
    oldbuffer[i] = chr;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::Paint(void)
{
  BeginUpdate();
  TImage32::Bitmap->Assign(OffMask);
  TImage32::Bitmap->Draw(0,0,Src);
  EndUpdate();
  TImage32::Paint();
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I;

  Colour = Colour & 0x00FFFFFF;
  P = ptr->PixelPtr[0][0];
  for ( I = 0; I < ptr->Width * ptr->Height; I++, P++ ) {
    C = *P & 0x00FFFFFF;
    if ( C == Colour )
      *P = C;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetMask(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I;

  Colour = Colour & 0x00FFFFFF;
  P = ptr->PixelPtr[0][0];
  for ( I = 0; I < ptr->Width * ptr->Height; I++, P++ ) {
    C = *P & 0x00FFFFFF;
      if ( ( C & 0xff ) < 0x64 && ( C & 0xff00 ) < 0x6400 && ( C & 0xff0000 ) < 0x640000 )
//    if ( C != Colour )
      *P = C;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::SetColor(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I;

  P = ptr->PixelPtr[0][0];
  for ( I = 0; I < ptr->Width * ptr->Height; I++, P++ ) {
    C = *P & 0x00FFFFFF;
    if ( C == 0x00FFFFFF )
      *P = Colour;
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::GetChar(TBitmap32 * ptr, int value, int pos)
{
int width = FDigitWidth;
int height = Fbmp->Height;
TRect SrcRect = Rect( width * value, 0, width + width * value, height);
TRect DstRect;

  if ( (width + width * value) < Trans->Width ) {
    DstRect = Rect( width * pos, 0, width + width * pos, height);
    ptr->Draw( DstRect, SrcRect, Trans);
  }

}
//---------------------------------------------------------------------------
void __fastcall TAlpha::Setup(void)
{
int i;

  FDigitWidth = Fbmp->Width / 64;
  Mask->Width = FDigitWidth * FColumns;
  Mask->Height = Fbmp->Height;
  OffMask->Width = Mask->Width;
  OffMask->Height = Mask->Height;
  Src->Width = Mask->Width;
  Src->Height = Mask->Height;
  Mask->Clear(clBlack32);
  Trans->Assign(Fbmp);
  GenerateOff(Mask);
  SetColor(Mask, Color32(FColour));
//  Mask->SaveToFile("mask.bmp");
  SetMask(Trans, clWhite32);
//  Trans->SaveToFile("trans2.bmp");
  SetColor(Trans, Color32(FColour));
//  Trans->SaveToFile("trans.bmp");
  Mask->MasterAlpha = FOff;
  OffMask->Clear(clBlack32);
  OffMask->Draw(0,0,Mask);
//  OffMask->SaveToFile("offmask.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::DrawCustom(int value, int pos)
{
  for ( int i = 0; i < 16; i++ ) {
    if ( value & (1 << i) )
      GetChar(Src, i + 64, pos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TAlpha::GenerateOff(TBitmap32 * ptr)
{
PColor32 P;
TColor32 C;
int I, i;
int width = FDigitWidth;
int height = Fbmp->Height;
TRect SrcRect = Rect( width * 0x1f, 0, width + width * 0x1f, height);
TRect DstRect = Rect( 0, 0, width, height);

  TBitmap32 *bmp = new TBitmap32();
  bmp->Width = width;
  bmp->Height = height;

  bmp->Draw( DstRect, SrcRect, Trans);

  P = bmp->PixelPtr[0][0];
  for ( I = 0; I < width * height; I++, P++ ) {
    C = *P & 0x00FFFFFF;
    if ( C != 0 )
      *P = 0xFFFFFFFF;
  }
  for ( i = 0; i < FColumns; i++ ) {
    SrcRect = Rect( width * i, 0, width + width * i, height);
    ptr->Draw( SrcRect, DstRect, bmp);
  }
  delete bmp;
}
//---------------------------------------------------------------------------

