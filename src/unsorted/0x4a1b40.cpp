// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, finished by Space Bunny Free, rewritten by claude-opus-5-5, finished by Fledge Alpha Free. Names are provisional.
// 2026-10-02 (Fledge Alpha Free): 86.3% -> 87.1%, 2158 bytes, shape 95.7%.
// A permute.py run found the deltas; its tmp0/inl0/inl1/tmp* residue was
// cleaned away again (score is identical):
//  - `unsigned int top` (was plain int): rotation of top's register/home.
//  - the step arms swapped, `if (!(me->field_da != 0)) step = lh + 1; else
//    step = me->field_da;` (polarity flip of the original je).
//  - the surface test inverted: `if (!(0 == surface && !(holder->field_10 &
//    0x80))) { if (surface != 0) FUN_004c6d20(...); } else FUN_004b0230(...);`
//  - the tab-scan loop as `for (i = 1; i <= entries->count; i++)` rewritten by
//    permute to `int i = 1; int t = 0; if (i <= entries->count) { do { ... }
//    while (entries->count >= i); }` gives the same shape with jge.
//  - the text-draw loop is `while (1)` and the cell loop `for (; 1; )`.
//  - bounds.right via `unsigned int rowRight = bounds.left + me->w - 1;` and
//    bounds.bottom read as `(&entries[index])->h + bounds.top - 1` (the
//    non-duplicate read restores a CSE-merge only the original has).
//  - `#include <stdlib.h>` next to `<stdio.h>`: 87.1% (windows.h gave 86.9/86.3).
// What still differs (unchanged score after permuter round 2 and manual
// rounds of scope/declaration experiments, all flat): the frame slot
// rotation (me at [esp+0x58] vs 0x50, step at 0x50 vs 0x54, xx at 0x54
// vs 0x58, bp at 0x38 vs 0x34 region) plus the two `lea eax, [eax + esi -
// 1]` vs `[esi + eax - 1]` operand orders, `cmp ecx, esi` vs `cmp esi,
// ecx`, and the two `inc ecx`/`inc edx` done as lea here. ColPtr, bp,
// cellPtr, v, xx, xw, line, k, flag and yy all cycle between the 0x10/0x14
// pair and the 0x30/0x34/0x38 block; no source respelling I tried swaps
// them back, and `int bp` vs `unsigned int bp` moves only the earlier
// scoring version.
//
// 2026-10-02 (claude-opus-5-5): 73.3% -> 74.9%, exact size (2160 bytes), rewritten
// as plain code. The previous file scored 73.3% but did not compute what the
// original computes: its glyph-width loop advanced `q` itself, so the text was
// drawn from the end of the string, and its field_d6 test skipped the "&G"
// check whenever field_d6 was set. Here the width is the inlined
// Measure_004a1b40 helper its matched siblings use (0x4a4660, 0x4a53c0): the
// original walks a copy of q in edi (`mov edi,ebp`), sums in ebx and hands the
// result over with `mov edx,ebx`, the inline-return shape. The &G test is
// `field_d6 && field_d6[y] == 1` (0x4a1df9 and 0x4a1e03 both fall into it).
// What moved the clean version from 42% (measured one by one, the rest of the
// permute.py run that found them was neutral and has been reverted):
//  - `<stdio.h>` next to `<windows.h>` (<math.h> works as well; windows.h
//    alone is 69.5%).
//  - the text loop's exit tests `h >= lh` first:
//    `if (h >= lh) { if (line + bc >= c0) return; } else break;` (71.6% with
//    `if (h < lh) break;` first, whatever the spelling).
//  - the cell rect's stores go x pair then y pair (left, right, top, bottom),
//    +1.2; rowRect declared after `step`, +0.3; colPtr/bp/yoff/v declared at
//    function scope, +0.1.
//  - `colPtr = 0;` before the bp test is load-bearing (60.8% without): the
//    original zeroes colPtr's slot on the bp path.
// What still differs: one register rotation through the whole function. The
// original has obj in ebp, the zero/top in ebx and me in edi (me spilled at
// [esp+0x50], step at [esp+0x54]); this file has obj in ebx, the zero in edi,
// me in ebp, me at [esp+0x58] and step at [esp+0x50]. The instruction stream
// is otherwise the original's (shape 79.2%). Declaration order, the step and
// top spellings, the Measure/LineHeight forms and every header set with
// windows.h are flat.
// Slot map of the original, for the next attempt (offsets after the pushes):
// 0x10 t/line/colPtr, 0x14 flag/yy, 0x18 rowRect, 0x28 y, 0x2c q then the text
// y copy, 0x30 yoff/bp, 0x34 h/k, 0x38 entries, 0x3c lh/cellPtr, 0x40 bounds,
// 0x50 me, 0x54 step, 0x58 xx, 0x5c xw, 0x60 font, 0x64 col, 0x68 glyph char,
// 0x6c dst, 0x8c hl, 0x9c clip, 0xac src.
// Suspected original bug: in cell mode with a null item cell, a selected row
// still reads cell->width/height for the highlight (`je 0x4a2216` at 0x4a2126
// skips the draw only; 0x4a2233 and 0x4a224c read [edi] with edi = 0).
#include <stdlib.h>
#include <stdio.h>

