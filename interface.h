//---------------------------------------------------------------------------
#ifndef interfaceH
#define interfaceH
//---------------------------------------------------------------------------
#include "Lamps.h"
#include "Zseg.hpp"
#include "Seven.hpp"
#include "Std2.hpp"
#include "zAnimate.hpp"
#include "SlideBar.hpp"
#include "zBtn2.hpp"
#include "zLed.hpp"
#include "zEdit.hpp"
#include <Mask.hpp>
#include "Gpshape.hpp"
#include "Lamp2.h"
#include <ComCtrls.hpp>
#include "CanvasPanel.h"
#include "Reelunit.h"
#include "DiscReel.h"
#include "BandReel.h"
#include "Reelunit.h"
#include "FancyReel.h"
#include "Frame2.h"
#include "sample.h"
#include "matrixdisplay.h"
#include "alphadisplay.h"
#include "newalpha.h"
#include "acedisplay.h"
#include "procondisplay.h"
#include "video.h"
//#include "Ledgadgt.hpp"
#include "GR32_Image.hpp"
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Grids.hpp>

#define NBR_REELS 10
#define NBR_STOPS 25
#define NBR_LAMPS 320
#define NBR_BUTTONS 128
#define BUFFLEN 6000
#define SAMPLERATE 64000
#define UNLOCKEDSN 0x12345678

#define LENGTH(x) ( sizeof(x) / sizeof(x[0]) )

#undef int8
#undef int16
#undef int32
#undef uint
#undef uint8
#undef uint16
#undef uint

#define int8   signed char			/* ASG: changed from char to signed char */
#define uint8  unsigned char
#define int16  short
#define uint16 unsigned short
#define int32  long
#define uint   unsigned int

#include "system.h"

    enum Lamp_Types {
      LT_NONE,
      LT_LAMP,
      LT_BUTTON,
      LT_LED,
      LT_SEVENSEG,
      LT_LABEL,
      LT_REEL,
      LT_FANCYREEL,
      LT_DISCREEL,
      LT_BANDREEL
    };

    enum Reel_Types {
      RT_NONE,
      RT_REEL,
      RT_FANCY,
      RT_BAND,
      RT_DISC
    };

    struct lamp {
      TControl *ptr;
      TColor   Colour;
      int nbr;
      int mask;
      Lamp_Types type;
      struct lamp *next;
    };

    struct led_type {
      TSevenSegment *digit;
      TzLed *led[8];
    };

