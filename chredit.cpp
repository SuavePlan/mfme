//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "chredit.h"
#include "shared.h"
//#include "interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChrEditor *ChrEditor;
bool update;
//---------------------------------------------------------------------------
__fastcall TChrEditor::TChrEditor(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TChrEditor::FormShow(TObject *Sender)
{
char buffer[6];
TEdit *box[10] = {
  bwb0, bwb1, bwb2, bwb3, bwb4, bwb5, bwb6, bwb7, bwb8, bwb9
};

  index= 0;
  Refresh();
  BoxClick(NULL);
  for ( int i = 0; i < 10; i++ ) {
    sprintf( buffer, "%02X", Character[i]);
    box[i]->Text = AnsiString(buffer);
  }
}
//---------------------------------------------------------------------------

void __fastcall TChrEditor::BoxClick(TObject *Sender)
{
String item;
TEdit *patch[8] = {
  Patch1, Patch2, Patch3, Patch4, Patch5, Patch6, Patch7, Patch8
};
int i;

  index = Box->ItemIndex;
  item = Box->Items->Strings[index];
  update = true;
  for ( i = 0; i < 8; i++ ) {
    patch[i]->Text = item.SubString(1 + 3*i, 2);
  }
  update = false;
}
//---------------------------------------------------------------------------
void __fastcall TChrEditor::BoxChange(TObject *Sender)
{
int value;
int number;

  if ( ((TEdit *)Sender)->Text.Length() == 2 && !update ) {
    value = StrToHex(((TEdit *)Sender)->Text.c_str());
    number = ((TComponent *)Sender)->Tag;
    Character[number + 8 * Box->ItemIndex] = value;
    Refresh();
    LayoutChanged();
  }
}
//---------------------------------------------------------------------------

void __fastcall TChrEditor::PatchDblClick(TObject *Sender)
{
int number, value;

  if ( Box->ItemIndex == 8 ) {
    value = StrToHex(((TEdit *)Sender)->Text.c_str()) >> 2;
    value = ((value + 1) & 31) << 2;
    ((TEdit *)Sender)->Text = IntToHex(value,2);
    number = ((TComponent *)Sender)->Tag;
    Character[number + 8 * Box->ItemIndex] = value;
    Refresh();
    LayoutChanged();
  }
}
//---------------------------------------------------------------------------

void __fastcall TChrEditor::Refresh(void)
{
char buffer[80];
int i;
unsigned char *ptr = Character;

  Box->Items->Clear();
  for ( i = 0; i < 9; i++ ) {
    sprintf( buffer, "%02X %02X %02X %02X %02X %02X %02X %02X",
                     ptr[0], ptr[1], ptr[2], ptr[3],
                     ptr[4], ptr[5], ptr[6], ptr[7]);
    ptr += 8;
    Box->Items->Add(AnsiString(buffer));
  }
  Box->ItemIndex = index;
}
//---------------------------------------------------------------------------

void __fastcall TChrEditor::FormClose(TObject *Sender,
      TCloseAction &Action)
{
  Release();
  ChrEditor = NULL;    
}
//---------------------------------------------------------------------------

void __fastcall TChrEditor::BWBChange(TObject *Sender)
{
int value, number;

  number = ((TEdit *)Sender)->Tag;
  value = StrToHex(((TEdit *)Sender)->Text.c_str());
  Character[number] = value;
}
//---------------------------------------------------------------------------

