// Decompiled by deepseek-v4.1-flash. Names are provisional.
// GUI callback for a "DESCLIST" entry: reads a count from the gadget text,
// formats it as "COUNT<n>", clamps the gadget's slider value to 100 (writing
// "No Limit"/-1 above that), stores it in the global 0x62-byte item table at
// 0x5129b4, pushes it to the unit type named by the item's field_52, and
// updates the entry's per-unit flag bytes.
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct Entry_0044be70 {
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0xb6 - 0x12];
    short count;                       // +0xb6
    char unknown_b8[0xbc - 0xb8];
    short field_bc;                    // +0xbc
    char unknown_be[0xd6 - 0xbe];
    char* field_d6;                    // +0xd6
    char unknown_da[0x15b - 0xda];
};

struct Item_005129b4 {
    char unknown_0[0x52];
    int field_52;                      // +0x52
    char unknown_56[0x5a - 0x56];
    int field_5a;                      // +0x5a
    int field_5e;                      // +0x5e
};
#pragma pack(pop)

struct Inner_0044be70 {
    int unknown_0;
    void* gadgets;                     // +0x4
};

struct Menu_0044be70 {
    char unknown_0[0x18];
    Inner_0044be70* inner;             // +0x18
};

struct UnitType_0044be70 {
    char unknown_0[0x249];
};

#pragma pack(push, 1)
struct Game_0044be70 {
    char unknown_0[0x519];
    Menu_0044be70 menu;                // +0x519
    char unknown_535[0x2a30 - 0x535];
    void* field_2a30;                  // +0x2a30
    char unknown_2a34[0x1439b - 0x2a34];
    UnitType_0044be70* field_1439b;    // +0x1439b
};
#pragma pack(pop)

class Class_0046e330 {
public:
    void FUN_0046e550(UnitType_0044be70* unit, int value);
};

extern Game_0044be70* g_game;
extern Item_005129b4* DAT_005129b4;

Entry_0044be70* __stdcall FUN_0049ff90(void* gadgets, char* name);
int __stdcall FUN_0045ba20(char* text);
char* __stdcall FUN_004c5740(char* text);
void __stdcall FUN_004a0bf0(void* obj, char* name, int param_3, int param_4);

// FUNCTION: 0x44be70
void __stdcall FUN_0044be70(void* obj, char* gadget)
{
    int n = atoi(gadget + 8);
    Entry_0044be70* desc = FUN_0049ff90(g_game->menu.inner->gadgets, "DESCLIST");
    char count[20];
    sprintf(count, "COUNT%d", n);
    int value = FUN_0045ba20(gadget);
    char buf[20];
    if (value > 0x64) {
        sprintf(buf, FUN_004c5740("No Limit"));
        value = -1;
    } else {
        _itoa(value, buf, 10);
    }
    DAT_005129b4[n + desc->field_bc].field_5a = value;
    ((Class_0046e330*)g_game->field_2a30)->FUN_0046e550(
        &g_game->field_1439b[DAT_005129b4[n + desc->field_bc].field_52], value);
    desc->field_d6[n + desc->field_bc] = DAT_005129b4[n + desc->field_bc].field_5e == 0;
    desc->field_d6[n + desc->field_bc] |= DAT_005129b4[n + desc->field_bc].field_5a == 0 ? 2 : 0;
    FUN_004a0bf0(obj, count, (int)buf, 0);
}
