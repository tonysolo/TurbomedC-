//---------------------------------------------------------------------------

#ifndef a5previewH
#define a5previewH

#include "a5base.h"
//---------------------------------------------------------------------------
#endif
class TA5Preview : public TA5Base
{
private:
Graphics::TBitmap* BMap;
int fpagecount;
TList* BMList;
protected:
public:
 __fastcall TA5Preview();
 __fastcall ~TA5Preview();
void NewPage();
void Finish();
void GetImage(int,TCanvas*, TRect);
};
