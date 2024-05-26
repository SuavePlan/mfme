//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GR32_Image"
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
  sel = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer)
{
int  X2 = ((( Image->Bitmap->Width * 65536 ) / Image->Width ) * X ) / 65536;
int  Y2 = ((( Image->Bitmap->Height * 65536 ) / Image->Height ) * Y ) / 65536;

  sel = TGRBase::List.FindXY(X2,Y2);
  if ( sel ) {
    if ( Button == mbRight ) {
      sel->State = 1 - sel->State;
//      sel->BringToFront();
      sel->Draw();
    }
    if ( sel->HasButton ) {
      sel->SetDown(true);
      Form1->MatrixChange(sel, true);
    }
    if ( Form1->EditMode ) {
      move = true;
      BiasX = X2 - sel->X;
      BiasY = Y2 - sel->Y;
    }
  } else
    sel = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ImageMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y, TCustomLayer *Layer)
{
  if ( sel ) {
    if ( sel->HasButton ) {
      sel->SetDown(false);
      Form1->MatrixChange(sel, false);
    }
  }
  sel = NULL;
  move = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ImageMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y, TCustomLayer *Layer)
{
int  X2 = ((( Image->Bitmap->Width * 65536 ) / Image->Width ) * X ) / 65536;
int  Y2 = ((( Image->Bitmap->Height * 65536 ) / Image->Height ) * Y ) / 65536;

  TGRBase * comp = TGRBase::List.FindXY(X2,Y2);
  if ( sel ) {
    if ( comp != sel ) {
      sel->SetDown(false);
      if ( sel->HasButton )
        Form1->MatrixChange2(sel->Button, false, false);
    }
    if ( move ) {
      sel->SetBounds(X2 - BiasX, Y2 - BiasY, sel->Width, sel->Height);
    }
  }
  if ( comp && comp->HasButton )
    Image->Cursor = crHandPoint;
  else
    Image->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ImageKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ( Form1->shortcuts[Key] != NULL && !Form1->EditMode ) {
    Form1->MatrixChange(Form1->shortcuts[Key], true);
    TGRBase * ptr = TGRBase::List.FindShortcut(Key);
    if ( ptr )
      ptr->SetDown(true);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::ImageKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if ( Form1->shortcuts[Key] != NULL  ) {
    Form1->MatrixChange(Form1->shortcuts[Key], false);
    TGRBase * ptr = TGRBase::List.FindShortcut(Key);
    if ( ptr )
      ptr->SetDown(false);
  }
}
//---------------------------------------------------------------------------

