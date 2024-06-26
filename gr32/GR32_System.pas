unit GR32_System;

{*********************************************}
{  This unit is a part of Graphics32 library  }
{  Copyright � 2000-2002 Alex Denisov         }
{  See License.txt for licence information    }
{*********************************************}
// $Id: GR32_System.pas,v 1.2 2002/02/12 04:22:50 alex Exp $

interface

{$I GR32.INC}

uses
  Windows;

{ HasMMX returns 'true' if CPU supports MMX instructions }
function HasMMX: Boolean;

{ Internal support for Windows XP themes }
var
  USE_THEMES: Boolean = False;
  SCROLLBAR_THEME: THandle = 0;
  GLOBALS_THEME: THandle = 0;

const
  THEMEMGR_VERSION                     = 1;
  WM_THEMECHANGED                      = $031A;

{ "Scrollbar" Parts & States }
  { SCROLLBARPARTS }
  SBP_ARROWBTN                         = 1;
  SBP_THUMBBTNHORZ                     = 2;
  SBP_THUMBBTNVERT                     = 3;
  SBP_LOWERTRACKHORZ                   = 4;
  SBP_UPPERTRACKHORZ                   = 5;
  SBP_LOWERTRACKVERT                   = 6;
  SBP_UPPERTRACKVERT                   = 7;
  SBP_GRIPPERHORZ                      = 8;
  SBP_GRIPPERVERT                      = 9;
  SBP_SIZEBOX                          = 10;

  { ARROWBTNSTATES }
  ABS_UPNORMAL                         = 1;
  ABS_UPHOT                            = 2;
  ABS_UPPRESSED                        = 3;
  ABS_UPDISABLED                       = 4;
  ABS_DOWNNORMAL                       = 5;
  ABS_DOWNHOT                          = 6;
  ABS_DOWNPRESSED                      = 7;
  ABS_DOWNDISABLED                     = 8;
  ABS_LEFTNORMAL                       = 9;
  ABS_LEFTHOT                          = 10;
  ABS_LEFTPRESSED                      = 11;
  ABS_LEFTDISABLED                     = 12;
  ABS_RIGHTNORMAL                      = 13;
  ABS_RIGHTHOT                         = 14;
  ABS_RIGHTPRESSED                     = 15;
  ABS_RIGHTDISABLED                    = 16;

  { SCROLLBARSTATES }
  SCRBS_NORMAL                         = 1;
  SCRBS_HOT                            = 2;
  SCRBS_PRESSED                        = 3;
  SCRBS_DISABLED                       = 4;

  { SIZEBOXSTATES }
  SZB_RIGHTALIGN                       = 1;
  SZB_LEFTALIGN                        = 2;

{ Access to uxtheme.dll }

type
  HIMAGELIST = THandle;
  HTHEME = THandle;
  _MARGINS = record
    cxLeftWidth: Integer;      // width of left border that retains its size
    cxRightWidth: Integer;     // width of right border that retains its size
    cyTopHeight: Integer;      // height of top border that retains its size
    cyBottomHeight: Integer;   // height of bottom border that retains its size
  end;
  MARGINS = _MARGINS;
  PMARGINS = ^MARGINS;
  TMargins = MARGINS;

var
  OpenThemeData: function(hwnd: HWND; pszClassList: LPCWSTR): HTHEME; stdcall;
  CloseThemeData: function(hTheme: HTHEME): HRESULT; stdcall;
  DrawThemeBackground: function(hTheme: HTHEME; hdc: HDC; iPartId, iStateId: Integer;
    const Rect: TRect; pClipRect: PRect): HRESULT; stdcall;
  DrawThemeEdge: function(hTheme: HTHEME; hdc: HDC; iPartId, iStateId: Integer; const pDestRect: TRect; uEdge,
    uFlags: UINT; pContentRect: PRECT): HRESULT; stdcall;
  GetThemeColor: function(hTheme: HTHEME; iPartId, iStateId, iPropId: Integer; var pColor: COLORREF): HRESULT; stdcall;
  GetThemeMetric: function(hTheme: HTHEME; hdc: HDC; iPartId, iStateId, iPropId: Integer;
    var piVal: Integer): HRESULT; stdcall;
  GetThemeMargins: function(hTheme: HTHEME; hdc: HDC; iPartId, iStateId, iPropId: Integer; prc: PRECT;
    var pMargins: MARGINS): HRESULT; stdcall;
  SetWindowTheme: function(hwnd: HWND; pszSubAppName: LPCWSTR; pszSubIdList: LPCWSTR): HRESULT; stdcall;
  IsThemeActive: function: BOOL; stdcall;
  IsAppThemed: function: BOOL; stdcall;
  EnableTheming: function(fEnable: BOOL): HRESULT; stdcall;

implementation

uses
  Messages, Classes, Forms;

function CPUID_Available: Boolean;
asm
        MOV       EDX,False
        PUSHFD
        POP       EAX
        MOV       ECX,EAX
        XOR       EAX,$00200000
        PUSH      EAX
        POPFD
        PUSHFD
        POP       EAX
        XOR       ECX,EAX
        JZ        @1
        MOV       EDX,True
@1:     PUSH      EAX
        POPFD
        MOV       EAX,EDX
