//---------------------------------------------------------------------------

#ifndef CompareH
#define CompareH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
struct CompData
{
void * curr;
void * comp;
};


class TCompareDlg : public TForm
{
__published:	// IDE-managed Components
        THeaderControl *HeaderControl1;
        TListBox *ListBox1;
        TBitBtn *BitBtn1;
        TSpeedButton *FileSaveButton;
        TLabel *Label1;
        TSpeedButton *SpeedButton1;
        TLabel *Label2;
        TSpeedButton *PrintButton;
        TLabel *Label8;
        void __fastcall ListBox1DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall FileSaveButtonClick(TObject *Sender);
        void __fastcall PrintButtonClick(TObject *Sender);
private:	// User declarations
TStringList* CompStrList;
        int Different(CompData*);
        void BuildCSV(TStringList*);
        char* FolderName(void*);
public:		// User declarations
        __fastcall TCompareDlg(TComponent* Owner);
        __fastcall ~TCompareDlg();
        void BuildComparison(TList*, TList*);
        String CompareFileName;
};
//---------------------------------------------------------------------------
extern PACKAGE TCompareDlg *CompareDlg;
//---------------------------------------------------------------------------
#endif
