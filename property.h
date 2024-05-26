//---------------------------------------------------------------------------
#ifndef propertyH
#define propertyH
//---------------------------------------------------------------------------
#include "Gpshape.hpp"
#include "Lamp2.h"
#include "Seven.hpp"
#include "zLed.hpp"
#include "Std2.hpp"
#include "Alphadisplay.h"
#include "Reelunit.h"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TPropertiesForm : public TForm
{
__published:	// IDE-managed Components
    TButton *OK;
    TEdit *Px;
    TEdit *Py;
    TLabel *Label8;
    TEdit *PHeight;
    TEdit *PWidth;
    TLabel *Label9;
    TLabel *Label10;
    TLabel *Label11;
    TMemo *Caption;
    TFontDialog *FontDialog1;
    TSpeedButton *Font;
    TPageControl *PageControl;
    TTabSheet *Labels;
    TLabel *Label4;
    TEdit *LabelLamp;
    TShape *BackColour;
    TLabel *Label12;
    TTabSheet *CheckBoxes;
    TLabel *Label2;
    TEdit *CheckBox;
    TTabSheet *LEDs;
    TCheckBox *DPLeft;
    TOpenPictureDialog *OpenPictureDialog1;
    TTabSheet *GraphicLamps;
    TLabel *Label1;
    TLabel *Label30;
    TLamp2 *OnImage1;
    TLamp2 *OnImage2;
    TLabel *OnLabel2;
    TLabel *OnLabel1;
    TLabel *OffLabel;
    TLamp2 *OffImage;
    TComboBox *GShape;
    TTabSheet *Bitmap;
    TImage *BitmapImage;
    TTabSheet *LED;
    TEdit *LEDdigit;
    TLabel *Label44;
    TzLed *SegA;
    TzLed *SegG;
    TzLed *SegD;
    TzLed *SegB;
    TzLed *SegF;
    TzLed *SegC;
    TzLed *SegE;
    TzLed *SegH;
    TTabSheet *None;
    TEdit *Glamp1;
    TEdit *Glamp2;
    TSaveDialog *SaveDialog1;
    TCheckBox *NoOutLine;
    TCheckBox *BitmapAutoSize;
    TCheckBox *GLAutoSize;
    TTabSheet *Alpha;
    TCheckBox *Transparent;
    TTabSheet *LEDLamp;
    TEdit *LED1;
    TLabel *Label18;
    TzLed *LEDColour;
    TLabel *Label19;
    TCheckBox *TransBitmap;
    TTabSheet *Reel;
    TLabel *Label15;
    TEdit *Edit1;
    TCheckBox *CheckBox1;
    TLabel *Label24;
    TEdit *Height;
    TLabel *Label25;
    TLabel *Label28;
    TEdit *Edit3;
    TEdit *BorderSize;
    TLabel *Label29;
    TEdit *Edit5;
    TLabel *Label32;
    TEdit *Edit6;
    TTabSheet *Frame;
    TComboBox *FrameShape;
    TLabel *Label6;
    TCheckBox *FrameRaised;
    TEdit *Points;
    TLabel *Label14;
    TShape *GColour1;
    TShape *GColour2;
    TCheckBox *Graphic;
    TLabel *Label23;
    TEdit *Edit7;
    TTabSheet *Background;
    TShape *BackgroundColour;
    TLabel *Label26;
    TLamp2 *BackgroundImage;
    TLabel *Label27;
    TLabel *Label33;
    TEdit *Edit8;
    TLabel *Label34;
    TEdit *Edit9;
    TLabel *Label35;
    TShape *ReelBorderColour;
    TCheckBox *ReelLamps;
    TTabSheet *BitmapButton;
    TEdit *ButtonNumber;
    TLabel *Label36;
    TShape *ButtonOffColour;
    TShape *ButtonOnColour;
    TEdit *ButtonLamp1;
    TLabel *Label38;
    TCheckBox *CheckBox2;
    TLamp2 *ButtonOffBMP;
    TLamp2 *ButtonOn1BMP;
    TLabel *Label41;
    TCheckBox *CheckBox3;
    TzcShapeBitColBtn *TestButton;
    TLabel *Label42;
    TEdit *DigitNumber;
    TLabel *Label43;
    TImage *AlphaImage;
    TLabel *Label45;
    TCheckBox *OptoInv;
    TLabel *Label46;
    TShape *OnColour;
    TLabel *Label47;
    TEdit *ButtonLamp2;
    TLamp2 *ButtonOn2BMP;
    TLabel *Label37;
    TLabel *Label3;
    TEdit *Roundness;
    TEdit *Angle;
    TLabel *Label7;
    TCheckBox *Horizontal;
    TEdit *LEDThickness;
    TLabel *Label13;
    TEdit *HSpacing;
    TLabel *label22;
    TEdit *VSpacing;
    TLabel *Label20;
    TLabel *Label5;
    TEdit *Edit12;
    TSpeedButton *Clear;
    TCheckBox *Seg16;
    TEdit *Seg0;
    TEdit *Seg1;
    TEdit *Seg2;
    TEdit *Seg3;
    TEdit *Seg4;
    TEdit *Seg5;
    TEdit *Seg6;
    TEdit *Seg7;
    TEdit *Seg8;
    TEdit *Seg9;
    TEdit *Seg10;
    TEdit *Seg11;
    TEdit *Seg12;
    TEdit *Seg13;
    TEdit *Seg14;
    TEdit *Seg15;
    TCheckBox *Programmable;
    TEdit *Spacing;
    TLabel *Label16;
    TCheckBox *Smooth;
    TCheckBox *Reversed;
    TLabel *Label17;
    TComboBox *CoinNumber;
    TLabel *Label21;
    TEdit *AlphaNbr;
    TTabSheet *DiscReel;
    TImage *Image;
    TImage *Overlay;
    TEdit *OuterH;
    TEdit *OuterL;
    TEdit *InnerH;
    TEdit *InnerL;
    TEdit *LampSize;
    TEdit *Outline;
    TLabel *Label31;
    TEdit *Steps;
    TLabel *Label39;
    TImage *Mask;
    TEdit *Edit2;
    TUpDown *UpDown;
    TEdit *Lamp;
    TSpeedButton *SpeedButton1;
    TEdit *Nbr;
    TEdit *Offset;
    TLabel *Label49;
    TLabel *Label50;
    TLabel *Label51;
    TLabel *LLamp;
    TLabel *LSegnum;
    TEdit *Darkness;
    TLabel *LDarkness;
    TCheckBox *ButFrame;
    TShape *AlphaColour;
    TLabel *Label55;
    TCheckBox *Reverse;
    TCheckBox *TransLabel;
    TShape *GColourOff;
    TEdit *LButton;
    TEdit *Lamp1;
    TEdit *Lamp2;
    TEdit *Lamp3;
    TCheckBox *Custom;
    TLabel *LInner;
    TLabel *LOuter;
    TLabel *LLampSize;
    TLabel *LOutline;
    TLabel *LOffset;
    TLabel *Label40;
    TTabSheet *DotMatrix;
    TLabel *Label48;
    TEdit *DotMatrixSize;
    TEdit *DRStops;
    TEdit *DRSteps;
    TLabel *Label52;
    TLabel *Label53;
    TEdit *LampOffset;
    TLabel *Label54;
    TLabel *Label56;
    TEdit *DigitWidth;
        TTabSheet *BandReel;
        TLabel *Label65;
        TEdit *BRnbr;
        TLabel *Label66;
        TEdit *BRstops;
        TLabel *Label67;
        TEdit *BRoffset;
        TLabel *Label68;
        TEdit *BRbordersize;
        TLabel *Label69;
        TShape *BRbordercolour;
        TCheckBox *BRlamps;
        TCheckBox *BRreversespin;
        TEdit *BRmotorsteps;
        TLabel *Label70;
        TCheckBox *BRoptoinvert;
        TLabel *Label71;
        TEdit *BRlamp1;
        TEdit *BRlamp2;
        TEdit *BRlamp3;
        TLabel *Label72;
        TLabel *Label73;
        TLabel *Label74;
        TLabel *Label75;
        TEdit *BRlamp4;
        TLabel *Label76;
        TEdit *BRlamp5;
    TCheckBox *DInvert;
    TCheckBox *DPOn;
    TUpDown *HeightAdjust;
    TLabel *Label77;
    TComboBox *ComboBox1;
    TLabel *Label78;
    TEdit *Edit4;
    TCheckBox *Vertical;
    TImage *BRlampmask;
    TImage *LampMask;
    TLabel *Label79;
    TShape *OffColour;
    TLabel *Label80;
    TShape *BGColour;
    TUpDown *UpDown1;
    TUpDown *UpDown2;
    TUpDown *UpDown3;
    TUpDown *UpDown4;
    TTabSheet *Video;
    TOpenDialog *OpenDialog1;
    TEdit *LeftMouse;
    TLabel *Label81;
    TLabel *Label82;
    TLabel *Label57;
    TLabel *Label58;
    TLabel *Label59;
    TSpeedButton *Clear2;
    TCheckBox *AutoSize;
    TCheckBox *DReversed;
    TLabel *Label60;
    TImage *BROverlay;
    TLabel *Label61;
    TEdit *StepsPerStop;
    TCheckBox *Rotated;
    void __fastcall OKClick(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall LampColourMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    
    void __fastcall PHeightChange(TObject *Sender);
    void __fastcall PWidthChange(TObject *Sender);
    void __fastcall PxChange(TObject *Sender);
    void __fastcall PyChange(TObject *Sender);

    void __fastcall FontClick(TObject *Sender);

    
    void __fastcall ShapeChange(TObject *Sender);
    
    
    void __fastcall OffImageClick(TObject *Sender);
    
    void __fastcall SegClick(TObject *Sender);
    
    
    void __fastcall SegDblClick(TObject *Sender);
    
    
    void __fastcall OnImage1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall BitmapImageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall OnLabel1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall ButColourMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    
    void __fastcall TransparentClick(TObject *Sender);
    
    void __fastcall FrameRaisedClick(TObject *Sender);
    void __fastcall PointsChange(TObject *Sender);
    
    
    void __fastcall GraphicClick(TObject *Sender);
    void __fastcall BackgroundImageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    
    
    void __fastcall CheckBox3Click(TObject *Sender);
    
    void __fastcall testbuttonClick(TObject *Sender);
    void __fastcall TestButtonMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall LEDColourMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall AlphaImageMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall LampMaskClick(TObject *Sender);
    void __fastcall BitmapButtonMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall RoundnessChange(TObject *Sender);
    void __fastcall Edit12KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall Edit12KeyPress(TObject *Sender, char &Key);
    void __fastcall ClearClick(TObject *Sender);
    void __fastcall Seg16Click(TObject *Sender);
    void __fastcall NumberChange(TObject *Sender);
    void __fastcall ImageClick(TObject *Sender);
    void __fastcall OverlayClick(TObject *Sender);
    void __fastcall MaskClick(TObject *Sender);
    void __fastcall UpDownClick(TObject *Sender, TUDBtnType Button);
    void __fastcall LampChange(TObject *Sender);
    void __fastcall SpeedButton1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall SpeedButton1MouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall CustomClick(TObject *Sender);
        void __fastcall BRlampsClick(TObject *Sender);
    void __fastcall LampMaskMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall HeightAdjustClick(TObject *Sender, TUDBtnType Button);
    void __fastcall LButtonChange(TObject *Sender);
    void __fastcall OuterHClick(TObject *Sender, TUDBtnType Button);
    void __fastcall OuterLClick(TObject *Sender, TUDBtnType Button);
    void __fastcall InnerHClick(TObject *Sender, TUDBtnType Button);
    void __fastcall InnerLClick(TObject *Sender, TUDBtnType Button);
    void __fastcall Clear2Click(TObject *Sender);
    void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall OverlayMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall MaskMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
    TTabSheet *last_tab;
public:		// User declarations
    TControl *sel_ptr;
    void __fastcall Update(void);
    void __fastcall Updateposition(TRect pos);
    void __fastcall DrawAlpha(Graphics::TBitmap *ptr, int DigitWidth);
    __fastcall TPropertiesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPropertiesForm *PropertiesForm;
//---------------------------------------------------------------------------
#endif
