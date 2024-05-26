// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_Add_BlendModes.pas' rev: 5.00

#ifndef GR32_Add_BlendModesHPP
#define GR32_Add_BlendModesHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_add_blendmodes
{
//-- type declarations -------------------------------------------------------
typedef Gr32::TColor32 TBlendmap[65536];

class DELPHICLASS TBlendmode;
class PASCALIMPLEMENTATION TBlendmode : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	void __fastcall Normal(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Screen(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Lighten(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Darken(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Additive(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Subtractive(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Multiply(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Burn(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Dodge(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Reflect(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Glow(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Heat(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Freeze(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Overlay(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Average(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Soft_Light(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Bright_Light(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Hard_Light(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Difference(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Exclusion(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Negation(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Red(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Green(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Blue(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Hue(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Saturation(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Lightness(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
	void __fastcall Custom(Gr32::TColor32 F, Gr32::TColor32 &B, Gr32::TColor32 M);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TBlendmode(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TBlendmode(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TBlendmode* Blendmode;
extern PACKAGE Gr32::TColor32 BlendMap[65536];
extern PACKAGE Gr32::TColor32 __fastcall RenderBlendmap(Gr32::TPixelCombineEvent Blend);
extern PACKAGE void __fastcall BlendmapToBitmap(const Gr32::TColor32 * Src, Gr32::TBitmap32* Dst);
extern PACKAGE void __fastcall BitmapToBlendmap(Gr32::TBitmap32* Src, /* out */ Gr32::TColor32 * Dst
	);
extern PACKAGE Classes::TStringList* __fastcall BlendModeList(void);
extern PACKAGE int __fastcall GetIndex(Gr32::TPixelCombineEvent Mode);
extern PACKAGE Gr32::TPixelCombineEvent __fastcall GetBlendMode(int index);

}	/* namespace Gr32_add_blendmodes */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_add_blendmodes;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_Add_BlendModes
