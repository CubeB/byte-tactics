// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Earlier low-scoring versions by space-bunny-free, GPT-6.1-sol and GPT-6 (47.2%).
// deepseek-v4.1 facts that still hold and are relied on here:
//   * MSVC5 sizes this frame as six live scalar slots plus 200 ints: sub esp,0x338,
//     array at [esp+0x28]. The zero loop clears only the first 50 ints (rep stosd 0x32).
//   * the scan loop walks a pointer based at entry[1].x1 (entry+0x172): type via
//     [b-0x17], field_1b via [b+4], field_13c via [b+0x125], field_157 via [b+0x140],
//     x0/y0 as [b-4]/[b-2] and x1/y1 as [b]/[b+2].
// Current status: PARTIAL 71.0% (deepseek-v4.1 left 70.6; the only gain is
// defining the real preceding function 0x4a7830 above this one, per the guide's
// compiler-state technique, worth +0.4 and 4 bytes off).
// Still differs (why this is not a match):
//   * the biggest single missed optimisation is in the first switch(dir): the
//     original shares one tail for the two -0x17d7840 arms (case 0 falls into the
//     shared start/bound block, case 2 falls through to it) and one for the two
//     +0x17d7840 arms, while ours emits the `lea ebp,[eax+ecx*8] / store start /
//     lea bound / store bound` tail in all four arms. All 24 source orderings of
//     the four cases were scored; 0,2,1,3 and 1,2,0,3 tie at 70.6 (best), every
//     other order is 63 to 67.4, so the case order is already optimal and the
//     merge is not reachable by reordering. A single shared `bound` after the
//     switch (start grouped 0/1 and 2/3) collapses the function to 1288 bytes /
//     53.3, so the original really has four separate arms.
//   * prologue order: the original reads menu->layer, index and entries, tests
//     index == -1, THEN runs the rep stosd zero loop, THEN loads cnt. Running the
//     zero loop first (as here) is what keeps the rest at 70.6%; moving it after
//     the scalar loads (vA, the original order) makes MSVC hoist the cnt load above
//     rep stosd and use edi instead of ebp for the scan walk, dropping to 69.6%.
//   * scalar slot rotation: ours has entries@0x18 (correct), cnt@0x20, layer@0x24;
//     the original has entries@0x18, layer@0x20, cnt@0x24, i.e. layer and cnt are
//     swapped. Declaring cnt early (vE) scores the same 70.6 and does not move them.
//   * loop1 inner search: ours strength-reduces q to used+2 and reads [ecx]; the
//     original keeps q=used+1 and reads [ecx+4] (one extra instruction).
//   * ours is 1436 bytes against the original 1404, mostly from the points above.
// (deepseek-v4.1-flash retry, all scored as free scratch variants against the
// 70.6 base, none beat it: prologue scalar-loads-before-zero-loop 69.6; memset
// instead of the zero loop 69.6/70.6 (identical bytes, so no barrier effect);
// short count then +1 68.0; all 24 first-switch case orders (best 70.6); the
// same `bound` computed once after the switch 53.3; DoSelect taking only
// (menu, sel) and reloading entries 70.6; the colour read via a colours-pointer
// field 70.6; the used-builder loop wrapped in a static inline helper 70.6.
// The last four changed nothing at all, not even the byte count: codegen here
// is pinned by the two loops, not by those spellings. Only adding the real
// preceding function moved the number.)
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
    for (int k = 0; k < 50; k++)
        used[k] = 0;

    Layer_004a7960* layer = menu->layer;
    int index = layer->field_20;
    Entry_004a7960* entries = layer->entries;
    if (index == -1)
        return;

    int cnt = entries->data.count + 1;
    if (cnt > 1) {
        int* out = used + 1;
        short* p = &entries[1].x0;
        int remaining = cnt - 1;
        do {
            int idx = -1;
            if (used[1] != 0) {
                int* q = used + 1;
                int v = used[1];
                int j = 1;
                do {
                    if (v - *p < 10 && v - *p > -10) {
                        idx = j;
                        break;
                    }
                    v = q[1];
                    q++;
                    j++;
                } while (v != 0);
            }
            if (idx == -1)
                *out = *p;
            else
                *out = used[idx];
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