//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TBevel *Bevel2;
	TOpenDialog *OpenDialog1;
    TPanel *MeterPanel;
    TEdit *TotalInMeter;
    TEdit *TotalOutMeter;
    TEdit *DriftMeter;
    TEdit *PercentMeter;
	TLabel *Label35;
	TLabel *Label36;
	TLabel *Label37;
    TLabel *Meterlabel;
	TSaveDialog *SaveDialog1;
    TPopupMenu *PropertiesMenu;
	TMenuItem *Delete1;
    TMenuItem *N2;
    TMenuItem *Properties;
    TPopupMenu *NewMenu;
    TMenuItem *NewLampMatrixExt1;
    TMenuItem *NewLabel;
    TMenuItem *NewCheckBox;
    TMenuItem *NewDisplay;
    TMenuItem *NewLED;
    TMenuItem *NewLampMatrixStd;
    TMenuItem *NewButtonMatrix1;
    TMenuItem *Changetocheckbox;
    TOpenPictureDialog *OpenPictureDialog1;
    TPopupMenu *AlignmentMenu;
    TMenuItem *DeleteBlock;
    TColorDialog *ColorDialog1;
    TMenuItem *File;
    TMenuItem *LoadGame;
    TMenuItem *Exit;
    TMenuItem *Align1;
    TMenuItem *AlignRight1;
    TMenuItem *AlignTop1;
    TMenuItem *AlignBottom1;
    TMenuItem *AlignLeft1;
    TMenuItem *Copy2;
    TMenuItem *N1;
    TMenuItem *Text1;
    TMenuItem *Font1;
    TMenuItem *Size1;
    TMenuItem *Shape1;
    TMenuItem *Colour1;
    TMenuItem *Apply1;
    TMenuItem *Colour2;
    TMenuItem *N3;
    TMenuItem *About1;
    TMenuItem *N4;
    TMenuItem *Apply2;
    TMenuItem *N5;
    TMenuItem *NewLampMatrixExt2;
    TMenuItem *NewLamp21;
    TMenuItem *Bitmap;
    TCheckBox *BackDoor;
    TCheckBox *Refill;
    TSpeedButton *Test;
    TMenuItem *Configuration;
        TMenuItem *Monitor;
    TMenuItem *LED;
    TMenuItem *Swap;
    TMenuItem *Spacing1;
    TMenuItem *TopBottom;
    TMenuItem *RightLeft;
    TMenuItem *BottomTop;
    TMenuItem *LeftRight;
    TMenuItem *N6;
    TMenuItem *N7;
    TMenuItem *Spacing4;
    TMenuItem *Spacing3;
    TMenuItem *Spacing2;
    TTimer *Timer1;
    TMenuItem *GameManager1;
    TMenuItem *LEDLamp;
    TMenuItem *LampLED;
    TMenuItem *NewReel;
    TPanel *Manager;
    TPanel *Panel2;
    TMenuItem *All1;
    TMenuItem *Frame;
    TMenuItem *N9;
    TMenuItem *Properties1;
    TMenuItem *BitmapButton;
    TMenuItem *N11;
    TMenuItem *NewLayout1;
    TMenuItem *Debug1;
    TMenuItem *Samples1;
    TMenuItem *ButtonMatrixJPM1;
    TMenuItem *LampMatrixJPM1;
    TLabel *Label1;
    TEdit *VTPmeter;
    TMenuItem *Design;
    TMenuItem *EditMenu;
    TMenuItem *LampImages1;
    TMenuItem *ExportOnLampImages1;
    TMenuItem *ImportOnLampImages1;
    TMenuItem *ExtractOnLampImages1;
    TMenuItem *ExtractOffLampImages1;
    TMenuItem *MaskOffImages1;
    TMenuItem *LoadLayout;
    TMenuItem *SaveLayout;
    TMenuItem *SaveLockedLayout;
    TMenuItem *LoadMPU3ROM;
    TMenuItem *LoadMPU4ROM;
    TMenuItem *LoadIMPACTROM;
    TMenuItem *LoadCharacter;
    TMenuItem *EditCharacter;
    TMenuItem *SaveCharacter;
    TMenuItem *LoadMPU4SoundROM;
    TMenuItem *Character1;
    TMenuItem *Sound1;
    TMenuItem *ReelEditor;
    TMenuItem *ROM1;
    TMenuItem *LayoutGame1;
    TMenuItem *SnapToGrid;
    TMenuItem *LoadIMPACTSoundROM;
    TMenuItem *SaveGame;
    TShape *Status;
    TCheckBox *CashDoor;
    TMenuItem *SendToback;
    TMenuItem *DeleteCharacter;
    TMenuItem *SetBackground;
    TMenuItem *N8;
    TMenuItem *H2;
    TMenuItem *H1;
    TMenuItem *H3;
    TMenuItem *H4;
    TMenuItem *H5;
    TMenuItem *H6;
    TMenuItem *H7;
    TMenuItem *H8;
    TMenuItem *H9;
    TMenuItem *H10;
