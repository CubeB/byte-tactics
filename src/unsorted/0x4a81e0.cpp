// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, finished by GPT-6.1-sol, edited by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// retry by deepseek-v4.1-flash (#3418, 10-minute timebox): re-checked only, still
// 26.2% (3780 bytes vs 5248); the timebox was spent on the closer 0x4a9fd0, so
// nothing new was tried here beyond a baseline check.
// Retry by deepseek-v4.1-flash (#3535, 10-minute timebox): 26.2 -> 37.0%.
// The big win was loop2's case 2 (0x4a91c7..0x4a92da): with L.force set the
// list box bases its sort key on the type 7 entry in the same group, then
// rounds its height down to a multiple of the font height plus two and
// stores FUN_004b6340() at u+0. Still missing: the post-loop FUN_004a16f0
// block (0x4a943b..0x4a950a, reads layer->field_20 and menu->field_a2 and
// scans for a matching name), and the remaining loop2 cases need their
// own scratch fields; a batch that added guards to cases 3/5/11 scored
// 36.7% and the full set of case 4/6/13/10 bodies scored 30.3%, so both
// were reverted, the guards do not line up with our stack layout yet.
// Learned from the disassembly and now encoded here: entry 0's union holds
// the two surface handles (SAVE UNDER +0xb8, GUI SURFACE +0xbc, then the
// frame GAF at +0xc0 and the background GAF at +0xc4); the forced path
// skips the SAVE UNDER bitmap when flags&0x20 and stores 0 there instead;
// after_entries tests menu->layer->field_24 (a layer field at +0x24), not
// entries[0].u.text, and passes entries[0].u.assets.background to
// FUN_004b0230; the flags&2 teardown is now the real one (saveUnder blit +
// free, surface free, per-entry free of archive and of u.list.filebuf at
// +0xd6 for types 7/8). loop2's case 2 body (0x4a91c7..0x4a92da) and the
// post-loop FUN_004a16f0 block (0x4a943b..0x4a950a, uses layer->field_20
// and menu->field_a2) are still missing, which is most of the byte deficit.
// Issue #2354 retry by GPT-6.1-sol: the saved 26.2% source remains best after one
// targeted stage-string variant scored 25.8%; three worker checks total, no MATCH.
// Partial: 26.2%, 3776 bytes versus the original 5248. What still differs:
//
// - Prologue. The frame is now exactly 0x3c0 like the original, and param_1
//   sits at [esp+0x3d4], but the original is `mov eax,[esp+4]; sub esp,0x3c0;
//   mov eax,[eax+0x18]; push ebx; push ebp; push esi; test eax,eax; push edi`
//   and reloads param_1 from [esp+0x3d4] at every later use, while ours homes
//   it in edi (`mov edi,[esp+0x3d4]`) and tests it after all four pushes.
//   That single register choice shifts every following byte of the entry block
//   and is what still keeps the score down: the original homes the layer
//   pointer (entries) in ebp and the flags argument in ebx, we use ebx and eax.
//   The allocator only picks those registers when the whole body is present, so
//   this should fall out once the missing blocks below are filled in.
// - Missing bodies. 0x4a9065..0x4a90d0 (the "SAVE UNDER" tail of the forced
//   path) and 0x4a90d1..0x4a95d1 (the flags&1 == 0 path) have no counterpart
//   here; together they are roughly 1280 bytes, most of the byte deficit, and
//   the block from 0x4a82f7 onwards is emitted in a different order than this
//   file, so the diff never fully resynchronises.
// - Fixed this round: the small pre-loop now walks a pointer with
//   `add eax,0x15b` while keeping the index in ecx (0x4a8321), and the big
//   entry loop indexes `entries[L.i].field` directly instead of through a
//   pointer local, which is what makes MSVC emit the shl/sub/lea stride chain
//   and keep the array base in ebp (0x4a83d1). The last text buffer is
//   char[0x6c] because the original's buffer at esp+0x350 ends exactly at the
//   frame end 0x3c0. 24.0 -> 26.2 in this round.
// - Scal_004a81e0 below matches the original's scalar slots only in shape:
//   the original keeps i at [esp+0x1c] and the menu+0x9b6 pointer at
//   [esp+0x2c]; ours land elsewhere because the frame still has 4 bytes of
//   compiler temporaries the original does not appear to need.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    char unknown_23[0x29 - 0x23];
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
            char unknown_a[0x14 - 0xa];
            GafEntry_004a81e0* gaf;
            char unknown_18[0x20 - 0x18];
            void* filebuf;                  // +0xd6, buffer type 7/8 loads
            short scroll;
        } list;
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
    char unknown_13e[0x142 - 0x13e];
    short sliderThumb;
    char unknown_144[0x14e - 0x144];
    GafEntry_004a81e0* sliderGaf;
    unsigned char sliderStyle;
    char unknown_153[0x15b - 0x153];
};

