// Decompiled by Claude Opus 5.5. Names are provisional.
// Click handler of the unit build/orders panel (installed by 0x41ace0 and
// 0x41b0f0): PREV/NEXT/ORDERS/BUILD buttons set request flags, a unit-type
// entry whose type has field_22f == 0 switches to mode 0xe with that type,
// the orders buttons go to FUN_0041a490, and any other entry adds or removes
// build queue entries (5 at a time when IsKeyDown(0xf9) is set).
// Notes: the per-button FUN_004ab0a0 calls are one call after an if/else-if
// chain (MSVC duplicates it into every branch; writing it in each branch puts
// the menu in esi instead of edi). The type-id buffer is char[17]: a [20]
// buffer is placed above the name buffer. The canBuild bit is tested in its
// own nested if (inside the && chain it becomes `test byte ptr`). <windows.h>
// is needed for the base/index order in the buildTypes lookup (found with
// tools/headers.py).

#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0041aa00;

struct Layer_0041aa00 {
    int unknown_0;
    Entry_0041aa00* entries;             // +0x04
};

struct Menu_0041aa00 {
    char unknown_0[0x18];
    Layer_0041aa00* layer;               // +0x18
    char unknown_1c[0x60 - 0x1c];
    int index;                           // +0x60
};

struct UnitType_0041aa00 {
    char unknown_0[0x111];
    unsigned int flags;                  // +0x111
};

struct Unit {
    char unknown_0[0x10];
    UnitType_0041aa00* type;             // +0x10
    char unknown_14[0x110 - 0x14];
    unsigned int flags_0 : 4;            // +0x110
    unsigned int canBuild : 1;           // bit 4
    unsigned int flags_5 : 24;
    unsigned int flag_29 : 1;            // bit 29
    unsigned int flags_30 : 2;
    char unknown_114[0x118 - 0x114];
};

struct BuildType_0041aa00 {
    char unknown_0[0x22f];
    unsigned char field_22f;             // +0x22f
    char unknown_230[0x249 - 0x230];
};

struct Sub_0041aa00 {
    char unknown_0[0x10];
};

struct Game {
    char unknown_0[0x519];
    Sub_0041aa00 menu;                   // +0x519
    char unknown_529[0x2cc3 - 0x529];
    unsigned char field_2cc3;            // +0x2cc3
    unsigned short field_2cc4;           // +0x2cc4
    char unknown_2cc6[0x14357 - 0x2cc6];
    Unit* units;                         // +0x14357
    char unknown_1435b[0x1439b - 0x1435b];
    BuildType_0041aa00* buildTypes;      // +0x1439b
    char unknown_1439f[0x37e9c - 0x1439f];
    unsigned short unitIndex;            // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags_0 : 7;          // +0x37ebe
    unsigned short next : 1;             // bit 7
    unsigned short prev : 1;             // bit 8
    unsigned short orders : 1;           // bit 9
    unsigned short build : 1;            // bit 10
    unsigned short flags_11 : 5;
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall GetGadgetName(Entry_0041aa00* entries, char* name, int index);
unsigned short __stdcall FindUnitTypeId(char* name);
void __stdcall FUN_004ab0a0(Menu_0041aa00* menu);
void __stdcall FUN_0047f1a0(char* name, int param_2);
int __stdcall FUN_0041a490(Menu_0041aa00* menu, Entry_0041aa00* entries);
int __stdcall FUN_00419be0(Menu_0041aa00* menu, Entry_0041aa00* entries);
int __stdcall IsKeyDown(int key);
int __stdcall FUN_004ab6b0(Menu_0041aa00* menu);
void __stdcall FUN_00419b00(char* name, Unit* unit, int count);
void __stdcall FUN_004199b0(Menu_0041aa00* menu, Unit* unit);
void __stdcall FUN_0049fa90(Sub_0041aa00* menu);

// FUNCTION: 0x41aa00
void __stdcall FUN_0041aa00(Menu_0041aa00* menu)
{
    if (menu->index != -1) {
        Entry_0041aa00* entries = menu->layer->entries;
        char idName[17];
        GetGadgetName(entries, idName, menu->index);
        idName[16] = 0;
        unsigned short id = FindUnitTypeId(idName);
        Unit* unit = &g_game->units[g_game->unitIndex];
        char name[32];
        GetGadgetName(entries, name, menu->index);
        if (strstr(name, "PREV")) {
            g_game->prev = 1;
        } else if (strstr(name, "NEXT")) {
            g_game->next = 1;
        } else if (strstr(name, "ORDERS")) {
            g_game->orders = 1;
            FUN_0047f1a0("ordersbutton", 0);
        } else if (strstr(name, "BUILD")) {
            g_game->build = 1;
            FUN_0047f1a0("buildbutton", 0);
        } else if (id != 0 && g_game->buildTypes[id].field_22f == 0) {
            g_game->field_2cc3 = 0xe;
            g_game->field_2cc4 = id;
            FUN_0047f1a0("addbuild", 0);
        } else if (!FUN_0041a490(menu, entries) && !FUN_00419be0(menu, entries)) {
            if (unit->canBuild) {
                char text[256];
                GetGadgetName(entries, text, menu->index);
                if (IsKeyDown(0xf9)) {
                    if (FUN_004ab6b0(menu) == 1)
                        FUN_00419b00(text, unit, 5);
                    else
                        FUN_00419b00(text, unit, -5);
                } else {
                    if (FUN_004ab6b0(menu) == 1)
                        FUN_00419b00(text, unit, 1);
                    else
                        FUN_00419b00(text, unit, -1);
                }
                if (unit->flag_29 || (unit->type->flags & 0x10000000))
                    FUN_004199b0(menu, unit);
                FUN_0049fa90(&g_game->menu);
            }
        }
        FUN_004ab0a0(menu);
    }
}
