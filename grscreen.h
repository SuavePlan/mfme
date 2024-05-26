//---------------------------------------------------------------------------

#ifndef grscreenH
#define grscreenH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
    TImage32 *Image;
    void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, TCustomLayer *Layer);
    void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, TCustomLayer *Layer);
    void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y, TCustomLayer *Layer);
    void __fastcall ImageKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall ImageKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    TGRBase * sel;
    int BiasX, BiasY;
    bool move;
    __fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
