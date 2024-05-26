//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "grbase.h"
#include "comps.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm3::CompView(void)
{
TGRBase *comp;
TListItem * item;

  ListView1->Items->Clear();
  ListView1->Items->BeginUpdate();

  for ( int i = 0; i < TGRBase::List.Count; i++ ) {
    comp = TGRBase::List[i];
    item = ListView1->Items->Add();
    item->Caption = IntToStr(comp->Number);
    item->SubItems->Add(comp->Name);
    item->SubItems->Add(IntToStr(comp->X));
    item->SubItems->Add(IntToStr(comp->Y));
    item->SubItems->Add(IntToStr(comp->Width));
    item->SubItems->Add(IntToStr(comp->Height));
    item->SubItems->Add(IntToStr(comp->Button));
    item->SubItems->Add(IntToStr(comp->Shortcut));
  }
  ListView1->Items->EndUpdate();
  Form3->Caption = IntToStr(TGRBase::List.Count) + " Components";
  Show();
}
