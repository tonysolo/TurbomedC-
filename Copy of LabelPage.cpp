//---------------------------------------------------------------------------
#include <printers.hpp>
#include <stdio.h>
#include <typeinfo.h>
#pragma hdrstop
#include "LabelPage.h"
#include "main.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
TlabelPg::TlabelPg()
{
Labels = new TList;
SaveFont = Printer()->Canvas->Font;
Printer()->Canvas->Font->Name = "Arial";
Printer()->Canvas->Font->Size=12;
Validated=false;
}
//---------------------------------------------------------------------------
//char str[101];
//float inches = Printer()->PageHeight/
//Printer()->Canvas->Font->PixelsPerInch;
//sprintf(str,"%6.2f",inches);
//int y = 100;
//GetWindowsDirectory(str,y);
//LPSTR lpszSysPath;	/* address of buffer for Windows directory	*/
//UINT cbSysPath;	/* size of directory buffer	*/
//Application->MessageBox(str, "Message Box", MB_OKCANCEL +
//    MB_DEFBUTTON1);
//inches = Printer()->PageHeight/60;
//Printer()->Canvas->Font->PixelsPerInch;
//sprintf(str,"%6.2f",inches);
//Application->MessageBox(str, "Message Box", MB_OKCANCEL +
//    MB_DEFBUTTON1);


//---------------------------------------------------------------------------
TlabelPg::~TlabelPg()
{
Printer()->Canvas->Font=SaveFont;
delete Labels;
}
//---------------------------------------------------------------------------

int TlabelPg::VerticalOffset(int lpos)
{
int offs=Printer()->Canvas->TextHeight("A");
int vpos = lpos/3;
return Printer()->PageHeight/8* vpos +(offs*2);
    //TODO: Add your source code here
}
//---------------------------------------------------------------------------

int TlabelPg::HorizOffset(int lpos)
{
int offs=Printer()->Canvas->TextHeight("A");
int hpos = lpos %3;
return Printer()->PageWidth/3* hpos + (offs*2);
    //TODO: Add your source code here
}
//---------------------------------------------------------------------------

void TlabelPg::PrintLabel(void* rec, int labelpos)
{

TCSVRec* R = (TCSVRec*) rec;
char* str = new char[strlen(R->Field[tmd::maddr3])
                     +strlen(R->Field[tmd::maddr4])+3];
sprintf(str,"%s %s",R->Field[tmd::maddr3],
                    R->Field[tmd::maddr4]);
int x=HorizOffset(labelpos);
int y=VerticalOffset(labelpos);
int z=Printer()->Canvas->TextHeight(R->Field[tmd::maname]);

//str=R->Field[tmd::maname];
//str=R->Field[tmd::maddr1];
//str=R->Field[tmd::maddr2];

Printer()->Canvas->TextOut(x,y,R->Field[tmd::maname]);
Printer()->Canvas->TextOut(x,y+z,"Claims Department");
Printer()->Canvas->TextOut(x,y+(2*z),R->Field[tmd::maddr1]);
Printer()->Canvas->TextOut(x,y+(3*z),R->Field[tmd::maddr2]);
Printer()->Canvas->TextOut(x,y+(4*z),str);
//Printer()->Canvas->TextOut(x,y+(4*z),R->Field[tmd::maddr4]);
delete[] str;
}

//---------------------------------------------------------------------------

void TlabelPg::PrintLabels()
{
if ((Validated)&&(Labels->Count))
{
char* str = new char[40];
sprintf(str,"Print %d Medical Aid Label(s)",Labels->Count);
if (MessageDlg(str, mtConfirmation,
              TMsgDlgButtons() << mbOK << mbCancel, 0)== mrOk)
{
Printer()->BeginDoc();
//Printer()->Canvas->Rectangle(0,0,
//        Printer()->PageWidth,
//        Printer()->PageHeight);

void* rec;
int i=0;
int rem = Labels->Count;

//first print 24 at a time until less than 24 remain

if (Labels->Count > 24)
{
while (i < (Labels->Count-24))
{
Application->MessageBox("Position Labels for Printing",
"Message Box", MB_OK);
for (int j=0;j<24;j++,i++)
        {
rec = Labels->Items[i];
PrintLabel(rec,j);
        }
Printer()->NewPage();
//eject page
}
i = Labels->Count - 24;//
rem = 24;   //then set up so the last 24 are printed
}
//else repeat printing items if the list is small
Application->MessageBox("Position Labels for Printing",
"Message Box", MB_OK);
for (int j=0;j<24;j++,i++)
{
rec = Labels->Items[i%rem];
PrintLabel(rec,j);
}
Printer()->EndDoc();
}
delete[] str;
}

}
//---------------------------------------------------------------------------

char* TlabelPg::Administers(char* s)
{
static char str[40];
strcpy(str,s);
strtok(str," "); // copy up to first space
TCSVRec* R=NULL;
TList* L = mainform->TMData->Medaids;
for (int i=0;i<L->Count;i++)
{
TCSVRec* T = (TCSVRec*) L->Items[i];
if (!strnicmp(T->Field[tmd::maname],str,strlen(str)))
{R = T ;break;}
}
if ((R)&&(R->Field[tmd::maddr1][0]=='('))
{
strcpy(str,&R->Field[tmd::maddr1][1]);
strtok(str,")");
}
return str;
}
//---------------------------------------------------------------------------

void* TlabelPg::AddressData(char*s)
{
TCSVRec* R=NULL;
TList* L = mainform->TMData->Medaids;
for (int i=0;i<L->Count;i++)
{
TCSVRec* T = (TCSVRec*)L->Items[i];
if (!strnicmp(T->Field[tmd::maname],s,strlen(s)))
{R = T ;break;}
}
return (void*)R;
}
//---------------------------------------------------------------------------

void TlabelPg::Validate(void)
{
TStringList* L = new TStringList;
L->Duplicates = dupIgnore;
L->Sorted = true;
L->Clear();
TList * TempLabels = new TList;
TempLabels->Clear();
//L is a temporary list of strings to process
//TempLabels is a temporary list the valid label data derived from L

for (int i =0;i<Labels->Count;i++)
L->Add(Administers((char*)Labels->Items[i]));

//int i= L->Count;
//remove duplicates names and substitutes administrators

for (int i =0;i<L->Count;i++)
{
void* V = AddressData(L->Strings[i].c_str());
if (V) TempLabels->Add(V);
}
Labels->Clear();
for (int i=0;i<TempLabels->Count ;i++)Labels->Add(TempLabels->Items[i]);
delete L;
delete TempLabels;
Validated=true;
}

