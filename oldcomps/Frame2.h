//---------------------------------------------------------------------------
#ifndef Frame2H
#define Frame2H
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TFrame2 : public TBevel
{
private:
protected:
public:
    __fastcall TFrame2(TComponent* Owner);
__published:
  __property OnMouseUp;
  __property OnMouseMove;
  __property OnMouseDown;
};
//---------------------------------------------------------------------------
#endif
