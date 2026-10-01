// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// Status (Space Bunny Free, #4393): 74.5 -> 85.2%, 5288 bytes vs 5248. What moved,
// in the order it moved:
// (1) Headers: `#include <windows.h>` on top of `<stdio.h>` (the string functions
//     come in through it). On its own that was 74.5 -> 78.6% and it fixed every
//     `[ebp+ebx+off]` vs `[ebx+ebp+off]` SIB base/index swap at once;
//     tools/headers.py reported 78.6% for five sets, all with <windows.h>.
// (2) The FUN_004c1ab0 wait is `do { } while (FUN_004c1ab0());`, not
//     `while (FUN_004c1ab0() != 0);`: the original is `xor esi, esi; call; cmp eax,esi;
//     jne top`, and the while-form rotated into two calls and two tests and moved
//     the `xor esi, esi`. That deleted a whole hunk.
// (3) The post-loop name scan walks the entries and derives the index from the
//     pointer (`j = (int)(other - entries)` in the match arm). That is what puts j
//     in edi and the walk in ebx as in the original (80.6 -> 82.5%), even though the
//     division by 0x15b costs seven instructions the original does not have.
// (4) The second loop and the post-loop scan are inside the
//     `if ((flags & 4) || force || (flags & 0x40))` block: the original's
//     `test bl,0x40; je 0x4a950a` jumps past both, so with them inside every branch
//     target from there on has the original's offset. Same score, but it is the
//     shape a match needs.
// (5) tools/permute.py, 17 min, 82.5 -> 85.2%. What it found that is worth keeping
//     (the temp names and the do-nothing casts are gone, the rest is here):
//     savedType, walk, orientation and pf are function-scope, not block-scope; the
//     first loop is `i = 0; while (i < count + 1) { ... i++; }` rather than a `for`;
//     the case 2 list-scroll sync writes the max twice (an initialiser and then the
//     recomputed expression for entries[i], and the local for the partner entry),
//     and that duplication alone is worth 3.0 points, so it looks like the original
//     really does recompute it; `base[i].h = (short)(hh - ((int)base[i].h) % (fh+2))`
//     re-reads the field instead of using hh (worth 3.8 points); `do { ... } while (0);`
//     around the checkbox/stage-button GAF lookup is load-bearing (2.7 points), as is
//     the comma-expression loop `if (j < g->count) do { ... } while (((j += 4),
//     (j < g->count)));` for the frame search; `1 & entries[i].resourceFlags`,
//     `-1 == entries[0].x`, `0 > entries[0].y`, `0 == g`, `(flags & 4) != 0` and
//     `0x40 & flags` are the operand orders the original had.
// What still differs (register, scheduling and slot noise; no missing code):
// (a) Stack slots are permuted. `force` is at [esp+0x1c] here and [esp+0x18] in the
//     original, and `i`, `&entries[i].flags`, `&entries[i].x/y` and the loop 2 `t`
//     counter follow it round. Local declaration order is not a lever: permuting
//     these ten locals, reordering the buffers and changing their sizes all compile
//     byte-identically (build/scratch/0x4a81e0/v1 and v13).
// (b) The case-4 slider geometry is 8 to 10 bytes short. The original stores
//     `secondEnd->x` from dx after a dead `add edx, ecx` (the address of
//     `entries[i].x` advanced by the width), and in the vertical arm does
//     `add esi, edx` with edx still the address of `entries[i].h`. Both look like an
//     MSVC 5 codegen artifact for `e->x - fw + e->w`; x+w-fw, a (short) cast, a
//     local pointer, two statements and swapping the arms all fail to reproduce it.
// (c) Case 1: the original merges the "CHECKBOX" call into the shared
//     `FUN_004b8d40(menu->gaf, stagebuf)` tail (`push "CHECKBOX"; jmp <past the
//     stagebuf push>`) and leaves the "BUTTONS0" call inline; ours does the opposite
//     and duplicates `menu->gaf`. Writing it with two call sites
//     (build/scratch/0x4a81e0/v8/k1_two_calls.cpp) gets within three instructions.
// (d) `entries[i].flags & 0x80` reads through the spilled `&entries[i].flags`
//     pointer here, directly from `[ebp+ebx+0x1b]` in the original; the 0x40 and
//     0x4000 accesses use the pointer on both sides.
// (e) Loop 2 type 13 spills `menu->layer->entries` across FUN_004b6340() here, where
//     the original keeps it in edi; that also costs the `mov edi,[esp+0x1c]` reload
//     at the end of that arm.
// (f) `textbuf[0x10] = 0` is scheduled before `add esp, 0xc` instead of after it, in
//     the case 0/11 and case 1 blocks.
// (g) `FUN_004c6b70(0, saveUnder, x, y)` loads y then x into edx/ecx here and
//     ecx/edx there: operand order only.
// Dead ends, so nobody repeats them: declaration and buffer order (a);
// `unsigned int force`; hoisting `entries[0].u.count + 1` or the compare string in
// the scan; while/do-while/`++j` spellings of the scan loop other than the one above;
// removing the named `base` locals in loop 2; `(unsigned char)` casts on the flags
// test; swapping the two arms of the slider if/else; putting `textbuf[0x10] = 0`
// after the strcat. build/scratch/0x4a81e0 holds sweep.ps1 (scores a folder of
// variants), gen.ps1 and the spec files; build/scratch/0x4a81e0/v10/k1_s5.cpp is
// 5248 bytes exactly at 81.7%, so the byte count is reachable but that shape is not.
#include <math.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>

