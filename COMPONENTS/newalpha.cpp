//---------------------------------------------------------------------------
//#include <classes.hpp>
//#pragma hdrstop

#include <stdio.h>
#include "newalpha.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static Word chrset[64] = {
0xA626,
0xE027,
0x462E,
0x2205,
0x062E,
0xA205,
0xA005,
0x6225,
0xE023,
0x060C,
0x2222,
0xA881,
0x2201,
0x20E3,
0x2863,
0x2227,
0xE007,
0x2A27,
0xE807,
0xC225,
0x040C,
0x2223,
0x2091,
0x2833,
0x08D0,
0x04C0,
0x0294,
0x2205,
0x0840,
0x0226,
0x0810,
0x0200,
0x0000,  // Space
0xC290,
0x0009,
0xC62A,
0xC62D,
0x0000,  // Flash
0x0000,  // Not Defined
0x0080,
0x0880,
0x0050,
0xCCD8,
0xC408,
0x1000,  // DP
0xC000,
0x1000,  // DP
0x0090,
0x22B7,
0x0408,
0xE206,
0xC226,
0xC023,
0xC225,
0xE225,
0x0026,
0xE227,
0xC227,
0x0000,  // User Defined
0x0000,  // Dummy Character
0x0290,
0xC200,
0x0A40,
0x4406
};

