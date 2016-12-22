//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditSetup.h"
#include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetupForm *SetupForm;
//---------------------------------------------------------------------------
__fastcall TSetupForm::TSetupForm(TComponent* Owner)
    : TForm(Owner)
{
//Left = (Screen->Width - Width)/2;
//if (Screen->Height > Height)
//Top = (Screen->Height - Height)/2;
//else Top = 1;
Tag = 0;
//sid,h1,h2,h3,h4,h5,h6,h7,h8,h9,h10,lastdate,vatdate,mod,
//  oldvat,newvat,reg,prin,paper,disc,discdays,mednet,edibat,lastedi
TCSVRec* T = mainform->TMData->Setup;
TCSVRec* T1;


//headings
Edit1->Text = (T)?T->Field[tmd::h1]:"";
Edit2->Text = (T)?T->Field[tmd::h2]:"";
Edit3->Text = (T)?T->Field[tmd::h3]:"";
Edit9->Text = (T)?T->Field[tmd::h4]:"";
Edit5->Text = (T)?T->Field[tmd::h5]:"";
Edit6->Text = (T)?T->Field[tmd::h6]:"";
Edit7->Text = (T)?T->Field[tmd::h7]:"";
Edit8->Text = (T)?T->Field[tmd::h8]:"";


// units gpu,gcu,gcl,wpu,wcu,wcl,mpu,mcu,mcl,
//gvu,wvu,mvu, = rvu's
gcpt->Text  = (T)?toRandCents(T->Field[tmd::gvu]):(String)"";
wcpt->Text  = (T)?toRandCents(T->Field[tmd::wvu]):(String)"";
mcpt->Text  = (T)?toRandCents(T->Field[tmd::mvu]):(String)"";

Edit13->Text = (T)? toRandCents(T->Field[tmd::gpu]):(String)"";
Edit14->Text = (T)? toRandCents(T->Field[tmd::gcu]):(String)"";
Edit15->Text = (T)? toRandCents(T->Field[tmd::gcl]):(String)"";
Edit16->Text = (T)? toRandCents(T->Field[tmd::wpu]):(String)"";
Edit17->Text = (T)? toRandCents(T->Field[tmd::wcu]):(String)"";
Edit18->Text = (T)? toRandCents(T->Field[tmd::wcl]):(String)"";
Edit19->Text = (T)? toRandCents(T->Field[tmd::mpu]):(String)"";
Edit20->Text = (T)? toRandCents(T->Field[tmd::mcu]):(String)"";
Edit21->Text = (T)? toRandCents(T->Field[tmd::mcl]):(String)"";
//Edit22->Text = (T)? T->Field[rvu]:"";

//vat,reg,prin,paper,disc,discdays,mednet,edibat,lastedi
Edit23->Text = (T)?T->Field[tmd::prnum]:"";
Edit10->Text = (T)?T->Field[tmd::hpcsa]:"";
Edit24->Text = (T)?T->Field[tmd::vatnum]:"";
Edit25->Text = (T)?T->Field[tmd::vat]:"";
Edit4->Text = (T)?T->Field[tmd::interest]:"";
Edit26->Text = (T)?T->Field[tmd::mednet]:"";
Edit28->Text = (T)?T->Field[tmd::reg]:"";
strcpy(PrefStr,(T)?T->Field[tmd::pref]:"0000");
if (strlen(PrefStr)!=4)strcpy(PrefStr,"0000");
CheckBox1->Checked = (PrefStr[0]=='1');
CheckBox2->Checked = (PrefStr[1]=='1');
CheckBox3->Checked = (PrefStr[2]=='1');
CheckBox4->Checked = (PrefStr[3]=='1');
//DisDays->Text = (T)?T->Field[ddays]:"";
//DisDays->Text=IntToStr(DisDays->Text.ToIntDef(30));
//Edit4->Text = (T)? T->Field[delfil]:"";
//messages mesid,mes11,mes12,mes21,mes22
T = mainform->TMData->Message[0];
T1 = mainform->TMData->Message[1];
M1line1->Text = (T)?T->Field[tmd::ml1]:"";
M1line2->Text = (T)?T->Field[tmd::ml2]:"";
M1line3->Text = (T)?T->Field[tmd::ml3]:"";

M2line1->Text = (T1)?T1->Field[tmd::ml1]:"";
M2line2->Text = (T1)?T1->Field[tmd::ml2]:"";
M2line3->Text = (T1)?T1->Field[tmd::ml3]:"";
Tag=0;
}
//---------------------------------------------------------------------------
void TSetupForm::Save()
{

TCSVRec* T = ((Tmainform*)Owner)->TMData->Setup;
TCSVRec* T1;
if (!T) {
        T = new TCSVRec;
        T->FieldCount = tmd::setcnt;
        T->Field[tmd::edibat] = "0";
        ((Tmainform*)Owner)->TMData->Setup = T;
        }

//headings
if (T->Field[tmd::edibat]=="") T->Field[tmd::edibat]="0";
T->Field[tmd::h1]= Edit1->Text.c_str();
T->Field[tmd::h2] = Edit2->Text.c_str();
T->Field[tmd::h3] = Edit3->Text.c_str();
T->Field[tmd::h4]=  Edit9 ->Text.c_str();
T->Field[tmd::h5] = Edit5->Text.c_str();
T->Field[tmd::h6] = Edit6->Text.c_str();
T->Field[tmd::h7] = Edit7->Text.c_str();
T->Field[tmd::h8] = Edit8->Text.c_str();
// units gpu,gcu,gcl,wpu,wcu,wcl,mpu,mcu,mcl,
T->Field[tmd::gpu] = toCents(Edit13->Text);
T->Field[tmd::gcu] = toCents(Edit14->Text);
T->Field[tmd::gcl] = toCents(Edit15->Text);
T->Field[tmd::wpu] = toCents(Edit16->Text);
T->Field[tmd::wcu] = toCents(Edit17->Text);
T->Field[tmd::wcl] = toCents(Edit18->Text);
T->Field[tmd::mpu] = toCents(Edit19->Text);
T->Field[tmd::mcu] = toCents(Edit20->Text);
T->Field[tmd::mcl] = toCents(Edit21->Text);
//T->Field[rvu] = Edit22->Text.c_str();

//vat,reg,prin,paper,disc,discdays,mednet,edibat,lastedi
T->Field[tmd::prnum] = Edit23->Text.c_str();
T->Field[tmd::hpcsa] = Edit10->Text.c_str();
T->Field[tmd::vatnum]= Edit24->Text.c_str();
T->Field[tmd::vat] = Edit25->Text.c_str();
T->Field[tmd::interest] = Edit4->Text.c_str();
T->Field[tmd::mednet] = Edit26->Text.c_str();
T->Field[tmd::reg] = Edit28->Text.c_str();
T->Field[tmd::gvu] = toCents(gcpt->Text);
T->Field[tmd::wvu] = toCents(wcpt->Text);
T->Field[tmd::mvu] = toCents(mcpt->Text);
PrefStr[0]=(CheckBox1->Checked)?'1':'0';
PrefStr[1]=(CheckBox2->Checked)?'1':'0';
PrefStr[2]=(CheckBox3->Checked)?'1':'0';
PrefStr[3]=(CheckBox4->Checked)?'1':'0';
T->Field[tmd::pref] = PrefStr;
//T->Field[ddays] = DisDays->Text.c_str();
//messages mesid,mes11,mes12,mes21,mes22
T = mainform->TMData->Message[0];
T1 = mainform->TMData->Message[1];
if (!T) {
        T = new TCSVRec;
        T->FieldCount=tmd::mescnt;
    mainform->TMData->Message[0] = T;
        }
if (!T1) {
        T1 = new TCSVRec;
        T1->FieldCount=tmd::mescnt;
    mainform->TMData->Message[1] = T1;
         }

T->Field[tmd::ml1] = M1line1->Text.c_str();
T->Field[tmd::ml2] = M1line2->Text.c_str();
T->Field[tmd::ml3] = M1line3->Text.c_str();

T1->Field[tmd::ml1] = M2line1->Text.c_str();
T1->Field[tmd::ml2] = M2line2->Text.c_str();
T1->Field[tmd::ml3] = M2line3->Text.c_str();
Tag=0;
mainform->TMData->isDirty = true;

}
//---------------------------------------------------------------------------
void __fastcall TSetupForm::BitBtn1Click(TObject *Sender)
{
if (Tag)
Save();
}
//---------------------------------------------------------------------------

