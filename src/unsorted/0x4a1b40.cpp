// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash. Names are provisional.
// RETRY (deepseek-v4.1-flash, issue 3552, best 26.6%, no change): four more
// allocator probes on top of the 3552 handout, all byte-identical at 26.6 /
// 2105 bytes (one frame is 0xc0 against the original 0xbc and the whole body
// is the one-step register rotation below). unsigned keepW, int keepC0,
// both together, and a swap/reuse of the two: no effect. The frame-shape
// trick that moved 0x49be60 (growing a scratch array by one element) has no
// analogue here because this function has no array local; the extra 4 bytes
// must come from a long-lived scalar the original keeps that we do not.
// RETRY (deepseek-v4.1-flash, issue 3575, best 26.6%, no change): three more
// allocator probes, all worse or neutral and reverted: the final highlight test
// spelled `holder->field_20` instead of `param_1->holder->field_20` to extend
// holder's live range (19.8), `int keepW = me->w;` hoisted above `int h` with
// the later declaration dropped (23.5), keepC0 declared before flags (26.6,
// byte-identical). So neither holder's live range nor keepW's first use moves
// the one-step rotation (need param_1=ebp, me=edi, zero=ebx, extra esi home).
// STATUS (deepseek-v4.1-flash, issue 3524): still best 26.6%, not MATCH. One
// more allocator attempt, reverted: defining `y` before `q` in the text prologue
// (so q's first use moves after y's) drops to 24.6, 2098 bytes (original 2160),
// so q must be defined first; the rotation still needs one long-lived node that
// this source shape does not create.
// STATUS (deepseek-v4.1-flash, issue 3325): best 26.6%, not MATCH.
// New evidence for the next attempt: every [esp+N] in the original is >= 0x10 and
// a multiple of 4 (checked with objdump over the whole function), i.e. the original
// frame has 16 bytes at esp+0x00..0x0f that no instruction ever touches, and its
// lowest live slot is t/line at 0x10. Ours uses esp+0x00..0x0f (t/line/colPtr,
// xx/row, entries/yy, flag), so our allocator starts 0x10 lower and the extra
// 16 bytes of the original are not the 4-byte frame difference (0xc0 vs 0xbc).
// A likely cause: a by-value struct temporary (the 16-byte Rect for FUN_004c6b10)
// whose home slot MSVC reserved at the bottom of the frame but never used, since
// the real copy is made with `sub esp,0x10; mov eax,esp` at 0x4a207f. Two more
// original details that are not in this file: the window rect is built twice in the
// cell branch (a dead store of x1 into the cell-loop rect at base+0x84 at 0x4a213c,
// and the real rect at base+0x18 later), and the text call's y argument is served
// from q's own slot (0x2c) because cy is copied there at 0x4a1e2e-0x4a1e32, so the
// source's cy/q live ranges really do interleave. Also tried on issue 3325 and
// neutral: an inline LineHeight_004a1b40() helper used for both lh and next
// (byte-identical to this file, 26.6%).
// STATUS (deepseek-v4.1-flash, issue 2934): best 26.6%, not MATCH.
// What moved it: (1) restore the TWO holder loads. The original reads
// [param_1+0x18] at 0x4a1b53 and again at 0x4a1b6c, because the store
// [holder+0x14]=1 may alias holder, so there is no single holder local spanning
// the store. Two-load alone is check.py 17.9% but aligns better (27.1% true LCS
// vs 23.9%). (2) two lenient locals that MSVC may re-load: `int keepW = me->w;`
// and `short keepC0 = me->field_c0;`. The original re-reads me->w (0x4a1bb5 and
// 0x4a1d35) and me->field_c0 (0x4a1c6a and 0x4a1f96); declaring a local for each
// is semantically free and changes the allocator state, 18.6 -> 26.6. keepW alone
// is 25.2 and keepC0 alone is 17.9, so both are load bearing. The three changes
// only work together: the same two locals on the original single holder load score
// 18.8, so the two holder loads are needed as well.
// Suspected original bug: the highlight call at 0x4a1fb6 has both arms dead
// identical (0x4a1fb8 and 0x4a1fcb both push 0x1e and lea the same [esp+0x1c]),
// so `holder->field_20 == param_2` has no effect on the output.
// What still differs: the body is a one-step register rotation. Original has
// param_1=ebp, me=edi, zero=ebx, and an extra esi home; ours has me=esi,
// zero=edi, param_1=ebx. So one more long-lived node ahead of `me` is missing.
// Adding speculative locals for type/flags/da/text/d6/ba/tab did not help. Frame
// is 0xc0 against the original 0xbc. Tried and rejected: keepW used for x2 still
// (17.9, the local must stay live into the loop); more persistent locals.
// Re-tried on issue 3304 (deepseek-v4.1-flash) with the same 26.6% ceiling:
// removing keepC0 (25.2), removing keepW (17.9), removing both (17.9), keepDa
// local (19.4), keepParam2 local (26.6, neutral), keepFlags local (24.9), self
// alias of param_1 (neutral), hoisting x1/x2/cy/cy2 declarations to the top
// (byte-identical to v0, so MSVC assigns slots by use, not declaration order),
// while(1) for the text loop (neutral), unsigned char colour index (neutral),
// headers.py (no header set beats 26.6). The text/cell loop bodies reconstruct
// instruction for instruction, so the remaining gap is pure allocator state,
// not missing code: the 55-byte shortfall is spill count (fewer memory operands
// where the original spills more).
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)

