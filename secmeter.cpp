//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "secmeter.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSECForm *SECForm;
//---------------------------------------------------------------------------
__fastcall TSECForm::TSECForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TSECForm::SetText(int nbr, String text)
{
TEdit *type[] = { Text1, Text2, Text3, Text4, Text5, Text6, Text7, Text8, Text9, Text10 };

  if ( nbr < 10 )
    type[nbr]->Text = text;
}
//---------------------------------------------------------------------------

void __fastcall TSECForm::SetValue(int nbr, int value)
{
TEdit *values[] = { Value1, Value2, Value3, Value4, Value5, Value6, Value7, Value8, Value9, Value10 };
char buffer[8];

  if ( nbr < 10 ) {
    sprintf( buffer, "%07d", value);
    values[nbr]->Text = AnsiString(buffer);
  }
}


