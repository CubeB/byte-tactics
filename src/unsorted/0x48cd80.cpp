// Decompiled by GPT-5.6-Terra, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Partial: 92.6% (431 bytes vs 428). Prologue, branch A and the whole branch B
// loop body match; only three instructions of the branch B preheader differ:
//   original: mov edx,[eax+0x14363]; mov eax,[eax+0x1436b]; cmp eax,edi;
//             mov edi,[esi+4]; mov esi,[esi]; mov [esp+0x18],eax
//   ours:     mov ecx,[eax+0x1436b]; mov edx,[eax+0x14363]; cmp ecx,edi;
//             mov esi,[esi]; mov edi,[eax+0x2c7a]; mov [esp+0x18],ecx
// The 3 extra bytes are the `mov edi,[eax+0x2c7a]` (6 bytes) that should be
// `mov edi,[esi+4]` (3 bytes): our loop reads the y coordinate as
// g_game->view.y, the original reads it through p, which also lets g_game die
// at the count2 load (so count2 lands in EAX, not ECX).
// Third pass (Space Bunny Free, notes at the bottom of this block): the missing
// piece is now identified but not reproduced. Two shapes get the prologue AND
// the whole branch B preheader byte-identical: a conditional redefinition of d
// or result after the squares (95.2%, residual = the guard's 9 bytes) and the
// && written as a value in an int (94.5%, residual = the bool materialization).
// Both work by adding a merge point inside the loop body, and every spelling
// without such a merge is locked at 71.2% (mirrored p/zero) or at this 92.6%.
// A file claiming one of those statements would be 9 or 10 bytes further from
// the original than this one, so neither is in the file; both are kept in
// build/scratch/0x48cd80/ (v/g5_guard_d.cpp, v/q2_int.cpp) with their scores.
// Structural result (mimo-v2.6-pro, extended by Space Bunny Free): the
// both-from-p form (dy = s->y - p->y; dx = s->x - p->x) compiles to the exact
// mirror of the target preheader (s load first, count2 in EAX, mov esi,[edi+4];
// mov edi,[edi]) but the whole function then swaps p to EDI and the zero
// constant to ESI (also swapping the p and counter spill slots and reloading p
// through the stack in branch A), scoring 71.2 to 72.7%. Any spelling that
// keeps g_game alive in the loop keeps p=ESI (92.6% here, 90.4% for dy-first
// with y from p and x from g_game->view.x, where p dies on the wrong load).
//
// Space Bunny Free notes (the p/result register choice, measured with check.py
// on scratch variants under build/scratch/0x48cd80/):
// The 0 in edi is the register copy of `result` (spilled to [esp+0x1c], its
// home at -12, in the prologue) and p owns the home at -8 that 0x48ce66 and
// 0x48ce79 reload, so p=esi with result=edi is the whole difference.
//  * loop reads g_game->view.y and p->x (this file): 92.6%, p=esi, right.
//  * loop reads only p->y (x from g_game): 90.4%, p=esi, one load too big.
//  * loop reads p->y then p->x, and branch A calls
//    FUN_0048c6a0(u, &g_game->view) instead of (u, p): 79.7%, and the
//    preheader is then byte-identical to the original (p=esi, count2 in eax,
//    mov edi,[esi+4], mov esi,[esi]). Only branch A differs, because p is dead
//    there: no p reloads, frame 0x8 instead of 0xc, and the call argument is
//    rebuilt as `add edx,0x2c76`.
//  * loop reads p->y then p->x with the branch A call using p: 71.2%. MSVC
//    gives p=edi and result=esi (the mirror of the wanted preheader), spills p
//    and reloads it from [esp+0x18] before every call, and swaps the p and
//    counter slots (-4 and -8).
//  * same, but with the two branch A tests the other way round
//    (FUN_0048c6a0(u,p) && u->field_a6 != 0, or the call first as a nested
//    if): 87.6%. That keeps p=esi and the preheader, but the flag test and the
//    push esi swap places, so branch A is wrong.
//  * a distance helper that takes the two coordinates as values
//    (int d = DistSq(s, p->y, p->x) with the subtractions inside the helper,
//    and the same with the Point passed by value): 92.6% with the same fine
//    score as this file, because MSVC normalises it back to the code above.
//    It does keep p=esi with both p reads present, which no other spelling
//    does, but it always emits the x load as the self-copy into p's register
//    first and then reloads the y from g_game. It is also a knife edge:
//    twelve plausible perturbations of it (nested tests, swapped operands, the
//    while form, ++s, one declaration group, best/s swapped, ...) all fall
//    back to 71.2%.
//  * LEAD, the closest this retry got: the same helper plus one statement
//    that materialises a difference before the squares, for example
//        if (dx == 0x7fffffff) dx = 0;   (or the unsigned form, 0xffffffff)
//    scores 93.8% and the preheader is then byte-identical to the original
//    (s load first, count2 in eax, mov edi,[esi+4], mov esi,[esi], counter
//    store), leaving only the three instructions of that guard. It is not in
//    this file because such a guard has no business in the source, but it
//    shows the preheader is reachable, and the open question is only what
//    makes MSVC hoist both p loads instead of reloading the y from g_game. A
//    sign fixup on dx or dy inside the same helper does the same thing
//    (91.2% and 87.2%) for the same reason.
// Space Bunny Free, second pass (60 min, ~50 variants scored with --sym): the
// best lead so far is the blocking guard on d, NOT on dx:
//     for (...) { int dy = s->y - p->y; int dx = s->x - p->x;
//                 int d = dx*dx + dy*dy;
//                 if (d == 0x7fffffff) d = 0;      <-- this one line
//                 if (d < 4 && d < best) { best = d; result = s->field_0; }
//                 s++; }
// scores 95.2% (437 bytes vs 428): prologue, both calls, branch A, the whole
// branch B preheader (mov edi,[esi+4]; mov esi,[esi]; count2 in EAX) and the
// whole loop body match, and the only difference left is the guard's own three
// instructions (cmp eax,0x7fffffff; jne; xor eax,eax, 9 bytes). The dx form of
// the same guard gives 93.8%, a guard on dy 91.2%: it is the extra
// redefinition of d after the squares that flips the allocation, not the dx
// sign fixup. Not in this file because those 9 bytes are not in the original.
// Measured and rejected in this pass (all with the both-from-p dy-first body,
// which is the only spelling that gets the preheader right):
//   declaration order of p/result and one extra unused int local, in all six
//   orders: 71.2% every time. Unused static inline helpers at file scope, five
//   shapes x N=0..4: no change at all (VC5 deletes them before allocation).
//   dead store between the p load and the loop (`int t = 0; if (t) best = 0;`,
//   a dead for, a dead store to s), p as `Point* const`, p as const Point*,
//   the guard written d<4&&d<best / nested ifs / else-if / !(d>=4) / 4>d,
//   continue forms, self-assignments dx=dx, dy=dy, dx+=0, d=d, squares swapped,
//   DistSq4(s->x,p->x,s->y,p->y) and DistSq4 with y first (80.9%), a
//   DistSq(Slot*,Point*) helper y-first (71.2%) and x-first (72.7%), the same
//   helper taking the Point by value (65.0%), s indexed by count2-i (67.9%),
//   a second pointer q=p in the loop, and reading the first rect through
//   g_game->view.x/y instead of p: every one of them 71.2% or lower.
//   The y-from-p, x-from-g_game mix is 90.4% and its preheader puts py in ESI
//   and px in EDI (the mirror of the wanted one), which is the same mirror the
//   both-from-p form has.
// Space Bunny Free, third pass (the mechanism is now known, the neutral spelling
// is not). TWO shapes produce the byte-identical branch B preheader and the
// byte-identical prologue, i.e. the original's p=esi / zero=edi allocation and
// the count2-in-EAX schedule; both leave only their own extra instructions:
//   A) a conditional redefinition of a value that is live after the merge, just
//      after the squares:
//          int d = dx * dx + dy * dy;
//          if (d == 0x7fffffff) d = 0;          (or: result = 0;)
//          if (d < 4 && d < best) { best = d; result = s->field_0; }
//      95.2% (437 bytes vs 428), residual = the guard itself
//      (cmp eax,0x7fffffff; jne; xor eax,eax, 9 bytes). Redefining dy there
//      instead does nothing (71.2%, the assignment is dead-store eliminated),
//      redefining best only reaches 90.8%, and testing dx instead of d
//      makes it worse (60.8%). File: build/scratch/0x48cd80/v/g5_guard_d.cpp,
//      or try_guard.cpp (same source, notes stripped).
//   B) the && as a value instead of a short circuit:
//          int ok = d < 4 && d < best;   (or bool ok, or while (ok) { ... break; })
//          if (ok) { best = d; result = s->field_0; }
//      94.5% (441 bytes), residual = the bool materialization VC5 will not fold
//      (mov ecx,1; jmp; xor ecx,ecx; test cl,cl; je). Files:
//      build/scratch/0x48cd80/v/q2_int.cpp and the permuter's independent
//      spelling of the same idea (build/permute/0x48cd80/best.cpp, 94.5%).
// So the trigger is an extra MERGE POINT in the loop body for a value live
// across it, not a register-allocation detail of the preheader: every spelling
// without such a merge (about sixty of them, listed below) is locked at 71.2%
// with the p/zero pair mirrored, and the merge always costs code, so the
// original's merge must be somewhere this reconstruction has not found yet.
// Locked at 71.2% in this pass, all with the both-from-p dy-first body:
//   a temp for p->x read inside the loop, a temp for s->y, the two squares in
//   temps, (int) casts on p and on p->x, a cast on the FUN_0048c6a0 argument,
//   p from a cast address, do-while and while and while(1)/break loops, an
//   outer `if (count2 > 0)` around the loop, comma in the for increment, s++
//   before the body, the loop indexed as list2[count2-i], dx/dy/d/e/i
//   pre-declared outside the loop (one to three extra locals, 72.7%),
//   the two-step `int d = dx*dx; d = d + dy*dy;` and `d +=`, two-step squares,
//   the result assigned before best, a temp for s->field_0, `result = result`,
//   a pre-declared `unsigned short prev`, dead stores of result/d/best behind a
//   folded condition, `if (!(d<4 && d<best)) else`, `d >= 4 || d >= best` with
//   an empty then, goto/continue forms, and (d<4 && d<best) == 1.
// The two extra permuter runs of this pass are plateaus, as expected: seed 21
// from lead B, 2988 candidates, 94.5% -> 94.5%; seed 33 from lead A, 2023
// candidates, 95.2% -> 95.2%. Nothing there can drop the invented statement,
// so the next attempt has to find the merge in the source, not around it.
// tools/permute.py on the both-from-p body, seed 11, 9 min, 3423 candidates:
// 71.2% -> 94.5% (fine score 739 -> 558), winners extract_helper+cast then
// strip_parens+temp_intro; it re-found lead B on its own, and its best.cpp
// keeps the preheader right. Two more runs (seed 21 from lead B, seed 33 from
// lead A) log to build/scratch/0x48cd80/permute_ok.log and permute_guard.log.
// Rule found: p keeps esi while it is used at most once after the second call,
// or while FUN_0048c6a0 is evaluated before the field_a6 test. The original
// does neither, so its branch A must differ from the nested-if shape in some
// other way that still produces its exact code.
// Tried with the flag test first and the loop through p, all still 71.2%:
// && instead of nested ifs, the two tests reversed, truthiness
// (if (u->field_a6)), continue forms, an explicit != 0 on the call, a shared
// function-level loop counter, the counter declared outside the for, a while
// loop with n--, s/best/ids declaration swaps, ids[i] versus pointer
// arithmetic, no u variable, no def variable, the FixMul chain inlined or in a
// helper, Drawable()/Score() inline helpers, else { if } instead of else if,
// the negated first test, const/char*-cast/int*-typed p, p[0].y, (*p).y, an
// uninitialised result, result assigned rather than initialised, p declared and
// assigned separately, a second pointer for the loop (q = p or
// q = &g_game->view), DistSq/DistSq2 helpers, InRect()/View() helpers.
// tools/permute.py, four runs, no gain in any of them: 7988 candidates from
// this file, 3363 from the 87.6% call-first variant, 2675 from the value-helper
// variant that is codegen-identical to this file, and 1068 from the 93.8% guard
// variant. It only rewrites the function and its inline helpers, so it cannot
// reach the p->y versus g_game->view.y axis on its own.
// Earlier retries (mimo-v2.6-pro, v91 to v108, and before that) worked on the
// both-from-p base and all stayed at 71 to 73%: DistSq(Slot*,Point*) and
// DistSq2(int,int,int,int) helpers, the products written inline in both
// operand orders, Point* const p, const Point* p, a Point& alias, q = p
// copies, q = &g_game->view, cast-based address forms, int* p[i], uninitialised
// dx/dy, a p/result declaration swap, pre-read int n = g_game->count2 and
// early-return control flow. So the p/zero colour choice has resisted every
// source perturbation so far, and the gap is the last three instructions.

