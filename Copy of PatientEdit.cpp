//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Math.hpp>
#pragma hdrstop
#include "tmFile.h"
#include "PatientEdit.h"
#include "globals.h"
#include "main.h"
#include "Library.h"
#include "Selection.h"
#include "strdate.h"
#include "assist.h"


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPatientEditForm *PatientEditForm;

//---------------------------------------------------------------------------

void TPatientEditForm::LoadComboBoxes()
{
ComboBox2->Items->Clear();
ComboBox3->Items->Clear();
ComboBox4->Items->Clear();
DiagBox->Items->Clear();
diagpos=0;

for (int i=0;i<mainform->TMData->Clinics->Count ;i++)
ComboBox2->Items->Add(mainform->TMData->Clinics->Strings[i]);

for (int i=0;i<mainform->TMData->Referred->Count ;i++)
ComboBox3->Items->Add(mainform->TMData->Referred->Strings[i]);

for (int i=0;i<mainform->TMData->Diagnoses->Count ;i++)
DiagBox->Items->Add(mainform->TMData->Diagnoses->Strings[i]);
//DiagBox->Items->Add("");

for (int i=0;i<mainform->TMData->MedAidStrings->Count ;i++)
{
ComboBox4->Items->AddObject(
mainform->TMData->MedAidStrings->Strings[i],
mainform->TMData->MedAidStrings->Objects[i]);
}
ComboBox1->ItemIndex=0;//ComboBox1->Text=ComboBox1->Items->Strings[0];
ComboBox5->ItemIndex=0;//ComboBox5->Text=ComboBox5->Items->Strings[0];
ComboBox6->ItemIndex=0;//ComboBox6->Text=ComboBox6->Items->Strings[0];
ComboBox7->ItemIndex=0;//ComboBox7->Text=ComboBox7->Items->Strings[0];
ComboBox2->Text="";
ComboBox3->Text="";
ComboBox4->Text="";

}

