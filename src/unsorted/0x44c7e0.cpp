// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Builds the "RESTRICT2.GUI" unit-restriction dialog: two lists (DESCLIST /
// PICLIST), the per-unit picture/text/restriction scratch arrays, twelve
// sliders, the scroll slider, the energy/metal labels and the Load/Save/Reset
// buttons. Family with 0x44c220 (per-frame update) and 0x44c420 (teardown).
//
// Still differs (73.0%). Two things moved the number the most:
//   * the item field at +0x245 must be a 1-bit BITFIELD (union Flags with
//     `flag : 1` at bit 15), reusing the shape already solved in 0x44c420.
//   * naming `void* panel = (char*)g_game + 0x519;` for the ENERGYTEXT /
//     METALTEXT block (where the original caches the menu address in edi
//     across the two sprintf/FUN_004a0bf0 pairs) was worth +7 points and
//     re-shuffled far-away allocation.
//
// What is left is all register allocation, no structural diff:
//   * the shared zero constant lands in ebp not eax at the top, which forces
//     layer->entries into ebx instead of the original's ebp (and its spill
//     slot to 0x0c instead of 0x08; textArray gets 0x08 in ours, 0x0c in the
//     original). The item flag test is still `test ch,0x80` where the original
//     has `shr ecx,0xf; test cl,1` even with the bitfield.
//   * the big scan loop: ours keeps i in ebp and the 0x62 record offset in
//     edi, the original keeps the 0x249 stride in edi, n in [esp+0x14] and i
//     in [esp+0x10] (both memory) with the items base reloaded each pass.
//   * the scroll block and the Load/Save/Reset tail reload g_game+0x519 where
//     the original sometimes caches it (adding a menu local there made it
//     WORSE: 65.6 -> 65.2 before the bitfield work, 73.0 -> 71.4 after).
// FUN_0046e330 fills a 0x10-byte struct whose fields live at +0xa (short) and
// +0xc (int); it is called as FUN_0046e330(item, &info) with ecx = g_game->queue.
//
// Remaining diff hunks by original address (all register allocation, no
// structural difference; ours is 1566 bytes vs the original 1586):
//   0x44c827  zero constant lands in ebp (`xor ebp,ebp`) where the original
//             has `xor eax,eax`, so entries goes to ebx vs the original's ebp
//             and the spill slots shift (0x18/0x1c swapped). Hoisting the
//             `entries = layer->entries` load changed nothing.
//   0x44c92b  item-init loop: original keeps 0 in edi, ours reloads ebp.
//   0x44c9e7  big scan loop: original keeps the 0x249 stride in edi, n in
//             [esp+0x14] and i in [esp+0x10] (both in memory) and tests the
//             item flag with `shr ecx,0xf; test cl,1`; ours uses ebp/edi for
//             the counters and `test ch,0x80`.
//   0x44cb95  string-copy loop counter moves between [esp+0x10] and [esp+0x14].
//   0x44cc2b  SCROLLSLIDER block: original caches the menu in eax/esi and keeps
//             idx in edi; ours reloads g_game+0x519 for the two FUN_0044bfd0
//             calls and reuses esi.
//   0x44cd87  Load/Save/Reset tail: ours schedules the address arithmetic
//             differently (one extra add/load pair).
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

union Flags_44c7e0 {                   // the dword at +0x245
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
    short field_148;                   // +0x148
    int field_14a;                     // +0x14a
};

struct Info_44c7e0 {                   // 0x10-byte output of FUN_0046e330
    char unknown_0[0xa];
    short field_2;                     // +0xa
    int field_4;                       // +0xc
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
    PlayerEntry_44c7e0 players[10];    // +0x1b8a, stride 0x14b
    char unknown_2878[0x2a30 - 0x2878];
    void* queue;                       // +0x2a30
    char unknown_2a34[0x2a42 - 0x2a34];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x1438f - 0x2a43];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Item_44c7e0* items;                // +0x1439b
};

struct Class_0046e330 {
    void FUN_0046e330(Item_44c7e0* item, Info_44c7e0* out);
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
    unsigned int flag = g_game->players[g_game->localPlayer].unit->field_97 & 1;

