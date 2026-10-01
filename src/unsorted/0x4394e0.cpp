// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, deepseek-v4.1-flash, GPT-6.1-sol, and Space Bunny Free. , edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
// PARTIAL, 83.0%, 601 bytes, the original's exact size. 66.7 -> 83.0 this pass;
// see the THREE FINDINGS below (each one is a source shape that moved the
// register allocator, and each is now in the code below). The single remaining
// difference is a swap of `flag` and the snapshot's x between ebp and edi in
// the prologue, and the throwaway probe that proves the direction is recorded
// with it, so whoever picks this up does not have to re-find it.
// PASS (claude-sonnet-5-5, 2026-10-01): 66.7 unchanged (the permuter's only gain is the
// redundant parentheses in `dist`, internal score 2132 -> 2007, same percent). Model that fits all
// measurements: the prologue values {out, flag, order, x} take esi, edi, ebx, ebp in priority
// order, each the first register not held by a conflicting value. Ours colours out > order > x >
// flag (order=edi, x=ebx, flag=ebp); the original colours out > x > order > flag (x=edi,
// order=ebx, flag=ebp). Measured on ~250 variants:
//  * the 24 orders of {t, dy, dz, dx} x pos before/after the anim group x anim load before/after
//    the deltas: the prologue is either ours (t first) or {flag=edi, order=ebx, x=ebp} (t after the
//    deltas), never the original's. Scores 66.7 (current) and 66.2.
//  * extra uses of x ((start.x.value >> k) sums): 1 to 3 extra uses change nothing, 4 jump x above
//    out (x=esi); x never lands between out and order. Extra uses of flag lift flag above order.
//    An extra use of idx in the loop flips the loop pair to the original's (idx=ebx, pos=ebp) but
//    moves flag to ebx. A single extra out->y use with 3 extra x uses gives out=ebx, order=esi,
//    x=edi, flag=ebp (order and out swapped relative to the original).
//  * byte-identical to the base: the snapshot as a comma expression inside any call argument, a
//    scalar sx next to Pos start, wrappers around the call or the timestamp read (the latter is
//    worse), declaration order of pos/idx, while and for loop shapes (do-while is much worse),
//    three scalar snapshot locals (58.9). `Pos end = *out;` after the call: 55.
//  * the z snapshot IS stored: x, y, z are contiguous at [esp+0x40], [esp+0x44], [esp+0x48] (z is
//    the `mov [esp+0x50], edx` after two argument pushes), so the "BUG" remark below is wrong.
// tools/permute.py 15 min (--jobs 4): 490 candidates, no percent gain.
// RETRY (deepseek-v4.1-flash, 2026-09-30): still 65.2%, the ebx/edi swap is the
// only residual. No source shape tried here moved it: an `extern int dummyN;`
// sweep N = 0,4,...,400 (65.2 at N<=12, 64.2 above; compiler state is not the
// lever); `Node* node = order;`, a `Pos* p = out;` alias and a `void*`-only
// parameter list all compiled to the same 601 bytes at 65.2. An early genuine
// use of `order` (a timestamp field read, or an `int& ts = order->timestamp;`
// before the snapshot) DOES change the allocation, confirming the lever is the
// order in which `order` and the snapshot's x become live, but it lands `order`
// in eax/ecx, not ebx: 57.8% and 61.7%. What still differs: the original keeps
// `order` in ebx (loaded before `push edi`) and the snapshot x in edi, folds the
// timestamp into `sub eax,[ebx+0x46]` and holds g_game in ebp across the sqrt;
// ours gives ebx to the snapshot x and edi to order, so it reuses ebp for the
// timestamp (plus an extra instruction each), and every local slot sits 4
// higher with the deltas spilled into the dead argument slots.
// #2391 retry by deepseek-v4.1 (8 check.py runs, best 65.2%, same as the four
// earlier attempts): confirmed the ebx(<->)edi swap is allocation-driven and
// immune to more shapes than the notes list. Byte-identical output (same 601
// bytes, same 65.2%) for: a `Node_004394e0* node = order;` local before the
// snapshot (and used at the call and the timestamp), a `snap(order, out)` and a
// `snap(out, order)` inline helper returning *out, a `memcpy(&start, out, 12)`
// snapshot, and a callee prototype with the order/out parameters swapped so the
// call expression evaluates order before out. Worse: explicit member stores in
// declaration order 58.9% (599 bytes), reverse member order 58.9%, an early
// `int* ts = (int*)((char*)order + 0x46)` address-of use of order before the
// snapshot 61.7%. The print `mov ebx,[esp+0x60]` before `push edi` versus
// `push edi; mov edi,[esp+0x64]` is not reachable from the statement order of
// the copy, the call arguments or the callee declaration.
// #1529 retry by Codex / GPT-6.1-sol: checkall reconfirmed 65.2% (601/601 bytes).
// BREAKTHROUGH (space-bunny-free, 2026-10-01): 66.7 -> 77.5 at the same 601 bytes,
// from the matched sibling 0x40beb0's two idioms, applied together:
//   1. the three deltas live in a `Vec3` of PLAIN INTS, not in the Fixed-union
//      `Pos` (the old code read `out->x.value` etc. straight into three int
//      locals), and
//   2. the length is a member function `int Length() const` that converts each
//      component into its OWN `double` local before summing the squares.
// The guide's "Three filds kept on the x87 stack in x, y, z order: convert each
// component into its own double local" is what changed; writing
// `(double)dx * dx + (double)dy * dy + ...` lets VC5 fold the products and emit
// the `fld st(2) / fmul st(3)` block only because of how it reassociates. With
// `double fx = d.x; double fy = d.y; double fz = d.z; return
// (int)sqrt(fx*fx + fy*fy + fz*fz);` the three filds come out in x, y, z order
// and the whole sqrt block, the three `_ftol` pops and the `dist` compare now
// match. Only the prologue rotation is left (see below).
// SECOND FINDING (same pass): 77.5 -> 78.0, still 601 bytes, from taking a
// reference to the timestamp field before the call: `int& ts = order->timestamp;`
// then `g_game->frame - ts`. Per the guide ("A parameter pointer loaded before
// the first branch while yours loads it in each branch: take a reference to the
// field at the top (`int& m = obj->field;`)"). That alone fixes the dead
// argument slot the length goes to: the original stores dist at esp+0x64 (the
// now-dead `order` slot) and ours at esp+0x6c; with the reference ours matches.
// THIRD FINDING (same pass): 78.0 -> 83.0, still 601 bytes, from putting the
// whole walk loop behind an inlined member function on a small struct that
// carries the loop state (`struct Trail_004394e0 { Pos start; Vec3 delta;
// int dist, pos, idx; void* surface; View* view; Anim* anim; void Run(); }`),
// and computing the three deltas BEFORE the timestamp clamp. Of the four
// combinations (loop inline vs behind Run(), deltas before vs after the
// clamp) the scores are 77.0 / 77.5 / 82.0 / 83.0, so the struct boundary is
// worth ~4 points and the delta order ~1: the inlined boundary is what lets
// `order` reach ebx, the register the original loads it into, and everything
// downstream follows (the timestamp folds into `sub eax,[ebx+0x46]`, g_game
// stays in ebp across the sqrt, and idx/pos take ebx/ebp as in the original).
// Measured and rejected on this baseline: a Trail that stores POINTERS to
// start/delta instead of copies (52.8%, 549 bytes), a `Make_...()` helper that
// builds and returns the Trail by value (54.7%, 719 bytes), the whole walk as
// a free helper taking the Pos by value (70.4%), a per-iteration `Step()`
// method called from a caller-side loop (78.0%), and the guard as a positive
// `if (flag != 0) { ... }` block (77.5%). Passing the loop state through a
// constructor-like init list is byte-identical to the eight assignments.
// What still differs is the SAME rotation one step along the register list:
// the original loads the five parameters into {esi, ebp, ebx} = {out, flag,
// order} and puts the snapshot's x in edi; ours loads {esi, ebx, edi} =
// {out, order, flag} and puts the snapshot's x in ebp. So `flag` and
// `start.x` are still swapped relative to the original, which cascades into
// `mov ebp, [g_game]` becoming a reload into edx, `mov ebp,[ebp+0x148d3]`
// into ecx, and the `frames`/`anim` spills at esp+0x68/0x6c.
//
// THE RESIDUAL, WITH THE PROBE THAT SETTLES ITS DIRECTION (all in
// build/scratch/0x4394e0/, tmpl.cpp plus the pf*/c1* probe scripts):
//   original  mov esi,[out] / mov ebp,[flag] / mov eax,esi / mov ebx,[order]
//             / push edi / mov edi,[eax]        <- snapshot x into edi
//   ours      mov esi,[out] / mov eax,esi / mov ebx,[order] / push edi
//             / mov edi,[flag] / mov ebp,[eax] <- snapshot x into ebp
// A throwaway probe with ONE extra genuine use of `flag` (`if (flag + flag ==
// 0x1234) dist = 0;` after Length()) does land the three parameters where the
// original has them, {esi, ebp, ebx} = {out, flag, order}, in every loop shape
// tried (pf1, sw11, shA1/shB1/shC1/shD1). So `flag` is the one that is one use
// short, and the lever is a THIRD reference to `flag`, which the source only
// has two of (the argument push and the `test`). The cost is that the probe
// then drops start.x out of the callee-saved set entirely (`mov ecx,[eax]`)
// instead of moving it to edi, so the natural construct has to add the use
// without losing the register. Adding uses of start.x instead does nothing:
// with the extra flag use present, 1, 2 and 3 extra uses of start.x all leave
// the prologue byte-identical (c10-c13). Measured and rejected at this baseline:
// seven orders of the Trail struct's fields (all 83.0, byte-identical, so the
// frame slots follow the assignments in the caller, not the declaration order),
// Run() taking the Pos by value and by const reference instead of a member
// (83.0), `Run` as a free helper (70.4%), a `Trail` of pointers instead of
// copies (52.8%), a `Make_Trail()` helper returning the struct by value (54.7%),
// a per-step `Step()` method under a caller-side loop (78.0%), the guard as a
// positive block (77.5%), `Pos start; start = *out;` (83.0, byte-identical),
// a `Pos& dst = *out` alias (83.0, byte-identical), the flat frac/whole `Pos`
// of the matched 0x438c00 read through `*(int*)&x.frac` (83.0), `Length` as a
// free function instead of a member (83.0), a `Game*& game = g_game` alias
// (83.0), an unsigned `dist` compare (82.5), the deltas in dy/dz/dx order
// (81.5), advancing `idx` at the top of the loop (75.1), a hand-written clamp
// instead of `__max` (61.1, 611 bytes: that one is the MIRROR rotation,
// {flag, order} in {edi, ebp} and start.x in ebx, so the clamp spelling and the
// register pair are coupled), `g_game->frame` written twice instead of cached
// (75.6), and eight extra `#include`s - string.h, memory.h, windows.h, ctype.h,
// float.h, limits.h, time.h, assert.h - all byte-identical at 83.0.
// tools/permute.py 15 min (--jobs 4) on this file: 1382 candidates (60 did not
// compile, 13 duplicates), 83.0 -> 83.0, internal score 1772 -> 1772, so the
// whole statement/declaration/temporary/loop-form space is exhausted here and
// the residual is an allocator decision no rewrite reaches.
// #1529 retry by Codex / GPT-6.1-sol: checkall reconfirmed 65.2% (601/601 bytes).
// Four worker checks found no better version; the remaining mismatch is the register/stack-slot rotation described below.
//
// Third pass (space-bunny-free, #1881): still 65.2%, the best of six more shapes,
// so the ebx/edi rotation is still the whole difference. NEW FACT for whoever
// picks this up, from working out the frame layout by hand (esp = E-0x58 after
// the call, E = entry esp, so the local slots are E-0x58..E-0x10):
//  * The snapshot copy at E-0x24/E-0x20 (x, y) and the products at E-0x0c/E-0x08
//    plus the loop's eight 16.16 spill pairs at E-0x48..E-0x2c are all confirmed
//    from the 0x4394e0 + 0x4394e4 offsets; the deltas the fild reads are the
//    three slots E-0x30/E-0x2c/E-0x28 and the loop then reuses those two of them.
//  * THE Z SNAPSHOT IS NEVER STORED. The slot E-0x1c is read twice (in the
//    `sub esi, ecx` that makes dz and in the `add eax, edx` that makes p.z) and
//    never written anywhere in the function: the only pre-call stores are
//    `mov [esp+0x40], edi` and `mov [esp+0x40+4], ecx` at esp = E-0x64, i.e.
//    E-0x24 and E-0x20. See the BUG line in the pull request.
//  * But dropping that store is NOT the fix. Measured with check.py --sym on
//    build/scratch/0x4394e0/v1.cpp and v2.cpp (all with the same body):
//    three scalar snapshot locals with an unassigned third, sx, sy, sz: 591
//    bytes, 47.8%, and the same three declared in reverse order 46.8%. The
//    struct `Pos start; start.x = out->x; start.y = out->y;` (z left out):
//    591 bytes, 59.2%. `Pos start; start.x = o->x; ... start.z = o->z;` through
//    a `Pos* o = out` local: 599 bytes, 57.9%. A `Node* node = order;` local
//    declared BEFORE the Pos copy: 601 bytes, 64.2%, and after it 64.2%. So
//    every shape that removes the third store drops 10 bytes and loses points,
//    and introducing a local copy of the node parameter is worth -1.0: the
//    allocator is not driven by the declaration order of those two values.
//  * What the two builds really disagree about is the whole g_game lifetime:
//    the original keeps g_game in ebp from `mov ebp, g_game` (0x439523) all the
//    way to anims[21] at 0x4395a8, and folds the timestamp into
//    `sub eax, [ebx+0x46]`, freeing ebx for the clamp. Ours holds the node in
//    edi, has to materialise the timestamp into ebp (the register the clamp
//    result then wants) and reloads g_game for the anims lookup. That is all
//    downstream of which of {order, start.x} got ebx first, and four passes
//    have not found the source-level knob for that.
//
// Second pass (deepseek-v4.1-flash): confirmed the register swap is the root
// and it does not respond to source-level changes. Rewriting the snapshot as
// explicit field stores, `start; start = *out;`, a const Pos, `*(Pos*)out`, a
// named node local, `(order, *out)`, and an `(int)order | 0` no-op alias all
// still score 65.2%, i.e. the allocator always gives ebx to start.x and edi to
// the node parameter. The original does the opposite (node in ebx, start.x in
// edi), so the prologue, the delta spills into the argument slots and the whole
// loop register assignment cascade from that one choice.
// PARTIAL, 65.2%, ours is the same size as the original (601) and the
// instruction sequence now matches the original one for one. What the function
// does: it snapshots the position the caller passed in `out`, calls
// FUN_00439740 (which recomputes `out` and draws the unit type icon), and when
// `flag` is set walks the line from the snapshot to the new position in
// 0x300000 steps, drawing the object's animation frame at each step. The frame
// index is (g_game->frame - order->timestamp, clamped at 0) / max(1,
// anim->field_2c) % anim->count, and `anim` is g_game->anims[21].
//
// Re-derived from the disassembly; the earlier attempt's 36% notes were wrong:
//  - the length is the plain sum of squares. Its third fmul multiplies st(2),
//    which is the *duplicate* of dz pushed by the preceding `fld st(1)`, not
//    dy; reading it as dz*dy makes MSVC fold dy*dy + dz*dy into dy*(dy+dz) and
//    the whole x87 block can never match,
//  - the loop start is (t % 30) * 0x300000 / 30 exactly. The shr/add after the
//    magic multiply is MSVC's own round-toward-zero fixup, not a halving; a
//    probe of /30, /60, /30/2 and >>1 shows only the plain /30 emits it,
//  - nothing is written back to *out after the walk (the old source stored the
//    snapshot there again, which also made the function too long),
//  - the FUN_004b7f90 arguments are the 16.16 hi words, the same
//    `p.x - view->cx + 0x80` / `p.z - (p.y>>1) - view->cy + 0x20` shape the
//    matched sibling 0x439740 uses.
//
// The one structural thing still missing was the loop body: the original keeps
// the three 16.16 products in memory (each stored as its _allmul/_allshr pair
// finishes) and only then adds the snapshot to all three, while a plain
// `p.x.value = start.x.value + (int)(((__int64)dx * f) >> 16)` lets MSVC fuse
// each add into the product's store. Building the products in a small static
// helper that returns a Pos is what reproduces the spill and the deferred adds.
//
// What still differs is only register and stack-slot choice:
//  - the original keeps `order` in ebx and the snapshot's x in edi; ours has
//    them the other way round. That cascades: the original folds the timestamp
//    into `sub eax,[ebx+0x46]` and holds g_game in ebp across the sqrt, while we
//    load the timestamp into a register and reload g_game for anims[21] (one
//    extra instruction each, paid back by the two `mov`s the original spends on
//    `pos`),
//  - the original spills dist, frames and anim into the dead argument slots
//    (esp+0x64/0x68/0x6c) and keeps the deltas at esp+0x28..0x30; ours spills
//    the deltas into the argument slots and puts anim at esp+0x10, so every
//    local slot sits 4 higher. Declaration order, local and parameter names,
//    the spelling of the clamp and of the ternary, and dummy externs (1..12)
//    all left that unchanged.
//  - A timestamp helper and a snapshot-return helper were each inlined but left the same register assignment at 65.2%. The original register swap remains unresolved.
//    it.
// RETRY (deepseek-v4.1-flash, 2026-10-01, 10 min): the ebx(order)/edi(start.x)
// swap is still the whole residual and no shape tried here moved it (two-local
// copies o2 = order used at the call / at the timestamp, call arg evaluation
// order, const Pos, separate assignment, node copies: all 65.2 with the same
// prologue). ONE improvement did land: declaring the three deltas in the order
// dy, dz, dx (rather than dx, dy, dz) raises the checker score 65.2 -> 66.7
// (and p_yzx combined with the p.x/y/z store order xyz is the best of all 36).
// It does not touch the prologue; the gain is in the sqrt/timestamp block.
// A full 6x6 sweep of delta-declaration order x p.x/y/z store order was scored.
// Second 10-minute run on 2026-10-01: moving the timestamp clamp down, below
// the three deltas, scores 66.2 (same 601 bytes), so the clamp must stay above
// the deltas; the ebx(order)/edi(start.x) swap remains the whole residual.
// deepseek-v4.1-flash 10-minute pass (2026-10-01): `Pos start = out[0];` in
// place of `*out` and a `register` hint on the `order` parameter both compile
// byte-identically at 66.7% / 601 bytes, so the ebx(order)/edi(start.x) swap
// still stands as the whole residual.
//
// Retry (mimo-v2.6-pro, 2026-10-01, 55 min, 24 check.py runs): best unchanged
// at 66.7%. I probed the register-priority tie directly (per the guide's
// "one weighted use" and "pointer kept across a run of calls" patterns):
//  - a real extra use of order->timestamp, start.x.value or out->z.value at
//    nine positions (before the snapshot, between snapshot and call, after the
//    call, after the flag check, mid-deltas, after the loop, inside the loop),
//    a separate `Pos start; start = *out;`, explicit field stores, and a
//    `Game_004394e0* game = g_game;` local for the frame/anims reads all keep
//    the prologue {out: esi, start.x: ebx, order: edi, flag: ebp} (57.0-66.7%).
//  - the ONLY variant that moved registers: an extra
//    `if (order->timestamp > K) return;` between the call and the t
//    computation (62.9%). Its asm shows why: both timestamp reads CSE into one
//    load, order's range then ends at that load, and order/flag swap into
//    ebp/edi (start.x keeps ebx). Still not ebx for order.
//  - three Pos types (the Fixed union; the flat x_frac/x layout of the matched
//    sibling 0x439740; the caller 0x439b30's plain `int x,y,z` with a hi-word
//    helper) all compile 65.7-66.7% with the same prologue, so the type is not
//    the knob.
//  - NEW FACT: the loop's register pair is pure downstream of the clamp: idx
//    always reuses t's register and pos takes the other callee-saved one, so
//    matching the loop needs only t to land in ebx, which follows from the
//    prologue swap (order freed at [ebx+0x46] then clamp into ebx).
// What remains: swap {order, start.x} between ebx and edi in the prologue.
// 20+ shapes across six attempts have not reached it.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)

