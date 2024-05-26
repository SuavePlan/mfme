// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_Filters.pas' rev: 5.00

#ifndef GR32_FiltersHPP
#define GR32_FiltersHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32_ByteMaps.hpp>	// Pascal unit
#include <GR32_Blend.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_filters
{
//-- type declarations -------------------------------------------------------
typedef Byte TLUT8[256];

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall AlphaToGrayscale(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall IntensityToAlpha(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall Invert(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall InvertRGB(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall ColorToGrayscale(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src);
extern PACKAGE void __fastcall ApplyLUT(Gr32::TBitmap32* Dst, Gr32::TBitmap32* Src, const Byte * LUT
	);

}	/* namespace Gr32_filters */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_filters;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_Filters
