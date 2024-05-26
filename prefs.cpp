//---------------------------------------------------------------------------

#include <vcl.h>
//#include <registry.hpp>
#pragma hdrstop

#include "prefs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGeneralConfig *GeneralConfig;
//---------------------------------------------------------------------------
__fastcall TGeneralConfig::TGeneralConfig(TComponent* Owner)
    : TForm(Owner)
{
  loaded = false;
  RegFile = new TRegIniFile("SOFTWARE\\CJW");
  Draggablereels->Checked = ReadBool("DragReels");
  ConvertReel->Checked = ReadBool("ConvertReel");
  MeterPanelResize->Checked = ReadBool("MeterPanelResize");
  NoAboutBox->Checked = ReadBool("NoAboutBox");
  StartInGameManager->Checked = ReadBool("GameManager");
  MeterPanelOff->Checked = ReadBool("MeterPanelOff");
  LongTermMeters->Checked = ReadBool("LongTermMeters");
  FileNames->Checked = ReadBool("UseFileNames");
  ActiveInEdit->Checked = ReadBool("ActiveInEdit");
  EscQuitsProgram->Checked = ReadBool("ESCQuitsProgram");
  HighPriority->Checked = ReadBool("HighPriority");
  RegFile->Free();
  loaded = true;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralConfig::DraggablereelsClick(TObject *Sender)
{
  SaveBool("DragReels", Draggablereels->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralConfig::ConvertReelClick(TObject *Sender)
{
  SaveBool("ConvertReel", ConvertReel->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralConfig::MeterPanelResizeClick(TObject *Sender)
{
  SaveBool("MeterPanelResize", MeterPanelResize->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralConfig::SaveBool(String name, bool value)
{
  if ( loaded ) {
    RegFile = new TRegIniFile("SOFTWARE\\CJW");
    RegFile->WriteBool("MFME",name,value);
    RegFile->Free();
  }
}
//---------------------------------------------------------------------------
bool __fastcall TGeneralConfig::ReadBool(String name)
{
bool result;

  result = RegFile->ReadBool("MFME",name,false);
  return result;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralConfig::NoAboutBoxClick(TObject *Sender)
{
  SaveBool("NoAboutBox", NoAboutBox->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::StartInGameManagerClick(TObject *Sender)
{
  SaveBool("GameManager", StartInGameManager->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::MeterPanelOffClick(TObject *Sender)
{
  SaveBool("MeterPanelOff", MeterPanelOff->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::LongTermMetersClick(TObject *Sender)
{
  SaveBool("LongTermMeters", LongTermMeters->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::FileNamesClick(TObject *Sender)
{
  SaveBool("UseFileNames", FileNames->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::ActiveInEditClick(TObject *Sender)
{
  SaveBool("ActiveInEdit", ActiveInEdit->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::EscQuitsProgramClick(TObject *Sender)
{
  SaveBool("EscQuitsProgram", EscQuitsProgram->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TGeneralConfig::HighPriorityClick(TObject *Sender)
{
  SaveBool("HighPriority", HighPriority->Checked);
}
//---------------------------------------------------------------------------

