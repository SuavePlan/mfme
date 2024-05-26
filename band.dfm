object LoadBandForm: TLoadBandForm
  Left = 541
  Top = 362
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Load Band'
  ClientHeight = 130
  ClientWidth = 149
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
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
    Left = 40
    Top = 96
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
    TabOrder = 0
    Items.Strings = (
      '8'
      '12'
      '16'
      '20'
      '24'
      '25')
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
    TabOrder = 1
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7')
  end
  object Horizontal: TCheckBox
    Left = 24
    Top = 72
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