struct Entry_004a1b40 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colours;             // +0x1f
    char unknown_23[0x28 - 0x23];
    char tab;                           // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                    // +0xb6 (entry 0)
        char text_b6[0xb8 - 0xb6];
    } b6;
    short unknown_b8;
    short field_ba;                     // +0xba
    union {
        void* surface;                  // +0xbc (entry 0)
        short field_bc;                 // +0xbc
    } bc;
    short field_c0;                     // +0xc0
    char* text;                         // +0xc2
    int* cells;                         // +0xc6
    char unknown_ca[0xd6 - 0xca];
    void* field_d6;                     // +0xd6
    short field_da;                     // +0xda
    char unknown_dc[0x15b - 0xdc];
};

struct Holder_004a1b40 {
    char unknown_00[4];
    Entry_004a1b40* entries;            // +0x04
    char unknown_08[0x10 - 0x08];
    int field_10;                       // +0x10
    int field_14;                       // +0x14
    char unknown_18b[0x20 - 0x18];
    int field_20;                       // +0x20
    char unknown_18[0x24 - 0x18];
    void* surface;                      // +0x24
};

struct Class_004a1b40 {
    char unknown_00[0x18];
    Holder_004a1b40* holder;            // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colour;               // +0x8b2
    char unknown_8b3[0x8be - 0x8b3];
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

struct Class_004c6ae0 {
    void FUN_004c6ae0(Rect_004a1b40* rect);
};

struct Class_004c6b10 {
    void FUN_004c6b10(Rect_004a1b40 rect);
};

#pragma pack(pop)

extern LanguageRoot_004a1b40* DAT_0051fba4;

void __stdcall FUN_004b0230(Class_004a1b40* obj, int index, void* bmp);
void __stdcall FUN_004c6d20(void* dst, void* src, Rect_004a1b40* rect, int* pos);
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
                            unsigned char colour);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a1b40* rect, int id);
void __stdcall FUN_004c7580(void* surface, void* bitmap, Quad_004a1b40* dst,
                            Quad_004a1b40* src);

