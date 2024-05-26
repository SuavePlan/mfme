//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "scan.h"
#include "interface.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TScanWindow *ScanWindow;
//---------------------------------------------------------------------------
__fastcall TScanWindow::TScanWindow(TComponent* Owner)
    : TForm(Owner)
{
  cancel = false;
}
//---------------------------------------------------------------------------
void __fastcall TScanWindow::CancelClick(TObject *Sender)
{
  cancel = true;
}
//---------------------------------------------------------------------------
void __fastcall TScanWindow::FormShow(TObject *Sender)
{
  Timer1->Enabled = true;
  Files->Caption = "0 files";
}
//---------------------------------------------------------------------------
void __fastcall TScanWindow::Timer1Timer(TObject *Sender)
{
  Timer1->Enabled = false;
  Form1->DoScan();
}
//---------------------------------------------------------------------------
