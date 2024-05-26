//---------------------------------------------------------------------------
#ifndef chreditH
#define chreditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TChrEditor : public TForm
{
__published:	// IDE-managed Components
    TPageControl *PageControl1;
    TTabSheet *Barcrest;
    TListBox *Box;
    TEdit *Patch1;
    TEdit *Patch2;
    TEdit *Patch3;
    TEdit *Patch4;
    TEdit *Patch5;
    TEdit *Patch6;
    TEdit *Patch7;
    TEdit *Patch8;
    TTabSheet *BWB;
    TEdit *bwb0;
    TEdit *bwb1;
    TEdit *bwb2;
    TEdit *bwb3;
    TEdit *bwb4;
    TEdit *bwb5;
    TEdit *bwb6;
    TEdit *bwb7;
    TEdit *bwb8;
    TEdit *bwb9;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall BoxClick(TObject *Sender);
    void __fastcall BoxChange(TObject *Sender);
    void __fastcall PatchDblClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BWBChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TChrEditor(TComponent* Owner);
    int index;
    void __fastcall Refresh(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TChrEditor *ChrEditor;
//---------------------------------------------------------------------------
#endif
