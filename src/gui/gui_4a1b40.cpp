// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, edited by claude-opus-5-5, finished by Space Bunny Free, rewritten by claude-opus-5-5, finished by Fledge Alpha Free, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #4997, 2026-10-03), from 97.5%.
// Draws one entry of a list gadget: the frame, then either the text rows
// (flags 0x10) or the cell rows (flags 0x20/0x80).
// The last two steps:
//  - The entry rectangle comes from FUN_004a1630 (src/gui/gui_4a1630.cpp),
//    defined below without an annotation and inlined (the matched sibling
//    0x4a4c90 inlines a rect helper of the same shape). That puts
//    bounds.right's `lea eax, [esi + eax - 1]`
//    (0x4a1bb9) in the original's operand order (97.5% -> 97.7%); the same
//    code written out gives `[eax + esi - 1]`.
//  - The text branch and the cell branch each declare their own Rect
//    (rowRect, cellRect); MSVC gives both the one frame slot at [esp+0x18].
//    With one function-scope rect, the text loop's me->w load
//    (`movsx eax, word ptr [edi+0x17]`, 0x4a1d35) stays below the
//    rowRect.left store, and the spill homes of me, step and xx come out
//    rotated (step, xx, me at 0x50..0x58 instead of me, step, xx).
//    97.7% -> MATCH.
// Both were found by deleting or changing one statement at a time and
// printing only where me, step and xx land and where the movsx sits. With one
// shared rect, merging the text and cell `y` into one function-scope int also
// fixes the frame order (99.8%), but not the movsx.
// Earlier fixes that are still load-bearing:
//  - Cell mode zeroes colPtr only in the bp (cell array) arm (0x4a20cd), and
//    the cell loop increments k before the colPtr step (0x4a2278).
//  - The tab scan runs to `entries->count + 1` in the loop test and in the
//    not-found test (0x4a1c89, 0x4a1cdb).
//  - The glyph width is the inlined Measure_004a1b40 helper its matched
//    siblings use (0x4a4660, 0x4a53c0); the "&G" test is
//    `field_d6 && field_d6[y] == 1` (0x4a1df9 and 0x4a1e03 both fall into it).
//  - The text loop's exit tests `h >= lh` first:
//    `if (h >= lh) { if (line + bc >= c0) return; } else break;`.
//  - The cell rect's stores go x pair then y pair (left, right, top, bottom).
//  - `#include <stdlib.h>` next to `<stdio.h>` (windows.h is worse).
// Known original quirks kept as they are (docs/bugs.md): a selected cell row
// reads cell->width/height even when the cell pointer is null (0x4a2233,
// 0x4a224c), and both arms of `holder->field_20 == index` draw with 0x1e.
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

struct Surface {
    void GetClipRect(Rect_004a1b40* rect);
};

struct Class_004c6b10 {
    void SetClipRect(Rect_004a1b40 rect);
};

#pragma pack(pop)

extern LanguageRoot_004a1b40* DAT_0051fba4;

