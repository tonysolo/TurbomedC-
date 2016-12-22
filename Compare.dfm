object CompareDlg: TCompareDlg
  Left = 208
  Top = 148
  BorderStyle = bsDialog
  Caption = 'Compare Changes'
  ClientHeight = 389
  ClientWidth = 616
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
  object FileSaveButton: TSpeedButton
    Left = 115
    Top = 361
    Width = 23
    Height = 23
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      0400000000000001000000000000000000001000000010000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333330070
      7700333333337777777733333333008088003333333377F73377333333330088
      88003333333377FFFF7733333333000000003FFFFFFF77777777000000000000
      000077777777777777770FFFFFFF0FFFFFF07F3333337F3333370FFFFFFF0FFF
      FFF07F3FF3FF7FFFFFF70F00F0080CCC9CC07F773773777777770FFFFFFFF039
      99337F3FFFF3F7F777F30F0000F0F09999937F7777373777777F0FFFFFFFF999
      99997F3FF3FFF77777770F00F000003999337F773777773777F30FFFF0FF0339
      99337F3FF7F3733777F30F08F0F0337999337F7737F73F7777330FFFF0039999
      93337FFFF7737777733300000033333333337777773333333333}
    NumGlyphs = 2
    OnClick = FileSaveButtonClick
  end
  object Label1: TLabel
    Left = 143
    Top = 366
    Width = 60
    Height = 13
    Caption = 'Save To File'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton1: TSpeedButton
    Left = 18
    Top = 361
    Width = 23
    Height = 22
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000120B0000120B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333000000
      033333FFFF77777773F330000077777770333777773FFFFFF733077777000000
      03337F3F3F777777733F0797A770003333007F737337773F3377077777778803
      30807F333333337FF73707888887880007707F3FFFF333777F37070000878807
      07807F777733337F7F3707888887880808807F333333337F7F37077777778800
      08807F333FFF337773F7088800088803308073FF777FFF733737300008000033
      33003777737777333377333080333333333333F7373333333333300803333333
      33333773733333333333088033333333333373F7F33333333333308033333333
      3333373733333333333333033333333333333373333333333333}
    NumGlyphs = 2
    OnClick = SpeedButton1Click
  end
  object Label2: TLabel
    Left = 45
    Top = 366
    Width = 60
    Height = 13
    Caption = 'To Clipboard'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object PrintButton: TSpeedButton
    Left = 222
    Top = 361
    Width = 24
    Height = 22
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      0400000000000001000000000000000000001000000010000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00300000000000
      0003377777777777777308888888888888807F33333333333337088888888888
      88807FFFFFFFFFFFFFF7000000000000000077777777777777770F8F8F8F8F8F
      8F807F333333333333F708F8F8F8F8F8F9F07F333333333337370F8F8F8F8F8F
      8F807FFFFFFFFFFFFFF7000000000000000077777777777777773330FFFFFFFF
      03333337F3FFFF3F7F333330F0000F0F03333337F77773737F333330FFFFFFFF
      03333337F3FF3FFF7F333330F00F000003333337F773777773333330FFFF0FF0
      33333337F3F37F3733333330F08F0F0333333337F7337F7333333330FFFF0033
      33333337FFFF7733333333300000033333333337777773333333}
    NumGlyphs = 2
    ParentShowHint = False
    ShowHint = False
    OnClick = PrintButtonClick
  end
  object Label8: TLabel
    Left = 250
    Top = 366
    Width = 21
    Height = 13
    Caption = 'Print'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object HeaderControl1: THeaderControl
    Left = 0
    Top = 0
    Width = 616
    Height = 17
    DragReorder = False
    Sections = <
      item
        ImageIndex = -1
        Text = 'Account'
        Width = 60
      end
      item
        ImageIndex = -1
        Text = 'Current Details'
        Width = 100
      end
      item
        ImageIndex = -1
        Text = 'Folder'
        Width = 50
      end
      item
        ImageIndex = -1
        Text = 'Payments'
        Width = 60
      end
      item
        ImageIndex = -1
        Text = 'Balance'
        Width = 60
      end
      item
        ImageIndex = -1
        Text = 'Compare Details'
        Width = 100
      end
      item
        ImageIndex = -1
        Text = 'Folder'
        Width = 50
      end
      item
        ImageIndex = -1
        Text = 'Payments'
        Width = 60
      end
      item
        ImageIndex = -1
        Text = 'Balance'
        Width = 60
      end>
  end
  object ListBox1: TListBox
    Left = 0
    Top = 17
    Width = 616
    Height = 336
    Style = lbVirtualOwnerDraw
    Align = alTop
    ItemHeight = 16
    TabOrder = 1
    OnDrawItem = ListBox1DrawItem
  end
  object BitBtn1: TBitBtn
    Left = 510
    Top = 360
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkClose
  end
end
