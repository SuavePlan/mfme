//---------------------------------------------------------------------------

#include <classes.hpp>
#pragma hdrstop


//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "grfancylamp.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
//---------------------------------------------------------------------------
__fastcall TGRFancyLamp::TGRFancyLamp(int ANumber,int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
      : TGRBase(ANumber,"TGRFancyLamp",AX,AY,AHeight,AWidth,Adest)
{
  overlay = new TBitmap32();
  overlay->SetSize(AWidth,AHeight);
  overlay->DrawMode=dmBlend;
  on1 = new TBitmap32();
  on1->SetSize(AWidth,AHeight);
  on1->Clear(clRed32);
  on1->DrawMode=dmBlend;
  on2 = new TBitmap32();
  on2->SetSize(AWidth,AHeight);
  on2->Clear(clBlue32);
  on2->DrawMode=dmBlend;
  off = new TBitmap32();
  off->SetSize(AWidth,AHeight);
  off->Clear(clYellow32);
  off->DrawMode=dmBlend;
  on1image = new TBitmap32();
  on1image->SetSize(AWidth,AHeight);
  on1image->Clear();
  on2image = new TBitmap32();
  on2image->SetSize(AWidth,AHeight);
  on2image->Clear();
  offimage = new TBitmap32();
  offimage->SetSize(AWidth,AHeight);
  offimage->Clear();
  On1Colour = clYellow;
  On2Colour = clYellow;
  OffColour = clBtnFace;
  Graphic = false;
  Outline = true;
//  FState = 0;
  Shape = 0;
  Down = false;
  Transparent = false;
}
//---------------------------------------------------------------------------
__fastcall TGRFancyLamp::~TGRFancyLamp(void)
{
  delete overlay;
  delete on1;
  delete on2;
  delete off;
  delete on1image;
  delete on2image;
  delete offimage;
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::Draw()
{
int offt = 0;

  if ( Down && HasButton )
    offt = 1;

//  image->BeginUpdate();
  switch ( State ) {
    case 1:
      on1->DrawTo(Bitmap,0,offt);
//      on1->SaveToFile("on1.bmp");
      break;
    case 0:
      off->DrawTo(Bitmap,0,offt);
//      off->SaveToFile("off.bmp");
      break;
    case 2:
      on2->DrawTo(Bitmap,0,offt);
      break;
  }
//  image->EndUpdate();
//  if ( Flush )
//    image->Flush(Dstrect);
  TGRBase::Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SetOn1(Graphics::TBitmap *ptr)
{
  if ( ptr->Width && ptr->Height ) {
    on1image->Assign(ptr);
    if ( !Transparent )
      on1image->ResetAlpha();
    else
      DoAlpha(on1image,*on1image->PixelPtr[0][on1image->Height-1]);
    Render(1, on1);
//    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SetOn2(Graphics::TBitmap *ptr)
{
  if ( ptr->Width && ptr->Height ) {
    on2image->Assign(ptr);
    if ( !Transparent )
      on2image->ResetAlpha();
    else
      DoAlpha(on2image,*on2image->PixelPtr[0][on2image->Height-1]);
    Render(2, on2);
//    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SetOff(Graphics::TBitmap *ptr)
{
  if ( ptr->Width && ptr->Height ) {
    offimage->Assign(ptr);
    if ( !Transparent )
      offimage->ResetAlpha();
    else
      DoAlpha(offimage,*offimage->PixelPtr[0][offimage->Height-1]);
    Render(0, off);
//    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SetTransparent(bool value)
{
  if ( Transparent != value ) {
    Transparent = value;
//    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetDown(bool value)
{
  Down = value;
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetGraphic(bool value)
{
  Graphic = value;
  Render(0, off);
  Render(1, on1);
  Render(2, on2);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetOutline(bool value)
{
  Outline = value;
  Render( 0, off);
  Render( 1, on1);
  Render( 2, on2);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetOn1Colour(TColor value)
{
  On1Colour = value;
  Render( 1, on1);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetOn2Colour(TColor value)
{
  On2Colour = value;
  Render( 2, on2);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetOffColour(TColor value)
{
  OffColour = value;
  Render( 0, off);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetText(String value)
{
  Text = value;
  Render( 0, off);
  Render( 1, on1);
  Render( 2, on2);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetFont(TFont * value)
{
  Font = value;
  Render( 0, off);
  Render( 1, on1);
  Render( 2, on2);
//  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyLamp::SetLamps(int state)
{
  if ( State != state ) {
    State = state;
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SetShape(int shape)
{
  if ( Shape != shape ) {
    Shape = shape;
    Render( 0, off);
    Render( 1, on1);
    Render( 2, on2);
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SetLamp(int nbr, int state)
{
  if ( state )
    State |= nbr;
  else
    State &= ~nbr;
  Draw();
}
//---------------------------------------------------------------------------

/*void __fastcall TGRFancyLamp::DoAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I, J;
//TColor32 Colour;

//  ptr->ResetAlpha();
  Colour = Colour & 0x00ffffff;

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
//---------------------------------------------------------------------------
*/
void __fastcall TGRFancyLamp::Render(int State, TBitmap32 * bmp)
{
TCanvas * ptr;
TRect rect;
TColor colour = (TColor)0xfdfdfd;

  overlay->Clear(0xfffefefe);

  if ( !Graphic ) {
    bmp->Clear(0xfffefefe);
    switch ( State ) {
      case 1:
        colour = On1Colour;
        break;
      case 0:
        colour = OffColour;
        break;
      case 2:
        colour = On2Colour;
        break;
    }
  } else {
    switch ( State ) {
      case 2:
        on2image->DrawTo(bmp,Rect(0,0,Width,Height));
        break;
      case 1:
        on1image->DrawTo(bmp,Rect(0,0,Width,Height));
        break;
      case 0:
        offimage->DrawTo(bmp,Rect(0,0,Width,Height));
        break;
    }
  }
  ptr = new TCanvas();
  ptr->Handle = overlay->Handle;
  ptr->Pen->Color = clBlack;
  ptr->Brush->Color = colour;
  if ( Outline )
    ptr->Pen->Style = psSolid;
  else
    ptr->Pen->Style = psClear;
//  if ( !Graphic )
    ptr->Brush->Style = bsSolid;
//  else
//    ptr->Brush->Style = bsClear;
  switch ( Shape ) {
    case 0: // Rectangle square
    case 1: // Square
      ptr->Rectangle(Rect(0,0,Width,Height));
      break;
    case 2: // Rectangle Round
    case 3: // Square Round
      ptr->RoundRect(0,0,Width,Height,5,5);
      break;
    case 4: // Ellipse
    case 5: // Circle
      ptr->Ellipse(0,0,Width,Height);
      break;
  }
  if ( Text != "" ) {
    ptr->Pen->Style = psSolid;
    ptr->Font = Font;
    ptr->Brush->Style = bsClear;
    rect = Rect(0,0,Width,Height);
    int x = DrawText( ptr->Handle, Text.c_str(), -1, &rect, DT_CALCRECT | DT_WORDBREAK);
    rect.Left = 0;
    rect.Right = Width;
    rect.Top = ( Height - x ) / 2;
    rect.Bottom = ( Height + x ) / 2;
    DrawText( ptr->Handle, Text.c_str(), -1, &rect, DT_CENTER | DT_WORDBREAK);
  }
  overlay->ResetAlpha();
  if ( Graphic )
    DoAlpha(overlay,Color32(colour));
  overlay->DrawTo(bmp);
  DoAlpha(bmp,0xfffefefe,false);
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyLamp::SaveToStream(TStream *ptr)
{
  on1->SaveToStream(ptr);
  on2->SaveToStream(ptr);
  off->SaveToStream(ptr);
}

