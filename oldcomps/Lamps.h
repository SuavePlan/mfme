//---------------------------------------------------------------------------
#ifndef LampsH
#define LampsH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TLamp : public TShape
{
private:
  String FCaption;
  TColor FColour1;
  TColor FColour2;
protected:
    virtual void __fastcall Paint(void);
    void __fastcall CMFontChanged(TMessage &Message);
    void __fastcall SetColour1(TColor Color);
    void __fastcall SetColour2(TColor Color);
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(CM_FONTCHANGED, TMessage, CMFontChanged);
END_MESSAGE_MAP(TShape)
public:
    __fastcall TLamp(TComponent* Owner);
    void __fastcall SetCaption(String Caption);
__published:
__property String Caption = { read=FCaption, write=SetCaption};
__property TColor Colour1 = { read=FColour1, write=SetColour1};
__property TColor Colour2 = { read=FColour2, write=SetColour2};
__property Font;
};
//---------------------------------------------------------------------------
#endif
