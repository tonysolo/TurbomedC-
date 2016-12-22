//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Assist.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAssistFee *AssistFee;
//---------------------------------------------------------------------------
__fastcall TAssistFee::TAssistFee(TComponent* Owner)
        : TForm(Owner)
{
Amounts = new TStringList;
}
//---------------------------------------------------------------------------


__fastcall TAssistFee::~TAssistFee()
{
delete Amounts;
}

void __fastcall TAssistFee::CheckListBox1ClickCheck(TObject *Sender)
{
double total=0;
for (int i=0;i<CheckListBox1->Items->Count;i++)
  if (CheckListBox1->Checked[i])
{
double x;
try {x= Amounts->Strings[i].ToDouble();} catch(EConvertError &){x=0;}
total+=x;
}
double ar = (AssistRate==66)? .6666 : (AssistRate==33)? .3333: .20;
LabeledEdit1->Text = FormatFloat("0.00",total* ar);
LabeledEdit2->Text = FormatFloat("0.00",total);
}
//---------------------------------------------------------------------------


void TAssistFee::UpdateRate(int rate)
{
AssistRate = rate;
LabeledEdit1->EditLabel->Caption= "Assistant Fee ("+ IntToStr(rate)+ "%)" ;
}

