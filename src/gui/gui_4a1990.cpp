// Decompiled by Opus, deepseek-v4.1, deepseek-v4.1-flash, claude-opus-5-5,
// Space Bunny Free, Fledge Alpha Free, GPT-6, claude-sonnet-5-5,
// GPT-5.6-Terra, GPT-6.1-sol, Sonnet, GPT-6-Luna and LongCat 2.5 Preview Free.
// Names are provisional.

// The third gui module (0x4a1990 to 0x4a51d0): the list gadget's drawing,
// scrolling and input, the slider, the menu hit test, the list fillers and
// the small font and rectangle helpers. 0x4a3ef0 (DrawSlider) keeps its own
// file (src/gui/gui_4a3ef0.cpp): it only matches at that file's symbol count.

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <ddraw.h>
#include <string.h>
#include <iostream>
#include <math.h>

// g_guiContext is the GUI context; its +0x14 layer holds the glyph table
// (Language/Font/List in the individual file views).
struct List_004a32a0 {
    char unknown_0[0xc];
    union {
        unsigned short* glyphs;        // +0x0c
        unsigned short* field_0c;      // +0x0c
    };
};

struct Entry_004a32a0;

struct Holder_004a32a0 {
    int unknown_0;
    Entry_004a32a0* entries;           // +0x04
};

struct Root_004a32a0 {                 // g_guiContext
    union { int current; int group; }; // +0x00
    char unknown_04[0x14 - 0x04];
    union {
        List_004a32a0* language;       // +0x14
        List_004a32a0* font;           // +0x14
        List_004a32a0* list;           // +0x14
    };
    Holder_004a32a0* holder;           // +0x18
};

extern Root_004a32a0* g_guiContext;

#pragma pack(push, 1)
struct Entry_004a1990 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a1990
int __stdcall FUN_004a1990(Entry_004a1990* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 2 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
#pragma pack(push, 1)
struct Entry_004a19f0 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a19f0
int __stdcall FUN_004a19f0(Entry_004a19f0* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
#pragma pack(push, 1)
struct Entry_004a1a50 {
    unsigned char type;                // +0x0
    unsigned char id;                  // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x15b - 0xb8];
};
#pragma pack(pop)

// FUNCTION: 0x4a1a50
int __stdcall FUN_004a1a50(Entry_004a1a50* entries, int index)
{
    unsigned char id = entries[index].id;
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 3 && entries[i].id == id) {
            return i;
        }
    }
    return 0;
}
// Redraws the rectangle of GUI entry `index` on the list's surface (entry 0
// holds the surface at +0xbc); an entry of type 0 is placed at (0, 0).

struct Surface_004a1ab0;

#pragma pack(push, 1)
struct Entry_004a1ab0 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0x13 - 0x1];
    short x;                           // +0x13
    short y;                           // +0x15
    short width;                       // +0x17
    short height;                      // +0x19
    char unknown_1b[0xbc - 0x1b];
    Surface_004a1ab0* surface;         // +0xbc (only meaningful in entry 0)
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a1ab0 {
    char unknown_0[4];
    Entry_004a1ab0* entries;           // +0x4
};

struct Dialog_4a1ab0 {
    char unknown_0[0x18];
    Holder_004a1ab0* holder;           // +0x18
};

struct Rect_004a1ab0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

void __stdcall GrayRectangle(Surface_004a1ab0* dst, Rect_004a1ab0* rect);
void __stdcall FadeRectangle(Surface_004a1ab0* dst, Rect_004a1ab0* rect, int level);

// FUNCTION: 0x4a1ab0
void __stdcall RedrawGadgetRect(Dialog_4a1ab0* obj, int index)
{
    Entry_004a1ab0* entries = obj->holder->entries;
    Entry_004a1ab0* e = &entries[index];
    Rect_004a1ab0 rect;
    if (e->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = e->x;
        rect.top = e->y;
    }
    rect.right = e->width + rect.left - 1;
    rect.bottom = e->height + rect.top - 1;
    GrayRectangle(entries->surface, &rect);
    FadeRectangle(entries->surface, &rect, -0x14);
}
// Draws one entry of a list gadget: the frame, then either the text rows
// (flags 0x10) or the cell rows (flags 0x20/0x80).
//
// Known original quirks kept as they are (docs/bugs.md): a selected cell row
// reads cell->width/height even when the cell pointer is null (0x4a2233,
// 0x4a224c), and both arms of `holder->field_20 == index` draw with 0x1e.
// Needed next to <stdio.h>; <windows.h> instead is worse.

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

struct Dialog_4a1b40 {
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
    void SetClipRect(Rect_004a1b40 rect);
};

#pragma pack(pop)

void __stdcall DrawListboxFrame(Dialog_4a1b40* obj, int index, void* bmp);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004a1b40* rect, Rect_004a1b40* pos);
void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
char* __stdcall SkipTextLines(char* text, int line);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
void __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                            int rem, int style);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FadeRectangle(void* surface, Rect_004a1b40* rect, int id);
void __stdcall DrawFrameQuad(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

// Must stay an inlined helper: the glyph width is measured through it.
static inline int Measure_004a1b40(char* text)
{
    int width = 0;
    if (0 == text)
        return 0;
    if (!g_guiContext->language)
        return GetTextWidth(GetFont(), text);
    char* q = text;
    while (*q) {
        char ch = *q;
        Glyph_004a1b40* glyph = (Glyph_004a1b40*)GetGafFrame(
            g_guiContext->language->glyphs, (unsigned char)ch);
        if (0 != glyph)
            width += glyph->width;
        q++;
    }
    return width;
}

static inline int LineHeight_004a1b40()
{
    if (0 == g_guiContext->language)
        return GetFontHeight();
    return ((Glyph_004a1b40*)GetGafFrame(g_guiContext->language->glyphs, 0x49))->height + 2;
}

// Defined unannotated and inlined: sets the operand order of the bounds sums.
void __stdcall GetGadgetRect(Entry_004a1b40* entry, Rect_004a1b40* rect)
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
void __stdcall DrawListBox(Dialog_4a1b40* obj, int index)
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
    GetGadgetRect(&entries[index], &bounds);
    surface = holder->surface;
    if (surface == 0)
        surface = obj->fallback;
    if (surface == 0 && !(holder->field_10 & 0x80))
        DrawListboxFrame(obj, index, surface);
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
        // Separate from cellRect: one shared rect changes the spill homes.
        Rect_004a1b40 rowRect;
        int i;
        int t = 0;
        // Loop test and not-found test both use count + 1.
        for (i = 1; i < entries->count + 1; i++) {
            if (7 == entries[i].type) {
                if (t == me->tab) {
                    SetFont((int)entries[i].field_d6);
                    break;
                }
                t++;
            }
        }
        if (i == entries->count + 1) { SetFont(g_guiContext->current); }
        GetFont();
        font = GetTextKeyColor();
        char* q = SkipTextLines(me->text, me->field_bc);
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
            q = SkipTextLines(q, 1);
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
            // Tests h >= lh first.
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
        ((Surface*)surf)->SetClipRect(bounds);
        int k = me->field_bc;
        if (!bp) {
            colPtr = &((Item_004a1b40**)me->field_c6)[k];
        } else {
            // colPtr is zeroed only in this arm.
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
                // Stores go x pair, then y pair.
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
            // k increments before the colPtr step.
            k++;
            if (!bp)
                colPtr++;
            yy += step;
            y += step;
            if (yy >= bounds.bottom || k >= me->field_c0)
                break;
        }
        ((Surface*)surf)->SetClipRect(clip);
    }
}
#pragma pack(push, 1)
struct Entry_004a23b0 {
    char unknown_0[0x13];
    short x;                    // +0x13
    short y;                    // +0x15
    short w;                    // +0x17
    short h;                    // +0x19
    unsigned char flags;        // +0x1b
    char unknown_1c[0x140 - 0x1c];
    short off;                  // +0x140
    short size;                 // +0x142
    char unknown_144[0x15b - 0x144];
};
#pragma pack(pop)

// FUNCTION: 0x4a23b0
void __stdcall FUN_004a23b0(Entry_004a23b0* base, int index, int* r1, int* r2)
{
    Entry_004a23b0* e = base + index;
    r1[0] = e->x;
    r1[1] = e->y;
    r1[2] = r1[0] + e->w;
    r1[3] = r1[1] + e->h;
    if (e->flags & 1) {
        int left = r1[0] + e->off + 1;
        r2[0] = left;
        r2[1] = r1[1] + 1;
        r2[2] = left + e->size;
        r2[3] = r2[1] + e->h - 2;
    } else {
        r2[0] = r1[0] + 1;
        r2[1] = r1[1] + e->off + 2;
        r2[2] = r2[0] + e->w - 2;
        r2[3] = r2[1] + e->size;
    }
}
// Draws one gadget entry's three glyphs (start, repeated middle, end) across
// the span [x, x + w] on the surface of the entry table. Same entry table as
// 0x4a23b0 (fields x/y/w/h at 0x13..0x19) and 0x4a0f30 (holder at +0x18).
// Needed only for compiler state: changes how the loop test's sum is formed.

struct Glyph_004a2480 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

#pragma pack(push, 1)
struct Entry_004a2480 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    char unknown_1b[0xbc - 0x1b];
    void* surface;                     // +0xbc
    char unknown_c0[0x13a - 0xc0];
    unsigned short* glyphs;            // +0x13a
    char unknown_13e[0x15b - 0x13e];
};
#pragma pack(pop)

struct Holder_004a2480 {
    char unknown_0[4];
    Entry_004a2480* entries;           // +0x4
};

#pragma pack(push, 1)
struct Class_004a2480 {
    char unknown_0[0x18];
    Holder_004a2480* holder;           // +0x18
};
#pragma pack(pop)

char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* image, int x, int y);

