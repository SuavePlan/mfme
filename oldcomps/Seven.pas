{ -----------------------------------------------------
  seven.pas

  Copyright (c) 1998, 1999, Antony Pranata
  (antonypr@poboxes.com)
  (http://www.antonypr.pair.com)

  TSevenSegment Component v2.0,
  used to draw "seven segment display" on a form

  More Delphi components? Please visit:
    http://www.antonypr.pair.com/delphi.html
  ----------------------------------------------------- }
unit Seven;

interface

uses
  WinProcs, WinTypes, Messages, SysUtils, Classes,
  Graphics, Controls, Forms, Dialogs, Menus;

{     0             0   1
    ------         --- ---
   |      |       | \ | / |
 5 |  6   |  1  7 |  \|/  | 2
    ------         --- ---
   |      |     6 |  /|\  | 3
 4 |      |  2    | / | \ |
    ------     . 7 --- ---
      3             5    4
}

type

  { -----------------------------------------------------
    TCustomSevenSegment
    base class for all classes of seven segment display
    ----------------------------------------------------- }
  TCustomSevenSegment = class(TGraphicControl)
  private
    FValue: integer;  { the value }
    {$ifndef WIN32}
    FPopupMenu: TPopupMenu;
    {$endif}

    FThickness: integer; { thickness of a segment }
    FSpace: integer; { space between segment }
    FHorzSpace: integer; { distance from segment to frame in
                           horizontal direction }
    FVertSpace: integer; { distance from segment to frame in
                           vertical direction }
    FOnColor: TColor;
    FOffColor: TColor;
    FdpRight: Boolean;
    FdpOn: Boolean;
    Fseg14: Boolean;
    FProgrammable: Boolean;
    FMasks : array [0..15] of integer;

    procedure SetValue (x: integer);
    procedure SetThickness (x: integer);
    procedure SetSpace (x: integer);
    procedure SetHorzSpace (x: integer);
    procedure SetVertSpace (x: integer);
    procedure SetOnColor (x: TColor);
    procedure SetOffColor (x: TColor);
    procedure SetDP (x: Boolean);
    procedure SetDPOn (x: Boolean);
    procedure Set14 (x: Boolean);
    procedure SetProgrammable (x: Boolean);
    procedure DrawSegment (Digit: integer; bitmap: TBitmap; rect: TRect);
  protected
    procedure IncRect (var MyRect: TRect; Delta: integer);

    procedure CMVisibleChanged (var msg: TMessage);
      message CM_VISIBLECHANGED;

    {$ifndef WIN32}
    procedure WMRButtonDown (var msg: TWMMouse); message WM_RBUTTONDOWN;
    {$endif}
  public
    procedure SetMasks (index: integer; mask : integer );
    function  GetMasks( index: integer) : integer;
    constructor Create (AOwner: TComponent); override;
  published
    property Value: integer read FValue write SetValue;
    {$ifndef WIN32}
    property PopupMenu: TPopupMenu read FPopupMenu write FPopupMenu;
    {$endif}
    property Thickness: integer read FThickness write SetThickness;
    property Space: integer read FSpace write SetSpace;
    property HorzSpace: integer read FHorzSpace write SetHorzSpace;
    property VertSpace: integer read FVertSpace write SetVertSpace;
    property Color;
    property OnColor: TColor read FOnColor write SetOnColor;
    property OffColor: TColor read FOffColor write SetOffColor;
    property DPRight: Boolean read FdpRight write SetDP;
    property DPOn: Boolean read FDPOn write SetDPOn;
    property Seg14: Boolean read Fseg14 write Set14;
    property Programmable: Boolean read FProgrammable write SetProgrammable;
    property Mask0: integer index 0 read GetMasks write SetMasks;
    property Mask1: integer index 1 read GetMasks write SetMasks;
    property Mask2: integer index 2 read GetMasks write SetMasks;
    property Mask3: integer index 3 read GetMasks write SetMasks;
    property Mask4: integer index 4 read GetMasks write SetMasks;
    property Mask5: integer index 5 read GetMasks write SetMasks;
    property Mask6: integer index 6 read GetMasks write SetMasks;
    property Mask7: integer index 7 read GetMasks write SetMasks;
    property Mask8: integer index 8 read GetMasks write SetMasks;
    property Mask9: integer index 9 read GetMasks write SetMasks;
    property Mask10: integer index 10 read GetMasks write SetMasks;
    property Mask11: integer index 11 read GetMasks write SetMasks;
    property Mask12: integer index 12 read GetMasks write SetMasks;
    property Mask13: integer index 13 read GetMasks write SetMasks;
    property Mask14: integer index 14 read GetMasks write SetMasks;
    property Mask15: integer index 15 read GetMasks write SetMasks;

    property Align;
    property Cursor;
    property DragCursor;
    property DragMode;
    property Hint;
    property ParentShowHint;
    {$ifdef WIN32}
    property PopupMenu;
    {$endif}
    property ShowHint;
    property Visible;

    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnEndDrag;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;
    {$ifdef WIN32}
    property OnStartDrag;
    {$endif}
  end;

  { -----------------------------------------------------
    TSevenSegment
    used to draw the "classic" seven segment display
    ----------------------------------------------------- }
  TSevenSegment = class(TCustomSevenSegment)
  private
  protected
    procedure Paint; override;
  public
    constructor Create (AOwner: TComponent); override;
  published
  end;


procedure Register;

implementation

{ -----------------------------------------------------
  Methods definition of TCustomSevenSegment
  ----------------------------------------------------- }
constructor TCustomSevenSegment.Create (AOwner: TComponent);
var
  i       : byte;
begin
  inherited Create (AOwner);

  {$ifdef WIN32}
  ControlStyle := ControlStyle + [csOpaque, csReplicatable];
  {$else}
  ControlStyle := ControlStyle + [csOpaque];
  {$endif}

  Width         := 200;
  Height        := 50;
  Space         := 2;
  HorzSpace     := 20;
  VertSpace     := 6;
  Value         := 0;
  Thickness     := 5;
  Color         := clBlack;
  OnColor       := clRed;
  OffColor      := RGB (64, 64, 64);
  FdpRight      := true;
  FdpOn         := false;
  for i := 0 to 15 do
    FMasks[i] := 256;
end;

procedure TCustomSevenSegment.SetValue (x: integer);
begin
  FValue := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetSpace (x: integer);
begin
  FSpace := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetHorzSpace (x: integer);
begin
  FHorzSpace := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetVertSpace (x: integer);
begin
  FVertSpace := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetThickness (x: integer);
begin
  FThickness := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetOnColor (x: TColor);
begin
  FOnColor := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetOffColor (x: TColor);
begin
  FOffColor := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetDP (x: Boolean);
begin
  FdpRight := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetDPOn (x: Boolean);
begin
  FdpOn := x;
  Invalidate;
end;

procedure TCustomSevenSegment.Set14 (x: Boolean);
begin
  Fseg14 := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetProgrammable (x: Boolean);
begin
  FProgrammable := x;
  Invalidate;
end;

procedure TCustomSevenSegment.SetMasks (index: integer ; mask: integer);
begin
  FMasks[index] := mask;
end;

function TCustomSevenSegment.GetMasks (index: integer) : integer;
begin
  Result := FMasks[index];
end;

procedure TCustomSevenSegment.DrawSegment (Digit: integer; bitmap: TBitmap;
                                           rect: TRect);
var
  i       : byte;
  CenterY : integer;
  CenterX : integer;
  Space2  : integer;
  Thick2  : integer;
  points4 : array[0..3] of TPoint;
  points6 : array[0..5] of TPoint;
begin
  CenterY := (rect.top + rect.bottom) shr 1;
  CenterX := (rect.left + rect.right) shr 1;
  Space2  := FSpace shr 1;
  Thick2  := FThickness shr 1;

  inc (rect.left, FHorzSpace shr 1);
  dec (rect.right, FHorzSpace shr 1);

  if FSeg14 = false then
  begin
  { Draw each segment }
    for i := 0 to 6 do
    begin
      if ((Digit shr i) and 1) = 1 then
        bitmap.Canvas.Brush.Color := FOnColor
      else
        bitmap.Canvas.Brush.Color := FOffColor;
      case (i) of
        { upper segment }
        0: begin
           points4[0] := Point (rect.left + FSpace, rect.top);
           points4[1] := Point (rect.left + FThickness + FSpace,
                         rect.top + FThickness);
           points4[2] := Point (rect.right - FThickness - FSpace,
                         rect.top + FThickness);
           points4[3] := Point (rect.right - FSpace, rect.top);
           bitmap.Canvas.Polygon (points4);
         end;

        { upper right segment }
        1: begin
           points6[0] := Point (rect.right, CenterY - Thick2 - FSpace);
           points6[1] := Point (rect.right - Thick2, CenterY - Space2);
           points6[2] := Point (rect.right - FThickness,
                         CenterY - Thick2 - FSpace{FThickness - Space2});
           points6[3] := Point (rect.right - FThickness,
                         rect.top + FThickness + FSpace);
           points6[4] := Point (rect.right, rect.top + FSpace);
           bitmap.Canvas.Polygon (slice(points6,5));
         end;

        { lower right segment }
        2: begin
           points6[0] := Point (rect.right, rect.bottom - FSpace);
           points6[1] := Point (rect.right - FThickness,
                         rect.bottom - FThickness - FSpace);
           points6[2] := Point (rect.right - FThickness,
                         CenterY + Thick2 + FSpace{FThickness + Space2});
           points6[3] := Point (rect.right - Thick2, CenterY + Space2);
           points6[4] := Point (rect.right, CenterY + Thick2 + FSpace);
           bitmap.Canvas.Polygon (slice(points6,5));
         end;

        { lower segment }
        3: begin
           points4[0] := Point (rect.left + FSpace, rect.bottom);
           points4[1] := Point (rect.left + FThickness + FSpace,
                         rect.bottom - FThickness);
           points4[2] := Point (rect.right - FThickness - FSpace,
                         rect.bottom - FThickness);
           points4[3] := Point (rect.right - FSpace, rect.bottom);
           bitmap.Canvas.Polygon (points4);
         end;

        { lower left segment }
        4: begin
           points6[0] := Point (rect.left, CenterY + Thick2 + FSpace);
           points6[1] := Point (rect.left + Thick2, CenterY + Space2);
           points6[2] := Point (rect.left + FThickness,
                          CenterY + Thick2{FThickness} + FSpace{Space2});
           points6[3] := Point (rect.left + FThickness,
                          rect.bottom - FThickness - FSpace);
           points6[4] := Point (rect.left, rect.bottom - FSpace);
           bitmap.Canvas.Polygon (slice(points6,5));
         end;

      { upper left segment }
        5: begin
           points6[0] := Point (rect.left, rect.top + FSpace);
           points6[1] := Point (rect.left + FThickness,
                         rect.top + FThickness + FSpace);
           points6[2] := Point (rect.left + FThickness,
                         CenterY - Thick2{- FThickness} - FSpace{Space2});
           points6[3] := Point (rect.left + Thick2, CenterY - Space2);
           points6[4] := Point (rect.left, CenterY - Thick2 - FSpace);
           bitmap.Canvas.Polygon (slice(points6,5));
           end;
      { middle segment }
        6: begin
           points6[0] := Point (rect.left + FSpace + Thick2{ - FThickness shr 1}, CenterY);
           points6[1] := Point (rect.left + FSpace + FThickness{Thick2}, CenterY - Thick2);
           points6[2] := Point (rect.right - FSpace - FThickness{Thick2}, CenterY - Thick2);
           points6[3] := Point (rect.right - FSpace - Thick2 {+ FThickness shr 1}, CenterY);
           points6[4] := Point (rect.right - FSpace - FThickness{Thick2}, CenterY + Thick2);
           points6[5] := Point (rect.left + FSpace + Thickness{Thick2}, CenterY + Thick2);
           bitmap.Canvas.Polygon (points6);

           end;
      end;
{    bitmap.Canvas.Polygon (points4);    }
    end;

    if (((Digit shr 7) and 1) = 1) or (DPOn = true) then
      bitmap.Canvas.Brush.Color := FOnColor
    else
      bitmap.Canvas.Brush.Color := FOffColor;
    if FdpRight then
      bitmap.Canvas.Rectangle (
        rect.right + FThickness shr 1,
        rect.bottom - FThickness,
        rect.right + FThickness shr 1 + FThickness,
        rect.bottom
      )
    else
      bitmap.Canvas.Rectangle (
        rect.left - FThickness shr 1 - FThickness,
        rect.bottom - FThickness,
        rect.left - FThickness shr 1,
        rect.bottom
      );
    end
  else
    begin
      for i := 0 to 15 do
      begin
        if ((Digit shr i) and 1) = 1 then
          bitmap.Canvas.Brush.Color := FOnColor
        else
          bitmap.Canvas.Brush.Color := FOffColor;
        case (i) of
        { upper left segment }
          0: begin
           points6[0] := Point (rect.left + FSpace, rect.top);
           points6[1] := Point (rect.left + FThickness + FSpace,
                         rect.top + FThickness);
           points6[2] := Point (CenterX - Thick2 - FSpace,
                         rect.top + FThickness);
           points6[3] := Point (CenterX - Space2, rect.top + Thick2);
           points6[4] := Point (CenterX - Thick2 - FSpace, rect.top);
           bitmap.Canvas.Polygon (slice(points6,5));
           end;

        { upper right segment }
          1: begin
           points6[0] := Point (CenterX + Thick2 + FSpace, rect.top);
           points6[1] := Point (CenterX + Space2,
                         rect.top + Thick2);
           points6[2] := Point (CenterX + Thick2 + FSpace,
                         rect.top + Thickness);
           points6[3] := Point (rect.right - FThickness - FSpace,
                         rect.top + FThickness);
           points6[4] := Point (rect.right - FSpace, rect.top);
           bitmap.Canvas.Polygon (slice(points6,5));
           end;

        { upper right segment }
          2: begin
             points6[0] := Point (rect.right, CenterY - Thick2 - FSpace);
             points6[1] := Point (rect.right - Thick2, CenterY - Space2);
             points6[2] := Point (rect.right - FThickness,
                         CenterY - Thick2 - FSpace{FThickness - Space2});
             points6[3] := Point (rect.right - FThickness,
                         rect.top + FThickness + FSpace);
             points6[4] := Point (rect.right, rect.top + FSpace);
             bitmap.Canvas.Polygon (slice(points6,5));
           end;

        { lower right segment }
          3: begin
             points6[0] := Point (rect.right, rect.bottom - FSpace);
             points6[1] := Point (rect.right - FThickness,
                         rect.bottom - FThickness - FSpace);
             points6[2] := Point (rect.right - FThickness,
                         CenterY + Thick2 + FSpace{FThickness + Space2});
             points6[3] := Point (rect.right - Thick2, CenterY + Space2);
             points6[4] := Point (rect.right, CenterY + Thick2 + FSpace);
             bitmap.Canvas.Polygon (slice(points6,5));
           end;

        { lower left segment }
          4: begin
             points6[0] := Point (CenterX + Thick2 + FSpace, rect.bottom - FThickness);
             points6[1] := Point (CenterX + Space2, rect.bottom - Thick2);
             points6[2] := Point (CenterX + Thick2 + FSpace, rect.bottom);
             points6[3] := Point (rect.right - FSpace, rect.bottom);
             points6[4] := Point (rect.right - FThickness - FSpace, rect.bottom - FThickness);
             bitmap.Canvas.Polygon (slice(points6,5));
           end;

      { upper left segment }
          5: begin
             points6[0] := Point (rect.left + FThickness + FSpace, rect.bottom - FThickness);
             points6[1] := Point (CenterX - Thick2 - FSpace, rect.bottom - FThickness);
             points6[2] := Point (CenterX - Space2, rect.bottom - Thick2);
             points6[3] := Point (CenterX - Thick2 - FSpace, rect.bottom);
             points6[4] := Point (rect.left + FSpace, rect.bottom);
             bitmap.Canvas.Polygon (slice(points6,5));
           end;
        { lower left segment }
          6: begin
             points6[0] := Point (rect.left, CenterY + Thick2 + FSpace);
             points6[1] := Point (rect.left + Thick2, CenterY + Space2);
             points6[2] := Point (rect.left + FThickness,
                          CenterY + Thick2{FThickness} + FSpace{Space2});
             points6[3] := Point (rect.left + FThickness,
                          rect.bottom - FThickness - FSpace);
             points6[4] := Point (rect.left, rect.bottom - FSpace);
             bitmap.Canvas.Polygon (slice(points6,5));
         end;

      { upper left segment }
          7: begin
             points6[0] := Point (rect.left, rect.top + FSpace);
             points6[1] := Point (rect.left + FThickness,
                         rect.top + FThickness + FSpace);
             points6[2] := Point (rect.left + FThickness,
                         CenterY - Thick2{- FThickness} - FSpace{Space2});
             points6[3] := Point (rect.left + Thick2, CenterY - Space2);
             points6[4] := Point (rect.left, CenterY - Thick2 - FSpace);
             bitmap.Canvas.Polygon (slice(points6,5));
           end;
        { upper left segment }
          8: begin
             points6[0] := Point (rect.left + Thick2 + FSpace, CenterY);
             points6[1] := Point (rect.left + FThickness + FSpace, CenterY - Thick2);
             points6[2] := Point (CenterX - Thick2 - FSpace, CenterY - Thick2);
             points6[3] := Point (CenterX - FSpace, CenterY);
             points6[4] := Point (CenterX - Thick2 - FSpace, CenterY + Thick2);
             points6[5] := Point (rect.left + FThickness + FSpace, CenterY + Thick2);
             bitmap.Canvas.Polygon (points6);
           end;

        { upper right segment }
          9: begin
             points6[0] := Point (CenterX + FSpace, CenterY);
             points6[1] := Point (CenterX + Thick2 + FSpace, CenterY - Thick2);
             points6[2] := Point (rect.right - FThickness - FSpace, CenterY - Thick2);
             points6[3] := Point (rect.right - Thick2 - FSpace, CenterY);
             points6[4] := Point (rect.right - FThickness - FSpace, CenterY + Thick2);
             points6[5] := Point (CenterX + Thick2 + FSpace, CenterY + Thick2);
             bitmap.Canvas.Polygon (points6);
           end;
        { upper right segment }
          10: begin
             points6[0] := Point (rect.left + Thickness + FSpace, rect.top + FThickness + FSpace);
             points6[1] := Point (rect.left + Thickness + Thickness + FSpace, rect.top + FThickness + FSpace);
             points6[2] := Point (CenterX - Thick2 - FSpace, CenterY - FThickness - Thick2 - FSpace);
             points6[3] := Point (CenterX - Thick2 - FSpace, CenterY - Thick2 - FSpace);
             points6[4] := Point (CenterX - FThickness - Thick2 - FSpace, CenterY - Thick2 - FSpace);
             points6[5] := Point (rect.left + Thickness + FSpace, rect.top + FThickness + FThickness + FSpace);
             bitmap.Canvas.Polygon (points6);
           end;
        { upper right segment }
          11: begin
             points6[0] := Point (CenterX, rect.top + Thick2 + FSpace);
             points6[1] := Point (CenterX + Thick2, rect.top + FThickness + FSpace);
             points6[2] := Point (CenterX + Thick2, CenterY - Thick2 - FSpace);
             points6[3] := Point (CenterX, CenterY - FSpace);
             points6[4] := Point (CenterX - Thick2, CenterY - Thick2 - FSpace);
             points6[5] := Point (CenterX - Thick2, rect.top + FThickness + FSpace);
             bitmap.Canvas.Polygon (points6);
           end;
        { upper right segment }
          12: begin
             points6[0] := Point (rect.right - FThickness - FThickness - FSpace, rect.top + FThickness + FSpace);
             points6[1] := Point (rect.right - FThickness - FSpace, rect.top + FThickness + FSpace);
             points6[2] := Point (rect.right - FThickness - FSpace, rect.top + FThickness + FThickness + FSpace);
             points6[3] := Point (CenterX + FThickness + Thick2 + FSpace, CenterY - Thick2 - FSpace);
             points6[4] := Point (CenterX + Thick2 + FSpace, CenterY - Thick2 - FSpace);
             points6[5] := Point (CenterX + Thick2 + FSpace, CenterY - FThickness - Thick2 - FSpace);
             bitmap.Canvas.Polygon (points6);
           end;
        { upper right segment }
          13: begin
             points6[0] := Point (CenterX + Thick2 + FSpace, CenterY + Thick2 + FSpace);
             points6[1] := Point (CenterX + FThickness + Thick2 + FSpace, CenterY + Thick2 + FSpace);
             points6[2] := Point (rect.right - FThickness - FSpace, rect.bottom - FThickness - FThickness - FSpace);
             points6[3] := Point (rect.right - FThickness - FSpace, rect.bottom - FThickness - FSpace);
             points6[4] := Point (rect.right - FThickness - FThickness - FSpace, rect.bottom - FThickness - FSpace);
             points6[5] := Point (CenterX + Thick2 + FSpace, CenterY + FThickness + Thick2 + FSpace);
             bitmap.Canvas.Polygon (points6);
           end;
        { upper right segment }
          14: begin
             points6[0] := Point (CenterX, CenterY + FSpace );
             points6[1] := Point (CenterX + Thick2, CenterY + Thick2 + FSpace);
             points6[2] := Point (CenterX + Thick2, rect.bottom - FThickness - FSpace);
             points6[3] := Point (CenterX, rect.bottom - Thick2 - FSpace);
             points6[4] := Point (CenterX - Thick2, rect.bottom - FThickness - FSpace);
             points6[5] := Point (CenterX - Thick2, CenterY + Thick2 + FSpace);
             bitmap.Canvas.Polygon (points6);
           end;
        { upper right segment }
          15: begin
             points6[0] := Point (rect.left + FThickness + FSpace, rect.bottom - FThickness - FSpace);
             points6[1] := Point (rect.left + FThickness + FSpace, rect.bottom - FThickness - FThickness - FSpace);
             points6[2] := Point (CenterX - Thickness - Thick2 - FSpace, CenterY + Thick2 + FSpace);
             points6[3] := Point (CenterX - Thick2 - FSpace, CenterY + Thick2 + FSpace);
             points6[4] := Point (CenterX - Thick2 - FSpace, CenterY + Thick2 + Thickness + FSpace);
             points6[5] := Point (rect.left + FThickness + FThickness + FSpace, rect.bottom - FThickness - FSpace);
             bitmap.Canvas.Polygon (points6);
           end;
        end;
      end;
  end;
end;

procedure TCustomSevenSegment.IncRect (var MyRect: TRect; Delta: integer);
begin
  inc (MyRect.left, Delta);
  inc (MyRect.right, Delta);
end;

procedure TCustomSevenSegment.CMVisibleChanged (var msg: TMessage);
begin
  Invalidate;
end;

{$ifndef WIN32}
procedure TCustomSevenSegment.WMRButtonDown (var msg: TWMMouse);
var
  P: TPoint;
begin
  if Assigned (FPopupMenu) and (FPopupMenu.AutoPopup) then
  begin
    P.x := msg.XPos;
    P.y := msg.YPos;
    P := ClientToScreen (P);
    FPopupMenu.Popup (P.x, P.y);
  end;
end;
{$endif}

{ -----------------------------------------------------
  Methods definition of TSevenSegment
  ----------------------------------------------------- }
constructor TSevenSegment.Create (AOwner: TComponent);
begin
  inherited Create (AOwner);

end;

procedure TSevenSegment.Paint;
var
  Delta     : integer;
  Digit     : integer;

  MyRect    : TRect;
  bitmap    : TBitmap;
begin
  if (not Visible) and (not (csDesigning in ComponentState)) then
    exit;

  { Prepare the buffer }
  bitmap := TBitmap.Create;
  bitmap.Width  := ClientRect.right;
  bitmap.Height := ClientRect.bottom;
  bitmap.Canvas.Pen.Style := psClear;

  { Draw background }
  bitmap.Canvas.Brush.Color := Color;
  bitmap.Canvas.FillRect (ClientRect);


  begin
    Delta := (ClientRect.right - ClientRect.left);
    MyRect.left := ((ClientRect.right - Delta) shr 1);
  end;
  MyRect.top    := ClientRect.top + FVertSpace shr 1;
  MyRect.right  := MyRect.left + Delta;
  MyRect.bottom := ClientRect.bottom - FVertSpace shr 1;

  { Start drawing }
  begin
    begin
      { Get the character at current position }
      Digit := FValue;

      { Draw each segment }
      DrawSegment (Digit, bitmap, MyRect);
    end;
  end;

  Canvas.Draw (0, 0, bitmap);
  bitmap.Free;
end;

procedure Register;
begin
  RegisterComponents ('CJW', [TSevenSegment]);
end;

end.
