//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "bkeep.h"
#include "CSVRec.h"
#include "StrDate.h"
#include "globals.h"
#include "tmFile.h"
//#include "a4page.h"
#include <Clipbrd.hpp>
#include "main.h"
#include "a5image.h"
#include "A5Pg.h"
#include "a5base.h"

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
templist = new TList;
datalist = new TList;
prelist = new TList;
UpdateDataList();

}
//---------------------------------------------------------------------------

void __fastcall TBookKeep::FormClose(TObject *Sender, TCloseAction &Action)
{
for (int i=0;i<datalist->Count;i++) delete (bookkeepdata*)datalist->Items[i];
for (int i=0;i<prelist->Count;i++) delete (preprocdata*)prelist->Items[i];
for (int i=0;i<templist->Count;i++) delete (TCSVRec*)templist->Items[i];
delete datalist;
delete prelist;
delete templist;
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
char* amt = "";
//char* bal = NULL;


if (datalist->Count)
{
TCanvas* can = ((TListBox*)Control)->Canvas;
can->Font->Color=clBlack;
can->Brush->Color=clWhite;

bookkeepdata* data = (bookkeepdata*)datalist->Items[Index];
date = BookDate(data->transact,data->account);
date = TGlobalFunct::DelimDate(date);

ref =   ((TAccount*)data->account)->Field[tmd::nc];
accname = ((TAccount*)data->account)->Field[tmd::asname];
code =  ((TCSVRec*)data->transact)->Field[tmd::code];
descr = ((TCSVRec*)data->transact)->Field[tmd::proc];
amt = ((TCSVRec*)data->transact)->Field[tmd::amt];


int specialcode = (!stricmp(code,"PAYM"))? 1:
                  (!stricmp(code,"INT")) ? 2:
                  (!stricmp(code,"ADJ")) ? 3:
                  (!stricmp(code,"DISC"))? 4:0;

switch (specialcode)
{
case 0:  debit=amt;can->Brush->Color=clWhite;break;                   //normal fee
case 1:  if (amt[0]=='-'){ debit=&amt[1];can->Brush->Color=clYellow;}   //refund neg
         else            {credit = amt;can->Brush->Color=clAqua;}break; //payment
case 2:  debit=amt;can->Brush->Color=clMoneyGreen;break;              //interest
case 3:  credit = &amt[1];can->Brush->Color=clLime;break;          //writeoff neg
case 4:  credit= &amt[1];can->Brush->Color=clSkyBlue;break;        //discount neg
}
if (Assistant(code)) can->Brush->Color = clLtGray;

////.c_str();
////bal= strcpy(new char[strlen(str)+1],str);

String bal = FormatFloat("#,##0.00",data->balance);
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
if (!bal.IsEmpty())
can->TextOut(Rect.Left+tab8-can->TextWidth(bal),Rect.Top,bal); //600
//delete[] bal;
}

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
for (int i=0;i<templist->Count;i++) delete (TCSVRec*)templist->Items[i];
templist->Clear();
Preprocess();   //find all account that relate to this financial period
                //and place them in prelist as preprocdata items
broughtforward=0;
for (int i=0;i<prelist->Count;i++)
{
bool interdone = false;    //interest done
bool adjdone = false;      //adjustment done
//bool accfinal = false;
preprocdata* proc = (preprocdata*)prelist->Items[i];
TAccount* acc= (TAccount*)proc->account;

//char* e = NULL;
//if (i==173)
//e = acc->Field[tmd::nc];
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
int dateval=dt.Val;
int startval= DateTimePicker1->Date.Val;
int endval= DateTimePicker2->Date.Val;
double amt; try{amt= StrToFloat(tr->Field[tmd::amt]);}catch(EConvertError &e){amt=0;};
bool pay = (!stricmp(tr->Field[tmd::code],"PAYM"));
bool adj =  (!stricmp(tr->Field[tmd::code],"ADJ"));
//if (pay||adj) accfinal=true;

if (dateval<startval)      //calcuate the balance brought forward
        {
        if (pay) broughtforward+=amt;   //payment
        else broughtforward-=amt;            //refund
        }

if ((dateval >= startval) && (dateval <=  endval))  //filter only the period items into the datalist

        {
        bookkeepdata* bk = new bookkeepdata;
        bk->account=acc;
        bk->transact=tr;
        datalist->Add(bk);
        }
       //todo fix multiple interest calc
        float inter = 0; //stores interest for writeoff procedure
        //interest calc procedure  ************NB  CHECK THE POLLOWING PROCEDURES *********************
     if  (
         // (acc->VatDate)&&
         //(tagchar)&&
         //(pay||adj)&&
         //(!stricmp(bd,acc->VatDate))&&
         (strlen(acc->Field[tmd::sc])>1)||(intcheck->Checked)) //test for interest
          {
           int age = TDate::CurrentDate().Val - endval;
           char* str = acc->IntCalc(mainform->TMData->Setup->Field[tmd::interest],age);
           try{inter = StrToFloat(str);}catch(EConvertError &e){inter=0;};
           if (!interdone)
           {
           bookkeepdata* bk = new bookkeepdata;
           bk->account=acc;
           bk->transact=newtrans("INT","Interest",inter,acc,DateTimePicker2->Date.FormatString("ddmmyyyy").c_str());
           datalist->Add(bk);
           interdone = true;
           }
          }
         //writeoff calc procedure for nonactive accounts only
     double bal; try{bal= StrToFloat(acc->Balance);}catch(EConvertError &e){bal=0;};
        if ((acc->VatDate)&&(pay||adj)&&(!stricmp(bd,acc->VatDate))&&(strlen(acc->Field[tmd::tag])))
        if (((inter + bal)>0)&&(!adjdone))
        {
        bookkeepdata* bk = new bookkeepdata;
        bk->account=acc;
        bk->transact=newtrans("ADJ","Auto Writeoff",(inter + bal)*-1,acc); // writeoff transsaction
        datalist->Add(bk);
        adjdone=true;
        }
          }  //end transaction processing
          }


}
datalist->Sort(Compare); // TListSortCompare
double runningbalance=broughtforward;
double totalpfees=0;
double totalafees=0;
double totalmapay=0;
double totalppay=0;
double totalupay=0;
double totalrefund=0;
double totaladjust=0;
//double totalassist=0;
double totaldiscount=0;
double totalinterest=0;


