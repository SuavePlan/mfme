//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "iomonitor.h"
#include "shared.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TIOMonitor *IOMonitor;
//---------------------------------------------------------------------------
__fastcall TIOMonitor::TIOMonitor(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TIOMonitor::UpdateTriacs(Word value)
{
TShape *Triacs[] = {
  IOMonitor->Triac1, IOMonitor->Triac2, IOMonitor->Triac3, IOMonitor->Triac4,
  IOMonitor->Triac5, IOMonitor->Triac6, IOMonitor->Triac7, IOMonitor->Triac8,
  IOMonitor->Triac9, IOMonitor->Triac10
};
int mask, count;

  for ( count = 0, mask = 1; count < 10; count++ ) {
    if ( value & mask )
      Triacs[count]->Brush->Color = clRed;
    else
      Triacs[count]->Brush->Color = clBtnFace;
    mask = mask * 2;
  }
}
//---------------------------------------------------------------------------

void __fastcall TIOMonitor::UpdateMeters(Byte value)
{
TShape *Meters[] = {
  IOMonitor->Meter1, IOMonitor->Meter2, IOMonitor->Meter3, IOMonitor->Meter4,
  IOMonitor->Meter5, IOMonitor->Meter6, IOMonitor->Meter7, IOMonitor->Meter8
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask )
      Meters[count]->Brush->Color = clRed;
    else
      Meters[count]->Brush->Color = clBtnFace;
    mask = mask * 2;
  }
}
//---------------------------------------------------------------------------

void __fastcall TIOMonitor::UpdateReel1(Byte value, int pos1, int pos2)
{
TShape *Reels[] = {
  IOMonitor->Reel11, IOMonitor->Reel12, IOMonitor->Reel13, IOMonitor->Reel14,
  IOMonitor->Reel15, IOMonitor->Reel16, IOMonitor->Reel17, IOMonitor->Reel18
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask )
      Reels[count]->Brush->Color = clRed;
    else
      Reels[count]->Brush->Color = clBtnFace;
    mask = mask * 2;
  }
  Pos3->Caption = IntToStr(pos1);
  Pos4->Caption = IntToStr(pos2);
}
//---------------------------------------------------------------------------

