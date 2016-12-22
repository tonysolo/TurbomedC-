//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Report.h"
#include "strdate.h"
#include "stdio.h"
#include "main.h"
#include "globals.h"
#include <math.h>
#include <stdlib.h>
#include <Clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//TReportForm *ReportForm;
//---------------------------------------------------------------------------
__fastcall TReportForm::TReportForm(TComponent* Owner)
        : TForm(Owner)
{
DisplaySL=new TStringList;
DisplayData=NULL;
}
//---------------------------------------------------------------------------


void __fastcall TReportForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
delete RangeList;
delete Select;
}
//---------------------------------------------------------------------------

void __fastcall TReportForm::FormCreate(TObject *Sender)
{
reversesort=false;
ListView1->SortType=Comctrls::stNone;
RangeList = new TList;
UpdateRange();
Select = new TSelection();
RadioGroup2 -> ItemIndex =0;
TAccount* acc = (TAccount*)mainform->TMData->Patients->First();
char* dt = acc->ProcedureDate;
DateTimePicker1->MinDate =
TDateTime(TStrDate(dt).Year(),TStrDate(dt).Month(),TStrDate(dt).Day());

DateTimePicker2->MinDate=  DateTimePicker1->MinDate;

DateTimePicker1->MaxDate = TDateTime().CurrentDate();

DateTimePicker2->MaxDate=  DateTimePicker1->MaxDate;

DateTimePicker1->Date = DateTimePicker1->MinDate;
DateTimePicker2->Date = DateTimePicker2->MaxDate;

RadioGroup1Click(this);

}
//---------------------------------------------------------------------------


void __fastcall TReportForm::RadioGroup1Click(TObject *Sender)
{
ListView1->SortType=Comctrls::stNone; // shows totals and list will not be sorted
//sortfield=0;
reversesort=false;
ResetColumns(RadioGroup1->Items->Strings[RadioGroup1->ItemIndex]);
ShowTotals=true;
ResetReport();
RadioGroup2->Enabled=RangeRadio->Checked;
}
//---------------------------------------------------------------------------


void TReportForm::ResetColumns(String s)
{

sortfield=0;
ListView1->Columns->Clear();
TListColumn  *NewColumn;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = s;
  ListCaption = NewColumn->Caption.c_str();
  NewColumn->Width = 100;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "#";
  NewColumn->Width=40;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Billed";
  NewColumn->Width=80;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "%Billed";
  NewColumn->Width=50;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Paid";
  NewColumn->Width=80;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Balance";
  NewColumn->Width=80;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Time";
  NewColumn->Width=50;
  NewColumn->Alignment=taRightJustify;

}


void TReportForm::ResetReport()
{
int rangetype;
if (RangeRadio->Checked) rangetype=0;
else if(AllAccounts->Checked) rangetype=1;
    else rangetype=2;

switch (rangetype)
{
case 0:
ReportList = RangeList;
RFolder->Caption="Date Range";
break;
case 1:
ReportList = mainform->TMData->Patients;
RFolder->Caption="All Accounts";
break;
case 2:
ReportList = Select->List;
RFolder->Caption=mainform->FolderName();
break;
}

RNumber->Caption=IntToStr(ReportList->Count);
RFilter->Caption=(AllAccounts->Checked) ? String("None"):
mainform->ToolButton4->Caption;
bool libflag;
switch (RadioGroup1->ItemIndex)
{

case 0:ExDiscountReport();break;
case 1:MAReport();break;
case 2:ClinicReport();break;
case 3:ReferralReport();break;
case 4:DOWReport();break;
case 5:MonthReport();break;
case 6:DiagnosisReport();break;
case 7:libflag=false;ProceduresReport(libflag);break;
case 8:libflag=true;ProceduresReport(libflag);break;

}
}




void TReportForm::MAReport()
{

DisplaySL->Clear();
DisplaySL->Sorted=true;
DisplaySL->Duplicates = dupIgnore;
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
DisplaySL->Add(A->Field[tmd::ma]);
}
ResetDat();
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
int j= DisplaySL->IndexOf(A->Field[tmd::ma]);
DisplayData[j].count++;
DisplayData[j].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[j].balance+=atof(A->Balance);
DisplayData[j].value+=(applydisc->Checked)?atof(A->Billed)*ratio:
                                   atof(A->Billed);