struct Layer_004a81e0 {
    char unknown_00[4];
    Entry_004a81e0* entries;            // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                       // +0x14
    char unknown_18[0x24 - 0x18];
    void* field_24;                     // +0x24
};

struct Menu_004a81e0 {                  // the object callers pass as arg1
    char unknown_00[4];
    void* gaf;                          // +0x04
    char unknown_08[0x18 - 0x08];
    Layer_004a81e0* layer;              // +0x18
    char unknown_1c[0x70 - 0x1c];
    int field_70;                       // +0x70
    char unknown_74[0x9b6 - 0x74];
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
void __stdcall FUN_004a4c90(void* obj, int index, unsigned char param3);
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

// The memory-resident scalars. &L.i escapes to FUN_004bbe50, which keeps the
// whole struct in memory. entries is deliberately not a field; it lives in
// ebp. See the frame note in the header.
struct Scal_004a81e0 {
    void* p4;                          // +0x10
    Glyph_004a81e0* g;                 // +0x14
    int force;                         // +0x18
    int i;                             // +0x1c
    short* pfield;                     // +0x20
    char* text;                        // +0x24
    Entry_004a81e0* e;                 // +0x28
    char* name9b6;                     // +0x2c
};

// FUNCTION: 0x4a81e0
int __stdcall FUN_004a81e0(Menu_004a81e0* menu, unsigned int flags)
{
    Entry_004a81e0* entries;
    char* name;
    Scal_004a81e0 L;
    char stagebuf[0x20];
    char textbuf[0x100];
    char buf1[0x100];
    char buf2[0x100];
    char buf3[0x6c];

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
    if (entries[0].x == -1) {
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

    L.force = flags & 1;
    if (L.force == 0)
        goto after_entries;

    while (FUN_004c1ab0() != 0)
        ;
    if (menu->field_70 != 0) {
        Entry_004a81e0* walk = entries;
        for (L.i = 0; L.i <= entries[0].u.count; L.i++, walk++) {
            if (walk->type == 1)
                walk->field_13a = 0;
        }
    }
    if (entries[0].w > FUN_004b6700())
        return 0;
    if (entries[0].h > FUN_004b6710())
        return 0;

    entries[0].u.assets.archive = 0;
    L.name9b6 = menu->str_9b6;
    for (L.i = 0; L.i < entries[0].u.count + 1; L.i++) {
        buf2[0] = 0;
        if (menu->str_9b6[0])
            strcpy(buf2, menu->str_9b6);
        L.p4 = 0;
        if (entries[L.i].resourceFlags & 1) {
            entries[L.i].archive = 0;
            entries[L.i].gaf = 0;
            buf1[0] = 0;
            if (menu->str_ab6[0])
                strncpy(buf1, menu->str_ab6, 0x100);
            strcat(buf1, entries[L.i].name);
            strcat(buf1, "_gadget");
            FUN_004baff0(buf1, buf1, "GAF");
            if (FUN_004bbc40(buf1)) {
                entries[L.i].archive = FUN_004b8c60(buf1);
                if (entries[L.i].archive)
                    entries[L.i].gaf = (GafEntry_004a81e0*)FUN_004b8d40(entries[L.i].archive, entries[L.i].name);
            }
        }
        switch (entries[L.i].type) {
        case 0:
        case 11: {
            if (entries[0].y < 0)
                entries[0].y += (short)FUN_004b6710();
            buf1[0] = 0;
            if (menu->str_ab6[0])
                strncpy(buf1, menu->str_ab6, 0x100);
            strncpy(textbuf, entries[0].name, 0x10);
            textbuf[0x10] = 0;
            strcat(buf1, textbuf);
            FUN_004baff0(buf1, buf1, "GAF");
            if (!entries[0].u.assets.archive && FUN_004bbc40(buf1))
                entries[0].u.assets.archive = FUN_004b8c60(buf1);
            strncpy(textbuf, entries[0].u.text + 0x46, 0x10);
            textbuf[0x10] = 0;
            GafEntry_004a81e0* background = 0;
            if (entries[0].u.assets.archive)
                background = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, textbuf);
            if (background == 0 && menu->gaf != 0) {
                background = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, textbuf);
                if (background == 0) {
                    background = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "BackTile");
                    if (background != 0) {
                        for (int frameIndex = 0; frameIndex < background->count; frameIndex++) {
                            Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(background, frameIndex);
                            frame->yoff = 0;
                            frame->xoff = 0;
                        }
                    }
                }
            }
            entries[0].u.assets.background = background;
            break;
        }

        case 4: {
            entries[L.i].field_13b = 0;
            GafEntry_004a81e0* sliders = 0;
            if (entries[0].u.assets.archive)
                sliders = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, "SLIDERS");
            if (sliders == 0 && menu->gaf != 0) {
                sliders = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "SLIDERS");
                if (sliders != 0) {
                    for (int f = 0; f < sliders->count; f++) {
                        Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(sliders, f);
                        frame->yoff = 0;
                        frame->xoff = 0;
                    }
                    int orientation = entries[L.i].w > entries[L.i].h ? 10 : 0;
                    Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(sliders, orientation);
                    if (entries[L.i].w < entries[L.i].h)
                        entries[L.i].w = frame->w;
                    else
                        entries[L.i].h = frame->h;
                    entries[L.i].sliderStyle = (unsigned char)orientation;
                }
            }
            entries[L.i].sliderGaf = sliders;
            if (sliders != 0) {
                Entry_004a81e0* active = menu->layer->entries;
                active[0].u.count++;
                Entry_004a81e0* created = &active[active[0].u.count];
                memset(created, 0, sizeof(*created));
                created->type = 1;
                created->field_29 = 1;
                Entry_004a81e0* firstEnd = &entries[active[0].u.count];
                firstEnd->x = entries[L.i].x;
                firstEnd->y = entries[L.i].y;
                firstEnd->gaf = sliders;
                firstEnd->field_13b = entries[L.i].sliderStyle + 6;
                firstEnd->group = entries[L.i].group;
                Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(sliders, entries[L.i].sliderStyle + 6);
                firstEnd->w = frame->w;
                firstEnd->h = frame->h;
                firstEnd->flags = 0x3400;
                firstEnd->field_29 = entries[L.i].field_29;
                active = menu->layer->entries;
                active[0].u.count++;
                created = &active[active[0].u.count];
                memset(created, 0, sizeof(*created));
                created->type = 1;
                created->field_29 = 1;
                Entry_004a81e0* secondEnd = &entries[active[0].u.count];
                secondEnd->field_29 = entries[L.i].field_29;
                frame = (Glyph_004a81e0*)FUN_004b7f30(sliders, entries[L.i].sliderStyle + 8);
                secondEnd->gaf = sliders;
                secondEnd->y = entries[L.i].x;
                secondEnd->field_13b = entries[L.i].sliderStyle + 8;
                secondEnd->group = entries[L.i].group;
                secondEnd->flags = 0x2c00;
                secondEnd->w = frame->w;
                secondEnd->h = frame->h;
                frame = (Glyph_004a81e0*)FUN_004b7f30(sliders, entries[L.i].sliderStyle + 6);
                if (entries[L.i].w > entries[L.i].h) {
                    secondEnd->x = entries[L.i].x - frame->w + entries[L.i].w;
                    entries[L.i].w += (short)(-2 * frame->w);
                    entries[L.i].x += frame->w;
                    frame = (Glyph_004a81e0*)FUN_004b7f30(sliders, entries[L.i].sliderStyle + 5);
                    entries[L.i].sliderThumb = frame->w;
                    entries[L.i].field_136 = entries[L.i].w - entries[L.i].sliderThumb - 4;
                } else {
                    secondEnd->y = entries[L.i].y - frame->h + entries[L.i].h;
                    secondEnd->x = entries[L.i].x;
                    entries[L.i].h += (short)(-2 * frame->h);
                    entries[L.i].y += frame->h;
                }
            } else {
                entries[L.i].field_136 = (entries[L.i].w > entries[L.i].h ? entries[L.i].w : entries[L.i].h) - 6;
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
            entries[L.i].inputGaf = input;
            if (entries[L.i].field_138 >= 0x80)
                entries[L.i].field_138 = 0x7f;
            memset(entries[L.i].u.text, 0, 0x80);
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
            entries[L.i].u.list.gaf = list;
            for (int j = 1; j <= entries[0].u.count; j++) {
                Entry_004a81e0* other = &entries[j];
                if (j != L.i && other->type == 2 && other->group == entries[L.i].group) {
                    short scroll = other->u.list.scroll > entries[L.i].u.list.scroll ? other->u.list.scroll : entries[L.i].u.list.scroll;
                    entries[L.i].u.list.scroll = scroll;
                    other->u.list.scroll = scroll;
                }
            }
            break;
        }
        case 12: {
            strncpy(textbuf, entries[L.i].name, 0x10);
            textbuf[0x10] = 0;
            entries[L.i].colours = 0;
            entries[L.i].u.animation.firstFrame = 0;
            GafEntry_004a81e0* animation = 0;
            if (entries[0].u.assets.archive)
                animation = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, textbuf);
            if (animation == 0)
                animation = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, textbuf);
            if (animation != 0)
                entries[L.i].u.animation.firstFrame = (Glyph_004a81e0*)FUN_004b7f30(animation, 0);
            break;
        }

        case 1: {
            entries[L.i].colours = 0;
            if ((entries[L.i].flags & 0x1800) || (entries[L.i].resourceFlags & 1))
                break;
            FUN_004a05e0(menu, L.i);
            GafEntry_004a81e0* entry = 0;
            strncpy(textbuf, entries[L.i].name, 0x10);
            textbuf[0xf] = 0;
            entries[L.i].field_13b = 0;
            if (entries[0].u.assets.archive)
                entry = (GafEntry_004a81e0*)FUN_004b8d40(entries[0].u.assets.archive, textbuf);
            if (entry == 0 && menu->gaf != 0) {
                entry = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, textbuf);
                if (entry == 0) {
                    if (entries[L.i].flags & 0x80) {
                        entry = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, "CHECKBOX");
                    } else if (entries[L.i].stage != 0) {
                        int n = entries[L.i].stage < 4 ? entries[L.i].stage : 4;
                        sprintf(stagebuf, "stagebuttn%d", n);
                        entry = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, stagebuf);
                        if (entries[L.i].stage == 1) {
                            entries[L.i].stage = 2;
                            entries[L.i].flags |= 0x4000;
                        }
                    } else {
                        strcpy(stagebuf, "BUTTONS0");
                        entry = (GafEntry_004a81e0*)FUN_004b8d40(menu->gaf, stagebuf);
                    }
                    if (entry != 0) {
                        int best = 1000;
                        for (int f = 0; f < entry->count; f++) {
                            Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(entry, f);
                            if (frame != 0) {
                                frame->yoff = 0;
                                frame->xoff = 0;
                            }
                        }
                        for (int j = 0; j < entry->count; j += 4) {
                            Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(entry, j);
                            int distance = abs(entries[L.i].h - frame->h) + abs(entries[L.i].w - frame->w);
                            if (distance < best) {
                                entries[L.i].field_13b = (unsigned char)j;
                                best = distance;
                            }
                        }
                    }
                }
            }
            entries[L.i].gaf = entry;
            if (entry != 0) {
                Glyph_004a81e0* frame = (Glyph_004a81e0*)FUN_004b7f30(entry, entries[L.i].field_13b);
                if (frame != 0) {
                    entries[L.i].w = frame->w;
                    entries[L.i].h = frame->h;
                }
            }
            break;
        }

        case 7:
            strncpy(buf2, menu->str_bb6, 0x100);
            { int fileSize; FUN_004bbe50(buf2, &fileSize); }
            break;

        case 8:
            strncpy(buf2, entries[L.i].name, 0x10);
            { int fileSize; FUN_004bbe50(buf2, &fileSize); }
            break;

        case 13:
            entries[L.i].field_138 = (short)FUN_004b6340();
            break;

        case 5:
            if (entries[L.i].field_136 != 0)
                FUN_004a05e0(menu, L.i);
            break;

        default:
            break;
        }
    }

    L.e = &entries[0];
    name = entries[0].name;
    if (name == 0)
        name = "GUI SURFACE";
    entries[0].u.assets.surface = FUN_004c69f0(name, entries[0].w, entries[0].h);
    FUN_004c6b70(entries[0].u.assets.surface, 0, -entries[0].x, -entries[0].y);
    if (flags & 0x20) {
        entries[0].u.assets.saveUnder = 0;
    } else {
        entries[0].u.assets.saveUnder = FUN_004c69f0("SAVE UNDER", entries[0].w, entries[0].h);
        FUN_004c6b70(entries[0].u.assets.saveUnder, entries[0].u.assets.surface, 0, 0);
    }