//    TMenuItem *DiscReel1;
    TMenuItem *LoadM1AROM;
    TMenuItem *ChangeToGraphic1;
    TMenuItem *LoadM1ASoundROM1;
    TMenuItem *LampMatrixM1A1;
    TMenuItem *LoadMPS2ROM;
    TMenuItem *FancyReel;
  TMenuItem *LoadSYS1ROM1;
    TMenuItem *DiscReel;
    TMenuItem *LoadSCORPION2ROM;
    TMenuItem *LoadSCORPION2SoundROM;
    TMenuItem *LoadBFMDotmatrixROM1;
    TMenuItem *DotMatrixDisplay;
    TMenuItem *MainBoard1;
    TMenuItem *DotMatrixBoard1;
    TMenuItem *N12;
    TMenuItem *LoadSYSTEM80ROM1;
        TMenuItem *BandReel1;
    TMenuItem *ButtonMatrixBFM;
    TMenuItem *LoadSYS5ROM1;
    TMenuItem *LoadMPU4VideoROM;
    TMenuItem *VideoScreen;
    TMenuItem *VideoBoard1;
    TMenuItem *LoadSCORPION1ROM;
    TMenuItem *LoadSCORPION1SoundROM1;
    TMenuItem *LoadSYS5SoundROM;
    TMenuItem *Notes1;
    TTimer *Timer2;
    TMenuItem *N10;
    TMenuItem *Preferences1;
    TMainMenu *MainMenu2;
    TMenuItem *LaunchGame1;
    TMenuItem *AddNotes1;
    TMenuItem *ExitManager;
    TMenuItem *Directory1;
    TMenuItem *Scan1;
    TPanel *Panel1;
    TPanel *Panel3;
    TListView *ListView1;
    TMemo *Notes;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *Manufacturer;
    TLabel *ReleaseDate;
    TLabel *MachineType;
    TLabel *Technology;
    TLabel *Stakes;
    TLabel *Prizes;
    TMenuItem *Tools1;
    TMenuItem *SetVTP1;
    TMenuItem *BackupRAM1;
    TMenuItem *ResetFileHistory1;
    TMenuItem *Filter;
    TMenuItem *ClearAll;
    TMenuItem *SelectAll;
    TMenuItem *N13;
    TLabel *Label14;
    TLabel *Name;
    TPanel *Panel4;
    TLabel *Label9;
    TLabel *NbrGames;
    TLabel *Label10;
    TLabel *MTotalIn;
    TLabel *Label11;
    TLabel *MTotalOut;
    TLabel *Label12;
    TLabel *MPAndL;
    TLabel *Label13;
    TLabel *MTotPercentage;
    TMenuItem *Extract1;
    TMenuItem *Import1;
    TMenuItem *ExtractOn1Bitmap1;
    TMenuItem *ExtractOn2Bitmap2;
    TMenuItem *ExtractOffBitmap1;
    TMenuItem *ImportOn1Bitmap1;
    TMenuItem *ImportOn2Bitmap1;
    TMenuItem *ImportOffBitmap1;
    TMenuItem *N14;
    TMenuItem *ChangeToGraphic2;
    TMenuItem *Spacing11;
    TMenuItem *ExtractOn2LampImages1;
    TMenuItem *Spacing01;
    TMenuItem *ButtonLamp;
    TMenuItem *SEC1;
    TMenuItem *ExportOffLampImages1;
    TMenuItem *Lamps1;
    TMenuItem *Buttons1;
    TMenuItem *Displays1;
    TMenuItem *Reels1;
    TMenuItem *LEDs1;
    TMenuItem *Misc1;
    TMenuItem *AceDisplay1;
    TMenuItem *ProconnDisplay1;
    TMenuItem *BFMAlphaDisplay1;
    TMenuItem *LoadProconnROM;
    TMenuItem *LoadSPACEROM;
    TMenuItem *LoadPROCONNSoundROM;
    TMenuItem *LoadSPACESoundROM;
	void __fastcall CheckBoxClick(TObject *Sender);
	void __fastcall ShapeMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall ShapeMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y);
	void __fastcall ShapeMouseMove(TObject *Sender,	TShiftState Shift, int X, int Y);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall Delete1Click(TObject *Sender);
    void __fastcall EditModeClick(TObject *Sender);
    void __fastcall NewLampMatrixExt1Click(TObject *Sender);
    void __fastcall NewLabelClick(TObject *Sender);
    void __fastcall NewCheckBoxClick(TObject *Sender);
    void __fastcall NewDisplayClick(TObject *Sender);
    void __fastcall MainPanelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall MainPanelMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall PropertiesClick(TObject *Sender);
    void __fastcall NewLEDClick(TObject *Sender);
    void __fastcall NewLampMatrixStdClick(TObject *Sender);
    void __fastcall NewButtonMatrix1Click(TObject *Sender);
    void __fastcall DebugClick(TObject *Sender);
    void __fastcall ReelEditorClick(TObject *Sender);
    void __fastcall LampToButton(TObject *Sender, TObject *Source, int X,
          int Y);
    void __fastcall CheckLampToButton(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
    void __fastcall LampEndDrag(TObject *Sender, TObject *Target, int X,
          int Y);
    void __fastcall MainPanelDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
    void __fastcall AlignLClick(TObject *Sender);
    void __fastcall AlignRClick(TObject *Sender);
    void __fastcall AlignTClick(TObject *Sender);
    void __fastcall AlignBClick(TObject *Sender);
    void __fastcall DeleteBlockClick(TObject *Sender);
    void __fastcall MainPanelMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall AlColorClick(TObject *Sender);
    void __fastcall ResizeLeftClick(TObject *Sender);
    void __fastcall ResizeRightClick(TObject *Sender);
    void __fastcall LoadCharacteriserClick(TObject *Sender);
    void __fastcall UnusedLampsClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall NewLayoutClick(TObject *Sender);
    void __fastcall LoadMPU4ROMClick(TObject *Sender);
    void __fastcall MainPanelDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
    void __fastcall LoadLayoutClick(TObject *Sender);
    void __fastcall LoadGameClick(TObject *Sender);
    void __fastcall SaveGameClick(TObject *Sender);
    void __fastcall Text1Click(TObject *Sender);
    void __fastcall Apply1Click(TObject *Sender);
    void __fastcall Size1Click(TObject *Sender);
    void __fastcall Shape1Click(TObject *Sender);
    void __fastcall Font1Click(TObject *Sender);
    void __fastcall Colour1Click(TObject *Sender);
    void __fastcall About1Click(TObject *Sender);
    void __fastcall Apply2Click(TObject *Sender);
    void __fastcall HopperType(TObject *Sender);
    void __fastcall ExtenderType(TObject *Sender);
    void __fastcall LoadSoundClick(TObject *Sender);
    void __fastcall NewLampMatrixExt2Click(TObject *Sender);
    void __fastcall NewLamp21Click(TObject *Sender);
    void __fastcall NewBitmap(TObject *Sender);
    void __fastcall TestMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall TestMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall BackDoorClick(TObject *Sender);
    void __fastcall ConfigurationClick(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall LoadMPU3ROMClick(TObject *Sender);
    void __fastcall MonitorClick(TObject *Sender);
    void __fastcall RefillClick(TObject *Sender);
    void __fastcall LEDClick(TObject *Sender);
    void __fastcall SwapClick(TObject *Sender);
    void __fastcall TopBottomClick(TObject *Sender);
    void __fastcall RightLeftClick(TObject *Sender);
    void __fastcall SpacingClick(TObject *Sender);
    void __fastcall BottomTopClick(TObject *Sender);
    void __fastcall LeftRightClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall GameManager1Click(TObject *Sender);
    void __fastcall LEDLampClick(TObject *Sender);
    void __fastcall LampLEDClick(TObject *Sender);
    void __fastcall NewReelClick(TObject *Sender);
    void __fastcall ListView1DblClick(TObject *Sender);
    void __fastcall ListView1ColumnClick(TObject *Sender,
          TListColumn *Column);
    void __fastcall ListView1Click(TObject *Sender);
    void __fastcall RunGameClick(TObject *Sender);
    void __fastcall SetvtpClick(TObject *Sender);
    void __fastcall BackupRAMClick(TObject *Sender);
    void __fastcall BrowseClick(TObject *Sender);
    void __fastcall ScanClick(TObject *Sender);
    void __fastcall HideClick(TObject *Sender);
    void __fastcall All1Click(TObject *Sender);
    void __fastcall FrameClick(TObject *Sender);
    void __fastcall BitmapButtonClick(TObject *Sender);
    void __fastcall SaveDatLayout(TObject *Sender);
    void __fastcall EditCharacteriserClick(TObject *Sender);
    void __fastcall SaveCharacteriserClick(TObject *Sender);
    void __fastcall ExportLampsClick(TObject *Sender);
    void __fastcall ImportLampsClick(TObject *Sender);
    void __fastcall ExtractOneClick(TObject *Sender);
    void __fastcall Samples1Click(TObject *Sender);
    void __fastcall ButtonMatrixJPM1Click(TObject *Sender);
    void __fastcall LampMatrixJPM1Click(TObject *Sender);
    void __fastcall SnapToGridClick(TObject *Sender);
    void __fastcall LoadIMPACTROMClick(TObject *Sender);
    void __fastcall MaskOffImagesClick(TObject *Sender);
    void __fastcall ExtractOffImagesClick(TObject *Sender);
    void __fastcall LoadJPMSoundClick(TObject *Sender);
    void __fastcall ChangetocheckboxClick(TObject *Sender);
    void __fastcall CashDoorClick(TObject *Sender);
    void __fastcall SendTobackClick(TObject *Sender);
    void __fastcall DeleteCharacterClick(TObject *Sender);
    void __fastcall SetBackgroundClick(TObject *Sender);
    void __fastcall LoadHistory(TObject *Sender);
    void __fastcall ResetHistoryClick(TObject *Sender);
    void __fastcall DiscReelClick(TObject *Sender);
    void __fastcall LoadM1AROMClick(TObject *Sender);
    void __fastcall ChangeToGraphic1Click(TObject *Sender);
    void __fastcall LoadM1ASoundROM1Click(TObject *Sender);
    void __fastcall LampMatrixM1A1Click(TObject *Sender);
    void __fastcall LoadMPS2ROMClick(TObject *Sender);
    void __fastcall FancyReelClick(TObject *Sender);
    void __fastcall LoadSYS1ROM1Click(TObject *Sender);
    void __fastcall LoadSCORPION2ROMClick(TObject *Sender);
    void __fastcall LoadSCORPION2SoundROMClick(TObject *Sender);
    void __fastcall LoadBFMDotmatrixROM1Click(TObject *Sender);
    void __fastcall DotMatrixDisplayClick(TObject *Sender);
    void __fastcall DotMatrixBoard1Click(TObject *Sender);
    void __fastcall LoadSYSTEM80ROM1Click(TObject *Sender);
    void __fastcall BandReel1Click(TObject *Sender);
    void __fastcall ButtonMatrixBFMClick(TObject *Sender);
    void __fastcall LoadSYS5ROM1Click(TObject *Sender);
    void __fastcall LoadMPU4VideoROMClick(TObject *Sender);
    void __fastcall VideoScreenClick(TObject *Sender);
    void __fastcall VideoBoard1Click(TObject *Sender);
    void __fastcall LoadSCORPION1ROMClick(TObject *Sender);
    void __fastcall LoadSCORPION1SoundROM1Click(TObject *Sender);
    void __fastcall LoadSYS5SoundROMClick(TObject *Sender);
    void __fastcall Notes1Click(TObject *Sender);
    void __fastcall Timer2Timer(TObject *Sender);
    void __fastcall GeneralClick(TObject *Sender);
    void __fastcall AddNotes1Click(TObject *Sender);
    void __fastcall ClearAllClick(TObject *Sender);
    void __fastcall SelectAllClick(TObject *Sender);
    void __fastcall ImportOneClick(TObject *Sender);
    void __fastcall ChangeToGraphic2Click(TObject *Sender);
    void __fastcall LoadProconnROMClick(TObject *Sender);
    void __fastcall ACELEDDisplayClick(TObject *Sender);
    void __fastcall BFMAlphaDisplayClick(TObject *Sender);
    void __fastcall ProconnAlphaClick(TObject *Sender);
    void __fastcall SEC1Click(TObject *Sender);
    void __fastcall LoadSPACEROMClick(TObject *Sender);
    void __fastcall LoadPROCONNSoundROMClick(TObject *Sender);
    void __fastcall LoadSPACESoundROMClick(TObject *Sender);
private:	// User declarations
    void __fastcall DoNewLayout(void);
    bool __fastcall LoadResourceFile(String Filename);
    void __fastcall LoadAGame(void);
    void __fastcall DrawFrame(TRect TheRect);
    bool __fastcall IsInside(TRect A, TRect B);
    TLamp2 * __fastcall CreateNewLamp(int nbr, int hint, int left, int top, TColor Colour=clYellow);
    TControl * __fastcall CreateCheckBox(int nbr, int left, int top, String caption);
    void __fastcall AddToGroup(TObject *Sender);
    void __fastcall ReleaseGroup();
    bool __fastcall IsInGroup(TObject *Sender);
    void __fastcall RemoveFromGroup(TObject *Sender);
    void __fastcall CreateNewGraphic(void);
    void __fastcall CheckLampToGraphic(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept);
    void __fastcall GraphicDragDrop(TObject *Sender, TObject *Source,
      int X, int Y);
    TControl * __fastcall CreateBitmapButton(int nbr, int left, int top, String caption);
    TMenuItem * __fastcall AddItem(TMenuItem *ptr, String caption, TNotifyEvent handler, int tag=0);
    void __fastcall CreateLEDLamp(int count, int left, int top);
    void __fastcall LampHintToCaption(void);
    void __fastcall SetEditMode(bool state);
    void __fastcall LoadSound(void);
    void __fastcall DoApply(TControl *ptr, bool group);
    void __fastcall BinToStr(Byte value, Char *ptr);
    void __fastcall LampColour(int lamp, TObject *ptr, TColor Colour);
    void __fastcall reset(Byte Channel);
    void __fastcall SaveGameFile(String name);
    void __fastcall ExtractBitmapClick(TLamp2 *ptr, int which);
    TControl * __fastcall TForm1::ButtonCheckBox(TControl *ptr);
    enum Arch_Types SystemType(char *name);
    void __fastcall LoadJPMSound(int rate);
    struct Points {
      TRect TheRect;
      TControl *comp;
      bool Resizable;
      struct Points *next_ptr;
    };
    struct Group {
      int left;
      int right;
      int top;
      int bottom;
      int items;
      struct Points *ptr;
    };
public:		// User declarations
    TCanvasPanel *MainPanel;
    String PathName;
    String CurrGamePath;
    String ROMname;
    String DOTROMname;
    String VIDROMname;
    String RAMname;
    String ScanDir;
    String CharName;
    String Editing;
    String Muted;
    String ResName;
    String Game;
    bool Layout_Changed;
    bool Terminated;
    bool EditMode;
    bool LampMode;
    int BiasX;
    int BiasY;
    Boolean Move;
    Boolean left, right, top, bottom;
    TControl *menu_ptr, *source_ptr;
    TAlpha *display[3];
    TDotmatrix *dotmatrix;
    TACELED *aceled;
    TPROCONLED *proconnled;
    TBFMAlpha *bfmalpha;
    TVideo *screen;
    struct led_type led_display[40];
    TControl *Reels[NBR_REELS];
    enum Reel_Types ReelType[NBR_REELS];
    struct lamp *Lamps[NBR_LAMPS];
    TControl *Buttons[128];
    Byte ReelStops;
    bool FDrawing, FMove, FFirstLine, FResizing, MovingGroup;
    Byte FResize;
    TRect RBox;
    struct Group group;
    Byte State;
    TStringList *SoundList;
    TStringList *ROMList;
    TStringList *DOTROMList;
    TStringList *VIDROMList;
    int CopyType;
    int XGrid;
    int YGrid;
    unsigned long sn;
    int playing[2];
    bool longterm;
    bool profitloss;
    bool reelview;
    unsigned long authsn;
    int TimesPlayed;
    TComponent *shortcuts[256];
    Arch_Types Arch;
    unsigned long createdbysn;
    int Total_In;
    int Total_Out;
    int Total_Games;
    bool Scanned;
    int sortstat[8];
    unsigned long layoutsn;
    String ID;
    String User;
    String Organisation;
    int scanned;
    int done;
    Byte *save_ptr;
    TSearchRec sr;
    String history[10];
    int historynbr;
    TCanvas *MainCanvas;
    bool resized;
    FILE *db_fp;
    int reelpos;
    bool reeldrag;
    int reeldragX;
    String GameImage;
    int GameImageNumber;
    int savedwidth, savedheight;
    int spacing;
    bool MeterPanelVisible;
    TImage32* Thumbnail;
    TStringList *filters;
    TList *games;
    __fastcall TForm1(TComponent* Owner);
    String __fastcall IntToBin(unsigned char value);
    String __fastcall IntToBin(unsigned short value);
    void __fastcall play(int nbr, int Channel);
    void __fastcall play2(int nbr, int Channel);
    void __fastcall UpdateMeters(void);
    void __fastcall FruitTerminated(TObject *Sender);
    void __fastcall DisplayTerminated(TObject *Sender);
    void __fastcall VideoTerminated(TObject *Sender);
    void __fastcall InsertLamp(int nbr, TObject *ptr, TColor Colour, int type, int mask = 0);
    void __fastcall RemoveLamp(int lamp, TObject *ptr);
    void __fastcall LayoutChanged(void);
     int __fastcall ExitGameBox(bool Sure);
    void __fastcall UpdateSample(void);
    void __fastcall SetOutlines(bool value);
    void __fastcall DeleteItem(TObject *Item);
    void __fastcall SpaceAlign(bool type, bool positive);
    void __fastcall ClearLamp(int X);
    void __fastcall CalculateGroupSize(void);
    void __fastcall SetScreenSize(bool dir);
    void __fastcall Load(String FileName);
    void __fastcall HaltFruit(Byte state);
    void __fastcall SetVTP(void);
    void __fastcall MatrixChange2(int pos, bool state, bool coin = false);
    void __fastcall MatrixChange3(int pos, bool state);
    void __fastcall SaveLockedClick(TObject *Sender);
    bool __fastcall NotLocked(bool box = false);
    void __fastcall Search(String Path, int Attributes, int depth);
    void __fastcall CheckFile(String Path);
    void __fastcall EnableConfig(bool state);
    void __fastcall CompEnable(TWinControl *ptr, bool state);
    void __fastcall ReadHeader(String Filename, char *name, char *version, char *realname);
    void __fastcall DumpScreen(void);
    void __fastcall Shutdown(void);
    void __fastcall ChangeVolume(void);
    void __fastcall UpdateCaption(void);
    void __fastcall UpdateStatus(int state);
    void __fastcall DefaultShortcuts(void);
    void __fastcall UpdateHistory(bool updateregistry = false);
    void __fastcall SaveDat(bool prompt);
    void __fastcall TImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer);
    void __fastcall TImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y, TCustomLayer *Layer);
    void __fastcall TImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer);
    void __fastcall SendChar(Byte value);
    void __fastcall WriteCharToVideo(char ch);
    void __fastcall WriteCharToMPU4(char ch);
    void __fastcall VideoCharReceived();
    void __fastcall MPU4CharReceived();
    void __fastcall SetMPU4DCD(Byte value);
    void __fastcall SetMPU4CTS(Byte value);
    int __fastcall LatchState(void);
    void __fastcall ResetMatrix(void);
    void __fastcall OpenMouse();
    void __fastcall FreeDirectInput();
    void __fastcall UpdateMouse();
    void __fastcall UpdateMouse2();
    void __fastcall AddToList(String Name, String Version, String System, String Status, String Path, int Plays, int Totali, int Totalo);
    char * __fastcall ReadString(void);
    void __fastcall UpdateTotals(void);
    void __fastcall DoScan(void);
    void __fastcall SwitchImage();
    void __fastcall ReadDBFile(String Name);
    void __fastcall DisplayScreenShot(String Name);
    void __fastcall SaveGameDB();
    TStringList * __fastcall FindGame();
    TListItem * __fastcall GameInView();
    void __fastcall LoadGameDB();
    void __fastcall UpdateGameData();
    void __fastcall ToggleMeterPanel();
    void __fastcall FilterItemClick(TObject *Sender);
    void __fastcall FilterList(String Name);
    void __fastcall PopulateListView();
    void __fastcall CreateFilterMenu();
    void __fastcall ClearGameList();
    void __fastcall MatrixChange(TObject *Sender, bool state);
    void __fastcall MoveGroup(int XShift, int YShift);
};


