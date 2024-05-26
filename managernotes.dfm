object GameNotes: TGameNotes
  Left = 557
  Top = 536
  BorderStyle = bsDialog
  Caption = 'Game Notes'
  ClientHeight = 179
  ClientWidth = 557
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 8
    Top = 36
    Width = 66
    Height = 13
    Caption = 'Manufacturer:'
  end
  object Label4: TLabel
    Left = 8
    Top = 60
    Width = 68
    Height = 13
    Caption = 'Release Date:'
  end
  object Label5: TLabel
    Left = 8
    Top = 108
    Width = 71
    Height = 13
    Caption = 'Machine Type:'
  end
  object Label6: TLabel
    Left = 8
    Top = 84
    Width = 59
    Height = 13
    Caption = 'Technology:'
  end
  object Label7: TLabel
    Left = 8
    Top = 132
    Width = 36
    Height = 13
    Caption = 'Stakes:'
  end
  object Label8: TLabel
    Left = 8
    Top = 156
    Width = 31
    Height = 13
    Caption = 'Prizes:'
  end
  object SpeedButton1: TSpeedButton
    Left = 408
    Top = 144
    Width = 65
    Height = 25
    Caption = 'Save'
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 480
    Top = 144
    Width = 65
    Height = 25
    Caption = 'Cancel'
    OnClick = SpeedButton2Click
  end
  object Label1: TLabel
    Left = 240
    Top = 20
    Width = 91
    Height = 13
    Caption = 'Comments / Notes:'
  end
  object Label2: TLabel
    Left = 8
    Top = 12
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object Manufacturer: TEdit
    Left = 96
    Top = 32
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 1
  end
  object ReleaseDate: TEdit
    Left = 96
    Top = 56
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 2
  end
  object MachineType: TEdit
    Left = 96
    Top = 104
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 4
  end
  object Technology: TEdit
    Left = 96
    Top = 80
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 3
  end
  object Stakes: TEdit
    Left = 96
    Top = 128
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 5
  end
  object Prizes: TEdit
    Left = 96
    Top = 152
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 6
  end
  object Notes: TMemo
    Left = 240
    Top = 42
    Width = 300
    Height = 97
    ScrollBars = ssVertical
    TabOrder = 7
  end
  object Name: TEdit
    Left = 96
    Top = 8
    Width = 113
    Height = 21
    MaxLength = 90
    TabOrder = 0
  end
end
