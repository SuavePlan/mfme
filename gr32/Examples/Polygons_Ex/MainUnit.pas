unit MainUnit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  GR32, GR32_Image, GR32_Layers, GR32_Polygons, StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    Image: TImage32;
    Panel1: TPanel;
    Antialiase: TCheckBox;
    Label1: TLabel;
    LineAlpha: TScrollBar;
    Label2: TLabel;
    FillAlpha: TScrollBar;
    Memo1: TMemo;
    FillMode: TRadioGroup;
    Button1: TButton;
    LineThickness: TScrollBar;
    Label3: TLabel;
    ThickOutline: TCheckBox;
    Label4: TLabel;
    BitmapList: TBitmap32List;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure ImageMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer; Layer: TCustomLayer);
    procedure ImageResize(Sender: TObject);
    procedure ParamsChanged(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure ThicknessChanged(Sender: TObject);
  private
    Polygon: TPolygon32;
    Outline: TPolygon32;
    UseOutlinePoly: Boolean;
    LineSize: Single;
    procedure Build;
    procedure Draw;
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Draw;
begin
  Image.Bitmap.BeginUpdate;
  Image.Bitmap.Clear(clWhite32);
  Image.Bitmap.Draw(50, 50, BitmapList.Bitmaps[0].Bitmap);
  Polygon.Antialiased := Antialiase.Checked;
  if UseOutlinePoly then Outline.Antialiased := Antialiase.Checked;
  if FillMode.ItemIndex = 0 then Polygon.FillMode := pfAlternate else Polygon.FillMode := pfWinding;

  Polygon.DrawFill(Image.Bitmap, SetAlpha(clGreen32, FillAlpha.Position));

  if UseOutlinePoly then
    Outline.DrawFill(Image.Bitmap, SetAlpha(clBlack32, LineAlpha.Position))
  else
    Polygon.DrawEdge(Image.Bitmap, SetAlpha(clBlack32, LineAlpha.Position));

  Image.Bitmap.EndUpdate;
  Image.Bitmap.Changed;
  Image.Refresh; // force repaint
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  Image.SetupBitmap;
  Polygon := TPolygon32.Create;
end;

procedure TForm1.FormDestroy(Sender: TObject);
begin
  Outline.Free;
  Polygon.Free;
end;

procedure TForm1.ImageMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer; Layer: TCustomLayer);
begin
  if Button = mbLeft then Polygon.Add(FixedPoint(X, Y))
  else Polygon.Clear;
  Build;
  Draw;
end;

procedure TForm1.ImageResize(Sender: TObject);
begin
  Image.SetupBitmap;
  Build;
  Draw;
end;

procedure TForm1.ParamsChanged(Sender: TObject);
begin
  Draw;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  Polygon.NewLine;
end;

procedure TForm1.Build;
var
  TmpPoly: TPolygon32;
begin
  Outline.Free;
  Outline := nil;

  if UseOutlinePoly then
  begin
    TmpPoly := Polygon.Outline;
    Outline := TmpPoly.Grow(Fixed(LineSize / 2), 0.5);
    Outline.FillMode := pfWinding;
    TmpPoly.Free;
  end;

  if UseOutlinePoly then
    Label4.Caption := Format('(%.1f)', [LineSize])
  else
    Label4.Caption := '(1)';
end;

procedure TForm1.ThicknessChanged(Sender: TObject);
begin
  UseOutlinePoly := ThickOutline.Checked;
  LineSize := LineThickness.Position / 10;
  Build;
  Draw;
end;

end.
