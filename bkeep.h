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
struct preprocdata
{
char flag;
void* account;
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
        TBevel *Bevel2;
        TLabel *Label15;
        TLabel *Label17;
        TLabel *Label18;
        TLabel *Label13;
        TLabel *Label14;
        TLabel *Label11;
        TLabel *Label16;
        TLabel *Label7;
        TLabel *Label10;
        TLabel *Label9;
        TLabel *Label5;
        TLabel *Label6;
        TEdit *pfees;
        TEdit *afees;
        TEdit *refunds;
        TEdit *interest;
        TEdit *totdebits;
        TEdit *mapay;
        TEdit *ppay;
        TEdit *upay;
        TEdit *discounts;
        TEdit *writeoffs;
        TEdit *totcredits;
        TLabel *Label12;
        TLabel *Label19;
        TLabel *Label20;
        TEdit *startbalance;
        TEdit *endbalance;
        TCheckBox *intcheck;
        TLabel *interestlabel;
        TLabel *discountlabel;
        TBevel *Bevel3;
        TLabel *Label8;
        TSpeedButton *printbutton;
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
        void __fastcall intcheckClick(TObject *Sender);
 //       void __fastcall ListBox2DrawItem(TWinControl *Control, int Index,
 //         TRect &Rect, TOwnerDrawState State);
private:	// User declarations
        double broughtforward;
        double totalpay;
    //    double totalrefund;
    //    double totaladjust;
   //     double totalassist;
        double totalbilled;
        double totaldebits;
        double totalcredits;
   //     double totalinterest;
   //     double totaldiscount;
        TList* datalist;
        TList* prelist;
        TList* templist;
        void UpdateDataList(void);
        void DrawLegend(void*, int);
        String newreportname(void);
        char* BookDate(void*, void*);
        bool Assistant(char*);
        void BuildCSV(TStringList*);
        char* OldestAccountDate(TList*);
        int AccountType(void*);
        void* newtrans(char*,char*,float,void*,char* dt = NULL);
        void Preprocess(void);

public:
        
        TList* allList;		// User declarations
        __fastcall TBookKeep(TComponent*,TList*);
};          
//---------------------------------------------------------------------------
extern PACKAGE TBookKeep *BookKeep;
//---------------------------------------------------------------------------
#endif
