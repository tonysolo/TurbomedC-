//---------------------------------------------------------------------------
//#include <vcl.h>
#pragma hdrstop

#include "CSVRec.h"
#include "StrDate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Math.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

int TAccount::m_count=0;
int TAccount::ndc=0;

TCSVRec::TCSVRec()
{
m_dat=NULL;
m_fieldcnt=0;
}

//---------------------------------------------------------------------------

TCSVRec::~TCSVRec()
{
//if (m_fieldcnt)
for (int i=0;i<m_fieldcnt;i++)
delete[] m_dat[i];
delete[] m_dat;
}

//---------------------------------------------------------------------------
char * TCSVRec::trim(char * s)
{
static char s1[100];
strcpy(s1,s);

if (s1[0]==' ')
{
for (unsigned i =0;i<strlen(s1)-1;i++)
if ((s1[i]==' ')&&(s1[i+1]!=' ')) {strcpy(s1,&s1[i+1]);break;}
}
if (s1[strlen(s1)-1]==' ')
{
for (unsigned i=strlen(s1)-1;i>0;i--)
if ((s1[i]!=' ')&&(s1[i+1]==' ')) {s1[i+1]=0x00; break;}
}
//finally replace any commas with semicolons
 for (unsigned i =0;i<strlen(s1);i++) if(s1[i]==',') s1[i]=';';

return s1;
}
//---------------------------------------------------------------------------

void TCSVRec::fsetcsv(char*s)
{
char *str = new char[strlen(s)+1];
strcpy(str,s);
int x=1;  //field count is 1 plus the number of commas
          //replace commas with zeros == null terminated strings
for (unsigned i=0;i<strlen(s);i++) if (str[i]==',')
{
str[i]=0;
x++;
}
if (m_fieldcnt<x) x = m_fieldcnt;// if fields have been reduced

if (m_dat[0]) delete[] m_dat[0];
m_dat[0] = new char[strlen(&str[0])+1];
strcpy(m_dat[0],&str[0]);// copy the first field

int j = 0;   // then copy all the others
for (unsigned i=0;i<strlen(s);i++)
{
if ((str[i]==0)&&(++j< m_fieldcnt))
    {
    if (m_dat[j]){delete[] m_dat[j];m_dat[j]=NULL;}
    if (j<x)
    {
    m_dat[j] = new char[strlen(&str[i+1])+1];
    strcpy(m_dat[j],&str[i+1]);
    }
    }
}
delete[] str; // delete the temporary string
}

//---------------------------------------------------------------------------

char* TCSVRec::fgetcsv()
{
if (!m_fieldcnt) return NULL;

static char str[MAXCSV];
strcpy(str,(Field[0])?trim(Field[0]):"");
for (int i=1;i < m_fieldcnt;i++)
{
strcat(str,",");
strcat(str,(Field[i])?trim(Field[i]):"");
}
return str;
}
//---------------------------------------------------------------------------

char TCSVRec::fgetid()
{
if (!m_fieldcnt) return 0;
return Field[0][0];
}
//---------------------------------------------------------------------------

void TCSVRec::fsetid(char c)
{
if (m_fieldcnt)Field[0][0]=c;
}
//---------------------------------------------------------------------------

int TCSVRec::fgetfldcnt()
{
return m_fieldcnt;
}
//---------------------------------------------------------------------------

void TCSVRec::fsetfldcnt(int n)
{
if (m_fieldcnt)
{
for (int i= 0;i<m_fieldcnt;i++)
{if (m_dat[i]) delete[] m_dat[i];}
delete[] m_dat;
}
m_fieldcnt = n;
m_dat = new char*[n];
for (int i= 0;i<m_fieldcnt;i++) m_dat[i]=NULL;
}
//---------------------------------------------------------------------------

char *TCSVRec::fgetfield(int i)
{
char* x = ((i > -1)&&(i< m_fieldcnt))? m_dat[i]:"";
return (strlen(x))? x:"";
}
//---------------------------------------------------------------------------

void TCSVRec::fsetfield(int i,char*s)
{
if ((i > -1)&&(i < m_fieldcnt))
{
if (m_dat[i]) delete[] m_dat[i];
m_dat[i] = new char[strlen(s)+1];
strcpy(m_dat[i],s);
}
}
//---------------------------------------------------------------------------
// TList* m_trans;
// TList* m_notes;



// public:
 //
// 
 //__property TList* Trans = {read=m_trans};
 //__property TList* Notes = {read=m_notes};

//---------------------------------------------------------------------------

