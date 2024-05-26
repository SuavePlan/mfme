//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "grlabel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
//---------------------------------------------------------------------------
__fastcall TGRLabel::TGRLabel(int ANumber,int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
      : TGRBase(ANumber,"TGRLabel",AX,AY,AHeight,AWidth,Adest)
{
  buffer = new TBitmap32();
  buffer->SetSize(AWidth,AHeight);
  FColour = Color32(clBtnFace);
  FTransparent = true;
  buffer->DrawMode = dmBlend;
}
//---------------------------------------------------------------------------
__fastcall TGRLabel::~TGRLabel(void)
{
  delete buffer;
}
//---------------------------------------------------------------------------

void __fastcall TGRLabel::Draw()
{
TRect rect = Rect(0,0,Width,Height);;
int X;
Graphics::TBitmap * ptr = new Graphics::TBitmap();

  ptr->Width = Width;
  ptr->Height = Height;
  ptr->Canvas->Brush->Color = WinColor(FColour);
  ptr->Canvas->FillRect(rect);
  ptr->Canvas->Font = buffer->Font;

  X = DrawText( ptr->Canvas->Handle, text.c_str(), -1, &rect, DT_CALCRECT | DT_WORDBREAK);
  rect.Left = 0;
  rect.Right = Width;
  rect.Top = ( Height - X ) / 2;
  rect.Bottom = ( Height + X ) / 2;
  DrawText( ptr->Canvas->Handle, text.c_str(), -1, &rect, DT_CENTER | DT_WORDBREAK);
  buffer->Assign(ptr);
  delete ptr;
  if ( FTransparent)
    DoAlpha(buffer);
  buffer->DrawTo(Bitmap);
  TGRBase::Draw();
}
//---------------------------------------------------------------------------

/*void __fastcall TGRLabel::DoAlpha(TBitmap32 *ptr)
{
PColor32 P;
TColor32 C;
int I, J;
TColor32 Colour;

  Colour = *ptr->PixelPtr[0][0] & 0x00ffffff;

  for ( I = 0; I < ptr->Height; I++ ) {
    for ( J = 0; J < ptr->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour ) {
        *P = C;
      }
    }
  }
}
//--------------------------------------------------------------------------- */
void __fastcall TGRLabel::SetText(String txt)
{
  text = txt;
  Draw();
}

//---------------------------------------------------------------------------

void __fastcall TGRLabel::SetColour(TColor Colour)
{
  if ( Colour != WinColor(FColour) ) {
    FColour = Color32(Colour);
    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRLabel::SetTransparent(bool value)
{
  if ( value != FTransparent ) {
    FTransparent = value;
    if ( value )
      buffer->DrawMode = dmOpaque;
    else
      buffer->DrawMode = dmBlend;
    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRLabel::SetFont(TFont *font)
{
  buffer->Font = font;
  Draw();
}

//---------------------------------------------------------------------------


void __fastcall TGRLabel::SaveToStream(TStream *ptr)
{
  buffer->SaveToStream(ptr);
//  off->SaveToStream(ptr);
}

