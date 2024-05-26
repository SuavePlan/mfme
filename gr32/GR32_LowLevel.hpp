// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_LowLevel.pas' rev: 5.00

#ifndef GR32_LowLevelHPP
#define GR32_LowLevelHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_lowlevel
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern PACKAGE Gr32::TColor32 __fastcall Clamp(int Value);
extern PACKAGE void __fastcall FillLongword(void *X, int Count, unsigned Value);
extern PACKAGE void __fastcall MoveLongword(const void *Source, void *Dest, int Count);
extern PACKAGE void __fastcall Swap(int &A, int &B);
extern PACKAGE void __fastcall TestSwap(int &A, int &B);
extern PACKAGE bool __fastcall TestClip(int &A, int &B, int Size);
extern PACKAGE int __fastcall Constrain(int Value, int Lo, int Hi);
extern PACKAGE int __fastcall SAR_4(int Value);
extern PACKAGE int __fastcall SAR_8(int Value);
extern PACKAGE int __fastcall SAR_9(int Value);
extern PACKAGE int __fastcall SAR_12(int Value);
extern PACKAGE int __fastcall SAR_13(int Value);
extern PACKAGE int __fastcall SAR_14(int Value);
extern PACKAGE int __fastcall SAR_16(int Value);
extern PACKAGE Gr32::TColor32 __fastcall ColorSwap(Graphics::TColor WinColor);
extern PACKAGE int __fastcall MulDiv(int Multiplicand, int Multiplier, int Divisor);

}	/* namespace Gr32_lowlevel */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_lowlevel;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_LowLevel
