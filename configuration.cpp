//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "system.h"
#include "configuration.h"
#include "shared.h"
#include "sample.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "zonzoff"
#pragma link "zPanel"
#pragma resource "*.dfm"

TConfig *Config;
extern TSystem *Fruit;

//---------------------------------------------------------------------------
__fastcall TConfig::TConfig(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TConfig::ReelsClick(TObject *Sender)
{
  Reel_Type = Reels->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::ProgramTypeClick(TObject *Sender)
{
  Program_Type = ProgramType->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::PayoutClick(TObject *Sender)
{
  Hopper_Type = Payout->ItemIndex;    
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::ExtenderClick(TObject *Sender)
{
  Extender_Type = Extender->ItemIndex;    
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::SegmentDisplayClick(TObject *Sender)
{
  LED_Type = SegmentDisplay->ItemIndex;
  if ( LED_Type > 1 && !loading )
    LED_Type += 2;
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::Dip1Click(TObject *Sender)
{
  if ( ((TzcLightOnOff *)Sender)->SwitchOn )
    DIP1 |= ((TzcLightOnOff *)Sender)->Tag;
  else
    DIP1 &= ~((TzcLightOnOff *)Sender)->Tag;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::Dip2Click(TObject *Sender)
{
  if ( ((TzcLightOnOff *)Sender)->SwitchOn )
    DIP2 |= ((TzcLightOnOff *)Sender)->Tag;
  else
    DIP2 &= ~((TzcLightOnOff *)Sender)->Tag;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::SetDip1(Byte value)
{
  if ( value & 128 )
    Dip10->SwitchOn = true;
  else
    Dip10->SwitchOn = false;

  if ( value & 64 )
    Dip11->SwitchOn = true;
  else
    Dip11->SwitchOn = false;

  if ( value & 32 )
    Dip12->SwitchOn = true;
  else
    Dip12->SwitchOn = false;

  if ( value & 16 )
    Dip13->SwitchOn = true;
  else
    Dip13->SwitchOn = false;

  if ( value & 8 )
    Dip14->SwitchOn = true;
  else
    Dip14->SwitchOn = false;

  if ( value & 4 )
    Dip15->SwitchOn = true;
  else
    Dip15->SwitchOn = false;

  if ( value & 2 )
    Dip16->SwitchOn = true;
  else
    Dip16->SwitchOn = false;

  if ( value & 1 )
    Dip17->SwitchOn = true;
  else
    Dip17->SwitchOn = false;

}
//---------------------------------------------------------------------------

void __fastcall TConfig::SetDip2(Byte value)
{
  if ( value & 128 )
    Dip20->SwitchOn = true;
  else
    Dip20->SwitchOn = false;

  if ( value & 64 )
    Dip21->SwitchOn = true;
  else
    Dip21->SwitchOn = false;

  if ( value & 32 )
    Dip22->SwitchOn = true;
  else
    Dip22->SwitchOn = false;

  if ( value & 16 )
    Dip23->SwitchOn = true;
  else
    Dip23->SwitchOn = false;

  if ( value & 8 )
    Dip24->SwitchOn = true;
  else
    Dip24->SwitchOn = false;

  if ( value & 4 )
    Dip25->SwitchOn = true;
  else
    Dip25->SwitchOn = false;

  if ( value & 2 )
    Dip26->SwitchOn = true;
  else
    Dip26->SwitchOn = false;

  if ( value & 1 )
    Dip27->SwitchOn = true;
  else
    Dip27->SwitchOn = false;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::SetDip3(Byte value)
{
  if ( value & 8 )
    Dip33->SwitchOn = true;
  else
    Dip33->SwitchOn = false;

  if ( value & 4 )
    Dip32->SwitchOn = true;
  else
    Dip32->SwitchOn = false;

  if ( value & 2 )
    Dip31->SwitchOn = true;
  else
    Dip31->SwitchOn = false;

  if ( value & 1 )
    Dip30->SwitchOn = true;
  else
    Dip30->SwitchOn = false;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::StakeKeyChange(TObject *Sender)
{
Byte Value;

  switch ( Stake = StakeKey->ItemIndex ) {
    case 0:
    case 1:
      Value = 0;
      break;
    case 2:
      Value = 1;
      break;
    case 3:
      Value = 2;
      break;
    case 4:
      Value = 3;
      break;
    case 5:
      Value = 4;
      break;
  }
  STkey = Value;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::JackpotKeyChange(TObject *Sender)
{
Byte Value;

  switch ( Jackpot = JackpotKey->ItemIndex ) {
    case 0:
      Value = 0;
      break;
    case 1:
      Value = 8;
      break;
    case 2:
      Value = 6;
      break;
    case 3:
      Value = 5;
      break;
    case 4:
      Value = 7;
      break;
    case 5:
      Value = 9;
      break;
    case 6:
      Value = 10;
      break;
  }
  JPkey = Value;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::PercentKeyChange(TObject *Sender)
{
  Percent = PercentKey->ItemIndex;
  PCkey = PercentKey->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::TDelayChange(TObject *Sender)
{
  Speed = StrToIntDef(TDelay->Text,0);
}
//---------------------------------------------------------------------------
void __fastcall TConfig::TotalInChange(TObject *Sender)
{
Byte item;
Byte type;
Byte mult;
Byte multiplier[5] = { 1, 2, 4, 5, 10};

  for ( item = 0; item < 10; item++ ) {
    TotalInMeterMult[item] = 0;
    TotalInTriacMult[item] = 0;
    TotalInSECMult[item] = 0;
  }

  if ( CashInSource->ItemIndex != -1 &&
       CashInMult->ItemIndex != -1 ) {
    item = CashInSource->ItemIndex % 10;
    type = CashInSource->ItemIndex / 10;
    mult = multiplier[CashInMult->ItemIndex];

    switch ( type ) {
      case 0:
        TotalInMeterMult[item] = mult;
        break;
      case 1:
        TotalInTriacMult[item] = mult;
        break;
      case 2:
        TotalInSECMult[item] = mult;
        break;
    }
  }

  if ( TokenInSource->ItemIndex != -1 &&
       TokenInMult->ItemIndex != -1 ) {

    item = TokenInSource->ItemIndex % 10;
    type = TokenInSource->ItemIndex / 10;
    mult = multiplier[TokenInMult->ItemIndex];

    switch ( type ) {
      case 0:
        TotalInMeterMult[item] = mult;
        break;
      case 1:
        TotalInTriacMult[item] = mult;
        break;
      case 2:
        TotalInSECMult[item] = mult;
        break;
    }
  }
  if ( CashInSource2->ItemIndex != -1 &&
       CashInMult2->ItemIndex != -1 ) {

    item = CashInSource2->ItemIndex % 10;
    type = CashInSource2->ItemIndex / 10;
    mult = multiplier[CashInMult2->ItemIndex];

    switch ( type ) {
      case 0:
        TotalInMeterMult[item] = mult;
        break;
      case 1:
        TotalInTriacMult[item] = mult;
        break;
      case 2:
        TotalInSECMult[item] = mult;
        break;
    }
  }
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::TotalOutChange(TObject *Sender)
{
Byte item;
Byte type;
Byte mult;
Byte multiplier[5] = { 1, 2, 4, 5, 10 };

  for ( item = 0; item < 10; item++ ) {
    TotalOutMeterMult[item] = 0;
    TotalOutTriacMult[item] = 0;
    TotalOutSECMult[item] = 0;
  }

  if ( CashOutSource->ItemIndex != -1 &&
       CashOutMult->ItemIndex != -1 ) {
    item = CashOutSource->ItemIndex % 10;
    type = CashOutSource->ItemIndex / 10;
    mult = multiplier[CashOutMult->ItemIndex];

    switch ( type ) {
      case 0:
        TotalOutMeterMult[item] = mult;
        break;
      case 1:
        TotalOutTriacMult[item] = mult;
        break;
      case 2:
        TotalOutSECMult[item] = mult;
        break;
    }
  }
  if ( TokenOutSource->ItemIndex != -1 &&
       TokenOutMult->ItemIndex != -1 ) {
    item = TokenOutSource->ItemIndex % 10;
    type = TokenOutSource->ItemIndex / 10;
    mult = multiplier[TokenOutMult->ItemIndex];

    switch ( type ) {
      case 0:
        TotalOutMeterMult[item] = mult;
        break;
      case 1:
        TotalOutTriacMult[item] = mult;
        break;
      case 2:
        TotalOutSECMult[item] = mult;
        break;
    }
  }
  if ( CashOutSource2->ItemIndex != -1 &&
       CashOutMult2->ItemIndex != -1 ) {
    item = CashOutSource2->ItemIndex % 10;
    type = CashOutSource2->ItemIndex / 10;
    mult = multiplier[CashOutMult2->ItemIndex];

    switch ( type ) {
      case 0:
        TotalOutMeterMult[item] = mult;
        break;
      case 1:
        TotalOutTriacMult[item] = mult;
        break;
      case 2:
        TotalOutSECMult[item] = mult;
        break;
    }
  }
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::PercentageChange(TObject *Sender)
{
  Percentage = StrToIntDef(TPercentage->Text,0);
  UpdateMeters();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::UpdateMeters(void)
{
int count;
int itemIn, itemOut;
int offset;
int mult;

  for ( count = 0, itemIn = 0, itemOut = 0; count < 10; count++ ) {
    mult = -1;
    if ( TotalInMeterMult[count] > 0 ) {
      offset = 0;
      mult = GetMultiplier( TotalInMeterMult[count]);
    } else if ( TotalInTriacMult[count] > 0 ) {
      offset = 10;
      mult = GetMultiplier( TotalInTriacMult[count]);
    } else if ( TotalInSECMult[count] > 0 ) {
      offset = 20;
      mult = GetMultiplier( TotalInSECMult[count]);
    }
    if ( mult >= 0 ) {
      switch ( itemIn) {
        case 0:
          CashInSource->ItemIndex = offset + count;
          CashInMult->ItemIndex = mult;
          itemIn++;
          break;
        case 1:
          TokenInSource->ItemIndex = offset + count;
          TokenInMult->ItemIndex = mult;
          itemIn++;
          break;
        case 2:
          CashInSource2->ItemIndex = offset + count;
          CashInMult2->ItemIndex = mult;
          itemIn++;
          break;
      }
    }
    mult = -1;
    if ( TotalOutMeterMult[count] > 0 ) {
      offset = 0;
      mult = GetMultiplier( TotalOutMeterMult[count]);
    } else if ( TotalOutTriacMult[count] > 0 ) {
      offset = 10;
      mult = GetMultiplier( TotalOutTriacMult[count]);
    } else if ( TotalOutSECMult[count] > 0 ) {
      offset = 20;
      mult = GetMultiplier( TotalOutSECMult[count]);
    }
    if ( mult >= 0 ) {
      switch ( itemOut ) {
        case 0:
          CashOutSource->ItemIndex = offset + count;
          CashOutMult->ItemIndex = mult;
          itemOut++;
          break;
        case 1:
          TokenOutSource->ItemIndex = offset + count;
          TokenOutMult->ItemIndex = mult;
          itemOut++;
          break;
        case 2:
          CashOutSource2->ItemIndex = offset + count;
          CashOutMult2->ItemIndex = mult;
          itemOut++;
          break;
      }
    }
  }
}
//---------------------------------------------------------------------------

int __fastcall TConfig::GetMultiplier(int mult)
{
int value;

  switch ( mult ) {
    case 1:
      value = 0;
      break;
    case 2:
      value = 1;
      break;
    case 4:
      value = 2;
      break;
    case 5:
      value = 3;
      break;
    case 10:
      value = 4;
      break;
    default:
      value = -1;
  }
  return value;
}
//---------------------------------------------------------------------------
void __fastcall TConfig::TGameNameChange(TObject *Sender)
{
  strcpy(GameName,TGameName->Text.c_str());
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::MeterDisplayClick(TObject *Sender)
{
  UpdateMeters();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::SndClick(TObject *Sender)
{
  LayoutChanged();
  Sound_Type = Snd->ItemIndex;
//  ChangeVolume();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::VolumeClick(TObject *Sender)
{
  Volume_Type = Volume->ItemIndex;
//  ChangeVolume();
}
//---------------------------------------------------------------------------


void __fastcall TConfig::TGameVersionKeyPress(TObject *Sender, char &Key)
{
  if ( Key == ' ' )
    Key = 0;
}
//---------------------------------------------------------------------------


void __fastcall TConfig::MeterChange(TObject *Sender)
{
int value;

    value = ((TEdit *)Sender)->Text.ToIntDef(0);
    if ( Sender == InMeter )
      TotalIn = value;
    else if ( Sender == OutMeter )
      TotalOut = value;
    else
      Percentage = value;

    UpdateMeters();

}
//---------------------------------------------------------------------------

void __fastcall TConfig::FormShow(TObject *Sender)
{
  InMeter->Text = IntToStr(TotalIn);
  OutMeter->Text = IntToStr(TotalOut);
}
//---------------------------------------------------------------------------


void __fastcall TConfig::TGameVersionChange(TObject *Sender)
{
  strcpy(GameVersion, TGameVersion->Text.c_str());
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::SenseChange(TObject *Sender)
{
int value;

  value = ((TEdit *)Sender)->Text.ToIntDef(-1);
  if ( value > 31 )
    value = -1;
  if ( value != -1 )
    value = value << 8;
  if ( Sender == TMeterSense )
    MeterSense = value;
  else
    TriacSense = value;

}
//---------------------------------------------------------------------------

void __fastcall TConfig::ChrAddressChange(TObject *Sender)
{
  CharAddr = StrToHex(ChrAddress->Text.c_str());
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::CharTypeClick(TObject *Sender)
{
  Char_Type = CharType->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::ResetClick(TObject *Sender)
{
  if ( Fruit != NULL )
    Fruit->state = MS_RESET;
}
//---------------------------------------------------------------------------

void __fastcall TConfig::RamclearClick(TObject *Sender)
{
  if ( Fruit ) {
    if ( Application->MessageBox("Are you sure ?",
                                 "Confirm Clear Ram",
                                 MB_OKCANCEL | MB_ICONQUESTION) == ID_OK ) {
      STotalIn = 0;
      STotalOut = 0;
      TotalIn = 0;
      TotalOut = 0;
      UpdateMeters();
      InMeter->Text = "0";
      OutMeter->Text = "0";
      if ( Fruit != NULL ) {
        Fruit->state = MS_EXECUTE_BP;
        Fruit->ClearRAM();
        Fruit->state = MS_RESET;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfig::TReelLampXChange(TObject *Sender)
{
  ReelLampX = TReelLampX->Text.ToIntDef(0);
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::TReelLampYChange(TObject *Sender)
{
  ReelLampY = TReelLampY->Text.ToIntDef(0);
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::MaskChange(TObject *Sender)
{
int value;

  value = StrToHex(((TEdit *)Sender)->Text.c_str());
  if ( Sender == TMeterMask )
    MeterMask = value;
  else
    TriacMask = value;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::LVDClick(TObject *Sender)
{
  LVD_Type = LVD->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------
void __fastcall TConfig::ResetSettings(void)
{
  TDelay->Text = "0";
  Delay2->Text = "0";
  StakeKey->ItemIndex = 0;
  JackpotKey->ItemIndex = 0;
  PercentKey->ItemIndex = 0;
  Volume->ItemIndex = 0;
  LVD->ItemIndex = 0;
  StakeKey->Enabled = false;
  JackpotKey->Enabled = false;
  PercentKey->Enabled = false;
  ShortName->Text = "";
  TGameName->Text = "";
  TGameVersion->Text = "";
  TPercentage->Text = "";
  CashInSource->ItemIndex = -1;
  CashInMult->ItemIndex = -1;
  TokenInSource->ItemIndex = -1;
  TokenInMult->ItemIndex = -1;
  CashOutSource->ItemIndex = -1;
  CashOutMult->ItemIndex = -1;
  TokenOutSource->ItemIndex = -1;
  TokenOutMult->ItemIndex = -1;
  Extender->ItemIndex = 0;
  Reels->ItemIndex = 0;
  SegmentDisplay->ItemIndex = 0;
  Payout->ItemIndex = 0;
  Volume->ItemIndex = 0;
  CharType->ItemIndex = 0;
  ProgramType->ItemIndex = 0;
  Snd->ItemIndex = 0;
  TotalInChange(NULL);
  TotalOutChange(NULL);
  ChrAddress->Text = "";
  TMeterSense->Text = "";
  TTriacSense->Text = "";
  TMeterMask->Text = "FF";
  TTriacMask->Text = "FF";
  TReelLampX->Text = "0";
  TReelLampY->Text = "0";
  SetDip1(0);
  SetDip2(0);
  TwoLines->Checked = false;
  Aux2inv->Checked = false;
  TDoorInvert->Checked = false;
  THopper1->Checked = false;
  THopper2->Checked = false;
  SampleRate->Text = "16000";
  Aux->ItemIndex = 0;
  Channels->ItemIndex = 0;
  MPU4Type->ItemIndex = 0;
  Door->Text = "";
  CashBox->Text = "";
  Refill->Text = "";
}
//---------------------------------------------------------------------------


void __fastcall TConfig::SelectChange(TObject *Sender)
{
int i;

  for ( i = 0; i < Select->ControlCount; i++ )
    Select->Controls[i]->Visible = false;

  switch ( Select->TabIndex ) {
    case 0: // MPU3
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      CharacterAddr->Left = 16;
      CharacterAddr->Top = 344;
      CharacterAddr->Visible = true;
      Meters->Left = 16;
      Meters->Top = 224;
      Meters->Visible = true;
      PayoutSense->Left = 320;
      PayoutSense->Top = 32;
      PayoutSense->Visible = true;
      Aux->Visible = true;
      break;
    case 1: // MPU4
      Reels->Visible = true;
      Snd->Visible = true;
      ProgramType->Visible = true;
      DIPSwitch1->Left = 264;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 264;
      DIPSwitch2->Visible = true;
      Volume->Visible = true;
      Payout->Visible = true;
      Extender->Visible = true;
      LVD->Visible = true;
      CharType->Visible = true;
      ReelLamps->Visible = true;
      StakePrize->Left = 264;
      StakePrize->Top = 224;
      StakePrize->Visible = true;
      Misc->Visible = true;
      SegmentDisplay->Visible = true;
      Freq->Left = 16;
      Freq->Top = 392;
      Freq->Visible = true;
      Meters->Left = 136;
      Meters->Top = 304;
      Meters->Visible = true;
      MPU4Type->Top = 304;
      MPU4Type->Left = 448;
      MPU4Type->Visible = true;
      break;
    case 2: // IMPACT
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      StakePrize->Left = 16;
      StakePrize->Top = 128;
      StakePrize->Visible = true;
      Meters->Left = 16;
      Meters->Top = 290;
      Meters->Visible = true;
      THoppers->Left = 16;
      THoppers->Top = 208;
      THoppers->Visible = true;
      CoinMech->Left = 112;
      CoinMech->Top = 208;
      CoinMech->Visible = true;
      TSECFitted->Left = 208;
      TSECFitted->Top = 208;
      TSECFitted->Visible = true;
      break;
    case 3: // M1A/B
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      StakePrize->Left = 16;
      StakePrize->Top = 224;
      StakePrize->Visible = true;
      Meters->Left = 16;
      Meters->Top = 304;
      Meters->Visible = true;
      CoinMech->Left = 200;
      CoinMech->Top = 224;
      CoinMech->Visible = true;
      Switches->Top = 32;
      Switches->Left = 326;
      Switches->Visible = true;
      Freq->Top = 32;
      Freq->Left = 428;
      Freq->Visible = true;
      THoppers->Left = 326;
      THoppers->Top = 160;
      THoppers->Visible = true;
      break;
    case 4: // MPS2
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      THoppers->Left = 16;
      THoppers->Top = 128;
      THoppers->Visible = true;
      RotarySwitch->Left = 16;
      RotarySwitch->Top = 208;
      RotarySwitch->Visible = true;
      Meters->Left = 16;
      Meters->Top = 270;
      Meters->Visible = true;
//      Switches->Top = 32;
//      Switches->Left = 326;
//      Switches->Visible = true;
      break;
    case 5: // SYS1
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      Meters->Left = 16;
      Meters->Top = 224;
      Meters->Visible = true;
      Switches->Top = 32;
      Switches->Left = 326;
      Switches->Visible = true;
      break;
    case 6: // SCORPION I
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      Meters->Left = 16;
      Meters->Top = 224;
      Meters->Visible = true;
      Switches->Top = 32;
      Switches->Left = 326;
      Switches->Visible = true;
      StakePrize->Left = 16;
      StakePrize->Top = 342;
      StakePrize->Visible = true;
      break;
    case 7: // SCORPION II
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      Meters->Left = 16;
      Meters->Top = 224;
      Meters->Visible = true;
      Switches->Top = 32;
      Switches->Left = 326;
      Switches->Visible = true;
      StakePrize->Left = 16;
      StakePrize->Top = 342;
      StakePrize->Visible = true;
      break;
    case 8: // sys80
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      DIPSwitch3->Left = 16;
      DIPSwitch3->Visible = true;
      Meters->Left = 16;
      Meters->Top = 320;
      Meters->Visible = true;
      RotarySwitch->Left = 204;
      RotarySwitch->Top = 224;
      RotarySwitch->Visible = true;
      IOMapping->Visible = True;
      IOMapping->Top = 32;
      IOMapping->Left = 326;
      Switches->Top = 226;
      Switches->Left = 326;
      Switches->Visible = true;
      break;
    case 9: // sys 5
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      Meters->Left = 16;
      Meters->Top = 224;
      Meters->Visible = true;
      RotarySwitch->Left = 16;
      RotarySwitch->Top = 344;
      RotarySwitch->Visible = true;
      sys5sound->Left = 104;
      sys5sound->Top = 344;
      sys5sound->Visible = true;
      Switches->Top = 32;
      Switches->Left = 326;
      Switches->Visible = true;
      break;
    case 10: // SPACE
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      DIPSwitch2->Left = 16;
      DIPSwitch2->Visible = true;
      Meters->Left = 16;
      Meters->Top = 224;
      Meters->Visible = true;
      break;
    case 11: // PROCONN
      DIPSwitch1->Left = 16;
      DIPSwitch1->Visible = true;
      Meters->Left = 16;
      Meters->Top = 128;
      Meters->Visible = true;
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfig::HopperClick(TObject *Sender)
{
Byte mask;

   LayoutChanged();
   if ( Sender == THopper1 )
     mask = 0x40;
   else
     mask = 0x10;
   if ( ((TCheckBox *)Sender)->Checked )
     Hoppers &= ~mask;
   else
     Hoppers |= mask;
}
//---------------------------------------------------------------------------

void __fastcall TConfig::CoinMechClick(TObject *Sender)
{
   LayoutChanged();
   MechType = CoinMech->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TConfig::TDoorInvertClick(TObject *Sender)
{
  DoorInvert = TDoorInvert->Checked ? 128 : 0;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::TwoLinesClick(TObject *Sender)
{
  ReelLamp2 = TwoLines->Checked ? 1 : 0;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::SampleRateChange(TObject *Sender)
{
   LayoutChanged();
   SampleFreq = SampleRate->Text.ToIntDef(16000);
}
//---------------------------------------------------------------------------

void __fastcall TConfig::SwitchChange(TObject *Sender)
{
int value = ((TEdit *)Sender)->Text.ToIntDef(255);

  if ( Sender == Door )
    DoorSwitch = value;
  else if ( Sender == CashBox )
    CashSwitch = value;
  else
    RefillSwitch = value;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::TSECFittedClick(TObject *Sender)
{
  SECFitted = TSECFitted->ItemIndex ? true : false;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::AuxClick(TObject *Sender)
{
  MPU3_AUX = Aux->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::Delay2Change(TObject *Sender)
{
  Speed2 = StrToIntDef(Delay2->Text,0);
}
//---------------------------------------------------------------------------

void __fastcall TConfig::ChannelsClick(TObject *Sender)
{
  Stereo = Channels->ItemIndex ? 0 : 1;
  Sound->SetStereo(Stereo);
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
  Rotary = UpDown1->Position;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::AuxinvClick(TObject *Sender)
{
  if ( Aux1inv->Checked )
    Auxinvert |= 2;
  else
    Auxinvert &= ~2;
  if ( Aux2inv->Checked )
    Auxinvert |= 1;
  else
    Auxinvert &= ~1;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::Dip3Click(TObject *Sender)
{
  if ( ((TzcLightOnOff *)Sender)->SwitchOn )
    DIP3 |= ((TzcLightOnOff *)Sender)->Tag;
  else
    DIP3 &= ~((TzcLightOnOff *)Sender)->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TConfig::PortChange(TObject *Sender)
{
  int port = Port->Text.ToIntDef(0);
  if ( port && port < 512) {
    Value->Text = IntToStr(sys80io[port].value);
    switch ( sys80io[port].type ) {
      case S80_UNUSED:
        unused->Checked = true;
        Value->Enabled = false;
        break;
      case S80_LAMP:
        lamp->Checked = true;
        Value->Enabled = false;
        break;
      case S80_METER:
        meter->Checked = true;
        Value->Enabled = true;
        break;
      case S80_TRIAC:
        triac->Checked = true;
        Value->Enabled = true;
        break;
      case S80_SEG:
        ledseg->Checked = true;
        Value->Enabled = true;
        break;
      case S80_BCD:
        ledbcd->Checked = true;
        Value->Enabled = true;
        break;
      case S80_SEG_DIGIT:
        ledsegdigit->Checked = true;
        Value->Enabled = true;
        break;
      case S80_BCD_DIGIT:
        ledbcddigit->Checked = true;
        Value->Enabled = true;
        break;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfig::TypeClick(TObject *Sender)
{
  int port = Port->Text.ToIntDef(0);
  if ( port && port < 512 ) {
    int value = Value->Text.ToIntDef(0);
    switch ( ((TRadioButton *)Sender)->Tag ) {
      case S80_UNUSED:
        sys80io[port].type = S80_UNUSED;
        Value->Enabled = false;
        break;
      case S80_LAMP:
        sys80io[port].type = S80_LAMP;
        Value->Enabled = false;
        break;
      case S80_METER:
        sys80io[port].type = S80_METER;
        sys80io[port].value = value;
        Value->Enabled = true;
        break;
      case S80_TRIAC:
        sys80io[port].type = S80_TRIAC;
        sys80io[port].value = value;
        Value->Enabled = true;
        break;
      case S80_SEG:
        sys80io[port].type = S80_SEG;
        sys80io[port].value = value;
        Value->Enabled = true;
        break;
      case S80_BCD:
        sys80io[port].type = S80_BCD;
        sys80io[port].value = value;
        Value->Enabled = true;
        break;
      case S80_SEG_DIGIT:
        sys80io[port].type = S80_SEG_DIGIT;
        sys80io[port].value = value;
        Value->Enabled = true;
        break;
      case S80_BCD_DIGIT:
        sys80io[port].type = S80_BCD_DIGIT;
        sys80io[port].value = value;
        Value->Enabled = true;
        break;
    }
    LayoutChanged();
  }
}
//---------------------------------------------------------------------------

void __fastcall TConfig::ValueChange(TObject *Sender)
{
  int port = Port->Text.ToIntDef(0);
  if ( port && port < 512) {
    sys80io[port].value = Value->Text.ToIntDef(0);
    LayoutChanged();
  }
}
//---------------------------------------------------------------------------


void __fastcall TConfig::ResetAllClick(TObject *Sender)
{
int count;

  for ( count = 0; count < 512; count++ ) {
    sys80io[count].type = S80_LAMP;
    sys80io[count].value = 0;
  }
  PortChange(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TConfig::sys5soundClick(TObject *Sender)
{
  SYS5_SND = sys5sound->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::MPU4TypeClick(TObject *Sender)
{
  MPU4_TYPE = MPU4Type->ItemIndex;
  LayoutChanged();
}
//---------------------------------------------------------------------------

void __fastcall TConfig::SetArch(Arch_Types type)
{
  switch ( type ) {
    default:
    case BARCREST_MPU3:
      Select->TabIndex = 0;
      break;
    case BARCREST_MPU4:
      Select->TabIndex = 1;
      break;
    case JPM_IMPACT:
      Select->TabIndex = 2;
      break;
    case MAYGAY_M1A:
      Select->TabIndex = 3;
      break;
    case JPM_MPS2:
      Select->TabIndex = 4;
      break;
    case ACE_SYSTEM1:
      Select->TabIndex = 5;
      break;
    case BFM_SCORPION1:
      Select->TabIndex = 6;
      break;
    case BFM_SCORPION2:
      Select->TabIndex = 7;
      break;
    case JPM_SYS80:
      Select->TabIndex = 8;
      break;
    case JPM_SYS5:
      Select->TabIndex = 9;
      break;
    case ACE_SPACE:
      Select->TabIndex = 10;
      break;
    case PROJECT_PROCONN:
      Select->TabIndex = 11;
      break;
  }
  SelectChange(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TConfig::ShortNameChange(TObject *Sender)
{
  strcpy(RealName,ShortName->Text.c_str());
  LayoutChanged();
}
//---------------------------------------------------------------------------