DisplayData[j].time+=A->AnaesTime();

}
ListReport();

}


void TReportForm::ClinicReport()
{
//TStringList *DisplaySL = new TStringList;
DisplaySL->Clear();
DisplaySL->Sorted=true;
DisplaySL->Duplicates = dupIgnore;
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
DisplaySL->Add(A->Field[tmd::clin]);
}
ResetDat();

for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
int j= DisplaySL->IndexOf(A->Field[tmd::clin]);
DisplayData[j].count++;
DisplayData[j].payment+=atof(A->Payments);
DisplayData[j].value+=(applydisc->Checked)?atof(A->Billed)*ratio:
                                   atof(A->Billed);
if (!applydisc->Checked) DisplayData[j].balance+=atof(A->Balance);

DisplayData[j].time+=A->AnaesTime();
}
ListReport();
}


void TReportForm::ReferralReport()
{
DisplaySL->Clear();
DisplaySL->Sorted=true;
DisplaySL->Duplicates = dupIgnore;
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
DisplaySL->Add(A->Field[tmd::ref]);
}
ResetDat();

for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
int j= DisplaySL->IndexOf(A->Field[tmd::ref]);
DisplayData[j].count++;
DisplayData[j].payment+=atof(A->Payments);
DisplayData[j].value+=(applydisc->Checked)?atof(A->Billed)*ratio:
                                   atof(A->Billed);
if (!applydisc->Checked)DisplayData[j].balance+=atof(A->Balance);

DisplayData[j].time+=A->AnaesTime();
}
ListReport();

}


void TReportForm::DOWReport()
{
DisplaySL->Clear();
DisplaySL->Sorted=false;
DisplaySL->Add("Sunday");
DisplaySL->Add("Monday");
DisplaySL->Add("Tuesday");
DisplaySL->Add("Wednesday");
DisplaySL->Add("Thursday");
DisplaySL->Add("Friday");
DisplaySL->Add("Saturday");

ResetDat();
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
TList * L = A-> Trans;
TCSVRec*R = (L->Count)?(TCSVRec*)L->Items[0]:NULL;
char* s = (R)? R->Field[tmd::tdate]:A->Field[tmd::dt];
if (strlen(s)!=8) s = A->Field[tmd::dt];
TStrDate d(s);
TDateTime accdate(d.Year(),d.Month(),d.Day());
int j= accdate.DayOfWeek()-1;
DisplayData[j].count++;
DisplayData[j].payment+=atof(A->Payments);
DisplayData[j].value+=(applydisc->Checked)?atof(A->Billed)*ratio:
                                   atof(A->Billed);
if (!applydisc->Checked)DisplayData[j].balance+=atof(A->Balance);

DisplayData[j].time+=A->AnaesTime();
}

 ListReport();

}




void TReportForm::MonthReport()
{
DisplaySL->Clear();
DisplaySL->Sorted=true;
DisplaySL->Duplicates = dupIgnore;

//DisplaySL->Clear();

for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
TList * L = A-> Trans;
TCSVRec*R = (L->Count)? (TCSVRec*)L->Items[0]:NULL;
char* s = (R)? R->Field[tmd::tdate]:A->Field[tmd::dt];
if (strlen(s)!=8) s = A->Field[tmd::dt];
DisplaySL->Add(yyyymm(s));
}
ResetDat();

for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
TList * L = A-> Trans;
TCSVRec*R = (L->Count)? (TCSVRec*)L->Items[0]:NULL;
char* s = (R)? R->Field[tmd::tdate]:A->Field[tmd::dt];
if (strlen(s)!=8) s = A->Field[tmd::dt];


int j= DisplaySL->IndexOf(yyyymm(s));
DisplayData[j].count++;
DisplayData[j].payment+=atof(A->Payments);
DisplayData[j].value+=(applydisc->Checked)?atof(A->Billed)*ratio:
                                   atof(A->Billed);
if (!applydisc->Checked)DisplayData[j].balance+=atof(A->Balance);
DisplayData[j].time+=A->AnaesTime();
}

DisplaySL->Sorted=false;
for (int i=0;i<DisplaySL->Count;i++)
DisplaySL->Strings[i]=MonthYearStr(DisplaySL->Strings[i].c_str());
ListReport();

}