    Layer_44c7e0* layer = (Layer_44c7e0*)FUN_004aa8f0((char*)g_game + 0x519, "RESTRICT2.GUI", 0x880);
    layer->handler_8 = (void*)FUN_0044c420;
    layer->field_c = 0;
    layer->handler_1c = (void*)FUN_0044c220;
    DAT_00512768 = 0;
    FUN_004288d0("UnitRestrict5x", 0, 0, 0);

    void* entries = layer->entries;
    void* flags = FUN_004d83b0("FLAGS", g_game->count);
    Gadget_44c7e0* desc = FUN_0049ff90(entries, "DESCLIST");
    desc->field_ce = (void*)FUN_0044c370;
    desc->field_d6 = flags;
    desc->field_da = 0x20;
    desc->field_1b |= 0x100;

    Gadget_44c7e0* pic = FUN_0049ff90(layer->entries, "PICLIST");
    pic->field_d6 = flags;
    pic->field_1b |= 0x180;
    pic->field_da = desc->field_da;

    int* picArray = (int*)FUN_004d83b0("UNITPICARRAY", g_game->count * 0x18);
    memset(picArray, 0, g_game->count * 0x18);

    char* textArray = (char*)FUN_004d83b0("UNITTEXTARRAY", g_game->count << 5);
    *(int*)textArray = 0;

    DAT_005129b4 = (Record_44c7e0*)FUN_004d83b0("UNITSRESTRICTINFO", g_game->count * 0x62);
    desc->field_d2 = DAT_005129b4;
    int i;
    for (i = 0; i < g_game->count; i++)
        DAT_005129b4[i].field_52 = 0;

    DAT_005129b8 = (int*)FUN_004d83b0("UNITSPICS", g_game->count << 2);
    memset(DAT_005129b8, 0, g_game->count << 2);
    memset(DAT_005129b4, 0, g_game->count * 0x62);
    DAT_005129c4 = (int*)FUN_004d83b0("OLDCOUNTS", g_game->count << 2);

    int n = 0;
    i = 1;
    if (g_game->count > 1) {
        int off = 0x249;
        do {
            Item_44c7e0* item = (Item_44c7e0*)((char*)g_game->items + off);
            if (!item->field_245.bits.flag && item->name != 0) {
                Info_44c7e0 info;
                sprintf((char*)&DAT_005129b4[n], "%s\r%s %dM  %dE",
                        (char*)item, FUN_004c5740((char*)item + 0xa0),
                        (int)item->field_18a, (int)item->field_186);
                DAT_005129b4[n].field_52 = i;
                ((Class_0046e330*)g_game->queue)->FUN_0046e330(item, &info);
                int value = info.field_4;
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

    char* dst = textArray;
    for (i = 0; i < g_game->count; i++) {
        strcpy(dst, DAT_005129b4[i].name);
        dst += strlen(DAT_005129b4[i].name) + 1;
    }

    for (i = 0; i < 0xc; i++) {
        char buf[0x14];
        sprintf(buf, "SLIDER%d", i);
        Gadget_44c7e0* slider = FUN_004a0200(entries, buf);
        slider->field_14a = (int)slider;
        slider->field_13c = 0x65;
        slider->field_144 = (void*)FUN_0044be70;
    }

    void* innerEntries = g_game->inner->entries;
    int idx = FUN_0049fdf0(innerEntries, "SCROLLSLIDER", 0xe);
    if (idx != -1) {
        Gadget_44c7e0* scroll = FUN_004a0200(innerEntries, "SCROLLSLIDER");
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

    void* panel = (char*)g_game + 0x519;
    Item_44c7e0* item = &g_game->items[desc->field_d2[desc->field_ba].field_52];
    {
        char buf[0x14];
        sprintf(buf, "%d", (int)item->field_186);
        FUN_004a0bf0(panel, "ENERGYTEXT", buf, 0);
        sprintf(buf, "%d", (int)item->field_18a);
        FUN_004a0bf0(panel, "METALTEXT", buf, 0);
    }

    int enabled = (flag == 0);
    FUN_004a1250((char*)g_game + 0x519, "Load", enabled);
    FUN_004a1250((char*)g_game + 0x519, "Save", enabled);
    FUN_004a1250((char*)g_game + 0x519, "Reset", enabled);
    FUN_004a81e0((char*)g_game + 0x519, 0x40);
    FUN_0049fb10((char*)g_game + 0x519, 1);
    DAT_005129c0 = 0;
}
