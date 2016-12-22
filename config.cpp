//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "config.h"
#include <Registry.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

TConfig::TConfig()
{
ini = new TIniFile(ChangeFileExt(Application->ExeName,".ini"));
File[0]=ini->ReadString("Files","F0","");
File[1]=ini->ReadString("Files","F1","");
File[2]=ini->ReadString("Files","F2","");
File[3]=ini->ReadString("Files","F3","");

}

TConfig::~TConfig()
{
TidyFiles();
ini->WriteString("Files","F0",File[0]);
ini->WriteString("Files","F1",File[1]);
ini->WriteString("Files","F2",File[2]);
ini->WriteString("Files","F3",File[3]);
delete ini;
}

void TConfig::PushFile(String f)
{
for (int i=0;i<4;i++) if (filestack[i]==f){filestack[i]="";}



if(!f.IsEmpty())
{
TidyFiles();

if (!filestack[3].IsEmpty()){filestack[3]="";}
filestack[3]=filestack[2];
filestack[2]=filestack[1];
filestack[1]=filestack[0];
filestack[0]=f;
}

}

void TConfig::SetFile(int i, String value)
{
if ((i>-1)&&(i<4)) filestack[i]=value;

}

String TConfig::GetFile(int i)
{
return ((i>-1)&&(i<4))?filestack[i]:String("");
}


int TConfig::GetFileCount()
{
int j=0;
for (int i=0;i<4 ;i++)
{
if (filestack[i]!="")j++;
}
return j;
}


void TConfig::TidyFiles(void)
{
String temp[4]={"","","",""};
int j=0;
for (int i=0;i<4 ;i++) if (!filestack[i].IsEmpty()) temp[j++]=filestack[i];
for (int i=0;i<4 ;i++) filestack[i]=temp[i];
}


void TConfig::MaxCSVSize(int x)
{
ini->WriteString("Debug","MaxCSVSize",String(x));
}
