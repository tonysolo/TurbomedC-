object ChangePrinter: TChangePrinter
  Left = 496
  Top = 406
  BorderStyle = bsDialog
  Caption = 'ChangePrinter'
  ClientHeight = 171
  ClientWidth = 219
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 24
    Top = 16
    Width = 169
    Height = 81
    Caption = 'Printer'
    TabOrder = 0
    object ComboBox1: TComboBox
      Left = 16
      Top = 32
      Width = 145
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      Text = 'ComboBox1'
    end
  end
  object BitBtn1: TBitBtn
    Left = 24
    Top = 120
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 118
    Top = 120
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkCancel
  end
end