after_entries:
    if ((flags & 4) || L.force || (flags & 0x40)) {
        if (menu->layer->field_24) {
            FUN_004c6b70(entries[0].u.assets.surface, menu->layer->field_24, 0, 0);
        } else if ((flags & 0x80) == 0) {
            FUN_004b0230(menu, 0, entries[0].u.assets.background);
        }
    }

    for (L.i = 1; L.i <= entries[0].u.count; L.i++) {
        if (entries[L.i].field_29 == 0)
            continue;
        switch (entries[L.i].type) {
        case 11:
            FUN_004b0230(menu, L.i, 0);
            break;
        case 12:
            FUN_004a5e50(menu, L.i);
            break;
        case 1:
            if (L.force || (flags & 0x48) != 0)
                FUN_004a5f40(menu, L.i);
            break;
        case 2: {
            if (L.force) {
                Entry_004a81e0* base = menu->layer->entries;
                int t = 0;
                int j;
                base[L.i].u.list.field_bc = 0;
                base[L.i].u.list.field_ba = 0;
                for (j = 1; j <= base[0].u.count; j++) {
                    if (base[j].type == 7) {
                        if (t == base[L.i].group)
                            break;
                        t++;
                    }
                }
                if (j != base[0].u.count + 1)
                    FUN_004c1420((int)base[j].u.list.filebuf);
                if (j == base[0].u.count + 1)
                    FUN_004c1420(DAT_0051fba4->current);
                int fh;
                if (DAT_0051fba4->language == 0)
                    fh = FUN_004c1450();
                else
                    fh = ((Glyph_004a81e0*)FUN_004b7f30(DAT_0051fba4->language->glyphs, 0x49))->h + 2;
                base[L.i].h -= (short)(base[L.i].h % (fh + 2));
                base[L.i].u.list.field_0 = FUN_004b6340();
            }
            if (L.force || (flags & 0x40))
                FUN_004a1b40(menu, L.i);
            break;
        }
        case 3:
            FUN_004a4d70(menu, L.i);
            break;
        case 4:
            entries[L.i].field_136 = 0;
            FUN_004a3ef0(menu, L.i);
            break;
        case 5:
            FUN_004a56b0(menu, L.i);
            break;
        case 6:
            if ((L.force == 0) && (flags & 0x40) == 0)
                break;
            FUN_004a4980(menu, L.i);
            break;
        case 13:
            if ((L.force == 0) && (flags & 0x40) == 0)
                break;
            FUN_004a4660(menu, L.i);
            break;
        case 10:
            if ((L.force == 0) && (flags & 0x40) == 0)
                break;
            FUN_004a4c90(menu, L.i, 0);
            break;
        default:
            break;
        }
    }
    FUN_004c1420(DAT_0051fba4->current);

    FUN_004a16f0(menu, 0, 8);
    if (L.i == entries[0].u.count + 1)
        FUN_004c1420(DAT_0051fba4->current);
    if (strncmp(buf1, buf3, 0x10) != 0)
        FUN_004a16f0(menu, 1, 8);

    if (flags & 2) {
        if (entries[0].u.assets.saveUnder != 0) {
            FUN_004c6b70(0, entries[0].u.assets.saveUnder, entries[0].x, entries[0].y);
            FUN_004c6ac0(entries[0].u.assets.saveUnder);
            entries[0].u.assets.saveUnder = 0;
        }
        FUN_004c6ac0(entries[0].u.assets.surface);
        entries[0].u.assets.surface = 0;
        for (int j = 0; j <= entries[0].u.count; j++) {
            if ((entries[j].resourceFlags & 1) && entries[j].archive != 0)
                FUN_004d85a0((int*)entries[j].archive);
            switch (entries[j].type) {
            case 0:
                FUN_004d85a0((int*)entries[j].u.assets.archive);
                break;
            case 7:
            case 8:
                FUN_004d85a0((int*)entries[j].u.list.filebuf);
                break;
            default:
                break;
            }
        }
    }

    L.pfield = &entries[0].x;
    (void)L.pfield;
    return 1;
}