//---------------------------------------------------------------------------
__fastcall TPatientEditForm::TPatientEditForm(TComponent* Owner)
    : TForm(Owner)//,m_dat(T)
{

//char str[20];
//sprintf(str,);

//Label9->Caption = "Med Aid +\nNumber";
SimilarTrans = new TList();
StringGrid1->Cells[0][0]="Date";
StringGrid1->Cells[1][0]="Code";
StringGrid1->Cells[2][0]="Description";
StringGrid1->Cells[3][0]="Amount";
LoadComboBoxes();
Load();
ActiveControl=ComboBox7;
DiagBox->AutoComplete=false;
//Tag=0;
}
//---------------------------------------------------------------------------
int TPatientEditForm::TimeValue(char* str)
{

//return 0;
char s[100];
strcpy(s,",");
strcat(s,str);
for (unsigned i=0;i<strlen(s);i++)
if ((s[i]!='.')&&(!isdigit(s[i])))  s[i]=',';


char *tok1;
char *tok2=NULL;
tok1 = strtok(s,",");
if (tok1 != NULL) tok2 = strtok(NULL,",");
double f1=0;
if (tok1)
{
char **endptr = NULL;
f1 = strtod(tok1,endptr);
if (endptr) f1=0;
}
double f2;
if (tok2=="") tok2 = NULL;
if (tok2)
{
char **endptr = NULL;
f2 = strtod(tok2,endptr);
if (endptr) f2=0;
}
else return int(f1);

 double m1= ceil(int(f1)*60 + (f1-int(f1))*99.99);
 double m2= ceil(int(f2)*60 + (f2-int(f2))*99.99);
 while(m2<m1) m2 += 720;
 return int(m2-m1);

}
//---------------------------------------------------------------------------
void TPatientEditForm::Payment_Balance()
{
double pay = 0;
double subtotal = 0;
double temp;
double disc;
double interest=0;

double intrate = (mainform->TMData->Setup)?
mainform->TMData->GetInterestRate():0;

bool interestflag=Interest1->Checked;

Edit6->Enabled =  (ComboBox5->ItemIndex==2);
ddaysedit->Enabled = (ComboBox5->ItemIndex==2);

for (int i=1;i<StringGrid1->RowCount ;i++)
{
try  {temp = StrToFloat(StringGrid1->Cells[3][i]);}
catch (EConvertError &e) {temp = 0;}
int t_age = TStrDate(StringGrid1->Cells[0][i].c_str()).Age();

if (!strcmp(Trim(StringGrid1->Cells[1][i]).c_str(),"PAYM"))
   {pay += temp;
   if ((interestflag)&&(t_age > 60))
       interest -= (temp * exp(intrate / 365 * t_age) - temp);
   }
else
   {subtotal += temp;
   if ((interestflag)&&(t_age > 60))
       interest += (temp * exp(intrate / 365 * t_age) -temp);
   }



if  (!strncmp(Trim(StringGrid1->Cells[1][i]).c_str(),"DIS",3))
{
Edit6->Enabled =   false;
ddaysedit->Enabled =   false;}

}

//discount
try  {temp = StrToFloat(Edit6->Text);}
catch (EConvertError &e) {temp = 0;}
disc = (Edit6->Enabled)? temp*subtotal/100 : 0;
interest = SimpleRoundTo(interest);
disc = SimpleRoundTo(disc);
//due
temp = subtotal + interest - (disc + pay) ;

Edit1->Text=FormatFloat("0.00",subtotal);
Edit2->Text=FormatFloat("0.00",disc);
Edit3->Text=FormatFloat("0.00",interest);
Edit4->Text=FormatFloat("0.00",pay);
Edit5->Text=FormatFloat("0.00",temp);
}
//---------------------------------------------------------------------------
char*TPatientEditForm::Fee(char* c,int t)
{

TCSVRec*S = mainform->TMData->Setup;
if (!S) return "";
else
{
static char s[10];
strcpy(s,"");
double f = 0;
int pu=0,cu=0,cl=0,rv=0;
bool gaz=false;
bool noround=false;

// get the unit values
switch (ComboBox5->Items->Strings[ComboBox5->ItemIndex].c_str()[0])
{
case 'C':
case 'E':
case 'M': //try to get scale from medical aid list
          {int i = ComboBox4->Items->
         IndexOf(ComboBox4->Text);
          if (i>-1){TCSVRec *T =
            (TCSVRec*)ComboBox4->Items->Objects[i];
          if (T) {
                  pu = atoi(T->Field[tmd::mproc]);
                  cl = atoi(T->Field[tmd::mclin]);
                  rv = atoi(T->Field[tmd::mrvu]);
                  cu = atoi(T->Field[tmd::mcons]);
                  }}
          }
          pu = (pu)? pu :atoi(S->Field[tmd::gpu]);
          cu = (cu)? cu :atoi(S->Field[tmd::gcu]);
          cl = (cl)? cl :atoi(S->Field[tmd::gcl]);
          rv = (rv)? rv :atoi(S->Field[tmd::gvu]);
          gaz=true;

        break;
case 'W': pu = atoi(S->Field[tmd::wpu]);
          cu = atoi(S->Field[tmd::wcu]);
          cl = atoi(S->Field[tmd::wcl]);
          rv = atoi(S->Field[tmd::wvu]);
        break;
case 'S':
case 'D': pu = atoi(S->Field[tmd::mpu]);
          cu = atoi(S->Field[tmd::mcu]);
          cl = atoi(S->Field[tmd::mcl]);
          rv = atoi(S->Field[tmd::mvu]);
        break;
case 'P': pu = 1.2 * atoi(S->Field[tmd::mpu]);
          cu = 1.2 * atoi(S->Field[tmd::mcu]);
          cl = 1.2 * atoi(S->Field[tmd::mcl]);
          rv = 1.2 * atoi(S->Field[tmd::mvu]);
        break;
}

if ((!pu)&&(!cu)&&(!cl)&&(!rv))
{pu = cu = cl = rv =100;noround = true;}


if (t)

// first process the time codes
switch (atoi(c))
{
//case 0:break; // c!=a number
case 23:
case 25: f = (int)(t+14.5)/15;
    f = (f<5)? f * 2 * pu :
        ((f-4) * pu * 3) +(4 * pu * 2);
    break;

case 36: f = (int)(t+14.5)/15;
    f = (f<5)? f * 2 * pu :
        (((f-4) * pu * 3 ) + (4 * pu * 2))* 4/5;
       break;

case 39: f = (int)(t+14.5)/15;
         f =  (f<5)?3:f-1;
         f *= pu;
         break;

case 11: f = (int)(t+29.5)/30;
         f *= 12 * cl;
         break;

case 12: f = (int)(t+29.5)/30;
         f *= 12 * cl;
         break;

case 1211: f = (int)(t+29)/30;
        f = (f>3) ? 4:f;
        f = (f<3) ? f * cl * 50 : (f*25+50)*cl;
        break;

}
//sprintf(s,"%5.2f",f/100);

else
{
TCSVRec * T = mainform->TMData->LibLookup(c);
if (T)
{
//f= atoi(T->Field[un]);
try {f = String(T->Field[tmd::un]).ToDouble();}
catch (EConvertError &) { f=0; }
switch (strupr(T->Field[tmd::cp])[0])
{
case 'C': f *= cu; break;
case 'P': f *= pu; break;
case 'L': f *= cl; break;
case 'R': f *= rv; break;
}
}
}
//if gazette then handle the rounding to one cent
//on modifiers

if ((noround)||((gaz)&&((atoi(c)<50)||(atoi(c)>5000))))
sprintf(s,"%5.2f",f/100);
else
{
f=floor((f+5)/10);
sprintf(s,"%5.2f",f/10);
}

//lookup the codes and process all the other possibiliies
//f = rv+cl+cu; //adapt

return s;
}
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TPatientEditForm::LoadTrans(TAccount *P)
{
TList *T = (TList*)P->Trans;
if (!T->Count) AllowTemplates=true;
StringGrid1->RowCount = (T) ? T->Count+ 2 : 2;
if (T){
for (int i=0;i<T->Count;i++)
      {
//tid,tdate,code,proc,amt
StringGrid1->Cells[0][i +1]=
    date2000(((TCSVRec*)T->Items[i])->Field[tmd::tdate]);
StringGrid1->Cells[1][i +1]=
    ((TCSVRec*)T->Items[i])->Field[tmd::code];
StringGrid1->Cells[2][i +1]=
    ((TCSVRec*)T->Items[i])->Field[tmd::proc];
StringGrid1->Cells[3][i +1]=
    ((TCSVRec*)T->Items[i])->Field[tmd::amt];
    }
}
}
//---------------------------------------------------------------------------

void TPatientEditForm::Load()
{
TAccount* P;
//Tag=0;
P = mainform->ToDuplicate;
if (!P)
{
TListItem* T = ((TListView*)Owner)->ItemFocused;
P = (T) ? (TAccount*) T->Data:NULL;
}
if (P)
{
//Tag=1;
ndx->Caption = (mainform->ToDuplicate) ? String(TAccount::ndc+1) : String(P->Field[tmd::nc]);
FileNum->Text = P->Field[tmd::filenum];
Label10->Caption=P->Field[tmd::asname];
title->Text=P->Field[tmd::atitle];
init->Text=P->Field[tmd::ainit];
sname->Text=P->Field[tmd::asname];
patname->Text=P->Field[tmd::pname];

Birthdate->Text=P->Field[tmd::pbday];
//Birthdate->Text=TDateTime::CurrentDate().FormatString("ddmmyyyy");

//sex->Text=P->Field[psex];
ComboBox1->ItemIndex = (P->Field[tmd::psex][0]=='M')? 0:1;
ComboBox7->ItemIndex = (P->Field[tmd::lang][0]=='E')? 0 : 1;
//nx->Text=P->Field[nc];
addr1->Text=P->Field[tmd::ad1];
addr2->Text=P->Field[tmd::ad2];
addr3->Text=P->Field[tmd::ad3];
addr4->Text=P->Field[tmd::ad4];
ComboBox4->Text=P->Field[tmd::ma];
manum->Text=P->Field[tmd::man];
ComboBox2->Text=P->Field[tmd::clin];
ComboBox3->Text=P->Field[tmd::ref];
//scale->Text=P->Field[sc];

if (mainform->ToDuplicate)
   AccdatePick->Date= TDateTime::CurrentDate();
else
  {
  TStrDate* sd = new TStrDate(P->Field[tmd::dt]);
  AccdatePick->Date = TDateTime(sd->Year(),sd->Month(),sd->Day());
  delete sd;
  }
//AccdatePick->Date= (mainform->ToDuplicate)?
//TDateTime::CurrentDate():
//TDateTime::CurrentDate();//String);
//Edit1->Text=P->Billed;
Edit6->Text = (atoi(P->Field[tmd::dsc]))? P->Field[tmd::dsc]:"0";
idnum->Text=P->Field[tmd::patid];
DiagBox->Text="";
authedit->Text="";
ioddate->Text=P->Field[tmd::ioddt];
ComboBox6->ItemIndex = P->Field[tmd::usm][0]-'0';
SetScaleBox(P->Field[tmd::sc][0]);
Interest1->Checked=(strlen(P->Field[tmd::sc])>1);
//int x=atoi(NULL);
if (!atoi(P->Field[tmd::ddys]))P->Field[tmd::ddys]="30";
ddaysedit->Text = P->Field[tmd::ddys];
AllowTemplates= (mainform->ToDuplicate) ? true:false;

if (!mainform->ToDuplicate)
{
DiagBox->Text=P->Field[tmd::diag];
authedit->Text=P->Field[tmd::auth];
 LoadTrans(P);
 Payment_Balance();

for (int i=0;i<P->Notes->Count;i++)
{
TCSVRec *T= (TCSVRec*)P->Notes->Items[i];
Memo1->Lines->Add(T->Field[tmd::pnote]);
}
}
}

else {
   Clear();
   Label10->Caption="New Account";
   AllowTemplates=true;
   ndx->Caption= TAccount::ndc+1;
   AccdatePick->Date = TDateTime::CurrentDate();
   ddaysedit->Text="30";
   LoadComboBoxes();
     }
UpdateMessage();
//ComboBox7->SetFocus();
if (mainform->ToDuplicate) Tag=1; else Tag=0;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void TPatientEditForm::ClearTrans()
{
for (int i=1;i<StringGrid1->RowCount;i++)
    for (int j=0;j<4;j++)
StringGrid1->Cells[j][i]="";
StringGrid1->RowCount=3;
}

//---------------------------------------------------------------------------

void TPatientEditForm::Clear()
{
for (int i=0;i<ComponentCount ;i++)
{
if (Components[i]->ClassNameIs("TEdit")) ((TEdit*)Components[i])->Text="";
if (Components[i]->ClassNameIs("TComboBox")) ((TComboBox*)Components[i])->Text="";
if (Components[i]->ClassNameIs("TMaskEdit")) ((TMaskEdit*)Components[i])->Text="";
}
ClearTrans();
Memo1->Lines->Clear();
}

//---------------------------------------------------------------------------
bool TPatientEditForm::EmptyRow(int x)
{   return
    (
    //(StringGrid1->Cells[0][x].IsEmpty())&&
    (StringGrid1->Cells[1][x].IsEmpty())&&
    (StringGrid1->Cells[2][x].IsEmpty())//&&
    //(StringGrid1->Cells[3][x].IsEmpty())
    );
}
//---------------------------------------------------------------------------


void TPatientEditForm::Save()
{
Tag=0;
mainform->TMData->isDirty = true;
TAccount* P;
TListItem* T = ((TListView*) Owner)->ItemFocused;
if (mainform->ToDuplicate)
{
 P = new TAccount;
 mainform->TMData->Patients->Add(P);
 P->FieldCount = tmd::patcnt;
 P->Field[tmd::tag]="";
 P->Field[tmd::pid]="3";
 P->Field[tmd::nc]= ndx->Caption.c_str();
 P->Check_ndc();
}
else if (T) P = (TAccount*)T->Data;
else
{
P = new TAccount;
mainform->TMData->Patients->Add(P);
P->FieldCount = tmd::patcnt;
P->Field[tmd::tag]="";
P->Field[tmd::pid]="3";
P->Field[tmd::nc]= ndx->Caption.c_str();
P->Check_ndc();
}

P->Field[tmd::filenum]= FileNum->Text.c_str();
P->Field[tmd::atitle]=title->Text.c_str();
P->Field[tmd::ainit]=init->Text.c_str();
P->Field[tmd::asname]=sname->Text.c_str();
P->Field[tmd::pname]=patname->Text.c_str();
P->Field[tmd::pbday]=Birthdate->Text.c_str();
//patbday->Text.c_str();
//P->Field[tmd::pbday]=patbday->Text.c_str();
P->Field[tmd::psex]=(ComboBox1->ItemIndex==0)?"M":"F";
P->Field[tmd::lang]=(ComboBox7->ItemIndex==0)?"E":"A";

//sex->Text.c_str();

//patdialog->ndx->Text=P->Field[nc];
P->Field[tmd::ad1]=addr1->Text.c_str();
P->Field[tmd::ad2]=addr2->Text.c_str();
P->Field[tmd::ad3]=addr3->Text.c_str();
P->Field[tmd::ad4]=addr4->Text.c_str();
P->Field[tmd::ma]=ComboBox4->Text.c_str();
P->Field[tmd::man]=manum->Text.c_str();
P->Field[tmd::clin]=ComboBox2->Text.c_str();
P->Field[tmd::ref]=ComboBox3->Text.c_str();
P->Field[tmd::dt]=AccdatePick->Date.FormatString("ddmmyyyy").c_str();
P->Field[tmd::diag]=DiagBox->Text.c_str();
P->Field[tmd::auth]= authedit->Text.c_str();
P->Field[tmd::dsc]=Edit6->Text.c_str();
P->Field[tmd::patid]=idnum->Text.c_str();
P->Field[tmd::ioddt]=ioddate->Text.c_str();
P->Field[tmd::ddys]= ddaysedit->Text.c_str();


switch (ComboBox6->ItemIndex)
{
case 0:P->Field[tmd::usm] = "0";break;
case 1:P->Field[tmd::usm] = "1";break;
case 2:P->Field[tmd::usm] = "2";break;
case 3:P->Field[tmd::usm] = "3";break;
}
switch (ComboBox5->ItemIndex)
{
case 0:P->Field[tmd::sc]="G";break;
case 1:P->Field[tmd::sc]="C";break;
case 2:P->Field[tmd::sc]="D";break;
case 3:P->Field[tmd::sc]="E";break;
case 4:P->Field[tmd::sc]="W";break;
case 5:P->Field[tmd::sc]="M";break;
case 6:P->Field[tmd::sc]="P";break;
}
String str = P->Field[tmd::sc];
if (Interest1->Checked) {str+='I';P->Field[tmd::sc]=str.c_str();};

TList* trans = (TList*)P->Trans;
for (int i= 0;i<trans->Count;i++)
    delete (TCSVRec*)trans->Items[i];

trans->Clear();

//TCSVRec *trans = (TCSVRec*)rec->Data;
for (int i = 1;i< StringGrid1->RowCount;i++)
{
if (!EmptyRow(i))
{
double temp;
char str[10];
TCSVRec *T = new TCSVRec;
T->FieldCount = tmd::transcnt;

//T->Index = nx;
    T->Field[tmd::tdate]=StringGrid1->Cells[0][i].c_str();
    T->Field[tmd::code]=StringGrid1->Cells[1][i].c_str();
    T->Field[tmd::proc]=StringGrid1->Cells[2][i].c_str();
try  {temp = StrToFloat(StringGrid1->Cells[3][i]);}
catch (EConvertError &e) {temp = 0;}
sprintf(str,"%9.2f",temp);
    T->Field[tmd::amt]=str;
trans->Add(T);
}
}
TList* notes = (TList*)P->Notes;
for (int i= 0;i<notes->Count;i++)
            delete (TCSVRec*)notes->Items[i];
notes->Clear();

for (int i=0;i<Memo1->Lines->Count;i++)
{
TCSVRec *T= new TCSVRec;
T->FieldCount = tmd::notecnt;
T->Field[tmd::pnote]= Memo1->Lines->Strings[i].c_str();
notes->Add(T);
}

mainform->TMData->Clinics->Add(P->Field[tmd::clin]);
mainform->TMData->Referred->Add(P->Field[tmd::ref]);
mainform->TMData->Diagnoses->Add(P->Field[tmd::diag]);
mainform->TMData->MedAidStrings->AddObject(P->Field[tmd::ma],NULL);

P->IsDirty=true;
//m_medaid->Add(p->Field[ma]);

//LoadComboBoxes();
//Tag=0;
//AllowTemplates=false;
//if (!((TListView*)Owner)->ItemFocused) ModalResult=1;
}
//---------------------------------------------------------------------------


void __fastcall TPatientEditForm::StringGrid1GetEditMask(TObject *Sender,
      int ACol, int ARow, AnsiString &Value)
{
{
switch (ACol)
{
 case 0: Value = "00/00/0000;0;";break;
//case 0:Value = "!00/00/0000;0;_";break;
case 1:Value = "";break;
case 2:Value = "";break;
case 3:Value = "";break;
}
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
if (Tag)
{
    if (MessageDlg("Save Changes ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
//{
Save();
//mainform->Save();
//}
}
delete SimilarTrans;
mainform->ToDuplicate=NULL;
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::BitBtn4Click(TObject *Sender)
{
//TListView* T =(TListView*)Owner;
//TListItem* P = T->ItemFocused;
//if (P)
//{
//if (Tag)
//{
//if (MessageDlg("Keep Changes ?", mtConfirmation,
//              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
//{
//Save();
//mainform->Save();
//}
//}
// if (!Tag)
//{
//     int i = P->Index;
//     if (i) {i--;P->Focused=false;}
 //    T->Items->Item[i]->Focused=true;
 //    Clear();
 //    Load();
//    }
//TabSheet2Enter(this);
//}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::BitBtn5Click(TObject *Sender)
{
TListView* T =(TListView*)Owner;
TListItem* P = T->ItemFocused;
if (P)
{
if (Tag)
//{
    if (MessageDlg("Keep Changes ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
//{
Save();
//mainform->Save();
//}
//}
//if (!Tag)
//{

     int i = P->Index;
 if (((TBitBtn*)Sender)->Name=="BitBtn5")
     {if (i<T->Items->Count-1) {i++;P->Focused=false;}}
 else
    if (i) {i--;P->Focused=false;}
    
     T->Items->Item[i]->Focused=true;
     Clear();
     Load();
//}
TabSheet2Enter(this);
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::Edit6Exit(TObject *Sender)
{
Payment_Balance();
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::BitBtn2Click(TObject *Sender)
{
Close();
}

//ModalResult=1;
//if (LibForm) delete LibForm;

//TListItem* T = ((TListView*)Owner)->ItemFocused;
//TAccount* P=NULL;
//if ((T)||(saved)) P = (saved)? saved : (TAccount*) T->Data;
//if (PageControl1->ActivePage==TabSheet2)
//{
//ClearTrans();
//if (P) LoadTrans(P);
//}
//else
//{
//Clear();
//Load();
//}
//SimilarTrans->Clear();
//diagpos=0;

//---------------------------------------------------------------------------
/*
void __fastcall TPatientEditForm::dateExit(TObject *Sender)
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

void __fastcall TPatientEditForm::patbdayExit(TObject *Sender)
{
if (((TMaskEdit*)Sender)->EditText!="__/__/____") dateExit(Sender);
}
//---------------------------------------------------------------------------
*/



void __fastcall TPatientEditForm::StringGrid1Exit(TObject *Sender)
{
unsigned short y,m,d;
         String str,ys,ms,ds;
int i = 1;
int j = StringGrid1->RowCount;

while (i<StringGrid1->RowCount)
        {
         str = StringGrid1->Cells[0][i];
         if (!str.IsEmpty())
         {
        ys = str.SubString(5,4);
         ms = str.SubString(3,2);
          ds = str.SubString(1,2);
         y = (unsigned short) StrToInt(ys);
         m = (unsigned short)StrToInt(ms);
         d = (unsigned short)StrToInt(ds);
          try{TDateTime(y,m,d);}
          catch (EConvertError& ){j=i;//StringGrid1->Cells[0][i]="";
          //StringGrid1->Cells[0][i] = date->Text;

             }
             }
       i++;}

   if    (j<StringGrid1->RowCount)
   {
   MessageDlg("Date Error", mtWarning,
              TMsgDlgButtons() << mbOK, 0);
StringGrid1->SetFocus();
StringGrid1->Col = 0; StringGrid1->Row = j;
//StringGrid1->EditorMode=true;
}
}
//---------------------------------------------------------------------------



void __fastcall TPatientEditForm::StringGrid1KeyDown(TObject *Sender,
      WORD &Key, TShiftState Shift)
{
switch (Key)
{
case VK_INSERT:  if (Shift.Contains(ssCtrl))
                   InsertGridLine();
                  else InsertFromLibrary();
                  break;
//case VK_BACK:
case VK_DELETE:  if (Shift.Contains(ssCtrl))
                      DeleteGridLine();
                 else DeleteGridField();
                      Payment_Balance();
                 break;
case VK_TAB:
case VK_UP:
case VK_DOWN:
case VK_RIGHT:
case VK_LEFT: //if (StringGrid1->Row==3)
                 Payment_Balance();
                 break;

}
//Payment_Balance();
}
//---------------------------------------------------------------------------




void __fastcall TPatientEditForm::FormKeyPress(TObject *Sender, char &Key)
{
if (Key!=VK_TAB)Tag = 1;
}
//---------------------------------------------------------------------------



void __fastcall TPatientEditForm::BitBtn1Click(TObject *Sender)
{
if (Tag)
Save();
Tag = 0;
}
//---------------------------------------------------------------------------


void __fastcall TPatientEditForm::UpDown1Click(TObject *Sender,
      TUDBtnType Button)
{
//StringGrid1->SetFocus();
//static int diagpos = 0;
if (SimilarTrans->Count)
{
switch (Button)
{
case  Comctrls::btNext: diagpos++;
     if (diagpos==SimilarTrans->Count)diagpos=SimilarTrans->Count-1;
                   ;break;
case  Comctrls::btPrev: diagpos--;
     if (diagpos<0) diagpos=0;
break;
}

if (AllowTemplates)
{
ClearTrans();
LoadTrans((TAccount*)SimilarTrans->Items[diagpos]);
for (int i=1;i<StringGrid1->RowCount;i++)
{StringGrid1->Cells[3][i]="";StringGrid1->Cells[0][i]="";}
}
}
}
//---------------------------------------------------------------------------


void TPatientEditForm::UpdateMessage()
{
//Payment_Balance();
char *str = new char[240];
int lang = ComboBox7->ItemIndex;
int mestpe;
switch (ComboBox5->ItemIndex)
{
case 0:
case 1:  mestpe=TData::m_gaz; break;
case 2:  mestpe=TData::m_dis; break;
case 3:  mestpe=TData::m_edi; break;
case 4:
case 5:
case 6:  mestpe=TData::m_mas; break;
//case 7:
}
if (ComboBox6->ItemIndex==1)mestpe=TData::m_us1;
if (ComboBox6->ItemIndex==2)mestpe=TData::m_us2;
if (ComboBox6->ItemIndex==3)mestpe=TData::m_nil;
if  (mestpe==TData::m_dis)
{
TStrDate sd(AccdatePick->Date.FormatString("ddmmyyyy").c_str());
//int x= sd.Age();
//int y= mainform->TMData->DiscDays();
//if (x>y)
//if (sd.Age() > mainform->TMData->DiscDays())
if (sd.Age() > atoi(ddaysedit->Text.c_str()))
mestpe=TData::m_mas;
}
//TDateTime d;
//try {
// d = TDateTime(sd.Year(),sd.Month(),sd.Day());
//   }
// catch (EConvertError &)
// {d= TDateTime::CurrentDate();}

//if (!mainform->TMData->Setup) d+=30;
//else
//{
//if (!atoi(mainform->TMData->Setup->Field[ddays]))
//        mainform->TMData->Setup->Field[ddays]="30";

//d += atoi(mainform->TMData->Setup->Field[ddays]);
//}
//if (TDateTime::CurrentDate() > d) mestpe=TData::m_mas;
//}

strcpy(str,mainform->TMData->MessageLine(mestpe,
                       lang,
                        0,
                        AccdatePick->Date.FormatString("ddmmyyyy").c_str(),
                        Edit6->Text.c_str(),
                        atoi(ddaysedit->Text.c_str()),
                        Edit1->Text.c_str()));
strcat(str,"\n");
strcat(str,mainform->TMData->MessageLine(mestpe,
                       lang,
                        1,
                        AccdatePick->Date.FormatString("ddmmyyyy").c_str(),
                        //date->Text.c_str(),
                        Edit6->Text.c_str(),
                        atoi(ddaysedit->Text.c_str()),
                        Edit1->Text.c_str()));
strcat(str,"\n");
strcat(str,mainform->TMData->MessageLine(mestpe,
                       lang,
                        2,
                        AccdatePick->Date.FormatString("ddmmyyyy").c_str(),
                        //date->Text.c_str(),
                        Edit6->Text.c_str(),
                        atoi(ddaysedit->Text.c_str()),
                        Edit1->Text.c_str()));
Label1->Caption=str;
delete[] str;
}

//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ComboBox7Change(TObject *Sender)
{
//Tag=1;
if ((addr1->Text=="P O Box ")||
     (addr1->Text=="Posbus ")||
     (addr1->Text=="P O BOX ")||
     (addr1->Text=="POSBUS ")||
     (addr1->Text.IsEmpty()))
{
addr1->Text = (ComboBox7->ItemIndex)?"Posbus ":"P O Box ";


if (mainform->TMData->Setup)
if (mainform->TMData->Setup->Field[tmd::pref][1]=='1')
addr1->Text=UpperCase(addr1->Text);
}

if ((title->Text.IsEmpty())||
    (title->Text=="Mr")||
    (title->Text=="MR")||
   (title->Text=="MNR")||
   (title->Text=="Mnr"))
{
title->Text = (ComboBox7->ItemIndex)?"Mnr":"Mr";
if(mainform->TMData->Setup)
if (mainform->TMData->Setup->Field[tmd::pref][1]=='1')
title->Text=UpperCase(title->Text);
}
if (!atoi(Edit6->Text.c_str()))
Edit6->Text=LastDiscount();

if (Sender==ComboBox5)ComboBox6->ItemIndex=0;
//if (!atoi(ddaysedit->Text.c_str()))
//ddaysedit->Text = 30;
UpdateMessage();
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::StringGrid1SelectCell(TObject *Sender,
      int Col, int Row, bool &CanSelect)
{
//enum{dte,cde,dsc,amt};
CanSelect = true;
String prom;// = " ";//liblookup will replace space with
TCSVRec *T;
//if ((LastRow!=Row)||(Col!=3))
     if (LastLibraryItem) // the last select cell added a new library code
         EditLibraryItem(LastLibraryItem,LastRow);
//Payment_Balance();
switch (Col)
{
case 0:if (StringGrid1->Cells[Col][Row].IsEmpty())
         {if (Row==1)
            StringGrid1->Cells[Col][Row]= AccdatePick->Date.FormatString("ddmmyyyy");
        else StringGrid1->Cells[Col][Row]=StringGrid1->Cells[0][Row-1];}
            break;

case 1: break;


case 2: if (StringGrid1->Cells[Col][Row].IsEmpty())
   {
   T=mainform->TMData->LibLookup(StringGrid1->Cells[1][Row].c_str());
   char* e = (T) ?T->Field[tmd::leng]:"";
   char* a = (T)?T->Field[tmd::lafr]:"";
   if (!strlen(a)) a=e;
   if (!strlen(e)) e=a;
    prom = (ComboBox7->ItemIndex)? a:e;
    StringGrid1->Cells[Col][Row]= prom;
 //   inlibrary = !prom.IsEmpty();

    }
         break;

case 3:
AllowTemplates=false;
T=mainform->TMData->LibLookup(StringGrid1->Cells[1][Row].c_str());
if ((!T)&&
   (!StringGrid1->Cells[1][Row].IsEmpty())&&
   (!StringGrid1->Cells[2][Row].IsEmpty())&&
   (StringGrid1->Cells[3][Row].IsEmpty()))
 {
   if (Application->MessageBox("Add New Code to Library", "TurboMED", MB_OKCANCEL +
             MB_DEFBUTTON1)==IDOK)
      {  TCSVRec *R = new TCSVRec;
        R->FieldCount = tmd::libcnt;
         if (mainform->TMData->Library)
         mainform->TMData->Library->Add(R);
        R->Field[tmd::lid] = "2";
        R->Field[tmd::lcode]= StringGrid1->Cells[1][Row].c_str();
        R->Field[tmd::un]="O";
        R->Field[tmd::cp]="P";
        R->Field[tmd::leng]=StringGrid1->Cells[2][Row].c_str();
        R->Field[tmd::lafr]=StringGrid1->Cells[2][Row].c_str();
        LastRow=Row;
        if (!LastLibraryItem) LastLibraryItem = (int)R;
        }  else LastLibraryItem =0;

  }
//         prom = StringGrid1->Cells[2][Row];
         if (Row > StringGrid1->RowCount-2) StringGrid1->RowCount++;
         if (StringGrid1->Cells[3][Row].IsEmpty())
                   {
                   AddFeeToGrid(Row,100); // 100% fee
                   Tag = 1;
                   }
//         int atime=0;
//         if (TimeCode(StringGrid1->Cells[1][Row].c_str()))//(atime)
//         {
//         atime = TimeValue(StringGrid1->Cells[2][Row].c_str());
//         if ((!atime)&&(Row)) atime = TimeValue(StringGrid1->Cells[2][Row-1].c_str());
//         T = mainform->TMData->LibLookup(StringGrid1->Cells[1][Row].c_str());
//         prom = (T) ? T->Field[tmd::leng]:"";
//         StringGrid1->Cells[2][Row] = prom+" "+IntToStr(atime)+" min";
//         }
//         StringGrid1->Cells[3][Row]=
//         Fee(StringGrid1->Cells[1][Row].c_str(),atime);
         //Payment_Balance();
//         }
         Payment_Balance();
         break;

}

}

//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::StringGrid1Enter(TObject *Sender)
{
if (StringGrid1->Cells[0][1].IsEmpty())
StringGrid1->Cells[0][1]= AccdatePick->Date.FormatString("ddmmyyyy");
}
//---------------------------------------------------------------------------



void __fastcall TPatientEditForm::ComboBox5Exit(TObject *Sender)
{
switch(ComboBox5->ItemIndex)
{
case 3:
if (!EDIValid())
{
ShowMessage("Invalid EDI Details");
ComboBox5->ItemIndex=0;
//UpdateMessage();
Edit6->Enabled =  false;
ddaysedit->Enabled = false;
}
break;

case 2:
Edit6->Enabled = true;
ddaysedit->Enabled = true;
if (!atoi(Edit6->Text.c_str()))
Edit6->Text=LastDiscount();
//if (!atoi(ddaysedit->Text.c_str()))
//ddaysedit->Text = 30;
//TabSheet2Enter(this);
//Payment_Balance();
break;

default:
//it6->Text = "";
{
ddaysedit->Enabled = false;
Edit6->Enabled = false;
}
}
UpdateMessage();
Payment_Balance();
}
//---------------------------------------------------------------------------


void __fastcall TPatientEditForm::ComboBox7DropDown(TObject *Sender)
{
Tag = 1;        
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::snameExit(TObject *Sender)
{
NewAccount= patname->Text.IsEmpty();
if (NewAccount)
{
patname->Text = sname->Text;
//TList * P = mainform->TMData->Patients;
//TAccount* A =(P->Count > 0)? (TAccount*)P->Items[P->Count-1]:NULL;

//if (A)
//{
//ComboBox2->Text=A->Field[clin];
//omboBox3->Text=A->Field[ref];
//SetScaleBox(A->Field[sc][0]);
//ComboBox5->Text = A->Field[sc];
//}
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::snameKeyPress(TObject *Sender, char &Key)
{
//Tag = 1;        
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::addr4Enter(TObject *Sender)
{
char *str=NULL;
char *str2=NULL;
for (int i = mainform->TMData->Patients->Count-1;
i>-1;i--)
{
TAccount* A = (TAccount*)mainform->TMData->Patients->Items[i];

if  (
     SameName(addr2->Text.c_str(),A->Field[tmd::ad2])||
     SameName(addr3->Text.c_str(),A->Field[tmd::ad3])||
     SameName(addr2->Text.c_str(),A->Field[tmd::ad3])||
     SameName(addr3->Text.c_str(),A->Field[tmd::ad2])
     )
{if ((AddressType(addr1->Text.c_str()))==(AddressType(A->Field[tmd::ad1])))
{str = A->Field[tmd::ad4];break;}
 else str2=A->Field[tmd::ad4];}
}
//if (!str)
//for (int i = mainform->TMData->Patients->Count-1;
//i>-1;i--)
//{
//TAccount* A = (TAccount*)mainform->TMData->Patients->Items[i];
//if ((strlen(addr3->Text.c_str()))&&(strlen(A->Field[ad3])))
//{
//if  (!stricmp(addr3->Text.c_str(),A->Field[ad3]))
//{if (AddressType(addr1->Text.c_str())==AddressType(A->Field[ad1]))
//{str = A->Field[ad4];break;}
//else str2=A->Field[ad4];}
//}}
if (!str) str=str2;
if (addr4->Text.IsEmpty())
addr4 -> Text = (str) ? str:"";
}
//---------------------------------------------------------------------------


void __fastcall TPatientEditForm::addr2Exit(TObject *Sender)
{
if ((!addr2->Text.IsEmpty())&&(NewAccount))
{
char *str=NULL;
for (int i = mainform->TMData->Patients->Count-1;
i>-1;i--)
{
TAccount* A = (TAccount*)mainform->TMData->Patients->Items[i];
if  (!strnicmp(addr2->Text.c_str(),A->Field[tmd::ad2],
strlen(addr2->Text.c_str())))
{str = A->Field[tmd::ad2];break;}
}
if (str) addr2->Text = str;
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::addr3Exit(TObject *Sender)
{
if ((!addr3->Text.IsEmpty())&&(NewAccount))
{
char *str=NULL;
for (int i = mainform->TMData->Patients->Count-1;
i>-1;i--)
{
TAccount* A = (TAccount*)mainform->TMData->Patients->Items[i];
if  (!strnicmp(addr3->Text.c_str(),A->Field[tmd::ad3],
strlen(addr3->Text.c_str())))
{str = A->Field[tmd::ad3];break;}
}
if (str) addr3->Text = str;
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ComboBox2Exit(TObject *Sender)
{
if (NewAccount)
{
String cbs = UpperCase(ComboBox2->Text);
for (int i = 0;i<ComboBox2->Items->Count;i++)
{
String look = UpperCase(ComboBox2->Items->Strings[i]);
if (look.Pos(cbs)==1)
{ComboBox2->Text = ComboBox2->Items->Strings[i];break;}
}
}
}

//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ComboBox3Exit(TObject *Sender)
{
if (NewAccount)
{
String cbs = UpperCase(ComboBox3->Text);
for (int i = 0;i<ComboBox3->Items->Count;i++)
{
String look = UpperCase(ComboBox3->Items->Strings[i]);
if (look.Pos(cbs))
{ComboBox3->Text = ComboBox3->Items->Strings[i];break;}
}
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ComboBox4Exit(TObject *Sender)
{
if (NewAccount)
{
String cbs = UpperCase(ComboBox4->Text);
for (int i = 0;i<ComboBox4->Items->Count;i++)
{
String look = UpperCase(ComboBox4->Items->Strings[i]);
if (look.Pos(cbs)==1)
{ComboBox4->Text = ComboBox4->Items->Strings[i];break;}
}
cbs = ComboBox4->Text;
//char * cbss = ComboBox4->Text.c_str();
for (int i = mainform->TMData->Patients->Count-1;i>-1;i--)
{
TAccount* A = (TAccount*)mainform->TMData->Patients->Items[i];
char *looks = A->Field[tmd::ma];
if (!stricmp(cbs.c_str(),looks))
{
SetScaleBox(A->Field[tmd::sc][0]);
ComboBox6->ItemIndex = A->Field[tmd::usm][0]-'0';
if(ComboBox5->ItemIndex==2)
{
Edit6->Enabled=  true;
ddaysedit->Enabled = true;
Edit6->Text=LastDiscount();
}
UpdateMessage();
break;
}
}
}
}

//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::addr1Enter(TObject *Sender)
{
if (addr1->Text.IsEmpty())
{
addr1->Text = (ComboBox7->ItemIndex)?"Posbus ":"P O Box ";
if (mainform->TMData->Setup)
if (mainform->TMData->Setup->Field[tmd::pref][1]=='1')
addr1->Text = UpperCase(addr1->Text);
}
}
//---------------------------------------------------------------------------


void TPatientEditForm::SetScaleBox(char c)
{
Edit6->Enabled =  false;
ddaysedit->Enabled = false;

switch (c)
{
case 'G':ComboBox5->ItemIndex=0;break;
case 'C':ComboBox5->ItemIndex=1;break;
case 'D':ComboBox5->ItemIndex=2;
Edit6->Enabled=true;
ddaysedit->Enabled=true;break;
case 'E':ComboBox5->ItemIndex=3;break;
case 'W':ComboBox5->ItemIndex=4;break;
case 'M':ComboBox5->ItemIndex=5;break;
case 'P':ComboBox5->ItemIndex=6;break;
}
}
//---------------------------------------------------------------------------
void TPatientEditForm::InsertGridLine()
{
StringGrid1->EditorMode=false;
StringGrid1->RowCount++;           //add a row
//for (int i=StringGrid1->RowCount-2; i >= StringGrid1->Row;  i--)   //copy
//StringGrid1->Rows[i+1]=StringGrid1->Rows[i];
for (int i=StringGrid1->RowCount-1; i > StringGrid1->Row;  i--)   //copy
StringGrid1->Rows[i]=StringGrid1->Rows[i-1];


StringGrid1->Cells[1][StringGrid1->Row]="";
StringGrid1->Cells[2][StringGrid1->Row]="";
StringGrid1->Cells[3][StringGrid1->Row]="";
//StringGrid1->Rows[StringGrid1->Row]->SetText("");
Tag = 1;
}
//---------------------------------------------------------------------------

void TPatientEditForm::InsertFromLibrary()

//{
// if ((StringGrid1->Cells[1][StringGrid1->Row] == "")||
//    (StringGrid1->Cells[2][StringGrid1->Row] == ""))
{
if (StringGrid1->Row==StringGrid1->RowCount-1) InsertGridLine();

StringGrid1->Cells[1][StringGrid1->Row] = "";
StringGrid1->Cells[2][StringGrid1->Row] = "";
LibForm = new TLibForm(this);
LibForm->Libview->Checkboxes = true;
LibForm->LoadData();

LibForm->ShowModal();
int j=0;
for (int i=0;i<LibForm->Libview->Items->Count;i++)
{
TCSVRec *T = (TCSVRec*)LibForm->Libview->Items->Item[i]->Data;
if (T->Field[0][0]!='0')
    {
    if (j) {StringGrid1->Row++;InsertGridLine();}
    //StringGrid1->Cells[0][StringGrid1->Row]="";
    StringGrid1->Cells[1][StringGrid1->Row]=T->Field[tmd::lcode];
    StringGrid1->Cells[2][StringGrid1->Row]=T->Field[tmd::leng];
    j++;

 //   if (StringGrid1->Row < StringGrid1->RowCount)
  //  StringGrid1->Row = StringGrid1->Row+1;
    //InsertGridLine();


//    InsertGridLine();
//    StringGrid1->RowCount++;

 //   for (int i=StringGrid1->RowCount-2;i>StringGrid1->Row;i--)
 //          StringGrid1->Rows[i+1]=StringGrid1->Rows[i];
 //           StringGrid1->Rows[StringGrid1->Row]->SetText("");

    }

}
//if (StringGrid1->Row  < StringGrid1->RowCount)
//DeleteGridLine();

           //if (StringGrid1->Row >1) StringGrid1->Row--;

Tag=1;
delete LibForm;
LibForm=NULL;
//}
}
//---------------------------------------------------------------------------

void TPatientEditForm::DeleteGridLine()
{
StringGrid1->EditorMode=false;
if(StringGrid1->Row){
     for (int i=0;i<4;i++) StringGrid1->Cells[i][StringGrid1->Row] = "";

     for (int i=StringGrid1->Row;i<StringGrid1->RowCount-1;i++)
            StringGrid1->Rows[i]=StringGrid1->Rows[i+1];

            if (StringGrid1->RowCount>2)StringGrid1->RowCount--;
            if (StringGrid1->Row>StringGrid1->RowCount)StringGrid1->Row--;

Tag=1;
//change 20032003 Row-- added to allow deleting of first line
Payment_Balance();
}
}
//---------------------------------------------------------------------------
void __fastcall TPatientEditForm::InsertLine1Click(TObject *Sender)
{
InsertGridLine();
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::DeleteLine1Click(TObject *Sender)
{
DeleteGridLine();
Payment_Balance();        
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::CodeLookUp1Click(TObject *Sender)
{
InsertFromLibrary();
}
//---------------------------------------------------------------------------


void TPatientEditForm::DeleteGridField()
{
StringGrid1->Cells[StringGrid1->
Col][StringGrid1->Row]= "";
Tag=1;
}

void __fastcall TPatientEditForm::DeleteField1Click(TObject *Sender)
{
DeleteGridField();
        
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::DiagBoxClick(TObject *Sender)
{
if (!AllowTemplates)
{
bool b=true;
for (int i=1;i<StringGrid1->RowCount;i++)
if (!EmptyRow(i)) {b=false;break;}
AllowTemplates=b;
}

if (AllowTemplates)
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
TList *L = mainform->TMData->Patients;
TAccount *P;
SimilarTrans->Clear();
diagpos=0;
if (L->Count)
for (int i=L->Count-1;i>-1;i--)
{
P= (TAccount*)L->Items[i];
if (((String)P->Field[tmd::diag]==DiagBox->Text)&&
   (!DiagBox->Text.IsEmpty()))
   {SimilarTrans->Add(P);if (SimilarTrans->Count>20) break;}
}
ClearTrans();
if (SimilarTrans->Count)
{
LoadTrans((TAccount*)SimilarTrans->Items[0]);
for (int i=1;i<StringGrid1->RowCount;i++)
{StringGrid1->Cells[3][i]="";StringGrid1->Cells[0][i]="";}
}
Screen->Cursor = Save_Cursor;
}
}





void __fastcall TPatientEditForm::patnameExit(TObject *Sender)
{
if (NewAccount)
{
char *s = new char[patname->Text.Length()+1];
strcpy(s,patname->Text.c_str());

for (unsigned i = 0;i<strlen(s);i++)
if (s[i]==' '){s[i+1]='\0';break;}

for (int i = mainform->TMData->Patients->Count-1;i>-1;i--)
{
TAccount* T = (TAccount*)mainform->TMData->Patients->Items[i];

if ((!strnicmp(s,T->Field[tmd::pname],strlen(s)))||
    (!strnicmp(s,T->Field[tmd::pname],strlen(T->Field[tmd::pname]))))
{
ComboBox1->ItemIndex = (T->Field[tmd::psex][0]=='M')? 0:1;break;
}
}
delete[]s;
}
}
//---------------------------------------------------------------------------



void __fastcall TPatientEditForm::addr2Enter(TObject *Sender)
{
NewAccount = ((TEdit*)Sender)->Text.IsEmpty();        
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ComboBox2Enter(TObject *Sender)
{
NewAccount = ((TComboBox*)Sender)->Text.IsEmpty();        
}
//---------------------------------------------------------------------------


bool TPatientEditForm::TimeCode(char* s)
{
return  ((strcmp(s,"0025")==0)||
         (strcmp(s,"0023")==0)||
         (strcmp(s,"0011")==0)||
         (strcmp(s,"0036")==0)||
         (strcmp(s,"0039")==0)||
         (strcmp(s,"0012")==0)||
         (strcmp(s,"1211")==0))
          ? true:false;
}

//---------------------------------------------------------------------------
int TPatientEditForm::AddressType(char*s)
{
if (!strnicmp(s,"P O Box",7)) return 1;
if (!strnicmp(s,"PO Box",6)) return 1;
if (!strnicmp(s,"Posbus",6)) return 1;
if (!strnicmp(s,"Pvt",3)) return 2;
if (!strnicmp(s,"Priv",4)) return 2;
return 0;
}

//---------------------------------------------------------------------------
bool TPatientEditForm::SameName(char*a, char*b)
{
if ((a==NULL)||(b==NULL)) return false;
if ((strlen(a)<2)||(strlen(b)<2)) return false;
return stricmp(a,b)==0;

}
//---------------------------------------------------------------------------

bool TPatientEditForm::EDIValid()
{
return ComboBox4->Text.Pos("*")!=0;
}
//---------------------------------------------------------------------------


char* TPatientEditForm::LastDiscount(void)
{
char* str = "20";
for (int i = mainform->TMData->Patients->Count-1;
        i>-1;i--)
        {
        TAccount* A = (TAccount*)mainform->TMData->Patients->Items[i];
        if  (A->Field[tmd::sc][0]=='D')
        {
        str = A->Field[tmd::dsc];
        break;
        }
        }
return str;

}

void __fastcall TPatientEditForm::TabSheet2Exit(TObject *Sender)
{
UpdateMessage();
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::TabSheet2Enter(TObject *Sender)
{
if (ComboBox5->ItemIndex==2)
{
ddaysedit->Visible=true;
StaticText1->Visible=true;
Label23->Visible=true;
Edit6->Visible=true;
Edit2->Visible=true;
}
else
{
ddaysedit->Visible=false;
StaticText1->Visible=false;
Label23->Visible=false;
Edit6->Visible=false;
Edit2->Visible=false;
}

}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if (Key==VK_DELETE) Tag = 1;
//if (Key==VK_RBUTTON) Tag = 1;
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::FileNumChange(TObject *Sender)
{
//PatientEditForm->Tag=1;
Tag=1;
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ComboBox4Change(TObject *Sender)
{
Tag=1;
UpdateMessage();
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::Interest1Click(TObject *Sender)
{
Tag=1;
Payment_Balance();
}
//---------------------------------------------------------------------------


void TPatientEditForm::EditLibraryItem(int libitem, int row)
{
TCSVRec* S=mainform->TMData->Setup;
double amt;
int value =0;
try{amt=StrToFloat(StringGrid1->Cells[3][row]);}
catch(EConvertError &e){amt=0;}
switch (ComboBox5->Items->Strings[ComboBox5->ItemIndex].c_str()[0])
{
case 'C':
case 'E':
case 'M': value=atoi(S->Field[tmd::gpu]); break;
case 'W': value = atoi(S->Field[tmd::wpu]);break;
case 'S':
case 'D': value = atoi(S->Field[tmd::mpu]);break;
case 'P': value = 1.2 * atoi(S->Field[tmd::mpu]);break;
}
amt = (value>0) ? amt*100 / value   : 0;

TCSVRec* LibItem = (TCSVRec*)libitem ;
char str[10];
sprintf(str,"%4.2f",amt);
LibItem->Field[tmd::un]=str;
//IntToStr((int)amt).c_str();
LastLibraryItem=0;
}


void __fastcall TPatientEditForm::BirthdateExit(TObject *Sender)
{
//unsigned short y,m,d;
//String str,ys,ms,ds;
//str = ((TMaskEdit*)Sender)->EditText;
//ys = str.SubString(7,4);
//ms = str.SubString(4,2);
//ds = str.SubString(1,2);
//y = (unsigned short) StrToInt(ys);
//m = (unsigned short)StrToInt(ms);
//d = (unsigned short)StrToInt(ds);
if (!((TMaskEdit*)Sender)->Text.IsEmpty())
{
TStrDate* sd = new TStrDate(((TMaskEdit*)Sender)->Text.c_str());
try{TDateTime(sd->Year(),sd->Month(),sd->Day());}
catch (EConvertError& ){FocusControl((TMaskEdit*)Sender);}
delete sd;
}
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::StringGrid1Click(TObject *Sender)
{
Tag=1;
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::PopupMenu1Popup(TObject *Sender)
{
ProcRecalc1->Enabled=false;
ProcRecalc2->Enabled=false;
ProcRecalc3->Enabled=false;
ProcRecalc4->Enabled=false;
AssistantFee1->Enabled=false;
CodeLookUp1->Enabled=false;
if ((StringGrid1->Cells[1][StringGrid1->Row].IsEmpty())||
    (StringGrid1->Cells[2][StringGrid1->Row].IsEmpty())) CodeLookUp1->Enabled=true;
if (StringGrid1->Col==3)
{
int cd = atoi(StringGrid1->Cells[1][StringGrid1->Row].c_str());
if ((cd==8)||(cd==9)||(cd==29))AssistantFee1->Enabled=true;

ProcRecalc1->Enabled=true;
ProcRecalc2->Enabled=true;
ProcRecalc3->Enabled=true;
ProcRecalc4->Enabled=true;

}
}
//---------------------------------------------------------------------------

//void __fastcall TPatientEditForm::AssistantFee1Click(TObject *Sender)
//{
//TAssistFee * Assist = new TAssistFee(this);
//for (int i=1;i<StringGrid1->RowCount ;i++)
//{
//Assist->Amounts->Items->Add(StringGrid1->Cells[3][i]);
//Assist->CheckListBox1->Items->Add(StringGrid1->Cells[1][i]+"  "+StringGrid1->Cells[3][i])
//ssist->CheckListBox1->Checked[i=true];
//}
//Assist->ShowModal();
//delete Assist;
//}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::AssistantFee1Click(TObject *Sender)
{
TAssistFee * Assist = new TAssistFee(this);
Assist->CheckListBox1->Clear();
Assist->Amounts->Clear();
int cd = atoi(StringGrid1->Cells[1][StringGrid1->Row].c_str());
/* TODO : need to put in the correct percentages */
if (cd == 8) Assist->UpdateRate(33);
    else if (cd==9) Assist->UpdateRate(20);
       else Assist->UpdateRate(66);

for (int i=1;i<StringGrid1->RowCount ;i++)
if ((!StringGrid1->Cells[3][i].IsEmpty())&&(i!=StringGrid1->Row))
{
Assist->Amounts->Add(StringGrid1->Cells[3][i]);
Assist->CheckListBox1->Items->Add(StringGrid1->Cells[0][i]+"  "+
                                  StringGrid1->Cells[1][i]+"  "+
                                  StringGrid1->Cells[2][i]+"   "+
                                  StringGrid1->Cells[3][i]);
}
for (int i=0;i<Assist->CheckListBox1->Items->Count;i++) Assist->CheckListBox1->Checked[i]=true;
Assist->CheckListBox1ClickCheck(this);
Assist->ShowModal();
StringGrid1->Cells[3][StringGrid1->Row]=Assist->LabeledEdit1->Text;
delete Assist;

}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ProcRecalc1Click(TObject *Sender)
{
AddFeeToGrid(StringGrid1->Row,100); //100% Fee calc
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ProcRecalc2Click(TObject *Sender)
{
AddFeeToGrid(StringGrid1->Row,75);  //75% Fee calc
}
//---------------------------------------------------------------------------

void __fastcall TPatientEditForm::ProcRecalc3Click(TObject *Sender)
{
AddFeeToGrid(StringGrid1->Row,50);  //50% Fee calc
}
//---------------------------------------------------------------------------


void TPatientEditForm::AddFeeToGrid(int row, int percent)
{
TCSVRec * T = mainform->TMData->LibLookup(StringGrid1->Cells[1][row].c_str());
int atime=0;
if (TimeCode(StringGrid1->Cells[1][row].c_str()))//(atime)
         {
         atime = TimeValue(StringGrid1->Cells[2][row].c_str());
         if ((!atime)&&(row)) atime = TimeValue(StringGrid1->Cells[2][row-1].c_str());
//         T = mainform->TMData->LibLookup(StringGrid1->Cells[1][Row].c_str());


          char* e = (T) ?T->Field[tmd::leng]:"";
          char* a = (T)?T->Field[tmd::lafr]:"";
          if (!strlen(a)) a=e;
          if (!strlen(e)) e=a;
          String prom = (ComboBox7->ItemIndex)? a:e;
          StringGrid1->Cells[2][row] = prom+" "+IntToStr(atime)+" min";
         }
double fee;
try
{fee = String(Fee(StringGrid1->Cells[1][row].c_str(),atime)).ToDouble();}
catch(EConvertError &){fee=0;}
StringGrid1->Cells[3][row] = FormatFloat("0.00",fee * percent/100);
Payment_Balance();
         //}
         //Payment_Balance();


}
void __fastcall TPatientEditForm::ProcRecalc4Click(TObject *Sender)
{
AddFeeToGrid(StringGrid1->Row,25);
}
//---------------------------------------------------------------------------


void __fastcall TPatientEditForm::DiagBoxKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
//bool UnEdited= true;
//for (int i=1;i<StringGrid1->RowCount;i++)
//if (!StringGrid1->Cells[3][i].IsEmpty()){UnEdited=false;break;}

if (DiagBox->Text.IsEmpty()&&
    AllowTemplates)// && UnEdited)
    {SimilarTrans->Clear();ClearTrans();};
}
//---------------------------------------------------------------------------




