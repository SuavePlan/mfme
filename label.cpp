//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "interface.h"
#include "label.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TPropertiesForm *PropertiesForm;
//---------------------------------------------------------------------------
__fastcall TPropertiesForm::TPropertiesForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OKClick(TObject *Sender)
{
int X;
int Y;
int old_x;
int old_y;
String type;

  type = String(((TControl *)Form1->menu_ptr)->ClassName());
  if ( type == "TLamp" ) {
    ((TLamp *)Form1->menu_ptr)->OnColor = Colour->Brush->Color;
  } else if ( type == "TLabel" ) {
    ((TLabel *)Form1->menu_ptr)->Caption = Caption->Text;
    ((TLabel *)Form1->menu_ptr)->Font->Color = Colour->Brush->Color;
  } else if ( type == "TLampButton" ) {
    ((TzColorBtn *)Form1->menu_ptr)->Caption = Caption->Text;
    ((TzColorBtn *)Form1->menu_ptr)->Color = Colour->Brush->Color;
  } else if ( type == "TCheckBox" ) {
   ((TCheckBox *)Form1->menu_ptr)->Caption = Caption->Text;
   ((TCheckBox *)Form1->menu_ptr)->Font->Color = Colour->Brush->Color;
  }
  PropertiesForm->Visible = FALSE;

}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::CancelClick(TObject *Sender)
{
  PropertiesForm->Visible = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::ColourMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ( ColorDialog1->Execute() )
    Colour->Brush->Color = ColorDialog1->Color;

}
//---------------------------------------------------------------------------



