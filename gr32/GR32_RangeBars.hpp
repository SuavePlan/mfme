// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_RangeBars.pas' rev: 5.00

#ifndef GR32_RangeBarsHPP
#define GR32_RangeBarsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ExtCtrls.hpp>	// Pascal unit
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

namespace Gr32_rangebars
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TRBDirection { drLeft, drUp, drRight, drDown };
#pragma option pop

typedef Set<TRBDirection, drLeft, drDown>  TRBDirections;

#pragma option push -b-
enum TRBZone { zNone, zBtnPrev, zTrackPrev, zHandle, zTrackNext, zBtnNext };
#pragma option pop

#pragma option push -b-
enum TRBStyle { rbsDefault, rbsMac };
#pragma option pop

#pragma option push -b-
enum TRBBackgnd { bgPattern, bgSolid };
#pragma option pop

typedef void __fastcall (__closure *TRBGetSizeEvent)(System::TObject* Sender, int &Size);

class DELPHICLASS TArrowBar;
class PASCALIMPLEMENTATION TArrowBar : public Controls::TCustomControl 
{
	typedef Controls::TCustomControl inherited;
	
private:
	TRBBackgnd FBackgnd;
	Forms::TFormBorderStyle FBorderStyle;
	int FButtonSize;
	Graphics::TColor FHandleColor;
	Forms::TScrollBarKind FKind;
	bool FShowArrows;
	bool FShowHandleGrip;
	TRBStyle FStyle;
	Classes::TNotifyEvent FOnChange;
	Classes::TNotifyEvent FOnUserChange;
	void __fastcall SetButtonSize(int Value);
	void __fastcall SetBorderStyle(Forms::TBorderStyle Value);
	void __fastcall SetHandleColor(Graphics::TColor Value);
	void __fastcall SetKind(Forms::TScrollBarKind Value);
	void __fastcall SetShowArrows(bool Value);
	void __fastcall SetShowHandleGrip(bool Value);
	void __fastcall SetStyle(TRBStyle Value);
	void __fastcall SetBackgnd(TRBBackgnd Value);
	HIDESBASE MESSAGE void __fastcall CMEnabledChanged(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall CMMouseLeave(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMNCCalcSize(Messages::TWMNCCalcSize &Message);
	HIDESBASE MESSAGE void __fastcall WMNCPaint(Messages::TMessage &Message);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TWMEraseBkgnd &Message);
	
protected:
	bool GenChange;
	TRBZone DragZone;
	TRBZone HotZone;
	Extctrls::TTimer* Timer;
	int TimerMode;
	int StoredX;
	int StoredY;
	float PosBeforeDrag;
	virtual void __fastcall DoChange(void);
	virtual void __fastcall DoDrawButton(const Windows::TRect &R, TRBDirection Direction, bool Pushed, 
		bool Enabled, bool Hot);
	virtual void __fastcall DoDrawHandle(const Windows::TRect &R, bool Horz, bool Pushed, bool Hot);
	virtual void __fastcall DoDrawTrack(const Windows::TRect &R, TRBDirection Direction, bool Pushed, bool 
		Enabled, bool Hot);
	DYNAMIC void __fastcall DrawNCArea(HDC ADC, const HRGN Clip);
	virtual bool __fastcall DrawEnabled(void);
	int __fastcall GetBorderSize(void);
	virtual Windows::TRect __fastcall GetHandleRect(void);
	int __fastcall GetButtonSize(void);
	Windows::TRect __fastcall GetTrackBoundary(void);
	TRBZone __fastcall GetZone(int X, int Y);
	Windows::TRect __fastcall GetZoneRect(TRBZone Zone);
	virtual void __fastcall MouseLeft(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, 
		int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	DYNAMIC void __fastcall MouseUp(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, int 
		Y);
	virtual void __fastcall Paint(void);
	void __fastcall StartDragTracking(void);
	void __fastcall StartHotTracking(void);
	void __fastcall StopDragTracking(void);
	void __fastcall StopHotTracking(void);
	virtual void __fastcall TimerHandler(System::TObject* Sender);
	
public:
	__fastcall virtual TArrowBar(Classes::TComponent* AOwner);
	__property Color ;
	__property TRBBackgnd Backgnd = {read=FBackgnd, write=SetBackgnd, nodefault};
	__property Forms::TBorderStyle BorderStyle = {read=FBorderStyle, write=SetBorderStyle, default=1};
	__property int ButtonSize = {read=FButtonSize, write=SetButtonSize, default=0};
	__property Graphics::TColor HandleColor = {read=FHandleColor, write=SetHandleColor, default=-2147483632
		};
	__property Forms::TScrollBarKind Kind = {read=FKind, write=SetKind, default=0};
	__property bool ShowArrows = {read=FShowArrows, write=SetShowArrows, default=1};
	__property bool ShowHandleGrip = {read=FShowHandleGrip, write=SetShowHandleGrip, nodefault};
	__property TRBStyle Style = {read=FStyle, write=SetStyle, default=0};
	__property Classes::TNotifyEvent OnChange = {read=FOnChange, write=FOnChange};
	__property Classes::TNotifyEvent OnUserChange = {read=FOnUserChange, write=FOnUserChange};
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TArrowBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TArrowBar(HWND ParentWindow) : Controls::TCustomControl(
		ParentWindow) { }
	#pragma option pop
	
};


typedef Word TRBIncrement;

class DELPHICLASS TCustomRangeBar;
class PASCALIMPLEMENTATION TCustomRangeBar : public TArrowBar 
{
	typedef TArrowBar inherited;
	
private:
	bool FCentered;
	int FEffectiveWindow;
	TRBIncrement FIncrement;
	float FPosition;
	int FRange;
	int FWindow;
	bool __fastcall IsPositionStored(void);
	void __fastcall SetPosition(float Value);
	void __fastcall SetRange(int Value);
	void __fastcall SetWindow(int Value);
	
protected:
	void __fastcall AdjustPosition(void);
	DYNAMIC bool __fastcall DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Windows::TPoint 
		&MousePos);
	virtual bool __fastcall DrawEnabled(void);
	virtual Windows::TRect __fastcall GetHandleRect(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, 
		int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall TimerHandler(System::TObject* Sender);
	void __fastcall UpdateEffectiveWindow(void);
	__property int EffectiveWindow = {read=FEffectiveWindow, nodefault};
	
public:
	__fastcall virtual TCustomRangeBar(Classes::TComponent* AOwner);
	DYNAMIC void __fastcall Resize(void);
	void __fastcall SetParams(int NewRange, int NewWindow);
	__property bool Centered = {read=FCentered, write=FCentered, nodefault};
	__property TRBIncrement Increment = {read=FIncrement, write=FIncrement, default=8};
	__property float Position = {read=FPosition, write=SetPosition, stored=IsPositionStored};
	__property int Range = {read=FRange, write=SetRange, default=0};
	__property int Window = {read=FWindow, write=SetWindow, default=0};
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TCustomRangeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomRangeBar(HWND ParentWindow) : TArrowBar(ParentWindow
		) { }
	#pragma option pop
	
};


class DELPHICLASS TRangeBar;
class PASCALIMPLEMENTATION TRangeBar : public TCustomRangeBar 
{
	typedef TCustomRangeBar inherited;
	
__published:
	__property Align ;
	__property Anchors ;
	__property Constraints ;
	__property Color ;
	__property Backgnd ;
	__property BorderStyle ;
	__property ButtonSize ;
	__property Enabled ;
	__property HandleColor ;
	__property Increment ;
	__property Kind ;
	__property Range ;
	__property Style ;
	__property Visible ;
	__property Window ;
	__property ShowArrows ;
	__property ShowHandleGrip ;
	__property Position ;
	__property OnChange ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnMouseWheelUp ;
	__property OnMouseWheelDown ;
	__property OnStartDrag ;
	__property OnUserChange ;
public:
	#pragma option push -w-inl
	/* TCustomRangeBar.Create */ inline __fastcall virtual TRangeBar(Classes::TComponent* AOwner) : TCustomRangeBar(
		AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TRangeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TRangeBar(HWND ParentWindow) : TCustomRangeBar(ParentWindow
		) { }
	#pragma option pop
	
};


class DELPHICLASS TCustomGaugeBar;
class PASCALIMPLEMENTATION TCustomGaugeBar : public TArrowBar 
{
	typedef TArrowBar inherited;
	
private:
	int FHandleSize;
	int FLargeChange;
	int FMax;
	int FMin;
	int FPosition;
	int FSmallChange;
	void __fastcall SetHandleSize(int Value);
	void __fastcall SetMax(int Value);
	void __fastcall SetMin(int Value);
	void __fastcall SetPosition(int Value);
	void __fastcall SetLargeChange(int Value);
	void __fastcall SetSmallChange(int Value);
	
protected:
	void __fastcall AdjustPosition(void);
	DYNAMIC bool __fastcall DoMouseWheel(Classes::TShiftState Shift, int WheelDelta, const Windows::TPoint 
		&MousePos);
	virtual Windows::TRect __fastcall GetHandleRect(void);
	int __fastcall GetHandleSize(void);
	DYNAMIC void __fastcall MouseDown(Controls::TMouseButton Button, Classes::TShiftState Shift, int X, 
		int Y);
	DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);
	virtual void __fastcall TimerHandler(System::TObject* Sender);
	
public:
	__fastcall virtual TCustomGaugeBar(Classes::TComponent* AOwner);
	__property int HandleSize = {read=FHandleSize, write=SetHandleSize, default=0};
	__property int LargeChange = {read=FLargeChange, write=SetLargeChange, default=1};
	__property int Max = {read=FMax, write=SetMax, default=100};
	__property int Min = {read=FMin, write=SetMin, default=0};
	__property int Position = {read=FPosition, write=SetPosition, nodefault};
	__property int SmallChange = {read=FSmallChange, write=SetSmallChange, default=1};
	__property OnChange ;
	__property OnUserChange ;
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TCustomGaugeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TCustomGaugeBar(HWND ParentWindow) : TArrowBar(ParentWindow
		) { }
	#pragma option pop
	
};


class DELPHICLASS TGaugeBar;
class PASCALIMPLEMENTATION TGaugeBar : public TCustomGaugeBar 
{
	typedef TCustomGaugeBar inherited;
	
__published:
	__property Align ;
	__property Anchors ;
	__property Constraints ;
	__property Color ;
	__property Backgnd ;
	__property BorderStyle ;
	__property ButtonSize ;
	__property Enabled ;
	__property HandleColor ;
	__property HandleSize ;
	__property Kind ;
	__property LargeChange ;
	__property Max ;
	__property Min ;
	__property ShowArrows ;
	__property ShowHandleGrip ;
	__property Style ;
	__property SmallChange ;
	__property Visible ;
	__property Position ;
	__property OnChange ;
	__property OnDragDrop ;
	__property OnDragOver ;
	__property OnEndDrag ;
	__property OnMouseDown ;
	__property OnMouseMove ;
	__property OnMouseUp ;
	__property OnStartDrag ;
	__property OnUserChange ;
public:
	#pragma option push -w-inl
	/* TCustomGaugeBar.Create */ inline __fastcall virtual TGaugeBar(Classes::TComponent* AOwner) : TCustomGaugeBar(
		AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TCustomControl.Destroy */ inline __fastcall virtual ~TGaugeBar(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TGaugeBar(HWND ParentWindow) : TCustomGaugeBar(ParentWindow
		) { }
	#pragma option pop
	
};


class DELPHICLASS TArrowBarAccess;
class PASCALIMPLEMENTATION TArrowBarAccess : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
private:
	TArrowBar* FMaster;
	TArrowBar* FSlave;
	TRBBackgnd __fastcall GetBackgnd(void);
	int __fastcall GetButtonSize(void);
	Graphics::TColor __fastcall GetColor(void);
	Graphics::TColor __fastcall GetHandleColor(void);
	bool __fastcall GetShowArrows(void);
	bool __fastcall GetShowHandleGrip(void);
	TRBStyle __fastcall GetStyle(void);
	void __fastcall SetBackgnd(TRBBackgnd Value);
	void __fastcall SetButtonSize(int Value);
	void __fastcall SetColor(Graphics::TColor Value);
	void __fastcall SetHandleColor(Graphics::TColor Value);
	void __fastcall SetShowArrows(bool Value);
	void __fastcall SetShowHandleGrip(bool Value);
	void __fastcall SetStyle(TRBStyle Value);
	
public:
	__property TArrowBar* Master = {read=FMaster, write=FMaster};
	__property TArrowBar* Slave = {read=FSlave, write=FSlave};
	
__published:
	__property Graphics::TColor Color = {read=GetColor, write=SetColor, nodefault};
	__property TRBBackgnd Backgnd = {read=GetBackgnd, write=SetBackgnd, default=0};
	__property int ButtonSize = {read=GetButtonSize, write=SetButtonSize, default=0};
	__property Graphics::TColor HandleColor = {read=GetHandleColor, write=SetHandleColor, default=-2147483632
		};
	__property bool ShowArrows = {read=GetShowArrows, write=SetShowArrows, default=1};
	__property bool ShowHandleGrip = {read=GetShowHandleGrip, write=SetShowHandleGrip, nodefault};
	__property TRBStyle Style = {read=GetStyle, write=SetStyle, nodefault};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TArrowBarAccess(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TArrowBarAccess(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_rangebars */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_rangebars;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_RangeBars
