unit MainUnit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  GR32, GR32_Image, GR32_Layers, StdCtrls, ExtCtrls, GR32_Add_Blendmodes,
  GR32_RangeBars, JPeg, ExtDlgs;

type
  TForm1 = class(TForm)
    ImgView: TImgView32;
    BlendComboBox: TComboBox;
    Label1: TLabel;
    MasterAlpha: TGaugeBar;
    Label2: TLabel;
    Button1: TButton;
    OPD: TOpenPictureDialog;
    Img: TImage32;
    Label3: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure MasterAlphaChange(Sender: TObject);
    procedure ImgViewMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer; Layer: TCustomLayer);
    procedure BlendComboBoxChange(Sender: TObject);
    procedure ImgViewMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer; Layer: TCustomLayer);
    procedure Button1Click(Sender: TObject);
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

const
     W = 200;
     H = 150;

procedure TForm1.FormCreate(Sender: TObject);
var Pos: TPoint;
    L: TBitmapLayer;
begin
  ImgView.Bitmap.LoadFromFile(ExtractFilePath(Application.Exename)+'background.jpg');
  Pos:= ImgView.BitmapToControl(Point(0,0));
  L := TBitmapLayer.Create(ImgView.Layers);
  L.Location := FloatRect(Pos.x, Pos.y, Pos.x+W, Pos.y+H);
  L.Bitmap.LoadFromFile(ExtractFilePath(Application.Exename)+'foreground.jpg');
  L.Bitmap.DrawMode := dmCustom;
  L.Bitmap.OnPixelCombine := BlendMode.Normal;
  ImgView.Layers.MouseListener := L;
  BlendComboBox.Items:= BlendModeList;
  Img.Bitmap:= TBitmap32.Create;
  Img.Bitmap.SetSize(256,256);
end;

procedure TForm1.MasterAlphaChange(Sender: TObject);
begin
 if ImgView.Layers.MouseListener is TBitmapLayer then
  with ImgView.Layers.MouseListener as TBitmapLayer do
   Bitmap.MasterAlpha:= 255-MasterAlpha.Position;
 Label2.Caption := 'Opacity: '+IntToStr(Round((255-MasterAlpha.Position)*(100/255)))+'%';
end;

procedure TForm1.ImgViewMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer; Layer: TCustomLayer);
var Pos: TFloatRect;
begin
if ssLeft in Shift then
 if Layer is TPositionedLayer then
  with Layer as TPositionedLayer do
   begin
    Pos:= GetAdjustedLocation;
    Pos.Left   :=  x-W/2;  Pos.Right  :=  x+W/2;
    Pos.Top    :=  y-H/2;  Pos.Bottom :=  y+H/2;
    Location   := Pos;
   end;
end;

procedure TForm1.BlendComboBoxChange(Sender: TObject);
begin
 if ImgView.Layers.MouseListener is TBitmapLayer then
  with ImgView.Layers.MouseListener as TBitmapLayer do
   begin
    Bitmap.OnPixelCombine:= GetBlendMode(BlendComboBox.ItemIndex);
    BlendmapToBitmap(RenderBlendmap(Bitmap.OnPixelCombine),Img.Bitmap);
    Img.Repaint;
    Bitmap.Changed;
   end; 
end;

procedure TForm1.ImgViewMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer; Layer: TCustomLayer);
begin
 ImgView.Layers.MouseListener:= Layer;
end;

procedure TForm1.Button1Click(Sender: TObject);
var Tmp : TBitmap32;
begin
 if OPD.Execute then
  begin
   Tmp:= TBitmap32.Create;
   Tmp.LoadFromFile(OPD.Filename);
   BitmapToBlendmap(Tmp,Blendmap);
   BlendComboBox.ItemIndex:= GetIndex(BlendMode.Custom);
   BlendComboBoxChange(self);
   Tmp.Free;
  end;
end;

end.
