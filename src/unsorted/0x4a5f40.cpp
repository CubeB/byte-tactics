// Decompiled by deepseek-v4.1, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by Fable 5.1. Names are provisional.
// Fable 5.1 (#4452): 76.7% -> 88.1%, 2705 bytes (original 2700). A permuter
// run from this file (7864 candidates) found nothing. Compared with
// the original block by block; the changes that paid, in order:
//  - The four inlined text measures are the real FUN_004a5030 (matched in
//    0x4a5030.cpp), spelled as the matched siblings 0x4a53c0 / 0x4a4660 spell
//    their inline copy: `int width = 0;` first, then `if (p == 0) return 0;`,
//    then `return FUN_004c1480(...)` on the no-font path, then the walk. The
//    early returns are what leave the result in eax at every merge (the first
//    copy ends `mov eax, edi`, the no-font path jumps straight to `add esi,
//    eax`); the old `if (text != 0) { ... width = FUN(...) }` form assigned
//    the call result to the accumulator instead (`mov edi, eax`).
//  - The hotkey copy accumulates in memory ([esp+0x20], load / add / store
//    around FUN_004b7f30 in the loop, `xor eax,eax` shared with the font test)
//    only when `measured` is address-taken: `int* pm = &measured;` with the
//    flags & 0x20 branch writing its y through *pm does it (82.6% -> 88.1%
//    on this shape, instance 2 then matches instruction for instruction).
//    `int& acc` parameters, `int*` parameters, a one-element array and a
//    {measured, rect} struct do not: the struct makes the accumulator aliased
//    but the inliner then keeps a temp (memory at 0x4c) and copies it to the
//    member. No `lea [esp+0x20]` exists in the original, so the real cause is
//    still unknown; the alias is a stand-in for it and emits no code itself.
//  - The flags & 0x20 branch: `*found = 0;` goes before the first
//    FUN_004a50e0 call (the original splices the pushes around the store),
//    its x is a block-local `xb` (never stored, unlike the flags & 2 x at
//    [esp+0x34]) and its y is `measured` (the original shares slot 0x20).
//  - The glyph draw tests `me->colours != 0` first (FUN_004b8310 falls
//    through, FUN_004b7f90 is the jump), the skip loop is a guarded
//    `for (unsigned int k = me->field_137; k != 0; k--)` (`xor eax,eax; mov al;
//    test eax,eax; je` before it), FUN_004be950's colour parameter is `int`
//    (the original zero-extends the byte), the colour call after the hotkey
//    draw has its `field_138 != 0` arm first like every other colour call
//    (worth 3.6 points on its own), and `int t` is declared last (0.9).
// Still differs:
//  - Frame slots: ours x 0x18, t 0x1c, me 0x20, found 0x34, width 0x38,
//    border 0x3c, textw 0x40, measured 0x48 against t 0x18, me 0x1c, measured
//    0x20, x 0x34, found 0x38, width 0x3c, border 0x40, textw 0x48 (rect, key1,
//    key2, surface, flagy, text, pass, saved and buf are right). All 56
//    adjacent swaps and single moves of the declarations score the same, so
//    the order is not declaration order. Probes (build/scratch/0x4a5f40/slots/)
//    show MSVC 5 lays spilled locals out by size class and, within a class, in
//    an order that follows definition and use rather than declaration;
//    rect as four ints and a {measured, rect} struct both place worse.
//  - The flags & 0x20 branch's two values come out in each other's register:
//    the original keeps xb in ebx and spills ys to 0x20 (`lea esi, [edx+eax-4];
//    mov [esp+0x20], esi`), ours puts ys in ebx and spills xb. Every spelling
//    tried (xb as `x`, as `y`, block-local ys, ys through the alias or plain)
//    keeps that swap; it costs the branch about 12 bytes and is the main
//    reason the function is 5 bytes long.
//  - The subtraction in that branch is `(right - left) - textw` here against
//    `(right - textw) - left` there (the same expression in the flags & 2 arm
//    matches), and the colour byte for FUN_004c13a0 in the 0x20 branch's third
//    call loads menu after the font call where the original loads it before.
//  - The 0x20 branch's FUN_004be950 argument block, and the second LineHeight
//    result register in the flags & 2 arm (edi vs eax on one path).
// Not a bug but noted: the `do { ... } while (pass--)` loop runs its body once
// (pass starts at 0 and nothing else writes it).
#include <memory.h>
#include <windows.h>
#include <stdio.h>

#pragma pack(push, 1)
struct GafEntry_004a5f40 {
    unsigned short count;              // +0x00
    char unknown_2[2];
};

struct Glyph_004a5f40 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
    short xoff;                        // +0x04
    short yoff;                        // +0x06
};

