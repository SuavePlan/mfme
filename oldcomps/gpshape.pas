unit Gpshape;

interface

uses WinTypes, WinProcs, Messages, SysUtils, Classes, Controls, Forms, Menus,
  Graphics, StdCtrls,ExtCtrls;

const MAXVECTORS = 100;

const
  HatchBits50: array[0..7] of Word = ($AA, $55,$AA, $55,$AA, $55,$AA, $55);  {50%}
  HatchBits25: array[0..7] of Word = ($AA, $0,$AA, $0,$AA, $0,$AA, $0);  {25%}


type

  THackCanvas = class(TCanvas)
  end;

  TSurfaceType = (stBrushTransparent,stBrushOpaque,stBrushImage,stGradientFill,stBrush50PercentOpaque,stBrush25PercentOpaque);

  TGPShapeType = (gstRectangle, gstSquare, gstRoundRect, gstRoundSquare,
                  gstEllipse, gstCircle,
                  gstDiamond, gstStar,gstPolyGon,
                  gstTriangleLeft,gstTriangleRight,gstTriangleUp,gstTriangleDown);

  TFillDirection = (gdUp, gdDown, gdLeft, gdRight);

  TShadowOrient = (soNone,soLowerRight,soUpperRight,soUpperLeft,soLowerLeft);

  TShadowFill = (sfSolid,sf25Percent,sf50Percent);

  TManyShape = class(TGraphicControl)
  private
    FBackColor: TColor;
    FColour1: TColor;
    FColour2: TColor;
    FFillDirection : TFillDirection;
    FForeColor : TColor;
    FGradientToColor : Tcolor;
    FOnImage1      : TBitmap;
    FOnImage2      : TBitmap;
    FOffimage      : TBitmap;
    FOld           : TBitmap;
    FInVectorPercent:integer;
    FOutlineColor : TColor;
    FOutlineStyle : TPenStyle;
    FOutlineWidth : integer;
    FPattern : TBrushStyle;
    FReserved: Byte;
    FRoundPerc : integer;
    FShadowColor : TColor;
    FShadowDepth : integer;
    FShadowOrient : TShadowOrient;
    FShape: TGPShapeType;
    FStartingAngle:integer;
    FSurfaceType : TSurfaceType;
    FVectors:integer;
    FShadowFill : TShadowFill;
    FTransparent : boolean;
    FState : integer;
    FButton : integer;

    procedure ChangedOnImage1(Sender: TObject);
    procedure ChangedOnImage2(Sender: TObject);
    procedure ChangedOffImage(Sender: TObject);
    Procedure PolyGonFromWinProcs(a:array of TPoint;i :integer);
    procedure SetPointsInPolyGonArray(var v:array of TPoint; l,t,w,h,offsetleft,offsettop : integer);
    procedure FillWithBitmap(FImage :TBitmap);
    procedure FillWithGradient;
    procedure GetShadowRects(var rMainRect,rShadowRect:TRect);
    procedure PaintDiamond;
    procedure PaintEllipse;
    Procedure PaintPolyGon;
    procedure PaintRectangle;
    procedure PaintRoundRectangle;
    procedure PaintTriangle;
    procedure SetBackColor(Value: TColor);
    procedure SetColor1(Value: TColor);
    procedure SetColor2(Value: TColor);
    procedure SetFillDirection(Value : TFillDirection);
    procedure SetForeColor(Value:  TColor);
    procedure SetGradientToColor(Value : TColor);
    procedure SetOnImage1(Value: TBitMap);
    procedure SetOnImage2(Value: TBitMap);
    procedure SetOffImage(Value: TBitMap);
    procedure SetState(Value: integer);
    procedure SetInVectorPercent(Value : integer);
    procedure SetOutlineColor(Value:  TColor);
    procedure SetOutlineStyle(Value:  TPenStyle);
    procedure SetOutlineWidth(Value:  integer);
    procedure SetPattern(Value :TBrushStyle);
    procedure SetPenBrushForBackground;
    procedure SetPenBrushForForeground;
    procedure SetPenBrushForGradient;
    procedure SetPenBrushForImage;
    procedure SetPenBrushForOutline;
    procedure SetPenBrushForShadow;
    procedure SetRoundPerc(Value: integer);
    procedure SetShadowColor(Value:  TColor);
    procedure SetShadowDepth(Value: integer);
    procedure SetShadowFill(Value:TShadowFill);
    procedure SetShadowOrient(Value: TShadowOrient);
    procedure SetShape(Value: TGPShapeType);
    procedure SetStartingAngle(Value : integer);
    procedure SetSurfaceType(Value : TSurfaceType);
    procedure SetVectors(Value : integer);
    procedure SetTransparent(Value : boolean);
    procedure TransparentBlt(Dest:TCanvas;Bmp:TBitmap;X,Y:Integer;TransColor:TColor);
    procedure SetButton(Value : integer);



  protected
    procedure Paint; override;

  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;

  published

    property Align;
    property BackColor:          TColor read FBackColor write SetBackColor;
    property Colour1:            TColor read FColour1 write SetColor1;
    property Colour2:            TColor read FColour2 write SetColor2;
    property FillDirection:      TFillDirection read FFillDirection write SetFillDirection;
    property ForeColor :         TColor read FForecolor write SetForeColor;
    property GradientToColor :   Tcolor read FGradientToColor write SetGradientToColor;
    property OnImage1  :         TBitMap read FOnImage1 write SetOnImage1;
    property OnImage2  :         TBitMap read FOnImage2 write SetOnImage2;
    property OffImage :          TBitMap read FOffImage write SetOffImage;
    property State :             integer read FState write SetState default 0;
    property InVectorPercent :   integer read FInVectorPercent write SetInVectorPercent;
    property OutlineColor :      TColor read FOutlineColor write SetOutlineColor;
    property OutlineStyle :      TPenStyle read FOutlineStyle write SetOutlineStyle;
    property OutlineWidth :      integer read FOutlinewidth write SetOutlineWidth;
    property Pattern :           TBrushStyle read FPattern write SetPattern;
    property PopupMenu;
    property RoundPerc :         integer read FRoundPerc write SetRoundPerc;
    property ShadowColor :       TColor read FShadowColor write SetShadowColor;
    property ShadowDepth :       integer read FShadowDepth write SetShadowDepth;
    property ShadowOrient :      TShadowOrient read FShadowOrient write SetShadowOrient;
    property ShadowFill :        TShadowFill read FShadowFill write SetShadowFill default sfSolid;
    property Shape:              TGPShapeType read FShape write SetShape default gstRectangle;
    property StartingAngle :     integer read FStartingAngle write SetStartingAngle;
    property SurfaceType:        TSurfaceType read FSurfaceType write SetSurfaceType;
    property Vectors :           integer read FVectors write SetVectors;
    property Trans :             boolean read FTransparent write SetTransparent;
    property Button :            integer read FButton write SetButton;

    property OnClick;
    property OnDblClick;
    property OnDragDrop;
    property OnDragOver;
    property OnMouseDown;
    property OnMouseMove;
    property OnMouseUp;


  end;

