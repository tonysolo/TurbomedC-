//---------------------------------------------------------------------------
#ifndef A5PgH
#define A5PgH
#include <printers.hpp>
#include "a5base.h"
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
class TA5Page : public TA5Base
{
private:
int fclear;
int fwidth;
int fheight;
int fpagecount;
int fpageoffset;
int fpage2offset;
int rhs;
TRect *lrect;
TRect *rrect;
//Graphics::TBitmap* BMap2;
        void PrintPage(int);

public:
TPrinter* printer;
__fastcall TA5Page();
__fastcall ~TA5Page();
void NewPage();
void Finish();
};
//---------------------------------------------------------------------------


#endif
