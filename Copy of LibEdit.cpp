//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "LibEdit.h"
//#include "TTokbase.h"
#include "CSVRec.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//TLibEditForm *LibEditForm;
//---------------------------------------------------------------------------
__fastcall TLibEditForm::TLibEditForm(TComponent* Owner)
//,
                                     //    int nx,
                                     //   TList *L)
    : TForm(Owner)                   //,m_n(nx),m_lib(L)
{
Tag=0;
//TListItem* T = ((TListView*)Owner)->ItemFocused;
Load();
}

void TLibEditForm::Load()
{
TListItem* l = ((TListView*)Owner)->ItemFocused;
ClearFields();
//if (m_n <0 ) Label5->Caption="New Library Code";
if (!l) Label5->Caption="New Library Code";

else {
      TCSVRec* R = (TCSVRec*) l->Data;
      Edit1->Text=R->Field[tmd::lcode];
      Edit2->Text=R->Field[tmd::un];
      Edit3->Text=R->Field[tmd::leng];
      Edit4->Text=R->Field[tmd::lafr];
  char c = R->Field[tmd::cp][0];
      switch (c)
      {
      case 'P':ComboBox1->ItemIndex=0;break;
      case 'C':ComboBox1->ItemIndex=2;break;
      case 'L':ComboBox1->ItemIndex=1; break;
      case 'R':ComboBox1->ItemIndex=3; break;
      case 'I':ComboBox1->ItemIndex=4; break;
      }
   //   lid,lcode,cp,un,prompt,nappi
    }
}

void TLibEditForm::ClearFields()
{
      Edit1->Text="";
      Edit2->Text="";
      Edit3->Text="";
      Edit4->Text="";
      ComboBox1->ItemIndex=0;
}
//---------------------------------------------------------------------------
void __fastcall TLibEditForm::BitBtn4Click(TObject *Sender)
{
TListView *Own = (TListView*)Owner;
  if (!Tag)
{if (Own->ItemFocused)
    {
     int i= Own->ItemFocused->Index;
     Own->Items->Item[(i>0)? --i
     :0]->Focused=true;
     ClearFields();
     Load();
    }
}
}

//---------------------------------------------------------------------------

void __fastcall TLibEditForm::BitBtn3Click(TObject *Sender)
{
TListView *Own = (TListView*)Owner;
  if (!Tag)
{if (Own->ItemFocused)
    {
     int i= Own->ItemFocused->Index;
     Own->Items->Item[(i<Own->Items->Count-2)? ++i
     :Own->Items->Count-1]->Focused=true;
     ClearFields();
     Load();
    }
}
}
//---------------------------------------------------------------------------

void __fastcall TLibEditForm::BitBtn2Click(TObject *Sender)
{
Close();

}
//---------------------------------------------------------------------------

void __fastcall TLibEditForm::BitBtn5Click(TObject *Sender)
{
if (Tag) Save();
}
//---------------------------------------------------------------------------

void __fastcall TLibEditForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
if(Key!=VK_TAB)Tag=1;
}
//---------------------------------------------------------------------------


void TLibEditForm::Save()
{

TListItem *T = ((TListView*)Owner)->ItemFocused;
TCSVRec*R;
if (!T)
  {
   R= new TCSVRec;
   R->FieldCount = tmd::libcnt;
   mainform->TMData->Library->Add(R);
  }
else  R =(TCSVRec*)T->Data;
      R->Field[tmd::lid] = "2";
      R->Field[tmd::lcode]= Edit1->Text.c_str();
      R->Field[tmd::un]=Edit2->Text.c_str();
      R->Field[tmd::leng]=Edit3->Text.c_str();
      R->Field[tmd::lafr]=Edit4->Text.c_str();
     // R->Field[cp]=" ";
      switch (ComboBox1->ItemIndex)
      {
      case 0:R->Field[tmd::cp]="P"; break;
      case 1:R->Field[tmd::cp]="L"; break;
      case 2:R->Field[tmd::cp]="C"; break;
      case 3:R->Field[tmd::cp]="R"; break;
      case 4:R->Field[tmd::cp]="I"; break;
      }
 Tag = 0;
 mainform->TMData->isDirty = true;
}



void __fastcall TLibEditForm::Edit1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Tag = 1;        
}
//---------------------------------------------------------------------------


void __fastcall TLibEditForm::ComboBox1DropDown(TObject *Sender)
{
Tag = 1;        
}
//---------------------------------------------------------------------------

void __fastcall TLibEditForm::Edit1KeyPress(TObject *Sender, char &Key)
{
Tag = 1;        
}
//---------------------------------------------------------------------------


void __fastcall TLibEditForm::FormClose(TObject *Sender,
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

void __fastcall TLibEditForm::Edit2Exit(TObject *Sender)
{
try {Edit2->Text.ToDouble();}
catch (EConvertError &) { Edit2->Text="0";}
}
//---------------------------------------------------------------------------

