// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
//
// 2026-09-30 (deepseek-v4.1): 65.2 -> 67.5 by declaring the function-scope
// `void* surface` FIRST among the locals (before `entries`/`e`), so it is
// loaded at entry into a real frame slot ([esp+0x10] for us; the original
// keeps it at [esp+0x1c]) instead of the freed index home [esp+0x58]. Slot
// choice for spilled locals is declaration-order sensitive here: moving an
// unrelated branch local's declaration earlier (`int limit` before `surf` in
// the h<=w branch) drops to 54.5%, `int n/i` before surface gives 67.3%, a
// fresh per-branch surface local gives 62.3%, one hoisted shared `int limit`
// is neutral at 65.2. Still differs (1605 bytes against 1631):
//  - w<h branch: ours spills its surface to [esp+0x58] and keeps `limit` in
//    ebp; the original keeps the surface in ebp, `limit` at [esp+0x10] and
//    the glyph pointers at [esp+0x58] (the freed index home).
//  - the w<h test: original `mov cx,[w]; mov dx,[h]; cmp cx,dx; jge`, ours
//    `mov cx,[h]; cmp [w],cx; jge` (flipping `<` to `>` was tried, no change).
//  - h<=w branch: our spare slot assignments ([esp+0x14] for both the w<h
//    limit and the h<=w surf) differ from the original's [esp+0x10] limit /
//    [esp+0x14] surf pattern.
//  - 26 bytes short, mostly in the flags&4 block, where the three _itoa
//    argument setups do not tail-merge the way the original's do.
// Retry 2026-09-30 (deepseek-v4.1-flash): kept the 65.2% v11 form. Re-tested
// moving the function-scope `surface` load down to just before FUN_004a23b0
// (the point of first use), which drops to 63.9%, so reverted. No new lever
// found; remaining diff is the two-slot swap described below plus the w<h
// branch keeping obj in ebp instead of the branch surface.
// Gave up near 62.3% (1605 bytes against 1631). Reload glyph pointers
// after callbacks. Reference-returning minimum helpers recover remaining-count
// stores, and shared glyph locals improve allocation. Remaining extra frame
// slot, glyph spills, and branch/scheduling differences.
//
// 2026-09-30 (deepseek-v4.1), 61.3% -> 62.3%: give each branch its own
// `void* surf` local instead of one function-scope one. That puts the h<=w
// surface at [esp+0x14] (frame 0x04, shared with lc) and the glyph pointers in
// the dead index home slot [esp+0x58], exactly as the original. Source kept as
// build/scratch/0x4a2580/v3.cpp.
//
// 2026-09-30 (deepseek-v4.1), 62.3% -> 65.2%: the w<h branch must not declare
// its own `void* surf`; assign the function-scope `surface` instead
// (`surface = obj->holder->entries->u.head.surface;`) and draw through it.
// That removes the extra frame dword (frame is now 0x40, buf at [esp+0x20] as
// in the original) and kills the whole +4 offset family of diffs. Source kept
// as build/scratch/0x4a2580/v11.cpp. Doing the same in the h<=w branch drops to
// 60.1% (v12), and dropping the reload chain in w<h drops to 52.8% (v13), so
// only the w<h branch wants this form.
//
// 2026-09-30 (deepseek-v4.1), retried four allocator levers, all worse than the
// 65.2% above, so the file keeps the v11 form:
//  - w<h with its own `void* vsurf` local (the ebp surface the original shows):
//    62.3%. Same for a distinct local name; a shared local is what the
//    allocator awards the freed param home slot.
//  - `surface` declared after the loop counters: 64.8%, slot assignment
//    unchanged ([esp+0x58] surface, [esp+0x14] walking pointer).
//  - `(int)e->w < (int)e->h` and `e->h > e->w`: 64.8%, same single hunk.
//  - explicit `Entry_004a2580* p = entries;` used by the scan loop: identical
//    codegen, p is optimised into the same induction pointer.
// The original's `mov cx,[w]; mov dx,[h]; cmp cx,dx` versus ours
// `mov cx,[h]; cmp [w],cx` is not steerable from the condition text; it is the
// same global allocation choice that puts surface at [esp+0x1c] there and
// [esp+0x58] here (the freed `index` home slot), so ebp keeps obj here and
// holds the branch surface there.
// Still differs (1605 bytes against 1631):
//  - slot swap: the original homes the loop's walking entry pointer at the dead
//    index slot [esp+0x58] and `surface` at [esp+0x1c]; ours homes `surface` at
//    [esp+0x58] and the pointer at [esp+0x14]. The original keeps the w<h
//    surface in ebp (and reloads the obj parameter into ebp after the branch at
//    0x4a29ef) while ours spills it to [esp+0x58] and keeps `limit` in ebp.
//    Writing `limit`/`t` as plain ifs instead of the Smaller() helper removes
//    the reference temps but not the extra allocation (56.1%), so this is the
//    allocator's pick, not a source-order lever.
//  - the branch test: original is `mov cx,[ebx+0x17]; mov dx,[ebx+0x19];
//    cmp cx,dx; jge` (both operands in registers), ours loads h into cx and
//    compares w from memory; flipping `<` to `>` did not change it.
//  - ours is 26 bytes shorter, most of it in the flags&4 block (the -430/+434
//    hunk), where the two _itoa call sites are not fully tail-merged.
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

