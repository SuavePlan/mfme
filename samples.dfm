object Sampler: TSampler
  Left = 484
  Top = 308
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Samples'
  ClientHeight = 273
  ClientWidth = 129
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poMainFormCenter
  Visible = True
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton1: TSpeedButton
    Left = 12
    Top = 184
    Width = 45
    Height = 25
    Caption = 'Play'
    OnClick = SpeedButton1Click
  end
  object Label1: TLabel
    Left = 16
    Top = 8
    Width = 17
    Height = 13
    Caption = 'Nbr'
  end
  object Label2: TLabel
    Left = 48
    Top = 8
    Width = 33
    Height = 13
    Caption = 'Length'
  end
  object Save: TSpeedButton
    Left = 76
    Top = 184
    Width = 45
    Height = 25
    Caption = 'Save'
    OnClick = SaveClick
  end
  object Apply: TSpeedButton
    Left = 76
    Top = 240
    Width = 45
    Height = 25
    Caption = 'Apply'
    Enabled = False
    OnClick = ApplyClick
  end
  object Label3: TLabel
    Left = 24
    Top = 224
    Width = 23
    Height = 13
    Caption = 'Rate'
  end
  object ListBox1: TListBox
    Left = 8
    Top = 24
    Width = 113
    Height = 145
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ItemHeight = 15
    ParentFont = False
    TabOrder = 0
  end
  object Rate: TEdit
    Left = 11
    Top = 240
    Width = 54
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 5
    ParentFont = False
    TabOrder = 1
    Text = '16000'
    OnChange = RateChange
  end
end
