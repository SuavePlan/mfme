unit MainUnit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  GR32, GR32_Image, GR32_Layers, GR32_Blend, StdCtrls, ExtCtrls;

type
  TForm1 = class(TForm)
    ImgView: TImgView32;
    RadioGroup1: TRadioGroup;
    procedure FormCreate(Sender: TObject);
    procedure RadioGroup1Click(Sender: TObject);
  protected
    procedure PC_Add(F: TColor32; var B: TColor32; M: TColor32);
    procedure PC_Sub(F: TColor32; var B: TColor32; M: TColor32);
    procedure PC_Modulate(F: TColor32; var B: TColor32; M: TColor32);
    procedure PC_Min(F: TColor32; var B: TColor32; M: TColor32);
    procedure PC_Max(F: TColor32; var B: TColor32; M: TColor32);
    procedure PC_Pattern(F: TColor32; var B: TColor32; M: TColor32);
  public
    PatCount: Integer;
    L: TBitmapLayer;
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.FormCreate(Sender: TObject);
var
  I, J: Integer;
begin
  L := TBitmapLayer.Create(ImgView.Layers);
  L.Bitmap.SetSize(200, 200);
  L.Bitmap.DrawMode := dmCustom;
  L.Location := FloatRect(20, 20, 220, 220);
  for J := 0 to 199 do
    for I := 0 to 199 do
      L.Bitmap[I, J] := Gray32(Round(((Sin(I / 10) + Sin(J / 10)) * 0.25 + 0.5) * 255));
  L.Bitmap.OnPixelCombine := nil; // none by default
end;

procedure TForm1.PC_Add(F: TColor32; var B: TColor32; M: TColor32);
begin
  B := ColorAdd(F, B);
end;

procedure TForm1.PC_Max(F: TColor32; var B: TColor32; M: TColor32);
begin
  B := ColorMax(F, B);
end;

procedure TForm1.PC_Min(F: TColor32; var B: TColor32; M: TColor32);
begin
  B := ColorMin(F, B);
end;

procedure TForm1.PC_Modulate(F: TColor32; var B: TColor32; M: TColor32);
begin
  B := ColorModulate(F, B);
end;

procedure TForm1.PC_Pattern(F: TColor32; var B: TColor32; M: TColor32);
begin
  PatCount := 1 - PatCount;
  if PatCount = 0 then B := F;
end;

procedure TForm1.PC_Sub(F: TColor32; var B: TColor32; M: TColor32);
begin
  B := ColorSub(F, B);
end;

procedure TForm1.RadioGroup1Click(Sender: TObject);
begin
  case RadioGroup1.ItemIndex of
    0: L.Bitmap.OnPixelCombine := nil;
    1: L.Bitmap.OnPixelCombine := PC_Add;
    2: L.Bitmap.OnPixelCombine := PC_Sub;
    3: L.Bitmap.OnPixelCombine := PC_Modulate;
    4: L.Bitmap.OnPixelCombine := PC_Min;
    5: L.Bitmap.OnPixelCombine := PC_Max;
    6: L.Bitmap.OnPixelCombine := PC_Pattern;
  end;
  L.Bitmap.Changed;
end;

end.
