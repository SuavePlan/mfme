//---------------------------------------------------------------------------

#ifndef bandH
#define bandH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TLoadBandForm : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TComboBox *Stops;
    TLabel *Label2;
    TSpeedButton *Apply;
    TComboBox *Reel;
    TCheckBox *Horizontal;
    void __fastcall ApplyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TLoadBandForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLoadBandForm *LoadBandForm;
//---------------------------------------------------------------------------
#endif