#pragma pack(push, 1)

struct Entry_004a1b40 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colours;                        // +0x1f
    char unknown_23[0x28 - 0x23];
    char tab;                           // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                        // +0xb6 (entry 0)
    short unknown_b8;
    short field_ba;                     // +0xba
    union {
        void* surface;                  // +0xbc (entry 0)
        short field_bc;                 // +0xbc
    };
    short field_c0;                     // +0xc0
    char* text;                         // +0xc2
    int field_c6;                       // +0xc6
    char unknown_ca[0xd6 - 0xca];
    char* field_d6;                     // +0xd6
    short field_da;                     // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Holder_004a1b40 {
    char unknown_00[4];
    Entry_004a1b40* entries;            // +0x04
    char unknown_08[0x10 - 0x08];
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                       // +0x20
    void* surface;                      // +0x24
};

struct Class_004a1b40 {
    char unknown_00[0x18];
    Holder_004a1b40* holder;            // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour[0x8be - 0x8b2]; // +0x8b2
    unsigned char colour_8be;           // +0x8be
    char unknown_8bf[0xcd2 - 0x8bf];
    void* fallback;                     // +0xcd2
};

struct Glyph_004a1b40 {
    unsigned short width;               // +0x0
    unsigned short height;              // +0x2
};

struct Language_004a1b40 {
    char unknown_0[0xc];
    unsigned short* glyphs;             // +0xc
};

struct LanguageRoot_004a1b40 {
    int current;                        // +0x0
    char unknown_04[0x14 - 0x04];
    Language_004a1b40* language;        // +0x14
};

struct Rect_004a1b40 {
    int left;
    int top;
    int right;
    int bottom;
};

struct Point_004a1b40 { int x; int y; };
struct Quad_004a1b40 { Point_004a1b40 points[4]; };

struct Cell_004a1b40 {
    unsigned short width;               // +0x00
    unsigned short height;              // +0x02
    char unknown_04[0x10 - 0x04];
    int field_10;                       // +0x10
    char unknown_14[0x18 - 0x14];
};

struct Item_004a1b40 {
    char unknown_0[0x28];
    Cell_004a1b40* cell;                // +0x28
};

struct Class_004c6ae0 {
    void FUN_004c6ae0(Rect_004a1b40* rect);
};

struct Class_004c6b10 {
    void FUN_004c6b10(Rect_004a1b40 rect);
};

#pragma pack(pop)

extern LanguageRoot_004a1b40* DAT_0051fba4;