struct Entry_004a5f40 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int flags;                         // +0x1b
    unsigned char* colours;            // +0x1f
    char unknown_23[0x28 - 0x23];
    signed char tab;                   // +0x28
    char unknown_29[0x2f - 0x29];
    GafEntry_004a5f40* gaf;            // +0x2f
    char unknown_33[0xb6 - 0x33];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0x20];               // +0xb6
    } u;
    int language;                      // +0xd6
    char unknown_da[0x136 - 0xda];
    unsigned char field_136;           // +0x136
    unsigned char field_137;           // +0x137
    short field_138;                   // +0x138
    unsigned char field_13a;           // +0x13a
    unsigned char field_13b;           // +0x13b
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x15b - 0x13d];
};

struct Layer_004a5f40 {
    char unknown_0[4];
    Entry_004a5f40* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14
};

struct Language_004a5f40 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct FontRoot_004a5f40 {
    int current;                       // +0x00
    char unknown_4[0x14 - 0x04];
    Language_004a5f40* language;       // +0x14
};

struct Menu_004a5f40 {
    char unknown_00[8];
    int values[3];                     // +0x08
    int current;                       // +0x14
    Layer_004a5f40* layer;             // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour_8b2;          // +0x8b2
    char unknown_8b3;
    unsigned char colour_8b4;          // +0x8b4
    char unknown_8b5[0x8bc - 0x8b5];
    unsigned char colour_8bc;          // +0x8bc
    char unknown_8bd[0x8c3 - 0x8bd];
    unsigned char colour_8c3;          // +0x8c3
    char unknown_8c4;
    unsigned char colour_8c5;          // +0x8c5
    unsigned char colour_8c6;          // +0x8c6
};
#pragma pack(pop)

struct Rect_004a5f40 { int left, top, right, bottom; };

extern FontRoot_004a5f40* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
Glyph_004a5f40* __stdcall FUN_004b7f30(GafEntry_004a5f40* table, int index);
void __stdcall FUN_004b7f90(void* surface, Glyph_004a5f40* glyph, int x, int y);
void __stdcall FUN_004b8310(void* surface, Glyph_004a5f40* glyph, int x, int y, int style);
int __stdcall FUN_004a5d50(Menu_004a5f40* menu, int index);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, int colour);
void __stdcall FUN_004bfe10(void* surface, Rect_004a5f40* rect);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a5f40* rect, int param);
void __stdcall FUN_004b04b0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FUN_004b04e0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);

