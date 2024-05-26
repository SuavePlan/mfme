//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "browse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBrowser *Browser;
//---------------------------------------------------------------------------
__fastcall TBrowser::TBrowser(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TBrowser::Button2Click(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TBrowser::OKClick(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
