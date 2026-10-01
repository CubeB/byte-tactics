// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// 2026-10-02 pass (Space Bunny Free): 87.5% -> 88.4%, still 1654 bytes.
// One win, found by permute.py and then isolated by reverting its hunks one at
// a time (every other hunk it applied was no-op churn and changed nothing):
// the field_147 block must read rect.left into a local BEFORE the Measure call
// and assign x0 from it AFTER, so rect.left's live range straddles the call:
//     int left = rect.left;
//     int w1 = Measure_004a56b0(buf);
//     int x0 = left + w1;
// `int x0 = rect.left; int w1 = Measure(...); x0 += w1;` and the same split
// with `int x0 = left;` written before the Measure both stay at 87.5, so it is
// the position of the READ, not the spelling, that matters (the guide's dead
// reload / split live range note). Scratch: build/scratch/0x4a56b0/gen*.py.
// Everything below is what I could not move. The whole file is one register
// allocation, and the original's `x` is not a separate local at all: it is
// rect.left, reloaded into ecx from its own stack slot after the image call
// (`mov ecx,[esp+0x1c]`, only inside the `image != 0` branch, which is what a
// spilled caller-saved live range looks like). Three facts pin that down: the
// image call loads the surface into ecx (the register x would occupy), the
// colour byte goes to edx there, and `i` is kept in edi across both later checks.
// Two shapes get part of the way and each loses somewhere else (all measured,
// scratch gen4/gen8/gen9/gen16/gen17):
//  A. `int nx = rect.left;` after the image call, arms on rect.left, image call
//     spelled `FUN_004bf6f0(entries->surface, &rect, ...)` (no surf local), plus
//     `Entry_004a56b0* entry = &entries[index];` for the arms: this matches the
//     original LINE FOR LINE from the rect block through the whole &4 arm
//     (`mov ebp,ecx; test al,4; movsx ebp,[+0x17]; lea edi,[+0xb6]; xor eax,eax;
//     add ebp,ecx`) and gives 85.5% / 1663 bytes. Its only defect is the &2
//     arm, where the inlined Measure's null test comes out as
//     `mov dword ptr [esp+0x10],0` + `test edi,edi` instead of the original's
//     `xor eax,eax` + `cmp edi,eax` + `mov [esp+0x10],eax`. Six Measure
//     spellings (p declared first, `return width` instead of `return 0`, an
//     explicit `width = 0` on the null path, `(int)p == 0`, `if (!p)`) and four
//     &2 arm spellings (no `half` local, a `m` local, `>> 1`, Measure through
//     an entry pointer) all leave that form alone, so it is the allocator, not
//     the helper.
//  B. the base (x a real local in edi) keeps the &2 arm's Measure form but
//     loses the &4 arm: `movsx ecx,w; add edi,ecx; mov ebp,edi` instead of
//     `movsx ebp,w; add ebp,ecx`. Giving the &4 arm a separate `int w =` local
//     does move the sum into ebp, but the local then CSEs into the &2 arm and
//     breaks its Measure form too (83.5% / 1661 bytes, scratch s10, u1-u7).
//     Two-statement arms, reversed operands, `int& x = rect.left`, short and
//     const x, an `else nx = x` arm, one-expression arms, a by-value
//     `operator`/helper around the sum (the lead from the matched 0x49abb0)
//     and a Rect built by value all canonicalise back to B or to 73%.
// Still differs (the retry-11 list minus the field_147 register swap, which the
// win above fixed; all of it downstream of x sitting in edi instead of ecx):
// rect.right is `lea ecx,[edi+edx-1]` instead of
// `lea edx,[edx+ecx-1]` (operand order in every source spelling is inert);
// the image call's surface load goes to eax instead of ecx and `mov ebp,edi`
// (nx = x) is hoisted above rect.right; the first `i` check uses eax instead of
// edi and the second reloads instead of reusing; in the field_147 block x0 is
// now in ebx and x1 in ebp as in the original, but the `mov [esp+0x18],ebx`
// spill is still missing (so the tail is `dec ebx` in place rather than the
// original's reload) and `lea esi,[buf]` / `lea esi,[pat]` are hoisted above
// the `jne`; the 148-block tail uses edx for
// `obj->field_14 = obj->field_08`; the final FUN_004be950 uses edi for obj and
// decrements x0 in place where the original reloads the spill.
// Sharpest lead for the next attempt: x has to be a value whose home is
// rect.left's stack slot, so MSVC re-materialises it after the call, and
// nx must not coalesce with it. A construct that stops the &4 arm folding the
// sum into x (so it lands in ebp) WITHOUT introducing a live `w` local in the
// &2 arm would finish it; nothing in the statement-shape or helper space
// tried here does that.
// Also re-measured on the 88.4% file and all inert (byte-identical): every
// include set (windows.h/string.h/stdlib.h/stdio.h/memory.h/math.h, alone,
// swapped and stacked), `int nx = rect.left` instead of `int nx = x`, nx
// declared after the image call, `void* const surf`, the &2 arm as `i >= 0`,
// the `i >= 0 && align & 8` test split into nested ifs, `int x0 = left; x0 +=
// w1;` and `int x1; x1 = x0;` in the field_147 block, and Measure's loop as
// `for (; *p != 0; ++p)`, `p++`, or a hoisted `char ch`. Load-bearing: the
// `void* surf` local (dropping it, moving it inside the if or after the x
// declaration all give 72.9%). Worse: `i != -1` / `i == -1` in either check
// (85.2), one arm reading rect.left and the other x (83.7 / 70.6), both arms
// reading rect.left (73.8), `else nx = x` (84.7), Measure's loop through a
// second pointer or `while ((ch = *p) != 0)` (29.2 / 69.4).
// 2026-10-01 retry 11 (deepseek-v4.1-flash): kept 87.5% (1654 bytes). ~350 more
// variants scored, nothing beat the retry-10 file. Measurements worth keeping:
//  - The shape `int x = rect.left; int nx = x; ... nx = entries[index].w + x;`
//    is right: the MATCHED sibling 0x4a53c0 uses exactly it and compiles to the
//    original's `movsx ebx,w; add ebx,esi` (sum into nx's register) there. What
//    differs here is the register context: 0x4a53c0 has x=esi, nx=ebx and no
//    image call, so its x stays in i's freed register and the arm computes into
//    nx. Here x is live across the image call and gets edi; the allocator then
//    folds nx into x's value and re-copies (`add edi,ecx; mov ebp,edi`).
//  - `nx = entries[index].w` assigned after the call (with x after too) is the
//    only family that emits the original's `add ebp,ecx` arm and hits 1662
//    bytes, but it hoists `movsx ebp,w` and the surface load above the align
//    test and the arm loses its own movsx: 72.9-73.3%. The hoisted movsx is the
//    pre-call nx = w materialisation; the compiler CSEs the arm's w load into
//    it. `nx = w + nx` (c4) also keeps the sum in ebp (`movsx eax,w; add
//    ebp,eax`) but scores 70.7%.
//  - Two-statement arms (`nx = w; nx += x;`), cast/paren forms, `nx = w + nx`,
//    `nx = x + w`, else-nx, x before/after the call in all 4 orders, entry
//    pointer for the arms only, Measure body reordering (p before width breaks
//    everything, 56.9%), and 189-combination sweeps of pre/mid/arm spellings
//    all canonicalise to the same coalesced form or score lower.
//  - i checks: only `i >= 0` / `i < 0` score 87.5; `i != -1`, `i == -1`, a
//    local copy of i, and `*pi` forms all give 84.3 (2-line memory compares
//    shift the diff). No spelling put i in edi; the original's edi reuse needs
//    x out of edi.
//  - field_147 x0/x1: `x0 += w1; int x1 = x0;` (x0=ebp,x1=ebx) beats
//    `int x1 = x0 + w1; x0 = x1;` (x0=ebx,x1=ebp, the original's registers,
//    86.9%) because the latter makes the second Measure use ebx as its width
//    accumulator and loses more lines than the swap wins. x0-1 materialisation,
//    extra x1 copies, declaration permutations and a temp for obj: all 87.5 or
//    worse. The missing `mov [esp+0x18],ebx` spill and the `mov ebx,[esp+0xc0]`
//    obj load are downstream of x0 living in ebp.
// Still differs: same list as retry 10 (x in edi instead of the original's
// caller-saved ecx reload, so the rect block, both arms, the i checks, the
// field_147 block's x0/x1/spill and the final call's obj/argument schedule all
// follow). Best lead: a source construct that keeps x out of a callee-saved
// register while stopping MSVC from folding nx into x at the arm; 0x4a53c0
// proves the statement shape itself is not the problem.
// 2026-10-01 retry 10 (deepseek-v4.1-flash): 84.7% -> 87.5%, 1647 -> 1654 bytes.
// Three wins, all found with the checker's own percentage (my instruction-level
// alignment disagreed with it twice and is not a reliable proxy):
//  1. First i check as `i >= 0` (not `i != -1`): the compiler then emits
//     `mov eax,[esp+0x14]; test eax,eax; jl` (3 lines) where the original has
//     `mov edi,[esp+0x14]; cmp edi,-1; je` (3 lines). `i != -1` gives a 2-line
//     memory compare and scores 84.7; the 3-line form aligns better (86.8).
//     Second check stays `i < 0`. `i >= 0` and `i != -1` are equivalent here
//     (i is 1..count or -1, never 0).
//  2. field_147 block back to `x0 += w1; int x1 = x0;` (not `int x1 = x0 + w1;
//     x0 = x1;`): at the 86.8 context this is 87.4 (the retry-9 advice was for
//     the 84.2 context and is now obsolete). It swaps x0/x1 into ebp/ebx (the
//     original has x0=ebx, x1=ebp) but the checker prefers it.
//  3. FUN_004be950's last parameter must be `int colour`, not `unsigned char`:
//     the caller then emits the original's `xor edx,edx; mov dl,[ebx+0x8b4]`
//     instead of a bare `mov dl,...` (+2 bytes, 87.4 -> 87.5). The matched file
//     0x4be950.cpp declares it `int color`; check every callee declaration
//     against its matched file before blaming the allocator.
// Still differs (one root cause: x is live across the image call, so it is in
// edi and the original's caller-saved ecx is never free):
//  - rect block uses edi for rect.left and `lea ecx,[edi+edx-1]` for rect.right;
//    the original keeps left in ecx and computes right into edx.
//  - `mov ebp,edi` (nx = x) is hoisted to just after the left load, +2 bytes
//    before the image call; the original's `mov ebp,ecx` sits after the call
//    and its `mov ecx,[esp+0x1c]` reload is missing from ours (4 bytes).
//  - &4 arm: `movsx ecx,w; add edi,ecx; mov ebp,edi` instead of the original's
//    `movsx ebp,w; add ebp,ecx` (+2 bytes). &2 arm: `lea edi` one line late.
//  - first i check uses eax where the original uses edi; the second reloads
//    instead of reusing (`cmp edi,-1; jne`).
//  - field_147: x0 in ebp not ebx, `lea esi,[buf]`/`[pat]` hoisted above the
//    `jne` twice, x0 not spilled to [esp+0x18] (tail is `lea edx,[ebx+ebp-1]`
//    instead of `mov edx,[esp+0x20]; dec edx`).
//  - 148 tail uses edx for `obj->field_14 = obj->field_08` where the original
//    uses ecx; final FUN_004be950 block uses edi for obj where the original
//    reuses ebx after spilling x0.
// ~120 more variants this round (arm operand order, temps, references,
// single-expression forms, x/nx/surf declaration order and permutations,
// `entry` pointer, `nx = w + nx` / `nx += w`, nx-in-else, bool/i-copy forms,
// dead stores, no-op self-assignments, uninitialised x/nx, nested if, align
// local, struct field types, Measure-helper spellings) all land on 1654 bytes
// or worse. Headers: all 128 sets (768 with --cpp) give <= 87.5.
// Lead for the next attempt (two shapes, each one edit from a big jump):
//  A. x after the image call (w1): the whole first half matches line for line
//     (1665 bytes, +3) but the diff aligns the repeated Measure bodies wrongly
//     and the checker drops to 72.9. Its only real defect is the &4 arm
//     accumulator (folded into ecx).
//  B. `int nx = rect.left;` BEFORE the image call, `int x = rect.left;` after,
//     and the &4 arm as `nx += entries[index].w;` (scratch q_q1b, 84.7%,
//     1659 bytes): this is the only spelling found that gives the original's
//     `add ebp, ecx` arm (the two rect.left loads make nx and x distinct
//     values, so MSVC cannot fold them). Everything around the arm matches the
//     original byte for byte, including the je target; the arm differs by one
//     register (`movsx ecx,w` where the original has `movsx ebp,w`). It still
//     hoists `mov ebp,ecx` to just after the left load (before rect.right) and
//     hoists the image call's surface load into edi, and those two hoists plus
//     the diff's alignment are why it scores below this file. Combining B's
//     arm with A's post-call copy placement (nx materialised after the call
//     but not foldable into x) is the next thing to try; `nx = x;` after the
//     call folds and gives A back.
// 2026-10-01 retry 9 (deepseek-v4.1-flash): 82.5% -> 84.7%, 1652 -> 1647 bytes.
// Two wins came from REORDERING statements in the field_147 block (the
// allocation is decided globally, so this block's shape moves the whole
// function's register assignment):
//  1. `int y = rect.top;` then `*p = 0;` then `int x0 = rect.left;`
//     (scratch sweep4: yp0x0w1) took 82.5 -> 84.2. With `*p = 0` first, x0
//     lands in ebp and x1 in ebx; with y first it flips to the original's
//     x0/x1 (ebx/ebp). Any other permutation of y / *p = 0 / x0 / w1 is worse.
//  2. `int x1 = x0 + w1; x0 = x1;` instead of `x0 += w1; int x1 = x0;`
//     (scratch b22 v5) took 84.2 -> 84.7: MSVC merges x0 and x1, keeps x1 in
//     ebp and defers x0's update, so the w2 accumulator comes out in edi like
//     the original and the tail's `lea ebx,[eax+ebp-1]` uses ebx.
// Both are semantic no-ops, so the compiler does the work.
// ~1000 more variants in build/scratch/0x4a56b0/{sweep1..sweep7,b1..b31}.py
// all land on 84.7% or lower, and every 84.7% variant is byte-identical code, so
// the remaining x/nx coalescing is a hard allocator decision, not a source
// spelling:
//  - x/nx declaration order and position (before/after the image call, before/
//    after rect.right, nx-first, two-step, uninitialised then assigned), const,
//    register, short, unsigned, int&/const int&, and rect.left-inline spellings
//    all give the same 1647-byte code.
//  - Arm spellings (w + x, x + w, w then += x, w + nx, nx += w, one-expression
//    forms, temp locals, casts, nested if, align local, switch) are all
//    canonicalised to the same `add edi, ecx` accumulate-into-x form.
//  - The only shape that puts x in ecx with the post-call reload is declaring
//    x/nx AFTER the image call (scratch t_d3_*.cpp), but then the sum still
//    coalesces into ecx (`add ecx, edx; mov ebp, ecx`) and the rest of the
//    allocation shifts: 66.1%, 1665 bytes. Same for the no-surf variants.
//  - surf local placement, text-pointer locals, else-nx arms, and no-op
//    self-assignments / dead stores (the agent-guide 0x461b10 live-range trick)
//    do not move the coalescing: the no-ops are deleted before allocation.
// Still differs: (a) the arms region (the largest hunk, 55 lines): x lives in
// edi so the &4 arm accumulates into x's register (`movsx ecx,w; add edi,ecx;
// mov ebp,edi`) instead of the original's `movsx ebp,w; add ebp,ecx`, rect.right
// is `lea ecx,[edi+edx-1]` instead of `lea edx,[edx+ecx-1]`, and the image
// call's surface load goes to eax instead of ecx; (b) the first `i != -1` check
// compares memory instead of `mov edi,[esp+0x14]; cmp edi,-1`; (c) the field_147
// block hoists `lea esi,[buf]`/`lea esi,[pat]` above the `jne` (the original
// puts them inside the loop path) and defers x0 so the tail is
// `lea ebx,[eax+ebp-1]` where the original does `add ebx,eax;
// mov [esp+0x18],ebx` and reloads it; (d) the 148-block tail uses edx instead
// of ecx; (e) the final FUN_004be950 argument schedule differs (ours decrements
// x0 in place, the original reloads it from [esp+0x18] and decs into edx).
// Lead: the original's x must be live past the &4 arm's sum so the sum cannot
// reuse its register (then the sum goes to ebp and edi is free for the Measure
// text pointer early, matching both arms' schedule). No source construct tried
// extends that live range without emitting code; the guide's 0x453360 note
// (dead reload = split live range, not a deleted statement) suggests the
// original had a real extra use of x or i that this reconstruction lacks.
// 2026-10-01 retry 8 (deepseek-v4.1-flash): 62.1% -> 82.5%, 1652 bytes.
// The big win came from reading the original's disassembly for source-level
// details instead of allocation guesses. Five source shapes each moved the
// score, all verified with check.py:
//  1. The final FUN_004be950 takes a RELOADED surface: write
//     `obj->holder->entries->surface`, not `entries->surface` (62.1 -> 64.5).
//     That ends entries' live range before the field_147 block and the
//     compiler then reloads holder/entries exactly like the original.
//  2. `char buf[0x80]`, not 0x7c: the original's frame is 0xac bytes and only
//     0x80 makes the prologue and every local offset line up (64.5 -> 66.0).
//  3. The FUN_004be950 arguments use lh2 for the 3rd argument and lh1 for the
//     5th (both are LineHeight calls; the original evaluates them in that
//     order): `..., x0, lh2 + y - 1, x0 + w2 - 1, lh1 + y - 1, ...`
//     (66.0 -> 66.4).
//  4. With those in place the PLAIN `entries[i]` loop gives the original's
//     loop exactly (bound ecx, walk edx, init after the guard) AND entries in
//     ebx: the old "plain loop puts entries in ebp" note is obsolete
//     (66.4 -> 67.5).
//  5. The image call reads its surface from a LOCAL: `void* surf =
//     entries->surface;` then `FUN_004bf6f0(surf, ...)`. This one flipped the
//     right-align arm from "add ecx,edx; mov ebp,ecx" to the original's
//     accumulate-into-nx shape and took 67.5 -> 77.3 in one step.
//  6. In the field_147 block, `int x1 = x0;` before the second Measure and
//     `x0 += w2;` after it, passing x1 and x0-1, is worth +0.6.
//  7. The two i checks want DIFFERENT spellings: `i != -1` for the
//     `i != -1 && (align & 8)` test and `i < 0` for the `i == -1` test
//     (mixing them either way gives 82.5; both `!= -1` gives 77.3, both
//     `>= 0` gives 78.6). The original's code is `mov edi,[esp+0x14];
//     cmp edi,-1; je` for the first one; this file compares memory there, so
//     a register-forcing construct for i is still missing.
// Also found: x and nx must be declared BEFORE the image call (69.7 with them
// after, 77.3 before), and CSE'd extra uses of entries do nothing (weights are
// computed after CSE/DCE), as do inline wrappers taking entries.
// What still differs at 82.5%: x/nx are coalesced into one register (edi)
// here, while the original keeps x in ecx and nx in ebp; the align&4 arm then
// computes "nx += w" instead of "nx = w; nx += x", the first i != -1 check
// compares memory instead of loading i into edi, and the field_147 block has
// x0/x1 in the opposite callee-saved registers (ebp/ebx vs ebx/ebp). All of
// these track the same coalescing. Scratch: build/scratch/0x4a56b0/batch*.py,
// mixi_first_ne.cpp (the file, 82.5), mixi_second_eq.cpp (the mirror mix,
// 82.5), last_i_lt0.cpp (78.6), ic_surf_local.cpp (77.3).
// Ruled out for the coalescing: const/unsigned/reference x, nx-based arm
// expressions, x from entries[index].x, x used after the arms, declaration
// order, and moving x/nx after the call.
// Lead for the next attempt: making nx a separate variable assigned in an
// `else nx = x;` arm (instead of `int nx = x;` up front) yields exactly the
// original's 1662-byte size at 77.2% (scratch el_else_nx.cpp). Its diff is
// the same shape as this file's, so the coalescing is still there, but the
// size match means the frame and instruction lengths line up; combining that
// structure with the right register assignment is the most promising next
// step.
// 2026-10-01 retry 7 (deepseek-v4.1-flash): best 62.1%, 1661 bytes (was 60.3%).
// Two findings, both verified by compiling scratch variants and reading the /Fa
// listing:
//  1. The loop allocation is decided GLOBALLY, not by the loop spelling. With a
//     plain `entries[i].type` loop the compiler strength-reduces a walk pointer
//     and puts `entries` in ebp, nx in ebx (55.2%, 1662 bytes: every instruction
//     matches except a global ebx<->ebp rename of entries/nx). Declaring an
//     EXPLICIT `Entry* e = &entries[i];` walk with `e->type`/`e++` instead moves
//     `entries` into ebx and nx into ebp (the original's allocation) and scores
//     62.1%. Every loop spelling (for/while/guard-do/do-while/while1), counter
//     type, bound spelling, declaration order and phi spelling I tried gives one
//     of those two, never the original's exact mixture.
//  2. What still differs at 62.1% (see the diff): (a) with the explicit walk the
//     walk pointer is initialized in the preheader and takes ecx while the bound
//     takes edx; the original computes the bound first into ecx and creates the
//     walk inside the loop entry (`lea edx,[ebx+0x15b]` after the guard), which
//     only happens when the walk is compiler-generated (and then entries goes to
//     ebp). (b) rect.right is `lea edx,[ecx+edx-1]` here vs `[edx+ecx-1]`
//     original. (c) the right-align arm computes `add ecx,edx; mov ebp,ecx`
//     (x += w; nx = x) instead of the original's `movsx ebp,w; add ebp,ecx`
//     (nx = w; nx += x). All three are downstream of the same register choice.
// Also tried and no better: moving the preceding matched sibling 0x4a53c0 into
// the same file (no effect), a named bound (int/short), pointer initializer
// spellings (&entries[i], entries+i, entries+1, char* cast), pointer declared
// inside the loop body (strength-reduced), hoisting entry/entries2, 400 random
// safe mutations, and routing the tail's surface loads through obj->holder->
// entries (which flips entries to ebx but changes the code and scores 58.6%).
// Sharpest clue for the next attempt: the two effects track the explicit
// pointer's live range. A pointer whose live range starts BEFORE the guard
// (`Entry* e = &entries[i];` or a dead `e = entries;` first) gives entries=ebx
// but puts the walk in ecx and the bound in edx. A pointer first assigned
// INSIDE the guard (`if (i < bound) { e = &entries[i]; do {...} while (...) }`)
// is strength-reduced away: the loop then matches the original exactly (bound
// ecx, walk edx, init after the guard) but entries goes back to ebp. No spelling
// found that keeps both; the original is likely one of these two shapes with a
// declaration detail that survives, so try declaring the pointer or the loop
// variables in a scope whose live range straddles the guard without being used
// there.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a56b0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    int align;                         // +0x1b
    int colours;                       // +0x1f
    int image;                         // +0x23
    char unknown_27[1];
    signed char tab;                   // +0x28
    char unknown_29[0xb6 - 0x29];
    union {
        short count;                   // +0xb6 (entry 0 only)
        char text[0xbc - 0xb6];        // +0xb6
    } b6;
    void* surface;                     // +0xbc
    char unknown_c0[0xd6 - 0xc0];
    int language;                      // +0xd6
    char unknown_da[0x147 - 0xda];
    unsigned char field_147;           // +0x147
    unsigned char field_148;           // +0x148
    char unknown_149[0x15b - 0x149];
};

