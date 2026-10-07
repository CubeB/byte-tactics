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
    int field_52;                      // +0x52 unit type index
    int field_56;                      // +0x56 previous value
    int field_5a;                      // +0x5a current value
    int field_5e;                      // +0x5e
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
    Flags_0044c420 field_245;          // +0x245
};

struct Inner_0044c420 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_0044c420 {
    char unknown_0[0x18];
    Inner_0044c420* inner;             // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                      // +0x60 current gadget, -1 for none
};

struct UnitSync {
    void SetUnitLimit(Item_0044c420* unit, int value);
    int DisallowUnit(Item_0044c420* unit);
    int AllowUnit(Item_0044c420* unit);
};

struct Game {
    char unknown_0[0x2a30];
    void* queue;                       // +0x2a30
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
extern int* DAT_005129b8;
extern Record_005129b4* DAT_005129b4;
extern int* DAT_005129c4;

Entry_0044c420* __stdcall FindGadgetChecked(void* gadgets, char* name);
int __stdcall IsCurrentGadgetNamed(Menu_0044c420* gui, char* name);
void __stdcall FUN_004ab0a0(Menu_0044c420* obj);
void __stdcall FUN_004ab190(int param_1, int param_2);
void __stdcall FUN_0049fa90(Menu_0044c420* obj);
void __stdcall PlaySoundByName(char* name, int param_2);
void __stdcall UpdateUnitSliders(Menu_0044c420* menu, int value);
void OpenLoadListDialog();
void OpenSaveGameDialog();
void __stdcall FreeSurface(Struct_004c6ac0* obj);
void __cdecl FUN_004d85a0(void* p);
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
    if (menu->field_60 == -1) {
        i = 0;
        p = DAT_005129b8;
        if (desc->count > 0) {
            do {
                if (*p != 0)
                    FreeSurface((Struct_004c6ac0*)*p);
                p++;
                i++;
            } while (i < desc->count);
        }
        FUN_004d85a0(desc->field_c6);
        FUN_004d85a0(DAT_005129b8);
        DAT_005129b8 = 0;
        FUN_004d85a0(desc->field_c2);
        FUN_004ab190((int)menu, 1);
        if (IsHostLocal() != 0) {
            for (i = 0; i < g_game->count; i++) {
                type = DAT_005129b4[i].field_52;
                if (type != 0) {
                    item = &g_game->items[type];
                    if (item->field_245.bits.flag) {
                    } else {
                        if (DAT_005129b4[i].field_5a == 0)
                            ((UnitSync*)g_game->queue)->DisallowUnit(item);
                        else
                            ((UnitSync*)g_game->queue)->AllowUnit(item);
                    }
                }
            }
        }
        FUN_004d85a0(DAT_005129b4);
        FUN_004d85a0(DAT_005129c4);
        FUN_004d85a0(desc->field_d6);
        pic = FindGadgetChecked(menu->inner->gadgets, "PICLIST");
        if (pic != 0 && pic->field_c6 != 0)
            FUN_004d85a0(pic->field_c6);
        DAT_005129b4 = 0;
        return;
    }

    if (IsCurrentGadgetNamed(menu, "Load") != 0) {
        PlaySoundByName("Options", 0);
        OpenLoadListDialog();
        FUN_004ab0a0(menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Save") != 0) {
        PlaySoundByName("Options", 0);
        OpenSaveGameDialog();
        FUN_004ab0a0(menu);
        return;
    }
    if (IsCurrentGadgetNamed(menu, "Reset") != 0) {
        PlaySoundByName("Options", 0);
        for (i = 0; i < g_game->count; i++) {
            type = DAT_005129b4[i].field_52;
            if (type != 0) {
                if ((g_game->items[type].field_245.raw & 0x10000) == 0)
                    DAT_005129b4[i].field_5a = 100;
                else
                    DAT_005129b4[i].field_5a = 0;
                if (DAT_005129b4[i].field_5a != DAT_005129b4[i].field_56) {
                    ((UnitSync*)g_game->queue)->SetUnitLimit(
                        &g_game->items[DAT_005129b4[i].field_52],
                        DAT_005129b4[i].field_5a);
                }
            }
        }
        UpdateUnitSliders(menu, 0);
        FUN_0049fa90(menu);
        FUN_004ab0a0(menu);
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
            if (g_game->items[i].field_245.bits.flag) {
            } else if (g_game->items[i].name != 0) {
                ((UnitSync*)g_game->queue)->SetUnitLimit(
                    &g_game->items[i], DAT_005129c4[n]);
                n++;
            }
        }
        return;
    }
    if (menu->field_60 != -1)
        FUN_004ab0a0(menu);
}