TAccount::TAccount()//:TCSVRec()
{
m_trans = new TList;
m_notes = new TList;
mpay = new char[10];
mbal = new char[10];
IsDirty = true;
//BalUpdated = false;
//PayUpdated = false;
m_count++;
}
//---------------------------------------------------------------------------
//TAccount::TAccount(TAccount & A)//:TCSVRec()
//{
//m_fieldcnt = A.m_fieldcnt;
//m_dat = new char*[m_fieldcnt];
//for (int i=0;i<m_fieldcnt ;i++)
//{
//m_dat[i] = new char[strlen(A.m_dat[i])+1];
//strcpy(m_dat[i],A.m_dat[i]);
//}
//m_trans = new TList;
//for (int i=0;i<A.m_trans->Count ;i++)
//m_trans->Add(A.m_trans->Items[i]);

//m_notes = new TList;
//for (int i=0;i<A.m_notes->Count ;i++)
//m_notes->Add(A.m_notes->Items[i]);
//}
//---------------------------------------------------------------------------

TAccount::~TAccount()
{
for (int i=0;i<m_trans->Count;i++)
delete (TCSVRec*)m_trans->Items[i];
for (int i=0;i<m_notes->Count;i++)
delete (TCSVRec*)m_notes->Items[i];
delete m_trans;
delete m_notes;
delete[] mpay;
delete[] mbal;
m_count--;
//TCSVRec::~TCSVRec();
}
//---------------------------------------------------------------------------
//char* TAccount::fbalance()
//{
//double f=0;
//static char str[10];
//for (int i=0;i<m_trans->Count ;i++)
//{
//TCSVRec* T = (TCSVRec*)m_trans->Items[i];
//if (!strcmp(T->Field[code],"PAYM"))f-= atof (T->Field[amt]);
//else f+= atof (T->Field[amt]);
//}
//sprintf(str,"%9.2f",f);
//return str;
//}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
char* TAccount::fbalance()
{
if (IsDirty) UpdateBalPay();
return mbal;
}
//---------------------------------------------------------------------------


bool TAccount::fnilbalance()
{
return (fabs(atof(fbalance()))<10)?true:false;
}

bool TAccount::fpaid()
{
bool b=false;
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if (!strcmp(T->Field[tmd::code],"PAYM")){b=true;break;}
}
return b;
}

//---------------------------------------------------------------------------
char* TAccount::fbilled()
{
double f =0;
double f1;
static char str[10];
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if (strcmp(T->Field[tmd::code],"PAYM"))
{
f1 = atof (T->Field[tmd::amt]);
f+= (f1>0) ? f1:0;
}
}
sprintf(str,"%9.2f",f);
return str;

}
//---------------------------------------------------------------------------
//char* TAccount::fpayments()
//{
//double f=0;
//static char str[10];
//for (int i=0;i<m_trans->Count ;i++)
//{
//TCSVRec* T = (TCSVRec*)m_trans->Items[i];
//if (!strcmp(T->Field[code],"PAYM"))
//f+= atof (T->Field[amt]);
//}
//sprintf(str,"%9.2f",f);
//return str;
//}
//---------------------------------------------------------------------------
 //---------------------------------------------------------------------------
char* TAccount::fpayments()
{
if (IsDirty) UpdateBalPay();
return mpay;
}
//---------------------------------------------------------------------------

bool TAccount::frecycled()
{
return (Field[tmd::tag][0]=='D')?true:false;
}
//---------------------------------------------------------------------------

bool TAccount::fvat()
{
return (Field[tmd::tag][0]=='V')?true:false;
}
//---------------------------------------------------------------------------
/*
char * TAccount::vatdate()
{
static char str[10];
strcpy(str,"");
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if (!strcmp(T->Field[tmd::code],"PAYM"))
strcpy(str,T->Field[tmd::tdate]);
}
return str;
}
*/
//---------------------------------------------------------------------------
bool TAccount::vatstamped()
{
bool b=false;
for (int i=0;i<m_notes->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_notes->Items[i];
if (!strncmp(T->Field[tmd::pnote],"[VAT",4)) b=true;
}
return b;
}
//---------------------------------------------------------------------------

void TAccount::datestamp(char*stmp)
{
char s[30];
sprintf(s,"[%s%s",stmp,TDateTime::CurrentDate().FormatString("dd/mm/yyyy").c_str());
TCSVRec *T = new TCSVRec;
T->FieldCount = tmd::notecnt;
T->Field[tmd::pnote] = s;
m_notes->Add(T);
}
//---------------------------------------------------------------------------
void TAccount::ToArchive()
{
Field[tmd::tag]="D";
}
//---------------------------------------------------------------------------
void TAccount::ToVat()
{
if (strlen(VatDate)>1) Field[tmd::tag]="V";
}
//---------------------------------------------------------------------------
void TAccount::ToActive()
{
Field[tmd::tag]="";
}

//---------------------------------------------------------------------------
void TAccount::Check_ndc()
{
int i = atoi(Field[tmd::nc]);
if (ndc<i) ndc=i;
}
//---------------------------------------------------------------------------

char* TAccount::GetAssistants()
{
double f=0;
static char str[10];
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if ((!strcmp(T->Field[tmd::code],"0009"))||
    (!strcmp(T->Field[tmd::code],"0008"))||
    (!strcmp(T->Field[tmd::code],"0029")))
f+= atof (T->Field[tmd::amt]);
}
sprintf(str,"%9.2f",f);
return str;
}


