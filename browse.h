//---------------------------------------------------------------------------
#ifndef browseH
#define browseH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TBrowser : public TForm
{
__published:	// IDE-managed Components
    TDirectoryListBox *DirectoryListBox1;
    TButton *OK;
    TButton *Button2;
    TDriveComboBox *DriveComboBox1;
    TLabel *Label1;
    void __fastcall Button2Click(TObject *Sender);
    void __fastcall OKClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TBrowser(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBrowser *Browser;
//---------------------------------------------------------------------------
#endif
