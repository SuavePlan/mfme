//---------------------------------------------------------------------------
#ifndef gamemanagerH
#define gamemanagerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TManager : public TForm
{
__published:	// IDE-managed Components
    TListView *ListView1;
    TButton *Scan;
    TEdit *RootDir;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *TotalIn;
    TLabel *TotalOut;
    TLabel *Label3;
    TLabel *NbrGames;
    TLabel *Label4;
    TLabel *Label5;
    TLabel *PAndL;
    TLabel *Label7;
    TLabel *TotPercentage;
    TLabel *Label6;
    TLabel *GamePath;
    TButton *Button1;
    TButton *BackupRAM;
    TButton *Setvtp;
    TButton *Browse;
    TCheckBox *SubDirs;
    void __fastcall ScanClick(TObject *Sender);
    void __fastcall ListView1DblClick(TObject *Sender);
    void __fastcall ListView1ColumnClick(TObject *Sender,
          TListColumn *Column);
    
    void __fastcall ListView1Click(TObject *Sender);
    void __fastcall Button1Click(TObject *Sender);
    
    void __fastcall SetvtpClick(TObject *Sender);
    
    
    void __fastcall BackupRAMClick(TObject *Sender);
    
    void __fastcall BrowseClick(TObject *Sender);
private:	// User declarations
    int Total_In;
    int Total_Out;
    int Total_Games;
public:		// User declarations
    void __fastcall Search(String Path, int Attributes, int depth);
    void __fastcall CheckFile(String Path);
    __fastcall TManager(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TManager *Manager;
//---------------------------------------------------------------------------
#endif
