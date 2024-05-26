//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include "GR32_Image.hpp"
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include "Std2.hpp"
//---------------------------------------------------------------------------

class TFancyReel;
class TFancyLamp;
class TAlpha;
class TDiscReel;
class TBandReel;
class TSevenSeg;
class TBFMLED;
class TDotAlpha;
class TDotmatrix;
class TVideo;
class TBackground;
class TTextLabel;

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TLabel *Label1;
        TLabel *Label2;
        TEdit *height;
        TEdit *width;
        TTrackBar *TrackBar1;
        TTrackBar *TrackBar2;
        TEdit *width2;
        TEdit *height2;
        TTrackBar *TrackBar3;
    TEdit *Edit1;
    TBitBtn *BitBtn2;
    TOpenPictureDialog *OpenPictureDialog1;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    TBitBtn *BitBtn5;
    TEdit *Edit2;
    TBitBtn *BitBtn6;
    TBitBtn *BitBtn7;
    TBitBtn *BitBtn8;
    TBitBtn *BitBtn9;
    TBitBtn *BitBtn10;
    TBitBtn *BitBtn11;
    TTrackBar *TrackBar4;
    TEdit *Edit3;
    TEdit *Edit4;
    TTimer *Timer1;
    TImage32 *Image;
    TBitBtn *BitBtn12;
    TBitBtn *BitBtn13;
        void __fastcall TrackBar1Change(TObject *Sender);
        void __fastcall TrackBar2Change(TObject *Sender);
        void __fastcall TrackBar3Change(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall Edit1Change(TObject *Sender);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, TCustomLayer *Layer);
    void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y, TCustomLayer *Layer);
    void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, TCustomLayer *Layer);
    void __fastcall BitBtn4Click(TObject *Sender);
    void __fastcall BitBtn5Click(TObject *Sender);
    void __fastcall BitBtn6Click(TObject *Sender);
    void __fastcall BitBtn7Click(TObject *Sender);
    void __fastcall BitBtn8Click(TObject *Sender);
    void __fastcall BitBtn9Click(TObject *Sender);
    void __fastcall BitBtn10Click(TObject *Sender);
    void __fastcall BitBtn11Click(TObject *Sender);
    void __fastcall TrackBar4Change(TObject *Sender);
    void __fastcall BitBtn12Click(TObject *Sender);
    void __fastcall BitBtn13Click(TObject *Sender);
private:	// User declarations
        void __fastcall OnPaintStage(TObject *Sender, TBitmap32 *Dest, unsigned int StageNum);
        void __fastcall DoPaintBuffer(TObject *Sender);
        void __fastcall Save();
        TBitmapLayer *inlay;
        bool dir;
        TBackground * background;
        TBandReel * band;
        TDiscReel * disc;
        TFancyReel * reel[3];
        TFancyLamp * lamps[320];
        TAlpha * alpha;
        TSevenSeg * seg;
        TBFMLED * bfmled;
        TDotAlpha * dotalpha;
        TDotmatrix * matrix;
        TVideo * video;
        TTextLabel *label;
        int pos;
        int pos2;
        bool on;
        int count;
        TBase * sel;
        bool move;
        int biasX, biasY;
        int oldX, oldY;
        public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