static inline void ValidCtrCheck(TBFMAlpha *)
{
    new TBFMAlpha(NULL);
}
//---------------------------------------------------------------------------
__fastcall TBFMAlpha::TBFMAlpha(TComponent* Owner)
    : TImage32(Owner)
{
int i;

  Fbmp = new Graphics::TBitmap();
  Src = new TBitmap32();
  Src->DrawMode = dmBlend;
  Src->MasterAlpha = 255;
  OffMask = new TBitmap32();
  FOff = 70;
  Trans = new TBitmap32();
  Trans->DrawMode = dmBlend;
  Charmap = new TBitmap32();
  Charmap->DrawMode = dmBlend;
  FColumns = 16;
  FColour = clLime;
  FOffColour = clLime;
  FDigitWidth = 17;
  for ( i = 0; i < 16; i++ ) {
    buffer[i] = 0x20;
    oldbuffer[i] = 0;
  }
  ScaleMode = Gr32_image::smStretch;
//  TImage32::Bitmap->StretchFilter = sfLinear2;
}
//---------------------------------------------------------------------------
__fastcall TBFMAlpha::~TBFMAlpha()
{
  delete Fbmp;
  delete Src;
  delete Trans;
  delete OffMask;
  delete Charmap;
}
//---------------------------------------------------------------------------
void __fastcall TBFMAlpha::Loaded(void)
{
  Setup();
}
//---------------------------------------------------------------------------
void __fastcall TBFMAlpha::SetBitmap(Graphics::TBitmap *ptr)
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
void __fastcall TBFMAlpha::SetColour(TColor AColour)
{
  if ( FColour != AColour ) {
    FColour = AColour;
    Setup();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMAlpha::SetOffColour(TColor AColour)
{
  if ( FOffColour != AColour ) {
    FOffColour = AColour;
    Setup();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TBFMAlpha::GetBitmap(void)
{
  return Fbmp;
}
//---------------------------------------------------------------------------
void __fastcall TBFMAlpha::SetColumns(int AColumns)
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
void __fastcall TBFMAlpha::SetDigitWidth(int AWidth)
{
  if ( AWidth != FDigitWidth ) {
    FDigitWidth = AWidth;
    Setup();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::SetDisplay2(int *ptr)
{
  if ( memcmp( ptr, oldbuff, 64) ) {
    memcpy( buff, ptr, 64);
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::SetIntensity(int AIntensity)
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
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::SetReversed(bool AReversed)
{
  if ( FReversed != AReversed ) {
    FReversed = AReversed;
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::Draw(void)
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
    if ( chr & 0x2000000 )
      DrawCustom( Src, chr, index);
    else {
      if ( chr && ((chr & 0x1000000) == 0) )
        ;
      else
        chr = 0x20;
      GetChar(Src, chr & 0x3f, index);
      if ( (chr & 0xff0000) == 0x2c0000 || (chr & 0xff0000) == 0x2e0000 ) // DP
        GetChar(Src, (chr & 0xff0000) >> 16, index);
    }
    oldbuffer[i] = chr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::Paint(void)
{
  BeginUpdate();
  TImage32::Bitmap->Assign(OffMask);
  TImage32::Bitmap->Draw(0,0,Src);
  EndUpdate();
  TImage32::Paint();
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::SetColor(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I;

  P = ptr->PixelPtr[0][0];
  for ( I = 0; I < ptr->Width * ptr->Height; I++, P++ ) {
    C = *P & 0x00FFFFFF;
    if ( C )
      *P = Colour;
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::GetChar(TBitmap32 * ptr, int value, int pos)
{
int width = FDigitWidth;
int height = Fbmp->Height;
TRect SrcRect = Rect( width * value, 0, width + width * value, height);
TRect DstRect;

  if ( (width + width * value) < Charmap->Width ) {
    DstRect = Rect( width * pos, 0, width + width * pos, height);
    ptr->Draw( DstRect, SrcRect, Charmap);
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::Setup(void)
{
  FDigitWidth = Fbmp->Width / 16;
  Charmap->Width = 64 * FDigitWidth;
  Charmap->Height = Fbmp->Height;
  OffMask->Width = FDigitWidth * FColumns;
  OffMask->Height = Fbmp->Height;
  DrawCharMap();
  Src->Width = FDigitWidth * FColumns;
  Src->Height = Fbmp->Height;
//  Charmap->SaveToFile("Charmap.bmp");
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::DrawCustom(TBitmap32 * ptr, int value, int pos)
{
int mask = 1;
TRect dstrect;
TRect srcrect;

  dstrect = Rect( FDigitWidth * pos, 0, FDigitWidth + FDigitWidth * pos, Fbmp->Height);

  for ( int i = 0; i < 16; i++ ) {
    if ( value & mask ) {
      srcrect = Rect(i * FDigitWidth, 0, FDigitWidth + i * FDigitWidth, Fbmp->Height);
      ptr->Draw(dstrect,srcrect,Trans);
    }
    mask <<= 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::DrawCharMap(void)
{
TRect dstrect, srcrect, targetrect;
TBitmap32 * newdigit = new TBitmap32();
int mask;

  newdigit->Width = FDigitWidth;
  newdigit->Height = Fbmp->Height;
  newdigit->DrawMode = dmBlend;
  newdigit->MasterAlpha = FOff;
//  Trans->DrawMode = dmBlend;
  Trans->Assign(Fbmp);
  SetTrans(Trans,clWhite32);
  dstrect = Rect( 0, 0, FDigitWidth, Fbmp->Height);
  Charmap->Clear(0);
//  Trans->SaveToFile("Trans.bmp");
  mask = 1;
  newdigit->Clear(0);

  for ( int i = 0; i < 16; i++ ) {
    if ( i == 8 )
      continue;
    srcrect = Rect(i * FDigitWidth, 0, FDigitWidth + i * FDigitWidth, Fbmp->Height);
    newdigit->Draw(dstrect, srcrect, Trans);
  }
  SetColor(newdigit, Color32(FOffColour));
  OffMask->Clear(clBlack32);

  for ( int i = 0; i < FColumns; i++ )
    OffMask->Draw( Rect( i * FDigitWidth, 0, FDigitWidth + i * FDigitWidth, Fbmp->Height),
                   dstrect, newdigit);
//  OffMask->SaveToFile("Offmask.bmp");
  newdigit->MasterAlpha = 255;
  for ( int i = 0; i < 64; i++ ) {
    targetrect = Rect( i * FDigitWidth, 0, FDigitWidth + i * FDigitWidth, Fbmp->Height);
    mask = 1;
    newdigit->Clear(0);
    for ( int j = 0; j < 16; j++ ) {
      srcrect = Rect(j * FDigitWidth, 0, FDigitWidth + j * FDigitWidth, Fbmp->Height);
      if ( chrset[i] & mask )
        newdigit->Draw(dstrect,srcrect,Trans);
      mask <<= 1;
    }
    Charmap->Draw(targetrect,dstrect,newdigit);
  }
  SetTrans(Charmap, clWhite32);
  SetColor(Charmap, Color32(FColour));
  SetColor(Trans,Color32(FColour));
  delete newdigit;
}
//---------------------------------------------------------------------------

void __fastcall TBFMAlpha::SetTrans(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I;

  Colour = Colour & 0x00FFFFFF;
  P = ptr->PixelPtr[0][0];
  for ( I = 0; I < ptr->Width * ptr->Height; I++, P++ ) {
    C = *P & 0x00FFFFFF;
    if ( C != Colour )
      *P = C;
  }
}
//---------------------------------------------------------------------------


