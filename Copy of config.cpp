//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "config.h"
#include <Registry.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

TConfig::TConfig()
{
for (int i=0;i<4 ;i++)filestack[i]=NULL;
TRegistry* reg = new TRegistry;
if (reg->OpenKey("\\Software\\turbomed\\TurboMED\\files",false))
{
File[0]= (reg->ValueExists("F0"))? reg->ReadString("F0").c_str():NULL;
File[1]= (reg->ValueExists("F1"))? reg->ReadString("F1").c_str():NULL;
File[2]= (reg->ValueExists("F2"))? reg->ReadString("F2").c_str():NULL;
File[3]= (reg->ValueExists("F3"))? reg->ReadString("F3").c_str():NULL;
}
delete reg;
//TidyFiles();
}


TConfig::~TConfig()
{
TidyFiles();
TRegistry* reg = new TRegistry;
 if (reg->KeyExists("\\Software\\turbomed\\TurboMED"))
   {
   reg->OpenKey("\\Software\\turbomed\\TurboMED\\files",true);
if (File[0]) reg->WriteString("F0",String(File[0]));
if (File[1]) reg->WriteString("F1",String(File[1]));
if (File[2]) reg->WriteString("F2",String(File[2]));
if (File[3]) reg->WriteString("F3",String(File[3]));
   }
delete reg;
for (int i=0;i<4 ;i++) if (filestack[i]) delete[] filestack[i];

}

void TConfig::PushFile(String f)
{
char* fn = new char[f.Length()+1];
strcpy(fn,f.c_str());
//bool b = true;
for (int i=0;i<4;i++)
{if (!strcmp((filestack[i])?filestack[i]:"",fn))
{delete[]filestack[i];filestack[i]=NULL ;break;}}
if(fn)
{
TidyFiles();
if (filestack[3])
{delete[] filestack[3]; filestack[3]=NULL;}
filestack[3]=filestack[2];
filestack[2]=filestack[1];
filestack[1]=filestack[0];
filestack[0]= new char[strlen(fn)+1];
strcpy(filestack[0],fn);
}
delete[] fn;
//TidyFiles();
}

void TConfig::SetFile(int i, char* value)
{
if ((i>-1)&&(i<4))
{
if (filestack[i]){delete[]filestack[i];filestack[i]=NULL;}
if (value)
{
filestack[i]=new char[strlen(value)+1];
strcpy(filestack[i],value);
}
}
//TidyFiles();
}

char* TConfig::GetFile(int i)
{
return ((i>-1)&&(i<4))?filestack[i]:NULL;
}


int TConfig::GetFileCount()
{
int j=0;
for (int i=0;i<4 ;i++)
{
if (filestack[i])j++;
}
return j;
}


void TConfig::TidyFiles(void)
{
char* temp[4]={NULL,NULL,NULL,NULL};
int j=0;
for (int i=0;i<4 ;i++) if (filestack[i]) temp[j++]=filestack[i];
for (int i=0;i<4 ;i++) filestack[i]=temp[i];
}

