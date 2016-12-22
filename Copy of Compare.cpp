//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Compare.h"
#include "CSVRec.h"
#include <Clipbrd.hpp>
#include <stdio.h>
#include "main.h"
#include "a4page.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCompareDlg *CompareDlg;
//---------------------------------------------------------------------------
__fastcall TCompareDlg::TCompareDlg(TComponent* Owner)
        : TForm(Owner)
{
CompStrList = new TStringList;
CompStrList->Sorted=true;
CompStrList->Duplicates=dupIgnore;
}
//---------------------------------------------------------------------------


__fastcall TCompareDlg::~TCompareDlg()
{
for (int i=0;i<CompStrList->Count;i++)
delete (CompData*) CompStrList->Objects[i];
delete CompStrList;
        //TODO: Add your source code here
}



void TCompareDlg::BuildComparison(TList* CurrL, TList* CompL)
{
CompStrList->Clear();

for (int i=0;i<CurrL->Count;i++)
{
TAccount* acc= (TAccount*)CurrL->Items[i];
char tg = acc->Field[tmd::tag][0];
if ((tg!='I')&&(tg!='Z')) //just in case the current list has unsaved deleted data
{
int n = CompStrList->AddObject(acc->Field[tmd::nc],(TObject*)new CompData);
((CompData*)CompStrList->Objects[n])->curr=CurrL->Items[i];
((CompData*)CompStrList->Objects[n])->comp=NULL;

}
}



for (int i=0;i<CompL->Count;i++)
{
TAccount* acc= (TAccount*)CompL->Items[i];
int n;
if (!CompStrList->Find(acc->Field[tmd::nc],n))
      {
          n = CompStrList->AddObject(acc->Field[tmd::nc],(TObject*)new CompData);
          ((CompData*)CompStrList->Objects[n])->curr = NULL;
        }

 //initialise curr to null in new additions to list
     ((CompData*)CompStrList->Objects[n])->comp=CompL->Items[i];

// either way place a pointer to the data
}

//now only keep the different accounts
for (int i=0;i<CompStrList->Count;i++)
{
CompData* dta = (CompData*)CompStrList->Objects[i];
if (!Different(dta)) {delete (dta);CompStrList->Objects[i]=NULL;}
}
int i =0;
while (i <CompStrList->Count)
if (CompStrList->Objects[i] == NULL) CompStrList->Delete(i); else i++;


//display list
//i = CompStrList->Count;
ListBox1->Count=CompStrList->Count;
ListBox1->Invalidate();
}

//pid,nc,lang,tag,pname,pbday,psex,
//atitle,ainit,asname,ad1,ad2,ad3,ad4,
//ma,man,dsc,clin,ref,sc,dt,diag,usm,auth,patid,ioddt,ddys,filenum

int TCompareDlg::Different(CompData* dta)
{
int x=0;
TAccount* acc1 = (TAccount*)dta->curr;
TAccount* acc2 = (TAccount*)dta->comp;
if ((acc1)&&(acc2))
{
if (strcmp(acc1->Field[tmd::asname],acc2->Field[tmd::asname])) x |= 1<<2;
if (strcmp(acc1->Field[tmd::tag],acc2->Field[tmd::tag])) x |= 1<<3;
if (strcmp(acc1->Payments,acc2->Payments))x |= 1<<4;
if (strcmp(acc1->Balance,acc2->Balance))x |= 1<<5;
} else x=1;

return x;
}


void __fastcall TCompareDlg::ListBox1DrawItem(TWinControl *Control,
      int Index, TRect &Rect, TOwnerDrawState State)
{
int tab1=HeaderControl1->Sections->Items[0]->Width;
int tab2=tab1+HeaderControl1->Sections->Items[1]->Width;
int tab3=tab2+HeaderControl1->Sections->Items[2]->Width;
int tab4=tab3+HeaderControl1->Sections->Items[3]->Width;
int tab5=tab4+HeaderControl1->Sections->Items[4]->Width;
int tab6=tab5+HeaderControl1->Sections->Items[5]->Width;
int tab7=tab6+HeaderControl1->Sections->Items[6]->Width;
int tab8=tab7+HeaderControl1->Sections->Items[7]->Width;
int tab9=tab8+HeaderControl1->Sections->Items[8]->Width;

TCanvas* can = ((TListBox*)Control)->Canvas;
can->Font->Color=clBlack;
can->Brush->Color=clWhite;

CompData* dta= (CompData*)CompStrList->Objects[Index];
int dif = Different(dta);
TAccount* acc1 = (TAccount*)dta->curr;
TAccount* acc2 = (TAccount*)dta->comp;
can->FillRect(Rect);
if (dif==1) can->Brush->Color=clLtGray; else can->Brush->Color=clWhite;
can->TextOut(Rect.Left,Rect.Top,CompStrList->Strings[Index]);
char* str;
char tg;
if (acc1)
{
//tg = acc1->Field[tmd::tag][0];
//if (tg=='V') str="VAT";
//else if (tg=='D') str="Archive";
//else str="Active";
if (dif & 1<< 2) can->Brush->Color=clLtGray; else can->Brush->Color=clWhite;
can->TextOut(Rect.Left+tab1,Rect.Top,acc1->Field[tmd::asname]);
if (dif& 1 << 3) can->Brush->Color=clLtGray; else can->Brush->Color=clWhite;
can->TextOut(Rect.Left+tab2,Rect.Top,FolderName(acc1));
if (dif&1 << 4) can->Brush->Color=clAqua; else can->Brush->Color=clWhite;
can->TextOut(Rect.Left+tab4-can->TextWidth(acc1->Payments)-4,Rect.Top,acc1->Payments);
if (dif&1 << 5) can->Brush->Color=clAqua; else can->Brush->Color=clWhite;
can->TextOut(Rect.Left+tab5-can->TextWidth(acc1->Balance)-4,Rect.Top,acc1->Balance);
}

if (acc2)
{
can->Brush->Color=clWhite;
//tg = acc2->Field[tmd::tag][0];
//if (tg=='V') str="VAT";
//else if (tg=='D') str="Archive";
//else str="Active";

can->TextOut(Rect.Left+tab5,Rect.Top,acc2->Field[tmd::asname]);
can->TextOut(Rect.Left+tab6,Rect.Top,FolderName(acc2));
can->TextOut(Rect.Left+tab8-can->TextWidth(acc2->Payments)-4,Rect.Top,acc2->Payments);
can->TextOut(Rect.Left+tab9-can->TextWidth(acc2->Balance)-4,Rect.Top,acc2->Balance);
}
}
//---------------------------------------------------------------------------

