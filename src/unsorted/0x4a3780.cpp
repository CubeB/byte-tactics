// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by Space Bunny Free, rewritten by claude-opus-5-5, finished by DeepSeek V4.1 Flash, notes by claude-opus-5-5, checked by GPT-6. Names are provisional.
// GPT-6 retry (#5050): current check.py confirms 88.1% at 1832 bytes. The
// file's current notes capture the unresolved y1 lea, flags register, and 0x20
// loop allocation constraints.
// 2026-10-03 (claude-opus-5-5): no gain (88.1%), but the remaining gap now has
// one known cause. Scratch files and sweep specs: build/scratch/0x4a3780/.
//  - The 0x20 block's pointer choice tests flag8 itself: the original's `je`
//    at 0x4a3c4a reuses ZF from `and eax,1`. Written as `if (flag8) fixed = ...;
//    else ip = ...;`, MSVC also reproduces the original's "dead"
//    `mov esi,[esp+0x50]` in the fixed arm: it is the load of the uninitialised
//    `ip` from its home, which MSVC packs into obj's parameter slot because ip
//    is never stored. It is not an obj reload and not an original bug.
//  - But with `if (flag8)` the `flags` local gets a register (ecx) for its whole
//    range, the frame loses flags's slot (0x3c -> 0x38) and every [esp+N]
//    shifts: 65.7%. The `flags & 0x80` test below keeps the slot, which is the
//    only reason this file stays on that form.
//  - Probe: making flags live across the 0x10 path's FUN_004b6af0/strncmp calls
//    (`&& flags` added to the entries loop's condition; a semantic change, +4
//    bytes) spills flags, and then the frame, flag8 (0x1c), fixed (0x14), ip
//    (esi, home in obj's slot), n2 (edx), the in-place `shr eax,7` and the whole
//    0x20 loop come out exactly as in the original: 88.5% even with the extra
//    test, shape 97.4%. Only the y1 lea, the 0x10 clamp and the post-loop obj
//    register (esi across the join to the field_cca store) still differ there.
//  - Ablations of the `if (flag8)` form: removing the 0x20 loop, or leaving
//    flag8 one use instead of three (fixed rows only or item rows only), also
//    spills flags. So flags wins its register through flag8's loop uses (the
//    two share a register at the in-place shift); the missing fact is what
//    stops that in the original.
//  - Flat with `if (flag8)` (flags stays in a register, frame 0x38): every
//    combination of flags vs me->flags for its four uses; flags or the field
//    as unsigned/long/DWORD; union, struct, array and bitfield wrappers,
//    `*(int*)&flags`, memcpy, reference-taking inline helpers; flag8 as
//    unsigned/long/bool/char/short, `!= 0`, `?:`, `!!`, `(f & 0x80) >> 7`, a
//    bitfield cast, an inline IsFixed(); flag8 or n2 declared or initialised
//    earlier; every declaration position of flags; flag8/k/n2/remain merged
//    with n/i/size/step/flags; fixed merged into entries; inline helpers for
//    the 0x10 or 0x20 block; `flags = flags;` anywhere (removed early);
//    redundant `flags & 0x10` tests (folded); sequential ifs instead of the
//    else-if; 1024 combinations of ten 0x20-block spellings and 256 of eight
//    0x10-path spellings; headers.py (256 sets); 0-63 extern-int fillers;
//    permute.py from the `if (flag8)` form (12 min, best 79.1% via junk temps).
//  - (a) below, the y1 `lea`: every spelling compiles the same; it follows the
//    TU's type count. Without <windows.h> (own min macro) the y1 lea is right
//    but the three `bc + step - 1` leas flip (82.7%). With N dummy struct types
//    instead: N <= 26 82.7%, 28-32 87.5%, 34-50 87.9%, >= 52 88.1% (the
//    <windows.h> state); extern-int fillers change nothing.
// 2026-10-03 (Space Bunny Free): 86.6 -> 88.1%, size still exact (1832).
// Seven spelling changes: five in the 0x20 walk loop, one 16-bit compare in
// the 0x10 clamp, one operand order in the rect setup. Found by hill-climbing
// and a random search over axis combinations from the 86.6% file. None of them is an improvement on its own (the loop bound alone
// drops to 84.7% and the rest are flat); the set is worth 1.5 points:
//  - the loop bound is `if (k > me->field_c0 - 1) break;` (was `int lim =
//    me->field_c0; if (lim < k + 1) break;`). That is the original's own
//    `movsx ecx,[ebp+0xc0] / inc / inc / dec / cmp eax,ecx / jg`; the old form
//    hoists a `lea ecx,[eax+1]` out of the loop and tests with `jl`.
//  - `((unsigned)flags >> 7) & 1` for flag8, which gives the original's `shr`
//    (a signed `>>` gives `sar`; `(flags & 0x80) != 0` is the same);
//  - the pointer step comes before the counters
//    (`if (flag8) fixed++; else ip++; n2++; k++;`);
//  - `int n2 = 0;` is declared above `int remain = ...`;
//  - `r.y1 = r.y0 + me->field_19 - 1;` (operand order only);
//  - the 0x10 clamp is `if (me->field_ba > me->field_c0 - 2)`, which keeps the
//    size at 1832 (`>= c0 - 1` gives the original's `dec ecx`/`jl` but drops
//    the function to 1830 bytes, so the two extra bytes of `sub ecx,2` are
//    what the rest of the block is short of);
//  - with those, the original's arm order (fixed-row arm first, `flags & 0x80`
//    tested directly) is right again, and the loop compiles to the original's
//    instruction sequence apart from its registers.
//  `field_c6` is declared `void*` (the code treats it as a pointer); it makes
//  no difference to the bytes.
// What still differs, all measured:
//  (a) `lea eax,[ecx+ebx-1]` at the y1 computation, the original's base and
//      index are the other way round. Every spelling of that expression and
//      every header set leaves it (tools/headers.py swept all 256 sets: no
//      change, 86.6% from the old file, and the same `lea` is wrong in the
//      other direction without <windows.h>);
//  (b) `flags` is loaded into ecx, the original uses eax (three instructions,
//      the frame and the spill are already right);
//  (c) the 0x20 loop's registers: the original computes flag8 in place
//      (`shr eax,7 / and eax,1 / mov [esp+0x1c],eax`), spills it, and keeps ip
//      in esi (its flag8 arm has a dead `mov esi,[esp+0x50]`, so ip shares esi
//      with obj, which is also why obj stays in esi through the select check
//      and the field_cca store). Here flag8 is computed into edx through a
//      copy (`mov edx,ecx / shr edx,7`, 2 bytes more) and ip is spilled to
//      [esp+0x1c]. Testing flag8 for the pointer choice (`if (flag8)`) is what
//      makes the shift in place and flag8 memory-resident, but then `flags`
//      loses its frame slot (the frame drops to 0x38 and the whole function to
//      65.7%): MSVC then uses `mov si,[ebp+0xbc]` in the 0x10 block instead of
//      `mov cx`, so nothing clobbers flags and no spill is needed. Nothing in
//      the 0x10 or 0x20 block's spelling brings that spill back with
//      `if (flag8)`: 64 combinations of the clamp/test spellings, every
//      declaration order, one shared pointer instead of two, a union member,
//      a helper for the pointer choice, two separate loops, a pointer-to-flag8,
//      a `goto` loop, and random searches over 24 axes (1789 and 2233 samples
//      with 1-4 mutations, plus 1937 permute.py candidates from this file)
//      all stay at 65-70% or keep ip in memory;
//  (d) `mov esi,[esp+0x50]` and `mov [esi+0xcca],1` (we reload obj into eax),
//      downstream of (c);
//  (e) `mov eax` vs `mov edx` for the index reload before FUN_004a1b40.
// 2026-10-02 (DeepSeek V4.1 Flash): 84.3% -> 86.6%, size now exact (1832).
// Two layout levers, both measured, moved the 0x20 block's register allocation
// and a few branch targets:
//  - The 0x20 loop's bottom test is `int lim = me->field_c0; if (lim < k + 1)
//    break;`. The `c0` bound in a local with `k + 1` on the other side makes
//    MSVC put the counter in ecx (`lea ecx,[eax+1]` before the loop, `inc ecx`
//    at the bottom) and the row pointer in eax; the old `if (k > c0 - 1)` left
//    them in the other registers. 84.3 -> 86.2.
//  - The first `FUN_004ab570` arm's `if (me->field_c0 != 0) { ... }` is written
//    as an early exit, `if (me->field_c0 == 0) goto skip0; ... skip0:;`, which
//    is how the original lays that guard out (`je` past the body). 86.2 -> 86.6.
// What still differs, unchanged from the note above: (a) flags loads into ecx
// not eax; (b) the 0x20 loop keeps flag8 in edx and spills ip to [esp+0x1c]
// where the original spills flag8 and keeps ip in esi (with n2/row/k in the
// matching rotation); (c) the y1 `lea` base/index order; (d) the two scroll
// tails' temp rotation, all downstream of (b). The exact 0x20 register map was
// not reachable: every spelling of flag8/ip (types, scopes, pointer-to-flag8
// forcing memory, a `char*` walk, one more use of either) either leaves the
// map unchanged or changes it globally and drops the score. The loop bound was
// the one knob that moved it.
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
    r.y1 = r.y0 + me->field_19 - 1;
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
                if (me->field_ba > me->field_c0 - 2)
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
            int flag8 = ((unsigned)flags >> 7) & 1;
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
