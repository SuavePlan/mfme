//---------------------------------------------------------------------------
#ifndef iomonitorH
#define iomonitorH
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
//#include <Classes.hpp>
//#include <Controls.hpp>
//#include <StdCtrls.hpp>
//#include <Forms.hpp>
//#include <ExtCtrls.hpp>
//#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TIOMonitor : public TForm
{
__published:	// IDE-managed Components
    TTabControl *Select;
    TGroupBox *Triacs;
    TShape *Triac8;
    TShape *Triac7;
    TShape *Triac6;
    TShape *Triac5;
    TShape *Triac4;
    TShape *Triac3;
    TShape *Triac2;
    TShape *Triac1;
    TGroupBox *Meters;
    TShape *Meter8;
    TShape *Meter7;
    TShape *Meter6;
    TShape *Meter5;
    TShape *Meter4;
    TShape *Meter3;
    TShape *Meter2;
    TShape *Meter1;
    TGroupBox *Reels;
    TShape *Reel18;
    TShape *Reel17;
    TShape *Reel16;
    TShape *Reel15;
    TShape *Reel14;
    TShape *Reel13;
    TShape *Reel12;
    TShape *Reel11;
    TShape *Reel28;
    TShape *Reel27;
    TShape *Reel26;
    TShape *Reel25;
    TShape *Reel24;
    TShape *Reel23;
    TShape *Reel22;
    TShape *Reel21;
    TLabel *Pos1;
    TLabel *Pos2;
    TLabel *Pos3;
    TLabel *Pos4;
    TGroupBox *Sinks;
    TShape *LampSel1;
    TShape *LampSel2;
    TShape *LampSel3;
    TShape *LampSel4;
    TShape *LampSel5;
    TShape *LampSel6;
    TShape *LampSel7;
    TShape *LampSel8;
    TShape *LampSel9;
    TShape *LampSel10;
    TShape *LampSel11;
    TShape *LampSel12;
    TShape *LampSel13;
    TShape *LampSel14;
    TShape *LampSel15;
    TShape *LampSel16;
    TGroupBox *Selects;
    TShape *LampDrv1;
    TShape *LampDrv2;
    TShape *LampDrv3;
    TShape *LampDrv4;
    TShape *LampDrv5;
    TShape *LampDrv6;
    TShape *LampDrv7;
    TShape *LampDrv8;
    TShape *LampDrv9;
    TShape *LampDrv10;
    TShape *LampDrv11;
    TShape *LampDrv12;
    TShape *LampDrv13;
    TShape *LampDrv14;
    TShape *LampDrv15;
    TShape *LampDrv16;
    TGroupBox *Switches1;
    TShape *Switch11;
    TShape *Switch12;
    TShape *Switch13;
    TShape *Switch14;
    TShape *Switch15;
    TShape *Switch16;
    TShape *Switch17;
    TShape *Switch18;
    TShape *Switch19;
    TShape *Switch110;
    TShape *Switch111;
    TShape *Switch112;
    TShape *Switch113;
    TShape *Switch114;
    TShape *Switch115;
    TShape *Switch116;
    TGroupBox *Switches2;
    TShape *Switch21;
    TShape *Switch22;
    TShape *Switch23;
    TShape *Switch24;
    TShape *Switch25;
    TShape *Switch26;
    TShape *Switch27;
    TShape *Switch28;
    TShape *Switch29;
    TShape *Switch210;
    TShape *Switch211;
    TShape *Switch212;
    TShape *Switch213;
    TShape *Switch214;
    TShape *Switch215;
    TShape *Switch216;
    TGroupBox *Aux2;
    TShape *Aux21;
    TShape *Aux22;
    TShape *Aux23;
    TShape *Aux24;
    TShape *Aux25;
    TShape *Aux26;
    TShape *Aux27;
    TShape *Aux28;
    TGroupBox *Aux3;
    TShape *Aux31;
    TShape *Aux32;
    TShape *Aux33;
    TShape *Aux34;
    TShape *Aux35;
    TShape *Aux36;
    TShape *Aux37;
    TShape *Aux38;
    TGroupBox *LEDS;
    TShape *seg1;
    TShape *seg2;
    TShape *seg3;
    TShape *seg4;
    TShape *seg5;
    TShape *seg6;
    TShape *seg7;
    TShape *seg8;
    TGroupBox *Aux1;
    TShape *Aux11;
    TShape *Aux12;
    TShape *Aux13;
    TShape *Aux14;
    TShape *Aux15;
    TShape *Aux16;
    TShape *Aux17;
    TShape *Aux18;
    TShape *Reel34;
    TShape *Reel33;
    TShape *Reel32;
    TShape *Reel31;
    TLabel *Pos5;
    TShape *Reel38;
    TShape *Reel37;
    TShape *Reel36;
    TShape *Reel35;
    TLabel *Pos6;
    TShape *Triac9;
    TShape *Triac10;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall SelectChange(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    
    
    
private:	// User declarations
public:		// User declarations
    __fastcall TIOMonitor(TComponent* Owner);
    void __fastcall UpdateTriacs(Word value);
    void __fastcall UpdateMeters(Byte value);
    void __fastcall UpdateReel1(Byte value, int pos1, int pos2);
    void __fastcall UpdateReel2(Byte value, int pos1, int pos2);
    void __fastcall UpdateReel3(Byte value, int pos1, int pos2);
    void __fastcall UpdateLampSelects(Word value);
    void __fastcall UpdateLampDrives1(Word value);
    void __fastcall UpdateAux1(Byte value);
    void __fastcall UpdateAux2(Byte value);
    void __fastcall UpdateAux3(Byte value);
    void __fastcall Update7segs(Byte value);
    void __fastcall Reset(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TIOMonitor *IOMonitor;
//---------------------------------------------------------------------------
#endif