//update the balances on the sorted list
for (int i=0;i<datalist->Count ;i++)
{
bookkeepdata *bk= (bookkeepdata*)datalist->Items[i];
TCSVRec* tr = (TCSVRec*)bk->transact;
double amt;
try{amt= StrToFloat(tr->Field[tmd::amt]);}catch(EConvertError &e){amt=0;}
char paychar = tr->Field[tmd::proc][strlen(tr->Field[tmd::proc])-1];


char* codestr = tr->Field[tmd::code];
int specialcode = (!stricmp(codestr,"PAYM"))? 1:
                  (!stricmp(codestr,"INT")) ? 2:
                  (!stricmp(codestr,"ADJ")) ? 3:
                  (!stricmp(codestr,"DISC"))? 4:0;

switch (specialcode)
{
case 0: runningbalance-=amt;
        if (Assistant(codestr)) totalafees+=amt;else totalpfees+=amt; break;//fees debit
case 1: if (amt>0 ){runningbalance+=amt;
         if (paychar=='p')totalppay+=amt;
         else if (paychar=='m')totalmapay+=amt;
         else totalupay+=amt;}      //payment credit
         else {runningbalance+=amt;totalrefund-=amt;} break;//  refund debit (negative)
case 2: runningbalance-=amt;totalinterest+=amt;break;//interest debit
case 3:  runningbalance-=amt; totaladjust-=amt; break;                        // writeoff credit (negative)
case 4:   runningbalance-=amt;totaldiscount-=amt;break; //discount credit (negative)
}
bk->balance=runningbalance;
}

