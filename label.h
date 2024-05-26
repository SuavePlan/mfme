//---------------------------------------------------------------------------
#ifndef labelH
#define labelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TPropertiesForm : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *Xval;
    TButton *OK;
    TButton *Cancel;
    TEdit *Yval;
    TLabel *Label2;
    TEdit *Caption;
    TLabel *Label3;
    TLabel *Label4;
    TShape *Colour;
    TColorDialog *ColorDialog1;
    TEdit *ButY;
    TEdit *ButX;
    TLabel *Label5;
    TLabel *Label6;
    void __fastcall OKClick(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall ColourMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    
    
private:	// User declarations
public:		// User declarations
    __fastcall TPropertiesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPropertiesForm *PropertiesForm;
//---------------------------------------------------------------------------
#endif
