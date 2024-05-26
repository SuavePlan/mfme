object CreateBandForm: TCreateBandForm
  Left = 702
  Top = 360
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Create Reel Band'
  ClientHeight = 141
  ClientWidth = 180
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 18
    Width = 32
    Height = 15
    Caption = 'Reel'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 45
    Width = 40
    Height = 15
    Caption = 'Stops'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Apply: TSpeedButton
    Left = 58
    Top = 104
    Width = 73
    Height = 25
    Caption = '&Apply'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = ApplyClick
  end
  object Reel: TComboBox
    Left = 72
    Top = 12
    Width = 57
    Height = 23
    Style = csDropDownList
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ItemHeight = 15
    ParentFont = False
    TabOrder = 0
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7')
  end
  object Stops: TComboBox
    Left = 72
    Top = 40
    Width = 57
    Height = 23
    Style = csDropDownList
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ItemHeight = 15
    ParentFont = False
    TabOrder = 1
    Items.Strings = (
      '8'
      '12'
      '16'
      '20'
      '24'
      '25')
  end
  object Horizontal: TCheckBox
    Left = 8
    Top = 78
    Width = 105
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Horizontal'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
end
