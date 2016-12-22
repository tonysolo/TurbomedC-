//---------------------------------------------------------------------------

#ifndef a5baseH
#define a5baseH
#include <vcl>
//---------------------------------------------------------------------------
//a4 = 210 x 297  8.27  x  11.69


class TA5Base
{
private:
int lastline;
int fmaxlines;
int flinespace;
int ffooterheight;
char *fheading;
void setheading(char*);
int FTextStyle;
void __fastcall SetTextStyle(int value);


protected:

TCanvas* Canvas;
TRect* Rect;

        void Initialise(void);

public:
bool Mosaic;
bool Sep_page;    //probably not necessary forces a new page for each account
                  //furure versions might be better to di A$ accounts
int DocPageCount;
enum {LJ,RJ};
bool SinglePage;
enum {a5Normal,a5Bold,a5Italic,a5BoldItalic};

__property int LineCount = {read = fmaxlines};

__property char *Heading = {read=fheading,write=setheading};
__property int FooterHeight = {read=ffooterheight,write=ffooterheight};

virtual void NewPage()=0;
void DrawBorder(void);
void Write(int,int,const char*, bool);
void Separator(int);
int  NextLine(int);
virtual void Finish()=0;
void ClearLine(int);
__property int TextStyle  = { read = FTextStyle, write = SetTextStyle };

TA5Base();
~TA5Base();
        void BankingDetails(char*, char*, char*);
         void ShadeAbove(int);
        void PrintMosaic(char*);

};
#endif
 