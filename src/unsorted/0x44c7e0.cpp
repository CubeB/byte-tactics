// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds the "RESTRICT2.GUI" unit-restriction dialog: two lists (DESCLIST /
// PICLIST), the per-unit picture/text/restriction scratch arrays, twelve
// sliders, the scroll slider, the energy/metal labels and the Load/Save/Reset
// buttons. Family with 0x44c220 (per-frame update) and 0x44c420 (teardown).
//
// Still differs (35.5%, 1492 vs 1586 bytes): structure is right but the stack
// frame is 0x44 where the original is 0x2c, so several locals are not being
// reused into one slot; the first item loop's sprintf/copy sequences and the
// Load/Save/Reset tail differ in register choice and evaluation order.
// Details worth keeping: the sprintf at 0x44cabf is
//   sprintf(rec, "%s\r%s %dM  %dE", item, FUN_004c5740(item+0xa0), (int)item->metal,
//           (int)item->energy);
// where the two (int) casts of the x87 fields (+0x186 energy, +0x18a metal)
// are pushed as varargs before the FUN_004c5740 call, leaving the stack
// balanced by the later `add esp, 0x18` after that sprintf.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma pack(push, 1)

struct Unit_44c7e0 {
    char unknown_0[0x97];
    unsigned char field_97;            // +0x97
};

struct PlayerEntry_44c7e0 {
    Unit_44c7e0* unit;                 // +0x00
    char unknown_4[0x14b - 4];
};

struct Record_44c7e0 {                 // 0x62-byte slider/picture record
    char name[0x52];                   // +0x00 formatted text
    int field_52;                      // +0x52 unit type index
    int field_56;                      // +0x56
    int field_5a;                      // +0x5a
    int field_5e;                      // +0x5e
};

union Flags_44c7e0 {
    unsigned int raw;
    struct {
        unsigned int low : 15;
        unsigned int flag : 1;         // bit 15
        unsigned int high : 16;
    } bits;
};

struct Item_44c7e0 {                   // 0x249-byte unit type instance
    char unknown_0[0x20];
    char name[0x166];                  // +0x20
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x245 - 0x18e];
    Flags_44c7e0 field_245;            // +0x245
};

struct Gadget_44c7e0 {
    char unknown_0[0x1b];
    unsigned int field_1b;             // +0x1b
    char unknown_1f[0xba - 0x1f];
    short field_ba;                    // +0xba
    char unknown_bc[0xc0 - 0xbc];
    int count;                         // +0xc0
    char unknown_c4[0xce - 0xc4];
    void* field_ce;                    // +0xce
    Record_44c7e0* field_d2;           // +0xd2
    void* field_d6;                    // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x13c - 0xdc];
    int field_13c;                     // +0x13c
    short field_140;                   // +0x140
    char unknown_142[0x144 - 0x142];
    void* field_144;                   // +0x144
    int field_14a;                     // +0x14a
};

struct Info_44c7e0 {
    short pad0;                        // +0x00
    short field_2;                     // +0x02
    int field_4;                       // +0x04
};

struct Inner_44c7e0 {
    char unknown_0[4];
    void* entries;                     // +0x04
};

struct Layer_44c7e0 {
    char unknown_0[4];
    void* entries;                     // +0x04
    void* handler_8;                   // +0x08
    int field_c;                       // +0x0c
    char unknown_10[0x1c - 0x10];
    void* handler_1c;                  // +0x1c
};

struct Game_44c7e0 {
    char unknown_0[0x519];
    char unknown_519[0x531 - 0x519];
    Inner_44c7e0* inner;               // +0x531
    char unknown_535[0x1b8a - 0x535];
    PlayerEntry_44c7e0 players[10];    // +0x1b8a
    char unknown_2858[0x2a30 - 0x2858];
    void* queue;                       // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a34];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x1438f - 0x2a43];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Item_44c7e0* items;                // +0x1439b
};

struct Class_0046e330 {
    void FUN_0046e330(Info_44c7e0* out, Item_44c7e0* item);
};

#pragma pack(pop)

extern Game_44c7e0* g_game;
extern Record_44c7e0* DAT_005129b4;
extern int* DAT_005129b8;
extern int* DAT_005129c4;
extern int DAT_00512768;
extern int DAT_005129c0;

void* __stdcall FUN_004aa8f0(void* menu, const char* name, int size);
void __stdcall FUN_004288d0(const char* name, int a, int b, int c);
void* FUN_004d83b0(char* name, unsigned int size);
Gadget_44c7e0* __stdcall FUN_0049ff90(void* entries, char* name);
int __stdcall FUN_0049fdf0(void* entries, char* name, int type);
Gadget_44c7e0* __stdcall FUN_004a0200(void* entries, char* name);
void __stdcall FUN_004a0bf0(void* menu, char* name, char* text, int flag);
void __stdcall FUN_004a1250(void* menu, char* name, int enabled);
void __stdcall FUN_004a32a0(void* menu, char* name, char* text, int count, int flag);
void __stdcall FUN_004a35a0(void* entries, char* name, void* pics, int count);
void __stdcall FUN_004a81e0(void* menu, int value);
void __stdcall FUN_0049fa90(void* menu);
void __stdcall FUN_0049fb10(void* menu, int value);
void __stdcall FUN_0045b9b0(Gadget_44c7e0* gadget, int value);
void __stdcall FUN_0044bfd0(void* menu, int value);
char* __stdcall FUN_004c5740(char* text);
void FUN_0044c370();
void FUN_0044c220();
void FUN_0044c420();
void FUN_0044be70();
int __cdecl FUN_0044c7a0(const void* a, const void* b);

