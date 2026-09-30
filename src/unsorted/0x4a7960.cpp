// Decompiled by space-bunny-free, verified by GPT-6.1-sol. Names are provisional.
//
// STATUS: not MATCH. Best 49.5%, our object is 1440 bytes against 1404.
// GPT-6.1-sol retry: five checker runs observed; retaining 49.5%. Keeping the layer
// alias throughout the tail scored 36.1%, and merging the initial direction switch
// before computing bound scored 46.9%. The existing note below records earlier variants.
// Everything below the top matches structurally; the diff is one allocator
// state and the stack frame it produces.
//
// The frame: the original is sub esp,0x338 with the 200-int `used` array at
// [esp+0x28] and six scalar slots at 0x10 out/start, 0x14 remaining/up,
// 0x18 entries, 0x1c bound, 0x20 layer, 0x24 cnt. This source gets sub
// esp,0x334 with `used` at [esp+0x24] and only five slots: MSVC puts
// `bound` in a register and lets it share `entries`' [esp+0x18] slot, so
// every esp offset is 4 low. Declaring `int used[201]` instead of [200]
// forces the frame back to 0x338 (arguments and the layer slot line up) and
// is what took the score from 47.2% to 49.5%; the array zero loop is still
// 50 dwords (`mov ecx,0x32`) in both, so only the declared size differs.
//
// The root allocator difference is at the top: the original loads
// menu->layer into ESI, field_20 (the index) into EBX and layer->entries
// into EDX; this source reuses EAX for layer and ends up with index in EBP,
// entries in ESI. Because the index is not in EBX, the fill loop takes EBX
// for its cluster index instead of EDI, which is the single shared cause of
// the remaining 8 `mov [esp+...]` swaps; the second loop then keeps `bound`
// in EBP (`start` is spilled instead), so `bound` never gets its own slot.
// Tried and did not move it: reordering the layer/index/entries declarations,
// using menu->layer directly, keeping the `layer` local live into the tail,
// declaring an extra `count` local, swapping start/bound declaration order,
// initialising them to 0, all five C++ headers and all 128 C-header sets
// (headers.py: every set 43.1% at the previous best). Reusing the original
// index variable as `best` (no separate local) was the one structural fix
// that worked, 43.6% to 47.2%.
//
// The first switch's four case bodies each carry their own
// `mov [esp],start; bound=start+/-K` tail instead of tail-merging to two
// shared tails as the original does, which is most of the 36 extra bytes.
// The second switch and both inlined DoSelect tails are instruction-for-
// instruction the original modulo the esp offsets.
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
    Layer_004a7960* layer = menu->layer;
    int index = layer->field_20;
    Entry_004a7960* entries = layer->entries;
    if (index == -1)
        return;

    int used[201];
    for (int k = 0; k < 50; k++)
        used[k] = 0;

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
    case 1:
        start = entries[index].x0 + entries[index].y0 * 5000;
        bound = start + 0x17d7840;
        break;
    case 2:
        start = entries[index].y0 + used[index] * 5000;
        bound = start - 0x17d7840;
        break;
    case 3:
        start = entries[index].y0 + used[index] * 5000;
        bound = start + 0x17d7840;
        break;
    }

    int i;
    int pos;
    {
        int* up = used + 1;
        Entry_004a7960* e = entries + 1;
        for (i = 1; i < cnt; i++) {
            if (e->field_29 != 0 && !(e->field_1b & 0x400)
                && !(e->type == 1 && (e->field_13c & 1))
                && !(e->type == 4 && e->field_157 != 0)) {
                if (e->type == 3 || e->type == 4 || e->type == 1
                    || e->type == 6 || e->type == 2) {
                    if (!(e->type == 4 && e->x1 < e->y1)) {
                        if (!(e->type == 2 && (e->field_1b & 0x100))
                            && !(e->type == 1 && (e->field_13c & 1))) {
                            switch (dir) {
                            case 0:
                            case 1:
                                pos = e->x0 + e->y0 * 5000;
                                break;
                            case 2:
                            case 3:
                                pos = e->y0 + *up * 5000;
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
            e = (Entry_004a7960*)((char*)e + 0x15b);
        }
    }

    menu->focus = -1;
    menu->layer->field_20 = index;
    if (menu->layer->entries[menu->layer->field_20].type == 3)
        DoSelect(menu, menu->layer->entries, menu->layer->field_20);
    if (menu->layer->entries[menu->layer->field_20].type == 3)
        DoSelect(menu, menu->layer->entries, menu->layer->field_20);
}
