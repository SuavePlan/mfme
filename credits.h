//---------------------------------------------------------------------------
#ifndef creditsH
#define creditsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "zPanel2.hpp"
#include "Mystd.hpp"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TCredit : public TForm
{
__published:	// IDE-managed Components
    TzcCreditScroll *scroll;
    TzcURLLabel *zcURLLabel1;
    Tz3DLabel *z3DLabel1;
    Tz3DLabel *z3DLabel2;
    TzcURLLabel *zcURLLabel2;
    TImage *Image1;
    void __fastcall scrollMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall scrollMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    
private:	// User declarations
public:		// User declarations
    __fastcall TCredit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCredit *Credit;
//---------------------------------------------------------------------------
#endif
