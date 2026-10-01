// Decompiled by deepseek-v4.1, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// PARTIAL 75.8% (was 62.3%). Earlier fixes, still in the file (claude-sonnet-5-5):
//  - the walking pointer `p` is NOT written back to `text`. The original keeps
//    `text` untouched in its slot (only read at the top of the loop) and passes
//    p to strstr/strcpy/strchr/FUN_004a50e0; the flags & 0x20 branch calls the
//    inlined Measure on `text` (not p), so it measures the un-advanced string.
//  - flags & 0x20: `if (field_13a != 0 && (found = strchr(p, key)) != 0)`, no
//    `found = 0` store (the original has none).
//  - gaf glyph pick with field_13c & 1 == 0: field_136 != 0 selects field_137,
//    else field_13b (the earlier source had them swapped), and the four arms
//    are four separate FUN_004b7f30 calls in an if/else chain (MSVC merges the
//    tails into the shared `push eax; call`), not one call with a ternary.
//  - the clamp is `val = count - 1; if (field_138 + 2 < val) val = field_138 + 2`
//    (jge in the original).
// What moved it from 62.3% to 75.8% (Space Bunny Free):
//  - the frame is 0xd8 and 12 of the 16 frame slots now sit at the original's
//    offsets. The missing dword was `measured`: MSVC 5 only gives a local a
//    frame slot once its address is visible, and nothing in the plain
//    `x += Measure_004a5f40(key1)` form makes it visible. The hotkey measure is
//    now a second inline helper that accumulates into a caller-supplied total
//    (`MeasureInto_004a5f40(key1, measured)`), which is also what the original
//    does: slot 0x20 is the accumulator of the inlined Measure for the hotkey
//    and the bottom-aligned y of the flags & 0x20 branch at the same time.
//    Putting `acc = 0` before the `if (p != 0)` instead of inside it drops the
//    frame back to 0xd4 and the score to 61.8%.
//  - the flags & 0x20 branch must not use the shared `x`. With the shared
//    variable the frame is right but `rect` lands on 0x28, `x` on 0x24 and
//    width/border and textw/flagy swap: 68.7%. A block-local `xb` there puts
//    rect on 0x24, x on 0x34 and the whole tail back in place: 73.5%.
//  - the loop's `y` is built by parking the line height in `y` first
//    (`y = LineHeight_004a5f40(); y = rect.bottom - y - rect.top; ...`). That
//    is what makes MSVC keep `rect.top` in ecx and reload `me->flags` where the
//    original does: 73.5% to 75.5%. Five other spellings of the same value
//    (one statement, split in two, a separate `lh` local, ...) all commute
//    back to `bottom - top - lh`.
//  - the field_136/field_137 skip walk is a bare `if (me->field_136 != 0)`
//    with the count tested by the loop, not `if (a != 0 && b != 0)`: the
//    `&&` left a dead `and eax, 0xff` test behind. 75.5% to 75.8%.
//  - the skip loop is `do { ... } while (--k);` (0.1%).
// Still differs:
//  - four frame slots. `me` and `measured` are swapped (0x20 and 0x1c against
//    0x1c and 0x20) and the tail is a 3-cycle: saved 0x44, flagy 0x4c, text
//    0x54 against flagy 0x44, text 0x4c, saved 0x54. MSVC 5's frame layout in
//    this function is NOT declaration order: renaming a local, moving its
//    declaration before or after its neighbours, and swapping whole adjacent
//    pairs all leave every offset unchanged, and a brand new local is
//    appended after `saved` instead of being inserted at its declaration. Only
//    the shape of the code moves a slot (the two items above), so these four
//    are slot allocation that the declaration order cannot reach.
//  - the original caches `menu` in esi from the loop-top colour call through
//    the flags & 0x8000 test; ours reloads it and gives esi to `rect.top`.
//  - the flags & 2 branch's x is `(v / 2 + t) + left + 1` in the original and
//    `(v / 2 + left) + t + 1` here. Parenthesised, three-statement, t-as-
//    accumulator and unsplit spellings all compile to the same reassociated
//    form, so this is MSVC commuting the two adds, not the source order.
//  - the original duplicates the whole `FUN_004be950` block in both arms of
//    the field_138 test; ours hoists the first inlined LineHeight above the
//    test and shares the tail. Naming the two line heights (l1/l2), wrapping
//    them in a helper and parking one in `measured` all scored lower
//    (68.0% to 73.5%).
//  - the inlined Measure's `language == 0` path keeps its result in edi here
//    and in eax there. An early `return` in Measure_004a5f40 gives the
//    original's shape but stops the inlining: 41.9%, 2764 bytes.
//  - the underline call's x and y are in the opposite registers, and the
//    `field_137 != 0` test is `test al, al` here and `test eax, eax` there.
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
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, unsigned char colour);
void __stdcall FUN_004bfe10(void* surface, Rect_004a5f40* rect);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a5f40* rect, int param);
void __stdcall FUN_004b04b0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FUN_004b04e0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);