procedure Register;

implementation

{$ifdef WIN32}
{$R GPSHAP32}
{$else}
{$R GPSHAP16}
{$endif}

constructor TManyShape.Create(AOwner: TComponent);
var
  X, Y: Integer;
begin
  inherited Create(AOwner);
  Width           := 25;
  Height          := 25;

  FOnImage1       := TBitMap.Create;
  FOnImage2       := TBitMap.Create;
  FOffImage       := TBitmap.Create;
  FOld            := TBitmap.Create;
  FOnImage1.OnChange := ChangedOnImage1;
  FOnImage2.OnChange := ChangedOnImage2;
  FOffImage.OnChange := ChangedOffImage;

  FRoundPerc       := 25;
  FShadowDepth     := 5;
  FShadowFill      := sfSolid;
  FVectors         := 10;
  FInVectorPercent := 50;
  FStartingAngle   := 90;

  FOutlineWidth    := 1;
  FBackColor       := clWhite;
  FPattern         := bsClear;

  FSurfacetype     := stBrushOpaque;
end;

destructor TManyShape.Destroy;
begin
  FOnImage1.Free;
  FOnImage2.Free;
  FOffImage.Free;
  FOld.Free;
  inherited Destroy;
end;




procedure TManyShape.Paint;
begin
  {make sure Align not set while one of the 'square'
   shapes is desired or it can get into an endless loop}
  if Align <> alNone then
   case FShape of
   gstSquare:  FShape := gstRectangle;
   gstRoundSquare: FShape := gstRoundRect;
   gstCircle: FShape := gstEllipse;
   end {case}
  else
   case FShape of
   gstSquare:  if width > height then width := height;
   gstRoundSquare: if width > height then width := height;
   gstCircle: if width > height then width := height;
   end; {case}


  with Canvas do
  begin
    case FShape of
      gstTriangleLeft,gstTriangleRight,gstTriangleUp,gstTriangleDown:
        PaintTriangle;
      gstRectangle, gstSquare:
        PaintRectangle;
      gstRoundRect, gstRoundSquare:
        PaintRoundRectangle;
      gstCircle, gstEllipse:
        PaintEllipse;
      gstDiamond:
        PaintDiamond;
      gstPolyGon:
        PaintPolyGon;
      gstStar:
        PaintPolyGon;
    end;
  end;
end;


procedure TManyShape.SetOnImage1(Value: TBitMap);
begin
  if Assigned(Value) then
    FOnImage1.Assign(Value)
  else
  begin
    FOnImage1.Height := 0;
    FOnImage1.Width  := 0;
  end;
end;

procedure TManyShape.SetOnImage2(Value: TBitMap);
begin
  if Assigned(Value) then
    FOnImage2.Assign(Value)
  else
  begin
    FOnImage2.Height := 0;
    FOnImage2.Width  := 0;
  end;
end;

procedure TManyShape.SetOffImage(Value: TBitMap);
begin
  if Assigned(Value) then
    FOffImage.Assign(Value)
  else
  begin
    FOffImage.Height := 0;
    FOffImage.Width  := 0;
  end;
end;

procedure TManyShape.SetState(Value: integer);
begin
  if FState <> Value then
  begin
    FState := Value;
    Paint;
  end;
end;

procedure TManyShape.SetTransparent(Value: boolean);
begin
    FTransparent := Value;
    Invalidate;
end;

procedure TManyShape.ChangedOnImage1(Sender: TObject);
begin
  Invalidate;
end;

procedure TManyShape.ChangedOnImage2(Sender: TObject);
begin
  Invalidate;
end;

procedure TManyShape.ChangedOffImage(Sender: TObject);
begin
  Invalidate;
end;


procedure TManyShape.SetPattern(Value :TBrushStyle);
begin
  if Value <> FPattern then
  begin
    FPattern := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetBackColor(Value: TColor);
begin
  if Value <> FBackColor then
  begin
    FBackColor := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetColor1(Value: TColor);
begin
  if Value <> FColour1 then
  begin
    FColour1 := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetColor2(Value: TColor);
begin
  if Value <> FColour2 then
  begin
    FColour2 := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetButton(Value: integer);
begin
  if Value <> FButton then
  begin
    FButton := Value;
  end;
end;

procedure TManyShape.SetForeColor(Value:  TColor);
begin
  if Value <> FForeColor then
  begin
    FForeColor := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetGradientToColor(Value:  TColor);
begin
  if Value <> FGradientToColor then
  begin
    FGradientToColor := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetShadowColor(Value:  TColor);
begin
  if Value <> FShadowColor then
  begin
    FShadowColor := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetShadowFill(Value:  TShadowFill);
begin
  if Value <> FShadowFill then
  begin
    FShadowFill := Value;
    Refresh;
  end;
end;


procedure TManyShape.SetOutlineColor(Value:  TColor);
begin
  if Value <> FOutlineColor then
  begin
    FOutlineColor := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetOutlineWidth(Value:  integer);
begin
  if Value <> FOutlineWidth then
  begin
    FOutlineWidth := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetOutlineStyle(Value:  TPenStyle);
begin
  if Value <> FOutlineStyle then
  begin
    FOutlineStyle := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetFillDirection(Value : TFillDirection);
begin
  if Value <> FFillDirection then
  begin
    FFillDirection := Value;
    Refresh;
  end;
end;

procedure TManyShape.SetSurfaceType(Value : TSurfaceType);
begin
  if Value <> FSurfaceType then
  begin
    FSurfaceType := Value;
    Refresh;
  end;
end;



