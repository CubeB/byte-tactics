// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Opens the selected unit's build menu at page param_1 (closes it for 0),
// if the unit has that many pages.
// The unit lookup is written out in place (the GetSelectedUnit helper used by
// 0x41c180 gives different code here), and <windows.h> is needed: without it
// MSVC loads the page count before param_1 and swaps ecx/edx/ebx (found with
// tools/headers.py; <ddraw.h> works too).
#include <windows.h>

#pragma pack(push, 1)
struct Sub_0041c060 {
    char unknown_0[0x22e];
    unsigned char pageCount;             // +0x22e
};

struct Unit {
    char unknown_0[0x92];
    Sub_0041c060* sub;                   // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                      // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int bits_110_0 : 22;
    unsigned int menuOpen : 1;           // bit 22
    unsigned int page : 3;               // bits 23-25
    unsigned int bits_110_26 : 6;
    char unknown_114[0x118 - 0x114];
};

struct Game_0041c060 {
    char unknown_0[0x14357];
    Unit* units;                         // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;            // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flags;                 // +0x37ebe
};
#pragma pack(pop)

extern Game_0041c060* g_game;
void __stdcall FUN_0047f1a0(char* name, int param);

// FUNCTION: 0x41c060
void __stdcall FUN_0041c060(int param_1)
{
    unsigned short index = g_game->unitIndex;
    if (index != 0) {
        Unit* unit = &g_game->units[index];
        if (unit->field_a6 == 0)
            unit = 0;
        if (unit != 0) {
            if (param_1 < unit->sub->pageCount) {
                unit->menuOpen = param_1 > 0;
                if (param_1 != 0)
                    unit->page = param_1;
                g_game->flags |= 0x10;
                FUN_0047f1a0("nextbuildmenu", 0);
            }
        }
    }
}
