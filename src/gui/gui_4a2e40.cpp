// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6. Names are
// provisional. MATCH, 640 bytes. Adding <iostream> and <math.h> restores the shared floating-point
// tail instead of duplicating the epilogue into both arms. Inline scale helpers and control-flow
// rewrites did not change the previous 96.0% result. The quotient and comparison must retain float
// conversions to reproduce the original integer-memory FPU multiply/divide forms. FindKind returns
// zero on a miss, so the rescale then uses entry zero.
#include <iostream>
#include <math.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a2e40 { // 0x15b bytes
    unsigned char type; // +0x00
    unsigned char kind; // +0x01
    char name[0x10];    // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19; // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group; // +0x28
    char unknown_29[0xb6 - 0x29];
    short count; // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba; // +0xba
    short field_bc; // +0xbc
    short field_be; // +0xbe
    char unknown_c0[0xd6 - 0xc0];
    int id; // +0xd6
    char unknown_da[0x136 - 0xda];
    short field_136; // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140; // +0x140
    char unknown_142[0x15b - 0x142];
};
#pragma pack(pop)

struct List_004a2e40 {
    char unknown_0[0x0c];
    unsigned short* glyphs; // +0x0c
};

struct Holder_004a2e40 {
    int current;             // +0x00
    Entry_004a2e40* entries; // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a2e40* list; // +0x14
};

#pragma pack(push, 1)
struct Class_004a2e40 {
    char unknown_00[0x18];
    Holder_004a2e40* holder; // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca; // +0xcca
};
#pragma pack(pop)

extern Holder_004a2e40* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
int FUN_004c1450();

static inline int FindEntry(Entry_004a2e40* entries, char* name) {
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

static inline int FindKind(Entry_004a2e40* entries, unsigned char kind) {
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// FUNCTION: 0x4a2e40
void __stdcall FUN_004a2e40(Class_004a2e40* param_1, char* param_2, int param_3) {
    Entry_004a2e40* entries = param_1->holder->entries;
    int found = FindEntry(entries, param_2);
    if (found == -1)
        return;

    Entry_004a2e40* me = &entries[found];
    me->field_ba = param_3;

    int n = 0;
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                FUN_004c1420(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size;
    if (DAT_0051fba4->list == 0)
        size = FUN_004c1450();
    else
        size = *(unsigned short*)(FUN_004b7f30(DAT_0051fba4->list->glyphs, 0x49) + 2) + 2;
    int step = (me->field_19 - 2) / (size + 1);
    short last = me->field_bc;
    short sel = me->field_ba;
    if (sel > step + last - 1 || sel < last) {
        if (me->field_be != 0)
            me->field_bc = sel;
        if (me->field_bc > me->field_be)
            me->field_bc = me->field_be;
        Entry_004a2e40* peer = &entries[FindEntry(entries, param_2)];
        unsigned char pkind = peer->kind;
        Entry_004a2e40* e3 = &entries[FindKind(entries, pkind)];
        float q = (float)e3->field_136 * me->field_bc / me->field_be;
        if ((float)e3->field_140 != q)
            e3->field_140 = (short)q;
    }
    param_1->field_cca = 1;
}
