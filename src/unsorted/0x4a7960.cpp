// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Earlier low-scoring versions by space-bunny-free, GPT-6.1-sol and GPT-6 (47.2%).
// Current status: PARTIAL 72.2% (best so far; the 70.6/71.0 era is over).
// This retry (deepseek-v4.1-flash) rewrote the used[] search loop as a plain
// while over an index j (v = used[j+1]; j++) instead of a walking q pointer,
// and flipped the store to `if (idx != -1) *out = used[idx]; else *out = *p;`.
// Together that fixed the [ecx+4] read, the loop-tail branch polarity and the
// store-tail branch polarity, worth +1.2 and it keeps every later jump offset
// in step with the original (ours is 1400 bytes against the original 1404).
// Still differs (why this is not a match):
//   * the rep stosd zero loop sits at the very top of the function, before the
//     menu/layer loads, while the original loads menu->layer (esi), index (ebx)
//     and entries (edx), tests index == -1, and only then runs the stosd. Any
//     spelling that puts the scalar loads first makes MSVC hoist the cnt load
//     (movsx eax,[edx+0xb6]) above the stosd, or the stosd gets scheduled up
//     anyway (v3, the whole body wrapped in `if (index != -1) {...}`, still has
//     the stosd first and scores the same 72.2).
//   * because of that, layer and cnt trade places: the original has
//     layer in esi spilled at [esp+0x20] and cnt born in eax after the stosd
//     spilled at [esp+0x24] (eax is the stosd's value register, so the load can
//     only come after it); ours has layer in eax spilled at [esp+0x24] and cnt
//     in esi spilled at [esp+0x20], and cnt then stays register-resident
//     through the scan loop (cmp esi,edi) where the original reloads
//     mov eax,[esp+0x24] at every loop bottom.
//   * loop1 inner search: ours strength-reduces the walk to q = used+2 reading
//     [ecx]; the original keeps q = used+1 reading [ecx+4] even in this
//     index spelling (one extra instruction). Tried: walking pointer q[1]/q++
//     (folds to used+2), while over used[j+1] (still folds), all free variants.
//   * dead `mov edx,[esp+0x20]` in the original's scan-loop head (edx reloads
//     layer but is overwritten before any use) accounts for most of the 4 byte
//     size gap; we have no source that reproduces a dead reload.
//   * loop2 head scheduling: original puts mov eax,[esp+0x24] / cmp / jle before
//     the lea of the up pointer and stores [esp+0x14] after the jle; ours
//     interleaves differently.
// Ideas tried this run (all scored free against the 71.0 base):
//   * search loop as while + used[j+1] + flipped store polarity: 72.2 (kept).
//   * whole body wrapped in if (index != -1) {...} instead of early return: 72.2.
//   * earlier era notes (prologue order variants, memset, case orders, shared
//     bound, DoSelect reloaded entries) all 53 to 70.6, see history in git.
//
#pragma pack(push, 1)

struct Entry_004a7960 {                // 0x15b bytes
    unsigned char type;                // +0x000
    char unknown_01[0x13 - 0x01];
    short x0;                          // +0x013
    short y0;                          // +0x015
    short x1;                          // +0x017
    short y1;                          // +0x019
    int field_1b;                      // +0x01b
    int colourIndex;                   // +0x01f
    char unknown_23[0x28 - 0x23];
    char group;                        // +0x028
    char field_29;                     // +0x029
    char unknown_2a[0xb6 - 0x2a];
    union {
        short count;                   // +0x0b6 (entry 0 only)
        char text[0x82];               // +0x0b6
        struct {
            char pad[0x20];
            int id;                    // +0x0d6
        } list;
    } data;
    short maxLength;                   // +0x138
    char unknown_13a[0x13c - 0x13a];
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x157 - 0x13d];
    int field_157;                     // +0x157
};

struct Layer_004a7960 {
    int unknown_00;
    Entry_004a7960* entries;           // +0x04
    char unknown_08[0x20 - 0x08];
    int field_20;                      // +0x20
};

struct Menu_004a7960 {
    char unknown_00[0x18];
    Layer_004a7960* layer;             // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
    char unknown_68[0x8b2 - 0x68];
    unsigned char colors[16];          // +0x8b2
};
#pragma pack(pop)

