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

class Class_004b3630 {
public:
    void FUN_004b3630();
};

class Class_004b48a0 {
public:
    char* FUN_004b48a0(char* name, char* def);
};

class Class_004b3620 {
public:
    int field_0;
    Class_004b3620* FUN_004b3620();
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

char* __stdcall FUN_004290f0(char* buf, char* dir, char* name, char* ext);
Class_004b3620* __stdcall FUN_00432520(char* name);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int value, int flag);
void __stdcall FUN_004af320(char* path, char* list, char* sizes, int mode, int flag, int what);
char* __stdcall FUN_004b6af0(char* text, int n);
int __stdcall FUN_004bc930(const char* path, int flag);
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

// FUNCTION: 0x492b10
char* __stdcall FUN_00492b10(int* count)
{
    char buf[0x100];
    int found;
    char* copy;
    int i;
    char* dp;

    FUN_004290f0(buf, DAT_005091c8, "*", "SAV");
    *count = FUN_004bc930(buf, 0);
    if (*count == 0) {
        FUN_004a32a0(g_game->menu, "GAMES", DAT_005119b8, 0, 0);
        return 0;
    }
    DAT_0051f2e0 = (char*)FUN_004d83b0("SAVEGAME NAMES", *count << 8);
    DAT_0051f2e4 = (char*)FUN_004d83b0("SAVEGAME DESCS", *count << 6);
    memset(DAT_0051f2e4, 0, *count << 6);
    memset(DAT_0051f2e0, 0, *count << 8);
    FUN_004af320(buf, DAT_0051f2e0, 0, 0, 0, 1);
    dp = DAT_0051f2e4;
    found = 0;
    copy = (char*)FUN_004d83b0("SAVEGAME2", *count << 8);
    memcpy(copy, DAT_0051f2e0, *count << 8);
    for (i = 0; i < *count; i++) {
        sprintf(buf, "%s\\%s", DAT_005091c8, FUN_004b6af0(copy, i));
        Class_004b3620* file = FUN_00432520(buf);
        char* desc = 0;
        if (file)
            desc = ((Class_004b48a0*)file)->FUN_004b48a0("Description", 0);
        if (file && desc) {
            strcpy(buf, desc);
            strcpy(dp, buf);
            dp += strlen(buf) + 1;
            found++;
            ((Class_004b3630*)file)->FUN_004b3630();
            delete file;
        } else {
            char* d = FUN_004b6af0(DAT_0051f2e0, found);
            for (int j = i + 1; j < *count; j++) {
                char* next = FUN_004b6af0(copy, j);
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
