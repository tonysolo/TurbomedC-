//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "bkeep.h"
#include "CSVRec.h"
#include "StrDate.h"
#include "globals.h"
#include "tmFile.h"
#include "a4page.h"
#include <Clipbrd.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBookKeep *BookKeep;
//---------------------------------------------------------------------------
__fastcall TBookKeep::TBookKeep(TComponent* Owner,TList*L)
        : TForm(Owner)
{
allList=L;
}
//---------------------------------------------------------------------------
void __fastcall TBookKeep::FormCreate(TObject *Sender)
{
//char* dt=NULL;
//if (allList->Count)
//{
//TAccount* acc = (TAccount*)allList->First();
//dt= acc->Field[tmd::dt];
//}
char* dt = OldestAccountDate(allList);
DateTimePicker1->MinDate = (dt)
? TDateTime(TStrDate(dt).Year(),TStrDate(dt).Month(),TStrDate(dt).Day())
: TDateTime().CurrentDate();
DateTimePicker2->MinDate =  DateTimePicker1->MinDate;
DateTimePicker1->MaxDate = TDateTime().CurrentDate();
DateTimePicker2->MaxDate = TDateTime().CurrentDate();
//OutputDebugString(DateTimePicker1->MaxDate.DateString().c_str());
//DateTimePicker2->MaxDate =  DateTimePicker1->MaxDate;
DateTimePicker2->Date = DateTimePicker2->MaxDate;

int x = DateTimePicker2->Date - DateTimePicker1->MinDate ;

DateTimePicker1->Date = (x > 30)
? DateTimePicker2->Date - 30
: DateTimePicker1->MinDate;

datalist = new TList;
UpdateDataList();
//ListBox2->Count=12;
//ListBox1->Count=datalist->Count;
}
//---------------------------------------------------------------------------

void __fastcall TBookKeep::FormClose(TObject *Sender, TCloseAction &Action)
{
for (int i=0;i<datalist->Count;i++) delete (bookkeepdata*)datalist->Items[i];
}
//---------------------------------------------------------------------------


void __fastcall TBookKeep::ListBox1DrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
char* date = "";
char* ref =  "";
char* accname ="";
char* code = "";
char* descr = "";
char* debit = "";
char* credit = "";
char* bal = NULL;
char* totaldebit=NULL;
char* totalcredit=NULL;

