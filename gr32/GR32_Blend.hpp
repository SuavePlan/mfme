// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_Blend.pas' rev: 5.00

#ifndef GR32_BlendHPP
#define GR32_BlendHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_blend
{
//-- type declarations -------------------------------------------------------
typedef Gr32::TColor32 __fastcall (*TCombineReg)(Gr32::TColor32 X, Gr32::TColor32 Y, Gr32::TColor32 
	W);

typedef void __fastcall (*TCombineMem)(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 W);

typedef Gr32::TColor32 __fastcall (*TBlendReg)(Gr32::TColor32 F, Gr32::TColor32 B);

typedef void __fastcall (*TBlendMem)(Gr32::TColor32 F, Gr32::TColor32 &B);

typedef Gr32::TColor32 __fastcall (*TBlendRegEx)(Gr32::TColor32 F, Gr32::TColor32 B, Gr32::TColor32 
	M);

typedef void __fastcall (*TBlendMemEx)(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);

typedef void __fastcall (*TBlendLine)(Gr32::PColor32 Src, Gr32::PColor32 Dst, int Count);

typedef void __fastcall (*TBlendLineEx)(Gr32::PColor32 Src, Gr32::PColor32 Dst, int Count, Gr32::TColor32 
	M);

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool MMX_ACTIVE;
extern PACKAGE TCombineReg CombineReg;
extern PACKAGE TCombineMem CombineMem;
extern PACKAGE TBlendReg BlendReg;
extern PACKAGE TBlendMem BlendMem;
extern PACKAGE TBlendRegEx BlendRegEx;
extern PACKAGE TBlendMemEx BlendMemEx;
extern PACKAGE TBlendLine BlendLine;
extern PACKAGE TBlendLineEx BlendLineEx;
extern PACKAGE void __fastcall EMMS(void);
extern PACKAGE Gr32::TColor32 __fastcall ColorAdd(Gr32::TColor32 C1, Gr32::TColor32 C2);
extern PACKAGE Gr32::TColor32 __fastcall ColorSub(Gr32::TColor32 C1, Gr32::TColor32 C2);
extern PACKAGE Gr32::TColor32 __fastcall ColorModulate(Gr32::TColor32 C1, Gr32::TColor32 C2);
extern PACKAGE Gr32::TColor32 __fastcall ColorMax(Gr32::TColor32 C1, Gr32::TColor32 C2);
extern PACKAGE Gr32::TColor32 __fastcall ColorMin(Gr32::TColor32 C1, Gr32::TColor32 C2);
extern PACKAGE Gr32::TColor32 __fastcall Lighten(Gr32::TColor32 C, int Amount);

}	/* namespace Gr32_blend */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_blend;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_Blend