static inline const int& Smaller(const int& a, const int& b) { return a < b ? a : b; }
// FUNCTION: 0x4a2580
void __stdcall FUN_004a2580(Object_004a2580* obj, int index)
{
    void* surface = obj->holder->entries->u.head.surface;
    Entry_004a2580* entries = obj->holder->entries;
    Entry_004a2580* e = &entries[index];
    Glyph_004a2580* g;
    Glyph_004a2580* mid;

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

    unsigned short* gl = e->glyphs;
    if (gl == 0) {
        FUN_004b0510(surface, r1, obj->field_8b2, obj->field_8c3, obj->field_8c6);
        FUN_004b0590(surface, r2, obj->field_8b2, obj->field_8c3, obj->field_8c6);
    } else if (e->w < e->h) {
        int y = e->y;
        surface = obj->holder->entries->u.head.surface;
        int x = e->x;
        int limit = y + e->h - 1;
        g = FUN_004b7f30(e->glyphs, e->field_152);
        if (g != 0)
            FUN_004b7f90(surface, g, x, y);
        y += g->height;
        mid = FUN_004b7f30(e->glyphs, e->field_152 + 1);
        while (y + mid->height <= limit) {
            FUN_004b7f90(surface, mid, x, y);
            y += mid->height;
        }
        Glyph_004a2580* last = FUN_004b7f30(e->glyphs, e->field_152 + 2);
        FUN_004b7f90(surface, last, x, limit - last->height + 1);
        x += last->width / 2;
        g = FUN_004b7f30(e->glyphs, e->field_152 + 3);
        x -= g->width / 2;
        int lc = e->h - 6;
        lc = Smaller(lc, (int)e->size);
        int ybase = e->off + e->y + 3;
        int lim2 = lc + ybase - 1;
        int t = e->h + e->y - 4;
        lim2 = Smaller(lim2, t);
        if (ybase > lim2 - lc + 1)
            ybase = lim2 - lc + 1;
        FUN_004b7f90(surface, g, x, ybase);
        lc -= g->height;
        ybase += g->height;
        mid = FUN_004b7f30(e->glyphs, e->field_152 + 4);
        while (ybase <= lim2 - mid->height) {
            FUN_004b7f90(surface, mid, x, ybase);
            lc -= mid->height;
            ybase += mid->height;
        }
        FUN_004b7f90(surface, mid, x, lim2 - mid->height);
        g = FUN_004b7f30(e->glyphs, e->field_152 + 5);
        FUN_004b7f90(surface, g, x, lim2 - g->height + 1);
    } else {
        void* surf = obj->holder->entries->u.head.surface;
        int x = e->x;
        int y = e->y;
        Glyph_004a2580* first = FUN_004b7f30(e->glyphs, e->field_152);
        int limit = x + e->w - 1;
        if (first != 0)
            FUN_004b7f90(surf, first, x, y);
        x += first->width;
        mid = FUN_004b7f30(e->glyphs, e->field_152 + 1);
        while (x + mid->width <= limit) {
            FUN_004b7f90(surf, mid, x, y);
            x += mid->width;
        }
        Glyph_004a2580* last = FUN_004b7f30(e->glyphs, e->field_152 + 2);
        FUN_004b7f90(surf, last, limit - last->width + 1, y);
        y += last->height / 2;
        g = FUN_004b7f30(e->glyphs, e->field_152 + 3);
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
                    g = FUN_004b7f30(
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