void TReportForm::DiagnosisReport()
{

DisplaySL->Clear();
DisplaySL->Sorted=true;
DisplaySL->Duplicates = dupIgnore;
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
DisplaySL->Add(A->Field[tmd::diag]);
}
ResetDat();

for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
int j= DisplaySL->IndexOf(A->Field[tmd::diag]);
DisplayData[j].count++;
DisplayData[j].payment+=atof(A->Payments);
DisplayData[j].value+=(applydisc->Checked)?atof(A->Billed)*ratio:
                                   atof(A->Billed);
if (!applydisc->Checked)DisplayData[j].balance+=atof(A->Balance);
DisplayData[j].time+=A->AnaesTime();
}
ListReport();

}


void TReportForm::ProceduresReport(bool l)
{

DisplaySL->Clear();
DisplaySL->Sorted=true;
DisplaySL->Duplicates = dupIgnore;
for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
TList *T= A->Trans;
   for(int j=0;j<T->Count;j++)
   {
   TCSVRec *rec = (TCSVRec*) T->Items[j];
   char * test = rec->Field[tmd::code];
   bool tst = ((strcmp(test,"PAYM"))&&
       (strcmp(test,"DISC"))&&
       (strcmp(test,"ADJ")));
   if (tst) DisplaySL->Add(rec->Field[tmd::code]);
   }
}
ResetDat();

for (int i=0;i<ReportList->Count;i++)
{
TAccount*A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
TList *T= A->Trans;
   for(int j=0;j<T->Count;j++)
   {
   TCSVRec *rec = (TCSVRec*)T->Items[j];

   int k = DisplaySL->IndexOf(rec->Field[tmd::code]);
   if (k >-1)
   {
   DisplayData[k].count++;
   DisplayData[k].value+=(applydisc->Checked)?atof(rec->Field[tmd::amt])*ratio:
                                   atof(rec->Field[tmd::amt]);
   }
   }
}
if (l)
{
TCSVRec *R;
DisplaySL->Sorted=false;
for (int i=0;i<DisplaySL->Count;i++)
{
R = mainform->TMData->
LibLookup(DisplaySL->Strings[i].c_str());
if (R) DisplaySL->Strings[i]= R->Field[tmd::leng];
}
}
ListReport();
}


void TReportForm::ListReport()
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try{
String str;
double totalbil=0;
double totalpay=0;
double totalbal=0;
double grandbil=0;
int totaltime=0;
int totalnum=0;
TListItem *ListItem;
ListView1->Items->Clear();
ListView1->Hide();
for (int i=0;i<DisplaySL->Count;i++)
grandbil += DisplayData[i].value;

for (int i=0;i<DisplaySL->Count;i++)
{

ListItem = ListView1->Items->Add();

    ListItem->Caption = (!DisplaySL->Strings[i].IsEmpty())?
    DisplaySL->Strings[i]:String("  <no name>");

   totalnum+=DisplayData[i].count;
   str=IntToStr(DisplayData[i].count);
   ListItem->SubItems->Add(str); //subitem 0 count


        totalbil+=DisplayData[i].value;
    str=FormatFloat("#,##0.00",DisplayData[i].value);
    ListItem->SubItems->Add(str);   //subitem 1 value

   if (grandbil>0)
    str=FormatFloat("##0.0",DisplayData[i].value/grandbil*100);
   else str=FormatFloat("##0.00",0.0);
    ListItem->SubItems->Add(str);  //subitem 2 % total billed

    totalpay+=DisplayData[i].payment;

    str=FormatFloat("#,##0.00",DisplayData[i].payment);

//if (rd[i].payment>0)
    ListItem->SubItems->Add(str);
//else  ListItem->SubItems->Add(FormatFloat("##0.0",0.0));//subitem 3 payment

   totalbal+=DisplayData[i].balance;
    //sprintf(str,"%10.2f",rd[i].balance);
    str=FormatFloat("#,##0.00",DisplayData[i].balance);

//if (rd[i].balance>0)
    ListItem->SubItems->Add(str);
//else  ListItem->SubItems->Add(FormatFloat("##0.0",0.0));//subitem 4 balance

 totaltime+=DisplayData[i].time;
  if (DisplayData[i].time) str=IntToStr(DisplayData[i].time); else str="";
  ListItem->SubItems->Add(str);
}

//if (ListView1->SortType == Comctrls::stNone)
if (ShowTotals)
{
ListItem = ListView1->Items->Add();
    ListItem->Caption = "";
ListItem = ListView1->Items->Add();
    ListItem->Caption = "Total";
//sprintf(str,"%5d",totalnum);
str=IntToStr(totalnum);
    ListItem->SubItems->Add(str);
//sprintf(str,"%10.2f",totalbil);
str=FormatFloat("#,##0.00",totalbil);
    ListItem->SubItems->Add(str);
    ListItem->SubItems->Add("100.0");
//sprintf(str,"%10.2f",totalpay);//Select->Payment);
str=FormatFloat("#,##0.00",totalpay);
if (totalpay>0) ListItem->SubItems->Add(str);
else ListItem->SubItems->Add("");
//sprintf(str,"%10.2f",totalbal);//Select->Balance);
str=FormatFloat("#,##0.00",totalbal);
if (totalbal>0) ListItem->SubItems->Add(str);
else ListItem->SubItems->Add("");

str=IntToStr(totaltime);
if (totaltime) ListItem->SubItems->Add(str);
else ListItem->SubItems->Add("");
}
ListView1->Show();
}
__finally {Screen->Cursor = Save_Cursor;}

}