if (datalist->Count)
{
TCanvas* can = ((TListBox*)Control)->Canvas;
can->Font->Color=clBlack;
can->Brush->Color=clWhite;
if (Index==0) {
         //strcpy(date,DateTimePicker1->Date.DateString().c_str());
         descr="Brought Forward";
         char* str=FormatFloat("#,##0.00",broughtforward).c_str();
         bal= strcpy(new char[strlen(str)+1],str);
         can->Font->Style=TFontStyles()<< fsBold;}
if (Index==ListBox1->Count-1){
        //strcpy(date,DateTimePicker2->Date.DateString().c_str());
        descr="Total";
        char* str= FormatFloat("#,##0.00",totalpay).c_str();
        totalcredit = strcpy(new char[strlen(str)+1],str);
        credit=totalcredit;
        str= FormatFloat("#,##0.00",totalbilled).c_str();
        totaldebit = strcpy(new char[strlen(str)+1],str);
        debit=totaldebit;
        
   //     bal= FormatFloat("#,##0.00",totalbilled);
   //     credit= FormatFloat("#,##0.00",totalpay).c_str();
   //     debit= FormatFloat("#,##0.00",totalbilled).c_str();
        can->Font->Style=TFontStyles()<< fsBold;}
if ((Index>0)&&(Index<ListBox1->Count-1))
{
bookkeepdata* data = (bookkeepdata*)datalist->Items[Index-1];
date = BookDate(data->transact,data->account);
date = DelimDate(date);
//date = DelimDate(((TCSVRec*)data->transact)->Field[tmd::tdate]);
ref =   ((TAccount*)data->account)->Field[tmd::nc];
accname = ((TAccount*)data->account)->Field[tmd::asname];
code =  ((TCSVRec*)data->transact)->Field[tmd::code];
descr = ((TCSVRec*)data->transact)->Field[tmd::proc];
if (!stricmp(code,"PAYM")) credit= ((TCSVRec*)data->transact)->Field[tmd::amt];
        else debit = ((TCSVRec*)data->transact)->Field[tmd::amt];


char* str=FormatFloat("#,##0.00",data->balance).c_str();
bal= strcpy(new char[strlen(str)+1],str);

}
if (!stricmp(code,"PAYM"))can->Brush->Color=(atoi(credit)<0)?clYellow:clAqua;
else can->Brush->Color=(atoi(debit)<0)?clLime:clWhite;

if (Assistant(code))can->Brush->Color = clLtGray;

int tab1=HeaderControl1->Sections->Items[0]->Width;
int tab2=tab1+HeaderControl1->Sections->Items[1]->Width;
int tab3=tab2+HeaderControl1->Sections->Items[2]->Width;
int tab4=tab3+HeaderControl1->Sections->Items[3]->Width;
int tab5=tab4+HeaderControl1->Sections->Items[4]->Width;
int tab6=tab5+HeaderControl1->Sections->Items[5]->Width;
int tab7=tab6+HeaderControl1->Sections->Items[6]->Width;
int tab8=tab7+HeaderControl1->Sections->Items[7]->Width;

can->FillRect(Rect);
can->TextOut(Rect.Left,Rect.Top,date);
can->TextOut(Rect.Left+tab1,Rect.Top,ref);
can->TextOut(Rect.Left+tab2,Rect.Top,String(accname).SubString(1,12)); //125
can->TextOut(Rect.Left+tab3,Rect.Top,code);     //225
can->TextOut(Rect.Left+tab4,Rect.Top,descr);    //275
//right justify
can->TextOut(Rect.Left+tab6-can->TextWidth(debit),Rect.Top,debit);   //475 445
can->TextOut(Rect.Left+tab7-can->TextWidth(credit),Rect.Top,credit); //525 515
if (bal) can->TextOut(Rect.Left+tab8-can->TextWidth(bal),Rect.Top,bal); //600


//can->TextOutA(Rect.Left+425,Rect.Top,debit);
//can->TextOutA(Rect.Left+475,Rect.Top,credit);
//can->TextOutA(Rect.Left+525,Rect.Top,bal.c_str());

}
delete[] bal;
delete[] totaldebit;
delete[] totalcredit;
}
//---------------------------------------------------------------------------




void TBookKeep::UpdateDataList(void)
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;

try
{
for (int i=0;i<datalist->Count ;i++) delete (bookkeepdata*)datalist->Items[i];
datalist->Clear();

broughtforward=0;   //global


for (int i=0;i<allList->Count;i++)
{
TAccount* acc= (TAccount*)allList->Items[i];
//TStrDate accsd(acc->Field[tmd::dt]);  //accStrDate
//TDateTime accdt(accsd.Year(),accsd.Month(),accsd.Day()); //accDateTime
//if (accdt<DateTimePicker2->Date) //if before or in range adjust balance BF
//    if (!strlen(acc->Field[tmd::tag]))//if active
//    broughtforward-=StrToFloat(acc->Balance);
char tagchar =  acc->Field[tmd::tag][0];

if (tagchar != 'Z')//&&(tagchar != 'I'))
for (int j=0;j<acc->Trans->Count ;j++)
{
TCSVRec* tr= (TCSVRec*)acc->Trans->Items[j];
char* bd = BookDate(tr,acc);
TStrDate sd(bd);
if (sd.Valid())
{
TDate dt(sd.Year(),sd.Month(),sd.Day());
if ((int)dt.Val< (int)DateTimePicker1->Date.Val)
{
double amt;
try{amt= StrToFloat(tr->Field[tmd::amt]);}catch(EConvertError &e){amt=0;};
if (!stricmp(tr->Field[tmd::code],"PAYM"))
broughtforward-=amt;
else broughtforward+=amt;
}

if (((int)dt.Val >= (int)(DateTimePicker1->Date.Val)) &&
((int)dt.Val <=  (int)(DateTimePicker2->Date.Val)))
{
bookkeepdata* bk = new bookkeepdata;
bk->account=acc;
bk->transact=tr;
datalist->Add(bk);
}
}
}
}

datalist->Sort(Compare); // TListSortCompare
double runningbalance=broughtforward;
totalpay=0;           //global totals
totalbilled=0;
//totalrefund=0;
//totaladjust=0;
//totalassist=0;

//update the balances on the sorted list
for (int i=0;i<datalist->Count ;i++)
{
bookkeepdata *bk= (bookkeepdata*)datalist->Items[i];
TCSVRec* tr = (TCSVRec*)bk->transact;
double amt;
try{amt= StrToFloat(tr->Field[tmd::amt]);}catch(EConvertError &e){amt=0;}
if (!stricmp(tr->Field[tmd::code],"PAYM"))
{runningbalance-=amt;
//if (amt>0)totalpay+=amt;else totalrefund+=amt*=-1;
totalpay+=amt;
}
else {runningbalance+=amt;
//if(amt>0)totalbilled+=amt; else totaladjust+=amt*=-1;
totalbilled+=amt;
}

//if (!stricmp(tr->Field[tmd::code],"0000")) totalassist+=amt;

bk->balance=runningbalance;
}
//amt1->Caption=FormatFloat("#,##0.00",broughtforward);
//amt2->Caption=FormatFloat("#,##0.00",totalpay);
//amt3->Caption=FormatFloat("#,##0.00",totalrefund);
//amt4->Caption=FormatFloat("#,##0.00",totaladjust);
//amt5->Caption=FormatFloat("#,##0.00",totalassist);
//amt6->Caption=FormatFloat("#,##0.00",totalbilled);

ListBox1->Count = (datalist->Count)? datalist->Count+2:0;
ListBox1->Invalidate();
UpdateButton->Enabled=false;
}
__finally
{
Screen->Cursor = Save_Cursor;
}
}