// FUNCTION: 0x4a2480
void __stdcall FUN_004a2480(Class_004a2480* param_1, int index)
{
    Entry_004a2480* base = param_1->holder->entries;
    void* surface = base->surface;
    Entry_004a2480* e = &base[index];
    unsigned short* glyphs = e->glyphs;
    Glyph_004a2480* glyph = (Glyph_004a2480*)GetGafFrame(glyphs, 0);
    int x = e->x;
    int y = e->y + e->h / 2;
    if (glyph != 0)
        y -= glyph->height / 2;
    else
        y = e->y;
    int limit = e->x + e->w;
    if (glyph != 0)
        DrawFrame(surface, glyph, x, y);
    x += glyph->width;
    Glyph_004a2480* mid = (Glyph_004a2480*)GetGafFrame(glyphs, 1);
    if (x + mid->width < limit) {
        do {
            DrawFrame(surface, mid, x, y);
            x += mid->width;
        } while (x + mid->width < limit);
    }
    Glyph_004a2480* last = (Glyph_004a2480*)GetGafFrame(glyphs, 2);
    DrawFrame(surface, last, limit - last->width, y);
}
#pragma pack(push, 1)
struct Glyph_004a2580 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Entry_004a2580 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    unsigned char flags;               // +0x1b
    char unknown_1c[0x28 - 0x1c];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        struct {
            short count;               // +0xb6 (entry 0)
            char head_pad[0xbc - 0xb8];
            void* surface;             // +0xbc (entry 0)
            char head_tail[0x13c - 0xc0];
        } head;
        char text[0x13c - 0xb6];       // +0xb6
    } u;
    int field_13c;                     // +0x13c
    short off;                         // +0x140
    short size;                        // +0x142
    char unknown_144[0x14a - 0x144];
    int field_14a;                     // +0x14a
    unsigned short* glyphs;            // +0x14e
    unsigned char field_152;           // +0x152
    char unknown_153[0x157 - 0x153];
    int field_157;                     // +0x157
};
#pragma pack(pop)

struct Holder_004a2580 {
    char unknown_0[4];
    Entry_004a2580* entries;           // +0x04
};

struct Object_004a2580 {
    char unknown_0[0x18];
    Holder_004a2580* holder;           // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char field_8b2;           // +0x8b2
    char unknown_8b3[0x8c1 - 0x8b3];
    unsigned char field_8c1;           // +0x8c1
    char unknown_8c2[0x8c3 - 0x8c2];
    unsigned char field_8c3;           // +0x8c3
    char unknown_8c4[0x8c6 - 0x8c4];
    unsigned char field_8c6;           // +0x8c6
};

struct Font_004a2580 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Dialog_4a2580 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a2580* font;               // +0x14
};

void __stdcall SetFont(int id);
void __stdcall FUN_004a23b0(Entry_004a2580* base, int index, int* r1, int* r2);
void __stdcall DrawRaisedBox(void* surface, int* r, int a, int b, int c);
void __stdcall DrawSunkenBox(void* surface, int* r, int a, int b, int c);
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* glyph, int x, int y);
int GetTextKeyColor();
void __stdcall SetTextColors(int a, int b);
int GetFont();
void __stdcall GetTextWidth(Font_004a2580* font, char* text);
int GetFontHeight();
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxw);
void __stdcall GrayRectangle(void* surface, void* rect);
void __stdcall FadeRectangle(void* surface, void* rect, int a);

static inline void* Surface_004a2580(Object_004a2580* o)
{
    return o->holder->entries->u.head.surface;
}

#include <setjmp.h>

// FUNCTION: 0x4a2580
void __stdcall FUN_004a2580(Object_004a2580* obj, int index)
{
    Entry_004a2580* entries = obj->holder->entries;
    Entry_004a2580* e = &entries[index];
    void* surface = Surface_004a2580(obj);
    // One glyph pointer for every fetch, and one limit in the w<h arm: frame slot order.
    Glyph_004a2580* g;

    int n = 0;
    int i = 1;
    for (; i < entries->u.head.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                SetFont(*(int*)((char*)&entries[i] + 0xd6));
                break;
            }
            n++;
        }
    }
    if (i == entries->u.head.count + 1)
        SetFont(g_guiContext->group);

    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    unsigned short* gl = e->glyphs;
    if (gl == 0) {
        DrawRaisedBox(surface, r1, obj->field_8b2, obj->field_8c3, obj->field_8c6);
        DrawSunkenBox(surface, r2, obj->field_8b2, obj->field_8c3, obj->field_8c6);
    } else {
        short w = e->w;
        short h = e->h;
        if (w < h) {
            void* surf = Surface_004a2580(obj);
            int y = e->y;
            int x = e->x;
            int limit = y + e->h - 1;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152);
            if (g != 0)
                DrawFrame(surf, g, x, y);
            y += g->height;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 1);
            while (y + g->height <= limit) {
                DrawFrame(surf, g, x, y);
                y += g->height;
            }
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 2);
            DrawFrame(surf, g, x, limit - g->height + 1);
            x += g->width / 2;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 3);
            x -= g->width / 2;
            int ybase = e->off + e->y + 3;
            // The clamps use the <windows.h> min() macro, pulled in by <ddraw.h>.
            int lc = min(e->h - 6, e->size);
            limit = lc + ybase - 1;
            int t = e->h + e->y - 4;
            limit = min(limit, t);
            if (ybase > limit - lc + 1)
                ybase = limit - lc + 1;
            DrawFrame(surf, g, x, ybase);
            lc -= g->height;
            ybase += g->height;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 4);
            while (ybase <= limit - g->height) {
                DrawFrame(surf, g, x, ybase);
                ybase += g->height;
                lc -= g->height;
            }
            DrawFrame(surf, g, x, limit - g->height);
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 5);
            DrawFrame(surf, g, x, limit - g->height + 1);
        } else {
            void* surf = Surface_004a2580(obj);
            int x = e->x;
            int y = e->y;
            int limit = x + e->w - 1;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152);
            if (g != 0)
                DrawFrame(surf, g, x, y);
            x += g->width;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 1);
            while (x + g->width <= limit) {
                DrawFrame(surf, g, x, y);
                x += g->width;
            }
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 2);
            DrawFrame(surf, g, limit - g->width + 1, y);
            y += g->height / 2;
            g = (Glyph_004a2580*)GetGafFrame(e->glyphs, e->field_152 + 3);
            y -= g->height / 2;
            int a = e->off + e->x + 3;
            a = min(a, limit - g->width - 2);
            DrawFrame(surf, g, a, y);
        }
    }

    if (e->flags & 4) {
        int cur = GetTextKeyColor();
        SetTextColors(obj->field_8c1, cur);
        char buf[0x10];
        // Original bug, kept as it is: the copy at 0x4a2a26 (strlen with repne
        // scasb, then rep movsd/rep movsb) is unbounded and the source field runs
        // from entry+0xb6 to the end of the 0x15b-byte entry, so a label longer
        // than 15 characters runs off the 0x10-byte buffer into r1, r2 and the
        // saved registers.
        if (e->u.text[0] != 0) {
            strcpy(buf, e->u.text);
        } else if (e->field_13c != 0) {
            int num = (int)((float)e->off * e->field_13c / (e->w - e->size));
            _itoa(num, buf, 10);
        } else if (e->flags & 8) {
            _itoa(e->off + 1, buf, 10);
        } else {
            _itoa(e->off, buf, 10);
        }
        char* text = buf;
        int total = 0;
        if (text != 0) {
            if (g_guiContext->font == 0) {
                GetTextWidth((Font_004a2580*)GetFont(), text);
            } else {
                char* p = text;
                for (; *p != 0; p++) {
                    unsigned char c = *p;
                    g = (Glyph_004a2580*)GetGafFrame((unsigned short*)g_guiContext->font->glyphs, c);
                    if (g != 0)
                        total += g->width;
                }
            }
        }
        if (g_guiContext->font == 0)
            GetFontHeight();
        else
            GetGafFrame((unsigned short*)g_guiContext->font->glyphs, 0x49);
        DrawString(surface, buf, e->x + e->w + 2, e->y + 4, -1);
    }

    if ((e->flags & 0x10) || e->field_157 != 0) {
        int rect[4];
        if (e->type == 0) {
            rect[0] = 0;
            rect[1] = 0;
        } else {
            rect[0] = e->x;
            rect[1] = e->y;
        }
        rect[2] = e->w + rect[0] - 1;
        rect[3] = e->h + rect[1] - 1;
        GrayRectangle(entries->u.head.surface, rect);
        FadeRectangle(entries->u.head.surface, rect, -0x14);
    }
}
#pragma pack(push, 1)
struct Entry_004a2be0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x19 - 0x01];      // +0x01
    short field_19;                    // +0x19
    int field_1b;                      // +0x1b (dword)
    char unknown_1f[0xb6 - 0x1f];      // +0x1f
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];      // +0xb8
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    char unknown_c6[0xd6 - 0xc6];      // +0xc6
    int id;                            // +0xd6
    char unknown_d8[0xda - 0xd8];      // +0xd8
    short field_da;                    // +0xda
    char unknown_dc[0x136 - 0xdc];     // +0xdc
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];   // +0x138
    short field_140;                   // +0x140
    char unknown_142[0x15b - 0x142];   // +0x142
};
#pragma pack(pop)

struct Holder_004a2be0 {
    int current;                       // +0x00
    Entry_004a2be0* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    void* list;                        // +0x14
};

struct Dialog_4a2be0 {
    char unknown_0[0x18];
    Holder_004a2be0* holder;           // +0x18
};

void __stdcall DrawListBox(Dialog_4a2be0* param_1, int param_2);
void __stdcall FUN_004a2580(Dialog_4a2be0* param_1, int param_2);
void __stdcall DrawTextInput(Dialog_4a2be0* param_1, int param_2);
char* __stdcall SkipTextLines(char* text, int line);

