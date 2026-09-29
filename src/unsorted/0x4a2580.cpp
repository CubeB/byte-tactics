// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 55.6% (1580 vs 1631 bytes). Structure and all callees are right.
// Fixes that got here from the 39.2% baseline:
//  - a single `surface` variable shared across the arms (no per-arm copies),
//  - the glyph arms fetch their surface through `obj->holder->entries`
//    directly, which is what the original recomputes in each arm,
//  - cache `e->glyphs` in a local `gl` used for all twelve FUN_004b7f30
//    calls (the original keeps e->glyphs in eax across both arms, loaded once
//    at 0x4a2650); this moved entries/surface to the original slots and took
//    the score 54.8 -> 55.6.
// Current slot map from the /Fa listing (ours, frame 0x44):
//   entries -56, surface -52, buf/rect -48, r1 -32, r2 -16  (all match the
//   original), but surf -68, lc -64, g/mid/first -60.
// The original frame is 0x40: limit -64, lc/surf -60, entries -56,
// surface -52, buf/rect -48, r1 -32, r2 -16. So ours has ONE extra 4-byte
// temp slot: the original shares -60 between the vertical `lc` and the
// horizontal `surf`, and spills g/mid to the argument slot [esp+0x58]
// instead of a frame slot. Ours needs surf and g/mid to be live at once.
// Everything else in the body is byte-identical modulo the +4 frame shift.
// Trying per-arm `surf` declarations instead (v2) scored 54.7, worse.
// The first block is the same "select the type 7 entry whose group matches"
// loop as matched 0x4a30c0 (group +0x28, id +0xd6, count +0xb6 in entry 0).
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

void __stdcall FUN_004c1420(int id);
void __stdcall FUN_004a23b0(Entry_004a2580* base, int index, int* r1, int* r2);
void __stdcall FUN_004b0510(void* surface, int* r, int a, int b, int c);
void __stdcall FUN_004b0590(void* surface, int* r, int a, int b, int c);
Glyph_004a2580* __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004b7f90(void* surface, void* glyph, int x, int y);
int FUN_004c13f0();
void __stdcall FUN_004c13a0(int a, int b);
int FUN_004c1440();
void __stdcall FUN_004c1480(Font_004a2580* font, char* text);
int FUN_004c1450();
void __stdcall FUN_004c14f0(void* surface, char* text, int x, int y, int maxw);
void __stdcall FUN_004bfe10(void* surface, void* rect);
void __stdcall FUN_004bf4d0(void* surface, void* rect, int a);

// FUNCTION: 0x4a2580
void __stdcall FUN_004a2580(Object_004a2580* obj, int index)
{
    Entry_004a2580* entries = obj->holder->entries;
    Entry_004a2580* e = &entries[index];
    void* surface = entries->u.head.surface;
    void* surf;
    unsigned short* gl = e->glyphs;

    int n = 0;
    int i = 1;
    for (; i < entries->u.head.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                FUN_004c1420(*(int*)((char*)&entries[i] + 0xd6));
                break;
            }
            n++;
        }
    }
    if (i == entries->u.head.count + 1)
        FUN_004c1420(DAT_0051fba4->group);

    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    if (gl == 0) {
        FUN_004b0510(surface, r1, obj->field_8b2, obj->field_8c3, obj->field_8c6);
        FUN_004b0590(surface, r2, obj->field_8b2, obj->field_8c3, obj->field_8c6);
    } else if (e->w < e->h) {
        surf = obj->holder->entries->u.head.surface;
        int y = e->y;
        int x = e->x;
        int limit = y + e->h - 1;
        Glyph_004a2580* g = FUN_004b7f30(gl, e->field_152);
        if (g != 0)
            FUN_004b7f90(surf, g, x, y);
        y += g->height;
        Glyph_004a2580* mid = FUN_004b7f30(gl, e->field_152 + 1);
        while (y + mid->height <= limit) {
            FUN_004b7f90(surf, mid, x, y);
            y += mid->height;
        }
        Glyph_004a2580* last = FUN_004b7f30(gl, e->field_152 + 2);
        FUN_004b7f90(surf, last, x, limit - last->height + 1);
        x += last->width / 2;
        g = FUN_004b7f30(gl, e->field_152 + 3);
        x -= g->width / 2;
        int lc = e->h - 6;
        if (lc >= e->size)
            lc = e->size;
        int ybase = e->off + e->y + 3;
        int lim2 = lc + ybase - 1;
        int t = e->h + e->y - 4;
        if (lim2 >= t)
            lim2 = t;
        if (ybase > lim2 - lc + 1)
            ybase = lim2 - lc + 1;
        FUN_004b7f90(surf, g, x, ybase);
        lc -= g->height;
        ybase += g->height;
        mid = FUN_004b7f30(gl, e->field_152 + 4);
        while (ybase <= lim2 - mid->height) {
            FUN_004b7f90(surf, mid, x, ybase);
            lc -= mid->height;
            ybase += mid->height;
        }
        FUN_004b7f90(surf, mid, x, lim2 - mid->height);
        g = FUN_004b7f30(gl, e->field_152 + 5);
        FUN_004b7f90(surf, g, x, lim2 - g->height + 1);
    } else {
        surf = obj->holder->entries->u.head.surface;
        int x = e->x;
        int y = e->y;
        Glyph_004a2580* first = FUN_004b7f30(gl, e->field_152);
        int limit = x + e->w - 1;
        if (first != 0)
            FUN_004b7f90(surf, first, x, y);
        x += first->width;
        Glyph_004a2580* mid = FUN_004b7f30(gl, e->field_152 + 1);
        while (x + mid->width <= limit) {
            FUN_004b7f90(surf, mid, x, y);
            x += mid->width;
        }
        Glyph_004a2580* last = FUN_004b7f30(gl, e->field_152 + 2);
        FUN_004b7f90(surf, last, limit - last->width + 1, y);
        y += last->height / 2;
        Glyph_004a2580* g = FUN_004b7f30(gl, e->field_152 + 3);
        y -= g->height / 2;
        int a = e->off + e->x + 3;
        int b = limit - g->width - 2;
        if (a >= b)
            a = b;
        FUN_004b7f90(surf, g, a, y);
    }

    if (e->flags & 4) {
        int cur = FUN_004c13f0();
        FUN_004c13a0(obj->field_8c1, cur);
        char buf[0x10];
        if (e->u.text[0] != 0) {
            strcpy(buf, e->u.text);
        } else {
            int v;
            if (e->field_13c != 0)
                v = (int)((float)e->off * e->field_13c / (e->w - e->size));
            else if (e->flags & 8)
                v = e->off + 1;
            else
                v = e->off;
            _itoa(v, buf, 10);
        }
        char* p = buf;
        if (p != 0) {
            if (DAT_0051fba4->font == 0) {
                FUN_004c1480((Font_004a2580*)FUN_004c1440(), buf);
            } else {
                int total = 0;
                for (; *p != 0; p++) {
                    Glyph_004a2580* g = FUN_004b7f30(
                        (unsigned short*)DAT_0051fba4->font->glyphs, (unsigned char)*p);
                    if (g != 0)
                        total += g->width;
                }
            }
        }
        if (DAT_0051fba4->font == 0)
            FUN_004c1450();
        else
            FUN_004b7f30((unsigned short*)DAT_0051fba4->font->glyphs, 0x49);
        FUN_004c14f0(surface, buf, e->x + e->w + 2, e->y + 4, -1);
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
        FUN_004bfe10(entries->u.head.surface, rect);
        FUN_004bf4d0(entries->u.head.surface, rect, -0x14);
    }
}
