//---------------------------------------------------------------------------

#ifndef secmeterH
#define secmeterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TSECForm : public TForm
{
__published:	// IDE-managed Components
    TEdit *Value1;
    TEdit *Value2;
    TEdit *Value3;
    TEdit *Value4;
    TEdit *Value5;
    TEdit *Value6;
    TEdit *Text1;
    TEdit *Text2;
    TEdit *Text3;
    TEdit *Text4;
    TEdit *Text5;
    TEdit *Text6;
    TLabel *Label1;
    TLabel *Label2;
    TEdit *Text7;
    TEdit *Value7;
    TEdit *Text8;
    TEdit *Value8;
    TEdit *Text9;
    TEdit *Value9;
    TEdit *Text10;
    TEdit *Value10;
private:	// User declarations
public:		// User declarations
    void __fastcall SetText(int nbr, String text);
    void __fastcall SetValue(int nbr, int value);
    __fastcall TSECForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSECForm *SECForm;
//---------------------------------------------------------------------------
#endif