union Fixed_004394e0 {
    int value;                          // 16.16
    struct {
        unsigned short frac;
        short whole;
    };
};

struct Pos_004394e0 {
    Fixed_004394e0 x, y, z;
};

// The three deltas as plain ints, with the length helper of the matched
// sibling 0x40beb0. Two things there matter and both are visible here: the
// three components go through their own `double` locals, which is what keeps
// the three `fild`s in x, y, z order, and the difference is a struct the
// helper reads through memory, which is what keeps the three deltas in the
// original's esp+0x28..0x30 slots.
struct Vec3_004394e0 {
    int x, y, z;

    int Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
};

struct Node_004394e0 {                  // the unit order 0x439740 walks
    char unknown_0[0x46];
    int timestamp;                      // +0x46
};

struct View_004394e0 {
    char unknown_0[0x2c];
    int scroll_x;                       // +0x2c
    int scroll_y;                       // +0x30
};

struct Anim_004394e0 {
    unsigned short count;               // +0x0
    char unknown_2[0x2c - 2];
    unsigned short field_2c;            // +0x2c
};

struct Game_004394e0 {
    char unknown_0[0x1487f];
    Anim_004394e0* anims[22];           // +0x1487f, this function uses [21]
    char unknown_148d7[0x38a47 - 0x148d7];
    unsigned int frame;                 // +0x38a47
};

