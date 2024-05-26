object ScanWindow: TScanWindow
  Left = 653
  Top = 508
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Scanning ...'
  ClientHeight = 110
  ClientWidth = 216
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label9: TLabel
    Left = 32
    Top = 14
    Width = 72
    Height = 15
    Caption = 'Scanned :'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Files: TLabel
    Left = 112
    Top = 14
    Width = 56
    Height = 15
    Caption = '0 files'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 32
    Top = 38
    Width = 72
    Height = 15
    Caption = 'Found   :'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Found: TLabel
    Left = 112
    Top = 38
    Width = 8
    Height = 15
    Caption = '0'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Cancel: TButton
    Left = 72
    Top = 72
    Width = 81
    Height = 25
    Caption = 'Cancel'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    TabStop = False
    OnClick = CancelClick
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 10
    OnTimer = Timer1Timer
    Left = 184
    Top = 8
  end
end