procedure TManyShape.SetShape(Value: TGPShapeType);
begin
  if FShape <> Value then
  begin

    FShape := Value;

    {set defaults for shape}
    case FShape of
    gstStar:
     begin
      if InVectorPercent=0 then InVectorPercent := 50;
      if Vectors < 10 then vectors := 10;
      if StartingAngle <> 90 then StartingAngle := 90;
     end;
    gstSquare:  if Align <> alNone then Align := alNone;
    gstRoundSquare: if Align <> alNone then Align := alNone;
    gstCircle: if Align <> alNone then Align := alNone;
    end; {case}

    Invalidate;
  end;
end;

procedure TManyShape.SetRoundPerc(Value: integer);
begin
  if FRoundPerc <> Value then
  begin
    FRoundPerc := Value;
    Invalidate;
  end;
end;

procedure TManyShape.SetVectors(Value : integer);
begin
  { less than 3 sides meaningless}
  if (Value < 3) or (value > MAXVECTORS) then exit; {invalid}

  if FVectors <> Value then
  begin
    FVectors := Value;
    Invalidate;
  end;
end;

procedure TManyShape.SetStartingAngle(Value : integer);
begin
  if (Value < 0) or (value > 359) then exit; {invalid}
  if FStartingAngle <> Value then
  begin
    FStartingAngle := Value;
    Invalidate;
  end;
end;

procedure TManyShape.SetInVectorPercent(Value : integer);
begin
  if (Value < 0) or (value > 100) then exit; {invalid}

  if FInVectorPercent <> Value then
  begin
    FInVectorPercent := Value;
    Invalidate;
  end;
end;

procedure TManyShape.SetShadowDepth(Value: integer);
begin
  if FShadowDepth > (width div 2) then exit;
  if FShadowDepth > (height div 2) then exit;
  if FShadowDepth <> Value then
  begin
    FShadowDepth := Value;
    Invalidate;
  end;
end;

procedure TManyShape.SetShadowOrient(Value: TShadowOrient);
begin
  if (Value >= soNone) and (value <=soLowerLeft) then
  begin
   if FShadowOrient <> Value then
   begin
    FShadowOrient := Value;
    Invalidate;
   end;
  end;
end;



{build the array of polygon points }
procedure TManyShape.SetPointsInPolyGonArray(var v:array of TPoint; l,t,w,h,offsetleft,offsettop : integer);
var   vector,radius: integer;
      radians : double;
      bInner : boolean;
      CtrX, CtrY : Integer;
      rInPercent : double;
      iVectors : integer;
begin
 iVectors := Vectors;
 rInPercent := (100-InVectorPercent)/100;
 CtrX := W DIV 2;
 CtrY := H DIV 2;
 IF CtrX < CtrY THEN Radius := CtrX - 4
 ELSE Radius := CtrY - 4;
 bInner := false;
 for vector := 0 to iVectors do
 begin
   radians := ((vector* (360 /iVectors))-StartingAngle) * pi /180;
   if bInner then
    begin
     v[vector].x := l+ctrx +round(cos(radians) * (radius*rInPercent))+offsetleft;
     v[vector].y := t+ctry + round(sin(radians) * (radius*rInPercent))+offsettop;
    end
   else
    begin
     v[vector].x := l+ctrx +round(cos(radians) * radius)+offsetleft;
     v[vector].y := t+ctry + round(sin(radians) * radius)+offsettop;
    end;
   binner := not bInner;
 end;

end;

{paints all polygons}
Procedure TManyShape.PaintPolyGon;
var   V : ARRAY[0..MAXVECTORS] of TPoint;
      r,rs : TRect;
      rgn : HRgn;
      i : integer;
    Bitmap: HBitmap;
    SaveBrush: HBrush;
    SaveTextColor, SaveBkColor: TColorRef;
begin
    GetShadowRects(r,rs);

    {draw shadow if applicable}
    if FShadowOrient <> soNone then {draw shadow}
    begin
     SetPenBrushForShadow;
     SetPointsInPolyGonArray(v,rs.left,rs.top,rs.right-rs.left,rs.bottom-rs.top,0,0);
     Case FShadowFill of
     sfSolid : PolyGonFromWinProcs(v,Vectors);
     sf25Percent,sf50Percent :
       begin
          SetPointsInPolyGonArray(v,rs.left,rs.top,rs.right-rs.left,r.bottom-rs.top,left,top);
          rgn :=  Winprocs.CreatePolygonRgn(v,Vectors,WINDING);
          SelectClipRgn(canvas.handle,rgn);
          if FShadowFill = sf50Percent then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, ShadowColor);
          with rs do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
       end;
     end;

    end;


    {draw the shape}
    if (FSurfaceType = stBrush50PercentOpaque) or (FSurfaceType = stBrush25PercentOpaque) then
      begin
          SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,left,top);
          rgn :=  Winprocs.CreatePolygonRgn(v,Vectors,WINDING);
          SelectClipRgn(canvas.handle,rgn);
          if (FSurfaceType = stBrush50PercentOpaque) then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, BackColor);
          with r do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
      end
    else if FSurfaceType = stBrushTransparent then
      begin
       SetPenBrushForForeground;
       SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,0,0);
       PolyGonFromWinProcs(v,Vectors);
      end
    else if FSurfaceType = stBrushOpaque then
      begin
{       SetPenBrushForBackground;
       SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,0,0);
       PolyGonFromWinProcs(v,Vectors); }
       SetPenBrushForForeground;
       SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,0,0);
       PolyGonFromWinProcs(v,Vectors);
      end
    else if FSurfaceType = stBrushImage then
    begin
     {draw the bitmap}
     SetPenBrushForImage;
     SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,left,top);
     rgn :=  Winprocs.CreatePolygonRgn(v,Vectors,WINDING);
     SelectClipRgn(canvas.handle,rgn);
     if FState and 1 = 1 then
       FillWithBitmap(FOnImage1)
     else if Fstate and 2 = 2 then
       FillWithBitmap(FOnImage2)
     else
       FillWithBitmap(FOffImage);
     SelectClipRgn(canvas.handle,0);
     DeleteObject(rgn);
    end
    else if FSurfaceType = stGradientFill then
    begin
     SetPenBrushForGradient;
     SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,left,top);
     rgn :=  Winprocs.CreatePolygonRgn(v,Vectors,WINDING);
     SelectClipRgn(canvas.handle,rgn);
     FillWithGradient;
     SelectClipRgn(canvas.handle,0);
     DeleteObject(rgn);
    end;
    {and draw the outside}
    SetPenBrushForOutline;
    SetPointsInPolyGonArray(v,r.left,r.top,r.right-r.left,r.bottom-r.top,0,0);
    PolyGonFromWinProcs(v,Vectors);