totalpay=totalmapay+totalppay+totalupay;           //global totals
totalbilled=totalpfees+totalafees;        //global total

refunds->Text=FormatFloat("#,##0.00",totalrefund);
startbalance->Text=FormatFloat("#,##0.00",broughtforward);
endbalance->Text=FormatFloat("#,##0.00",runningbalance);
pfees->Text = FormatFloat("#,##0.00",totalpfees);
afees->Text = FormatFloat("#,##0.00",totalafees);
refunds->Text = FormatFloat("#,##0.00",totalrefund);
interest->Text = FormatFloat("#,##0.00",totalinterest);

mapay->Text = FormatFloat("#,##0.00",totalmapay);
ppay->Text = FormatFloat("#,##0.00",totalppay);
upay->Text = FormatFloat("#,##0.00",totalupay);
discounts->Text = FormatFloat("#,##0.00",totaldiscount);
writeoffs->Text = FormatFloat("#,##0.00",totaladjust);

totaldebits = totalbilled+totalrefund+totalinterest;
totdebits->Text = FormatFloat("#,##0.00",totaldebits);
totalcredits = totalpay+totaldiscount+totaladjust;
totcredits->Text = FormatFloat("#,##0.00",totalcredits);

ListBox1->Count = datalist->Count;//(datalist->Count)? datalist->Count+2:0;
ListBox1->Invalidate();
UpdateButton->Enabled=false;

}
__finally {Screen->Cursor = Save_Cursor;}

}

//---------------------------------------------------------------------------
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

String str1 = TGlobalFunct::revdate(date1);
String str2 = TGlobalFunct::revdate(date2);

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
UpdateButton->Enabled=true;
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
DrawLegend(discountlabel,clSkyBlue);
DrawLegend(interestlabel,clMoneyGreen);
}
//---------------------------------------------------------------------------


void __fastcall TBookKeep::DateTimePicker1Click(TObject *Sender)
{
//UpdateDataList();
}
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

//---------------------------------------------------------------------------

void __fastcall TBookKeep::PrintButtonClick(TObject *Sender)
{

TPrintDialog* pd = new TPrintDialog(this);
pd->Options<<poPrintToFile;
String str;
if (pd->Execute())
{
if (pd->PrintToFile) str= "Print to JPG File in Current Folder"; else str = "Printer: "+
                              Printer()->Printers->Strings[Printer()->PrinterIndex];
if (Application->MessageBox(str.c_str(),"Print Financial Report", MB_OKCANCEL)==IDOK)
{
TA5Base* bs = (pd->PrintToFile) ? (TA5Base*) new TA5Image:
                                   (TA5Base*)new TA5Page;
if (pd->PrintToFile) ((TA5Image*)bs)->FileName = "FinReport";
bs->NewPage();
bs->TextStyle=TA5Base::a5BoldItalic;
bs->Write(40,8,"FINANCIAL REPORT",TA5Base::RJ);
bs->TextStyle=TA5Base::a5Normal;
str=("Period Start "+ DateTimePicker1->Date.FormatString("dd MMM yyyy"));
bs->Write(40,11,str.c_str(),TA5Base::RJ);
str=("Period End "+ DateTimePicker2->Date.FormatString("dd MMM yyyy"));
bs->Write(90,11,str.c_str(),TA5Base::RJ);

str= "Balance  R "+ startbalance->Text;
bs->Write(40,13,str.c_str(),TA5Base::RJ);

str= "Balance  R "+ endbalance->Text;
bs->Write(90,13,str.c_str(),TA5Base::RJ);


bs->Write(40,20,"Debits",TA5Base::RJ);bs->Write(90,20,"Credits",TA5Base::RJ);
bs->Write(40,22,pfees->Text.c_str(),TA5Base::RJ);bs->Write(90,22,mapay->Text.c_str(),TA5Base::RJ);
bs->Write(40,23,afees->Text.c_str(),TA5Base::RJ);bs->Write(90,23,ppay->Text.c_str(),TA5Base::RJ);
bs->Write(40,24,refunds->Text.c_str(),TA5Base::RJ);bs->Write(90,24,upay->Text.c_str(),TA5Base::RJ);
bs->Write(40,25,interest->Text.c_str(),TA5Base::RJ);bs->Write(90,25,discounts->Text.c_str(),TA5Base::RJ);
                                                    bs->Write(90,26,writeoffs->Text.c_str(),TA5Base::RJ);
bs->Write(40,28,totdebits->Text.c_str(),TA5Base::RJ);bs->Write(90,28,totcredits->Text.c_str(),TA5Base::RJ);


bs->Write(25,22,"Practice Fees",TA5Base::RJ);bs->Write(75,22,"Med Aid Payments",TA5Base::RJ);
bs->Write(25,23,"Assistant Fees",TA5Base::RJ);bs->Write(75,23,"Patient Payments",TA5Base::RJ);
bs->Write(25,24,"Refunds",TA5Base::RJ);bs->Write(75,24,"Unspec. Payments",TA5Base::RJ);
bs->Write(25,25,"Interest",TA5Base::RJ);bs->Write(75,25,"Discounts",TA5Base::RJ);
                                                    bs->Write(75,26,"Writeoffs",TA5Base::RJ);
bs->Write(25,28,"Total Debits",TA5Base::RJ);bs->Write(75,28,"Total Credits",TA5Base::RJ);


bs->Finish();
delete bs;
}
}
delete pd;
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

//---------------------------------------------------------------------------
}