struct Class_0051fba4 {
    int group;                         // +0x00
};
extern Class_0051fba4* DAT_0051fba4;

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int colour, int font);
void __stdcall FUN_004c1420(int id);
void FUN_004c1a40();
int __stdcall FUN_0049fc50(Menu_004a7960* menu, int index);
void __stdcall FUN_004ab6c0(Menu_004a7960* menu, int index, char* text,
                            int maxLength, int clear);

// The type==3 body shared with 0x4a7190/0x4a76b0/0x4a7830.
static inline void DoSelect(Menu_004a7960* menu, Entry_004a7960* entries, int sel)
{
    Entry_004a7960* entry = &entries[sel];
    FUN_004c13a0(menu->colors[entry->colourIndex], FUN_004c13f0());

    int n = 0;
    int i;
    for (i = 1; i < entries->data.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entry->group) {
                FUN_004c1420(entries[i].data.list.id);
                break;
            }
            n++;
        }
    }
    if (i == entries->data.count + 1)
        FUN_004c1420(DAT_0051fba4->group);

    FUN_0049fc50(menu, sel);
    menu->layer->field_20 = sel;
    FUN_004ab6c0(menu, sel, entry->data.text, entry->maxLength, 0);
    FUN_004c1a40();
}

// ---- preceding function (compiler state experiment) ----
// Selecting the entry with the index the caller passes: remembers the index,
// and if the entry it names is a type 3 (text) control it makes the group's
// type 7 list entry current and puts the entry's own text back into the field.
// The entry array is loaded twice, once into the `first` copy used by the type
// test and once inside the branch; the type test needs its copy in a register
// before the two stores, which is what makes the reload land in ebx.
#pragma pack(push, 1)

// A window's colour table: indexed from the window pointer itself.
struct Colour_004a7830 {
    char unknown_0[0x8b2];
    unsigned char colour;              // +0x8b2
};

struct Entry_004a7830 {                // 0x15b bytes
    unsigned char type;                // +0x000
    char unknown_01[0x1f - 0x01];
    Colour_004a7830* colours;          // +0x01f
    char unknown_23[0x28 - 0x23];
    char group;                        // +0x028
    char unknown_29[0xb6 - 0x29];
    union {
        char text[0x82];               // +0x0b6 (a text control)
        short count;                   // +0x0b6 (entry 0: number of entries)
        struct {
            char pad[0x20];
            int id;                    // +0x0d6 (a list entry)
        } list;
    } data;
    short maxLength;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Holder_004a7830 {
    int unknown_0;
    Entry_004a7830* entries;           // +0x4
    char unknown_8[0x20 - 0x8];
    int field_20;                      // +0x20
};

struct Menu_004a7830 {
    char unknown_0[0x18];
    Holder_004a7830* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
};

#pragma pack(pop)

struct Class_004a7830_pre {
    int group;                         // +0x0
};

extern Class_004a7830_pre* DAT_004a7830_pre;

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int param_1, int param_2);
void __stdcall FUN_004c1420(int param_1);
void FUN_004c1a40();
int __stdcall FUN_0049fc50(Menu_004a7830* obj, int index);
void __stdcall FUN_004ab6c0(Menu_004a7830* control, int param_2, char* text,
                            int maxLength, int clear);

void __stdcall FUN_004a7830(Menu_004a7830* menu, int index)
{
    Entry_004a7830* first = menu->holder->entries;
    menu->focus = -1;
    menu->holder->field_20 = index;
    if (first[menu->holder->field_20].type == 3) {
        int i = menu->holder->field_20;
        Entry_004a7830* entries = menu->holder->entries;
        Entry_004a7830* entry = &entries[i];
        int font = FUN_004c13f0();
        FUN_004c13a0((int)((unsigned char*)entry->colours)[(int)menu + 0x8b2], font);

        int n = 0;
        int j = 1;
        for (; j < entries->data.count + 1; j++) {
            if (entries[j].type == 7) {
                if (n == entry->group) {
                    FUN_004c1420(entries[j].data.list.id);
                    break;
                }
                n++;
            }
        }
        if (j == entries->data.count + 1) {
            FUN_004c1420(DAT_004a7830_pre->group);
        }

        FUN_0049fc50(menu, i);
        menu->holder->field_20 = i;
        FUN_004ab6c0(menu, i, entry->data.text, entry->maxLength, 0);
        FUN_004c1a40();
    }
}

