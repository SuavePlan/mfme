//---------------------------------------------------------------------------
#include <vcl.h>
#include <regstr.h>
#include <registry.hpp>
#pragma hdrstop

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <filectrl.hpp>
#include <io.h>
#include <dir.h>
#include <sys\stat.h>
#include "easycompression.hpp"

#include "shared.h"
#include "scan.h"
#include "notes.h"
#include "managernotes.h"
#include "prefs.h"
#include "interface.h"
#include "debug.h"
#include "secmeter.h"
#include "iomonitor.h"
#include "reeleditor.h"
#include "property.h"
#include "credits.h"
#include "Configuration.h"
#include "chredit.h"
#include "mpu3.h"
#include "mpu4.h"
#include "impact.h"
#include "scorpion1.h"
#include "scorpion2.h"
#include "dotmatrix.h"
#include "sys80.h"
#include "m1a.h"
#include "mps2.h"
#include "ace1.h"
#include "sys80.h"
#include "space.h"
#include "proconn.h"
#include "sys5.h"
#include "samples.h"
#include "snap.h"
#include "mpu4video.h"

//---------------------------------------------------------------------------
#pragma link "Lamps"
#pragma link "Seven"
#pragma link "Std2"
#pragma link "zBtn2"
#pragma link "zLed"
#pragma link "Gpshape"
#pragma link "Lamp2"
#pragma link "CanvasPanel"
#pragma link "Reel"
#pragma link "Frame2"
//#pragma link "Ledgadgt"
#pragma link "GR32_Image"
#pragma resource "*.dfm"
#pragma resource "alpha.res"

#pragma package(smart_init)
TForm1 *Form1;
TSystem *Fruit;
TSystem *Display;
TSystem *Video;
Sample *Sound;
#define UNLOCKED

int __fastcall NumericSort(TStringList *List, int index1, int index2)
{
char *ptr1 = List->Strings[index1].c_str();
char *ptr2 = List->Strings[index2].c_str();
int nbr1, nbr2;

  ptr1 += List->Strings[index1].Length() - 1;
  ptr2 += List->Strings[index2].Length() - 1;
  
  while ( *ptr1 >= '0' && *ptr1 <= '9' )
    ptr1--;
  ptr1++;

  while ( *ptr2 >= '0' && *ptr2 <= '9' )
    ptr2--;
  ptr2++;

  nbr1 = AnsiString(ptr1).ToIntDef(0);
  nbr2 = AnsiString(ptr2).ToIntDef(0);

  if ( nbr1 > nbr2 )
    return 1;
  else if ( nbr1 < nbr2 )
    return -1;
  else
    return 0;
}

const char *arch_types[] = {
  "Unknown",
  "MPU3",
  "MPU4",
  "IMPACT",
  "M1AB",
  "MPS2",
  "ACE1",
  "SCORPION2",
  "DOTMATRIX",
  "SYSTEM80",
  "SYS5",
  "MPU4VIDEO",
  "SCORPION1",
  "PROCONN",
  "spACE"
};

static int coinmasks[] = { 0x08, 0x0c, 0x0b, 0x0e, 0x0d, 0x1a, 0x1f,
                           0x10, 0x38, 0xd0, 0xb0, 0x58, 0xf8,
                           0x58, 0xf8,
                           0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};

char buffer[30000];

