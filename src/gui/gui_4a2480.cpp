// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Draws one gadget entry's three glyphs (start, repeated middle, end) across
// the span [x, x + w] on the surface of the entry table. Same entry table as
// 0x4a23b0 (fields x/y/w/h at 0x13..0x19) and 0x4a0f30 (holder at +0x18).
// Needed only for compiler state: changes how the loop test's sum is formed.
#include <windows.h>

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

Glyph_004a2480* __stdcall GetGafFrame(unsigned short* table, int index);
void __stdcall DrawFrame(void* surface, void* image, int x, int y);

// FUNCTION: 0x4a2480
void __stdcall FUN_004a2480(Class_004a2480* param_1, int index)
{
    Entry_004a2480* base = param_1->holder->entries;
    void* surface = base->surface;
    Entry_004a2480* e = &base[index];
    unsigned short* glyphs = e->glyphs;
    Glyph_004a2480* glyph = GetGafFrame(glyphs, 0);
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
    Glyph_004a2480* mid = GetGafFrame(glyphs, 1);
    if (x + mid->width < limit) {
        do {
            DrawFrame(surface, mid, x, y);
            x += mid->width;
        } while (x + mid->width < limit);
    }
    Glyph_004a2480* last = GetGafFrame(glyphs, 2);
    DrawFrame(surface, last, limit - last->width, y);
}