bool TBookKeep::Assistant(char* proccode)
{
return ((!strcmp(proccode,"0009"))||
        (!strcmp(proccode,"0008"))||
        (!strcmp(proccode,"0029")))? true: false;

}
//---------------------------------------------------------------------------

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

bookkeepdata* data = (bookkeepdata*)datalist->Items[i];
char* date = BookDate(data->transact,data->account);
date = TGlobalFunct::USADate(date);

char* ref =   ((TAccount*)data->account)->Field[tmd::nc];
char* accname = ((TAccount*)data->account)->Field[tmd::asname];
char* code =  ((TCSVRec*)data->transact)->Field[tmd::code];
char* descr = ((TCSVRec*)data->transact)->Field[tmd::proc];
char* amt =  ((TCSVRec*)data->transact)->Field[tmd::amt];

String balance = FormatFloat("0.00",data->balance);//.c_str();

int specialcode = (!stricmp(code,"PAYM"))? 1:
                  (!stricmp(code,"INT")) ? 2:
                  (!stricmp(code,"ADJ")) ? 3:
                  (!stricmp(code,"DISC"))? 4:0;
char* ttype;
switch (specialcode)
{
case 0:  debit=amt;ttype = "bill";break;                   //normal fee
case 1:  if (amt[0]=='-'){debit=&amt[1];ttype = "refund";}   //refund neg
         else credit = amt;ttype="payment";break;         //payment
case 2:  debit=amt;ttype="interest";break;                 //interest
case 3:  credit = &amt[1];ttype="writeoff";break;          //writeoff neg
case 4:  credit= &amt[1];ttype="discount";break;           //discount neg
}
if (Assistant(code))ttype = "assist";
//date//ref //account//code//description //debit//credit //balance
str = String(ttype)+','
    +String(date)+','
    +String(ref)+','
    +String(accname)+','
    +String(code)+','
    +String(descr)+','
    +String(debit)+','
    +String(credit)+','
    +balance;
sl->Add(str);

}
str=",,,,,Total,"+
        FormatFloat("0.00",totaldebits)+","+     //global value
        FormatFloat("0.00",totalcredits)+",";    //global value from updatelist
