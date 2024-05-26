//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("test.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("Lamp2.cpp");
USEUNIT("gpshape.pas");
USEUNIT("Seven.pas");
USEUNIT("Frame2.cpp");
USEUNIT("Lamps.cpp");
USEPACKAGE("bcbsmp50.bpi");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