int __fastcall Compare(void* item1, void* item2)
{
bookkeepdata* rec1 = (bookkeepdata*)item1;
bookkeepdata* rec2 = (bookkeepdata*)item2;
TCSVRec* csv1 = (TCSVRec*)rec1->transact;
TCSVRec* csv2 = (TCSVRec*)rec2->transact;

TAccount* acc1=  (TAccount*) rec1->account;
TAccount* acc2=  (TAccount*) rec2->account;

char * date1 = csv1->Field[tmd::tdate];
date1 = (strlen(date1)==8)? date1: acc1->ProcedureDate;
char * date2 = csv2->Field[tmd::tdate];
date2 = (strlen(date2)==8)? date2: acc2->ProcedureDate;

//String str1 = revdate(csv1->Field[tmd::tdate]);
//String str2 = revdate(csv2->Field[tmd::tdate]);

String str1 = revdate(date1);
String str2 = revdate(date2);

str1+=String(acc1->Field[tmd::asname]);
str2+=String(acc2->Field[tmd::asname]);
//TStrDate sd1(csv1->Field[tmd::tdate]);
//TStrDate sd2(csv2->Field[tmd::tdate]);

return str1.AnsiCompareIC(str2);
//int x = atoi(str1);
//int y = atoi(str2);
//if (x<y) return -1;
//else if (x<y) return 1;
//else return 0;
}
//---------------------------------------------------------------------------





void __fastcall TBookKeep::DateTimePicker1Change(TObject *Sender)
{
UpdateButton->Enabled=true;;
}
//---------------------------------------------------------------------------
void TBookKeep::DrawLegend(void* label, int col)
{
TLabel* L= (TLabel*)label;

TColor savecol=BookKeep->Canvas->Brush->Color;
BookKeep->Canvas->Brush->Color=(TColor)col;
int x=L->Height;
TRect r(L->Left - 2*x,
        L->Top,
        L->Left - x,
        L->Top + x);
BookKeep->Canvas->Rectangle(r);
BookKeep->Canvas->Brush->Color=savecol;
}
//---------------------------------------------------------------------------

void __fastcall TBookKeep::FormPaint(TObject *Sender)
{
DrawLegend(paylabel,clAqua);
DrawLegend(refundlabel,clYellow);
DrawLegend(adjustlabel,clLime);
DrawLegend(assistantlabel,clLtGray);

}
//---------------------------------------------------------------------------


