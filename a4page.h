//---------------------------------------------------------------------------

#ifndef a4pageH
#define a4pageH
//---------------------------------------------------------------------------
class TA4Page
{
private:
int LineSpace;
int CurrentLine;


protected:

public:
        TA4Page();
        ~TA4Page();
        int NextLine(int = 0);
        void PrintAt(int, char*, int);
        enum{LJ,RJ};
};



#endif
 