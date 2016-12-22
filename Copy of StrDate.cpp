//---------------------------------------------------------------------------
//#include <vcl.h>
//#pragma hdrstop

#include "strdate.h"
 #include <string.h>
 #include <stdlib.h>
 #include<vcl>
//---------------------------------------------------------------------------
//#pragma package(smart_init)

TStrDate::TStrDate()
{
mdate = new char[9];
}


TStrDate::TStrDate(char*s)
{
mdate = new char[9];
strcpy(mdate,"        ");
if (strlen(s)==8)
{
mdate[0]=s[0];
mdate[1]=s[1];
mdate[2]=s[2];
mdate[3]=s[3];
mdate[4]=s[4];
mdate[5]=s[5];
mdate[6]=s[6];
mdate[7]=s[7];
}
// for delimited dates
if (strlen(s)==10)
{
mdate[0]=s[0];
mdate[1]=s[1];
mdate[2]=s[3];
mdate[3]=s[4];
mdate[4]=s[6];
mdate[5]=s[7];
mdate[6]=s[8];
mdate[7]=s[9];
//strncpy(mdate,s,2);
//strncat(mdate,&s[3],2);
//strcat(mdate,&s[6]);
}
}

TStrDate::~TStrDate()
{
//OutputDebugString("In Tstrdate destruct");
delete[] mdate;
}

short TStrDate::Year()
{
char s[5];
strncpy(s,&mdate[4],4);
s[4]='\0';
return (short)atoi(s);
}

short TStrDate::Month()
{
char s[3];
strncpy(s,&mdate[2],2);
s[2]='\0';

return (short)atoi(s);
}

short TStrDate::Day()
{
char s[3];
strncpy(s,mdate,2);
s[2]='\0';
return (short)atoi(s);
}


int TStrDate::Age(void)
{
TDateTime d;
try
{
d = TDateTime (Year(),Month(),Day());}
catch (EConvertError &)
 {d= TDateTime::CurrentDate();}
return (int)TDateTime::CurrentDate()-d;
}


bool TStrDate::Valid(void)
{
TDateTime d;
bool valid=true;
try
{d = TDateTime (Year(),Month(),Day());}
catch (EConvertError &)
{ valid=false;}
return valid;
}
        //TODO: Add your source code here

