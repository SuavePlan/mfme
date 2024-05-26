//---------------------------------------------------------------------------

#ifndef notesH
#define notesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TLayoutNotes : public TForm
{
__published:	// IDE-managed Components
    TMemo *Notes;
    void __fastcall NotesChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TLayoutNotes(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLayoutNotes *LayoutNotes;
//---------------------------------------------------------------------------
#endif
