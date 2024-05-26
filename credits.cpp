//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "credits.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "zPanel2"
#pragma link "Mystd"
#pragma resource "*.dfm"
TCredit *Credit;

//---------------------------------------------------------------------------
__fastcall TCredit::TCredit(TComponent* Owner)
    : TForm(Owner)
{
int count;
char *chr_ptr;
int sum = 0;

  for ( count = 0; count < scroll->Lines->Count; count++ ) {
    chr_ptr = scroll->Lines->Strings[count].c_str();
    for ( ; *chr_ptr ; chr_ptr++)
      sum += *chr_ptr;
  }
  if ( sum != 64323 )
    Application->Terminate();
}
//---------------------------------------------------------------------------


void __fastcall TCredit::scrollMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  scroll->Interval = 0;
}
//---------------------------------------------------------------------------

void __fastcall TCredit::scrollMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  scroll->Interval = 50;
}
//---------------------------------------------------------------------------

