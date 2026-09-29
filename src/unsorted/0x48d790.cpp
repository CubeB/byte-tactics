// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5. Names are provisional.
// PARTIAL: 92.2% (392 bytes against the original's 388; was 89.6% / 380).
// Selects the next unit
// of the current team: finds the first eligible unit (flag 0x20, float 0, field_fb 0,
// owner absent or owner bit 30), and when that unit already has the 0x10 bit clears
// the bits (0xffffff2f) of every unit of the global list, calls FUN_00491d70(0) and
// scans on from it for the next eligible unit (which gets 0x10 and clears
// field_37e9c), falling back to the first one; the game flag word gets 0x10 in
// every case. The first scan tests `owner->flags & 0x40000000`, which MSVC narrows
// to a byte test on its own; the 0x10 bit is a 1-bit bitfield (`shr ecx,4; test cl,1`).
//
// Claude Sonnet 5.5 pass (#744):
//  * Solved: the team pointer. Forming `Team* t = &g_game->teams[idx]` FIRST and
//    reading `begin` and `end` through `t` gives the original's
//    `lea ebp, [edx+eax*2+0x1b63]` and `[ebp + 0x6b]` (80.6%); reading them from
//    g_game and taking `t` separately gave the bare-base lea.
//  * Second scan: the loop with the found-block breaking out and a test of
//    `u <= t->end` after it (the current form) is 89.6%. What is still different: the
//    original lays the hit block (`or [esi+0x110]`, `field_37e9c = 0`, its own copy
//    of the `g_game->flags |= flag` tail and the `ret`) directly after the loop and
//    ends the loop with `ja exit; jmp top`, with no re-test after it; ours re-tests
//    `cmp esi, ecx; ja` after `jbe top`, jumps back to a shared tail, and puts the
//    `found->flags |= flag` block after the hit. The versions with `return` inside the
//    loop (80.6%) put the hit block after the function's last `ret` and re-load the
//    0x10 into edx there.
//  * Tried without effect or worse: a `continue` form, a while loop with the
//    increment at the bottom or in else branches (64.5%), a do-while with a guard
//    (67.9%), `for (;;)` with the end test inside (81.1%), the end test duplicated at
//    the bottom (61.9%), the end read into a local (80.6%), an early return for
//    `found == 0` (53.2%), gotos with a shared or an inlined tail (66.7 to 85.2%), the
//    arms of the final test swapped or returning (86.9 to 89.0%), and 25 forms of the
//    team pointer (all 67.6%, only the one above helped). The declaration-count
//    sweep (0 to 400) is flat at the old 67.6% and headers.py gives nothing beyond it,
//    so this is source shape, not compiler state.
//
// space-bunny-free pass (issue 1247):
//  * Solved: the shape of the second scan, which is what the "two exits in the other
//    physical order" residual above was about. It is NOT a for/while with the end test
//    at the top. The original tests `esi <= end` once before the loop and again at the
//    bottom, and both exits jump PAST the hit block, so the source is a guard plus a
//    do-while whose hit arm RETURNS from inside the loop:
//        u++;
//        if (u <= t->end) { do { if (eligible) { ...; return; } u++; } while (u <= t->end); }
//    That reproduces the original's `ja exit; jmp top` bottom edge, the hit block placed
//    between the loop and the fallback, and its own pop/ret epilogue, with the found
//    path falling through to the shared tail. 89.6% (380 bytes, 2 wrong jump forms and
//    a store to the wrong unit) to 92.2% (392 bytes). The earlier "return inside the
//    loop" attempts above failed because they were `for` loops, whose bottom edge is
//    `jbe top`; only the guard plus do-while gives the `ja` plus `jmp` pair.
//    The note above about the team pointer coming out as the bare base is stale: the
//    body already forms `t` first, and `lea ebp,[edx+eax*2+0x1b63]` matches.
//  * What still differs, verified from the bytes: only the register allocation of the
//    hit block, and the two halves of it are one decision. The original runs
//        mov eax,[esi+0x110] / or eax,edx / mov [esi+0x110],eax
//    then loads g_game into eax, stores field_37e9c through eax, and reloads g_game
//    into EDI for the flag word. We run the load-or-store in EDI, rematerialise the
//    constant with a second `mov edx,0x10` (5 bytes, inside a loop, so it is a fresh
//    copy of the 0x10 already in edx), and reuse eax for the second g_game load. The
//    5-byte remat and the eax/edi swap cancel to 4 bytes over. Note the two g_game
//    loads are NOT CSEd in either version, so this is not an aliasing question: it is
//    which of the two dead registers the allocator spends first, and MSVC 5 answers
//    edi here and eax in the original.
//  * Tried on that block, all still 92.2% / 392: the constant as a literal only in the
//    hit block, only in the tails, or everywhere (no `flag` variable at all); the
//    variable as unsigned int, int, short, const unsigned short, declared at the top
//    and assigned after the first loop; the |= as `x = x | flag` and as an explicit
//    temporary; the flags read through a second struct member at the same address in
//    the hit block (91.2% when the tail does the same), a cast through (char*)g_game,
//    the field store as `(short)0` and through a local pointer, the owner local as
//    const and the test written with no local at all, the eligibility chain as nested
//    ifs and as one expression, the guard written as if/else with the fallback first
//    (53.8%), and hit/fallback statements reordered (those two permutations lose the
//    remat and land at 387 bytes but reshuffle every register in the function).
//    The block is on a knife edge: the reorders and the fallback-first form change the
//    whole function's allocation, so the tie may still flip from a shape further away.
//  * The body this replaces stored `found->u.flags |= flag` a second time after the
//    if/else, which both emitted a store the original does not have and set 0x10 on the
//    first eligible unit as well as on the one the second scan found. The return form
//    drops it, so the file no longer changes behaviour.
//  * Not a bug in the original, but worth knowing: the `u++` before the second scan is
//    unconditional, so when the first loop ran off the end of the list (u already one
//    past `end`) the scan starts two past it and can never fire. The "next eligible
//    unit" arm is reachable only when the first loop broke on the 0x10 bit.
//
// deepseek-v4.1-flash wall check (issue 1252):
//  * WALL CONFIRMED as a callee-saved register rotation, not a reference error. The
//    original hit-block sequence `mov eax,[esi+0x110] / or eax,edx / mov [esi+0x110],eax`
//    (bytes 8B 86 10 01 00 00 0B C2 89 86 10 01 00 00) occurs exactly once in
//    TotalA.exe, at file offset 0x8ccc6 = VA 0x48d8c6 (the hit block itself). headers.py
//    is flat at 92.2% for all 128 header sets. Free scratch variants that kept 92.2% and
//    the same 392 bytes: named temp `f = u->u.flags | flag`, `u->u.flags = u->u.flags |
//    flag`, a `short f = flag` copy, and a `p = u` pointer alias. Field-store-first drops
//    to 71.3%. The residual is exactly the allocator choosing EDI for the OR temp and
//    rematerialising edx=0x10, where the original spends EAX first and keeps edx live
//    from 0x48d866. Nothing in the source shapes tried flips that tie.
#pragma pack(push, 1)

