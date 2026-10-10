// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Handler/teardown for the "Options" menu. When the menu closes (its current
// gadget is -1) it frees the DESCLIST/PICLIST scratch globals (0x5129b4,
// 0x5129b8, 0x5129c4) and unregisters the queued unit rectangles; otherwise it
// dispatches the gadget named Load/Save/Reset/OK/Cancel.

// <stdio.h>, not <windows.h>: fixes the operand order and scheduling.
#include <stdio.h>

#pragma pack(push, 1)
struct Entry_0044c420 {                // DESCLIST / PICLIST gadget
    char unknown_0[0xbc];
    short field_bc;                    // +0xbc
    char unknown_be[0xc0 - 0xbe];
    short count;                       // +0xc0 picture slot count
    void* field_c2;                    // +0xc2 freed scratch
    void* field_c6;                    // +0xc6 freed scratch
    char unknown_ca[0xd6 - 0xca];
    char* field_d6;                    // +0xd6 per-item flags, freed
};

struct Record_005129b4 {               // 0x62-byte slider/picture record
    char unknown_0[0x52];
    int unitIndex;                     // +0x52 unit type index
    int previousMax;                   // +0x56 previous value
    int max;                           // +0x5a current value
    int peerEnabled;                   // +0x5e
};

union Flags_0044c420 {                 // the dword at +0x245
    unsigned int raw;
    struct {
        unsigned int low : 15;
        unsigned int flag : 1;         // bit 15
        unsigned int high : 16;
    } bits;
};

struct Item_0044c420 {                 // 0x249-byte unit type instance
    char unknown_0[0x20];
    char name[0x225];                  // +0x20
    Flags_0044c420 flags2;             // +0x245
};

struct Inner_0044c420 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_0044c420 {
    char unknown_0[0x18];
    Inner_0044c420* inner;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int current;                       // +0x60 current gadget, -1 for none
};

#include "../network/unit_sync.h"

struct Game {
    char unknown_0[0x2a30];
    UnitSync* sync;                    // +0x2a30
    char unknown_2a34[0x1438f - 0x2a34];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Item_0044c420* items;              // +0x1439b
};
#pragma pack(pop)

struct Struct_004c6ac0 {
    char unknown_0[0x2c];
    unsigned char flags;               // +0x2c
};

extern Game* g_game;
extern int* g_unitRestrictPics;
extern Record_005129b4* g_unitRestrictEntries;
extern int* g_unitRestrictOldCounts;

Entry_0044c420* __stdcall FindGadgetChecked(void* gadgets, char* name);
int __stdcall IsCurrentGadgetNamed(Menu_0044c420* gui, char* name);
void __stdcall ClearSelectedGadget(Menu_0044c420* obj);
void __stdcall SetDescListCleanupFlag(int param_1, int param_2);
void __stdcall MarkChanged(Menu_0044c420* obj);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall UpdateUnitSliders(Menu_0044c420* menu, int value);
void OpenLoadListDialog();
void OpenSaveGameDialog();
void __stdcall FreeSurface(Struct_004c6ac0* obj);
void __cdecl GameFreeThunk(void* p);
int IsHostLocal();

// FUNCTION: 0x44c420
void __stdcall HandleRestrictionsClick(Menu_0044c420* menu)
{
    int i;
    int n;
    int type;
    int* p;
    Item_0044c420* item;
    Entry_0044c420* desc;
    Entry_0044c420* pic;

    desc = FindGadgetChecked(menu->inner->gadgets, "DESCLIST");
    if (menu->current == -1) {
        i = 0;
        p = g_unitRestrictPics;
        if (desc->count > 0) {
            do {
                if (*p != 0)
                    FreeSurface((Struct_004c6ac0*)*p);
                p++;
                i++;
            } while (i < desc->count);
        }
        GameFreeThunk(desc->field_c6);
        GameFreeThunk(g_unitRestrictPics);
        g_unitRestrictPics = 0;
        GameFreeThunk(desc->field_c2);
        SetDescListCleanupFlag((int)menu, 1);
        if (IsHostLocal() != 0) {
            for (i = 0; i < g_game->count; i++) {
                type = g_unitRestrictEntries[i].unitIndex;
                if (type != 0) {
                    item = &g_game->items[type];
                    if (item->flags2.bits.flag) {
                    } else {
                        if (g_unitRestrictEntries[i].max == 0)
                            g_game->sync->DisallowUnit((Unit_0046e330*)item);
                        else
                            g_game->sync->AllowUnit((Unit_0046e330*)item);
                    }
                }
            }
        }
        GameFreeThunk(g_unitRestrictEntries);
        GameFreeThunk(g_unitRestrictOldCounts);
        GameFreeThunk(desc->field_d6);
        pic = FindGadgetChecked(menu->inner->gadgets, "PICLIST");
        if (pic != 0 && pic->field_c6 != 0)
            GameFreeThunk(pic->field_c6);
        g_unitRestrictEntries = 0;
        return;
    }

    if (IsCurrentGadgetNamed(menu, "Load") != 0) {
        PlaySoundByName("Options", 0);
        OpenLoadListDialog();
        ClearSelectedGadget(menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Save") != 0) {
        PlaySoundByName("Options", 0);
        OpenSaveGameDialog();
        ClearSelectedGadget(menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Reset") != 0) {
        PlaySoundByName("Options", 0);
        for (i = 0; i < g_game->count; i++) {
            type = g_unitRestrictEntries[i].unitIndex;
            if (type != 0) {
                if ((g_game->items[type].flags2.raw & 0x10000) == 0)
                    g_unitRestrictEntries[i].max = 100;
                else
                    g_unitRestrictEntries[i].max = 0;
                if (g_unitRestrictEntries[i].max != g_unitRestrictEntries[i].previousMax) {
                    g_game->sync->SetUnitLimit(
                        (Unit_0046e330*)&g_game->items[g_unitRestrictEntries[i].unitIndex],
                        g_unitRestrictEntries[i].max);
                }
            }
        }
        UpdateUnitSliders(menu, 0);
        MarkChanged(menu);
        ClearSelectedGadget(menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "OK") != 0) {
        PlaySoundByName("Options", 0);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Cancel") != 0) {
        PlaySoundByName("Previous", 0);
        n = 0;
        for (i = 1; i < g_game->count; i++) {
            // The original also tests the address of items[i].name (an array at
            // +0x20, so its address can never be null); kept for byte fidelity.
            if (g_game->items[i].flags2.bits.flag) {
            } else if (g_game->items[i].name != 0) {
                g_game->sync->SetUnitLimit(
                    (Unit_0046e330*)&g_game->items[i], g_unitRestrictOldCounts[n]);
                n++;
            }
        }
        return;
    }
    if (menu->current != -1)
        ClearSelectedGadget(menu);
}
