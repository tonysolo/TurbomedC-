//---------------------------------------------------------------------------


#pragma hdrstop

#include "a5preview.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

 __fastcall TA5Preview::TA5Preview()
{
//BMap = new Graphics::TBitmap();
BMList = new TList;
BMList->Clear();
DocPageCount=0;

}
 __fastcall TA5Preview::~TA5Preview()
{
//if (fpagecount>-1) Finish();
for (int i=0;i<BMList->Count;i++) delete (Graphics::TBitmap*)BMList->Items[i];
delete Rect;
delete BMList;
}

void TA5Preview::NewPage()
{
int i = BMList->Add(new Graphics::TBitmap);
BMap=(Graphics::TBitmap*)BMList->Items[i];
BMap->HandleType=bmDIB;
BMap->PixelFormat= pf1bit;
BMap->Monochrome=true;
BMap->Width = (int)BMap->Canvas->Font->PixelsPerInch * (297/50.8);      //8.27
BMap->Height = (int)BMap->Canvas->Font->PixelsPerInch * (210/25.4);     //11.69
Canvas = BMap->Canvas;
Rect = new TRect(0,0,BMap->Width,BMap->Height);
fpagecount++;
Initialise();
DrawBorder();
DocPageCount++;

}

void TA5Preview::Finish()       // must call finish after each account
{                             // to make sure all pages are printed
//DrawBorder();
//DocPageCount=0;
//fpagecount=-1;
}

void TA5Preview::GetImage(int pg, TCanvas* c, TRect R)
{
int page = pg;
if (page > BMList->Count-1) page=BMList->Count-1;
if (page<0)page = 0;
c->StretchDraw(R,(Graphics::TBitmap*)BMList->Items[page]);
}


