//---------------------------------------------------------------------------

#include <classes.hpp>
#pragma hdrstop


//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "grbitmap.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
//---------------------------------------------------------------------------
__fastcall TGRBitmap::TGRBitmap(int ANumber,int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
      : TGRBase(ANumber,"TGRBitmap",AX,AY,AHeight,AWidth,Adest)
{
  bmp = new TBitmap32();
  bmp->SetSize(AWidth,AHeight);
  Transparent = false;
  bmp->DrawMode = dmOpaque;
}
//---------------------------------------------------------------------------
__fastcall TGRBitmap::~TGRBitmap(void)
{
  delete bmp;
}
//---------------------------------------------------------------------------

void __fastcall TGRBitmap::Draw()
{
  bmp->DrawTo(Bitmap,Rect(0,0,Width,Height));
  TGRBase::Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRBitmap::SetBitmap(Graphics::TBitmap *ptr)
{
  bmp->Assign(ptr);
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TGRBitmap::SetTransparent(bool value)
{
  if ( value != Transparent ) {
    Transparent = value;
    Draw();
  }
}

//---------------------------------------------------------------------------


void __fastcall TGRBitmap::SaveToStream(TStream *ptr)
{
  bmp->SaveToStream(ptr);
//  off->SaveToStream(ptr);
}

