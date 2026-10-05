// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

extern char DAT_0050372c[]; // "*"
extern char DAT_00505f40[]; // "LST"
extern char DAT_00505f18[]; // "GAMES"
extern char DAT_00505f30[]; // "SAVEGAME NAMES"
extern char DAT_00505f20[]; // "SAVEGAME DESCS"
extern char DAT_005119b8[];
extern char* DAT_005091c8;  // savegame directory
extern char* g_game;
extern void* DAT_005129ac;
extern void* DAT_005129b0;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bc930(const char* path, int flag);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall ScanDirectory(char* path, void* buffer, char* p3, int p4, int p5, int p6);
void __stdcall FUN_004a32a0(void* menu, char* name, void* text, int value, int flag);

// FUNCTION: 0x44b4e0
void* __stdcall FUN_0044b4e0(int* out)
{
    char path[0x100];
    BuildDataPath(path, DAT_005091c8, DAT_0050372c, DAT_00505f40);
    int count = FUN_004bc930(path, 0);
    *out = count;
    if (count == 0) {
        FUN_004a32a0(g_game + 0x519, DAT_00505f18, DAT_005119b8, 0, 0);
        return 0;
    }
    DAT_005129ac = FUN_004d83b0(DAT_00505f30, count << 8);
    DAT_005129b0 = FUN_004d83b0(DAT_00505f20, *out << 8);
    memset(DAT_005129b0, 0, *out << 8);
    memset(DAT_005129ac, 0, *out << 8);
    ScanDirectory(path, DAT_005129ac, 0, 0, 0, 1);
    FUN_004a32a0(g_game + 0x519, DAT_00505f18, DAT_005129ac, *out, 0);
    return *out ? DAT_005129ac : 0;
}
