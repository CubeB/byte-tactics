// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Space Bunny Free. finished by claude-sonnet-5-5, finished by claude-opus-5-5, finished by DeepSeek V4.1 Flash, finished by Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
// 2026-10-03 (claude-opus-5-5): MATCH. The last residual (the w<h arm's two
// frame slots swapped: lc nearest esp here, limit/lim2 nearest in the original)
// was variable identity, not spelling:
//  - the function has ONE glyph pointer, `g`, reused for every glyph it fetches
//    (no separate mid/first/last), and the w<h arm has ONE `limit`, reused for
//    the clamped bound of its second stack (the old `lim2`);
//  - MSVC 5 packs spilled variables into shared frame slots per variable, so
//    the merged `limit` is heavy enough to take the nearest slot before `lc`,
//    and the merged `g` is heavier still and keeps the dead `index` home that
//    the glyph pointers use. Merging only limit/lim2 sent limit to the `index`
//    home (94.6%); merging only the glyph pointers changed nothing (96.8%).
// The three clamps are now all the <windows.h> min() macro (<ddraw.h> pulls it
// in); the old "add edi,-2 needs double evaluation" note no longer applies, a
// plain if for `a` matches too.
// FUN_004a2480, the function just before this one in the exe, is defined above
// without an annotation (it has its own file). It replaces the old unused
// `int b;` local and unused Smaller() helper: like them it only moves MSVC's
// symbol counter, which decides a tie in the lc computation (filler sweeps show
// a period of 32 global declarations, half of them matching). The old pair
// still matches in place of the predecessor; with neither, the lc lines come
// out reordered (91.9%). With FUN_004a23b0 defined above it as well, both predecessors
// reproduce their own bytes here but this function drops to 91.5%; this file's
// copy of FUN_004a2480 compiles to 84.1% of its original, which only says the
// TU state differs.
#include <ddraw.h>
#include <string.h>
#include <stdlib.h>

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

struct Class_0051fba4 {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a2580* font;               // +0x14
};

extern Class_0051fba4* DAT_0051fba4;

void __stdcall SetFont(int id);
void __stdcall FUN_004a23b0(Entry_004a2580* base, int index, int* r1, int* r2);
void __stdcall FUN_004b0510(void* surface, int* r, int a, int b, int c);
void __stdcall FUN_004b0590(void* surface, int* r, int a, int b, int c);
Glyph_004a2580* __stdcall GetGafFrame(unsigned short* glyphs, int c);
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

// FUN_004a2480 (src/gui/gui_4a2480.cpp), the preceding function in the
// exe, copied here without its annotation: see the note at the top.
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


void __stdcall FUN_004a2480(Class_004a2480* param_1, int index)
{
    Entry_004a2480* base = param_1->holder->entries;
    void* surface = base->surface;
    Entry_004a2480* e = &base[index];
    unsigned short* glyphs = e->glyphs;
    Glyph_004a2580* glyph = GetGafFrame(glyphs, 0);
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
    Glyph_004a2580* mid = GetGafFrame(glyphs, 1);
    if (x + mid->width < limit) {
        do {
            DrawFrame(surface, mid, x, y);
            x += mid->width;
        } while (x + mid->width < limit);
    }
    Glyph_004a2580* last = GetGafFrame(glyphs, 2);
    DrawFrame(surface, last, limit - last->width, y);
}

// FUNCTION: 0x4a2580
void __stdcall FUN_004a2580(Object_004a2580* obj, int index)
{
    Entry_004a2580* entries = obj->holder->entries;
    Entry_004a2580* e = &entries[index];
    void* surface = Surface_004a2580(obj);
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
        SetFont(DAT_0051fba4->group);

    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    unsigned short* gl = e->glyphs;
    if (gl == 0) {
        FUN_004b0510(surface, r1, obj->field_8b2, obj->field_8c3, obj->field_8c6);
        FUN_004b0590(surface, r2, obj->field_8b2, obj->field_8c3, obj->field_8c6);
    } else {
        short w = e->w;
        short h = e->h;
        if (w < h) {
            void* surf = Surface_004a2580(obj);
            int y = e->y;
            int x = e->x;
            int limit = y + e->h - 1;
            g = GetGafFrame(e->glyphs, e->field_152);
            if (g != 0)
                DrawFrame(surf, g, x, y);
            y += g->height;
            g = GetGafFrame(e->glyphs, e->field_152 + 1);
            while (y + g->height <= limit) {
                DrawFrame(surf, g, x, y);
                y += g->height;
            }
            g = GetGafFrame(e->glyphs, e->field_152 + 2);
            DrawFrame(surf, g, x, limit - g->height + 1);
            x += g->width / 2;
            g = GetGafFrame(e->glyphs, e->field_152 + 3);
            x -= g->width / 2;
            int ybase = e->off + e->y + 3;
            int lc = min(e->h - 6, e->size);
            limit = lc + ybase - 1;
            int t = e->h + e->y - 4;
            limit = min(limit, t);
            if (ybase > limit - lc + 1)
                ybase = limit - lc + 1;
            DrawFrame(surf, g, x, ybase);
            lc -= g->height;
            ybase += g->height;
            g = GetGafFrame(e->glyphs, e->field_152 + 4);
            while (ybase <= limit - g->height) {
                DrawFrame(surf, g, x, ybase);
                ybase += g->height;
                lc -= g->height;
            }
            DrawFrame(surf, g, x, limit - g->height);
            g = GetGafFrame(e->glyphs, e->field_152 + 5);
            DrawFrame(surf, g, x, limit - g->height + 1);
        } else {
            void* surf = Surface_004a2580(obj);
            int x = e->x;
            int y = e->y;
            int limit = x + e->w - 1;
            g = GetGafFrame(e->glyphs, e->field_152);
            if (g != 0)
                DrawFrame(surf, g, x, y);
            x += g->width;
            g = GetGafFrame(e->glyphs, e->field_152 + 1);
            while (x + g->width <= limit) {
                DrawFrame(surf, g, x, y);
                x += g->width;
            }
            g = GetGafFrame(e->glyphs, e->field_152 + 2);
            DrawFrame(surf, g, limit - g->width + 1, y);
            y += g->height / 2;
            g = GetGafFrame(e->glyphs, e->field_152 + 3);
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
            if (DAT_0051fba4->font == 0) {
                GetTextWidth((Font_004a2580*)GetFont(), text);
            } else {
                char* p = text;
                for (; *p != 0; p++) {
                    unsigned char c = *p;
                    g = GetGafFrame((unsigned short*)DAT_0051fba4->font->glyphs, c);
                    if (g != 0)
                        total += g->width;
                }
            }
        }
        if (DAT_0051fba4->font == 0)
            GetFontHeight();
        else
            GetGafFrame((unsigned short*)DAT_0051fba4->font->glyphs, 0x49);
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