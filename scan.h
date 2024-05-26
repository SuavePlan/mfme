//---------------------------------------------------------------------------

#ifndef scanH
#define scanH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TScanWindow : public TForm
{
__published:	// IDE-managed Components
    TButton *Cancel;
    TLabel *Label9;
    TLabel *Files;
    TTimer *Timer1;
    TLabel *Label1;
    TLabel *Found;
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
    bool cancel;
    __fastcall TScanWindow(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TScanWindow *ScanWindow;
//---------------------------------------------------------------------------
#endif
