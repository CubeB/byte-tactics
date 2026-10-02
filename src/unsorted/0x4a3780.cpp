// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free, rewritten by claude-opus-5-5. Names are provisional.
// 2026-10-02 (claude-opus-5-5): 67.6% -> 84.3%, exact size (1830 of 1832 bytes),
// rewritten from scratch as plain structured code. The old file's goto web, its
// `if (i < 1) break;` nudge and its `(unsigned short)` addend are all gone:
// written naturally, MSVC already gives the original's prologue and first-loop
// allocation (point.x in edi then stored, the counter in esi, point.y in edi).
// What the rewrite established, each measured:
//  - Both line computations are `me->field_ba = (point.y - r.y0) / span +
//    me->field_bc;` followed by tests on me->field_ba itself: that gives the
//    original's `mov cx,[ebp+0xbc] / add eax,ecx / test ax,ax` (a 16-bit add)
//    and the later `movsx edx,cx`.
//  - The 0x20/0x80 block builds only the pointer its mode needs
//    (`if (...) fixed = ...; else ip = ...;`) and steps only that one, as the
//    original does; `remain -= span` / `remain -= row->height` are two
//    statements (the original subtracts in both arms). `flags & 0x20 | 0x80`
//    is the original's always-true test (`and ecx,0x20 / or cl,0x80`).
//  - The scroll-up/scroll-down tails share one FUN_004a1b40/FUN_004a2be0 pair
//    through `goto finish`; with separate copies MSVC merges the two
//    `me->field_ba = orig_sel` restores, which the original keeps apart. The
//    three early `return 1`s need `goto ret1` (MSVC 5 does not merge identical
//    return blocks and the original has one).
//  - The FUN_004b6af0 result goes through a `char* s` local, so `push 2` comes
//    after the call as in the original.
//  - The second 0x10 loop's clamp is `min(...)` (windows.h): the
//    if-form keeps entries in ebx, the ternary/min puts it in esi and the walk
//    pointer in eax as the original does.
//  - `flags` lives in its own stack slot in the original ([esp+0x20], stored
//    after `test al,0x10`, reloaded for `test ah,2`). Two things together make
//    MSVC do that here: one of the 0x10 block's field_ba stores goes through a
//    `short*` (the `< 0` clamp below; any of the three clamp stores works, the
//    first store does not), and the 0x20 block's pointer choice tests
//    `flags & 0x80` rather than flag8. Either one alone leaves flags in ecx
//    (65-73%). Testing flag8 is what the original does (its `je` reuses the
//    flags of `and eax,1`), so this costs one `test cl,0x80`.
// What still differs:
//  (a) flags is loaded into ecx, the original uses eax (3 instructions, 1 byte).
//  (b) the 0x20 loop: the original spills flag8 to [esp+0x1c] and keeps ip in
//      esi (its flag8 arm has a dead `mov esi,[obj]`, so ip shares esi with
//      obj, which is also why obj stays in esi through the select check and the
//      field_cca store). Here flag8 is in edx and ip is spilled to [esp+0x1c].
//      Every spelling of the block (flag8 as shr/!= 0, ternary or if/else row,
//      increment order, declaration order, flag8 or ip at function scope, a
//      struct-member flag8, step reused as flag8) is flat or worse.
//  (c) `lea eax,[ecx+ebx-1]` at the y1 computation comes out `[ebx+ecx-1]` with
//      <windows.h> (it is right without it, but min() and the 0x10 loop need it).
//  (d) scroll-up's orig_sel restore uses dx (cx in the original) and
//      scroll-down's bc compare uses ax (dx): temp rotation, downstream of (b).
// LEAD, not taken: testing the saved `flags & 0x40` instead of re-reading
// `me->flags & 0x40` after the callbacks (a semantic change: the callback
// could change me->flags) also spills flags, without the short* store or the
// `& 0x80` test, and scores 84.9%. permute.py from this file found only noise
// (a reordered in-rect `&&` chain, 85.4%).
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
    void* field_c2;                    // +0xc2
    int field_c6;                      // +0xc6
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
void* __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);
char* __stdcall FUN_004b6af0(void* text, int line);
int FUN_004b6340();
int __stdcall FUN_004ab570(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab510(Object_004a3780* obj, int mask);
int __stdcall FUN_004ab5b0(Object_004a3780* obj, int mask);
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
    int flags;
    if (me->type == 0) {
        r.x0 = 0;
        r.y0 = 0;
    } else {
        r.x0 = me->field_13;
        r.y0 = me->field_15;
    }
    r.x1 = me->field_17 + r.x0 - 1;
    int n = 0;
    r.y1 = me->field_19 + r.y0 - 1;
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

    int size = (DAT_0051fba4->list == 0) ? FUN_004c1450()
        : (*(unsigned short*)((char*)FUN_004b7f30(DAT_0051fba4->list->field_0c, 0x49) + 2) + 2);
    short da = me->field_da;
    int span = (da != 0) ? da : size + 1;
    int step = (me->field_19 - 2) / span;
    char* s;

    if (FUN_004ab570(obj, 1)) {
        if (point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 && point.y <= r.y1) {
            if (me->field_c0 != 0) {
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
            }
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
                short* sel = &me->field_ba;
                if (*sel < 0)
                    *sel = 0;
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
            int flag8 = (flags >> 7) & 1;
            int k = me->field_bc;
            Row_004a3780* fixed;
            Item_004a3780** ip;
            if (flags & 0x80)
                fixed = &((Row_004a3780*)me->field_c6)[k];
            else
                ip = &((Item_004a3780**)me->field_c6)[k];
            int n2 = 0;
            int remain = point.y - r.y0 - 2;
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
