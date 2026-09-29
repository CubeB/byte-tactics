// Decompiled by Sonnet 5.5. Names are provisional.
// Sets the scroll position of the GUI entry called `name` (a slider's
// thumb): stores `value` in it, selects the group's text style as 0x4a30c0
// does, keeps the first visible line inside the range the entry's height
// allows and then moves the linked bar (the type 4 entry with the same id
// byte) to the matching proportional position. Flags the GUI as changed.
//
// NOT MATCHED: 92.0%, 659 of 640 bytes. Two differences left:
// 1. The ratio `a * lo / hi` is float here because that is what turns the
//    products into `fild; fimul; fidiv` (double gives `fild; fild; fmulp`),
//    but the conversion temp lands in the `value` argument slot ([esp+0x1c])
//    where the original uses the `name` slot ([esp+0x18]).
// 2. The original ends both the update and the no-update path in one shared
//    `changed = 1` tail (`jmp`); here the update path gets its own copy of it.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a2e40 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char id;                  // +0x01
    char name[0x17];                   // +0x02
    short height;                      // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    char unknown_c0[0xd6 - 0xc0];
    int id2;                           // +0xd6
    char unknown_da[0x136 - 0xda];
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140;                   // +0x140
    char unknown_142[0x15b - 0x142];
};
#pragma pack(pop)

struct Holder_004a2e40 {
    char unknown_0[4];
    Entry_004a2e40* entries;           // +0x04
};

#pragma pack(push, 1)
struct Class_004a2e40 {
    char unknown_0[0x18];
    Holder_004a2e40* holder;           // +0x18
    char unknown_1c[0xcca - 0x1c];
    int changed;                       // +0xcca
};
#pragma pack(pop)

struct Font_004a2e40 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Class_0051fba4 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a2e40* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1450();
void* __stdcall FUN_004b7f30(void* a, int b);

static inline int FindName_004a2e40(Entry_004a2e40* entries, char* name)
{
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

static inline int FindBar_004a2e40(Entry_004a2e40* entries, unsigned char id)
{
    int k;
    for (k = 1; k < entries->count + 1; k++) {
        if (entries[k].type == 4 && entries[k].id == id)
            return k;
    }
    return 0;
}

// FUNCTION: 0x4a2e40
void __stdcall FUN_004a2e40(Class_004a2e40* obj, char* name, short value)
{
    Entry_004a2e40* entries = obj->holder->entries;
    int idx = FindName_004a2e40(entries, name);
    if (idx == -1)
        return;
    Entry_004a2e40* e = &entries[idx];
    e->field_ba = value;
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                FUN_004c1420(entries[i].id2);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1)
        FUN_004c1420(DAT_0051fba4->group);
    int step;
    if (DAT_0051fba4->font == 0) {
        step = FUN_004c1450();
    } else {
        step = *(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->font->glyphs, 0x49) + 2) + 2;
    }
    short bc = e->field_bc;
    short ba = e->field_ba;
    if (ba > (e->height - 2) / (step + 1) + bc - 1 || ba < bc) {
        short be = e->field_be;
        if (be != 0)
            e->field_bc = ba;
        if (e->field_bc > be)
            e->field_bc = be;
        int j = FindName_004a2e40(entries, name);
        unsigned char id = entries[j].id;
        int k = FindBar_004a2e40(entries, id);
        Entry_004a2e40* bar = &entries[k];
        float pos = (float)bar->field_136 * e->field_bc / e->field_be;
        if ((float)bar->field_140 != pos)
            bar->field_140 = (short)pos;
    }
    obj->changed = 1;
}
