// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Handles one entry of the nine-entry sound request list: picks a random
// sample from the entry's sound category slot, plays "sounds/<name>.WAV" when
// the entry is due, re-arms the repeat timer, and (when logging is enabled)
// appends "<unit name>: <sound text>" to the message log.
//
// The structs are packed: pointer fields sit at odd offsets in the original
// (entry +0x8/+0xc, emitter +0x92, game +0x37e13).
//
// The first two accesses go through the array expression instead of the e
// pointer: written that way MSVC 5 keeps the index in eax and forms the entry
// address as `lea esi, [eax+ecx]`, as the original does. Using e for them
// materialises the sum early (`add eax, ecx; mov esi, eax`).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class Class_004d0640 {
public:
    void FUN_004d0640(const char* param1, int param2, int param3);
};

#pragma pack(push, 1)
struct Emitter_0047fd70 {
    char unknown_0[0x92];
    char* name;                            // +0x92 (its +0x20e is the category)
    char unknown_96[0xa8 - 0x96];
    unsigned short field_a8;               // +0xa8
    char unknown_aa[0x110 - 0xaa];
    int field_110;                         // +0x110
};

struct Slot_0047fd70 {
    int count;                             // +0x0
    char* table1;                          // +0x4
    char* table2;                          // +0x8
};

struct SoundCat_0047fd70 {
    char unknown_0[0x40];
    Slot_0047fd70 slots[24];               // +0x40
};

struct Table_0047fd70 {
    int field_0;                           // +0x0
    int unknown_4;
    int unknown_8;
    int field_c;                           // +0xc
    int unknown_10;
    int unknown_14;
};

struct Game_0047fd70 {
    char unknown_0[0x10];
    Class_004d0640* sound;                 // +0x10
    char unknown_14[0x37e13 - 0x14];
    SoundCat_0047fd70* categories;         // +0x37e13
    char unknown_37e17[0x37f0c - 0x37e17];
    int field_37f0c;                       // +0x37f0c
    char unknown_37f10[0x37f17 - 0x37f10];
    unsigned char field_37f17;             // +0x37f17
    unsigned char field_37f18;             // +0x37f18
    unsigned char field_37f19;             // +0x37f19
    char unknown_37f1a[0x38a47 - 0x37f1a];
    unsigned int frame;                    // +0x38a47
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Entry_0047fd70 {                    // 0x11 bytes
    int field_0;                           // +0x0
    int field_4;                           // +0x4
    Emitter_0047fd70* field_8;             // +0x8
    char* data;                            // +0xc
    unsigned char field_10;                // +0x10
};

class Class_0047f960 {
public:
    Entry_0047fd70 entries[9];             // +0x0
    int count;                             // +0x99
    int field_9d;                          // +0x9d
    int field_a1;                          // +0xa1
    int field_a5;                          // +0xa5

    void FUN_0047fd70(int index, int param_2, int param_3);
};
#pragma pack(pop)

extern Game_0047fd70* g_game;
extern Table_0047fd70 DAT_005086e0[24];
extern int DAT_0051e690;
extern int DAT_0051e694;
extern int DAT_0051e698;

char* __stdcall FUN_004290f0(char* buf, char* dir, char* name, char* ext);
void __stdcall FUN_00463ca0(char* text, unsigned char key, unsigned short value, char last);
int __stdcall FUN_0049f6c0(char* path);

// FUNCTION: 0x47fd70
void Class_0047f960::FUN_0047fd70(int index, int param_2, int param_3)
{
    Entry_0047fd70* e = &entries[index];
    int slot = entries[index].field_0;
    unsigned int catIndex = 0;
    catIndex = *(unsigned short*)(entries[index].field_8->name + 0x20e);
    SoundCat_0047fd70* cat = &g_game->categories[catIndex];
    int count = cat->slots[slot].count;
    int idx = (int)((__int64)rand() * count / 0x8000);

    if ((int)e->field_10 > 10 - g_game->field_37f17 && count > 0 && param_2 != 0
        && (g_game->field_37f19 & 0x40)) {
        char* name;
        if (DAT_0051e698)
            name = ((g_game->frame / 30) & 7) ? "sing" : "honk";
        else
            name = cat->slots[slot].table1 + idx * 0x40;

        char path[256];
        FUN_004290f0(path, "sounds", name, "WAV");
        if (DAT_0051e694) {
            FUN_0049f6c0(path);
        } else if (path && strlen(path) && g_game->field_37f0c
                   && (g_game->field_37f19 & 7) && DAT_0051e690 == 0) {
            g_game->sound->FUN_004d0640(path, -0x249, 0);
        }
        DAT_005086e0[slot].field_c =
            g_game->frame + DAT_005086e0[slot].field_0 * 0x1e;
    }

    if (param_3 != 0 && (int)e->field_10 > 10 - g_game->field_37f18) {
        char* text = e->data;
        if (text == 0) {
            if (idx != -1)
                text = cat->slots[slot].table2 + idx * 0x40;
            if (text == 0)
                return;
        }
        if (*text == 0)
            return;
        if (e->field_8->field_110 & 0x10000000) {
            char msg[100];
            sprintf(msg, "%s: %s", e->field_8->name, text);
            FUN_00463ca0(msg, 1, e->field_8->field_a8, '\n');
        }
    }
}