end;


procedure TManyShape.PaintRectangle;
var
    r,rs : TRect;
    rgn : Hrgn;

    Bitmap: HBitmap;
    SaveBrush: HBrush;
    SaveTextColor, SaveBkColor: TColorRef;

begin
    GetShadowRects(r,rs);

    if FShadowOrient <> soNone then {draw shadow}
    begin
     SetPenBrushForShadow;
     Case FShadowFill of
     sfSolid : canvas.rectangle(rs.left,rs.top,rs.right,rs.bottom);
     sf25Percent,sf50Percent :
       begin
          if FShadowFill = sf50Percent then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, ShadowColor);
          with rs do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
       end;
     end;

    end;

    {draw the shape}
    if (FSurfaceType = stBrush50PercentOpaque) or (FSurfaceType = stBrush25PercentOpaque) then
     begin
          rgn := CreateRectRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
          SelectClipRgn(canvas.handle,rgn);
          if (FSurfaceType = stBrush50PercentOpaque) then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, BackColor);
          with r do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
     end
    else if FSurfaceType = stBrushTransparent then
      begin
       {set all defaults - color before style}
       SetPenBrushForForeground;
       canvas.rectangle(r.left,r.top,r.right,r.bottom);
      end
    else if FSurfaceType = stBrushOpaque then
      begin
{       SetPenBrushForBackground;
       canvas.rectangle(r.left,r.top,r.right,r.bottom); }
       {set all defaults - color before style}
       SetPenBrushForForeground;
       canvas.rectangle(r.left,r.top,r.right,r.bottom);
      end
    else if FSurfaceType = stBrushImage then
    begin
     {draw the bitmap}
     SetPenBrushForImage;
     rgn := CreateRectRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
     SelectClipRgn(canvas.handle,rgn);
     if FState = 1 then
       FillWithBitmap(FOnImage1)
     else if FState = 2 then
       FillWithBitmap(FOnImage2)
     else
       FillWithBitmap(FOffImage);
     SelectClipRgn(canvas.handle,0);
     DeleteObject(rgn);
    end
    else if FSurfaceType = stGradientFill then
    begin
     SetPenBrushForGradient;
     rgn := CreateRectRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
     SelectClipRgn(canvas.handle,rgn);
     FillWithGradient;
     SelectClipRgn(canvas.handle,0);
     DeleteObject(rgn);
    end;
    {and draw the outside}
    SetPenBrushForOutline;
    canvas.rectangle(r.left,r.top,r.right,r.bottom);
end;



procedure TManyShape.PaintRoundRectangle;
var
    r,rs : TRect;
    mw : integer;
    Corner : integer;
    rgn : Hrgn;
    Bitmap: HBitmap;
    SaveBrush: HBrush;
    SaveTextColor, SaveBkColor: TColorRef;
begin
    if Width > Height then MW := Width else MW := Height;
    Corner := trunc(mw*(RoundPerc/100));

    GetShadowRects(r,rs);

    if FShadowOrient <> soNone then {draw shadow}
    begin
     SetPenBrushForShadow;
     Case FShadowFill of
     sfSolid : canvas.RoundRect(rs.left,rs.top,rs.right,rs.bottom,Corner,Corner);
     sf25Percent,sf50Percent :
       begin
          if FShadowFill = sf50Percent then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, ShadowColor);
          rgn := CreateRoundRectRgn(left+rs.left,top+rs.top,left+rs.right,top+rs.bottom,Corner,Corner);
          SelectClipRgn(canvas.handle,rgn);
          with rs do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
       end;
     end;

    end;

    {draw the shape}
    if (FSurfaceType = stBrush50PercentOpaque) or (FSurfaceType = stBrush25PercentOpaque) then
     begin
          SetPenBrushForForeground;
          rgn := CreateRoundRectRgn(left+r.left,top+r.top,left+r.right,top+r.bottom,Corner,Corner);
          SelectClipRgn(canvas.handle,rgn);
          if (FSurfaceType = stBrush50PercentOpaque) then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, BackColor);
          with r do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
     end
    else if FSurfaceType = stBrushTransparent then
      begin
       {set all defaults - color before style}
       SetPenBrushForForeground;
       SetBkMode(canvas.handle,Transparent);
       canvas.RoundRect(r.left,r.top,r.right,r.bottom,Corner,Corner);
      end
    else if FSurfaceType = stBrushOpaque then
      begin
{       SetPenBrushForBackground;
       canvas.RoundRect(r.left,r.top,r.right,r.bottom,Corner,Corner);   }
       {set all defaults - color before style}
       SetPenBrushForForeground;
       canvas.RoundRect(r.left,r.top,r.right,r.bottom,Corner,Corner);
      end
    else if FSurfaceType = stBrushImage then
    begin
     {draw the bitmap}
     SetPenBrushForImage;
     rgn := CreateRoundRectRgn(left+r.left,top+r.top,left+r.right,top+r.bottom,Corner,Corner);
     SelectClipRgn(canvas.handle,rgn);
     if FState = 1 then
       FillWithBitmap(FOnImage1)
     else if FState = 2 then
       FillWithBitmap(FOnImage2)
     else
       FillWithBitmap(FOffImage);
     SelectClipRgn(canvas.handle,0);
     DeleteObject(rgn);
    end
    else if FSurfaceType = stGradientFill then
    begin
     SetPenBrushForGradient;
     rgn := CreateRoundRectRgn(left+r.left,top+r.top,left+r.right,top+r.bottom,Corner,Corner);
     SelectClipRgn(canvas.handle,rgn);
     FillWithGradient;
     SelectClipRgn(canvas.handle,0);
     DeleteObject(rgn);
    end;
    {and draw the outside}
    SetPenBrushForOutline;
    SetBkMode(canvas.handle,Transparent);
    canvas.RoundRect(r.left,r.top,r.right,r.bottom,Corner,Corner);
end;