void __fastcall TReportForm::PrintClick(TObject *Sender)
{
char *str = new char[200];
char *str1;
TA5Page *pg = new TA5Page();
if (Application->MessageBox("Continue Printing ?","Print Report",
MB_OKCANCEL)==IDOK)
{
pg->NewPage();
pg->Heading="Report";
pg->Write(2,1,"TURBOMED REPORT",TA5Page::LJ);
pg->Write(40,1,TDateTime::CurrentDate().FormatString("dd mmm yyyy").
c_str(),TA5Page::LJ);
pg->Write(70,1,"Folder :",TA5Page::RJ);
pg->Write(72,1,mainform->FolderName(),TA5Page::LJ);
pg->Separator(2);
str1=RadioGroup1->Items->Strings[RadioGroup1->ItemIndex].c_str();
pg->Write(2,3,str1,TA5Page::LJ);
pg->Write(35,3,"#",TA5Page::RJ);
pg->Write(50,3,"BILLED",TA5Page::RJ);
pg->Write(65,3,"%BILLED",TA5Page::RJ);
pg->Write(80,3,"PAID",TA5Page::RJ);
pg->Write(95,3,"BALANCE",TA5Page::RJ);
pg->NextLine(4);

int n;
double totalbil=0;
double totalpay=0;
double totalbal=0;
double grandbil=0;
int totalnum=0;
for (int i=0;i<DisplaySL->Count;i++)
grandbil += DisplayData[i].value;

for (int i=0;i<DisplaySL->Count;i++)
{
n = pg->NextLine(0);
str1 = (!DisplaySL->Strings[i].IsEmpty())? DisplaySL->Strings[i].c_str():"<no name>";
pg->Write(2,n,str1,TA5Page::LJ);

totalnum+=DisplayData[i].count;
sprintf(str,"%5d",DisplayData[i].count);
pg->Write(35,n,str,TA5Page::RJ);

totalbil+=DisplayData[i].value;
sprintf(str,"%10.2f",DisplayData[i].value);
pg->Write(50,n,str,TA5Page::RJ);

if(grandbil>0)
sprintf(str,"%4.1f",DisplayData[i].value/grandbil*100);
else strcpy(str,"0.0");
pg->Write(65,n,str,TA5Page::RJ);

totalpay+=DisplayData[i].payment;
sprintf(str,"%10.2f",DisplayData[i].payment);
pg->Write(80,n,str,TA5Page::RJ);

totalbal+=DisplayData[i].balance;
sprintf(str,"%10.2f",DisplayData[i].balance);
pg->Write(95,n,str,TA5Page::RJ);
}

pg->Separator(pg->NextLine(0));
n = pg->NextLine(0);
pg->Write(2,n,"TOTAL",TA5Page::LJ);

sprintf(str,"%5d",totalnum);
pg->Write(35,n,str,TA5Page::RJ);

sprintf(str,"%10.2f",totalbil);
pg->Write(50,n,str,TA5Page::RJ);

pg->Write(65,n,"100.0",TA5Page::RJ);

sprintf(str,"%10.2f",totalpay);
pg->Write(80,n,str,TA5Page::RJ);

sprintf(str,"%10.2f",totalbal);
pg->Write(95,n,str,TA5Page::RJ);

pg->Separator(pg->NextLine(0));
pg->Finish();
}
delete pg;
delete[] str;
}
//---------------------------------------------------------------------------


