//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MedAid.h"
#include "tmFile.h"
#include "main.h"
#include "MedAidEd.h"
#include "LabelPage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMedAidList *MedAidList;

//---------------------------------------------------------------------------
__fastcall TMedAidList::TMedAidList(TComponent* Owner)
    : TForm(Owner)
{

MedaidView->Columns->Clear();
TListColumn  *NewColumn;

  NewColumn = MedaidView->Columns->Add();
  NewColumn->Caption = "Name";
  NewColumn->Width = 100;
  NewColumn = MedaidView->Columns->Add();
  NewColumn->Caption = "Plan";
  NewColumn->Width=60;

  NewColumn = MedaidView->Columns->Add();
  NewColumn->Caption = "Telephone";
  NewColumn->Width=100;

  NewColumn = MedaidView->Columns->Add();
  NewColumn->Caption = "Fax";
  NewColumn->Width=100;
LoadData();
}
//---------------------------------------------------------------------------
//{maid,maname,mplan,maddr1,maddr2,maddr3,maddr4,mtel,
//       mfax,mproc,mcons,mclin,medi};
void TMedAidList::LoadData()
{
MedaidView->Hide();
MedaidView->Items->Clear();
TListItem *ListItem;
TCSVRec *L;
for (int i=0;i<mainform->TMData->Medaids->Count;i++)
{
 L = (TCSVRec*)mainform->TMData->Medaids->Items[i];
 ListItem = MedaidView->Items->Add();
    ListItem->Data = L;

    ListItem->Caption = L->Field[tmd::maname];
    ListItem->SubItems->Add(L->Field[tmd::mplan]);
    ListItem->SubItems->Add(L->Field[tmd::mtel]);
    ListItem->SubItems->Add(L->Field[tmd::mfax]);
  }
MedaidView->Show();
}


//---------------------------------------------------------------------------
//void TLibForm::StoreData()
//{
//TCSVRec *L;
//}
//---------------------------------------------------------------------------


void __fastcall TMedAidList::SpeedButton1Click(TObject *Sender)
{
if (MedaidView->ItemFocused)
{
int F = MedaidView->ItemFocused->Index;
int ht  =
MedaidView->TopItem->DisplayRect(drSelectBounds).bottom -
MedaidView->TopItem->DisplayRect(drSelectBounds).top;
int scrollpos = MedaidView->TopItem->Index;

MedAidEdForm = new TMedAidEdForm(MedaidView);
MedAidEdForm->ShowModal();
delete MedAidEdForm;
LoadData();
MedaidView->Items->Item[F]->Selected=true;
MedaidView->Items->Item[F]->Focused=true;
MedaidView->Scroll(0,ht*scrollpos);
MedaidView->SetFocus();
}
}
//---------------------------------------------------------------------------

void __fastcall TMedAidList::SpeedButton2Click(TObject *Sender)
{
if (MedaidView->ItemFocused)
MedaidView->ItemFocused->Focused=false;
MedAidEdForm = new TMedAidEdForm(MedaidView);
MedAidEdForm->ShowModal();
delete MedAidEdForm;
LoadData();
}
//---------------------------------------------------------------------------


void __fastcall TMedAidList::MedaidViewKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
/*
if (MedaidView->ItemFocused)
switch (Key)
{
case VK_DELETE:
if (MessageDlg("Delete Record?", mtWarning,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{TCSVRec*R = (TCSVRec*)MedaidView->ItemFocused->Data;
mainform->TMData->Medaids->Remove((void*)R);
delete R;
mainform->TMData->isDirty=true;
LoadData();}break;

case VK_RETURN: SpeedButton1Click(this);break;
}
*/
}

//---------------------------------------------------------------------------

void __fastcall TMedAidList::SpeedButton3Click(TObject *Sender)
{
TlabelPg *pg = new TlabelPg();
for (int i=0;i<MedaidView->Items->Count;i++)
if (MedaidView->Items->Item[i]->Selected)
{
TCSVRec* R = (TCSVRec*)MedaidView->Items->Item[i]->Data;
pg->Labels->Add(R->Field[tmd::maname]);
}
pg->Validate();
if (pg->Labels->Count) pg->PrintLabels();
else ShowMessage("No Labels Available");
delete pg;
}

//---------------------------------------------------------------------------

void __fastcall TMedAidList::Delete1Click(TObject *Sender)
{
if (MessageDlg("Delete Record?", mtWarning,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
{
TCSVRec*R = (TCSVRec*)MedaidView->ItemFocused->Data;
mainform->TMData->Medaids->Remove((void*)R);
delete R;
mainform->TMData->isDirty=true;
LoadData();
}
}
//---------------------------------------------------------------------------

void __fastcall TMedAidList::PopupMenu1Popup(TObject *Sender)
{
if (MedaidView->ItemFocused)
{New1->Enabled=true;Delete1->Enabled=true;}
else {New1->Enabled=false;Delete1->Enabled=false;}
}
//---------------------------------------------------------------------------