procedure TManyShape.PaintEllipse;
var
    r,rs : TRect;
    rgn :Hrgn;
    Bitmap: HBitmap;
    SaveBrush: HBrush;
    SaveTextColor, SaveBkColor: TColorRef;
begin
    GetShadowRects(r,rs);

    if FShadowOrient <> soNone then {draw shadow}
    begin
     SetPenBrushForShadow;
     Case FShadowFill of
     sfSolid : canvas.Ellipse(rs.left,rs.top,rs.right,rs.bottom);
     sf25Percent,sf50Percent :
       begin
          if FShadowFill = sf50Percent then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, ShadowColor);
          rgn := CreateEllipticRgn(left+rs.left,top+rs.top,left+rs.right,top+rs.bottom);
          SelectClipRgn(canvas.handle,rgn);
          with rs do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
       end;
     end;

    end;



     if (FSurfaceType = stBrush50PercentOpaque) or (FSurfaceType = stBrush25PercentOpaque) then
     begin
          SetPenBrushForForeground;
          rgn := CreateEllipticRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
          SelectClipRgn(canvas.handle,rgn);
          if (FSurfaceType = stBrush50PercentOpaque) then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, BackColor);
          with r do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
     end
    else if FSurfaceType = stBrushTransparent then
     begin
      SetPenBrushForForeground;
      rgn := CreateEllipticRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
      SelectClipRgn(canvas.handle,rgn);
      {can't just draw an ellipse, because color will always
       be white on black. Windows bug?}
      canvas.rectangle(0,0,width,height);
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end
    else if FSurfaceType = stBrushOpaque then
     begin
{      SetPenBrushForBackGround;
      canvas.Ellipse(r.left,r.top,r.right,r.bottom);  }
      SetPenBrushForForeground;
      rgn := CreateEllipticRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
      SelectClipRgn(canvas.handle,rgn);
      {can't just draw an ellipse, because color will always
       be white on black. Windows bug?}
      canvas.rectangle(0,0,width,height);
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end
    else if FSurfaceType = stBrushImage then
     begin
      SetPenBrushForImage;
      rgn := CreateEllipticRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
      SelectClipRgn(canvas.handle,rgn);
     if FState = 1 then
       FillWithBitmap(FOnImage1)
     else if FState = 2 then
       FillWithBitmap(FOnImage2)
     else
       FillWithBitmap(FOffImage);
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end
    else if FSurfaceType = stGradientFill then
     begin
      SetPenBrushForGradient;
      rgn := CreateEllipticRgn(left+r.left,top+r.top,left+r.right,top+r.bottom);
      SelectClipRgn(canvas.handle,rgn);
      FillWithGradient;
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end;
     SetPenBrushForOutLine;
     canvas.Ellipse(r.left,r.top,r.right,r.bottom);
end;

procedure TManyShape.PaintTriangle;
var
    r,rs : TRect;
    triangle,sTriangle : ARRAY[0..2] of TPoint;
    oTriangle: array[0..2] of TPoint;
    rgn :Hrgn;
    i : integer;
    w,h : integer;
    Bitmap: HBitmap;
    SaveBrush: HBrush;
    SaveTextColor, SaveBkColor: TColorRef;
begin
    GetShadowRects(r,rs);

    w := r.right-r.left;
    h := r.bottom-r.top;
    {fill out the points of the triangle array,
     depending on type of triangle}
    case fShape of
    gstTriangleLeft:
     begin
       triangle[0].x := r.left+w-1;
       triangle[0].y := r.top;
       triangle[1].x := r.left+w-1;
       triangle[1].y := r.top+h-1;
       triangle[2].x := r.left;
       triangle[2].y := r.top+(h div 2);

       striangle[0].x := rs.left+w-1;
       striangle[0].y := rs.top;
       striangle[1].x := rs.left+w-1;
       striangle[1].y := rs.top+h-1;
       striangle[2].x := rs.left;
       striangle[2].y := rs.top+(h div 2);

     end;
    gstTriangleRight:
     begin
       triangle[0].x := r.left;
       triangle[0].y := r.top;
       triangle[1].x := r.left+w-1;
       triangle[1].y := r.top+(h div 2);
       triangle[2].x := r.left;
       triangle[2].y := r.top+h-1;

       striangle[0].x := rs.left;
       striangle[0].y := rs.top;
       striangle[1].x := rs.left+w-1;
       striangle[1].y := rs.top+(h div 2);
       striangle[2].x := rs.left;
       striangle[2].y := rs.top+h-1;

     end;

    gstTriangleUp:
     begin
       triangle[0].x := r.left+(w div 2);
       triangle[0].y := r.top;
       triangle[1].x := r.left;
       triangle[1].y := r.top+(h-1);
       triangle[2].x := r.left+w-1;
       triangle[2].y := r.top+(h-1);

       striangle[0].x := rs.left+(w div 2);
       striangle[0].y := rs.top;
       striangle[1].x := rs.left;
       striangle[1].y := rs.top+(h-1);
       striangle[2].x := rs.left+w-1;
       striangle[2].y := rs.top+(h-1);

     end;
    gstTriangleDown:
     begin
       triangle[0].x := r.left;
       triangle[0].y := r.top;
       triangle[1].x := r.left+(w div 2);
       triangle[1].y := r.top+h-1;
       triangle[2].x := r.left+w-1;
       triangle[2].y := r.top;

       striangle[0].x := rs.left;
       striangle[0].y := rs.top;
       striangle[1].x := rs.left+(w div 2);
       striangle[1].y := rs.top+h-1;
       striangle[2].x := rs.left+w;
       striangle[2].y := rs.top;

     end;

    end;
    for i := 0 to 2 do
    begin
       Otriangle[i].x := left+triangle[i].x;
       Otriangle[i].y := top+triangle[i].y;
    end;


    if FShadowOrient <> soNone then {draw shadow}
    begin
     SetPenBrushForShadow;
     Case FShadowFill of
     sfSolid : canvas.polygon(striangle);
     sf25Percent,sf50Percent :
       begin
          {offset for use with region}
          for i := 0 to 2 do
          begin
            striangle[i].x := left+striangle[i].x;
            striangle[i].y := top+striangle[i].y;
          end;
          rgn :=  Winprocs.CreatePolygonRgn(striangle,3,WINDING);
          SelectClipRgn(canvas.handle,rgn);
          if FShadowFill = sf50Percent then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, ShadowColor);
          {canvas.rectangle(rs.left,rs.top,rs.right,rs.bottom);}
          with rs do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
       end;
     end;

    end;


    if (FSurfaceType = stBrush50PercentOpaque) or (FSurfaceType = stBrush25PercentOpaque) then
     begin
          rgn :=  Winprocs.CreatePolygonRgn(otriangle,3,WINDING);
          SelectClipRgn(canvas.handle,rgn);
          if (FSurfaceType = stBrush50PercentOpaque) then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, BackColor);
          with r do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
     end
    else if FSurfaceType = stBrushTransparent then
     begin
      SetPenBrushForForeground;
      canvas.polygon(triangle);
     end
    else if FSurfaceType = stBrushOpaque then
     begin
{      SetPenBrushForBackGround;
      canvas.polygon(triangle);  }
      SetPenBrushForForeground;
      canvas.polygon(triangle);
     end
    else if FSurfaceType = stBrushImage then
     begin
      SetPenBrushForImage;
      rgn :=  Winprocs.CreatePolygonRgn(otriangle,3,WINDING);
      SelectClipRgn(canvas.handle,rgn);
     if FState = 1 then
       FillWithBitmap(FOnImage1)
     else if FState = 2 then
       FillWithBitmap(FOnImage2)
     else
       FillWithBitmap(FOffImage);
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end
    else if FSurfaceType = stGradientFill then
     begin
      SetPenBrushForGradient;
      rgn :=  Winprocs.CreatePolygonRgn(otriangle,3,WINDING);
      SelectClipRgn(canvas.handle,rgn);
      FillWithGradient;
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)

     end;
     SetPenBrushForOutLine;
     canvas.polygon(triangle);
