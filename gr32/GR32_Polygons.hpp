// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_Polygons.pas' rev: 5.00

#ifndef GR32_PolygonsHPP
#define GR32_PolygonsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32_Blend.hpp>	// Pascal unit
#include <GR32_LowLevel.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_polygons
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TPolyFillMode { pfAlternate, pfWinding };
#pragma option pop

class DELPHICLASS TPolygon32;
class PASCALIMPLEMENTATION TPolygon32 : public System::TObject 
{
	typedef System::TObject inherited;
	
private:
	bool FAntialiased;
	bool FClosed;
	TPolyFillMode FFillMode;
	DynamicArray<DynamicArray<Gr32::TFixedPoint > >  FNormals;
	DynamicArray<DynamicArray<Gr32::TFixedPoint > >  FPoints;
	
protected:
	void __fastcall BuildNormals(void);
	
public:
	__fastcall virtual TPolygon32(void);
	__fastcall virtual ~TPolygon32(void);
	void __fastcall Add(const Gr32::TFixedPoint &P);
	void __fastcall AddPoints(Gr32::TFixedPoint &First, int Count);
	void __fastcall Clear(void);
	TPolygon32* __fastcall Grow(const Gr32::TFixed Delta, float EdgeSharpness);
	void __fastcall Draw(Gr32::TBitmap32* Bitmap, Gr32::TColor32 OutlineColor, Gr32::TColor32 FillColor
		);
	void __fastcall DrawEdge(Gr32::TBitmap32* Bitmap, Gr32::TColor32 Color);
	void __fastcall DrawFill(Gr32::TBitmap32* Bitmap, Gr32::TColor32 Color);
	void __fastcall NewLine(void);
	void __fastcall Offset(const Gr32::TFixed Dx, const Gr32::TFixed Dy);
	TPolygon32* __fastcall Outline(void);
	__property bool Antialiased = {read=FAntialiased, write=FAntialiased, nodefault};
	__property bool Closed = {read=FClosed, write=FClosed, nodefault};
	__property TPolyFillMode FillMode = {read=FFillMode, write=FFillMode, nodefault};
	__property Gr32::TArrayOfArrayOfFixedPoint Normals = {read=FNormals, write=FNormals};
	__property Gr32::TArrayOfArrayOfFixedPoint Points = {read=FPoints, write=FPoints};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall PolylineTS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points
	, Gr32::TColor32 Color, bool Closed);
extern PACKAGE void __fastcall PolylineAS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points
	, Gr32::TColor32 Color, bool Closed);
extern PACKAGE void __fastcall PolylineXS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points
	, Gr32::TColor32 Color, bool Closed);
extern PACKAGE void __fastcall PolyPolylineTS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint 
	Points, Gr32::TColor32 Color, bool Closed);
extern PACKAGE void __fastcall PolyPolylineAS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint 
	Points, Gr32::TColor32 Color, bool Closed);
extern PACKAGE void __fastcall PolyPolylineXS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint 
	Points, Gr32::TColor32 Color, bool Closed);
extern PACKAGE void __fastcall PolygonTS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points
	, Gr32::TColor32 Color, TPolyFillMode Mode);
extern PACKAGE void __fastcall PolygonXS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfFixedPoint Points
	, Gr32::TColor32 Color, TPolyFillMode Mode);
extern PACKAGE void __fastcall PolyPolygonTS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint 
	Points, Gr32::TColor32 Color, TPolyFillMode Mode);
extern PACKAGE void __fastcall PolyPolygonXS(Gr32::TBitmap32* Bitmap, const Gr32::TArrayOfArrayOfFixedPoint 
	Points, Gr32::TColor32 Color, TPolyFillMode Mode);

}	/* namespace Gr32_polygons */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_polygons;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_Polygons
