//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "grbutton.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
//---------------------------------------------------------------------------
__fastcall TGRButton::TGRButton(int ANumber,int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
      : TGRBase(ANumber,"TGRButton",AX,AY,AHeight,AWidth,Adest)
{
  bitmap = new TBitmap32();
  bitmap->SetSize(AWidth,AHeight);
  bitmap->Clear(Color32(clBtnFace));
  on = new TBitmap32();
  on->SetSize(AWidth,AHeight);
  on->Clear(Color32(clBtnFace));
  on->DrawMode=dmBlend;
  off = new TBitmap32();
  off->SetSize(AWidth,AHeight);
  off->Clear(clYellow32);
  off->DrawMode=dmBlend;
//  State = 0;
  Down = false;
  Graphic = false;
  OnColour = clBtnFace;
  OffColour = clBtnFace;
  Draw();
  Bitmap->DrawMode = dmOpaque;
}
//---------------------------------------------------------------------------
__fastcall TGRButton::~TGRButton(void)
{
  delete bitmap;
  delete on;
  delete off;
}
//---------------------------------------------------------------------------

void __fastcall TGRButton::Draw()
{
TCanvas * ptr = new TCanvas();
TRect rect, rect2;
TBitmap32 * which;

  ptr->Handle = bitmap->Handle;
  if ( State ) {
    which = on;
  } else {
    which = off;
  }

  ptr->Pen->Width = 1;
  ptr->Pen->Style = psSolid;

  if ( !Down ) {
    ptr->Pen->Color = clWhite;
    ptr->MoveTo(0,Height-1);
    ptr->LineTo(0,0);
    ptr->LineTo(Width-1,0);
    ptr->Pen->Color = clBlack;
    ptr->MoveTo(0,Height-1);
    ptr->LineTo(Width,Height-1);
    ptr->MoveTo(Width-1,0);
    ptr->LineTo(Width-1,Height-1);
    ptr->Pen->Color = clGray;
    ptr->MoveTo(1,Height-2);
    ptr->LineTo(Width-2,Height-2);
    ptr->LineTo(Width-2,0);
    rect2 = Rect(1,1,Width-2,Height-2);
  } else {
    ptr->Pen->Color = clBlack;
    ptr->MoveTo(0,Height-1);
    ptr->LineTo(0,0);
    ptr->LineTo(Width,0);
    ptr->Pen->Color = clWhite;
    ptr->MoveTo(1,Height-1);
    ptr->LineTo(Width,Height-1);
    ptr->MoveTo(Width-1,1);
    ptr->LineTo(Width-1,Height-1);
    ptr->Pen->Color = clGray;
    ptr->MoveTo(1,Height-2);
    ptr->LineTo(1,1);
    ptr->LineTo(Width-1,1);
    rect2 = Rect(2,2,Width-1,Height-1);
  }

  if ( !Graphic ) {
    if ( State )
      ptr->Brush->Color = OnColour;
    else
      ptr->Brush->Color = OffColour;

//  ptr->Brush->Style = bsSolid;
    ptr->FillRect(rect2);

    ptr->Font = on->Font;
    ptr->Brush->Style = bsClear;
    rect = Rect(0,0,Width,Height);
    int X = DrawText( ptr->Handle, Text.c_str(), -1, &rect, DT_CALCRECT | DT_WORDBREAK);
    rect.Left = 0;
    rect.Right = Width;
    rect.Top = ( Height - X ) / 2;
    rect.Bottom = ( Height + X ) / 2;
    if ( Down ) {
      rect.Left += 1;
      rect.Top += 1;
      rect.Right += 1;
      rect.Bottom += 1;
    }
    DrawText( ptr->Handle, Text.c_str(), -1, &rect, DT_CENTER | DT_WORDBREAK);
  } else {
    if ( State )
      on->DrawTo(bitmap, rect2);
    else
      off->DrawTo(bitmap, rect2);
  }
  delete ptr;

//  on->SaveToFile("button.bmp");
//  on->ResetAlpha();
//  image->BeginUpdate();
  bitmap->DrawTo(Bitmap);
//  image->EndUpdate();
//  if ( Flush )
//    image->Flush(Dstrect);
//  bitmap->DrawTo(Bitmap);
//  Bitmap->SaveToFile("bitmap.bmp");
  TGRBase::Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRButton::SetGraphic(bool Value)
{
  if ( Graphic != Value ) {
    Graphic = Value;
    Draw();
  }
}

//---------------------------------------------------------------------------
void __fastcall TGRButton::SetOn(Graphics::TBitmap *ptr)
{
  on->Assign(ptr);
  if ( ptr->Height && ptr->Width ) {
    DoAlpha(on);
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRButton::SetOff(Graphics::TBitmap *ptr)
{
  off->Assign(ptr);
  if ( ptr->Height && ptr->Width ) {
    DoAlpha(off);
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRButton::SetOnColour(TColor colour)
{
  OnColour = colour;
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TGRButton::SetOffColour(TColor colour)
{
  OffColour = colour;
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TGRButton::SetDown(bool value)
{
  Down = value;
  Draw();
}

//---------------------------------------------------------------------------

void __fastcall TGRButton::SetFont(TFont *font)
{
  on->Font = font;
}

//---------------------------------------------------------------------------
void __fastcall TGRButton::SetText(String text)
{
  Text = text;
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRButton::SetLamps(int state)
{
  if ( State != state ) {
    State = state;
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRButton::SetLamp(int nbr, int state)
{
  if ( State != state ) {
    State = state;
    Draw();
  }
}
//---------------------------------------------------------------------------

/*void __fastcall TGRButton::DoAlpha(TBitmap32 *ptr)
{
PColor32 P;
TColor32 C;
int I, J;
TColor32 Colour;

  ptr->ResetAlpha();
  Colour = *ptr->PixelPtr[0][ptr->Height-1] & 0x00ffffff;

  for ( I = 0; I < ptr->Height; I++ ) {
    for ( J = 0; J < ptr->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour ) {
        *P = C;
      }
    }
  }
}   */

void __fastcall TGRButton::SaveToStream(TStream *ptr)
{
  on->SaveToStream(ptr);
  off->SaveToStream(ptr);
}

