//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "band.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoadBandForm *LoadBandForm;
//---------------------------------------------------------------------------
__fastcall TLoadBandForm::TLoadBandForm(TComponent* Owner)
    : TForm(Owner)
{
  Reel->ItemIndex = 0;
  Stops->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TLoadBandForm::ApplyClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

