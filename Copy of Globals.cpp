//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Globals.h"
#include "strdate.h"
#include <stdlib.h>
#include <stdio.h>


//---------------------------------------------------------------------------
#pragma package(smart_init)



//---------------------------------------------------------------------------
char* EDIDate(char*d)
{
//fixed 21112002 - s was not initialised to 8 chars
static char* s = "00000000";
if (strlen(d)==8)
{
s[0]=d[4];
s[1]=d[5];
s[2]=d[6];
s[3]=d[7];
s[4]=d[2];
s[5]=d[3];
s[6]=d[0];
s[7]=d[1];
}
return s;
}
//---------------------------------------------------------------------------

char *revdate(char*d)
{
static char* s = "00000000";
s[0]=d[4];
s[1]=d[5];
s[2]=d[6];
s[3]=d[7];
s[4]=d[2];
s[5]=d[3];
s[6]=d[0];
s[7]=d[1];
return s;
}
//---------------------------------------------------------------------------



char*date2000(char*d)
{
if (strlen(d)==8) return d;
static char* s="  /  /    ";
//             "01/34/6789"
s[6]= '1';
s[7]= '9';
s[0]= d[0];
s[1]= d[1];
s[3]= d[2];
s[4]= d[3];
s[8]= d[4];
s[9]= d[5];
if (d[4]<'6') {s[6]='2';s[7]='0';}
return s;
}

char* DelimDate (char*s)
{
if (strlen(s)!=8) return "";
static char* d = "  /  /    ";
d[0]= s[0];
d[1]= s[1];
d[3]= s[2];
d[4]= s[3];
d[6]= s[4];
d[7]= s[5];
d[8]= s[6];
d[9]= s[7];
return d;
}

char* USADate(char* s)
{
if (strlen(s)!=8) return "";
static char* d = "    /  /  ";
d[0]= s[4];
d[1]= s[5];
d[2]= s[6];
d[3]= s[7];
d[5]= s[2];
d[6]= s[3];
d[8]= s[0];
d[9]= s[1];
return d;


}


char* d_ymd(char* s)
 {
char *d="  ";
char *m="  ";
char *y="    ";
d[0]= s[0];
d[1]= s[1];
m[0]= s[2];
m[1]= s[3];
y[0]= s[4];
y[1]= s[5];
y[2]= s[6];
y[3]= s[7];
try
    {
return TDateTime((short)atoi(y),(short)atoi(m),(short)atoi(d))
.FormatString("dd/mm/yy").c_str();
    }
catch(EConvertError &e) {return "error";}
    }


    
char* ddmmmyyyy(char* s)
 {
char *d="  ";
char *m="  ";
char *y="    ";
static char dt[11];
d[0]= s[0];
d[1]= s[1];
m[0]= s[2];
m[1]= s[3];
y[0]= s[4];
y[1]= s[5];
y[2]= s[6];
y[3]= s[7];
try
{
strcpy(dt,TDateTime((short)atoi(y),
                 (short)atoi(m),
                 (short)atoi(d))
.FormatString("dd mmm yyyy").c_str());
}
catch(EConvertError &e) {strcpy(dt,"");}
return dt;
}


char* MonthYearStr (char* yyyymm) //199901 to Jan 99
{
static char dt[10];
char *yr ="    ";
char month[12][4]=
{"Jan","Feb","Mar","Apr","May","Jun",
 "Jul","Aug","Sep","Oct","Nov","Dec"};
strcpy(dt,yyyymm);
strncpy(yr,dt,4);
sprintf(dt,"%s %s",(month[atoi(&dt[4])-1]),yr);
return dt;
}


char *yyyymm(char* dmy) // 01011999 to 199901
{

static char*s = "      ";
if (strlen(dmy)==8){
s[0]=dmy[4];
s[1]=dmy[5];
s[2]=dmy[6];
s[3]=dmy[7];

s[4]=dmy[2];
s[5]=dmy[3];
}
return s;
}

 char *Year(char* d)
 {
 return &d[4];
 }

  char *Month(char* d)
 {
 static char day[9];
 strcpy(day,d);
 day[4]='\0';
 return &day[2];
 }

 char *Day(char* d)
 {
 static char day[9];
 strcpy(day,d);
 day[2]='\0';
 return day;
 }


char *incdate(char*s)
{
TStrDate d(s);
TDateTime dt(d.Year(),d.Month(),d.Day());
dt +=1;
return dt.FormatString("ddmmyyyy").c_str();
}


 char *decdate(char*s)
{
TStrDate d(s);
TDateTime dt(d.Year(),d.Month(),d.Day());
dt -=1;
return dt.FormatString("ddmmyyyy").c_str();
}