#pragma pack(push, 1)
struct Point_0048cd80 {
    int x;                             // +0x0
    int y;                             // +0x4
};

struct Rect_0048cd80 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct UnitDef_0048cd80 {
    char unknown_0[0x176];
    int field_176;                     // +0x176
    int field_17a;                     // +0x17a
    int field_17e;                     // +0x17e
};

struct Unit_0048cd80 {
    char unknown_0[0x92];
    UnitDef_0048cd80* def;             // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    unsigned short field_a8;           // +0xa8
    char unknown_aa[0x118 - 0xaa];
};

struct Slot_0048cd80 {
    unsigned short field_0;            // +0x0
    int x;                             // +0x2
    int y;                             // +0x6
};

struct Game_0048cd80 {
    char unknown_0[0x2c76];
    Point_0048cd80 view;               // +0x2c76
    char unknown_2c7e[0x142bb - 0x2c7e];
    Rect_0048cd80 rect_142bb;          // +0x142bb
    char unknown_142cb[0x14357 - 0x142cb];
    Unit_0048cd80* units;              // +0x14357
    Unit_0048cd80* units_end;          // +0x1435b
    unsigned short* list;              // +0x1435f
    Slot_0048cd80* list2;              // +0x14363
    int count;                         // +0x14367
    int count2;                        // +0x1436b
    char unknown_1436f[0x37e27 - 0x1436f];
    Rect_0048cd80 rect_37e27;          // +0x37e27
};
#pragma pack(pop)

