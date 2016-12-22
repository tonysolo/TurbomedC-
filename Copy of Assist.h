//---------------------------------------------------------------------------

#ifndef AssistH
#define AssistH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <CheckLst.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TAssistFee : public TForm
{
__published:	// IDE-managed Components
        TLabeledEdit *LabeledEdit1;
        TCheckListBox *CheckListBox1;
        TLabeledEdit *LabeledEdit2;
        TBitBtn *BitBtn1;
        void __fastcall CheckListBox1ClickCheck(TObject *Sender);
private:	// User declarations
        int AssistRate;
public:		// User declarations
        __fastcall TAssistFee(TComponent* Owner);
        TStringList* Amounts;

          void UpdateRate(int);
        __fastcall ~TAssistFee();

};
//---------------------------------------------------------------------------
extern PACKAGE TAssistFee *AssistFee;
//---------------------------------------------------------------------------
#endif