end;


procedure TManyShape.PaintDiamond;
var
    r,rs : TRect;
    diamond,sDiamond,oDiamond : ARRAY[0..3] of TPoint;
    rgn :Hrgn;
    i : integer;
    w,h : integer;
    Bitmap: HBitmap;
    SaveBrush: HBrush;
    SaveTextColor, SaveBkColor: TColorRef;
begin
    GetShadowRects(r,rs);

    w := r.right-r.left;
    h := r.bottom-r.top;


    {fill out the array of points for the diamond}
    diamond[0].x := r.left+(w div 2);
    diamond[0].y := r.top;
    diamond[1].x := r.left+w-1;
    diamond[1].y := r.top+(h div 2);
    diamond[2].x := r.left+(w div 2);
    diamond[2].y := r.top+h-1;
    diamond[3].x := r.left;
    diamond[3].y := r.top+(h div 2);

    sdiamond[0].x := rs.left+(w div 2);
    sdiamond[0].y := rs.top;
    sdiamond[1].x := rs.left+w-1;
    sdiamond[1].y := rs.top+(h div 2);
    sdiamond[2].x := rs.left+(w div 2);
    sdiamond[2].y := rs.top+h-1;
    sdiamond[3].x := rs.left;
    sdiamond[3].y := rs.top+(h div 2);


    for i := 0 to 3 do
    begin
       ODiamond[i].x := left+diamond[i].x;
       ODiamond[i].y := top+diamond[i].y;
    end;

    if FShadowOrient <> soNone then {draw shadow}
    begin
     SetPenBrushForShadow;
     Case FShadowFill of
     sfSolid : canvas.polygon(sdiamond);
     sf25Percent,sf50Percent :
       begin
          {offset for use with region}
          for i := 0 to 3 do
          begin
            sdiamond[i].x := left+sdiamond[i].x;
            sdiamond[i].y := top+sdiamond[i].y;
          end;
          rgn :=  Winprocs.CreatePolygonRgn(sdiamond,4,WINDING);
          SelectClipRgn(canvas.handle,rgn);
          if FShadowFill = sf50Percent then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, ShadowColor);
          with rs do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
       end;
     end;

    end;


    if (FSurfaceType = stBrush50PercentOpaque) or (FSurfaceType = stBrush25PercentOpaque) then
     begin
          rgn :=  Winprocs.CreatePolygonRgn(odiamond,4,WINDING);
          SelectClipRgn(canvas.handle,rgn);
          if (FSurfaceType = stBrush50PercentOpaque) then
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits50)
          else
           Bitmap := CreateBitmap(8, 8, 1, 1, @HatchBits25);
          SaveBrush := SelectObject(canvas.Handle, CreatePatternBrush(Bitmap));
          SaveTextColor := SetTextColor(canvas.Handle, clWhite);
          SaveBkColor := SetBkColor(canvas.Handle, BackColor);
          with r do PatBlt(canvas.Handle, Left, Top, Right - Left, Bottom - Top, $00A000C9);
          SetBkColor(canvas.Handle, SaveBkColor);
          SetTextColor(canvas.Handle, SaveTextColor);
          DeleteObject(SelectObject(canvas.Handle, SaveBrush));
          DeleteObject(Bitmap);
          SelectClipRgn(canvas.handle,0);
          DeleteObject(rgn);
     end
    else if FSurfaceType = stBrushTransparent then
     begin
      SetPenBrushForForeground;
      canvas.polygon(diamond);
     end
    else if FSurfaceType = stBrushOpaque then
     begin
{      SetPenBrushForBackGround;
      canvas.polygon(diamond);   }
      SetPenBrushForForeground;
      canvas.polygon(diamond);
     end
    else if FSurfaceType = stBrushImage then
     begin
      SetPenBrushForImage;
      rgn :=  Winprocs.CreatePolygonRgn(odiamond,4,WINDING);
      SelectClipRgn(canvas.handle,rgn);
     if FState = 1 then
       FillWithBitmap(FOnImage1)
     else if FState = 2 then
       FillWithBitmap(FOnImage2)
     else
       FillWithBitmap(FOffImage);
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end
    else if FSurfaceType = stGradientFill then
     begin
      SetPenBrushForGradient;
      rgn :=  Winprocs.CreatePolygonRgn(odiamond,4,WINDING);
      SelectClipRgn(canvas.handle,rgn);
      FillWithGradient;
      SelectClipRgn(canvas.handle,0);
      DeleteObject(rgn)
     end;
     SetPenBrushForOutLine;
     canvas.polygon(diamond);
end;


