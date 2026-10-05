// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Entry_00445300 {
    unsigned char state;               // +0x00
    char unknown_1;
    char name[0x13];                   // +0x02
    short field_15;                    // +0x15
    char unknown_17[4];
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    char text[0x13e - 0xb6];           // +0xb6
};

struct Holder_00445300 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_00445300 {
    char unknown_0[0x18];
    Holder_00445300* holder;           // +0x18
};

struct Game {
    char unknown_0[0x519];
    Menu_00445300 menu;                // +0x519
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FindGadgetIndex(void* gadgets, char* name, int type);
void __stdcall FUN_004a5d50(Menu_00445300* menu, int index);

// FUNCTION: 0x445300
void __stdcall FUN_00445300(Entry_00445300* param_1)
{
    if (param_1->state == 1) {
        Entry_00445300 tmp = *param_1;
        int index = FindGadgetIndex(g_game->menu.holder->gadgets, param_1->name, 0xe);
        FUN_004a5d50(&g_game->menu, index);
        param_1->field_15 += 2;
        param_1->state = 5;
        strcpy(param_1->text, tmp.text);
        param_1->flags |= 0x10;
    }
}
