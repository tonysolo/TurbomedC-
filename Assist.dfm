object AssistFee: TAssistFee
  Left = 537
  Top = 216
  BorderStyle = bsDialog
  Caption = 'Assistant Fee Calculator'
  ClientHeight = 373
  ClientWidth = 312
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object LabeledEdit1: TLabeledEdit
    Left = 105
    Top = 343
    Width = 121
    Height = 21
    EditLabel.Width = 71
    EditLabel.Height = 13
    EditLabel.Caption = 'Assistant Fees '
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clBlue
    EditLabel.Font.Height = -11
    EditLabel.Font.Name = 'MS Sans Serif'
    EditLabel.Font.Style = []
    EditLabel.ParentFont = False
    LabelPosition = lpLeft
    LabelSpacing = 3
    ReadOnly = True
    TabOrder = 0
  end
  object CheckListBox1: TCheckListBox
    Left = 0
    Top = 0
    Width = 312
    Height = 313
    OnClickCheck = CheckListBox1ClickCheck
    Align = alTop
    ItemHeight = 13
    TabOrder = 1
  end
  object LabeledEdit2: TLabeledEdit
    Left = 104
    Top = 318
    Width = 121
    Height = 21
    EditLabel.Width = 72
    EditLabel.Height = 13
    EditLabel.Caption = 'Selected Total '
    EditLabel.Font.Charset = DEFAULT_CHARSET
    EditLabel.Font.Color = clBlue
    EditLabel.Font.Height = -11
    EditLabel.Font.Name = 'MS Sans Serif'
    EditLabel.Font.Style = []
    EditLabel.ParentFont = False
    LabelPosition = lpLeft
    LabelSpacing = 3
    TabOrder = 2
  end
  object BitBtn1: TBitBtn
    Left = 239
    Top = 341
    Width = 65
    Height = 25
    TabOrder = 3
    Kind = bkOK
  end
end
