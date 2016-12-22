//---------------------------------------------------------------------------

#ifndef bkeepH
#define bkeepH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------


struct bookkeepdata
{
void* account;
void* transact;
double balance;
};

int __fastcall Compare(void*, void*);


class TBookKeep : public TForm
{
__published:	// IDE-managed Components
        TListBox *ListBox1;
        TDateTimePicker *DateTimePicker1;
        TDateTimePicker *DateTimePicker2;
        TSpeedButton *FileSaveButton;
        TLabel *Label3;
        TSpeedButton *PrintButton;
        TLabel *Label8;
        TBitBtn *BitBtn1;
        TLabel *Label1;
        TLabel *Label2;
        THeaderControl *HeaderControl1;
        TLabel *paylabel;
        TLabel *refundlabel;
        TLabel *adjustlabel;
        TLabel *assistantlabel;
        TBevel *Bevel1;
        TButton *UpdateButton;
        TSpeedButton *Clip;
        TLabel *Label4;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall ListBox1DrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
        void __fastcall DateTimePicker1Change(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall DateTimePicker1Click(TObject *Sender);
        void __fastcall HeaderControl1SectionResize(
          THeaderControl *HeaderControl, THeaderSection *Section);
        void __fastcall FileSaveButtonClick(TObject *Sender);
        void __fastcall PrintButtonClick(TObject *Sender);
        void __fastcall UpdateButtonClick(TObject *Sender);
        void __fastcall ClipClick(TObject *Sender);
 //       void __fastcall ListBox2DrawItem(TWinControl *Control, int Index,
 //         TRect &Rect, TOwnerDrawState State);
private:	// User declarations
        double broughtforward;
        double totalpay;
//        double totalrefund;
//        double totaladjust;
//        double totalassist;
        double totalbilled;
        TList* datalist;
        void UpdateDataList(void);
        void DrawLegend(void*, int);
        String newreportname(void);
        char* BookDate(void*, void*);
        bool Assistant(char*);
        void BuildCSV(TStringList*);
        char* OldestAccountDate(TList*);

public:
        
        TList* allList;		// User declarations
        __fastcall TBookKeep(TComponent*,TList*);
};          
//---------------------------------------------------------------------------
extern PACKAGE TBookKeep *BookKeep;
//---------------------------------------------------------------------------
#endif