struct Holder_004a56b0 {
    int current;                       // +0x00
    Entry_004a56b0* entries;           // +0x04
};

struct Glyph_004a56b0 { unsigned short width, height; };

struct List_004a56b0 {
    char unknown_0[0x0c];
    unsigned short* glyphs;            // +0x0c
};

struct LanguageRoot_004a56b0 {
    int current;                       // +0x00
    char unknown_04[0x14 - 0x04];
    List_004a56b0* language;           // +0x14
};

struct Class_004a56b0 {
    char unknown_00[0x08];
    void* field_08;                    // +0x08
    void* field_0c;                    // +0x0c
    char unknown_10[0x14 - 0x10];
    void* field_14;                    // +0x14
    Holder_004a56b0* holder;           // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colours[256];          // +0x8b2, +0x8b3, +0x8b4
};

struct Rect_004a56b0 { int left, top, right, bottom; };
#pragma pack(pop)

extern LanguageRoot_004a56b0* DAT_0051fba4;

void __stdcall FUN_004c1420(int id);
int FUN_004c1440();
int __stdcall FUN_004c1480(int font, char* text);
int FUN_004c1450();
int __stdcall FUN_004b7f30(unsigned short* glyphs, int c);
void __stdcall FUN_004c13a0(int colour, int font);
int FUN_004c13f0();
void __stdcall FUN_004c14f0(void* surface, char* text, int x, int y, int maxw);
int __stdcall FUN_004bf6f0(void* surface, Rect_004a56b0* rect, int colour);
void __stdcall FUN_004bfe10(void* surface, Rect_004a56b0* rect);
void __stdcall FUN_004bf4d0(void* surface, Rect_004a56b0* rect, int param);
void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2,
                            int colour);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw,
                            int style);
