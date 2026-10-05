// Decompiled by Claude Opus 5.5. Names are provisional.
// Click handler for the unit orders panel: cycles the move order, fire order,
// activation or cloak setting named by the clicked entry, sends the matching
// order and updates the button. Returns 0 when the entry is none of these.
// FUN_004a11c0's value is declared int here (its own file has short): with
// short, MSVC shifts the 2-bit fields in a byte register (shr dl, 3).

#include <string.h>

class Class_00438760 {
public:
    unsigned char index;
    Class_00438760(const char* name);
};

#pragma pack(push, 1)
struct Entry_0041a490;

struct Unit_0041a490 {
    char unknown_0[0x118];
};

struct Menu_0041a490 {
    char unknown_0[0x60];
    int index;                           // +0x60
};

struct Sub_0041a490 {
    char unknown_0[0x10];
};

struct Game_0041a490 {
    char unknown_0[0x519];
    Sub_0041a490 menu;                   // +0x519
    char unknown_529[0x2c76 - 0x529];
    char orders[0x10];                   // +0x2c76
    char unknown_2c86[0x14357 - 0x2c86];
    Unit_0041a490* units;                // +0x14357
    char unknown_1435b[0x37e9c - 0x1435b];
    unsigned short unitIndex;            // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags_0 : 12;         // +0x37ebe
    unsigned short fireOrder : 3;        // bits 12-14
    unsigned short flags_15 : 1;
    unsigned short moveOrder : 3;        // +0x37ec0 bits 0-2
    unsigned short cloak : 2;            // bits 3-4
    unsigned short activation : 2;       // bits 5-6
    unsigned short flags_7 : 9;
};
#pragma pack(pop)

extern Game_0041a490* g_game;

void __stdcall FUN_0049fed0(Entry_0041a490* entries, char* name, int index);
void __stdcall FUN_0048cf30(void* a, int b, Class_00438760 kind, int d, int e, int f);
void __stdcall FUN_0047f1a0(char* name, int param_2);
void __stdcall FUN_004a11c0(Sub_0041a490* menu, int index, int value);
void __stdcall FUN_0041a120(Unit_0041a490* unit);
void __stdcall FUN_004a81e0(Sub_0041a490* menu, int value);

// Inlined copy of FUN_00419630.
static inline int Contains(Entry_0041a490* entries, char* text, int index)
{
    char name[32];
    FUN_0049fed0(entries, name, index);
    return strstr(name, text) != 0;
}

// FUNCTION: 0x41a490
int __stdcall FUN_0041a490(Menu_0041a490* menu, Entry_0041a490* entries)
{
    Unit_0041a490* unit = &g_game->units[g_game->unitIndex];
    void* orders = g_game->orders;
    int index = menu->index;

    if (Contains(entries, "MOVEORD", index)) {
        switch (g_game->moveOrder) {
        case 0:
            FUN_0048cf30(orders, 0, "STANDING_MOVEORDER", 0, 1, 0);
            g_game->moveOrder = 1;
            break;
        case 1:
            FUN_0048cf30(orders, 0, "STANDING_MOVEORDER", 0, 2, 0);
            g_game->moveOrder = 2;
            break;
        case 2:
        case 3:
            FUN_0048cf30(orders, 0, "STANDING_MOVEORDER", 0, 0, 0);
            g_game->moveOrder = 0;
            break;
        }
        FUN_0047f1a0("setmoveorders", 0);
        FUN_004a11c0(&g_game->menu, index, g_game->moveOrder);
    } else if (Contains(entries, "FIREORD", index)) {
        switch (g_game->fireOrder) {
        case 0:
            FUN_0048cf30(orders, 0, "STANDING_FIREORDER", 0, 1, 0);
            g_game->fireOrder = 1;
            break;
        case 1:
            FUN_0048cf30(orders, 0, "STANDING_FIREORDER", 0, 2, 0);
            g_game->fireOrder = 2;
            break;
        case 2:
        case 3:
            FUN_0048cf30(orders, 0, "STANDING_FIREORDER", 0, 0, 0);
            g_game->fireOrder = 0;
            break;
        }
        FUN_0047f1a0("setfireorders", 0);
        FUN_004a11c0(&g_game->menu, index, g_game->fireOrder);
    } else if (Contains(entries, "STATUS", index) || Contains(entries, "ONOFF", index)) {
        switch (g_game->activation) {
        case 0:
            FUN_0048cf30(orders, 0, "ACTIVATE", 0, 0, 0);
            g_game->activation = 1;
            break;
        case 1:
            FUN_0048cf30(orders, 0, "DEACTIVATE", 0, 0, 0);
            g_game->activation = 0;
            break;
        case 2:
            FUN_0048cf30(orders, 0, "ACTIVATE", 0, 0, 0);
            g_game->activation = 1;
            break;
        }
        FUN_0047f1a0("specialorders", 0);
        FUN_004a11c0(&g_game->menu, index, g_game->activation);
    } else if (Contains(entries, "CLOAK", index)) {
        if (g_game->cloak) {
            FUN_0048cf30(orders, 0, "CLOAK_OFF", 0, 0, 0);
            g_game->cloak = 0;
        } else {
            FUN_0048cf30(orders, 0, "CLOAK_ON", 0, 0, 0);
            g_game->cloak = 1;
        }
        FUN_0047f1a0("specialorders", 0);
        FUN_004a11c0(&g_game->menu, index, g_game->cloak);
    } else {
        return 0;
    }
    FUN_0041a120(g_game->unitIndex ? unit : 0);
    FUN_004a81e0(&g_game->menu, 0x40);
    return 1;
}
