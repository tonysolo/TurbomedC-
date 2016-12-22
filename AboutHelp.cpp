//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AboutHelp.h"
# include "main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutHelpForm *AboutHelpForm;
//---------------------------------------------------------------------------
__fastcall TAboutHelpForm::TAboutHelpForm(TComponent* Owner)
    : TForm(Owner)
{
//char* v = ((Tmainform*)Owner)->Version;
Label1->Caption=
"Copyright Tony Manicom 2000\n\n\
Acknowledgements:\n\
* All users of the DOS version for\n\
   helping to evolve the design since 1986.\n\
Especially the users from the very start:\n\
* Pat Tweedie for all sorts of input\n\
* Iain Gibson for testing and comments\n\
* Ilinda Du Preez for message language\n\
* Arrin Katz for consulting and testing\n\
   the numerous Windows revisions"
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



