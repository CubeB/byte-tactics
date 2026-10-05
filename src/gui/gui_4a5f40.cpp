// Decompiled by deepseek-v4.1, finished by Sonnet 5.5, finished by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by Fable 5.1, finished by DeepSeek V4.1 Flash, finished by claude-opus-5-5, finished by GPT-6, finished by claude-opus-5-5. Names are provisional.
// MATCH (claude-opus-5-5, #5089, 2026-10-03), from 91.8%.
// Draws one list-gadget entry: its glyph or frame, then the text (left,
// right, centred, or centred with an underlined hotkey letter, flags 1/4/2/0x20).
// What the last steps changed, each measured:
//  - The key1 hotkey width is the plain inlined FUN_004a5030 again, and the
//    flags 0x20 y (`ys`) is a block local. The previous `int* pm = &measured`
//    alias and the by-reference MeasureInto copy are gone: MSVC packs the
//    inline's accumulator and ys into one slot by itself ([esp+0x20]).
//  - `#include <math.h>` plus LineHeight returning through a named `glyph`
//    local gives the flags 0x20 branch the original's registers: xb in ebx
//    computed before the LineHeight call, ys in esi with its home store
//    (82.5% -> 96.1%; the include alone gives 89.2%). Found by permute.py.
//  - Frame order, worked out with docs/agent-guide.md's frame-layout rule
//    (refs counted in code order, `inc [mem]` counting twice, then the
//    refs*1000/size quicksort, which reproduced both our old and new frames):
//    the tab scan counts in its own `tab` (it shares t's slot), and each
//    hotkey branch declares its own `found` (one slot, created at 3 refs
//    instead of 6). 96.1% -> 99.9%.
//  - key1 declared in the hotkey block lets the scheduler hoist
//    `key1[1] = 0` into the inlined strcpy, as the original has it.
//  - Declaration order is load-bearing: `rect` before `flagy` and `t` (the
//    operand order of their adds), and `textw` last: several other places
//    for it make MSVC share `rect.right - rect.left` between x and width
//    (2696 bytes).
#include <math.h>
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
    int colours;                       // +0x1f
    char unknown_23[0x28 - 0x23];
    signed char tab;                   // +0x28
    char unknown_29[0x2f - 0x29];
    GafEntry_004a5f40* gaf;            // +0x2f
    char unknown_33[0xb6 - 0x33];
    union {
        struct {
            short count;               // +0xb6
            char unknown_b8[0xbc - 0xb8];
            void* surface;             // +0xbc
        } list;                        // entry 0 only
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
    unsigned char colours[0x15];       // +0x8b2
};
#pragma pack(pop)

struct Rect_004a5f40 { int left, top, right, bottom; };

extern FontRoot_004a5f40* DAT_0051fba4;

void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
Glyph_004a5f40* __stdcall GetGafFrame(GafEntry_004a5f40* table, int index);
void __stdcall DrawFrame(void* surface, Glyph_004a5f40* glyph, int x, int y);
void __stdcall DrawFrameLit(void* surface, Glyph_004a5f40* glyph, int x, int y, int style);
int __stdcall FUN_004a5d50(Menu_004a5f40* menu, int index);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int colour);
void __stdcall GrayRectangle(void* surface, Rect_004a5f40* rect);
void __stdcall FadeRectangle(void* surface, Rect_004a5f40* rect, int param);
void __stdcall FUN_004b04b0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FUN_004b04e0(void* surface, Rect_004a5f40* rect, unsigned int a, unsigned int b, unsigned int c);

