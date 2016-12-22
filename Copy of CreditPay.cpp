//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CreditPay.h"
#include "CSVRec.h"
#include "Main.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCredit *Credit;
//---------------------------------------------------------------------------
__fastcall TCredit::TCredit(TComponent* Owner)
    : TForm(Owner)
{
float temp;
char disstr[4];
TAccount* A = (TAccount*)((TListView*)Owner)->Selected->Data;
strcpy(disstr,(A->Field[tmd::sc][0]=='D')?A->Field[tmd::dsc]:"0");
ndc->Caption=A->Field[tmd::nc];
name->Caption=A->Field[tmd::asname];
date->Text=TDateTime::CurrentDate().FormatString("ddmmyyyy");//A->Field[dt];
balance= atof(A->Balance);
if ((!A->DiscountGiven)&&(A->Field[tmd::sc][0]=='D'))
discount=ceil(atof(disstr)*(atof(A->Balance)+atof(A->Payments)))/100;
else discheck->Enabled=false;
payment = balance-discount;
remainder=0;
bal->Text= FormatFloat("0.00",balance);
discheck->Caption="Discount "+String(disstr)+"%";
temp = (discheck->Checked)? discount:0;
dis->Text = FormatFloat("0.00",temp);
pay->Text = FormatFloat("0.00",payment);
adj->Text = FormatFloat("0.00",0);
due->Text = FormatFloat("0.00",0);

switch (mainform->FolderName()[1])
{
case 'c':RBact->Checked=true;break;//aCtive
case 'A':RBvat->Checked=true;break; //vAt
case 'r':RBarc->Checked=true;break; //aRchive
}

}
//---------------------------------------------------------------------------
void __fastcall TCredit::discheckClick(TObject *Sender)
{
float temp = (discheck->Checked)? discount:0;
payment = balance-temp;
dis->Text = FormatFloat("0.00",temp);
pay->Text = FormatFloat("0.00",payment);
adj->Text = FormatFloat("0.00",0);
due->Text = FormatFloat("0.00",0);
remainder=0;
}
//---------------------------------------------------------------------------

void __fastcall TCredit::payExit(TObject *Sender)
{
float distemp = (discheck->Checked)? discount:0;
float paytemp = atof(pay->Text.c_str());
remainder = balance - (paytemp + distemp);

if (adjcheck->Checked){
adj->Text = FormatFloat("0.00",remainder);
due->Text = FormatFloat("0.00",0);
                        }
else{
adj->Text = FormatFloat("0.00",0);
due->Text = FormatFloat("0.00",remainder);
}
}
//---------------------------------------------------------------------------

void __fastcall TCredit::adjcheckClick(TObject *Sender)
{
if (adjcheck->Checked){
adj->Text = FormatFloat("0.00",remainder);
due->Text = FormatFloat("0.00",0);
                        }
else{
adj->Text = FormatFloat("0.00",0);
due->Text = FormatFloat("0.00",remainder);
}
}
//---------------------------------------------------------------------------
void TCredit::Store()
{
TAccount* A = (TAccount*)((TListView*)Owner)->Selected->Data;
TList* trans = (TList*)A->Trans;

char **endptr = NULL;
double x = strtod(pay->Text.c_str(),endptr);
if ((x)&&(!endptr))
//if (atof(pay->Text.c_str())>0)
  {
    TCSVRec *T = new TCSVRec;
    T->FieldCount = tmd::transcnt;
    T->Field[tmd::tdate]=date->Text.c_str();
    T->Field[tmd::code]="PAYM";

    T->Field[tmd::proc]= (pay->Text.c_str()[0]=='-')
    ?"Refund":"Payment/Credit";
    
    T->Field[tmd::amt]=pay->Text.c_str();
    trans->Add(T);
  }
x = strtod(dis->Text.c_str(),endptr);
if ((x)&&(!endptr))
//if (atof(dis->Text.c_str())>0)
  {
    TCSVRec *T = new TCSVRec;
    T->FieldCount = tmd::transcnt;
    T->Field[tmd::tdate]=date->Text.c_str();
    T->Field[tmd::code]="DISC";
    T->Field[tmd::proc]="Discount";
    T->Field[tmd::amt] =("-" + dis->Text).c_str();
    trans->Add(T);
  }
x = strtod(adj->Text.c_str(),endptr);
if ((x)&&(!endptr))
//if (atof(adj->Text.c_str())>0)
  {
    TCSVRec *T = new TCSVRec;
    T->FieldCount = tmd::transcnt;
    T->Field[tmd::tdate]=date->Text.c_str();
    T->Field[tmd::code]="ADJ";
    T->Field[tmd::proc]="Adjustment";
    T->Field[tmd::amt]= ("-" + adj->Text).c_str();
    trans->Add(T);
   }

if (RBact->Checked)A->Field[tmd::tag]="";//aCtive
if (RBvat->Checked)A->Field[tmd::tag]="V"; //vAt
if (RBarc->Checked)A->Field[tmd::tag]="D"; //aRchive
A->IsDirty=true;
}


void __fastcall TCredit::BitBtn2Click(TObject *Sender)
{
ModalResult=mrCancel;
Close();
}
//---------------------------------------------------------------------------

void __fastcall TCredit::FormClose(TObject *Sender, TCloseAction &Action)
{
//if (ModalResult==mrCancel)
//   {
//    if (MessageDlg("Save Changes ?", mtConfirmation,
//              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
//        ModalResult=mrOk;
//   }
}
//---------------------------------------------------------------------------

void __fastcall TCredit::BitBtn1Click(TObject *Sender)
{
ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TCredit::RBactClick(TObject *Sender)
{
Tag=1;
}
//---------------------------------------------------------------------------

void __fastcall TCredit::dateExit(TObject *Sender)
{
unsigned short y,m,d;
String str,ys,ms,ds;
str = ((TMaskEdit*)Sender)->EditText;
ys = str.SubString(7,4);
ms = str.SubString(4,2);
ds = str.SubString(1,2);
y = (unsigned short) StrToInt(ys);
m = (unsigned short)StrToInt(ms);
d = (unsigned short)StrToInt(ds);
try{TDateTime(y,m,d);}
catch (EConvertError& ){FocusControl((TMaskEdit*)Sender);}
}
//---------------------------------------------------------------------------