sl->Add(str);

}

//---------------------------------------------------------------------------
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
if (atoi(TGlobalFunct::revdate(date))  <  atoi(TGlobalFunct::revdate(olddate))) olddate=date;
if (i>50) break;
}
return olddate;  //returns the oldest acc date in first 50 accounts or NULL
                 //if the list is empty
}

//---------------------------------------------------------------------------
int TBookKeep::AccountType(void* a)
{
// 0 does not count - written off before period  vatdate <startperiod or procedure date after endperiod
// 1 written off during period  vatdate=> startperiod && =< endpereiod  or no vatdate and tag!='';
// 2 written off after period vatdate >endperiod
// or  not written off yet      no vatdate tag="";
TAccount* acc = (TAccount*)a;
char* vd = acc->VatDate;
char* pd = acc->ProcedureDate;
int tg = strlen(acc->Field[tmd::tag]);
TDate pdd = TDate(TStrDate(pd).Year(),TStrDate(pd).Month(),TStrDate(pd).Day());
TDate vdd = (vd)? TDate(TStrDate(vd).Year(),TStrDate(vd).Month(),TStrDate(vd).Day()):pdd;
TDate start = DateTimePicker1->Date;
TDate end = DateTimePicker2->Date;
if ((pdd.Val>end.Val)||(vdd.Val<start.Val)) return 0;   //only if paid after beginning of period and
                                                        //started before the end of the period
if ((vdd.Val>=start)&&(pdd.Val<=end)&&(tg)) return 1;   //if paid during the period then special processing reqd
return 2;                                               //eg interest and writeoffs

}
//---------------------------------------------------------------------------
void* TBookKeep::newtrans(char* code,char* desc,float f,void* acc,char* dt) //makes a temp transaction (templist permits final deletion)
{
char* str= new char[10];
sprintf(str,"%9.2f",f);
TCSVRec* cs = new TCSVRec;
TAccount* ac= (TAccount*)acc;
char* date = ac->VatDate;
if (!date) date = ac->ProcedureDate;
if (dt) date = dt;
cs->FieldCount=tmd::transcnt;
cs->Field[tmd::tdate]=date;
cs->Field[tmd::code]=code;
cs->Field[tmd::proc]=desc;
cs->Field[tmd::amt] = String(str).Trim().c_str();
//tid,tdate,code,proc,amt
delete[] str;
templist->Add(cs);
return cs;
}

//---------------------------------------------------------------------------
void TBookKeep::Preprocess(void)
{
for (int i=0;i<prelist->Count;i++) delete  (preprocdata*)prelist->Items[i];
prelist->Clear();
for (int i=0;i<allList->Count;i++)
{
char bits =0;
TAccount* acc= (TAccount*)allList->Items[i];
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
        int dateval=dt.Val;
        int startval= DateTimePicker1->Date.Val;
        int endval= DateTimePicker2->Date.Val;

bits |= (dateval<startval)? 0x01:
        ((dateval >= startval)&&(dateval <=  endval))? 0x02:
        (dateval>endval)? 0x04:0x00;
       } }
bool incomplete = !strlen(acc->Field[tmd::tag]);//filter accounts that relate to period
if (((bits & 0x03 )==  0x03)||                  //before and during
    ((bits & 0x02) == 0x02)||                   //during
    ((bits & 0x05) == 0x05)||                   //before and after
    ((bits & 0x06)== 0x06)||                    //during and after
    (((bits & 0x01)== 0x01) & incomplete)||      //before and incomplete
    (((bits & 0x02)== 0x02) & incomplete))       //during and incomplete

{
preprocdata* proc = new preprocdata;
proc->flag=bits;
proc->account = acc;
prelist->Add(proc);
}
}
}
//---------------------------------------------------------------------------
void __fastcall TBookKeep::intcheckClick(TObject *Sender)
{
UpdateButton->Enabled=true;       
}
//---------------------------------------------------------------------------