#define _LampMask  0x20000
#define _LampMask2 0x10000
#define _ButtonMask 0x40000000
#define _CoinMask 0x80000000
#define _LEDLampMask 0x1000
#define _NoOutLineMask  0x40000
#define _OldCoinMask  0x40000
#define _LockMask  0x80000
#define _ToggleMask 0x20000
#define _HasLamp(x) ( (x) & _LampMask )
#define _HasLamp2(x) ( (x) & _LampMask2 )
#define _HasButton(x) ( (x) & _ButtonMask )
#define _HasCoin(x) ( (x) & _CoinMask )
#define _ReelWindow(x) (((x) & 0x38) >> 3)
#define _ReelStops(x) (((x) & 0xc0) >> 6)
#define _ReelFromTag(x)   (((x) & 7) | (((x) & 0x10000000) >> 25))
#define _ReelTag(x)   (((x) & 7) | (((x) & 0x8) << 25))
#define _ReelSym(x) (((x) & 0xff00) >> 8)
#define _LampTag(x)  (((x) & 0xff) | (((x) & 0x100) << 20))
#define _LampTag2(x)  ((((x) & 0xff) << 8) | (((x) & 0x100) << 21))
#define _LampFromTag(x) (((x) & 0xff) | (((x) & 0x10000000) >> 20))
#define _Lamp2FromTag(x) ((((x) & 0xff00) >> 8) | (((x) & 0x20000000) >> 21) )
#define _ButtonTag(x) ((x) << 8)
#define _ButtonFromTag(x) ((((x) & 0xff00) >> 8 ) & 0x7f )
#define _ShortcutFromTag(x) ( ((x) & 0x0ff00000) >> 20 )
#define _DigitMask(x) ( ((x) & 0xff0) >> 4 )
#define _SegmentMask(x) ( (x) & 0x0f)
#define _RampMask(x) ( ((x) & 0x1c0000) >> 18)
#define _FlagMask(x) ( ((x) & 0x200000) >> 21)

/*
     87654321876543218765432187654321
  Lamp Format:

                   2   2   1   1
     cBxx........lo21xxxxxyyyxxxxxyyy

  Button Format:

                     B     1   1
     c.xxbbbbbbbblc2.xxxxxyyyxxxxxyyy

  Reel Format:

     ...n......frrrTBssssssssSSwwwnnn

*/
//---------------------------------------------------------------------------
extern TForm1 *Form1;
//---------------------------------------------------------------------------
#endif



