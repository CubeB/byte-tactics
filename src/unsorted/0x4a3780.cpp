// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free, rewritten by claude-opus-5-5, finished by DeepSeek V4.1 Flash, notes by claude-opus-5-5, checked by GPT-6, improved by claude-opus-5-5. Names are provisional.
// 2026-10-03 (claude-opus-5-5, #5158): 88.1% -> 93.4%, 1830 of 1832 bytes.
// GPT-6 retry (#5177): rechecked the 93.4% source; the remaining object
// register split at the 0x40 return join is unchanged.
// One difference is left (below); ignoring the jump targets it moves, the
// function is 98.8%. Scratch files, sweep scripts and a C2 split tracer:
// build/scratch/0x4a3780/ (c2split.py is tools/c2prio.py plus hooks on
// FUN_00439385/FUN_00438f79; patch_c2split.py builds it).
// What changed, each measured:
//  - The flag8 block declares `k` after the pointer choice and indexes the
//    rows with me->field_bc directly. With `int k = me->field_bc;` before the
//    `if (flag8)`, the flags local (whose last use is the in-place `shr`)
//    has priority 47 in C2 (tools/c2prio.py), takes ecx for its whole range
//    and the frame loses its slot. With k after the arms its priority is 23:
//    it loses ecx to the 0x10 block's field_bc temp and ebx to r.y0 and is
//    split into an eax piece plus its [esp+0x20] home, as in the original.
//    The pointer choice now tests flag8 itself (the original's `je` reuses
//    the flags of `and eax,1`), and the loop allocation comes out exactly:
//    flag8 and fixed in memory, ip in esi (with the load of the
//    uninitialised ip in the fixed arm), n2 edx, row ecx, remain edi.
//    88.1 -> 93.1.
//  - `int remain` before `int n2 = 0;` (the xor after both subs), and the
//    0x10 clamps written plainly: `>= field_c0 - 1` (the original's dec/jl)
//    and `if (me->field_ba < 0) me->field_ba = 0;`. The old short* store and
//    the `> c0 - 2` size filler are gone. 93.2.
//  - The rectangle is FUN_004a1630 (matched, src/unsorted/0x4a1630.cpp),
//    defined here without an annotation and inlined, called as
//    `FUN_004a1630(&entries[index], &r)` the way the matched sibling 0x4a1b40
//    calls it. With `me` as the argument the y1 lea stays `[ebx+ecx-1]`.
//    That lea, and the three `field_bc + step - 1` leas, follow how many
//    symbols the front end has created before `r` and before `step`:
//    dummy declarations inserted after `Rect r;` fix the y1 lea, before it
//    they break it, and no file-level count fixes all four. 93.4.
//  - Neutral, kept because the siblings use them: the LineHeight helper with
//    a glyph struct (0x4a1b40), `unsigned int flags` (0x4a1b40), and the
//    callee prototypes from the callees' own matched files.
// What still differs: obj is not kept in esi across the join after the
// callbacks. The original has `jmp` + `mov esi,[esp+0x50]` on the
// orig_sel == field_ba edge and `mov [esi+0xcca],1`; ours reloads obj into
// ecx at the cca store, which also shifts two scratch registers in the scroll
// paths (cx/dx, dx/ax) through the temporary rotation. Cause, traced in C2:
// in the second joint live-range split (FUN_00438f79) the ret1 block (shared
// by the three first-arm exits and the 0x40 test) joins the region of its
// first compatible predecessor, the first arm's `!(flags & 0x200)` block.
// The 0x40-test block is in another region, so a split point is inserted at
// its end, obj's web is cut there and the cca block's piece stays in memory.
// Confirmed both ways: with the first arm returning 1 itself (ret1 reached
// only from the 0x40 test) the join is byte-identical to the original but
// three extra epilogues cost 37 bytes; an obj use after the 0x40 test also
// gives esi. Flat: ret1 placed in the first arm, at the end of the function
// or behind a trampoline label; nested first-arm forms; a short c0 local;
// `if (!(flags & 0x40)) { cca; goto end; }`; entries[index] spellings in the
// tail; the callback through a local; /Gi; struct and extern count scans;
// permute.py (two 20-minute runs).
// Earlier findings that are still load-bearing:
//  - `if (me->field_c0 == 0) goto skip0;` with skip0 at the end of the
//    in-rect block: the original reloads point.x on that edge only (the
//    nested `if (c0 != 0) {...}` form puts the reload on the out-of-rect
//    edges too, 92.9%).
//  - The scroll-up/scroll-down tails share one FUN_004a1b40/FUN_004a2be0 pair
//    through `goto finish`; the three first-arm `return 1`s are `goto ret1`
//    (MSVC 5 does not merge return blocks).
//  - The 0x10 line computation stores straight into me->field_ba and tests
//    the field (16-bit add, `movsx edx,cx`); the FUN_004b6af0 result goes
//    through `char* s`; the sync loop clamp is min() (entries in esi).
//  - The 0x20 test is `flags & 0x20 | 0x80`, an original bug kept as is: it
//    parses as `(flags & 0x20) | 0x80` and is always true (docs/bugs.md).
#include <string.h>
#include <windows.h>

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

