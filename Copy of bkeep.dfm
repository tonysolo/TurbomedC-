object BookKeep: TBookKeep
  Left = 304
  Top = 146
  BorderStyle = bsDialog
  Caption = 'Bookkeeping (Debtors Ledger)'
  ClientHeight = 393
  ClientWidth = 632
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object FileSaveButton: TSpeedButton
    Left = 374
    Top = 364
    Width = 23
    Height = 22
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
  object Label3: TLabel
    Left = 398
    Top = 368
    Width = 56
    Height = 13
    Caption = 'Save to File'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object PrintButton: TSpeedButton
    Left = 479
    Top = 364
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
    Left = 507
    Top = 368
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
  object Label1: TLabel
    Left = 13
    Top = 347
    Width = 28
    Height = 13
    Caption = 'From'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 108
    Top = 347
    Width = 12
    Height = 13
    Caption = 'to'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object paylabel: TLabel
    Left = 64
    Top = 326
    Width = 41
    Height = 13
    Alignment = taRightJustify
    Caption = 'Payment'
    Color = clBtnFace
    ParentColor = False
  end
  object refundlabel: TLabel
    Left = 200
    Top = 326
    Width = 35
    Height = 13
    Caption = 'Refund'
  end
  object adjustlabel: TLabel
    Left = 337
    Top = 326
    Width = 52
    Height = 13
    Caption = 'Adjustment'
  end
  object assistantlabel: TLabel
    Left = 474
    Top = 326
    Width = 60
    Height = 13
    Caption = 'Assistant fee'
  end
  object Bevel1: TBevel
    Left = 0
    Top = 336
    Width = 625
    Height = 7
    Shape = bsBottomLine
  end
  object Clip: TSpeedButton
    Left = 271
    Top = 364
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
    OnClick = ClipClick
  end
  object Label4: TLabel
    Left = 297
    Top = 368
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
  object ListBox1: TListBox
    Left = 0
    Top = 17
    Width = 632
    Height = 304
    Style = lbVirtualOwnerDraw
    Align = alTop
    BevelInner = bvNone
    BevelOuter = bvNone
    ExtendedSelect = False
    ItemHeight = 13
    TabOrder = 0
    OnDrawItem = ListBox1DrawItem
  end
  object DateTimePicker1: TDateTimePicker
    Left = 10
    Top = 365
    Width = 90
    Height = 21
    CalAlignment = dtaLeft
    Date = 37596.6528653819
    Format = 'dd/MM/yyyy'
    Time = 37596.6528653819
    DateFormat = dfShort
    DateMode = dmUpDown
    Kind = dtkDate
    ParseInput = False
    TabOrder = 1
    OnClick = DateTimePicker1Click
    OnChange = DateTimePicker1Change
  end
  object DateTimePicker2: TDateTimePicker
    Left = 105
    Top = 365
    Width = 90
    Height = 21
    CalAlignment = dtaLeft
    Date = 37596.6528653819
    Format = 'dd/MM/yyyy'
    Time = 37596.6528653819
    DateFormat = dfShort
    DateMode = dmUpDown
    Kind = dtkDate
    ParseInput = False
    TabOrder = 2
    OnClick = DateTimePicker1Click
    OnChange = DateTimePicker1Change
  end
  object BitBtn1: TBitBtn
    Left = 548
    Top = 361
    Width = 76
    Height = 25
    TabOrder = 3
    Kind = bkClose
  end
  object HeaderControl1: THeaderControl
    Left = 0
    Top = 0
    Width = 632
    Height = 17
    DragReorder = False
    Sections = <
      item
        ImageIndex = -1
        Text = 'Date'
        Width = 75
      end
      item
        ImageIndex = -1
        Text = 'Ref'
        Width = 40
      end
      item
        ImageIndex = -1
        Text = 'Account'
        Width = 80
      end
      item
        ImageIndex = -1
        Text = 'Code'
        Width = 40
      end
      item
        ImageIndex = -1
        Text = 'Description'
        Width = 120
      end
      item
        Alignment = taRightJustify
        ImageIndex = -1
        Text = 'Debit'
        Width = 70
      end
      item
        Alignment = taRightJustify
        ImageIndex = -1
        Text = 'Credit'
        Width = 80
      end
      item
        Alignment = taRightJustify
        ImageIndex = -1
        Text = 'Balance Owing'
        Width = 100
      end>
    Style = hsFlat
    OnSectionResize = HeaderControl1SectionResize
  end
  object UpdateButton: TButton
    Left = 202
    Top = 363
    Width = 48
    Height = 25
    Caption = 'Update'
    TabOrder = 5
    OnClick = UpdateButtonClick
  end
end
