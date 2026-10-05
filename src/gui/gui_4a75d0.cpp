// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <string.h>

struct Holder_004a75d0 {
    char unknown_0[0xc0];
    void* gaf;                         // +0xc0
};

struct Screen_004a75d0 {
    char unknown_0[4];
    Holder_004a75d0* holder;           // +0x4
};

struct Obj_004a75d0 {
    char unknown_0[4];
    void* gaf;                         // +0x4
    char unknown_8[0x18 - 8];
    Screen_004a75d0* screen;           // +0x18
    char unknown_1c[0xab6 - 0x1c];
    char name[0x100];                  // +0xab6
};

void __stdcall ChangeExtension(char* out, char* in, const char* ext);
int __stdcall FUN_004bbc40(char* path);
void* __stdcall LoadGaf(char* path);

// FUNCTION: 0x4a75d0
int __stdcall FUN_004a75d0(Obj_004a75d0* obj, char* name)
{
    char path[256];
    path[0] = 0;
    if (obj->name[0] != 0)
        strncpy(path, obj->name, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (FUN_004bbc40(path)) {
        obj->screen->holder->gaf = LoadGaf(path);
        if (obj->screen->holder->gaf != 0)
            return 1;
    }
    return 0;
}
