// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_ByteMaps.pas' rev: 5.00

#ifndef GR32_ByteMapsHPP
#define GR32_ByteMapsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32_Transforms.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_bytemaps
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TConversionType { ctRed, ctGreen, ctBlue, ctAlpha, ctUniformRGB, ctWeightedRGB };
#pragma option pop

class DELPHICLASS TByteMap;
class PASCALIMPLEMENTATION TByteMap : public Gr32::TCustomMap 
{
	typedef Gr32::TCustomMap inherited;
	
private:
	DynamicArray<Byte >  FBytes;
	Byte __fastcall GetValue(int X, int Y);
	Windows::PByte __fastcall GetValPtr(int X, int Y);
	void __fastcall SetValue(int X, int Y, Byte Value);
	
protected:
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	
public:
	__fastcall virtual ~TByteMap(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(Byte FillValue);
	void __fastcall ReadFrom(Gr32::TBitmap32* Source, TConversionType Conversion);
	virtual bool __fastcall SetSize(int NewWidth, int NewHeight);
	void __fastcall WriteTo(Gr32::TBitmap32* Dest, TConversionType Conversion)/* overload */;
	void __fastcall WriteTo(Gr32::TBitmap32* Dest, const Gr32::TColor32 * Palette)/* overload */;
	__property Gr32::TArrayOfByte Bytes = {read=FBytes};
	__property Windows::PByte ValPtr[int X][int Y] = {read=GetValPtr};
	__property Byte Value[int X][int Y] = {read=GetValue, write=SetValue/*, default*/};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TByteMap(void) : Gr32::TCustomMap() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_bytemaps */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_bytemaps;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_ByteMaps