#pragma pack(pop)

extern Game_004394e0* g_game;

void __stdcall FUN_00439740(void* surface, View_004394e0* view,
                            Node_004394e0* node, Pos_004394e0* out, int unused);
void __stdcall FUN_004b7f90(void* surface, void* bmp, int x, int y);

// The three 16.16 steps of the interpolated position. The original keeps them
// in memory (the values are stored as each _allmul/_allshr pair finishes and
// the three adds with `start` happen only after all three), which is what
// building them in a helper returning a Pos reproduces.
static Pos_004394e0 offset_004394e0(int dx, int dy, int dz, int f)
{
    Pos_004394e0 d;
    d.x.value = (int)(((__int64)dx * f) >> 16);
    d.y.value = (int)(((__int64)dy * f) >> 16);
    d.z.value = (int)(((__int64)dz * f) >> 16);
    return d;
}

// The walk itself, as an inline method on a struct that holds the whole loop
// state. Per the guide, "an inlined function boundary changes the order MSVC
// evaluates things in and which registers it keeps values in"; moving the loop
// behind this boundary is what moves `order` out of edi and into ebx (the
// register the original loads it into).
struct Trail_004394e0 {
    Pos_004394e0 start;
    Vec3_004394e0 delta;
    int dist;
    int pos;
    int idx;
    void* surface;
    View_004394e0* view;
    Anim_004394e0* anim;