// The real FUN_004a5030 (matched in 0x4a5030.cpp), which /Ob2 inlines four times here.
static inline int FUN_004a5030(char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    char* p = text;
    while (*p != 0) {
        char ch = *p;
        Glyph_004a5f40* glyph = (Glyph_004a5f40*)FUN_004b7f30(
            (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

static inline int LineHeight_004a5f40()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return FUN_004b7f30(
        (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, 0x49)->height + 2;
}

// Reading the field_13c bit through a helper is load bearing: spelled inline
// here, MSVC 5 lays the loop's frame out differently and the score drops.
static inline bool Style_004a5f40(Entry_004a5f40* e)
{
    return (e->field_13c & 1) != 0;
}

// FUNCTION: 0x4a5f40
void __stdcall FUN_004a5f40(Menu_004a5f40* menu, int index)
{
    char* p;
    char key1[2];
    char key2[2];
    void* surface;
    Entry_004a5f40* me;
    int measured;
    int* pm = &measured;
    Rect_004a5f40 rect;
    int x;
    char* found;
    int width;
    int border;
    int flagy;
    int textw;
    char* text;
    int pass;
    int saved;
    char buf[0x80];
    int y, i;
    int t;

    border = 0;
    if (menu->layer != 0)
        menu->layer->field_14 = 1;
    Entry_004a5f40* entries = menu->layer->entries;
    me = &entries[index];
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = rect.top + me->h - 1;
    if (me->flags & 0x8000)
        menu->current = menu->values[1];

    t = 0;
    for (i = 1; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == me->tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries->u.count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    textw = FUN_004a5d50(menu, index);
    surface = *(void**)((char*)entries + 0xbc);
    if (me->gaf != 0) {
        Glyph_004a5f40* glyph;
        if (me->field_13c & 1) {
            if (me->flags & 0x100) {
                glyph = FUN_004b7f30(me->gaf, me->gaf->count - 1);
            } else if (me->field_136 != 0) {
                glyph = FUN_004b7f30(me->gaf, me->field_137);
                border = 1;
            } else if (me->flags & 0x1800) {
                glyph = FUN_004b7f30(me->gaf, me->field_13b);
                border = 1;
            } else {
                int val = me->gaf->count - 1;
                if (me->field_138 + 2 < val)
                    val = me->field_138 + 2;
                glyph = FUN_004b7f30(me->gaf, ((int)val) + me->field_13b);
                if (!(me->flags & 0x80))
                    border = 1;
            }
        } else {
            if (me->field_138 != 0 && (unsigned short)me->gaf->count > (unsigned short)me->field_136) {
                if (me->field_136 != 0)
                    glyph = FUN_004b7f30(me->gaf, me->gaf->count - 2);
                else
                    glyph = FUN_004b7f30(me->gaf, me->field_13b + me->field_138);
            } else if (me->field_136 != 0)
                glyph = FUN_004b7f30(me->gaf, me->field_137);
            else
                glyph = FUN_004b7f30(me->gaf, me->field_13b);
        }
        if (glyph != 0) {
            if (me->colours != 0)
                FUN_004b8310(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top, (int)me->colours);
            else
                FUN_004b7f90(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top);
        }
    } else {
        if (me->field_13c & 1) {
            FUN_004b04b0(surface, &rect, menu->colour_8b2, menu->colour_8c5, menu->colour_8c5);
        } else if (me->field_138 != 0) {
            FUN_004b04b0(surface, &rect, menu->colour_8b2, menu->colour_8c3, menu->colour_8c6);
        } else {
            FUN_004b04e0(surface, &rect, menu->colour_8b2, menu->colour_8c3, menu->colour_8c6);
        }
    }

    t = 0;
    flagy = 0;
    if (me->field_138 != 0) {
        t = 1;
        flagy = 1;
    }
    text = me->u.text;
    pass = 0;
    do {
        if (me->field_138 != 0)
            FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
        else
            FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());

        p = text;
        if (me->field_136 != 0) {
            for (unsigned int k = me->field_137; k != 0; k--) {
                while (*p != 0)
                    p++;
                p++;
            }
        }

        y = LineHeight_004a5f40();
        y = rect.bottom - y;
        int top = rect.top;
        y -= top;
        y = y / 2 + flagy + rect.top;
        if (me->flags & 0x8000)
            menu->current = menu->values[1];

        if (me->flags & 1) {
            FUN_004a50e0(surface, p, t + rect.left + 3, y,
                         rect.right - rect.left + 1, 0);
        } else if (me->flags & 4) {
            x = rect.right - textw - 3;
            if (x < rect.left)
                x = rect.left;
            FUN_004a50e0(surface, p, x, y, rect.right - rect.left + 1, 0);
        } else if (me->flags & 2) {
            x = (rect.right - textw - rect.left) / 2 + t;
            x += rect.left + 1;
            if (me->field_13a == 0 || Style_004a5f40(me)) {
                FUN_004a50e0(surface, p, x, y, 1 + (rect.right - rect.left), 0);
            } else {
                key1[0] = me->field_13a;
                width = rect.right - rect.left + 1;
                key1[1] = 0;
                strcpy(buf, p);
                found = strstr(buf, key1);
                if (found != 0) {
                    FUN_004c1440();
                    strcpy(buf, p);
                    *found = 0;
                    FUN_004a50e0(surface, buf, x, y, width, 0);
                    x += FUN_004a5030(buf);
                    saved = x;
                    if (me->field_138 != 0)
                        FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
                    else
                        FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                    FUN_004a50e0(surface, key1, x, y, width, 0);
                    measured = FUN_004a5030(key1);
                    x += measured;
                    if (me->field_138 != 0) {
                        FUN_004be950(surface, saved, LineHeight_004a5f40() + y - 1,
                                     x - 1, LineHeight_004a5f40() + y - 1,
                                     menu->colour_8b2);
                    } else {
                        FUN_004be950(surface, saved, LineHeight_004a5f40() + y - 1,
                                     x - 1, LineHeight_004a5f40() + y - 1,
                                     menu->colour_8b4);
                    }
                    if (me->field_138 != 0)
                        FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
                    else
                        FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                    FUN_004a50e0(surface, found + 1, x, y, width, 0);
                } else {
                    FUN_004a50e0(surface, p, x, y, width, 0);
                }
            }
        } else if (me->flags & 0x20) {
            int xb;
            xb = (rect.right - textw - rect.left) / 2 + t;
            xb += rect.left + 1;
            *pm = flagy - LineHeight_004a5f40();
            *pm += rect.bottom - 4;
            if (me->field_13a != 0 && (found = strchr(p, (signed char)me->field_13a)) != 0) {
                width = rect.right - rect.left + 1;
                key2[0] = me->field_13a;
                key2[1] = 0;
                FUN_004c1440();
                *found = 0;
                FUN_004a50e0(surface, p, xb, measured, width, 0);
                xb += FUN_004a5030(text);
                FUN_004c13a0(menu->colour_8bc, FUN_004c13f0());
                FUN_004a50e0(surface, key2, xb, measured, width, 0);
                xb += FUN_004a5030(key2);
                FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                FUN_004a50e0(surface, found + 1, xb, measured, width, 0);
            } else {
                FUN_004a50e0(surface, p, xb, measured, rect.right - rect.left + 1, 0);
            }
        }
    } while (pass--);

    menu->current = menu->values[0];
    if (border) {
        FUN_004bfe10(surface, &rect);
        FUN_004bf4d0(surface, &rect, -0x14);
    }
}