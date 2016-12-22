//---------------------------------------------------------------------------

#pragma hdrstop

#include "a5base.h"
#include <stdio.h>
//---------------------------------------------------------------------------

#pragma package(smart_init)


TA5Base::TA5Base()
{
DocPageCount=0;
SinglePage=false;
fheading = NULL;
ffooterheight=1;
Mosaic=false;
Sep_page=false;   
}

TA5Base::~TA5Base()
{
if (fheading)delete[] fheading;
}

void TA5Base::Write(int xpos ,int ypos,const char* s, bool rjustify)
{
if (ypos > LineCount) return;

double x = xpos;
       x = x / 100 * Rect->Width();
double y = ypos * flinespace;

if (rjustify) x-= Canvas->TextWidth(s);

Canvas->TextOut(Rect->Left+x,Rect->Top+y,s);
}
//---------------------------------------------------------------------------


void TA5Base::Separator(int line)
{
Canvas->MoveTo(Rect->Left,line * flinespace);
Canvas->LineTo(Rect->Left+Rect->Width(),line * flinespace);
}


//---------------------------------------------------------------------------
 void TA5Base::setheading(char* s)
 {
 if (fheading) delete[] fheading;
 fheading = new char[strlen(s)+1];
 strcpy(fheading,s);

 }
//---------------------------------------------------------------------------


int  TA5Base::NextLine(int i)
{
char str[100];
static int line;
if (i) line = i; else line ++;
if (line > LineCount-(FooterHeight+2))
{
int ts = TextStyle;
TextStyle = a5Normal;
Separator(LineCount-(FooterHeight+1));
sprintf(str,"/cont.. Page %d",DocPageCount+1);
Write(2,LineCount-1,str,false);
NewPage();
sprintf(str,"Page %d  %s",DocPageCount,Heading);
Write(2,1,str,false);
Separator(2);
line = 3;
TextStyle=ts;
}
lastline=line;
return line;

}

void __fastcall TA5Base::SetTextStyle(int value)
{
FTextStyle = value;
Canvas->Font->Style =
Canvas->Font->Style.Clear();
switch (FTextStyle)
{
case 1:Canvas->Font->Style=
Canvas->Font->Style << fsBold; break;

case 2:Canvas->Font->Style=
Canvas->Font->Style << fsItalic; break;

case 3:Canvas->Font->Style=
Canvas->Font->Style << fsItalic << fsBold; break;
}
}


void TA5Base::ClearLine(int l)
{
int line = (l)?l:1;

TRect r(Rect->Left + Canvas->Pen->Width,                //reduced width lef+2 and width-4 to stop cutting thick lines
        line * flinespace - Canvas->TextHeight("ABC"),
        Rect->Left+Rect->Width() - Canvas->Pen->Width,
        line * flinespace + Canvas->TextHeight("ABC"));
Canvas->FillRect(r);
}


void TA5Base::DrawBorder()
{
//TColor col = Canvas->Brush->Color;
//Canvas->Brush->Color = clLtGray;
int x =  Rect->Width() /30;
Canvas->RoundRect(Rect->Left,Rect->Top,Rect->Left+Rect->Width(),Rect->Top+Rect->Height(),x,x);
//Canvas->Brush->Color = col;
int fnt = Canvas->Font->Size;
Canvas->Font->Size=6;
SetTextStyle(0);
String s = "Turbomed Software - www.turbomed.co.za";
int h = Canvas->TextHeight(s); h+=3;
int w = Canvas->TextWidth(s); w+=50;
Canvas->TextOutA(Rect->Right-w,Rect->Bottom-h,s);
Canvas->Font->Size=fnt;
}

void TA5Base::Initialise(void)
{
Canvas->Font->Name = "Arial";
Canvas->Font->Size=8;
//Canvas->Pen->Width=1;
Canvas->Pen->Color=clBlack;
Canvas->Font->Color=clBlack;
Canvas->Brush->Color=clWhite;
flinespace = Rect->Height()/49;
fmaxlines = 49;
//ffooterheight=1;
SetTextStyle(0);
}

void TA5Base::BankingDetails(char* l1, char* l2, char* l3)
{
if ( lastline < (LineCount-7))
{
int fnt = Canvas->Font->Size;
Canvas->Font->Size=8;
ClearLine(LineCount-4);
int y = (LineCount - 6);

//Separator(y);
int h = Canvas->TextHeight(l1);
int y1 = y * flinespace;// + 6;//(Canvas->TextHeight(l1)/6);
int y2 = y1 + h;
int y3 = y2 + h;
int x1 = Canvas->TextWidth(l1);  //(Rect->Width()-Canvas->TextWidth(l1));
int x2 = Canvas->TextWidth(l2);  //(Rect->Width()-Canvas->TextWidth(l2));
int x3 = Canvas->TextWidth(l3);  //(Rect->Width()-Canvas->TextWidth(l3));
int maxx = (x1>x2)? x1:x2;
maxx = (maxx>x3)? maxx:x3;
int xoff = (Rect->Width()-maxx) /2;
//maxy = Rect->Width()-maxy;
Canvas->Rectangle(Rect->Left+xoff-h,
                  y1 - h/2,
                  Rect->Left+xoff+maxx,
                  y3 + h*1.5);
SetTextStyle(2);
Canvas->TextOutA(Rect->Left+xoff,y1,l1);
Canvas->TextOutA(Rect->Left+xoff,y2,l2);
Canvas->TextOutA(Rect->Left+xoff,y3,l3);
SetTextStyle(0);
Canvas->Font->Size=fnt;
}
}

void TA5Base::ShadeAbove(int line)
{

//TColor co = Canvas->Brush->Color;
//Canvas->Brush->Color = clLtGray;
//Canvas->Brush->Style=bsDiagCross;
//Canvas->FillRect(*Rect);
//Canvas->Brush->Color= co;
//Canvas->Brush->Style=bsSolid;

}

void TA5Base::PrintMosaic(char* fn)
{
float w = Canvas->Font->PixelsPerInch;
int bmheight =1.5748 *w;
int bmwidth = 3.970 * w;
if (lastline>28) NextLine(LineCount);
NextLine(0);
NextLine(0);
NextLine(0);
int bmorigin = Rect->Left+ (Rect->Width()-bmwidth)/2;
TRect R(bmorigin,
        lastline*flinespace,
        bmorigin+bmwidth,
        lastline*flinespace+bmheight);

Graphics::TBitmap* bm = new  Graphics::TBitmap();
try
{
bm->LoadFromFile(fn);
Canvas->StretchDraw(R,bm);
}
catch(...){;}
delete bm;

}
