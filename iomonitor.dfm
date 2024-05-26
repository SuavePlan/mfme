object IOMonitor: TIOMonitor
  Left = 407
  Top = 212
  BorderStyle = bsDialog
  Caption = 'IO Monitor'
  ClientHeight = 522
  ClientWidth = 346
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poMainFormCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Select: TTabControl
    Left = 0
    Top = 0
    Width = 345
    Height = 521
    TabOrder = 0
    Tabs.Strings = (
      'MPU3'
      'MPU4'
      'IMPACT'
      'M1A')
    TabIndex = 0
    OnChange = SelectChange
    object Triacs: TGroupBox
      Left = 8
      Top = 32
      Width = 177
      Height = 49
      Caption = 'Triacs - White'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object Triac8: TShape
        Left = 123
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac7: TShape
        Left = 107
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac6: TShape
        Left = 91
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac5: TShape
        Left = 75
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac4: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac3: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac2: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac1: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac9: TShape
        Left = 139
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Triac10: TShape
        Left = 155
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Meters: TGroupBox
      Left = 192
      Top = 32
      Width = 145
      Height = 49
      Caption = 'Meters - Yellow'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object Meter8: TShape
        Left = 123
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter7: TShape
        Left = 107
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter6: TShape
        Left = 91
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter5: TShape
        Left = 75
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter4: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter3: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter2: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Meter1: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Reels: TGroupBox
      Left = 8
      Top = 88
      Width = 329
      Height = 105
      Caption = 'Reel Drives - Red'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      object Reel18: TShape
        Left = 227
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel17: TShape
        Left = 243
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel16: TShape
        Left = 259
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel15: TShape
        Left = 275
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel14: TShape
        Left = 155
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel13: TShape
        Left = 171
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel12: TShape
        Left = 187
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel11: TShape
        Left = 203
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel28: TShape
        Left = 83
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel27: TShape
        Left = 99
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel26: TShape
        Left = 115
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel25: TShape
        Left = 131
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel24: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel23: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel22: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel21: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Pos1: TLabel
        Left = 32
        Top = 40
        Width = 16
        Height = 15
        Caption = '00'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Pos2: TLabel
        Left = 104
        Top = 40
        Width = 16
        Height = 15
        Caption = '00'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Pos3: TLabel
        Left = 176
        Top = 40
        Width = 16
        Height = 15
        Caption = '00'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Pos4: TLabel
        Left = 248
        Top = 40
        Width = 16
        Height = 15
        Caption = '00'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Reel34: TShape
        Left = 11
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel33: TShape
        Left = 27
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel32: TShape
        Left = 43
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel31: TShape
        Left = 59
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Pos5: TLabel
        Left = 32
        Top = 80
        Width = 16
        Height = 15
        Caption = '00'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Reel38: TShape
        Left = 83
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel37: TShape
        Left = 99
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel36: TShape
        Left = 115
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Reel35: TShape
        Left = 131
        Top = 59
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Pos6: TLabel
        Left = 104
        Top = 80
        Width = 16
        Height = 15
        Caption = '00'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
    end
    object Sinks: TGroupBox
      Left = 8
      Top = 200
      Width = 329
      Height = 49
      Caption = 'Lamp Selects - Blue'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 3
      object LampSel1: TShape
        Left = 19
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel2: TShape
        Left = 35
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel3: TShape
        Left = 51
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel4: TShape
        Left = 67
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel5: TShape
        Left = 83
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel6: TShape
        Left = 99
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel7: TShape
        Left = 115
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel8: TShape
        Left = 131
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel9: TShape
        Left = 147
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel10: TShape
        Left = 163
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel11: TShape
        Left = 179
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel12: TShape
        Left = 195
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel13: TShape
        Left = 211
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel14: TShape
        Left = 227
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel15: TShape
        Left = 243
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampSel16: TShape
        Left = 259
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Selects: TGroupBox
      Left = 8
      Top = 256
      Width = 329
      Height = 49
      Caption = 'Lamp Drives - Green'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      object LampDrv1: TShape
        Left = 19
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv2: TShape
        Left = 35
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv3: TShape
        Left = 51
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv4: TShape
        Left = 67
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv5: TShape
        Left = 83
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv6: TShape
        Left = 99
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv7: TShape
        Left = 115
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv8: TShape
        Left = 131
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv9: TShape
        Left = 147
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv10: TShape
        Left = 163
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv11: TShape
        Left = 179
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv12: TShape
        Left = 195
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv13: TShape
        Left = 211
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv14: TShape
        Left = 227
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv15: TShape
        Left = 243
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object LampDrv16: TShape
        Left = 259
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Switches1: TGroupBox
      Left = 10
      Top = 312
      Width = 327
      Height = 49
      Caption = 'Switches - Orange'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = 33023
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      object Switch11: TShape
        Left = 19
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch12: TShape
        Left = 35
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch13: TShape
        Left = 51
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch14: TShape
        Left = 67
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch15: TShape
        Left = 83
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch16: TShape
        Left = 99
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch17: TShape
        Left = 115
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch18: TShape
        Left = 131
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch19: TShape
        Left = 147
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch110: TShape
        Left = 163
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch111: TShape
        Left = 179
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch112: TShape
        Left = 195
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch113: TShape
        Left = 211
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch114: TShape
        Left = 227
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch115: TShape
        Left = 243
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch116: TShape
        Left = 259
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Switches2: TGroupBox
      Left = 10
      Top = 368
      Width = 327
      Height = 49
      Caption = 'Switches - Black'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 6
      object Switch21: TShape
        Left = 19
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch22: TShape
        Left = 35
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch23: TShape
        Left = 51
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch24: TShape
        Left = 67
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch25: TShape
        Left = 83
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch26: TShape
        Left = 99
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch27: TShape
        Left = 115
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch28: TShape
        Left = 131
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch29: TShape
        Left = 147
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch210: TShape
        Left = 163
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch211: TShape
        Left = 179
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch212: TShape
        Left = 195
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch213: TShape
        Left = 211
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch214: TShape
        Left = 227
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch215: TShape
        Left = 243
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Switch216: TShape
        Left = 259
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Aux2: TGroupBox
      Left = 192
      Top = 424
      Width = 145
      Height = 41
      Caption = 'Aux2 - White'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 7
      object Aux21: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux22: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux23: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux24: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux25: TShape
        Left = 75
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux26: TShape
        Left = 91
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux27: TShape
        Left = 107
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux28: TShape
        Left = 123
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Aux3: TGroupBox
      Left = 194
      Top = 472
      Width = 145
      Height = 41
      Caption = 'Aux3 - White'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWhite
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 8
      object Aux31: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux32: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux33: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux34: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux35: TShape
        Left = 75
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux36: TShape
        Left = 91
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux37: TShape
        Left = 107
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux38: TShape
        Left = 123
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object LEDS: TGroupBox
      Left = 8
      Top = 472
      Width = 145
      Height = 41
      Caption = '7segs - Yellow'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clYellow
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 9
      object seg1: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg2: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg3: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg4: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg5: TShape
        Left = 75
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg6: TShape
        Left = 91
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg7: TShape
        Left = 107
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object seg8: TShape
        Left = 123
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
    object Aux1: TGroupBox
      Left = 8
      Top = 424
      Width = 145
      Height = 41
      Caption = 'Aux1 - Black'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 10
      object Aux11: TShape
        Left = 11
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux12: TShape
        Left = 27
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux13: TShape
        Left = 43
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux14: TShape
        Left = 59
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux15: TShape
        Left = 75
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux16: TShape
        Left = 91
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux17: TShape
        Left = 107
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
      object Aux18: TShape
        Left = 123
        Top = 19
        Width = 14
        Height = 13
        Brush.Color = clBtnFace
        ParentShowHint = False
        Shape = stCircle
        ShowHint = True
      end
    end
  end
end
