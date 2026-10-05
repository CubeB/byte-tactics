// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdio.h>

class TdfRecord;

class TdfFile {
public:
    int field_0;
    TdfRecord* current;                 // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    int SelectRecord(char* name);
};

class TdfRecord {
public:
    int GetFieldInt(const char* name, int def);
};

extern int DAT_0050289c;
extern int DAT_00511de0;
extern int DAT_00511de4;

char __stdcall FindNextCdDrive(char c);

// FUNCTION: 0x41d6a0
char __stdcall FindGameCdDrive(int side)
{
    if (DAT_0050289c != 0)
        return '.';
    char* name;
    switch (side) {
    case 0:
        name = "Campaign";
        break;
    case 1:
        name = "Multiplayer";
        break;
    default:
        return 0;
    }
    char drive = 0;
    do {
        if (DAT_00511de0 != 0)
            drive = drive ? '\0' : 'h';
        else
            drive = FindNextCdDrive(drive);
        if (drive == 0)
            continue;
        char path[256];
        sprintf(path, "%c:\\TOTALA.ID", drive);
        if (path[0] != drive)
            DAT_00511de4 = 1;
        TdfFile parser;
        if (((TdfFile*)&parser)->LoadFile(path)
            && ((TdfFile*)&parser)->SelectRecord("Contents")
            && parser.current->GetFieldInt(name, 0))
            return drive;
    } while (drive);
    return 0;
}