extern Game_0048cd80* g_game;

int __stdcall FUN_004b6720(Rect_0048cd80* rect, int x, int y);
int __stdcall FUN_0048c6a0(Unit_0048cd80* unit, Point_0048cd80* p);

static inline int FixMul(int a, int b)
{
    return (int)(((__int64)a * b) >> 16);
}

// Rewriting the branch-B counter as a declaration-ordered while loop or as a
// top-tested while(1)/break loop did not improve it either (92.6% and 65.4%).
// FUNCTION: 0x48cd80
unsigned short __stdcall FUN_0048cd80(void)
{
    Point_0048cd80* p = &g_game->view;
    unsigned short result = 0;
    if (FUN_004b6720(&g_game->rect_37e27, p->x, p->y)) {
        int best = 0x7fff0000;
        unsigned short* ids = g_game->list;
        if (ids == 0)
            return 0;
        for (int i = 0; i < g_game->count; i++) {
            Unit_0048cd80* u = &g_game->units[ids[i]];
            if (u->field_a6 != 0) {
                if (FUN_0048c6a0(u, p)) {
                    UnitDef_0048cd80* def = u->def;
                    int v = FixMul(def->field_17a, 0x8000) + def->field_17e;
                    v = FixMul(v, def->field_176);
                    if (v < best) {
                        result = u->field_a8;
                        best = v;
                    }
                }
            }
        }
    } else if (FUN_004b6720(&g_game->rect_142bb, p->x, p->y)) {
        int best = 99999;
        Slot_0048cd80* s = g_game->list2;
        for (int i = g_game->count2; i > 0; i--) {
            int dx = s->x - p->x;
            int dy = s->y - g_game->view.y;
            int d = dx * dx + dy * dy;
            if (d < 4 && d < best) {
                best = d;
                result = s->field_0;
            }
            s++;
        }
    }
    return result;
}
