object PropertiesForm: TPropertiesForm
  Left = 387
  Top = 325
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSingle
  Caption = 'Properties'
  ClientHeight = 466
  ClientWidth = 277
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -10
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label8: TLabel
    Left = 213
    Top = 406
    Width = 8
    Height = 15
    Caption = 'W'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 213
    Top = 330
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
  object Label10: TLabel
    Left = 213
    Top = 354
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
  object Label11: TLabel
    Left = 213
    Top = 382
    Width = 8
    Height = 15
    Caption = 'H'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    ParentFont = False
  end
  object Font: TSpeedButton
    Left = 4
    Top = 432
    Width = 49
    Height = 25
    Caption = 'Font'
    OnClick = FontClick
  end
  object OK: TButton
    Left = 120
    Top = 433
    Width = 56
    Height = 27
    Caption = 'Apply'
    TabOrder = 0
    OnClick = OKClick
  end
  object Px: TEdit
    Left = 228
    Top = 326
    Width = 40
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 1
    OnChange = PxChange
  end
  object Py: TEdit
    Left = 228
    Top = 350
    Width = 40
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 2
    OnChange = PyChange
  end
  object PHeight: TEdit
    Left = 228
    Top = 378
    Width = 40
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 3
    OnChange = PHeightChange
  end
  object PWidth: TEdit
    Left = 228
    Top = 402
    Width = 40
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Fixedsys'
    Font.Style = []
    MaxLength = 4
    ParentFont = False
    TabOrder = 4
    OnChange = PWidthChange
  end
  object Caption: TMemo
    Left = 4
    Top = 324
    Width = 189
    Height = 101
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Challenge Extra Bold'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 5
  end
  object PageControl: TPageControl
    Left = 4
    Top = 4
    Width = 269
    Height = 317
    ActivePage = DiscReel
    MultiLine = True
    TabOrder = 6
    object Labels: TTabSheet
      Caption = 'Label'
      object Label4: TLabel
        Left = 8
        Top = 24
        Width = 32
        Height = 15
        Caption = 'Lamp'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object BackColour: TShape
        Left = 120
        Top = 20
        Width = 57
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label12: TLabel
        Left = 180
        Top = 24
        Width = 80
        Height = 15
        Caption = 'Background'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LabelLamp: TEdit
        Left = 52
        Top = 20
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 0
      end
      object TransLabel: TCheckBox
        Left = 8
        Top = 56
        Width = 122
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Transparent'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = TransparentClick
      end
    end
    object CheckBoxes: TTabSheet
      Caption = 'CheckBox'
      object Label2: TLabel
        Left = 0
        Top = 24
        Width = 48
        Height = 15
        Caption = 'Button'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object CheckBox: TEdit
        Left = 52
        Top = 20
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 0
      end
    end
    object LEDs: TTabSheet
      Caption = 'LED Digit'
      object Label42: TLabel
        Left = 2
        Top = 8
        Width = 40
        Height = 15
        Caption = 'Digit'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label46: TLabel
        Left = 2
        Top = 52
        Width = 72
        Height = 15
        Caption = 'On Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object OnColour: TShape
        Left = 86
        Top = 50
        Width = 24
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label13: TLabel
        Left = 2
        Top = 128
        Width = 72
        Height = 15
        Caption = 'Thickness'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object label22: TLabel
        Left = 2
        Top = 152
        Width = 72
        Height = 15
        Caption = 'H Spacing'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label20: TLabel
        Left = 2
        Top = 176
        Width = 72
        Height = 15
        Caption = 'V Spacing'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label16: TLabel
        Left = 2
        Top = 200
        Width = 56
        Height = 15
        Caption = 'Spacing'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label79: TLabel
        Left = 2
        Top = 76
        Width = 80
        Height = 15
        Caption = 'Off Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object OffColour: TShape
        Left = 86
        Top = 74
        Width = 24
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label80: TLabel
        Left = 2
        Top = 100
        Width = 72
        Height = 15
        Caption = 'BG Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object BGColour: TShape
        Left = 86
        Top = 98
        Width = 24
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object DPLeft: TCheckBox
        Left = 1
        Top = 30
        Width = 97
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Left DP'
        Checked = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 0
      end
      object DigitNumber: TEdit
        Left = 84
        Top = 4
        Width = 23
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 1
      end
      object LEDThickness: TEdit
        Left = 86
        Top = 124
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 2
      end
      object HSpacing: TEdit
        Left = 86
        Top = 148
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 3
      end
      object VSpacing: TEdit
        Left = 86
        Top = 174
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 4
      end
      object Seg16: TCheckBox
        Left = 2
        Top = 222
        Width = 98
        Height = 17
        Alignment = taLeftJustify
        Caption = '16 Seg'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnClick = Seg16Click
      end
      object Seg0: TEdit
        Tag = 1
        Left = 150
        Top = 2
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 6
      end
      object Seg1: TEdit
        Tag = 2
        Left = 150
        Top = 22
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 7
      end
      object Seg2: TEdit
        Tag = 4
        Left = 150
        Top = 42
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 8
      end
      object Seg3: TEdit
        Tag = 8
        Left = 150
        Top = 62
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 9
      end
      object Seg4: TEdit
        Tag = 16
        Left = 150
        Top = 82
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 10
      end
      object Seg5: TEdit
        Tag = 32
        Left = 150
        Top = 102
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 11
      end
      object Seg6: TEdit
        Tag = 64
        Left = 150
        Top = 122
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 12
      end
      object Seg7: TEdit
        Tag = 128
        Left = 150
        Top = 142
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 13
      end
      object Seg8: TEdit
        Tag = 256
        Left = 222
        Top = 2
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 14
      end
      object Seg9: TEdit
        Tag = 512
        Left = 222
        Top = 22
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 15
      end
      object Seg10: TEdit
        Tag = 1024
        Left = 222
        Top = 42
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 16
      end
      object Seg11: TEdit
        Tag = 2048
        Left = 222
        Top = 62
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 17
      end
      object Seg12: TEdit
        Tag = 4096
        Left = 222
        Top = 82
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 18
      end
      object Seg13: TEdit
        Tag = 8192
        Left = 222
        Top = 102
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 19
      end
      object Seg14: TEdit
        Tag = 16384
        Left = 222
        Top = 122
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 20
      end
      object Seg15: TEdit
        Tag = 32768
        Left = 222
        Top = 142
        Width = 33
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 21
      end
      object Programmable: TCheckBox
        Left = 122
        Top = 174
        Width = 113
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Programmable'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
        OnClick = Seg16Click
      end
      object Spacing: TEdit
        Left = 86
        Top = 198
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 23
      end
      object DPOn: TCheckBox
        Left = 2
        Top = 238
        Width = 98
        Height = 17
        Alignment = taLeftJustify
        Caption = 'DP On'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 24
        OnClick = Seg16Click
      end
    end
    object GraphicLamps: TTabSheet
      Caption = 'Lamp'
      object Label1: TLabel
        Left = 0
        Top = 64
        Width = 48
        Height = 15
        Caption = 'Lamp 1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label30: TLabel
        Left = 0
        Top = 92
        Width = 48
        Height = 15
        Caption = 'Lamp 2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object OnImage1: TLamp2
        Tag = 1
        Left = 184
        Top = 24
        Width = 49
        Height = 33
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = OnImage1MouseDown
      end
      object OnImage2: TLamp2
        Tag = 2
        Left = 184
        Top = 57
        Width = 49
        Height = 33
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = OnImage1MouseDown
      end
      object OnLabel2: TLabel
        Tag = 2
        Left = 236
        Top = 64
        Width = 16
        Height = 15
        Caption = 'On'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = OnLabel1MouseDown
      end
      object OnLabel1: TLabel
        Tag = 1
        Left = 236
        Top = 32
        Width = 16
        Height = 15
        Caption = 'On'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = OnLabel1MouseDown
      end
      object OffLabel: TLabel
        Left = 236
        Top = 96
        Width = 24
        Height = 15
        Caption = 'Off'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = OnLabel1MouseDown
      end
      object OffImage: TLamp2
        Left = 184
        Top = 88
        Width = 49
        Height = 33
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = OnImage1MouseDown
      end
      object Label14: TLabel
        Left = 0
        Top = 168
        Width = 40
        Height = 15
        Caption = 'Sides'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object GColour1: TShape
        Left = 144
        Top = 60
        Width = 33
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object GColour2: TShape
        Left = 144
        Top = 88
        Width = 33
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label3: TLabel
        Left = 56
        Top = 168
        Width = 40
        Height = 15
        Caption = 'Round'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label7: TLabel
        Left = 112
        Top = 168
        Width = 40
        Height = 15
        Caption = 'Angle'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object GColourOff: TShape
        Left = 144
        Top = 34
        Width = 33
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label31: TLabel
        Left = 0
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Button'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label77: TLabel
        Left = 0
        Top = 32
        Width = 32
        Height = 15
        Caption = 'Coin'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label78: TLabel
        Left = 0
        Top = 116
        Width = 96
        Height = 15
        Caption = 'Shortcut Key'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Clear2: TSpeedButton
        Left = 144
        Top = 112
        Width = 33
        Height = 23
        Caption = 'Clear'
        OnClick = Clear2Click
      end
      object GShape: TComboBox
        Left = 4
        Top = 140
        Width = 161
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 0
        OnChange = ShapeChange
        Items.Strings = (
          'Rectangle(Square)'
          'Square'
          'Rectangle(Round)'
          'Square(Round)'
          'Ellipse'
          'Circle'
          'Diamond'
          'Star'
          'Polygon'
          'Triangle Left'
          'Triangle Right'
          'Triangle Up'
          'Triangle Down')
      end
      object Glamp1: TEdit
        Left = 52
        Top = 60
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 1
      end
      object Glamp2: TEdit
        Left = 52
        Top = 88
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 2
      end
      object NoOutLine: TCheckBox
        Left = 4
        Top = 208
        Width = 101
        Height = 17
        Caption = 'No Outline'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object GLAutoSize: TCheckBox
        Left = 4
        Top = 224
        Width = 97
        Height = 17
        Caption = 'Auto Size'
        Checked = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 4
      end
      object Transparent: TCheckBox
        Left = 144
        Top = 224
        Width = 113
        Height = 17
        Caption = 'Transparent'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnClick = TransparentClick
      end
      object Points: TEdit
        Left = 4
        Top = 184
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 6
        OnChange = PointsChange
      end
      object Graphic: TCheckBox
        Left = 144
        Top = 210
        Width = 77
        Height = 17
        Caption = 'Graphic'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        OnClick = GraphicClick
      end
      object Roundness: TEdit
        Left = 60
        Top = 184
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 8
        OnChange = RoundnessChange
      end
      object Angle: TEdit
        Left = 116
        Top = 184
        Width = 30
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 9
        OnChange = RoundnessChange
      end
      object LButton: TEdit
        Left = 52
        Top = 2
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 10
        OnChange = LButtonChange
      end
      object ComboBox1: TComboBox
        Left = 52
        Top = 29
        Width = 85
        Height = 23
        Style = csDropDownList
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 11
        OnChange = LButtonChange
        Items.Strings = (
          ''
          '5  JPM'
          '10 JPM'
          '20 JPM'
          'Tk JPM'
          '50 JPM'
          '£1 JPM'
          '£2 JPM'
          '5  MPU4'
          '10 MPU4'
          '20 MPU4'
          '50 MPU4'
          '£1 MPU4'
          '£2 MPU4'
          '£1 Misc'
          '£2 Misc'
          '1'
          '2'
          '4'
          '8'
          '16'
          '32'
          '64'
          '128')
      end
      object Edit4: TEdit
        Left = 101
        Top = 112
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 4
        ParentFont = False
        TabOrder = 12
        OnKeyDown = Edit12KeyDown
        OnKeyPress = Edit12KeyPress
      end
    end
    object Bitmap: TTabSheet
      Caption = 'Bitmap'
      object BitmapImage: TImage
        Left = 8
        Top = 8
        Width = 89
        Height = 97
        Stretch = True
        OnMouseDown = BitmapImageMouseDown
      end
      object BitmapAutoSize: TCheckBox
        Left = 112
        Top = 8
        Width = 97
        Height = 17
        Caption = 'Auto Size'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object TransBitmap: TCheckBox
        Left = 112
        Top = 32
        Width = 113
        Height = 17
        Caption = 'Transparent'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
    end
    object LED: TTabSheet
      Caption = 'LED seg'
      object Label44: TLabel
        Left = 20
        Top = 56
        Width = 40
        Height = 15
        Caption = 'Digit'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object SegA: TzLed
        Left = 88
        Top = 16
        Width = 33
        Height = 9
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegG: TzLed
        Tag = 6
        Left = 88
        Top = 48
        Width = 33
        Height = 9
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegD: TzLed
        Tag = 3
        Left = 88
        Top = 80
        Width = 33
        Height = 9
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegB: TzLed
        Tag = 1
        Left = 120
        Top = 24
        Width = 9
        Height = 25
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegF: TzLed
        Tag = 5
        Left = 80
        Top = 24
        Width = 9
        Height = 25
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegC: TzLed
        Tag = 2
        Left = 120
        Top = 56
        Width = 9
        Height = 25
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegE: TzLed
        Tag = 4
        Left = 80
        Top = 56
        Width = 9
        Height = 25
        LedStyle = lstSquare
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object SegH: TzLed
        Tag = 7
        Left = 128
        Top = 80
        Width = 9
        Height = 9
        LedStyle = lstSimpleRound
        LEDColor = lctRed
        OnClick = SegClick
        OnDblClick = SegDblClick
      end
      object LEDdigit: TEdit
        Left = 28
        Top = 28
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 0
      end
    end
    object None: TTabSheet
      Caption = 'No Properties'
    end
    object Alpha: TTabSheet
      Caption = 'Alpha'
      object AlphaImage: TImage
        Left = 96
        Top = 8
        Width = 153
        Height = 33
        OnMouseDown = AlphaImageMouseDown
      end
      object Label21: TLabel
        Left = 8
        Top = 16
        Width = 48
        Height = 15
        Caption = 'Number'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object AlphaColour: TShape
        Left = 62
        Top = 52
        Width = 17
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label55: TLabel
        Left = 8
        Top = 56
        Width = 48
        Height = 15
        Caption = 'Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label56: TLabel
        Left = 8
        Top = 88
        Width = 40
        Height = 15
        Caption = 'Width'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Reversed: TCheckBox
        Left = 8
        Top = 120
        Width = 67
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Rev'#39'd'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object AlphaNbr: TEdit
        Left = 62
        Top = 12
        Width = 15
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 1
      end
      object DigitWidth: TEdit
        Left = 62
        Top = 84
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 2
      end
    end
    object LEDLamp: TTabSheet
      Caption = 'LED Lamp'
      object Label18: TLabel
        Left = 8
        Top = 24
        Width = 32
        Height = 15
        Caption = 'Lamp'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LEDColour: TzLed
        Left = 134
        Top = 19
        Width = 25
        Height = 25
        LedStyle = lstSimpleRound
        LEDColor = lctRed
        Enabled = True
        OnMouseDown = LEDColourMouseDown
      end
      object Label19: TLabel
        Left = 124
        Top = 0
        Width = 48
        Height = 15
        Caption = 'Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LED1: TEdit
        Left = 52
        Top = 20
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 0
      end
    end
    object Reel: TTabSheet
      Caption = 'Reel'
      object Label15: TLabel
        Left = 8
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Number'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label24: TLabel
        Left = 8
        Top = 32
        Width = 40
        Height = 15
        Caption = 'Stops'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label25: TLabel
        Left = 97
        Top = 8
        Width = 72
        Height = 15
        Caption = 'Reel Size'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label28: TLabel
        Left = 8
        Top = 56
        Width = 48
        Height = 15
        Caption = 'Offset'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label29: TLabel
        Left = 97
        Top = 32
        Width = 88
        Height = 15
        Caption = 'Border Size'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label32: TLabel
        Left = 8
        Top = 80
        Width = 32
        Height = 15
        Caption = 'Flag'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label23: TLabel
        Left = 97
        Top = 56
        Width = 64
        Height = 15
        Caption = 'Winlines'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label33: TLabel
        Left = 97
        Top = 80
        Width = 96
        Height = 15
        Caption = 'Winline Size'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label34: TLabel
        Left = 97
        Top = 104
        Width = 104
        Height = 15
        Caption = 'Winline Width'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label35: TLabel
        Left = 249
        Top = 104
        Width = 8
        Height = 15
        Caption = '%'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object ReelBorderColour: TShape
        Left = 214
        Top = 124
        Width = 33
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label43: TLabel
        Left = 97
        Top = 128
        Width = 104
        Height = 15
        Caption = 'Border Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label45: TLabel
        Left = 101
        Top = 270
        Width = 32
        Height = 15
        Caption = 'Mask'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label39: TLabel
        Left = 100
        Top = 212
        Width = 88
        Height = 15
        Caption = 'Motor Steps'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label49: TLabel
        Left = 8
        Top = 170
        Width = 48
        Height = 15
        Caption = 'Lamp T'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label50: TLabel
        Left = 8
        Top = 197
        Width = 48
        Height = 15
        Caption = 'Lamp M'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label51: TLabel
        Left = 8
        Top = 224
        Width = 48
        Height = 15
        Caption = 'Lamp B'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LampMask: TImage
        Left = 214
        Top = 264
        Width = 28
        Height = 28
        Stretch = True
        OnMouseDown = LampMaskMouseDown
      end
      object Label61: TLabel
        Left = 100
        Top = 236
        Width = 112
        Height = 15
        Caption = 'Steps per Stop'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Edit1: TEdit
        Left = 62
        Top = 4
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 0
      end
      object CheckBox1: TCheckBox
        Left = 7
        Top = 102
        Width = 68
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Toggle'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object Height: TEdit
        Left = 214
        Top = 4
        Width = 32
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 2
        Text = '0'
      end
      object Edit3: TEdit
        Left = 62
        Top = 52
        Width = 27
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 3
      end
      object BorderSize: TEdit
        Left = 214
        Top = 28
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 4
      end
      object Edit5: TEdit
        Left = 62
        Top = 28
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 5
      end
      object Edit6: TEdit
        Left = 62
        Top = 76
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 6
      end
      object Edit7: TEdit
        Left = 214
        Top = 52
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 7
      end
      object Edit8: TEdit
        Left = 214
        Top = 76
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 8
      end
      object Edit9: TEdit
        Left = 214
        Top = 100
        Width = 32
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 9
      end
      object ReelLamps: TCheckBox
        Left = 7
        Top = 144
        Width = 68
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Lamps'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
      end
      object OptoInv: TCheckBox
        Left = 97
        Top = 148
        Width = 129
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Opto Invert'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
      end
      object Horizontal: TCheckBox
        Left = 97
        Top = 167
        Width = 129
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Horizontal'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object Smooth: TCheckBox
        Left = 7
        Top = 124
        Width = 68
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Smooth'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
      object Reverse: TCheckBox
        Left = 98
        Top = 186
        Width = 128
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Reverse Spin'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
      end
      object Steps: TEdit
        Left = 214
        Top = 208
        Width = 32
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 15
      end
      object Lamp1: TEdit
        Left = 62
        Top = 166
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 16
      end
      object Lamp2: TEdit
        Left = 62
        Top = 193
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 17
      end
      object Lamp3: TEdit
        Left = 62
        Top = 220
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 18
      end
      object Custom: TCheckBox
        Left = 7
        Top = 248
        Width = 68
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Custom'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
        OnClick = CustomClick
      end
      object HeightAdjust: TUpDown
        Left = 198
        Top = 4
        Width = 16
        Height = 23
        AlignButton = udLeft
        Associate = Height
        Min = 0
        Max = 400
        Position = 0
        TabOrder = 20
        Thousands = False
        Wrap = False
        OnClick = HeightAdjustClick
      end
      object StepsPerStop: TEdit
        Left = 214
        Top = 232
        Width = 32
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 21
      end
    end
    object Frame: TTabSheet
      Caption = 'Frame'
      object Label6: TLabel
        Left = 0
        Top = 42
        Width = 40
        Height = 15
        Caption = 'Shape'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object FrameShape: TComboBox
        Left = 60
        Top = 38
        Width = 117
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 0
        OnChange = ShapeChange
        Items.Strings = (
          'Box'
          'Frame'
          'Top Line'
          'Bottom Line'
          'Left Line'
          'Right Line')
      end
      object FrameRaised: TCheckBox
        Left = 0
        Top = 12
        Width = 73
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Raised'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = FrameRaisedClick
      end
    end
    object Background: TTabSheet
      Caption = 'Background'
      object BackgroundColour: TShape
        Left = 144
        Top = 24
        Width = 57
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label26: TLabel
        Left = 148
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object BackgroundImage: TLamp2
        Left = 8
        Top = 24
        Width = 105
        Height = 81
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        State = 1
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = BackgroundImageMouseDown
      end
      object Label27: TLabel
        Left = 36
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Bitmap'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object AutoSize: TCheckBox
        Left = 16
        Top = 112
        Width = 89
        Height = 17
        Caption = 'Autosize'
        Checked = True
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 0
      end
    end
    object BitmapButton: TTabSheet
      Caption = 'Button'
      object Label36: TLabel
        Left = 0
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Button'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object ButtonOffColour: TShape
        Left = 152
        Top = 14
        Width = 24
        Height = 21
        Brush.Color = clBtnFace
        OnMouseDown = LampColourMouseDown
      end
      object ButtonOnColour: TShape
        Left = 152
        Top = 64
        Width = 24
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label38: TLabel
        Left = 0
        Top = 58
        Width = 48
        Height = 15
        Caption = 'Lamp 1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object ButtonOffBMP: TLamp2
        Tag = 3
        Left = 184
        Top = 8
        Width = 49
        Height = 33
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = OnImage1MouseDown
      end
      object Label40: TLabel
        Tag = 1
        Left = 236
        Top = 52
        Width = 16
        Height = 15
        Caption = 'On'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = BitmapButtonMouseDown
      end
      object ButtonOn1BMP: TLamp2
        Tag = 4
        Left = 184
        Top = 42
        Width = 49
        Height = 33
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = OnImage1MouseDown
      end
      object Label41: TLabel
        Left = 236
        Top = 16
        Width = 24
        Height = 15
        Caption = 'Off'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = BitmapButtonMouseDown
      end
      object Label47: TLabel
        Left = 0
        Top = 84
        Width = 48
        Height = 15
        Caption = 'Lamp 2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object ButtonOn2BMP: TLamp2
        Tag = 5
        Left = 184
        Top = 76
        Width = 49
        Height = 33
        BackColor = clWhite
        Colour1 = clBlack
        Colour2 = clBlack
        FillDirection = gdUp
        ForeColor = clBlack
        GradientToColor = clBlack
        InVectorPercent = 50
        OutlineColor = clBlack
        OutlineStyle = psSolid
        OutlineWidth = 1
        Pattern = bsClear
        RoundPerc = 25
        ShadowColor = clBlack
        ShadowDepth = 5
        ShadowOrient = soNone
        StartingAngle = 90
        SurfaceType = stBrushImage
        Vectors = 10
        Trans = False
        Button = 0
        OnMouseDown = OnImage1MouseDown
      end
      object Label37: TLabel
        Tag = 2
        Left = 236
        Top = 84
        Width = 16
        Height = 15
        Caption = 'On'
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = BitmapButtonMouseDown
      end
      object Label5: TLabel
        Left = 0
        Top = 116
        Width = 96
        Height = 15
        Caption = 'Shortcut Key'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Clear: TSpeedButton
        Left = 144
        Top = 112
        Width = 33
        Height = 23
        Caption = 'Clear'
        OnClick = ClearClick
      end
      object Label17: TLabel
        Left = 0
        Top = 32
        Width = 32
        Height = 15
        Caption = 'Coin'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object ButtonNumber: TEdit
        Left = 52
        Top = 4
        Width = 23
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 0
        OnChange = NumberChange
      end
      object ButtonLamp1: TEdit
        Left = 52
        Top = 54
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 1
      end
      object CheckBox2: TCheckBox
        Left = 120
        Top = 145
        Width = 78
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Lock    '
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object CheckBox3: TCheckBox
        Left = 120
        Top = 161
        Width = 78
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Graphic'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = CheckBox3Click
      end
      object TestButton: TzcShapeBitColBtn
        Left = 24
        Top = 144
        Width = 41
        Height = 41
        ShowFocus = False
        TextSize.Left = 0
        TextSize.Top = 0
        TextSize.Width = 0
        TextSize.Height = 0
        Shape = sbCircle
        ParentColor = False
        TabOrder = 4
        OnMouseDown = TestButtonMouseDown
        OffsetGlyph.Left = 2
        OffsetGlyph.Top = 2
      end
      object ButtonLamp2: TEdit
        Left = 52
        Top = 80
        Width = 37
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 5
      end
      object Edit12: TEdit
        Left = 101
        Top = 112
        Width = 41
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 4
        ParentFont = False
        TabOrder = 6
        OnKeyDown = Edit12KeyDown
        OnKeyPress = Edit12KeyPress
      end
      object CoinNumber: TComboBox
        Left = 52
        Top = 29
        Width = 85
        Height = 23
        Style = csDropDownList
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ItemHeight = 15
        ParentFont = False
        TabOrder = 7
        OnChange = NumberChange
        Items.Strings = (
          ''
          '5  JPM'
          '10 JPM'
          '20 JPM'
          'Tk JPM'
          '50 JPM'
          '£1 JPM'
          '£2 JPM'
          '5  MPU4'
          '10 MPU4'
          '20 MPU4'
          '50 MPU4'
          '£1 MPU4'
          '£2 MPU4'
          '£1 Misc'
          '£2 Misc'
          '1'
          '2'
          '4'
          '8'
          '16'
          '32'
          '64'
          '128')
      end
      object ButFrame: TCheckBox
        Left = 120
        Top = 177
        Width = 78
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Frame'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        Visible = False
        OnClick = CheckBox3Click
      end
    end
    object DiscReel: TTabSheet
      Caption = 'Disc Reel'
      ImageIndex = 13
      object Image: TImage
        Left = 16
        Top = 32
        Width = 57
        Height = 57
        Stretch = True
        OnMouseDown = ImageMouseDown
      end
      object Overlay: TImage
        Left = 16
        Top = 112
        Width = 57
        Height = 57
        Stretch = True
        OnMouseDown = OverlayMouseDown
      end
      object LOuter: TLabel
        Left = 204
        Top = 0
        Width = 40
        Height = 15
        Caption = 'Outer'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LInner: TLabel
        Left = 148
        Top = 0
        Width = 40
        Height = 15
        Caption = 'Inner'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Mask: TImage
        Left = 80
        Top = 32
        Width = 41
        Height = 41
        Stretch = True
        OnMouseDown = MaskMouseDown
      end
      object SpeedButton1: TSpeedButton
        Left = 200
        Top = 208
        Width = 49
        Height = 25
        Caption = 'Test'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        OnMouseDown = SpeedButton1MouseDown
        OnMouseUp = SpeedButton1MouseUp
      end
      object LLampSize: TLabel
        Left = 132
        Top = 64
        Width = 56
        Height = 15
        Caption = 'Lamp Sz'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LOutline: TLabel
        Left = 196
        Top = 64
        Width = 56
        Height = 15
        Caption = 'Outline'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LOffset: TLabel
        Left = 204
        Top = 105
        Width = 48
        Height = 15
        Caption = 'Offset'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label510: TLabel
        Left = 8
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Number'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LLamp: TLabel
        Left = 208
        Top = 176
        Width = 32
        Height = 15
        Caption = 'Lamp'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LSegnum: TLabel
        Left = 140
        Top = 176
        Width = 56
        Height = 15
        Caption = 'Seg num'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LDarkness: TLabel
        Left = 132
        Top = 105
        Width = 64
        Height = 15
        Caption = 'Darkness'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label52: TLabel
        Left = 56
        Top = 196
        Width = 40
        Height = 15
        Caption = 'Stops'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label53: TLabel
        Left = 56
        Top = 220
        Width = 40
        Height = 15
        Caption = 'Steps'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label54: TLabel
        Left = 86
        Top = 145
        Width = 56
        Height = 15
        Caption = 'LOffset'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label57: TLabel
        Left = 80
        Top = 80
        Width = 32
        Height = 15
        Caption = 'Mask'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label58: TLabel
        Left = 16
        Top = 94
        Width = 32
        Height = 15
        Caption = 'Disc'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label59: TLabel
        Left = 16
        Top = 174
        Width = 56
        Height = 15
        Caption = 'Overlay'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object OuterH: TEdit
        Left = 208
        Top = 16
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
        Text = '0'
      end
      object OuterL: TEdit
        Left = 208
        Top = 40
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 1
        Text = '0'
      end
      object InnerH: TEdit
        Left = 152
        Top = 16
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 2
        Text = '0'
      end
      object InnerL: TEdit
        Left = 152
        Top = 40
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 3
        Text = '0'
      end
      object LampSize: TEdit
        Left = 152
        Top = 80
        Width = 25
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 4
      end
      object Outline: TEdit
        Left = 208
        Top = 80
        Width = 17
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 5
      end
      object Edit2: TEdit
        Left = 152
        Top = 152
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 6
        Text = '1'
      end
      object UpDown: TUpDown
        Left = 185
        Top = 152
        Width = 15
        Height = 23
        Associate = Edit2
        Min = 1
        Max = 24
        Position = 1
        TabOrder = 7
        Thousands = False
        Wrap = False
        OnClick = UpDownClick
      end
      object Lamp: TEdit
        Left = 208
        Top = 152
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 8
        OnChange = LampChange
      end
      object Nbr: TEdit
        Left = 62
        Top = 4
        Width = 17
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 9
      end
      object Offset: TEdit
        Left = 208
        Top = 120
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 10
      end
      object Darkness: TEdit
        Left = 152
        Top = 120
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 11
      end
      object DRStops: TEdit
        Left = 16
        Top = 192
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 12
      end
      object DRSteps: TEdit
        Left = 16
        Top = 216
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 13
      end
      object LampOffset: TEdit
        Left = 88
        Top = 120
        Width = 33
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 14
      end
      object DInvert: TCheckBox
        Left = 17
        Top = 244
        Width = 128
        Height = 18
        Caption = '  Opto Invert'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
      end
      object UpDown1: TUpDown
        Left = 241
        Top = 16
        Width = 15
        Height = 23
        Associate = OuterH
        Min = 0
        Max = 200
        Position = 0
        TabOrder = 16
        Thousands = False
        Wrap = False
        OnClick = OuterHClick
      end
      object UpDown2: TUpDown
        Left = 241
        Top = 40
        Width = 15
        Height = 23
        Associate = OuterL
        Min = 0
        Max = 240
        Position = 0
        TabOrder = 17
        Thousands = False
        Wrap = False
        OnClick = OuterLClick
      end
      object UpDown3: TUpDown
        Left = 185
        Top = 16
        Width = 15
        Height = 23
        Associate = InnerH
        Min = 0
        Max = 200
        Position = 0
        TabOrder = 18
        Thousands = False
        Wrap = False
        OnClick = InnerHClick
      end
      object UpDown4: TUpDown
        Left = 185
        Top = 40
        Width = 15
        Height = 23
        Associate = InnerL
        Min = 0
        Max = 200
        Position = 0
        TabOrder = 19
        Thousands = False
        Wrap = False
        OnClick = InnerLClick
      end
      object DReversed: TCheckBox
        Left = 17
        Top = 262
        Width = 144
        Height = 18
        Caption = '  Reverse Spin'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
      end
    end
    object DotMatrix: TTabSheet
      Caption = 'Dot Matrix'
      ImageIndex = 14
      object Label48: TLabel
        Left = 8
        Top = 8
        Width = 32
        Height = 15
        Caption = 'Size'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object DotMatrixSize: TEdit
        Left = 62
        Top = 4
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 0
      end
      object Rotated: TCheckBox
        Left = 4
        Top = 40
        Width = 71
        Height = 19
        Alignment = taLeftJustify
        Caption = 'Rotate'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
    end
    object BandReel: TTabSheet
      Caption = 'Band Reel'
      ImageIndex = 17
      object Label65: TLabel
        Left = 8
        Top = 8
        Width = 48
        Height = 15
        Caption = 'Number'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label66: TLabel
        Left = 8
        Top = 32
        Width = 40
        Height = 15
        Caption = 'Stops'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label67: TLabel
        Left = 8
        Top = 56
        Width = 48
        Height = 15
        Caption = 'Offset'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label68: TLabel
        Left = 97
        Top = 8
        Width = 88
        Height = 15
        Caption = 'Border Size'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label69: TLabel
        Left = 97
        Top = 32
        Width = 104
        Height = 15
        Caption = 'Border Colour'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object BRbordercolour: TShape
        Left = 214
        Top = 28
        Width = 33
        Height = 21
        OnMouseDown = LampColourMouseDown
      end
      object Label70: TLabel
        Left = 100
        Top = 127
        Width = 88
        Height = 15
        Caption = 'Motor Steps'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label71: TLabel
        Left = 101
        Top = 158
        Width = 32
        Height = 15
        Caption = 'Mask'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label72: TLabel
        Left = 8
        Top = 160
        Width = 48
        Height = 15
        Caption = 'Lamp 3'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label73: TLabel
        Left = 8
        Top = 133
        Width = 48
        Height = 15
        Caption = 'Lamp 2'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label74: TLabel
        Left = 8
        Top = 106
        Width = 48
        Height = 15
        Caption = 'Lamp 1'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label75: TLabel
        Left = 8
        Top = 187
        Width = 48
        Height = 15
        Caption = 'Lamp 4'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label76: TLabel
        Left = 8
        Top = 214
        Width = 48
        Height = 15
        Caption = 'Lamp 5'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object BRlampmask: TImage
        Left = 214
        Top = 152
        Width = 28
        Height = 28
        Stretch = True
        OnMouseDown = LampMaskMouseDown
      end
      object Label60: TLabel
        Left = 101
        Top = 190
        Width = 56
        Height = 15
        Caption = 'Overlay'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object BROverlay: TImage
        Left = 214
        Top = 184
        Width = 28
        Height = 28
        Stretch = True
        OnMouseDown = LampMaskMouseDown
      end
      object BRnbr: TEdit
        Left = 62
        Top = 4
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 0
      end
      object BRstops: TEdit
        Left = 62
        Top = 28
        Width = 24
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 1
      end
      object BRoffset: TEdit
        Left = 62
        Top = 52
        Width = 27
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 2
      end
      object BRbordersize: TEdit
        Left = 214
        Top = 4
        Width = 15
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 1
        ParentFont = False
        TabOrder = 3
      end
      object BRlamps: TCheckBox
        Left = 7
        Top = 80
        Width = 68
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Lamps'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnClick = BRlampsClick
      end
      object BRreversespin: TCheckBox
        Left = 98
        Top = 74
        Width = 128
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Reverse Spin'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object BRmotorsteps: TEdit
        Left = 214
        Top = 123
        Width = 32
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 6
      end
      object BRoptoinvert: TCheckBox
        Left = 97
        Top = 52
        Width = 129
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Opto Invert'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
      object BRlamp1: TEdit
        Left = 62
        Top = 102
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 8
      end
      object BRlamp2: TEdit
        Left = 62
        Top = 129
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 9
      end
      object BRlamp3: TEdit
        Left = 62
        Top = 156
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 10
      end
      object BRlamp4: TEdit
        Left = 62
        Top = 183
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 11
      end
      object BRlamp5: TEdit
        Left = 62
        Top = 210
        Width = 32
        Height = 23
        Enabled = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 3
        ParentFont = False
        TabOrder = 12
      end
      object Vertical: TCheckBox
        Left = 98
        Top = 98
        Width = 128
        Height = 18
        Alignment = taLeftJustify
        Caption = 'Vertical'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
      end
    end
    object Video: TTabSheet
      Caption = 'Video'
      ImageIndex = 18
      object Label81: TLabel
        Left = 8
        Top = 32
        Width = 48
        Height = 15
        Caption = 'Button'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object Label82: TLabel
        Left = 8
        Top = 8
        Width = 80
        Height = 15
        Caption = 'Left Mouse'
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        ParentFont = False
      end
      object LeftMouse: TEdit
        Left = 60
        Top = 28
        Width = 23
        Height = 23
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Fixedsys'
        Font.Style = []
        MaxLength = 2
        ParentFont = False
        TabOrder = 0
        OnChange = NumberChange
      end
    end
  end
  object FontDialog1: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MinFontSize = 0
    MaxFontSize = 0
    Left = 188
    Top = 432
  end
  object OpenPictureDialog1: TOpenPictureDialog
    DefaultExt = 'bmp'
    Filter = 'Bitmaps (*.bmp)|*.bmp'
    Left = 244
    Top = 430
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Bitmaps (*.bmp)|*.bmp'
    Options = [ofOverwritePrompt, ofHideReadOnly]
    Left = 216
    Top = 432
  end
  object OpenDialog1: TOpenDialog
    Left = 88
    Top = 432
  end
end
