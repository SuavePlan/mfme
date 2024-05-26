//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Frame2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TFrame2 *)
{
    new TFrame2(NULL);
}
//---------------------------------------------------------------------------
__fastcall TFrame2::TFrame2(TComponent* Owner)
    : TBevel(Owner)
{
}
//---------------------------------------------------------------------------
namespace Frame2
{
    void __fastcall PACKAGE Register()
    {
        TComponentClass classes[1] = {__classid(TFrame2)};
        RegisterComponents("CJW", classes, 0);
    }
}
//---------------------------------------------------------------------------
 