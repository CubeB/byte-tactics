// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// 30-min checkpoint (space-bunny-free): still 84.7%, 354 of 354 bytes, 43 masked
// bytes differ, all in one hunk, and this pass established that the hunk is
// INVARIANT: of ~380 source shapes scored this pass (ten hand batches plus two
// random sweeps of 60 and 90 variants) every one that keeps 354 bytes and the y
// subtraction first emits the first thirteen instructions byte for byte as the
// current file, b.x's load always landing in esi. Nothing in the loop, the max,
// the divisions, the parameter spelling or the copy moves it: stripping the
// loop body down to `for (; i <= n; i++) a.x += d.x;` keeps the same prologue,
// and so do all six spellings of the loop condition, the loop body spellings,
// the six pointer-alias forms and the scalar/Vec3 helper forms of the
// difference. tools/permute.py, 5154 candidates with the default focus plus 2950
// more with --no-focus (19 minutes in all): 84.7% to 84.7% both times, no change.
// Two new diagnostics for the next attempt:
// 1. MSVC 5 produces only TWO load orders for the three subtractions, whatever
//    the source: y-first sources give b.y, a.x, a.y, [y-sub], b.x, a.z; a
//    non-y-first source gives b.y, a.y, a.x, b.x, a.z (a.y hoisted into the
//    second caller-saved register). The original's order, b.y, b.x, a.x, a.y,
//    a.z, is neither, so no permutation of the three statements reaches it.
// 2. I scanned the exe for prologues that hoist two argument loads above
//    `push ebx` (49 hits, 8 of them already matched: 0x4233a0, 0x423710,
//    0x49fb50, 0x49fba0, 0x49fbf0, 0x4b8b30, 0x4c06e0, 0x4cbab0). In every
//    matched one the two hoisted values are whole by-value parameters that no
//    callee-saved register is needed for (0x4c06e0, MATCH: `mov eax, [param2];
//    mov ecx, [param3]; push ebx`, then edx/ebp/ebx/esi/edi for the derived
//    values). The original here has that same shape, two caller-saved values
//    hoisted and four callee-saved ones after; ours instead gives the second
//    slot to a callee-saved value. So the missing source is one where b.x needs
//    no callee-saved register and is allocated before a.y, which pins the
//    remaining search on making the x subtraction's minuend a plain temporary
//    that is NOT also the destination, without the `mov esi, ebx` copy that
//    `b.x = a.x - b.x` costs (356 bytes).
// Also ruled out this pass: the dead-store lever (five spellings x three
// positions between the subtractions) and the Identity wrapper on a scalar
// leave the prologue byte-identical (they only perturb the loop, 81.5%);
// arithmetic shapes (`+= -a.f`, `- (int)a.f`, `+ 0`, `* 1`, `,`, a block, an
// `if (1)`), the member and free operator-= in all six body orders, MaxAbs with
// the abs inside the helper, GetCell as a macro, a named constant for the
// shift, the aggregate initialiser, three scalar difference locals, subtract on
// a local copy and copy that into d, and every return type.
// Space Bunny Free pass: still 84.7%, 354 bytes, and I now know exactly what the
// tie is. Register by register, the original allocates six different registers:
// b.y->eax, b.x->ecx, a.x->ebx, a.y->esi, a.z->ebp, b.z->edi. Ours allocates the
// same six values but only four registers: b.y->eax, a.x->ebx, a.y->esi, then
// b.x REUSES esi (a.y is dead one instruction earlier than in the original),
// a.z->ebp, b.z->edi. So the whole difference is that the x-difference temp
// takes the register the y subtraction just freed instead of the still-free ecx,
// and |dx| and n then take ecx instead of esi. The two `mov` hoists follow from
// that: a load into a caller-saved register can be hoisted above `push ebx`, one
// into esi cannot. Nothing else differs: the dead `mov [esp+0x10], eax` /
// `mov [esp+0x10], ecx`, the copy's slot mixup and everything from
// `mov eax, [esp+0x10]` (0x4852ec) to the `ret` are byte-identical.
// New source shapes scored this pass, all 84.7% or worse: y,x,z produces output
// byte-identical to y,z,x (MSVC normalises the three in-place subtractions to
// y,x,z whatever the source order, so the order is not the lever; all six were
// re-scored, yzx and yxz 84.7%, the rest 83.9%); 43 compiler-state files of
// unused padding before the function in five kinds (extern int, extern int(),
// `static int f()`, struct, extern const int) at 4 to 196 items, every one
// exactly 84.7% and not one byte moved; a Vec3 with a user-defined two-argument
// constructor doing the difference (eight body orders, by value and by
// reference), 350 bytes, 64.8 to 72.1%; a helper taking the Vec3 by value that
// returns the difference (six body orders), 69.1 to 81.5%; field-by-field
// differences into d and field-by-field copies of b into d after the in-place
// subtractions, all six orders each, 350 bytes, 68.0 to 75.3%; `d = b` then
// subtract in place on d (82.3%, and 76.6% in x,y,z order); the max spelled
// `p > q ? p : q`, as a `max(a,b)` macro in both argument orders and as
// `__max` (83.9, 83.9, 76.6 and 84.7%, only `p < q ? q : p` matches the
// original's `jl`); four declaration orders of d/n/best/i (two of them cost 8
// bytes, the rest 84.7%); the max read from b's fields with the copy before
// and after the divisions (84.7% and 64.5%); comma expressions pairing two
// subtractions, `b.x = b.x - a.x`, `d.x = d.x / n`, the two divisions swapped,
// the three subtractions or the max inside their own block, and a `while`
// loop: 84.7% or worse. One diagnostic worth keeping: a stripped function with
// only the three subtractions and the copy folds to `mov eax,[d.x]; mov
// ecx,[b.x]; sub eax,ecx`, so the tie cannot be reproduced in a small
// reproducer, only in the whole function.
// Also closed this pass: all 48 combinations of the six subtraction orders with
// the two assignment forms (`-=` and `= b.F - a.F`) for each subtraction. The
// assignment form makes no difference at all, the eight forms of each order
// compile to identical bytes: the sixteen y-first combinations are 84.7% and
// the other thirty-two are 83.9%, so the only lever in the whole preamble is
// whether the first subtraction is the y one. tools/permute.py, 2474 candidates
// in 15 minutes, 84.7% to 84.7%, score 355 unchanged.
// claude-opus-5-5 (#4378): still 84.7%. The only difference is in the head:
// the original loads b.x into ecx before `push ebx` and keeps dx in ecx and the
// step count n in esi; ours swaps them (dx in esi, n in ecx). Tried: d built
// from b.x - a.x directly (74.5%), x/y/z subtraction order (83.9%), named abs
// locals, an if-based max (74.7%), and the 0x485140 GetCell spelling (a width
// local after the x >= 0 test) plus <memory.h> (no change). A 12-minute permuter
// run (542 candidates) found nothing.
//
// mimo-v2.6-pro retry pass (issue #4059), still 84.7%, 354 bytes. The one
// remaining hunk is unchanged: the original keeps the x difference in ecx
// (b.x loaded into ecx before the register pushes) and the abs/max/n chain in
// esi, ours keeps the x difference in esi and the chain in ecx. This pass
// ruled out the compiler-state theory far more thoroughly than before:
// - the unused-declaration sweep, redone at step 1 for N = 0..399 and at spot
//   values up to N = 3000, is bit-identical at every N (diff hash unchanged),
//   in every placement tried (after the include, before the function, after
//   the struct block) and with five unused prototype spellings up to 3000;
//   unlike 0x47dfc0 and 0x41ce90, declarations here move no byte at all;
// - tools/headers.py --cpp, all 768 sets (every C set crossed with none or
//   one of <string>, <vector>, <map>, <list>, <iostream>): flat 84.7%;
//   windows.h alone is 83.9% and flips the loop's [edx+ecx+0xfa] to
//   [ecx+edx+0xfa]; windows.h plus any C++ header returns to 84.7%;
// - defining each real neighbour above this function (0x485140, 0x485330,
//   0x485070, 0x485010, 0x47dfc0) as the guide suggests: only 0x485140 moves
//   anything (83.9%, same loop base/index flip as windows.h), the prologue
//   tie is identical in every one;
// - pack(2)/pack(4)/pack(8)/pack(16) on the struct block: codegen moves but
//   only downwards (66.7% and 54.3%).
// New source shapes scored here, all worse or flat: n read from b fields
// after the in-place subtractions (64.5%), divisions done on b (55.9%),
// no step struct at all (55.9%), three scalar difference locals used in the
// loop (75.3%), the x difference as a local rebuilt into d (77.4%),
// member operator-= (80.6%), a copy-returning helper (81.5%), labs (flat),
// throwaway extra uses of dx/dy/dz around the copy (all fold away, flat),
// interleaved per-field stores between the subtractions (75.3%), and T&
// field references (81.5% all three, flat with only x or only y). The
// prologue tie never moved in any of them, so the register swap is still
// unexplained; the instruction stream is byte-identical from the `sub`
// pair onward apart from the ecx/esi names.
// Third pass (space-bunny-free), still 84.7%, and a new fact about the tie:
// writing the x difference with the operands the other way round
// (`b.x = a.x - b.x`) is the only rewrite found that makes MSVC hoist b.x's
// load above `push ebx` into ecx, and it then reproduces the original's
// first ten instructions byte for byte, `mov ecx, [esp+0x1c]` ... `sub ecx,
// ebx` included. It cannot be the source: with a.x as the minuend the
// destination can no longer be a.x's own register (ebx is live across the
// loop), so the compiler copies it first and the function grows two bytes
// (`mov esi, ebx; sub esi, ecx`) and drops to 75.5%. So the original really
// is `<hoisted temp> - <loop-live register>`, i.e. `b.x -= a.x`, and the hoist
// of that one load is a scheduling decision no spelling reached. All 48
// combinations of the three subtraction orders and the two operand orders of
// each subtraction were scored this pass: the unflipped y-first orders stay
// best at 84.7%, the flipped ones are 67.5 to 75.5% and 354 to 358 bytes.
// Also re-scored this pass, all 354 bytes but worse: the house-style inline
// `operator-` (free const-ref, as at 0x40beb0, and a const member, as at
// 0x404730) with its body in x,y,z / y,x,z / z,y,x order (80.6 to 81.5%, all
// still stopping after the second instruction), a pointer to b modified in
// place (81.5%), the comma-operator and `-a.x` spellings of the same
// in-place subtractions (84.7%, identical code).
// Not matched yet, 84.7% (354 bytes, same size as the original). The only
// difference left is register allocation in the prologue: the original keeps
// the x difference in ecx (b.x is loaded into ecx before the register pushes,
// so `sub ecx, ebx`, the abs block runs in esi, and n ends up in esi), while
// here the difference lands in esi and the abs temporary and n use ecx.
//
// What made the jump from 75.3%: subtract in place on the by-value parameter
// (b.y -= a.y; b.z -= a.z; b.x -= a.x) and then copy it whole into the step
// vector (`Vec3 d = b;`). The struct copy is what keeps the original's dead
// store of the undivided x difference (`mov [esp+0x10], ecx`) and drops the z
// one; field-by-field copies, a local per difference, a Diff() helper, a
// `d = b` before the subtractions and an aggregate initialiser all score lower
// (71 to 82%). The y step is the undivided difference, as the original has it.
// Scored without changing 84.7%: all six subtraction orders (yzx and yxz are
// best), the three spellings of the abs maximum, `/=` vs `= x / n`, taking abs
// and the divisions from b or from d, n/i/c/best declared up front in every
// order, spelling each subtraction five ways, and 1500 random mixes of in-place
// and local differences.
//
// deepseek-v4.1-flash additions (all still 84.7% or lower): all 128 header
// sets from tools/headers.py (best 84.7% with <stdlib.h>), a Sub/MaxAbs
// static inline helper in every field order, separate `int` difference
// locals built into d, an explicit max local, __max, swapped abs order and
// swapped division order, `Vec3 p = a` copies, and six-arg layouts. The
// original loads b.x into ecx before the callee-saved pushes (a.y already
// owns esi there, and esi is reused for abs(dx)/n), while ours reuses esi for
// b.x and gives ecx to abs(dx)/n. Nothing tried moves that one choice.
//
// deepseek-v4.1-flash third pass (retry), still 84.7%, 354 bytes. Re-derived
// the allocator tie precisely: original has dx in ecx (b.x loaded into ecx
// before `push ebx`) and abs(dx)/n in esi; ours has dx in esi and abs(dx)/n in
// ecx. Scored every sign/operand-order spelling of the x and z subtractions
// (`b.f -= a.f`, `b.f = a.f - b.f`, `b.f = -a.f + b.f`, `b.f = b.f + -a.f`):
// the reversed x form (`b.x = a.x - b.x`) is again the only one that hoists
// `mov ecx, [b.x]` above the pushes (it reproduces `mov ecx, [esp+0x1c]`), but
// it then costs `mov esi, ebx; sub esi, ecx` (356 bytes, 75.5%) because a.x
// owns ebx and is loop-live; every other mixed form is 84.7% or lower. Also
// re-tested: n computed from the raw parameters before the differences (77.4%),
// d declared before n (77.4%), separate y/x/z int locals rebuilt into d with
// every division order (62.3 to 69.6%), and the earlier forms. The `b.x`
// hoist and the ecx/esi assignment are one allocator decision that no spelling
// reached; the instruction sequence after the first two instructions is
// identical. Everything below the division (the whole loop) is byte-identical.
//
// Second pass (deepseek-v4.1-flash), still 84.7%: the whole hunk is the one
// register pair dx=ecx/abs=esi (original) vs dx=esi/abs=ecx (ours). Tried
// again and ruled out: `Vec3_004851c0 d = b - a` with an inline operator-
// whose body order is xyz, yxz, yzx, and by-value or const-ref parameters
// (80.6 to 81.5%); `Vec3 d = b; d -= a;` (82.3%), and the copy before and
// after the subtractions, `d(b)`, default-construct then assign, d declared
// at function scope, and field-by-field copies (74.5 to 84.7%): every form
// that keeps the copy after the in-place subtraction reproduces the byte
// sequence and the dead `mov [esp+0x10], dx`, so the source shape is right.
// Also tried: all six subtraction orders, six raw int component locals
// initialised in the original load order (b.y, b.x, a.x, a.y, a.z, b.z) then
// subtracted, n computed from the parameter fields before the copy, and a
// reference alias of b. tools/headers.py confirms no header set beats 84.7%.
// The original's b.x load is hoisted above the push ebx and above a.y's
// load, which is a scheduler/allocator decision this source shape does not
// reach; the instruction sequence is otherwise identical.
//
// deepseek-v4.1-flash fourth pass (retry), still 84.7%, 354 bytes. Confirmed
// this is a compiler-state tie, not a source shape: the unused-declaration
// sweep (`extern int dummyN;` for N = 0..400, step 4) is flat at 84.7%, and
// tools/headers.py reports all 128 header sets flat at 84.7% (closest
// <stdlib.h>). Also scored this pass, all worse: a by-value member
// `operator-` in every one of the six body component orders (80.6 to 81.5%)
// and all six in-place subtraction orders (yxz and yzx stay best at 84.7%).
// The single remaining hunk is dx in ecx plus abs(dx)/n in esi (original)
// versus dx in esi plus abs(dx)/n in ecx (ours); both instruction streams are
// identical from the division onward. Left as-is per the flat-sweep rule.
// GPT-6.1-sol retry in issue #3205: baseline remains best at 84.7%.
// Register-qualified per-component locals for dx/dy/dz fell to 71.3%.
// Hoisting b.x to a local before the y/z in-place differences stayed at
// 84.7% with the same dx=esi versus dx=ecx allocation swap in the prologue.
#include <stdlib.h>