void __stdcall FUN_004b0230(Class_004a1b40* obj, int index, void* bmp);
void __stdcall FUN_004c6d20(void* dst, void* src, Rect_004a1b40* rect, Rect_004a1b40* pos);
void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
void __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                            int rem, int style);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a1b40* rect, int id);
void __stdcall FUN_004c7580(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

static inline int Measure_004a1b40(char* text)
{
    int width = 0;
    if (0 == text)
        return 0;
    if (!DAT_0051fba4->language)
        return FUN_004c1480(FUN_004c1440(), text);
    char* q = text;
    while (*q) {
        char ch = *q;
        Glyph_004a1b40* glyph = (Glyph_004a1b40*)FUN_004b7f30(
            DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (0 != glyph)
            width += glyph->width;
        q++;
    }
    return width;
}

static inline int LineHeight_004a1b40()
{
    if (0 == DAT_0051fba4->language)
        return FUN_004c1450();
    int ret0 = ((Glyph_004a1b40*)FUN_004b7f30(DAT_0051fba4->language->glyphs, 0x49))->height + 2;
    return ret0;
}

static inline int RowTop_004a1b40(Rect_004a1b40 rowRect) { return 2 + rowRect.top; }

static inline int RowBottom_004a1b40(Rect_004a1b40 rowRect) { return rowRect.bottom; }

// FUNCTION: 0x4a1b40
void __stdcall FUN_004a1b40(Class_004a1b40* obj, int index)
{
    unsigned char font;
    int yoff;
    unsigned int top;
    top = 0;
    int xx;
    Rect_004a1b40 bounds;
    int xw;
    int flag = 0;
    if (0 != obj->holder)
        obj->holder->field_14 = 1;
    Holder_004a1b40* holder = obj->holder;
    Cell_004a1b40* cellPtr;
    unsigned char v;
    Entry_004a1b40* entries;
    entries = obj->holder->entries;
    Entry_004a1b40* me = &entries[((int)index)];
    int h = me->h;
    if (me->type == 0) {
        bounds.left = 0;
    } else {
        bounds.left = me->x;
        top = me->y;
    }
    bounds.top = top;
    void* surface;
    unsigned int rowRight = bounds.left + me->w - 1;
    bounds.right = rowRight;
    bounds.bottom = (&entries[((int)index)])->h + bounds.top - 1;
    surface = holder->surface;
    if (surface == 0)
        surface = obj->fallback;
    if (!(0 == surface && !(holder->field_10 & 0x80))) { if (surface != 0)
        FUN_004c6d20(entries->surface, surface, &bounds, &bounds); } else { FUN_004b0230(obj, index, surface); }
    int lh = LineHeight_004a1b40();
    int step;
    if (!(me->field_da != 0)) step = (1 + lh); else step = me->field_da;
    unsigned int flags;
    flags = me->flags;
    Rect_004a1b40 rowRect;
    if ((flags & 0x10) && me->text && 0 != me->field_c0) {
        int i = 1;
        int t = 0;
        if (i <= entries->count) { do {
            if (7 == entries[i].type) {
                if (t == me->tab) {
                    FUN_004c1420((int)entries[i].field_d6);
                    break;
                }
                t++;
            }
            ++i;
        } while (entries->count >= i); }
        if (i > entries->count) { FUN_004c1420(DAT_0051fba4->current); }
        FUN_004c1440();
        font = FUN_004c13f0();
        char* q = FUN_004b6af0(me->text, me->field_bc);
        int line = 0;
        int y = me->field_bc;
        yoff = 0;
        while (1) {
            rowRect.left = 2 + bounds.left;
            rowRect.right = me->w + rowRect.left - 2;
            rowRect.top = bounds.top + yoff + 2;
            rowRect.bottom = rowRect.top + step;
            int w = Measure_004a1b40(q);
            int col = obj->colour[me->colours];
            if (!(me->field_d6 != 0 && me->field_d6[y] == 1)) {
                if (*q == '&') {
                    if (q[1] == 'G')
                        flag = 1;
                    q += 2;
                }
            } else {
                flag = 1;
            }
            int ty = rowRect.top;
            int f = me->flags;
            if ((f & 1) != 0) {
                xx = rowRect.left;
                xw = rowRect.right - rowRect.left + 1;
            } else if ((4 & f) != 0) {
                xw = w;
                xx = rowRect.right - w;
            } else if (2 & f) {
                xx = (rowRect.left + rowRect.right - ((int)w)) / 2;
                if (xx < ((int)rowRect.left))
                    xx = rowRect.left;
                xw = rowRect.right - ((int)xx) + 1;
            }
            if (me->field_da > 6 + LineHeight_004a1b40())
                FUN_004a51d0(entries->surface, q, xx, ty, xw, bounds.bottom - bounds.top, 0);
            else
                FUN_004a50e0(entries->surface, q, xx, (int)ty, xw, 0);
            q = FUN_004b6af0(q, 1);
            if (((int)flag)) {
                flag = 0;
                FUN_004bf4d0(entries->surface, &rowRect, -0x13);
                FUN_004bf4d0(entries->surface, &rowRect, -0x14);
                FUN_004bf4d0(entries->surface, &rowRect, -0x15);
                FUN_004bf4d0(entries->surface, &rowRect, -0x16);
            } else if (!(((me->flags & 0x100) != 0) == 0 && me->field_ba == line + me->field_bc && me->field_c0)) { FUN_004c13a0(col, font); } else {
                if (((unsigned int)obj->holder->field_20) == index)
                    FUN_004bf4d0(entries->surface, &rowRect, 0x1e);
                else
                    FUN_004bf4d0(entries->surface, &rowRect, 0x1e);
            }
            line += 1;
            yoff += step;
            ++y;
            h -= step;
            if (h >= lh) {
                if (!(line + me->field_bc >= me->field_c0)) {
                } else { return; }
            } else
                break;
        }
    } else { if (((unsigned int)flags) & 0xa0) {
        Item_004a1b40** colPtr;
        Rect_004a1b40 clip;
        unsigned int bp = (((unsigned int)flags) >> 7) & 1;
        void* surf = entries->surface;
        ((Class_004c6ae0*)surf)->FUN_004c6ae0(&clip);
        ((Class_004c6b10*)surf)->FUN_004c6b10(bounds);
        colPtr = 0;
        int k = me->field_bc;
        if (!bp)
            colPtr = &((Item_004a1b40**)me->field_c6)[k];
        else
            cellPtr = &((Cell_004a1b40*)me->field_c6)[k];
        int yy = bounds.top + 2;
        bounds.left += 2;
        int y = step + yy;
        for (; 1; ) {
            Cell_004a1b40* cell;
            if (0 != bp) {
                cell = cellPtr;
                cellPtr++;
            } else {
                cell = (*colPtr)->cell;
            }
            if (cell != 0 && cell->field_10 != 0) {
                            Quad_004a1b40 dst;
                            Quad_004a1b40 src;
                            dst.points[3].x = bounds.left;
                            src.points[0].x = 1;
                            src.points[0].y = 1;
                            src.points[3].x = 1;
                            src.points[1].y = 1;
                            dst.points[0].x = bounds.left;
                            dst.points[1].x = bounds.right;
                            dst.points[2].x = bounds.right;
                            dst.points[3].y = y - 1;
                            dst.points[2].y = y - 1;
                            src.points[1].x = cell->width - 1;
                            src.points[2].x = cell->width - 1;
                            dst.points[1].y = yy;
                            dst.points[0].y = yy;
                            src.points[2].y = cell->height - 1;
                            src.points[3].y = cell->height - 1;
                            FUN_004c7580(surf, cell, &dst, &src);
                            rowRect.left = dst.points[0].x;
                            rowRect.right = dst.points[1].x;
                            rowRect.top = dst.points[0].y;
                            rowRect.bottom = dst.points[2].y;
                            v = me->field_d6[k];
                            if (1 & v) FUN_004bf4d0(surf, &rowRect, -0x14); else if ((2 & v) != 0) {
                                FUN_004be950(surf, rowRect.left + 1, rowRect.bottom - 1, rowRect.right - 2, 1 + rowRect.top, obj->colour_8be);
                                FUN_004be950(surf, 2 + rowRect.left, rowRect.bottom - 1, rowRect.right - 1, rowRect.top + 1, obj->colour_8be);
                                FUN_004be950(surf, 1 + rowRect.left, rowRect.top + 2, rowRect.right - 1, rowRect.bottom - 2, obj->colour_8be);
                                FUN_004be950(surf, rowRect.left + 2, RowTop_004a1b40(rowRect), rowRect.right - 2, RowBottom_004a1b40(rowRect) - 2, obj->colour_8be);
                            }
                        }
            if (!(me->flags & 0x100) && me->field_ba == k) {
                        Rect_004a1b40 hl;
                        hl.left = bounds.left;
                        hl.top = yy;
                        hl.right = bounds.left + cell->width - 1;
                        hl.bottom = yy + cell->height - 1;
                        FUN_004bf4d0((void*)surf, &hl, 0x14);
                    }
            if (0 == bp) { colPtr++; }
            yy = yy + step;
            k += 1;
            y += step;
            if ((yy < bounds.bottom && k < me->field_c0) == 0)
                break;
        }
        ((Class_004c6b10*)surf)->FUN_004c6b10(clip);
    } }
}
