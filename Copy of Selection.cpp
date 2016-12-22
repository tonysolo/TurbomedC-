//---------------------------------------------------------------------------
#pragma hdrstop
#include <vcl.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "CSVRec.h"
#include "Selection.h"
#include "main.h"
#include "A5Pg.h"
#include "Globals.h"
#include "strdate.h"
#include <fstream.h>
#include <Filectrl.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

TSelection::TSelection()
{
TListItems* L = mainform->ListView1->Items;
//Tmainform * m = (Tmainform*)L->Owner->Owner;
msetup = mainform->TMData->Setup;
mselection = new TList();
for (int i=0;i<L->Count ;i++)
    if (L->Item[i]->Selected) mselection->Add(L->Item[i]->Data);
}
//---------------------------------------------------------------------------
TSelection::~TSelection()
{
delete mselection;
}
//---------------------------------------------------------------------------
//TAccount* TSelection::fgetitem(int i)
//{
//return (TAccount*) mselection->Items[i];
//}
//---------------------------------------------------------------------------
int TSelection::fgetcount()
{
return mselection->Count;
}
//---------------------------------------------------------------------------
double TSelection::fgetpayment()
{
double x=0;
for (int i=0;i<mselection->Count ;i++)
x += atof(((TAccount*) mselection->Items[i])->Payments);
return x;
}
//---------------------------------------------------------------------------
int TSelection::ReceiptCount()
{
int x=0;
for (int i=0;i<mselection->Count ;i++)
if (atof(((TAccount*) mselection->Items[i])->Payments)>0)
x++;
return x;
}