// FUNCTION: 0x4a7960
void __stdcall FUN_004a7960(Menu_004a7960* menu, int dir)
{
    int used[200];

    Layer_004a7960* layer = menu->layer;
    int index = layer->field_20;
    Entry_004a7960* entries = layer->entries;
    if (index == -1)
        return;

    for (int k = 0; k < 50; k++)
        used[k] = 0;

    int cnt = entries->data.count + 1;
    if (cnt > 1) {
        int* out = used + 1;
        short* p = &entries[1].x0;
        int remaining = cnt - 1;
        do {
            int idx = -1;
            int v = used[1];
            int j = 1;
            while (v != 0) {
                if (v - *p < 10 && v - *p > -10) {
                    idx = j;
                    break;
                }
                v = used[j + 1];
                j++;
            }
            if (idx != -1)
                *out = used[idx];
            else
                *out = *p;
            p = (short*)((char*)p + 0x15b);
            out++;
            remaining--;
        } while (remaining != 0);
    }

    int start;
    int bound;
    switch (dir) {
    case 0:
        start = entries[index].x0 + entries[index].y0 * 5000;
        bound = start - 0x17d7840;
        break;
    case 2:
        start = entries[index].y0 + used[index] * 5000;
        bound = start - 0x17d7840;
        break;
    case 1:
        start = entries[index].x0 + entries[index].y0 * 5000;
        bound = start + 0x17d7840;
        break;
    case 3:
        start = entries[index].y0 + used[index] * 5000;
        bound = start + 0x17d7840;
        break;
    }

    int* up = used + 1;

    int i;
    int pos;
    {
        char* b = (char*)&entries[1].x1;
        for (i = 1; i < cnt; i++) {
            if (*(signed char*)(b + 0x12) != 0 && !(*(int*)(b + 4) & 0x400)
                && !(*(unsigned char*)(b - 0x17) == 1 && (*(unsigned char*)(b + 0x125) & 1))
                && !(*(unsigned char*)(b - 0x17) == 4 && *(int*)(b + 0x140) != 0)) {
                if (*(unsigned char*)(b - 0x17) == 3 || *(unsigned char*)(b - 0x17) == 4
                    || *(unsigned char*)(b - 0x17) == 1
                    || *(unsigned char*)(b - 0x17) == 6
                    || *(unsigned char*)(b - 0x17) == 2) {
                    if (!(*(unsigned char*)(b - 0x17) == 4
                          && *(short*)b < *(short*)(b + 2))) {
                        if (!(*(unsigned char*)(b - 0x17) == 2 && (*(int*)(b + 4) & 0x100))
                            && !(*(unsigned char*)(b - 0x17) == 1
                                 && (*(unsigned char*)(b + 0x125) & 1))) {
                            switch (dir) {
                            case 0:
                            case 1:
                                pos = *(short*)(b - 4) + *(short*)(b - 2) * 5000;
                                break;
                            case 2:
                            case 3:
                                pos = *(short*)(b - 2) + *up * 5000;
                                break;
                            }
                            switch (dir) {
                            case 1:
                            case 3:
                                if (pos <= start)
                                    pos += 0x17d7840;
                                if (pos < bound) {
                                    bound = pos;
                                    index = i;
                                }
                                break;
                            case 0:
                            case 2:
                                if (pos >= start)
                                    pos -= 0x17d7840;
                                if (pos > bound) {
                                    bound = pos;
                                    index = i;
                                }
                                break;
                            }
                        }
                    }
                }
            }
            up++;
            b += 0x15b;
        }
    }

    menu->focus = -1;
    menu->layer->field_20 = index;
    if (entries[menu->layer->field_20].type == 3)
        DoSelect(menu, menu->layer->entries, menu->layer->field_20);
    if (entries[menu->layer->field_20].type == 3)
        DoSelect(menu, menu->layer->entries, menu->layer->field_20);
}