void __fastcall TBookKeep::DateTimePicker1Click(TObject *Sender)
{
//UpdateDataList();
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void __fastcall TBookKeep::HeaderControl1SectionResize(
      THeaderControl *HeaderControl, THeaderSection *Section)
{
ListBox1->Invalidate();
}
//---------------------------------------------------------------------------


void __fastcall TBookKeep::FileSaveButtonClick(TObject *Sender)
{
TStringList* sl= new TStringList;
BuildCSV(sl);
TSaveDialog* D = new TSaveDialog(this);
//D->FileName = "Export";
D->Title = "Report";
D->FileName = newreportname();
D->Filter = "CSV File (*.csv)|*.CSV";
D->DefaultExt = String("csv");
if (D->Execute()) sl->SaveToFile(D->FileName);
delete D;

//delete[] balance;
delete sl;
}

//---------------------------------------------------------------------------


String TBookKeep::newreportname(void)
{
int i;
String f;
for (i=1;i<100;i++)
{
f = "Report"+IntToStr(i)+".csv";
if (!FileExists(f)) break;
}
return f;
}

void __fastcall TBookKeep::PrintButtonClick(TObject *Sender)
{
if (Application->MessageBox("Continue Printing ?","Print Debtors Journal",
MB_OKCANCEL)==IDOK)
{
char* str=new char[100];
TA4Page* A4= new TA4Page;
sprintf(str,"DEBTORS JOURNAL FOR PERIOD %s to %s",
DateTimePicker1->Date.DateString().c_str(),
DateTimePicker2->Date.DateString().c_str());

//Heading //lineTop //LinesBody
A4->PrintAt(0,str,TA4Page::LJ);
//Captions
A4->NextLine();A4->NextLine();
//"Date,Ref,Account,Code,Description,Debit,Credit,Balance"
A4->PrintAt(0,"---",TA4Page::LJ);
A4->PrintAt(5,"Date",TA4Page::LJ);
A4->PrintAt(15,"Ref",TA4Page::LJ);
A4->PrintAt(20,"Account",TA4Page::LJ);
A4->PrintAt(32,"Code",TA4Page::LJ);
A4->PrintAt(38,"Description",TA4Page::LJ);
A4->PrintAt(67,"Debit",TA4Page::RJ);
A4->PrintAt(82,"Credit",TA4Page::RJ);
A4->PrintAt(100,"Balance",TA4Page::RJ);
A4->NextLine(); A4->NextLine();
//lineTop
A4->PrintAt(38,"Brought Forward",TA4Page::LJ);
A4->PrintAt(100,FormatFloat("0.00",broughtforward).c_str(),TA4Page::RJ);
A4->NextLine();
//for (int i=0;i<70;i++)
//{

//A4->PrintAt(100,"Tony Manicom",TA4Page::RJ);
//}
//
for (int i=0;i<datalist->Count ;i++)
{
A4->NextLine();
char* debit="";
char* credit="";
//char* balance=NULL;
bookkeepdata* data = (bookkeepdata*)datalist->Items[i];
char* date = BookDate(data->transact,data->account);
date = DelimDate(date);
//DelimDate(((TCSVRec*)data->transact)->Field[tmd::tdate]);
char* ref =   ((TAccount*)data->account)->Field[tmd::nc];
char* accname = ((TAccount*)data->account)->Field[tmd::asname];
char* code =  ((TCSVRec*)data->transact)->Field[tmd::code];
char* descr = ((TCSVRec*)data->transact)->Field[tmd::proc];
if (!stricmp(code,"PAYM")) credit= ((TCSVRec*)data->transact)->Field[tmd::amt];
        else debit = ((TCSVRec*)data->transact)->Field[tmd::amt];
strcpy(str,FormatFloat("0.00",data->balance).c_str());


//if (!stricmp(code,"PAYM")) credit= ((TCSVRec*)data->transact)->Field[tmd::amt];
//        else debit = ((TCSVRec*)data->transact)->Field[tmd::amt];
//char* str=FormatFloat("#,##0.00",data->balance).c_str();
//bal= strcpy(new char[strlen(str)+1],str);
//}

char* str1;
if (!stricmp(code,"PAYM"))str1=(credit[0]=='-')?"ref":"pay";
else str1=(debit[0]=='-')?"adj":"bil";
if (Assistant(code)) str1 = "ast";

A4->PrintAt(0,str1,TA4Page::LJ);
A4->PrintAt(5,date,TA4Page::LJ);
A4->PrintAt(15,ref,TA4Page::LJ);
A4->PrintAt(20,accname,TA4Page::LJ);
A4->PrintAt(32,code,TA4Page::LJ);
A4->PrintAt(38,descr,TA4Page::LJ);
A4->PrintAt(67,debit,TA4Page::RJ);
A4->PrintAt(82,credit,TA4Page::RJ);
A4->PrintAt(100,str,TA4Page::RJ);

}
A4->NextLine();A4->NextLine();
A4->PrintAt(38,"Total",TA4Page::LJ);

A4->PrintAt(67,FormatFloat("0.00",totalbilled).c_str(),TA4Page::RJ);
A4->PrintAt(82,FormatFloat("0.00",totalpay).c_str(),TA4Page::RJ);

//        FormatFloat("0.00",totalbilled)+","+
//        FormatFloat("0.00",totalpay)+",";



delete A4;
delete[] str;
}
}
//---------------------------------------------------------------------------

void __fastcall TBookKeep::UpdateButtonClick(TObject *Sender)
{
UpdateDataList();
}
//---------------------------------------------------------------------------


char* TBookKeep::BookDate(void* tr, void* ac)
{
TAccount* acc = (TAccount*)ac;
TCSVRec* tran = (TCSVRec*)tr;

return (strlen(tran->Field[tmd::tdate])==8)? tran->Field[tmd::tdate]:
                                              acc->ProcedureDate;

//TStrDate ad(acc->Field[tmd::dt]);
//TStrDate ad(acc->ProcedureDate);
//TDateTime dt;
//try {dt = TDateTime(sd.Year(),sd.Month(),sd.Day());}
//catch(EConvertError &e)
//{dt = TDateTime(ad.Year(),ad.Month(),ad.Day());}
//ShowMessage("Date Error "+String(tr->Field[tmd::tdate]));


}

bool TBookKeep::Assistant(char* proccode)
{
return ((!strcmp(proccode,"0009"))||
        (!strcmp(proccode,"0008"))||
        (!strcmp(proccode,"0029")))? true: false;

}


void TBookKeep::BuildCSV(TStringList* sl)
{
String str("Type,Date,Ref,Account,Code,Description,Debit,Credit,Balance");
sl->Add(str);

str=",,,,,Brought Forward,,,"+FormatFloat("0.00",broughtforward);
sl->Add(str);

for (int i=0;i<datalist->Count ;i++)
{
char* debit="";
char* credit="";
//char* balance=NULL;
bookkeepdata* data = (bookkeepdata*)datalist->Items[i];
char* date = BookDate(data->transact,data->account);
date = USADate(date);
//char* date = USADate(((TCSVRec*)data->transact)->Field[tmd::tdate]);
char* ref =   ((TAccount*)data->account)->Field[tmd::nc];
char* accname = ((TAccount*)data->account)->Field[tmd::asname];
char* code =  ((TCSVRec*)data->transact)->Field[tmd::code];
char* descr = ((TCSVRec*)data->transact)->Field[tmd::proc];
if (!stricmp(code,"PAYM")) credit= ((TCSVRec*)data->transact)->Field[tmd::amt];
        else debit = ((TCSVRec*)data->transact)->Field[tmd::amt];
String balance = FormatFloat("0.00",data->balance);//.c_str();
//char* balance= strcpy(new char[strlen(str1)+1],str1);
//char * balance=FormatFloat("0.00",data->balance).c_str();

char* str1;
if (!stricmp(code,"PAYM"))str1=(credit[0]=='-')?"refund":"payment";
else str1=(debit[0]=='-')?"adjust":"bill";
if (Assistant(code)) str1 = "assist";

//date//ref //account//code//description //debit//credit //balance
str = String(str1)+','
    +String(date)+','
    +String(ref)+','
    +String(accname)+','
    +String(code)+','
    +String(descr)+','
    +String(debit)+','
    +String(credit)+','
    +balance;
sl->Add(str);
//delete[] balance;
}
str=",,,,,Total,"+
        FormatFloat("0.00",totalbilled)+","+
        FormatFloat("0.00",totalpay)+",";
sl->Add(str);

}
void __fastcall TBookKeep::ClipClick(TObject *Sender)
{
TStringList* SL = new TStringList;
BuildCSV(SL);
Clipboard()->AsText=SL->Text;
delete SL;
}
//---------------------------------------------------------------------------


char* TBookKeep::OldestAccountDate(TList*L)
{
char* olddate =(L->Count)? ((TAccount*)L->Items[0])-> ProcedureDate:NULL;
for (int i=1;i<L->Count;i++)
{
char* date = ((TAccount*)L->Items[i])-> ProcedureDate;
if (atoi(revdate(date))  <  atoi(revdate(olddate))) olddate=date;
if (i>50) break;
}
return olddate;  //returns the oldest acc date in first 50 accounts or NULL
                 //if the list is empty
}
