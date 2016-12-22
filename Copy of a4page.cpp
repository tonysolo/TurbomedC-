//---------------------------------------------------------------------------


#pragma hdrstop
#include <Printers.hpp>
#include "a4page.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)


TA4Page::TA4Page()
{
TPrinter* pr = Printer();
  pr->Canvas->Font->Style =
  pr->Canvas->Font->Style.Clear();
  pr->Canvas->Font->Name = "Arial";
  pr->Canvas->Font->Size=10;
  pr->Orientation=poPortrait;
  LineSpace = pr->Canvas->TextHeight("Test");
  CurrentLine=1;
  pr->Canvas->Brush->Color=clWhite;
  pr->Canvas->Font->Color=clBlack;
  pr->BeginDoc();

}

TA4Page::~TA4Page()
{
Printer()->EndDoc();
}

int TA4Page::NextLine(int y)
{
if (y) CurrentLine=y; else CurrentLine++;
if ((CurrentLine+1)*LineSpace > Printer()->PageHeight)
{Printer()->NewPage();CurrentLine=1;}
return CurrentLine;
}

void TA4Page::PrintAt(int xpos, char* s, int j)
{
TPrinter* pr = Printer();
int x = pr->PageWidth * xpos/100; // xpos is percent position
int y = CurrentLine*  LineSpace;      //calc pixel position
if (j) x-= pr->Canvas->TextWidth(s);
pr->Canvas->TextOutA(x,y,s);
}
