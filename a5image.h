//---------------------------------------------------------------------------
#include "a5base.h"
#ifndef a5imageH
#define a5imageH
//---------------------------------------------------------------------------

class TA5Image : public TA5Base
{
private:
Graphics::TBitmap* BMap;
int fpagecount;
String fname;
void SaveJPG();
        void __fastcall SetFileName(AnsiString value);
protected:
public:
 __fastcall TA5Image();
 __fastcall ~TA5Image();
void NewPage();
void Finish();
        __property AnsiString FileName  = { read=fname, write=SetFileName };


};



#endif
 