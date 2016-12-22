//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include "find.h"
#include "CSVRec.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindForm *FindForm;
//---------------------------------------------------------------------------
__fastcall TFindForm::TFindForm(TComponent* Owner)
        : TForm(Owner)
{
FoundList = new TList;
prompt = new char[100];
}
//---------------------------------------------------------------------------


void __fastcall TFindForm::FormCreate(TObject *Sender)
{

TListColumn  *NewColumn;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Folder";
  NewColumn->Width = 50;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Ref";
  NewColumn->Width=40;
  NewColumn->Alignment=taRightJustify;

 NewColumn = ListView1->Columns->Add();
 NewColumn->Caption = "File#";
  NewColumn->Width=40;
  NewColumn->Alignment=taRightJustify;
  
  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Account";
  NewColumn->Width=100;
  NewColumn->Alignment=taLeftJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Patient";
  NewColumn->Width=100;
  NewColumn->Alignment=taLeftJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Balance";
  NewColumn->Width=60;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Discount";
  NewColumn->Width=60;
  NewColumn->Alignment=taRightJustify;

  NewColumn = ListView1->Columns->Add();
  NewColumn->Caption = "Payment";
  NewColumn->Width=60;
  NewColumn->Alignment=taRightJustify;
//  strcpy(prompt," Enter name or reference number to find");
//  Panel1->Caption=prompt;
    //TODO: Add your source code here
}
//---------------------------------------------------------------------------
void __fastcall TFindForm::SearchClick(TObject *Sender)
{
BuildList();
DisplayList();
DisplayPrompt();

}
//---------------------------------------------------------------------------


void TFindForm::BuildList()
{

TAccount* acc;
TList* accs = mainform->TMData->Patients;
char* sstr = new char[35];
char* str = new char[9];
char* ssstr = new char[35];
char* tstr1 = new char[35];
char* tstr2 = new char[35];
strcpy(sstr,Edit1->Text.c_str());
sprintf(ssstr," %s",sstr);
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;

try
{
FoundList->Clear();
for (int i=0;i<accs->Count;i++)
{
acc = (TAccount*)accs->Items[i];
strncpy(tstr1,acc->Field[tmd::asname],34);
strncpy(tstr2,acc->Field[tmd::pname],34);

char **endptr = NULL;
double bala = strtod(acc->Balance,endptr);
if (endptr) bala = 0;
if (acc->Field[tmd::sc][0]=='D')
{
int disco = atoi(acc->Field[tmd::dsc]);
bala = bala * (100-disco)/100;
sprintf(str,"%8.2f",bala);
}
else strcpy(str,"");

if ((strnicmp(tstr1,sstr,strlen(sstr))==0)||
    (strnicmp(tstr2,sstr,strlen(sstr))==0)||
    (strcmp(acc->Field[tmd::nc],sstr)==0)||
    (strcmp(acc->Field[tmd::filenum],sstr)==0)||
    (strstr(strupr(tstr1),strupr(ssstr)))||
    (strstr(strupr(tstr2),strupr(ssstr)))||
    (strstr(acc->Balance,sstr))||
    (strstr(acc->Payments,sstr))||
    (strstr(str,sstr)))

FoundList->Add(acc);
}
}
__finally
{
Screen->Cursor = Save_Cursor;
delete[] str;
delete[] sstr;
delete[] ssstr;
delete[] tstr1;
delete[] tstr2;
}
}

//---------------------------------------------------------------------------

void TFindForm::DisplayList()
{

//todo displays discount incorrectly  
char str[9];
TListItem* ListItem;
TAccount* acc;
ListView1->Items->Clear();

for (int i=0;i<FoundList->Count;i++)
{
acc = (TAccount*)FoundList->Items[i];
char **endptr = NULL;
double bala = strtod(acc->Balance,endptr);
if (endptr) bala = 0;
int disco = atoi(acc->Field[tmd::dsc]);
if (acc->Field[tmd::sc][0]=='D')
{
bala = bala * (100-disco)/100;
sprintf(str,"%8.2f",bala);
}
else strcpy(str,"");
ListItem = ListView1->Items->Add();
ListItem->Caption = FolderName(acc->Field[tmd::tag]);
ListItem->SubItems->Add(acc->Field[tmd::nc]);
ListItem->SubItems->Add(acc->Field[tmd::filenum]);
ListItem->SubItems->Add(acc->Field[tmd::asname]);
ListItem->SubItems->Add(acc->Field[tmd::pname]);
ListItem->SubItems->Add(acc->Balance);
ListItem->SubItems->Add(str);//acc->Field[tmd::dsc]);
ListItem->SubItems->Add(acc->Payments);
ListItem->Data=acc;
}
}



char* TFindForm::FolderName(char* s)
{
if (s[0]=='V')return "VAT";
if (s[0]=='D')return "Archive";
if (s[0]=='I')return "Inbox";
return "Active";
}

void __fastcall TFindForm::ListView1DblClick(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------


__fastcall TFindForm::~TFindForm()
{
delete FoundList;
delete[] prompt;
}


void __fastcall TFindForm::ListView1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if (Key==VK_RETURN) Close();
}
//---------------------------------------------------------------------------


void TFindForm::DisplayPrompt(void)
{
int i = ListView1->Items->Count;
if (i)
sprintf(prompt," %d Items Found - Double click in list to locate account",i);
else
strcpy(prompt,"    ---No items found---");
Panel1->Caption=prompt;
}

void __fastcall TFindForm::Edit1Enter(TObject *Sender)
{
strcpy(prompt," Enter name, reference or file number or amount to find");
Panel1->Caption=prompt;
Button1->Enabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TFindForm::ListView1Click(TObject *Sender)
{
Button1->Enabled=true;
}
//---------------------------------------------------------------------------

