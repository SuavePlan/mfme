//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "grBase.h"
#include "grbfmleddisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TGRBFMLED::TGRBFMLED(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
    : TGRBase(ANumber,"TGRBFMLED",AX,AY,AHeight,AWidth,Adest)
{
  FSizeX = 4;
  FSizeY = 4;
  FDigitSpacing = 4;
  FColour = clRed;
  Src = new TBitmap32();
//  Src->SetSize(AWidth,AHeight);
//  Src->DrawMode = dmBlend;
  Init();
//  ScaleMode = Gr32_image::smStretch;
//  TImage32::Bitmap->StretchFilter = sfNearest;
  for ( int i = 0; i < 20; i++ )
    map[i] = 0;
  Src->Clear(clBlack32);
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRBFMLED::~TGRBFMLED()
{
  delete Src;
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::SetDot(int nbr, int value)
{
  if ( nbr < 20 ) {
    map[nbr] = value;
    DrawColumn(nbr);
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::SetSizeX(int ASize)
{
  if ( ASize != FSizeX ) {
    FSizeX = ASize;
    Init();
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::SetSizeY(int ASize)
{
  if ( ASize != FSizeY ) {
    FSizeY = ASize;
    Init();
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::SetSpacing(int ASpacing)
{
  if ( ASpacing != FDigitSpacing ) {
    FDigitSpacing = ASpacing;
    Init();
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::SetColour(TColor AColour)
{
  if ( AColour != FColour ) {
    FColour = AColour;
//    Init();
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::Render(void)
{
int i;

  Src->Clear(0);
  for ( i = 0; i < 20; i++ )
    DrawColumn(i);
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::DrawColumn(int i)
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
void __fastcall TGRBFMLED::Draw(void)
{
//  BeginUpdate();
//  Src->Clear(0);
  Src->DrawTo(Bitmap);
  TGRBase::Draw();
//  EndUpdate();
//  TImage32::Paint();
}
//---------------------------------------------------------------------------
void __fastcall TGRBFMLED::Init(void)
{
//  if ( FSize > 4 )
//    Spacing = 2;
//  else
//    Spacing = 1;
  Width = 4 * ( ((FSizeX + 1) * 5) + FDigitSpacing) + 3;
  Height = (8 * (FSizeY + 1)) + 6;
  SetBounds(X,Y,Width,Height);
  Src->SetSize(Width,Height);
//  Src->Height = Height;
//  Bitmap->Width = Width;
//  Bitmap->Height = Height;
//  Bitmap->Clear(0);
}
//---------------------------------------------------------------------------

