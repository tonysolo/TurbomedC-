//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Library.h"
#include "tmFile.h"
#include "main.h"
#include "A5Pg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLibForm *LibForm;

//---------------------------------------------------------------------------
__fastcall TLibForm::TLibForm(TComponent* Owner)
    : TForm(Owner)
{

fcompare = tmd::lcode;
freverse = false;
for (int i=0;i< mainform->TMData->Library->Count;i++)
  {
 TCSVRec*   L = (TCSVRec*)mainform->TMData->Library->Items[i];
    L->Field[0]="0";
  }
}
//---------------------------------------------------------------------------

void TLibForm::LoadData()
{
TCursor Save_Cursor = Screen->Cursor;
Screen->Cursor = crHourGlass;

Libview->Hide();
Libview->Items->Clear();
TListItem *ListItem;
char *str;
TCSVRec *L;
for (int i=0;i< mainform->TMData->Library->Count;i++)
  {
    L = (TCSVRec*)mainform->TMData->Library->Items[i];
    ListItem = Libview->Items->Add();
    if (Libview->Checkboxes)

    ListItem->Checked = (L->Field[0][0]!='0');
    ListItem->Data = L;

    ListItem->Caption = L->Field[1];
    switch (L->Field[2][0])
    {
    case 'P': str = "Proc";break;
    case 'L': str = "Clin";break;
    case 'C': str = "Cons";break;
    case 'R': str = "CPT4";break;
    case 'I': str = "ICD10";break;
    }
    ListItem->SubItems->Add(str);

    ListItem->SubItems->Add(L->Field[3]);
    ListItem->SubItems->Add(L->Field[4]);
    ListItem->SubItems->Add(L->Field[5]);
  }
Libview->Show();
Screen->Cursor = Save_Cursor;
}

//---------------------------------------------------------------------------

void __fastcall TLibForm::FormCreate(TObject *Sender)
{
Libview->Columns->Clear();
TListColumn  *NewColumn;

  NewColumn = Libview->Columns->Add();
  NewColumn->Caption = "Code";
  NewColumn->Width = 50;
  NewColumn = Libview->Columns->Add();
  NewColumn->Caption = "Type";
  NewColumn->Width=40;

  NewColumn = Libview->Columns->Add();
  NewColumn->Caption = "Value";
  NewColumn->Width=40;

  NewColumn = Libview->Columns->Add();
  NewColumn->Caption = "Language 1";
  NewColumn->Width=100;

  NewColumn = Libview->Columns->Add();
  NewColumn->Caption = "Language 2";
  NewColumn->Width=100;

  TCSVRec *L;
for (int i=0;i< mainform->TMData->Library->Count;i++)
  {
    L = (TCSVRec*)mainform->TMData->Library->Items[i];
    L->Field[0]="0";//use the first field to check mark selections
  }
  }
//---------------------------------------------------------------------------

void __fastcall TLibForm::SpeedButton1Click(TObject *Sender)
{

//LibEditForm = new TLibEditForm(this,Libview
//                ->Items->IndexOf(Libview->Selected),m_lib);
if (Libview->ItemFocused)
{
int F = Libview->ItemFocused->Index;
int ht  =
Libview->TopItem->DisplayRect(drSelectBounds).bottom -
Libview->TopItem->DisplayRect(drSelectBounds).top;
int scrollpos = Libview->TopItem->Index;
//int Curr =(int)Libview->ItemFocused->Data;
TLibEditForm * LibEditForm = new TLibEditForm(Libview);
LibEditForm->ShowModal();
LoadData();
Libview->Items->Item[F]->Selected=true;
Libview->Items->Item[F]->Focused=true;
Libview->Scroll(0,ht*scrollpos);
delete LibEditForm;
Libview->SetFocus();
}
}
//---------------------------------------------------------------------------

void __fastcall TLibForm::SpeedButton2Click(TObject *Sender)
{

TListItem* L;
L= Libview->ItemFocused;
if (L) L->Focused=false;
TLibEditForm* LibEditForm = new TLibEditForm(Libview);
LibEditForm->ShowModal();
delete LibEditForm;
LoadData();
}
//---------------------------------------------------------------------------


void __fastcall TLibForm::LibviewDblClick(TObject *Sender)
{
//int t = LibForm->Tag;
//if (t)
SpeedButton1Click(Sender);
//else
//{
//LibForm->Tag = (int)Libview->ItemFocused->Data;
//LibForm->ModalResult=mrCancel;
//}
}

//---------------------------------------------------------------------------


void __fastcall TLibForm::LibviewCompare(TObject *Sender, TListItem *Item1,
      TListItem *Item2, int Data, int &Compare)
{
char *R1,*R2;
R1= ((TCSVRec*)Item1->Data)->Field[fcompare];
R2= ((TCSVRec*)Item2->Data)->Field[fcompare];
Compare = (freverse)? stricmp(R2,R1):stricmp(R1,R2);
}
//---------------------------------------------------------------------------