// FUNCTION: 0x4a2be0
void __stdcall FUN_004a2be0(Dialog_4a2be0* param_1, int param_2)
{
    Entry_004a2be0* entries = param_1->holder->entries;
    int i = 1;
    char* me = (char*)entries + param_2 * 0x15b;
    int type = *(unsigned char*)me;
    int field_1b = *(int*)(me + 0x1b);
    for (; i < (short)entries->count + 1; i++) {
        // Built fresh inside the loop body: strength reduction then rotates the preheader.
        char* entry = (char*)entries + i * 0x15b + 0x140;
        if (i != param_2) {
            if (entry[-0x13f] == me[1]) {
                switch ((unsigned char)entry[-0x140]) {
                case 2:
                    if (type == 2) {
                        *(short*)(entry - 0x84) = *(short*)(me + 0xbc);
                        *(short*)(entry - 0x86) = *(short*)(me + 0xba);
                        DrawListBox(param_1, i);
                    } else if (type == 4) {
                        int esi_val;
                        if (*(unsigned char*)(entry - 0x125) & 0x20) {
                            esi_val = *(short*)(me + 0x136) / (*(short*)(entry - 0x82) + 1);
                        } else {
                            esi_val = 0;
                        }
                        short rows = *(short*)(entry - 0x66);
                        if (rows != 0) {
                            int edx_val = *(short*)(entry - 0x80) -
                                *(short*)(entry - 0x127) / rows;
                            int eax_val = *(short*)(me + 0x140) + esi_val;
                            int result = (int)((float)edx_val * eax_val /
                                (*(short*)(me + 0x136) - 1));
                            *(short*)(entry - 0x84) = result;
                        }
                        DrawListBox(param_1, i);
                    }
                    break;
                case 3:
                    if (type == 2 && field_1b & 8) {
                        char* line = SkipTextLines(*(char**)(me + 0xc2), *(short*)(me + 0xba));
                        strcpy(entry - 0x8a, line);
                        DrawTextInput(param_1, i);
                    }
                    break;
                case 4:
                    if (type == 2) {
                        if (*(short*)(me + 0xc0) > 1) {
                            int result;
                            if (*(short*)(me + 0xbe) != 0) {
                                short scale = *(short*)(me + 0xbc);
                                short height = *(short*)(entry - 0xa);
                                short count = *(short*)(me + 0xbe);
                                // Split into a named ratio: fixes the x87 operand-staging order.
                                float ratio = (float)scale * height;
                                result = (int)(ratio / count);
                            } else {
                                result = 0;
                            }
                            if (*(short*)entry != result) {
                                *(short*)entry = result;
                                FUN_004a2580(param_1, i);
                            }
                        }
                    }
                    break;
                }
            }
        }
    }
}
// FindKind returns zero on a miss, so the rescale then uses entry zero.
// <iostream> and <math.h> are needed: they restore the shared floating-point tail.

#pragma pack(push, 1)
struct Entry_004a2e40 { // 0x15b bytes
    unsigned char type; // +0x00
    unsigned char kind; // +0x01
    char name[0x10];    // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19; // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group; // +0x28
    char unknown_29[0xb6 - 0x29];
    short count; // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba; // +0xba
    short field_bc; // +0xbc
    short field_be; // +0xbe
    char unknown_c0[0xd6 - 0xc0];
    int id; // +0xd6
    char unknown_da[0x136 - 0xda];
    short field_136; // +0x136
    char unknown_138[0x140 - 0x138];
    short field_140; // +0x140
    char unknown_142[0x15b - 0x142];
};
#pragma pack(pop)

struct List_004a2e40 {
    char unknown_0[0x0c];
    unsigned short* glyphs; // +0x0c
};

struct Holder_004a2e40 {
    int current;             // +0x00
    Entry_004a2e40* entries; // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a2e40* list; // +0x14
};

#pragma pack(push, 1)
struct Dialog_4a2e40 {
    char unknown_00[0x18];
    Holder_004a2e40* holder; // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca; // +0xcca
};
#pragma pack(pop)

void __stdcall SetFont(int id);
char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFontHeight();

static inline int FindEntry(Entry_004a2e40* entries, char* name) {
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

static inline int FindKind(Entry_004a2e40* entries, unsigned char kind) {
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// FUNCTION: 0x4a2e40
void __stdcall FUN_004a2e40(Dialog_4a2e40* param_1, char* param_2, int param_3) {
    Entry_004a2e40* entries = param_1->holder->entries;
    int found = FindEntry(entries, param_2);
    if (found == -1)
        return;

    Entry_004a2e40* me = &entries[found];
    me->field_ba = param_3;

    int n = 0;
    int i;
    for (i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1)
        SetFont(g_guiContext->current);

    int size;
    if (g_guiContext->list == 0)
        size = GetFontHeight();
    else
        size = *(unsigned short*)(GetGafFrame(g_guiContext->list->glyphs, 0x49) + 2) + 2;
    int step = (me->field_19 - 2) / (size + 1);
    short last = me->field_bc;
    short sel = me->field_ba;
    if (sel > step + last - 1 || sel < last) {
        if (me->field_be != 0)
            me->field_bc = sel;
        if (me->field_bc > me->field_be)
            me->field_bc = me->field_be;
        Entry_004a2e40* peer = &entries[FindEntry(entries, param_2)];
        unsigned char pkind = peer->kind;
        Entry_004a2e40* e3 = &entries[FindKind(entries, pkind)];
        // Keep the float conversions: the original uses integer-memory FPU multiply/divide.
        float q = (float)e3->field_136 * me->field_bc / me->field_be;
        if ((float)e3->field_140 != q)
            e3->field_140 = (short)q;
    }
    param_1->field_cca = 1;
}
// Refreshes GUI entry `index` (0x15b-byte entries, entry 0 holds the count at
// +0xb6): clears the two words at +0xba/+0xbc, selects the entry of type 7
// whose group number matches this entry's group and makes its id current, then
// quantises this entry's height (+0x19) down to a multiple of the current font
// line step and stamps the current time into +0xb6.

#pragma pack(push, 1)
struct Entry_004a30c0 {                // 0x15b bytes
    unsigned char type;               // +0x00
    char unknown_01[0x19 - 0x01];
    short height;                     // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                       // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                      // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                   // +0xba
    short field_bc;                   // +0xbc
    char unknown_be[0xd6 - 0xbe];
    int id;                           // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct Holder_004a30c0 {
    char unknown_0[4];
    Entry_004a30c0* entries;           // +0x04
};

struct Class_004a30c0 {
    char unknown_0[0x18];
    Holder_004a30c0* holder;           // +0x18
};

struct Font_004a30c0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Dialog_4a30c0 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a30c0* font;               // +0x14
};

void __stdcall SetFont(int id);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
int __cdecl GetTicks();
// FUNCTION: 0x4a30c0
void __stdcall FUN_004a30c0(Class_004a30c0* obj, int index)
{
    Entry_004a30c0* entries = obj->holder->entries;
    Entry_004a30c0* e = &entries[index];
    e->field_bc = 0;
    e->field_ba = 0;
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        SetFont(g_guiContext->group);
    }
    int step;
    if (g_guiContext->font == 0) {
        step = GetFontHeight();
    } else {
        step = *(unsigned short*)((char*)GetGafFrame(g_guiContext->font->glyphs, 0x49) + 2) + 2;
    }
    int h = e->height;
    e->height = h - h % (step + 2);
    *(int*)&e->count = GetTicks();
}
void* __cdecl FUN_004d83b0(const char* name, unsigned int size);
char* __stdcall SkipTextLines(char* text, int n);
void __stdcall TruncateTextWithEllipsis(int a1, char* text, int a3, int a4, int a5);

// Builds a block of NUL-terminated item strings from the text blob at a3
// (one name per line) and copies it back over a3. Each line is truncated to
// 100 bytes, expanded by TruncateTextWithEllipsis (marker glyphs) and appended in place.
// FUNCTION: 0x4a31c0
void __stdcall FUN_004a31c0(int a1, int a2, char* a3, int a4)
{
    char* items = (char*)FUN_004d83b0("SCROLLITEMS", *(int*)(a3 - 0x44));
    char* out = items;
    for (int i = 0; i < a4; i++) {
        char buf[100];
        strncpy(buf, SkipTextLines(a3, i), 100);
        TruncateTextWithEllipsis(a1, buf, a2, -1, 0);
        strcpy(out, buf);
        out += strlen(buf);
        *out = 0;
        out++;
    }
    memcpy(a3, items, out - items);
}
// The missing-name path calls the fatal-error routine FatalError, which
// exits the process. Its following null-entry accesses are unreachable.

#pragma pack(push, 1)
struct Entry_004a32a0 { // 0x15b bytes
    unsigned char type; // +0x00
    unsigned char kind; // +0x01, matched by the type-4 search
    char name[0x10];    // +0x02
    char unknown_12[0x19 - 0x12];
    short height; // +0x19, the rows are fitted into this
    int flags;    // +0x1b
    char unknown_1f[0x29 - 0x1f];
    unsigned char f_29; // +0x29, gates the whole second half
    char unknown_2a[0xb6 - 0x2a];
    short count; // +0xb6, entry 0 holds the entry count
    char unknown_b8[0xba - 0xb8];
    short f_ba;  // +0xba
    short f_bc;  // +0xbc
    short first; // +0xbe, first row that still fits
    short num;   // +0xc0, the row count
    int bitmap;  // +0xc2
    char unknown_c6[0xd6 - 0xc6];
    int id;     // +0xd6
    short f_da; // +0xda, the line height
    char unknown_dc[0x15b - 0xdc];
};
#pragma pack(pop)

struct Glyph_004a32a0 {
    unsigned short width;
    unsigned short height; // +0x02
};

struct Dialog_4a32a0 {
    char unknown_00[0x18];
    Holder_004a32a0* holder; // +0x18
};

void __stdcall FatalError(char* msg);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall FUN_004a03f0(Root_004a32a0* menu, int index, int value);
// Keeps its own file, src/gui/gui_4a3ef0.cpp: DrawSlider only
// matches at that file's symbol count.
void __stdcall DrawSlider(Root_004a32a0* param_1, int param_2);

// The line height of one row: the default font height, or the height of the
// glyph for 'I' plus two. Written out three times in the caller because the
// original evaluates it again in the second arm of the +0xda minimum.
static inline int FontHeight_004a32a0() {
    if (g_guiContext->language == 0)
        return GetFontHeight();
    return (int)((Glyph_004a32a0*)GetGafFrame(g_guiContext->language->glyphs, 0x49))->height + 2;
}

