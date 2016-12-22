//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop           
#include "main.h"
#include <math.h>
#include "PatientEdit.h"
#include "globals.h"
#include "Library.h"
#include "AboutHelp.h"
#include "EditSetup.h"
#include <stdio.h>
#include <dir.h>
#include <stdlib.h>
//#include<string>
//#include <iostream>
#include "CSVRec.h"
#include "MedAid.h"
#include "Selection.h"
#include "CreditPay.h"
#include "Report.h"
#include "find.h"
#include "StrDate.h"
#include "config.h"
#include "changeprint.h"
#include "bkeep.h"
#include "summary.h"
#include <Clipbrd.hpp>
#include <DateUtils.hpp>
#include <fstream.h>
#include "Compare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//using namespace std;
Tmainform *mainform;
String Tmainform::filename = "";
String Tmainform::pathname = "";

//---------------------------------------------------------------------------
bool Tmainform::TMRegistered(int yr)
{
int n[8]={11,13,17,19,23,29,31,37};
TCSVRec *set = TMData->Setup;
if (!set) return false;
char str[20];
char *str1;
int numo = 0;
int num = 0;
strcpy(str,set->Field[tmd::prnum]);
str1 = &str[strlen(str)-4];  //last 4 chars
for (int i=0;i<4;i++) {
num += (n[(str1[i]+yr-2)& 0x07] * (i+1));
numo += (n[(str1[i])& 0x07] * (i+1)); }

//num += (n[(str1[i]& 0x07)] * (i+1));
return ((atoi(set->Field[tmd::reg])==num*num)||   //if new number valid
        (atoi(set->Field[tmd::reg])==numo*numo));  //or old number valid
}
//---------------------------------------------------------------------------

__fastcall Tmainform::Tmainform(TComponent* Owner)
    : TForm(Owner)
{
allowlistupdate=true;
fcompare = tmd::nc;//asname;
DoubleBuffered=true;
ToDuplicate = NULL;
//if (TMData->Setup)
//if (TMData->Setup->Field[tmd::pref][2]=='1')WindowState=wsMaximized;
StatusBar1->Panels->Items[2]->Style=psOwnerDraw;

//ListView1->ColumnRightClick==true;
}

//---------------------------------------------------------------------------

void Tmainform::ListViewHeadings(int d,int m)
{
TListColumn  *NewColumn;

ListView1->Columns->Clear();

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Ref";
  NewColumn->Width = 40;
  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Last Name";
  NewColumn->Width = 100;
  NewColumn = ListView1->Columns->Add();

  if (d==2) NewColumn->Caption = "Payment Date";
        else if (d==1) NewColumn->Caption = "Procedure Date";
                else NewColumn->Caption ="Account Date";

  NewColumn->Width=100;
  NewColumn = ListView1->Columns->Add();
  switch(m)
  {
  case 0: NewColumn->Caption = "Medical Aid";break;
  case 1: NewColumn->Caption = "Referred by";break;
  case 2: NewColumn->Caption = "Clinic";break;
  case 3: NewColumn->Caption = "Diagnosis";break;
  case 4: NewColumn->Caption = "File Number";break;
  }
  NewColumn->Width = 100;
  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Payment";
  NewColumn->Alignment=taRightJustify;
  NewColumn->Width = 80;
  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Balance";
  NewColumn->Alignment=taRightJustify;
  NewColumn->Width = 80;

}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall Tmainform::FormCreate(TObject *Sender)
{
filename="";
//Application->OnIdle=IdleHandler;
savedateseparator=DateSeparator;
saveshortdateformat=ShortDateFormat;
DateSeparator='/';
ShortDateFormat="dd/MM/yyyy";
//WindowState=wsMaximized;
TAboutHelpForm * Splashscr = new TAboutHelpForm(this);
Splashscr->BorderStyle=bsNone;
Splashscr->Show();
Splashscr->Update();

 TMData = new(TData);
 TMData->isDirty = false;
 ListView1->ViewStyle = vsReport;
 ListView1->RowSelect = true;
 ListViewHeadings(0,0);
 ToolButton4->Caption = PopupMenu2->Items->Items[0]->Caption;//"Show All Types";
 
BuildFileMenu();

TConfig* Conf = new TConfig();
 if (ParamCount()>0)
 {
 filename = ExpandUNCFileName(ParamStr(1));
 /* TODO : check parameters passed */
 //char* fn=new char[filename.Length()+1];
 //strcpy(fn,filename.c_str());
 Conf->PushFile(filename);
 //delete[]fn;
 }
else filename=String(Conf->File[0]);
delete Conf;

pathname = ExtractFilePath(filename);
SetCurrentDir(pathname);
if (FileExists(filename))
TMData->LoadFromFile(filename); /* TODO : prob with opening file from param */

BuildTree();
mainform->Caption="Turbomed "+filename;
CompareField=tmd::nc;
UpdateList(0);

delete Splashscr;
//ListView1->SetFocus();

}
//---------------------------------------------------------------------------
void Tmainform::CheckSaveData()
{
String f=filename;
if (f.IsEmpty()) f=TMData->newfilename();
if (MessageDlg("Save Changes to "+f+"?", mtWarning,
TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{
Save1Click(this);
// filename=f;
// TMData->SaveToFile(filename);

}
SaveButton->Enabled = TMData->isDirty;
}

//---------------------------------------------------------------------------

void __fastcall Tmainform::FormClose(TObject *Sender,
      TCloseAction &Action)
{
ListView1->ClearSelection();
DateSeparator=savedateseparator;
ShortDateFormat=saveshortdateformat;
if (TMData->isDirty) CheckSaveData();
delete (TData*)TMData;
}

//---------------------------------------------------------------------------


void Tmainform::BuildTree()
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try{
TMData->Vat->Clear();
TMData->Recycled->Clear();
int x=0;

for (int i=0;i<TMData->Patients->Count;i++)
{
TAccount* p =(TAccount*)TMData->Patients->Items[i];
if (p->Field[tmd::tag][0]=='V')
{
char* str=yyyymm(p->VatDate);
if (!TMData->Vat->Find(str,x))
//TMData->Vat->Insert(x,yyyymm(p->VatDate)) ;
TMData->Vat->Add(str);
}
if (p->Field[tmd::tag][0]=='D')
{
char* str= Year(p->Field[tmd::dt]);
if (!TMData->Recycled->Find(str,x))
//TMData->Recycled->Insert(Year(p->Field[tmd::dt]));
TMData->Recycled->Add(str);
}
}
UpdateTree();
}
__finally{

Screen->Cursor = Save_Cursor;}

TreeView1->Items->Item[1]->Selected = true;
}







//---------------------------------------------------------------------------

void Tmainform:: AddToListView(TAccount *pat,int d)
 {

if ((ToolButton4->Tag==0)||
  ((ToolButton4->Tag==1)&&(pat->Field[tmd::sc][0]=='G'))||
  ((ToolButton4->Tag==2)&&(pat->Field[tmd::sc][0]=='C'))||
  ((ToolButton4->Tag==3)&&(pat->Field[tmd::sc][0]=='W'))||
  ((ToolButton4->Tag==4)&&(pat->Field[tmd::sc][0]=='D'))||
  ((ToolButton4->Tag==5)&&(pat->Field[tmd::sc][0]=='M'))||
  ((ToolButton4->Tag==6)&&(pat->Field[tmd::sc][0]=='E'))||
  ((ToolButton4->Tag==7)&&(pat->Field[tmd::sc][0]=='P')))
{
 TListItem *ListItem;
    ListItem = ListView1->Items->Add();
    ListItem->Data = (void*)pat;
    ListItem->Caption = (pat->Field[tmd::nc]);
    //ListItem->SubItems->Add;
    ListItem->SubItems->Add(pat->Field[tmd::asname]);
    if (CurrDateDisplay==2) ListItem->SubItems->Add((pat->VatDate)?DelimDate(pat->VatDate):"");
        else if (CurrDateDisplay==1) ListItem->SubItems->Add(DelimDate(pat->ProcedureDate));//(d_ymd(pat->Field[dt]));
                        else ListItem->SubItems->Add(DelimDate(pat->Field[tmd::dt]));//(d_ymd(pat->Field[dt]));

    switch (d)
    {
    case 0: ListItem->SubItems->Add(pat->Field[tmd::ma]);break;
    case 1: ListItem->SubItems->Add(pat->Field[tmd::ref]);break;
    case 2: ListItem->SubItems->Add(pat->Field[tmd::clin]);break;
    case 3: ListItem->SubItems->Add(pat->Field[tmd::diag]);break;
    case 4: ListItem->SubItems->Add(pat->Field[tmd::filenum]);break;
    }

    ListItem->SubItems->Add(FormatFloat("#,##0.00",StrToFloat(pat->Payments)));
    ListItem->SubItems->Add(FormatFloat("#,##0.00",StrToFloat(pat->Balance)));
 //   ListPay+=atof(pat->Payments);
 //   ListBalance+=atof(pat->Balance);
    ListCount++;
}
    
 }
//---------------------------------------------------------------------------


void Tmainform::UpdateList(int scrollpos)
{
TSelection *s = new TSelection();
char* str= new char[50];
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
ListView1->Hide();
try
{
//ListView1->Cursor=crHourGlass;
//StatusBar1->Panels->Items[2]->Text="Sorting...";
//ListView1->Items->BeginUpdate();
enum{impor,activ,vat,recyc,activsub,vatsub,recycsub};
int sw;

TAccount *p;
char* c;
int ht  = (ListView1->Items->Count)?
ListView1->TopItem->DisplayRect(drSelectBounds).bottom -
ListView1->TopItem->DisplayRect(drSelectBounds).top : 0;
ListCount=0;
ListView1->Items->Clear();


if (TreeView1->Selected->Parent==NULL)
sw = TreeView1->Selected->Index;
else sw = TreeView1->Selected->Parent->Index+3;
//MessageDlg("Switch "+ IntToStr(sw),
//mtConfirmation,TMsgDlgButtons() << mbYes << mbNo, 0);


//sw==0 : inport root
//sw==1 : active root
//sw==2 : vat root
//sw==3 : recycled root
//sw==4 : active accounts
//        index 0==current
//              1==30days
//              2==60days
//              3== 90days
//              4==120days
//sw==5 : vat month periods
//sw==6 : recycled years


switch (sw)
{
case recyc:
for (int i = 0;i<TMData->Patients->Count;i++) //TMData->Patients->Count ;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
 if  (p->Field[tmd::tag][0] == 'D')
 AddToListView(p,CurrMADisplay);}
 break;


case vat:
for (int i = 0;i< TMData->Patients->Count ;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
   if  (p->Field[tmd::tag][0] == 'V')
   AddToListView(p,CurrMADisplay);}
   break;


case activ:
for (int i = 0;i< TMData->Patients->Count ;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
//   UpdateProgress((int)(double)(i*100)/TMData->Patients->Count);
 if (p->Field[tmd::tag][0] == '\0')
   AddToListView(p,CurrMADisplay);}
  // Application->MessageBox(p->Field[asname], "Modal", MB_OK);
   break;

 case impor:
for (int i = 0;i< TMData->Patients->Count ;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
 if (p->Field[tmd::tag][0] == 'I')
   AddToListView(p,CurrMADisplay);}

  // Application->MessageBox(p->Field[asname], "Modal", MB_OK);
   break;

 case recycsub:
      c= TMData->Recycled
      ->Strings[TreeView1->Selected->Index].c_str();
 for (int i = 0;i<TMData->Patients->Count;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
 if  ((p->Field[tmd::tag][0] == 'D')&&
      (!strcmp(Year(p->Field[tmd::dt]),c)))
        AddToListView(p,CurrMADisplay);}
 break;

case vatsub:
    c= TMData->Vat
      ->Strings[TreeView1->Selected->Index].c_str();
for (int i = 0;i< TMData->Patients->Count ;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
 if ((p->Field[tmd::tag][0] == 'V')&&
     (!strcmp(yyyymm(p->VatDate),c)))
        AddToListView(p,CurrMADisplay);}break;

 case activsub:
  for (int i = 0;i< TMData->Patients->Count ;i++)
  {p = (TAccount*)TMData->Patients->Items[i];
 if (p->Field[tmd::tag][0] == '\0')
     {
//     unsigned short y = (unsigned short)atoi(Year(p->Field[dt]));
//      unsigned short m = (unsigned short)atoi(Month(p->Field[dt]));
//      unsigned short d = (unsigned short)atoi(Day(p->Field[dt]));

//TDateTime datex;
//try {datex = TDateTime(y,m,d);}
//catch( EConvertError &e){datex=TDateTime::CurrentDate();}
//     int days = TDateTime::CurrentDate() - datex;
 int days = TStrDate(p->Field[tmd::dt]).Age();
        switch(TreeView1->Selected->Index)
        {
         case 0://current acc
         if (days<31)AddToListView(p,CurrMADisplay);
         break;
         case 1://30 days
         if ((days>30)&&(days<61))AddToListView(p,CurrMADisplay);
         break;
         case 2://60 days
         if ((days>60)&&(days<91))AddToListView(p,CurrMADisplay);
         break;
         case 3://90 days
         if ((days>90)&&(days<121))AddToListView(p,CurrMADisplay);
         break;
         case 4://120 days
         if (days>120)AddToListView(p,CurrMADisplay);
         break;
        }
     }
}
break;
}

 sprintf(str,"Accounts in List: %d of %d",ListCount,TAccount::Count()-DelCount);
 StatusBar1->Panels->Items[0]->Text=str;
 strcpy(str,"");
 if (sw==activ)
 sprintf(str,"Debtor Days %s",DebtDays());
 StatusBar1->Panels->Items[3]->Text=str;

//if (TMData->Setup)
//{
//if ((TMData->Setup->Field[tmd::pref][0]=='1')
//     &&(TMData->isDirty))
//{
//if (!filename.IsEmpty())
//{
//TMData->SaveToFile(filename);
//TMData->isDirty = false;
//}
//}
//} auto save to file taken out
SaveButton->Enabled = TMData->isDirty;

////////////////////////////
for (int i=0;i<s->Count;i++)
{
for (int j=0;j<ListView1->Items->Count;j++)
{
if (ListView1->Items->Item[j]->Data==s->List->Items[i])
{ListView1->Items->Item[j]->Selected=true;break;}
}
}
/////////////////////////////

ht*=(scrollpos<ListView1->Items->Count)?scrollpos:0;
ListView1->Scroll(0,ht);
//ListView1->Items->Item[10]->Selected=true;
//ListView1->Items->EndUpdate();

PopupMenu1Popup(this);
/* done : check why this popup was used */
}
__finally
{
ListView1->Show();
delete[]str;
Screen->Cursor = Save_Cursor;
delete s;
}
}