{
 Determine the rectangles for the shape and for the
 shadow. Both rectangles are Width-FShadowDepth wide
 and Height-FShadowDepth High, and are placed according
 to FShadowOrient.

 TShadowOrient = (soNone,soLowerRight,soUpperRight,soUpperLeft,soLowerLeft);
}
procedure TManyShape.GetShadowRects(var rMainRect,rShadowRect:TRect);
var x,y,w,h : integer;
begin
    X := FOutlineWidth div 2;
    Y := X;
    W := Width - FOutlineWidth + 1;
    H := Height - FOutlineWidth + 1;

    case FShadowOrient of
    soLowerRight:
     begin
      {lower right}
      rShadowRect := Rect(X+FShadowDepth,Y+FShadowDepth,w,h);
      rMainRect := Rect(x,y,W-FShadowDepth,H-FShadowDepth);
      if FShape = gstDiamond then
      begin
       rShadowRect := Rect(X+FShadowDepth,Y,w,h);
       rMainRect := Rect(x,y,W-FShadowDepth,H);
      end;
     end;
    soUpperRight:
     begin
      {upper right}
      rShadowRect := Rect(X+FShadowDepth,Y,w,h-FShadowDepth);
      rMainRect := Rect(x,y+FShadowDepth,W-FShadowDepth,H);
      if FShape = gstDiamond then
      begin
       rShadowRect := Rect(X+FShadowDepth,Y,w,h);
       rMainRect := Rect(x,y,W-FShadowDepth,H);
      end;
     end;
    soUpperLeft:
     begin
      {upper left}
      rShadowRect := Rect(X,Y,w-FShadowDepth,h-FShadowDepth);
      rMainRect := Rect(x+FShadowDepth,y+FShadowDepth,W,H);
      if FShape = gstDiamond then
      begin
       rMainRect := Rect(X+FShadowDepth,Y,w,h);
       rShadowRect := Rect(x,y,W-FShadowDepth,H);
      end;
     end;
    soLowerLeft:
     begin
      {lower left}
      rShadowRect := Rect(X,Y+FShadowDepth,w-FShadowDepth,h);
      rMainRect := Rect(x+FShadowDepth,y,W,H-FShadowDepth);
      if FShape = gstDiamond then
      begin
       rMainRect := Rect(X+FShadowDepth,Y,w,h);
       rShadowRect := Rect(x,y,W-FShadowDepth,H);
      end;
     end;
    soNone:
     begin
      {none}
      rShadowRect := Rect(0,0,0,0);
      rMainRect := Rect(x,y,w,h);
     end;
    end; {case}
end;

{
  This just let's me call Windows' Polygon, which
  ignores the extra leftover points in the array. I
  know I could use ASM, but I didn't, (Wish they had
  a Slice() in Delphi 16)
}
Procedure TManyShape.PolyGonFromWinProcs(a:array of TPoint;i :integer);
begin
  THackCanvas(canvas).changing;
  Winprocs.polygon(canvas.handle,a,i);
  THackCanvas(canvas).Changed;
end;

