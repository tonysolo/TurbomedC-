object SummaryDialog: TSummaryDialog
  Left = 458
  Top = 176
  BorderStyle = bsDialog
  Caption = 'Send Summary'
  ClientHeight = 378
  ClientWidth = 235
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object BitBtn1: TBitBtn
    Left = 22
    Top = 348
    Width = 75
    Height = 25
    TabOrder = 0
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 138
    Top = 348
    Width = 75
    Height = 25
    TabOrder = 1
    Kind = bkCancel
  end
  object CheckListBox1: TCheckListBox
    Left = 0
    Top = 0
    Width = 235
    Height = 177
    Align = alTop
    ItemHeight = 13
    Items.Strings = (
      'Ref Num'
      'Folder Num'
      'Account Name'
      'Patient Name'
      'Patient ID'
      'Account Date'
      'Procedure Date'
      'Payment Date'
      'Account Type'
      'Referral'
      'Clinic'
      'Diagnosis'
      'Medical Aid'
      'Med Aid Number'
      'Auth Code'
      'Billed'
      'Discount'
      'Discount Date'
      'Interest'
      'Payments'
      'Balance'
      'Notes (User)'
      'Notes (All)')
    TabOrder = 2
    OnClick = CheckListBox1Click
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 209
    Width = 233
    Height = 83
    TabOrder = 3
    object Label1: TLabel
      Left = 98
      Top = 54
      Width = 13
      Height = 13
      Caption = 'To'
    end
    object RadioButton1: TRadioButton
      Left = 5
      Top = 14
      Width = 163
      Height = 17
      Caption = 'Selected Accounts Only'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object RadioButton2: TRadioButton
      Left = 5
      Top = 32
      Width = 113
      Height = 17
      Caption = 'All Accounts   From'
      TabOrder = 1
    end
    object DateTimePicker1: TDateTimePicker
      Left = 135
      Top = 32
      Width = 90
      Height = 21
      CalAlignment = dtaLeft
      Date = 37724.6822813889
      Format = 'dd/MM/yyyy'
      Time = 37724.6822813889
      DateFormat = dfShort
      DateMode = dmUpDown
      Kind = dtkDate
      ParseInput = False
      TabOrder = 2
    end
    object DateTimePicker2: TDateTimePicker
      Left = 135
      Top = 56
      Width = 90
      Height = 21
      CalAlignment = dtaLeft
      Date = 37724.6823349421
      Format = 'dd/MM/yyyy'
      Time = 37724.6823349421
      DateFormat = dfShort
      DateMode = dmUpDown
      Kind = dtkDate
      ParseInput = False
      TabOrder = 3
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 293
    Width = 233
    Height = 50
    TabOrder = 4
    object RadioButton3: TRadioButton
      Left = 8
      Top = 10
      Width = 113
      Height = 17
      Caption = 'Send To File'
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object RadioButton4: TRadioButton
      Left = 7
      Top = 26
      Width = 113
      Height = 17
      Caption = 'Send To Clipboard'
      TabOrder = 1
    end
  end
  object RadioGroup1: TRadioGroup
    Left = 0
    Top = 176
    Width = 233
    Height = 33
    Items.Strings = (
      'Check All')
    TabOrder = 5
    OnClick = RadioGroup1Click
  end
end