int __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw,
                           int rem, int style);

static inline int Measure_004a56b0(char* text)
{
    int width = 0;
    char* p = text;
    if (p == 0)
        return 0;
    if (DAT_0051fba4->language == 0)
        return FUN_004c1480(FUN_004c1440(), text);
    while (*p != 0) {
        char ch = *p;
        Glyph_004a56b0* glyph = (Glyph_004a56b0*)FUN_004b7f30(
            DAT_0051fba4->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

static inline int LineHeight_004a56b0()
{
    if (DAT_0051fba4->language == 0)
        return FUN_004c1450();
    return (int)((Glyph_004a56b0*)FUN_004b7f30(
        DAT_0051fba4->language->glyphs, 0x49))->height + 2;
}

// FUNCTION: 0x4a56b0
void __stdcall FUN_004a56b0(Class_004a56b0* obj, int index)
{
    obj->field_14 = obj->field_0c;
    Entry_004a56b0* entries = obj->holder->entries;

    int i = 1;
    int t = 0;
    for (; i < entries[0].b6.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                FUN_004c1420(entries[i].language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].b6.count + 1) {
        FUN_004c1420(DAT_0051fba4->current);
        i = -1;
    }


    if (entries[index].x == -1)
        entries[index].x = (short)((entries[0].w - Measure_004a56b0(entries[index].b6.text)) / 2);

    Rect_004a56b0 rect;
    if (entries[index].type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = entries[index].x;
        rect.top = entries[index].y;
    }
    rect.right = entries[index].w + rect.left - 1;
    rect.bottom = entries[index].h + rect.top - 1;

    int x = rect.left;
    int nx = x;
    void* surf = entries->surface;
    if (entries[index].image != 0)
        FUN_004bf6f0(surf, &rect, obj->colours[entries[index].image]);

    if (entries[index].align & 4) {
        nx = entries[index].w + x;
        nx -= Measure_004a56b0(entries[index].b6.text);
    } else if (entries[index].align & 2) {
        nx = entries[index].w / 2 + x;
        int half = Measure_004a56b0(entries[index].b6.text) / 2;
        nx -= half;
    }

    if (i >= 0 && (entries[index].align & 8)) {
        FUN_004c13a0(obj->colours[0], FUN_004c13f0());
        FUN_004c14f0(entries->surface, entries[index].b6.text, nx + 1, rect.top + 3, -1);
    }

    FUN_004c13a0(entries[index].colours, FUN_004c13f0());

    if (i < 0) {
        int lh = LineHeight_004a56b0();
        if (rect.bottom - rect.top > lh * 2)
            FUN_004a51d0(entries->surface, entries[index].b6.text, nx, rect.top,
                         rect.right - rect.left + 1,
                         rect.bottom - rect.top + 1, entries[index].colours);
        else
            FUN_004a50e0(entries->surface, entries[index].b6.text, nx, rect.top,
                         rect.right - rect.left + 1, entries[index].colours);
    } else {
        FUN_004c14f0(entries->surface, entries[index].b6.text, nx, rect.top, -1);
    }

    if (entries[index].field_148 & 1) {
        Entry_004a56b0* entries2 = obj->holder->entries;
        Rect_004a56b0 rect2;
        if (entries2[index].type == 0) {
            rect2.left = 0;
            rect2.top = 0;
        } else {
            rect2.left = entries2[index].x;
            rect2.top = entries2[index].y;
        }
        rect2.right = entries2[index].w + rect2.left - 1;
        rect2.bottom = entries2[index].h + rect2.top - 1;
        FUN_004bfe10(entries2->surface, &rect2);
        FUN_004bf4d0(entries2->surface, &rect2, -0x14);
        obj->field_14 = obj->field_08;
        return;
    }

    unsigned char c = entries[index].field_147;
    if (c != 0) {
        char pat[2];
        pat[0] = (char)c;
        pat[1] = 0;
        char buf[0x80];
        strcpy(buf, entries[index].b6.text);
        char* p = strstr(buf, pat);
        if (p != 0) {
            int y = rect.top;
            *p = 0;
            int left = rect.left;
            int w1 = Measure_004a56b0(buf);
            int x0 = left + w1;
            int x1 = x0;
            int w2 = Measure_004a56b0(pat);
            x0 += w2;
            int lh1 = LineHeight_004a56b0();
            int lh2 = LineHeight_004a56b0();
            FUN_004be950(obj->holder->entries->surface, x1, lh2 + y - 1, x0 - 1,
                         lh1 + y - 1, obj->colours[2]);
        }
    }

    obj->field_14 = obj->field_08;
}
