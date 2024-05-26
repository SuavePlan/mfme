// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Gpshape.pas' rev: 5.00

#ifndef GpshapeHPP
#define GpshapeHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ExtCtrls.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gpshape
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS THackCanvas;
class PASCALIMPLEMENTATION THackCanvas : public Graphics::TCanvas 
{
	typedef Graphics::TCanvas inherited;
	
public:
	#pragma option push -w-inl
	/* TCanvas.Create */ inline __fastcall THackCanvas(void) : Graphics::TCanvas() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCanvas.Destroy */ inline __fastcall virtual ~THackCanvas(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TSurfaceType { stBrushTransparent, stBrushOpaque, stBrushImage, stGradientFill, stBrush50PercentOpaque, 
	stBrush25PercentOpaque };
#pragma option pop

#pragma option push -b-
enum TGPShapeType { gstRectangle, gstSquare, gstRoundRect, gstRoundSquare, gstEllipse, gstCircle, gstDiamond, 
	gstStar, gstPolyGon, gstTriangleLeft, gstTriangleRight, gstTriangleUp, gstTriangleDown };
#pragma option pop

#pragma option push -b-
enum TFillDirection { gdUp, gdDown, gdLeft, gdRight };
#pragma option pop

#pragma option push -b-
enum TShadowOrient { soNone, soLowerRight, soUpperRight, soUpperLeft, soLowerLeft };
#pragma option pop

#pragma option push -b-
enum TShadowFill { sfSolid, sf25Percent, sf50Percent };
#pragma option pop

class DELPHICLASS TManyShape;
class PASCALIMPLEMENTATION TManyShape : public Controls::TGraphicControl 
{
	typedef Controls::TGraphicControl inherited;
	
private:
	Graphics::TColor FBackColor;
	Graphics::TColor FColour1;
	Graphics::TColor FColour2;
	TFillDirection FFillDirection;
	Graphics::TColor FForeColor;
	Graphics::TColor FGradientToColor;
	Graphics::TBitmap* FOnImage1;
	Graphics::TBitmap* FOnImage2;
	Graphics::TBitmap* FOffimage;
	Graphics::TBitmap* FOld;
	int FInVectorPercent;
	Graphics::TColor FOutlineColor;
	Graphics::TPenStyle FOutlineStyle;
	int FOutlineWidth;
	Graphics::TBrushStyle FPattern;
	Byte FReserved;
	int FRoundPerc;
	Graphics::TColor FShadowColor;
	int FShadowDepth;
	TShadowOrient FShadowOrient;
	TGPShapeType FShape;
	int FStartingAngle;
	TSurfaceType FSurfaceType;
	int FVectors;
	TShadowFill FShadowFill;
	bool FTransparent;
	int FState;
	int FButton;
	void __fastcall ChangedOnImage1(System::TObject* Sender);
	void __fastcall ChangedOnImage2(System::TObject* Sender);
	void __fastcall ChangedOffImage(System::TObject* Sender);
	void __fastcall PolyGonFromWinProcs(const Windows::TPoint * a, const int a_Size, int i);
	void __fastcall SetPointsInPolyGonArray(Windows::TPoint * v, const int v_Size, int l, int t, int w, 
		int h, int offsetleft, int offsettop);
	void __fastcall FillWithBitmap(Graphics::TBitmap* FImage);
	void __fastcall FillWithGradient(void);
	void __fastcall GetShadowRects(Windows::TRect &rMainRect, Windows::TRect &rShadowRect);
	void __fastcall PaintDiamond(void);
	void __fastcall PaintEllipse(void);
	void __fastcall PaintPolyGon(void);
	void __fastcall PaintRectangle(void);
	void __fastcall PaintRoundRectangle(void);
	void __fastcall PaintTriangle(void);
	void __fastcall SetBackColor(Graphics::TColor Value);
	void __fastcall SetColor1(Graphics::TColor Value);
	void __fastcall SetColor2(Graphics::TColor Value);
	void __fastcall SetFillDirection(TFillDirection Value);
	void __fastcall SetForeColor(Graphics::TColor Value);
	void __fastcall SetGradientToColor(Graphics::TColor Value);
	void __fastcall SetOnImage1(Graphics::TBitmap* Value);
	void __fastcall SetOnImage2(Graphics::TBitmap* Value);
	void __fastcall SetOffImage(Graphics::TBitmap* Value);
	void __fastcall SetState(int Value);
	void __fastcall SetInVectorPercent(int Value);
	void __fastcall SetOutlineColor(Graphics::TColor Value);
	void __fastcall SetOutlineStyle(Graphics::TPenStyle Value);
	void __fastcall SetOutlineWidth(int Value);
	void __fastcall SetPattern(Graphics::TBrushStyle Value);
	void __fastcall SetPenBrushForBackground(void);
	void __fastcall SetPenBrushForForeground(void);
	void __fastcall SetPenBrushForGradient(void);
	void __fastcall SetPenBrushForImage(void);
	void __fastcall SetPenBrushForOutline(void);
	void __fastcall SetPenBrushForShadow(void);
	void __fastcall SetRoundPerc(int Value);
	void __fastcall SetShadowColor(Graphics::TColor Value);
	void __fastcall SetShadowDepth(int Value);
	void __fastcall SetShadowFill(TShadowFill Value);
	void __fastcall SetShadowOrient(TShadowOrient Value);
	void __fastcall SetShape(TGPShapeType Value);
	void __fastcall SetStartingAngle(int Value);
	void __fastcall SetSurfaceType(TSurfaceType Value);
	void __fastcall SetVectors(int Value);
	void __fastcall SetTransparent(bool Value);
	void __fastcall TransparentBlt(Graphics::TCanvas* Dest, Graphics::TBitmap* Bmp, int X, int Y, Graphics::TColor 
		TransColor);
	void __fastcall SetButton(int Value);
	
protected:
	virtual void __fastcall Paint(void);
	
public:
	__fastcall virtual TManyShape(Classes::TComponent* AOwner);
	__fastcall virtual ~TManyShape(void);
	
__published:
	__property Align ;
	__property Graphics::TColor BackColor = {read=FBackColor, write=SetBackColor, nodefault};
	__property Graphics::TColor Colour1 = {read=FColour1, write=SetColor1, nodefault};
	__property Graphics::TColor Colour2 = {read=FColour2, write=SetColor2, nodefault};
	__property TFillDirection FillDirection = {read=FFillDirection, write=SetFillDirection, nodefault};
		
	__property Graphics::TColor ForeColor = {read=FForeColor, write=SetForeColor, nodefault};
	__property Graphics::TColor GradientToColor = {read=FGradientToColor, write=SetGradientToColor, nodefault
		};
	__property Graphics::TBitmap* OnImage1 = {read=FOnImage1, write=SetOnImage1};
	__property Graphics::TBitmap* OnImage2 = {read=FOnImage2, write=SetOnImage2};
	__property Graphics::TBitmap* OffImage = {read=FOffimage, write=SetOffImage};
	__property int State = {read=FState, write=SetState, default=0};
	__property int InVectorPercent = {read=FInVectorPercent, write=SetInVectorPercent, nodefault};
	__property Graphics::TColor OutlineColor = {read=FOutlineColor, write=SetOutlineColor, nodefault};
	__property Graphics::TPenStyle OutlineStyle = {read=FOutlineStyle, write=SetOutlineStyle, nodefault
		};
	__property int OutlineWidth = {read=FOutlineWidth, write=SetOutlineWidth, nodefault};
	__property Graphics::TBrushStyle Pattern = {read=FPattern, write=SetPattern, nodefault};
	__property PopupMenu ;
	__property int RoundPerc = {read=FRoundPerc, write=SetRoundPerc, nodefault};
	__property Graphics::TColor ShadowColor = {read=FShadowColor, write=SetShadowColor, nodefault};
	__property int ShadowDepth = {read=FShadowDepth, write=SetShadowDepth, nodefault};
	__property TShadowOrient ShadowOrient = {read=FShadowOrient, write=SetShadowOrient, nodefault};
	__property TShadowFill ShadowFill = {read=FShadowFill, write=SetShadowFill, default=0};
	__property TGPShapeType Shape = {read=FShape, write=SetShape, default=0};
	__property int StartingAngle = {read=FStartingAngle, write=SetStartingAngle, nodefault};
	__property TSurfaceType SurfaceType = {read=FSurfaceType, write=SetSurfaceType, nodefault};
	__property int Vectors = {read=FVectors, write=SetVectors, nodefault};
	__property bool Trans = {read=FTransparent, write=SetTransparent, nodefault};
	__property int Button = {read=FButton, write=SetButton, nodefault};
	__property OnClick ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
};


//-- var, const, procedure ---------------------------------------------------
static const Shortint MAXVECTORS = 0x64;
extern PACKAGE Word HatchBits50[8];
extern PACKAGE Word HatchBits25[8];
extern PACKAGE void __fastcall Register(void);

}	/* namespace Gpshape */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gpshape;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Gpshape
