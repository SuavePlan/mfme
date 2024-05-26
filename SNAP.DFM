object SnapForm: TSnapForm
  Left = 502
  Top = 361
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Snap To Grid'
  ClientHeight = 103
  ClientWidth = 159
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
    Top = 11
    Width = 72
    Height = 15
    Caption = 'X Spacing'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton1: TSpeedButton
    Left = 56
    Top = 72
    Width = 57
    Height = 25
    Caption = 'Apply'
    OnClick = SpeedButton1Click
  end
  object Label2: TLabel
    Left = 24
    Top = 42
    Width = 72
    Height = 15
    Caption = 'Y Spacing'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object X: TEdit
    Left = 104
    Top = 8
    Width = 25
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 2
    ParentFont = False
    TabOrder = 0
    Text = '0'
  end
  object Y: TEdit
    Left = 104
    Top = 40
    Width = 25
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 2
    ParentFont = False
    TabOrder = 1
    Text = '0'
  end
end
