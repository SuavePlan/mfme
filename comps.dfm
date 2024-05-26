object Form3: TForm3
  Left = 779
  Top = 565
  Width = 733
  Height = 564
  Caption = 'Components'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ListView1: TListView
    Left = 0
    Top = 0
    Width = 725
    Height = 530
    Align = alClient
    Columns = <
      item
        AutoSize = True
        Caption = 'Number'
      end
      item
        AutoSize = True
        Caption = 'Name'
      end
      item
        AutoSize = True
        Caption = 'X'
      end
      item
        AutoSize = True
        Caption = 'Y'
      end
      item
        AutoSize = True
        Caption = 'Width'
      end
      item
        AutoSize = True
        Caption = 'Height'
      end
      item
        AutoSize = True
        Caption = 'Button'
      end
      item
        AutoSize = True
        Caption = 'Shortcut'
      end>
    GridLines = True
    TabOrder = 0
    ViewStyle = vsReport
  end
end