#pragma pack(push, 1)
struct Cell_004851c0 {
    unsigned short unit;               // +0x0
    char unknown_2[0x4 - 0x2];
    unsigned char height;              // +0x4
    char unknown_5[0x8 - 0x5];
    unsigned short field_8;            // +0x8
    char unknown_a[0xd - 0xa];
};

struct Type_004851c0 {
    char unknown_0[0x16e];
    int field_16e;                     // +0x16e
};

struct Unit_004851c0 {
    char unknown_0[0x6e];
    int field_6e;                      // +0x6e
    char unknown_72[0x92 - 0x72];
    Type_004851c0* type;               // +0x92
    char unknown_96[0x118 - 0x96];
};

struct Game_004851c0 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x1426f - 0x1423b];
    unsigned char* mapping;            // +0x1426f
    char unknown_14273[0x14287 - 0x14273];
    Cell_004851c0* cells;              // +0x14287
    char unknown_1428b[0x14357 - 0x1428b];
    Unit_004851c0* units;              // +0x14357
};
#pragma pack(pop)

extern Game_004851c0* g_game;

struct Vec3_004851c0 {
    int x;
    int y;
    int z;
};

static inline Cell_004851c0* GetCell(int x, int y)
{
    if (x >= 0 && x < g_game->width && y >= 0 && y < g_game->height)
        return &g_game->cells[y * g_game->width + x];
    return 0;
}

// FUNCTION: 0x4851c0
int __stdcall FUN_004851c0(Vec3_004851c0 a, Vec3_004851c0 b)
{
    b.y -= a.y;
    b.z -= a.z;
    b.x -= a.x;
    Vec3_004851c0 d = b;
    int n = (abs(d.x) < abs(d.z) ? abs(d.z) : abs(d.x)) / 0x100000 + 1;
    d.x /= n;
    d.z /= n;
    short best = 0;
    for (int i = 0; i <= n; i++) {
        Cell_004851c0* c = GetCell(a.x / 0x100000, a.z / 0x100000);
        if (c) {
            short v = g_game->mapping[c->field_8 * 256 + 0xfa] + c->height;
            if (best < v) best = v;
            if (c->unit) {
                short w = (g_game->units[c->unit].type->field_16e + g_game->units[c->unit].field_6e) >> 16;
                if (best < w) best = w;
            }
        }
        a.x += d.x; a.y += d.y; a.z += d.z;
    }
    return best;
}