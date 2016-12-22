//---------------------------------------------------------------------------
#ifndef configH
#define configH
//---------------------------------------------------------------------------
class TConfig
{
private:
char *filestack[4];
void SetFile(int, char*);
char* GetFile(int);
    int GetFileCount();
    void TidyFiles(void);
protected:

public:
TConfig();

~TConfig();
    void PushFile(String);
    __property char* File[int] = { read = GetFile, write = SetFile};
    __property int FileCount  = { read = GetFileCount };
};

#endif
