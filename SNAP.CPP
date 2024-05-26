//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "snap.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSnapForm *SnapForm;
//---------------------------------------------------------------------------
__fastcall TSnapForm::TSnapForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSnapForm::SpeedButton1Click(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
