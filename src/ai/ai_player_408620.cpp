// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <windows.h>

#pragma pack(push, 1)
struct Item_00408620 {                 // 0x249 bytes
    char unknown_0[0x20];
    char name[0x249 - 0x20];           // +0x20
};

struct Game_00408620 {
    char unknown_0[0x1439b];
    Item_00408620* items;              // +0x1439b
};
#pragma pack(pop)

struct Obj_00408620 {
    char unknown_0[0x5c];
    int field_5c;                      // +0x5c
};

extern Game_00408620* g_game;

unsigned short __stdcall FUN_0040bdb0(int param_1, Obj_00408620* param_2);
void __stdcall FUN_00419b00(char* name, Obj_00408620* param_2, int param_3);

class Class_00408620 {
public:
    char unknown_0[0x10];
    int field_10;                      // +0x10

    void FUN_00408620(Obj_00408620* param_1);
};

// FUNCTION: 0x408620
void Class_00408620::FUN_00408620(Obj_00408620* param_1)
{
    if (param_1->field_5c != 0)
        return;
    unsigned short idx = FUN_0040bdb0(field_10, param_1);
    if (idx != 0)
        FUN_00419b00(g_game->items[idx].name, param_1, 1);
}
