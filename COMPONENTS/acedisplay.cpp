//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "acedisplay.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TACELED *)
{
    new TACELED(NULL);
}
//---------------------------------------------------------------------------
__fastcall TACELED::TACELED(TComponent* Owner)
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
__fastcall TACELED::~TACELED()
{
  delete Src;
}
//---------------------------------------------------------------------------
void __fastcall TACELED::Loaded(void)
{
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TACELED::SetPixel(int nbr, bool state)
{
  if ( state )
    map[nbr] = 1;
  else
    map[nbr] = 0;
  Draw();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TACELED::SetMap(Byte *ptr)
{
  memcpy(newmap, ptr, 192);
  Draw();
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TACELED::SetSize(int ASize)
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
void __fastcall TACELED::SetRotated(bool ARotated)
{
  if ( ARotated != FRotated ) {
    FRotated = ARotated;
    Draw();
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TACELED::Draw(void)
{
int x, y;
int xpos, ypos;
//int pos;
int mask = 0x01;
Byte *ptr = map;
Byte *nptr = newmap;
bool updated = false;
int ydiff;

  if ( FRotated ) {
    ydiff = -FSize;
    ypos = 2 + FSize * 20;
  } else {
    ydiff = FSize;
    ypos = 2;
  }
  xpos = 2;
  for ( y = 0; y < 21; y++ ) {
    for ( x = 0; x < 60; x++ ) {
      if ( (*ptr ^ *nptr) & mask ) {
        updated = true;
        if ( *nptr & mask )
          Src->FillRect( xpos, ypos, xpos + FSize - Spacing, ypos + FSize - Spacing, clRed32);
        else
          Src->FillRect( xpos, ypos, xpos + FSize - Spacing, ypos + FSize - Spacing, clBlack32);
      }
      mask <<= 1;
      if ( mask == 0x100 ) {
        mask = 0x1;
        if ( updated ) {
          *ptr = *nptr;
          updated = false;
        }
        ptr++;
        nptr++;
      }
      xpos += FSize;
    }
    mask = 0x1;
    if ( updated ) {
      *ptr = *nptr;
      updated = false;
    }
    ptr++;
    nptr++;
    xpos = 2;
    ypos += ydiff;
  }
}
//---------------------------------------------------------------------------
void __fastcall TACELED::Paint(void)
{
  BeginUpdate();
  TImage32::Bitmap->Draw(0,0,Src);
  EndUpdate();
  TImage32::Paint();
}
//---------------------------------------------------------------------------
void __fastcall TACELED::Init(void)
{
  if ( FSize > 4 )
    Spacing = 2;
  else
    Spacing = 1;
  Width = 60 * FSize + 2;
  Height = 21 * FSize + 2;
  Src->Width = Width;
  Src->Height = Height;
  Bitmap->Width = Width;
  Bitmap->Height = Height;
  Src->Clear();
}