//---------------------------------------------------------------------------
double TSelection::fgetbalance()
{
double x=0;
for (int i=0;i<mselection->Count ;i++)
x += atof(((TAccount*) mselection->Items[i])->Balance);
return x;
}
//---------------------------------------------------------------------------
double TSelection::fgetbilled()
{
double x=0;
for (int i=0;i<mselection->Count ;i++)
x += atof(((TAccount*) mselection->Items[i])->Billed);
return x;

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TSelection::PrintInvoices(int xedi)
{
//if xedi is positive then exclude edi
//xedi is the number if edi accounts
//acc = (TAccount*)mselection->Items[i];
//if (acc->Field[tmd::sc][0]=='E')
if  ((mselection->Count - xedi) > 0)
{
char *str = new char[100];
TPrinter * prin = Printer();

sprintf(str,"\
Printer: %s\n\
%d Invoice(s) Selected\n\
Continue Printing ?",
prin->Printers->Strings[prin->PrinterIndex],
mselection->Count - xedi);

if (Application->MessageBox(str,"Print Invoices",
MB_OKCANCEL)==IDOK)
{
TA5Page *pg = new TA5Page();
TAccount*acc;
pg->FooterHeight=3;
pg->printer->Canvas->Pen->Width=5;
//pg->Heading = "Cont...";
for (int i=0;i<mselection->Count;i++)
{
bool skip = false;
acc = (TAccount*)mselection->Items[i];
if (xedi) skip = (acc->Field[tmd::sc][0]=='E');

if (!skip)
{
if ((!acc->NilBalance)||(mselection->Count==1))
{
pg->NewPage();
sprintf(str,"%s %s",acc->Field[tmd::nc],acc->Field[tmd::asname]);
pg->Heading = str;
PrintHeading(pg);
PrintInvoiceType(pg,acc);
PrintDetails(pg,acc);
PrintTransactions(pg,acc);
PrintInvoiceMessage(pg,acc);
acc->datestamp("INVOICE");
mainform->TMData->isDirty =true;
}
}
}
pg->Finish();
delete pg;
}
delete[] str;
}
}

//---------------------------------------------------------------------------
void TSelection::PrintReceipts()
{
char *str = new char[100];
TPrinter * prin= Printer();
sprintf(str,"\
Printer: %s\n\
%d Receipts(s) Selected\n\
Continue Printing ?",prin->Printers->Strings[prin->PrinterIndex],ReceiptCount());



if (Application->MessageBox(str,"Print Receipts",
MB_OKCANCEL)==IDOK)
{
TA5Page *pg = new TA5Page();
TAccount*acc;
pg->printer->Canvas->Pen->Width=5;
pg->FooterHeight=2;
for (int i=0;i<mselection->Count;i++)
{
acc = (TAccount*)mselection->Items[i];
if (acc->Paid)
{
sprintf(str,"%s %s",acc->Field[tmd::nc],acc->Field[tmd::asname]);
pg->Heading = str;
pg->NewPage();
PrintHeading(pg);
PrintReceiptType(pg,acc);
PrintDetails(pg,acc);
PrintTransactions(pg,acc);
PrintReceiptMessage(pg,acc);
acc->datestamp("RECEIPT");
mainform->TMData->isDirty=true;
}
}
pg->Finish();
delete pg;
}
delete[] str;
}

//---------------------------------------------------------------------------
void TSelection::DateStamp(char *s)
{
for (int i=0;i<mselection->Count ;i++)
((TAccount*) mselection->Items[i])->datestamp(s);
}

//---------------------------------------------------------------------------
bool TSelection::VatStamped()
{
bool b=false;
for (int i=0;i<mselection->Count ;i++)
if (((TAccount*) mselection->Items[i])->
            vatstamped()== true)
{b=true;break;}
return b;
}
//---------------------------------------------------------------------------
void TSelection::ToArchive()
{
for (int i=0;i<mselection->Count ;i++)
((TAccount*) mselection->Items[i])->ToArchive();
}
//---------------------------------------------------------------------------
void TSelection::ToVat()
{
for (int i=0;i<mselection->Count ;i++)
((TAccount*) mselection->Items[i])->ToVat();
}
//---------------------------------------------------------------------------
void TSelection::ToActive()
{
for (int i=0;i<mselection->Count ;i++)
((TAccount*) mselection->Items[i])->ToActive();
}
//---------------------------------------------------------------------------
void TSelection::Renumber()
{
for (int i=0;i<mselection->Count ;i++)
{
TAccount* T = (TAccount*) mselection->Items[i];
String s = TAccount::ndc+1;
T->Field[tmd::nc] = s.c_str();
T->Check_ndc();
}
}

//---------------------------------------------------------------------------

void TSelection::PrintHeading(TA5Page *p)
{
p->TextStyle=TA5Page::a5Bold;
p->Write (5,1,msetup->Field[tmd::h1],TA5Page::LJ);
p->Write (5,2,msetup->Field[tmd::h2],TA5Page::LJ);
p->Write (5,3,msetup->Field[tmd::h3],TA5Page::LJ);
p->Write (5,4,msetup->Field[tmd::h4],TA5Page::LJ);
p->Write (95,1,msetup->Field[tmd::h5],TA5Page::RJ);
p->Write (95,2,msetup->Field[tmd::h6],TA5Page::RJ);
p->Write (95,3,msetup->Field[tmd::h7],TA5Page::RJ);
p->Write (95,4,msetup->Field[tmd::h8],TA5Page::RJ);


p->TextStyle=TA5Page::a5Normal;
p->Write (5,6,"Practice Number :",TA5Page::LJ);
p->Write (27,6,msetup->Field[tmd::prnum],TA5Page::LJ);

if (strlen(msetup->Field[tmd::vatnum]))
{
char str[40];
sprintf(str,"Vat Reg Number : %s",msetup->Field[tmd::vatnum]);
//p->Write (50,6,"VAT Reg Number :",TA5Page::LJ);
//p->Write (73,6,msetup->Field[vatnum],TA5Page::LJ);
p->Write (95,6,str,TA5Page::RJ);
}
}
//---------------------------------------------------------------------------
void TSelection::PrintInvoiceType(TA5Page *p, TAccount *a)
{
bool vatf = ((strlen(msetup->Field[tmd::vatnum])>0)&&
             (a->Field[tmd::usm][0]!= '3'));
char *s;
p->TextStyle=TA5Page::a5BoldItalic;
p->Separator(7);
if (vatf)
s = (a->Field[tmd::lang][0]=='E')?"TAX INVOICE" : "BELASTING FAKTUUR";
else
s = (a->Field[tmd::lang][0]=='E')?"INVOICE" : "FAKTUUR";
p->Write (5,8,s,TA5Page::LJ);
p->Separator(9);
}
//---------------------------------------------------------------------------

 void TSelection::PrintReceiptType(TA5Page *p, TAccount *a)
{
p->TextStyle=TA5Page::a5BoldItalic;
p->Separator(7);
char *s = (a->Field[tmd::lang][0]=='E')?"RECEIPT" : "KWITANSIE";
p->Write (5,8,s,TA5Page::LJ);
p->Separator(9);
}
//---------------------------------------------------------------------------

void TSelection::PrintDetails(TA5Page *p, TAccount *a)
{
p->TextStyle=TA5Page::a5Normal;
bool e = (a->Field[tmd::lang][0]=='E');
char s[60];
char *s1;
sprintf(s,"%s %s %s",a->Field[tmd::atitle],
                    a->Field[tmd::ainit],
                    a->Field[tmd::asname]);
s1 = (e)?"To :":"Aan :";

//p->printer->Canvas->Font->Style = TFontStyles << fsBold;
p->Write (15,10,s1,TA5Page::LJ);
p->Write (15,11,s,TA5Page::LJ);
p->Write (15,12,a->Field[tmd::ad1],TA5Page::LJ);
p->Write (15,13,a->Field[tmd::ad2],TA5Page::LJ);
p->Write (15,14,a->Field[tmd::ad3],TA5Page::LJ);
p->Write (15,15,a->Field[tmd::ad4],TA5Page::LJ);
//p->printer->Canvas->Font->Style.Clear();
if (e)
{
p->Write (70,10,    "Date :",TA5Page::RJ);
p->Write (70,11,"Our Ref :",TA5Page::RJ);
{
if (msetup->Field[tmd::pref][3]=='1')
p->Write (70,12, "Diagnosis :",TA5Page::RJ);
else
p->Write (70,12, "Clinic :",TA5Page::RJ);
}
p->Write (70,13, "Ref By :",TA5Page::RJ);
p->Write (70,14,"Med Aid :",TA5Page::RJ);
p->Write (70,15,"M A Num :",TA5Page::RJ);
}
else
{
p->Write (70,10,   "Datum :",TA5Page::RJ);
p->Write (70,11, "Ons Nr :",TA5Page::RJ);
{
if (msetup->Field[tmd::pref][3]=='1')
p->Write (70,12,"Diaknose :",TA5Page::RJ);
else
p->Write (70,12,   "Hosp :",TA5Page::RJ);
}
p->Write (70,13, "Verwys :",TA5Page::RJ);
p->Write (70,14,"M Fonds :",TA5Page::RJ);
p->Write (70,15, "M F Nr :",TA5Page::RJ);
}


TStrDate ssd(a->Field[tmd::dt]);

//TDateTime sdt(ssd.Year(),ssd.Month(),ssd.Day());
//if (sdt > TDateTime::CurrentDate())
if (ssd.Age()< 0)
p->Write (72,10,ddmmmyyyy((a->Field[tmd::dt])),TA5Page::LJ);
else
p->Write (72,10,TDateTime::CurrentDate().
FormatString("dd mmm yyyy").c_str(),TA5Page::LJ);
p->Write (72,11,a->Field[tmd::nc],TA5Page::LJ);
{
if (msetup->Field[tmd::pref][3]=='1')
p->Write (72,12,a->Field[tmd::diag],TA5Page::LJ);
else
p->Write (72,12,a->Field[tmd::clin],TA5Page::LJ);
}
p->Write (72,13,a->Field[tmd::ref],TA5Page::LJ);
p->Write (72,14,a->Field[tmd::ma],TA5Page::LJ);
p->Write (72,15,a->Field[tmd::man],TA5Page::LJ);
p->Separator(17);                                    
s1 = (e)? "Patient :":"Pasient :";

p->Write (15,18,s1,TA5Page::RJ);
//sprintf(s,"%s : %s",s1,a->Field[pname]);
p->Write (17,18,a->Field[tmd::pname],TA5Page::LJ);

if ((a->Field[tmd::auth])&&(strlen(a->Field[tmd::auth])))
{
p->Write (70,18, "Auth No. :",TA5Page::RJ);
p->Write (72,18,a->Field[tmd::auth],TA5Page::LJ);
}
if ((a->Field[tmd::patid])&&(strlen(a->Field[tmd::patid])))
{
p->Write (15,19,"ID :",TA5Page::RJ);
p->Write (17,19,a->Field[tmd::patid],TA5Page::LJ);
}
if ((a->Field[tmd::ioddt])&&(strlen(a->Field[tmd::ioddt])))
{
p->Write (70,19, "IOD :",TA5Page::RJ);
p->Write (72,19,ddmmmyyyy(a->Field[tmd::ioddt]),TA5Page::LJ);
}
p->Separator(20);
}
//---------------------------------------------------------------------------
void TSelection::PrintTransactions(TA5Page *p, TAccount *a)
{
p->TextStyle=TA5Page::a5Normal;
//if english
bool e = (a->Field[tmd::lang][0]=='E');
if (e)
{
 p->Write (5,21,"DATE",TA5Page::LJ);
 p->Write (25,21,"CODE",TA5Page::LJ);
 p->Write (35,21,"SERVICE",TA5Page::LJ);
 p->Write (90,21,"DEBIT",TA5Page::RJ);
 }
else
{
p->Write (5,21,"DATUM",TA5Page::LJ);
p->Write (25,21,"KODE",TA5Page::LJ);
p->Write (35,21,"DIENS",TA5Page::LJ);
p->Write (90,21,"DEBIET",TA5Page::RJ);
}


 p->NextLine(22);
 int j;
 char *s;
 for (int i=0;i<a->Trans->Count;i++)
 {
 j = p->NextLine(0);
 s = ddmmmyyyy(((TCSVRec*)a->Trans->Items[i])->Field[tmd::tdate]);
 if (i>0) {if  (!strcmp(((TCSVRec*)a->Trans->Items[i])->Field[tmd::tdate],
              ((TCSVRec*)a->Trans->Items[i-1])->Field[tmd::tdate]))
               s="    ''";}
 p->Write(5,j,s,TA5Page::LJ);
 p->Write(25,j,((TCSVRec*)a->Trans->Items[i])->Field[tmd::code],TA5Page::LJ);
 p->Write(35,j,((TCSVRec*)a->Trans->Items[i])->Field[tmd::proc],TA5Page::LJ);
 p->Write(90,j,((TCSVRec*)a->Trans->Items[i])->Field[tmd::amt],TA5Page::RJ);

 }
 bool intflag = (strlen(a->Field[tmd::sc])>1);
 if (intflag)
 {
 j=p->NextLine(0);
 p->Write(25,j,"INT",TA5Page::LJ);
 p->Write(35,j,"Interest Charged",TA5Page::LJ);
 p->Write(90,j,a->IntCalc(msetup->Field[tmd::interest]),TA5Page::RJ);
 }


 p->NextLine(0);
  j = p->NextLine(0);

 bool vatf = ((strlen(msetup->Field[tmd::vatnum])>0)&&
             (a->Field[tmd::usm][0]!= '3'));
 if (vatf)
 p->Write(5,j,msetup->Field[tmd::vat],TA5Page::LJ);
 if (e)
 {
 if (vatf)
 p-> Write(10,j,"% VAT INCLUDED",TA5Page::LJ);
 p-> Write(60,j,"TOTAL  R",TA5Page::LJ);
 }
 else
 {
 if (vatf)
 p-> Write(10,j,"% BTW INGESLUIT",TA5Page::LJ);
 p-> Write(60,j,"TOTAAL  R",TA5Page::LJ);
 }
 //change 20032003 position of total changes from 65 to 60
 if (intflag)
  p-> Write(90,j,a->Int_Balance(msetup->Field[tmd::interest]),TA5Page::RJ);
  else p-> Write(90,j,a->Balance,TA5Page::RJ);
 //delete[] s;
 }
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TSelection::PrintInvoiceMessage(TA5Page *p, TAccount *a)
{
if (atoi(a->Balance)>0)
{
int mestype;
p->TextStyle=TA5Page::a5BoldItalic;
int l = (a->Field[tmd::lang][0]=='E') ? 0:1;
switch (a->Field[tmd::sc][0])
{
case 'G':
case 'C':mestype=TData::m_gaz;break;
case 'E':mestype=TData::m_edi;break;
case 'D':mestype=TData::m_dis;break;
case 'W':
case 'M':
case 'P':mestype=TData::m_mas;break;
}
if  (mestype==TData::m_dis)
{
TStrDate sd(a->Field[tmd::dt]);
if (!atoi(a->Field[tmd::ddys])) a->Field[tmd::ddys]=="30";
if (sd.Age() > atoi(a->Field[tmd::ddys]))
mestype=TData::m_mas;
}
//TDateTime d;
//try {
// d = TDateTime(sd.Year(),sd.Month(),sd.Day());
//    }
// catch (EConvertError &)
// {d= TDateTime::CurrentDate();}

//if (!mainform->TMData->Setup) d+=30;
//else
//{
//if (!atoi(mainform->TMData->Setup->Field[ddays]))
//        mainform->TMData->Setup->Field[ddays]="30";
//d += atoi(mainform->TMData->Setup->Field[ddays]);
//}
//if (TDateTime::CurrentDate() > d)

switch (a->Field[tmd::usm][0])
{
// '0'==std message
case '1':mestype=TData::m_us1;break;
case '2':mestype=TData::m_us2;break;
case '3':mestype=TData::m_nil;break;
}
 p->Separator(p->LineCount-4);
 
 p->Write(5,p->LineCount-3,
 mainform->TMData->
 MessageLine(mestype,
            l,
            0,
            a->Field[tmd::dt],
            a->Field[tmd::dsc],
            atoi(a->Field[tmd::ddys]),
            a->Billed),
 TA5Page::LJ);

 p->Write(5,p->LineCount-2,
 mainform->TMData->
 MessageLine(mestype,
            l,
            1,
            a->Field[tmd::dt],
            a->Field[tmd::dsc],
            atoi(a->Field[tmd::ddys]),
            a->Billed),
 TA5Page::LJ);
 p->Write(5,p->LineCount-1,
 mainform->TMData->
 MessageLine(mestype,
            l,
            2,
            a->Field[tmd::dt],
            a->Field[tmd::dsc],
            atoi(a->Field[tmd::ddys]),
            a->Billed),
 TA5Page::LJ);
}}

//---------------------------------------------------------------------------

void TSelection::PrintReceiptMessage(TA5Page *p, TAccount *a)
{
p->TextStyle=TA5Page::a5BoldItalic;
int l = (a->Field[tmd::lang][0]=='E') ? 0:1;
char *R1 = (!l) ?
"RECEIPT OF PAYMENT ACKNOWLEDGED WITH THANKS" :
"BETALING MET DANK ERKEN";

 p->Separator(p->LineCount-3);
 p->Write(5,p->LineCount-2,R1,TA5Page::LJ);

}
//---------------------------------------------------------------------------

//void TSelection::PrintVatLine(TA5Page *p,TAccount *a)
//{
//}
//---------------------------------------------------------------------------

void TSelection::PrintVatReport()
{
char *str = new char[100];
sprintf(str,"\
Folder:\t%s\n\
     #:\t%d\n\
Select:\t%d\n\n\
Continue Printing ?",
mainform->FolderName(),
mainform->ListCount,
mselection->Count);

if (Application->MessageBox(str,"Print VAT Report",
MB_OKCANCEL)==IDOK)
{
TA5Page *pg = new TA5Page();
TAccount*acc;
pg->Heading = mainform->FolderName();
pg->NewPage();
sprintf(str,"VAT REPORT    File = %s",mainform->FolderName());
pg->Write(2,1,str,TA5Page::LJ);
pg->Separator(2);
pg->Write(2,3,"REF",TA5Page::LJ);
pg->Write(12,3,"NAME",TA5Page::LJ);
pg->Write(38,3,"BILLED",TA5Page::RJ);
pg->Write(48,3,"ASST",TA5Page::RJ);//53
pg->Write(62,3,"PAYM",TA5Page::RJ);//68
pg->Write(74,3,"ADJ",TA5Page::RJ);
pg->Write(86,3,"VAT",TA5Page::RJ);//83
pg->Write(98,3,"BAL",TA5Page::RJ);
pg->NextLine(4);
int n;
for (int i=0;i<mselection->Count;i++)
{
n = pg->NextLine(0);
acc = (TAccount*)mselection->Items[i];
pg->Write(2,n,acc->Field[tmd::nc],TA5Page::LJ);
pg->Write(12,n,acc->Field[tmd::asname],TA5Page::LJ);
pg->Write(38,n,acc->Billed,TA5Page::RJ);
pg->Write(48,n,acc->Assistants,TA5Page::RJ); //assist
pg->Write(62,n,acc->Payments,TA5Page::RJ);
double adjst= atof(acc->Billed) -
               (atof(acc->Payments)  +
                 atof(acc->Balance));
sprintf(str,"%10.2f",adjst);
pg->Write(74,n,str,TA5Page::RJ); //adjust
pg->Write(86,n,acc->GetVatDue(msetup->Field[tmd::vat]),TA5Page::RJ);
pg->Write(98,n,acc->Balance,TA5Page::RJ);
//PrintVatLine(pg,acc);
//sprintf(str,"%s %s",acc->Field[nc],acc->Field[asname]);
}
double asst  = Assistants;
double blld  = Billed;
double paym  = Payment;
double bala  = Balance;
double adjst = blld - (bala+paym);
double gros  = paym - asst;
//double vata   =  atof(msetup->Field[vat]);
double vatfinal = Vat;//gros * vata / (100 + vata);
double neta  = gros - vatfinal;

pg->Separator(pg->NextLine(0));
n= pg->NextLine(0);
pg->Write(2,n,"TOTALS",TA5Page::LJ);
sprintf(str,"%10.2f",blld);
pg->Write(38,n,str,TA5Page::RJ); //total billed
sprintf(str,"%10.2f",asst);
pg->Write(48,n,str,TA5Page::RJ); //total assist
sprintf(str,"%10.2f",paym);
pg->Write(62,n,str,TA5Page::RJ); //total pay
sprintf(str,"%10.2f",adjst);
pg->Write(74,n,str,TA5Page::RJ); //total adjst
sprintf(str,"%10.2f",vatfinal);
pg->Write(86,n,str,TA5Page::RJ); //total vat
sprintf(str,"%10.2f",bala);
pg->Write(98,n,str,TA5Page::RJ); //total balance

pg->Separator(pg->NextLine(0));
n= pg->NextLine(0);
pg->Write(60,n,"GROSS INCOME (Payments -Assist Fees)",TA5Page::RJ);

sprintf(str,"%10.2f",gros);
pg->Write(86,n,str,TA5Page::RJ);//gross
n= pg->NextLine(0);
sprintf(str,"VAT @ %s\%",msetup->Field[tmd::vat]);
pg->Write(60,n,str,TA5Page::RJ);

sprintf(str,"%10.2f",vatfinal);
pg->Write(86,n,str,TA5Page::RJ);//vat
//pg->Separator(pg->NextLine(0));
n= pg->NextLine(0);
pg->Write(60,n,"NET INCOME",TA5Page::RJ);

sprintf(str,"%10.2f",neta);
pg->Write(86,n,str,TA5Page::RJ);//net
pg->Separator(pg->NextLine(0));
pg->Finish();
delete pg;
}
delete[] str;
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//Helper routine Called by PrintAgeReport

void TSelection::PrintAgeGroup(TList *l, TA5Page* p)
{
int n;
TAccount* a;
for (int i=0;i<l->Count ;i++)
{
n = p->NextLine(0);
a = (TAccount*)l->Items[i];
p->Write (5,n,a->Field[tmd::nc],TA5Page::LJ);
p->Write (20,n,a->Field[tmd::asname],TA5Page::LJ);
p->Write (75,n,a->Balance,TA5Page::RJ);
switch (a->Field[tmd::sc][0])
{ case 'D':;
  case 'M':;
  case 'P':;
  case 'W':p->Write (90,n,"**",TA5Page::RJ);break;
}
}
n= p->NextLine(0);

double x=0;
char s[10];
for (int i=0;i<l->Count ;i++)
x += atof(((TAccount*) l->Items[i])->Balance);

sprintf(s,"%9.2f",x);
p->Write (50,n,"SUB TOTAL",TA5Page::RJ);
p->Write (75,n,s,TA5Page::RJ);

p->Separator(p->NextLine(0));
}
//---------------------------------------------------------------------------
void TSelection::PrintAgeReport()
{
char *str = new char[100];
TA5Page *pg = new TA5Page();
TAccount*acc;
//unsigned short y,m,d;
TList *L0,*L3,*L6,*L9,*L12;
sprintf(str,"\
Folder:\t%s\n\
     #:\t%d\n\
Select:\t%d\n\n\
Continue Printing ?",
mainform->FolderName(),
mainform->ListCount,
mselection->Count);
if (Application->MessageBox(str,"Print Account List",
MB_OKCANCEL)==IDOK)
{
L0 = new TList();
L3 = new TList();
L6 = new TList();
L9 = new TList();
L12 = new TList();
pg->NewPage();
pg->Heading="List";
sprintf(str,"ACCOUNTS LIST    Folder: %s        Date: %s",
            mainform->FolderName(),
           TDateTime::CurrentDate().FormatString("dd mmm yyyy ").c_str());

pg->Write (5,1,str,TA5Page::LJ);

sprintf(str,"Selected Accounts: %d    Total in Folder: %d\
    ** = fees not Scale of Benefits",
           mselection->Count,
            mainform->ListCount);
pg->Write (5,2,str,TA5Page::LJ);

pg->Separator(3);
pg->NextLine(4);
//check current
 for (int i = 0;i< mselection->Count ;i++)
  {
  acc = (TAccount*)mselection->Items[i];
//      y = (unsigned short)atoi(Year(acc->Field[dt]));
//      m = (unsigned short)atoi(Month(acc->Field[dt]));
//      d = (unsigned short)atoi(Day(acc->Field[dt]));
//     int days = TDateTime::CurrentDate()- TDateTime(y,m,d);
int days = TStrDate(acc->Field[tmd::dt]).Age();
         if (days<31)L0->Add(acc);
         else if ((days>30)&&(days<61))L3->Add(acc);
         else if ((days>60)&&(days<91))L6->Add(acc);
         else if ((days>90)&&(days<121))L9->Add(acc);
         else L12->Add(acc);
   }

if (L0->Count){
pg->Write (5,pg->NextLine(0),"CURRENT",TA5Page::LJ);
PrintAgeGroup(L0,pg);
}
if (L3->Count){
pg->Write (5,pg->NextLine(0),"30 DAYS",TA5Page::LJ);
PrintAgeGroup(L3,pg);
}
if (L6->Count){
pg->Write (5,pg->NextLine(0),"60 DAYS",TA5Page::LJ);
PrintAgeGroup(L6,pg);
}
if (L9->Count){
pg->Write (5,pg->NextLine(0),"90 DAYS",TA5Page::LJ);
PrintAgeGroup(L9,pg);
}
if (L12->Count){
pg->Write (5,pg->NextLine(0),"120 DAYS",TA5Page::LJ);
PrintAgeGroup(L12,pg);
}
sprintf(str,"%9.2f",fgetbalance());

int n = pg->NextLine(0);
pg->Write (50,n,"OVERALL TOTAL SELECTED",TA5Page::RJ);
pg->Write (75,n,str,TA5Page::RJ);
pg->Separator(pg->NextLine(0));
pg->Finish();

delete pg;
delete[] str;
delete L0;
delete L3;
delete L6;
delete L9;
delete L12;
}
}
//---------------------------------------------------------------------------

double __fastcall TSelection::GetAssistants()
{
double x=0;
for (int i=0;i<mselection->Count ;i++)
x += atof(((TAccount*) mselection->Items[i])->Assistants);
return x;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//1	Header Identifier  =  1 		1
//2	Practice Number issued by RAMS     		15
//3	System Identification if practice number is used on multiple systems		3	Integer
//(Default = 1)
//4  	Type of Transaction = D     		1
//5 	Batch Number  (numeric sequence)		10	Integer
//6  	Batch Date   (CCYYMMDD) 	   	8
//7  	Source Software Identification ( e.g. Medix, Medit, etc.)		20
//8 	Source Software Version (e.g. 3.5h, V3.12, etc.) 	 	10


//DETAIL

//1 	Transaction Identifier = D	   	1
//2  	Sequential Transaction Number in Batch      		10	Integer
//3 	Provider's System Transaction Number	 	10
//(Used for medical claim response transaction identification)
//4 	Provider's System Patient Reference Number (Account Number)	 	10
//5 	Type of Service		1
//D = Chronic Medicines; R = Repeat script indicator)
//6 	Medical Aid Reference Number (QEDI activation code)   		4
//7 	Not used
//8  	Medical Aid Membership Number      		20
//9  	Member Surname 		20
//10 	Member Initials	        	4
//11 	Dependant Number and  Patient name (Eg. 01 FRED)		20
//12 	Patient date of birth   (CCYYMMDD)		8
//13 	RAMS Tariff Code (or GMED for medicine)      		10
//14 	Service Amount (Inclusive of VAT; before deduction of levy or discount)		15	Numeric
//15 	Diagnosis / Tariff Description / Medicine Description 		30
//16 	Discount Amount (not negative)        		15	Numeric
//17  	Service Date   (CCYYMMDD)        		8
//18 	Number of Services, Time in minutes, Kilometres, etc.	      	7	Numeric
//19 	Service Fee			1	Integer
//(1=MASA; 2=Scale of Benefits; 3=Injured on Duty; 4=Managed Fee
// for Service; 5 = Group Capitation; 6 = Individual Capitation)
//20 	Modifier 1			5
//21	Modifier 2			5
//22	Modifier 3			5
//23	Modifier 4			5
//24 	Invoice Number / Dentist Lab Slip Number / Script Number		8
//25 	Referring Doctor's RAMS Practice Number or Name		15
//7	Referred to Practice Number x 5		35
//28	Not used ( Previously used for Medimo patient Reference Number)
//29	Attending Doctor's RAMS Practice Number 		7
//30	Hospital indicator (H = Procedure done while patient in hospital)		1
//31	Authorisation Number 		21
//32	Resubmission flag (R or original fund rejection code)		   5
//33	Diagnostic codes: ICPC, ICD10 or ICD-DA  (up to 5 codes, use / as delimiter)	64
//34     Diagnostic Code Type (P=ICPC; D=ICD10; A=ICD-DA)		1
//35	Dosage duration		4
//36	Attending Doctor's INMDC Number		15
//37	Tooth Numbers (string up to 8 x 2-digit numbers)		16
//38	Gender (M=Male; F=Female)		1
//39	Tariff Code Type		1
//(A=CPT; D=CDT; E=Optometrists; P=Orthotists; T=Tariff)
//40	CPT Code / CDT Code		8
//41	Free Text			210
//42	Indicator: Place of service		2
//43	Referring Doctor's INMDC Number		15
//44	Referral / Tracking number		15
//45	Reading additions (Optometry)		12
//46	Lens prescription (Optometry)		34
//47	Density of tint (Optometry)		6
//48	Patient ID number		13


//BATCH TRAILER

//1	Trailer Identifier  =  Z		1
//2  	Total Number of Detail Transactions in Batch		10	Integer
//3 	Total Amount of Detail Transactions		15	Numeric

//---------------------------------------------------------------------------

void TSelection::EDI()
{
bool continueanyway = false;
if (ContainsNonEDI())
continueanyway = (MessageDlg("Include Non EDI Accounts\nIn this EDI Batch?",
               mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0)==mrYes);


int seqnum=0;
TAccount* acc;
char str[50];
TCSVRec* header= new TCSVRec;
TList*   detail= new TList;
TCSVRec* tail  = new TCSVRec;
header->FieldCount=8;
tail->FieldCount=3;
if (!isdigit(msetup->Field[tmd::edibat][0]))msetup->Field[tmd::edibat]="0";

header->Field[0]="1";
header->Field[1]=msetup->Field[tmd::prnum];
header->Field[2]=msetup->Field[tmd::mednet];
header->Field[3]="D";
header->Field[4]=itoa((atoi(msetup->Field[tmd::edibat])+1),str,10);
header->Field[5]=TDateTime::CurrentDate().FormatString("yyyymmdd").c_str();
header->Field[6]="Turbomed";
header->Field[7]="1.1";

for (int i=0;i<mselection->Count;i++)
{
acc = (TAccount*)mselection->Items[i];

if ((acc->Field[tmd::sc][0]=='E')||continueanyway)
{
acc->datestamp("EDI");
mainform->TMData->isDirty=true;
for (int i=0;i<acc->Trans->Count;i++)
{
TCSVRec* CT = (TCSVRec*)acc->Trans->Items[i];
if (TransValidEDI(CT))
{
TCSVRec* T = new TCSVRec;
seqnum++;
T->FieldCount=48;
detail->Add(T);
T->Field[0]="D";
T->Field[1]=itoa(seqnum,str,10);
sprintf(str,"%s %d",acc->Field[tmd::nc],i);
T->Field[2]=str; //transaction number
T->Field[3]=acc->Field[tmd::nc];//"acc num";
T->Field[4]="T";//"R = repeat type of service";
T->Field[5]=MedAidEDINum(acc->Field[tmd::ma]);//"med aid qedi";
T->Field[6]="";
T->Field[7]=strncpy(str,acc->Field[tmd::man],20);//"manum";
T->Field[8]=strncpy(str,acc->Field[tmd::asname],20);//"accsname";
T->Field[9]=strncpy(str,acc->Field[tmd::ainit],4);//"accinit";
strcpy(str,strtok(acc->Field[tmd::pname]," "));
if (!str) strncpy(str,acc->Field[tmd::pname],20);
T->Field[10]=str;//"patname";
T->Field[11]=EDIDate(acc->Field[tmd::pbday]);// "dob";
T->Field[12]=CT->Field[tmd::code];//"ramscode";
T->Field[13]=CT->Field[tmd::amt];//"amount";
T->Field[14]=strncpy(str,CT->Field[tmd::proc],30);//"descrip";
T->Field[15]="";//"discount";
T->Field[16]=EDIDate(CT->Field[tmd::tdate]);//"dateof ser";
T->Field[17]=EDIAnaesTime(CT);//"time min";
T->Field[18]="2";//"2=med aid rate";
T->Field[19]="";//"modifier";
T->Field[20]="";//"modifier";
T->Field[21]="";//"modifier";
T->Field[22]="";//"modifier";
T->Field[23]="";//"invoice";
T->Field[24]=acc->Field[tmd::ref];//"referred by";
T->Field[25]="";//"refer to";
T->Field[26]="";
T->Field[27]="";
T->Field[28]=msetup->Field[tmd::prnum];//"rams pr num";
T->Field[29]="H";//"H= hospitat";
T->Field[30]=acc->Field[tmd::auth];//"Auth num";
T->Field[31]="";//"R=resubmission";
T->Field[32]="";//"diag codes";
T->Field[33]="";//"diag code type";
T->Field[34]="";
T->Field[35]="";//"imdcnum";
T->Field[36]="";
T->Field[37]=acc->Field[tmd::psex];//"gender";
T->Field[38]="T";//"tariff T";
T->Field[39]="";//"cptcode";
T->Field[40]="";//"free text";
T->Field[41]="";//"place of service";
T->Field[42]="";
T->Field[43]="";
T->Field[44]="";
T->Field[45]="";
T->Field[46]="";
T->Field[47] = (strlen(acc->Field[tmd::patid])==13)
? acc->Field[tmd::patid]:"";//"patient id num";
}
}
}
}
if (detail->Count>0)
{
//SetCurrentDir(mainform->pathname);
SetCurrentDir("\\");
if (!DirectoryExists("liveedi")) CreateDir("liveedi");
ofstream *out = new ofstream("liveedi\\export.dat",ios::out|ios::app);
*out<< header->CSV<<endl;
double amt=0;
for (int i=0;i<detail->Count;i++)
{
TCSVRec* T= (TCSVRec*)detail->Items[i];
amt+=atof(T->Field[13]);
*out<< T->CSV<<endl;
delete (TCSVRec*) T;
}
sprintf(str,"%12.2f",amt);
tail->Field[2]=str;
tail->Field[0]="Z";
tail->Field[1]=itoa(seqnum,str,10);
*out<< tail->CSV<<endl;
out->close();
delete out;
msetup->Field[tmd::edibat]=header->Field[4];
ShowMessage("EDI Export.dat file rebuilt successfully");
}
else
ShowMessage("Warning: EDI Export.dat file not changed");
delete header;
delete detail;
delete tail;
}


//int TSelection::InvoiceCount(void)
//{
//int x=0;
//for (int i=0;i<mselection->Count ;i++)
//if (!((TAccount*) mselection->Items[i])->NilBalance)
//x++;
//return x;
//}


double __fastcall TSelection::GetVat()
{
double x=0;
for (int i=0;i<mselection->Count ;i++)
x += atof(((TAccount*) mselection->Items[i])->
GetVatDue(msetup->Field[tmd::vat]));
return x;
}

char* TSelection::MedAidEDINum(char*s)
{
//change 19032003 updated to return valid edinum or ""
char* edinum = "";
//look through the medical aid list
for (int i=0;i < mainform->TMData->MedAidStrings->Count;i++)
{
if (String(s).UpperCase().Trim() ==
mainform->TMData->MedAidStrings->Strings[i].UpperCase().Trim())
{
   TCSVRec* M = (TCSVRec*)mainform->TMData->MedAidStrings->Objects[i];
   edinum = (M)? M->Field[tmd::medi]:"";
   if (strlen(edinum)) break; //if valid EDINUM then thats it
}
}
return edinum;  // valid edinumber or ""
}
/*
char* TSelection::MedAidEDINum(char*s)
{
char* edinum = "";
for (int i=0;i < mainform->TMData->MedAidStrings->Count;i++)
{
TCSVRec* M = (TCSVRec*)mainform->TMData->MedAidStrings->Objects[i];
edinum = (M)? M->Field[tmd::medi]:"";
if ((String(s) == mainform->TMData->MedAidStrings->Strings[i])&&
     (strlen(edinum))) break;
}
//done: changed to not just return the first MA found
//int x = mainform->TMData->MedAidStrings->IndexOf(s);
//TCSVRec* M = (TCSVRec*)mainform->TMData->MedAidStrings->Objects[x];
//return (M)? M->Field[tmd::medi]:"";
return edinum;
}
*/

bool TSelection::TransValidEDI(TCSVRec*T)
{
if (!strcmp(T->Field[tmd::code],"PAYM")) return false;
return (atof(T->Field[tmd::amt])>0);
}


char* TSelection::EDIAnaesTime(TCSVRec * T)
{
char* s= T->Field[tmd::code];
char* d= T->Field[tmd::proc];
static char r[5];
memset(r,0,5);
int x=0;
if      ((strcmp(s,"0025")==0)||
         (strcmp(s,"0023")==0)||
         (strcmp(s,"0011")==0)||
         (strcmp(s,"0039")==0)||
         (strcmp(s,"0012")==0)||
         (strcmp(s,"1211")==0))
 {
 for(unsigned i=0;i<strlen(d);i++)
 if (isdigit(d[i])) r[x++]=d[i];
 }
else strcpy(r,"1");  //number of services=1;

return r;
}




void TSelection::BuildStatementList(TList* SL)
{
//make a list of lists of accounts
//first build a list of potential statements from the selection
//then looks thru the active accounts and adds matching active accounts
//the second and subsequent accounts in the list are for processing
//the first account in the list is deleted.
for (int i=0;i<List->Count;i++)
{
void *s = StatementExists(SL,List->Items[i]);
if (!s) //((TList*)s)->Add(List->Items[i]);
        //else
        {SL->Add(new TList);
              ((TList*)SL->Last())->Add(List->Items[i]);}

}
//now go thru all the active accounts
for (int i=0;i<mainform->TMData->Patients->Count;i++)
{
void* acc= mainform->TMData->Patients->Items[i];
if (((TAccount*)acc)->Field[tmd::tag][0] == '\0') //add only active accounts
        {
        void *s = StatementExists(SL,acc);
        if (s) ((TList*)s)->Add(acc);
        }
}
for (int i=0;i<SL->Count;i++)
((TList*)SL->Items[i])->Delete(0);//delete the first account to leave only those from the active folder
}

void* TSelection::StatementExists(TList* SL, void* ptr)
{
void* vp=NULL;
TAccount* acc = (TAccount*) ptr;
for (int i=0;i<SL->Count;i++)
{
TList * lp = (TList*)SL->Items[i];
TAccount* acc2 = (TAccount*)lp->Items[0];
if ((!strcmp(acc->Field[tmd::asname],acc2->Field[tmd::asname]))&&
    (!strcmp(acc->Field[tmd::ainit],acc2->Field[tmd::ainit])))
{vp=lp;break;}
}
return vp;
}

int TSelection::StatementCount(TList* SL)
{
int x=0;
for(int i=0;i<SL->Count;i++)
        if (((TList*)SL->Items[i])->Count) x++;

return  x;
}



void TSelection::PrintStatements(void)
{
TList* SL = new TList;
BuildStatementList(SL);

char *str = new char[100];
TPrinter * prin= Printer();
sprintf(str,"\
Printer: %s\n\
%d Statements(s) Selected\n\
Continue Printing ?",prin->Printers->Strings[prin->PrinterIndex],StatementCount(SL));




if (Application->MessageBox(str,"Print Statements",
MB_OKCANCEL)==IDOK)
{
TA5Page *pg = new TA5Page();
TAccount*acc;
pg->FooterHeight=1;
pg->printer->Canvas->Pen->Width=5;
//pg->Heading = "Cont...";
for (int i=0;i<SL->Count;i++)
{
acc = (TAccount*)((TList*)SL->Items[i])->Last();
//if ((!acc->NilBalance)||(mselection->Count==1))
//{
pg->NewPage();
sprintf(str,"%s %s",acc->Field[tmd::nc],acc->Field[tmd::asname]);
pg->Heading = str;
PrintHeading(pg);
//PrintInvoiceType(pg,acc);
pg->TextStyle=TA5Page::a5BoldItalic;
pg->Separator(7);
pg->Write (5,8,"STATEMENT",TA5Page::LJ);
pg->Separator(9);

PrintDetails(pg,acc);
pg->ClearLine(18);
pg->ClearLine(19);
pg->ClearLine(20);
pg->NextLine(17);
 int l;
 char *s;

for (int j=0;j< ((TList*)SL->Items[i])->Count;j++)
{
TAccount* a= (TAccount*)((TList*)SL->Items[i])->Items[j];
//pg->NextLine(0);
l= pg->NextLine(0);

sprintf(str,"Invoice: %s Patient: %s",a->Field[tmd::nc],a->Field[tmd::pname]);
pg->TextStyle=TA5Page::a5Italic;
pg->Write (22,l,str,TA5Page::LJ);
pg->TextStyle=TA5Page::a5Normal;

for (int k=0;k<a->Trans->Count;k++)
 {
 l = pg->NextLine(0);
 s = ddmmmyyyy(((TCSVRec*)a->Trans->Items[k])->Field[tmd::tdate]);
 if (k>0) {if  (!strcmp(((TCSVRec*)a->Trans->Items[i])->Field[tmd::tdate],
              ((TCSVRec*)a->Trans->Items[k-1])->Field[tmd::tdate]))
               s="    ''";}
 pg->Write(5,l,s,TA5Page::LJ);
 pg->Write(25,l,((TCSVRec*)a->Trans->Items[k])->Field[tmd::code],TA5Page::LJ);
 pg->Write(35,l,((TCSVRec*)a->Trans->Items[k])->Field[tmd::proc],TA5Page::LJ);
 pg->Write(90,l,((TCSVRec*)a->Trans->Items[k])->Field[tmd::amt],TA5Page::RJ);
 }

//bool intflag = (strlen(a->Field[tmd::sc])>1);
// if (intflag)
// {
// j=p->NextLine(0);
// p->Write(25,j,"INT",TA5Page::LJ);
// p->Write(35,j,"Interest Charged",TA5Page::LJ);
// p->Write(90,j,a->IntCalc(msetup->Field[tmd::interest]),TA5Page::RJ);
// }



//PrintTransactions(pg,subacc);
}
PrintStatementMessage(pg,(TList*)SL->Items[i]);
//acc->datestamp("STATEMENT");
//}
}
pg->Finish();
delete pg;
}
delete[] str;
for (int i=0;i<SL->Count;i++) delete SL->Items[i];
delete SL;
}




void TSelection::PrintStatementMessage(TA5Page* p, TList* accs)
{
double current = 0;
double day60   = 0;
double day30 = 0;
double interest = 0;
double temp;
char* err;

for (int i=0;i<accs->Count;i++)
{
TAccount* acc = (TAccount*)accs->Items[i];
bool intflag = (strlen(acc->Field[tmd::sc])>1);
interest += (intflag)?
atof(acc->IntCalc(msetup->Field[tmd::interest])):0;
temp = atof(acc->Balance);

int age= TStrDate(acc->Field[tmd::dt]).Age();
if (age<30) current+=temp;
  else if (age>=60) day60+=temp;
        else day30+=temp;
}
char* str = new char[11];

p->TextStyle=TA5Page::a5Bold;
p->NextLine(0);
p->Separator(p->NextLine(0));
int l = p->NextLine(0);
p->Write(15,l,"Interest",TA5Page::RJ);
p->Write(35,l,"60 Days+",TA5Page::RJ);
p->Write(55,l,"30 Days",TA5Page::RJ);
p->Write(75,l,"Current",TA5Page::RJ);
p->Write(95,l,"Owing",TA5Page::RJ);
p->NextLine(0);
//interest
//interest = 0/100/12*day60;
sprintf(str,"%10.2f",interest);
p->Write(15,l,str,TA5Page::RJ);
//60days
sprintf(str,"%10.2f",day60);
p->Write(35,l,str,TA5Page::RJ);
//30days
sprintf(str,"%10.2f",day30);
p->Write(55,l,str,TA5Page::RJ);
//current
sprintf(str,"%10.2f",current);
p->Write(75,l,str,TA5Page::RJ);
//owing
sprintf(str,"%10.2f",current + day30 + day60 + interest);
p->Write(95,l,str,TA5Page::RJ);
p->Separator(p->NextLine(0));
delete[] str;
}


int TSelection::ContainsEDI(void)
{
bool edi = 0;
for (int i=0;i<mselection->Count;i++)
{
TAccount* acc = (TAccount*)mselection->Items[i];
if (acc->Field[tmd::sc][0]=='E')edi++;
}
return edi;
}

bool TSelection::ContainsNonEDI(void)
{
bool nonedi = false;
for (int i=0;i<mselection->Count;i++)
{
TAccount* acc = (TAccount*)mselection->Items[i];
if (acc->Field[tmd::sc][0]!='E') {nonedi=true;break;}
}
return nonedi;
}
