//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "grbase.h"
#include "grvideo.h"
#pragma link "GR32_Image"
#pragma package(smart_init)
//---------------------------------------------------------------------------

static Byte conversion[16] = {
  0, 97, 122, 143, 158, 171, 183, 194,
  204, 211, 219, 227, 234, 242, 250, 255
  };

static int colourmasks[16] = {
 0x00000000,
 0x000000ff,
 0x0000ff00,
 0x0000ffff,
 0x00ff0000,
 0x00ff00ff,
 0x00ffff00,
 0x00ffffff,
 0xff000000,
 0xff0000ff,
 0xff00ff00,
 0xff00ffff,
 0xffff0000,
 0xffff00ff,
 0xffffff00,
 0xffffffff
};

//---------------------------------------------------------------------------
__fastcall TGRVideo::TGRVideo(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
    : TGRBase(ANumber,"TGRVideo",AX,AY,AHeight,AWidth,Adest)
{
//  Bitmap->Clear();
  shadow = new TBitmap32();
  shadow->DrawMode = dmOpaque;
  shadow->SetSize(504,296);
//  shadow->Width = 504;
//  shadow->Height = 296;
  shadow->Clear();
  SetBounds(X,Y,504,296);
//  Height = 504;
//  Width = 296;
//  Bitmap->Height = 504;
//  Bitmap->Width = 296;
//  Bitmap->MasterAlpha = 0xff;
  colours[15] = clWhite32;
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRVideo::~TGRVideo(void)
{
  delete shadow;
}
//---------------------------------------------------------------------------

void __fastcall TGRVideo::Draw(void)
{
//  BeginUpdate();
  shadow->DrawTo(Bitmap);
  TGRBase::Draw();
//  TImage32::Paint();
}
//---------------------------------------------------------------------------
void __fastcall TGRVideo::WriteMemory(Byte *map)
{
  memory = map;
}
//---------------------------------------------------------------------------
void __fastcall TGRVideo::Dump()
{
  shadow->SaveToFile("dump.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TGRVideo::DrawRow(int row)
{
int x,y;
int pos = 64 * row;
Byte dbl;

  y = row * 8;
  dbl = (rowtable[pos++] & 0xc000) >> 12;
  for ( x = 0; x < 504; x+=8 )
    DisplayChar( x, y, rowtable[pos++], dbl);
  if ( row == 36 )
    Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRVideo::SetColour(int nbr, TColor colour)
{
  colours[nbr] = Color32(colour);
}
//---------------------------------------------------------------------------

TColor __fastcall TGRVideo::GetColour(int nbr)
{
  return WinColor(colours[nbr]);
}
//---------------------------------------------------------------------------

void __fastcall TGRVideo::WriteRowTable(Word *ptr)
{
  rowtable = ptr;
}
//---------------------------------------------------------------------------
void __fastcall TGRVideo::WritePalette(Byte *ptr)
{
  for ( int i = 0; i < 16; i++ ) {
    Byte green = conversion[ptr[i*2] >> 4];
    Byte red  = conversion[ptr[1+2*i] & 0xf];
    Byte blue = conversion[ptr[i*2] & 0xf];
    colours[i] = Color32(red, green, blue, 0xff);
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRVideo::DisplayChar(int x, int y, int pos, Byte dbl)
{
int i, j;
Byte value;
Byte value1;
Byte value2;
Byte value3;
Byte value4;
int mask2 = (pos & 0xf000) >> 12;
//int mask3 = colourmasks[mask2];
int pos2;

  pos &= 0xfff;
  pos *= 0x20;

  for ( int j = 0; j < 8; j++ ) {
    if ( dbl == 0 )
      pos2 = pos + j * 4;
    else if ( dbl == 0x8 )
      pos2 = pos + (4 * (j >> 1));
    else
      pos2 = pos + 16 + (4 * (j >> 1));
    if ( mask2 & 0x8 )
      value1 = memory[pos2];
    else
      value1 = 0;
    if ( mask2 & 0x4 )
      value2 = memory[pos2+1];
    else
      value2 = 0;
    if ( mask2 & 0x2 )
      value3 = memory[pos2+2];
    else
      value3 = 0;
    if ( mask2 & 0x1 )
      value4 = memory[pos2+3];
    else
      value4 = 0;
//    int value5 = *(int *)&memory[pos2];
//    value5 = value5 & mask3;
    Byte mask = 0x80;
//    int mask4 = 0x80808080;
    for ( i = 0; i < 8; i++) {
//      int value6 = value5 & mask4;
//      value = (value6 >> 31 ) | (value6 >> 23 ) | ( value6 >> 15) | (value6 >> 7);
      value = 0;
      if ( value1 & mask )
        value |= 8;
      if ( value2 & mask )
        value |= 4;
      if ( value3 & mask )
        value |= 2;
      if ( value4 & mask )
        value |= 1;
      PColor32 ptr = shadow->PixelPtr[x+i][y+j];
      *ptr = colours[value];
      mask >>= 1;
    }
  }
}
//---------------------------------------------------------------------------


