object BookKeep: TBookKeep
  Left = 472
  Top = 101
  BorderStyle = bsDialog
  Caption = 'Financial Report'
  ClientHeight = 413
  ClientWidth = 632
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object FileSaveButton: TSpeedButton
    Left = 598
    Top = 327
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
    Left = 529
    Top = 332
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
  object Label1: TLabel
    Left = 397
    Top = 247
    Width = 30
    Height = 13
    Caption = 'Period'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 536
    Top = 226
    Width = 19
    Height = 13
    Caption = 'End'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
  end
  object paylabel: TLabel
    Left = 364
    Top = 197
    Width = 41
    Height = 13
    Alignment = taRightJustify
    Caption = 'Payment'
    Color = clBtnFace
    ParentColor = False
  end
  object refundlabel: TLabel
    Left = 144
    Top = 197
    Width = 35
    Height = 13
    Caption = 'Refund'
  end
  object adjustlabel: TLabel
    Left = 585
    Top = 197
    Width = 37
    Height = 13
    Caption = 'Writeoff'
  end
  object assistantlabel: TLabel
    Left = 34
    Top = 197
    Width = 60
    Height = 13
    Caption = 'Assistant fee'
  end
  object Bevel1: TBevel
    Left = 0
    Top = 336
    Width = 625
    Height = -22
    Shape = bsBottomLine
  end
  object Clip: TSpeedButton
    Left = 599
    Top = 349
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
    Left = 501
    Top = 354
    Width = 84
    Height = 13
    Caption = 'Save to Clipboard'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Bevel2: TBevel
    Left = 4
    Top = 210
    Width = 625
    Height = 7
    Shape = bsBottomLine
  end
  object Label15: TLabel
    Left = 181
    Top = 268
    Width = 81
    Height = 13
    Caption = 'Patient payments'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label17: TLabel
    Left = 177
    Top = 289
    Width = 85
    Height = 13
    Caption = 'Unspec payments'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label18: TLabel
    Left = 238
    Top = 354
    Width = 24
    Height = 13
    Caption = 'Total'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
  end
  object Label13: TLabel
    Left = 215
    Top = 311
    Width = 47
    Height = 13
    Caption = 'Discounts'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label14: TLabel
    Left = 217
    Top = 332
    Width = 45
    Height = 13
    Caption = 'Write offs'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label11: TLabel
    Left = 15
    Top = 268
    Width = 65
    Height = 13
    Caption = 'Assistant fees'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label16: TLabel
    Left = 176
    Top = 247
    Width = 86
    Height = 13
    Caption = 'Med aid payments'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 15
    Top = 247
    Width = 65
    Height = 13
    Caption = 'Practice Fees'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label10: TLabel
    Left = 40
    Top = 289
    Width = 40
    Height = 13
    Caption = 'Refunds'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 45
    Top = 311
    Width = 35
    Height = 13
    Caption = 'Interest'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 266
    Top = 226
    Width = 32
    Height = 13
    Caption = 'Credits'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
  end
  object Label6: TLabel
    Left = 85
    Top = 225
    Width = 30
    Height = 13
    Caption = 'Debits'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
  end
  object Label12: TLabel
    Left = 56
    Top = 354
    Width = 24
    Height = 13
    Caption = 'Total'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
  end
  object Label19: TLabel
    Left = 434
    Top = 226
    Width = 22
    Height = 13
    Caption = 'Start'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsUnderline]
    ParentFont = False
  end
  object Label20: TLabel
    Left = 387
    Top = 268
    Width = 39
    Height = 13
    Caption = 'Balance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object interestlabel: TLabel
    Left = 254
    Top = 198
    Width = 35
    Height = 13
    Caption = 'Interest'
  end
  object discountlabel: TLabel
    Left = 474
    Top = 198
    Width = 42
    Height = 13
    Caption = 'Discount'
  end
  object Bevel3: TBevel
    Left = 7
    Top = 375
    Width = 625
    Height = 7
    Shape = bsBottomLine
  end
  object Label8: TLabel
    Left = 562
    Top = 309
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
  object printbutton: TSpeedButton
    Left = 598
    Top = 303
    Width = 23
    Height = 22
    Glyph.Data = {
      F6000000424DF600000000000000760000002800000010000000100000000100
      04000000000080000000C40E0000C40E00001000000000000000000000000000
      8000008000000080800080000000800080008080000080808000C0C0C0000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00F00000000000
      000F0888888888888880088888888888888000000000000000000F8F8F8F8F8F
      8F8008F8F8F8F8F8F9F00F8F8F8F8F8F8F800000000000000000FFF0FFFFFFFF
      0FFFFFF0F0000F0F0FFFFFF0FFFFFFFF0FFFFFF0F00F00000FFFFFF0FFFF0FF0
      FFFFFFF0F08F0F0FFFFFFFF0FFFF00FFFFFFFFF000000FFFFFFF}
    OnClick = printbuttonClick
  end
  object ListBox1: TListBox
    Left = 0
    Top = 17
    Width = 632
    Height = 176
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
    Left = 434
    Top = 243
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
    Left = 536
    Top = 243
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
    Top = 386
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
        Width = 75
      end
      item
        Alignment = taRightJustify
        ImageIndex = -1
        Text = 'Credit'
        Width = 75
      end
      item
        Alignment = taRightJustify
        ImageIndex = -1
        Text = 'Balance'
        Width = 100
      end>
    Style = hsFlat
    OnSectionResize = HeaderControl1SectionResize
  end
  object UpdateButton: TButton
    Left = 471
    Top = 294
    Width = 45
    Height = 25
    Caption = 'Update'
    TabOrder = 4
    OnClick = UpdateButtonClick
  end
  object pfees: TEdit
    Left = 85
    Top = 243
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 6
  end
  object afees: TEdit
    Left = 85
    Top = 264
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 7
  end
  object refunds: TEdit
    Left = 85
    Top = 285
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 8
  end
  object interest: TEdit
    Left = 85
    Top = 307
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 9
  end
  object totdebits: TEdit
    Left = 85
    Top = 350
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 10
  end
  object mapay: TEdit
    Left = 266
    Top = 243
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 11
  end
  object ppay: TEdit
    Left = 266
    Top = 264
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 12
  end
  object upay: TEdit
    Left = 266
    Top = 285
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 13
  end
  object discounts: TEdit
    Left = 266
    Top = 307
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 14
  end
  object writeoffs: TEdit
    Left = 266
    Top = 328
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 15
  end
  object totcredits: TEdit
    Left = 266
    Top = 350
    Width = 80
    Height = 21
    ReadOnly = True
    TabOrder = 16
  end
  object startbalance: TEdit
    Left = 434
    Top = 264
    Width = 87
    Height = 21
    ReadOnly = True
    TabOrder = 17
  end
  object endbalance: TEdit
    Left = 536
    Top = 264
    Width = 87
    Height = 21
    ReadOnly = True
    TabOrder = 18
  end
  object intcheck: TCheckBox
    Left = 377
    Top = 298
    Width = 70
    Height = 17
    Alignment = taLeftJustify
    Caption = 'All Interest'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
    OnClick = intcheckClick
  end
end
