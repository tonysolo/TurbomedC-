//---------------------------------------------------------------------------
#ifndef A5PgH
#define A5PgH
#include <printers.hpp>
//---------------------------------------------------------------------------
// TA5Page class
// Updated 14/09/99
// Minimal borderwidth for maximum page space
// (just a separator between the pages)
// Makes two a5 print pages from a single A4 sheet
// Keeps track of borders etc.
// BorderMargin establishes border units in pixels
// Maxlines == max number of text lines in the A5 portrait

// TAPage() constructor gets metrics from global printer etc
// sets up a 2% of A4 height border and 10 pt font.
// NewPage() advances page and prints the sheet every 2 pages
// DrawBorder() == called automatically with each NewPage()
// Write (%of page pos, line,string,right justify )
// Height == read only of  A5 page in pixels
// Width == read only of page in pixels
// LineCount == read only set to 49 (48 usable 1..48 inc)

// Heading is the heading for each new page after page 1

// Separator() prints a divider line at pos y
// NextLine(int i) if i==0 returns the nextline number
// if i>0 resets the line counter.
// automatically prints headers and footers
// Finish() ends the printing (calls EndDoc())
// fclear == pixels spacing around border
class TA5Page
{
private:

//int fA4width;
//int fA4height;
int fwidth;
int fheight;
int fpageoffset;
//int fbordermargin;
int fmaxlines;
int flinespace;
int fpagecount;
int fdocpagecount;
int fpage2offset;
int ffooterheight;
char *fheading;
void setheading(char*);
int fclear;
    int FTextStyle;
    void __fastcall SetTextStyle(int value);


public:
enum {LJ,RJ};
enum {a5Normal,a5Bold,a5Italic,a5BoldItalic};
__fastcall TA5Page();
__fastcall ~TA5Page();
__property int LineCount = {read = fmaxlines};
__property int Height = {read = fheight};
__property int Width = {read = fwidth};
__property char*Heading = {read=fheading,write=setheading};
__property int FooterHeight = {read=ffooterheight,write=ffooterheight};

void NewPage();
void DrawBorder(void);
void Write(int,int,const char*, bool);
void Separator(int);
int  NextLine(int);
void Finish();
TPrinter* printer;
        void ClearLine(int);
__property int TextStyle  = { read = FTextStyle, write = SetTextStyle };
};
//---------------------------------------------------------------------------


#endif
