// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash (#4124, 10-minute box, no new variant scored above 72.7%):
// re-ran the top-allocation search with the ctx.py frame decoded exactly (used[]
// is 200 ints at post-push esp+0x28, frame 0x338 = 24 bytes of scalars at
// esp+0x10..0x27 plus 800 bytes of array; a 196-int array gives 0x328 and drops
// to 70.4%, so 200 is right). Every top-of-function respelling still compiles to
// `mov eax,[eax+0x18]` (ours) instead of `mov esi,[eax+0x18]` (original): no
// `layer` local at all (menu->layer at each use), entries-before-index, index
// through menu->layer with the local assigned later, `Menu* self = menu`, a
// static-inline LayerOf/EntriesOf getter, deferred declarations, index-only
// deferred, layering after cnt, and unsigned cnt all scored 72.7% byte-for-byte
// identically at the top (cnt stays in esi, layer's temp in eax spilled to
// esp+0x24). `int cnt` computed before the early return scored 72.4. The only
// remaining leads are a `layer` live range that reaches a call (ours reloads
// menu->layer after both DoSelects, so no) or an extra callee-saved candidate.
// deepseek-v4.1-flash (#4054, 10-minute box, no new variant scored): reconfirmed
// 72.7% / 1400 bytes. Still differs: the top-of-function allocation (layer=EAX,
// cnt=ESI in ours against layer=ESI spilled at [esp+0x20], cnt born in EAX after
// the rep stosd at [esp+0x24] in the original) plus the dead preheader reload and
// the loop1 walk bracket [ecx+4]; details and the full list of failed respellings
// in the notes below.
// Earlier low-scoring versions by space-bunny-free, GPT-6.1-sol and GPT-6 (47.2%).
// deepseek-v4.1-flash retry #2 (72.4 -> 72.7): rewrote the loop1 inner search as
// an explicit pointer walk `int* q = &used[1]; int v = *q; while (v != 0) { ...
// v = q[1]; q++; j++; }` (was `used[j+1]` indexed by j). This keeps the walk at
// `q = &used[1]` reading q[1] instead of strength-reducing to `&used[2]`, and the
// resulting graph nudged +0.3. It is still 1400 bytes vs 1404 and the top
// allocation is unchanged (layer=EAX, cnt=ESI). Re-confirmed this run that the
// top allocation is not compiler state: a sweep of 0..400 unused `extern int`
// declarations before the function changed nothing, and the same for the 24-way
// sweep of used[]/cnt index/entries declaration orders plus `int* pcnt = &cnt`,
// a struct-wrapped cnt, and `Menu* self = menu`. Only a `volatile int cnt`
// (diagnostic only, not committed) spilt cnt and moved layer off EAX, and then
// to EDX, not ESI.
//
// Current status: PARTIAL 72.7% (best so far, this run; the 70.6/71.0 era is over).
// deepseek-v4.1-flash retry (72.2 -> 72.4): moving `int cnt = ...` ABOVE the
// `used[]` zero loop in the source gained the 0.2. It is a scheduling lever, not
// a semantic one: the compiler reorders the movsx/inc and the `rep stosd` the same
// way regardless of source order, and the swap only nudges the difflib alignment
// of one line. Confirmed everything below with the /Fa listing (build/scratch/
// 0x4a7960/ours.asm): the four-byte size gap and almost every mismatching line
// trace to ONE allocation at the top. `layer` lands in EAX in ours but in ESI in
// the original, so cnt (which the original leaves in memory and reloads at each
// loop2 iteration) gets ESI in ours and is kept register-resident, dropping the
// extra `mov eax,[esp+0x24]` at loop2 head. That one register choice is also why
// the original has a dead preheader reload `mov edx,[esp+0x20]` of the spilled
// layer before loop2, so the preheader reload is a CONSEQUENCE, not a separate
// missing instruction. Tried to flip layer to ESI (all free --sym scored, all
// stayed 72.2/72.4 or fell): every declaration order of layer/index/entries and
// of used[] (including declared after the early return); layer via a separate
// declaration then assignment; a `Menu* self = menu;` copy feeding the initial
// loads; layer->entries spelled for entries/b/cnt/the switch cases/tail; an
// explicit `entries = layer->entries;` at loop2 setup (CSE'd away, no reload);
// memset() forms of the zero loop; `cnt` declared early and assigned late;
// `if (cnt >= 2)` and `1 + count` spellings. Adding a redundant `layer` use that
// survives (w4) collapsed the function to 44.1%. So the allocator picks
// layer=EAX and cnt=ESI robustly under the whole-body live ranges, and no
// single-site source edit moved it. The remaining work is finding the upstream
// change that gives `layer` a callee-saved home; everything else follows.
//
// Current status: PARTIAL 72.2% (best so far; the 70.6/71.0 era is over).
// Sonnet 5.5 pass (no scored change, all free --sym scores stayed 72.2 or fell):
// root cause is one allocation decision at the top. The original gives `layer`
// the temp ESI (mov esi,[eax+0x18]) so the movsx of cnt has to land in EAX and
// therefore after `xor eax,eax / rep stosd`; ours puts layer in EAX and cnt in
// ESI, which lets the scheduler hoist movsx/inc/cmp above the stosd setup.
// Byte-identical output (the compiler normalises them): memset(used,0,200),
// memset(.., 50*sizeof(int)), a pointer-walk zero loop, do/while zero loop,
// `int cnt;` declared early, cnt as `n + 1` / `1 + n` / `+= 1`, top reads via
// menu->layer->.. without a `layer` local, `layer->entries` spelled in loop 2 and
// in the start computations, `layer->field_20 = index` instead of
// menu->layer->field_20 at the tail. Adding real extra `layer` references (a dummy
// store after the zero loop) does promote layer to a register but into EDX with a
// 0x334 frame (66.7 to 68.8), not ESI+spill, so more weight on layer alone is not it.
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

    int cnt = entries->data.count + 1;

    for (int k = 0; k < 50; k++)
        used[k] = 0;

    if (cnt > 1) {
        int* out = used + 1;
        short* p = &entries[1].x0;
        int remaining = cnt - 1;
        do {
            int idx = -1;
            int* q = &used[1];
            int v = *q;
            int j = 1;
            while (v != 0) {
                if (v - *p < 10 && v - *p > -10) {
                    idx = j;
                    break;
                }
                v = q[1];
                q++;
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