void __fastcall TIOMonitor::UpdateReel2(Byte value, int pos1, int pos2)
{
TShape *Reels[] = {
  IOMonitor->Reel21, IOMonitor->Reel22, IOMonitor->Reel23, IOMonitor->Reel24,
  IOMonitor->Reel25, IOMonitor->Reel26, IOMonitor->Reel27, IOMonitor->Reel28
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask )
      Reels[count]->Brush->Color = clRed;
    else
      Reels[count]->Brush->Color = clBtnFace;
    mask = mask * 2;
  }
  Pos1->Caption = IntToStr(pos1);
  Pos2->Caption = IntToStr(pos2);
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::UpdateReel3(Byte value, int pos1, int pos2)
{
TShape *Reels[] = {
  IOMonitor->Reel31, IOMonitor->Reel32, IOMonitor->Reel33, IOMonitor->Reel34,
  IOMonitor->Reel35, IOMonitor->Reel36, IOMonitor->Reel37, IOMonitor->Reel38
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask )
      Reels[count]->Brush->Color = clRed;
    else
      Reels[count]->Brush->Color = clBtnFace;
    mask = mask * 2;
  }
  Pos5->Caption = IntToStr(pos1);
  Pos6->Caption = IntToStr(pos2);
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::UpdateLampSelects(Word value)
{
TShape *Lamps[] = {
  IOMonitor->LampSel1, IOMonitor->LampSel2, IOMonitor->LampSel3, IOMonitor->LampSel4,
  IOMonitor->LampSel5, IOMonitor->LampSel6, IOMonitor->LampSel7, IOMonitor->LampSel8,
  IOMonitor->LampSel9, IOMonitor->LampSel10, IOMonitor->LampSel11, IOMonitor->LampSel12,
  IOMonitor->LampSel13, IOMonitor->LampSel14, IOMonitor->LampSel15, IOMonitor->LampSel16
};
TShape *Switches[] = {
  IOMonitor->Switch11, IOMonitor->Switch12, IOMonitor->Switch13, IOMonitor->Switch14,
  IOMonitor->Switch15, IOMonitor->Switch16, IOMonitor->Switch17, IOMonitor->Switch18,
  IOMonitor->Switch19, IOMonitor->Switch110, IOMonitor->Switch111, IOMonitor->Switch112,
  IOMonitor->Switch113, IOMonitor->Switch114, IOMonitor->Switch115, IOMonitor->Switch116,
  IOMonitor->Switch21, IOMonitor->Switch22, IOMonitor->Switch23, IOMonitor->Switch24,
  IOMonitor->Switch25, IOMonitor->Switch26, IOMonitor->Switch27, IOMonitor->Switch28,
  IOMonitor->Switch29, IOMonitor->Switch210, IOMonitor->Switch211, IOMonitor->Switch212,
  IOMonitor->Switch213, IOMonitor->Switch214, IOMonitor->Switch215, IOMonitor->Switch216,
};
int count;
int mask;

  for ( count = 0; count < 16; count++ ) {
    if ( count == value ) {
      Lamps[count]->Brush->Color = clRed;
//      Lamps[count+8]->Brush->Color = clRed;
    } else {
      Lamps[count]->Brush->Color = clBtnFace;
//      Lamps[count+8]->Brush->Color = clBtnFace;
    }
  }
  mask = 1;
  if ( value < 6 ) {
    if ( value > 3 )
      value -= 4;
    for ( count = 0; count < 8; count++ ) {
      if ( matrix[value] & mask )
        Switches[count+value*8]->Brush->Color = clRed;
      else
        Switches[count+value*8]->Brush->Color = clBtnFace;
      mask = mask << 1;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TIOMonitor::UpdateLampDrives1(Word value)
{
TShape *Lamps[] = {
  IOMonitor->LampDrv1, IOMonitor->LampDrv2, IOMonitor->LampDrv3, IOMonitor->LampDrv4,
  IOMonitor->LampDrv5, IOMonitor->LampDrv6, IOMonitor->LampDrv7, IOMonitor->LampDrv8,
  IOMonitor->LampDrv9, IOMonitor->LampDrv10, IOMonitor->LampDrv11, IOMonitor->LampDrv12,
  IOMonitor->LampDrv13, IOMonitor->LampDrv14, IOMonitor->LampDrv15, IOMonitor->LampDrv16
};
int mask, count;

  for ( count = 0, mask = 1; count < 16; count++ ) {
    if ( value & mask ) {
      Lamps[count]->Brush->Color = clRed;
    } else {
      Lamps[count]->Brush->Color = clBtnFace;
    }
    mask <<= 1;
  }
}
//---------------------------------------------------------------------------

void __fastcall TIOMonitor::Update7segs(Byte value)
{
TShape *segs[] = {
  IOMonitor->seg1, IOMonitor->seg2, IOMonitor->seg3, IOMonitor->seg4,
  IOMonitor->seg5, IOMonitor->seg6, IOMonitor->seg7, IOMonitor->seg8
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask ) {
      segs[count]->Brush->Color = clRed;
    } else {
      segs[count]->Brush->Color = clBtnFace;
    }
    mask = mask * 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::FormClose(TObject *Sender, TCloseAction &Action)
{
  MonitorActive = false;
  Hide();
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::UpdateAux1(Byte value)
{
TShape *Lamps[] = {
  IOMonitor->Aux11, IOMonitor->Aux12, IOMonitor->Aux13, IOMonitor->Aux14,
  IOMonitor->Aux15, IOMonitor->Aux16, IOMonitor->Aux17, IOMonitor->Aux18
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask ) {
      Lamps[count]->Brush->Color = clRed;
    } else {
      Lamps[count]->Brush->Color = clBtnFace;
    }
    mask = mask * 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::UpdateAux2(Byte value)
{
TShape *Lamps[] = {
  IOMonitor->Aux21, IOMonitor->Aux22, IOMonitor->Aux23, IOMonitor->Aux24,
  IOMonitor->Aux25, IOMonitor->Aux26, IOMonitor->Aux27, IOMonitor->Aux28
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask ) {
      Lamps[count]->Brush->Color = clRed;
    } else {
      Lamps[count]->Brush->Color = clBtnFace;
    }
    mask = mask * 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::UpdateAux3(Byte value)
{
TShape *Lamps[] = {
  IOMonitor->Aux31, IOMonitor->Aux32, IOMonitor->Aux33, IOMonitor->Aux34,
  IOMonitor->Aux35, IOMonitor->Aux36, IOMonitor->Aux37, IOMonitor->Aux38
};
int mask, count;

  for ( count = 0, mask = 1; count < 8; count++ ) {
    if ( value & mask ) {
      Lamps[count]->Brush->Color = clRed;
    } else {
      Lamps[count]->Brush->Color = clBtnFace;
    }
    mask = mask * 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TIOMonitor::Reset(void)
{
int i;

  for ( i = 0; i < IOMonitor->ComponentCount; i++ )
    if ( IOMonitor->Components[i]->ClassNameIs("TShape") )
      ((TShape *)IOMonitor->Components[i])->Brush->Color = clBtnFace;
}
//---------------------------------------------------------------------------


void __fastcall TIOMonitor::SelectChange(TObject *Sender)
{
  switch ( Select->TabIndex ) {
    case 0: // MPU3
      Reel31->Visible = false;
      Reel32->Visible = false;
      Reel33->Visible = false;
      Reel34->Visible = false;
      Reel35->Visible = false;
      Reel36->Visible = false;
      Reel37->Visible = false;
      Reel38->Visible = false;
      Pos5->Visible = false;
      Pos6->Visible = false;
      Meter7->Visible = true;
      Meter8->Visible = true;
      Triac9->Visible = true;
      Triac10->Visible = true;
      break;
    case 1: // MPU4
      Reel31->Visible = false;
      Reel32->Visible = false;
      Reel33->Visible = false;
      Reel34->Visible = false;
      Reel35->Visible = false;
      Reel36->Visible = false;
      Reel37->Visible = false;
      Reel38->Visible = false;
      Pos5->Visible = false;
      Pos6->Visible = false;
      Meter7->Visible = true;
      Meter8->Visible = true;
      Triac9->Visible = false;
      Triac10->Visible = false;
      break;
    case 2: // IMPACT
      Reel31->Visible = true;
      Reel32->Visible = true;
      Reel33->Visible = true;
      Reel34->Visible = true;
      Reel35->Visible = true;
      Reel36->Visible = true;
      Reel37->Visible = true;
      Reel38->Visible = true;
      Pos5->Visible = true;
      Pos6->Visible = true;
      Meter7->Visible = false;
      Meter8->Visible = false;
      Triac9->Visible = false;
      Triac10->Visible = false;
      break;
  }
}
//---------------------------------------------------------------------------


void __fastcall TIOMonitor::FormShow(TObject *Sender)
{
  SelectChange(NULL);    
}
//---------------------------------------------------------------------------

