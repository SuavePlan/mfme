//---------------------------------------------------------------------------
//#include <classes.hpp>
//#pragma hdrstop

#include <stdio.h>
#include "grbase.h"
#include "grAlphadisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TGRAlpha::TGRAlpha(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
    : TGRBase(ANumber, "TGRAlpha", AX, AY, AHeight, AWidth, Adest)
{
int i;

//  dest = Adest->Bitmap;
//  tmp = new TBitmap32();
//  tmp->SetSize(Width,Height);
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
    buff[i] = 0x20;
    buffer[i] = 0x20;
    oldbuffer[i] = 0;
  }
  Bitmap->DrawMode = dmOpaque;
  Bitmap->StretchFilter = sfLinear2;
  Render();
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRAlpha::~TGRAlpha()
{
  delete Fbmp;
  delete Src;
  delete Mask;
  delete Trans;
  delete OffMask;
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetBitmap(Graphics::TBitmap *ptr)
{
    Fbmp->Assign(ptr);
    Trans->Assign(ptr);
    Setup();
    Render();
    Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetColour(TColor AColour)
{

    if ( FColour != AColour ) {
      FColour = AColour;
      Setup();
      Render();
      Draw();
    }
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRAlpha::GetBitmap(void)
{
    return Fbmp;
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetColumns(int AColumns)
{
  if ( AColumns != FColumns ) {
    FColumns = AColumns;
//    Width = FDigitWidth * FColumns;
//    Height = Fbmp->Height;
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetDigitWidth(int AWidth)
{
  if ( AWidth != FDigitWidth ) {
    FDigitWidth = AWidth;
    Setup();
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetDisplay2(int *ptr)
{
  if ( memcmp( ptr, oldbuff, 64) ) {
    memcpy( buff, ptr, 64);
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetIntensity(int AIntensity)
{
static int intensities[32] = {
15,   39,  70,  95, 110, 125, 135, 145,
169, 175, 181, 187, 193, 198, 203, 208,
213, 218, 222, 226, 230, 234, 237, 240,
243, 246, 248, 250, 252, 253, 254, 255
};

  if ( FIntensity != AIntensity ) {
    FIntensity = AIntensity;
    Src->MasterAlpha = intensities[FIntensity];
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetReversed(bool AReversed)
{
  if ( FReversed != AReversed ) {
    FReversed = AReversed;
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetOffBrightness(int AOff)
{
  if ( FOff != AOff ) {
    FOff = AOff;
    Mask->MasterAlpha = FOff;
    OffMask->Clear(clBlack32);
    OffMask->Draw(0,0,Mask);
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::Render(void)
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
void __fastcall TGRAlpha::Draw(void)
{
  Bitmap->Draw(Rect(0,0,Width,Height),Rect(0,0,OffMask->Width,OffMask->Height),OffMask);
  Bitmap->Draw(Rect(0,0,Width,Height),Rect(0,0,OffMask->Width,OffMask->Height),Src);
  TGRBase::Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
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
void __fastcall TGRAlpha::SetMask(TBitmap32 *ptr, TColor32 Colour)
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
void __fastcall TGRAlpha::SetColor(TBitmap32 *ptr, TColor32 Colour)
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
void __fastcall TGRAlpha::GetChar(TBitmap32 * ptr, int value, int pos)
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
void __fastcall TGRAlpha::Setup(void)
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
void __fastcall TGRAlpha::DrawCustom(int value, int pos)
{
  for ( int i = 0; i < 16; i++ ) {
    if ( value & (1 << i) )
      GetChar(Src, i + 64, pos);
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRAlpha::GenerateOff(TBitmap32 * ptr)
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

void __fastcall TGRAlpha::SaveToStream(TStream *ptr)
{
  Trans->SaveToStream(ptr);
}

