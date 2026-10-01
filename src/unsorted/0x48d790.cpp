// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, verified by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// Space Bunny Free pass (issue #4398): best stays 92.6% (387 of 388 bytes), no
// MATCH, and the permuter confirms 92.6% is flat (2786 candidates, nothing
// better). The residual is still one allocator decision, but the two halves of
// it are now separated, and the leading half is understood:
//  * The hit block's three temps. The original allocates (EAX, EAX, EDI) for
//    (the unit value, the first g_game, the second g_game) and reads the mask
//    from the live EDX of 0x48d866. This build allocates (EDI, EAX, EAX) and
//    re-materialises the mask as `mov edx, 0x10` inside the block. I confirmed
//    with a micro test (the whole function, compiled and disassembled in a few
//    seconds per variant) that the block is byte-identical, remat included,
//    across about fifty spellings: the mask as int/short/char/long/unsigned
//    long/const/register and as a literal; `|=` vs `= x | f`; an explicit
//    temporary; the 1-bit bitfield vs the dword field; a `p = u` alias; a
//    `&u->flags` pointer, a `&g->field_37e9c` pointer, `(short*)((char*)g_game
//    + 0x37e9c)` and `gs[0x1cfd3]` offset forms, two separate g_game locals, a
//    `& 0xffff` mask, a zero-extended mask, a second 32-bit mask, an inlined
//    helper returning 0x10, an inlined helper holding the three hit-block
//    statements, the eligibility test as a helper, the loop as
//    do-while+guard / while / for(;;) / for(;;)+break, the loop bound through a
//    local, and the first and clear loops as for/while/do-while.
//  * The order of the two game writes, and the number of g_game loads. This is
//    the part earlier notes did not have. The original does NOT hoist the
//    `flags |=` above the `field_37e9c = 0` store, and it loads g_game TWICE
//    (once into eax for the store, once into edi for the RMW). This build does
//    both, because when both writes go through one tracked pointer (`Game* g =
//    g_game; g->field_37e9c = 0; g->flags |= flag;`) MSVC 5 proves the two
//    fields are disjoint, commutes the RMW up and merges the two loads. Writing
//    the store through a local the compiler does not propagate and the RMW
//    through the global reproduces the original exactly:
//        Game_0048d790* g = g_game;
//        g->field_37e9c = 0;
//        g_game->flags |= flag;
//    That gives the original's statement order and its two loads, and 92.2%
//    (392 bytes), the hit block becoming
//        mov edi,[esi+0x110] / mov edx,0x10 / or edi,edx / mov [esi+0x110],edi
//        mov eax,[0x511de8] / mov word [eax+0x37e9c],0
//        mov eax,[0x511de8] / or word [eax+0x37ebe],dx
//    which is the original's block with EAX and EDI swapped and the remat still
//    there. So that pointer split is the right shape; the file keeps the 92.6%
//    version because the swap costs it a point.
//  * The remat is not a source-shape choice. MSVC 5 rematerialises the mask
//    inside the hit block whenever the mask is a local constant live out of the
//    `test ebx,ebx; je` branch, and stops rematerialising only when the value is
//    not live there (assign it inside `if (found != 0)`), which then homes the
//    variable in a stack slot (`mov edx,[esp+0x10]` on the found==0 path),
//    rotates the saved set to `pop ecx` and costs 407 bytes. Also measured: the
//    mask used only inside the loop (remat), the def at four positions around
//    the loop guard (remat), a two-arm phi (remat), a mask derived from the
//    clear-loop mask `~(flag | 0x20)` (remat), the mask `register` (remat).
//  * The hit block's registers are decided locally, not by a function-wide
//    rotation: swapping the first loop's `found == 0` and bit4 tests, swapping
//    the eligibility sub-conditions and changing the first and clear loop forms
//    all leave it byte-identical. That also confirms the "callee-saved rotation
//    set" theory in the older notes is wrong for this function.
//  * The guard plus do-while is required: `while` and `for` forms put the hit
//    block after the function's last `ret`, as the older notes say.
// mimo-v2.6-pro retry (issue #3353): best stays 92.6% (387 of 388 bytes), no
// MATCH. The single residual is unchanged: the hit block rematerialises the
// mask (`mov edi,[esi+0x110]; mov edx,0x10; or edi,edx`) where the original
// keeps EDX live from 0x48d866 and spends EAX on the unit RMW, then EAX/EDI on
// the two g_game reloads. Measured this pass: the RMW spelling (`|= flag` vs
// the bitfield) and the mask source (literal 0x10, unsigned short, int, the
// register keyword, declare/assign split) are all byte-neutral within each
// tail shape; the one-load alias tail (this file) is 387 bytes, the two-load
// direct g_game tail is 392 at 92.2% with the same remat plus an eax/edi swap
// in the reloads. The hit block remats in every structure tried: goto to a
// label after the loop (both fall-through and miss-label forms), a `second`
// pointer set in the loop and tested after (63.1%), if/else arms with a
// duplicated `g_game->flags |= flag` at both arm tails (67.3%, MSVC does not
// tail-merge it the way 0x499200 does), the mask as an inline helper
// parameter at all four use sites, struct methods taking the mask, and
// __cdecl/__fastcall on the function itself. headers.py is flat at 92.6% for
// all 128 header sets. The fallback and shared tail keep EDX live in all
// shapes, so the split is specific to the block reached from inside the second
// scan loop; only the z1 shape (hit block outside the loop) removes the remat,
// and it folds both uses to immediates and rotates the callee-saved set.
// #3225 retry by GPT-6.1-sol: best remains 92.6% (387 of 388 bytes), no MATCH.
// A whole-hit helper and reordering the unit RMW before loading the game pointer preserved the same single diff. Hoisting a game alias to function entry dropped to 53.2%; a parent early-mask declaration trial also stayed at 92.6%. The unresolved hit block still uses EDI for the unit RMW and rematerializes 0x10 in EDX; original uses EAX and the pre-existing EDX mask, then EAX/EDI for the game updates.
// #2862 retry by deepseek-v4.1-flash: local `g = g_game` in the hit block
// reuses one g_game load for both writes, giving 92.6% (387 of 388 bytes).
// Still differs: the hit RMW keeps EDI plus a `mov edx,0x10` rematerialization
// where the original uses EAX and keeps edx live from 0x48d866; and the tail
// reuses EAX instead of reloading g_game into EDI. See the long notes below.
// #3030 retry by deepseek-v4.1-flash: kept the 92.6% best (v0 below). The
// residual is a single allocator decision in the hit block, and it is not
// source reachable. Measured this pass: on the two-load shape (direct
// `g_game->field_37e9c = 0; g_game->flags |= flag;`) the hit block rematerialises
// the mask (`mov edi,[esi+0x110]; mov edx,0x10; or edi,edx; ...`) because the
// hit arm RETURNS, so EDI (the g_game home loaded at 0x48d85e for the found==0
// path) is dead there and the allocator spends it on the RMW temp; the original
// spends EAX and keeps EDX live, which leaves EDI free to be reloaded with
// g_game for the flags OR. Swapping operands, casts, bitfield, int/short/register
// mask, pointer aliases, `|=` vs `|`, an inlined helper taking the mask (hit-only
// and whole-hit-block) all compile identically to the 92.2% / 392 shape. A dummy
// `extern int` declaration sweep (0..400, step 2) is flat at 92.2 on that shape
// and flat at 92.6 on v0, so this is compiler state, not source shape; the
// allocator tie persists into all tested states.
// #2403 retry by GPT-6.1-sol: six worker checks kept the 92.2% best. Narrow
// helper variants kept the same register mismatch or fell to 71.3%; previous
// mismatch notes below are retained. No MATCH.
// #1612 retry by Codex / GPT-6.1-sol: checkall reconfirmed 92.2% (392/388 bytes), no MATCH.
// Three worker checks found no better source; the bitfield variant kept the same register mismatch.
// GPT-6.1-sol rechecked this best at 92.2% (392 bytes, two runs). A bitfield
// assignment for the second-scan hit path leaves the same diff: the original
// uses EAX for the flags OR and EDI for the later g_game load, while this build
// uses EDI for the OR, rematerializes 0x10 in EDX, then uses EAX for g_game.
// The remaining branch targets shift by four bytes because of that extra
// instruction. The saved candidate is the best-scoring source.
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
//
// space-bunny-free pass (issue 1883, second visit, measured with /Fa):
//  * The recorded "callee-saved rotation set" wall is WRONG for this function.
//    The original pushes and pops ebx, ebp, esi, edi, in that order, and so
//    does this build, so there is no rotation to reproduce. z1 below is the
//    only shape found that changes the set (it pops ecx instead of ebx), and
//    it is provably not the original.
//  * The residual is ONE decision, not a rotation: does the 0x10 mask copy
//    written at 0x48d866 stay live in EDX inside the hit block? The original
//    says yes (`or eax, edx`, then `or word ptr [edi+..], dx`); this build
//    re-materialises it (`mov edx, 0x10` as the block's first instruction) and
//    that extra definition is the whole 4 bytes. It also shifts the scratch
//    rotation by one, which is why the container lands in EDI and both g_game
//    loads in EAX here, against EAX/EAX/EDI in the original.
//  * Why the original's allocation is what it is: in that block ESI (u), EBX
//    (found) and ECX (t->end) are live, so only EAX and EDI are free for the
//    three temps. With the mask pinned in EDX the original's EAX, EAX, EDI is
//    exactly that; with the mask rematerialised the first def inside the block
//    consumes a rotation step and the rest slides to EDI, EAX, EAX.
//  * Measured on the block alone: trimmed to one temp (only the unit RMW) this
//    build already puts the container in EAX, the original's choice, and folds
//    the mask to `or al, 16`. With two temps it rematerialises and moves the
//    container to EDI. So it is the mask, not the RMW spelling, that decides.
//  * The original's register form (`or eax, edx`) is the tell: MSVC 5 emits the
//    register form only for a mask it cannot rematerialise, i.e. a variable.
//    A constant mask in a loop-reachable block is either folded to an immediate
//    (one use) or re-materialised into a register (two or more uses), and is
//    never read from the copy made before the loop. A micro test confirms it:
//    a mask arriving as a function parameter gives `or WORD PTR [eax], dx`,
//    while every local spelling gives the folded or rematerialised form.
//  * Tried this pass, all still 92.2% / 392 bytes with the same single diff:
//    the RMW as `|= flag`, `= x | flag`, an explicit temporary, the bitfield,
//    and a `p = u` alias; the mask as unsigned short, short, char, int,
//    const int, each with and without a cast in the RMW; `g_game->flags` as a
//    16-bit bitfield written directly (that one turns the fallback into an
//    in-place `or [mem], reg`, so it is wrong); the three hit-block statements
//    as an inlined helper taking the mask; the field store and the game word
//    in either order; the mask declared before the first loop.
//  * The one shape that does remove the remat is z1: hold a `second` pointer
//    set inside the loop, test it after, so the hit block is not dominated by
//    the loop header. The remat goes, but then MSVC folds both mask uses to
//    immediates (`or esi, 16`, `or BYTE PTR [..], 16`) and the callee-saved set
//    rotates to pop ecx, so the original is not that either.
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
                        u->u.bits.bit4 = 1;
                        Game_0048d790* g = g_game;
                        g->field_37e9c = 0;
                        g->flags |= flag;
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
