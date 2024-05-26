//---------------------------------------------------------------------------

#ifndef snapH
#define snapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TSnapForm : public TForm
{
__published:	// IDE-managed Components
    TEdit *X;
    TEdit *Y;
    TLabel *Label1;
    TSpeedButton *SpeedButton1;
    TLabel *Label2;
    void __fastcall SpeedButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TSnapForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSnapForm *SnapForm;
//---------------------------------------------------------------------------
#endif
