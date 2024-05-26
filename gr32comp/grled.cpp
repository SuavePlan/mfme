//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "grled.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
//---------------------------------------------------------------------------
__fastcall TGRLed::TGRLed(int ANumber,int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
      : TGRBase(ANumber,"TGRLed",AX,AY,AHeight,AWidth,Adest)
{
  on = new TBitmap32();
  on->SetSize(AWidth,AHeight);
  off = new TBitmap32();
  off->SetSize(AWidth,AHeight);
  FColour = Color32(clRed);
  on->DrawMode = dmBlend;
  off->DrawMode = dmBlend;
  FType = 0;
  FState = 0;
  Render();
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRLed::~TGRLed(void)
{
  delete on;
  delete off;
}
//---------------------------------------------------------------------------

void __fastcall TGRLed::Draw()
{
  if ( FState ) {
    on->DrawTo(Bitmap);
  } else {
    off->DrawTo(Bitmap);
  }
  TGRBase::Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRLed::Render()
{
TCanvas * canvas = new TCanvas();
int PW, PW1;

  PW = Width / 17;
  if ( PW < 1 )
    PW = 1;
  if ( PW > 6 )
    PW = 6;
  PW1 = PW / 2;
  if ( PW1 < 1 )
    PW1 = 1;
  on->Clear(clLime32);
  off->Clear(clLime32);
  canvas->Handle = on->Handle;
  canvas->Brush->Color = WinColor(FColour);
  for ( int i = 0; i < 2; i++ ) {
    canvas->Pen->Color = clBlack;
    switch ( FType ) {
      case 0:
        canvas->Ellipse(0,0,Width,Height);
        canvas->Pen->Width=PW1;
        canvas->Pen->Color=clGray;
        canvas->Arc(PW*4,PW*4,Width-(PW*4),Height-(PW*4),Width-(PW*4),Height-(PW*2),Width-(PW*4),Height-(PW*9));
        canvas->Pen->Color=clWhite;
        canvas->Arc(PW*4,PW*4,Width-(PW*4),Height-(PW*4),PW*8,0,PW*7,PW*9);
        canvas->Pen->Color=clSilver;
        canvas->Arc(PW*4,PW*4,Width-(PW*4),Height-(PW*4),PW*8,0,PW*7,PW*8);
      break;
    }
    canvas->Handle = off->Handle;
    canvas->Brush->Color = clMaroon;
  }
  DoAlpha(on);
  DoAlpha(off);
//  on->SaveToFile("ledon.bmp");
//  off->SaveToFile("ledoff.bmp");
  delete canvas;
}
//---------------------------------------------------------------------------

void __fastcall TGRLed::SetType(int type)
{
  if ( FType != type ) {
    FType = type;
    Render();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRLed::SetLamp(int nbr, int value)
{
  if ( FState != value ) {
    FState = value;
    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRLed::SetColour(TColor Colour)
{
  if ( Colour != WinColor(FColour) ) {
    FColour = Color32(Colour);
    Render();
    Draw();
  }
}

//---------------------------------------------------------------------------

/*void __fastcall TGRLed::DoAlpha(TBitmap32 *ptr)
{
PColor32 P;
TColor32 C;
int I, J;
TColor32 Colour;

  ptr->ResetAlpha();
  Colour = *ptr->PixelPtr[0][0] & 0x00ffffff;

  for ( I = 0; I < Bitmap->Height; I++ ) {
    for ( J = 0; J < Bitmap->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour ) {
        *P = C;
      }
    }
  }
}
//---------------------------------------------------------------------------*/

void __fastcall TGRLed::SaveToStream(TStream *ptr)
{
  on->SaveToStream(ptr);
  off->SaveToStream(ptr);
//  off->SaveToStream(ptr);
}