void __stdcall FUN_004b0230(Class_004a1b40* obj, int index, void* bmp);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004a1b40* rect, Rect_004a1b40* pos);
void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
int __stdcall GetGafFrame(unsigned short* glyphs, int c);
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
char* __stdcall FUN_004b6af0(char* text, int line);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
void __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                            int rem, int style);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FadeRectangle(void* surface, Rect_004a1b40* rect, int id);
void __stdcall DrawFrameQuad(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

static inline int Measure_004a1b40(char* text)
{
    int width = 0;
    if (0 == text)
        return 0;
    if (!DAT_0051fba4->language)
        return GetTextWidth(GetFont(), text);
    char* q = text;
    while (*q) {
        char ch = *q;
        Glyph_004a1b40* glyph = (Glyph_004a1b40*)GetGafFrame(
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
        return GetFontHeight();
    return ((Glyph_004a1b40*)GetGafFrame(DAT_0051fba4->language->glyphs, 0x49))->height + 2;
}

void __stdcall FUN_004a1630(Entry_004a1b40* entry, Rect_004a1b40* rect)
{
    if (entry->type == 0) {
        rect->left = 0;
        rect->top = 0;
    } else {
        rect->left = entry->x;
        rect->top = entry->y;
    }
    rect->right = entry->w + rect->left - 1;
    rect->bottom = entry->h + rect->top - 1;
}

// FUNCTION: 0x4a1b40
void __stdcall FUN_004a1b40(Class_004a1b40* obj, int index)
{
    unsigned char font;
    int yoff;
    int xx;
    Rect_004a1b40 bounds;
    int xw;
    int flag = 0;
    if (0 != obj->holder)
        obj->holder->field_14 = 1;
    Holder_004a1b40* holder = obj->holder;
    Entry_004a1b40* entries;
    entries = obj->holder->entries;
    Entry_004a1b40* me = &entries[index];
    int h = me->h;
    void* surface;
    FUN_004a1630(&entries[index], &bounds);
    surface = holder->surface;
    if (surface == 0)
        surface = obj->fallback;
    if (surface == 0 && !(holder->field_10 & 0x80))
        FUN_004b0230(obj, index, surface);
    else if (surface != 0)
        CopySurfaceRect(entries->surface, surface, &bounds, &bounds);
    int lh = LineHeight_004a1b40();
    int step;
    if (me->field_da == 0)
        step = lh + 1;
    else
        step = me->field_da;
    unsigned int flags;
    flags = me->flags;
    if ((flags & 0x10) && me->text && 0 != me->field_c0) {
        Rect_004a1b40 rowRect;
        int i;
        int t = 0;
        for (i = 1; i < entries->count + 1; i++) {
            if (7 == entries[i].type) {
                if (t == me->tab) {
                    SetFont((int)entries[i].field_d6);
                    break;
                }
                t++;
            }
        }
        if (i == entries->count + 1) { SetFont(DAT_0051fba4->current); }
        GetFont();
        font = GetTextKeyColor();
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
            if (me->field_d6 == 0 || me->field_d6[y] != 1) {
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
                xx = (rowRect.left + rowRect.right - w) / 2;
                if (xx < rowRect.left)
                    xx = rowRect.left;
                xw = rowRect.right - xx + 1;
            }
            if (me->field_da > 6 + LineHeight_004a1b40())
                FUN_004a51d0(entries->surface, q, xx, ty, xw, bounds.bottom - bounds.top, 0);
            else
                FUN_004a50e0(entries->surface, q, xx, ty, xw, 0);
            q = FUN_004b6af0(q, 1);
            if (flag) {
                flag = 0;
                FadeRectangle(entries->surface, &rowRect, -0x13);
                FadeRectangle(entries->surface, &rowRect, -0x14);
                FadeRectangle(entries->surface, &rowRect, -0x15);
                FadeRectangle(entries->surface, &rowRect, -0x16);
            } else if (!(me->flags & 0x100) && me->field_ba == line + me->field_bc && me->field_c0) {
                if (obj->holder->field_20 == index)
                    FadeRectangle(entries->surface, &rowRect, 0x1e);
                else
                    FadeRectangle(entries->surface, &rowRect, 0x1e);
            } else {
                SetTextColors(col, font);
            }
            line += 1;
            yoff += step;
            ++y;
            h -= step;
            if (h >= lh) {
                if (line + me->field_bc >= me->field_c0)
                    return;
            } else {
                break;
            }
        }
    } else if (flags & 0xa0) {
        Item_004a1b40** colPtr;
        Cell_004a1b40* cellPtr;
        Rect_004a1b40 cellRect;
        Rect_004a1b40 clip;
        unsigned int bp = (flags >> 7) & 1;
        void* surf = entries->surface;
        ((Surface*)surf)->GetClipRect(&clip);
        ((Class_004c6b10*)surf)->SetClipRect(bounds);
        int k = me->field_bc;
        if (!bp) {
            colPtr = &((Item_004a1b40**)me->field_c6)[k];
        } else {
            colPtr = 0;
            cellPtr = &((Cell_004a1b40*)me->field_c6)[k];
        }
        int yy = bounds.top + 2;
        bounds.left += 2;
        int y = step + yy;
        for (;;) {
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
                DrawFrameQuad(surf, cell, &dst, &src);
                cellRect.left = dst.points[0].x;
                cellRect.right = dst.points[1].x;
                cellRect.top = dst.points[0].y;
                cellRect.bottom = dst.points[2].y;
                unsigned char v = me->field_d6[k];
                if (1 & v) {
                    FadeRectangle(surf, &cellRect, -0x14);
                } else if ((2 & v) != 0) {
                    DrawLine(surf, cellRect.left + 1, cellRect.bottom - 1, cellRect.right - 2, 1 + cellRect.top, obj->colour_8be);
                    DrawLine(surf, 2 + cellRect.left, cellRect.bottom - 1, cellRect.right - 1, cellRect.top + 1, obj->colour_8be);
                    DrawLine(surf, 1 + cellRect.left, cellRect.top + 2, cellRect.right - 1, cellRect.bottom - 2, obj->colour_8be);
                    DrawLine(surf, cellRect.left + 2, cellRect.top + 2, cellRect.right - 2, cellRect.bottom - 2, obj->colour_8be);
                }
            }
            // A selected row reads cell->width/height even when cell is null
            // (docs/bugs.md).
            if (!(me->flags & 0x100) && me->field_ba == k) {
                Rect_004a1b40 hl;
                hl.left = bounds.left;
                hl.top = yy;
                hl.right = bounds.left + cell->width - 1;
                hl.bottom = yy + cell->height - 1;
                FadeRectangle(surf, &hl, 0x14);
            }
            k++;
            if (!bp)
                colPtr++;
            yy += step;
            y += step;
            if (yy >= bounds.bottom || k >= me->field_c0)
                break;
        }
        ((Class_004c6b10*)surf)->SetClipRect(clip);
    }
}