#pragma pack(push, 1)

struct GafEntry_004a81e0 {              // 4 bytes: frame table header
    unsigned short count;               // +0x00
    unsigned short unknown_2;           // +0x02
};

struct Glyph_004a81e0 {                 // 8 bytes, returned by FUN_004b7f30
    unsigned short w;                    // +0x00
    unsigned short h;                    // +0x02
    short xoff;                         // +0x04
    short yoff;                         // +0x06
};

struct Entry_004a81e0 {                 // 0x15b bytes, one GUI list entry
    unsigned char type;                 // +0x00
    unsigned char group;                // +0x01
    char name[0x11];                    // +0x02 (strncpy 0x10)
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    unsigned char* colours;             // +0x1f
    char unknown_23[0x28 - 0x23];
    char field_28;                      // +0x28 (signed; list box group select)
    unsigned char field_29;             // +0x29 (second loop keep-test)
    char unknown_2a;
    void* archive;                      // +0x2b
    GafEntry_004a81e0* gaf;             // +0x2f
    char unknown_33[0xb4 - 0x33];
    unsigned char resourceFlags;
    char unknown_b5;
    union {
        short count;                    // +0xb6 (entry 0 only)
        struct { short unused_b6; Glyph_004a81e0* firstFrame; } animation;
        struct {
            int field_0;                    // +0xb6 (type 2: sort key)
            short field_ba;                 // +0xba
            short field_bc;                 // +0xbc
            char unknown_a[0x14 - 0x8];
            GafEntry_004a81e0* gaf;
            char unknown_18[0x20 - 0x18];
            void* filebuf;                  // +0xd6, buffer type 7/8 loads
            short scroll;
        } list;
        struct {
            int f_b6;
            int f_ba;
            int f_be;
            int f_c2;
            short f_c6;
        } t6;
        struct {
            char unknown_0[0xc2 - 0xb6];
            int f_c2;
            int f_c6;
        } t13;
        char text[0x80];                // +0xb6
        struct {
            char unknown_0[2];
            void* saveUnder;                // +0xb8, the SAVE UNDER bitmap
            void* surface;                  // +0xbc, the GUI SURFACE bitmap
            void* archive;                  // +0xc0
            GafEntry_004a81e0* background;  // +0xc4
        } assets;
    } u;
    union {
        short field_136;
        struct { unsigned char stage; unsigned char stageIndex; };
    };
    short field_138;                    // +0x138
    union {
        struct {
            unsigned char field_13a;
            unsigned char field_13b;
            unsigned char field_13c;
            char unknown_13d;
        };
        GafEntry_004a81e0* inputGaf;
    };
    union {
        struct {
            char unknown_13e[0x142 - 0x13e];
            short sliderThumb;
            char unknown_144[0x14e - 0x144];
            GafEntry_004a81e0* sliderGaf;
            unsigned char sliderStyle;
        };
        struct {
            char unknown_13e_b[2];
            short f140;
            short pad142;
            int f144;
            short pad148;
            int f14a;
        };
    };
    char unknown_153[0x15b - 0x153];
};

