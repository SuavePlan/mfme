//---------------------------------------------------------------------------
//#include <classes.hpp>
//#pragma hdrstop

#include <stdio.h>
#include "grbase.h"
#include "grSevenSeg.h"
//---------------------------------------------------------------------------
#pragma link "GR32_Image"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TGRSevenSeg::TGRSevenSeg(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
   : TGRBase(ANumber, "TGRSevenSeg", AX, AY, AHeight, AWidth, Adest)
{
  FSpace = 1;
  FHorzSpace = 10;
  FVertSpace = 6;
  FValue = 0;
  FThickness = 3;
  FBackColour = clBlack;
  FOnColour = clRed;
  FOffColour = RGB(64,64,64);
  dst = new TBitmap32();
  dst->Width = AWidth;
  dst->Height = AHeight;
  FdpOn = false;
  Fseg14 = false;
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRSevenSeg::~TGRSevenSeg()
{
  delete dst;
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetOnColour(TColor FColour)
{
  if ( FOnColour != FColour ) {
    FOnColour = FColour;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetOffColour(TColor FColour)
{
  if ( FOffColour != FColour ) {
    FOffColour = FColour;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetBackColour(TColor FColour)
{
  if ( FBackColour != FColour ) {
    FBackColour = FColour;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetSpace(int Space)
{
  if ( FSpace != Space ) {
    FSpace = Space;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetHorzSpace(int Space)
{
  if ( FHorzSpace != Space ) {
    FHorzSpace = Space;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetVertSpace(int Space)
{
  if ( FVertSpace != Space ) {
    FVertSpace = Space;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetThickness(int Value)
{
  if ( FThickness != Value ) {
    FThickness = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetLamp(int nbr, int Value)
{
  if ( Value )
    FValue |= nbr;
  else
    FValue &= ~nbr;
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetLamps(int Value)
{
  FValue = Value;
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetDPOn(bool Value)
{
  if ( FdpOn != Value ) {
    FdpOn = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetDP(bool Value)
{
  if ( FdpRight != Value ) {
    FdpRight = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::Set14(bool Value)
{
  if ( Fseg14 != Value ) {
    Fseg14 = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::SetProgrammable(bool Value)
{
  if ( FProgrammable != Value ) {
    FProgrammable = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::Draw(void)
{
TRect MyRect;
TCanvas * ptr = new TCanvas();

  Bitmap->Clear(Color32(FBackColour));
  ptr->Handle = Bitmap->Handle;
  ptr->Pen->Style = psClear;
  ptr->Brush->Style = bsSolid;
  MyRect = Rect( 0 , FVertSpace >> 1, Width, Height - (FVertSpace >> 1));
  DrawSegment( FValue, ptr, MyRect);
  Bitmap->ResetAlpha();
  TGRBase::Draw();
  delete ptr;
}
//---------------------------------------------------------------------------
void __fastcall TGRSevenSeg::DrawSegment(int digit, TCanvas * ptr, TRect rect)
{
int i;
int CentreY;
int CentreX;
int Space2;
int Thick2;
Windows::TPoint points4[4];
Windows::TPoint points6[6];

  CentreY = (rect.Bottom + rect.Top) >> 1;
  CentreX = (rect.Left + rect.Right) >> 1;
  Space2 = FSpace >> 1;
  Thick2 = FThickness >> 1;

  rect.left += (FHorzSpace >> 1);
  rect.right -= (FHorzSpace >> 1);

  if ( !Fseg14 ) {
    for ( i = 0; i < 7; i++ ) {
      if ( (digit >> i) & 1 )
        ptr->Brush->Color = FOnColour;
      else
        ptr->Brush->Color = FOffColour;
      switch ( i ) {
        case 0:
           points4[0] = Classes::Point (rect.left + FSpace, rect.top);
           points4[1] = Classes::Point (rect.left + FThickness + FSpace,
                         rect.top + FThickness);
           points4[2] = Classes::Point (rect.right - FThickness - FSpace,
                         rect.top + FThickness);
           points4[3] = Classes::Point (rect.right - FSpace, rect.top);
           ptr->Polygon(points4, 3);
        break;
        case 1:
           points6[0] = Classes::Point (rect.right, CentreY - Thick2 - FSpace);
           points6[1] = Classes::Point (rect.right - Thick2, CentreY - Space2);
           points6[2] = Classes::Point (rect.right - FThickness,
                         CentreY - Thick2 - FSpace);
           points6[3] = Classes::Point (rect.right - FThickness,
                         rect.top + FThickness + FSpace);
           points6[4] = Classes::Point (rect.right, rect.top + FSpace);
           ptr->Polygon(points6, 4);
        break;
        case 2:
           points6[0] = Classes::Point (rect.right, rect.bottom - FSpace);
           points6[1] = Classes::Point (rect.right - FThickness,
                         rect.bottom - FThickness - FSpace);
           points6[2] = Classes::Point (rect.right - FThickness,
                         CentreY + Thick2 + FSpace);
           points6[3] = Classes::Point (rect.right - Thick2, CentreY + Space2);
           points6[4] = Classes::Point (rect.right, CentreY + Thick2 + FSpace);
           ptr->Polygon(points6, 4);
        break;
        case 3:
           points4[0] = Classes::Point (rect.left + FSpace, rect.bottom);
           points4[1] = Classes::Point (rect.left + FThickness + FSpace,
                         rect.bottom - FThickness);
           points4[2] = Classes::Point (rect.right - FThickness - FSpace,
                         rect.bottom - FThickness);
           points4[3] = Classes::Point (rect.right - FSpace, rect.bottom);
           ptr->Polygon(points4, 3);
        break;  
        case 4:
           points6[0] = Classes::Point (rect.left, CentreY + Thick2 + FSpace);
           points6[1] = Classes::Point (rect.left + Thick2, CentreY + Space2);
           points6[2] = Classes::Point (rect.left + FThickness,
                          CentreY + Thick2);
           points6[3] = Classes::Point (rect.left + FThickness,
                          rect.bottom - FThickness - FSpace);
           points6[4] = Classes::Point (rect.left, rect.bottom - FSpace);
           ptr->Polygon(points6, 4);
        break;
        case 5:
           points6[0] = Classes::Point (rect.left, rect.top + FSpace);
           points6[1] = Classes::Point (rect.left + FThickness,
                         rect.top + FThickness + FSpace);
           points6[2] = Classes::Point (rect.left + FThickness,
                         CentreY - Thick2 - FSpace);
           points6[3] = Classes::Point (rect.left + Thick2, CentreY - Space2);
           points6[4] = Classes::Point (rect.left, CentreY - Thick2 - FSpace);
           ptr->Polygon(points6, 4);
        break;
        case 6:
           points6[0] = Classes::Point (rect.left + FSpace + Thick2, CentreY);
           points6[1] = Classes::Point (rect.left + FSpace + FThickness, CentreY - Thick2);
           points6[2] = Classes::Point (rect.right - FSpace - FThickness, CentreY - Thick2);
           points6[3] = Classes::Point (rect.right - FSpace - Thick2 , CentreY);
           points6[4] = Classes::Point (rect.right - FSpace - FThickness, CentreY + Thick2);
           points6[5] = Classes::Point (rect.left + FSpace + FThickness, CentreY + Thick2);
           ptr->Polygon(points6, 5);
        break;
      }
    }
    if ( ((digit >> 7 ) & 1 ) || FdpOn )
      ptr->Brush->Color = FOnColour;
    else
      ptr->Brush->Color = FOffColour;
    if ( FdpRight ) {
      ptr->Rectangle(
        rect.right + (FThickness >> 1),
        rect.bottom - FThickness,
        rect.right + (FThickness >> 1) + FThickness,
        rect.bottom );
    } else {
      ptr->Rectangle(
        rect.left - (FThickness >> 1) - FThickness,
        rect.bottom - FThickness,
        rect.left - (FThickness >> 1),
        rect.bottom );
    }
  } else {
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRSevenSeg::SaveToStream(TStream *ptr)
{
//  TGRBase::SaveToStream(ptr);
}


