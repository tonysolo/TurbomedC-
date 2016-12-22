//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutHelp.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutHelpForm *AboutHelpForm;
//---------------------------------------------------------------------------
__fastcall TAboutHelpForm::TAboutHelpForm(TComponent* Owner)
    : TForm(Owner)
{
Label1->Caption=
"Copyright Tony Manicom 2000\n\
Written by Tony Manicom\n\n\
Acknowledgements:\n\
* All users of the DOS version for\n\
helping to evolve the design\n\
since 1986.\n\
* Arrin Katz for consulting and input\n\
on the new Windows implementation"
;
}
//---------------------------------------------------------------------------

void __fastcall TAboutHelpForm::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if ((Button==mbRight)&&(Shift.Contains(ssShift)))
{//StaticText2->Visible=false;
Panel1->Visible=false;
Label1->Visible=true;}
}
//---------------------------------------------------------------------------



