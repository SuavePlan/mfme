//---------------------------------------------------------------------------

#ifndef managernotesH
#define managernotesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TGameNotes : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label3;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TEdit *Manufacturer;
    TEdit *ReleaseDate;
    TEdit *MachineType;
    TEdit *Technology;
    TEdit *Stakes;
    TEdit *Prizes;
    TMemo *Notes;
    TSpeedButton *SpeedButton1;
    TSpeedButton *SpeedButton2;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Name;
    void __fastcall SpeedButton1Click(TObject *Sender);
    void __fastcall SpeedButton2Click(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TGameNotes(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGameNotes *GameNotes;
//---------------------------------------------------------------------------
#endif