end;

function CPU_Signature: Integer;
asm
        PUSH    EBX
        MOV     EAX,1
        DW      $A20F   // CPUID
        POP     EBX
end;

function CPU_Features: Integer;
asm
        PUSH    EBX
        MOV     EAX,1
        DW      $A20F   // CPUID
        POP     EBX
        MOV     EAX,EDX
end;

function HasMMX: Boolean;
begin
  Result := False;
  if not CPUID_Available then Exit;              // no CPUID available
  if CPU_Signature shr 8 and $0F < 5 then Exit;  // not a Pentium class
  if CPU_Features and $800000 = 0 then Exit;     // no MMX
  Result := True;
end;
 
const
  UXTHEME_DLL = 'uxtheme.dll';

var
  DllHandle: THandle;

procedure FreeXPThemes;
begin
  if DllHandle <> 0 then
  begin
    FreeLibrary(DllHandle);
    DllHandle := 0;
    OpenThemeData := nil;
    CloseThemeData := nil;
    DrawThemeBackground := nil;
    DrawThemeEdge := nil;
    GetThemeColor := nil;
    GetThemeMetric := nil;
    GetThemeMargins := nil;
    SetWindowTheme := nil;
    IsThemeActive := nil;
    IsAppThemed := nil;
    EnableTheming := nil;
  end;
end;

function InitXPThemes: Boolean;
begin
  if DllHandle = 0 then
  begin
    DllHandle := LoadLibrary(UXTHEME_DLL);
    if DllHandle > 0 then
    begin
      OpenThemeData := GetProcAddress(DllHandle, 'OpenThemeData');
      CloseThemeData := GetProcAddress(DllHandle, 'CloseThemeData');
      DrawThemeBackground := GetProcAddress(DllHandle, 'DrawThemeBackground');
      DrawThemeEdge := GetProcAddress(DllHandle, 'DrawThemeEdge');
      GetThemeColor := GetProcAddress(DllHandle, 'GetThemeColor');
      GetThemeMetric := GetProcAddress(DllHandle, 'GetThemeMetric');
      GetThemeMargins := GetProcAddress(DllHandle, 'GetThemeMargins');
      SetWindowTheme := GetProcAddress(DllHandle, 'SetWindowTheme');
      IsThemeActive := GetProcAddress(DllHandle, 'IsThemeActive');
      IsAppThemed := GetProcAddress(DllHandle, 'IsAppThemed');
      EnableTheming := GetProcAddress(DllHandle, 'EnableTheming');
      if (@OpenThemeData = nil) or (@CloseThemeData = nil) or (@IsThemeActive = nil) or
        (@IsAppThemed = nil) or (@EnableTheming = nil) then FreeXPThemes;
    end;
  end;
  Result := DllHandle > 0;
end;

function UseXPThemes: Boolean;
begin
  Result := (DllHandle > 0) and IsAppThemed  and IsThemeActive;
end;

type
  TThemeNexus = class
  private
    FWindowHandle: HWND;
  protected
    procedure WndProc(var Message: TMessage);
    procedure OpenVisualStyles;
    procedure CloseVisualStyles;
  public
    constructor Create;
    destructor Destroy; override;
  end;

{$IFDEF XPTHEMES}
var
  ThemeNexus: TThemeNexus;
{$ENDIF}

{ TThemeNexus }

procedure TThemeNexus.CloseVisualStyles;
begin
  if UseXPThemes then
  begin
    if SCROLLBAR_THEME <> 0 then
    begin
      CloseThemeData(SCROLLBAR_THEME);
      SCROLLBAR_THEME := 0;
    end;
    if GLOBALS_THEME <> 0 then
    begin
      CloseThemeData(GLOBALS_THEME);
      GLOBALS_THEME := 0;
    end;
  end;
  FreeXPThemes;
end;

constructor TThemeNexus.Create;
begin
  FWindowHandle := {$IFDEF VER140}Classes.{$ENDIF}AllocateHWnd(WndProc);
  OpenVisualStyles;
end;

destructor TThemeNexus.Destroy;
begin
  CloseVisualStyles;
  {$IFDEF VER140}Classes.{$ENDIF}DeallocateHWnd(FWindowHandle);
  inherited;
end;

procedure TThemeNexus.OpenVisualStyles;
begin
  USE_THEMES := False;
  if InitXPThemes then
  begin
    USE_THEMES := UseXPThemes;
    if USE_THEMES then
    begin
      SCROLLBAR_THEME := OpenThemeData(FWindowHandle, 'SCROLLBAR');
      GLOBALS_THEME := OpenThemeData(FWindowHandle, 'GLOBALS');
    end;
  end;
end;

procedure TThemeNexus.WndProc(var Message: TMessage);
begin
  case Message.Msg of
    WM_THEMECHANGED:
      begin
        CloseVisualStyles;
        OpenVisualStyles;
      end;
  end;
  with Message do Result := DefWindowProc(FWindowHandle, Msg, wParam, lParam);
end;

initialization
{$IFDEF XPTHEMES}
ThemeNexus := TThemeNexus.Create;
{$ENDIF}

finalization
{$IFDEF XPTHEMES}
ThemeNexus.Free;
{$ENDIF}

end.
