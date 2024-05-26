//---------------------------------------------------------------------------

#ifndef createbandH
#define createbandH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TCreateBandForm : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TComboBox *Reel;
    TLabel *Label2;
    TComboBox *Stops;
    TSpeedButton *Apply;
        TCheckBox *Horizontal;
    void __fastcall ApplyClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TCreateBandForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCreateBandForm *CreateBandForm;
//---------------------------------------------------------------------------
#endif
