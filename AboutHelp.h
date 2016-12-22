//---------------------------------------------------------------------------
#ifndef AboutHelpH
#define AboutHelpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TAboutHelpForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TPanel *Panel1;
        TImage *Image1;
        TStaticText *StaticText1;
        TStaticText *StaticText2;
        void __fastcall Image1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
public:		// User declarations
    __fastcall TAboutHelpForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAboutHelpForm *AboutHelpForm;
//---------------------------------------------------------------------------
#endif
