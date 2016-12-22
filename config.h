//---------------------------------------------------------------------------
#ifndef configH
#define configH
#include <IniFiles.hpp> 
//---------------------------------------------------------------------------
class TConfig
{
private:
TIniFile* ini;
String filestack[4];
void SetFile(int, String);
String GetFile(int);
    int GetFileCount();
    void TidyFiles(void);
protected:

public:
TConfig();

~TConfig();
    void PushFile(String);
        void MaxCSVSize(int);
    __property String File[int] = { read = GetFile, write = SetFile};
    __property int FileCount  = { read = GetFileCount };
};

#endif