struct Layer_004a81e0 {
    char unknown_00[4];
    Entry_004a81e0* entries;            // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                       // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                       // +0x20 (entry index, -1 for none)
    void* field_24;                     // +0x24
};

struct Menu_004a81e0 {                  // the object callers pass as arg1
    char unknown_00[4];
    void* gaf;                          // +0x04
    char unknown_08[0x18 - 0x08];
    Layer_004a81e0* layer;              // +0x18
    char unknown_1c[0x70 - 0x1c];
    int field_70;                       // +0x70
    char unknown_74[0xa2 - 0x74];
    int field_a2;                       // +0xa2 (nonzero = selection active)
    char unknown_a6[0x9b6 - 0xa6];
    char str_9b6[0x100];                // +0x9b6
    char str_ab6[0x100];                // +0xab6
    char str_bb6[0x100];                // +0xbb6
};

struct Language_004a81e0 {
    char unknown_00[0xc];
    void* glyphs;                       // +0x0c
};

struct FontRoot_004a81e0 {
    int current;                        // +0x00
    char unknown_04[0x14 - 0x04];
    Language_004a81e0* language;        // +0x14
};

#pragma pack(pop)

extern FontRoot_004a81e0* DAT_0051fba4;

int FUN_004b6700(void);
int FUN_004b6710(void);
int FUN_004c1ab0(void);
unsigned int FUN_004b6340(void);
int FUN_004c1450(void);

