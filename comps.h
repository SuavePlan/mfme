//---------------------------------------------------------------------------

#ifndef compsH
#define compsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TForm3 : public TForm
{
__published:	// IDE-managed Components
    TListView *ListView1;
private:	// User declarations
public:		// User declarations
    __fastcall TForm3(TComponent* Owner);
    void __fastcall CompView(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