// The real FUN_004a5030 (matched in 0x4a5030.cpp), which /Ob2 inlines four times here.
static inline int FUN_004a5030(char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return GetTextWidth(GetFont(), text);
    char* p = text;
    while (*p != 0) {
        char ch = *p;
        Glyph_004a5f40* glyph = (Glyph_004a5f40*)GetGafFrame(
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
        return GetFontHeight();
    Glyph_004a5f40* glyph = GetGafFrame(
        (GafEntry_004a5f40*)DAT_0051fba4->language->glyphs, 0x49);
    return glyph->height + 2;
}

// FUNCTION: 0x4a5f40
void __stdcall FUN_004a5f40(Menu_004a5f40* menu, int index)
{
    char* p;
    char key2[2];
    void* surface;
    Entry_004a5f40* me;
    Rect_004a5f40 rect;
    int x;
    int width;
    int border;
    int flagy;
    char* text;
    int pass;
    int saved;
    char buf[0x80];
    int y, i;
    int t;
    int textw;

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

    int tab = 0;
    for (i = 1; i < entries->u.list.count + 1; i++) {
        if (entries[i].type == 7) {
            if (tab == me->tab) {
                SetFont(entries[i].language);
                break;
            }
            tab++;
        }
    }
    if (i == entries->u.list.count + 1)
        SetFont(DAT_0051fba4->current);

    textw = FUN_004a5d50(menu, index);
    surface = entries->u.list.surface;
    if (me->gaf != 0) {
        Glyph_004a5f40* glyph;
        if (me->field_13c & 1) {
            if (me->flags & 0x100) {
                glyph = GetGafFrame(me->gaf, me->gaf->count - 1);
            } else if (me->field_136 != 0) {
                glyph = GetGafFrame(me->gaf, me->field_137);
                border = 1;
            } else if (me->flags & 0x1800) {
                glyph = GetGafFrame(me->gaf, me->field_13b);
                border = 1;
            } else {
                int val = me->gaf->count - 1;
                if (me->field_138 + 2 < val)
                    val = me->field_138 + 2;
                glyph = GetGafFrame(me->gaf, val + me->field_13b);
                if (!(me->flags & 0x80))
                    border = 1;
            }
        } else {
            if (me->field_138 != 0 && (unsigned short)me->gaf->count > (unsigned short)me->field_136) {
                if (me->field_136 != 0)
                    glyph = GetGafFrame(me->gaf, me->gaf->count - 2);
                else
                    glyph = GetGafFrame(me->gaf, me->field_13b + me->field_138);
            } else if (me->field_136 != 0)
                glyph = GetGafFrame(me->gaf, me->field_137);
            else
                glyph = GetGafFrame(me->gaf, me->field_13b);
        }
        if (glyph != 0) {
            if (me->colours != 0)
                DrawFrameLit(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top, me->colours);
            else
                DrawFrame(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top);
        }
    } else {
        if (me->field_13c & 1) {
            FUN_004b04b0(surface, &rect, menu->colours[0], menu->colours[0x13], menu->colours[0x13]);
        } else if (me->field_138 != 0) {
            FUN_004b04b0(surface, &rect, menu->colours[0], menu->colours[0x11], menu->colours[0x14]);
        } else {
            FUN_004b04e0(surface, &rect, menu->colours[0], menu->colours[0x11], menu->colours[0x14]);
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
            SetTextColors(menu->colours[0], GetTextKeyColor());
        else
            SetTextColors(menu->colours[me->colours], GetTextKeyColor());

        p = text;
        if (me->field_136 != 0) {
            for (unsigned int k = me->field_137; k != 0; k--) {
                while (*p != 0)
                    p++;
                p++;
            }
        }

        y = (rect.bottom - LineHeight_004a5f40() - rect.top) / 2 + flagy + rect.top;
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
                FUN_004a50e0(surface, p, x, y, 1 + (rect.right - rect.left), 0);
            } else {
                char key1[2];
                char* found;
                key1[0] = me->field_13a;
                width = rect.right - rect.left + 1;
                strcpy(buf, p);
                key1[1] = 0;
                found = strstr(buf, key1);
                if (found != 0) {
                    GetFont();
                    strcpy(buf, p);
                    *found = 0;
                    FUN_004a50e0(surface, buf, x, y, width, 0);
                    x += FUN_004a5030(buf);
                    saved = x;
                    if (me->field_138 != 0)
                        SetTextColors(menu->colours[0], GetTextKeyColor());
                    else
                        SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                    FUN_004a50e0(surface, key1, x, y, width, 0);
                    x += FUN_004a5030(key1);
                    if (me->field_138 != 0) {
                        DrawLine(surface, saved, LineHeight_004a5f40() + y - 1,
                                     x - 1, LineHeight_004a5f40() + y - 1,
                                     menu->colours[0]);
                    } else {
                        DrawLine(surface, saved, LineHeight_004a5f40() + y - 1,
                                     x - 1, LineHeight_004a5f40() + y - 1,
                                     menu->colours[2]);
                    }
                    if (me->field_138 != 0)
                        SetTextColors(menu->colours[0], GetTextKeyColor());
                    else
                        SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                    FUN_004a50e0(surface, found + 1, x, y, width, 0);
                } else {
                    FUN_004a50e0(surface, p, x, y, width, 0);
                }
            }
        } else if (me->flags & 0x20) {
            int xb;
            char* found;
            xb = (rect.right - textw - rect.left) / 2 + t;
            xb += rect.left + 1;
            int ys = flagy - LineHeight_004a5f40();
            ys += rect.bottom - 4;
            if (me->field_13a != 0 && (found = strchr(p, (signed char)me->field_13a)) != 0) {
                width = rect.right - rect.left + 1;
                key2[0] = me->field_13a;
                key2[1] = 0;
                GetFont();
                *found = 0;
                FUN_004a50e0(surface, p, xb, ys, width, 0);
                // Suspected original bug: this measures `text` (the first
                // string, [esp+0x4c] at 0x4a681e), not the drawn prefix `p`,
                // so the underline is misplaced when field_136 selects a later
                // string. The flags 2 branch measures its truncated copy.
                xb += FUN_004a5030(text);
                SetTextColors(menu->colours[10], GetTextKeyColor());
                FUN_004a50e0(surface, key2, xb, ys, width, 0);
                xb += FUN_004a5030(key2);
                SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                FUN_004a50e0(surface, found + 1, xb, ys, width, 0);
            } else {
                FUN_004a50e0(surface, p, xb, ys, rect.right - rect.left + 1, 0);
            }
        }
    } while (pass--);

    menu->current = menu->values[0];
    if (border) {
        GrayRectangle(surface, &rect);
        FadeRectangle(surface, &rect, -0x14);
    }
}