// Partial (18.6%), not MATCH. Attribution: started by deepseek-v4.1-flash, then GPT-6,
// edited by deepseek-v4.1 (issue 2379).
// Frame size (0xbc), ret 8, the &entries[param_2] lea chain and the overall branch
// structure are right; the differences are prologue register assignment and stack slots.
// Original prologue: sub esp,0xbc / push ebx / push ebp / mov ebp,[esp+0xc8] /
//   xor ebx,ebx / push esi / push edi, i.e. param_1 is loaded into ebp and ebx holds the
//   zero used for flag=0 and for top=0. Ours loads param_1 into another register
//   (eax/ebx depending on the variant) and zeroes into edi, so every [esp+N] drifts.
// Original slot map (verified from the disassembly, all offsets relative to esp after
// the four pushes): 0x10 line/tab counter (both share one slot, disjoint ranges),
// 0x14 flag, 0x18 x1, 0x1c cy, 0x20 x2, 0x24 cy2, 0x28 y, 0x2c q, 0x30 yoff, 0x34 h,
// 0x38 entries, 0x3c lh, 0x40..0x4c bounds (left,top,right,bottom), 0x50 me, 0x54 step,
// 0x60 font byte, 0x64 col, 0x68 glyph char temp. Ours: entries=0x10, flag=0x1c,
// h=0x2c, bounds=0x38..0x44, me=0x48, so the whole frame is reshuffled, not shifted.
// Original keeps me in edi, q in ebp, step in ebx, x1 in esi, x2 in ecx, step=ebx in the
// loop tail; ours uses different roles, so most of the text loop (0x4a1c5c) differs.
// The loop bottom is 0x4a1ff3: yoff+=step, line++ (slot 0x10), y++, h-=step, h<lh exit.
// Tried and rejected: dropping the `holder` local for fresh param_1->holder derefs
// (18.6% -> 17.9%, but the original does reload [ebp+0x18] twice at 0x4a1b53/0x4a1b6c, so
// a `holder` local is closer over the whole function); hoisting the tab counter `t` to
// the top (neutral); removing <windows.h> (18.6% -> 15.6%, include is needed).
// Open question worth solving next: the highlight call at 0x4a1fbe passes esp+0x1c after
// the 0x1e id was pushed, i.e. rowRect+8, and both of its arms (0x4a1fb8/0x4a1fcb) pass
// the same pointer. Either the original really indexes past rowRect or its highlight rect
// is a second rect whose slot overlaps rowRect (mutually exclusive branches).
//
// FUNCTION: 0x4a1b40
void __stdcall FUN_004a1b40(Class_004a1b40* param_1, int param_2)
{
    int t;
    int flag = 0;
    Rect_004a1b40 bounds;
    int& top = bounds.top;
    int& left = bounds.left;
    int& right = bounds.right;
    int& bottom = bounds.bottom;

    if (param_1->holder != 0)
        param_1->holder->field_14 = 1;

    Holder_004a1b40* holder = param_1->holder;
    Entry_004a1b40* entries = holder->entries;
    Entry_004a1b40* me = &entries[param_2];

    int h = me->h;
    if (me->type == 0) {
        top = 0;
        left = 0;
    } else {
        left = me->x;
        top = me->y;
    }
    int keepW = me->w;
    right = keepW + left - 1;
    bottom = me->h + top - 1;

    void* surface = holder->surface;
    if (surface == 0)
        surface = param_1->fallback;
    if (surface == 0) {
        if (!(holder->field_10 & 0x80))
            FUN_004b0230(param_1, param_2, 0);
    } else {
        FUN_004c6d20(entries->bc.surface, surface, &bounds,
                     &left);
    }

    int lh;
    if (DAT_0051fba4->language == 0)
        lh = FUN_004c1450();
    else
        lh = ((Glyph_004a1b40*)FUN_004b7f30(DAT_0051fba4->language->glyphs,
                                            0x49))->height + 2;

    int step = lh + 1;
    if (me->field_da != 0)
        step = me->field_da;

    int yoff = 0;
    int xx;
    int xw;
    unsigned int flags = (unsigned int)me->flags;
    short keepC0 = me->field_c0;

    if ((flags & 0x10) != 0 && me->text != 0 && keepC0 != 0) {
        // ---- text-line renderer ----
        int i = 1;
        t = 0;
        for (; i < entries->b6.count + 1; i++) {
            if (entries[i].type == 7) {
                if (t == me->tab) {
                    FUN_004c1420(*(int*)((char*)&entries[i] + 0xd6));
                    break;
                }
                t++;
            }
        }
        if (i == entries->b6.count + 1)
            FUN_004c1420(DAT_0051fba4->current);

        FUN_004c1440();
        unsigned char font = (unsigned char)FUN_004c13f0();
        char* q = FUN_004b6af0(me->text, me->bc.field_bc);
        int y = me->bc.field_bc;
        int line = 0;

        for (;;) {
            int x1 = left + 2;
            int x2 = x1 + keepW - 2;
            int cy = top + yoff + 2;
            int cy2 = cy + step;
            Rect_004a1b40 rowRect = {x1, cy, x2, cy2};

            int w;
            if (q == 0) {
                w = 0;
            } else if (DAT_0051fba4->language == 0) {
                w = FUN_004c1480(FUN_004c1440(), q);
            } else {
                char* pp = q;
                w = 0;
                while (*pp != 0) {
                    unsigned short* g = (unsigned short*)FUN_004b7f30(
                        DAT_0051fba4->language->glyphs, (unsigned char)*pp);
                    if (g != 0)
                        w += *g;
                    pp++;
                }
            }

            unsigned int col = (unsigned int)*((unsigned char*)param_1 + 0x8b2 + me->colours);
            if (me->field_d6 != 0) {
                if (*((char*)me->field_d6 + y) == 1)
                    flag = 1;
            } else if (*q == 0x26) {
                if (q[1] == 0x47)
                    flag = 1;
                q += 2;
            }

            flags = (unsigned int)me->flags;
            if (flags & 1) {
                xx = x1;
                xw = x2 - x1 + 1;
            } else if (flags & 4) {
                xx = x2 - w;
                xw = w;
            } else if (flags & 2) {
                xx = (x1 + x2 - w) / 2;
                if (xx < x1)
                    xx = x1;
                xw = x2 - xx + 1;
            }

            int next = 0;
            if (DAT_0051fba4->language == 0)
                next = FUN_004c1450();
            else
                next = ((Glyph_004a1b40*)FUN_004b7f30(
                            DAT_0051fba4->language->glyphs, 0x49))->height + 2;

            if (me->field_da > next + 6)
                FUN_004a51d0(entries->bc.surface, q, xx, cy, xw, bottom - top,
                             0);
            else
                FUN_004a50e0(entries->bc.surface, q, xx, cy, xw, 0);

            q = FUN_004b6af0(q, 1);

            if (flag) {
                flag = 0;
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x13);
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x14);
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x15);
                FUN_004bf4d0(entries->bc.surface, &rowRect,
                             -0x16);
            } else if ((me->flags & 0x100) == 0 &&
                       me->field_ba == line + me->bc.field_bc &&
                       keepC0 != 0) {
                // both arms (0x4a1fb8 and 0x4a1fcb) pass the same rect and id,
                // the arm is chosen by holder->field_20 == param_2
                if (param_1->holder->field_20 == param_2)
                    FUN_004bf4d0(entries->bc.surface, &rowRect, 0x1e);
                else
                    FUN_004bf4d0(entries->bc.surface, &rowRect, 0x1e);
            } else {
                FUN_004c13a0((int)col, (int)(font & 0xff));
            }

            yoff += step;
            line++;
            y++;
            h -= step;
            if (h < lh)
                return;
            if (line + me->bc.field_bc >= me->field_c0)
                return;
        }
    } else if ((flags & 0xa0) != 0) {
        // ---- cell-grid renderer ----
        int bp = (int)((flags >> 7) & 1);
        void* surf = entries->bc.surface;
        Rect_004a1b40 clip;
        ((Class_004c6ae0*)surf)->FUN_004c6ae0(&clip);
        ((Class_004c6b10*)surf)->FUN_004c6b10(bounds);

        int row = me->bc.field_bc;
        int* colPtr = 0;
        char* cellPtr = 0;
        if (bp == 0)
            colPtr = &me->cells[row];
        else
            cellPtr = (char*)me->cells + row * 0x18;

        int yy = top + 2;
        int x1 = left + 2;
        int yEnd = yy + step;

        for (;;) {
            void* cell;
            if (bp == 0) {
                cell = *(void**)(*colPtr + 0x28);
            } else {
                cell = cellPtr;
                cellPtr += 0x18;
            }
            if (cell != 0 && *(int*)((char*)cell + 0x10) != 0) {
                Quad_004a1b40 dst = {{{x1, yy}, {right, yy},
                    {right, yEnd - 1}, {x1, yEnd - 1}}};
                int width = *(unsigned short*)cell - 1;
                int height = *((unsigned short*)cell + 1) - 1;
                Quad_004a1b40 src = {{{1, 1}, {width, 1},
                    {width, height}, {1, height}}};
                FUN_004c7580(surf, cell, &dst, &src);
                Rect_004a1b40 rowRect = {x1, yy, right, yEnd - 1};
            if ((*((unsigned char*)me->field_d6 + row) & 1) == 0) {
                if ((*((unsigned char*)me->field_d6 + row) & 2) != 0) {
                    unsigned char c = param_1->colour_8be;
                    FUN_004be950(surf, x1 + 1, yEnd - 1, right - 2, yy + 1, c);
                    FUN_004be950(surf, x1 + 2, yEnd - 1, right - 1, yy + 1, c);
                    FUN_004be950(surf, x1 + 1, yy + 2, right - 1, yEnd - 2, c);
                    FUN_004be950(surf, x1 + 2, yy + 2, right - 2, yEnd - 2, c);
                }
            } else {
                FUN_004bf4d0(surf, &rowRect, -0x14);
            }

            }
            if ((me->flags & 0x100) == 0 && me->field_ba == row) {
                Rect_004a1b40 hl;
                hl.left = x1;
                hl.top = yy;
                hl.right = x1 + *(unsigned short*)cell - 1;
                hl.bottom = yy + *((unsigned short*)cell + 1) - 1;
                FUN_004bf4d0(surf, &hl, 0x14);
            }

            row++;
            if (bp == 0)
                colPtr++;
            yy += step;
            yEnd += step;
            if (yy >= bottom)
                break;
            if (row >= me->field_c0)
                break;
        }
        ((Class_004c6b10*)surf)->FUN_004c6b10(clip);
    }
}
