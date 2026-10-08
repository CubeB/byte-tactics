// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Steps the selected unit's build menu to the next page (wrapping to page 1),
// or closes it after the last page when param_1 is set.
// Needed: without it the sub-object pointer is allocated to edx, not esi.
// Kept its own file: in control_panel.cpp the include set and prelude move
// that pointer from esi to edx.
#include <stdlib.h>

#pragma pack(push, 1)
struct Sub_0041bde0 {
    char unknown_0[0x22e];
    unsigned char pageCount;             // +0x22e
};

struct Unit {
    char unknown_0[0x92];
    Sub_0041bde0* sub;                   // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                      // +0xa6
    char unknown_a8[0x110 - 0xa8];
    unsigned int bits_110_0 : 22;
    unsigned int menuOpen : 1;           // bit 22
    unsigned int page : 3;               // bits 23-25
    unsigned int bits_110_26 : 6;
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                         // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;            // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned char flags;                 // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;
void __stdcall PlaySoundByName(char* name, int param);

// FUNCTION: 0x41bde0
void __stdcall StepBuildMenuPage(int param_1)
{
    unsigned short index = g_game->unitIndex;
    if (index != 0) {
        Unit* unit = &g_game->units[index];
        if (unit->field_a6 == 0)
            unit = 0;
        if (unit != 0) {
            if (param_1 != 0) {
                if (!unit->menuOpen) {
                    unit->menuOpen = 1;
                    unit->page = 1;
                } else if (unit->page == unit->sub->pageCount - 1) {
                    unit->menuOpen = 0;
                } else {
                    unit->page++;
                }
            } else {
                // Compared as page == pageCount - 1; keeps the original load order.
                if (unit->page == unit->sub->pageCount - 1)
                    unit->page = 1;
                else
                    unit->page++;
                unit->menuOpen = 1;
            }
            g_game->flags |= 0x10;
        }
    }
    PlaySoundByName("nextbuildmenu", 0);
}
