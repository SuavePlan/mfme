// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_System.pas' rev: 5.00

#ifndef GR32_SystemHPP
#define GR32_SystemHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Gr32_system
{
//-- type declarations -------------------------------------------------------
typedef unsigned HIMAGELIST;

typedef unsigned HTHEME;

struct _MARGINS
{
	int cxLeftWidth;
	int cxRightWidth;
	int cyTopHeight;
	int cyBottomHeight;
} ;

typedef _MARGINS  MARGINS;

typedef _MARGINS *PMARGINS;

typedef _MARGINS  TMargins;

//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool USE_THEMES;
extern PACKAGE unsigned SCROLLBAR_THEME;
extern PACKAGE unsigned GLOBALS_THEME;
static const Shortint THEMEMGR_VERSION = 0x1;
static const Word WM_THEMECHANGED = 0x31a;
static const Shortint SBP_ARROWBTN = 0x1;
static const Shortint SBP_THUMBBTNHORZ = 0x2;
static const Shortint SBP_THUMBBTNVERT = 0x3;
static const Shortint SBP_LOWERTRACKHORZ = 0x4;
static const Shortint SBP_UPPERTRACKHORZ = 0x5;
static const Shortint SBP_LOWERTRACKVERT = 0x6;
static const Shortint SBP_UPPERTRACKVERT = 0x7;
static const Shortint SBP_GRIPPERHORZ = 0x8;
static const Shortint SBP_GRIPPERVERT = 0x9;
static const Shortint SBP_SIZEBOX = 0xa;
static const Shortint ABS_UPNORMAL = 0x1;
static const Shortint ABS_UPHOT = 0x2;
static const Shortint ABS_UPPRESSED = 0x3;
static const Shortint ABS_UPDISABLED = 0x4;
static const Shortint ABS_DOWNNORMAL = 0x5;
static const Shortint ABS_DOWNHOT = 0x6;
static const Shortint ABS_DOWNPRESSED = 0x7;
static const Shortint ABS_DOWNDISABLED = 0x8;
static const Shortint ABS_LEFTNORMAL = 0x9;
static const Shortint ABS_LEFTHOT = 0xa;
static const Shortint ABS_LEFTPRESSED = 0xb;
static const Shortint ABS_LEFTDISABLED = 0xc;
static const Shortint ABS_RIGHTNORMAL = 0xd;
static const Shortint ABS_RIGHTHOT = 0xe;
static const Shortint ABS_RIGHTPRESSED = 0xf;
static const Shortint ABS_RIGHTDISABLED = 0x10;
static const Shortint SCRBS_NORMAL = 0x1;
static const Shortint SCRBS_HOT = 0x2;
static const Shortint SCRBS_PRESSED = 0x3;
static const Shortint SCRBS_DISABLED = 0x4;
static const Shortint SZB_RIGHTALIGN = 0x1;
static const Shortint SZB_LEFTALIGN = 0x2;
extern PACKAGE unsigned __stdcall (*OpenThemeData)(HWND hwnd, wchar_t * pszClassList);
extern PACKAGE HRESULT __stdcall (*CloseThemeData)(unsigned hTheme);
extern PACKAGE HRESULT __stdcall (*DrawThemeBackground)(unsigned hTheme, HDC hdc, int iPartId, int iStateId
	, const Windows::TRect &Rect, Windows::PRect pClipRect);
extern PACKAGE HRESULT __stdcall (*DrawThemeEdge)(unsigned hTheme, HDC hdc, int iPartId, int iStateId
	, const Windows::TRect &pDestRect, unsigned uEdge, unsigned uFlags, Windows::PRect pContentRect);
extern PACKAGE HRESULT __stdcall (*GetThemeColor)(unsigned hTheme, int iPartId, int iStateId, int iPropId
	, unsigned &pColor);
extern PACKAGE HRESULT __stdcall (*GetThemeMetric)(unsigned hTheme, HDC hdc, int iPartId, int iStateId
	, int iPropId, int &piVal);
extern PACKAGE HRESULT __stdcall (*GetThemeMargins)(unsigned hTheme, HDC hdc, int iPartId, int iStateId
	, int iPropId, Windows::PRect prc, _MARGINS &pMargins);
extern PACKAGE HRESULT __stdcall (*SetWindowTheme)(HWND hwnd, wchar_t * pszSubAppName, wchar_t * pszSubIdList
	);
extern PACKAGE BOOL __stdcall (*IsThemeActive)(void);
extern PACKAGE BOOL __stdcall (*IsAppThemed)(void);
extern PACKAGE HRESULT __stdcall (*EnableTheming)(BOOL fEnable);
extern PACKAGE bool __fastcall HasMMX(void);

}	/* namespace Gr32_system */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_system;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_System