void __fastcall TCompareDlg::SpeedButton1Click(TObject *Sender)
{
TStringList* SL = new TStringList;
BuildCSV(SL);
Clipboard()->AsText=SL->Text;
delete SL;
}
//---------------------------------------------------------------------------

void __fastcall TCompareDlg::FileSaveButtonClick(TObject *Sender)
{
TStringList* SL = new TStringList;
BuildCSV(SL);

TSaveDialog* D = new TSaveDialog(this);
D->Title = "Report";
D->FileName = mainform->TMData->newreportname();
D->Filter = "CSV File (*.csv)|*.CSV";
D->DefaultExt = String("csv");

if (D->Execute()) SL->SaveToFile(D->FileName);
delete D;
delete SL;
}
//---------------------------------------------------------------------------


void TCompareDlg::BuildCSV(TStringList* SL)
{
String str("Account,Current Details,Folder,Payments,Balance,Compare Details,Folder,Payments,Balance");
SL->Add(str);
for (int i=0;i<CompStrList->Count;i++)
{
CompData* dta= (CompData*)CompStrList->Objects[i];
TAccount* acc1 = (TAccount*)dta->curr;
TAccount* acc2 = (TAccount*)dta->comp;
str=CompStrList->Strings[i];
str+=',';
if (acc1)
{
str+=acc1->Field[tmd::asname];
str+=',';
str+=FolderName(acc1);
str+=',';
str+=acc1->Payments;
str+=',';
str+=acc1->Balance;
str+=',';
} else str+=",,,,";

if (acc2)
{
str+=acc2->Field[tmd::asname];
str+=',';
str+=FolderName(acc2);
str+=',';
str+=acc2->Payments;
str+=',';
str+=acc2->Balance;
} else str+=",,,";
SL->Add(str);
}
}

char* TCompareDlg::FolderName(void* a)
{
TAccount* acc= (TAccount*)a;
char tg = acc->Field[tmd::tag][0];
if (tg=='V') return "VAT";
if (tg=='D') return "Archive";
return "Active";
}


void __fastcall TCompareDlg::PrintButtonClick(TObject *Sender)
{
if (Application->MessageBox("Continue Printing ?","Compare Turbomed Data",
MB_OKCANCEL)==IDOK)
{
char* str=new char[100];
TA4Page* A4= new TA4Page;
sprintf(str,"COMPARING FILES  %s  AND %s",
mainform->filename.c_str(),
CompareFileName.c_str());

//Heading //lineTop //LinesBody
A4->PrintAt(0,str,TA4Page::LJ);
//Captions
A4->NextLine();A4->NextLine();
//"Date,Ref,Account,Code,Description,Debit,Credit,Balance"
//Account,Current Details,Folder,Payments,Balance,Compare Details,Folder,Payments,Balance");
A4->PrintAt(0,"Ref",TA4Page::LJ);
A4->PrintAt(7,"Account",TA4Page::LJ);
A4->PrintAt(25,"Folder",TA4Page::RJ);
A4->PrintAt(40,"Payments",TA4Page::RJ);
A4->PrintAt(50,"Balance",TA4Page::RJ);

A4->PrintAt(57,"Compare",TA4Page::LJ);
A4->PrintAt(75,"Folder",TA4Page::RJ);
A4->PrintAt(90,"Payments",TA4Page::RJ);
A4->PrintAt(100,"Balance",TA4Page::RJ);
A4->NextLine(); A4->NextLine();
//lineTop

for (int i=0;i<CompStrList->Count;i++)
{
CompData* dta= (CompData*)CompStrList->Objects[i];
TAccount* acc1 = (TAccount*)dta->curr;
TAccount* acc2 = (TAccount*)dta->comp;

A4->PrintAt(0,CompStrList->Strings[i].c_str(),TA4Page::LJ);

if (acc1)
{
A4->PrintAt(7,acc1->Field[tmd::asname],TA4Page::LJ);
A4->PrintAt(25,FolderName(acc1),TA4Page::RJ);
A4->PrintAt(40,acc1->Payments,TA4Page::RJ);
A4->PrintAt(50,acc1->Balance,TA4Page::RJ);
}
A4->PrintAt(52,"||",TA4Page::LJ);
if (acc2)
{
A4->PrintAt(57,acc2->Field[tmd::asname],TA4Page::LJ);
A4->PrintAt(75,FolderName(acc2),TA4Page::RJ);
A4->PrintAt(90,acc2->Payments,TA4Page::RJ);
A4->PrintAt(100,acc2->Balance,TA4Page::RJ);
}
A4->NextLine();
}
delete A4;
delete[] str;
}
}
//---------------------------------------------------------------------------

