//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Lamp2.h"
#pragma link "Gpshape"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TLamp2 *)
{
    new TLamp2(NULL);
}
//---------------------------------------------------------------------------
__fastcall TLamp2::TLamp2(TComponent* Owner)
    : TManyShape(Owner)
{
}
void __fastcall TLamp2::CMFontChanged(TMessage &Message)
{
  Invalidate();
//  TShape::CMFontChanged(Message);
}
//---------------------------------------------------------------------------
void __fastcall TLamp2::SetCaption(String Caption)
{
  if ( Caption != FCaption ) {
    FCaption = Caption;
    Invalidate();
  }
}
//---------------------------------------------------------------------------
void __fastcall TLamp2::Paint(void)
{
tagRECT Rect;
int X;

   TManyShape::Paint();
   if ( FCaption != "" ) {
     Rect = ClientRect;
     Canvas->Font = Font;
     X = DrawText( Canvas->Handle, FCaption.c_str(), -1, &Rect, DT_CALCRECT | DT_WORDBREAK);
     switch ( Shape ) {
       case gstTriangleUp:
         Rect.top = ( (Height * 1.25) - X ) / 2;
         Rect.bottom = ( (Height * 1.25) + X ) / 2;
         Rect.left = 0;
         Rect.right = Width;
         break;
       case gstTriangleDown:
         Rect.top = ( (Height * 0.75) - X ) / 2;
         Rect.bottom = ( (Height * 0.75) + X ) / 2;
         Rect.left = 0;
         Rect.right = Width;
         break;
       case gstTriangleLeft:
         Rect.top = ( Height - X ) / 2;
         Rect.bottom = ( Height + X ) / 2;
         Rect.left = 0;
         Rect.right = Width * 1.25;
         break;
       case gstTriangleRight:
         Rect.top = ( Height - X ) / 2;
         Rect.bottom = ( Height + X ) / 2;
         Rect.left = 0;
         Rect.right = Width * 0.75;
         break;
       default:
         Rect.top = ( Height - X ) / 2;
         Rect.bottom = ( Height + X ) / 2;
         Rect.left = 0;
         Rect.right = Width;
         break;
     }
     DrawText( Canvas->Handle, FCaption.c_str(), -1, &Rect, DT_CENTER | DT_WORDBREAK);
   }
}
//---------------------------------------------------------------------------
namespace Lamp2
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TLamp2)};
        RegisterComponents("CJW", classes, 0);
    }
}
//---------------------------------------------------------------------------
