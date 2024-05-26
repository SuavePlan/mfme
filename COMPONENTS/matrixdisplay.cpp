//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "matrixdisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TDotmatrix *)
{
    new TDotmatrix(NULL);
}
//---------------------------------------------------------------------------
__fastcall TDotmatrix::TDotmatrix(TComponent* Owner)
    : TImage32(Owner)
{
  FSize = 7;
  Src = new TBitmap32();
  memset( map, 0, sizeof(map));
  Init();
  ScaleMode = Gr32_image::smStretch;
  TImage32::Bitmap->StretchFilter = sfNearest;
}
//---------------------------------------------------------------------------
__fastcall TDotmatrix::~TDotmatrix()
{
  delete Src;
}
//---------------------------------------------------------------------------
void __fastcall TDotmatrix::Loaded(void)
{
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TDotmatrix::SetPixel(int nbr, bool state)
{
  if ( state )
    map[nbr] = 1;
  else
    map[nbr] = 0;
  Draw();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TDotmatrix::SetMap(Byte *ptr)
{
  memcpy(newmap, ptr, 9*21);
  Draw();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TDotmatrix::SetSize(int ASize)
{
  if ( ASize != FSize ) {
    FSize = ASize;
    memset( map, 0, sizeof(map));
    Init();
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TDotmatrix::Draw(void)
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
void __fastcall TDotmatrix::Paint(void)
{
  BeginUpdate();
  TImage32::Bitmap->Draw(0,0,Src);
  EndUpdate();
  TImage32::Paint();
}
//---------------------------------------------------------------------------
void __fastcall TDotmatrix::Init(void)
{
  if ( FSize > 4 )
    Spacing = 2;
  else
    Spacing = 1;
  Width = 65 * FSize + 2;
  Height = 21 * FSize + 2;
  Src->Width = Width;
  Src->Height = Height;
  Bitmap->Width = Width;
  Bitmap->Height = Height;
  Src->Clear();
}
