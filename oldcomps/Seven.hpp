// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Seven.pas' rev: 5.00

#ifndef SevenHPP
#define SevenHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Menus.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Seven
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TCustomSevenSegment;
class PASCALIMPLEMENTATION TCustomSevenSegment : public Controls::TGraphicControl 
{
	typedef Controls::TGraphicControl inherited;
	
private:
	int FValue;
	int FThickness;
	int FSpace;
	int FHorzSpace;
	int FVertSpace;
	Graphics::TColor FOnColor;
	Graphics::TColor FOffColor;
	bool FdpRight;
	bool FdpOn;
	bool Fseg14;
	bool FProgrammable;
	int FMasks[16];
	void __fastcall SetValue(int x);
	void __fastcall SetThickness(int x);
	void __fastcall SetSpace(int x);
	void __fastcall SetHorzSpace(int x);
	void __fastcall SetVertSpace(int x);
	void __fastcall SetOnColor(Graphics::TColor x);
	void __fastcall SetOffColor(Graphics::TColor x);
	void __fastcall SetDP(bool x);
	void __fastcall SetDPOn(bool x);
	void __fastcall Set14(bool x);
	void __fastcall SetProgrammable(bool x);
	void __fastcall DrawSegment(int Digit, Graphics::TBitmap* bitmap, const Windows::TRect &rect);
	
protected:
	void __fastcall IncRect(Windows::TRect &MyRect, int Delta);
	HIDESBASE MESSAGE void __fastcall CMVisibleChanged(Messages::TMessage &msg);
	
public:
	void __fastcall SetMasks(int index, int mask);
	int __fastcall GetMasks(int index);
	__fastcall virtual TCustomSevenSegment(Classes::TComponent* AOwner);
	
__published:
	__property int Value = {read=FValue, write=SetValue, nodefault};
	__property int Thickness = {read=FThickness, write=SetThickness, nodefault};
	__property int Space = {read=FSpace, write=SetSpace, nodefault};
	__property int HorzSpace = {read=FHorzSpace, write=SetHorzSpace, nodefault};
	__property int VertSpace = {read=FVertSpace, write=SetVertSpace, nodefault};
	__property Color ;
	__property Graphics::TColor OnColor = {read=FOnColor, write=SetOnColor, nodefault};
	__property Graphics::TColor OffColor = {read=FOffColor, write=SetOffColor, nodefault};
	__property bool DPRight = {read=FdpRight, write=SetDP, nodefault};
	__property bool DPOn = {read=FdpOn, write=SetDPOn, nodefault};
	__property bool Seg14 = {read=Fseg14, write=Set14, nodefault};
	__property bool Programmable = {read=FProgrammable, write=SetProgrammable, nodefault};
	__property int Mask0 = {read=GetMasks, write=SetMasks, index=0, nodefault};
	__property int Mask1 = {read=GetMasks, write=SetMasks, index=1, nodefault};
	__property int Mask2 = {read=GetMasks, write=SetMasks, index=2, nodefault};
	__property int Mask3 = {read=GetMasks, write=SetMasks, index=3, nodefault};
	__property int Mask4 = {read=GetMasks, write=SetMasks, index=4, nodefault};
	__property int Mask5 = {read=GetMasks, write=SetMasks, index=5, nodefault};
	__property int Mask6 = {read=GetMasks, write=SetMasks, index=6, nodefault};
	__property int Mask7 = {read=GetMasks, write=SetMasks, index=7, nodefault};
	__property int Mask8 = {read=GetMasks, write=SetMasks, index=8, nodefault};
	__property int Mask9 = {read=GetMasks, write=SetMasks, index=9, nodefault};
	__property int Mask10 = {read=GetMasks, write=SetMasks, index=10, nodefault};
	__property int Mask11 = {read=GetMasks, write=SetMasks, index=11, nodefault};
	__property int Mask12 = {read=GetMasks, write=SetMasks, index=12, nodefault};
	__property int Mask13 = {read=GetMasks, write=SetMasks, index=13, nodefault};
	__property int Mask14 = {read=GetMasks, write=SetMasks, index=14, nodefault};
	__property int Mask15 = {read=GetMasks, write=SetMasks, index=15, nodefault};
	__property Align ;
	__property Cursor ;
	__property DragCursor ;
	__property DragMode ;
	__property Hint ;
	__property ParentShowHint ;
	__property PopupMenu ;
	__property ShowHint ;
	__property Visible ;
	__property OnClick ;
	__property OnDblClick ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnStartDrag ;
public:
	#pragma option push -w-inl
	/* TGraphicControl.Destroy */ inline __fastcall virtual ~TCustomSevenSegment(void) { }
	#pragma option pop
	
};


class DELPHICLASS TSevenSegment;
class PASCALIMPLEMENTATION TSevenSegment : public TCustomSevenSegment 
{
	typedef TCustomSevenSegment inherited;
	
protected:
	virtual void __fastcall Paint(void);
	
public:
	__fastcall virtual TSevenSegment(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TGraphicControl.Destroy */ inline __fastcall virtual ~TSevenSegment(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall Register(void);

}	/* namespace Seven */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Seven;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Seven
