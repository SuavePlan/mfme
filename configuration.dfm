object Config: TConfig
  Left = 613
  Top = 194
  BorderStyle = bsDialog
  Caption = 'Game Configuration'
  ClientHeight = 566
  ClientWidth = 568
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poMainFormCenter
  ShowHint = True
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 450
    Width = 568
    Height = 116
    Align = alBottom
    TabOrder = 0
    object Reset: TSpeedButton
      Left = 80
      Top = 82
      Width = 49
      Height = 25
      Caption = 'Reset'
      OnClick = ResetClick
    end
    object Ramclear: TSpeedButton
      Left = 8
      Top = 82
      Width = 65
      Height = 25
      Caption = 'RAM Clear'
      OnClick = RamclearClick
    end
    object GroupBox3: TGroupBox
      Left = 8
      Top = 4
      Width = 121
      Height = 73
      Caption = 'Speed'
      TabOrder = 0
      object Label48: TLabel
        Left = 6
        Top = 20
        Width = 32
        Height = 15
        Caption = 'CPU1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label49: TLabel
        Left = 6
        Top = 44
        Width = 32
        Height = 15
        Caption = 'CPU2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object TDelay: TEdit
        Left = 48
        Top = 16
        Width = 65
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = '0'
        OnChange = TDelayChange
      end
      object Delay2: TEdit
        Left = 48
        Top = 40
        Width = 65
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Text = '0'
        OnChange = Delay2Change
      end
    end
    object GroupBox4: TGroupBox
      Left = 136
      Top = 4
      Width = 233
      Height = 109
      Caption = 'Game Details'
      TabOrder = 1
      object Label45: TLabel
        Left = 6
        Top = 20
        Width = 56
        Height = 15
        Caption = 'Caption'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label46: TLabel
        Left = 6
        Top = 50
        Width = 32
        Height = 15
        Caption = 'Name'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label47: TLabel
        Left = 6
        Top = 80
        Width = 56
        Height = 15
        Caption = 'Version'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object TGameName: TEdit
        Tag = 1
        Left = 72
        Top = 16
        Width = 153
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 100
        ParentFont = False
        TabOrder = 0
        OnChange = TGameNameChange
      end
      object TGameVersion: TEdit
        Left = 72
        Top = 75
        Width = 65
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 7
        ParentFont = False
        TabOrder = 1
        OnChange = TGameVersionChange
        OnKeyPress = TGameVersionKeyPress
      end
      object ShortName: TEdit
        Tag = 1
        Left = 72
        Top = 45
        Width = 153
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 100
        ParentFont = False
        TabOrder = 2
        OnChange = ShortNameChange
      end
    end
    object GroupBox7: TGroupBox
      Left = 378
      Top = 4
      Width = 177
      Height = 109
      Caption = 'Statistics'
      TabOrder = 2
      object Label31: TLabel
        Left = 6
        Top = 24
        Width = 64
        Height = 15
        Caption = 'Total In'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label32: TLabel
        Left = 7
        Top = 50
        Width = 72
        Height = 15
        Caption = 'Total Out'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label33: TLabel
        Left = 7
        Top = 78
        Width = 80
        Height = 15
        Caption = 'Percentage'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object TPercentage: TEdit
        Left = 96
        Top = 76
        Width = 25
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 0
        OnChange = MeterChange
      end
      object InMeter: TEdit
        Left = 96
        Top = 20
        Width = 73
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 8
        ParentFont = False
        TabOrder = 1
        Text = '0'
        OnChange = MeterChange
      end
      object OutMeter: TEdit
        Left = 96
        Top = 48
        Width = 73
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 8
        ParentFont = False
        TabOrder = 2
        Text = '0'
        OnChange = MeterChange
      end
    end
  end
  object Select: TTabControl
    Left = 0
    Top = 0
    Width = 569
    Height = 449
    TabOrder = 1
    Tabs.Strings = (
      'MPU3'
      'MPU4'
      'IMPACT'
      'M1A/B'
      'MPS2'
      'SYS1'
      'SCORPION I'
      'SCORPION II'
      'SYS80'
      'SYS5'
      'SPACE'
      'PROCONN')
    TabIndex = 0
    OnChange = SelectChange
    object DIPSwitch3: TGroupBox
      Left = 264
      Top = 224
      Width = 177
      Height = 89
      Caption = 'DIP Switch 3'
      TabOrder = 24
      Visible = False
      object Label41: TLabel
        Left = 131
        Top = 66
        Width = 8
        Height = 15
        Caption = '4'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label42: TLabel
        Left = 99
        Top = 66
        Width = 8
        Height = 15
        Caption = '3'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label43: TLabel
        Left = 67
        Top = 66
        Width = 8
        Height = 15
        Caption = '2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label44: TLabel
        Left = 35
        Top = 66
        Width = 8
        Height = 15
        Caption = '1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Dip33: TzcLightOnOff
        Tag = 1
        Left = 24
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 0
        OnClick = Dip3Click
      end
      object Dip32: TzcLightOnOff
        Tag = 2
        Left = 56
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 1
        OnClick = Dip3Click
      end
      object Dip31: TzcLightOnOff
        Tag = 4
        Left = 88
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 2
        OnClick = Dip3Click
      end
      object Dip30: TzcLightOnOff
        Tag = 8
        Left = 120
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 3
        OnClick = Dip3Click
      end
    end
    object IOMapping: TGroupBox
      Left = 208
      Top = 232
      Width = 201
      Height = 185
      Caption = 'IO Mappings'
      TabOrder = 25
      object Label37: TLabel
        Left = 8
        Top = 30
        Width = 32
        Height = 15
        Caption = 'Port'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label38: TLabel
        Left = 104
        Top = 30
        Width = 40
        Height = 15
        Caption = 'Value'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object ResetAll: TSpeedButton
        Left = 128
        Top = 64
        Width = 17
        Height = 17
        OnClick = ResetAllClick
      end
      object Label39: TLabel
        Left = 152
        Top = 66
        Width = 28
        Height = 13
        Caption = 'Reset'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Port: TEdit
        Left = 48
        Top = 28
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 0
        Text = '16'
        OnChange = PortChange
      end
      object UpDown2: TUpDown
        Left = 81
        Top = 28
        Width = 16
        Height = 23
        Associate = Port
        Min = 16
        Max = 511
        Position = 16
        TabOrder = 1
        Thousands = False
        Wrap = False
      end
      object lamp: TRadioButton
        Left = 8
        Top = 80
        Width = 57
        Height = 17
        Caption = 'Lamp'
        Checked = True
        TabOrder = 2
        TabStop = True
        OnClick = TypeClick
      end
      object meter: TRadioButton
        Tag = 2
        Left = 8
        Top = 96
        Width = 57
        Height = 17
        Caption = 'Meter'
        TabOrder = 3
        OnClick = TypeClick
      end
      object triac: TRadioButton
        Tag = 3
        Left = 8
        Top = 112
        Width = 57
        Height = 17
        Caption = 'Triac'
        TabOrder = 4
        OnClick = TypeClick
      end
      object ledseg: TRadioButton
        Tag = 4
        Left = 8
        Top = 128
        Width = 81
        Height = 17
        Caption = 'LED Seg'
        TabOrder = 5
        OnClick = TypeClick
      end
      object ledsegdigit: TRadioButton
        Tag = 6
        Left = 80
        Top = 128
        Width = 97
        Height = 17
        Caption = 'LED (Seg) Digit'
        TabOrder = 6
        OnClick = TypeClick
      end
      object Value: TEdit
        Left = 152
        Top = 28
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 7
        Text = '0'
        OnChange = ValueChange
      end
      object unused: TRadioButton
        Tag = 1
        Left = 8
        Top = 64
        Width = 73
        Height = 17
        Caption = 'Un-Used'
        TabOrder = 8
        OnClick = TypeClick
      end
      object ledbcd: TRadioButton
        Tag = 5
        Left = 8
        Top = 144
        Width = 73
        Height = 17
        Caption = 'LED BCD'
        TabOrder = 9
        OnClick = TypeClick
      end
      object ledbcddigit: TRadioButton
        Tag = 7
        Left = 80
        Top = 144
        Width = 105
        Height = 17
        Caption = 'LED (BCD) Digit'
        TabOrder = 10
        OnClick = TypeClick
      end
    end
    object Meters: TGroupBox
      Left = 136
      Top = 304
      Width = 305
      Height = 113
      Caption = 'Meters'
      TabOrder = 12
      Visible = False
      object Label9: TLabel
        Left = 92
        Top = 38
        Width = 8
        Height = 15
        Caption = '*'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label10: TLabel
        Left = 244
        Top = 38
        Width = 8
        Height = 15
        Caption = '*'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label11: TLabel
        Left = 244
        Top = 62
        Width = 8
        Height = 15
        Caption = '*'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label12: TLabel
        Left = 92
        Top = 62
        Width = 8
        Height = 15
        Caption = '*'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 70
        Top = 16
        Width = 16
        Height = 15
        Caption = 'In'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 222
        Top = 16
        Width = 24
        Height = 15
        Caption = 'Out'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 92
        Top = 86
        Width = 8
        Height = 15
        Caption = '*'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label8: TLabel
        Left = 244
        Top = 86
        Width = 8
        Height = 15
        Caption = '*'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object CashInSource: TComboBox
        Left = 8
        Top = 36
        Width = 81
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 0
        OnChange = TotalInChange
        Items.Strings = (
          'Meter 1'
          'Meter 2'
          'Meter 3'
          'Meter 4'
          'Meter 5'
          'Meter 6'
          'Meter 7'
          'Meter 8'
          'Meter 9'
          'Meter 10'
          'Triac 1'
          'Triac 2'
          'Triac 3'
          'Triac 4'
          'Triac 5'
          'Triac 6'
          'Triac 7'
          'Triac 8'
          'Triac 9'
          'Triac 10'
          'Sec 1'
          'Sec 2'
          'Sec 3'
          'Sec 4'
          'Sec 5'
          'Sec 6'
          'Sec 7'
          'Sec 8'
          'Sec 9'
          'Sec 10')
      end
      object CashInMult: TComboBox
        Left = 104
        Top = 36
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 1
        OnChange = TotalInChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '5'
          '10')
      end
      object CashOutMult: TComboBox
        Left = 256
        Top = 36
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 2
        OnChange = TotalOutChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '5'
          '10')
      end
      object CashOutSource: TComboBox
        Left = 160
        Top = 36
        Width = 81
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 3
        OnChange = TotalOutChange
        Items.Strings = (
          'Meter 1'
          'Meter 2'
          'Meter 3'
          'Meter 4'
          'Meter 5'
          'Meter 6'
          'Meter 7'
          'Meter 8'
          'Meter 9'
          'Meter 10'
          'Triac 1'
          'Triac 2'
          'Triac 3'
          'Triac 4'
          'Triac 5'
          'Triac 6'
          'Triac 7'
          'Triac 8'
          'Triac 9'
          'Triac 10'
          'Sec 1'
          'Sec 2'
          'Sec 3'
          'Sec 4'
          'Sec 5'
          'Sec 6'
          'Sec 7'
          'Sec 8'
          'Sec 9'
          'Sec 10')
      end
      object TokenInSource: TComboBox
        Left = 8
        Top = 60
        Width = 81
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 4
        OnChange = TotalInChange
        Items.Strings = (
          'Meter 1'
          'Meter 2'
          'Meter 3'
          'Meter 4'
          'Meter 5'
          'Meter 6'
          'Meter 7'
          'Meter 8'
          'Meter 9'
          'Meter 10'
          'Triac 1'
          'Triac 2'
          'Triac 3'
          'Triac 4'
          'Triac 5'
          'Triac 6'
          'Triac 7'
          'Triac 8'
          'Triac 9'
          'Triac 10'
          'Sec 1'
          'Sec 2'
          'Sec 3'
          'Sec 4'
          'Sec 5'
          'Sec 6'
          'Sec 7'
          'Sec 8'
          'Sec 9'
          'Sec 10')
      end
      object TokenInMult: TComboBox
        Left = 104
        Top = 60
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 5
        OnChange = TotalInChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '5'
          '10')
      end
      object TokenOutMult: TComboBox
        Left = 256
        Top = 60
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 6
        OnChange = TotalOutChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '5'
          '10')
      end
      object TokenOutSource: TComboBox
        Left = 160
        Top = 60
        Width = 81
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 7
        OnChange = TotalOutChange
        Items.Strings = (
          'Meter 1'
          'Meter 2'
          'Meter 3'
          'Meter 4'
          'Meter 5'
          'Meter 6'
          'Meter 7'
          'Meter 8'
          'Meter 9'
          'Meter 10'
          'Triac 1'
          'Triac 2'
          'Triac 3'
          'Triac 4'
          'Triac 5'
          'Triac 6'
          'Triac 7'
          'Triac 8'
          'Triac 9'
          'Triac 10'
          'Sec 1'
          'Sec 2'
          'Sec 3'
          'Sec 4'
          'Sec 5'
          'Sec 6'
          'Sec 7'
          'Sec 8'
          'Sec 9'
          'Sec 10')
      end
      object CashInSource2: TComboBox
        Left = 8
        Top = 84
        Width = 81
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 8
        OnChange = TotalInChange
        Items.Strings = (
          'Meter 1'
          'Meter 2'
          'Meter 3'
          'Meter 4'
          'Meter 5'
          'Meter 6'
          'Meter 7'
          'Meter 8'
          'Meter 9'
          'Meter 10'
          'Triac 1'
          'Triac 2'
          'Triac 3'
          'Triac 4'
          'Triac 5'
          'Triac 6'
          'Triac 7'
          'Triac 8'
          'Triac 9'
          'Triac 10'
          'Sec 1'
          'Sec 2'
          'Sec 3'
          'Sec 4'
          'Sec 5'
          'Sec 6'
          'Sec 7'
          'Sec 8'
          'Sec 9'
          'Sec 10')
      end
      object CashInMult2: TComboBox
        Left = 104
        Top = 84
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 9
        OnChange = TotalInChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '5'
          '10')
      end
      object CashOutSource2: TComboBox
        Left = 160
        Top = 84
        Width = 81
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 10
        OnChange = TotalOutChange
        Items.Strings = (
          'Meter 1'
          'Meter 2'
          'Meter 3'
          'Meter 4'
          'Meter 5'
          'Meter 6'
          'Meter 7'
          'Meter 8'
          'Meter 9'
          'Meter 10'
          'Triac 1'
          'Triac 2'
          'Triac 3'
          'Triac 4'
          'Triac 5'
          'Triac 6'
          'Triac 7'
          'Triac 8'
          'Triac 9'
          'Triac 10'
          'Sec 1'
          'Sec 2'
          'Sec 3'
          'Sec 4'
          'Sec 5'
          'Sec 6'
          'Sec 7'
          'Sec 8'
          'Sec 9'
          'Sec 10')
      end
      object CashOutMult2: TComboBox
        Left = 256
        Top = 84
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 11
        OnChange = TotalOutChange
        Items.Strings = (
          '1'
          '2'
          '4'
          '5'
          '10')
      end
    end
    object DIPSwitch1: TGroupBox
      Left = 264
      Top = 32
      Width = 297
      Height = 89
      Caption = 'DIP Switch 1'
      TabOrder = 0
      Visible = False
      object Label14: TLabel
        Left = 67
        Top = 66
        Width = 8
        Height = 15
        Caption = '2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label15: TLabel
        Left = 35
        Top = 66
        Width = 8
        Height = 15
        Caption = '1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label17: TLabel
        Left = 99
        Top = 66
        Width = 8
        Height = 15
        Caption = '3'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label18: TLabel
        Left = 131
        Top = 66
        Width = 8
        Height = 15
        Caption = '4'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label19: TLabel
        Left = 163
        Top = 66
        Width = 8
        Height = 15
        Caption = '5'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label20: TLabel
        Left = 195
        Top = 66
        Width = 8
        Height = 15
        Caption = '6'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label21: TLabel
        Left = 227
        Top = 66
        Width = 8
        Height = 15
        Caption = '7'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label22: TLabel
        Left = 259
        Top = 66
        Width = 8
        Height = 15
        Caption = '8'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Dip17: TzcLightOnOff
        Tag = 1
        Left = 24
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 0
        OnClick = Dip1Click
      end
      object Dip16: TzcLightOnOff
        Tag = 2
        Left = 56
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 1
        OnClick = Dip1Click
      end
      object Dip15: TzcLightOnOff
        Tag = 4
        Left = 88
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 2
        OnClick = Dip1Click
      end
      object Dip14: TzcLightOnOff
        Tag = 8
        Left = 120
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 3
        OnClick = Dip1Click
      end
      object Dip13: TzcLightOnOff
        Tag = 16
        Left = 152
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 4
        OnClick = Dip1Click
      end
      object Dip12: TzcLightOnOff
        Tag = 32
        Left = 184
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 5
        OnClick = Dip1Click
      end
      object Dip11: TzcLightOnOff
        Tag = 64
        Left = 216
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 6
        OnClick = Dip1Click
      end
      object Dip10: TzcLightOnOff
        Tag = 128
        Left = 248
        Top = 24
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 7
        OnClick = Dip1Click
      end
    end
    object ProgramType: TRadioGroup
      Left = 184
      Top = 32
      Width = 73
      Height = 49
      Caption = 'Program'
      ItemIndex = 0
      Items.Strings = (
        'No Data'
        'Data')
      TabOrder = 1
      Visible = False
      OnClick = ProgramTypeClick
    end
    object Extender: TRadioGroup
      Left = 184
      Top = 88
      Width = 73
      Height = 97
      Caption = 'Extender'
      ItemIndex = 0
      Items.Strings = (
        'None'
        'Small'
        'Large 1'
        'Large 2'
        'Large 3')
      TabOrder = 2
      Visible = False
      OnClick = ExtenderClick
    end
    object Snd: TRadioGroup
      Left = 96
      Top = 32
      Width = 81
      Height = 89
      Caption = 'Sound'
      ItemIndex = 0
      Items.Strings = (
        'Empire'
        'Barcrest'
        'Barcrest 2'
        'Other'
        'Yamaha')
      TabOrder = 3
      Visible = False
      OnClick = SndClick
    end
    object Reels: TRadioGroup
      Left = 16
      Top = 32
      Width = 73
      Height = 121
      Caption = 'Reels'
      ItemIndex = 0
      Items.Strings = (
        '3/4'
        '5a'
        '6'
        '5b'
        '5c'
        '7'
        '6b')
      TabOrder = 4
      Visible = False
      OnClick = ReelsClick
    end
    object Volume: TRadioGroup
      Left = 16
      Top = 160
      Width = 73
      Height = 73
      Caption = 'Volume'
      ItemIndex = 0
      Items.Strings = (
        'High'
        'Low'
        'Off')
      TabOrder = 5
      Visible = False
      OnClick = VolumeClick
    end
    object Payout: TRadioGroup
      Left = 96
      Top = 128
      Width = 81
      Height = 105
      Caption = 'Payout'
      ItemIndex = 0
      Items.Strings = (
        'Tube'
        'Hopper 1'
        'Hopper 2'
        'Hopper 3'
        'Hopper 4'
        'Hopper 5')
      TabOrder = 6
      Visible = False
      OnClick = PayoutClick
    end
    object CharType: TRadioGroup
      Left = 96
      Top = 240
      Width = 81
      Height = 57
      Caption = 'Character'
      ItemIndex = 0
      Items.Strings = (
        'Barcrest'
        'BWB')
      TabOrder = 7
      Visible = False
      OnClick = CharTypeClick
    end
    object LVD: TRadioGroup
      Left = 16
      Top = 240
      Width = 73
      Height = 57
      Caption = 'LVD'
      ItemIndex = 0
      Items.Strings = (
        'No'
        'Yes')
      TabOrder = 8
      Visible = False
      OnClick = LVDClick
    end
    object StakePrize: TGroupBox
      Left = 264
      Top = 224
      Width = 177
      Height = 73
      BiDiMode = bdLeftToRight
      Caption = 'Stake / Prize'
      ParentBiDiMode = False
      TabOrder = 10
      Visible = False
      object Label1: TLabel
        Left = 14
        Top = 50
        Width = 40
        Height = 15
        Caption = 'Stake'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 70
        Top = 50
        Width = 40
        Height = 15
        Caption = 'Prize'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label5: TLabel
        Left = 134
        Top = 50
        Width = 8
        Height = 15
        Caption = '%'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object StakeKey: TComboBox
        Left = 14
        Top = 22
        Width = 44
        Height = 21
        TabStop = False
        Style = csDropDownList
        Enabled = False
        ItemHeight = 13
        TabOrder = 0
        OnChange = StakeKeyChange
        Items.Strings = (
          ''
          '5p'
          '10p'
          '20p'
          '25p'
          '30p')
      end
      object JackpotKey: TComboBox
        Left = 65
        Top = 22
        Width = 50
        Height = 21
        TabStop = False
        Style = csDropDownList
        Enabled = False
        ItemHeight = 13
        TabOrder = 1
        OnChange = JackpotKeyChange
        Items.Strings = (
          ''
          '£5C'
          '£8T'
          '£8C'
          '£10C'
          '£15C'
          '£25C')
      end
      object PercentKey: TComboBox
        Left = 121
        Top = 22
        Width = 44
        Height = 21
        TabStop = False
        Style = csDropDownList
        Enabled = False
        ItemHeight = 13
        TabOrder = 2
        OnChange = PercentKeyChange
        Items.Strings = (
          ''
          '70%'
          '72%'
          '74%'
          '76%'
          '78%'
          '80%'
          '82%'
          '84%'
          '86%'
          '88%'
          '90%'
          '92%'
          '94%'
          '96%'
          '98%')
      end
    end
    object Misc: TGroupBox
      Left = 448
      Top = 224
      Width = 113
      Height = 73
      Caption = 'Misc'
      TabOrder = 11
      Visible = False
      object Aux2inv: TCheckBox
        Left = 8
        Top = 32
        Width = 81
        Height = 17
        Caption = 'Aux2 Invert'
        TabOrder = 0
        OnClick = AuxinvClick
      end
      object TDoorInvert: TCheckBox
        Left = 8
        Top = 48
        Width = 81
        Height = 17
        Caption = 'Door Invert'
        TabOrder = 1
        OnClick = TDoorInvertClick
      end
      object Aux1inv: TCheckBox
        Left = 8
        Top = 16
        Width = 81
        Height = 17
        Caption = 'Aux1 Invert'
        TabOrder = 2
        OnClick = AuxinvClick
      end
    end
    object SegmentDisplay: TRadioGroup
      Left = 16
      Top = 304
      Width = 113
      Height = 81
      Caption = '7 Segment Display'
      ItemIndex = 0
      Items.Strings = (
        'Barcrest'
        'Extended'
        'Extended 2'
        'Extended 3')
      TabOrder = 13
      Visible = False
      OnClick = SegmentDisplayClick
    end
    object DIPSwitch2: TGroupBox
      Left = 264
      Top = 128
      Width = 297
      Height = 89
      Caption = 'DIP Switch 2'
      TabOrder = 14
      Visible = False
      object Label30: TLabel
        Left = 259
        Top = 66
        Width = 8
        Height = 15
        Caption = '8'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label29: TLabel
        Left = 227
        Top = 66
        Width = 8
        Height = 15
        Caption = '7'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label28: TLabel
        Left = 195
        Top = 66
        Width = 8
        Height = 15
        Caption = '6'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label27: TLabel
        Left = 163
        Top = 66
        Width = 8
        Height = 15
        Caption = '5'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label26: TLabel
        Left = 131
        Top = 66
        Width = 8
        Height = 15
        Caption = '4'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label25: TLabel
        Left = 99
        Top = 66
        Width = 8
        Height = 15
        Caption = '3'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label24: TLabel
        Left = 67
        Top = 66
        Width = 8
        Height = 15
        Caption = '2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label23: TLabel
        Left = 35
        Top = 66
        Width = 8
        Height = 15
        Caption = '1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Dip27: TzcLightOnOff
        Tag = 1
        Left = 24
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 0
        OnClick = Dip2Click
      end
      object Dip26: TzcLightOnOff
        Tag = 2
        Left = 56
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 1
        OnClick = Dip2Click
      end
      object Dip25: TzcLightOnOff
        Tag = 4
        Left = 88
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 2
        OnClick = Dip2Click
      end
      object Dip24: TzcLightOnOff
        Tag = 8
        Left = 120
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 3
        OnClick = Dip2Click
      end
      object Dip23: TzcLightOnOff
        Tag = 16
        Left = 152
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 4
        OnClick = Dip2Click
      end
      object Dip22: TzcLightOnOff
        Tag = 32
        Left = 184
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 5
        OnClick = Dip2Click
      end
      object Dip21: TzcLightOnOff
        Tag = 64
        Left = 216
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 6
        OnClick = Dip2Click
      end
      object Dip20: TzcLightOnOff
        Tag = 128
        Left = 248
        Top = 22
        Width = 28
        Height = 43
        ParentColor = False
        TabOrder = 7
        OnClick = Dip2Click
      end
    end
    object PayoutSense: TGroupBox
      Left = 448
      Top = 304
      Width = 113
      Height = 89
      Caption = 'Meter Payout/Sense'
      TabOrder = 15
      Visible = False
      object Label3: TLabel
        Left = 12
        Top = 26
        Width = 8
        Height = 15
        Caption = 'M'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 52
        Top = 26
        Width = 8
        Height = 15
        Caption = 'T'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object TMeterSense: TEdit
        Left = 24
        Top = 22
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
        OnChange = SenseChange
      end
      object TTriacSense: TEdit
        Left = 64
        Top = 22
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
        OnChange = SenseChange
      end
      object TMeterMask: TEdit
        Left = 24
        Top = 54
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 2
        Text = 'FF'
        OnChange = MaskChange
      end
      object TTriacMask: TEdit
        Left = 64
        Top = 54
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 3
        Text = 'FF'
        OnChange = MaskChange
      end
    end
    object CharacterAddr: TGroupBox
      Left = 448
      Top = 368
      Width = 65
      Height = 49
      Caption = 'Chr Addr'
      TabOrder = 16
      Visible = False
      object ChrAddress: TEdit
        Left = 8
        Top = 18
        Width = 49
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 5
        ParentFont = False
        TabOrder = 0
        OnChange = ChrAddressChange
      end
    end
    object THoppers: TGroupBox
      Left = 16
      Top = 208
      Width = 89
      Height = 73
      Caption = 'Hoppers'
      TabOrder = 28
      Visible = False
      object THopper1: TCheckBox
        Left = 10
        Top = 16
        Width = 73
        Height = 25
        Caption = 'Hopper 1'
        TabOrder = 0
        OnClick = HopperClick
      end
      object THopper2: TCheckBox
        Left = 10
        Top = 40
        Width = 73
        Height = 25
        Caption = 'Hopper 2'
        TabOrder = 1
        OnClick = HopperClick
      end
    end
    object CoinMech: TRadioGroup
      Left = 112
      Top = 208
      Width = 89
      Height = 73
      Caption = 'Coin Mech'
      ItemIndex = 0
      Items.Strings = (
        'Parallel'
        'Binary')
      TabOrder = 17
      Visible = False
      OnClick = CoinMechClick
    end
    object ReelLamps: TGroupBox
      Left = 184
      Top = 192
      Width = 73
      Height = 105
      Caption = 'Reel Lamps'
      TabOrder = 9
      Visible = False
      object Label35: TLabel
        Left = 4
        Top = 26
        Width = 8
        Height = 15
        Caption = 'X'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label36: TLabel
        Left = 36
        Top = 26
        Width = 8
        Height = 15
        Caption = 'Y'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object TReelLampX: TEdit
        Left = 16
        Top = 22
        Width = 17
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 0
        OnChange = TReelLampXChange
      end
      object TReelLampY: TEdit
        Left = 48
        Top = 22
        Width = 17
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 1
        OnChange = TReelLampYChange
      end
      object TwoLines: TCheckBox
        Left = 6
        Top = 56
        Width = 59
        Height = 17
        Caption = '2 Lines'
        TabOrder = 2
        OnClick = TwoLinesClick
      end
    end
    object TSECFitted: TRadioGroup
      Left = 184
      Top = 304
      Width = 89
      Height = 57
      Caption = 'SEC'
      ItemIndex = 0
      Items.Strings = (
        'No'
        'Yes')
      TabOrder = 21
      Visible = False
      OnClick = TSECFittedClick
    end
    object Freq: TGroupBox
      Left = 16
      Top = 392
      Width = 113
      Height = 49
      Caption = 'Sample Rate'
      TabOrder = 18
      Visible = False
      object SampleRate: TEdit
        Left = 8
        Top = 20
        Width = 65
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 8
        ParentFont = False
        TabOrder = 0
        Text = '16000'
        OnChange = SampleRateChange
      end
    end
    object Switches: TGroupBox
      Left = 328
      Top = 32
      Width = 97
      Height = 121
      Caption = 'Switches'
      TabOrder = 19
      Visible = False
      object Label16: TLabel
        Left = 38
        Top = 26
        Width = 32
        Height = 15
        Caption = 'Main'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label34: TLabel
        Left = 38
        Top = 58
        Width = 32
        Height = 15
        Caption = 'Cash'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label40: TLabel
        Left = 38
        Top = 90
        Width = 48
        Height = 15
        Caption = 'Refill'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Door: TEdit
        Left = 8
        Top = 22
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
        OnChange = SwitchChange
      end
      object CashBox: TEdit
        Left = 8
        Top = 54
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
        OnChange = SwitchChange
      end
      object Refill: TEdit
        Left = 8
        Top = 86
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 2
        OnChange = SwitchChange
      end
    end
    object Aux: TRadioGroup
      Left = 96
      Top = 344
      Width = 81
      Height = 65
      Caption = 'Display Port'
      ItemIndex = 0
      Items.Strings = (
        'Displays'
        'Meters'
        'bwb')
      TabOrder = 20
      Visible = False
      OnClick = AuxClick
    end
    object Channels: TRadioGroup
      Left = 280
      Top = 304
      Width = 89
      Height = 57
      Caption = 'Sound'
      ItemIndex = 0
      Items.Strings = (
        'Stereo'
        'Mono')
      TabOrder = 22
      Visible = False
      OnClick = ChannelsClick
    end
    object RotarySwitch: TGroupBox
      Left = 176
      Top = 344
      Width = 81
      Height = 57
      Caption = 'Rotary Switch'
      TabOrder = 23
      Visible = False
      object UpDown1: TUpDown
        Left = 41
        Top = 24
        Width = 16
        Height = 23
        Associate = RotarySW
        Min = 0
        Max = 15
        Position = 0
        TabOrder = 0
        Wrap = False
        OnClick = UpDown1Click
      end
      object RotarySW: TEdit
        Left = 16
        Top = 24
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Text = '0'
      end
    end
    object sys5sound: TRadioGroup
      Left = 480
      Top = 384
      Width = 81
      Height = 57
      Caption = 'Sound Type'
      ItemIndex = 0
      Items.Strings = (
        'Std'
        'Yamaha')
      TabOrder = 26
      Visible = False
      OnClick = sys5soundClick
    end
    object MPU4Type: TRadioGroup
      Left = 264
      Top = 384
      Width = 73
      Height = 57
      Caption = 'Type'
      ItemIndex = 0
      Items.Strings = (
        'AWP'
        'SWP')
      TabOrder = 27
      Visible = False
      OnClick = MPU4TypeClick
    end
  end
end
