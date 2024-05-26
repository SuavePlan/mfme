//---------------------------------------------------------------------------
#ifndef reeleditorH
#define reeleditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TEditor : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TLabel *Label21;
    TLabel *Label25;
    TLabel *Label26;
    TLabel *Label27;
    TLabel *Label28;
    TLabel *Label29;
    TLabel *Label30;
    TLabel *Label31;
    TLabel *Label32;
    TLabel *Label33;
    TLabel *Label34;
    TLabel *Label35;
    TLabel *Label36;
    TLabel *Label37;
    TLabel *Label38;
    TLabel *Label39;
    TLabel *Label40;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TMainMenu *MainMenu1;
    TOpenPictureDialog *OpenPictureDialog1;
    TMenuItem *Edit1;
    TSpeedButton *Apply;
    TSaveDialog *SaveDialog1;
    TMenuItem *Clear;
    TMenuItem *Default;
    TLabel *Label41;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label8;
    TLabel *Label13;
    TLabel *Label14;
    TLabel *Label15;
    TLabel *Label16;
    TLabel *Label22;
    TFontDialog *FontDialog1;
    TFontDialog *FontDialog2;
    TMenuItem *File1;
    TMenuItem *LoadImages1;
    TMenuItem *SaveImages1;
    TLabel *Label10;
    TMenuItem *LoadBand1;
    TLabel *Label11;
    TLabel *Label12;
    TLabel *Label17;
  TGroupBox *GroupBox1;
  TImage *BigReel;
  TSpeedButton *AddOverlay;
    TSpeedButton *LoadPreview;
  TGroupBox *GroupBox2;
  TImage *Overlay;
  TSpeedButton *LoadOverlay;
  TGroupBox *GroupBox3;
  TEdit *Thickness;
  TLabel *Label7;
  TEdit *Offset;
  TLabel *Label9;
  TSpeedButton *AddBox;
  TGroupBox *GroupBox4;
  TSpeedButton *AddNumber;
  TEdit *Symbol;
    TSpeedButton *SymbolFont;
  TGroupBox *GroupBox5;
    TSpeedButton *TxtFont;
  TSpeedButton *AddText;
  TEdit *Word;
  TPanel *SymFont;
  TPanel *TextFont;
  TColorDialog *ColorDialog1;
    TLabel *Label18;
    TMenuItem *SaveBands;
    TImage *Reel1;
    TImage *Reel2;
    TImage *Reel3;
    TImage *Reel4;
    TImage *Reel5;
    TImage *Reel6;
    TImage *Reel7;
    TImage *Reel8;
    TImage *Reel9;
    TImage *Reel10;
    TLabel *PreviewSize;
    TLabel *OverlaySize;
    void __fastcall ApplyClick(TObject *Sender);
    void __fastcall ImageDrop(TObject *Sender, TObject *Source,
          int X, int Y);
    void __fastcall ImageDragOver(TObject *Sender, TObject *Source,
          int X, int Y, TDragState State, bool &Accept);
    void __fastcall SaveImagesClick(TObject *Sender);
    void __fastcall LoadImagesClick(TObject *Sender);
    void __fastcall ClearClick(TObject *Sender);
    void __fastcall DefaultClick(TObject *Sender);
    void __fastcall TxtFontClick(TObject *Sender);
    void __fastcall AddNumberClick(TObject *Sender);
    void __fastcall CopyToBigReel(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall BigReelMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall AddTextClick(TObject *Sender);
    void __fastcall SymbolFontClick(TObject *Sender);
    void __fastcall DeleteRow(TObject *Sender);
    void __fastcall DeleteColumn(TObject *Sender);
    void __fastcall DoneClick(TObject *Sender);
    void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
    void __fastcall AddBoxClick(TObject *Sender);
    
    void __fastcall LoadOverlayClick(TObject *Sender);
    void __fastcall AddOverlayClick(TObject *Sender);
    void __fastcall LoadBand1Click(TObject *Sender);
    void __fastcall LoadPreviewClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FontBGClick(TObject *Sender);
    void __fastcall SaveBandsClick(TObject *Sender);
private:	// User declarations
    void __fastcall ClearImage(TImage *Sender, TColor colour);
    int drag_symbol;
    Graphics::TBitmap * __fastcall GetSymbol(int Y, TImage *source);
    void __fastcall SetSymbol(int symbol, TImage *dest, Graphics::TBitmap *src);
    TImage *Reels[10];
public:		// User declarations
    __fastcall TEditor(TComponent* Owner);
    bool HasImage;
};
//---------------------------------------------------------------------------
extern PACKAGE TEditor *Editor;
//---------------------------------------------------------------------------
#endif
