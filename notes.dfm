object LayoutNotes: TLayoutNotes
  Left = 365
  Top = 296
  Width = 576
  Height = 266
  BorderIcons = [biSystemMenu]
  Caption = 'Layout Notes'
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
  object Notes: TMemo
    Left = 0
    Top = 0
    Width = 568
    Height = 239
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Courier'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
    OnChange = NotesChange
  end
end
