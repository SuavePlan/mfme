//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "createband.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCreateBandForm *CreateBandForm;
//---------------------------------------------------------------------------
__fastcall TCreateBandForm::TCreateBandForm(TComponent* Owner)
    : TForm(Owner)
{
  Reel->ItemIndex = 0;
  Stops->ItemIndex = 0;
  Horizontal->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TCreateBandForm::ApplyClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