    void Run()
    {
        for (; pos < dist; pos += 0x300000) {
            int f = (int)(((__int64)pos << 16) / dist);
            Pos_004394e0 o = offset_004394e0(delta.x, delta.y, delta.z, f);
            Pos_004394e0 p;
            p.x.value = start.x.value + o.x.value;
            p.y.value = start.y.value + o.y.value;
            p.z.value = start.z.value + o.z.value;
            FUN_004b7f90(surface, *(void**)((char*)anim + idx * 8 + 0x28),
                         p.x.whole - view->scroll_x + 0x80,
                         p.z.whole - (p.y.whole >> 1) - view->scroll_y + 0x20);
            idx = (idx + 1) % anim->count;
        }
    }
};

// FUNCTION: 0x4394e0
void __stdcall FUN_004394e0(void* surface, View_004394e0* view,
                            Node_004394e0* order, Pos_004394e0* out, int flag)
{
    Pos_004394e0 start = *out;
    int& ts = order->timestamp;
    FUN_00439740(surface, view, order, out, flag);
    if (flag == 0)
        return;

    // The deltas before the timestamp: with this order the three `out` loads
    // are hoisted together the way the original hoists them at 0x439529.
    Vec3_004394e0 d;
    d.x = out->x.value - start.x.value;
    d.y = out->y.value - start.y.value;
    d.z = out->z.value - start.z.value;
    int t = __max(g_game->frame - ts, 0);
    int dist = d.Length();
    if (dist < 0x10000)
        return;

    int pos = (t % 30) * 0x300000 / 30;
    Anim_004394e0* anim = g_game->anims[21];
    unsigned short len = anim->field_2c;
    int frames = len < 1 ? 1 : (int)len;
    int idx = (t / frames) % anim->count;

    Trail_004394e0 tr;
    tr.start = start;
    tr.delta = d;
    tr.dist = dist;
    tr.pos = pos;
    tr.idx = idx;
    tr.surface = surface;
    tr.view = view;
    tr.anim = anim;
    tr.Run();
}