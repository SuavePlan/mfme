object GeneralConfig: TGeneralConfig
  Left = 357
  Top = 705
  BorderStyle = bsDialog
  Caption = 'General Config'
  ClientHeight = 364
  ClientWidth = 516
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Draggablereels: TCheckBox
    Left = 8
    Top = 16
    Width = 105
    Height = 17
    Caption = 'Draggable Reels'
    TabOrder = 0
    OnClick = DraggablereelsClick
  end
  object ConvertReel: TCheckBox
    Left = 8
    Top = 42
    Width = 89
    Height = 17
    Caption = 'Convert Reel'
    TabOrder = 1
    OnClick = ConvertReelClick
  end
  object MeterPanelResize: TCheckBox
    Left = 8
    Top = 92
    Width = 137
    Height = 17
    Caption = 'Meter Panel Off Resize'
    TabOrder = 2
    OnClick = MeterPanelResizeClick
  end
  object NoAboutBox: TCheckBox
    Left = 8
    Top = 116
    Width = 145
    Height = 17
    Caption = 'No About box on Start-up'
    TabOrder = 3
    OnClick = NoAboutBoxClick
  end
  object StartInGameManager: TCheckBox
    Left = 8
    Top = 164
    Width = 137
    Height = 17
    Caption = 'Start in Game Manager'
    TabOrder = 4
    OnClick = StartInGameManagerClick
  end
  object MeterPanelOff: TCheckBox
    Left = 8
    Top = 68
    Width = 97
    Height = 17
    Caption = 'Meter Panel Off'
    TabOrder = 5
    OnClick = MeterPanelOffClick
  end
  object LongTermMeters: TCheckBox
    Left = 8
    Top = 140
    Width = 113
    Height = 17
    Caption = 'Long Term Meters'
    TabOrder = 6
    OnClick = LongTermMetersClick
  end
  object FileNames: TCheckBox
    Left = 8
    Top = 188
    Width = 185
    Height = 17
    Caption = 'Use Filenames in Game Manager'
    TabOrder = 7
    OnClick = FileNamesClick
  end
  object ActiveInEdit: TCheckBox
    Left = 8
    Top = 212
    Width = 153
    Height = 17
    Caption = 'Buttons active in Edit Mode'
    TabOrder = 8
    OnClick = ActiveInEditClick
  end
  object EscQuitsProgram: TCheckBox
    Left = 8
    Top = 236
    Width = 113
    Height = 17
    Caption = 'ESC quits program'
    TabOrder = 9
    OnClick = EscQuitsProgramClick
  end
  object HighPriority: TCheckBox
    Left = 8
    Top = 260
    Width = 145
    Height = 17
    Caption = 'High Priority ( 100% CPU )'
    TabOrder = 10
    OnClick = HighPriorityClick
  end
end