void __fastcall TLibForm::LibviewColumnClick(TObject *Sender,
      TListColumn *Column)
{
switch (Column->Index)
{
//lid,lcode,cp,un,eng,afr
case 0: if (fcompare==tmd::lcode)
    freverse=!freverse;
     else {fcompare=tmd::lcode;freverse=false;}
      break;
case 1:if (fcompare==tmd::cp)
    freverse=!freverse;
     else {fcompare=tmd::cp;freverse=false;}
     break;
//case 2:if (fcompare==un)
//    freverse=!freverse;
//     else {fcompare=un;freverse=false;}
//     break;
case 3: if (fcompare==tmd::leng)
    freverse=!freverse;
     else {fcompare=tmd::leng;freverse=false;}
     break;
case 4: if (fcompare==tmd::lafr)
    freverse=!freverse;
     else {fcompare=tmd::lafr;freverse=false;}
     break;
}
LoadData();
}
//---------------------------------------------------------------------------







void __fastcall TLibForm::LibviewClick(TObject *Sender)
{
//if ((Libview->Checkboxes)&&(Libview->ItemFocused))
//{
//TCSVRec*T = (TCSVRec*)Libview->ItemFocused->Data;
//if (T)
//if (Libview->ItemFocused->Checked)T->Field[0]="1";
//else T->Field[0]="0";
//{
//if (T->Field[0][0]=='0')
//        {T->Field[0]="1";Libview->ItemFocused->Checked=true;}
//else    {T->Field[0]="0";Libview->ItemFocused->Checked=false;}
//}
//}
for (int i=0;i<Libview->Items->Count;i++)
{
if   (Libview->Items->Item[i]->Checked)
     ((TCSVRec*)Libview->Items->Item[i]->Data)->Field[0]="1";
else ((TCSVRec*)Libview->Items->Item[i]->Data)->Field[0]="0";
}
//TCSVRec* L
//for (int i=0;i< mainform->TMData->Library->Count;i++)
//  {
//    L = (TCSVRec*)mainform->TMData->Library->Items[i];
//    L->Field[0]="0";/
}
//---------------------------------------------------------------------------


void __fastcall TLibForm::LibviewKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
/*
if (Libview->ItemFocused)
switch(Key)
{
case VK_DELETE:
if (MessageDlg("Delete Code?", mtWarning,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{TCSVRec*R = (TCSVRec*)Libview->ItemFocused->Data;
mainform->TMData->Library->Remove((void*)R);
delete R;
mainform->TMData->isDirty=true;
LoadData();}break;
case VK_RETURN: SpeedButton1Click(this);break;
}
*/
}
//---------------------------------------------------------------------------


void __fastcall TLibForm::SpeedButton3Click(TObject *Sender)
{
TA5Page *pg = new TA5Page();
if (Application->MessageBox("Print Code Library","TurboMED",
MB_OKCANCEL)==IDOK)
{
pg->NewPage();
pg->Heading="Code List";
pg->Write(2,1,"PROCEDURE CODE LIST",TA5Page::LJ);
pg->Separator(2);
pg->Write(2,3,"CODE",TA5Page::LJ);
pg->Write(15,3,"TYPE",TA5Page::LJ);
pg->Write(25,3,"VALUE",TA5Page::LJ);
pg->Write(40,3,"LANGUAGE-1",TA5Page::LJ);
pg->Write(70,3,"LANGUAGE-2",TA5Page::LJ);
//pg->Write(95,3,"BALANCE",TA5Page::RJ);
pg->NextLine(4);

for (int i=0;i<Libview->Items->Count;i++)
{
//lid,lcode,cp,un,leng,lafr
int n = pg->NextLine(0);
char* str;
TCSVRec* L = (TCSVRec*)Libview->Items->Item[i]->Data;
switch (L->Field[tmd::cp][0])
    {
    case 'P': str = "Proc";break;
    case 'L': str = "Clin";break;
    case 'C': str = "Cons";break;
    case 'R': str = "CPT4";break;
    case 'I': str = "ICD10";break;
    }
pg->Write(2,n,L->Field[tmd::lcode],TA5Page::LJ);
pg->Write(15,n,str,TA5Page::LJ);
pg->Write(25,n,L->Field[tmd::un],TA5Page::LJ);
pg->Write(40,n,L->Field[tmd::leng],TA5Page::LJ);
pg->Write(70,n,L->Field[tmd::lafr],TA5Page::LJ);
}
pg->Finish();
delete pg;
}
}
//---------------------------------------------------------------------------

void __fastcall TLibForm::Delete1Click(TObject *Sender)
{
if (MessageDlg("Delete Code?", mtWarning,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{TCSVRec*R = (TCSVRec*)Libview->ItemFocused->Data;
mainform->TMData->Library->Remove((void*)R);
delete R;
mainform->TMData->isDirty=true;
LoadData();//}break;
}
}
//---------------------------------------------------------------------------

void __fastcall TLibForm::PopupMenu1Popup(TObject *Sender)
{
if (Libview->ItemFocused)
{Edit1->Enabled=true;Delete1->Enabled=true;}
else {Edit1->Enabled=false;Delete1->Enabled=false;}
}
//---------------------------------------------------------------------------

