//---------------------------------------------------------------------------

#ifndef prefsH
#define prefsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <registry.hpp>
//---------------------------------------------------------------------------
class TGeneralConfig : public TForm
{
__published:	// IDE-managed Components
    TCheckBox *Draggablereels;
    TCheckBox *ConvertReel;
    TCheckBox *MeterPanelResize;
    TCheckBox *NoAboutBox;
    TCheckBox *StartInGameManager;
    TCheckBox *MeterPanelOff;
    TCheckBox *LongTermMeters;
    TCheckBox *FileNames;
    TCheckBox *ActiveInEdit;
    TCheckBox *EscQuitsProgram;
    TCheckBox *HighPriority;
    void __fastcall DraggablereelsClick(TObject *Sender);
    void __fastcall ConvertReelClick(TObject *Sender);
    void __fastcall MeterPanelResizeClick(TObject *Sender);
    void __fastcall NoAboutBoxClick(TObject *Sender);
    void __fastcall StartInGameManagerClick(TObject *Sender);
    void __fastcall MeterPanelOffClick(TObject *Sender);
    void __fastcall LongTermMetersClick(TObject *Sender);
    void __fastcall FileNamesClick(TObject *Sender);
    void __fastcall ActiveInEditClick(TObject *Sender);
    void __fastcall EscQuitsProgramClick(TObject *Sender);
    void __fastcall HighPriorityClick(TObject *Sender);
private:	// User declarations
    void __fastcall SaveBool(String name, bool value);
    bool __fastcall ReadBool(String name);
    bool loaded;
    TRegIniFile *RegFile;
    public:		// User declarations
    __fastcall TGeneralConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGeneralConfig *GeneralConfig;
//---------------------------------------------------------------------------
#endif