{
 Draw the gradient fill. This could really be improved by using
 more colors (see Neil Rubenking's book). Presumes region has been set up.
}
procedure TManyShape.FillWithGradient;
var
  TargetRect : TRect;
  i         : Integer;
  clrFrom,clrTo : TColor;
  RGBFromR,RGBFromG,RGBFromB : Byte;
  RGBDiffR,RGBDiffG,RGBDiffB : integer;

begin
   clrFrom := FGradientToColor;
   clrTo := FGradientToColor;

   Case FFillDirection of
   gdUp: clrFrom := FBackColor;
   gdDown: clrTo := FBackColor;
   gdLeft: clrFrom := FBackColor;
   gdRight: clrTo := FBackColor;
   end;


   RGBFromR := GetRValue (ColorToRGB (ClrFrom));
   RGBFromG := GetGValue (ColorToRGB (ClrFrom));
   RGBFromB := GetBValue (ColorToRGB (ClrFrom));
   RGBDiffR := GetRValue (ColorToRGB (ClrTo)) - RGBFromR;
   RGBDiffG := GetGValue (ColorToRGB (ClrTo)) - RGBFromG;
   RGBDiffB := GetBValue (ColorToRGB (ClrTo)) - RGBFromB;

   Canvas.Pen.Style := psSolid;
   Canvas.Pen.Mode := pmCopy;
   if (FFillDirection = gdLeft) or (FFillDirection = gdRight) then
    begin
     for i := 0 to 255 do
     begin
         TargetRect := Rect(MulDiv (i,Width,256),0,
                            MulDiv (i+1,Width,256),Height);
         Canvas.Brush.Color := RGB (RGBFromR + MulDiv(i,RGBDiffR, 255),
                                    RGBFromG + MulDiv(i,RGBDiffG, 255),
                                    RGBFromB + MulDiv(i,RGBDiffB, 255));
         Canvas.FillRect (TargetRect);
     end;
    end {if left or right}
   else {up down}
    begin
     for i := 0 to $ff do
     begin
         TargetRect := Rect(0,MulDiv (i,Height,256),
                            Width,MulDiv (i+1,Height,256));
         Canvas.Brush.Color := RGB (RGBFromR + MulDiv(i,RGBDiffR,255),
                                    RGBFromG + MulDiv(i,RGBDiffG,255),
                                    RGBFromB + MulDiv(i,RGBDiffB,255));
         Canvas.FillRect (TargetRect);
     end;
   end; {if up or down}
end;

{   presumes region has been set up
    one could add a transparent bitmap property
    and SRCAND and SRCPAINT for a cool effect
}

procedure TManyShape.FillWithBitmap(FImage : TBitmap);
Var
  ARect, BRect : TRect;
  X : Integer;
  Picture,
  Tmp : TBitMap;
  Cx,Cy : Integer;
  TransparentColor : TColor;
Begin
  If FImage.Height > 0 Then Begin
    ARect := Rect(0,0,Width,Height);
    TransparentColor:=ColorTORGB(FImage.Canvas.Pixels[0,FImage.Height-1]);
    X := 0;
    BRect := Rect(X,0, X+Width, Height);


    Picture:=tBitmap.create;
    Picture.Assign(FImage);
    Picture.Width:=Width;
    Picture.height:=Height;
    Picture.Canvas.CopyRect(Rect(0,0,Width,Height),FImage.canvas,Rect(0,0,FImage.Width,FImage.height));


    If (fTransparent) Then Begin
{      Tmp := TBitmap.Create;
      Tmp.Assign(Picture); {Be sure of palette}
{      Tmp.Height := Picture.Height;
      Tmp.Width := Picture.Width;
        Tmp.Canvas.CopyRect(ARect,Picture.Canvas,BRect);  }
        TransparentBlt(Canvas,Picture,ARect.Left,ARect.Top,TransparentColor);
{      Tmp.Free;    }
    End Else  {can draw direct}
      Canvas.CopyRect(ARect, Picture.Canvas, BRect);
    Picture.Free;
  End; { Else Begin
    Canvas.Brush.Color := clWhite;
    Canvas.FillRect(BoundsRect);
  End; }
  If csDesigning In ComponentState Then Begin
    Canvas.Pen.Style := psDash;
    Canvas.Brush.Style := bsClear;
    Canvas.Rectangle(0, 0, Width, Height);
  End;
End;

Procedure TManyShape.TransparentBlt(Dest:TCanvas;Bmp:TBitmap;X,Y:Integer;TransColor:TColor);
var
  srcDC,saveDC,maskDC,invDC,resultDC: HDC;
  hResultBmp,hSaveBmp,hMaskBmp,hInvBmp,
  hSrcPrevBmp,hSavePrevBmp,hDestPrevBmp,hMaskPrevBmp,hInvPrevBmp : hBitmap;
  Success, BH, BW, hB : Integer;
  OC : TColor;
Begin
  BH := BMP.Height; BW := BMP.Width; hB := TBitmap(BMP).Handle;
  srcDC:=CreateCompatibleDC(Dest.handle);
  saveDC:=CreateCompatibleDC(srcDC);
  maskDC:=CreateCompatibleDC(srcDC);
  invDC:=CreateCompatibleDC(srcDC);
  resultDC:=CreateCompatibleDC(srcDC);
  hMaskBmp:=CreateBitmap(BW,BH,1,1,Nil);
  hInvBmp:=CreateBitmap(BW,BH,1,1,Nil);
  hResultBmp:=CreateCompatibleBitmap(Dest.handle,BW,BH);
  hSaveBmp:=CreateCompatibleBitmap(hResultBMP,BW,BH);
  hSrcPrevBmp:=SelectObject(srcDC,hB);
  hSavePrevBmp:=SelectObject(saveDC,hSaveBmp);
  hMaskPrevBmp:=SelectObject(maskDC,hMaskBmp);
  hInvPrevBmp:=SelectObject(invDC,hInvBmp);
  hDestPrevBmp:=SelectObject(resultDC,hResultBmp);
  BitBlt(saveDC,0,0,BW,BH,srcDC,0, 0, SRCCOPY);
  OC:=SetBkColor(srcDC,TransColor);
  BitBlt(maskDC,0,0,BW,BH,srcDC,0,0,SRCCOPY);
  TransColor := SetBkColor(srcDC, OC);
  BitBlt(invDC,0,0,BW,BH,maskDC,0,0,NOTSRCCOPY);
  BitBlt(resultDC,0,0,BW,BH,Dest.handle,X,Y,SRCCOPY);
  BitBlt(resultDC,0,0,BW,BH,maskDC,0,0,SRCAND);
  BitBlt(srcDC,0,0,BW,BH,invDC,0,0,SRCAND);
  BitBlt(resultDC,0,0,BW,BH,srcDC,0,0,SRCPAINT);
  BitBlt(Dest.handle, X, Y, BW, BH, resultDC, 0, 0, SRCCOPY);
  BitBlt(srcDC,0,0,BW,BH,saveDC,0,0,SRCCOPY);
  SelectObject(srcDC,hSrcPrevBmp);SelectObject(saveDC,hSavePrevBmp);
  SelectObject(resultDC,hDestPrevBmp);SelectObject(maskDC,hMaskPrevBmp);
  SelectObject(invDC,hInvPrevBmp);
  DeleteObject(hSaveBmp);DeleteObject(hMaskBmp);DeleteObject(hInvBmp);DeleteObject(hResultBmp);
  DeleteDC(srcDC);DeleteDC(saveDC);DeleteDC(invDC);DeleteDC(maskDC);DeleteDC(resultDC);
End;



{
  Set the canvas brush and pen properties
  for drawing the shadow
 }
procedure TManyShape.SetPenBrushForShadow;
begin
     canvas.brush.color := FShadowColor;
     canvas.brush.style := bsSolid;
     canvas.pen.style := psSolid;
     canvas.pen.width := 1;
     canvas.pen.color := clBlack;
end;

{
  Set the canvas brush and pen properties
  for drawing the background
 }
procedure TManyShape.SetPenBrushForBackground;
begin
       canvas.brush.color := FBackColor;
       canvas.brush.style :=bsSolid;
       canvas.pen.color := FOutlinecolor;
       canvas.pen.width := FOutlinewidth;
       canvas.pen.style := FOutlinestyle;
end;

{
  Set the canvas brush and pen properties
  for drawing the foreground
 }
procedure TManyShape.SetPenBrushForForeground;
begin
     if FState = 1 then
       canvas.brush.color := FColour1
     else if Fstate = 2 then
       canvas.brush.color := FColour2
     else
       canvas.brush.color := FBackColor;
    canvas.brush.style := bsSolid;
    canvas.pen.color := FOutlinecolor;
    canvas.pen.width := FOutlinewidth;
    canvas.pen.style := FOutlinestyle;
end;

{
  Set the canvas brush and pen properties
  for drawing the outline
 }
procedure TManyShape.SetPenBrushForOutline;
begin
    canvas.Brush.style := bsClear;
    canvas.pen.color := FOutlinecolor;
    canvas.pen.width := FOutlinewidth;
    canvas.pen.style := FOutlinestyle;
end;

{
  Set the canvas brush and pen properties
  for drawing the brush image
 }
procedure TManyShape.SetPenBrushForImage;
begin
     canvas.Brush.style := bsSolid;
end;

{
  Set the canvas brush and pen properties
  for drawing the gradient fill
 }
procedure TManyShape.SetPenBrushForGradient;
begin
     canvas.Brush.style := bsSolid;
end;



procedure Register;
begin
  RegisterComponents('PD', [TManyShape]);
end;


end.