//---------------------------------------------------------------------------


void __fastcall Tmainform::ListView1ColumnClick(TObject *Sender,
      TListColumn *Column)
{
//PopupMenu1->AutoPopup=true;
switch (Column->Index)
{
//pid,nc,lang,tag,pname,pbday,psex,
//atitle,ainit,asname,ad1,ad2,ad3,ad4,
//ma,man,dsc,clin,ref,sc,dt,diag,usm,auth,patid,ioddt
//case 0: reverse =(CompareField==nc)?!reverse:reverse;CompareField=nc;break;
case 0: reverse =(CompareField==tmd::nc)?!reverse:reverse;CompareField=tmd::nc;
UpdateList(0);break;
case 1: reverse =(CompareField==tmd::asname)?!reverse:reverse;CompareField=tmd::asname;
UpdateList(0);break;
case 2:
reverse =((CompareField==tmd::dt)||
          (CompareField==tmd::procdatesort)||
          (CompareField==tmd::paydatesort))
?!reverse:reverse;
   if (CurrDateDisplay==2)  CompareField=tmd::paydatesort;
    else if (CurrDateDisplay==1) CompareField=tmd::procdatesort;
        else CompareField=tmd::dt;
UpdateList(0);break;

case 4:reverse =(CompareField==tmd::paysort)?!reverse:reverse;CompareField=tmd::paysort;
UpdateList(0);break;
case 5:reverse =(CompareField==tmd::balsort)?!reverse:reverse;CompareField=tmd::balsort;
UpdateList(0);break;
case 3:
 {
reverse =((CompareField==tmd::ma)||
(CompareField==tmd::ref)||
(CompareField==tmd::clin)||
(CompareField==tmd::filenum)||
(CompareField==tmd::diag))
?!reverse:reverse;
switch (CurrMADisplay)
{
case 0: CompareField=tmd::ma;break;
case 1: CompareField=tmd::ref;break;
case 2: CompareField=tmd::clin;break;
case 3: CompareField=tmd::diag;break;
case 4: CompareField=tmd::filenum;break;
}
UpdateList(0);break;
}
}
}
//---------------------------------------------------------------------------



void __fastcall Tmainform::ListView1Compare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
{
TAccount* p1= (TAccount*)Item1->Data;
TAccount* p2= (TAccount*)Item2->Data;
char *s1= p1->Field[CompareField];
char *s2= p2->Field[CompareField];
char str1[15];
char str2[15];

int n1,n2;
//double d1,d2;

switch (CompareField)
{
case tmd::nc: if (!reverse){n1=atoi(s1);
                       n2=atoi(s2);}
         else         {n1=atoi(s2);
                       n2=atoi(s1);}

            if (n1==n2) Compare=0;
            if (n1>n2) Compare=1;
            if (n1<n2) Compare=-1;
         break;


case tmd::asname:
case tmd::ma:
case tmd::ref:
case tmd::clin:
case tmd::filenum:
case tmd::diag:if (!reverse)Compare = stricmp(s1,s2);
                    else Compare = stricmp(s2,s1);break;

case tmd::dt: if (!reverse){n1=atoi(revdate(s1));
                       n2=atoi(revdate(s2));}
         else         {n1=atoi(revdate(s2));
                       n2=atoi(revdate(s1));}

            if (n1==n2) Compare=0;
            if (n1>n2) Compare=1;
            if (n1<n2) Compare=-1;
         break;
case tmd::paysort:
             strcpy(str1,p1->Payments);
             strcpy(str2,p2->Payments);
             n1=(atof(str1)*100);
             n2=(atof(str2)*100);
             if (n1==n2) Compare=0;
            if (n1>n2) Compare=1;
            if (n1<n2) Compare=-1;
            //Compare=strcmp(str1,str2);
            if (reverse) Compare*=-1;
        break;
case tmd::balsort:
             strcpy(str1,p1->Balance);
             strcpy(str2,p2->Balance);
             n1=(atof(str1)*100);
             n2=(atof(str2)*100);
             if (n1==n2) Compare=0;
            if (n1>n2) Compare=1;
            if (n1<n2) Compare=-1;
            //Compare=strcmp(str1,str2);
            if (reverse) Compare*=-1;
        break;
case tmd::procdatesort:

            n1=atoi(revdate(p1->ProcedureDate));
            n2=atoi(revdate(p2->ProcedureDate));
            if (n1==n2) Compare=0;
            if (n1>n2) Compare=1;
            if (n1<n2) Compare=-1;
            if (reverse) Compare*=-1;
         break;

case tmd::paydatesort:
           // if ((p1->VatDate)&&(p2->VatDate))
           // {
            n1=(p1->VatDate)?atoi(revdate(p1->VatDate)):0;
            n2=(p2->VatDate)?atoi(revdate(p2->VatDate)):0;
            if (n1==n2) Compare=0;
            if (n1>n2) Compare=1;
            if (n1<n2) Compare=-1;
            if (reverse) Compare*=-1;
           // }
           // else Compare=0;
         break;

}
}
//---------------------------------------------------------------------------