// The entry search of 0x4a0180, 0x4a0200, 0x4a0280 and 0x4a35a0.
static inline int FindName_004a32a0(Entry_004a32a0* entries, char* name) {
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// The type-4 entry search; returns 0 when nothing matches.
static inline int FindType_004a32a0(Entry_004a32a0* entries, unsigned char kind) {
    for (int i = 1; i < entries->count + 1; i++) {
        if (entries[i].type == 4 && entries[i].kind == kind)
            return i;
    }
    return 0;
}

// FUNCTION: 0x4a32a0
void __stdcall FUN_004a32a0(Dialog_4a32a0* param_1, char* name, int bitmap, int count, int flag) {
    Holder_004a32a0* holder = param_1->holder;
    Entry_004a32a0* entries = holder->entries;
    int index = FindName_004a32a0(entries, name);
    Entry_004a32a0* me;
    if (index != -1) {
        me = &entries[index];
    } else {
        FatalError("Error in GUI layout");
        me = 0;
    }
    me->num = (short)count;
    me->bitmap = bitmap;
    me->flags |= 0x10;
    me->f_da =
        (short)((me->f_da > FontHeight_004a32a0() + 1) ? (int)me->f_da : FontHeight_004a32a0() + 1);
    if (flag != 0) {
        me->id = flag;
        me->flags |= 0x800;
    }
    me->f_bc = 0;
    me->f_ba = 0;
    // Initialised before the me->first store: sets the final stack-store order.
    int remain = me->height;
    me->first = (short)(count - 1);
    int step;
    if (me->f_da == 0) {
        step = FontHeight_004a32a0() + 1;
    } else {
        step = me->f_da;
    }
    for (int j = count - 1; j > -1; j--) {
        remain -= step;
        if (remain < 0)
            break;
        me->first = (short)j;
    }
    if (me->f_29 == 0)
        return;
    int i2 = FindName_004a32a0(holder->entries, name);
    Entry_004a32a0* list = holder->entries;
    unsigned char kind = list[i2].kind;
    int found = FindType_004a32a0(list, kind);
    if (found == -1)
        return;
    char* text = (char*)&holder->entries[found].name;
    // Captured in a local before the name search, and passed on to FUN_004a03f0.
    Root_004a32a0* root = g_guiContext;
    if (root->holder != 0) {
        int j2 = FindName_004a32a0(root->holder->entries, text);
        if (j2 != -1) {
            FUN_004a03f0(root, j2, remain < 0);
        }
    }
    if (remain < 0) {
        // A fresh global lookup, not root.
        DrawSlider(g_guiContext, found);
    }
}
// Finds the GUI layout entry named `name` (0x15b-byte entries whose entry 0
// stores the entry count as a short at +0xb6) and gives it the row array
// `rows` and its count: +0xc6 gets the array, +0xc0 the count, flags +0x1b
// gets bit 7, and +0xbe the index of the topmost row that still fits in the
// entry's height (+0x19) when the row heights are summed from the bottom.
// Each row is 0x18 bytes with an unsigned short height at +0x2; when the
// entry's +0xda is non-zero it overrides the row height with its own value.
// A missing entry is reported and then treated as a null entry (the stores
// that follow then go through a null pointer, as in the original).

#pragma pack(push, 1)
struct Entry_004a35a0 {                // 0x15b-byte GUI entry
    char unknown_0[0x2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short height;                      // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0 holds the entry count)
    char unknown_b8[0xba - 0xb8];
    short f_ba;                        // +0xba
    short f_bc;                        // +0xbc
    short first;                       // +0xbe
    short num;                         // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int f_c6;                          // +0xc6
    char unknown_ca[0xda - 0xca];
    short f_da;                        // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Row_004a35a0 {
    char unknown_0[2];
    unsigned short height;             // +0x2
    char unknown_4[0x18 - 4];
};

struct Table_004a35a0 {
    int unknown_0;
    Entry_004a35a0* entries;           // +0x4
};
#pragma pack(pop)

void __stdcall FatalError(const char* msg);

static inline int FindEntry(Entry_004a35a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// FUNCTION: 0x4a35a0
void __stdcall SetGadgetRows(Table_004a35a0* table, char* name,
                            Row_004a35a0* rows, int count)
{
    Entry_004a35a0* entries = table->entries;
    int index = FindEntry(entries, name);
    Entry_004a35a0* e;
    if (index != -1) {
        e = &entries[index];
    } else {
        FatalError("Error in GUI layout");
        e = 0;
    }
    e->f_c6 = (int)rows;
    // Computed right after the f_c6 store and before the num store.
    Row_004a35a0* row = rows + count - 1;
    e->num = (short)count;
    e->flags |= 0x80;
    e->f_bc = 0;
    e->f_ba = 0;
    e->first = (short)(count - 1);
    int remain = e->height;
    // The test is i > -1, not i >= 0.
    for (int i = count - 1; i > -1; i--, row--) {
        // An if/else with one remain -= per arm, not one remain -= cond ? a : b.
        if (e->f_da != 0) remain -= e->f_da; else remain -= row->height;
        if (remain < 0)
            break;
        e->first = (short)i;
    }
}
// Finds the GUI layout entry whose name matches `name`. The entry table holds
// 0x15b-byte entries whose entry 0 stores the entry count as a short at +0xb6;
// entries are searched from 1. A missing entry is reported and then treated as
// a null entry. The caller supplies an array of item pointers and its count:
// the entry gets the array, the count, and the index of the first item that
// still fits in the entry's field +0x19 when the item heights are summed from
// the bottom of the array. Item heights are the unsigned short at offset +2 of
// the object found through each item's field +0x28.

#pragma pack(push, 1)
struct Entry_004a36a0 {                // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                   // +0x02
    char unknown_12[0x19 - 0x12];
    short field_19;                    // +0x19
    int flags_1b;                      // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    short count;                       // +0xb6 (entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char unknown_c2[0xc6 - 0xc2];
    int field_c6;                      // +0xc6
    char unknown_ca[0x15b - 0xca];
};
#pragma pack(pop)

struct Table_004a36a0 {
    char unknown_0[4];
    Entry_004a36a0* entries;           // +0x4
};

struct Item_004a36a0 {
    char unknown_0[0x28];
    int field_28;                      // +0x28
};

void __stdcall FatalError(char* path);

static inline int FindEntry(Entry_004a36a0* entries, char* name)
{
    for (int i = 1; i < entries->count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4a36a0
void __stdcall SetGadgetItems(Table_004a36a0* table, char* name, int* items, int count)
{
    Entry_004a36a0* entries = table->entries;
    int i = FindEntry(entries, name);
    Entry_004a36a0* e;
    if (i != -1) {
        e = &entries[i];
    } else {
        FatalError("Error in GUI layout");
        e = 0;
    }
    // field_c0 is stored before field_c6: keeps the tail's store order.
    e->field_c0 = (short)count;
    e->field_c6 = (int)items;
    int v = e->field_19;
    e->field_bc = 0;
    e->field_ba = 0;
    e->flags_1b |= 0x20;
    int* p = &items[count - 1];
    e->field_be = (short)(count - 1);
    // Decrements stay in the body with no for-increment, and the test is j > -1.
    for (int j = count - 1; j > -1; ) {
        Item_004a36a0* item = (Item_004a36a0*)*p;
        unsigned short w = *(unsigned short*)(item->field_28 + 2);
        v -= w;
        if (v < 0) {
            break;
        }
        e->field_be = (short)j;
        j--;
        p--;
    }
}
#pragma pack(push, 1)
struct Entry_004a3780 {                // 0x15b bytes
    unsigned char type;                // +0x00
    unsigned char kind;                // +0x01
    char unknown_02[0x13 - 0x02];
    short field_13;                    // +0x13
    short field_15;                    // +0x15
    short field_17;                    // +0x17
    short field_19;                    // +0x19
    int flags;                         // +0x1b
    char unknown_1f[0x28 - 0x1f];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0)
        int field_b6;                  // +0xb6 (the scroll repeat timer)
    };
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    void* field_c6;                    // +0xc6
    char unknown_ca[0xce - 0xca];
    void (__stdcall* field_ce)(void*, void*);  // +0xce
    char unknown_d2[0xd6 - 0xd2];
    int field_d6;                      // +0xd6
    short field_da;                    // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct List_004a3780 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a3780 {
    int current;                       // +0x00
    Entry_004a3780* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a3780* list;               // +0x14
    char unknown_18[0x20 - 0x18];
    int field_20;                      // +0x20
};

struct Rect_004a3780 { int x0, y0, x1, y1; };

struct Point_004a3780 {                // 0x18 bytes, copied with rep movsd x6
    int x;                             // +0x00
    int y;                             // +0x04
    int unknown_08[4];                 // +0x08
};

struct Object_004a3780 {
    char unknown_00[0x18];
    Holder_004a3780* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a3780 point;              // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                      // +0x60
    int focus;                         // +0x64
    char unknown_68[0xcca - 0x68];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

extern char DAT_00502a20[];

void __stdcall SetFont(int id);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
char* __stdcall SkipTextLines(char* text, int n);
int __cdecl GetTicks();
int __stdcall IsDoubleClickMessage(Object_004a3780* obj, unsigned char buttons);
int __stdcall IsMouseButtonMessage(Object_004a3780* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Object_004a3780* obj, unsigned int mask);
void __stdcall SetClickMode(Object_004a3780* obj, int param_2);
void __stdcall FUN_0049fc50(Object_004a3780* obj, int index);
void __stdcall DrawListBox(Object_004a3780* obj, int index);
void __stdcall FUN_004a2be0(Object_004a3780* obj, int index);

struct Row_004a3780 {
    short unknown_0;
    unsigned short height;             // +0x02
    char unknown_4[0x18 - 0x4];
};

struct Item_004a3780 {
    char unknown_0[0x28];
    Row_004a3780* row;                 // +0x28
};

void __stdcall GetGadgetRect(Entry_004a3780* entry, Rect_004a3780* rect)
{
    if (entry->type == 0) {
        rect->x0 = 0;
        rect->y0 = 0;
    } else {
        rect->x0 = entry->field_13;
        rect->y0 = entry->field_15;
    }
    rect->x1 = entry->field_17 + rect->x0 - 1;
    rect->y1 = entry->field_19 + rect->y0 - 1;
}

struct Glyph_004a3780 {
    unsigned short width;              // +0x00
    unsigned short height;             // +0x02
};

static inline int LineHeight_004a3780()
{
    if (0 == g_guiContext->list)
        return GetFontHeight();
    return ((Glyph_004a3780*)GetGafFrame(g_guiContext->list->field_0c, 0x49))->height + 2;
}

// FUNCTION: 0x4a3780
int __stdcall HandleListBoxInput(Object_004a3780* obj, int index, int param_3)
{
    if (obj->field_60 != -1)
        return 0;
    Entry_004a3780* entries = obj->holder->entries;
    Entry_004a3780* me = &entries[index];
    int orig_sel = me->field_ba;
    if (me->field_c0 == 0)
        return 0;
    Rect_004a3780 r;
    unsigned int flags;
    // Called on &entries[index], not me: with me the y1 lea operands swap.
    GetGadgetRect(&entries[index], &r);
    int n = 0;
    r.y0 += 2;
    r.y1 -= 3;
    Point_004a3780 point = obj->point;
    point.x -= entries[0].field_13;
    point.y -= entries[0].field_15;
    int i;
    for (i = 1; i < entries[0].count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                SetFont(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == entries[0].count + 1)
        SetFont(g_guiContext->current);

    int size = LineHeight_004a3780();
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    // The SkipTextLines results go through this s.
    char* s;

    if (IsDoubleClickMessage(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            // skip0 sits at the end of the in-rect block: reloads point.x on this edge only.
            if (me->field_c0 == 0) goto skip0;
            if (!(me->flags & 0x200))
                goto ret1;
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba < 0)
                goto above;
            if (me->field_ba - me->field_bc > step - 1)
                me->field_ba = me->field_bc + step - 1;
            if (me->field_ba >= me->field_c0 - 1)
                me->field_ba = me->field_c0 - 1;
            s = SkipTextLines(me->field_c2, me->field_ba);
            if (strncmp(DAT_00502a20, s, 2) != 0)
                goto ret1;
            me->field_ba = orig_sel;
            return 0;
skip0:;
        }
    } else if (IsMouseButtonMessage(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            SetClickMode(obj, 1);
        }
    } else if (IsMouseButtonMessage(obj, 2)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            SetClickMode(obj, 2);
        }
    }

    if (obj->focus != index)
        goto end;
    if (!HasMouseKeyFlags(obj, 3))
        obj->focus = -1;
    if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
        obj->holder->field_20 = index;
        flags = me->flags;
        if (flags & 0x10) {
            // Stores straight into me->field_ba and tests the field; clamps written plainly.
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba >= 0) {
                if (me->field_ba - me->field_bc > step - 1)
                    me->field_ba = me->field_bc + step - 1;
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    s = SkipTextLines(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
                    if (entries[i].type == 2 && entries[i].kind == me->kind)
                        // min() for the sync clamp.
                        entries[i].field_ba = min(entries[i].field_c0 - 1, me->field_ba);
                }
            } else {
                me->field_ba = orig_sel;
            }
        } else if (flags & 0x20 | 0x80) {
            // Original bug, kept: `(flags & 0x20) | 0x80` is always true
            // (and ecx,0x20 / or cl,0x80 / test cl,cl at 0x4a3c29).
            int flag8 = (flags >> 7) & 1;
            Row_004a3780* fixed;
            Item_004a3780** ip;
            if (flag8)
                fixed = &((Row_004a3780*)me->field_c6)[me->field_bc];
            else
                ip = &((Item_004a3780**)me->field_c6)[me->field_bc];
            // k is declared after the pointer choice, not before the if (flag8).
            int k = me->field_bc;
            // remain stays declared before n2.
            int remain = point.y - r.y0 - 2;
            int n2 = 0;
            for (;;) {
                Row_004a3780* row = flag8 ? fixed : (*ip)->row;
                if (me->field_da != 0)
                    remain -= span;
                else
                    remain -= row->height;
                if (remain <= 0) {
                    me->field_ba = n2 + me->field_bc;
                    break;
                }
                if (flag8)
                    fixed++;
                else
                    ip++;
                n2++;
                k++;
                if (k > me->field_c0 - 1)
                    break;
            }
        }
        if (orig_sel != me->field_ba) {
            DrawListBox(obj, index);
            if (me->field_ce)
                me->field_ce(obj, me);
        }
        if (me->flags & 0x40) {
            // Two labels in this order: the return block's live-range split depends on it.
above:
ret1:
            return 1;
        }
        obj->field_cca = 1;
    } else if (point.y < r.y0) {
        if (me->field_bc > 0 && me->field_b6 < GetTicks()) {
            me->field_b6 = GetTicks() + 2;
            if (me->field_ba > me->field_bc)
                me->field_ba = me->field_bc;
            me->field_bc--;
            me->field_ba--;
            short sel = me->field_ba;
            if (me->field_c2 != 0) {
                s = SkipTextLines(me->field_c2, sel < 0 ? 0 : sel);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
            goto finish;
        }
        if (me->field_ba > 0) {
            me->field_ba = 0;
            goto finish;
        }
    } else if (point.y > r.y1) {
        if (me->field_bc < me->field_be && me->field_b6 < GetTicks()) {
            me->field_b6 = GetTicks() + 2;
            me->field_bc++;
            me->field_ba = me->field_bc + step - 1;
            if (me->field_c2 != 0) {
                s = SkipTextLines(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
            // Shared by both scroll tails via goto: MSVC 5 does not merge return blocks.
finish:
            DrawListBox(obj, index);
            FUN_004a2be0(obj, index);
        }
    }
end:
    return obj->field_60 != -1;
}
// Clears three fields of GUI entry i (0x15b-byte entries). The stores sit in
// an inline helper taking the entry pointer; a local pointer to the entry
// loads the entries pointer before the index multiply instead of after it.

#pragma pack(push, 1)
struct Entry_004a3eb0 {
    char unknown_0[0x140];
    short field_140;                   // +0x140
    char unknown_142[2];
    int field_144;                     // +0x144
    char unknown_148[2];
    int field_14a;                     // +0x14a
    char unknown_14e[0x15b - 0x14e];
};
#pragma pack(pop)

struct Table_004a3eb0 {
    char unknown_0[4];
    Entry_004a3eb0* entries;           // +0x4
};

struct Dialog_4a3eb0 {
    char unknown_0[0x18];
    Table_004a3eb0* table;             // +0x18
};

static inline void ClearEntry(Entry_004a3eb0* e)
{
    e->field_140 = 0;
    e->field_144 = 0;
    e->field_14a = 0;
}

// FUNCTION: 0x4a3eb0
void __stdcall FUN_004a3eb0(Dialog_4a3eb0* obj, int i)
{
    ClearEntry(&obj->table->entries[i]);
}
// The list gadget's scroll
// bar thumb: while the entry has the focus, either drag the offset (+0x140)
// with the mouse or step it by one when the mouse is outside the thumb, then
// clamp it to 0..field_136-1 and, if it changed, mark the holder dirty,
// redraw (FUN_004a2580, FUN_004a2be0) and call the entry's callback. Without
// the focus, a left or right press inside the gadget takes the focus, and a
// press on the thumb starts a drag.

#pragma pack(push, 1)
struct Entry_004a4170 {                // 0x15b bytes, the table of 0x4a23b0
    char unknown_00[0x13];
    short x1;                          // +0x13
    short y1;                          // +0x15
    char unknown_17[0x1b - 0x17];
    unsigned char flags;               // +0x1b, bit 1 = vertical, 0x10 = dead
    char unknown_1c[0x136 - 0x1c];
    short field_136;                   // +0x136, largest usable offset
    char unknown_138[0x140 - 0x138];
    short off;                         // +0x140, the scroll offset
    char unknown_142[0x144 - 0x142];
    int (__stdcall *cb)(void*, int);   // +0x144, called when off changed
    char unknown_148[0x14a - 0x148];
    int field_14a;                     // +0x14a, cb's second argument
    char unknown_14e[0x157 - 0x14e];
    int field_157;                     // +0x157, entry is being dragged
};
#pragma pack(pop)

struct Holder_004a4170 {
    char unknown_00[4];
    Entry_004a4170* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14, set when off changed
};

struct Point_004a4170 {                // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Object_004a4170 {
    char unknown_00[0x18];
    Holder_004a4170* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4170 point;              // +0x3c, the mouse, table relative
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64, -1 when nothing has the focus
    char unknown_68[0x78 - 0x68];
    int field_78;                      // +0x78, non-zero while dragging
    Point_004a4170 saved;              // +0x7c, the mouse when the drag began
    short field_94;                    // +0x94, off when the drag began
};

void __stdcall FUN_0049fc50(Object_004a4170* obj, int index);
void __stdcall FUN_004a23b0(Entry_004a4170* base, int index, int* r1, int* r2);
void __stdcall FUN_004a2580(Object_004a4170* obj, int index);
void __stdcall FUN_004a2be0(Object_004a4170* obj, int index);
int __stdcall IsMouseButtonMessage(Object_004a4170* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Object_004a4170* obj, unsigned int mask);
void __stdcall SetClickMode(Object_004a4170* obj, int param_2);

static inline void OffsetChanged_004a4170(Object_004a4170* obj, int index, Entry_004a4170* e, int old)
{
    if (e->off > e->field_136 - 1)
        e->off = e->field_136 - 1;
    if (e->off < 0)
        e->off = 0;
    if (e->off == old)
        return;
    if (obj->holder)
        obj->holder->field_14 = 1;
    FUN_004a2580(obj, index);
    FUN_004a2be0(obj, index);
    if (e->cb)
        e->cb(obj, e->field_14a);
}

// FUNCTION: 0x4a4170
void __stdcall HandleSliderInput(Object_004a4170* obj, int index)
{
    Entry_004a4170* entries = obj->holder->entries;
    Entry_004a4170* e = &entries[index];
    if (e->flags & 0x10)
        return;
    if (e->field_157)
        return;

    Point_004a4170 p = obj->point;
    p.x -= entries->x1;
    p.y -= entries->y1;
    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    if (obj->focus == index) {
        if (!HasMouseKeyFlags(obj, 3)) {
            obj->focus = -1;
            obj->field_78 = 0;
        }
        // Inline OffsetChanged ends both arms: the duplicated zero uses keep 0 in EDX.
        if (obj->field_78) {
            int old = e->off;
            if (e->flags & 1)
                e->off = obj->field_94 - obj->saved.x + p.x;
            else
                e->off = obj->field_94 - obj->saved.y + p.y;
            OffsetChanged_004a4170(obj, index, e, old);
        } else {
            // Per-path stores to e->off (not one after the if/else): keeps the 16-bit loads.
            int old = e->off;
            if (e->flags & 1) {
                if (p.x < r2[0])
                    e->off--;
                else if (p.x > r2[2])
                    e->off++;
            } else {
                if (p.y < r2[1])
                    e->off--;
                else if (p.y > r2[3])
                    e->off++;
            }
            OffsetChanged_004a4170(obj, index, e, old);
        }
        return;
    }

    if (obj->field_78)
        return;
    if (IsMouseButtonMessage(obj, 1)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        SetClickMode(obj, 1);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
        return;
    }
    if (IsMouseButtonMessage(obj, 2)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        SetClickMode(obj, 2);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
    }
}
int __cdecl tolower(int);
int __cdecl toupper(int);

#pragma pack(push, 1)
struct Entry_0049fc50 {                // 0x15b bytes
    unsigned char type;                // +0x0
    char unknown_1[0x13 - 0x1];
    short x1;                          // +0x13
    short y1;                          // +0x15
    short x2;                          // +0x17
    short y2;                          // +0x19
    unsigned char flags;               // +0x1b
    char unknown_2[0xb6 - 0x1c];
    char text[0x15b - 0xb6];           // +0xb6
};
#pragma pack(pop)

struct Holder_0049fc50 {
    int unknown_0;
    Entry_0049fc50* entries;           // +0x4
};

struct Point_0049fc50 {
    int x;
    int y;
    int unknown_8[4];
};

#pragma pack(push, 1)
struct Object_0049fc50 {
    char unknown_0[0x18];
    Holder_0049fc50* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_0049fc50 point;              // +0x3c to +0x50
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64
    char unknown_68[0xcc6 - 0x68];
    int field_cc6;                     // +0xcc6
};
#pragma pack(pop)

int __stdcall FUN_0049fc50(Object_0049fc50* obj, int index);
int __stdcall IsMouseButtonMessage(Object_0049fc50* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Object_0049fc50* obj, unsigned int mask);
void __stdcall SetClickMode(Object_0049fc50* obj, int param_2);
int PopKey(void);
int __stdcall IsKeyDown(int key);

// FUNCTION: 0x4a4440
int __stdcall FUN_004a4440(Object_0049fc50* obj, int index, char key)
{
    Entry_0049fc50* entries = obj->holder->entries;
    Entry_0049fc50* entry = &entries[index];
    struct Rect_0049fc50 { int x1, y1, x2, y2; } rect;
    Point_0049fc50 point;
    int rel_x, rel_y;

    if (entry->text[0x147 - 0xb6] == 0 && (entry->flags & 0x10))
        return 0;

    if (entry->type == 0) {
        rect.x1 = 0;
        rect.y1 = 0;
    } else {
        rect.x1 = entry->x1;
        rect.y1 = entry->y1;
    }
    rect.x2 = entry->x2 + rect.x1 - 1;
    rect.y2 = entry->y2 + rect.y1 - 1;

    memcpy(&point, &obj->point, 24);

    rel_x = point.x - entries->x1;
    rel_y = point.y - entries->y1;

    if (IsMouseButtonMessage(obj, 1)) {
        if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
            goto fail;
        FUN_0049fc50(obj, index);
        SetClickMode(obj, 1);
    } else if (IsMouseButtonMessage(obj, 2)) {
        if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
            goto fail;
        FUN_0049fc50(obj, index);
        SetClickMode(obj, 2);
    }

fail:
    if (obj->focus != index)
        goto check_queue;
    if (HasMouseKeyFlags(obj, 3))
        goto check_queue;
    obj->focus = -1;
    if (rel_x < rect.x1 || rel_x > rect.x2 || rel_y < rect.y1 || rel_y > rect.y2)
        goto check_queue;
    return 1;

check_queue:
    if (obj->focus != -1) {
        Entry_0049fc50* e = &entries[obj->focus];
        if (e->type == 3 && !IsKeyDown(0xfb))
            return 0;
    }

final_check:
    if (obj->field_cc6 != 1 || *(int*)&key == 0)
        return 0;
    if ((char)tolower(entry->text[0x147 - 0xb6]) != key &&
        (char)toupper(entry->text[0x147 - 0xb6]) != key)
        return 0;
    PopKey();
    return 1;
}
// Sets entry i's end time to now plus its duration.

#pragma pack(push, 1)
struct Entry_004a4620 {
    char unknown_0[0xc2];
    unsigned int duration;             // +0xc2
    unsigned int end;                  // +0xc6
    char unknown_ca[0x15b - 0xca];
};
#pragma pack(pop)

struct Table_004a4620 {
    char unknown_0[4];
    Entry_004a4620* entries;           // +0x4
};

struct Dialog_4a4620 {
    char unknown_0[0x18];
    Table_004a4620* table;             // +0x18
};

int __cdecl GetTicks();

#include <time.h>

// FUNCTION: 0x4a4620
void __stdcall FUN_004a4620(Dialog_4a4620* obj, int i)
{
    Entry_004a4620* e = &obj->table->entries[i];
    e->end = GetTicks() + e->duration;
}
#pragma pack(push, 1)
struct Entry_004a4660 {
    unsigned char type;
    char unknown_01[0x13 - 1];
    short x;
    short y;
    short w;
    short h;
    char unknown_1b[0x1f - 0x1b];
    int color1;
    int color2;
    char unknown_27[0xba - 0x27];
    int number;
    char unknown_be[0xd2 - 0xbe];
    int showText;
    char unknown_d6[0x15b - 0xd6];
};
#pragma pack(pop)

struct Holder_004a4660 {
    char unknown_0[4];
    Entry_004a4660 *entries;
};

struct Dialog_4a4660 {
    char unknown_0[0xc];
    void *surface;
    int unknown_10;
    void *oldSurface;
    Holder_004a4660 *holder;
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char color1;
    char unknown_8b3[0x8c3 - 0x8b3];
    unsigned char color2;
    char unknown_8c4[2];
    unsigned char color3;
    char unknown_8c7[0xcd2 - 0x8c7];
    void *fallbackSurface;
};

struct Rect_004a4660 { int left, top, right, bottom; };
struct Glyph_004a4660 { unsigned short width, height; };
struct Language_004a4660 { char unknown_0[0xc]; unsigned short *glyphs; };
struct LanguageRoot_004a4660 { char unknown_0[0x14]; Language_004a4660 *language; };

void __stdcall LockScreen(void *);
void __stdcall FillBevelBox(void *, Rect_004a4660 *, unsigned int, unsigned int, unsigned int);
void __stdcall FillRectangle(void *, Rect_004a4660 *, int);
void __stdcall FUN_004a50e0(void *, char *, int, int, int, int);
char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int, char *);
int GetFontHeight();
void __stdcall UnlockScreen(void *);

// Kept as a separate static inline: its zero is hoisted into edi at the prologue.
static inline int Measure_004a4660(char *text)
{
    int width = 0;
    char *p = text;
    if (p == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    char *q = text;
    while (*q != 0) {
        char ch = *q;
        Glyph_004a4660 *glyph = (Glyph_004a4660 *)GetGafFrame(
            g_guiContext->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4660
void __stdcall FUN_004a4660(Dialog_4a4660 *obj, int index)
{
    Entry_004a4660 *entries = obj->holder->entries;
    Entry_004a4660 *entry = (Entry_004a4660 *)((char *)entries + index * 0x15b);
    obj->oldSurface = obj->surface;

    void *surface = *(void **)((char *)entries + 0xbc);
    if (surface == 0)
        surface = *(void **)((char *)obj + 0xcd2);
    LockScreen(surface);

    Rect_004a4660 rect;
    rect.left = entry->x;
    rect.top = entry->y;
    rect.right = entry->w + entry->x;
    rect.bottom = entry->h + entry->y;
    FillBevelBox(surface, &rect, obj->color1, obj->color2, obj->color3);

    rect.left += 2;
    rect.top += 2;
    rect.right -= 2;
    rect.bottom -= 2;
    FillRectangle(surface, &rect, *(int *)((char *)entry + 0x23));

    float scale = (float)*(int *)((char *)entry + 0xba) / *(int *)((char *)entry + 0xb6);
    rect.right = (int)(scale * (entry->w - 4)) + rect.left;
    FillRectangle(surface, &rect, *(int *)((char *)entry + 0x1f));

    if (entry->showText != 0) {
        char text[20];
        _itoa(entry->number, text, 10);
        int width = Measure_004a4660(text);
        int height;
        if (g_guiContext->language == 0) {
            height = GetFontHeight();
        } else {
            Glyph_004a4660 *glyph = (Glyph_004a4660 *)GetGafFrame(
                g_guiContext->language->glyphs, 0x49);
            height = glyph->height + 2;
        }
        FUN_004a50e0(surface, text,
            (entry->w / 2 - width / 2) + entry->x,
            (entry->h / 2 - height / 2) + entry->y, -1, 0);
    }

    obj->oldSurface = *(void **)((char *)obj + 8);
    UnlockScreen(surface);
}
// Advances GUI entry i's animated value by its step each time its interval
// elapses, until it reaches its maximum, then redraws the entry.
// Needs a header (any of <windows.h>, <stdio.h>, ...: tools/headers.py) for
// the table pointer to be loaded between the steps of the index multiply.

#pragma pack(push, 1)
struct Entry_004a4890 {
    char unknown_0[0xba];
    int value;                         // +0xba
    int max;                           // +0xbe
    int interval;                      // +0xc2
    int next;                          // +0xc6
    float step;                        // +0xca
    int active;                        // +0xce
    char unknown_d2[0x15b - 0xd2];
};
#pragma pack(pop)

struct Table_004a4890 {
    char unknown_0[4];
    Entry_004a4890* entries;           // +0x4
};

struct Dialog_4a4890 {
    char unknown_0[0x18];
    Table_004a4890* table;             // +0x18
};

int __cdecl GetTicks();
void __stdcall FUN_004a4660(Dialog_4a4890* obj, int i);

// FUNCTION: 0x4a4890
void __stdcall FUN_004a4890(Dialog_4a4890* obj, int i)
{
    Entry_004a4890* e = &obj->table->entries[i];
    if (e->active && e->value < e->max) {
        if (GetTicks() > e->next) {
            e->value += (int)e->step;
            if (e->value > e->max) {
                e->value = e->max;
                e->active = 0;
            }
            e->next = GetTicks() + e->interval;
        }
        FUN_004a4660(obj, i);
    }
}
struct ElemArray_4a4930 {
    char unknown_0[4];
    char* base;               // +4
};

struct GameState_4a4930 {
    char unknown_0[0x18];
    ElemArray_4a4930* arr;    // +0x18
};

// FUNCTION: 0x4a4930
void __stdcall FUN_004a4930(GameState_4a4930* param_1, int index)
{
    char* e = param_1->arr->base + index * 0x15b;
    *(int*)(e + 0xb6) = 0;
    *(int*)(e + 0xbe) = 0;
    *(int*)(e + 0xc2) = 0;
    *(short*)(e + 0xc6) = 0;
}
// Draws one gadget entry: builds the entry's bounding rect and a destination
// quad, then either blits a texture (field_be via GetGafFrame, or field_c2)
// onto it, or fills the rect with the colour at obj+0x8b9.

#pragma pack(push, 1)
struct Entry_004a4980 {
    unsigned char type;               // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                          // +0x13
    short y;                          // +0x15
    short w;                          // +0x17
    short h;                          // +0x19
    char unknown_1b[0xbc - 0x1b];
    union {
        void* surface;                // +0xbc (entry 0 only)
        char unknown_bc[0xc2 - 0xbc]; // +0xbc to +0xc1
    };
    void* field_c2;                   // +0xc2
    short field_c6;                   // +0xc6
    char unknown_c8[0x15b - 0xc8];
};
#pragma pack(pop)

struct Holder_004a4980 {
    char unknown_0[4];
    Entry_004a4980* entries;           // +0x04
};

#pragma pack(push, 1)
struct Dialog_4a4980 {
    char unknown_0[0x18];
    Holder_004a4980* holder;           // +0x18
    char unknown_1c[0x8b9 - 0x1c];
    unsigned char field_8b9;           // +0x8b9
};
#pragma pack(pop)

struct Point_004a4980 {
    int x;
    int y;
};

struct Quad_004a4980 {
    Point_004a4980 p[4];
};

struct Rect_004a4980 {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct Frame_004a4980 {
    unsigned short w;                 // +0x00
    unsigned short h;                 // +0x02
    short field_4;                    // +0x04
    short field_6;                    // +0x06
    char unknown_8;                   // +0x08
    unsigned char field_9;            // +0x09
};

char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* frame, int x, int y);
void __stdcall FillRectangle(void* surface, Rect_004a4980* rect, int color);
void __stdcall DrawFrameQuad(void* surf, void* entry, Quad_004a4980* dst, Quad_004a4980* src);

// FUNCTION: 0x4a4980
void __stdcall FUN_004a4980(Dialog_4a4980* obj, int index)
{
    Entry_004a4980* entries = obj->holder->entries;
    Entry_004a4980* e = &entries[index];

    Rect_004a4980 rect;
    if (e->type == 0) {
        rect.x1 = 0;
        rect.y1 = 0;
    } else {
        rect.x1 = e->x;
        rect.y1 = e->y;
    }
    rect.x2 = e->w + rect.x1 - 1;
    rect.y2 = e->h + rect.y1 - 1;

    Quad_004a4980 dst;
    dst.p[0].x = rect.x1;
    dst.p[3].x = rect.x1;
    dst.p[0].y = rect.y1;
    dst.p[1].x = rect.x2;
    dst.p[1].y = rect.y1;
    dst.p[2].x = rect.x2;
    dst.p[2].y = rect.y2;
    dst.p[3].y = rect.y2;

    Quad_004a4980 src;
    src.p[0].x = 1;
    src.p[0].y = 1;
    src.p[3].x = 1;
    src.p[1].y = 1;

    void* field_be = *(void**)((char*)e + 0xbe);
    if (field_be != 0) {
        Frame_004a4980* result = (Frame_004a4980*)GetGafFrame(field_be, e->field_c6);
        if (result != 0) {
            src.p[1].x = result->w - 1;
            src.p[2].x = result->w - 1;
            src.p[2].y = result->h - 1;
            src.p[3].y = result->h - 1;
            if (result->field_9 == 0) {
                DrawFrameQuad(*(void**)((char*)entries + 0xbc), result, &dst, &src);
                return;
            }
            DrawFrame(*(void**)((char*)entries + 0xbc), result, result->field_4 + rect.x1, result->field_6 + rect.y1);
            return;
        }
    } else if (e->field_c2 != 0) {
        src.p[1].x = ((Frame_004a4980*)e->field_c2)->w - 1;
        src.p[2].x = ((Frame_004a4980*)e->field_c2)->w - 1;
        src.p[2].y = ((Frame_004a4980*)e->field_c2)->h - 1;
        src.p[3].y = ((Frame_004a4980*)e->field_c2)->h - 1;
        DrawFrameQuad(*(void**)((char*)entries + 0xbc), e->field_c2, &dst, &src);
    } else {
        FillRectangle(*(void**)((char*)entries + 0xbc), &rect, obj->field_8b9);
    }
}
// GUI hit test for menu entry `index` (0x15b-byte entries in the object's table
// at +0x18 -> +4). The entry's rectangle comes from its header (x,y,width,
// height; a type-0 header uses origin 0,0), the entry may have a callback at
// +0xb6, and bit 0 of +0xc8 enables mouse handling. A left click (or a right
// click when there is no left) selects the entry and stores 1/2 via
// SetClickMode; when the entry was already focused and HasMouseKeyFlags says no
// button of mask 3 is down, focus is cleared. Returns 1 when the click landed
// inside the rectangle of the entry whose focus was just cleared.
#pragma pack(push, 1)

struct Dialog_4a4b50;
struct Point_004a4b50 { int x, y; };

struct Entry_004a4b50 {                       // 0x15b bytes
    unsigned char type;                       // +0x0
    char unknown_1[0x13 - 0x1];
    short x;                                  // +0x13
    short y;                                  // +0x15
    short width;                              // +0x17
    short height;                             // +0x19
    char unknown_1b[0xb6 - 0x1b];
    void (__stdcall* callback)(Dialog_4a4b50*, Entry_004a4b50*);          // +0xb6
    char unknown_ba[0xc8 - 0xba];
    unsigned char flags;                      // +0xc8
    char unknown_c9[0x15b - 0xc9];
};

struct Table_004a4b50 {
    char unknown_0[4];
    Entry_004a4b50* entries;                  // +0x4
};

struct Dialog_4a4b50 {
    char unknown_0[0x18];
    Table_004a4b50* table;                    // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4b50 pos;                       // +0x3c
    char unknown_44[0x64 - 0x44];
    int focus;                                // +0x64
};
#pragma pack(pop)

extern int __stdcall IsMouseButtonMessage(Dialog_4a4b50* obj, unsigned char buttons);
extern int __stdcall HasMouseKeyFlags(Dialog_4a4b50* obj, unsigned int mask);
extern void __stdcall SetClickMode(Dialog_4a4b50* obj, int value);
extern int __stdcall FUN_0049fc50(Dialog_4a4b50* obj, int index);

// FUNCTION: 0x4a4b50
int __stdcall FUN_004a4b50(Dialog_4a4b50* obj, int index)
{
    Entry_004a4b50* entries = obj->table->entries;
    // Entries are indexed as entries[index], not through a stored pointer.
    // 16-byte stack struct: gives the frame its size.
    struct Rect { int left, top, right, bottom; } r;
    if (entries[index].type == 0) {
        r.left = 0;
        r.top = 0;
    } else {
        r.left = entries[index].x;
        r.top = entries[index].y;
    }
    r.right = entries[index].width - 1 + r.left;
    r.bottom = entries[index].height - 1 + r.top;
    if (entries[index].callback)
        entries[index].callback(obj, &entries[index]);
    if (entries[index].flags & 1) {
        if (IsMouseButtonMessage(obj, 1)) {
            // Position copied into a local Point before each hit test.
            Point_004a4b50 p = obj->pos;
            if (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom) {
                FUN_0049fc50(obj, index);
                SetClickMode(obj, 1);
            }
        } else if (IsMouseButtonMessage(obj, 2)) {
            Point_004a4b50 p = obj->pos;
            if (p.x >= r.left && p.x <= r.right && p.y >= r.top && p.y <= r.bottom) {
                FUN_0049fc50(obj, index);
                SetClickMode(obj, 2);
            }
        }
        if (obj->focus == index && !HasMouseKeyFlags(obj, 3)) {
            obj->focus = -1;
            // Read through a pointer so the two loads stay after the focus store.
            Point_004a4b50* pp = &obj->pos;
            int py2 = pp->y;
            int px2 = pp->x;
            if (px2 >= r.left && px2 <= r.right && py2 >= r.top && py2 <= r.bottom)
                return 1;
        }
    }
    return 0;
}
// Draws one side of a GUI entry's rectangle when bit 0 of param_3 is set; the
// side is chosen by bits 0/1/2 of the entry's flags and the colour comes from
// the index `(int)obj + 0x8b2` into the entry's colour table at +0x1f.

#pragma pack(push, 1)
struct Entry_004a4c90 {                // 0x15b bytes
    char type;                         // +0x00
    char unknown_1[0x12];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int flags;                         // +0x1b
    unsigned char* colours;            // +0x1f
    char unknown_23[0xbc - 0x23];
    void* surface;                     // +0xbc
    char unknown_c0[0x15b - 0xc0];
};
#pragma pack(pop)

struct Holder_004a4c90 {
    char unknown_0[4];
    Entry_004a4c90* entries;           // +0x4
};

struct Dialog_4a4c90 {
    char unknown_0[0x18];
    Holder_004a4c90* holder;           // +0x18
};

struct Rect_004a4c90 {
    int x1, y1, x2, y2;
};

// The colour parameter must be int, not unsigned char: forces a zero-extending load.
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int color);

static inline void FillRect_004a4c90(Entry_004a4c90* e, Rect_004a4c90* r)
{
    if (e->type == 0) {
        r->x1 = 0;
        r->y1 = 0;
    } else {
        r->x1 = e->x;
        r->y1 = e->y;
    }
    r->x2 = e->w - 1 + r->x1;
    r->y2 = e->h - 1 + r->y1;
}

// FUNCTION: 0x4a4c90
void __stdcall FUN_004a4c90(Dialog_4a4c90* obj, int index, unsigned char param_3)
{
    Entry_004a4c90* entries = obj->holder->entries;
    Entry_004a4c90* e = (Entry_004a4c90*)((char*)entries + index * 0x15b);
    Rect_004a4c90 rect;
    FillRect_004a4c90(e, &rect);
    if (param_3 & 1) {
        if (e->flags & 1)
            DrawLine(entries->surface, rect.x1, rect.y1, rect.x2, rect.y1,
                         e->colours[(int)obj + 0x8b2]);
        else if (e->flags & 2)
            DrawLine(entries->surface, rect.x1, rect.y1, rect.x1, rect.y2,
                         e->colours[(int)obj + 0x8b2]);
        else if (e->flags & 4)
            DrawLine(entries->surface, rect.x1, rect.y1, rect.x2, rect.y2,
                         e->colours[(int)obj + 0x8b2]);
    }
}
// Draws one list-gadget
// entry: makes the entry's language current, fills or blits its rectangle,
// draws its text, and when the entry has the focus draws the text cursor (a
// vertical line) after the text up to the cursor position.
//
// The +0x1f field is an int colour index into the object's colour table at
// +0x8b2 (`obj->colours[entry->colours]`). The byte saved, zeroed and restored
// around the width measurement is `text[obj->cursor]`, the character at the
// cursor, so the measured width is that of the text before the cursor.

#pragma pack(push, 1)
struct Entry_004a4d70 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    unsigned char align;               // +0x1b
    char unknown_1c[0x1f - 0x1c];
    int colours;                       // +0x1f, index into Class::colours
    char unknown_23[0x28 - 0x23];
    char tab;                          // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xbc - 0xb6];        // +0xb6
    } b6;
    void* surface;                     // +0xbc
    char unknown_c0[0xd6 - 0xc0];
    int language;                      // +0xd6
    char unknown_da[0x15b - 0xda];
};

struct List_004a4d70 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct Holder_004a4d70 {
    int current;                       // +0x00
    Entry_004a4d70* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a4d70* language;           // +0x14
    char unknown_18[0x24 - 0x18];
    void* surface;                     // +0x24
};

struct Dialog_4a4d70 {
    char unknown_00[0x18];
    Holder_004a4d70* holder;           // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
    char unknown_68[0x74 - 0x68];
    int cursor;                        // +0x74
    char unknown_78[0x8b2 - 0x78];
    unsigned char colours[0xcd2 - 0x8b2]; // +0x8b2
    void* fallback;                    // +0xcd2
};

struct Rect_004a4d70 { int left, top, right, bottom; };
struct Glyph_004a4d70 { unsigned short width, height; };

struct LanguageRoot_004a4d70 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a4d70* language;           // +0x14
};
#pragma pack(pop)

void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
int __stdcall DrawListboxFrame(Dialog_4a4d70* obj, int index, void* bmp);
void __stdcall CopySurfaceRect(void* dst, void* src, Rect_004a4d70* rect, int* pos);
int __stdcall FillRectangle(void* surface, Rect_004a4d70* rect, int colour);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
// The colour parameter must be int: forces the zero extension of the colour load.
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2,
                            int colour);

static inline Glyph_004a4d70* GetGlyph_004a4d70(unsigned char c)
{
    return (Glyph_004a4d70*)GetGafFrame(g_guiContext->language->glyphs, c);
}

static inline int Measure_004a4d70(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    char* q = text;
    while (*q != 0) {
        char ch = *q;
        Glyph_004a4d70* glyph = GetGlyph_004a4d70(ch);
        if (glyph != 0)
            width += glyph->width;
        ++q;
    }
    return width;
}

// FUNCTION: 0x4a4d70
void __stdcall DrawTextInput(Dialog_4a4d70* obj, int index)
{
    Entry_004a4d70* entries = obj->holder->entries;
    int i = 1;
    int t = 0;
    for (; i < entries->b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                SetFont(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries->b6.count + 1)
        SetFont(g_guiContext->current);

    Entry_004a4d70* me = &entries[index];

    Rect_004a4d70 rect;
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = me->h + rect.top - 1;

    if (me->align & 1) {
        FillRectangle(entries->surface, &rect, obj->colours[0]);
    } else {
        void* surface = obj->holder->surface;
        if (surface == 0)
            surface = obj->fallback;
        if (surface == 0) {
            DrawListboxFrame(obj, index, 0);
        } else {
            CopySurfaceRect(entries->surface, surface, &rect, (int*)&rect);
        }
    }

    SetTextColors(obj->colours[me->colours], GetTextKeyColor());
    rect.top += 3;
    // The style is read as entries[index].colours rather than me->colours:
    // sharing one load with the colour read above swaps the SIB registers of
    // that read (see the 0x4a4d70 entry in docs/field-notes.md, Part 5).
    FUN_004a50e0(entries->surface, me->b6.text, rect.left, rect.top,
                 rect.right - rect.left, entries[index].colours);

    if (index == obj->focus) {
        char* at = &me->b6.text[obj->cursor];
        char save = *at;
        *at = 0;
        int w = Measure_004a4d70(me->b6.text);
        *at = save;
        int height;
        if (g_guiContext->language == 0)
            height = GetFontHeight();
        else
            // Via GetGlyph, like every glyph fetch: keeps the width in esi.
            height = GetGlyph_004a4d70(0x49)->height + 2;
        int x = rect.left + w;
        // Arguments stay expressions; only x is a local.
        DrawLine(entries->surface, x, rect.top, x, height + rect.top, obj->colours[9]);
    }
}
// The glyph of a character in the current font (see 0x4a5030).

struct Font_004a5010 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a5010 {
    char unknown_0[0x14];
    Font_004a5010* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
// FUNCTION: 0x4a5010
void* __stdcall GetCharGlyph(unsigned char c)
{
    return GetGafFrame(g_guiContext->font->glyphs, c);
}
// Width of a string in pixels: the sum of the glyph widths of the current
// font, or GetTextWidth's measurement when no font is loaded.

struct Font_004a5030 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a5030 {
    char unknown_0[0x14];
    Font_004a5030* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int param_1, unsigned char* text);

// FUNCTION: 0x4a5030
int __stdcall GetTextPixelWidth(unsigned char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (g_guiContext->font == 0)
        return GetTextWidth(GetFont(), text);
    // Index text[i] and hold the character in its own local, not a walked pointer.
    for (int i = 0; text[i]; i++) {
        unsigned char c = text[i];
        unsigned short* glyph = (unsigned short*)GetGafFrame(g_guiContext->font->glyphs, c);
        if (glyph)
            width += *glyph;
    }
    return width;
}
// Line height of the current font: the height of the 'I' glyph plus 2, or
// GetFontHeight's value when no font is loaded.

struct Glyph_004a50b0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50b0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a50b0 {
    char unknown_0[0x14];
    Font_004a50b0* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFontHeight();

// FUNCTION: 0x4a50b0
int GetFontLineHeight()
{
    if (g_guiContext->font == 0) {
        return GetFontHeight();
    }
    return ((Glyph_004a50b0*)GetGafFrame(g_guiContext->font->glyphs, 'I'))->height + 2;
}
struct Glyph_004a50e0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a50e0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a50e0 {
    char unknown_0[0x14];
    Font_004a50e0* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
void __stdcall DrawFrame(void* surface, void* glyph, int x, int y);
void __stdcall DrawFrameLit(void* surface, void* glyph, int x, int y, int style);
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxWidth);

// FUNCTION: 0x4a50e0
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style)
{
    if (g_guiContext->font == 0) {
        DrawString(surface, text, x, y, -1);
        return;
    }
    unsigned char* s = (unsigned char*)text;
    while (*s) {
        if (*s >= ' ') {
            unsigned char c = *s;
            Glyph_004a50e0* g = (Glyph_004a50e0*)GetGafFrame(g_guiContext->font->glyphs, c);
            if (g) {
                if (maxw != -1 && (int)g->width > maxw)
                    return;
                if (*s != ' ') {
                    if (style == 0)
                        DrawFrame(surface, g, x, y);
                    else
                        DrawFrameLit(surface, g, x, y, style);
                }
                if (maxw != -1) {
                    maxw -= g->width;
                    if (maxw < 0)
                        return;
                }
                x += g->width;
            }
        }
        s++;
    }
}
// Started by Space Bunny Free (partial, 80.1%); finished by deepseek-v4.1-flash.

struct Glyph_004a51d0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
};

struct Font_004a51d0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0xc
};

struct Dialog_4a51d0 {
    char unknown_0[0x14];
    Font_004a51d0* font;               // +0x14
};

char* __stdcall GetGafFrame(void* glyphs, int c);
int GetFont();
int __stdcall GetTextWidth(int a, char* text);
int GetFontHeight();
void __stdcall FUN_004a50e0(char* dest, char* text, int p3, int x, int maxw, int style);

static inline int LineHeight_004a50b0()
{
    if (g_guiContext->font == 0)
        return GetFontHeight();
    return (int)((Glyph_004a51d0*)GetGafFrame(g_guiContext->font->glyphs, 'I'))->height + 2;
}

static inline int Measure(char* word, int t)
{
    if (word == 0)
        return t;
    if (g_guiContext->font == 0)
        return GetTextWidth(GetFont(), word);
    for (char* n = word; *n; n++) {
        unsigned char ch = *n;
        unsigned short* g = (unsigned short*)GetGafFrame(g_guiContext->font->glyphs, ch);
        if (g)
            t += *g;
    }
    return t;
}

// FUNCTION: 0x4a51d0
int __stdcall FUN_004a51d0(char* p2, char* text, int p4, int y, int maxw, int rem, int p7)
{
    int last = 0;
    int w;
    int k = 0;
    int i = 0;
    int len = strlen(text);
    while (text[k] != 0) {
        while (i != len && text[i] != ' ' && text[i] != '\r')
            i++;
        char saved = text[i];
        char* word = text + k;
        text[i] = 0;
        w = 0;
        w = Measure(word, w);
        if (w > maxw) {
            text[i] = saved;
            i = last;
            saved = text[i];
            text[i] = 0;
        } else if (saved != '\r' && i != len) {
            last = i;
            text[i] = saved;
            i++;
            continue;
        }
        FUN_004a50e0(p2, word, p4, y, maxw, p7);
        text[i] = saved;
        y += LineHeight_004a50b0() + 2;
        rem -= LineHeight_004a50b0() + 2;
        if (text[i] == 0)
            goto done;
        k = i + 1;
        if (rem <= 0)
            goto done;
        last = k;
        i = k;
    }
done:
    return y;
}
