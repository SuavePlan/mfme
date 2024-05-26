//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "bfmleddisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TBFMLED *)
{
    new TBFMLED(NULL);
}
//---------------------------------------------------------------------------
__fastcall TBFMLED::TBFMLED(TComponent* Owner)
    : TImage32(Owner)
{
  FSizeX = 4;
  FSizeY = 4;
  FDigitSpacing = 4;
  FColour = clRed;
  Src = new TBitmap32();
//  Src->DrawMode = dmBlend;
  Init();
  ScaleMode = Gr32_image::smStretch;
  TImage32::Bitmap->StretchFilter = sfNearest;
  for ( int i = 0; i < 20; i++ )
    map[i] = 0;
}
//---------------------------------------------------------------------------
__fastcall TBFMLED::~TBFMLED()
{
  delete Src;
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::Loaded(void)
{
  Init();
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::SetDot(int nbr, int value)
{
  if ( nbr < 20 ) {
    map[nbr] = value;
    DrawColumn(nbr);
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::SetSizeX(int ASize)
{
  if ( ASize != FSizeX ) {
    FSizeX = ASize;
    Init();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::SetSizeY(int ASize)
{
  if ( ASize != FSizeY ) {
    FSizeY = ASize;
    Init();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::SetSpacing(int ASpacing)
{
  if ( ASpacing != FDigitSpacing ) {
    FDigitSpacing = ASpacing;
    Init();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::SetColour(TColor AColour)
{
  if ( AColour != FColour ) {
    FColour = AColour;
//    Init();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::Draw(void)
{
int i;

  Src->Clear(0);
  for ( i = 0; i < 20; i++ )
    DrawColumn(i);
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::DrawColumn(int i)
{
int j;
int xpos, ypos;
int spacing;
int bits = 7;

  spacing = FDigitSpacing * ( i / 5 );
  xpos = 3 + spacing + (FSizeX + 1) * i;
  ypos = 4;
  if ( (i % 5) == 4 && i != 19 )
    bits = 8;
  for ( j = 0; j < bits; j++ ) {
    if ( j == 7 )
      xpos += (FSizeX + 1);
    if ( map[i] & ( 1<<j ) )
      Src->FillRect( xpos, ypos, xpos + FSizeX, ypos + FSizeY, Color32(FColour));
    else
      Src->FillRect( xpos, ypos, xpos + FSizeX, ypos + FSizeY, 0);
    ypos += ( FSizeY + 1 );
  }
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::Paint(void)
{
  BeginUpdate();
//  TImage32::Bitmap->Clear(0);
  TImage32::Bitmap->Draw(0,0,Src);
  EndUpdate();
  TImage32::Paint();
}
//---------------------------------------------------------------------------
void __fastcall TBFMLED::Init(void)
{
//  if ( FSize > 4 )
//    Spacing = 2;
//  else
//    Spacing = 1;
  Width = 4 * ( ((FSizeX + 1) * 5) + FDigitSpacing) + 3;
  Height = (8 * (FSizeY + 1)) + 6;
  Src->Width = Width;
  Src->Height = Height;
  Bitmap->Width = Width;
  Bitmap->Height = Height;
  Bitmap->Clear(0);
}
//---------------------------------------------------------------------------

