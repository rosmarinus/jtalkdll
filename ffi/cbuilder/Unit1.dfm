object Form1: TForm1
  Left = 0
  Top = 0
  Caption = #30330#22768#12398#12486#12473#12488
  ClientHeight = 121
  ClientWidth = 234
  Color = clBtnFace
  Constraints.MaxHeight = 160
  Constraints.MaxWidth = 250
  Constraints.MinHeight = 160
  Constraints.MinWidth = 250
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 88
    Width = 98
    Height = 25
    Caption = #30330#22768
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 128
    Top = 88
    Width = 98
    Height = 25
    Caption = #20572#27490
    TabOrder = 2
    OnClick = Button2Click
  end
  object Memo1: TMemo
    Left = 8
    Top = 32
    Width = 218
    Height = 50
    Lines.Strings = (
      #20309#12363#20837#21147#12375#12390#12367#12384#12373#12356#12290)
    TabOrder = 0
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 5
    Width = 218
    Height = 21
    Style = csDropDownList
    TabOrder = 3
    OnChange = ComboBox1Change
  end
end
