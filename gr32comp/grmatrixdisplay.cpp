//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "grbase.h"
#include "grmatrixdisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TGRDotMatrix::TGRDotMatrix(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
    : TGRBase(ANumber,"TGRDotMatrix",AX,AY,AHeight,AWidth,Adest)
{
  FSize = 7;
  Src = new TBitmap32();
  memset( map, 0, sizeof(map));
  memset( newmap, 255, sizeof(map));
  Init();
//  ScaleMode = Gr32_image::smStretch;
//  TImage32::Bitmap->StretchFilter = sfNearest;
  Src->Clear();
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRDotMatrix::~TGRDotMatrix()
{
  delete Src;
}
//---------------------------------------------------------------------------
void __fastcall TGRDotMatrix::SetPixel(int nbr, int state)
{
//  if ( state )
    map[nbr] = ~state;
//  else
//    map[nbr] = 0;
  Render();
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRDotMatrix::SetMap(Byte *ptr)
{
  memcpy(newmap, ptr, 9*21);
  Render();
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRDotMatrix::SetSize(int ASize)
{
  if ( ASize != FSize ) {
    FSize = ASize;
    memset( map, 0, sizeof(map));
    Init();
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDotMatrix::Render(void)
{
int x, y;
int xpos, ypos;
//int pos;
int mask = 0x80;
Byte *ptr = map;
Byte *nptr = newmap;
bool updated = false;

  xpos = ypos = 2;
  for ( y = 0; y < 21; y++ ) {
    for ( x = 0; x < 65; x++ ) {
      if ( (*ptr ^ *nptr) & mask ) {
        updated = true;
        if ( *nptr & mask )
          Src->FillRect( xpos, ypos, xpos + FSize - Spacing, ypos + FSize - Spacing, clRed32);
        else
          Src->FillRect( xpos, ypos, xpos + FSize - Spacing, ypos + FSize - Spacing, clBlack32);
      }
      mask >>= 1;
      if ( mask == 0 ) {
        mask = 0x80;
        if ( updated ) {
          *ptr = *nptr;
          updated = false;
        }
        ptr++;
        nptr++;
      }
      xpos += FSize;
    }
    mask = 0x80;
    if ( updated ) {
      *ptr = *nptr;
      updated = false;
    }
    ptr++;
    nptr++;
    xpos = 2;
    ypos += FSize;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRDotMatrix::Draw(void)
{
  Src->DrawTo(Bitmap,Rect(0,0,Width,Height),Rect(0,0,Src->Width,Src->Height));
  TGRBase::Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRDotMatrix::Init(void)
{
  if ( FSize > 4 )
    Spacing = 2;
  else
    Spacing = 1;
//  Width = 65 * FSize + 2;
//  Height = 21 * FSize + 2;
  Src->SetSize(65 * FSize + 2,21 * FSize + 2);
  SetBounds(X,Y,Width,Height);
  Src->Clear();
}