// FUNCTION: 0x44c7e0
void FUN_0044c7e0()
{
    unsigned char flag;
    Layer_44c7e0* layer;
    Gadget_44c7e0* desc;
    Gadget_44c7e0* pic;
    Gadget_44c7e0* slider;
    Gadget_44c7e0* scroll;
    void* flags;
    int* picArray;
    char* textArray;
    char* dst;
    char* src;
    Info_44c7e0 info;
    char buf[0x20];
    int i;
    int n;
    int count;
    int off;
    int idx;
    Item_44c7e0* item;
    int value;

    flag = g_game->players[g_game->localPlayer].unit->field_97 & 1;

    layer = (Layer_44c7e0*)FUN_004aa8f0((char*)g_game + 0x519, "RESTRICT2.GUI", 0x880);
    layer->handler_8 = (void*)FUN_0044c420;
    layer->field_c = 0;
    layer->handler_1c = (void*)FUN_0044c220;
    DAT_00512768 = 0;
    FUN_004288d0("UnitRestrict5x", 0, 0, 0);

    count = g_game->count;
    flags = FUN_004d83b0("FLAGS", count);
    desc = FUN_0049ff90(layer->entries, "DESCLIST");
    desc->field_ce = (void*)FUN_0044c370;
    desc->field_d6 = flags;
    desc->field_da = 0x20;
    desc->field_1b |= 0x100;

    pic = FUN_0049ff90(layer->entries, "PICLIST");
    pic->field_d6 = flags;
    pic->field_1b |= 0x180;
    pic->field_da = desc->field_da;

    picArray = (int*)FUN_004d83b0("UNITPICARRAY", count * 0x18);
    memset(picArray, 0, count * 0x18);

    textArray = (char*)FUN_004d83b0("UNITTEXTARRAY", count << 5);
    *(int*)textArray = 0;

    DAT_005129b4 = (Record_44c7e0*)FUN_004d83b0("UNITSRESTRICTINFO", count * 0x62);
    desc->field_d2 = DAT_005129b4;
    for (i = 0; i < count; i++)
        DAT_005129b4[i].field_52 = 0;

    DAT_005129b8 = (int*)FUN_004d83b0("UNITSPICS", count << 2);
    memset(DAT_005129b8, 0, count << 2);
    memset(DAT_005129b4, 0, count * 0x62);
    DAT_005129c4 = (int*)FUN_004d83b0("OLDCOUNTS", count << 2);

    n = 0;
    if (count > 1) {
        i = 1;
        off = 0x249;
        do {
            item = (Item_44c7e0*)((char*)g_game->items + off);
            if (!item->field_245.bits.flag && item->name != 0) {
                sprintf(buf, "%s\r%s %dM  %dE", (char*)item,
                        FUN_004c5740((char*)item + 0xa0),
                        (int)item->field_18a, (int)item->field_186);
                DAT_005129b4[n].field_52 = i;
                ((Class_0046e330*)g_game->queue)->FUN_0046e330(&info, item);
                value = info.field_4;
                if (value == -1)
                    value = 0x65;
                DAT_005129b4[n].field_5a = value;
                DAT_005129c4[n] = value;
                DAT_005129b4[n].field_5e = info.field_2;
                n++;
            }
            i++;
            off += 0x249;
        } while (i < g_game->count);
    }

    qsort(DAT_005129b4, n, 0x62, FUN_0044c7a0);

    dst = textArray;
    if (count > 0) {
        i = 0;
        src = (char*)DAT_005129b4;
        do {
            strcpy(dst, src);
            dst += strlen(src) + 1;
            src += 0x62;
            i++;
        } while (i < count);
    }

    for (i = 0; i < 0xc; i++) {
        sprintf(buf, "SLIDER%d", i);
        slider = FUN_004a0200(layer->entries, buf);
        slider->field_14a = (int)slider;
        slider->field_13c = 0x65;
        slider->field_144 = (void*)FUN_0044be70;
    }

    idx = FUN_0049fdf0(g_game->inner->entries, "SCROLLSLIDER", 0xe);
    if (idx != -1) {
        scroll = FUN_004a0200(g_game->inner->entries, "SCROLLSLIDER");
        scroll->field_13c = 0xd2;
        scroll->field_144 = (void*)FUN_0044bfd0;
        scroll->field_140 = 0;
        FUN_0045b9b0(scroll, 0);
        scroll->field_14a = (int)g_game;
    }
    FUN_0044bfd0((char*)g_game + 0x519, idx);
    FUN_0049fa90((char*)g_game + 0x519);

    FUN_004a32a0((char*)g_game + 0x519, "DESCLIST", textArray, n, 0);
    FUN_004a35a0(g_game->inner->entries, "PICLIST", picArray, n);
    FUN_0044bfd0((char*)g_game + 0x519, 0);

    item = &g_game->items[desc->field_d2[desc->field_ba].field_52];
    sprintf(buf, "%d", (int)item->field_186);
    FUN_004a0bf0((char*)g_game + 0x519, "ENERGYTEXT", buf, 0);
    sprintf(buf, "%d", (int)item->field_18a);
    FUN_004a0bf0((char*)g_game + 0x519, "METALTEXT", buf, 0);

    bool enabled = (flag == 0);
    FUN_004a1250((char*)g_game + 0x519, "Load", enabled);
    FUN_004a1250((char*)g_game + 0x519, "Save", enabled);
    FUN_004a1250((char*)g_game + 0x519, "Reset", enabled);
    FUN_004a81e0((char*)g_game + 0x519, 0x40);
    FUN_0049fb10((char*)g_game + 0x519, 1);
    DAT_005129c0 = 0;
}
