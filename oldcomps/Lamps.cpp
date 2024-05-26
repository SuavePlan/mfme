//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Lamps.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TLamp *)
{
    new TLamp(NULL);
}
//---------------------------------------------------------------------------
__fastcall TLamp::TLamp(TComponent* Owner)
    : TShape(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLamp::CMFontChanged(TMessage &Message)
{
  Invalidate();
//  TShape::CMFontChanged(Message);
}
//---------------------------------------------------------------------------
void __fastcall TLamp::SetCaption(String Caption)
{
  if ( Caption != FCaption ) {
    FCaption = Caption;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TLamp::SetColour1(TColor Color)
{
  if ( Color != FColour1 ) {
    FColour1 = Color;
  }
}
//---------------------------------------------------------------------------
void __fastcall TLamp::SetColour2(TColor Color)
{
  if ( Color != FColour2 ) {
    FColour2 = Color;
  }
}
//---------------------------------------------------------------------------
void __fastcall TLamp::Paint(void)
{
tagRECT Rect;
int X;

   TShape::Paint();

   Rect = ClientRect;
   Canvas->Font = Font;
   X = DrawText( Canvas->Handle, FCaption.c_str(), -1, &Rect, DT_CALCRECT | DT_WORDBREAK);
   Rect.top = ( Height - X ) / 2;
   Rect.bottom = ( Height + X ) / 2;
   Rect.left = 0;
   Rect.right = Width;
   DrawText( Canvas->Handle, FCaption.c_str(), -1, &Rect, DT_CENTER | DT_WORDBREAK);

}
//---------------------------------------------------------------------------
namespace Lamps
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TLamp)};
        RegisterComponents("CJW", classes, 0);
    }
}
//---------------------------------------------------------------------------
 