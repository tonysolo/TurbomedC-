//---------------------------------------------------------------------------

#ifndef changeprintH
#define changeprintH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TChangePrinter : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *GroupBox1;
        TComboBox *ComboBox1;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
private:	// User declarations
public:		// User declarations
        __fastcall TChangePrinter(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TChangePrinter *ChangePrinter;
//---------------------------------------------------------------------------
#endif
