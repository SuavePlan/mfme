//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "interface.h"
#include "notes.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLayoutNotes *LayoutNotes;
//---------------------------------------------------------------------------
__fastcall TLayoutNotes::TLayoutNotes(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TLayoutNotes::NotesChange(TObject *Sender)
{
    Form1->LayoutChanged();
}
//---------------------------------------------------------------------------
