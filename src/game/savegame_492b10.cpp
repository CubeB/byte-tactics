// Decompiled by Sonnet 5.5. Names are provisional.
// Lists the saved games: builds the search pattern for the "SAV" files in the
// save directory, asks how many there are, and fills two tables: the file
// names (0x100 bytes each) and, packed one after another, each save's
// "Description" line. A file without a description is dropped by shifting the
// remaining names down. Publishes the descriptions in the "GAMES" menu and
// returns the names table (0 when there are no saves); the count goes back
// through the parameter.
#include <string.h>
#include <stdio.h>

class HapiBank {
public:
    int field_0;
    HapiBank* InitBank();
    void CloseBank();
    char* GetStringItem(char* name, char* def);
};

struct Game {
    char unknown_0[0x519];
    char menu[1];                      // +0x519
};

extern Game* g_game;
extern char* DAT_005091c8;
extern char DAT_005119b8[];
extern char* DAT_0051f2e0;
extern char* DAT_0051f2e4;

char* __stdcall BuildDataPath(char* buf, char* dir, char* name, char* ext);
HapiBank* __stdcall FUN_00432520(char* name);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int value, int flag);
void __stdcall ScanDirectory(char* path, char* list, char* sizes, int mode, int flag, int what);
char* __stdcall SkipTextLines(char* text, int n);
int __stdcall CountDirectoryEntries(const char* path, int flag);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// FUNCTION: 0x492b10
char* __stdcall ListSavedGames(int* count)
{
    char buf[0x100];
    int found;
    char* copy;
    int i;
    char* dp;

    BuildDataPath(buf, DAT_005091c8, "*", "SAV");
    *count = CountDirectoryEntries(buf, 0);
    if (*count == 0) {
        FUN_004a32a0(g_game->menu, "GAMES", DAT_005119b8, 0, 0);
        return 0;
    }
    DAT_0051f2e0 = (char*)FUN_004d83b0("SAVEGAME NAMES", *count << 8);
    DAT_0051f2e4 = (char*)FUN_004d83b0("SAVEGAME DESCS", *count << 6);
    memset(DAT_0051f2e4, 0, *count << 6);
    memset(DAT_0051f2e0, 0, *count << 8);
    ScanDirectory(buf, DAT_0051f2e0, 0, 0, 0, 1);
    dp = DAT_0051f2e4;
    found = 0;
    copy = (char*)FUN_004d83b0("SAVEGAME2", *count << 8);
    memcpy(copy, DAT_0051f2e0, *count << 8);
    for (i = 0; i < *count; i++) {
        sprintf(buf, "%s\\%s", DAT_005091c8, SkipTextLines(copy, i));
        HapiBank* file = FUN_00432520(buf);
        char* desc = 0;
        if (file)
            desc = ((HapiBank*)file)->GetStringItem("Description", 0);
        if (file && desc) {
            strcpy(buf, desc);
            strcpy(dp, buf);
            dp += strlen(buf) + 1;
            found++;
            ((HapiBank*)file)->CloseBank();
            delete file;
        } else {
            char* d = SkipTextLines(DAT_0051f2e0, found);
            for (int j = i + 1; j < *count; j++) {
                char* next = SkipTextLines(copy, j);
                strcpy(d, next);
                d += strlen(next) + 1;
            }
        }
    }
    FUN_004d85a0(copy);
    FUN_004a32a0(g_game->menu, "GAMES", DAT_0051f2e4, found, 0);
    *count = found;
    return found ? DAT_0051f2e0 : 0;
}
