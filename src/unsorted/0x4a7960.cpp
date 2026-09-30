// Decompiled by deepseek-v4.1. Names are provisional.
// Earlier version by space-bunny-free, GPT-6.1-sol and GPT-6 kept below; 47.2%.
// deepseek-v4.1 lifted it to PARTIAL 61.4% with three source-level facts:
//   * MSVC5 sizes this frame as (four bytes per live scalar slot) + (declared bytes
//     of the local array), so `int used[200]` with SIX live scalars gives exactly
//     `sub esp, 0x338` with the array at [esp+0x28]: the original's prologue, both
//     argument reads ([esp+0x34c]/[esp+0x350]) and every array access now match.
//     Declaring the array (and its zero loop) FIRST in the body is what puts the
//     `rep stosd` before the count load, as in the original; with the array declared
//     after layer/entries the count is hoisted and the frame rotated (51.3%).
//   * the big scan loop walks a pointer based at entry+0x17 (x1): type via
//     [b-0x17], field_1b via [b+4], field_29 via [b+0x12], field_13c via [b+0x125],
//     field_157 via [b+0x140], x0/y0 as [b-4]/[b-2] and x1/y1 as [b]/[b+2].
//     An entry-based pointer scores 51.3%; this scores 61.4%.
//   * the sixth live scalar (which is what pushes the array to [esp+0x28]) comes from
//     hoisting `int* up = used + 1;` above the dir switch. Defining it after the first
//     loop drops back to five slots and 0x334 (55.9%).
// Still differs (why this is not a match):
//   * scalar slots are still rotated: ours has out@0x14, cnt@0x1c, layer@0x20,
//     entries@0x24; the original has out/start@0x10, remaining/up@0x14, entries@0x18,
//     bound@0x1c, layer@0x20, cnt@0x24. The original's peak of six live scalars is in
//     the dir-switch loop, ours is in the first loop, so its slot reuse never happens
//     here. Declaration-order permutations tried so far move index into ebx (correct)
//     but not the slot numbers.
//   * registers: original homes layer in esi (ours eax) and the scan walk in ebp
//     (ours edi); the original's loop-1 walk pointer is ebp-based like ours.
//   * the two type==3 tails: the original indexes the saved `entries` local (slot
//     0x18) and reloads menu->layer->field_20; writing that out here costs a seventh
//     slot and drops to 27.5%.
// Earlier notes: array 196/197 with five slots give frames 0x328/0x334 (47.2%);
// a Walk view based at entries+0x172 with `int used[196]` scored 44.2%.
// Tried by deepseek-v4.1 (all below this file, reverted):
//   * moving the layer/index/entries loads above the zero loop (which is the
//     order the original disassembly shows) makes MSVC5 hoist the count load
//     above the `rep stosd`, puts index in ebp instead of ebx and grows the
//     function to 1440 bytes: 53.6%. The zero-loop-first shape here scores
//     higher even though the prologue order then differs.
//   * using the saved `entries` local in the tail (`entries[menu->layer->field_20]`,
//     which is what the original's `mov edx, [esp+0x18]` shows) keeps entries
//     live across loop 2 and rotates every slot: 34.3%. The fresh
//     menu->layer->entries reload used here scores higher.
// These two say the remaining gap is slot live-range shaping, not statement
// order: the original keeps entries at [esp+0x18] into the tail AND has the
// count at [esp+0x24] with up sharing [esp+0x14] with `remaining` (up is born
// only at loop 2), while every source shape tried here either drops to five
// slots (frame 0x334) or rotates the set.
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
