//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "interface.h"
#include "managernotes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGameNotes *GameNotes;
//---------------------------------------------------------------------------
__fastcall TGameNotes::TGameNotes(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGameNotes::SpeedButton1Click(TObject *Sender)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGameNotes::SpeedButton2Click(TObject *Sender)
{
  ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TGameNotes::FormShow(TObject *Sender)
{
  Name->Text = Form1->Name->Caption;
  Manufacturer->Text = Form1->Manufacturer->Caption;
  ReleaseDate->Text = Form1->ReleaseDate->Caption;
  MachineType->Text = Form1->MachineType->Caption;
  Technology->Text = Form1->Technology->Caption;
  Stakes->Text = Form1->Stakes->Caption;
  Prizes->Text = Form1->Prizes->Caption;
  Notes->Text = Form1->Notes->Text;
}
//---------------------------------------------------------------------------