extern Holder_004a3780* DAT_0051fba4;
extern char DAT_00502a20[];

void __stdcall FUN_004c1420(int id);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
char* __stdcall FUN_004b6af0(char* text, int n);
int FUN_004b6340();
int __stdcall FUN_004ab570(Object_004a3780* obj, unsigned char buttons);
int __stdcall FUN_004ab510(Object_004a3780* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_004a3780* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_004a3780* obj, int param_2);
void __stdcall FUN_0049fc50(Object_004a3780* obj, int index);
void __stdcall FUN_004a1b40(Object_004a3780* obj, int index);
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

void __stdcall FUN_004a1630(Entry_004a3780* entry, Rect_004a3780* rect)
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
    if (0 == DAT_0051fba4->list)
        return FUN_004c1450();
    return ((Glyph_004a3780*)FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49))->height + 2;
}

// FUNCTION: 0x4a3780
int __stdcall FUN_004a3780(Object_004a3780* obj, int index, int param_3)
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
    FUN_004a1630(&entries[index], &r);
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
                FUN_004c1420(entries[i].field_d6);
                break;
            }
            n++;
        }
    }
    if (i == entries[0].count + 1)
        FUN_004c1420(DAT_0051fba4->current);

    int size = LineHeight_004a3780();
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    char* s;

    if (FUN_004ab570(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            if (me->field_c0 == 0) goto skip0;
            if (!(me->flags & 0x200))
                goto ret1;
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba < 0)
                goto ret1;
            if (me->field_ba - me->field_bc > step - 1)
                me->field_ba = me->field_bc + step - 1;
            if (me->field_ba >= me->field_c0 - 1)
                me->field_ba = me->field_c0 - 1;
            s = FUN_004b6af0(me->field_c2, me->field_ba);
            if (strncmp(DAT_00502a20, s, 2) != 0)
                goto ret1;
            me->field_ba = orig_sel;
            return 0;
skip0:;
        }
    } else if (FUN_004ab510(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 1);
        }
    } else if (FUN_004ab510(obj, 2)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            FUN_0049fc50(obj, index);
            FUN_004ab690(obj, 2);
        }
    }

    if (obj->focus != index)
        goto end;
    if (!FUN_004ab5b0(obj, 3))
        obj->focus = -1;
    if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
        obj->holder->field_20 = index;
        flags = me->flags;
        if (flags & 0x10) {
            me->field_ba = (point.y - r.y0) / span + me->field_bc;
            if (me->field_ba >= 0) {
                if (me->field_ba - me->field_bc > step - 1)
                    me->field_ba = me->field_bc + step - 1;
                if (me->field_ba >= me->field_c0 - 1)
                    me->field_ba = me->field_c0 - 1;
                if (me->field_ba < 0)
                    me->field_ba = 0;
                if (flags & 0x200) {
                    s = FUN_004b6af0(me->field_c2, me->field_ba);
                    if (strncmp(DAT_00502a20, s, 2) == 0)
                        me->field_ba = orig_sel;
                }
                for (i = 1; i <= entries[0].count; i++) {
                    if (entries[i].type == 2 && entries[i].kind == me->kind)
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
            int k = me->field_bc;
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
            FUN_004a1b40(obj, index);
            if (me->field_ce)
                me->field_ce(obj, me);
        }
        if (me->flags & 0x40) {
ret1:
            return 1;
        }
        obj->field_cca = 1;
    } else if (point.y < r.y0) {
        if (me->field_bc > 0 && me->field_b6 < FUN_004b6340()) {
            me->field_b6 = FUN_004b6340() + 2;
            if (me->field_ba > me->field_bc)
                me->field_ba = me->field_bc;
            me->field_bc--;
            me->field_ba--;
            short sel = me->field_ba;
            if (me->field_c2 != 0) {
                s = FUN_004b6af0(me->field_c2, sel < 0 ? 0 : sel);
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
        if (me->field_bc < me->field_be && me->field_b6 < FUN_004b6340()) {
            me->field_b6 = FUN_004b6340() + 2;
            me->field_bc++;
            me->field_ba = me->field_bc + step - 1;
            if (me->field_c2 != 0) {
                s = FUN_004b6af0(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, s, 2) == 0)
                    me->field_ba = orig_sel;
            }
finish:
            FUN_004a1b40(obj, index);
            FUN_004a2be0(obj, index);
        }
    }
end:
    return obj->field_60 != -1;
}
