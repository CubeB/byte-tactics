// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int LoadFile(char* file);
};

class Class_004c3410 {
public:
    int SelectRecord(char* name);
};

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, int size, char* def);
};

extern char DAT_005119b8[];
extern char* g_game;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
char* __stdcall Translate(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);

// FUNCTION: 0x429000
void CheckGpfVersion()
{
    Class_004c2ea0 parser;
    char buf[64];
    char path[256];
    int found = 0;

    BuildDataPath(path, "gamedata", "version", "tdf");
    if (((Class_004c2f60*)&parser)->LoadFile(path)) {
        if (((Class_004c3410*)&parser)->SelectRecord("Version")) {
            if (((TdfRecord*)parser.current)->GetFieldString(buf, "GPFVersion", 0x40, DAT_005119b8)) {
                found = 1;
                if (_strcmpi("v3.0", buf) != 0) {
                    OpenMessageBox(g_game + 0x519,
                                 Translate("Warning!  Your copy of Revision.GPF is the wrong version for this executable.  You may experience some problems if you continue playing.  Please download the latest version of the TA patch from www.cavedog.com and reinstall the patch."),
                                 0x1e0, 1, 1);
                }
            }
        }
        if (found == 0) {
            OpenMessageBox(g_game + 0x519,
                         Translate("Warning!  Your copy of Revision.GPF is the wrong version for this executable.  You may experience some problems if you continue playing.  Please download the latest version of the TA patch from www.cavedog.com and reinstall the patch."),
                         0x1e0, 1, 1);
        }
    }
}