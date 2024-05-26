//---------------------------------------------------------------------------

#ifndef samplesH
#define samplesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TSampler : public TForm
{
__published:	// IDE-managed Components
    TSpeedButton *SpeedButton1;
    TListBox *ListBox1;
    TLabel *Label1;
    TLabel *Label2;
    TSpeedButton *Save;
    TEdit *Rate;
    TSpeedButton *Apply;
    TLabel *Label3;
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SaveClick(TObject *Sender);
    void __fastcall ApplyClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall RateChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TSampler(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSampler *Sampler;
//---------------------------------------------------------------------------
#endif