void __fastcall TSetupForm::BitBtn2Click(TObject *Sender)
{
Close();
//ModalResult=1;

//Close();
}
//---------------------------------------------------------------------------


void __fastcall TSetupForm::Edit1KeyPress(TObject *Sender, char &Key)
{
Tag = 1;        
}
//---------------------------------------------------------------------------


void __fastcall TSetupForm::CheckBox1Click(TObject *Sender)
{
Tag=1;        
}
//---------------------------------------------------------------------------

void __fastcall TSetupForm::FormClose(TObject *Sender,
      TCloseAction &Action)
{
if (Tag)
{
    if (MessageDlg("Save Changes ?", mtConfirmation,
              TMsgDlgButtons() << mbYes << mbNo, 0)== mrYes)
//{
Save();
//mainform->Save();
//}
}
}
//---------------------------------------------------------------------------

void __fastcall TSetupForm::Edit4Exit(TObject *Sender)
{
double x;
try{x=StrToFloat(((TEdit*)Sender)->Text);}
catch(EConvertError &e){x=0;}
((TEdit*)Sender)->Text=FormatFloat("##.##",x);
}
//---------------------------------------------------------------------------


String TSetupForm::toRandCents(char* s)
{
double d= ((double)atoi(s))/100;
return (d>0)? FormatFloat("0.00",d):(String)"";
}

//---------------------------------------------------------------------------

char* TSetupForm::toCents(String s)
{
static char* str ="00000";
double x;
try{x=StrToFloat(s);}
catch(EConvertError &e){x=0;}
x*=100;
strcpy(str,IntToStr((int)x).c_str());
return str; // (x>0)?IntToStr((int)x).c_str():"";
}

//---------------------------------------------------------------------------
void __fastcall TSetupForm::Edit1Change(TObject *Sender)
{
Tag=1;
}
//---------------------------------------------------------------------------

