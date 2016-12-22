//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MedAidEd.h"
//#include "TTokbase.h"
#include "CSVRec.h"
#include"main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMedAidEdForm *MedAidEdForm;
//---------------------------------------------------------------------------
__fastcall TMedAidEdForm::TMedAidEdForm(TComponent* Owner)
    : TForm(Owner)
{
LoadForm();
Tag = 0;
}

//enum {maname,mplan,maddr1,maddr2,maddr3,maddr4,mtel,mfax,mproc,mcons,mclin,mrvu,medi}
//---------------------------------------------------------------------------
void TMedAidEdForm::ClearForm()
{
for (int i=0;i<ComponentCount ;i++)

if (Components[i]->ClassNameIs("TEdit"))
((TEdit*)Components[i])->Text="";
}


//---------------------------------------------------------------------------
void TMedAidEdForm::LoadForm()
{
TListItem* T =  ((TListView*)Owner)->ItemFocused;
ClearForm();
if (!T) {Label5->Caption="New Medical Aid";
         Edit2->Text="PO Box";}
else {
TCSVRec* R= (TCSVRec*)T->Data;

      Edit1->Text=R->Field[tmd::maname];
      Edit2->Text=R->Field[tmd::maddr1];
      Edit3->Text=R->Field[tmd::maddr2];
      Edit4->Text=R->Field[tmd::maddr3];
      Edit5->Text=R->Field[tmd::maddr4];
      Edit6->Text=R->Field[tmd::mtel];
      Edit7->Text=R->Field[tmd::mfax];
      Edit8->Text=R->Field[tmd::mproc];
      Edit9->Text=R->Field[tmd::mcons];
      Edit10->Text=R->Field[tmd::mclin];
      Edit11->Text=R->Field[tmd::mrvu];
      Edit12->Text=R->Field[tmd::medi];
      Edit13->Text=R->Field[tmd::mplan];

    }
}
//---------------------------------------------------------------------------
void __fastcall TMedAidEdForm::BitBtn4Click(TObject *Sender)
{
if (!Tag)
{
TListItems* L = ((TListView*)Owner)->Items;
TListItem* T = ((TListView*)Owner)->ItemFocused;
if (T)
{
int i = T->Index;
if (i)
    {
    i--;
    T->Focused=false;
    L->Item[i]->Focused=true;
    LoadForm();
    }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TMedAidEdForm::BitBtn3Click(TObject *Sender)
{
if(!Tag)
{
TListItems* L = ((TListView*)Owner)->Items;
TListItem* T = ((TListView*)Owner)->ItemFocused;
if (T)
{
int i = T->Index;
if (i < L->Count - 1)
    {
    i++;
    T->Focused=false;
    L->Item[i]->Focused=true;
    LoadForm();
    }
}
}
}
//---------------------------------------------------------------------------

void __fastcall TMedAidEdForm::BitBtn5Click(TObject *Sender)
{
Save();
}

//---------------------------------------------------------------------------

void __fastcall TMedAidEdForm::BitBtn2Click(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------


void TMedAidEdForm::Save()
{
TCSVRec *R;
TListItem* T = ((TListView*)Owner)->ItemFocused;
if (!T)
  {
  R = new  TCSVRec;
  R->FieldCount = tmd::macnt;
  mainform->TMData->Medaids->Add(R);
  }
else  R =(TCSVRec*)T->Data;

      R->Field[tmd::maname]  =(Edit1->Text.IsEmpty())?"":Edit1->Text.c_str();
      R->Field[tmd::maddr1]  =(Edit2->Text.IsEmpty())?"":Edit2->Text.c_str();
      R->Field[tmd::maddr2]  =(Edit3->Text.IsEmpty())?"":Edit3->Text.c_str();
      R->Field[tmd::maddr3]  =(Edit4->Text.IsEmpty())?"":Edit4->Text.c_str();
      R->Field[tmd::maddr4]  =(Edit5->Text.IsEmpty())?"":Edit5->Text.c_str();
      R->Field[tmd::mtel]    =(Edit6->Text.IsEmpty())?"":Edit6->Text.c_str();
      R->Field[tmd::mfax]    =(Edit7->Text.IsEmpty())?"":Edit7->Text.c_str();
      R->Field[tmd::mproc]   =(Edit8->Text.IsEmpty())?"":Edit8->Text.c_str();
      R->Field[tmd::mcons]   =(Edit9->Text.IsEmpty())?"":Edit9->Text.c_str();
      R->Field[tmd::mclin]   =(Edit10->Text.IsEmpty())?"":Edit10->Text.c_str();
      R->Field[tmd::mrvu]    =(Edit11->Text.IsEmpty())?"":Edit11->Text.c_str();
      R->Field[tmd::medi]    =(Edit12->Text.IsEmpty())?"":Edit12->Text.c_str();
      R->Field[tmd::mplan]   =(Edit13->Text.IsEmpty())?"":Edit13->Text.c_str();

mainform->TMData->isDirty = true;
Tag = 0;
}



void __fastcall TMedAidEdForm::BitBtn1Click(TObject *Sender)
{
//Close();
}
//---------------------------------------------------------------------------

void __fastcall TMedAidEdForm::Edit1KeyPress(TObject *Sender, char &Key)
{
Tag = 1;        
}
//---------------------------------------------------------------------------


void __fastcall TMedAidEdForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
if (Tag)
{
if (MessageDlg("Save Changes ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{
Save();
mainform->Save();
}
}
}
//---------------------------------------------------------------------------