// The object at +0x86 of a unit. Bit 30 of the flags dword at +0x110 is the
// same bit the first scan tests as byte [owner+0x113] & 0x40.
struct Owner_0048d790 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Unit_0048d790 {
    char unknown_0[0x86];
    Owner_0048d790* owner;             // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    union {
        unsigned int flags;                            // +0x110
        struct {
            unsigned int low : 4;
            unsigned int bit4 : 1;                     // the 0x10 bit
            unsigned int high : 27;
        } bits;
    } u;
    char unknown_114[0x118 - 0x114];
};

struct Team_0048d790 {
    char unknown_0[0x67];
    Unit_0048d790* begin;              // +0x67
    Unit_0048d790* end;                // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048d790 {
    char unknown_0[0x1b63];
    Team_0048d790 teams[10];           // +0x1b63, 0x14b each
    char unknown_2a43[0x2a43 - (0x1b63 + 10 * 0x14b)];
    unsigned char field_2a43;
    char unknown_2a44[0x14357 - 0x2a44];
    Unit_0048d790* list_begin;         // +0x14357
    Unit_0048d790* list_end;           // +0x1435b
    char unknown_1435f[0x37e9c - 0x1435f];
    short field_37e9c;                 // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags;              // +0x37ebe
};
#pragma pack(pop)

extern Game_0048d790* g_game;

int __stdcall FUN_00491d70(int force);

// FUNCTION: 0x48d790
void __stdcall FUN_0048d790(void)
{
    Unit_0048d790* found = 0;
    Team_0048d790* t = &g_game->teams[g_game->field_2a43];
    Unit_0048d790* u = t->begin;
    Unit_0048d790* last = t->end;

    for (; u <= last; u++) {
        if (u->u.flags & 0x20) {
            if (u->field_104 == 0.0f && u->field_fb == 0) {
                Owner_0048d790* owner = u->owner;
                if (owner == 0 || (owner->flags & 0x40000000)) {
                    if (found == 0) {
                        found = u;
                    }
                    if (u->u.bits.bit4) {
                        for (Unit_0048d790* q = g_game->list_begin;
                             q <= g_game->list_end; q++) {
                            q->u.flags &= 0xffffff2f;
                        }
                        FUN_00491d70(0);
                        break;
                    }
                }
            }
        }
    }

    unsigned short flag = 0x10;
    if (found != 0) {
        u++;
        if (u <= t->end) {
            do {
                if ((u->u.flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0) {
                    Owner_0048d790* owner = u->owner;
                    if (owner == 0 || (owner->flags & 0x40000000)) {
                        u->u.flags |= flag;
                        g_game->field_37e9c = 0;
                        g_game->flags |= flag;
                        return;
                    }
                }
                u++;
            } while (u <= t->end);
        }
        found->u.flags |= flag;
    }
    g_game->flags |= flag;
}