char* TAccount::GetVatDue(char* v)
{
double f= atof(fpayments())-atof(GetAssistants());
double vt=(Field[tmd::usm][0]!= '3')? atof(v):0;
static char str[10];
f*= vt/(vt+100);
sprintf(str,"%9.2f",f);
return str;
}


bool TAccount::Due(void)
{
bool b=true;
TCSVRec * n;
for (int i=0;i<Notes->Count;i++)
{
n = (TCSVRec *)Notes->Items[i];
char* str= n->Field[tmd::pnote];
if //((strlen(n->Field[tmd::pnote])==18)&&
   ((!strncmp(str,"[EDI",4))||
   (!strncmp(str,"[INV",4)))
        {
       TStrDate d(&str[strlen(str)-10]);
       b = (d.Age() > 30);

//        double x = TDateTime::CurrentDate();
//        double y;
//        try {
//        y = TDateTime(d.Year(),d.Month(),d.Day());
//            }
//        catch (EConvertError &)
//        {
//        y = x-31;
 //       }
//        short p = x - y;
 //       if (p<31) {b=false;break;}
        }
}
return b;
}


void TAccount::UpdateBalPay(void)
{
FVatDate=NULL;
double f=0;
/* TODO : Check this code - it should validate the procedure date */
TCSVRec* D =(m_trans->Count)? ((TCSVRec*)m_trans->Items[0]):NULL;
char* pd = (D) ? D->Field[tmd::tdate]:"";
FProcedureDate = (TStrDate(pd).Valid()) ? pd : Field[tmd::dt];
FVatDate = NULL;//Field[tmd::dt];
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if (!strcmp(T->Field[tmd::code],"PAYM"))
f-= atof (T->Field[tmd::amt]);
else f+= atof (T->Field[tmd::amt]);
}
sprintf(mbal,"%9.2f",f);

f=0;
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if (!strcmp(T->Field[tmd::code],"PAYM"))
{f+= atof (T->Field[tmd::amt]);
if (TStrDate(T->Field[tmd::tdate]).Valid()) FVatDate=T->Field[tmd::tdate];}
}
sprintf(mpay,"%9.2f",f);

IsDirty=false;
}


bool __fastcall TAccount::GetDiscountGiven()
{
bool b=false;
for (int i=0;i<m_trans->Count ;i++)
{
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
if (!strncmp(T->Field[tmd::code],"DIS",3))
   {b=true;break;}
}
return b;
}


char* __fastcall TAccount::GetProcedureDate()
{
if (IsDirty) UpdateBalPay();
return FProcedureDate;
}


char* TAccount::IntCalc(char* inter)
{

double interest=0;
double intrate;
static char s[10];

try {intrate = StrToFloat(inter);}
catch(EConvertError &e){intrate = 0;}
intrate /=100;
for (int i=0;i<m_trans->Count ;i++)
{
double temp;
TCSVRec* T = (TCSVRec*)m_trans->Items[i];
try  {temp = StrToFloat(T->Field[tmd::amt]);}
catch (EConvertError &e) {temp = 0;}

int t_age = TStrDate(T->Field[tmd::tdate]).Age();

if (t_age > 60)

        if (!strcmp(T->Field[tmd::code],"PAYM"))
               interest -= (temp * exp(intrate / 365 * t_age) - temp);
        else   interest += (temp * exp(intrate / 365 * t_age) -temp);
}
sprintf(s,"%7.2f",SimpleRoundTo(interest));
return s;
}


char* TAccount::Int_Balance(char* inter)
{
double interest,balance;
if (strlen(Field[tmd::sc])>1)
try {interest = StrToFloat(IntCalc(inter));} catch(EConvertError &e){interest=0;}
try {balance =  StrToFloat(Balance);} catch(EConvertError &e){balance=0;}
balance+=interest;
static char s[13];
sprintf(s,"%8.2f",balance);
return s;
}


int TAccount::AnaesTime(void)
{
int time = 0;
for (int i=0;i<Trans->Count;i++)
{
TCSVRec* tc= (TCSVRec*)Trans->Items[i];
if (tc)
{
int code = atoi(tc->Field[tmd::code]);
if
(
 (code == 11)||
 (code == 23)||
 (code == 25)||
 (code == 36)||
 (code==24)
)
{
//extractnumber
    char* s = tc->Field[tmd::proc];
    char* valstr = NULL;
    for (unsigned i=0;i<strlen(s);i++)
       if ((s[i]>='0')&&(s[i]<='9')) {valstr=&s[i];break;}
int n=0;
    if (valstr) n = atoi(valstr);

if (n > time) time = n;
}
}
}
return time;
}

char* __fastcall TAccount::GetVatDate()
{
if (IsDirty) UpdateBalPay();
return FVatDate;
}
