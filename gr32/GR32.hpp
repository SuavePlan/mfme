// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32.pas' rev: 5.00

#ifndef GR32HPP
#define GR32HPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Graphics.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32
{
//-- type declarations -------------------------------------------------------
typedef unsigned TColor32;

typedef TColor32 *PColor32;

typedef TColor32 TColor32Array[1];

typedef TColor32 *PColor32Array;

typedef DynamicArray<TColor32 >  TArrayOfColor32;

typedef TColor32 TPalette32[256];

typedef TColor32 *PPalette32;

typedef int TFixed;

typedef TFixed *PFixed;

typedef Windows::TPoint  TPoint;

typedef Windows::TPoint *PPoint;

struct TFloatPoint
{
	float X;
	float Y;
} ;

typedef TFloatPoint *PFloatPoint;

struct TFixedPoint
{
	TFixed X;
	TFixed Y;
} ;

typedef TFixedPoint *PFixedPoint;

typedef DynamicArray<Windows::TPoint >  TArrayOfPoint;

typedef DynamicArray<DynamicArray<Windows::TPoint > >  TArrayOfArrayOfPoint;

typedef DynamicArray<TFloatPoint >  TArrayOfFloatPoint;

typedef DynamicArray<DynamicArray<TFloatPoint > >  TArrayOfArrayOfFloatPoint;

typedef DynamicArray<TFixedPoint >  TArrayOfFixedPoint;

typedef DynamicArray<DynamicArray<TFixedPoint > >  TArrayOfArrayOfFixedPoint;

#pragma pack(push, 1)
struct TFloatRect
{
	
	union
	{
		struct 
		{
			TFloatPoint TopLeft;
			TFloatPoint BottomRight;
			
		};
		struct 
		{
			float Left;
			float Top;
			float Right;
			float Bottom;
			
		};
		
	};
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct TFixedRect
{
	
	union
	{
		struct 
		{
			TFixedPoint TopLeft;
			TFixedPoint BottomRight;
			
		};
		struct 
		{
			TFixed Left;
			TFixed Top;
			TFixed Right;
			TFixed Bottom;
			
		};
		
	};
} ;
#pragma pack(pop)

#pragma option push -b-
enum TRectRounding { rrClosest, rrOutside, rrInside };
#pragma option pop

typedef DynamicArray<Byte >  TArrayOfByte;

typedef DynamicArray<int >  TArrayOfInteger;

typedef DynamicArray<DynamicArray<int > >  TArrayOfArrayOfInteger;

typedef DynamicArray<float >  TArrayOfSingle;

#pragma option push -b-
enum TDrawMode { dmOpaque, dmBlend, dmCustom };
#pragma option pop

#pragma option push -b-
enum TStretchFilter { sfNearest, sfLinear, sfLinear2, sfSpline };
#pragma option pop

class DELPHICLASS TThreadPersistent;
class PASCALIMPLEMENTATION TThreadPersistent : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	_RTL_CRITICAL_SECTION FLock;
	int FLockCount;
	int FUpdateCount;
	Classes::TNotifyEvent FOnChange;
	
protected:
	__property int LockCount = {read=FLockCount, nodefault};
	__property int UpdateCount = {read=FUpdateCount, nodefault};
	
public:
	__fastcall virtual TThreadPersistent(void);
	__fastcall virtual ~TThreadPersistent(void);
	virtual void __fastcall Changed(void);
	void __fastcall BeginUpdate(void);
	void __fastcall EndUpdate(void);
	void __fastcall Lock(void);
	void __fastcall Unlock(void);
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
};


class DELPHICLASS TCustomMap;
class PASCALIMPLEMENTATION TCustomMap : public TThreadPersistent 
{
	typedef TThreadPersistent inherited;
	
private:
	int FHeight;
	int FWidth;
	Classes::TNotifyEvent FOnResize;
	void __fastcall SetHeight(int NewHeight);
	void __fastcall SetWidth(int NewWidth);
	
public:
	virtual void __fastcall Delete(void);
	virtual bool __fastcall Empty(void);
	virtual void __fastcall Resized(void);
	bool __fastcall SetSizeFrom(Classes::TPersistent* Source);
	virtual bool __fastcall SetSize(int NewWidth, int NewHeight);
	__property int Height = {read=FHeight, write=SetHeight, nodefault};
	__property int Width = {read=FWidth, write=SetWidth, nodefault};
	__property Classes::TNotifyEvent OnResize = {read=FOnResize, write=FOnResize};
public:
	#pragma option push -w-inl
	/* TThreadPersistent.Create */ inline __fastcall virtual TCustomMap(void) : TThreadPersistent() { }
		
	#pragma option pop
	#pragma option push -w-inl
	/* TThreadPersistent.Destroy */ inline __fastcall virtual ~TCustomMap(void) { }
	#pragma option pop
	
};


typedef void __fastcall (__closure *TPixelCombineEvent)(TColor32 F, TColor32 &B, TColor32 M);

class DELPHICLASS TBitmap32;
class PASCALIMPLEMENTATION TBitmap32 : public TCustomMap 
{
	typedef TCustomMap inherited;
	
private:
	#pragma pack(push, 1)
	tagBITMAPINFO FBitmapInfo;
	#pragma pack(pop)
	
	TColor32 *FBits;
	TDrawMode FDrawMode;
	Graphics::TFont* FFont;
	HBITMAP FHandle;
	HDC FHDC;
	unsigned FMasterAlpha;
	TColor32 FOuterColor;
	TColor32 FPenColor;
	float FStippleCounter;
	DynamicArray<TColor32 >  FStipplePattern;
	float FStippleStep;
	TStretchFilter FStretchFilter;
	Classes::TNotifyEvent FOnHandleChanged;
	TPixelCombineEvent FOnPixelCombine;
	void __fastcall FontChanged(System::TObject* Sender);
	TColor32 __fastcall GetPixel(int X, int Y);
	TColor32 __fastcall GetPixelS(int X, int Y);
	PColor32 __fastcall GetPixelPtr(int X, int Y);
	PColor32Array __fastcall GetScanLine(int Y);
	void __fastcall SetDrawMode(TDrawMode Value);
	void __fastcall SetFont(Graphics::TFont* Value);
	void __fastcall SetMasterAlpha(unsigned Value);
	void __fastcall SetPixel(int X, int Y, TColor32 Value);
	void __fastcall SetPixelS(int X, int Y, TColor32 Value);
	void __fastcall SetStretchFilter(TStretchFilter Value);
	
protected:
	HFONT FontHandle;
	int RasterX;
	int RasterY;
	TFixed RasterXF;
	TFixed RasterYF;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dst);
	virtual void __fastcall HandleChanged(void);
	bool __fastcall Equal(TBitmap32* B);
	void __fastcall SET_T256(int X, int Y, TColor32 C);
	void __fastcall SET_TS256(int X, int Y, TColor32 C);
	virtual void __fastcall ReadData(Classes::TStream* Stream);
	virtual void __fastcall WriteData(Classes::TStream* Stream);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	TColor32 __fastcall GetPixelB(int X, int Y);
	
public:
	__fastcall virtual TBitmap32(void);
	__fastcall virtual ~TBitmap32(void);
	virtual void __fastcall Assign(Classes::TPersistent* Source);
	virtual bool __fastcall SetSize(int NewWidth, int NewHeight);
	virtual bool __fastcall Empty(void);
	void __fastcall Clear(void)/* overload */;
	void __fastcall Clear(TColor32 FillColor)/* overload */;
	virtual void __fastcall Delete(void);
	void __fastcall LoadFromStream(Classes::TStream* Stream);
	void __fastcall SaveToStream(Classes::TStream* Stream);
	void __fastcall LoadFromFile(const AnsiString FileName);
	void __fastcall SaveToFile(const AnsiString FileName);
	void __fastcall LoadFromResourceID(unsigned Instance, int ResID);
	void __fastcall LoadFromResourceName(unsigned Instance, const AnsiString ResName);
	void __fastcall ResetAlpha(void);
	void __fastcall Draw(int DstX, int DstY, TBitmap32* Src)/* overload */;
	void __fastcall Draw(const Windows::TRect &DstRect, const Windows::TRect &SrcRect, TBitmap32* Src)/* overload */
		;
	void __fastcall Draw(const Windows::TRect &DstRect, const Windows::TRect &SrcRect, HDC hSrc)/* overload */
		;
	void __fastcall DrawTo(TBitmap32* Dst)/* overload */;
	void __fastcall DrawTo(TBitmap32* Dst, int DstX, int DstY)/* overload */;
	void __fastcall DrawTo(TBitmap32* Dst, const Windows::TRect &DstRect)/* overload */;
	void __fastcall DrawTo(TBitmap32* Dst, const Windows::TRect &DstRect, const Windows::TRect &SrcRect
		)/* overload */;
	void __fastcall DrawTo(HDC hDst, int DstX, int DstY)/* overload */;
	void __fastcall DrawTo(HDC hDst, const Windows::TRect &DstRect, const Windows::TRect &SrcRect)/* overload */
		;
	__property TColor32 Pixel[int X][int Y] = {read=GetPixel, write=SetPixel/*, default*/};
	__property TColor32 PixelS[int X][int Y] = {read=GetPixelS, write=SetPixelS};
	void __fastcall SetPixelT(int X, int Y, TColor32 Value)/* overload */;
	void __fastcall SetPixelT(PColor32 &Ptr, TColor32 Value)/* overload */;
	void __fastcall SetPixelTS(int X, int Y, TColor32 Value);
	void __fastcall SetPixelF(float X, float Y, TColor32 Value);
	void __fastcall SetPixelX(TFixed X, TFixed Y, TColor32 Value);
	void __fastcall SetPixelFS(float X, float Y, TColor32 Value);
	void __fastcall SetPixelXS(TFixed X, TFixed Y, TColor32 Value);
	void __fastcall SetStipple(TArrayOfColor32 NewStipple)/* overload */;
	void __fastcall SetStipple(const TColor32 * NewStipple, const int NewStipple_Size)/* overload */;
	void __fastcall AdvanceStippleCounter(float LengthPixels);
	TColor32 __fastcall GetStippleColor(void);
	void __fastcall HorzLine(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineS(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineT(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineTS(int X1, int Y, int X2, TColor32 Value);
	void __fastcall HorzLineTSP(int X1, int Y, int X2);
	void __fastcall VertLine(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineS(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineT(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineTS(int X, int Y1, int Y2, TColor32 Value);
	void __fastcall VertLineTSP(int X, int Y1, int Y2);
	void __fastcall Line(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L);
	void __fastcall LineS(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L);
	void __fastcall LineT(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L);
	void __fastcall LineTS(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L);
	void __fastcall LineA(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L);
	void __fastcall LineAS(int X1, int Y1, int X2, int Y2, TColor32 Value, bool L);
	void __fastcall LineX(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, TColor32 Value, bool L)/* overload */
		;
	void __fastcall LineF(float X1, float Y1, float X2, float Y2, TColor32 Value, bool L)/* overload */
		;
	void __fastcall LineXS(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, TColor32 Value, bool L)/* overload */
		;
	void __fastcall LineFS(float X1, float Y1, float X2, float Y2, TColor32 Value, bool L)/* overload */
		;
	void __fastcall LineXP(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, bool L)/* overload */;
	void __fastcall LineFP(float X1, float Y1, float X2, float Y2, bool L)/* overload */;
	void __fastcall LineXSP(TFixed X1, TFixed Y1, TFixed X2, TFixed Y2, bool L)/* overload */;
	void __fastcall LineFSP(float X1, float Y1, float X2, float Y2, bool L)/* overload */;
	__property TColor32 PenColor = {read=FPenColor, write=FPenColor, nodefault};
	void __fastcall MoveTo(int X, int Y);
	void __fastcall LineToS(int X, int Y);
	void __fastcall LineToTS(int X, int Y);
	void __fastcall LineToAS(int X, int Y);
	void __fastcall MoveToX(TFixed X, TFixed Y);
	void __fastcall MoveToF(float X, float Y);
	void __fastcall LineToXS(TFixed X, TFixed Y);
	void __fastcall LineToFS(float X, float Y);
	void __fastcall LineToXSP(TFixed X, TFixed Y);
	void __fastcall LineToFSP(float X, float Y);
	void __fastcall FillRect(int X1, int Y1, int X2, int Y2, TColor32 Value);
	void __fastcall FillRectS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FillRectT(int X1, int Y1, int X2, int Y2, TColor32 Value);
	void __fastcall FillRectTS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FillRectS(const Windows::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall FillRectTS(const Windows::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall FrameRectS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FrameRectTS(int X1, int Y1, int X2, int Y2, TColor32 Value)/* overload */;
	void __fastcall FrameRectTSP(int X1, int Y1, int X2, int Y2);
	void __fastcall FrameRectS(const Windows::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall FrameRectTS(const Windows::TRect &ARect, TColor32 Value)/* overload */;
	void __fastcall RaiseRectTS(int X1, int Y1, int X2, int Y2, int Contrast)/* overload */;
	void __fastcall RaiseRectTS(const Windows::TRect &ARect, int Contrast)/* overload */;
	void __fastcall UpdateFont(void);
	void __fastcall Textout(int X, int Y, const AnsiString Text)/* overload */;
	void __fastcall Textout(int X, int Y, const Windows::TRect &ClipRect, const AnsiString Text)/* overload */
		;
	void __fastcall Textout(const Windows::TRect &ClipRect, const unsigned Flags, const AnsiString Text
		)/* overload */;
	tagSIZE __fastcall TextExtent(const AnsiString Text);
	int __fastcall TextHeight(const AnsiString Text);
	int __fastcall TextWidth(const AnsiString Text);
	void __fastcall RenderText(int X, int Y, const AnsiString Text, int AALevel, TColor32 Color);
	void __fastcall Roll(int Dx, int Dy, bool FillBack, TColor32 FillColor);
	__property HBITMAP BitmapHandle = {read=FHandle, nodefault};
	__property tagBITMAPINFO BitmapInfo = {read=FBitmapInfo};
	__property PColor32Array Bits = {read=FBits};
	__property Graphics::TFont* Font = {read=FFont, write=SetFont};
	__property HDC Handle = {read=FHDC, nodefault};
	__property PColor32 PixelPtr[int X][int Y] = {read=GetPixelPtr};
	__property PColor32Array ScanLine[int Y] = {read=GetScanLine};
	__property float StippleCounter = {read=FStippleCounter, write=FStippleCounter};
	__property float StippleStep = {read=FStippleStep, write=FStippleStep};
	
__published:
	__property TDrawMode DrawMode = {read=FDrawMode, write=SetDrawMode, default=0};
	__property unsigned MasterAlpha = {read=FMasterAlpha, write=SetMasterAlpha, default=255};
	__property TColor32 OuterColor = {read=FOuterColor, write=FOuterColor, default=0};
	__property TStretchFilter StretchFilter = {read=FStretchFilter, write=SetStretchFilter, default=0};
		
	__property OnChange ;
	__property Classes::TNotifyEvent OnHandleChanged = {read=FOnHandleChanged, write=FOnHandleChanged};
		
	__property TPixelCombineEvent OnPixelCombine = {read=FOnPixelCombine, write=FOnPixelCombine};
	__property OnResize ;
};


//-- var, const, procedure ---------------------------------------------------
#define Graphics32Version "1.4"
static const TColor32 clBlack32 = 0xff000000;
static const TColor32 clDimGray32 = 0xff3f3f3f;
static const TColor32 clGray32 = 0xff7f7f7f;
static const TColor32 clLightGray32 = 0xffbfbfbf;
static const TColor32 clWhite32 = 0xffffffff;
static const TColor32 clMaroon32 = 0xff7f0000;
static const TColor32 clGreen32 = 0xff007f00;
static const TColor32 clOlive32 = 0xff7f7f00;
static const TColor32 clNavy32 = 0xff00007f;
static const TColor32 clPurple32 = 0xff7f007f;
static const TColor32 clTeal32 = 0xff007f7f;
static const TColor32 clRed32 = 0xffff0000;
static const TColor32 clLime32 = 0xff00ff00;
static const TColor32 clYellow32 = 0xffffff00;
static const TColor32 clBlue32 = 0xff0000ff;
static const TColor32 clFuchsia32 = 0xffff00ff;
static const TColor32 clAqua32 = 0xff00ffff;
static const TColor32 clTrWhite32 = 0x7fffffff;
static const TColor32 clTrBlack32 = 0x7f000000;
static const TColor32 clTrRed32 = 0x7fff0000;
static const TColor32 clTrGreen32 = 0x7f00ff00;
static const TColor32 clTrBlue32 = 0x7f0000ff;
extern PACKAGE Byte GAMMA_TABLE[256];
extern PACKAGE TColor32 __fastcall Color32(Graphics::TColor WinColor)/* overload */;
extern PACKAGE TColor32 __fastcall Color32(Byte R, Byte G, Byte B, Byte A)/* overload */;
extern PACKAGE TColor32 __fastcall Color32(Byte Index, TColor32 * Palette)/* overload */;
extern PACKAGE TColor32 __fastcall Gray32(Byte Intensity, Byte Alpha);
extern PACKAGE Graphics::TColor __fastcall WinColor(TColor32 Color32);
extern PACKAGE TArrayOfColor32 __fastcall ArrayOfColor32(const TColor32 * Colors, const int Colors_Size
	);
extern PACKAGE int __fastcall RedComponent(TColor32 Color32);
extern PACKAGE int __fastcall GreenComponent(TColor32 Color32);
extern PACKAGE int __fastcall BlueComponent(TColor32 Color32);
extern PACKAGE int __fastcall AlphaComponent(TColor32 Color32);
extern PACKAGE int __fastcall Intensity(TColor32 Color32);
extern PACKAGE TColor32 __fastcall SetAlpha(TColor32 Color32, int NewAlpha);
extern PACKAGE TColor32 __fastcall HSLtoRGB(float H, float S, float L);
extern PACKAGE void __fastcall RGBtoHSL(TColor32 RGB, /* out */ float &H, /* out */ float &S, /* out */ 
	float &L);
extern PACKAGE TFixed __fastcall Fixed(float S)/* overload */;
extern PACKAGE TFixed __fastcall Fixed(int I)/* overload */;
extern PACKAGE int __fastcall FixedFloor(TFixed A);
extern PACKAGE int __fastcall FixedCeil(TFixed A);
extern PACKAGE int __fastcall FixedRound(TFixed A);
extern PACKAGE TFixed __fastcall FixedMul(TFixed A, TFixed B);
extern PACKAGE TFixed __fastcall FixedDiv(TFixed A, TFixed B);
extern PACKAGE Windows::TPoint __fastcall Point(int X, int Y)/* overload */;
extern PACKAGE Windows::TPoint __fastcall Point(const TFloatPoint &FP)/* overload */;
extern PACKAGE Windows::TPoint __fastcall Point(const TFixedPoint &FXP)/* overload */;
extern PACKAGE TFloatPoint __fastcall FloatPoint(float X, float Y)/* overload */;
extern PACKAGE TFloatPoint __fastcall FloatPoint(const Windows::TPoint &P)/* overload */;
extern PACKAGE TFloatPoint __fastcall FloatPoint(const TFixedPoint &FXP)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(int X, int Y)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(float X, float Y)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(const Windows::TPoint &P)/* overload */;
extern PACKAGE TFixedPoint __fastcall FixedPoint(const TFloatPoint &FP)/* overload */;
extern PACKAGE Windows::TRect __fastcall MakeRect(int L, int T, int R, int B);
extern PACKAGE Windows::TRect __fastcall MakeRectF(const TFloatRect &FR, TRectRounding Rounding);
extern PACKAGE Windows::TRect __fastcall MakeRectX(const TFixedRect &FXR, TRectRounding Rounding);
extern PACKAGE TFloatRect __fastcall FloatRect(float L, float T, float R, float B)/* overload */;
extern PACKAGE TFloatRect __fastcall FloatRect(const Windows::TRect &ARect)/* overload */;
extern PACKAGE bool __fastcall IntersectRect(/* out */ Windows::TRect &Dst, const Windows::TRect &R1
	, const Windows::TRect &R2);
extern PACKAGE bool __fastcall IntersectRectF(/* out */ TFloatRect &Dst, const TFloatRect &FR1, const 
	TFloatRect &FR2);
extern PACKAGE bool __fastcall EqualRect(const Windows::TRect &R1, const Windows::TRect &R2);
extern PACKAGE void __fastcall InflateRect(Windows::TRect &R, int Dx, int Dy)/* overload */;
extern PACKAGE void __fastcall InflateRectF(TFloatRect &FR, float Dx, float Dy);
extern PACKAGE void __fastcall OffsetRect(Windows::TRect &R, int Dx, int Dy)/* overload */;
extern PACKAGE void __fastcall OffsetRectF(TFloatRect &FR, float Dx, float Dy)/* overload */;
extern PACKAGE bool __fastcall IsRectEmpty(const Windows::TRect &R)/* overload */;
extern PACKAGE bool __fastcall IsRectEmptyF(const TFloatRect &FR)/* overload */;
extern PACKAGE bool __fastcall PtInRect(const Windows::TRect &R, const Windows::TPoint &P);
extern PACKAGE void __fastcall SetGamma(float Gamma);

}	/* namespace Gr32 */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32