void __fastcall TReportForm::FileClick(TObject *Sender)
{

TStringList *FS = new TStringList;
BuildCSV(FS);

TSaveDialog* D = new TSaveDialog(this);
//D->FileName = "Export";
D->Title = "Report";
D->FileName = mainform->TMData->newreportname();
D->Filter = "CSV File (*.csv)|*.CSV";
D->DefaultExt = String("csv");

if (D->Execute()) FS->SaveToFile(D->FileName);
delete D;
delete FS;
}
//---------------------------------------------------------------------------



void TReportForm::ExDiscountReport()
{
//TStringList *DisplaySL = new TStringList;
DisplaySL->Clear();
DisplaySL->Sorted=false;
DisplaySL->Add("Current");
DisplaySL->Add("30 Days");
DisplaySL->Add("60 Days");
DisplaySL->Add("90 Days");
DisplaySL->Add("120 Days");
DisplaySL->Add("150 Days");
DisplaySL->Add("180 Days");
DisplaySL->Add(">210 Days");
ResetDat();
TAccount* A;
for (int i=0;i<ReportList->Count;i++)
{
A = (TAccount*)ReportList->Items[i];
double ratio = DiscountRatio(A);
TList * L = A-> Trans;
TCSVRec*R =(L->Count)? (TCSVRec*)L->Items[0]:NULL;
char* s = (R) ? R->Field[tmd::tdate]:A->Field[tmd::dt];
if (strlen(s)!=8) s = A->Field[tmd::dt];
TStrDate d(s);
int days = d.Age();
if (days<31)
{
DisplayData[0].count++;
DisplayData[0].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[0].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[0].balance+=atof(A->Balance);
DisplayData[0].time+=A->AnaesTime();
}
if ((days>30)&&(days<61))
{
DisplayData[1].count++;
DisplayData[1].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[1].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[1].balance+=atof(A->Balance);
DisplayData[1].time+=A->AnaesTime();
}

if ((days>60)&&(days<91))
{
DisplayData[2].count++;
DisplayData[2].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[2].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[2].balance+=atof(A->Balance);
DisplayData[2].time+=A->AnaesTime();
}

if ((days>90)&&(days<121))
{
DisplayData[3].count++;
DisplayData[3].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[3].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[3].balance+=atof(A->Balance);
DisplayData[3].time+=A->AnaesTime();
}

if ((days>120)&&(days<151))
{
DisplayData[4].count++;
DisplayData[4].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[4].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[4].balance+=atof(A->Balance);
DisplayData[4].time+=A->AnaesTime();
}
if ((days>150)&&(days<181))
{
DisplayData[5].count++;
DisplayData[5].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);

DisplayData[5].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[5].balance+=atof(A->Balance);
DisplayData[5].time+=A->AnaesTime();
}
if ((days>180)&&(days<211))
{
DisplayData[6].count++;
DisplayData[6].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[6].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[6].balance+=atof(A->Balance);
DisplayData[6].time+=A->AnaesTime();
}

if (days>210)
{
DisplayData[7].count++;
DisplayData[7].value+= (applydisc->Checked) ? atof(A->Billed) * ratio
                                      :atof(A->Billed);
DisplayData[7].payment+=atof(A->Payments);
if (!applydisc->Checked)DisplayData[7].balance+=atof(A->Balance);
DisplayData[7].time+=A->AnaesTime();
}
}
ListReport();
}


double TReportForm::DiscountRatio(TAccount* a)
{
int disc = (a->Field[tmd::sc][0]=='D')? atoi(a->Field[tmd::dsc]):0;
return (double)(100-disc)/100;
}

