object Form2: TForm2
  Left = 634
  Top = 344
  Width = 800
  Height = 600
  ActiveControl = Image
  BorderIcons = [biMinimize]
  Caption = 'Rendered Screen'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Image: TImage32
    Left = 0
    Top = 0
    Width = 792
    Height = 566
    Align = alClient
    Bitmap.StretchFilter = sfLinear
    BitmapAlign = baTopLeft
    Scale = 1
    ScaleMode = smStretch
    TabOrder = 0
    OnKeyDown = ImageKeyDown
    OnKeyUp = ImageKeyUp
    OnMouseDown = ImageMouseDown
    OnMouseMove = ImageMouseMove
    OnMouseUp = ImageMouseUp
  end
end