void Tmainform::EditAccount()

{
int scrl= (ListView1->TopItem)?ListView1->TopItem->Index:0;
int foc = (ListView1->ItemFocused)?
        ListView1->ItemFocused->Index:-1;
PatientEditForm = new TPatientEditForm(ListView1);
PatientEditForm->ShowModal();
delete  PatientEditForm;
UpdateList(scrl);
if (foc>-1)
{
ListView1->Items->Item[foc]->Selected=true;
ListView1->Items->Item[foc]->Focused=true;
}
ListView1->SetFocus();
PopupMenu1Popup(this);
/* done : check why this popup was used */
}

//---------------------------------------------------------------------------
char * Tmainform::FolderName(void)
{
static char str[50];

if (TreeView1->Selected->Parent)
    sprintf(str,"%s \\%s",
    TreeView1->Selected->Parent->Text.c_str(),
    TreeView1->Selected->Text.c_str());

else strcpy(str,TreeView1->Selected->Text.c_str());
return str;

}

//---------------------------------------------------------------------------

void __fastcall Tmainform::ListView1DblClick(TObject *Sender)
{
EditAccount();
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::SpeedButton1Click(TObject *Sender)
{
char *str = new char[200];
sprintf(str,
"Accounts Total: %d\n\n\
Turbomed demonstation program is\n\
   limited to 50 accounts.\n\n\
To register for a full working program\n\n\
Phone: Turbomed Support\n\
011 888 4681\n\n\
support@turbomed.co.za",TAccount::Count());

//if (filename.IsEmpty()) {delete[]str;EditAccount();return; }
unsigned short y,m,d;
TDateTime::CurrentDate().DecodeDate(&y,&m,&d);
if ((!TMRegistered(y))&&(!filename.IsEmpty()))
{
Application->MessageBox(str,
"Turbomed Registration",MB_OK);
if (TAccount::Count()<51)
{
TListItem* T = ListView1->ItemFocused;
if (T) T->Focused = false;
EditAccount();
}
}
else
{
TListItem* T = ListView1->ItemFocused;
if (T) T->Focused = false;
EditAccount();
}
//TListItem* T = NULL;
//EditAccount();
delete[] str;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Library1Click(TObject *Sender)
{
LibForm = new TLibForm(this);
LibForm->Libview->Checkboxes = false;

LibForm->Tag=1;
LibForm->LoadData();
LibForm->ShowModal();
delete LibForm;
LibForm = NULL;
SaveButton->Enabled = TMData->isDirty;

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::About1Click(TObject *Sender)
{
AboutHelpForm = new TAboutHelpForm(this);
AboutHelpForm->ShowModal();
delete AboutHelpForm;
AboutHelpForm = NULL;
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::Exit1Click(TObject *Sender)
{
mainform->Close();
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Setup1Click(TObject *Sender)
{
SetupForm = new TSetupForm(this);
SetupForm->ShowModal();
delete SetupForm;
SaveButton->Enabled = TMData->isDirty;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::All1Click(TObject *Sender)
{
//ToolButton2->Caption = ((TMenuItem*)Sender)->Caption;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::TreeView1Changing(TObject *Sender,
      TTreeNode *Node, bool &AllowChange)
{
AllowChange = true;//allownodeselect;//setupcomplete;
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::AllFees1Click(TObject *Sender)
{
ToolButton4->Caption = ((TMenuItem*)Sender)->Caption;
ToolButton4->Tag=((TMenuItem*)Sender)->MenuIndex;
UpdateList(0);
ListView1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall Tmainform::SelectAll1Click(TObject *Sender)
{
for (int i=0;i<ListView1->Items->Count ;i++)
{
ListView1->Items->Item[i]->Selected=true;
}
ListView1->SetFocus();
PopupMenu1Popup(this);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------



void __fastcall Tmainform::TreeView1Change(TObject *Sender,
      TTreeNode *Node)
{
if (allowlistupdate)
{
if ((Node->Index<2)||(Node->Parent))
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try
{
//UpdateFolderLabel();
StatusBar1->Repaint();
//UpdateList(0);
}
__finally
{
Screen->Cursor = Save_Cursor;
}
UpdateList(0);
}
else
{ListView1->Clear();
StatusBar1->Panels->Items[0]->Text="Contains "+
IntToStr(Node->Count)+" Subfolders";
StatusBar1->Panels->Items[3]->Text="";
StatusBar1->Invalidate();}
}
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Open1Click(TObject *Sender)
{
if (TMData->isDirty) CheckSaveData();

TOpenDialog* D = new TOpenDialog(this);
D->InitialDir = pathname;
D->Title = "Open Turbomed File";
D->Filter = "Turbomed data files (*.tmd)|*.TMD";
if (D->Execute()){
    if (FileExists(D->FileName))
    {
    filename = D->FileName;
    TConfig *Config = new TConfig();
    Config->PushFile(filename);
    delete Config;
    BuildFileMenu();
    delete (TData*)TMData;
    DelCount=0;
    TMData = new TData();
    pathname = ExtractFilePath(filename);
    TMData->LoadFromFile(filename);//"c:\\Development\\tmed2\\Default.txt");
    BuildTree();
    mainform->Caption="Turbomed "+filename;
    TMData->isDirty = false;
    UpdateList(0);
    }
                }
delete D;
//ListView1->Visible=true;
ListView1->SetFocus();
//if (ListView1->Items->Count)
//{ListView1->ItemIndex = 0;
//ListView1->Selected=ListView1->Items->Item[0];}
//char x[60];
//getcurdir(0,x);
//OutputDebugString(x);

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::ViewMedAids1Click(TObject *Sender)
{
MedAidList  = new TMedAidList(this);
MedAidList ->Tag=1;
MedAidList ->ShowModal();
delete MedAidList;
TMData->PatientToStrings();
SaveButton->Enabled = TMData->isDirty;
}
//---------------------------------------------------------------------------




void __fastcall Tmainform::Details1Click(TObject *Sender)
{

TSelection * s = new TSelection();
char *str = new char[255];
sprintf(str,
"\
  Folder:\t%s\n\
Accounts: %4d\n\
Selected: %4d\n\n\
Billed:\t%10s\n\
Payment:\t%10s\n\
Balance:\t%10s\
",
FolderName(),
ListCount,
s->Count,
FormatFloat("#,##0.00",s->Billed),
FormatFloat("#,##0.00",s->Payment),
FormatFloat("#,##0.00",s->Balance));

Application->MessageBox(str,"Details",MB_OK);
//s->PrintAgeReport();
delete s;
delete[] str;

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::PopupMenu1Popup(TObject *Sender)
{
int sw;
TSelection * s = new TSelection();

for (int i=0;i<File1->Count;i++)
File1->Items[i]->Enabled=true;

for (int i=0;i<Actions1->Count;i++)
Actions1->Items[i]->Enabled=true;

for (int i=0;i<PopupMenu1->Items->Count;i++)
PopupMenu1->Items->Items[i]->Enabled=true;

sw=ListView1->Items->Count;
AddButton->Enabled=true; //new acc
EditButton->Enabled=(sw)?true:false; //edit
PrintButton->Enabled=(sw)?true:false; //print
CreditButton->Enabled=(sw)?true:false; //credit
FindButton->Enabled=(sw)?true:false;


Delete2->Enabled = (s->Balance>0) ? false : true;
Delete1->Enabled = (s->Balance>0) ? false : true;
Receipt2->Enabled = (s->Balance==0) ? false : true;
Receipt1->Enabled = (s->Balance==0) ? false : true;
CreditButton->Enabled=(s->Balance==0) ? false : true;
PrintReceipts2->Enabled =(s->Payment==0)? false:true;
PrintReceipts1->Enabled =(s->Payment==0)? false:true;

//if (ActiveControl!=ListView1) NewAccount1->Enabled=false;

if (TreeView1->Selected->Parent==NULL)
sw = TreeView1->Selected->Index;
else sw = TreeView1->Selected->Parent->Index;
switch (sw)
{
case 3:MovetoArchive1->Enabled=false;
    MovetoArchive2->Enabled=false;
    Delete2->Enabled =  true;
    Delete1->Enabled =  true;

 //   MovetoVAT2->Enabled=false;
    PrintVATreport1->Enabled=false;
    PrintAnalysis1->Enabled=false;
    if (s->VatStamped()){
    MovetoActive1->Enabled=false;
    MovetoActive2->Enabled=false;
                        }
    break;
case 2:
    MovetoVAT1->Enabled=false;
    MovetoVAT2->Enabled=false;
    PrintAnalysis1->Enabled=false;
    if (s->VatStamped()){
    Receipt2->Enabled =false;
    Receipt1->Enabled = false;
    CreditButton->Enabled=false;
    MovetoActive1->Enabled=false;
    MovetoActive2->Enabled=false;
                        }
    break;

case 1:
//if (s->Balance>0)
//       {
//       MovetoArchive1->Enabled=false;
//      MovetoArchive2->Enabled=false;
//        }
       MovetoActive1->Enabled=false;
       MovetoActive2->Enabled=false;
       PrintVATreport1->Enabled=false;
       //PrintAnalysis1->Enabled=false;
       break;
case 0:
       MovetoArchive1->Enabled=false;
       MovetoArchive2->Enabled=false;
       MovetoVAT1->Enabled=false;
       MovetoVAT2->Enabled=false;
       PrintVATreport1->Enabled=false;
       //PrintAnalysis1->Enabled=false;
       break;

}
if (filename.IsEmpty())
{
Import1->Enabled = false;
EditButton->Enabled=false;
SelectAll1->Enabled = false;
SelectDue1->Enabled = false;

}
//PrintButton->Enabled= (s->Payment==0)? false:true;
if (TMData->Setup==NULL)
{
Duplicate1->Enabled =false;
PrintAnalysis1->Enabled=false;
EDI1->Enabled=false;
Export1->Enabled=false;
PrintVATreport1->Enabled=false;
//Delete2->Enabled = false;
//Delete1->Enabled = false;
//Receipt2->Enabled =false;
//Receipt1->Enabled = false;
//CreditButton->Enabled=false;
PrintButton->Enabled=false;
PrintReceipts2->Enabled =false;
PrintReceipts1->Enabled =false;
PrintInvoices2->Enabled =false;
PrintInvoices1->Enabled =false;
PrintStatements1->Enabled=false;
//EditButton->Enabled=false;
//AddButton->Enabled =false;
//EditAccount1->Enabled =false;
//EditAccount2->Enabled =false;
}
else // if there is a valid setup
{
if (strlen(TMData->Setup->Field[tmd::mednet])==0)
EDI1->Enabled=false;
}
if (s->Count==0)

//if no selection most functions
//are disabled
{
//SelectAll1->Enabled = false;
//SelectDue1->Enabled = false;
Duplicate1->Enabled =false;
PrintAnalysis1->Enabled=false;
EDI1->Enabled=false;
Export1->Enabled=false;
PrintVATreport1->Enabled=false;
MovetoActive1->Enabled=false;
MovetoActive2->Enabled=false;
MovetoVAT1->Enabled=false;
MovetoVAT2->Enabled=false;
MovetoArchive1->Enabled=false;
MovetoArchive2->Enabled=false;
Delete2->Enabled = false;
Delete1->Enabled = false;
Receipt2->Enabled =false;
Receipt1->Enabled = false;
PrintButton->Enabled=false;
PrintReceipts2->Enabled =false;
PrintReceipts1->Enabled =false;
PrintInvoices2->Enabled =false;
PrintInvoices1->Enabled =false;
PrintStatements1->Enabled=false;
EditButton->Enabled=false;
CreditButton->Enabled =false;
EditAccount1->Enabled =false;
EditAccount2->Enabled =false;
Reports1->Enabled=false;
Reports2->Enabled=false;
PrintLabels1->Enabled=false;
PrintLabels2->Enabled=false;
SendSummary->Enabled=false;

//Find1->Enabled=false;
//Find2->Enabled=false;
//FindButton->Enabled=false;
}
if (s->Count>1){
Receipt2->Enabled =false;
Receipt1->Enabled = false;
CreditButton->Enabled=false;
EditButton->Enabled =false;
EditAccount1->Enabled =false;
EditAccount2->Enabled =false;
Duplicate1->Enabled =false;
Duplicate2->Enabled =false;
}
if (s->Payment==0)
{
MovetoVAT1->Enabled=false;
MovetoVAT2->Enabled=false;
//MovetoArchive1->Enabled=false;
//MovetoArchive2->Enabled=false;
}
StatusBar1->Panels->Items[1]->Text="Selected: "+IntToStr(s->Count);
delete s;
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::PrintInvoices1Click(TObject *Sender)
{

TSelection *s = new TSelection();
int edicount = s->ContainsEDI();
if (edicount)
    if (MessageDlg("Selection includes EDI Accounts\nDo you want to print these ?",
                     mtInformation,
                     TMsgDlgButtons() << mbYes << mbNo, 0) ==mrYes)
    edicount=0;
s->PrintInvoices(edicount);
//s->DateStamp("INVOICE");                   )
//TMData->isDirty=true;
SaveButton->Enabled = TMData->isDirty;
delete s;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::PrintReceipts2Click(TObject *Sender)
{
TSelection *s = new TSelection();
s->PrintReceipts();
//s->DateStamp("RECEIPT");
//TMData->isDirty=true;
SaveButton->Enabled = TMData->isDirty;

delete s;
}
//---------------------------------------------------------------------------



void __fastcall Tmainform::PrintVATreport1Click(TObject *Sender)
{
TSelection *s = new TSelection();
s->PrintVatReport();
s->DateStamp("VAT");
TMData->isDirty=true;
SaveButton->Enabled = TMData->isDirty;

delete s;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::MovetoActive1Click(TObject *Sender)
{
int listpos = ListView1->TopItem->Index;
TSelection *s = new TSelection();
if (!s->VatStamped())
{
if (!strcmp("Inbox",FolderName()))
s->Renumber();
s->ToActive();
TMData->isDirty=true;
//TreeView1->Visible=false;
//BuildTree();
UpdateList(listpos);
//TreeView1->Visible=true;
}
//else
//if (MessageDlg("Cannot Move VAT Account", mtError,
//              TMsgDlgButtons() << mbOK,0)==mrOk)
// else if (MessageBox(Selected Accounts
// have been processed for VAT
// Press Continue if you are sure you want to reacivate
// otherwise Cancel) == IFYes){s->ToActive();UpdateList();}
delete s;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::MovetoVAT1Click(TObject *Sender)
{
int listpos =ListView1->TopItem->Index;
TSelection *s = new TSelection();
s->ToVat();
//TMData->PatientToStrings();
TMData->isDirty=true;
//TreeView1->Visible=false;
//if (FolderName()[1]=='c') BuildTree();
CheckTree(s->List);
UpdateList(listpos);
//TreeView1->Visible=true;
delete s;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::MovetoArchive1Click(TObject *Sender)
{
int listpos=ListView1->TopItem->Index;
TSelection *s = new TSelection();
s->ToArchive();
//TMData->PatientToStrings();
TMData->isDirty=true;
//TreeView1->Visible=false;
CheckTree(s->List);
UpdateList(listpos);
//TreeView1->Visible=true;
delete s;
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::Receipt1Click(TObject *Sender)
{
TSelection *s= new TSelection();
TListItem* T = ListView1->Selected;
if (T)
{
int listpos=ListView1->TopItem->Index;
Credit = new TCredit(ListView1);
if (Credit->ShowModal() == mrOk)
{
Credit->Store();
if (Credit->Tag)CheckTree(s->List);
TMData->isDirty = true;
UpdateList(listpos);
}
delete Credit;
SaveButton->Enabled = TMData->isDirty;
}
ListView1->SetFocus();
delete s;
}

//---------------------------------------------------------------------------
void __fastcall Tmainform::PrintAnalysis1Click(TObject *Sender)
{
TSelection * s = new TSelection();
s->PrintAgeReport();
delete s;
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::SaveAs1Click(TObject *Sender)
{

TSaveDialog* D = new TSaveDialog(this);
D->FileName = (filename.IsEmpty()) ? TMData->newfilename():filename;
D->Title = "Save";
D->Filter = "Turbomed data files (*.tmd)|*.TMD";
D->DefaultExt = String("tmd");

if (D->Execute())
{
bool ContinueOK= true;
if (filename!=D->FileName)
    {
    if (FileExists(D->FileName)) // Check that its ok to overwrite
      { ContinueOK = (MessageDlg("Overwrite "+D->FileName,
               mtWarning, TMsgDlgButtons() << mbOK<<mbCancel,0) ==
               mrOk); }
    if (ContinueOK) { //if filename has changed then update registry
                     filename = D->FileName;
                     pathname = ExtractFilePath(D->FileName);
                     TConfig *Config = new TConfig();
                     Config->PushFile(filename);
                     delete Config;
                     }
     }

//char * fn= new char[filename.Length()+1];
//strcpy(fn,filename.c_str());
if (ContinueOK)
{
TMData->SaveToFile(filename);

//delete[] fn;
BuildFileMenu();
//TMData->isDirty = false;
SaveButton->Enabled = TMData->isDirty;//false;
mainform->Caption="Turbomed "+filename;
}
}
delete D;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Import1Click(TObject *Sender)
{
String ImportFile;
TOpenDialog* D = new TOpenDialog(this);
D->Title = "Import";
D->Filter = "Turbomed data files (*.tmd)|*.TMD";
if (D->Execute()) {
    ImportFile = D->FileName;
    TMData->ImportFile(ImportFile.c_str());//"c:\\Development\\tmed2\\Default.txt");
    //BuildTree();
 //   mainform->Caption="TurboMED "+filename;
    if (ListView1->Items->Count)UpdateList(ListView1->TopItem->Index);
                    }

delete D;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Export1Click(TObject *Sender)
{
bool OKflag = true;
TSaveDialog* D = new TSaveDialog(this);
//D->FileName = "Export";
TSelection* s = new TSelection();
char* str = new char[50];
D->Title = "Export";
D->FileName = TMData->newexportname();
D->Filter = "Turbomed data files (*.tmd)|*.TMD";
D->DefaultExt = "TMD";

if (D->Execute())
{
sprintf(str,"Overwrite %s ?",D->FileName);
if (D->FileName!=filename) // continue otherwise goto exit1
{
        if (FileExists(D->FileName)) // if it is possible to overwrite data then warn
              OKflag = (Application->MessageBox(str, "Warning", MB_YESNO)==IDYES);

if (OKflag) TMData->ExportToFile(D->FileName,s->List);


}
else Application->MessageBox("Export FileName Error", "Error", MB_OK);//exit1
}

delete[] str;
delete s;
delete D;
}


//---------------------------------------------------------------------------

void __fastcall Tmainform::Save1Click(TObject *Sender)
{
 TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try
{Save();}
__finally
{
Screen->Cursor = Save_Cursor;
}
}
//---------------------------------------------------------------------------



void __fastcall Tmainform::Delete2Click(TObject *Sender)
{
TStringList* sl= new TStringList;
sl->Sorted=true;
sl->Duplicates = dupIgnore;
char* f= new char[13];
TSelection *s = new TSelection();
if (MessageDlg("Delete "+IntToStr(s->Count)+" accounts?", mtWarning,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{
SetCurrentDir(pathname);
for (int i=0;i<s->Count;i++)
{
TAccount *A = (TAccount*)s->List->Items[i];
//A->Field[tmd::tag] = "Z";
//if (atof(A->Balance)!=0)
//{   char* bal = new char[10];
//    sprintf(bal,"%9.2f",atof(A->Balance)*-1);
//    TCSVRec *T = new TCSVRec;
//    T->FieldCount = tmd::transcnt;
//    T->Field[tmd::tdate]= A->Field[tmd::dt];
//    T->Field[tmd::code]="ADJ";
//    T->Field[tmd::proc]="Adjustment";
//    T->Field[tmd::amt]=bal;
//    String("-" + ((String)A->Balance)).c_str();
//    A->Trans->Add(T);
//    delete[] bal;
//   }

sprintf(f,"arc%s.tmd",&A->Field[tmd::dt][4]);

/* TODO : check this new achive code */

int ndx = sl->IndexOf(f);
if (ndx==-1) ndx=sl->AddObject(f,new TList);
((TList*)sl->Objects[ndx])->Add(A);

// end of new archive code
//TMData->AppendToFile(f,A);
}
for (int i=0;i<sl->Count;i++)
{
if (MessageDlg("The following changes are about to be made:\n\
\nAccount(s) will be removed from: "+ filename +
"\n              and archived in: " + ExpandFileName(sl->Strings[i]),
               mtWarning, TMsgDlgButtons() << mbOK << mbAbort, 0)==mrOk)

{
TList* L = (TList*)sl->Objects[i];
for (int i=0;i<L->Count;i++)
        { TAccount* A= (TAccount*)L->Items[i];
          A->Field[tmd::tag] = "Z";
        }
if (FileExists(sl->Strings[i]))
TMData->AppendToFile(sl->Strings[i],(TList*)sl->Objects[i]);
else TMData->ExportToFile(sl->Strings[i],(TList*)sl->Objects[i]);
TMData->isDirty=true;
}
delete (TList*)sl->Objects[i];}
}
delete s;
delete sl;
delete[] f;

DelCount=0;
TAccount::ndc=0;
for (int i=0;i<TMData->Patients->Count;i++)
{
TAccount *A = (TAccount*)TMData->Patients->Items[i];
char tg=A->Field[tmd::tag][0];
//A->Check_ndc();
//Check_ndc() updates the next account number
//but dont count the inbox
if (tg == 'Z')DelCount++;else if (tg != 'I')A->Check_ndc();
}
//TreeView1DblClick(this);
//if (!ListView1->Items->Count)
//{
//BuildTree();
UpdateList(0);
//StatusBar1->Repaint();
//}
//UpdateList(ListView1->TopItem->Index);
//OutputDebugString("after delete");
//char x[60];
//getcurdir(0,x);
//OutputDebugString(x);
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::Contents1Click(TObject *Sender)
{
Application->HelpFile = ExtractFilePath(Application->ExeName) + "tmed.hlp";
Application->HelpCommand(15,-3);//(HELP_CONTENTS,-3);

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Duplicate1Click(TObject *Sender)
{
TListItem* T = ListView1->ItemFocused;
if (T)
{
T->Focused=false;
ToDuplicate = (TAccount*)T->Data;
EditAccount();
}

}
//---------------------------------------------------------------------------




//void Tmainform::UpdateFolderLabel()
//{
//}

void __fastcall Tmainform::Reports1Click(TObject *Sender)
{
TReportForm * R = new TReportForm(this);//(ListView1);
R->ShowModal();
delete R;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::ListView1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if (Key==VK_RETURN) EditAccount();// else PopupMenu1Popup(this);
}
//---------------------------------------------------------------------------


/* TODO : check that save updates pathname */

void Tmainform::Save()
{
if ((!filename.IsEmpty())&&
    (FileExists(filename)))
{
quikBackup();

//char * fn= new char[filename.Length()+1];
//strcpy(fn,filename.c_str());
TMData->SaveToFile(filename);
//delete[] fn;
//TMData->isDirty = false;
SaveButton->Enabled = TMData->isDirty;//false;
}
else
SaveAs1Click(this);

}


//void Tmainform::Blank(void)
//{
//TreeView1->Visible=false;
//ListView1->Visible=false;
    //DONE: Add your source code here
//}


//void Tmainform::Restore(void)
//{
//TreeView1->Visible=true;
//ListView1->Visible=true;
    //DONE: Add your source code here
//}

void __fastcall Tmainform::DispMAClick(TObject *Sender)
{
((TMenuItem*)Sender)->Checked=true;
CurrMADisplay=((TMenuItem*)Sender)->Tag;
ListViewHeadings(CurrDateDisplay,CurrMADisplay);
reverse=false;
switch(((TMenuItem*)Sender)->Tag)
{
case 0:CompareField=tmd::ma;break;
case 1:CompareField=tmd::ref;break;
case 2:CompareField=tmd::clin;break;
case 3:CompareField=tmd::diag;break;
case 4:CompareField=tmd::filenum;break;
}
PopupMenu1->AutoPopup=true;
UpdateList(0);
}

//---------------------------------------------------------------------------

void __fastcall Tmainform::EDI1Click(TObject *Sender)
{
 TSelection *s = new TSelection();
 s->EDI();
// s->DateStamp("EDI");
//TMData->isDirty=true;
SaveButton->Enabled = TMData->isDirty;
 delete s;
}
//---------------------------------------------------------------------------








void __fastcall Tmainform::SelectDue1Click(TObject *Sender)
{
TAccount * acc;
for (int i=0;i<ListView1->Items->Count ;i++)
{
acc = (TAccount*) ListView1->Items->Item[i]->Data;
ListView1->Items->Item[i]->Selected = acc->Due();
}
ListView1->SetFocus();
PopupMenu1Popup(this);

}
//---------------------------------------------------------------------------


void __fastcall Tmainform::StatusBar1DrawPanel(TStatusBar *StatusBar,
      TStatusPanel *Panel, const TRect &Rect)
{
char str[50];
if (TreeView1->Selected->Parent)
    sprintf(str,"%s \\%s",
    TreeView1->Selected->Parent->Text.c_str(),
    TreeView1->Selected->Text.c_str());
else strcpy(str,TreeView1->Selected->Text.c_str());

TCanvas *pCanvas = StatusBar->Canvas;
pCanvas->Font->Color = clBlack;
switch (str[1])
{
case 'c':pCanvas->Brush->Color=clGreen;//aCtive
pCanvas->Font->Color=clWhite;break;
case 'A':pCanvas->Brush->Color=clYellow; //vAt
//Label1->Font->Color=clWhite;
break;
case 'r':pCanvas->Brush->Color=clMaroon;
pCanvas->Font->Color=clWhite;break; //aRchive
case 'n':pCanvas->Brush->Color=clWhite;break;
}
pCanvas->FillRect(Rect);
  //pCanvas->Font->Color = clYellow;
  //ImageList1->Draw(pCanvas,Rect.Left,Rect.Top, Panel->Index);
pCanvas->TextOut(
(Rect.Right-Rect.left)/2 -
 pCanvas->TextWidth(str)/2 +
 Rect.Left,
 Rect.top,str);

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::Find1Click(TObject *Sender)
{
TListItem* T=ListView1->TopItem;
if (T)
{
int ht  = T->DisplayRect(drSelectBounds).bottom -
           T->DisplayRect(drSelectBounds).top;

TFindForm* ff = new TFindForm(this);
ff->ShowModal();
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try
{
if (ff->ListView1->Selected)
{
TAccount* A = (TAccount*)ff->ListView1->Selected->Data;
DisplayFolder((void*)A);
//DisplayFolder(A->Field[tmd::tag][0]);
//first list the right folder here
TListItem* LI = ListView1->ItemFocused;
if (LI) LI->Focused = false;
ListView1->Selected=NULL;
for (int i=0;i<ListView1->Items->Count;i++)
{
TAccount* B = (TAccount*)ListView1->Items->Item[i]->Data;
if (B == A)
{
ListView1->Items->Item[i]->Focused = true;
ListView1->Items->Item[i]->Selected = true;break;}
}
ListView1->Scroll(0,ht*ListView1->Selected->Index);
}}
__finally
{
Screen->Cursor = Save_Cursor;
delete ff;
ListView1->SetFocus();
}
//TreeView1->Items->Item[1]->Selected = true;
//if (b)
//{
//ListView1->Items->BeginUpdate();
//while (!FocusInView()) ListView1->
//Scroll(0,ht*ListView1->VisibleRowCount);
//ListView1->Items->EndUpdate();
//}
PopupMenu1Popup(this);
/* done : check why this popup was used */
}
}
//---------------------------------------------------------------------------


//bool Tmainform::FocusInView(void)
//{
//bool b=false;
//int k= ListView1->TopItem->Index;
//int j= k+ListView1->VisibleRowCount;

//for (int i=k ;i<j;i++)
//{if (ListView1->Items->Item[i]==ListView1->Selected)
//{b=true;break;}}
//return b;
//}


void Tmainform::DisplayFolder(void* accptr)
{
//
//TAccount* p =(TAccount*)TMData->Patients->Items[i];
//if (p->Field[tmd::tag][0]=='V')
//TMData->Vat->Add(yyyymm(p->vatdate()));
//if (p->Field[tmd::tag][0]=='D')
//TMData->Recycled->Add(Year(p->Field[tmd::dt]));
//
char s = ((TAccount*)accptr)->Field[tmd::tag][0];
int x;
String str="Active";
switch (s)
{
case 'I': str="Inbox";break;
case 'D': str=Year(((TAccount*)accptr)->Field[tmd::dt]);break;
case 'V': str=yyyymm(((TAccount*)accptr)->VatDate);
          str=MonthYearStr(str.c_str());break;
}
//String str ="Active";
//if (s=='D') str="Archive";
//if (s=='V') str="VAT";
//if (s=='I') str="Inbox";
//bool treechanged=true;
for (int i=0;i<TreeView1->Items->Count;i++)
{if (TreeView1->Items->Item[i]->Text==str){x=i;break;}}
//if (treechanged)   // the folder is missing
//       {BuildTree();
//       for (int i=0;i<TreeView1->Items->Count;i++)
//       {if (TreeView1->Items->Item[i]->Text==str){treechanged=false;x=i;break;}}}



TreeView1->Items->Item[x]->Selected = true;
ToolButton4->Tag=0;  // resets listview to display all accounts
ToolButton4->Caption = PopupMenu2->Items->Items[0]->Caption;
UpdateList(0);
}
//----------------------------------
//TList * L = A-> Trans;
//TCSVRec*R = (L->Count)?(TCSVRec*)L->Items[0]:NULL;
//char* s = (R)? R->Field[tdate]:A->Field[dt];
//if (strlen(s)!=8) s = A->Field[dt];
//TStrDate d(s);
//TDateTime accdate(d.Year(),d.Month(),d.Day());
//int j= accdate.DayOfWeek()-1;
//-----------------------------------
char* Tmainform::DebtDays(void)
{
static char str[6];
double turnover=0;
double tot=0;
strcpy(str,"error");
for (int i=0;i<ListView1->Items->Count;i++)
{
TAccount* acc = (TAccount*)ListView1->Items->Item[i]->Data;
tot += atof(acc->Balance);
TList * L = acc-> Trans;
TCSVRec*R = (L->Count)?(TCSVRec*)L->Items[0]:NULL;
char* s = (R)? R->Field[tmd::tdate]:acc->Field[tmd::dt];
if (strlen(s)!=8) s = acc->Field[tmd::dt];
int j = TStrDate(s).Age();
//TDateTime accdate;
//try{accdate=TDateTime(d.Year(),d.Month(),d.Day());}
//catch(EConvertError &)
//{accdate=TDateTime::CurrentDate();}
//int j= TDateTime::CurrentDate()-accdate;

turnover+=(j<31)?atof(acc->Balance):0;
}
if ((turnover!=0)&&((tot/turnover)<100))
sprintf(str,"%5.0f",tot/turnover*30);
return str;
}

void __fastcall Tmainform::TreeView1Click(TObject *Sender)
{
ListView1->Selected=NULL;
PopupMenu1Popup(this);
/* done : check why this popup was used */
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::PrintLabels1Click(TObject *Sender)
{
TAccount * acc;
TlabelPg* pg = new TlabelPg;
for (int i=0;i<ListView1->Items->Count ;i++)
if (ListView1->Items->Item[i]->Selected)
{
acc = (TAccount*) ListView1->Items->Item[i]->Data;
pg->Labels->Add(acc->Field[tmd::ma]);
}
pg->Validate();
if (pg->Labels->Count) pg->PrintLabels();
else ShowMessage("No Labels Available");
delete pg;
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::savClick(TObject *Sender)
{
int ndx = atoi(&((TMenuItem*)Sender)->Caption.c_str()[1]);
String FN = "";
TConfig* Conf = new TConfig();
if ((Conf->FileCount)&&(Conf->File[ndx-1])) FN = (Conf->File[ndx-1]);
delete Conf;
//char* FN =&((TMenuItem*)Sender)->Caption.c_str()[3];
//String FN1 = ExpandFileName(FN);
if (FileExists(FN))
    {
 if (TMData->isDirty) CheckSaveData();
    filename = FN;//String(FN);
    pathname = ExtractFilePath(filename);
    TConfig *Config = new TConfig();
    Config->PushFile(filename);
    delete Config;
    delete TMData;
    DelCount=0;
    TMData = new TData();
    TMData->LoadFromFile(filename);//"c:\\Development\\tmed2\\Default.txt");
    BuildTree();
    mainform->Caption="Turbomed "+filename;
    TMData->isDirty = false;
    UpdateList(0);
    BuildFileMenu();
    }
ListView1->SetFocus();
}

//---------------------------------------------------------------------------



void Tmainform::BuildFileMenu(void)
{
TConfig* Conf = new TConfig();
if (Conf->FileCount) N19->Visible=true;
if (Conf->File[0])
{
sav1->Visible=true;
sav1->Caption ="&1 "+ ExtractFileName(Conf->File[0]);
}
if (Conf->File[1])
{
sav2->Visible=true;
sav2->Caption ="&2 " + ExtractFileName(Conf->File[1]);
}
if (Conf->File[2])
{
sav3->Visible=true;
sav3->Caption ="&3 " + ExtractFileName(Conf->File[2]);
}
if (Conf->File[3])
{
sav4->Visible=true;
sav4->Caption ="&4 " + ExtractFileName(Conf->File[3]);
}
delete Conf;
}

void __fastcall Tmainform::New2Click(TObject *Sender)
{
//filename=""; //??
pathname= ExtractFilePath(Application->ExeName) + "Examples";
//SetCurrentDir(pathname);
Open1Click(this);
}
//---------------------------------------------------------------------------



void __fastcall Tmainform::ListView1ColumnRightClick(TObject *Sender,
      TListColumn *Column, TPoint &Point)
{
int x=Column->Index;
switch (x)
{
case 3:
for (int i=0;i<5;i++)
PopupMenu3->Items->Items[i]->Visible=true;
for (int i=5;i<8;i++)
PopupMenu3->Items->Items[i]->Visible=false;
PopupMenu3->Popup(Point.x+ListView1->ClientOrigin.x,Point.y+ListView1->ClientOrigin.y);
break;

case 2:
for (int i=0;i<5;i++)
PopupMenu3->Items->Items[i]->Visible=false;
for (int i=5;i<8;i++)
PopupMenu3->Items->Items[i]->Visible=true;
PopupMenu3->Popup(Point.x+ListView1->ClientOrigin.x,Point.y+ListView1->ClientOrigin.y);
break;

default :PopupMenu1->AutoPopup=true;
}

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::PopupMenu3Popup(TObject *Sender)
{
PopupMenu1->AutoPopup=false;
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::TreeView1DblClick(TObject *Sender)
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try
{
//UpdateFolderLabel();
StatusBar1->Repaint();
//UpdateList(0);
}
__finally
{
Screen->Cursor = Save_Cursor;
}
UpdateList(0);
}
//---------------------------------------------------------------------------

void __fastcall Tmainform::PrintStatements1Click(TObject *Sender)
{
TSelection *s = new TSelection();
s->PrintStatements();
//s->DateStamp("INVOICE");
//TMData->isDirty=true;
SaveButton->Enabled = TMData->isDirty;
delete s;        
}
//---------------------------------------------------------------------------


void __fastcall Tmainform::InvDate1Click(TObject *Sender)
{
((TMenuItem*)Sender)->Checked=true;
CurrDateDisplay=((TMenuItem*)Sender)->Tag;
ListViewHeadings(CurrDateDisplay,CurrMADisplay);
reverse=false;
if (CurrDateDisplay==2) CompareField=tmd::paydatesort;
        else if(CurrDateDisplay==1) CompareField=tmd::procdatesort;
                else CompareField=tmd::dt;
PopupMenu1->AutoPopup=true;
UpdateList(0);

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::ChangePrinter1Click(TObject *Sender)
{
TPrinter* prn= Printer();
TChangePrinter* cp = new TChangePrinter(this);
cp->ComboBox1->Items = prn->Printers;
cp->ComboBox1->ItemIndex = prn->PrinterIndex;
if (cp->ShowModal()==mrOk)prn->PrinterIndex=cp->ComboBox1->ItemIndex;
delete cp;
       
}
//---------------------------------------------------------------------------
/*
void Tmainform::ExportToRTF(String fname, TList*L)
{

TCSVRec* rec = TMData->Setup;
if ((!rec)||(!L->Count)) return;

TRichEdit* pRE = new TRichEdit(this);
pRE->Parent = this;
pRE->DefAttributes->Charset=DEFAULT_CHARSET;
pRE->DefAttributes->Name="Courier";
pRE->DefAttributes->Size=12;
pRE->DefAttributes->Pitch=fpFixed;

pRE-> SelAttributes->Style << fsBold;

//font Bold 12 point courier
char * str= new char[81];
   //3 lines plus Heading 4 lines  of address
sprintf(str,"\n\n\n%-30.30s%30.30s","Dr A Manicom","P O Box 1881"); pRE->Lines->Add(str);
sprintf(str,"%-30.30s%30.30s","MBCHB FFA","Honeydew 2040"); pRE->Lines->Add(str);
sprintf(str,"%-30.30s%30.30s","Anaesthesiologist","TEL 658-1023"); pRE->Lines->Add(str);
sprintf(str,"%-30.30s%30.30s","","Fax 658-1020"); pRE->Lines->Add(str);
//vatinfo
pRE-> SelAttributes->Style >> fsBold;
sprintf(str,"\n%-30.30s%30.30s","Practice Number: 1003968","Vat Reg Number:4780103034"); pRE->Lines->Add(str);
pRE-> SelAttributes->Style << fsBold <<fsItalic;
//blank line
//BOLD ITALIC CAPITAL "TAX INVOICE"
sprintf(str,"\n%-30.30s","TAX INVOICE"); pRE->Lines->Add(str);
//blank line
//Normal Paiient name , auth 2 lines
//blank line
//CAPITAL DATE CODE  SERVICE DEBIT
//Blank line
//dateLJ codeLJ ServeLJ DebitRJ X transcount
//interest
//VAT and total
//blank blank
//Message 3 lines
//formfeed
//
pRE->Lines->SaveToFile("C:\\Tony.rtf");
delete[] str;
delete pRE;
}
*/

void __fastcall Tmainform::Bookkeeping1Click(TObject *Sender)
{
BookKeep = new TBookKeep(this,TMData->Patients);
BookKeep->ShowModal();
}
//---------------------------------------------------------------------------







//---------------------------------------------------------------------------



//---------------------------------------------------------------------------

void __fastcall Tmainform::ListView1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
PopupMenu1Popup(this);
}
//---------------------------------------------------------------------------


//Summary has variable length based on selected items in Sendsummary dialog
void __fastcall Tmainform::SendSummaryClick(TObject *Sender)
{

TList* L = new TList;
TSelection* sel = new TSelection;
for (int i=0;i<sel->Count;i++) L->Add(sel->List->Items[i]);
delete sel;

TSummaryDialog* SD = new TSummaryDialog(this);

TAccount* acc = (TAccount*)TMData->Patients->Items[0];
char* dt = DelimDate(acc->Field[tmd::dt]);
SD->DateTimePicker1->MinDate = TDateTime(dt);
SD->DateTimePicker2->MinDate = TDateTime(dt);
SD->DateTimePicker1->Date = TDateTime(dt);


SD->DateTimePicker1->MaxDate = TDateTime::CurrentDate();
SD->DateTimePicker2->MaxDate = TDateTime::CurrentDate();
SD->DateTimePicker2->Date = TDateTime::CurrentDate();

if (SD->ShowModal()==mrOk)
{
if (!SD->RadioButton1->Checked)
{
L->Clear();
for (int i=0;i<TMData->Patients->Count;i++)
{
TAccount* acc= (TAccount*)TMData->Patients->Items[i];
int compdate = atoi(revdate(acc->Field[tmd::dt]));
int date1 = StrToInt(SD->DateTimePicker1->Date.FormatString("yyyymmdd"));
int date2 = StrToInt(SD->DateTimePicker2->Date.FormatString("yyyymmdd"));
if ((compdate>=date1)&&
    (compdate<=date2))
L->Add(TMData->Patients->Items[i]);
}
}

//now make up the stringlist
TCSVRec* csv = new TCSVRec;
TStringList* SL = new TStringList;
//SL->Clear();
//first get the csvrec size and the set fields
int fieldbits = 0;
int fldcnt=0;
for (int i=0;i<SD->CheckListBox1->Items->Count;i++)
if (SD->CheckListBox1->Checked[i]){fldcnt++;fieldbits |= (1 << i);}
csv->FieldCount=fldcnt;
int fld=0;


//build the first csvrec with the field names
//0 Ref Num
//1 Folder Num
//2 Account Name
//3 Patient Name
//PatientID
//4 Account Date
//5 Procedure Date
//6 Payment Date
//7 Account Type
//8 Referral
//9 Clinic
//10 Diagnosis
//11 Medical Aid
//MedAidNum
//AuthCode
//12 Billed
//13 Discount
//Discount Date
//14 Interest
//15 Payments
//16 Balance
//17 Notes (User)
//18 Notes (All)

for (int i=0;i<SD->CheckListBox1->Items->Count;i++)
if (fieldbits & (1<<i)) csv->Field[fld++]=SD->CheckListBox1->Items->Strings[i].c_str();
SL->Add((String)csv->CSV);

//enum {pid,nc,lang,tag,pname,pbday,psex,
//atitle,ainit,asname,ad1,ad2,ad3,ad4,
//ma,man,dsc,clin,ref,sc,dt,diag,usm,auth,patid,ioddt,ddys,filenum};

//now build the data csvrecs
char* str = new char[200];

for (int i=0;i<L->Count;i++)
{
//strcpy(str,"");
TAccount* acc= (TAccount*) L->Items[i];
fld=0;
if (fieldbits & (1<<0)) csv->Field[fld++]=acc->Field[tmd::nc];
if (fieldbits & (1<<1)) csv->Field[fld++]=acc->Field[tmd::filenum];
if (fieldbits & (1<<2)) csv->Field[fld++]=acc->Field[tmd::asname];
if (fieldbits & (1<<3)) csv->Field[fld++]=acc->Field[tmd::pname];
//patid
if (fieldbits & (1<<4)) csv->Field[fld++]=acc->Field[tmd::patid];

if (fieldbits & (1<<5)) csv->Field[fld++]=DelimDate(acc->Field[tmd::dt]);
if (fieldbits & (1<<6)) csv->Field[fld++]=(acc->ProcedureDate)?DelimDate(acc->ProcedureDate):"";
if (fieldbits & (1<<7)) csv->Field[fld++]=(acc->VatDate)?DelimDate(acc->VatDate):"";
if (fieldbits & (1<<8)) {strcpy(str,"");
                        char tchar = acc->Field[tmd::sc][0];
                        if (tchar=='G') strcpy(str,"MedAid");
                        if (tchar=='C') strcpy(str,"CPT4");
                        if (tchar=='D') strcpy(str,"Discount");
                        if (tchar=='E') strcpy(str,"EDI");
                        if (tchar=='W') strcpy(str,"WCA");
                        if (tchar=='M') strcpy(str,"SAMA");
                        if (tchar=='P') strcpy(str,"Private");
                        csv->Field[fld++]= str;//acc->type;
                        }

if (fieldbits & (1<<9)) csv->Field[fld++]=acc->Field[tmd::ref];
if (fieldbits & (1<<10)) csv->Field[fld++]=acc->Field[tmd::clin];
if (fieldbits & (1<<11)) csv->Field[fld++]=acc->Field[tmd::diag];
if (fieldbits & (1<<12)) csv->Field[fld++]=acc->Field[tmd::ma];
if (fieldbits & (1<<13)) csv->Field[fld++]=acc->Field[tmd::man];
if (fieldbits & (1<<14)) csv->Field[fld++]=acc->Field[tmd::auth];
//manum
//auth

//enum {pid,nc,lang,tag,pname,pbday,psex,
//atitle,ainit,asname,ad1,ad2,ad3,ad4,
//ma,man,dsc,clin,ref,sc,dt,diag,usm,auth,patid,ioddt,ddys,filenum};




if (fieldbits & (1<<15)) csv->Field[fld++]=acc->Billed;
if (fieldbits & (1<<16))  {strcpy(str,"");
                        if (acc->Field[tmd::sc][0]=='D')
                        {
                        //char **endptr = NULL;
                        double bala = atof(acc->Balance);
                        //if (endptr) bala = 0;
                        int disco = atoi(acc->Field[tmd::dsc]);
                        bala = bala * (100-disco)/100;
                        strcpy(str,FormatFloat("0.00",bala).c_str());
                        }
                        csv->Field[fld++]=str;}
//discountdate
if (fieldbits & (1<<17))  {strcpy(str,"");
                        if (acc->Field[tmd::sc][0]=='D')
                        {
                        TDateTime dt(DelimDate(acc->Field[tmd::dt]));
                        dt += atoi(acc->Field[tmd::ddys]);
                        strcpy(str,dt.FormatString("dd/mm/yyyy").c_str());
                        }
                        csv->Field[fld++]=str;}



if (fieldbits & (1<<18)) {strcpy(str,"");
                         if (strlen(acc->Field[tmd::sc])>1)
               {
               double interest = atof(acc->IntCalc(TMData->Setup->Field[tmd::interest]));
               if (interest>0)
               strcpy(str,FormatFloat("0.00",interest).c_str());
                }
                        csv->Field[fld++]=str;}

if (fieldbits & (1<<19)) csv->Field[fld++]=acc->Payments;
if (fieldbits & (1<<20)) csv->Field[fld++]=acc->Balance;
if (fieldbits & (1<<21)) {strcpy(str,"");
                        for (int i=0;i<acc->Notes->Count;i++)
                        {
                        TCSVRec* note = (TCSVRec*)acc->Notes->Items[i];
                        char* nt = note->Field[tmd::pnote];
                        if ((nt[0] != '[')&&((strlen(str)+ strlen(nt))<190))
                        {strcat(str,nt);
                        strcat(str,"; ");}//else break;
                        }
                        csv->Field[fld++]=str;}
//acc->usernotes (done)
if (fieldbits & (1<<22)){strcpy(str,"");
                        for (int i=0;i<acc->Notes->Count;i++)
                        {
                        TCSVRec * note = (TCSVRec*)acc->Notes->Items[i];
                        char* nt = note->Field[tmd::pnote];
                        if ((strlen(str)+strlen(nt))<190)
                        {strcat(str,nt);
                        strcat(str,"; ");}else break;
                        }
                        csv->Field[fld++] = str;}
String temp("");
for (int i=0;i<fld;i++)
{temp+=String(csv->Field[i]).Trim();
if (i<(fld-1))temp+=',';}
SL->Add(temp);
}
//output the stringlist to either the clipboard or file
if (SD->RadioButton4->Checked) Clipboard()->AsText=SL->Text;
else
{
TSaveDialog* D = new TSaveDialog(this);
D->Title = "Report";
D->FileName = mainform->TMData->newreportname();
D->Filter = "CSV File (*.csv)|*.CSV";
D->DefaultExt = String("csv");
if (D->Execute()) SL->SaveToFile(D->FileName);
delete D;
}
delete[] str;
delete csv;
delete SL;
}
delete SD;
delete L;

}
//---------------------------------------------------------------------------

void __fastcall Tmainform::CompareClick(TObject *Sender)
{
TList* CompList = new TList;
String compfilename;
CompList->Clear();
TOpenDialog* D = new TOpenDialog(this);
D->Title = "Compare";
D->Filter = "Turbomed data files (*.tmd)|*.TMD";

if (D->Execute())
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;
try{
char *s = new char[MAXCSV+1];
TCSVRec *rec;
TAccount *currpat;
compfilename=D->FileName;
ifstream *in = new ifstream(D->FileName.c_str());
while (!in->eof())
{
in->getline(s,MAXCSV);

if (s[0]=='3')
{
   currpat = new TAccount;
   currpat->FieldCount = tmd::patcnt;
   currpat->CSV = s;
   CompList->Add(currpat);
}
if (s[0]=='4')
{
    rec = new TCSVRec;
    rec->FieldCount = tmd::transcnt;
    rec->CSV = s;
    currpat->Trans-> Add(rec);
}
}
in->close();
delete[] s;
delete in;
}
__finally{Screen->Cursor = Save_Cursor;}
}

delete D;

if (CompList->Count)
{
TCompareDlg* CD = new TCompareDlg(this);
CD->CompareFileName=compfilename;
CD->BuildComparison(TMData->Patients,CompList);
CD->ShowModal();
//new compareform
delete CD;
}
for (int i=0;i<CompList->Count;i++)
delete (TAccount*)CompList->Items[i];
delete CompList;
}


//---------------------------------------------------------------------------

void __fastcall Tmainform::ListView1SelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
PopupMenu1->AutoPopup=true;
}
//---------------------------------------------------------------------------




void Tmainform::quikBackup(void)
{
//backs up by renaming extention TMD to ~TMD
//only works if there is space (hard disk eg C:)
//it will only back up old data
//the backup cannot be todays date
//this guards against accidental backup of corrupt data
char dr[3];
fnsplit(filename.c_str(),dr, NULL, NULL,NULL); //get the drive ID
if (UpCase(dr[0])<'C')return;  // no backup to floppy disk

String backup = ChangeFileExt(filename,".~TMD");
int backdate = FileAge(backup);

if (backdate>-1)
if (CompareDate(TDateTime::CurrentDate(),
                FileDateToDateTime(backdate))  ==0) return;
                         //exit if a backup has already been made today
if (backdate>-1) DeleteFile(backup);   //delete if there is a backup file
RenameFile(filename,backup);
}


//void Tmainform::CheckTreeChange(void* acc)
//{
//if (!TreeChanged)
//{
//har s = ((TAccount*)acc)->Field[tmd::tag][0];

//String str="Active";
//switch (s)
//{
//case 'I': str="Inbox";break;
//case 'D': str=Year(((TAccount*)acc)->Field[tmd::dt]);break;
//case 'V': str=yyyymm(((TAccount*)acc)->VatDate);
//          str=MonthYearStr(str.c_str());break;
//}
//bool treechanged = true;
//for (int i=0;i<TreeView1->Items->Count;i++)
//{if (TreeView1->Items->Item[i]->Text==str){treechanged=false;break;}}
//TreeChanged = treechanged;
//}
//}

void Tmainform::UpdateTree(void)
{
//String str = TreeView1->Selected->Text;
allowlistupdate=false;
TreeView1->Items->BeginUpdate();

TTreeNode *recyc,*vat,*activ,*impor,*tn;
TreeView1->Items->Clear();

 impor=TreeView1->Items->Add(NULL, "Inbox");
 impor->SelectedIndex=7;
  impor->ImageIndex=6;

 activ= TreeView1->Items->Add(NULL, "Active");
 activ->SelectedIndex=3;
 activ->ImageIndex=2;

 //todo: why does the vat node have no imageindex
 vat= TreeView1->Items->Add(NULL, "VAT");
 vat->SelectedIndex=1;

 recyc = TreeView1->Items->Add(NULL, "Archive");
 recyc->SelectedIndex=5;
 recyc->ImageIndex=4;

 //

 tn = TreeView1->Items->AddChild(activ,"Current");
 tn->SelectedIndex=3; tn->ImageIndex=2;
 tn = TreeView1->Items->AddChild(activ,"30 days");
 tn->SelectedIndex=3; tn->ImageIndex=2;
 tn =TreeView1->Items->AddChild(activ,"60 days");
 tn->SelectedIndex=3; tn->ImageIndex=2;
 tn =TreeView1->Items->AddChild(activ,"90 days");
 tn->SelectedIndex=3; tn->ImageIndex=2;
 tn =TreeView1->Items->AddChild(activ,"120 days");
 tn->SelectedIndex=3; tn->ImageIndex=2;



for  (int i=0;i<TMData->Recycled->Count;i++)
{
  if (TMData->Recycled->Strings[i]!="")
  {
  tn=TreeView1->Items->AddChild(recyc,TMData->Recycled->Strings[i]);  //recyc
  tn->SelectedIndex=5; tn->ImageIndex=4;
  }
}
if (TMData->Vat->Count)
{
for  (int i=0;i<TMData->Vat->Count;i++)
{
if (TMData->Vat->Strings[i]!="")
 TreeView1->Items->AddChild(vat,MonthYearStr(TMData->Vat
 ->Strings[i].c_str()))->SelectedIndex=1;          //vat
}
}

TreeView1->Items->EndUpdate();
allowlistupdate=true;
TreeView1->Items->Item[1]->Selected = true;
}


//void Tmainform::CheckTree(void* acc)
//{
//bool update=false;
//nt x;
//TAccount* p =(TAccount*)acc;
//if (p->Field[tmd::tag][0]=='V')
//{
//char* str=yyyymm(p->VatDate);
//if (!TMData->Vat->Find(str,x))
//{TMData->Vat->Add(str); update=true;}
//}
//if (p->Field[tmd::tag][0]=='D')
//{
//char* str= Year(p->Field[tmd::dt]);
//if (!TMData->Recycled->Find(str,x))

//{TMData->Recycled->Add(str);update=true;}
//}
//if (update) UpdateTree();
//}


void Tmainform::CheckTree(TList* L)
{
bool update=false;
int x;
for (int i=0;i<L->Count;i++)
{
TAccount* p =(TAccount*)L->Items[i];
if (p->Field[tmd::tag][0]=='V')
{
char* str=yyyymm(p->VatDate);
if (!TMData->Vat->Find(str,x))
{TMData->Vat->Add(str); update=true;}
}
if (p->Field[tmd::tag][0]=='D')
{
char* str= Year(p->Field[tmd::dt]);
if (!TMData->Recycled->Find(str,x))

{TMData->Recycled->Add(str);update=true;}
}
}
if (update) UpdateTree();
}