void __fastcall TReportForm::RangeRadioClick(TObject *Sender)
{
UpdateRange();
ResetReport();
RadioGroup2->Enabled=RangeRadio->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TReportForm::DateTimePicker1Exit(TObject *Sender)
{
reversesort=false;
RangeRadio->Checked=true;
RangeRadioClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TReportForm::ListView1Compare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
if ((sortfield>0)&&(sortfield<7))
{
Compare = NumberStrCmp(Item2->SubItems->Strings[sortfield-1],Item1->SubItems->Strings[sortfield-1]);
       if (reversesort) Compare*=-1;
}
}
//---------------------------------------------------------------------------

void __fastcall TReportForm::ListView1ColumnClick(TObject *Sender,
      TListColumn *Column)
//hides totals and sorts list will be sorted according to data
{
if (Column->Index==0) RadioGroup1Click(this);
else
{
//if (ListView1->SortType==Comctrls::stNone) //if this is the first time thru
if (ShowTotals)
{
ListView1->Clear();
ListView1->SortType=Comctrls::stData; //turn off sorting
ShowTotals=false;  //turn off display of totals
ResetReport();  //and reset report list
} // sortfield=0;

if (Column->Index == sortfield) reversesort = !reversesort;
sortfield=Column->Index;
ListView1->AlphaSort();
}
reversesort=false;
}
//---------------------------------------------------------------------------


int TReportForm::NumberStrCmp(String stra, String strb)
{
int compare=0;
String str1 = (stra.IsEmpty())? String("0.00"):stra;
String str2 = (strb.IsEmpty())? String("0.00"):strb;
double x,y;
try {x=StrToFloat(StripStr(str1));}
catch (EConvertError &e){x=0;}
try {y=StrToFloat(StripStr(str2));}
catch (EConvertError &e){y=0;}
if (x<y) compare=-1;
else if (x>y) compare=1;
return compare;
}

String TReportForm::StripStr(String str1)
{
char* str= str1.c_str();
String str2("");
for (unsigned i=0;i<strlen(str);i++)
if (str[i] != ',') str2+=str[i];
return str2.Trim();
}

void TReportForm::UpdateRange(void)
{
RangeList->Clear();

for (int i=0; i< mainform->TMData->Patients->Count;i++)
{
TAccount* acc = (TAccount*)mainform->TMData->Patients->Items[i];
char tagchar =  acc->Field[tmd::tag][0];

if (tagchar != 'Z')//&&(tagchar != 'I'))
{
char* dt= (RadioGroup2->ItemIndex)?
acc->ProcedureDate:acc->Field[tmd::dt];
TDate date1(TStrDate(dt).Year(),TStrDate(dt).Month(),TStrDate(dt).Day());

if(((int)date1.Val >= ((int)DateTimePicker1->Date.Val)) &&
   ((int)date1.Val <= ((int)DateTimePicker2->Date.Val)))
RangeList->Add(acc);
}
}
}

//void __fastcall TReportForm::RadioGroup2Click(TObject *Sender)
//{
//UpdateRange();
//}
//---------------------------------------------------------------------------


void TReportForm::BuildCSV(TStringList* FS)
{
double grandbil=0;
FS->Clear();
FS->Add(RadioGroup1->Items->Strings[RadioGroup1->ItemIndex]+","+
"#"+","+
"Billed"+","+
"%"+","+
"Paid"+","+
"Balance"+","+
"Time");

String title,num,bil,percent,pay,bala,antime;
for (int i=0;i<DisplaySL->Count;i++)
grandbil += DisplayData[i].value;

for (int i=0;i<DisplaySL->Count;i++)
{
title = (!DisplaySL->Strings[i].IsEmpty())? DisplaySL->Strings[i]:String("<no name>");
num = DisplayData[i].count;
bil = FormatFloat("0.00",DisplayData[i].value);
if (grandbil>0)
percent= FormatFloat("0.0",DisplayData[i].value/grandbil*100);
else percent="0.0";
pay = FormatFloat("0.00",DisplayData[i].payment);
bala = FormatFloat("0.00",DisplayData[i].balance);
antime = (DisplayData[i].time)?IntToStr(DisplayData[i].time):String("");
FS->Add(title+","+num+","+bil+","+percent+","+pay+","+bala+","+antime);
}
}




void TReportForm::ResetDat()
{
delete[] DisplayData;
DisplayData = new reportdata[DisplaySL->Count];
memset(DisplayData,0,sizeof(reportdata)*DisplaySL->Count);
}



void __fastcall TReportForm::ClipClick(TObject *Sender)
{
TStringList *FS = new TStringList;
BuildCSV(FS);
Clipboard()->AsText=FS->Text;
delete FS;
}
//---------------------------------------------------------------------------


 __fastcall TReportForm::~TReportForm()
{
delete DisplayData;
}
