//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <dir.h>

#include "interface.h"
#include "gamemanager.h"
#include "browse.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TManager *Manager;
//---------------------------------------------------------------------------
__fastcall TManager::TManager(TComponent* Owner)
    : TForm(Owner)
{
  RootDir->Text = Form1->PathName;
}
//---------------------------------------------------------------------------
void __fastcall TManager::Search(String Path, int Attributes, int depth)
{
TSearchRec sr;
int done;
TListItem *ListItem;

  depth++;
  if ( depth < 3 || SubDirs->Checked ) {
    done = FindFirst(Path + "\\*", Attributes, sr);
    while ( !done )
    {
      if ( sr.Attr == Attributes ) {
        if ( sr.Name.SubString(0, 1) != "." ) {
          Search( Path + "\\" + sr.Name, faDirectory, depth);
        }
      }
      if ( LowerCase(ExtractFileExt( sr.Name)) == ".gam" )
        CheckFile(Path + "\\" + sr.Name);
      done = FindNext(sr);
    }
    FindClose(sr);
  }
  depth--;
}
//---------------------------------------------------------------------------
void __fastcall TManager::CheckFile(String Path)
{
FILE *fp;
char classname[50];
char caption[80];
char *cp, ch;
int Tag, Value[8];
bool found;
TListItem *ListItem;
String Name, System;
String Totali, Totalo;
String Version;
int TotI, TotO;
float Percentage;
String Status;
bool FoundROM;
bool FoundRES;

  chdir(ExtractFilePath(Path).c_str());
  fp = fopen(Path.c_str(), "r");
  if ( fp ) {
    found = false;
    TotI = 0;
    TotO = 0;
    Status = "OK";
    FoundROM = false;
    FoundRES = false;
    while ( !feof(fp) ) {
      fscanf( fp, "%s", classname);
      if ( feof(fp) )
        break;
      if ( !strcmp(classname,"System") ) {
        fscanf( fp, "%s\n", caption);
        found = true;
        System = (AnsiString)caption;
//        Arch = SystemType(caption);
      } else if ( found == false ) {
        break;
      } else if ( !strcmp(classname,"ROM" ) ) {
        fscanf( fp, "%s\n", caption);
        FoundROM = true;
        if ( !FileExists(caption) ) {
          Status = (AnsiString)caption + " missing";
          break;
        }
      } else if ( !strcmp(classname,"Layout") ) {
        fscanf( fp, "%s\n", caption);
        FoundRES = true;
        if ( !FileExists(caption) ) {
          Status = (AnsiString)caption + " missing";
          break;
        }
      } else if ( !strcmp(classname,"Version") ) {
        fscanf( fp, "%s\n", caption);
        Version = (AnsiString)caption;
      } else if ( !strcmp(classname,"DIP") ) {
        fscanf( fp, "%d %s\n", &Tag, caption);
//        if ( Tag == 1 ) {
//          matrix[6] = StrToBin(AnsiString(caption));
//          Config->SetDip1(matrix[6]);
//        } else {
//          matrix[7] = StrToBin(AnsiString(caption));
//          Config->SetDip2(matrix[7]);
//        }
      } else if ( !strcmp(classname,"Character") ) {
        fscanf( fp, "%s\n", caption);
        if ( !FileExists(caption) ) {
          Status = (AnsiString)caption + " missing";
          break;
        }
//        CharName = AnsiString(caption);
//        fp2 = fopen( caption, "r");
//        if ( fp2 != NULL ) {
//          for ( count = 0; count < 72; count++ ) {
//            ch = fgetc(fp2);
//            Character[count] = ch;
//          }
//          fclose(fp2);
//        } else
//          Application->MessageBox("Character file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"Sound") ) {
        fscanf( fp, "%s\n", caption);
        if ( !FileExists(caption) ) {
          Status = (AnsiString)caption + " missing";
          break;
        }
//          if ( SoundList != NULL )
//            SoundList->Add(AnsiString(caption));
//        } else
//          Application->MessageBox("Sound file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"Game") ) {
        cp = caption;
        while ( ( ch = fgetc(fp)) == ' ' )
          ;
        while ( ch != '\n' && !feof(fp) ) {
          *cp++ = ch;
          ch = fgetc(fp);
        }
        *cp = '\0';
        Name = (AnsiString)caption;
//        Caption = AnsiString(caption);
//        GameName = Caption;
//        Config->GameName->Text = GameName;
      } else if ( !strcmp(classname,"Stake") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 5 ) {
//          Tag++;
//          Config->StakeKey->ItemIndex = Tag;
//          Config->StakeKeyChange(NULL);
//        }
      } else if ( !strcmp(classname,"Jackpot") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 5 ) {
//          Tag++;
//          Config->JackpotKey->ItemIndex = Tag;
//          Config->JackpotKeyChange(NULL);
//        }
      } else if ( !strcmp(classname,"Percentage") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 16 ) {
//          Tag++;
//          Config->PercentKey->ItemIndex = Tag;
//          Config->PercentKeyChange(NULL);
//        }
      } else if ( !strcmp(classname,"Hopper") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < Config->Payout->Items->Count ) {
//          Config->Payout->ItemIndex = Tag;
//          Hopper_Type = Tag;
//        }
      } else if ( !strcmp(classname,"Extender") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < Config->Extender->Items->Count ) {
//          Config->Extender->ItemIndex = Tag;
//          Extender_Type = Tag;
//        }
      } else if ( !strcmp(classname,"Delay") ) {
        fscanf( fp, "%d\n", &Tag);
//        Config->Delay->Text = IntToStr(Delay);
      } else if ( !strcmp(classname,"CharAddr") ) {
        fscanf( fp, "%d\n", &Tag);
      } else if ( !strcmp(classname,"DataType") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 2 ) {
//          Config->ProgramType->ItemIndex = Tag;
//          Program_Type = Tag;
//        }
      } else if ( !strcmp(classname,"Reel") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 3 ) {
//          Config->Reels->ItemIndex = Tag;
//          Reel_Type = Tag;
//        }
      } else if ( !strcmp(classname,"LED") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 4 ) {
//          Config->SegmentDisplay->ItemIndex = Tag;
//          LED_Type = Tag;
//        }
      } else if ( !strcmp(classname,"Alpha") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 2 ) {
//          Config->AlphaDisplay->ItemIndex = Tag;
//          Alpha_Type = Tag;
//        }
      } else if ( !strcmp(classname,"AudioType") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 3 ) {
//          Config->Sound->ItemIndex = Tag;
//          Sound_Type = Tag;
//        }
      } else if ( !strcmp(classname,"Volume") ) {
        fscanf( fp, "%d\n", &Tag);
//        if ( Tag < 2 ) {
//          Config->Volume->ItemIndex = Tag;
//          Volume_Type = Tag;
//        }
      } else if ( !strcmp(classname,"InMeter") ) {
        fscanf( fp, "%d %d %d %d %d %d %d %d\n", &Value[0], &Value[1],
                                                 &Value[2], &Value[3],
                                                 &Value[4], &Value[5],
                                                 &Value[6], &Value[7]);
      } else if ( !strcmp(classname,"OutMeter") ) {
        fscanf( fp, "%d %d %d %d %d %d %d %d\n", &Value[0], &Value[1],
                                                 &Value[2], &Value[3],
                                                 &Value[4], &Value[5],
                                                 &Value[6], &Value[7]);
      } else if ( !strcmp(classname,"InTriac") ) {
        fscanf( fp, "%d %d %d %d %d %d %d %d\n", &Value[0], &Value[1],
                                                 &Value[2], &Value[3],
                                                 &Value[4], &Value[5],
                                                 &Value[6], &Value[7]);
      } else if ( !strcmp(classname,"OutTriac") ) {
        fscanf( fp, "%d %d %d %d %d %d %d %d\n", &Value[0], &Value[1],
                                                 &Value[2], &Value[3],
                                                 &Value[4], &Value[5],
                                                 &Value[6], &Value[7]);
      } else if ( !strcmp(classname,"SetPercent") ) {
        fscanf( fp, "%d\n", &Tag);
//        Config->Percentage->Text = StrToInt(Percentage);
      } else if ( !strcmp(classname,"TotalIn") ) {
        fscanf( fp, "%d\n", &TotI);
        Totali = IntToStr(TotI);
//      Total_In += TotI;
      } else if ( !strcmp(classname,"TotalOut") ) {
        fscanf( fp, "%d\n", &TotO);
        Totalo = IntToStr(TotO);
//      Total_Out += TotO;
      } else
        fscanf( fp, "\n");
    }
    fclose(fp);
    if ( found && Name != "" ) {
      Total_In += TotI;
      Total_Out += TotO;
      ListItem = ListView1->Items->Add();
      ListItem->Caption = Name;
      ListItem->SubItems->Add(Version);
      ListItem->SubItems->Add(System);
      ListItem->SubItems->Add(Totali);
      ListItem->SubItems->Add(Totalo);
      if ( TotI )
        Percentage = ( (float)TotO / (float)TotI ) * 100;
      else
        Percentage = 0;
      sprintf( caption, "%.2f", Percentage);
      ListItem->SubItems->Add((AnsiString)caption);
      if ( FoundROM == false )
        Status = "ROM entry missing";
      else if ( FoundRES == false )
        Status = "RES entry missing";
      ListItem->SubItems->Add(Status);
      ListItem->SubItems->Add(Path);
      Total_Games++;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TManager::ScanClick(TObject *Sender)
{
String dir;
char buffer[50];
float perc;

  ListView1->Items->Clear();
  Total_Games = 0;
  Total_In = 0;
  Total_Out = 0;
  dir = RootDir->Text;
  if ( dir.SubString( dir.Length(), 1) == "\\" )
    dir = dir.SubString( 1, dir.Length()-1);
  Search( dir, faDirectory, 0);
  NbrGames->Caption = IntToStr(Total_Games);
  TotalIn->Caption = IntToStr(Total_In);
  TotalOut->Caption = IntToStr(Total_Out);
  PAndL->Caption = IntToStr(Total_Out-Total_In);
  if ( Total_In )
    perc = ( (float)Total_Out / (float)Total_In ) * 100;
  else
    perc = 0;
  sprintf( buffer, "%.2f", perc);
  TotPercentage->Caption = (AnsiString)buffer;
}
//---------------------------------------------------------------------------

void __fastcall TManager::ListView1DblClick(TObject *Sender)
{
  Button1Click(Sender);
}
//---------------------------------------------------------------------------

int __stdcall CustomSortProc(long Item1, long Item2, long ParamSort)
{
  switch ( ParamSort ) {
    case 255:
      return lstrcmp(((TListItem *)Item1)->Caption.c_str(),
                     ((TListItem *)Item2)->Caption.c_str());
      break;
    case 0:
    case 1:
    case 5:
      return lstrcmp(((TListItem *)Item1)->SubItems->Strings[ParamSort].c_str(),
                     ((TListItem *)Item2)->SubItems->Strings[ParamSort].c_str());
      break;
    default:
      if ( atof(((TListItem *)Item1)->SubItems->Strings[ParamSort].c_str()) >=
           atof(((TListItem *)Item2)->SubItems->Strings[ParamSort].c_str()) )
        return -1;
      else
        return 1;
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TManager::ListView1ColumnClick(TObject *Sender,
      TListColumn *Column)
{
  if ( Column->Caption == "Game Name" )
    ListView1->CustomSort(CustomSortProc, 255);
  else if ( Column->Caption == "Version" )
    ListView1->CustomSort(CustomSortProc, 0);
  else if ( Column->Caption == "Type" )
    ListView1->CustomSort(CustomSortProc, 1);
  else if ( Column->Caption == "Total In" )
    ListView1->CustomSort(CustomSortProc, 2);
  else if ( Column->Caption == "Total Out" )
    ListView1->CustomSort(CustomSortProc, 3);
  else if ( Column->Caption == "%" )
    ListView1->CustomSort(CustomSortProc, 4);
  else if ( Column->Caption == "Status" )
    ListView1->CustomSort(CustomSortProc, 5);
}
//---------------------------------------------------------------------------


void __fastcall TManager::ListView1Click(TObject *Sender)
{
  if ( ListView1->Selected )
    GamePath->Caption = ListView1->Selected->SubItems->Strings[6];
}
//---------------------------------------------------------------------------

void __fastcall TManager::Button1Click(TObject *Sender)
{
  if ( ListView1->Selected && ListView1->Selected->SubItems->Strings[5] == "OK" ) {
    Manager->Hide();
    Form1->HaltFruit(4);
  }
}
//---------------------------------------------------------------------------


void __fastcall TManager::SetvtpClick(TObject *Sender)
{
  Form1->VTP = Total_In;
  Form1->SetVTP();
  Form1->UpdateMeters();    
}
//---------------------------------------------------------------------------



void __fastcall TManager::BackupRAMClick(TObject *Sender)
{
int count;
String src;
String dst;
TFileStream *srcp, *dstp;
int total = 0;
char message[50];

  if ( !DirectoryExists( Form1->PathName + "\\RAMBackups" ) )
    CreateDir(Form1->PathName + "\\RAMBackups");
  for ( count = 0; count < ListView1->Items->Count; count++ ) {
    src = ListView1->Items->Item[count]->SubItems->Strings[5];
    src = src.SubString( 1, src.Length() - 3) + "ram";
    if ( FileExists(src) ) {
      dst = Form1->PathName + "RAMBackups\\" + ExtractFileName(src);
      srcp = new TFileStream( src, fmOpenRead);
      dstp = new TFileStream( dst, fmCreate);
      dstp->CopyFrom( srcp, 0);
      delete srcp;
      delete dstp;
      total++;
    }
  }
  sprintf( message, "%d RAM files written", total);
  Application->MessageBox(message, "Success", MB_OK | MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------



void __fastcall TManager::BrowseClick(TObject *Sender)
{
  if ( Browser->ShowModal() == mrOk )
    RootDir->Text = Browser->DirectoryListBox1->Directory;
}
//---------------------------------------------------------------------------

