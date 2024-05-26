//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "managerotes.h"
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
