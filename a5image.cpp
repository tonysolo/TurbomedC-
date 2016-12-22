//---------------------------------------------------------------------------

#pragma hdrstop
#include "a5image.h"
#include "jpeg.hpp"


//---------------------------------------------------------------------------

#pragma package(smart_init)

 __fastcall TA5Image::TA5Image():TA5Base()
{
BMap = new Graphics::TBitmap();
BMap->HandleType=bmDIB;
BMap->PixelFormat= pf1bit;
BMap->Monochrome=true;
BMap->Width = (int)BMap->Canvas->Font->PixelsPerInch * (297/50.8);      //8.27
BMap->Height = (int)BMap->Canvas->Font->PixelsPerInch * (210/25.4);     //11.69
Canvas = BMap->Canvas;
//Rect = Canvas->ClipRect;
Rect = new TRect(0,0,BMap->Width,BMap->Height);
DocPageCount=0;
fpagecount=-1;
fname = "JPG";
Initialise();
DrawBorder();
}
 __fastcall TA5Image::~TA5Image()
{
if (fpagecount>-1) Finish();
delete BMap;
delete Rect;
}

void TA5Image::NewPage()
{
fpagecount++;
  if (fpagecount)
{
  SaveJPG();
  DrawBorder();
  ++DocPageCount;
}
}

void TA5Image::Finish()       // must call finish after each account
{                             // to make sure all pages are printed
SaveJPG();
DrawBorder();
DocPageCount=0;
fpagecount=-1;
}

void TA5Image::SaveJPG()
{
 String fn = fname + "_" + IntToStr(DocPageCount)+ ".JPG";
  TJPEGImage *jp = new TJPEGImage();
  jp->Palette = 0;
  jp->PixelFormat=jf8Bit;
 try
  {
    jp->CompressionQuality=70;
    jp->Assign(BMap);
    jp->Compress();
    jp->SaveToFile(fn);
  }
  __finally
  {
    delete jp;
  }
}



void __fastcall TA5Image::SetFileName(AnsiString value)
{
fname = value;
DocPageCount=0;
}
