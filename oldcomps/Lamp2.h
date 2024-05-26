//---------------------------------------------------------------------------
#ifndef Lamp2H
#define Lamp2H
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "Gpshape.hpp"
//---------------------------------------------------------------------------
class PACKAGE TLamp2 : public TManyShape
{
  String FCaption;
protected:
    virtual void __fastcall Paint(void);
    void __fastcall CMFontChanged(TMessage &Message);
BEGIN_MESSAGE_MAP
  MESSAGE_HANDLER(CM_FONTCHANGED, TMessage, CMFontChanged);
END_MESSAGE_MAP(TManyShape)
public:
    __fastcall TLamp2(TComponent* Owner);
    void __fastcall SetCaption(String Caption);
__published:
__property String Caption = { read=FCaption, write=SetCaption};
__property Font;
__property OnEndDrag;
};
//---------------------------------------------------------------------------
#endif
