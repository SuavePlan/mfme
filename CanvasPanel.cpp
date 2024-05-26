//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CanvasPanel.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TCanvasPanel *)
{
    new TCanvasPanel(NULL);
}
//---------------------------------------------------------------------------
__fastcall TCanvasPanel::TCanvasPanel(TComponent* Owner)
    : TPanel(Owner)
{
  FBackground = new Graphics::TBitmap();
//  FFlicker = false;
//  ControlStyle << csOpaque;
}
//---------------------------------------------------------------------------
__fastcall TCanvasPanel::~TCanvasPanel()
{
  delete FBackground;
}
//---------------------------------------------------------------------------
void __fastcall TCanvasPanel::LoadBackground(Graphics::TBitmap *ptr)
{
  FBackground->Assign(ptr);
  Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TCanvasPanel::SetFlicker(bool AFlicker)
{
  if ( AFlicker != FFlicker )
    FFlicker = AFlicker;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TCanvasPanel::GetBackground(void)
{
  return FBackground;
}
//---------------------------------------------------------------------------
void __fastcall TCanvasPanel::Paint(void)
{
  TPanel::Paint();
  Canvas->CopyMode = cmSrcCopy;
  if ( !Background->Empty )
    Canvas->CopyRect(Canvas->ClipRect,FBackground->Canvas,Canvas->ClipRect);
//  if (FOnPaint)
//    FOnPaint(this);
}
//---------------------------------------------------------------------------
namespace Canvaspanel
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TCanvasPanel)};
        RegisterComponents("CJW", classes, 0);
    }
}
//---------------------------------------------------------------------------
