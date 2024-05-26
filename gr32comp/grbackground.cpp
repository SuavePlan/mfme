//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "grbackground.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
//---------------------------------------------------------------------------
__fastcall TGRBackground::TGRBackground(int ANumber,int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
      : TGRBase(ANumber,"TGRBackground",AX,AY,AHeight,AWidth,Adest)
{
  background = new TBitmap32();
  background->SetSize(AWidth,AHeight);
  background->Clear(Color32(clBtnFace));
  background->DrawMode = dmBlend;
  image->Bitmap->SetSize(AWidth,AHeight);
  FDarkness = 0xff;
  Colour = clBtnFace;
  Moveable = false;
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRBackground::~TGRBackground(void)
{
  delete background;
}
//---------------------------------------------------------------------------

void __fastcall TGRBackground::Draw()
{
  background->DrawTo(Bitmap);
  TGRBase::Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRBackground::SetBitmap(Graphics::TBitmap *ptr)
{
  if ( ptr->Width && ptr->Height ) {
    TBitmap32 * newptr = new TBitmap32();
    newptr->Assign(ptr);
    TBitmap32 * newptr2 = new TBitmap32();
    if ( Width > ptr->Width || Height > ptr->Height ) {
//      image->Bitmap->SetSize(ptr->Width, ptr->Height);
      SetBounds(0,0,ptr->Width,ptr->Height);
    }
    newptr2->SetSize(Width,Height);
    newptr2->Draw(Rect(0,0,Width,Height),Rect(0,0,Width,Height),newptr);
    background->Assign(newptr2);
    delete newptr;
    delete newptr2;
    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRBackground::SetColour(TColor colour)
{
  if ( Colour != colour ) {
    Colour = colour;
    background->Clear(Color32(Colour));
    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRBackground::SetDarkness(int FValue)
{
  if ( FValue != FDarkness ) {
    FDarkness = FValue;
    background->MasterAlpha = FDarkness;
    Draw();
  }
}

//---------------------------------------------------------------------------

void __fastcall TGRBackground::SaveToStream(TStream *ptr)
{
  background->SaveToStream(ptr);
//  off->SaveToStream(ptr);
}