void __stdcall FUN_004a05e0(void* obj, int index);
void __stdcall FUN_004a16f0(void* obj, int index, int param3);
void __stdcall FUN_004a1b40(void* obj, int index);
void __stdcall FUN_004a3ef0(void* obj, int index);
void __stdcall FUN_004a4660(void* obj, int index);
void __stdcall FUN_004a4980(void* obj, int index);
void __stdcall FUN_004a4c90(void* obj, int index, unsigned int param3);
void __stdcall FUN_004a4d70(void* obj, int index);
void __stdcall FUN_004a56b0(void* obj, int index);
void __stdcall FUN_004a5e50(void* obj, int index);
void __stdcall FUN_004a5f40(void* obj, int index);
void __stdcall FUN_004b0230(void* obj, int index, void* bmp);
void* __stdcall FUN_004b7f30(void* gaf, int index);
void* __stdcall FUN_004b8c60(char* name);
void* __stdcall FUN_004b8d40(void* gaf, const char* name);
char* __stdcall FUN_004baff0(char* a, char* b, char* c);
long __stdcall FUN_004bbc40(char* name);
char* __stdcall FUN_004bbe50(char* name, int* size);
void __stdcall FUN_004c1420(int id);
char* __stdcall FUN_004c5740(char* key);
void* __stdcall FUN_004c69f0(char* name, int width, int height);
void __stdcall FUN_004c6ac0(void* obj);
void __stdcall FUN_004c6b70(void* dst, void* bmp, int x, int y);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4a81e0
int __stdcall FUN_004a81e0(Menu_004a81e0* menu, unsigned int flags)
{
    int savedType;
    Entry_004a81e0* walk;
    int orientation;
    Entry_004a81e0* entries;
    char* name;
    int* pf;
    int i, force;
    GafEntry_004a81e0* g;
    char stagebuf[0x20];
    char textbuf[0x100];
    char buf1[0x100];
    char buf2[0x100];
    char buf3[0x80];

    if (!menu->layer)
        return 0;
    entries = menu->layer->entries;
    if ((flags & 0x100) && (flags & 1)) {
        entries[0].y = -1;
        entries[0].x = -1;
    }
    if ((flags & 0x1000) && (flags & 1)) {
        entries[0].y = -2;
        entries[0].x = -2;
    }
    if (-1 == entries[0].x) {
        entries[0].x = (short)((FUN_004b6700() - entries[0].w) / 2);
        entries[0].y = (short)((FUN_004b6710() - entries[0].h) / 2);
    }
    if (entries[0].x == -2) {
        entries[0].x = (short)(((FUN_004b6700() - 0x80 - entries[0].w) / 2) + 0x80);
        entries[0].y = (short)((FUN_004b6710() - entries[0].h) / 2);
    }
    if (entries[0].x + entries[0].w > FUN_004b6700())
        entries[0].x = (short)((FUN_004b6700() - entries[0].w) / 2);
    if (entries[0].y + entries[0].h > FUN_004b6710())
        entries[0].y = (short)((FUN_004b6710() - entries[0].h) / 2);

    force = flags & 1;
    if (force) {

    do {
    } while (FUN_004c1ab0());
    if (menu->field_70 != 0) {
        walk = entries;
        for (i = 0; i <= entries[0].u.count; i++, walk++) {
            if (walk->type == 1)
                walk->field_13a = 0;
        }
    }
    if (entries[0].w > FUN_004b6700() || entries[0].h > FUN_004b6710())
        return 0;

    entries[0].u.assets.archive = 0;
    i = 0;
    while (i < entries[0].u.count + 1) {
        buf2[0] = 0;
        if (menu->str_9b6[0])
            strcpy(buf2, menu->str_9b6);
        g = 0;
        if (entries[i].resourceFlags & 1) {
            entries[i].archive = 0;
            entries[i].gaf = 0;
            buf1[0] = 0;
            if (menu->str_ab6[0])
                strncpy(buf1, menu->str_ab6, 0x100);
            strcat(buf1, entries[i].name);
            strcat(buf1, "_gadget");
            FUN_004baff0(buf1, buf1, "GAF");
            if (FUN_004bbc40(buf1)) {
                entries[i].archive = FUN_004b8c60(buf1);
                if (entries[i].archive)
                    entries[i].gaf = (GafEntry_004a81e0*)FUN_004b8d40(entries[i].archive, entries[i].name);
            }
        }
        switch (entries[i].type) {
        case 0:
        case 11: {
            if (0 > entries[0].y)
                entries[0].y += (short)FUN_004b6710();
            buf1[0] = 0;
            if (menu->str_ab6[0])
                strncpy(buf1, menu->str_ab6, 0x100);
            strncpy(textbuf, entries[0].name, 0x10);
            textbuf[0x10] = 0;
            strcat(buf1, textbuf);
            FUN_004baff0(buf1, buf1, "GAF");
            if (!entries[0].u.assets.archive) {
                if (FUN_004bbc40(buf1))
                    entries[0].u.assets.archive = FUN_004b8c60(buf1);
            }
            strncpy(textbuf, entries[0].u.text + 0x46, 0x10);
            textbuf[0x10] = 0;
            if (entries[0].u.assets.archive)
                g = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, textbuf);
            if (g == 0) {
                if (0 != menu->gaf) {
                    g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, textbuf);
                    if (g == 0) {
                        g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "BackTile");
                        if (g != 0) {
                            for (int frameIndex = 0; frameIndex < g->count; frameIndex++) {
                                Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, frameIndex);
                                frame->yoff = 0;
                                frame->xoff = 0;
                            }
                        }
                    }
                }
            }
            entries[0].u.assets.background = g;
            break;
        }

        case 4: {
            entries[i].field_13b = 0;
            if (entries[0].u.assets.archive)
                g = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, "SLIDERS");
            if (g == 0 && menu->gaf != 0) {
                g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "SLIDERS");
                if (g != 0) {
                    for (int f = 0; f < g->count; f++) {
                        Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, f);
                        frame->yoff = 0;
                        frame->xoff = 0;
                    }
                    orientation = entries[i].w > entries[i].h ? 10 : 0;
                    Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, orientation);
                    if (entries[i].w < entries[i].h)
                        entries[i].w = frame->w;
                    else
                        entries[i].h = frame->h;
                    entries[i].sliderStyle = (unsigned char)orientation;
                }
            }
            entries[i].sliderGaf = g;
            if (g != 0) {
                Entry_004a81e0* active = menu->layer->entries;
                ++active[0].u.count;
                Entry_004a81e0* created = &active[active[0].u.count];
                memset(created, 0, sizeof(*created));
                created->type = 1;
                created->field_29 = 1;
                Entry_004a81e0* firstEnd = &entries[active[0].u.count];
                firstEnd->x = entries[i].x;
                firstEnd->y = entries[i].y;
                firstEnd->gaf = g;
                firstEnd->field_13b = entries[i].sliderStyle + 6;
                firstEnd->group = entries[i].group;
                Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, entries[i].sliderStyle + 6);
                firstEnd->w = frame->w;
                firstEnd->h = frame->h;
                firstEnd->flags = 0x3400;
                firstEnd->field_29 = entries[i].field_29;
                active = menu->layer->entries;
                active[0].u.count++;
                created = &active[active[0].u.count];
                memset(created, 0, sizeof(*created));
                created->type = 1;
                created->field_29 = 1;
                Entry_004a81e0* secondEnd = &entries[active[0].u.count];
                secondEnd->field_29 = entries[i].field_29;
                frame = (Glyph_004a81e0*)FUN_004b7f30(g, entries[i].sliderStyle + 8);
                secondEnd->y = entries[i].y;
                secondEnd->gaf = g;
                secondEnd->field_13b = entries[i].sliderStyle + 8;
                secondEnd->group = entries[i].group;
                secondEnd->flags = 0x2c00;
                secondEnd->w = frame->w;
                secondEnd->h = frame->h;
                frame = (Glyph_004a81e0*)FUN_004b7f30(g, entries[i].sliderStyle + 6);
                if (entries[i].w > entries[i].h) {
                    secondEnd->x = entries[i].x - frame->w + entries[i].w;
                    entries[i].w += (short)(frame->w * -2);
                    entries[i].x += frame->w;
                    frame = (Glyph_004a81e0*)FUN_004b7f30(g, entries[i].sliderStyle + 5);
                    entries[i].sliderThumb = frame->w;
                    entries[i].field_136 = entries[i].w - entries[i].sliderThumb - 4;
                } else {
                    secondEnd->y = entries[i].y - frame->h + entries[i].h;
                    secondEnd->x = entries[i].x;
                    entries[i].h += (short)(-2 * frame->h);
                    entries[i].y += frame->h;
                }
            } else {
                entries[i].field_136 = (entries[i].w > entries[i].h ? entries[i].w : entries[i].h) - 6;
            }
            break;
        }

        case 3: {
            GafEntry_004a81e0* input = menu->gaf ? (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "TEXTINPUT") : 0;
            if (input != 0) {
                for (int f = 0; f < input->count; f++) {
                    Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(input, f);
                    frame->yoff = 0;
                    frame->xoff = 0;
                }
            }
            entries[i].inputGaf = input;
            if (entries[i].field_138 >= 0x80)
                entries[i].field_138 = 0x7f;
            memset(entries[i].u.text, 0, 0x80);
            break;
        }
        case 2: {
            GafEntry_004a81e0* list = menu->gaf ? (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "LISTBOX") : 0;
            if (list != 0) {
                for (int f = 0; f < list->count; f++) {
                    Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(list, f);
                    frame->yoff = 0;
                    frame->xoff = 0;
                }
            }
            entries[i].u.list.gaf = list;
            int j = 1;
            for (; j <= entries[0].u.count; ) {
                Entry_004a81e0* other = &entries[j];
                if (j != i && other->type == 2) {
                    if (other->group == entries[i].group) {
                        short scroll = other->u.list.scroll > entries[i].u.list.scroll
                                         ? other->u.list.scroll : entries[i].u.list.scroll;
                        entries[i].u.list.scroll = (short)(other->u.list.scroll > entries[i].u.list.scroll
                                                             ? other->u.list.scroll : entries[i].u.list.scroll);
                        other->u.list.scroll = scroll;
                    }
                }
                j = j + 1;
            }
            break;
        }
        case 12: {
            entries[i].colours = 0;
            strncpy(textbuf, entries[i].name, 0x10);
            textbuf[0x10] = 0;
            entries[i].u.animation.firstFrame = 0;
            if (entries[0].u.assets.archive != 0)
                g = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, textbuf);
            if (0 == g)
                g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, textbuf);
            if (g != 0)
                entries[i].u.animation.firstFrame = (Glyph_004a81e0*)FUN_004b7f30(g, 0);
            break;
        }

        case 1: {
            pf = &entries[i].flags;
            entries[i].colours = 0;
            if ((entries[i].flags & 0x1800) || (1 & entries[i].resourceFlags))
                break;
            FUN_004a05e0(menu, i);
            strncpy(textbuf, entries[i].name, 0x10);
            entries[i].field_13b = 0;
            textbuf[0x10] = 0;
            if (entries[0].u.assets.archive)
                g = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, textbuf);
            if (g == 0) {
                if (menu->gaf != 0) {
                    g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, textbuf);
                    if (0 == g) {
                        do {
                            if (0x80 & entries[i].flags) {
                                g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "CHECKBOX");
                            } else if (entries[i].stage != 0) {
                                if (strcmp(entries[i].u.text, "Off|On") != 0 && entries[i].stage != 1 && 0 == (*pf & 0x4000)) {
                                    int n = entries[i].stage < 4 ? entries[i].stage : 4;
                                    sprintf(stagebuf, "stagebuttn%d", n);
                                } else {
                                    entries[i].stage = 2;
                                    strcpy(stagebuf, "stagebuttn1");
                                    *pf |= 0x4000;
                                }
                                g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, stagebuf);
                            } else {
                                strcpy(stagebuf, "BUTTONS0");
                                g = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, stagebuf);
                            }
                            if (g) {
                                int j = 0, best = 1000;
                                for (int f = 0; f < g->count; ++f) {
                                    Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, f);
                                    if (frame != 0) {
                                        frame->yoff = 0;
                                        frame->xoff = 0;
                                    }
                                }
                                if (j < g->count) do {
                                    Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, j);
                                    int distance = abs(entries[i].h - frame->h) + abs(entries[i].w - frame->w);
                                    if (distance < best) {
                                        entries[i].field_13b = (unsigned char)j;
                                        best = distance;
                                    }
                                } while (((j += 4), (j < g->count)));
                            }
                        } while (0);
                    }
                }
            }
            entries[i].gaf = g;
            if (g != 0) {
                Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(g, entries[i].field_13b);
                if (frame != 0) {
                    entries[i].w = frame->w;
                    entries[i].h = frame->h;
                }
            }
            if (0 != entries[i].stage) {
                char* p = entries[i].u.text;
                while (*p) {
                    if (*p == '|')
                        *p = 0;
                    p++;
                }
                Entry_004a81e0* cur = &menu->layer->entries[i];
                char* dst = buf3;
                char* src = cur->u.text;
                int k = 0;
                for (; k < cur->stage; k++) {
                    strcpy(dst, FUN_004c5740(src));
                    dst += strlen(dst) + 1;
                    src += strlen(src) + 1;
                }
                memcpy(cur->u.text, buf3, sizeof(buf3));
                *pf = (*pf & 0x4000) | 1;
            }
            break;
        }

        case 7:
            strcpy(buf2, menu->str_bb6);
            strcat(buf2, entries[i].u.text);
            strcat(buf2, ".FNT");
            entries[i].u.list.filebuf = FUN_004bbe50(buf2, 0);
            break;

        case 8:
            strcat(buf2, entries[i].u.text);
            entries[i].u.list.filebuf = FUN_004bbe50(buf2, 0);
            break;

        case 13: {
            Entry_004a81e0* en = menu->layer->entries;
            en[i].u.t13.f_c6 = FUN_004b6340() + en[i].u.t13.f_c2;
            break;
        }

        case 5:
            if (strlen((char*)&entries[i].field_136) == 0)
                entries[i].flags |= 0x10;
            else
                FUN_004a05e0(menu, i);
            entries[i].colours = 0;
            break;

        default:
            break;
        }
        i++;
    }

    name = entries[0].name;
    if (0 == name)
        name = "GUI SURFACE";
    entries[0].u.assets.surface = FUN_004c69f0(name, entries[0].w, entries[0].h);
    FUN_004c6b70(entries[0].u.assets.surface, 0, -entries[0].x, -entries[0].y);
    if (!(flags & 0x20)) {
        entries[0].u.assets.saveUnder = FUN_004c69f0("SAVE UNDER", entries[0].w, entries[0].h);
        FUN_004c6b70(entries[0].u.assets.saveUnder, entries[0].u.assets.surface, 0, 0);
    } else {
        entries[0].u.assets.saveUnder = 0;
    }
    }

    if ((flags & 4) != 0 || force || (flags & 0x40)) {
        if (force || (flags & 0x40)) {
            if (menu->layer->field_24)
                FUN_004c6b70(entries[0].u.assets.surface, menu->layer->field_24, 0, 0);
            else if ((flags & 0x80) == 0)
                FUN_004b0230(menu, 0, entries[0].u.assets.background);
        }

        for (i = 1; i < 1 + entries[0].u.count; i++) {
            if (entries[i].field_29 == 0)
                continue;
            switch (entries[i].type) {
            case 11:
                FUN_004b0230(menu, i, entries[i].u.assets.background);
                break;
            case 12:
                FUN_004a5e50(menu, i);
                break;
            case 1:
                if (force != 0 || (flags & 0x48) != 0)
                    FUN_004a5f40(menu, i);
                break;
            case 2: {
                if (force) {
                    int fh;
                    Entry_004a81e0* base = menu->layer->entries;
                    int t = 0;
                    int j;
                    base[i].u.list.field_bc = 0;
                    base[i].u.list.field_ba = 0;
                    for (j = 1; j < base[0].u.count + 1; j++) {
                        if (base[j].type == 7) {
                            if (t == base[i].field_28) {
                                FUN_004c1420((int)base[j].u.list.filebuf);
                                break;
                            }
                            t++;
                        }
                    }
                    if (j == base[0].u.count + 1)
                        FUN_004c1420(DAT_0051fba4->current);
                    if (DAT_0051fba4->language == 0)
                        fh = FUN_004c1450();
                    else
                        fh = ((Glyph_004a81e0*)FUN_004b7f30(DAT_0051fba4->language->glyphs, 0x49))->h + 2;
                    int hh = base[i].h;
                    base[i].h = (short)(hh - ((int)base[i].h) % (fh + 2));
                    base[i].u.list.field_0 = FUN_004b6340();
                }
                if (force || (flags & 0x40))
                    FUN_004a1b40(menu, i);
                break;
            }
            case 3:
                if (force || (flags & 0x40))
                    FUN_004a4d70(menu, i);
                break;
            case 4:
                if (force) {
                    Entry_004a81e0* base = menu->layer->entries;
                    base[i].f140 = 0;
                    base[i].f144 = 0;
                    base[i].f14a = 0;
                }
                if (force || (0x40 & flags))
                    FUN_004a3ef0(menu, i);
                break;
            case 5:
                if (force || (flags & 0x40))
                    FUN_004a56b0(menu, i);
                break;
            case 6:
                if (force) {
                    Entry_004a81e0* base = menu->layer->entries;
                    base[i].u.t6.f_b6 = 0;
                    base[i].u.t6.f_be = 0;
                    base[i].u.t6.f_c2 = 0;
                    base[i].u.t6.f_c6 = 0;
                }
                if (force || (flags & 0x40))
                    FUN_004a4980(menu, i);
                break;
            case 13:
                if (force) {
                    Entry_004a81e0* base = menu->layer->entries;
                    base[i].u.t13.f_c6 = FUN_004b6340() + base[i].u.t13.f_c2;
                }
                if (force || (flags & 0x40))
                    FUN_004a4660(menu, i);
                break;
            case 10:
                if (force || (flags & 0x40))
                    FUN_004a4c90(menu, i, flags);
                break;
            default:
                break;
            }
    }
        if (menu->layer->field_20 != -1 && menu->field_a2 != 0) {
            savedType = entries[menu->layer->field_20].type;
            FUN_004a16f0(menu, menu->layer->field_20, 8);
            int j = 1;
            Entry_004a81e0* other = &entries[1];
            while (j < entries[0].u.count + 1) {
                if (0 == strncmp(other->name, entries[0].u.text + 0x16, 0x10)) {
                    j = (int)(other - entries);
                    goto found;
                }
                j++, other++;
            }
            j = -1;
        found:
            if (j != -1 && savedType != 1 && entries[i].field_29 != 0)
                FUN_004a16f0(menu, j, 8);
        }
    }

    if (flags & 2) {
        if (entries[0].u.assets.saveUnder != 0) {
            FUN_004c6b70(0, entries[0].u.assets.saveUnder, entries[0].x, entries[0].y);
            FUN_004c6ac0(entries[0].u.assets.saveUnder);
            entries[0].u.assets.saveUnder = 0;
        }
        FUN_004c6ac0(entries[0].u.assets.surface);
        entries[0].u.assets.surface = 0;
        for (int j = 0; j < 1 + entries[0].u.count; j = j + 1) {
            if ((entries[j].resourceFlags & 1) && entries[j].archive)
                FUN_004d85a0((int*)entries[j].archive);
            switch (entries[j].type) {
            case 0:
                FUN_004d85a0((int*)entries[j].u.assets.archive);
                break;
            case 7:
                FUN_004d85a0((int*)entries[j].u.list.filebuf);
                break;
            case 8:
                FUN_004d85a0((int*)entries[j].u.list.filebuf);
                break;
            default:
                break;
            }
        }
    }

    return 1;
}
