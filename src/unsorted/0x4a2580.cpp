// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// 2026-10-01 retry 8 (deepseek-v4.1-flash, #3660): best is still 69.8% (the form below).
// New measurements, all scored:
//  - The w<h register tie is real and use-count sensitive, but only for a BRANCH-LOCAL
//    surface. In v1 (branch-local surf) one extra FUN_004b7f90(surf,...) call flips the
//    allocator exactly as the original wants (surf -> ebp, limit -> a home, probes p3/p4/p6),
//    while the same extra call on the function-scope surface form (z2) does NOT flip: a
//    function-scope surface that is live from the top is never a register candidate, so its
//    use count is irrelevant. The original's w<h surface therefore IS a separate variable.
//  - q1 (entries/e declared before surface, branch-local surf) reproduces the ORIGINAL
//    prologue instruction for instruction (surface load 0x4a25b5, store to [esp+0x1c],
//    entries at [esp+0x18]) but the frame grows to 0x44 because the w<h temps and the h<=w
//    surf take a fresh [esp+0x20] slot instead of reusing limit's/lc's; the branch-local surf
//    still loses ebp to limit, 64.7%. Adding the extra use to q1 (q1p) does flip surf to ebp
//    but the frame/slot order stays wrong (limit at 0x14, lc/temp at 0x10) and the score
//    falls to 63.9%. So the flip alone is not enough: the original's frame has exactly four
//    scalar slots (limit 0x10, lc 0x14, entries 0x18, surface 0x1c) and NO slot for the
//    enregistered surf, which only happens if surf is enregistered without a home.
//  - Tested and inert: v1/v8/v9/v10/v12/v15/v16, w1 (inline Draw wrapper), w2/w3 (inline
//    wrapper with its own redundant g!=0 check, the guide's 0x4bcb50 shape), s1/s3
//    (surf=surf), f3/f4 (folded uses), v14a/b and u1/u2 (uninitialised declarations), v18
//    (shared function-scope limit), v17 (one surface variable for all three branches), v19
//    (separate function-scope vsurf for the branch only), v20 (inline Surface(obj) getter),
//    v21 (branch-local named surface, shadowing), v22 (whole w<h branch in an inline helper
//    taking the surface as a parameter), declaration order permutations o1-o4, q1-q3, z1,
//    r1 (lc declared first), x1/x2 (short operands, flipped condition), v11a/v11b (hoisted
//    limit/lc), w3/m1/m2 (Min by reference, i.e. inline-if min shape), m3.
//  - p7 (an extra real use of limit in the loop test) does NOT flip it either, so the tie is
//    not a plain use count of the two variables; the p3/p4/p6 flips changed a whole block's
//    shape, not just a count.
// Remaining gap: the w<h branch wants its own surface variable enregistered in ebp with no
// frame home, so limit/lc/lim2 stay in the 0x10/0x14 slots and the inline-if min shape
// (not the Smaller reference shape the current 69.8% uses) can match. A natural construct
// that gives that variable one more counted use without emitting code (the guide's 0x4bcb50
// pattern) is still missing.
// 2026-10-01 retry 7 (deepseek-v4.1-flash, #3660): kept 69.8%. New measurements, all scored:
//  - declaring `entries` first and initialising `surface` from it compiles byte-identically
//    to the stored form (1631 bytes, same slots: surface 0x10, lc 0x14, entries 0x18, min
//    temp 0x1c, loop pointer in the freed index home 0x58), so the declaration order of the
//    two entry pointers is not a lever.
//  - deferring the surface definition to a plain assignment after `e = &entries[index]`
//    (tested with the declaration first and after e) reproduces the ORIGINAL prologue
//    instruction for instruction through 0x4a25a1 (entries), the lea chain, 0x4a25b5 (load)
//    and 0x4a25c2 (store), but the allocator then homes `surface` in the freed index home
//    [esp+0x58] and gives the loop pointer [esp+0x14]: the exact swap of the original, 67.6%.
//    Adding hoisted `int limit; int lc;` (with or without forced initial values, which get
//    dead-code eliminated) on top is still 67.6% / 69.8% and does not move the surface.
//  - an explicit `Entry_004a2580* p = &entries[1]` walking pointer drops to 55.1%.
//  - a by-value `inline int Smaller(int,int)` (instead of the const int& form) drops to
//    62.4% with 1604 bytes: the reference form is what produces the shared 26-byte _itoa tail.
// Remaining gap: the allocator's home for the function-scope `surface`, ours [esp+0x10] and
// the original [esp+0x1c] (which in the original frees 0x10 for the w<h `limit`, keeps the
// reloaded surface in ebp through that branch and leaves no min temp at 0x1c).
// 2026-10-01 retry 6 (deepseek-v4.1-flash): six more levers tested, none beat 69.8%.
// Hoisting limit/lc as function-scope locals before entries/surface (the slot order the
// original shows is limit 0x00, lc 0x04, entries 0x08, surface 0x0c) gives 67.6%: the surface
// then lands in the freed index home [esp+0x58] and the scan loop pointer takes its frame slot.
// Reordering the prologue so entries/e are computed before the surface load gives 67.6% too,
// and adding a fresh branch-local surface in the w<h branch on top gives 64.7%. A fresh w<h
// branch-local surface alone gives 63.1% whether it is declared before or after `int y = e->y`
// (it gets a home, local 0x0c, instead of ebp); naming the branch test operands `short w/h`
// gives 62.6%; plain ifs instead of Smaller() in the lc/lim2 block give 62.4% (1604 bytes, the
// shared 26-byte _itoa tail merge is lost); a plain if for lc only gives 62.3% (1616 bytes);
// moving the h<=w surf declaration after x/y gives 58.4%. Conclusion: in the original the
// function-scope surface keeps slot 0x0c while the w<h branch draws through a register (ebp)
// copy, and MSVC only awards that register when the branch variable has no stack home, which
// our formulations never achieve (they either keep the home or spend a local frame slot).
// 2026-10-01 retry 5 (deepseek-v4.1-flash): best stays 69.8%, 1631 bytes. A fresh branch-local surface (with and without a const function-scope surface) gives 63.1%; hoisting function-scope `int limit; int lc;` before entries/surface lands the surface in the freed [esp+0x58] home at 67.6%; the w<h test cast to unsigned short is byte-identical. Still differs: surface home [esp+0x10] vs the original [esp+0x1c] and the w<h register split (surface in ebp vs our stack home).
// Sonnet 5.5 (#3246): re-checked 69.8%. Turning r1/r2/rect from int[4] into a `Rect` struct (and
// the callee parameters to Rect*) is byte-identical, so the frame/slot differences are not an
// array-versus-struct question.
//
// 2026-10-01 (deepseek-v4.1-flash, 67.5 -> 69.8, both 1631 bytes): the 26-byte
// gap was the _itoa dispatch. Splitting the value computation into three
// separate `_itoa(...)` calls (float path, flags&8 path, plain off path)
// instead of computing a single `int v` first reproduces the original's
// tail-merged shared `call _itoa` and closes the byte count. Still differs
// (not byte-identical): the frame slot for the function-scope `surface` is
// [esp+0x10] in ours versus [esp+0x1c] in the original (original keeps w<h
// `limit` at [esp+0x10] and the branch surface/lc at [esp+0x14] with
// `entries` at [esp+0x18]); in the w<h branch the original keeps the reloaded
// surface in ebp and reloads obj from [esp+0x54] afterwards. Reordering the
// surface/entries declarations was tried and does not move the slot.
//
// 2026-09-30 retry (deepseek-v4.1-flash, 67.5%): tested two more allocator
// levers, neither beat 67.5%, so kept the current form. Declaring `entries`
// then `surface` (vB) gives 65.2%, declaring a function-scope `int limit`
// before `surface` (vC) is identical at 67.5%. The surface load is scheduled
// at function entry no matter where its declaration sits. Still differs: the
// original homes `surface` at [esp+0x1c] and keeps it in ebp through the w<h
// branch, while ours homes it at [esp+0x10] and keeps `limit` in ebp; the
// branch test operand order (`mov cx,[w]; mov dx,[h]` vs ours
// `mov cx,[h]; cmp [w],cx`) and the missing 26 bytes in the flags&4 block
// (the three _itoa setups do not tail-merge) remain.
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
        } else if (e->field_13c != 0) {
            _itoa((int)((float)e->off * e->field_13c / (e->w - e->size)), buf, 10);
        } else if (e->flags & 8) {
            _itoa(e->off + 1, buf, 10);
        } else {
            _itoa(e->off, buf, 10);
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