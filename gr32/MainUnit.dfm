object Form1: TForm1
  Left = 133
  Top = 461
  Width = 579
  Height = 362
  Caption = 'BlendMode Ex'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 425
    Top = 40
    Width = 52
    Height = 13
    Caption = 'Blendmode'
  end
  object Label2: TLabel
    Left = 425
    Top = 85
    Width = 73
    Height = 13
    Caption = 'Opacity: 100%'
  end
  object Label3: TLabel
    Left = 435
    Top = 190
    Width = 50
    Height = 13
    Caption = 'Blendmap:'
  end
  object ImgView: TImgView32
    Left = 6
    Top = 5
    Width = 415
    Height = 326
    Anchors = [akLeft, akTop, akRight, akBottom]
    Color = clBtnShadow
    ParentColor = False
    Scale = 1
    ScrollBars.Color = clBtnShadow
    ScrollBars.ShowHandleGrip = True
    ScrollBars.Style = rbsDefault
    SizeGrip = sgAuto
    TabOrder = 0
    OnMouseMove = ImgViewMouseMove
    OnMouseUp = ImgViewMouseUp
  end
  object BlendComboBox: TComboBox
    Left = 430
    Top = 55
    Width = 136
    Height = 21
    DropDownCount = 100
    ItemHeight = 13
    TabOrder = 1
    Text = 'Normal'
    OnChange = BlendComboBoxChange
  end
  object MasterAlpha: TGaugeBar
    Left = 430
    Top = 100
    Width = 136
    Height = 16
    Backgnd = bgPattern
    Max = 255
    ShowHandleGrip = True
    Position = 0
    OnChange = MasterAlphaChange
  end
  object Button1: TButton
    Left = 430
    Top = 145
    Width = 136
    Height = 25
    Caption = 'Load Custom Blendmap'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Img: TImage32
    Left = 445
    Top = 210
    Width = 100
    Height = 100
    BitmapAlign = baTopLeft
    Color = clBlack
    ParentColor = False
    Scale = 1
    ScaleMode = smStretch
    TabOrder = 4
  end
  object OPD: TOpenPictureDialog
    Left = 15
    Top = 15
  end
end