void LayoutChanged(void)
{
  Form1->LayoutChanged();
}
void UpdateMeters(void)
{
  Form1->UpdateMeters();
}
//---------------------------------------------------------------------------
int __stdcall CustomSortProc(long Item1, long Item2, long ParamSort)
{
int ret;

  switch ( ParamSort ) {
    case 0:
      ret = lstrcmp(((TListItem *)Item1)->Caption.c_str(),
                     ((TListItem *)Item2)->Caption.c_str());
      break;
    case 1:
    case 2:
    case 7:
      ret = lstrcmp(((TListItem *)Item1)->SubItems->Strings[ParamSort-1].c_str(),
                     ((TListItem *)Item2)->SubItems->Strings[ParamSort-1].c_str());
      break;
    default:
      if ( atof(((TListItem *)Item1)->SubItems->Strings[ParamSort-1].c_str()) >=
           atof(((TListItem *)Item2)->SubItems->Strings[ParamSort-1].c_str()) )
        ret = -1;
      else
        ret = 1;
      break;
  }
  ret = ret * Form1->sortstat[ParamSort];

  return ret;
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
unsigned long a, b;
int loop;
char buffer[10];
bool result;

  TRegIniFile *RegFile = new TRegIniFile("SOFTWARE\\CJW");
  KeyPreview = TRUE;
  State = 0;

  RegisterClass(__classid(TLamp));
  RegisterClass(__classid(TLamp2));
  RegisterClass(__classid(TImage));
  RegisterClass(__classid(TSevenSegment));
//  RegisterClass(__classid(TLEDLabel));
  RegisterClass(__classid(TzNWColorBtn));
  RegisterClass(__classid(TzcShapeBitColBtn));
  RegisterClass(__classid(TCheckBox));
  RegisterClass(__classid(TLabel));
  RegisterClass(__classid(TzLed));
  RegisterClass(__classid(TReel));
  RegisterClass(__classid(TFrame2));
  RegisterClass(__classid(TAlpha));
  RegisterClass(__classid(TDiscReel));
  RegisterClass(__classid(TBandReel));
  RegisterClass(__classid(TFancyReel));
  RegisterClass(__classid(TDotmatrix));
  RegisterClass(__classid(TVideo));
  RegisterClass(__classid(TACELED));
  RegisterClass(__classid(TPROCONLED));
  RegisterClass(__classid(TBFMAlpha));
  ResName = "";
  GameName[0] = 0;
  RealName[0] = 0;
  CharLoaded = false;
  MonitorActive = false;
  SampleFreq = 16000;
  db_fp = NULL;
  PathName = ExtractFilePath(Application->ExeName);
  ROMList = new TStringList();
  DOTROMList = new TStringList();
  VIDROMList = new TStringList();
  GetVolumeInformation("c:\\", NULL, 0, &sn, &a, &b, NULL, 0);
  layoutsn = sn;
  SoundList = new TStringList();
  Sound = new Sample();
  SampleRate = SAMPLERATE;
  if ( Sound->Init(Handle, &SampleRate) == false )
  {
    Application->MessageBox("DirectSound initialisation failed.", "Error", MB_OK | MB_ICONWARNING);
    delete Sound;
    Sound = NULL;
    NoSound = true;
  } else {
    NoSound = false;
    if ( SampleRate != SAMPLERATE ) {
      sprintf(buffer,"Sampling rate reduced to %dbps", SampleRate);
      Application->MessageBox(buffer, "Warning", MB_OK | MB_ICONWARNING);
    }
  }
  group.ptr = NULL;
  ReleaseGroup();
  VTP = RegFile->ReadInteger("MFME","VTP",0);
  ScanDir = RegFile->ReadString("MFME","BrowseDir", PathName);
  for ( loop = 0; loop < 10; loop++ ) {
    sprintf(buffer,"history%d", loop);
    history[loop] = RegFile->ReadString("MFME", AnsiString(buffer), "");
  }
  UpdateHistory();
  RegFile->RootKey = HKEY_LOCAL_MACHINE;
  result = RegFile->OpenKey("Software\\Microsoft\\Windows NT",false);
  if ( !result )
    result = RegFile->OpenKey("Software\\Microsoft\\Windows",false);
  if ( !result ) {
    Application->MessageBox("Registry read failed", "Error", MB_OK | MB_ICONWARNING);
  } else {
    ID = RegFile->ReadString("CurrentVersion","ProductName", PathName);
    Organisation = RegFile->ReadString("CurrentVersion","RegisteredOrganisation", PathName);
    User = RegFile->ReadString("CurrentVersion","RegisteredOwner", PathName);
  }
  CurrGamePath = ScanDir;
  SetCurrentDirectory(ScanDir.c_str());
  ForceCurrentDirectory = true;
  sprintf( buffer, "%8d", VTP);
  VTPmeter->Text = AnsiString(buffer);
  RegFile->Free();
  for ( loop = 0; loop < 8; loop++ )
    sortstat[loop] = 1;
  DoorSwitch = 255;
  CashSwitch = 255;
  RefillSwitch = 255;
  spacing = 1;
  Throttle = 0;
  done = true;
  MainPanel = new TCanvasPanel(this);
  MainPanel->Parent = this;
  MainPanel->Align = alClient;
  MainPanel->OnMouseDown = MainPanelMouseDown;
  MainPanel->OnMouseMove = MainPanelMouseMove;
  MainPanel->OnMouseUp = MainPanelMouseUp;
  MainPanel->OnDragOver = MainPanelDragOver;
  MainPanel->OnDragDrop = MainPanelDragDrop;
//  MainPanel->FullRepaint = false;
  MainPanel->SendToBack();
  MainCanvas = new TCanvas();
  MainDebug = new TDebugForm(this);
  MatrixDebug = new TDebugForm(this);
  VideoDebug = new TDebugForm(this);
  Screen->Cursor = crDefault;
  OpenMouse();
  reeldrag = false;
  Thumbnail = new TImage32(Panel1);
  Thumbnail->Parent = Panel1;
  Thumbnail->BitmapAlign = baCenter;
  Thumbnail->Bitmap->StretchFilter = sfLinear2;
  Thumbnail->ScaleMode = smResize;
  Thumbnail->SetBounds( 16, 24, 274, 235);
  filters = new TStringList;
  filters->Sorted = true;
  filters->Duplicates = dupIgnore;
  games = new TList;
  CopyType = 0;
//  LoadGameDB();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckBoxClick(TObject *Sender)
{

  if ( ((TCheckBox *)Sender)->Checked ) {
    MatrixChange( Sender, true);
  } else {
    MatrixChange( Sender, false);
  }
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadMPU3ROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "MPU3 ROM images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( OpenDialog1->Files->Count ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = BARCREST_MPU3;
      Sound->SetSampleRate(64000);
      Fruit = new TMPU3(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadMPU4ROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "MPU4 ROM images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( OpenDialog1->Files->Count ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = BARCREST_MPU4;
      Sound->SetSampleRate(16000);
      Fruit = new TMPU4(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShapeMouseDown(TObject *Sender,

      TMouseButton Button, TShiftState Shift, int X, int Y)
{
tagPOINT tp, P;
String type;
int TX,TY;

  if ( EditMode ) {
    P = Classes::Point(X,Y);
    tp = MainPanel->ScreenToClient(((TControl *)Sender)->ClientToScreen(P));
    TX = tp.x;
    TY = tp.y;

    if ( Button == mbLeft ) {
      if ( Shift.Contains(ssShift) ) {
        if ( IsInGroup(Sender) )
          RemoveFromGroup(Sender);
        else
          AddToGroup(Sender);
      } else {
        if ( Shift.Contains(ssCtrl) ) {
          if ( Sender->ClassNameIs("TLamp2") ||
               ( Sender->ClassNameIs("TzcShapeBitColBtn") &&
                 _HasLamp(((TControl *)Sender)->Tag)) )
            ((TControl *)Sender)->BeginDrag(TRUE,0);
        } else {
          FMove = true;
          BiasX = X;
          BiasY = Y;
          if ( IsInGroup(Sender) ) {
            if ( FResize ) {
              FResizing = true;
              FMove = false;
            } else {
              BiasX = TX - group.left;
              BiasY = TY - group.top;
            }
          } else if ( PropertiesForm->Visible && Sender != PropertiesForm->sel_ptr ) {
            menu_ptr = (TControl *)Sender;
            PropertiesForm->Update();
          }
        }
      }
    } else if ( Button == mbRight && !Shift.Contains(ssLeft) ) {
      tp = MainPanel->ClientOrigin;
      if ( IsInGroup( Sender ) ) {
        Apply1->Enabled = true;
        switch ( CopyType ) {
          case 0:
            Apply1->Caption = "Apply";
            Apply1->Enabled = false;
            break;
          case 1:
            Apply1->Caption = "Apply Text";
            break;
          case 2:
            Apply1->Caption = "Apply Font";
            break;
          case 4:
            Apply1->Caption = "Apply Size";
            break;
          case 8:
            Apply1->Caption = "Apply Shape";
            break;
          case 16:
            Apply1->Caption = "Apply Colour";
            break;
          case 31:
            Apply1->Caption = "Apply All";
            break;
        }
        if ( group.items == 2 )
          Swap->Enabled = true;
        else
          Swap->Enabled = false;
        AlignmentMenu->Popup(tp.x + TX, tp.y + TY);
      } else {
        menu_ptr = (TControl *)Sender;
        type = menu_ptr->ClassName();
        Apply2->Enabled = true;
        switch ( CopyType ) {
          case 0:
            Apply2->Caption = "Apply";
            Apply2->Enabled = false;
            break;
          case 1:
            Apply2->Caption = "Apply Text";
            break;
          case 2:
            Apply2->Caption = "Apply Font";
            break;
          case 4:
            Apply2->Caption = "Apply Size";
            break;
          case 8:
            Apply2->Caption = "Apply Shape";
            break;
          case 16:
            Apply2->Caption = "Apply Colour";
            break;
          case 31:
            Apply2->Caption = "Apply All";
            break;
        }
        if ( PropertiesForm->Visible && Sender != PropertiesForm->sel_ptr ) {
            menu_ptr = (TControl *)Sender;
            PropertiesForm->Update();
        }
        PropertiesMenu->Popup(tp.x + TX, tp.y + TY);
      }
    }
  }
  if ( !EditMode || GeneralConfig->ActiveInEdit->Checked ) {
    if ( Button == mbLeft ) {
      if ( Sender->ClassNameIs("TzcShapeBitColBtn") ||
           (Sender->ClassNameIs("TLamp2") && (((TControl *)Sender)->Tag & (_ButtonMask | _CoinMask))) )
        MatrixChange( Sender, true);
      if ( Sender->ClassNameIs("TVideo") )
        MatrixChange( Sender, true);
      if ( GeneralConfig->Draggablereels->Checked && !EditMode ) {
        if ( Sender->ClassNameIs("TFancyReel") ) {
          reelpos = ((TFancyReel *)Sender)->Position;
          reeldrag = true;
          reeldragX = Y;
        } else if ( Sender->ClassNameIs("TReel") ) {
          reelpos = ((TReel *)Sender)->Position;
          reeldrag = true;
          reeldragX = Y;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ShapeMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
struct Points *pptr;

  if ( EditMode ) {
    if ( FResizing ) {
      pptr = group.ptr;
      DrawFrame(pptr->TheRect);
      Application->ProcessMessages();
      pptr->comp->SetBounds( pptr->TheRect.Left + 2,
                             pptr->TheRect.Top + 2,
                             pptr->TheRect.Right - pptr->TheRect.Left - 3,
                             pptr->TheRect.Bottom - pptr->TheRect.Top - 3);
      FResizing = false;
//      MainPanel->Repaint();
      Application->ProcessMessages();
      DrawFrame(pptr->TheRect);
      CalculateGroupSize();
    } else {
      if ( group.ptr != NULL ) {
        for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
          DrawFrame(pptr->TheRect);
        Application->ProcessMessages();
        for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
          pptr->comp->SetBounds( pptr->TheRect.Left + 2,
                                 pptr->TheRect.Top + 2,
                                 pptr->comp->Width,
                                 pptr->comp->Height);
          if ( !pptr->comp->ClassNameIs("TFrame2") )
            pptr->comp->BringToFront();
        }
        Application->ProcessMessages();
        for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
          DrawFrame(pptr->TheRect);
//        MainPanel->Repaint();
      }// else {
//        if ( FMove && Sender->ClassNameIs("TLamp2") && LampMode ) {
//          ExtractBitmapClick((TLamp2 *)Sender);
//        }
//      }
      FMove = false;
    }
  }
  if ( !EditMode || GeneralConfig->ActiveInEdit->Checked ) {
    if ( Button == mbLeft ) {
      if ( Sender->ClassNameIs("TzcShapeBitColBtn") ||
           (Sender->ClassNameIs("TLamp2") && (((TControl *)Sender)->Tag & (_ButtonMask | _CoinMask))) )
        MatrixChange( Sender, false);
      if ( Sender->ClassNameIs("TVideo") )
        MatrixChange( Sender, false);
      if ( reeldrag ) {
        if ( Sender->ClassNameIs("TFancyReel") ) {
          ((TFancyReel *)Sender)->Position = reelpos;
          reeldrag = false;
        } else if ( Sender->ClassNameIs("TReel") ) {
          ((TReel *)Sender)->Position = reelpos;
          reeldrag = false;
        }
      }
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ShapeMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)

{
struct Points *pptr;
TCursor Resize_cursors[16] = {
  crArrow, crSizeWE, crSizeWE, crArrow,
  crSizeNS, crSizeNWSE, crSizeNESW, crArrow,
  crSizeNS, crSizeNESW, crSizeNWSE, crArrow,
  crArrow, crArrow, crArrow, crArrow
};
tagPOINT tp, P;
int new_edge;
int Xshift, Yshift;
TControl *ptr = (TControl *)Sender;
int new_top, new_left;
int reelY = Y;

  P = Mouse->CursorPos;
  tp = MainPanel->ScreenToClient(P);
  X = tp.x;
  Y = tp.y;

  if (  Shift.Contains(ssLeft) ) {
    if ( !FResizing ) {
      if ( FMove ) {
        LayoutChanged();
        if ( group.ptr != NULL && IsInGroup(Sender) ) {
          new_top = Y - BiasY;
          if ( YGrid )
            new_top = (( new_top / YGrid) * YGrid) - 2;
          new_left = X - BiasX;
          if ( XGrid )
            new_left = (( new_left / XGrid ) * XGrid) - 2;
          if ( new_left < 2 )
            new_left = 2;
          if ( new_left + group.right - group.left > MainPanel->Width - 2 )
            new_left = MainPanel->Width - 2 - (group.right - group.left);
          if ( new_top < 2 )
            new_top = 2;
          if ( new_top + group.bottom - group.top > MainPanel->Height - 2 )
            new_top = MainPanel->Height - 2 - (group.bottom - group.top);
          Yshift = new_top - group.top;
          Xshift = new_left - group.left;

          group.top += Yshift;
          group.left += Xshift;
          group.bottom += Yshift;
          group.right += Xshift;
          for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
            DrawFrame(pptr->TheRect);
            pptr->TheRect.Top += Yshift;
            pptr->TheRect.Left += Xshift;
            pptr->TheRect.Bottom += Yshift;
            pptr->TheRect.Right += Xshift;
            DrawFrame(pptr->TheRect);
            if ( PropertiesForm->Visible && pptr->comp == PropertiesForm->sel_ptr ) {
              PropertiesForm->Updateposition(Rect(pptr->TheRect.Left + 3,pptr->TheRect.Top + 3,
                                             pptr->TheRect.Right,pptr->TheRect.Bottom));
            }
          }
        } else {
          new_top = Y - BiasY;
          new_left = X - BiasX;
          if ( YGrid )
            new_top = (new_top / YGrid) * YGrid;
          if ( XGrid )
            new_left = (new_left / XGrid) * XGrid;
          if ( new_left < 2 )
            new_left = 2;
          if ( new_left + ptr->Width > MainPanel->Width - 1 )
            new_left = MainPanel->Width - 1 - ptr->Width;
          if ( new_top < 2 )
            new_top = 2;
          if ( new_top + ptr->Height > MainPanel->Height - 1 )
            new_top = MainPanel->Height - 1 - ptr->Height;
          ptr->Top = new_top;
          ptr->Left = new_left;
          if ( PropertiesForm->Visible && ptr == PropertiesForm->sel_ptr )
            PropertiesForm->Updateposition(ptr->BoundsRect);
        }
      }
    } else  {
      pptr = group.ptr;
      if ( FResize & 1 ) {
        if ( pptr->TheRect.Right - X < 15 )
          new_edge = pptr->TheRect.Right - 15;
        else
          new_edge = X;
        if ( new_edge < 0 )
          new_edge = 0;
        DrawFrame(pptr->TheRect);
        pptr->TheRect.Left = new_edge;
        DrawFrame(pptr->TheRect);
      } else if ( FResize & 2 ) {
        if ( X - pptr->TheRect.Left < 15 )
          new_edge = pptr->TheRect.Left + 15;
        else
          new_edge = X;
        if ( new_edge > MainPanel->Width )
          new_edge = MainPanel->Width;
        DrawFrame(pptr->TheRect);
        pptr->TheRect.Right = new_edge;
        DrawFrame(pptr->TheRect);
      }
      if ( FResize & 4 ) {
        if ( pptr->TheRect.Bottom - Y < 15 )
          new_edge = pptr->TheRect.Bottom - 15;
        else
          new_edge = Y;
        if ( new_edge < 0 )
          new_edge = 0;
        DrawFrame(pptr->TheRect);
        pptr->TheRect.Top = new_edge;
        DrawFrame(pptr->TheRect);
      } else if ( FResize & 8 ) {
        if ( Y - pptr->TheRect.Top < 15 )
          new_edge = pptr->TheRect.Top + 15;
        else
          new_edge = Y;
        if ( new_edge > MainPanel->Height )
          new_edge = MainPanel->Height;
        DrawFrame(pptr->TheRect);
        pptr->TheRect.Bottom = new_edge;
        DrawFrame(pptr->TheRect);
      }
      if ( PropertiesForm->Visible && pptr->comp == PropertiesForm->sel_ptr )
        PropertiesForm->Updateposition(Rect(pptr->TheRect.Left + 3,pptr->TheRect.Top + 3,
                                             pptr->TheRect.Right,pptr->TheRect.Bottom));
    }
  } else {
    FResize = 0;
    pptr = group.ptr;
    if ( EditMode ) {
      if ( pptr != NULL ) {
        if ( pptr->next_ptr == NULL && pptr->Resizable ) {
          if ( IsInGroup(Sender) ) {
            if ( X - pptr->TheRect.Left < 5 )
              FResize |= 1;
            else if ( pptr->TheRect.Right - X < 5 )
              FResize |= 2;
            if ( Y - pptr->TheRect.Top < 5 )
              FResize |= 4;
            else if ( pptr->TheRect.Bottom - Y < 5 )
              FResize |= 8;
          }
        }
      } else {
//        if ( PropertiesForm->Visible && ptr != menu_ptr ) {
//          menu_ptr = ptr;
//          PropertiesForm->Update();
//        }
      }
    }
    if ( FResize )
      Screen->Cursor = Resize_cursors[FResize];
    else if ( Sender->ClassNameIs("TVideo") ) {
      Screen->Cursor = crNone;
    } else
      Screen->Cursor = crDefault;
  }
  if ( reeldrag ) {
    int diff = (reeldragX - reelY) / 2;
    if ( Sender->ClassNameIs("TFancyReel") ) {
      diff = diff % ((TFancyReel *)Sender)->TotalSteps;
      if ( ((TFancyReel *)Sender)->Reversed )
        diff = -diff;
      ((TFancyReel *)Sender)->Position = reelpos + diff;
    } else if ( Sender->ClassNameIs("TReel") ) {
      diff = diff % ((TReel *)Sender)->TotalSteps;
      if ( ((TReel *)Sender)->Reversed )
        diff = -diff;
      ((TReel *)Sender)->Position = reelpos + diff;
    }
  }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::SaveGameClick(TObject *Sender)
{

  SaveDialog1->Filter = "MFME Game (*.gam)|*.gam";
  SaveDialog1->DefaultExt = String("gam");
  SaveDialog1->Options << ofOverwritePrompt;
  SaveDialog1->FileName = "";

  if ( Fruit )
    Fruit->Suspend();
  if ( Display )
    Display->Suspend();
  if ( Video )
    Video->Suspend();

  if ( SaveDialog1->Execute() ) {
    Game = ExtractFileName(SaveDialog1->FileName);
    SaveGameFile(SaveDialog1->FileName);
    CurrGamePath = ExtractFilePath(SaveDialog1->FileName);
//    if ( CharName != "" && !FileExists(CharName) ) {
//      fd = open( CharName.c_str(), O_WRONLY | O_CREAT | O_BINARY);
//      write( fd, Form1->Character, 72);
//      close(fd);
//    }
//    if ( ROMname != "" && !FileExists(ROMname) ) {
//      fd = open( ROMname.c_str(), O_WRONLY | O_CREAT | O_BINARY);
//      write( fd, &Fruit->board.memory[0], ROMsize);
//      close(fd);
//    }
  }
  if ( Fruit )
    Fruit->Resume();
  if ( Display )
    Display->Resume();
  if ( Video )
    Video->Resume();

}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveGameFile(String name)
{
int count;
FILE *fp;
Char buffer[9];

  SetVTP();
  chmod(name.c_str(),S_IWRITE);
  fp = fopen( name.c_str(), "w");
  if ( fp != NULL ) {
    fprintf( fp, "System %s\n", arch_types[Arch]);
    BinToStr(DIP1, buffer);
    fprintf( fp, "DIP 1 %s\n", buffer);
    if ( Arch != JPM_IMPACT ) {
      BinToStr(DIP2, buffer);
      fprintf( fp, "DIP 2 %s\n", buffer);
    }
    if ( Arch == JPM_SYS80 ) {
      BinToStr(DIP3, buffer);
      fprintf( fp, "DIP 3 %s\n", buffer);
    }
    if ( Speed )
      fprintf( fp, "Delay %d\n", Speed);
    if ( Speed2 )
      fprintf( fp, "DMDelay %d\n", Speed2);
    if ( Throttle )
      fprintf( fp, "Throttle %d\r\n", Throttle);
    if ( Arch == BARCREST_MPU4 ) {
      fprintf( fp, "Volume %d\n", Volume_Type);
    }
    if ( Arch == BARCREST_MPU4 || Arch == JPM_IMPACT || Arch == MAYGAY_M1A ||
         Arch == BFM_SCORPION1 || Arch == BFM_SCORPION2 ) {
      if ( Stake )
        fprintf( fp, "Stake %d\n", Stake - 1);
      if ( Jackpot )
        fprintf( fp, "Jackpot %d\n", Jackpot - 1);
      if ( Percent )
        fprintf( fp, "Percentage %d\n", Percent - 1);
    }
    fprintf( fp, "SetPercent %d\n", Percentage);
    fprintf( fp, "TotalIn %d\n", TotalIn);
    fprintf( fp, "TotalOut %d\n", TotalOut);
    for ( int i = 0; i < 31; i++ ) {
      if ( SEC_Strings[i][0]  )
        fprintf( fp, "SEC%d \"%s\" %d\n", i+1, SEC_Strings[i], SEC_Counters[i]);
    }
    fprintf( fp, "Played %d\n", TimesPlayed);
    if ( ResName != "" ) {
        fprintf( fp, "Layout %s\n", ResName.c_str());
    }
    if ( SoundList != NULL )
      for ( count = 0; count < SoundList->Count; count++ )
        fprintf( fp, "Sound %s\n", ExtractFileName(SoundList->Strings[count]).c_str());
    for ( count = 0; count < DOTROMList->Count; count++ )
      fprintf( fp, "DOTROM %s\n", ExtractFileName(DOTROMList->Strings[count].c_str()));
    for ( count = 0; count < VIDROMList->Count; count++ )
      fprintf( fp, "VIDROM %s\n", ExtractFileName(VIDROMList->Strings[count].c_str()));
    for ( count = 0; count < ROMList->Count; count++ )
      fprintf( fp, "ROM %s\n", ExtractFileName(ROMList->Strings[count].c_str()));
    if ( RAMname != "" )
      fprintf( fp, "RAM %s\n", ExtractFileName(RAMname));
    fclose(fp);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteItem(TObject *Item)
{
String type;
int nbr, X, Y;
int tag;

  tag = ((TComponent *)Item)->Tag;
  type = String(((TControl *)Item)->ClassName());
  if ( type == "TLamp2" ) {
    RemoveLamp( _LampFromTag(tag), Item);
    if ( _HasLamp2(tag) )
      RemoveLamp( _Lamp2FromTag(tag), Item);
    shortcuts[_ShortcutFromTag(tag)] = NULL;
  } else if ( type == "TLabel" ) {
    RemoveLamp( _LampFromTag(tag), Item);
  } else if ( type == "TAlpha" ) {
    display[tag] = NULL;
  } else if ( type == "TDotmatrix" ) {
    dotmatrix = NULL;
  } else if ( type == "TACELED" ) {
    aceled = NULL;
  } else if ( type == "TBFMAlpha" ) {
    bfmalpha = NULL;
  } else if ( type == "TPROCONLED" ) {
    proconnled = NULL;
  } else if ( type == "TReel" ) {
    nbr = _ReelFromTag(tag);
    if ( ((TReel *)Item)->CustomLamps ) {
      for ( int i = 0; i < 3; i++ )
        RemoveLamp( ((TReel *)Item)->GetLampNumber(i), Item);
    }
    Reels[nbr] = NULL;
    ReelType[nbr] = RT_NONE;
  } else if ( type == "TFancyReel" ) {
    nbr = _ReelFromTag(tag);
    for ( int i = 0; i < 5; i++ )
      RemoveLamp( ((TFancyReel *)Item)->GetLampNumber(i), Item);
    Reels[nbr] = NULL;
    ReelType[nbr] = RT_NONE;
  } else if ( type == "TBandReel" ) {
    nbr = _ReelFromTag(tag);
    for ( int i = 0; i < 5; i++ )
      RemoveLamp( ((TBandReel *)Item)->GetLampNumber(i), Item);
    Reels[nbr] = NULL;
    ReelType[nbr] = RT_NONE;
  } else if ( type == "TDiscReel" ) {
    nbr = _ReelFromTag(tag);
    for ( int i = 0; i < 24; i++ ) {
      if ( ((TDiscReel *)Item)->GetLamp(i) < NBR_LAMPS )
        RemoveLamp( ((TDiscReel *)Item)->GetLamp(i), Item);
    }
    Reels[nbr] = NULL;
    ReelType[nbr] = RT_NONE;
  } else if ( type == "TSevenSegment" ) {
    TSevenSegment *ptr = (TSevenSegment *)Item;
    if ( ptr->Programmable ) {
      for ( int i = 0; i < 16; i++ ) {
        if ( ptr->GetMasks(i) < 256 && ptr->GetMasks(i) > 0 )
          RemoveLamp( ptr->GetMasks(i), ptr);
      }
    }
    led_display[tag].digit = NULL;
  } else if ( type == "TCheckBox" || type == "TzcShapeBitColBtn") {
    if ( type == "TCheckBox" )
      MatrixChange( Item, false);
    nbr = _ButtonFromTag(tag);
    Buttons[nbr] = NULL;
    shortcuts[_ShortcutFromTag(tag)] = NULL;
    if ( _HasLamp(tag) )
      RemoveLamp( _LampFromTag(tag), Item);
    if ( _HasLamp2(tag) )
      RemoveLamp( _Lamp2FromTag(tag), Item);
    if ( type == "TzcShapeBitColBtn" ) {
      if ( ((TzcShapeBitColBtn *)Item)->Lamp2 )
        RemoveLamp( ((TzcShapeBitColBtn *)Item)->Lamp2 - 1, Item);
    }
  } else if ( type == "TzLed" ) {
    if ( tag & _LEDLampMask ) {
      RemoveLamp( _LampFromTag(tag), Item);
    } else {
      X = _DigitMask(tag);
      if ( X < 32 ) {
        Y = _SegmentMask(tag);
        led_display[X].led[Y] = NULL;
      }
    }
  } else if ( type == "TVideo" )
    screen = NULL;

  if ( Item == menu_ptr && PropertiesForm->Visible )
    PropertiesForm->Hide();
  if ( IsInGroup(Item) )
    RemoveFromGroup(Item);
  delete Item;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Delete1Click(TObject *Item)
{
  LayoutChanged();
  DeleteItem(menu_ptr);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PropertiesClick(TObject *Sender)
{
  PropertiesForm->Update();
  PropertiesForm->Visible = TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditModeClick(TObject *Sender)
{
  if ( EditMode )
    SetEditMode(false);
  else if ( NotLocked(true) )
    SetEditMode(true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetEditMode(bool state)
{
bool Hints;

  if ( state ) {
    EditMenu->Checked = true;
    EditMode = true;
    MeterPanel->Color = clRed;
    Hints = true;
    SetOutlines(true);
  } else {
    EditMenu->Checked = false;
    EditMode = false;
    LampMode = false;
    MeterPanel->Color = clBtnFace;
    Hints = false;
    ReleaseGroup();
    SetOutlines(false);
    PropertiesForm->Hide();
  }
  ShowHint = Hints;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::NewLampMatrixExt1Click(TObject *Sender)
{
int X, Y;

  for ( X = 16; X < 24; X++ ) {
    for ( Y = 0; Y < 5; Y++ ) {
      if ( Lamps[Y + X * 8] == NULL )
        CreateNewLamp(Y + X * 8,32 + Y + X * 6,BiasX + (X - 16) * 20,BiasY + Y * 20);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewLabelClick(TObject *Sender)
{
TLabel *new_ptr;

  LayoutChanged();
  new_ptr = new TLabel(MainPanel);
  new_ptr->Top = BiasY;
  new_ptr->Left = BiasX;
  new_ptr->AutoSize = true;
  new_ptr->WordWrap = true;
  new_ptr->Layout = tlCenter;
  new_ptr->Alignment = taCenter;
  new_ptr->Caption = "Label";
  new_ptr->Transparent = true;
  new_ptr->OnMouseDown = ShapeMouseDown;
  new_ptr->OnMouseMove = ShapeMouseMove;
  new_ptr->OnMouseUp = ShapeMouseUp;
  new_ptr->Parent = MainPanel;
}
//---------------------------------------------------------------------------



void __fastcall TForm1::NewCheckBoxClick(TObject *Sender)
{
int count;

  LayoutChanged();
  for ( count = 0; count < 48; count++ )
    if ( Buttons[count] == NULL ) {
      CreateCheckBox(count, BiasX, BiasY, "");
      break;
    }
}

//---------------------------------------------------------------------------
TControl * __fastcall TForm1::CreateCheckBox(int nbr, int left, int top, String caption)

{
TCheckBox *new_ptr;

  LayoutChanged();
  new_ptr = new TCheckBox(MainPanel);
  if ( caption == "" )
    new_ptr->Caption = IntToStr(nbr);
  else
    new_ptr->Caption = caption;
  new_ptr->SetBounds( left, top, 20 + 8 * new_ptr->Caption.Length(), 25);
  new_ptr->Color = clBtnFace;
  new_ptr->OnClick = CheckBoxClick;
  new_ptr->OnMouseDown = ShapeMouseDown;
  new_ptr->OnMouseMove = ShapeMouseMove;
  new_ptr->OnMouseUp = ShapeMouseUp;
  new_ptr->Parent = MainPanel;
  new_ptr->Tag = nbr << 8;
  if ( nbr > 31 )
    new_ptr->Hint = "AUX" + IntToStr((nbr-32)/8) + " , " + IntToStr(nbr%8);
  else
    new_ptr->Hint = IntToStr(nbr);
  if ( Buttons[nbr] != NULL ) {
    DeleteItem(Buttons[nbr]);
  }
  Buttons[nbr] = new_ptr;

  return new_ptr;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::NewDisplayClick(TObject *Sender)
{
Graphics::TBitmap *ptr;
TAlpha *new_ptr;
int i;

  for ( i = 0; i < 3; i++ ) {
    if ( display[i] == NULL ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromResourceName((int)HInstance,"ALPHA");
      new_ptr = new TAlpha(MainPanel);
      new_ptr->Tag = i;
      new_ptr->Top = BiasY;
      new_ptr->Left = BiasX;
      new_ptr->SetBitmap(ptr);
      new_ptr->OnMouseDown = TImageMouseDown;
      new_ptr->OnMouseMove = TImageMouseMove;
      new_ptr->OnMouseUp   = TImageMouseUp;
      new_ptr->Parent = MainPanel;
      LayoutChanged();
      display[i] = new_ptr;
      delete ptr;
      break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewLEDClick(TObject *Sender)
{
int count, loop, X;
TSevenSegment *led_ptr;

  for ( count = 0; count < 40; count+= 8 ) {
    if ( led_display[count].digit == NULL ) {
      for ( loop = 0; loop < 8; loop++ ) {
        if ( led_display[count+loop].digit == NULL ) {
          X = 24 * loop;
          if ( IsInside( Rect(BiasX + X, BiasY, BiasX + X + 24, BiasY + 32),
                         Rect(0,0,MainPanel->Width,MainPanel->Height) ) ) {
            led_ptr = new TSevenSegment(MainPanel);
            LayoutChanged();
            led_ptr->SetBounds( BiasX + X, BiasY, 24, 32);
            led_ptr->Space = 1;
            led_ptr->Thickness = 3;
            led_ptr->HorzSpace = 10;
            led_ptr->VertSpace = 6;
            led_ptr->Value = 0xffff;
            led_ptr->OnMouseDown = ShapeMouseDown;
            led_ptr->OnMouseMove = ShapeMouseMove;
            led_ptr->OnMouseUp = ShapeMouseUp;
            led_ptr->Parent = MainPanel;
            led_ptr->Hint = IntToStr(count+loop);
            led_ptr->Tag = count+loop;
            led_display[count+loop].digit = led_ptr;
          }
        }
      }
      break;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::NewLampMatrixStdClick(TObject *Sender)
{
int X, Y;

  for ( X = 0; X < 16; X++ ) {
    for ( Y = 0; Y < 8; Y++ ) {
      if ( Lamps[Y + X * 8] == NULL )
        CreateNewLamp(Y + X * 8,Y + X * 8,BiasX + X * 20,BiasY + Y * 20);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewButtonMatrix1Click(TObject *Sender)
{
int X, Y;

  for ( X = 0; X < 6; X++ ) {
    for ( Y = 0; Y < 8; Y++ ) {
      if ( Buttons[Y+X*8] == NULL )
        CreateBitmapButton(Y+X*8, BiasX + Y * 80, BiasY + X * 30, "");
    }
  }
}
//---------------------------------------------------------------------------
TControl * __fastcall TForm1::ButtonCheckBox(TControl *ptr)
{
String caption;
int left, top;
int Tag;
int nbr;

  Tag = ptr->Tag;
  top = ptr->Top;
  left = ptr->Left;
  if ( ptr->ClassNameIs("TzcShapeBitColBtn") ) {
    LayoutChanged();
    caption = ((TzcShapeBitColBtn *)ptr)->Caption;
    DeleteItem(ptr);
    return CreateCheckBox(_ButtonFromTag(Tag),left,top,caption);
  } else if ( ptr->ClassNameIs("TCheckBox") ) {
    LayoutChanged();
    nbr = _ButtonFromTag(Tag);
    caption = ((TCheckBox *)ptr)->Caption;
    return CreateBitmapButton(nbr,left,top,caption);
  }
  return NULL; // shouldn't reach this
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReelEditorClick(TObject *Sender)
{
  if ( NotLocked() )
    if ( Editor == NULL )
      Editor = new TEditor(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LampToButton(TObject *Sender, TObject *Source, int X, int Y)
{
int lamp;
TColor colour;

  lamp = _LampFromTag(((TComponent *)Source)->Tag);
  if ( Source->ClassNameIs("TLamp2") ) {
    colour = ((TLamp2 *)Source)->Colour1;
    if ( _HasLamp2(((TComponent *)Source)->Tag) )
      RemoveLamp(_Lamp2FromTag(((TComponent *)Source)->Tag), Source);
  } else if ( Source->ClassNameIs("TzcShapeBitColBtn") ) {
    colour = ((TzcShapeBitColBtn *)Source)->OnColor;
    ((TComponent *)Source)->Tag &= ~_LampMask;
  }
  RemoveLamp( lamp, Source);
  if ( _HasLamp(((TComponent *)Sender)->Tag) )
    RemoveLamp(_LampFromTag(((TComponent *)Sender)->Tag), Sender);
  ((TComponent *)Sender)->Tag &= 0xefffff00;
  ((TComponent *)Sender)->Tag |= _LampMask + _LampTag(lamp);
  InsertLamp(lamp, Sender, colour,1);
  ((TzcShapeBitColBtn *)Sender)->OnColor = colour;
  ((TControl *)Source)->EndDrag(TRUE);
  LayoutChanged();
}

//---------------------------------------------------------------------------

void __fastcall TForm1::CheckLampToButton(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
  if ( Source->ClassNameIs("TLamp2") || Source->ClassNameIs("TzcShapeBitColBtn") )
    Accept = TRUE;
  else
    Accept = FALSE;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::LampEndDrag(TObject *Sender, TObject *Target,
      int X, int Y)
{
  if ( Target != NULL )
    delete Sender;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CheckLampToGraphic(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
  if ( Source->ClassNameIs("TLamp2") && Sender != Source )
    Accept = TRUE;
  else
    Accept = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::GraphicDragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
TLamp2 *ptr = (TLamp2 *)Source;
TLamp2 *target = (TLamp2 *)Sender;
int lamp;
int lamp2;

  lamp = _LampFromTag(ptr->Tag);
  RemoveLamp(lamp, Source);
  lamp2 = _Lamp2FromTag(ptr->Tag);
  if ( _HasLamp2(ptr->Tag) )
    RemoveLamp( lamp2, Source);
  lamp2 = _LampFromTag(target->Tag);
  RemoveLamp(lamp2, Sender);
  target->Tag &= 0xEFFFFF00;
  target->Tag |= _LampTag(lamp);
  target->Colour1 = ptr->Colour1;
  InsertLamp( lamp, target, ptr->Colour1, 1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MainPanelDragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
int Lamp;
TColor Colour;

  if ( Source->ClassNameIs("TzcShapeBitColBtn") ) {
    Colour = ((TzcShapeBitColBtn *)Source)->OnColor;
    ((TzcShapeBitColBtn *)Source)->OnColor = clBtnFace;
    Lamp = _LampFromTag(((TComponent *)Source)->Tag);
    ((TComponent *)Source)->Tag &= ~_LampMask;
    RemoveLamp(Lamp, Source);
    CreateNewLamp( Lamp, Lamp, X, Y, Colour);
    ((TControl *)Source)->EndDrag(TRUE);
  }

}
//---------------------------------------------------------------------------
bool __fastcall TForm1::IsInside(TRect A, TRect B)
{

  if ( A.Top > B.Top && A.Bottom < B.Bottom &&
       A.Left > B.Left && A.Right < B.Right )
    return true;
  else
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DrawFrame(TRect TheRect)
{
Windows::TPoint pt, pt2;
HDC hand = GetWindowDC(Handle);

  MainCanvas->Handle = hand;

  pt2.x = TheRect.Left;
  pt2.y = TheRect.Top;

  pt = MainPanel->ClientToScreen(pt2);
  pt.x -= Left;
  pt.y -= Top;

  MainCanvas->Pen->Mode = pmNotXor;
  MainCanvas->Pen->Style = psDot;
  MainCanvas->Pen->Color = clBlack;
  MainCanvas->MoveTo( pt.x, pt.y );
  MainCanvas->LineTo( pt.x + TheRect.Right - TheRect.Left , pt.y);
  MainCanvas->LineTo( pt.x + TheRect.Right - TheRect.Left, pt.y + TheRect.Bottom - TheRect.Top);
  MainCanvas->LineTo( pt.x, pt.y + TheRect.Bottom - TheRect.Top);
  MainCanvas->LineTo( pt.x, pt.y);
  MainCanvas->Pen->Mode = pmCopy;
  MainCanvas->Pen->Style = psSolid;
  ReleaseDC(Handle, hand);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::DeleteBlockClick(TObject *Sender)
{
  if ( group.ptr ) {
    LayoutChanged();
    while ( group.ptr ) {
      DeleteItem(group.ptr->comp);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AlignTClick(TObject *Sender)
{
struct Points *pptr;
int Highest;

  Highest = 65535;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Top < Highest )
      Highest = pptr->comp->Top;
  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Top != Highest ) {
      LayoutChanged();
      pptr->TheRect.Top -= (pptr->comp->Top - Highest);
      pptr->TheRect.Bottom -= (pptr->comp->Top - Highest);
      pptr->comp->Top = Highest;
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AlignLClick(TObject *Sender)
{
struct Points *pptr;
int LeftMost;

  LeftMost = 65535;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left < LeftMost )
      LeftMost = pptr->comp->Left;
  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left != LeftMost ) {
      LayoutChanged();
      pptr->TheRect.Left -= (pptr->comp->Left - LeftMost);
      pptr->TheRect.Right -= (pptr->comp->Left - LeftMost);
      pptr->comp->Left = LeftMost;
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AlignRClick(TObject *Sender)
{
struct Points *pptr;
int RightMost;

  RightMost = 0;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left + pptr->comp->Width > RightMost )
      RightMost = pptr->comp->Left + pptr->comp->Width;
  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left + pptr->comp->Width != RightMost ) {
      LayoutChanged();
      pptr->TheRect.Left += RightMost - (pptr->comp->Left + pptr->comp->Width);
      pptr->TheRect.Right += RightMost - (pptr->comp->Left + pptr->comp->Width);
      pptr->comp->Left = RightMost - pptr->comp->Width;
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AlignBClick(TObject *Sender)
{
struct Points *pptr;
int BottomMost;

  BottomMost = 0;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Top + pptr->comp->Height > BottomMost )
      BottomMost = pptr->comp->Top + pptr->comp->Height;
  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Top + pptr->comp->Height != BottomMost ) {
      LayoutChanged();
      pptr->TheRect.Top += BottomMost - (pptr->comp->Top + pptr->comp->Height);
      pptr->TheRect.Bottom += BottomMost - (pptr->comp->Top + pptr->comp->Height);
      pptr->comp->Top = BottomMost - pptr->comp->Height;
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddToGroup(TObject *Sender)
{
TRect R;
struct Points *new_ptr;

  R = ((TControl *)Sender)->BoundsRect;
  new_ptr = new struct Points;
  new_ptr->TheRect = Rect( R.Left - 2, R.Top - 2, R.Right + 1, R.Bottom + 1);
  if ( new_ptr->TheRect.Top < group.top )
    group.top = new_ptr->TheRect.Top;
  if ( new_ptr->TheRect.Left < group.left )
    group.left = new_ptr->TheRect.Left;
  if ( new_ptr->TheRect.Right > group.right )
    group.right = new_ptr->TheRect.Right;
  if ( new_ptr->TheRect.Bottom > group.bottom )
    group.bottom = new_ptr->TheRect.Bottom;
  new_ptr->next_ptr = group.ptr;
  new_ptr->comp = (TControl *)Sender;
  if ( Sender->ClassNameIs("TzcShapeBitColBtn") ||
       Sender->ClassNameIs("TLamp2") ||
       Sender->ClassNameIs("TzLed") ||
       Sender->ClassNameIs("TReel") ||
       Sender->ClassNameIs("TFancyReel") ||
       Sender->ClassNameIs("TFrame2") ||
       Sender->ClassNameIs("TSevenSegment") ||
       Sender->ClassNameIs("TDiscReel") ||
       Sender->ClassNameIs("TBandReel") ||
       Sender->ClassNameIs("TAlpha") ||
       Sender->ClassNameIs("TDotmatrix") ||
       Sender->ClassNameIs("TImage") )
    new_ptr->Resizable = true;
  else
    new_ptr->Resizable = false;
  DrawFrame(new_ptr->TheRect);
  group.items++;
  group.ptr = new_ptr;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReleaseGroup()
{
struct Points *pptr, *old_ptr;

  for ( pptr = group.ptr; pptr != NULL; ) {
    DrawFrame(pptr->TheRect);
    old_ptr = pptr;
    pptr = pptr->next_ptr;
    delete old_ptr;
  }
  group.ptr = NULL;
  group.top = group.left = 65535;
  group.right = group.bottom = 0;
  group.items = 0;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::IsInGroup(TObject *Sender)
{
struct Points *pptr;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr)
    if ( Sender == pptr->comp )
      break;

  if ( pptr != NULL )
    return true;
  else
    return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RemoveFromGroup(TObject *Sender)
{
struct Points **prev_ptr, *pptr;

  for ( prev_ptr = &group.ptr, pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr) {
    if ( Sender == pptr->comp )
      break;
    prev_ptr = &pptr->next_ptr;
  }

  if ( pptr != NULL ) {
    DrawFrame(pptr->TheRect);
    *prev_ptr = pptr->next_ptr;
    delete pptr;
    group.items--;
  }

  CalculateGroupSize();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AlColorClick(TObject *Sender)
{
struct Points *pptr;
int lamp;

  if ( ColorDialog1->Execute() )
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
      lamp = _LampFromTag(((TComponent *)pptr->comp)->Tag);
      if ( pptr->comp->ClassNameIs("TLamp2") ) {
        LayoutChanged();
        LampColour(lamp, pptr->comp, ColorDialog1->Color);
        ((TLamp2 *)pptr->comp)->Colour1 = ColorDialog1->Color;
      } else if ( pptr->comp->ClassNameIs("TzcShapeBitColBtn") ) {
        if ( _HasLamp(((TComponent *)pptr->comp)->Tag) ) {
          LayoutChanged();
          LampColour(lamp, pptr->comp, ColorDialog1->Color);
          ((TzcShapeBitColBtn *)pptr->comp)->OnColor = ColorDialog1->Color;
        }
      }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ResizeLeftClick(TObject *Sender)
{
struct Points *pptr;
int LeftMost;
int delta;

  LeftMost = 65535;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left < LeftMost )
      LeftMost = pptr->comp->Left;
  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left != LeftMost ) {
      LayoutChanged();
      delta = (pptr->comp->Left - LeftMost);
      pptr->TheRect.Left -= delta;
      pptr->comp->Left = LeftMost;
      pptr->comp->Width += delta;
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ResizeRightClick(TObject *Sender)
{
struct Points *pptr;
int RightMost;
int delta;

  RightMost = 0;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left + pptr->comp->Width > RightMost )
      RightMost = pptr->comp->Left + pptr->comp->Width;
  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
    if ( pptr->comp->Left + pptr->comp->Width != RightMost ) {
      LayoutChanged();
      delta = RightMost - (pptr->comp->Left + pptr->comp->Width);
      pptr->TheRect.Right += delta;
      pptr->comp->Width += delta;
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadCharacteriserClick(TObject *Sender)
{
int fd;
struct stat statbuf;

  if ( NotLocked() ) {
    OpenDialog1->Filter = "MPU4 Characteriser (*.chr)|*.chr";
    OpenDialog1->DefaultExt = AnsiString("chr");
    OpenDialog1->Options << ofFileMustExist;
    OpenDialog1->FileName = "";

    if ( OpenDialog1->Execute() ) {
      CharLoaded = true;
      CharName = ExtractFileName(OpenDialog1->FileName);
      fd = open( OpenDialog1->FileName.c_str(), O_RDONLY);
      fstat( fd, &statbuf);
      if ( statbuf.st_size == 72 ) {
        read( fd, Character, 72);
      }
      close(fd);
      LayoutChanged();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveCharacteriserClick(TObject *Sender)
{
int fd;
String CharName;

  if ( NotLocked() ) {
    SaveDialog1->Filter = "MPU4 Characteriser (*.chr)|*.chr";
    SaveDialog1->DefaultExt = AnsiString("chr");
    SaveDialog1->Options << ofOverwritePrompt;
    SaveDialog1->FileName = "";

    if ( SaveDialog1->Execute() ) {
      CharName = ExtractFileName(SaveDialog1->FileName);
      fd = open( CharName.c_str(), O_WRONLY | O_CREAT | O_BINARY);
      if ( fd == -1 )
        Application->MessageBox("Cannot open file", "Error", MB_OK | MB_ICONWARNING);
      else {
        write( fd, Character, 72);
        close(fd);
      }
    }
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::UnusedLampsClick(TObject *Sender)
{
int nbr,X,Y;

  for ( nbr = 0; nbr < 128; nbr++ ) {
    if ( Lamps[nbr] == NULL )
        CreateNewLamp(nbr,nbr,BiasX + (nbr / 8) * 20,BiasY + (nbr % 8) * 20, clYellow);
  }
  if ( Extender_Type == 1 ) {
    for ( X = 16; X < 24; X++ ) {
      for ( Y = 0; Y < 5; Y++ ) {
        if ( Lamps[Y + X * 8] == NULL )
          CreateNewLamp(Y + X * 8,32 + Y + X * 6,BiasX + X * 20,BiasY + Y * 20,clYellow);
      }
    }
  } else if ( Extender_Type > 1 ) {
    for ( X = 16; X < 32; X++ ) {
      for ( Y = 0; Y < 6; Y++ ) {
        if ( Lamps[Y + X * 8] == NULL )
          CreateNewLamp(Y + X * 8,32 + Y + X * 6,BiasX + X * 20,BiasY + Y * 20,clYellow);
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewLayoutClick(TObject *Sender)
{
  if ( Fruit != NULL ) {
    if ( ExitGameBox(true) == IDCANCEL )
      return;
  }
  HaltFruit(2);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MainPanelDragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
  if ( Source->ClassNameIs("TzcShapeBitColBtn") )
    Accept = TRUE;
  else
    Accept = FALSE;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadLayoutClick(TObject *Sender)
{
  OpenDialog1->Filter = "MFME Layout (*.dat )|*.dat";
  OpenDialog1->DefaultExt = AnsiString("dat");
  OpenDialog1->Options << ofFileMustExist;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    DoNewLayout();
    loading = true;
    LoadResourceFile(OpenDialog1->FileName);
    loading = false;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveDatLayout(TObject *Sender)
{
int locked = ((TControl *)Sender)->Tag;

  if ( locked )
    layoutsn = sn;
  else
    layoutsn = UNLOCKEDSN;
  SaveDat(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveDat(bool prompt)
{
TECLFileStream *save_ptr;
TMemoryStream *ptr;
int j;
String key;
unsigned long number, length;
bool dosave = true;

  if ( NotLocked() ) {
    if ( Fruit )
      Fruit->Suspend();
    if ( Display )
      Display->Suspend();
    if ( Video )
      Video->Suspend();
    if ( prompt || ResName == "" ) {
      SaveDialog1->Filter = "MFME Layout (*.dat)|*.dat";
      SaveDialog1->DefaultExt = String("dat");
      SaveDialog1->Options << ofOverwritePrompt;
      SaveDialog1->FileName = ResName.SubString(1, ResName.Length()- 4);
      SetCurrentDirectory(CurrGamePath.c_str());
      if ( SaveDialog1->Execute() )
        ResName = ExtractFileName(SaveDialog1->FileName);
      else
        dosave = false;
    } else if ( Layout_Changed == false )
      dosave = false;
    if ( dosave ) {
      ptr = new TMemoryStream();
      ptr->SetSize(7000000);
      for ( number = 0; number <= 45; number++ ) {
        ptr->Write(&number, 4);
        switch ( number ) {
          case 0:  // sn or Auth sn !
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&sn, 4);
            break;
          case 1:  // Createdbysn
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&createdbysn, 4);
            break;
          case 2:  // layoutsn
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&layoutsn, 4);
            break;
          case 3:  // Product ID
            length = ID.Length();
            ptr->Write(&length,4);
            ptr->Write(ID.c_str(), length);
            break;
          case 4:  // User
            length = User.Length();
            ptr->Write(&length,4);
            ptr->Write(User.c_str(), length);
            break;
          case 5:  // Character
            length = 72;
            ptr->Write(&length,4);
            ptr->Write(&Character, length);
            break;
          case 6:  // Reels
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Reel_Type, 4);
            break;
          case 7:  // Payout
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Hopper_Type, 4);
            break;
          case 8:  // Program
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Program_Type, 4);
            break;
          case 9:  // 7Seg
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&LED_Type, 4);
            break;
          case 10:  // Extender
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Extender_Type, 4);
            break;
          case 11:  // Meters
            length = 128;
            ptr->Write(&length,4);
            for ( j = 0; j < 8; j++ ) {
              ptr->Write(&TotalInMeterMult[j], 4);
              ptr->Write(&TotalOutMeterMult[j], 4);
              ptr->Write(&TotalInTriacMult[j], 4);
              ptr->Write(&TotalOutTriacMult[j], 4);
            }
            break;
          case 12:  // name
            length = 101;
            ptr->Write(&length,4);
            ptr->Write(GameName,101);
            break;
          case 13:  // version
            length = 8;
            ptr->Write(&length,4);
            ptr->Write(GameVersion,8);
            break;
          case 14:  // Sound
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Sound_Type, 4);
            break;
          case 15:  // Char
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Char_Type, 4);
            break;
          case 16:  // MeterSense
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&MeterSense, 4);
            break;
          case 17:  // TriacSense
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&TriacSense, 4);
            break;
          case 18:  // Char Address
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&CharAddr, 4);
            break;
          case 19:  // Volume
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Volume_Type, 4);
            break;
          case 20:  // Reel Lamp X & Y
            length = 8;
            ptr->Write(&length,4);
            ptr->Write(&ReelLampX, 4);
            ptr->Write(&ReelLampY, 4);
            break;
          case 21:  // Meter/Triac Masks
            length = 8;
            ptr->Write(&length,4);
            ptr->Write(&MeterMask, 4);
            ptr->Write(&TriacMask, 4);
            break;
          case 22:  // LVD
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&LVD_Type, 4);
            break;
          case 23:  // AUX 2 inv
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Auxinvert, 4);
            break;
          case 24:  // Hoppers
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Hoppers, 4);
            break;
          case 25:  // Mech Type
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&MechType, 4);
            break;
          case 26:  // Door inv
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&DoorInvert, 4);
            break;
          case 27:  // 2 Win Lines
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&ReelLamp2, 4);
            break;
          case 28:  // Hopper Type
            length = 4;
            ptr->Write(&length,4);
            length = 0;
            ptr->Write(&length,4);
            break;
          case 29:  // SampleFreq
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&SampleFreq, 4);
            break;
          case 30:  // SEC Fitted
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&SECFitted, 4);
            break;
          case 31:  // Door Switch
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&DoorSwitch, 4);
            break;
          case 32:  // CashBox Switch
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&CashSwitch, 4);
            break;
          case 33:  // MPU3 Aux
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&MPU3_AUX, 4);
            break;
          case 34:  // SEC
            length = 240;
            ptr->Write(&length,4);
            for ( j = 0; j < 10; j++ ) {
              ptr->Write(&TotalInMeterMult[j], 4);
              ptr->Write(&TotalOutMeterMult[j], 4);
              ptr->Write(&TotalInTriacMult[j], 4);
              ptr->Write(&TotalOutTriacMult[j], 4);
              ptr->Write(&TotalInSECMult[j], 4);
              ptr->Write(&TotalOutSECMult[j], 4);
            }
            break;
          case 35:  // Stereo
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Stereo, 4);
            break;
          case 36:  // Rotary Switch
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&Rotary, 4);
            break;
          case 37:  // sys80 io map
            length = sizeof(sys80io);
            ptr->Write(&length,4);
            ptr->Write(&sys80io, length);
            break;
          case 38:  // SYS5 snd
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&SYS5_SND, 4);
            break;
          case 39:  // Reel5 jumpers
            length = 8;
            ptr->Write(&length,4);
            length = 0;
            ptr->Write(&length,4);
            ptr->Write(&length,4);
//            ptr->Write(&jump_lo, 4);
//            ptr->Write(&jump_hi, 4);
            break;
          case 40:  // MPU4 Type
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&MPU4_TYPE, 4);
            break;
          case 41:  // Notes
            {
              length = LayoutNotes->Notes->GetTextLen();
              length++;
              char * buff = new char[length];
              LayoutNotes->Notes->GetTextBuf(buff,length);
              ptr->Write(&length,4);
              ptr->Write(buff, length);
              delete buff;
            }
            break;
          case 42: // Notes window size
            length = 8;
            ptr->Write(&length,4);
            length = LayoutNotes->Height;
            ptr->Write(&length, 4);
            length = LayoutNotes->Width;
            ptr->Write(&length, 4);
            break;
          case 43:  // Refill Switch
            length = 4;
            ptr->Write(&length,4);
            ptr->Write(&RefillSwitch, 4);
            break;
          case 44:  // name
            length = 101;
            ptr->Write(&length,4);
            ptr->Write(RealName,101);
            break;
          case 45:  // Org
            length = Organisation.Length();
            ptr->Write(&length,4);
            ptr->Write(Organisation.c_str(), length);
            break;
        }
      }
      number = 0xffffffff;
      ptr->Write(&number, 4);
      number = 0x0;
      ptr->Write(&number, 4);
      ptr->WriteComponentRes("Layout", MainPanel);
      ptr->SetSize(ptr->Position);
      save_ptr = new TECLFileStream(ResName, fmCreate, "", eclNormal);
      save_ptr->LoadFromStream(ptr);
      delete save_ptr;
      delete ptr;
      Editing = "";
      UpdateCaption();
      Layout_Changed = false;
    }
    if ( Fruit )
      Fruit->Resume();
    if ( Display )
      Display->Resume();
    if ( Video )
      Video->Resume();
  }
}

void __fastcall TForm1::Text1Click(TObject *Sender)
{
String type;

  source_ptr = menu_ptr;
  CopyType = 1;
  type = String(((TControl *)menu_ptr)->ClassName());
  if ( type == "TLamp2" ||
       type == "TzcShapeBitColBtn" ||
       type == "TCheckBox" ) {
    ;
  } else {
    CopyType = 0;
    source_ptr = NULL;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Apply1Click(TObject *Sender)
{
struct Points *pptr;

  if ( CopyType ) {
    LayoutChanged();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
      DoApply(pptr->comp, true);
      if ( CopyType & 4 ) {
        pptr->TheRect.Right = pptr->TheRect.Left + source_ptr->Width + 2;
        pptr->TheRect.Bottom = pptr->TheRect.Top + source_ptr->Height + 2;
      }
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Size1Click(TObject *Sender)
{
  CopyType = 4;
  source_ptr = menu_ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Shape1Click(TObject *Sender)
{
  source_ptr = menu_ptr;
  CopyType = 8;
  if ( menu_ptr->ClassNameIs("TLamp2") ) {
    ;
  } else {
    CopyType = 0;
    source_ptr = NULL;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Font1Click(TObject *Sender)
{
String type;

  source_ptr = menu_ptr;
  CopyType = 2;
  type = String(menu_ptr->ClassName());
  if ( type == "TLamp2" ||
       type == "TzcShapeBitColBtn" ||
       type == "TCheckBox" ||
       type == "TLabel" ) {
    ;
  } else {
    CopyType = 0;
    source_ptr = NULL;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Colour1Click(TObject *Sender)
{
String type;

  source_ptr = menu_ptr;
  CopyType = 16;
  type = menu_ptr->ClassName();
  if ( type == "TLamp2" ||
       type == "TSevenSegment" ||
       type == "TzcShapeBitColBtn" ) {
    ;
  } else {
    CopyType = 0;
    source_ptr = NULL;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::DoApply(TControl *ptr, bool group)
{
int lamp;

  if ( ptr->ClassNameIs("TLamp2") ) {
    TLamp2 *lamp2_ptr = (TLamp2 *)ptr;
    if ( CopyType & 1 )
      lamp2_ptr->Caption = ((TLamp2 *)source_ptr)->Caption;
    if ( CopyType & 2 )
      lamp2_ptr->Font = ((TLamp2 *)source_ptr)->Font;
    if ( CopyType & 4) {
      lamp2_ptr->Width = ((TLamp2 *)source_ptr)->Width;
      lamp2_ptr->Height = ((TLamp2 *)source_ptr)->Height;
    }
    if ( CopyType & 8) {
      lamp2_ptr->Shape = ((TLamp2 *)source_ptr)->Shape;
      lamp2_ptr->RoundPerc = ((TLamp2 *)source_ptr)->RoundPerc;
      lamp2_ptr->Vectors = ((TLamp2 *)source_ptr)->Vectors;
      lamp2_ptr->StartingAngle = ((TLamp2 *)source_ptr)->StartingAngle;
    }
    if ( CopyType & 16 ) {
      lamp2_ptr->BackColor = ((TLamp2 *)source_ptr)->BackColor;
      lamp2_ptr->Colour1 = ((TLamp2 *)source_ptr)->Colour1;
      lamp2_ptr->Colour2 = ((TLamp2 *)source_ptr)->Colour2;
    }
  } else if ( ptr->ClassNameIs("TzcShapeBitColBtn") ) {
    TzcShapeBitColBtn *button_ptr = (TzcShapeBitColBtn *)ptr;
    if ( CopyType & 1 ) {
      button_ptr->Caption = ((TzcShapeBitColBtn *)source_ptr)->Caption;
      button_ptr->MultipleLines = ((TzcShapeBitColBtn *)source_ptr)->MultipleLines;
    }
    if ( CopyType & 2 )
      button_ptr->Font = ((TzcShapeBitColBtn *)source_ptr)->Font;
    if ( CopyType & 4 ) {
      button_ptr->Width = source_ptr->Width;
      button_ptr->Height = source_ptr->Height;
    }
    if ( CopyType & 16 ) {
      if ( _HasLamp(((TComponent *)ptr)->Tag) ) {
        lamp = _LampFromTag(ptr->Tag);
        LampColour(lamp, ptr, ((TzcShapeBitColBtn *)source_ptr)->OnColor);
        button_ptr->OnColor = ((TzcShapeBitColBtn *)source_ptr)->OnColor;
      }
    }
  } else if (ptr->ClassNameIs("TCheckBox") ) {
    TCheckBox *checkbox_ptr = (TCheckBox *)ptr;
    if ( CopyType & 1 )
      checkbox_ptr->Caption = ((TCheckBox *)source_ptr)->Caption;
    if ( CopyType & 2 )
      checkbox_ptr->Font = ((TCheckBox *)source_ptr)->Font;
  } else if ( ptr->ClassNameIs("TImage") ) {
    TImage *image_ptr = (TImage *)ptr;
    if ( CopyType & 4 ) {
      image_ptr->Width = source_ptr->Width;
      image_ptr->Height = source_ptr->Height;
      image_ptr->Picture->Bitmap->Width = source_ptr->Width;
      image_ptr->Picture->Bitmap->Height = source_ptr->Height;
    }
  } else if ( ptr->ClassNameIs("TReel") ) {
    TReel *reel_ptr = (TReel *)ptr;
    if ( CopyType == 31 ) {
      reel_ptr->WinLines = ((TReel *)source_ptr)->WinLines;
      reel_ptr->Flag = ((TReel *)source_ptr)->Flag;
      reel_ptr->Offset = ((TReel *)source_ptr)->Offset;
      reel_ptr->Stops = ((TReel *)source_ptr)->Stops;
      reel_ptr->RHeight = ((TReel *)source_ptr)->RHeight;
      reel_ptr->Toggle = ((TReel *)source_ptr)->Toggle;
      reel_ptr->WinLine = ((TReel *)source_ptr)->WinLine;
      reel_ptr->WinLineWidth = ((TReel *)source_ptr)->WinLineWidth;
      reel_ptr->Coverage = ((TReel *)source_ptr)->Coverage;
      reel_ptr->BorderWidth = ((TReel *)source_ptr)->BorderWidth;
      reel_ptr->BorderColor = ((TReel *)source_ptr)->BorderColor;
      reel_ptr->Inverted = ((TReel *)source_ptr)->Inverted;
      reel_ptr->SetMask(((TReel *)source_ptr)->GetMask());
      reel_ptr->ReelLamps = ((TReel *)source_ptr)->ReelLamps;
    }
    if ( CopyType & 4 ) {
      reel_ptr->Width = ((TReel *)source_ptr)->Width;
      reel_ptr->Height = ((TReel *)source_ptr)->Height;
      reel_ptr->DrawReels();
      reel_ptr->Draw();
    }
  } else if ( ptr->ClassNameIs("TSevenSegment") ) {
    TSevenSegment *led_ptr = (TSevenSegment *)ptr;
    if ( CopyType & 16 ) {
      led_ptr->OnColor = ((TSevenSegment *)source_ptr)->OnColor;
      led_ptr->OffColor = ((TSevenSegment *)source_ptr)->OffColor;
      led_ptr->Color = ((TSevenSegment *)source_ptr)->Color;
    }
    if ( CopyType & 4 ) {
      led_ptr->Width = ((TSevenSegment *)source_ptr)->Width;
      led_ptr->Height = ((TSevenSegment *)source_ptr)->Height;
      led_ptr->Thickness = ((TSevenSegment *)source_ptr)->Thickness;
      led_ptr->HorzSpace = ((TSevenSegment *)source_ptr)->HorzSpace;
      led_ptr->VertSpace = ((TSevenSegment *)source_ptr)->VertSpace;
    }
  } else if ( ptr->ClassNameIs("TLabel") ) {
    TLabel *label_ptr = (TLabel *)ptr;
    if ( CopyType & 2)
      label_ptr->Font = ((TLabel *)source_ptr)->Font;
    if ( CopyType & 4) {
      label_ptr->Width = source_ptr->Width;
      label_ptr->Height = source_ptr->Height;
    }
  }
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Apply2Click(TObject *Sender)
{
  DoApply((TControl *)menu_ptr, false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadSoundClick(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "MPU4 Sound images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadSound();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadJPMSoundClick(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "IMPACT Sound images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadJPMSound(8000);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewLampMatrixExt2Click(TObject *Sender)
{
int X, Y;

  for ( X = 16; X < 32; X++ ) {
    for ( Y = 0; Y < 6; Y++ ) {
      if ( Lamps[Y + X * 8] == NULL )
        CreateNewLamp(Y + X * 8,32 + Y + X * 6,BiasX + (X - 16) * 20,BiasY + Y * 20,clYellow);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LEDClick(TObject *Sender)
{
TzLed *led_ptr;

    led_ptr = new TzLed(MainPanel);
    led_ptr->Parent = MainPanel;
    led_ptr->SetBounds( BiasX, BiasY, 17, 17);
    led_ptr->OnMouseDown = ShapeMouseDown;
    led_ptr->OnMouseMove = ShapeMouseMove;
    led_ptr->OnMouseUp   = ShapeMouseUp;
    led_ptr->LedStyle    = lstSimpleRound;
    led_ptr->Tag = 0xfff;
    LayoutChanged();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::ExtractBitmapClick(TLamp2 *ptr, int which)
{
Graphics::TBitmap *bmp;
TRect src_rect, dst_rect;

  if ( (ptr->Top + ptr->Height) <= MainPanel->Background->Height &&
       (ptr->Left + ptr->Width) <= MainPanel->Background->Width ) {
    src_rect = ptr->BoundsRect;
    bmp = new Graphics::TBitmap();
    bmp->Height = ptr->Height;
    bmp->Width = ptr->Width;
    bmp->Canvas->CopyRect(
    Rect(0,0,ptr->Width,ptr->Height), MainPanel->Background->Canvas, src_rect);
    switch ( which ) {
      case 0:
        ptr->OffImage->Assign(bmp);
        break;
      case 1:
        ptr->OnImage1->Assign(bmp);
        break;
      case 2:
        ptr->OnImage2->Assign(bmp);
        break;
    }
    delete bmp;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetOutlines(bool value)
{
int count;

  for ( count = 0; count < MainPanel->ControlCount; count++ ) {
    if ( MainPanel->Controls[count]->ClassNameIs("TLamp2") ) {
      if ( value )
        ((TLamp2 *)MainPanel->Controls[count])->OutlineStyle = psSolid;
      else if ( (((TLamp2 *)MainPanel->Controls[count])->Tag & _NoOutLineMask) )
        ((TLamp2 *)MainPanel->Controls[count])->OutlineStyle = psClear;
    } else if ( MainPanel->Controls[count]->ClassNameIs("TzcShapeBitColBtn") ) {
      if ( value )
        ((TzcShapeBitColBtn *)MainPanel->Controls[count])->Enabled = true;
      else if ( ((TzcShapeBitColBtn *)MainPanel->Controls[count])->Tag & _LockMask ) {
        if ( ((TzcShapeBitColBtn *)MainPanel->Controls[count])->OnColor == clBtnFace )
          ((TzcShapeBitColBtn *)MainPanel->Controls[count])->Enabled = false;
      }
    } else if ( MainPanel->Controls[count]->ClassNameIs("TCheckBox") ) {
      if ( value )
        ((TCheckBox *)MainPanel->Controls[count])->Visible = true;
      else
        ((TCheckBox *)MainPanel->Controls[count])->Visible = false;
    } else if ( MainPanel->Controls[count]->ClassNameIs("TLabel") ) {
      if ( value )
        ((TLabel *)MainPanel->Controls[count])->Visible = true;
//      else
//        ((TLabel *)MainPanel->Controls[count])->Visible = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SwapClick(TObject *Sender)
{
int left, top;
TControl *ptr1, *ptr2;

  ptr1 = group.ptr->comp;
  ptr2 = group.ptr->next_ptr->comp;
  ReleaseGroup();
  left = ptr1->Left;
  top = ptr1->Top;
  ptr1->SetBounds(ptr2->Left, ptr2->Top, ptr1->Width, ptr1->Height);
  ptr2->SetBounds(left, top, ptr2->Width, ptr2->Height);
  MainPanel->Repaint();
  AddToGroup(ptr1);
  AddToGroup(ptr2);
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExportLampsClick(TObject *Sender)
{
int count;
TControl *ptr;
String file;
int total = 0;
String Total;
String Filename;
int number;
int which = ((TMenuItem *)Sender)->Tag;

  if ( NotLocked() && EditMode ) {
    if ( Application->MessageBox("Export Lamps, Are you sure ?",
                      "Confirm", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDOK ) {
      for ( count = 0; count < MainPanel->ControlCount; count++ ) {
        ptr = MainPanel->Controls[count];
        if ( ptr->ClassNameIs("TLamp2") ) {
          file = IntToStr(ptr->Left) + "_" + IntToStr(ptr->Top) + "_" +
                 IntToStr(_LampTag(ptr->Tag)) + "_";
          switch ( which ) {
            case 0:
              file = file + "off";
              break;
            case 1:
              file = file + "on1";
              break;
            case 2:
              file = file + "on2";
              break;
          }
          number = 1;
          do {
            Filename = CurrGamePath + "\\" + file + "_" + IntToStr(number);
            number++;
          } while ( FileExists(Filename+".bmp") );
          switch ( which ) {
            case 0:
              ((TLamp2 *)ptr)->OffImage->SaveToFile(Filename+".bmp");
              break;
            case 1:
              ((TLamp2 *)ptr)->OnImage1->SaveToFile(Filename+".bmp");
              break;
            case 2:
              ((TLamp2 *)ptr)->OnImage2->SaveToFile(Filename+".bmp");
              break;
          }
          total++;
        }
      }
      if ( MainPanel->Background != NULL )
        MainPanel->Background->SaveToFile(CurrGamePath + "background.bmp");
      Total = IntToStr(total) + " lamp(s) exported to " + CurrGamePath;
      Application->MessageBox(Total.c_str(), "Information", MB_OK | MB_ICONINFORMATION);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportLampsClick(TObject *Sender)
{
int count, nbr_images;
String bmp_file;
String name;
int x,y,nbr,inst;
TLamp2 *ptr;
Graphics::TBitmap *bgrnd_ptr;
char type[10];

  if ( NotLocked() && EditMode ) {
    OpenPictureDialog1->Filter = "Lamp Images (*.bmp)|*.bmp";
    OpenPictureDialog1->DefaultExt = AnsiString("bmp");
    OpenPictureDialog1->Options << ofFileMustExist;
    OpenPictureDialog1->FileName = "";
//  add = ((TControl *)Sender)->Tag;

    if ( OpenPictureDialog1->Execute() ) {
      LayoutChanged();
      nbr_images = OpenPictureDialog1->Files->Count;
      for ( count = 0; count < nbr_images; count++ ) {
        bmp_file = OpenPictureDialog1->Files->Strings[count];
        name = ExtractFileName(bmp_file);
        if ( name == "background.bmp" ) {
          bgrnd_ptr = new Graphics::TBitmap();
          bgrnd_ptr->LoadFromFile(bmp_file);
          MainPanel->Background = bgrnd_ptr;
          delete bgrnd_ptr;
        } else {
          sscanf(name.c_str(), "%d_%d_%d_%s_%d.", &x, &y, &nbr, type, &inst);
          ClearLamp(nbr);
          ptr = CreateNewLamp(nbr,nbr,x,y);
          if ( ptr ) {
            ptr->Tag |= _NoOutLineMask;
            ptr->SurfaceType = stBrushImage;
            ptr->Trans = true;
            ptr->OutlineStyle = psClear;
            ptr->OnImage1->LoadFromFile(bmp_file);
            ptr->SetBounds( x, y, ptr->OnImage1->Width, ptr->OnImage1->Height);
          }
        }
//        ptr->Trans = trans == 0 ? false : true;
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpaceAlign(bool type, bool positive)
{
struct Points *pptr;
int left;
int top;
TRect R;
bool swapped = true;
struct Points tmp;
bool resize_tmp;
TControl *comp_ptr;

    while ( swapped ) {
      swapped = false;
      for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
        if ( !pptr->next_ptr )
          break;
        if ( (pptr->comp->Left * pptr->comp->Top) >
             (pptr->next_ptr->comp->Left * pptr->next_ptr->comp->Top) ) {
          resize_tmp = pptr->Resizable;
          R = pptr->TheRect;
          comp_ptr = pptr->comp;
          pptr->Resizable = pptr->next_ptr->Resizable;
          pptr->comp = pptr->next_ptr->comp;
          pptr->TheRect = pptr->next_ptr->TheRect;
          pptr->next_ptr->Resizable = resize_tmp;
          pptr->next_ptr->comp = comp_ptr;
          pptr->next_ptr->TheRect = R;
          swapped = true;
        }
      }
    }
    if ( positive ) {
      left = group.ptr->comp->Left;
      top = group.ptr->comp->Top;
    } else {
      for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
        if ( !pptr->next_ptr ) {
          left = pptr->comp->Left;
          top = pptr->comp->Top;
        }
      }
    }
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
      pptr->comp->Left = left;
      pptr->comp->Top = top;
      R = pptr->comp->BoundsRect;
      pptr->TheRect = Rect( R.Left - 2, R.Top - 2, R.Right + 1, R.Bottom + 1);
      if ( type == 0 ) {
        if ( positive )
          left = left + pptr->comp->Width + spacing;
        else
          left = left - pptr->comp->Width - spacing;
      } else {
        if ( positive )
          top = top + pptr->comp->Height + spacing;
        else
          top = top - pptr->comp->Height - spacing;
      }
    }
    MainPanel->Repaint();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpacingClick(TObject *Sender)
{
  ((TMenuItem *)Sender)->Checked = true;
  spacing = ((TMenuItem *)Sender)->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BottomTopClick(TObject *Sender)
{
  SpaceAlign(1, false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LeftRightClick(TObject *Sender)
{
  SpaceAlign(0, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TopBottomClick(TObject *Sender)
{
  SpaceAlign(1, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RightLeftClick(TObject *Sender)
{
  SpaceAlign(0, false);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ClearLamp(int X)
{
struct lamp *ptr, *next_ptr;

    ptr = Lamps[X];
    while ( ptr ) {
      next_ptr = ptr->next;
      delete ptr->ptr;
      delete ptr;
      ptr = next_ptr;
    }
    Lamps[X] = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CalculateGroupSize(void)
{
struct Points *pptr;

  group.top = group.left = 65535;
  group.right = group.bottom = 0;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr) {
    if ( pptr->TheRect.Top < group.top )
      group.top = pptr->TheRect.Top;
    if ( pptr->TheRect.Bottom > group.bottom )
      group.bottom = pptr->TheRect.Bottom;
    if ( pptr->TheRect.Left < group.left )
      group.left = pptr->TheRect.Left;
    if ( pptr->TheRect.Right > group.right )
      group.right = pptr->TheRect.Right;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LEDLampClick(TObject *Sender)
{
int count;

  for ( count = 0; count < NBR_LAMPS; count++ )
    if ( Lamps[count] == NULL ) {
      CreateLEDLamp(count, BiasX, BiasY);
      break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LampLEDClick(TObject *Sender)
{
int left, top;
int Tag;

  Tag = ((TComponent *)menu_ptr)->Tag;
  top = menu_ptr->Top;
  left = menu_ptr->Left;
  if ( menu_ptr->ClassNameIs("TLamp2") ) {
    if ( _HasLamp(Tag) ) {
      RemoveLamp(_LampFromTag(Tag), menu_ptr);
    }
    if ( _HasLamp2(Tag) ) {
      RemoveLamp(_Lamp2FromTag(Tag), menu_ptr);
    }
    DeleteItem(menu_ptr);
    CreateLEDLamp(_LampFromTag(Tag),left,top);
  } else if ( menu_ptr->ClassNameIs("TzLED") && (menu_ptr->Tag & _LEDLampMask) ) {
    RemoveLamp(_LampFromTag(Tag), menu_ptr);
    DeleteItem(menu_ptr);
    CreateNewLamp(_LampFromTag(Tag),_LampFromTag(Tag),left,top,clYellow);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateLEDLamp(int count, int left, int top)
{
TzLed *led_ptr;

  led_ptr = new TzLed(MainPanel);
  led_ptr->Parent = MainPanel;
  led_ptr->SetBounds( left, top, 17, 17);
  led_ptr->OnMouseDown = ShapeMouseDown;
  led_ptr->OnMouseMove = ShapeMouseMove;
  led_ptr->OnMouseUp   = ShapeMouseUp;
  led_ptr->LedStyle    = lstSimpleRound;
  led_ptr->Tag = _LEDLampMask + count;
  InsertLamp( count, led_ptr, clRed, 1);
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LampHintToCaption(void)
{
int count;
TControl *ptr;
String tmp;
TLamp2 *lamp_ptr;

  for ( count = 0; count < MainPanel->ControlCount; count++ ) {
    ptr = MainPanel->Controls[count];
    if ( ptr->ClassNameIs("TLamp2") ) {
      lamp_ptr = (TLamp2 *)ptr;
      tmp = lamp_ptr->Caption;
      lamp_ptr->Caption = lamp_ptr->Hint;
      lamp_ptr->Hint = tmp;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewReelClick(TObject *Sender)
{
TReel *reel_ptr;
int count;

  for ( count = 0; count < NBR_REELS; count++ )
    if ( Reels[count] == NULL ) {
      reel_ptr = new TReel(MainPanel);
      reel_ptr->Parent = MainPanel;
      reel_ptr->SetBounds( BiasX, BiasY, 80, 140);
      reel_ptr->Tag = _ReelTag(count);
      reel_ptr->Stops = 16;
      reel_ptr->OnMouseDown = ShapeMouseDown;
      reel_ptr->OnMouseMove = ShapeMouseMove;
      reel_ptr->OnMouseUp   = ShapeMouseUp;
      reel_ptr->Number      = count;
      Reels[count] = reel_ptr;
      ReelType[count] = RT_REEL;
      break;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::All1Click(TObject *Sender)
{
String type;

  CopyType = 31;
  source_ptr = menu_ptr;
  type = String(menu_ptr->ClassName());
  if ( type == "TReel" ||
       type == "TLamp2" ||
       type == "TSevenSegment" ||
       type == "TzcShapeBitColBtn" ||
       type == "TCheckBox" ) {
    ;
  } else {
    CopyType = 0;
    source_ptr = NULL;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::FrameClick(TObject *Sender)
{
TFrame2 *new_ptr;

  LayoutChanged();
  new_ptr = new TFrame2(MainPanel);
  new_ptr->Top = BiasY;
  new_ptr->Left = BiasX;
  new_ptr->Width = 50;
  new_ptr->Height = 50;
  new_ptr->Shape = bsFrame;
  new_ptr->OnMouseDown = ShapeMouseDown;
  new_ptr->OnMouseMove = ShapeMouseMove;
  new_ptr->OnMouseUp = ShapeMouseUp;
  new_ptr->Parent = MainPanel;
  new_ptr->SendToBack();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::CreateNewGraphic()
{
TLamp2 *ptr;
int count;
int Top, Left, Right, Bottom;

  Top = RBox.Top <= 0 ? 1 : RBox.Top;
  Left = RBox.Left <= 0 ? 1 : RBox.Left;
  Right = RBox.Right >= MainPanel->Width ? MainPanel->Width -1 : RBox.Right;
  Bottom = RBox.Bottom >= MainPanel->Height ? MainPanel->Height -1 : RBox.Bottom;

  for ( count = 0; count < NBR_LAMPS; count++ )
    if ( Lamps[count] == NULL ) {
      ptr = CreateNewLamp(count, count, Left, Top);
      ptr->Tag |= _NoOutLineMask;
      ptr->Width = Right - Left;
      ptr->Height = Bottom - Top;
      ptr->Trans = true;
//      ExtractBitmapClick(ptr);
      break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitmapButtonClick(TObject *Sender)
{
int count;

  for ( count = 0; count < 48; count++ )
    if ( Buttons[count] == NULL ) {
      CreateBitmapButton(count, BiasX, BiasY, "");
      break;
    }
}
//---------------------------------------------------------------------------
TControl * __fastcall TForm1::CreateBitmapButton(int nbr, int left, int top, String caption)
{
TzcShapeBitColBtn *new_ptr;

  if ( IsInside( Rect(left,top,left+73,top+25),
                 Rect(0,0,MainPanel->Width, MainPanel->Height)) ) {
    LayoutChanged();
    new_ptr = new TzcShapeBitColBtn(MainPanel);
    new_ptr->SetBounds( left, top, 73, 25);
    if ( caption == "" ) {
      if ( nbr > 31 )
        new_ptr->Caption = "AUX" + IntToStr((nbr-24)/8) + " , " + IntToStr(nbr % 8);
      else
        new_ptr->Caption = IntToStr(nbr);
    } else
      new_ptr->Caption = caption;
    new_ptr->ShowFocus = false;
    new_ptr->OnMouseDown = ShapeMouseDown;
    new_ptr->OnMouseMove = ShapeMouseMove;
    new_ptr->OnMouseUp = ShapeMouseUp;
    new_ptr->OnDragOver  = CheckLampToButton;
    new_ptr->OnDragDrop  = LampToButton;
    new_ptr->Parent = MainPanel;
    new_ptr->Tag = nbr << 8;
    new_ptr->Hint = new_ptr->Caption;
    if ( Buttons[nbr] != NULL ) {
      DeleteItem(Buttons[nbr]);
    }
    Buttons[nbr] = (TControl *)new_ptr;
  }
  return new_ptr;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::NewLamp21Click(TObject *Sender)
{
int count;

  for ( count = 0; count < NBR_LAMPS; count++ )
    if ( Lamps[count] == NULL ) {
      CreateNewLamp(count, count, BiasX, BiasY);
      break;
    }
}
//---------------------------------------------------------------------------

TLamp2 * __fastcall TForm1::CreateNewLamp(int nbr, int hint, int left, int top, TColor Colour)
{
TLamp2 *new_ptr = NULL;

  if ( IsInside( Rect(left,top,left+17,top+17),
                 Rect(0,0,MainPanel->Width, MainPanel->Height)) ) {
    new_ptr = new TLamp2(MainPanel);
    new_ptr->SetBounds( left, top, 17, 17);
    new_ptr->Shape = gstRectangle;
    new_ptr->Parent = MainPanel;
    new_ptr->OnMouseDown = ShapeMouseDown;
    new_ptr->OnMouseMove = ShapeMouseMove;
    new_ptr->OnMouseUp = ShapeMouseUp;
    new_ptr->OnDragOver = CheckLampToGraphic;
    new_ptr->OnDragDrop = GraphicDragDrop;
    new_ptr->OnEndDrag = LampEndDrag;
    new_ptr->Pattern = bsSolid;
    new_ptr->BackColor = clBtnFace;
    new_ptr->Colour1 = Colour;
    new_ptr->Colour2 = clWhite;
    new_ptr->RoundPerc = 10;
    new_ptr->SurfaceType = stBrushOpaque;
    new_ptr->InVectorPercent = 0;
    new_ptr->Vectors = 4;
    new_ptr->Trans = false;
    new_ptr->Tag = _LampTag(nbr);
    new_ptr->Hint = IntToStr(hint);
    InsertLamp(nbr, new_ptr, clYellow, 1);
    new_ptr->Tag |= _LampMask;
    new_ptr->Visible = true;
  }
  return new_ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::EditCharacteriserClick(TObject *Sender)
{
  if ( NotLocked() )
    if ( ChrEditor == NULL )
      ChrEditor = new TChrEditor(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NewBitmap(TObject *Sender)
{
TImage *bmp_ptr;

  bmp_ptr = new TImage(MainPanel);
  bmp_ptr->Parent = MainPanel;
  bmp_ptr->SetBounds( BiasX, BiasY, 25, 25);
  bmp_ptr->Tag = 0x10000;
  bmp_ptr->OnMouseDown = ShapeMouseDown;
  bmp_ptr->OnMouseMove = ShapeMouseMove;
  bmp_ptr->OnMouseUp   = ShapeMouseUp;
  bmp_ptr->Canvas->Brush->Color = clWhite;
  bmp_ptr->Canvas->FillRect(Rect(0,0,bmp_ptr->Width,bmp_ptr->Height));
  bmp_ptr->Stretch = true;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DoNewLayout(void)
{
int count, X, Y;
struct lamp *ptr, *next_ptr;

  loading = true;
  if ( !NoSound )
    Sound->ClearBuffer();
  EditMode = false;
  LampMode = false;
  EnableConfig(true);
  MeterPanel->Color = clBtnFace;
  layoutsn = sn;
  createdbysn = sn;
  if ( EditMenu )
    EditMenu->Checked = false;
  ShowHint = false;
  for ( X = 0; X < 3; X++ )
    display[X] = NULL;
  dotmatrix = NULL;
  aceled = NULL;
  bfmalpha = NULL;
  proconnled = NULL;
  for ( X = 0; X < NBR_LAMPS; X++ ) {
    ptr = Lamps[X];
    while ( ptr ) {
      next_ptr = ptr->next;
      delete ptr;
      ptr = next_ptr;
    }
    Lamps[X] = NULL;
  }
  for ( X = 0; X < NBR_REELS; X++ ) {
    Reels[X] = NULL;
    ReelType[X] = RT_NONE;
  }
  for ( X = 0; X < 48; X++ )
    Buttons[X] = NULL;
  for ( X = 0; X < 40; X++ ) {
    led_display[X].digit = NULL;
    for ( Y = 0; Y < 8; Y++ )
      led_display[X].led[Y] = NULL;
  }
  for ( X = 0; X < 72; X++ )
    Character[X] = 0;
  MainPanel->Color = clBtnFace;
  MainPanel->Background = NULL;
  Speed = 5000;
  Speed2 = 0;
  Config->ResetSettings();
  BackDoor->Checked = false;
  CashDoor->Checked = false;
  Refill->Checked = false;
  Volume_Type = 0;
  Reel_Type = 0;
  Sound_Type = 0;
  Extender_Type = 0;
  LED_Type = 0;
  Stake = 0;
  Jackpot = 0;
  Percent = 0;
  Hoppers = 0x50;
  MeterSense = -1;
  TriacSense = -1;
  MeterMask = 255;
  TriacMask = 255;
  TimesPlayed = 0;
  ReelLampX = 0;
  ReelLampY = 0;
  ReelLamp2 = 0;
  LVD_Type = 0;
  DIP1 = 0;
  DIP2 = 0;
  DIP3 = 0;
  PCkey = 0;
  STkey = 0;
  JPkey = 0;
  SampleFreq = 16000;
  DoorSwitch = 255;
  CashSwitch = 255;
  RefillSwitch = 255;
  MPU3_AUX = 0;
  SYS5_SND = 0;
  MPU4_TYPE = 0;
  Stereo = 1;
  for ( count = 0; count < 512; count++ ) {
    sys80io[count].type = S80_LAMP;
    sys80io[count].value = 0;
  }
  for ( count = 0; count < MainPanel->ControlCount; count++ ) {
    DeleteItem(MainPanel->Controls[count]);
    count--;
  }

  for ( count = 0; count < 16; count++ )
    matrix[count] = 0;

  for ( count = 0; count < 31; count++ ) {
    SEC_Counters[count] = 0;
    SEC_Strings[count][0] = 0;
  }

  for ( count = 0; count < 255; count++ )
    shortcuts[count] = NULL;

  for ( count = 0; count < 10; count++ ) {
    TotalInMeterMult[count] = 0;
    TotalOutMeterMult[count] = 0;
    TotalInTriacMult[count] = 0;
    TotalOutTriacMult[count] = 0;
    TotalInSECMult[count] = 0;
    TotalOutSECMult[count] = 0;
  }
  TotalInMeterMult[0] = 1;
  TotalOutMeterMult[1] = 1;

  reelview = false;
  if ( GeneralConfig->LongTermMeters->Checked )
    longterm = true;
  else
    longterm = false;
  profitloss = false;
  STotalIn = 0;
  STotalOut = 0;
  TotalIn = 0;
  TotalOut = 0;
  UpdateMeters();

  ROMname = "";
  DOTROMname = "";
  VIDROMname = "";
  RAMname = "";
  CharLoaded = false;
  ResName = "";
  strcpy(GameName, "MFME");
  UpdateCaption();

  SoundList->Clear();

  ROMList->Clear();
  DOTROMList->Clear();
  VIDROMList->Clear();
  Status->Brush->Color = clBtnFace;
  
  Layout_Changed = false;
  Manager->Visible = false;
  loading = false;
  LayoutNotes->Notes->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DebugClick(TObject *Sender)
{
  MainDebug->Show();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::MainPanelMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
tagPOINT tp;

  if ( EditMode )
    if ( Button == mbLeft ) {
      if ( FMove == false ) {
        ReleaseGroup();
        RBox.Left = X;
        RBox.Top = Y;
        FFirstLine = true;
        FDrawing = true;
      }
    } else if ( Button == mbRight ) {
      tp = ((TControl *)Sender)->ClientOrigin;
      menu_ptr = (TControl *)Sender;
      BiasX = X;
      BiasY = Y;
      NewMenu->Popup(tp.x + X, tp.y + Y);
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MainPanelMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
  if ( Shift.Contains(ssLeft) )
    if ( FDrawing == true ) {
      if ( FFirstLine == false )
        DrawFrame(RBox);
      RBox.Right = X;
      RBox.Bottom = Y;
      FFirstLine = false;
      DrawFrame(RBox);
    }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MainPanelMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
TRect R;
int count;
int tmp;

    if ( FMove == false && FDrawing == true ) {
      RBox.Right = X;
      RBox.Bottom = Y;
      FDrawing = false;
      DrawFrame(RBox);
      if ( RBox.Right < RBox.Left ) {
        tmp = RBox.Right;
        RBox.Right = RBox.Left;
        RBox.Left = tmp;
      }
      if ( RBox.Bottom < RBox.Top ) {
        tmp = RBox.Bottom;
        RBox.Bottom = RBox.Top;
        RBox.Top = tmp;
      }
      if ( !LampMode ) {
        for ( count = 0; count < MainPanel->ControlCount; count++ ) {
          R = MainPanel->Controls[count]->BoundsRect;
          if ( IsInside( R, RBox ) )
            AddToGroup(MainPanel->Controls[count]);
        }
      } else if ( !FFirstLine )
        CreateNewGraphic();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MatrixChange(TObject *Sender, bool state)
{
  bool coin = false;
  if ( ((TComponent *)Sender)->Tag & _CoinMask )
    coin = true;

  if ( Sender->ClassNameIs("TLAMP2") )
    MatrixChange2(((TLamp2 *)Sender)->Button, state, coin);
  else
    MatrixChange2(_ButtonFromTag(((TComponent *)Sender)->Tag), state, coin);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MatrixChange3(int Tag, bool state)
{
  MatrixChange2(_ButtonFromTag(Tag), state);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MatrixChange2(int button, bool state, bool coin)
{
int ButX, ButY, row;

  if ( coin ) {
    if ( Arch == BARCREST_MPU4 ) {
      ButY = coinmasks[button];
      if ( state ) {
        if ( !cointimer ) {
          matrix[5] |= ButY;
          cointimer = 60;
          coinTag = ButY;
        }
      }
      return;
    } else if ( Arch == JPM_IMPACT ) {
      ButY = coinmasks[button];
      if ( state ) {
        if ( !cointimer ) {
          matrix[9] |= ButY;
          cointimer = 80;
          coinTag = ButY;
        }
      }
      return;
    } else if ( Arch == BFM_SCORPION2 && Fruit != NULL ) {
      if ( state )
        ((TSCORPION2 *)Fruit)->board.coin |= coinmasks[button];
      else
        ((TSCORPION2 *)Fruit)->board.coin &= ~coinmasks[button];
      return;
    } else if ( Arch == JPM_SYS5 ) {
      ButY = coinmasks[button];
      if ( state ) {
        if ( !cointimer ) {
          matrix[7] |= ButY;
          cointimer = 60;
          coinTag = ButY;
        }
      }
      return;
    } else if ( Arch == MAYGAY_M1A ) {
      ButY = coinmasks[button];
      if ( state ) {
        if ( !cointimer ) {
          matrix[0] |= ButY;
          cointimer = 60;
          coinTag = ButY;
        }
      }
      return;
    } else {
      return;
    }
/*    if ( state )
      matrix[row] |= ButY;
    else
      matrix[row] &= ~ButY;   */
  } else {
    ButX = (button & 0x78) >> 3;
    ButY = button & 0x7;
    if ( state == true ) {
      matrix[ButX] |= 1 << ButY;
      if ( ButX == 4 && MonitorActive && Arch == BARCREST_MPU4 )
        IOMonitor->UpdateAux1(matrix[4]);
      else if ( ButX == 5 && MonitorActive && Arch == BARCREST_MPU4 )
        IOMonitor->UpdateAux2(matrix[5]);
    } else {
      matrix[ButX] &= ~(1 << ButY);
      if ( ButX == 4 && MonitorActive && Arch == BARCREST_MPU4 )
        IOMonitor->UpdateAux1(matrix[4]);
      else if ( ButX == 5 && MonitorActive && Arch == BARCREST_MPU4 )
        IOMonitor->UpdateAux2(matrix[5]);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HaltFruit(Byte state)
{

  State = state;
  if ( HasSampledSound ) {
    Sound->Stop();
    HasSampledSound = false;
  }
  if ( Display != NULL )
    Display->Terminate();
  if ( Video != NULL )
    Video->Terminate();
  if ( Fruit != NULL ) {
    SaveGameFile(CurrGamePath + Game);
    UpdateGameData();
    Fruit->Terminate();
  } else
    FruitTerminated(NULL);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  State = 0;
  if ( Display )
    Display->Terminate();
  if ( Video )
    Video->Terminate();
  if ( Fruit != NULL ) {
    if ( ExitGameBox(false) == IDOK ) {
      SaveGameFile(CurrGamePath + Game);
      UpdateGameData();
      Fruit->Terminate();
    }
    Action = caNone;
  } else {
    FruitTerminated(NULL);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FruitTerminated(TObject *Sender)
{

  Fruit = NULL;
  CurrGamePath = ScanDir;

  switch ( State ) {
    case 0:
      Shutdown();
      break;
    case 2:
      DoNewLayout();
      break;
    case 3:
      LoadAGame();
      break;
    case 4:
      Manager->Visible = false;
      MeterPanel->Visible = true;
      GameManager1->Visible = true;
      ExitManager->Visible = false;
      Menu = MainMenu1;
      Load(ListView1->Selected->SubItems->Strings[7]);
      break;
    case 5:
      Load(history[historynbr]);
      break;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::DisplayTerminated(TObject *Sender)
{
  Display = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VideoTerminated(TObject *Sender)
{
  Video = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Exit1Click(TObject *Sender)
{
  if ( ExitGameBox(false) == IDOK )
    HaltFruit(0);
}

//---------------------------------------------------------------------------

bool __fastcall TForm1::LoadResourceFile(String Filename)
{
int count, X, Y;
String type;
TControl *ptr;
int Tag;
TECLFileStream *res_ptr;
TMemoryStream *read_ptr;
int j;
String key;
char buffer[300];
int number, length;
String LoadedID, LoadedUser, LoadedOrganisation;
TImage *ReelSymbols[NBR_REELS][NBR_STOPS];
bool invalid = false;
bool first = true;
int stops[4] = { 12, 12, 16, 24 };

    memset( ReelSymbols, 0, sizeof(ReelSymbols));
    ResName = ExtractFileName(Filename);
    if ( !FileExists(Filename ) )
      return false;
    MainPanel->Visible = false;
    if ( ExtractFileExt(Filename).LowerCase() == ".dat" ) {
      res_ptr = new TECLFileStream(Filename, fmOpenRead, "", eclNone);
      read_ptr = new TMemoryStream();
      read_ptr->SetSize(res_ptr->Size);
      res_ptr->SaveToStream(read_ptr);
      delete res_ptr;
      do {
        read_ptr->Read(&number, 4);
        if ( first && number )
          invalid = true;
        else
          first = false;
        read_ptr->Read(&length, 4);
        switch ( number ) {
          case 0:
            read_ptr->Read(&authsn, 4);
            break;
          case 1:
            read_ptr->Read(&createdbysn, 4);
            break;
          case 2:
            read_ptr->Read(&layoutsn, 4);
            break;
          case 3:
            read_ptr->Read(buffer, length);
            buffer[length] = 0;
            LoadedID = AnsiString(buffer);
            break;
          case 4:
            read_ptr->Read(buffer, length);
            buffer[length] = 0;
            LoadedUser = AnsiString(buffer);
            break;
          case 5:
            read_ptr->Read(&Character,72);
            if ( Character[1] )
              CharLoaded = true;
            break;
          case 6:
            read_ptr->Read(&Reel_Type,4);
            Config->Reels->ItemIndex = Reel_Type;
            break;
          case 7:
            read_ptr->Read(&Hopper_Type,4);
            Config->Payout->ItemIndex = Hopper_Type;
            break;
          case 8:
            read_ptr->Read(&Program_Type,4);
            Config->ProgramType->ItemIndex = Program_Type;
            break;
          case 9:
            read_ptr->Read(&LED_Type,4);
//            if ( LED_Type > 0 )
//              MPU3_AUX = 1;
            if ( LED_Type > 2 )
              LED_Type -= 2;
            Config->SegmentDisplay->ItemIndex = LED_Type;
            break;
          case 10:
            read_ptr->Read(&Extender_Type,4);
            Config->Extender->ItemIndex = Extender_Type;
            break;
          case 11:
            for ( j = 0; j < 8; j++ ) {
              read_ptr->Read(&TotalInMeterMult[j], 4);
              read_ptr->Read(&TotalOutMeterMult[j], 4);
              read_ptr->Read(&TotalInTriacMult[j], 4);
              read_ptr->Read(&TotalOutTriacMult[j], 4);
            }
            Config->UpdateMeters();
            break;
          case 12:
            read_ptr->Read(GameName,101);
            UpdateCaption();
            Config->TGameName->Text = AnsiString(GameName);
            break;
          case 13:
            read_ptr->Read(GameVersion,8);
            Config->TGameVersion->Text = AnsiString(GameVersion);
            break;
          case 14:
            read_ptr->Read(&Sound_Type, 4);
            Config->Snd->ItemIndex = Sound_Type;
            break;
          case 15:
            read_ptr->Read(&Char_Type, 4);
            Config->CharType->ItemIndex = Char_Type;
            break;
          case 16:
            read_ptr->Read(&MeterSense, 4);
            if ( MeterSense != - 1 )
              Config->TMeterSense->Text = IntToStr(MeterSense >> 8);
            break;
          case 17:
            read_ptr->Read(&TriacSense, 4);
            if ( TriacSense != -1 )
              Config->TTriacSense->Text = IntToStr(TriacSense >> 8);
            break;
          case 18:
            read_ptr->Read(&CharAddr, 4);
            if ( CharAddr )
              Config->ChrAddress->Text = IntToHex(CharAddr,5);
            else
              Config->ChrAddress->Text = "";
            break;
          case 19:
            read_ptr->Read(&Volume_Type, 4);
            Config->Volume->ItemIndex = Volume_Type;
            break;
          case 20:
            read_ptr->Read(&ReelLampX, 4);
            Config->TReelLampX->Text = IntToStr(ReelLampX);
            read_ptr->Read(&ReelLampY, 4);
            Config->TReelLampY->Text = IntToStr(ReelLampY);
            break;
          case 21:
            read_ptr->Read(&MeterMask, 4);
            Config->TMeterMask->Text = IntToHex(MeterMask,2);
            read_ptr->Read(&TriacMask, 4);
            Config->TTriacMask->Text = IntToHex(TriacMask,2);
            break;
          case 22:
            read_ptr->Read(&LVD_Type, 4);
            Config->LVD->ItemIndex = LVD_Type;
            break;
          case 23:
            read_ptr->Read(&Auxinvert, 4);
            Config->Aux2inv->Checked = Auxinvert & 1 ? true : false;
            Config->Aux1inv->Checked = Auxinvert & 2 ? true : false;
            break;
          case 24:
            read_ptr->Read(&Hoppers, 4);
            Config->THopper1->Checked = Hoppers & 0x40 ? false : true;
            Config->THopper2->Checked = Hoppers & 0x10 ? false : true;
            break;
          case 25:
            read_ptr->Read(&MechType, 4);
            Config->CoinMech->ItemIndex = MechType;
            break;
          case 26:
            read_ptr->Read(&DoorInvert, 4);
            Config->TDoorInvert->Checked = DoorInvert ? true : false;
            break;
          case 27:
            read_ptr->Read(&ReelLamp2, 4);
            Config->TwoLines->Checked = ReelLamp2 == 0 ? false : true;
            break;
          case 28:
            read_ptr->Read(&Tag, 4);
            break;
          case 29:
            read_ptr->Read(&SampleFreq, 4);
            Config->SampleRate->Text = IntToStr(SampleFreq);
            break;
          case 30:
            read_ptr->Read(&SECFitted, 4);
            Config->TSECFitted->ItemIndex = SECFitted ? 1 : 0;
            break;
          case 31:
            read_ptr->Read(&DoorSwitch, 4);
            if ( DoorSwitch < 255 )
              Config->Door->Text = IntToStr(DoorSwitch);
            else
              Config->Door->Text = "";
            break;
          case 32:
            read_ptr->Read(&CashSwitch, 4);
            if ( CashSwitch < 255 )
              Config->CashBox->Text = IntToStr(CashSwitch);
            else
              Config->CashBox->Text = "";
            break;
          case 33:
            read_ptr->Read(&MPU3_AUX, 4);
            Config->Aux->ItemIndex = MPU3_AUX;
            break;
          case 34:
            for ( j = 0; j < 10; j++ ) {
              read_ptr->Read(&TotalInMeterMult[j], 4);
              read_ptr->Read(&TotalOutMeterMult[j], 4);
              read_ptr->Read(&TotalInTriacMult[j], 4);
              read_ptr->Read(&TotalOutTriacMult[j], 4);
              read_ptr->Read(&TotalInSECMult[j], 4);
              read_ptr->Read(&TotalOutSECMult[j], 4);
            }
            Config->UpdateMeters();
            break;
          case 35:
            read_ptr->Read(&Stereo, 4);
            Config->Channels->ItemIndex = Stereo ? 0 : 1;
            break;
          case 36:
            read_ptr->Read(&Rotary, 4);
            Config->UpDown1->Position = Rotary;
            break;
          case 37:
            read_ptr->Read(&sys80io, sizeof(sys80io));
            Config->PortChange(NULL);
            break;
          case 38:
            read_ptr->Read(&SYS5_SND, 4);
            Config->sys5sound->ItemIndex = SYS5_SND;
            break;
          case 39:
            read_ptr->Read(&Tag, 4);
            read_ptr->Read(&Tag, 4);
//            read_ptr->Read(&jump_lo, 4);
//            read_ptr->Read(&jump_hi, 4);
            break;
          case 40:
            read_ptr->Read(&MPU4_TYPE, 4);
            Config->MPU4Type->ItemIndex = MPU4_TYPE;
            break;
          case 41:
            {
              char * buff = new char[length];
              read_ptr->Read(buff, length);
              LayoutNotes->Notes->SetTextBuf(buff);
              delete buff;
            }
            break;
          case 42:
            read_ptr->Read(&Tag, 4);
            LayoutNotes->Height = Tag;
            read_ptr->Read(&Tag, 4);
            LayoutNotes->Width = Tag;
            break;
          case 43:
            read_ptr->Read(&RefillSwitch, 4);
            if ( RefillSwitch < 255 )
              Config->Refill->Text = IntToStr(RefillSwitch);
            else
              Config->Refill->Text = "";
            break;
          case 44:
            read_ptr->Read(RealName,101);
            Config->ShortName->Text = AnsiString(RealName);
            break;
          case 45:
            read_ptr->Read(buffer, length);
            buffer[length] = 0;
            LoadedOrganisation = AnsiString(buffer);
            break;
          case 0xffffffff:
            break;
          default:
            read_ptr->Position += length;
            break;
        }
      } while ( number != 0xffffffff && !invalid );
      try {
//        read_ptr->SaveToFile("fred.res");
        read_ptr->ReadComponentRes(MainPanel);
      }
      catch ( Exception &exception ) {
        delete read_ptr;
        MainPanel->Visible = true;
        Application->MessageBox("Invalid Layout", "Error", MB_OK | MB_ICONEXCLAMATION);
        return false;
      }
      delete read_ptr;
    } else {
      read_ptr = new TMemoryStream();
      read_ptr->LoadFromFile(Filename);
      read_ptr->ReadComponentRes(MainPanel);
      delete read_ptr;
    }
#ifndef UNLOCKED
    if ( layoutsn != sn && layoutsn != UNLOCKEDSN )
      EnableConfig(false);
#endif
    MainPanel->Top = 33;
    MainPanel->BevelInner = bvNone;
    MainPanel->BevelOuter = bvRaised;
    ClientWidth = MainPanel->Width;
    ClientHeight = MainPanel->Height + MeterPanel->Height;
    MainPanel->Align = alClient;
    SetBounds( (Screen->Width/2) - (Width/2), (Screen->Height/2) - (Height/2), Width, Height);
    for ( count = 0; count < MainPanel->ControlCount; count++ ) {
      ptr = MainPanel->Controls[count];
      Tag = ptr->Tag;
      type = ptr->ClassName();
      if ( type == "TLamp" ) {
        TLamp *ptr2 = (TLamp *)ptr;
        TLamp2 *new_ptr = new TLamp2(MainPanel);
        new_ptr->BackColor = clBtnFace;
        new_ptr->Colour1 = ptr2->Colour1;
        new_ptr->Colour2 = ptr2->Colour2;
        new_ptr->Font = ptr2->Font;
        new_ptr->Caption = ptr2->Caption;
        new_ptr->Shape = TGPShapeType(ptr2->Shape);
        new_ptr->SurfaceType = stBrushOpaque;
        new_ptr->RoundPerc = 5;
        new_ptr->Trans = false;
        new_ptr->SetBounds(ptr2->Left,ptr2->Top,ptr2->Width,ptr2->Height);
        new_ptr->Parent = MainPanel;
        new_ptr->InVectorPercent = 0;
        new_ptr->Vectors = 4;
        new_ptr->Tag = Tag;
        new_ptr->Button = 0;
        new_ptr->Hint = IntToStr(_LampFromTag(Tag));
        delete ptr;
        count--;
      } else if ( type == "TLamp2" ) {
        TLamp2 *ptr2 = (TLamp2 *)ptr;
        ptr2->OnMouseDown = ShapeMouseDown;
        ptr2->OnMouseUp   = ShapeMouseUp;
        ptr2->OnMouseMove = ShapeMouseMove;
        ptr2->OnDragOver  = CheckLampToGraphic;
        ptr2->OnDragDrop  = GraphicDragDrop;
        ptr2->OnEndDrag   = LampEndDrag;
        ptr2->ForeColor   = clBtnFace;
        ptr2->State       = 0;
        if ( (Tag & _ButtonMask) && ptr2->Button == 0 )
          ptr2->Button = _ButtonFromTag(Tag);
        if ( Tag & _NoOutLineMask )
          ptr2->OutlineStyle = psClear;
        else
          ptr2->OutlineStyle = psSolid;
        ptr2->Hint = IntToStr(_LampFromTag(Tag));
        InsertLamp(_LampFromTag(Tag), ptr, ptr2->Colour1, 1);
        if ( _HasLamp2(Tag) ) {
          ptr2->Hint = IntToStr(_LampFromTag(Tag)) + "," +
                                 IntToStr(_Lamp2FromTag(Tag));
          InsertLamp(_Lamp2FromTag(Tag), ptr, ptr2->Colour2, 2);
        }
        if ( Tag & _ButtonMask )
          ptr2->Cursor = crHandPoint;
        ptr2->Hint = IntToStr(_LampFromTag(Tag));
        if ( _ShortcutFromTag(Tag) && (ptr->Tag & (_CoinMask + _ButtonMask)) ) {
          shortcuts[_ShortcutFromTag(Tag)] = ptr;
        }
      } else if ( type == "TCheckBox" ) {
        TCheckBox *ptr2 = (TCheckBox *)ptr;
        ptr2->OnMouseDown = ShapeMouseDown;
        ptr2->OnMouseUp   = ShapeMouseUp;
        ptr2->OnMouseMove = ShapeMouseMove;
        ptr2->OnClick = CheckBoxClick;
        ptr2->Visible = false;
        Buttons[_ButtonFromTag(Tag)] = ptr;
        CheckBoxClick((TObject *)ptr);
      } else if ( type == "TzNWColorBtn" ) {
        TzNWColorBtn *ptr2 = (TzNWColorBtn *)ptr;
        TzcShapeBitColBtn *new_ptr = new TzcShapeBitColBtn(MainPanel);
        new_ptr->OnColor = ptr2->Colour1;
        new_ptr->OffColor = ptr2->Colour2;
        new_ptr->Caption = ptr2->Caption;
        new_ptr->Font = ptr2->Font;
        new_ptr->MultipleLines = ptr2->MultipleLines;
        new_ptr->SetBounds(ptr2->Left,ptr2->Top,ptr2->Width,ptr2->Height);
        new_ptr->Parent = MainPanel;
        new_ptr->ShowFocus = false;
        new_ptr->Tag = ptr2->Tag;
        new_ptr->Cursor = crHandPoint;
        new_ptr->Hint = ptr->Hint;
        if ( ptr2->Colour2 != clBlack )
          new_ptr->OffColor = ptr2->Colour2;
        else {
          new_ptr->OffColor = clBtnFace;
        }
        delete ptr;
        count--;
      } else if ( type == "TImage" ) {
        if ( Tag == 0x10000 ) {
          ((TImage *)ptr)->OnMouseDown = ShapeMouseDown;
          ((TImage *)ptr)->OnMouseUp   = ShapeMouseUp;
          ((TImage *)ptr)->OnMouseMove = ShapeMouseMove;
        } else if ( (Tag & 0x1ffff) < 255 ) {
          TReel *new_ptr = new TReel(MainPanel);
          new_ptr->Parent = MainPanel;
          new_ptr->Tag = _ReelTag(Tag & 7);
          new_ptr->SetBounds(ptr->Left, ptr->Top, ptr->Width, ptr->Height);
          if ( Tag  & _ToggleMask )
            new_ptr->Toggle = true;
          new_ptr->View = _ReelWindow(Tag);
          new_ptr->Stops = stops[_ReelStops(Tag)];
          new_ptr->Offset = _RampMask(Tag);
          new_ptr->Flag = _FlagMask(Tag);
          if ( new_ptr->Stops == 12 )
            new_ptr->WinLine = false;
          new_ptr->OnMouseDown = ShapeMouseDown;
          new_ptr->OnMouseMove = ShapeMouseMove;
          new_ptr->OnMouseUp   = ShapeMouseUp;
          Reels[_ReelFromTag(Tag)] = new_ptr;
          ReelType[_ReelFromTag(Tag)] = RT_REEL;
          delete ptr;
          count--;
        } else {
          X = _ReelSym(Tag) / 24;
          Y = _ReelSym(Tag) % 24;
          ReelSymbols[X-1][Y] = (TImage *)ptr;
        }
      } else if ( type == "TReel" ) {
        TReel *reel_ptr = (TReel *)ptr;
        if ( GeneralConfig->ConvertReel->Checked && reel_ptr->Horizontal == false ) {
          TFancyReel *new_ptr = new TFancyReel(MainPanel);
          new_ptr->Parent = MainPanel;
          new_ptr->SetBounds( reel_ptr->Left, reel_ptr->Top, reel_ptr->Width, reel_ptr->Height);
          new_ptr->Inverted    = reel_ptr->Inverted;
          new_ptr->Toggle      = reel_ptr->Toggle;
          new_ptr->Reversed    = reel_ptr->Reversed;
          new_ptr->Offset      = reel_ptr->Offset;
          new_ptr->Stops       = reel_ptr->Stops;
          new_ptr->TotalSteps  = reel_ptr->TotalSteps;
          new_ptr->Flag        = reel_ptr->Flag;
          new_ptr->OnMouseDown = ShapeMouseDown;
          new_ptr->OnMouseUp   = ShapeMouseUp;
          new_ptr->OnMouseMove = ShapeMouseMove;
          new_ptr->Number      = _ReelFromTag(Tag);
          new_ptr->Tag         = Tag;
          new_ptr->SetMask(reel_ptr->GetMask());
          new_ptr->Lamps       = reel_ptr->ReelLamps;
          new_ptr->CustomLamps = reel_ptr->CustomLamps;
          new_ptr->BorderWidth = reel_ptr->BorderWidth;
          new_ptr->BorderColour = reel_ptr->BorderColor;
          new_ptr->WinLines    = reel_ptr->WinLines;
          Reels[_ReelFromTag(Tag)] = (TFancyReel *)new_ptr;
          ReelType[_ReelFromTag(Tag)] = RT_FANCY;
          if ( reel_ptr->ReelLamps && reel_ptr->CustomLamps ) {
            for ( int i = 0; i < 3; i++ ) {
              if ( reel_ptr->GetLampNumber(i) >= 0 ) {
                new_ptr->SetLampNumber(i+1, reel_ptr->GetLampNumber(i));
                InsertLamp( reel_ptr->GetLampNumber(i), new_ptr, clYellow, i+1);
              }
            }
          }
          Graphics::TBitmap *symbols = new Graphics::TBitmap();
          symbols->Width = 80;
          symbols->Height = new_ptr->Stops * 56;
          for ( int i = 0; i < new_ptr->Stops; i++ ) {
            Graphics::TBitmap *ptr = reel_ptr->GetBitmap(i);
            TRect rect = Rect( 0, 56*i, 80, 56+56*i);
            symbols->Canvas->StretchDraw(rect, ptr);
          }
          new_ptr->SetBand(symbols);
          new_ptr->SetOverlay(reel_ptr->GetBackground());
          if ( reel_ptr->RHeight > (reel_ptr->Height * 2) )
            new_ptr->RHeight = 60;  // 1 View
          else
            new_ptr->RHeight = 190;  // 3 View
          delete symbols;
          delete reel_ptr;
          count--;
        } else {
          reel_ptr->OnMouseDown = ShapeMouseDown;
          reel_ptr->OnMouseUp   = ShapeMouseUp;
          reel_ptr->OnMouseMove = ShapeMouseMove;
          reel_ptr->Number      = _ReelFromTag(Tag);
          Reels[_ReelFromTag(Tag)] = reel_ptr;
          ReelType[_ReelFromTag(Tag)] = RT_REEL;
          if ( reel_ptr->CustomLamps ) {
            for ( int i = 0; i < 3; i++ ) {
              if ( reel_ptr->GetLampNumber(i) >= 0 )
                InsertLamp(reel_ptr->GetLampNumber(i), ptr, clYellow, i);
            }
          }
        }
      } else if ( type == "TFancyReel" ) {
        ((TFancyReel *)ptr)->OnMouseDown = ShapeMouseDown;
        ((TFancyReel *)ptr)->OnMouseUp   = ShapeMouseUp;
        ((TFancyReel *)ptr)->OnMouseMove = ShapeMouseMove;
        ((TFancyReel *)ptr)->Number      = _ReelFromTag(Tag);
        Reels[_ReelFromTag(Tag)] = (TFancyReel *)ptr;
        ReelType[_ReelFromTag(Tag)] = RT_FANCY;
//        if ( ((TReel *)ptr)->CustomLamps ) {
          for ( int i = 0; i < 5; i++ ) {
            if ( ((TFancyReel *)ptr)->GetLampNumber(i) >= 0 )
              InsertLamp(((TFancyReel *)ptr)->GetLampNumber(i), ptr, clYellow, i);
          }
//        }
      } else if ( type == "TBandReel" ) {
        ((TBandReel *)ptr)->OnMouseDown = ShapeMouseDown;
        ((TBandReel *)ptr)->OnMouseUp   = ShapeMouseUp;
        ((TBandReel *)ptr)->OnMouseMove = ShapeMouseMove;
        ((TBandReel *)ptr)->Number      = _ReelFromTag(Tag);
        Reels[_ReelFromTag(Tag)] = (TBandReel *)ptr;
        ReelType[_ReelFromTag(Tag)] = RT_BAND;
        if ( ((TBandReel *)ptr)->Lamps ) {
          for ( int i = 0; i < 5; i++ ) {
            if ( ((TBandReel *)ptr)->GetLampNumber(i) >= 0 )
              InsertLamp(((TBandReel *)ptr)->GetLampNumber(i), ptr, clYellow, i);
          }
        }
      } else if ( type == "TLabel" ) {
        ((TLabel *)ptr)->OnMouseDown = ShapeMouseDown;
        ((TLabel *)ptr)->OnMouseUp   = ShapeMouseUp;
        ((TLabel *)ptr)->OnMouseMove = ShapeMouseMove;
        if ( _HasLamp(Tag) )
          InsertLamp(_LampFromTag(Tag), ptr, ((TLabel *)ptr)->Color, 1);
      } else if ( type == "TFrame2" ) {
        ((TFrame2 *)ptr)->OnMouseDown = ShapeMouseDown;
        ((TFrame2 *)ptr)->OnMouseUp   = ShapeMouseUp;
        ((TFrame2 *)ptr)->OnMouseMove = ShapeMouseMove;
        ((TFrame2 *)ptr)->SendToBack();
      } else if ( type == "TAlpha" ) {
        TAlpha *alpha = (TAlpha *)ptr;
        alpha->OnMouseDown = TImageMouseDown;
        alpha->OnMouseUp   = TImageMouseUp;
        alpha->OnMouseMove = TImageMouseMove;
        alpha->Display     = "                ";
        alpha->Intensity   = 0;
        display[alpha->Tag] = alpha;
      } else if ( type == "TACELED" ) {
        TACELED *alpha = (TACELED *)ptr;
        alpha->OnMouseDown = TImageMouseDown;
        alpha->OnMouseUp   = TImageMouseUp;
        alpha->OnMouseMove = TImageMouseMove;
        aceled = alpha;
      } else if ( type == "TPROCONLED" ) {
        TPROCONLED *alpha = (TPROCONLED *)ptr;
        alpha->OnMouseDown = TImageMouseDown;
        alpha->OnMouseUp   = TImageMouseUp;
        alpha->OnMouseMove = TImageMouseMove;
        proconnled = alpha;
      } else if ( type == "TBFMAlpha" ) {
        TBFMAlpha *alpha = (TBFMAlpha *)ptr;
        alpha->OnMouseDown = TImageMouseDown;
        alpha->OnMouseUp   = TImageMouseUp;
        alpha->OnMouseMove = TImageMouseMove;
        bfmalpha = alpha;
      } else if ( type == "TSevenSegment" ) {
        TSevenSegment *led = (TSevenSegment *)ptr;
        led->OnMouseDown = ShapeMouseDown;
        led->OnMouseMove = ShapeMouseMove;
        led->OnMouseUp   = ShapeMouseUp;
        led->Value       = 0;
        if ( !led->Programmable )
          led_display[Tag % 40].digit = (TSevenSegment *)ptr;
        else {
          for ( int i = 0; i < 16; i++ ) {
            if ( led->GetMasks(i) > 0 && led->GetMasks(i) < 256 )
              InsertLamp( led->GetMasks(i), led, clYellow, 0, 1<<i);
            else if ( led->GetMasks(i) == -1 )
              led->Value |= (1 << i);
          }
        }
      } else if ( type == "TzLed" ) {
        ((TzLed *)ptr)->OnMouseDown = ShapeMouseDown;
        ((TzLed *)ptr)->OnMouseMove = ShapeMouseMove;
        ((TzLed *)ptr)->OnMouseUp   = ShapeMouseUp;
        ((TzLed *)ptr)->Enabled = false;
        if ( Tag & _LEDLampMask ) {
          InsertLamp(_LampFromTag(Tag), ptr, clRed, 1);
        } else {
          X = _DigitMask(Tag);
          Y = _SegmentMask(Tag);
          if ( X < 24 )
            led_display[X].led[Y] = (TzLed *)ptr;
        }
      } else if ( type == "TzcShapeBitColBtn" ) {
        TzcShapeBitColBtn *ptr2 = (TzcShapeBitColBtn *)ptr;
        ptr2->OnMouseDown = ShapeMouseDown;
        ptr2->OnMouseUp   = ShapeMouseUp;
        ptr2->OnMouseMove = ShapeMouseMove;
        ptr2->OnDragOver  = CheckLampToButton;
        ptr2->OnDragDrop  = LampToButton;
        ptr2->State  = false;
        ptr2->State2  = false;
        ptr2->Cursor = crHandPoint;
        if ( Tag & _OldCoinMask )
          ptr2->Tag |= _CoinMask;
        if ( (Tag & 0x8000 ) == 0 && (Tag & _CoinMask) == 0 )
          Buttons[_ButtonFromTag(Tag)] = ptr;
        if ( _HasLamp(Tag) ) {
          InsertLamp(_LampFromTag(Tag), ptr, ptr2->OnColor,1);
          ptr2->Tag |= _LampMask;
//          ptr2->Tag &= ~0x4000;
        }
        if ( ptr2->Lamp2 ) {
          InsertLamp(ptr2->Lamp2 - 1, ptr, ptr2->OnColor,2);
        }
        if ( _ShortcutFromTag(Tag) ) {
          shortcuts[_ShortcutFromTag(Tag)] = ptr;
        }
        if ( Tag & _LockMask )
          ptr2->Enabled = false;
        if ( Tag & 0x8000 ) {
          int coin = (Tag & 0xff00) >> 8;
          for ( j = 0; j < LENGTH(coinmasks); j++ )
            if ( coin == coinmasks[j] )
              break;
          if ( j < LENGTH(coinmasks) )
            ptr2->Tag = (Tag & 0xffff00ff) | _CoinMask | ((j + 1) << 8);
          else
            ptr2->Tag = Tag & 0xffff00ff;
        }
      } else if ( type == "TDiscReel" ) {
        TDiscReel *reel = (TDiscReel *)ptr;
        reel->OnMouseDown = TImageMouseDown;
        reel->OnMouseMove = TImageMouseMove;
        reel->OnMouseUp   = TImageMouseUp;
        reel->State       = 0;
        Reels[_ReelFromTag(Tag)] = ptr;
        ReelType[_ReelFromTag(Tag)] = RT_DISC;
        for ( int i = 0; i < 24; i++ )
          if ( reel->GetLamp(i) < NBR_LAMPS )
            InsertLamp( reel->GetLamp(i), reel, clYellow, 0, 1<<i);
      } else if ( type == "TDotmatrix" ) {
        TDotmatrix *display = (TDotmatrix *)ptr;
        display->OnMouseDown = TImageMouseDown;
        display->OnMouseUp   = TImageMouseUp;
        display->OnMouseMove = TImageMouseMove;
        dotmatrix = display;
      } else if ( type == "TVideo" ) {
        TVideo *video = (TVideo *)ptr;
        video->OnMouseDown = TImageMouseDown;
        video->OnMouseUp   = TImageMouseUp;
        video->OnMouseMove = TImageMouseMove;
        screen = video;
      } else {
        delete ptr;
        count--;
      }
    }
    for ( X = 0; X < NBR_REELS; X++ ) {
      if ( Reels[X] )
        Reels[X]->SendToBack();
      for ( Y = 0; Y < NBR_STOPS; Y++ ) {
        if ( ReelSymbols[X][Y] ) {
          if ( Reels[X] ) {
            if ( Reels[X]->ClassNameIs("TReel") )
              ((TReel *)Reels[X])->SetBitmap(Y,ReelSymbols[X][Y]->Picture->Bitmap);
          }
          delete ReelSymbols[X][Y];
          ReelSymbols[X][Y] = NULL;
        }
      }
      if ( Reels[X] && Reels[X]->ClassNameIs("TReel") )
        ((TReel *)Reels[X])->DrawReels();
    }
    MainPanel->Visible = true;
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadGameClick(TObject *Sender)
{
  if ( Fruit != NULL ) {
    if ( ExitGameBox(true) == IDCANCEL )
      return;
  }
  HaltFruit(3);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadAGame(void)
{

  SetCurrentDirectory(ScanDir.c_str());
  OpenDialog1->Filter = "MFME Game (*.gam)|*.gam";
  OpenDialog1->DefaultExt = AnsiString("gam");
  OpenDialog1->Options << ofFileMustExist;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() )
    Load(OpenDialog1->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Load(String Name)
{
char classname[100];
char caption[150];
FILE *fp;
char *FileName;
String Bitmapname;
int Tag, i;
Byte ch;
Byte *cp;
bool validres = true;

    if ( !FileExists(Name) )
      return;

    Throttle = 0;
    for ( i = 0; i < 10; i++ )
      if ( history[i] == "" || history[i] == Name )
        break;
    if ( i == 10 ) {
      for ( i = 1; i < 10; i++ )
        history[i-1] = history[i];
      i = 9;
    }
    history[i] = Name;
    UpdateHistory(true);
    CurrGamePath = ExtractFilePath(Name);
    chdir(CurrGamePath.c_str());
    Game = ExtractFileName(Name);
    DoNewLayout();

    loading = true;
    FileName = Name.c_str();
    fp = fopen( FileName, "r");
    while ( !feof(fp) ) {
      fscanf( fp, "%s", classname);
      if ( feof(fp) )
        break;
      if ( !strcmp(classname,"System") ) {
        fscanf( fp, "%s\n", caption);
        Arch = SystemType(caption);
        Config->SetArch(Arch);
      } else if ( !strcmp(classname,"DOTROM" ) ) {
        fscanf( fp, "%s\n", caption);
        if ( FileExists(caption) ) {
          DOTROMList->Add(AnsiString(caption));
          DOTROMname = AnsiString(caption);
        } else
          Application->MessageBox("ROM file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"VIDROM" ) ) {
        fscanf( fp, "%s\n", caption);
        if ( FileExists(caption) ) {
          VIDROMList->Add(AnsiString(caption));
          VIDROMname = AnsiString(caption);
        } else
          Application->MessageBox("ROM file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"ROM" ) ) {
        fscanf( fp, "%s\n", caption);
        if ( FileExists(caption) ) {
          ROMList->Add(AnsiString(caption));
          ROMname = AnsiString(caption);
        } else
          Application->MessageBox("ROM file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"Layout") ) {
        fscanf( fp, "%s\n", caption);
        if ( FileExists(caption) )
          validres = LoadResourceFile(caption);
        else
          Application->MessageBox("Layout file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"RAM") ) {
        fscanf( fp, "%s\n", caption);
        RAMname = AnsiString(caption);
      } else if ( !strcmp(classname,"DIP") ) {
        fscanf( fp, "%d %s\n", &Tag, caption);
        if ( Tag == 1 ) {
          DIP1 = StrToBin(caption);
          Config->SetDip1(DIP1);
        } else if ( Tag == 2 ) {
          DIP2 = StrToBin(caption);
          Config->SetDip2(DIP2);
        } else if ( Tag == 3 ) {
          DIP2 = StrToBin(caption);
          Config->SetDip3(DIP3);
        }
      } else if ( !strcmp(classname,"Sound") ) {
        fscanf( fp, "%s\n", caption);
        if ( FileExists(caption) ) {
          SoundList->Add(AnsiString(caption));
        } else
          Application->MessageBox("Sound file not found", "Error", MB_OK | MB_ICONWARNING);
      } else if ( !strcmp(classname,"Stake") ) {
        fscanf( fp, "%d\n", &Tag);
        if ( Tag < Config->StakeKey->Items->Count - 1 ) {
          Tag++;
          Config->StakeKey->ItemIndex = Tag;
          Config->StakeKeyChange(NULL);
        }
      } else if ( !strcmp(classname,"Jackpot") ) {
        fscanf( fp, "%d\n", &Tag);
        if ( Tag < Config->JackpotKey->Items->Count - 1 ) {
          Tag++;
          Config->JackpotKey->ItemIndex = Tag;
          Config->JackpotKeyChange(NULL);
        }
      } else if ( !strcmp(classname,"Percentage") ) {
        fscanf( fp, "%d\n", &Tag);
        if ( Tag < Config->PercentKey->Items->Count - 1 ) {
          Tag++;
          Config->PercentKey->ItemIndex = Tag;
          Config->PercentKeyChange(NULL);
        }
      } else if ( !strcmp(classname,"DMDelay") ) {
        fscanf( fp, "%d\n", &Speed2);
        Config->Delay2->Text = IntToStr(Speed2);
      } else if ( !strcmp(classname,"Delay") ) {
        fscanf( fp, "%d\n", &Speed);
        Config->TDelay->Text = IntToStr(Speed);
      } else if ( !strcmp(classname,"Throttle") ) {
        fscanf( fp, "%d\n", &Throttle);
      } else if ( !strcmp(classname,"SetPercent") ) {
        fscanf( fp, "%d\n", &Percentage);
        Config->TPercentage->Text = StrToInt(Percentage);
      } else if ( !strcmp(classname,"TotalIn") ) {
        fscanf( fp, "%d\n", &TotalIn);
      } else if ( !strcmp(classname,"TotalOut") ) {
        fscanf( fp, "%d\n", &TotalOut);
      } else if ( !strncmp(classname,"SEC",3) ) {
        Tag = classname[3] - '1';
        if ( Tag < 31 ) {
          cp = caption;
          while ( ( ch = fgetc(fp)) != '\"' )
            ;
          ch = fgetc(fp);
          while ( ch != '\"' ) {
            *cp++ = ch;
            ch = fgetc(fp);
          }
          *cp = '\0';
          fscanf( fp, "%d\n", &SEC_Counters[Tag]);
          strcpy( SEC_Strings[Tag], caption);
          SECForm->SetText(Tag, AnsiString(SEC_Strings[Tag]));
          SECForm->SetValue(Tag, SEC_Counters[Tag]);
        }
      } else if ( !strcmp(classname,"Played") ) {
        fscanf( fp, "%d\n", &TimesPlayed);
      } else
        fscanf( fp, "\n");
    }
    fclose(fp);

    BackDoorClick(BackDoor);
    CashDoorClick(CashDoor);
    RefillClick(Refill);
    ROMList->Sort();
    SoundList->Sort();
    
    if ( (ROMList->Count || VIDROMList->Count ) /*&& validres */) {
      switch ( Arch ) {
        case JPM_IMPACT:
          if ( !NoSound ) {
            Sound->SetSampleRate(8000);
            LoadJPMSound(8000);
          }
          Fruit = new TIMPACT(Game, CurrGamePath, ROMList);
          break;
        case BARCREST_MPU3:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
          }
          Fruit = new TMPU3(Game, CurrGamePath, ROMList);
          break;
        case BARCREST_MPU4:
          if ( VIDROMList->Count )
            Video = new TMPU4VIDEO(Game, CurrGamePath, VIDROMList);
          if ( !NoSound ) {
            Sound->SetSampleRate(16000);
            LoadSound();
          }
          Fruit = new TMPU4(Game, CurrGamePath, ROMList);
          break;
        case MAYGAY_M1A:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
            LoadSound();
          }
          Fruit = new TM1A(Game, CurrGamePath, ROMList);
          break;
         case JPM_MPS2:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
          }
          Fruit = new TMPS2(Game, CurrGamePath, ROMList);
          break;
        case ACE_SYSTEM1:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
          }
          Fruit = new TACE1(Game, CurrGamePath, ROMList);
          break;
        case ACE_SPACE:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
            LoadSound();
          }
          Fruit = new TSPACE(Game, CurrGamePath, ROMList);
          break;
        case BFM_SCORPION2:
          if ( DOTROMList->Count )
            Display = new TDOTMATRIX(Game, CurrGamePath, DOTROMList);
          if ( !NoSound ) {
            Sound->SetSampleRate(40000);
            LoadJPMSound(0);
          }
          Fruit = new TSCORPION2(Game, CurrGamePath, ROMList);
          break;
         case JPM_SYS80:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
          }
          Fruit = new TSYS80(Game, CurrGamePath, ROMList);
          break;
        case PROJECT_PROCONN:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
            LoadJPMSound(12000);
          }
          Fruit = new TProconn(Game, CurrGamePath, ROMList);
          break;
        case JPM_SYS5:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
            LoadJPMSound(0);
          }
          Fruit = new TSYS5(Game, CurrGamePath, ROMList);
          break;
/*        case BARCREST_MPU4VIDEO:
          if ( !NoSound ) {
            Sound->SetSampleRate(64000);
          }
          Video = new TMPU4VIDEO(Game, CurrGamePath, VIDROMList);
          break; */
        case BFM_SCORPION1:
          if ( !NoSound ) {
            Sound->SetSampleRate(8000);
            LoadJPMSound(0);
          }
          Fruit = new TSCORPION1(Game, CurrGamePath, ROMList);
          break;
      }
      if ( Fruit ) {
        Fruit->OnTerminate = FruitTerminated;
        Fruit->Suspended = false;
      }
      if ( Display ) {
        Display->OnTerminate = DisplayTerminated;
        Display->Suspended = false;
      }
      if ( Video ) {
        Video->OnTerminate = VideoTerminated;
        Video->Suspended = false;
      }
      Config->UpdateMeters();
      UpdateMeters();
    }
    loading = false;
    TimesPlayed++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::About1Click(TObject *Sender)
{
//  Credit = new TCredit(this);
  Credit->scroll->Reset();
  Credit->scroll->Enabled = true;
  Credit->ShowModal();
  Credit->scroll->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSound(void)
{
TMemoryStream *file_ptr;
int hdrcheck;
Byte filler;
int volume;


  if ( SoundList->Count ) {
    // Check for NEC sound
    file_ptr = new TMemoryStream();
    file_ptr->LoadFromFile(SoundList->Strings[0]);
    file_ptr->Position = 0;
    file_ptr->Read( &filler, 1);
    file_ptr->Read( &hdrcheck, 4);
    delete file_ptr;
    HasSampledSound = true;
    switch ( Volume_Type ) {
      case 0:
        volume = 15;
        break;
      case 1:
        volume = 8;
        break;
      case 2:
        volume = 0;
        break;
    }
    if ( hdrcheck != 0x5569A55A ) {
      Sound->LoadSound(SoundList, volume, SampleFreq);
      M1SampType = 0;
    } else {
      Sound->LoadJPMSound(SoundList, volume, 8000);
      M1SampType = 1;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadJPMSound(int rate)
{
  if ( SoundList->Count ) {
    Sound->LoadJPMSound(SoundList, 8 * (2 - Volume_Type), rate);
    HasSampledSound = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::HopperType(TObject *Sender)
{
  if (((TMenuItem *)Sender)->Checked )
    ((TMenuItem *)Sender)->Checked = false;
  else {
    ((TMenuItem *)Sender)->Checked = true;
    Hopper_Type = ((TComponent *)Sender)->Tag;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ExtenderType(TObject *Sender)
{
  if (((TMenuItem *)Sender)->Checked )
    ((TMenuItem *)Sender)->Checked = false;
  else {
    ((TMenuItem *)Sender)->Checked = true;
    Extender_Type = ((TComponent *)Sender)->Tag;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::play(int nbr, int Channel)
{
  if ( Sound->Details(nbr) || nbr == 0 )
    Sound->PlaySample2(nbr, Channel);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::play2(int nbr, int Channel)
{
  if ( Sound->Details(nbr) || nbr == 0 )
    Sound->PlaySample(nbr, Channel);
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffermpu4(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TMPU4 *)Fruit)->board.gi.Update, 0);
  Sound->UpdateSoundBuff2(((TMPU4 *)Fruit)->board.yamaha.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffermpu3(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TMPU3 *)Fruit)->board.misc.SoundUpdate, cycles);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBufferm1a(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TM1A *)Fruit)->board.gi.Update, 0);
  Sound->UpdateSoundBuff2(((TM1A *)Fruit)->board.yamaha.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffersys1(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TACE1 *)Fruit)->board.gi.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBufferproconn(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TProconn *)Fruit)->board.gi.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffermps2(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TMPS2 *)Fruit)->board.sn.SN76496Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBufferscorp2(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TSCORPION2 *)Fruit)->board.yamaha.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBufferscorp1(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TSCORPION1 *)Fruit)->board.gi.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffersys80(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TSYS80 *)Fruit)->board.gi.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffersys5(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TSYS5 *)Fruit)->board.yamaha.Update, 0);
  Sound->UpdateSoundBuff2(((TSYS5 *)Fruit)->board.saa.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBuffermpu4video(unsigned int cycles)
{
  Sound->UpdateSoundBuff3(((TMPU4VIDEO *)Video)->board.sound.Update, 0);
//  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------
void __fastcall UpdateSoundBufferspace(unsigned int cycles)
{
  Sound->UpdateSoundBuff(((TSPACE *)Fruit)->board.gi1.Update, 0);
  Sound->UpdateSoundBuff2(((TSPACE *)Fruit)->board.gi2.Update, 0);
  Fruit->sound_updates++;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BinToStr(Byte value, Char *ptr)
{
Byte mask;
Byte count;

  mask = 1;

  for ( count = 0; count < 8; count++ ) {
    if ( value & mask )
      *ptr++ = '1';
    else
      *ptr++ = '0';
    mask <<= 1;
  }
  *ptr = '\0';
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TestMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if ( Arch == BARCREST_MPU3 )
    MatrixChange3(5120, true);
  else if ( Arch == BARCREST_MPU4 )
    MatrixChange3(5376, true);
  else if ( Arch == JPM_IMPACT && Fruit != NULL )
    ((TIMPACT *)Fruit)->board.duart.ip &= ~0x20;
  else if ( Arch == MAYGAY_M1A )
    MatrixChange3(2048, true);
  else if ( Arch == BFM_SCORPION1 )
    MatrixChange3(1280, true);
  else if ( Arch == BFM_SCORPION2 )
    MatrixChange3(18432, true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TestMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
  if ( Arch == BARCREST_MPU3 )
    MatrixChange3(5120, false);
  else if ( Arch == BARCREST_MPU4 )
    MatrixChange3(5376, false);
  else if ( Arch == JPM_IMPACT && Fruit != NULL )
    ((TIMPACT *)Fruit)->board.duart.ip |= 0x20;
  else if ( Arch == MAYGAY_M1A )
    MatrixChange3(2048, false);
  else if ( Arch == BFM_SCORPION1 )
    MatrixChange3(1280, false);
  else if ( Arch == BFM_SCORPION2 )
    MatrixChange3(18432, false);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BackDoorClick(TObject *Sender)
{
  if ( ((TCheckBox *)Sender)->Checked ) {
    Config->StakeKey->Enabled = true;
    Config->JackpotKey->Enabled = true;
    Config->PercentKey->Enabled = true;
    Config->InMeter->Enabled = true;
    Config->OutMeter->Enabled = true;
    Config->TPercentage->Enabled = true;
    switch ( Arch ) {
      case BARCREST_MPU3:
        MatrixChange3( 2048, true);
        break;
      case BARCREST_MPU4:
        MatrixChange3( 5888, true);
        break;
      case JPM_IMPACT:
        MatrixChange3( 8192, true);
        break;
      case MAYGAY_M1A:
      case BFM_SCORPION1:
      case BFM_SCORPION2:
      case JPM_SYS5:
        MatrixChange3( _ButtonTag(DoorSwitch), false);
        break;
      case JPM_MPS2:
        MatrixChange3( 0, true);
        break;
      case ACE_SPACE:
        MatrixChange3( 8192, true);
        break;
    }
  } else {
    Config->StakeKey->Enabled = false;
    Config->JackpotKey->Enabled = false;
    Config->PercentKey->Enabled = false;
    Config->InMeter->Enabled = false;
    Config->OutMeter->Enabled = false;
    Config->TPercentage->Enabled = false;
    switch ( Arch ) {
      case BARCREST_MPU3:
        MatrixChange3( 2048, false);
        break;
      case BARCREST_MPU4:
        MatrixChange3( 5888, false);
        break;
      case JPM_IMPACT:
        MatrixChange3( 8192, false);
        break;
      case MAYGAY_M1A:
      case BFM_SCORPION1:
      case BFM_SCORPION2:
      case JPM_SYS5:
        MatrixChange3( _ButtonTag(DoorSwitch), true);
        break;
      case JPM_MPS2:
        MatrixChange3( 0, false);
        break;
      case ACE_SPACE:
        MatrixChange3( 8192, false);
        break;
    }
  }
  UpdateMeters();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::UpdateMeters(void)
{
float percent;
float Drift;
float pl;
char buffer[20];
int Totin, Totout;

  if ( longterm || BackDoor->Checked ) {
    Totin = TotalIn;
    Totout = TotalOut;
  } else {
    Totin = STotalIn;
    Totout = STotalOut;
  }

  if ( Totin )
    percent = ( (float)Totout / (float)Totin ) * 100;
  else
    percent = 0;

  Drift = -( (((float)Totin/10)) * (((float)(100 - Percentage)) / 100)) + ((float)((Totin - Totout) / 10));
  pl = (float)(Totout-Totin)/10;

  sprintf( buffer, "%7d", Totin);
  TotalInMeter->Text = AnsiString(buffer);
  sprintf( buffer, "%7d", Totout);
  TotalOutMeter->Text = AnsiString(buffer);

  sprintf( buffer, "%7.2f", percent);
  PercentMeter->Text = AnsiString(buffer);
  if ( !profitloss ) {
    sprintf( buffer, "%8.2f", Drift);
    Meterlabel->Caption = "Drift";
  } else {
    sprintf( buffer, "%8.2f", pl);
    Meterlabel->Caption = "Profit/Loss";
  }
  DriftMeter->Text = AnsiString(buffer);
  sprintf( buffer, "%8d", VTP);
  VTPmeter->Text = AnsiString(buffer);
  if ( Config->Visible && !BackDoor->Checked ) {
    sprintf( buffer, "%d", TotalIn);
    Config->InMeter->Text = AnsiString(buffer);
    sprintf( buffer, "%d", TotalOut);
    Config->OutMeter->Text = AnsiString(buffer);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ConfigurationClick(TObject *Sender)
{
  Config->SetArch(Arch);
  Config->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
int count;

/*  if ( EditMode ) {
    switch ( Key ) {
      case 38: // Cursor Up
        MoveGroup(0,-1);
        break;
      case 40: // Cursor Down
        MoveGroup(0,1);
        break;
      case 37: // Cursor Left
        MoveGroup(-1,0);
        break;
      case 39: // Cursor Right
        MoveGroup(1,0);
        break;
    }
  } */
  if ( Key == 38 ) {
    for ( int i = 0; i < NBR_REELS; i++ )
      if ( Reels[i] )
        ((TFancyReel *)Reels[i])->SetExpand(true);
  }
  if ( Key == 77 && Shift.Contains(ssCtrl) ) {
    if ( Sound->Mute() )
      Muted = " [Mute]";
    else
      Muted = "";
    UpdateCaption();
  }

  if ( Key == 40 ) {
    for ( int i = 0; i < NBR_REELS; i++ )
      if ( Reels[i] )
        ((TFancyReel *)Reels[i])->SetExpand(false);
  }

  if ( Key == 69 && Shift.Contains(ssCtrl) ) {
    EditModeClick(NULL);
    Key = 0;
    return;
  }
  if ( Key == 83 && Shift.Contains(ssCtrl) ) {
    SaveDat(false);
    return;
  }
  if ( Key == 187 && Shift.Contains(ssShift) && EditMode ) {
    SetScreenSize(true);
    Key = 0;
    return;
  }
  if ( Key == 189 && Shift.Contains(ssShift) && EditMode ) {
    SetScreenSize(false);
    Key = 0;
    return;
  }
  if ( Key == 84 && Shift.Contains(ssCtrl) && EditMode ) {
    LampHintToCaption();
    Key = 0;
    return;
  }
  if ( Key == 80 && Shift.Contains(ssCtrl) && Fruit != NULL ) {
    if ( !Fruit->state ) {
      Fruit->state = MS_EXECUTE_BP;
      SetEditMode(true);
    } else {
      Fruit->state = MS_RESUME;
      SetEditMode(false);
    }
    Key = 0;
    return;
  }
  if ( Key == 82 && Shift.Contains(ssCtrl) ) {
    if ( Fruit )
      Fruit->state = MS_RESET;
    if ( Display )
      Display->state = MS_RESET;
    if ( Video )
      Video->state = MS_RESET;
    Key = 0;
    return;
  }
  if ( Key == 76 && Shift.Contains(ssCtrl) && EditMode ) {
    if ( LampMode ) {
      LampMode = false;
      MeterPanel->Color = clRed;
    } else {
      LampMode = true;
      MeterPanel->Color = clYellow;
    }
    Key = 0;
    return;
  }
  switch ( Key ) {
    case 27:
      if ( GeneralConfig->EscQuitsProgram->Checked )
        HaltFruit(0);
      break;
    case 46:  // Delete
      if ( EditMode )
        DeleteBlockClick(NULL);
      break;
    case VK_F12:
      if ( longterm )
        longterm = false;
      else
        longterm = true;
      UpdateMeters();
      break;
    case VK_F11:
      if ( profitloss )
        profitloss = false;
      else
        profitloss = true;
      UpdateMeters();
      break;
    case VK_F10:
      for ( count = 0; count < NBR_REELS; count++ ) {
        if ( Reels[count] ) {
          if ( Reels[count]->ClassNameIs("TReel") ) {
            if ( ((TReel *)Reels[count])->Toggle ) {
              if ( ((TReel *)Reels[count])->RHeight == Reels[count]->Height )
                ((TReel *)Reels[count])->RHeight = ( ((TReel *)Reels[count])->RHeight * 186 ) / 140;
              else
                ((TReel *)Reels[count])->RHeight = Reels[count]->Height;
            }
          } else if ( Reels[count]->ClassNameIs("TFancyReel") ) {
            if ( ((TFancyReel *)Reels[count])->Toggle || ((TFancyReel *)Reels[count])->RHeight > 110 ) {
              if ( ((TFancyReel *)Reels[count])->RHeight == 190 )
                ((TFancyReel *)Reels[count])->RHeight = 250;  // 5 view
              else
                ((TFancyReel *)Reels[count])->RHeight = 190;  // 3 view
            }
          }
        }
      }
      break;
    case VK_F9:
      ToggleMeterPanel();
      break;
/*    case VK_F6:
      if ( screen ) {
        screen->Dump();
      }
      break;  */
    case VK_F8:
      DumpScreen();
      break;
/*    case VK_F7:
      if ( Fruit ) {
        if ( save_ptr )
          delete save_ptr;
        Byte *tmp = save_ptr = new Byte[50000];
        Fruit->state = MS_HALTED;
        while ( !Fruit->Stopped )
          Application->ProcessMessages();
        Fruit->SaveToStream(tmp);
//        save_ptr = tmp;
        Fruit->state = MS_RUNNING;
//        save_ptr->SaveToFile("fred.bin");
//        delete save_ptr;
      }
      break; */
/*    case VK_F8:
      if ( Fruit ) {
//        Byte *tmp = save_ptr;
        if ( save_ptr ) {
//        if ( !save_ptr ) {
//          save_ptr = new TMemoryStream();
//          save_ptr->LoadFromFile("fred.bin");
//        }
          Fruit->state = MS_HALTED;
          while ( !Fruit->Stopped )
            Application->ProcessMessages();
//        save_ptr->Position = 0;
          Fruit->LoadFromStream(save_ptr);
          Fruit->state = MS_RUNNING;
        }
      }
      break;  */
    default:
      if ( shortcuts[Key] != NULL && ((TControl *)shortcuts[Key])->Enabled && !EditMode )
        MatrixChange(shortcuts[Key], true);
      break;
  }
  Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
/*  if ( MovingGroup ) {
    MovingGroup = false;
    struct Points *pptr;
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
    Application->ProcessMessages();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
      pptr->comp->SetBounds( pptr->TheRect.Left + 2,
                             pptr->TheRect.Top + 2,
                             pptr->comp->Width,
                             pptr->comp->Height);
          if ( !pptr->comp->ClassNameIs("TFrame2") )
            pptr->comp->BringToFront();
    }
    Application->ProcessMessages();
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
  }  */
  if ( Sender == TotalInMeter || Sender == TotalOutMeter ) {
    Key = 0;
    return;
  }
  if ( shortcuts[Key] != NULL && !EditMode )
    MatrixChange(shortcuts[Key], false);
  Key = 0;
}
//---------------------------------------------------------------------------

String __fastcall TForm1::IntToBin(unsigned char value)
{
int count;
char buff[9];

  buff[8] = '\0';

  for ( count = 0; count < 8; count++ ) {
    if ( value & 128 )
      buff[count] = '1';
    else
      buff[count] = '0';
    value = value << 1;
  }

  return AnsiString(buff);
}
//---------------------------------------------------------------------------
String __fastcall IntToBin(unsigned short value)
{
int count;
char buff[17];

  buff[16] = '\0';

  for ( count = 0; count < 16; count++ ) {
    if ( value & 0x8000 )
      buff[count] = '1';
    else
      buff[count] = '0';
    value = value << 1;
  }

  return AnsiString(buff);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertLamp(int nbr, TObject *ptr, TColor Colour, int type, int mask)
{
struct lamp *last_ptr, *new_ptr;

  if ( nbr >= 0 && nbr < NBR_LAMPS ) {
    new_ptr = new struct lamp;
    new_ptr->ptr = (TControl *)ptr;
    new_ptr->Colour = Colour;
    new_ptr->nbr = type;
    new_ptr->mask = mask;
    if ( ptr->ClassNameIs("TLamp2") )
      new_ptr->type = LT_LAMP;
    else if ( ptr->ClassNameIs("TzcShapeBitColBtn") )
      new_ptr->type = LT_BUTTON;
    else if ( ptr->ClassNameIs("TzLED") )
      new_ptr->type = LT_LED;
    else if ( ptr->ClassNameIs("TSevenSegment") )
      new_ptr->type = LT_SEVENSEG;
    else if ( ptr->ClassNameIs("TLabel") )
      new_ptr->type = LT_LABEL;
    else if ( ptr->ClassNameIs("TReel") )
      new_ptr->type = LT_REEL;
    else if ( ptr->ClassNameIs("TFancyReel") )
      new_ptr->type = LT_FANCYREEL;
    else if ( ptr->ClassNameIs("TDiscReel") )
      new_ptr->type = LT_DISCREEL;
    else if ( ptr->ClassNameIs("TBandReel") )
      new_ptr->type = LT_BANDREEL;
    else
      new_ptr->type = LT_NONE;
    last_ptr = Lamps[nbr];
    Lamps[nbr] = new_ptr;
    new_ptr->next = last_ptr;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RemoveLamp(int lamp, TObject *ptr)
{
struct lamp **last_ptr, *lamp_ptr;

  if ( lamp >= 0 && lamp < 320 ) {
    lamp_ptr = Lamps[lamp];
    last_ptr = &Lamps[lamp];

    if ( lamp_ptr ) {
      while ( lamp_ptr && lamp_ptr->ptr != ptr ) {
        last_ptr = &lamp_ptr->next;
        lamp_ptr = lamp_ptr->next;
      }
      if ( lamp_ptr ) {
        *last_ptr = lamp_ptr->next;
        delete lamp_ptr;
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LampColour(int lamp, TObject *ptr, TColor Colour)
{
struct lamp *lamp_ptr;

  lamp_ptr = Lamps[lamp];

  if ( lamp_ptr ) {
    while ( lamp_ptr && lamp_ptr->ptr != ptr ) {
      lamp_ptr = lamp_ptr->next;
    }
    lamp_ptr->Colour = Colour;
  }
}
//---------------------------------------------------------------------------
enum Arch_Types TForm1::SystemType(char *name)
{
int i;

  for ( i = 1; i < LENGTH(arch_types); i++ ) {
    if ( !strcmp( name, arch_types[i]) )
      return (Arch_Types)i;
  }
  return UNKNOWN;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MonitorClick(TObject *Sender)
{
  switch ( Arch ) {
    default:
    case BARCREST_MPU3:
      IOMonitor->Select->TabIndex = 0;
      break;
    case BARCREST_MPU4:
      IOMonitor->Select->TabIndex = 1;
      break;
    case JPM_IMPACT:
      IOMonitor->Select->TabIndex = 3;
      break;
    case MAYGAY_M1A:
      IOMonitor->Select->TabIndex = 2;
      break;
  }
  IOMonitor->Show();
  MonitorActive = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LayoutChanged(void)
{
  if ( !loading ) {
    if ( !Layout_Changed ) {
      Layout_Changed = true;
      Editing = " [edited]";
    }
    UpdateCaption();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateCaption(void)
{
  Caption = AnsiString(GameName) + Muted + Editing;
}
//---------------------------------------------------------------------------
int __fastcall TForm1::ExitGameBox(bool Sure)
{
int result;

  if ( Layout_Changed && NotLocked() ) {
    result = Application->MessageBox("Layout has changed, Save before exit ?", "Confirm", MB_YESNOCANCEL | MB_ICONSTOP /*| MB_DEFBUTTON2*/);
    if ( result == IDYES )
      SaveDat(false);
    else if ( result == IDCANCEL )
      return IDCANCEL;
    else
      return IDOK;
  } else if ( Sure )
    return Application->MessageBox("Are you sure ?", "Confirm", MB_OKCANCEL | MB_ICONQUESTION);

  return IDOK;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RefillClick(TObject *Sender)
{
  if ( ((TCheckBox *)Sender)->Checked ) {
    switch ( Arch ) {
      case BARCREST_MPU3:
        MatrixChange3( 2304, true);
        break;
      case BARCREST_MPU4:
        MatrixChange3( 5632, true);
        break;
      case JPM_IMPACT:
        MatrixChange3( 15104, true);
        break;
     case MAYGAY_M1A:
        MatrixChange3( 6400, true);
        break;
     case BFM_SCORPION1:
     case BFM_SCORPION2:
        MatrixChange3( _ButtonTag(RefillSwitch), true);
        break;
     case ACE_SPACE:
        MatrixChange3( 8704, true);
        break;
    }
  } else {
    switch ( Arch ) {
      case BARCREST_MPU3:
        MatrixChange3( 2304, false);
        break;
      case BARCREST_MPU4:
        MatrixChange3( 5632, false);
        break;
      case JPM_IMPACT:
        MatrixChange3( 15104, false);
        break;
      case MAYGAY_M1A:
        MatrixChange3( 6400, false);
        break;
     case BFM_SCORPION1:
     case BFM_SCORPION2:
        MatrixChange3( _ButtonTag(RefillSwitch), false);
        break;
     case ACE_SPACE:
        MatrixChange3( 8704, false);
        break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
    if ( Width < 740 && !resized ) {
      MeterPanel->Height += 28;
      resized = true;
      PercentMeter->Left -= 193;
      Label37->Left -= 193;
      DriftMeter->Left   -= 180;
      Meterlabel->Left   -= 180;
    } else if ( resized && Width >= 740 ) {
      resized = false;
      MeterPanel->Height -= 28;
      PercentMeter->Left += 193;
      Label37->Left      += 193;
      DriftMeter->Left   += 180;
      Meterlabel->Left   += 180;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetScreenSize(bool dir)
{
int width, height;

  if ( dir ) {
    if ( Width < 1024 ) {
      width = 1024;
      height = 768;
    } else if ( Width < 1152 ) {
      width = 1152;
      height = 864;
    } else if ( Width < 1280 ) {
      width = 1280;
      height = 968;
    } else if ( Width < 1600 ) {
      width = 1600;
      height = 1200;
    } else {
      width = 1920;
      height = 1440;
    }
  } else {
    if ( Width > 1600 ) {
      width = 1600;
      height = 1200;
    } else if ( Width > 1280 ) {
      width = 1280;
      height = 968;
    } else if ( Width > 1152 ) {
      width = 1152;
      height = 864;
    } else if ( Width > 1024 ) {
      width = 1024;
      height = 768;
    } else {
      width = 800;
      height = 600;
    }
  }

  if ( width <= Screen->Width && height <= Screen->Height )
    SetBounds( (Screen->Width/2) - (width/2), (Screen->Height/2) - (height/2), width, height);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  switch ( Timer1->Tag ) {
    case 0:
      Timer1->Enabled = false;
      LoadGameDB();
      if ( GeneralConfig->MeterPanelOff->Checked )
        ToggleMeterPanel();
      if ( ParamCount() ) {
        Timer2->Enabled = false;
        Load(ParamStr(1));
      } else {
        if ( GeneralConfig->StartInGameManager->Checked )
          GameManager1Click(NULL);
      }
      break;
    case 1:
      SwitchImage();
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GameManager1Click(TObject *Sender)
{
  savedwidth = Width;
  savedheight = Height;
  MeterPanelVisible = MeterPanel->Visible;
  if ( Fruit ) {
    ListView1->Selected = GameInView();
    if ( ListView1->Selected ) {
      ListView1->Selected->MakeVisible(false);
      UpdateGameData();
    }
  }
  if ( !ListView1->Selected ) {
    Name->Caption = "";
    Manufacturer->Caption = "";
    ReleaseDate->Caption = "";
    MachineType->Caption = "";
    Technology->Caption = "";
    Stakes->Caption = "";
    Prizes->Caption = "";
//    GameLabel->Caption = "";
    Notes->Clear();
    Thumbnail->Bitmap = NULL;
  }
  Width = 800;
  Height = 600;
  Manager->Visible = true;
  MeterPanel->Visible = false;
  GameManager1->Visible = false;
  ExitManager->Visible = true;
  Menu = MainMenu2;
  ListView1Click(NULL);
}
//---------------------------------------------------------------------------
char * __fastcall TForm1::ReadString()
{
static char buffer[1000];
char *ptr = buffer;

  while ( *ptr = fgetc(db_fp) )
    ptr++;

  *ptr = 0;

  return buffer;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateTotals(void)
{
char buffer[20];
float perc;

  NbrGames->Caption = IntToStr(Total_Games);
  MTotalIn->Caption = FloatToStrF(Total_In/10,ffCurrency,7,0);
  MTotalOut->Caption = FloatToStrF(Total_Out/10,ffCurrency,7,0);
  MPAndL->Caption = FloatToStrF((Total_Out-Total_In)/10,ffCurrency,7,0);
  if ( Total_In > Total_Out )
    MPAndL->Font->Color = clRed;
  else
    MPAndL->Font->Color = clBlack;
  if ( Total_In )
    perc = ( (float)Total_Out / (float)Total_In ) * 100;
  else
    perc = 0;
  sprintf( buffer, "%.2f", perc);
  MTotPercentage->Caption = AnsiString(buffer);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetVTP(void)
{
  TRegIniFile *RegFile = new TRegIniFile("SOFTWARE\\CJW");
  RegFile->WriteInteger("MFME","VTP",VTP);
  RegFile->Free();
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::NotLocked(bool box)
{
#ifndef UNLOCKED
  if ( layoutsn != sn && layoutsn != UNLOCKEDSN ) {
//    if ( box )
//      Application->MessageBox("Layout is Locked", "Error", MB_OK|MB_ICONEXCLAMATION);
    return false;
  } else
#endif
    return true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Search(String Path, int Attributes, int depth)
{
TSearchRec sr;

    done = FindFirst(Path + "\\*", Attributes, sr);
    while ( !done && !ScanWindow->cancel )
    {
      if ( sr.Attr & Attributes ) {
        if ( sr.Name.SubString(1, 1) != "." ) {
          Search( Path + "\\" + sr.Name, faDirectory, depth);
        }
      }
      if ( LowerCase(ExtractFileExt( sr.Name)) == ".gam" ) {
        CheckFile(Path + "\\" + sr.Name);
      }
      scanned++;
      ScanWindow->Files->Caption = IntToStr(scanned) + " Files";
      ScanWindow->Found->Caption = IntToStr(Total_Games);
      Application->ProcessMessages();
      done = FindNext(sr);
    }
    FindClose(sr);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CheckFile(String Path)
{
FILE *fp;
char classname[50];
char caption[80];
int Tag;
bool found;
String Name, System, RealName;
String Totali, Totalo;
String Version;
int TotI, TotO, Plays;
String Status;
bool FoundROM;
bool FoundRES;
char NName[101], NVersion[8], NRealName[101];

  chdir(ExtractFilePath(Path).c_str());
  fp = fopen(Path.c_str(), "r");
  if ( fp ) {
    found = false;
    TotI = 0;
    TotO = 0;
    Plays = 0;
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
        System = AnsiString(caption);
      } else if ( found == false ) {
        break;
      } else if ( !strcmp(classname,"ROM" ) ) {
        fscanf( fp, "%s\n", caption);
        FoundROM = true;
        if ( !FileExists(caption) ) {
          Status = AnsiString(caption) + " missing";
          break;
        }
      } else if ( !strcmp(classname,"Layout") ) {
        fscanf( fp, "%s\n", caption);
        FoundRES = true;
        if ( !FileExists(caption) ) {
          Status = AnsiString(caption) + " missing";
          break;
        } else if ( ExtractFileExt( AnsiString(caption)) == ".dat" ) {
          NName[0] = 0;
          NVersion[0] = 0;
          NRealName[0] = 0;
          ReadHeader(AnsiString(caption), NName, NVersion, NRealName);
          Name = AnsiString(NName);
          Version = AnsiString(NVersion);
          RealName = AnsiString(NRealName);
        }
      } else if ( !strcmp(classname,"Sound") ) {
        fscanf( fp, "%s\n", caption);
        if ( !FileExists(caption) ) {
          Status = AnsiString(caption) + " missing";
          break;
        }
      } else if ( !strcmp(classname,"SetPercent") ) {
        fscanf( fp, "%d\n", &Tag);
      } else if ( !strcmp(classname,"TotalIn") ) {
        fscanf( fp, "%d\n", &TotI);
        Totali = IntToStr(TotI);
      } else if ( !strcmp(classname,"TotalOut") ) {
        fscanf( fp, "%d\n", &TotO);
        Totalo = IntToStr(TotO);
      } else if ( !strcmp(classname,"Played") ) {
        fscanf( fp, "%d\n", &Plays);
      } else if ( !strcmp(classname,"Game") ) {
        char *cp = NName;
        char ch;
        while ( ( ch = fgetc(fp)) == ' ' )
          ;
        while ( ch != '\n' ) {
          *cp++ = ch;
          ch = fgetc(fp);
        }
        *cp = '\0';
        Name = AnsiString(NName);
      } else if ( !strcmp(classname,"Version") ) {
        fscanf( fp, "%s\n", NVersion);
      } else if ( !strcmp(classname,"Played") ) {
        fscanf( fp, "%d\n", &Plays);
      } else
        fscanf( fp, "\n");
    }
    fclose(fp);
    if ( found && Name != "" ) {
//      Total_In += TotI;
//      Total_Out += TotO;
      if ( FoundROM == false )
        Status = "ROM entry missing";
      else if ( FoundRES == false )
        Status = "RES entry missing";
/*      if ( db_fp ) {
        fprintf( db_fp, "%s%c", Name.c_str(),0);
        fprintf( db_fp, "%s%c", Version.c_str(),0);
        fprintf( db_fp, "%s%c", System.c_str(),0);
        fprintf( db_fp, "%s%c", Path.c_str(),0);
        fprintf( db_fp, "%s%c", Status.c_str(),0);
        fprintf( db_fp, "%d%c", Plays, 0);
        fprintf( db_fp, "%d%c", TotI, 0);
        fprintf( db_fp, "%d%c", TotO, 0);
      }   */
      if ( RealName == "" )
        AddToList( Name, Version, System, Status, Path, Plays, TotI, TotO);
      else
        AddToList( RealName, Version, System, Status, Path, Plays, TotI, TotO);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::AddToList(String Name, String Version, String System, String Status, String Path, int Plays, int TotI, int TotO)
{
char caption[80];
//char buffer[50];
//TListItem *ListItem;
float Percentage;
TStringList * ptr = new TStringList;

/*  ListItem = ListView1->Items->Add();
//  ListItem = games->Add();
  ListItem->Caption = Name;
  ListItem->SubItems->Add(Version);
  ListItem->SubItems->Add(System);
  ListItem->SubItems->Add(IntToStr(Plays));
  ListItem->SubItems->Add(IntToStr(TotI));
  ListItem->SubItems->Add(IntToStr(TotO));
  if ( TotI )
    Percentage = ( (float)TotO / (float)TotI ) * 100;
  else
    Percentage = 0;
  sprintf( caption, "%.2f", Percentage);
  ListItem->SubItems->Add(AnsiString(caption));
  ListItem->SubItems->Add(Status);
  ListItem->SubItems->Add(Path);  */
  ptr->Add(Name);
  ptr->Add(Version);
  ptr->Add(System);
  filters->Add(System);
  ptr->Add(IntToStr(Plays));
  ptr->Add(IntToStr(TotI));
  ptr->Add(IntToStr(TotO));
  if ( TotI )
    Percentage = ( (float)TotO / (float)TotI ) * 100;
  else
    Percentage = 0;
  sprintf( caption, "%.2f", Percentage);
  ptr->Add(AnsiString(caption));
  ptr->Add(Status);
  ptr->Add(Path);
  games->Add(ptr);
  Total_Games++;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1DblClick(TObject *Sender)
{
    RunGameClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1ColumnClick(TObject *Sender,
      TListColumn *Column)
{
  ListView1->CustomSort(CustomSortProc, Column->Index);
  sortstat[Column->Index] *= -1;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListView1Click(TObject *Sender)
{
String pic;

  if ( ListView1->Selected ) {
    if ( ListView1->Selected->SubItems->Strings[6] == "OK" )
      LaunchGame1->Enabled = true;
    AddNotes1->Enabled = true;
    GameImageNumber = 0;
    Thumbnail->Bitmap = NULL;
    pic = ListView1->Selected->SubItems->Strings[7];
    pic = pic.SubString(1, pic.Length() - 4);
    GameImage = pic;
//    GameLabel->Caption = ExtractFileName(pic);
    ReadDBFile(pic + ".db");
    SwitchImage();
    Timer1->Tag = 1;
    Timer1->Interval = 1000;
    Timer1->Enabled = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SwitchImage()
{
bool firstpass = false;

  if ( GameImageNumber == 0 )
    firstpass = true;

  switch ( GameImageNumber ) {
    case 0:
      GameImageNumber++;
      if ( FileExists(GameImage + ".bmp") ) {
        DisplayScreenShot(GameImage + ".bmp");
        return;
      }
    default:
      while ( GameImageNumber < 10 ) {
        if ( FileExists(GameImage + IntToStr(GameImageNumber) + ".bmp") ) {
          DisplayScreenShot(GameImage + IntToStr(GameImageNumber) + ".bmp");
          GameImageNumber++;
          return;
        }
        GameImageNumber++;
      }
      GameImageNumber++;
      if ( GameImageNumber > 10 ) {
        GameImageNumber = 0;
        if ( !firstpass )
          SwitchImage();
        else
          Timer1->Enabled = false;
      }
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DisplayScreenShot(String Name)
{
//Graphics::TBitmap * ptr = new Graphics::TBitmap();

//  ptr->LoadFromFile(Name);
//  if ( ptr->Height > ptr->Width )
//    ;
//  else
//    ;
  Thumbnail->Bitmap->LoadFromFile(Name);
//  delete ptr;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RunGameClick(TObject *Sender)
{
    if ( ListView1->Selected && ListView1->Selected->SubItems->Strings[6] == "OK" ) {
      LaunchGame1->Enabled = false;
      AddNotes1->Enabled = false;
      HaltFruit(4);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SetvtpClick(TObject *Sender)
{
    VTP = Total_In;
    SetVTP();
    UpdateMeters();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BackupRAMClick(TObject *Sender)
{
int count;
String src;
String dst;
TFileStream *srcp, *dstp;
int total = 0;
char message[50];

    if ( !DirectoryExists( PathName + "\\RAMBackups" ) )
      CreateDir(PathName + "\\RAMBackups");
    for ( count = 0; count < ListView1->Items->Count; count++ ) {
      src = ListView1->Items->Item[count]->SubItems->Strings[7];
      src = src.SubString( 1, src.Length() - 3) + "ram";
      if ( FileExists(src) ) {
        dst = PathName + "RAMBackups\\" + ExtractFileName(src);
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

void __fastcall TForm1::BrowseClick(TObject *Sender)
{
    AnsiString Dir = ScanDir;
    if ( SelectDirectory( Dir, TSelectDirOpts(), 0) ) {
      ScanDir = Dir.SubString(1, 1).UpperCase() + Dir.SubString(2, Dir.Length()-1);
      TRegIniFile *RegFile = new TRegIniFile("SOFTWARE\\CJW");
      RegFile->WriteString("MFME","BrowseDir",ScanDir);
      RegFile->Free();
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ScanClick(TObject *Sender)
{
//  ListView1->Items->Clear();
  Total_Games = 0;
  Total_In = 0;
  Total_Out = 0;
  UpdateTotals();
  ScanWindow->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DoScan(void)
{
String dir, games;

    Total_Games = 0;
    Total_In = 0;
    Total_Out = 0;
    scanned = 0;
    filters->Clear();
    filters->Sorted = true;
    ListView1->Items->BeginUpdate();
    ListView1->Items->Clear();
    ListView1->Items->EndUpdate();
    ClearGameList();
    Search( ScanDir, faDirectory, 0);
    done = true;
    ScanWindow->cancel = false;
    ScanWindow->ModalResult = mrOk;
    CreateFilterMenu();
    PopulateListView();
    if ( Fruit ) {
      ListView1->Selected = GameInView();
      if ( ListView1->Selected ) {
        ListView1->Selected->MakeVisible(false);
        UpdateGameData();
      }
    }
//    UpdateTotals();
//    TRegIniFile *RegFile = new TRegIniFile("SOFTWARE\\CJW");
//    RegFile->WriteString("MFME","BrowseDir",ScanDir);
//    RegFile->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::HideClick(TObject *Sender)
{
  Timer1->Enabled = false;
  Manager->Visible = false;
  GameManager1->Visible = true;
  ExitManager->Visible = false;
  LaunchGame1->Enabled = false;
  AddNotes1->Enabled = false;
  Menu = MainMenu1;
  if ( MeterPanelVisible )
    MeterPanel->Visible = true;
  Width = savedwidth;
  Height = savedheight;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EnableConfig(bool state)
{
int i;
TMenuItem *ptr;

  LayoutNotes->Notes->ReadOnly = !state;
  if ( state )
    LayoutNotes->BorderStyle = bsSizeable;
  else
    LayoutNotes->BorderStyle = bsSingle;
  CompEnable(Config->Reels, state);
  CompEnable(Config->Payout, state);
  CompEnable(Config->ProgramType, state);
  CompEnable(Config->SegmentDisplay, state);
  CompEnable(Config->Extender, state);
  CompEnable(Config->CharType, state);
  CompEnable(Config->Meters, state);
  CompEnable(Config->Snd, state);
  CompEnable(Config->TGameName, state);
  CompEnable(Config->TGameVersion, state);
  CompEnable(Config->TMeterSense, state);
  CompEnable(Config->TTriacSense, state);
  CompEnable(Config->LVD, state);
  CompEnable(Config->ReelLamps, state);
  CompEnable(Config->CharacterAddr, state);
//  CompEnable(Config->StakeKey, state);
//  CompEnable(Config->JackpotKey, state);
  CompEnable(Config->PayoutSense, state);
  CompEnable(Config->Misc, state);
  CompEnable(Config->Freq, state);
  CompEnable(Config->Switches, state);
  CompEnable(Config->CoinMech, state);
  CompEnable(Config->Aux, state);
  CompEnable(Config->RotarySwitch, state);
  CompEnable(Config->MPU4Type, state);
  CompEnable(Config->sys5sound, state);
  CompEnable(Config->THoppers, state);
  CompEnable(Config->IOMapping, state);
  ptr = MainMenu1->Items->Items[2];
  for ( i = 0; i < ptr->Count; i++ )
    ptr->Items[i]->Visible = state;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::CompEnable(TWinControl *ptr, bool state)
{
int i;

  ptr->Enabled = state;
  for ( i = 0; i < ptr->ControlCount; i++ )
    ptr->Controls[i]->Enabled = state;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ReadHeader(String Filename, char *name, char *version, char *realname)
{
unsigned int number, length;
TECLFileStream *res_ptr;
TMemoryStream *read_ptr;

  try {
    res_ptr = new TECLFileStream(Filename, fmOpenRead, "", eclNone);
    read_ptr = new TMemoryStream();
    read_ptr->SetSize(5000);
    res_ptr->Read( read_ptr->Memory, 5000);
  }
  catch ( Exception &exception ) {
    delete res_ptr;
    delete read_ptr;
    return;
  }
  delete res_ptr;
  if ( read_ptr != NULL ) {
    do {
      read_ptr->Read(&number,4);
      read_ptr->Read(&length,4);
      switch ( number ) {
        case 12:
          read_ptr->Read(name, length);
          name[length] = 0;
          break;
        case 13:
          read_ptr->Read(version, length);
          version[length] = 0;
          break;
        case 44:
          read_ptr->Read(realname, length);
          name[length] = 0;
          break;
        case 0xffffffff:
          break;
        default:
          read_ptr->Position += length;
          break;
      }
    } while ( number != 0xffffffff && number < 44 );
    delete read_ptr;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DumpScreen(void)
{
Graphics::TBitmap *ptr = new Graphics::TBitmap();
TRect area = Rect(0, 0, MainPanel->Width, MainPanel->Height);
String Filename;
int version = 1;

  ptr->Width = MainPanel->Width;
  ptr->Height = MainPanel->Height;
  ptr->PixelFormat = pf24bit;

  ptr->Canvas->CopyRect(area, MainPanel->Canvas,area);
  if ( Game != "" )
    Filename = CurrGamePath + Game.SubString(1, Game.Pos(".")-1);
  else
    Filename = "screenshot";
  while ( FileExists(Filename+IntToStr(version)+".bmp") )
    version++;
  ptr->SaveToFile(Filename+IntToStr(version)+".bmp");
  delete ptr;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Shutdown(void)
{
  if ( !NoSound )
    delete Sound;
  delete Thumbnail;
  FreeDirectInput();
  SaveGameDB();
  Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExtractOffImagesClick(TObject *Sender)
{
int loop;
int which = ((TComponent *)Sender)->Tag;

  if ( NotLocked() && EditMode ) {
    for ( loop = 0; loop < MainPanel->ControlCount; loop++ ) {
      if ( MainPanel->Controls[loop]->ClassNameIs("TLamp2") ) {
        TLamp2 *ptr = (TLamp2 *)MainPanel->Controls[loop];
        ExtractBitmapClick(ptr, which);
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MaskOffImagesClick(TObject *Sender)
{
int loop;
Graphics::TBitmap *mask = new Graphics::TBitmap();
Graphics::TBitmap *mask2 = new Graphics::TBitmap();

  if ( NotLocked() && EditMode ) {
    for ( loop = 0; loop < MainPanel->ControlCount; loop++ ) {
      if ( MainPanel->Controls[loop]->ClassNameIs("TLamp2") ) {
        TLamp2 *ptr = (TLamp2 *)MainPanel->Controls[loop];
        if ( ptr->Trans ) {
          mask->Width = ptr->OnImage1->Width;
          mask->Height = ptr->OnImage1->Height;
          mask2->Width = ptr->OnImage1->Width;
          mask2->Height = ptr->OnImage1->Height;
          TRect rect = Rect(0,0,ptr->Width,ptr->Height);
          mask->Assign(ptr->OnImage1);
          mask->Mask(ptr->OnImage1->Canvas->Pixels[0][ptr->OnImage1->Height-1]);
          mask->PixelFormat = pf24bit;
          mask2->Assign(mask);
          mask2->Canvas->CopyMode = cmSrcErase;
          mask2->Canvas->CopyRect(rect, ptr->OffImage->Canvas, rect);
          mask->Canvas->CopyMode = cmSrcAnd;
          mask->Canvas->CopyRect(rect, ptr->OnImage1->Canvas, rect);
          mask2->Canvas->CopyMode = cmSrcPaint;
          mask2->Canvas->CopyRect(rect, mask->Canvas, rect);
          ptr->OffImage->Assign(mask2);
        }
      }
    }
    delete mask;
    delete mask2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExtractOneClick(TObject *Sender)
{
int which = ((TComponent *)Sender)->Tag;

  if ( menu_ptr->ClassNameIs("TLamp2") )
    ExtractBitmapClick((TLamp2 *)menu_ptr, which);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ChangeVolume(void)
{
  if ( Sound ) {
    if ( SoundList->Count )
      Sound->LoadSound(SoundList, 8 * (2 - Volume_Type), SampleFreq);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Samples1Click(TObject *Sender)
{
  if ( Sampler == NULL )
    Sampler = new TSampler(this);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpdateStatus(int state)
{
  if ( (state & 0x30) == 0 )
    Status->Brush->Color = clBtnFace;
  else if ( (state & 0x30) == 0x10 )
    Status->Brush->Color = clRed;
  else if ( (state & 0x30) == 0x20 )
    Status->Brush->Color = clLime;
  else
    Status->Brush->Color = clYellow;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadIMPACTROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "IMPACT ROM images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( (OpenDialog1->Files->Count % 2) == 0 && OpenDialog1->Files->Count ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = JPM_IMPACT;
      Sound->SetSampleRate(8000);
      Fruit = new TIMPACT(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonMatrixJPM1Click(TObject *Sender)
{
int X, Y;

  for ( X = 0; X < 10; X++ ) {
    for ( Y = 0; Y < 8; Y++ ) {
      if ( Buttons[Y+X*8] == NULL )
        CreateBitmapButton(Y+X*8, BiasX + Y * 80, BiasY + X * 30, IntToStr(Y+X*8));
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LampMatrixJPM1Click(TObject *Sender)
{
int X, Y;

  for ( X = 0; X < 16; X++ ) {
    for ( Y = 0; Y < 16; Y++ ) {
//      if ( Lamps[Y + X * 16] == NULL )
        CreateNewLamp(Y + X * 16,Y + X * 16,BiasX + Y * 20,BiasY + X * 20);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SnapToGridClick(TObject *Sender)
{
  if ( EditMode ) {
    if ( SnapToGrid->Checked ) {
      XGrid = 0;
      YGrid = 0;
      SnapToGrid->Checked = false;
    } else {
      if ( SnapForm->ShowModal() == mrOk ) {
        XGrid = SnapForm->X->Text.ToIntDef(0);
        YGrid = SnapForm->Y->Text.ToIntDef(0);
        if ( XGrid | YGrid )
          SnapToGrid->Checked = true;
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::ChangetocheckboxClick(TObject *Sender)
{
  ButtonCheckBox(menu_ptr);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CashDoorClick(TObject *Sender)
{
  if ( ((TCheckBox *)Sender)->Checked ) {
    switch ( Arch ) {
      case JPM_IMPACT:
        MatrixChange3( 15616, true);
        break;
      case MAYGAY_M1A:
      case BFM_SCORPION1:
      case BFM_SCORPION2:
        MatrixChange3( _ButtonTag(CashSwitch), false);
        break;
      case ACE_SPACE:
        MatrixChange3( 8448, true);
        break;
   }
  } else {
    switch ( Arch ) {
      case JPM_IMPACT:
        MatrixChange3( 15616, false);
        break;
      case MAYGAY_M1A:
      case BFM_SCORPION1:
      case BFM_SCORPION2:
        MatrixChange3( _ButtonTag(CashSwitch), true);
        break;
      case ACE_SPACE:
        MatrixChange3( 8448, false);
        break;
    }
  }
  UpdateMeters();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SendTobackClick(TObject *Sender)
{
  menu_ptr->SendToBack();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DeleteCharacterClick(TObject *Sender)
{
  CharLoaded = false;
  for ( int i = 0; i < 72; i++ )
    Character[i] = 0;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SetBackgroundClick(TObject *Sender)
{
TRect Area;
Graphics::TBitmap *ptr = new Graphics::TBitmap();

//  ptr->PixelFormat = pf24bit;
  ptr->Width = menu_ptr->Width;
  ptr->Height = menu_ptr->Height;
  ptr->PixelFormat = pf24bit;
  Area = menu_ptr->BoundsRect;
  ptr->Canvas->CopyRect(Rect(0,0,menu_ptr->Width, menu_ptr->Height), MainPanel->Background->Canvas, Area);
  if ( menu_ptr->ClassNameIs("TReel") )
  ((TReel *)menu_ptr)->SetBackground(ptr);
  else if ( menu_ptr->ClassNameIs("TFancyReel") )
    ((TFancyReel *)menu_ptr)->SetOverlay(ptr);
  else if ( menu_ptr->ClassNameIs("TBandReel") )
    ((TBandReel *)menu_ptr)->SetOverlay(ptr);
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadHistory(TObject *Sender)
{
  historynbr = ((TMenuItem *)Sender)->Tag;
  if ( Fruit != NULL ) {
    if ( ExitGameBox(false) == IDCANCEL )
      return;
  }
  HaltFruit(5);
}

//---------------------------------------------------------------------------

void __fastcall TForm1::UpdateHistory(bool updateregistry)
{
TMenuItem *ptr;
int loop, j;
String Name;
bool exists;

  TRegIniFile *RegFile = new TRegIniFile("SOFTWARE\\CJW");
  ptr = MainMenu1->Items->Items[0];
  for ( loop = 0; loop < 10; ) {
    if ( (history[loop] != "") && ( exists = FileExists(history[loop])) ) {
      Name = ExtractFileName(history[loop]);
      Name = Name.SubString( 1, Name.Length() - 4);
      ptr->Items[6+loop]->Caption = IntToStr(loop+1) + ".  " + Name;
      ptr->Items[6+loop]->Visible = true;
      ptr->Items[5]->Visible = true;
    } else {
      ptr->Items[6+loop]->Visible = false;
      if ( !exists && history[loop] != "" ) {
        updateregistry = true;
        for ( j = loop; j < 9; j++ )
          history[j] = history[j+1];
        history[j] = "";
        continue;
      }
    }
    loop++;
    }
    if ( updateregistry ) {
    for ( loop = 0; loop < 10; loop++ ) {
      char buffer[20];
      sprintf(buffer,"history%d", loop);
      RegFile->WriteString("MFME",buffer,history[loop]);
    }
  }
  RegFile->Free();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ResetHistoryClick(TObject *Sender)
{
int i;

  for ( i = 0; i < 10; i++ )
    history[i] = "";
  UpdateHistory(true);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DiscReelClick(TObject *Sender)
{
TDiscReel *reel_ptr;
int count;

  for ( count = 0; count < NBR_REELS; count++ )
    if ( Reels[count] == NULL ) {
      reel_ptr = new TDiscReel(MainPanel);
      reel_ptr->Parent = MainPanel;
      reel_ptr->SetBounds( BiasX, BiasY, 160, 160);
      reel_ptr->Tag = count;
//      reel_ptr->Stops = 16;
      reel_ptr->OnMouseDown = TImageMouseDown;
      reel_ptr->OnMouseMove = TImageMouseMove;
      reel_ptr->OnMouseUp   = TImageMouseUp;
//      reel_ptr->Number      = count;
      Reels[count] = reel_ptr;
      ReelType[count] = RT_DISC;
      break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer)
{
  ShapeMouseDown( Sender, Button, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y, TCustomLayer *Layer)
{
  ShapeMouseMove( Sender, Shift, X, Y);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer)
{
  ShapeMouseUp( Sender, Button, Shift, X, Y);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::LoadM1AROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "M1A ROM images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
//    if ( (OpenDialog1->Files->Count % 2) == 0 && OpenDialog1->Files->Count ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = MAYGAY_M1A;
      Sound->SetSampleRate(8000);
      Fruit = new TM1A(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
//    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChangeToGraphic1Click(TObject *Sender)
{
int count;
TControl *ptr;
String tmp;
TLamp2 *lamp_ptr;

  if ( NotLocked() && EditMode ) {
    if ( Application->MessageBox("Convert to graphics, Are you sure ?",
                      "Confirm", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDOK ) {
      for ( count = 0; count < MainPanel->ControlCount; count++ ) {
        ptr = MainPanel->Controls[count];
        if ( ptr->ClassNameIs("TLamp2") ) {
          lamp_ptr = (TLamp2 *)ptr;
          lamp_ptr->Caption = "";
          lamp_ptr->SurfaceType = stBrushImage;
          lamp_ptr->Trans = true;
          ptr->Tag |= _NoOutLineMask;
        }
      }
      LayoutChanged();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadM1ASoundROM1Click(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "M1A Sound (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadSound();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LampMatrixM1A1Click(TObject *Sender)
{
int X, Y;

  for ( X = 0; X < 16; X++ ) {
    for ( Y = 0; Y < 8; Y++ ) {
      if ( Lamps[Y + X * 8] == NULL )
        CreateNewLamp(Y + X * 8,Y + X * 8,BiasX + X * 20,BiasY + Y * 20);
    }
    for ( Y = 0; Y < 8; Y++ ) {
      if ( Lamps[Y + 128 + X * 8] == NULL )
        CreateNewLamp(Y + 128 + X * 8,Y + 128 + X * 8,BiasX + X * 20,BiasY + (Y + 8) * 20);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::LoadMPS2ROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "MPS2 ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = JPM_MPS2;
      Sound->SetSampleRate(64000);
      Fruit = new TMPS2(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FancyReelClick(TObject *Sender)
{
TFancyReel *reel_ptr;
int count;

  for ( count = 0; count < NBR_REELS; count++ )
    if ( Reels[count] == NULL ) {
      reel_ptr = new TFancyReel(MainPanel);
      reel_ptr->Parent = MainPanel;
      reel_ptr->SetBounds( BiasX, BiasY, 80, 140);
      reel_ptr->Tag = _ReelTag(count);
      reel_ptr->OnMouseDown = ShapeMouseDown;
      reel_ptr->OnMouseMove = ShapeMouseMove;
      reel_ptr->OnMouseUp   = ShapeMouseUp;
      reel_ptr->Number      = count;
      Reels[count] = reel_ptr;
      ReelType[count] = RT_FANCY;
      break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSYS1ROM1Click(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "ACE System-One ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = ACE_SYSTEM1;
      Sound->SetSampleRate(64000);
      Fruit = new TACE1(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSCORPION2ROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "BFM Scorpion2 ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = BFM_SCORPION2;
      Sound->SetSampleRate(64000);
      Fruit = new TSCORPION2(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSCORPION2SoundROMClick(TObject *Sender)
{
TMemoryStream *file_ptr;
int count;
int hdrcheck;
Byte filler;

  OpenDialog1->Filter = "SCORPION2 Sound (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();

    if ( !NoSound ) {
      if ( SoundList->Count ) {
        // Check for NEC sound
        file_ptr = new TMemoryStream();
        file_ptr->LoadFromFile(SoundList->Strings[0]);
        file_ptr->Position = 0;
        file_ptr->Read( &filler, 1);
        file_ptr->Read( &hdrcheck, 4);
        delete file_ptr;
        if ( hdrcheck != 0x5569A55A ) {
          Sound->LoadYMZSound(SoundList, 8 * (2 - Volume_Type));
          HasSampledSound = true;
        } else {
          LoadJPMSound(0);
        }
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadBFMDotmatrixROM1Click(TObject *Sender)
{
int count;

  if ( Display != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "BFM DotMatrix ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        DOTROMList->Add(OpenDialog1->Files->Strings[count]);
      DOTROMList->Sort();
      DOTROMname = ExtractFileName(OpenDialog1->FileName);
      Display = new TDOTMATRIX(Game, CurrGamePath, ROMList);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SendChar(Byte value)
{
  if ( Display ) {
    ((TDOTMATRIX *)Display)->QueueChar(value);
//    ((TDOTMATRIX *)Display)->board.chr = value;
//    ((TDOTMATRIX *)Display)->board.latch = 1;
//    ((TDOTMATRIX *)Display)->board.irq = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ResetMatrix(void)
{
  if ( Display ) {
    ((TDOTMATRIX *)Display)->state = MS_RESET;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WriteCharToVideo(char ch)
{
  if ( Video ) {
    ((TMPU4VIDEO *)Video)->board.acia.receive_char(ch);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::VideoCharReceived()
{
  if ( Fruit ) {
      ((TMPU4 *)Fruit)->board.acia.char_received();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WriteCharToMPU4(char ch)
{
  if ( Fruit ) {
    ((TMPU4 *)Fruit)->board.acia.receive_char(ch);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MPU4CharReceived()
{
  if ( Video ) {
    ((TMPU4VIDEO *)Video)->board.acia.char_received();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetMPU4DCD(Byte value)
{
  if ( Fruit ) {
    ((TMPU4 *)Fruit)->board.acia.set_dcd(value);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SetMPU4CTS(Byte value)
{
  if ( Fruit ) {
    ((TMPU4 *)Fruit)->board.acia.set_cts(value);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DotMatrixDisplayClick(TObject *Sender)
{
TDotmatrix *new_ptr;

  if ( dotmatrix == NULL ) {
    new_ptr = new TDotmatrix(MainPanel);
    new_ptr->Tag = 0;
    new_ptr->Top = BiasY;
    new_ptr->Left = BiasX;
    new_ptr->OnMouseDown = TImageMouseDown;
    new_ptr->OnMouseMove = TImageMouseMove;
    new_ptr->OnMouseUp   = TImageMouseUp;
    new_ptr->Parent = MainPanel;
    LayoutChanged();
    dotmatrix = new_ptr;
  }
}
//---------------------------------------------------------------------------

int __fastcall TForm1::LatchState(void)
{
  if ( Display ) {
    if ( /*(((TDOTMATRIX *)Display)->board.cc & 0x10 ) ||*/
         /*(((TDOTMATRIX *)Display)->board.latch == 1) ||*/
         ((((TDOTMATRIX *)Display)->board.status & 8 ) == 0) ) {
       return 0x40;
    }
  }
  return 0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DotMatrixBoard1Click(TObject *Sender)
{
  MatrixDebug->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSYSTEM80ROM1Click(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "SYS80 ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = JPM_SYS80;
      Sound->SetSampleRate(64000);
      Fruit = new TSYS80(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BandReel1Click(TObject *Sender)
{
TBandReel *reel_ptr;
int count;

  for ( count = 0; count < NBR_REELS; count++ )
    if ( Reels[count] == NULL ) {
      reel_ptr = new TBandReel(MainPanel);
      reel_ptr->Parent = MainPanel;
      reel_ptr->SetBounds( BiasX, BiasY, 200, 80);
      reel_ptr->Tag = _ReelTag(count);
      reel_ptr->Stops = 16;
      reel_ptr->OnMouseDown = ShapeMouseDown;
      reel_ptr->OnMouseMove = ShapeMouseMove;
      reel_ptr->OnMouseUp   = ShapeMouseUp;
      reel_ptr->Number      = count;
      Reels[count] = reel_ptr;
      ReelType[count] = RT_BAND;
      break;
    }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadProconnROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "Project Proconn ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = PROJECT_PROCONN;
      Sound->SetSampleRate(64000);
      Fruit = new TProconn(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonMatrixBFMClick(TObject *Sender)
{
int X, Y;

  for ( X = 0; X < 12; X++ ) {
    for ( Y = 0; Y < 6; Y++ ) {
      if ( Buttons[Y+X*8] == NULL )
        CreateBitmapButton(Y+X*8, BiasX + Y * 80, BiasY + X * 30, IntToStr(X) + "," + IntToStr(Y));
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSYS5ROM1Click(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "JPM SYS5 ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = JPM_SYS5;
      Sound->SetSampleRate(64000);
      Fruit = new TSYS5(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::LoadMPU4VideoROMClick(TObject *Sender)
{
int count;

  if ( Video != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "MPU4 Video ROM images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( OpenDialog1->Files->Count ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        VIDROMList->Add(OpenDialog1->Files->Strings[count]);
      VIDROMList->CustomSort(NumericSort);
      VIDROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = BARCREST_MPU4;
      Sound->SetSampleRate(64000);
      Video = new TMPU4VIDEO(Game, CurrGamePath, VIDROMList);
      Video->OnTerminate = VideoTerminated;
      Video->Suspended = false;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VideoScreenClick(TObject *Sender)
{
  if ( screen == NULL ) {
    screen = new TVideo(MainPanel);
    screen->Parent = MainPanel;
    screen->SetBounds( BiasX, BiasY, 504, 296);
    screen->OnMouseDown = TImageMouseDown;
    screen->OnMouseMove = TImageMouseMove;
    screen->OnMouseUp   = TImageMouseUp;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::VideoBoard1Click(TObject *Sender)
{
  VideoDebug->Show();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSCORPION1ROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "BFM Scorpion1 ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = BFM_SCORPION1;
      Sound->SetSampleRate(64000);
      Fruit = new TSCORPION1(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
      Fruit->Suspended = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSCORPION1SoundROM1Click(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "SCORPION1 Sound images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadJPMSound(6000);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSYS5SoundROMClick(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "SYS5 Sound images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadJPMSound(8000);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Notes1Click(TObject *Sender)
{
  LayoutNotes->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer2Timer(TObject *Sender)
{
  Timer2->Enabled = false;
  if ( !GeneralConfig->NoAboutBox->Checked ) {
//    Credit = new TCredit(this);
    Credit->scroll->Reset();
    Credit->scroll->Enabled = true;
    Credit->ShowModal();
    Credit->scroll->Enabled = false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::GeneralClick(TObject *Sender)
{
  GeneralConfig->ShowModal();
  if ( GeneralConfig->LongTermMeters->Checked )
    longterm = true;
  else
    longterm = false;
  UpdateMeters();
  PopulateListView();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::AddNotes1Click(TObject *Sender)
{
String pic;

  if ( ListView1->Selected ) {
    if ( GameNotes->ShowModal() == mrOk ) {
      Name->Caption = GameNotes->Name->Text;
      Manufacturer->Caption = GameNotes->Manufacturer->Text;
      ReleaseDate->Caption = GameNotes->ReleaseDate->Text;
      MachineType->Caption = GameNotes->MachineType->Text;
      Technology->Caption = GameNotes->Technology->Text;
      Stakes->Caption = GameNotes->Stakes->Text;
      Prizes->Caption = GameNotes->Prizes->Text;
      Notes->Text = GameNotes->Notes->Text;
      pic = ListView1->Selected->SubItems->Strings[7];
      pic = pic.SubString(1, pic.Length() - 4);
      pic = pic + ".db";
      db_fp = fopen(pic.c_str(),"wb");
      if ( db_fp ) {
        fprintf( db_fp, "%s%c", Name->Caption.c_str(),0);
        fprintf( db_fp, "%s%c", Manufacturer->Caption.c_str(),0);
        fprintf( db_fp, "%s%c", ReleaseDate->Caption.c_str(),0);
        fprintf( db_fp, "%s%c", MachineType->Caption.c_str(),0);
        fprintf( db_fp, "%s%c", Technology->Caption.c_str(),0);
        fprintf( db_fp, "%s%c", Stakes->Caption.c_str(),0);
        fprintf( db_fp, "%s%c", Prizes->Caption.c_str(), 0);
        int length = Notes->GetTextLen();
        length++;
        char *buff = new char[length];
        Notes->GetTextBuf(buff,length);
        fprintf( db_fp, "%s%c", buff, 0);
        delete buff;
        fprintf( db_fp, "%c", 0);
        fclose( db_fp );
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ReadDBFile(String Name)
{
  if ( FileExists(Name) ) {
    db_fp = fopen(Name.c_str(), "rb");
    if ( db_fp ) {
      Form1->Name->Caption = AnsiString(ReadString());
      Manufacturer->Caption = AnsiString(ReadString());
      ReleaseDate->Caption = AnsiString(ReadString());
      MachineType->Caption = AnsiString(ReadString());
      Technology->Caption = AnsiString(ReadString());
      Stakes->Caption = AnsiString(ReadString());
      Prizes->Caption = AnsiString(ReadString());
      Notes->Text = AnsiString(ReadString());
      fclose(db_fp);
    }
  } else {
    Form1->Name->Caption = "";
    Manufacturer->Caption = "";
    ReleaseDate->Caption = "";
    MachineType->Caption = "";
    Technology->Caption = "";
    Stakes->Caption = "";
    Prizes->Caption = "";
    Notes->Clear();
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SaveGameDB()
{
int count;
String dir, gamesfile;
TStringList *ptr;

  dir = ScanDir;
  if ( dir.SubString( dir.Length(), 1) == "\\" ) {
    gamesfile = dir + "games.db";
    dir = dir.SubString( 1, dir.Length()-1);
  } else
    gamesfile = dir + "\\" + "games.db";
  db_fp = fopen(gamesfile.c_str(), "wb");
  if ( db_fp ) {
    for ( count = 0; count < games->Count; count++ ) {
      ptr = (TStringList *)games->Items[count];
      fprintf( db_fp, "%s%c", ptr->Strings[0].c_str(),0);
      fprintf( db_fp, "%s%c", ptr->Strings[1].c_str(),0);
      fprintf( db_fp, "%s%c", ptr->Strings[2].c_str(),0);
      fprintf( db_fp, "%s%c", ptr->Strings[3].c_str(),0);
      fprintf( db_fp, "%s%c", ptr->Strings[4].c_str(),0);
      fprintf( db_fp, "%s%c", ptr->Strings[5].c_str(), 0);
      fprintf( db_fp, "%s%c", ptr->Strings[7].c_str(), 0);
      fprintf( db_fp, "%s%c", ptr->Strings[8].c_str(), 0);
    }
    fprintf(db_fp, "%c", 0);
    fclose(db_fp);
    db_fp = NULL;
  }
}
//---------------------------------------------------------------------------

TStringList * __fastcall TForm1::FindGame()
{
    int count;
    String currgame = CurrGamePath + Game;
    currgame = currgame.UpperCase();
    for ( count = 0; count < games->Count; count++ ) {
      if ( ((TStringList *)games->Items[count])->Strings[8].UpperCase() == currgame ) {
        return (TStringList *)games->Items[count];
      }
    }
    return NULL;
}
//---------------------------------------------------------------------------

TListItem * __fastcall TForm1::GameInView()
{
    int count;
    String currgame = CurrGamePath + Game;
    currgame = currgame.UpperCase();
    for ( count = 0; count < ListView1->Items->Count; count++ ) {
      if ( ListView1->Items->Item[count]->SubItems->Strings[7].UpperCase() == currgame ) {
        return ListView1->Items->Item[count];
      }
    }
    return NULL;
}

//---------------------------------------------------------------------------

void __fastcall TForm1::LoadGameDB()
{
String games, Name, Version, System, Path, Status;
int Plays, TotalIn, TotalOut;

  games = ScanDir;
  if ( games.SubString( games.Length(), 1) != '\\' )
    games = games + "\\";
  games = games + "games.db";
  if ( FileExists(games) ) {
    if ( !scanned ) {
      db_fp = fopen(games.c_str(),"rb");
      if ( db_fp ) {
        filters->Clear();
//        MainMenu2->Items->Items[2]->Clear();
        while ( 1 ) {
          Name = AnsiString(ReadString());
          if ( Name == "" )
            break;
          Version = AnsiString(ReadString());
          System = AnsiString(ReadString());
          Plays = StrToInt(AnsiString(ReadString()));
          TotalIn = StrToInt(AnsiString(ReadString()));
          TotalOut = StrToInt(AnsiString(ReadString()));
          Status = AnsiString(ReadString());
          Path = AnsiString(ReadString());
          AddToList(Name, Version, System, Status, Path, Plays, TotalIn, TotalOut);
        }
//        UpdateTotals();
      }
      fclose(db_fp);
      scanned = true;
      CreateFilterMenu();
      PopulateListView();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::UpdateGameData()
{
TListItem * ptr = GameInView();
TStringList * ptr2 = FindGame();
float Percentage;
char buffer[10];

  if ( ptr ) {
    ptr->SubItems->Strings[2] = IntToStr(TimesPlayed);
    ptr->SubItems->Strings[3] = IntToStr(TotalIn);
    ptr->SubItems->Strings[4] = IntToStr(TotalOut);
    if ( TotalIn )
      Percentage = ( (float)TotalOut / (float)TotalIn ) * 100;
    else
      Percentage = 0;
    sprintf( buffer, "%.2f", Percentage);
    ptr->SubItems->Strings[5] = AnsiString(buffer);
  }
  if ( ptr2 ) {
    ptr2->Strings[3] = IntToStr(TimesPlayed);
    ptr2->Strings[4] = IntToStr(TotalIn);
    ptr2->Strings[5] = IntToStr(TotalOut);
    ptr2->Strings[6] = AnsiString(buffer);
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ToggleMeterPanel()
{
  if ( MeterPanel->Visible ) {
    MeterPanel->Visible = false;
    if ( GeneralConfig->MeterPanelResize->Checked )
      Height -= MeterPanel->Height;
  } else if ( !Manager->Visible ) {
    MeterPanel->Visible = true;
    if ( GeneralConfig->MeterPanelResize->Checked )
      Height += MeterPanel->Height;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FilterItemClick(TObject *Sender)
{
TMenuItem *ptr = (TMenuItem *)Sender;

  if ( ptr->Checked ) {
    ptr->Checked = false;
    filters->Strings[ptr->Tag] = "-" + filters->Strings[ptr->Tag];
  } else {
    ptr->Checked = true;
    filters->Strings[ptr->Tag] = filters->Strings[ptr->Tag].SubString(2,filters->Strings[ptr->Tag].Length() - 1);
  }
  PopulateListView();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PopulateListView()
{
int count;
TStringList *ptr;
TListItem * ListItem;
String filename;

  Total_Games = 0;
  Total_In = 0;
  Total_Out = 0;

  ListView1->Items->BeginUpdate();
  ListView1->Items->Clear();

  for ( count = 0; count < games->Count; count++ ) {
    ptr = (TStringList *)games->Items[count];
    if ( filters->IndexOf(ptr->Strings[2]) != -1 ) {
      ListItem = ListView1->Items->Add();
      if ( GeneralConfig->FileNames->Checked ) {
        filename = ExtractFileName(ptr->Strings[8]);
        filename = filename.SubString(1, filename.Length() - 4);
        ListItem->Caption = filename;
      } else
        ListItem->Caption = ptr->Strings[0];
      ListItem->SubItems->Add(ptr->Strings[1]);
      ListItem->SubItems->Add(ptr->Strings[2]);
      ListItem->SubItems->Add(ptr->Strings[3]);
      ListItem->SubItems->Add(ptr->Strings[4]);
      ListItem->SubItems->Add(ptr->Strings[5]);
      ListItem->SubItems->Add(ptr->Strings[6]);
      ListItem->SubItems->Add(ptr->Strings[7]);
      ListItem->SubItems->Add(ptr->Strings[8]);
      Total_In += ptr->Strings[4].ToInt();
      Total_Out += ptr->Strings[5].ToInt();
      Total_Games++;
    }
  }
  
  ListView1->CustomSort(CustomSortProc, 0);
  sortstat[0] *= -1;
  ListView1->Items->EndUpdate();
  UpdateTotals();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CreateFilterMenu()
{
int count;

//  MainMenu2->Items->Items[2]->Clear();
  while ( MainMenu2->Items->Items[2]->Count > 3 )
    delete MainMenu2->Items->Items[2]->Items[3];
  filters->Sorted = false;
  for ( count = 0; count < filters->Count; count++ ) {
    TMenuItem *ptr = new TMenuItem(this);
    ptr->AutoHotkeys = maManual;
    ptr->Caption = filters->Strings[count];
    ptr->Checked = true;
    ptr->Tag = count;
    ptr->OnClick = FilterItemClick;
    MainMenu2->Items->Items[2]->Add(ptr);
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClearGameList()
{
int count;
TStringList *ptr;

  for ( count = 0; count < games->Count; count++ ) {
    ptr = (TStringList *)games->Items[count];
    delete ptr;
  }
  games->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ClearAllClick(TObject *Sender)
{
int tag;

  for ( int count = 3; count < MainMenu2->Items->Items[2]->Count; count++ ) {
    if ( MainMenu2->Items->Items[2]->Items[count]->Checked ) {
      MainMenu2->Items->Items[2]->Items[count]->Checked = false;
      tag = MainMenu2->Items->Items[2]->Items[count]->Tag;
      filters->Strings[tag] = "-" + filters->Strings[tag];
    }
  }
  PopulateListView();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SelectAllClick(TObject *Sender)
{
int tag;

  for ( int count = 3; count < MainMenu2->Items->Items[2]->Count; count++ ) {
    if ( !MainMenu2->Items->Items[2]->Items[count]->Checked ) {
      MainMenu2->Items->Items[2]->Items[count]->Checked = true;
      tag = MainMenu2->Items->Items[2]->Items[count]->Tag;
      filters->Strings[tag] = filters->Strings[tag].SubString(2,filters->Strings[tag].Length() - 1);
    }
  }
  PopulateListView();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImportOneClick(TObject *Sender)
{
String filename;
int which = ((TMenuItem *)Sender)->Tag;

  if ( menu_ptr->ClassNameIs("TLamp2") ) {
    filename = IntToStr(menu_ptr->Left) + "_" + IntToStr(menu_ptr->Top) + "_" +
               IntToStr(_LampFromTag(menu_ptr->Tag));
    switch ( which ) {
      case 1:
        filename = filename + "_on1";
        break;
      case 2:
        filename = filename + "_on2";
        break;
      case 0:
        filename = filename + "_off";
        break;
    }
    filename = filename + "_1.bmp";
    if ( FileExists(filename) ) {
      TLamp2 *ptr = (TLamp2 *)menu_ptr;
//      ptr->Tag |= _NoOutLineMask;
//      ptr->SurfaceType = stBrushImage;
//      ptr->Trans = true;
//      ptr->OutlineStyle = psClear;
      switch ( which ) {
        case 0:
          ptr->OffImage->LoadFromFile(filename);
          break;
        case 1:
          ptr->OnImage1->LoadFromFile(filename);
          break;
        case 2:
          ptr->OnImage2->LoadFromFile(filename);
          break;
      }
      if ( PropertiesForm->Visible && menu_ptr == PropertiesForm->sel_ptr )
        PropertiesForm->Update();
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ChangeToGraphic2Click(TObject *Sender)
{
struct Points *pptr;

  for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
    if ( pptr->comp->ClassNameIs("TLamp2") ) {
      LayoutChanged();
      ((TLamp2 *)pptr->comp)->SurfaceType = stBrushImage;
      ((TLamp2 *)pptr->comp)->Caption = "";
      ((TLamp2 *)pptr->comp)->Trans = true;
      ((TLamp2 *)pptr->comp)->Tag |= _NoOutLineMask;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ACELEDDisplayClick(TObject *Sender)
{
TACELED *new_ptr;

  if ( aceled == NULL ) {
    new_ptr = new TACELED(MainPanel);
    new_ptr->Tag = 0;
    new_ptr->Top = BiasY;
    new_ptr->Left = BiasX;
    new_ptr->OnMouseDown = TImageMouseDown;
    new_ptr->OnMouseMove = TImageMouseMove;
    new_ptr->OnMouseUp   = TImageMouseUp;
    new_ptr->Parent = MainPanel;
    LayoutChanged();
    aceled = new_ptr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BFMAlphaDisplayClick(TObject *Sender)
{
TBFMAlpha *new_ptr;
Graphics::TBitmap * ptr;

  if ( bfmalpha == NULL ) {
    ptr = new Graphics::TBitmap();
    ptr->LoadFromResourceName((int)HInstance,"BFMALPHA");
    new_ptr = new TBFMAlpha(MainPanel);
    new_ptr->Tag = 0;
    new_ptr->Top = BiasY;
    new_ptr->Left = BiasX;
    new_ptr->OnMouseDown = TImageMouseDown;
    new_ptr->OnMouseMove = TImageMouseMove;
    new_ptr->OnMouseUp   = TImageMouseUp;
    new_ptr->SetBitmap(ptr);
    new_ptr->SetReversed(true);
    new_ptr->Parent = MainPanel;
    LayoutChanged();
    bfmalpha = new_ptr;
    delete ptr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ProconnAlphaClick(TObject *Sender)
{
TPROCONLED *new_ptr;

  if ( proconnled == NULL ) {
    new_ptr = new TPROCONLED(MainPanel);
    new_ptr->Tag = 0;
    new_ptr->Top = BiasY;
    new_ptr->Left = BiasX;
    new_ptr->OnMouseDown = TImageMouseDown;
    new_ptr->OnMouseMove = TImageMouseMove;
    new_ptr->OnMouseUp   = TImageMouseUp;
    new_ptr->Parent = MainPanel;
    LayoutChanged();
    proconnled = new_ptr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SEC1Click(TObject *Sender)
{
  SECForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::MoveGroup(int Xshift, int Yshift)
{
struct Points *pptr;

  if ( group.ptr ) {
    MovingGroup = true;
    group.top += Yshift;
    group.left += Xshift;
    group.bottom += Yshift;
    group.right += Xshift;
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr )
      DrawFrame(pptr->TheRect);
    for ( pptr = group.ptr; pptr != NULL; pptr = pptr->next_ptr ) {
      pptr->TheRect.Top += Yshift;
      pptr->TheRect.Left += Xshift;
      pptr->TheRect.Bottom += Yshift;
      pptr->TheRect.Right += Xshift;
//      pptr->comp->SetBounds( pptr->TheRect.Left + 2,
//                             pptr->TheRect.Top + 2,
//                             pptr->comp->Width,
//                             pptr->comp->Height);
      DrawFrame(pptr->TheRect);
    }
   }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSPACEROMClick(TObject *Sender)
{
int count;

  if ( Fruit != NULL ) {
    Application->MessageBox("Cannot load ROM while game is running", "Error", MB_OK | MB_ICONEXCLAMATION);
    return;
  }

  OpenDialog1->Filter = "ACE SPACE ROM images (*.hex *.bin)|*.hex;*.bin| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("hex");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
      for ( count = 0; count < OpenDialog1->Files->Count; count++ )
        ROMList->Add(OpenDialog1->Files->Strings[count]);
      ROMList->Sort();
      ROMname = ExtractFileName(OpenDialog1->FileName);
      Arch = ACE_SPACE;
      Sound->SetSampleRate(64000);
      Fruit = new TSPACE(Game, CurrGamePath, ROMList);
      Fruit->OnTerminate = FruitTerminated;
  }

}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadPROCONNSoundROMClick(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "PROCONN Sound images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadJPMSound(12000);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LoadSPACESoundROMClick(TObject *Sender)
{
int count;

  OpenDialog1->Filter = "SPACE Sound images (*.hex *.bin *.p*)|*.hex;*.bin;*.p*| Any file (*.*) | *.*";
  OpenDialog1->DefaultExt = AnsiString("bin");
  OpenDialog1->Options << ofFileMustExist << ofAllowMultiSelect;
  OpenDialog1->FileName = "";

  if ( OpenDialog1->Execute() ) {
    if ( SoundList->Count ) {
      Sound->DeleteTunes();
      SoundList->Clear();
    }
    for ( count = 0; count < OpenDialog1->Files->Count; count++ )
      SoundList->Add(OpenDialog1->Files->Strings[count]);
    SoundList->Sort();
    if ( !NoSound ) {
      LoadSound();
    }
  }
}
//---------------------------------------------------------------------------

