//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "A5Pg.h"
#include <stdio.h>
#include "main.h";

//---------------------------------------------------------------------------
#pragma package(smart_init)


 __fastcall TA5Page::TA5Page(): TA5Base()
 {
  printer = Printer();
  printer->Copies = 1;
  printer->Orientation=poLandscape;

  Canvas = printer->Canvas;
  char light = (mainform->TMData->Setup)? mainform->TMData->Setup->Field[tmd::pref][5]:'1';
  Canvas->Pen->Width= (light=='0') ? 6:1;
  fheight = printer->PageHeight;
  fwidth = printer->PageWidth / 2* 0.965;
  lrect = new TRect(0,0,fwidth,fheight);
  rrect = new TRect(printer->PageWidth-fwidth,
                    0,
                    printer->PageWidth,
                    fheight);
  Rect = (SinglePage)? rrect:lrect;
  fpagecount=-1;
  DocPageCount=0;
  Initialise();

}
//---------------------------------------------------------------------------
 __fastcall TA5Page::~TA5Page()
 {
delete lrect;
delete rrect;
 }

//---------------------------------------------------------------------------


void TA5Page::NewPage()
{
bool std = ((!Mosaic)||((Mosaic)&&(!Sep_page)));
/* TODO : right and left single page print preference */
fpagecount++;
  if (fpagecount==0) {Rect = (SinglePage)? rrect:lrect;
                      if (Mosaic) Rect= lrect;
                          printer->BeginDoc();
                      }//DrawBorder();
    else {
          if (
              ((fpagecount%2==0)&&(std))||
              ((DocPageCount%2==0)&&(!std))
              )
                {
                 printer->NewPage();
                 Rect = lrect;
               }
          else  Rect = rrect;
               }

  DrawBorder();
  ++DocPageCount;
}

//---------------------------------------------------------------------------

void TA5Page::Finish()
{
if (printer->Printing) printer->EndDoc();
}







