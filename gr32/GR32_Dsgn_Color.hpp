// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_Dsgn_Color.pas' rev: 5.00

#ifndef GR32_Dsgn_ColorHPP
#define GR32_Dsgn_ColorHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <DsgnIntf.hpp>	// Pascal unit
#include <GR32_Image.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <Consts.hpp>	// Pascal unit
#include <Registry.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_dsgn_color
{
//-- type declarations -------------------------------------------------------
struct TColorEntry;
typedef TColorEntry *PColorEntry;

struct TColorEntry
{
	System::SmallString<31>  Name;
	Gr32::TColor32 Color;
} ;

class DELPHICLASS TColorManager;
class PASCALIMPLEMENTATION TColorManager : public Classes::TList 
{
	typedef Classes::TList inherited;
	
public:
	__fastcall virtual ~TColorManager(void);
	void __fastcall AddColor(const AnsiString AName, Gr32::TColor32 AColor);
	void __fastcall EnumColors(Classes::TGetStrProc Proc);
	Gr32::TColor32 __fastcall FindColor(const AnsiString AName);
	Gr32::TColor32 __fastcall GetColor(const AnsiString AName);
	AnsiString __fastcall GetColorName(Gr32::TColor32 AColor);
	void __fastcall RegisterDefaultColors(void);
	void __fastcall RemoveColor(const AnsiString AName);
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TColorManager(void) : Classes::TList() { }
	#pragma option pop
	
};


class DELPHICLASS TColor32Property;
class PASCALIMPLEMENTATION TColor32Property : public Dsgnintf::TIntegerProperty 
{
	typedef Dsgnintf::TIntegerProperty inherited;
	
public:
	virtual Dsgnintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual AnsiString __fastcall GetValue();
	virtual void __fastcall GetValues(Classes::TGetStrProc Proc);
	virtual void __fastcall SetValue(const AnsiString Value);
protected:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TColor32Property(const Dsgnintf::_di_IFormDesigner 
		ADesigner, int APropCount) : Dsgnintf::TIntegerProperty(ADesigner, APropCount) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TColor32Property(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TColorManager* ColorManager;
extern PACKAGE void __fastcall RegisterColor(const AnsiString AName, Gr32::TColor32 AColor);
extern PACKAGE void __fastcall UnregisterColor(const AnsiString AName);

}	/* namespace Gr32_dsgn_color */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_dsgn_color;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_Dsgn_Color