static inline int Measure_004a5f40(char* text)
{
    int width = 0;
    char* p = text;
    if (p != 0) {
        if (DAT_0051fba4->language == 0) {
            width = FUN_004c1480(FUN_004c1440(), text);
        } else {
            while (*p != 0) {
                char ch = *p;
                Glyph_004a5f40* glyph = FUN_004b7f30(
                    (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, (unsigned char)ch);
                if (glyph != 0)
                    width += glyph->width;
                ++p;
            }
        }
    }
    return width;
}

// The hotkey measure accumulates into a caller-supplied total, so the
// compiler keeps that total in a frame slot (this is what makes the frame
// 0xd8 and puts the slot at [esp+0x20], as the original does).
static inline void MeasureInto_004a5f40(char* text, int& acc)
{
    char* p = text;
    if (p != 0) {
        acc = 0;
        if (DAT_0051fba4->language == 0) {
            acc = FUN_004c1480(FUN_004c1440(), text);
        } else {
            while (*p != 0) {
                char ch = *p;
                Glyph_004a5f40* glyph = FUN_004b7f30(
                    (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, (unsigned char)ch);
                if (glyph != 0)
                    acc += glyph->width;
                ++p;
            }
        }
    }
}

static inline int LineHeight_004a5f40()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return FUN_004b7f30(
        (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, 0x49)->height + 2;
}

// FUNCTION: 0x4a5f40
void __stdcall FUN_004a5f40(Menu_004a5f40* menu, int index)
{
    char key1[2];
    char key2[2];
    void* surface;
    int t;
    Entry_004a5f40* me;
    int measured;
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
    int i;
    int y;

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
    rect.bottom = me->h + rect.top - 1;
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
                glyph = FUN_004b7f30(me->gaf, val + me->field_13b);
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
                FUN_004b8310(surface, glyph, glyph->xoff + rect.left,
                             glyph->yoff + rect.top, (int)me->colours);
            else
                FUN_004b7f90(surface, glyph, glyph->xoff + rect.left,
                             glyph->yoff + rect.top);
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

        char* p = text;
        if (me->field_136 != 0) {
            int k = me->field_137;
            do {
                while (*p != 0)
                    p++;
                p++;
            } while (--k != 0);
        }

        y = LineHeight_004a5f40();
        y = rect.bottom - y;
        y -= rect.top;
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
            if (me->field_13a == 0 || (me->field_13c & 1)) {
                FUN_004a50e0(surface, p, x, y, rect.right - rect.left + 1, 0);
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
                    x += Measure_004a5f40(buf);
                    saved = x;
                    if (me->field_138 != 0)
                        FUN_004c13a0(menu->colour_8b2, FUN_004c13f0());
                    else
                        FUN_004c13a0(me->colours[(int)menu + 0x8b2], FUN_004c13f0());
                    FUN_004a50e0(surface, key1, x, y, width, 0);
                    MeasureInto_004a5f40(key1, measured);
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
            int xb = (rect.right - textw - rect.left) / 2 + t;
            xb += rect.left + 1;
            measured = flagy - LineHeight_004a5f40();
            measured += rect.bottom - 4;
            if (me->field_13a != 0 && (found = strchr(p, (signed char)me->field_13a)) != 0) {
                width = rect.right - rect.left + 1;
                key2[0] = me->field_13a;
                key2[1] = 0;
                FUN_004c1440();
                FUN_004a50e0(surface, p, xb, measured, width, 0);
                *found = 0;
                xb += Measure_004a5f40(text);
                FUN_004c13a0(menu->colour_8bc, FUN_004c13f0());
                FUN_004a50e0(surface, key2, xb, measured, width, 0);
                xb += Measure_004a5f40(key2);
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