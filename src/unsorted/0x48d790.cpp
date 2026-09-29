// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5. Names are provisional.
// PARTIAL: 89.6% (380 of 388 bytes; was 67.6%, 385 bytes). Selects the next unit
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
        for (u++; u <= t->end; u++) {
            if ((u->u.flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0) {
                Owner_0048d790* owner = u->owner;
                if (owner == 0 || (owner->flags & 0x40000000))
                    break;
            }
        }
        if (u <= t->end) {
            u->u.flags |= flag;
            g_game->field_37e9c = 0;
        } else {
            found->u.flags |= flag;
        }
        found->u.flags |= flag;
    }
    g_game->flags |= flag;
}
