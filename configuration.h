//---------------------------------------------------------------------------
#ifndef configurationH
#define configurationH
//---------------------------------------------------------------------------
#include "zonzoff.hpp"
#include "zPanel.hpp"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TConfig : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TGroupBox *GroupBox3;
    TEdit *TDelay;
    TGroupBox *GroupBox4;
    TEdit *TGameName;
    TEdit *TGameVersion;
    TGroupBox *GroupBox7;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TEdit *TPercentage;
    TEdit *InMeter;
    TEdit *OutMeter;
    TSpeedButton *Reset;
    TSpeedButton *Ramclear;
    TTabControl *Select;
    TGroupBox *DIPSwitch1;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label17;
    TLabel *Label18;
    TLabel *Label19;
    TLabel *Label20;
    TLabel *Label21;
    TLabel *Label22;
    TzcLightOnOff *Dip17;
    TzcLightOnOff *Dip16;
    TzcLightOnOff *Dip15;
    TzcLightOnOff *Dip14;
    TzcLightOnOff *Dip13;
    TzcLightOnOff *Dip12;
    TzcLightOnOff *Dip11;
    TzcLightOnOff *Dip10;
    TRadioGroup *ProgramType;
    TRadioGroup *Extender;
    TRadioGroup *Snd;
    TRadioGroup *Reels;
    TRadioGroup *Volume;
    TRadioGroup *Payout;
    TRadioGroup *CharType;
    TRadioGroup *LVD;
    TGroupBox *ReelLamps;
    TLabel *Label35;
    TLabel *Label36;
    TEdit *TReelLampX;
    TEdit *TReelLampY;
    TGroupBox *StakePrize;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label5;
    TComboBox *StakeKey;
    TComboBox *JackpotKey;
    TComboBox *PercentKey;
    TGroupBox *Misc;
    TCheckBox *Aux2inv;
    TGroupBox *Meters;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label6;
    TLabel *Label13;
    TLabel *Label7;
    TLabel *Label8;
    TComboBox *CashInSource;
    TComboBox *CashInMult;
    TComboBox *CashOutMult;
    TComboBox *CashOutSource;
    TComboBox *TokenInSource;
    TComboBox *TokenInMult;
    TComboBox *TokenOutMult;
    TComboBox *TokenOutSource;
    TComboBox *CashInSource2;
    TComboBox *CashInMult2;
    TComboBox *CashOutSource2;
    TComboBox *CashOutMult2;
    TRadioGroup *SegmentDisplay;
    TGroupBox *DIPSwitch2;
    TzcLightOnOff *Dip27;
    TzcLightOnOff *Dip26;
    TzcLightOnOff *Dip25;
    TzcLightOnOff *Dip24;
    TzcLightOnOff *Dip23;
    TzcLightOnOff *Dip22;
    TzcLightOnOff *Dip21;
    TzcLightOnOff *Dip20;
    TLabel *Label30;
    TLabel *Label29;
    TLabel *Label28;
    TLabel *Label27;
    TLabel *Label26;
    TLabel *Label25;
    TLabel *Label24;
    TLabel *Label23;
    TEdit *TMeterSense;
    TEdit *TTriacSense;
    TEdit *TMeterMask;
    TEdit *TTriacMask;
    TLabel *Label3;
    TLabel *Label4;
    TGroupBox *CharacterAddr;
    TEdit *ChrAddress;
    TGroupBox *PayoutSense;
    TGroupBox *THoppers;
    TCheckBox *THopper1;
    TCheckBox *THopper2;
    TRadioGroup *CoinMech;
    TCheckBox *TDoorInvert;
    TCheckBox *TwoLines;
    TRadioGroup *TSECFitted;
    TGroupBox *Freq;
    TEdit *SampleRate;
    TGroupBox *Switches;
    TEdit *Door;
    TEdit *CashBox;
    TLabel *Label16;
    TLabel *Label34;
    TRadioGroup *Aux;
    TEdit *Delay2;
    TRadioGroup *Channels;
    TGroupBox *RotarySwitch;
    TUpDown *UpDown1;
    TEdit *RotarySW;
    TCheckBox *Aux1inv;
    TGroupBox *DIPSwitch3;
    TLabel *Label41;
    TLabel *Label42;
    TLabel *Label43;
    TLabel *Label44;
    TzcLightOnOff *Dip33;
    TzcLightOnOff *Dip32;
    TzcLightOnOff *Dip31;
    TzcLightOnOff *Dip30;
    TGroupBox *IOMapping;
    TEdit *Port;
    TUpDown *UpDown2;
    TLabel *Label37;
    TRadioButton *lamp;
    TRadioButton *meter;
    TRadioButton *triac;
    TRadioButton *ledseg;
    TRadioButton *ledsegdigit;
    TEdit *Value;
    TRadioButton *unused;
    TLabel *Label38;
    TRadioButton *ledbcd;
    TRadioButton *ledbcddigit;
    TSpeedButton *ResetAll;
    TLabel *Label39;
    TRadioGroup *sys5sound;
    TRadioGroup *MPU4Type;
    TEdit *Refill;
    TLabel *Label40;
    TEdit *ShortName;
    TLabel *Label45;
    TLabel *Label46;
    TLabel *Label47;
    TLabel *Label48;
    TLabel *Label49;
    void __fastcall ReelsClick(TObject *Sender);
    void __fastcall ProgramTypeClick(TObject *Sender);
    void __fastcall PayoutClick(TObject *Sender);
    void __fastcall ExtenderClick(TObject *Sender);
    void __fastcall SegmentDisplayClick(TObject *Sender);
    void __fastcall Dip1Click(TObject *Sender);
    void __fastcall Dip2Click(TObject *Sender);
    void __fastcall StakeKeyChange(TObject *Sender);
    void __fastcall JackpotKeyChange(TObject *Sender);
    void __fastcall PercentKeyChange(TObject *Sender);
    void __fastcall TDelayChange(TObject *Sender);

    void __fastcall TotalInChange(TObject *Sender);

    void __fastcall TotalOutChange(TObject *Sender);
    void __fastcall PercentageChange(TObject *Sender);
    void __fastcall TGameNameChange(TObject *Sender);
    void __fastcall MeterDisplayClick(TObject *Sender);

    void __fastcall SndClick(TObject *Sender);


    void __fastcall VolumeClick(TObject *Sender);

    void __fastcall TGameVersionKeyPress(TObject *Sender, char &Key);

    void __fastcall MeterChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);

    void __fastcall TGameVersionChange(TObject *Sender);
    void __fastcall SenseChange(TObject *Sender);

    void __fastcall ChrAddressChange(TObject *Sender);
    void __fastcall CharTypeClick(TObject *Sender);
    void __fastcall ResetClick(TObject *Sender);
    void __fastcall RamclearClick(TObject *Sender);
    void __fastcall TReelLampXChange(TObject *Sender);
    void __fastcall TReelLampYChange(TObject *Sender);
    void __fastcall MaskChange(TObject *Sender);
    void __fastcall LVDClick(TObject *Sender);
    void __fastcall SelectChange(TObject *Sender);
    void __fastcall HopperClick(TObject *Sender);
    void __fastcall CoinMechClick(TObject *Sender);
    void __fastcall TDoorInvertClick(TObject *Sender);
    void __fastcall TwoLinesClick(TObject *Sender);
    void __fastcall TSECFittedClick(TObject *Sender);
    void __fastcall SampleRateChange(TObject *Sender);
    void __fastcall SwitchChange(TObject *Sender);
    void __fastcall AuxClick(TObject *Sender);
    void __fastcall Delay2Change(TObject *Sender);
    void __fastcall ChannelsClick(TObject *Sender);
    void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
    void __fastcall AuxinvClick(TObject *Sender);
    void __fastcall Dip3Click(TObject *Sender);
    void __fastcall PortChange(TObject *Sender);
    void __fastcall TypeClick(TObject *Sender);
    void __fastcall ValueChange(TObject *Sender);
    void __fastcall ResetAllClick(TObject *Sender);
    void __fastcall sys5soundClick(TObject *Sender);
    void __fastcall MPU4TypeClick(TObject *Sender);
    void __fastcall ShortNameChange(TObject *Sender);
private:	// User declarations
    int __fastcall GetMultiplier(int mult);
public:		// User declarations
    void __fastcall SetDip1(Byte value);
    void __fastcall SetDip2(Byte value);
    void __fastcall SetDip3(Byte value);
    void __fastcall UpdateMeters(void);
    void __fastcall ResetSettings(void);
    void __fastcall SetArch(Arch_Types type);
    __fastcall TConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfig *Config;
//---------------------------------------------------------------------------
#endif
