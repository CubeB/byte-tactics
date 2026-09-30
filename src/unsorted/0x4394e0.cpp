// Decompiled by DeepSeek V4.1 Flash, finished by Space Bunny Free, deepseek-v4.1-flash, GPT-6.1-sol, and Space Bunny Free. , edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
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

// FUNCTION: 0x4394e0
void __stdcall FUN_004394e0(void* surface, View_004394e0* view,
                            Node_004394e0* order, Pos_004394e0* out, int flag)
{
    Pos_004394e0 start = *out;
    FUN_00439740(surface, view, order, out, flag);
    if (flag == 0)
        return;

    int t = __max(g_game->frame - order->timestamp, 0);
    int dy = out->y.value - start.y.value;
    int dz = out->z.value - start.z.value;
    int dx = out->x.value - start.x.value;
    int dist = (int)sqrt((double)dx * dx + (double)dy * dy + (double)dz * dz);
    if (dist < 0x10000)
        return;

    int pos = (t % 30) * 0x300000 / 30;
    Anim_004394e0* anim = g_game->anims[21];
    unsigned short len = anim->field_2c;
    int frames = len < 1 ? 1 : (int)len;
    int idx = (t / frames) % anim->count;

    for (; pos < dist; pos += 0x300000) {
        int f = (int)(((__int64)pos << 16) / dist);
        Pos_004394e0 d = offset_004394e0(dx, dy, dz, f);
        Pos_004394e0 p;
        p.x.value = start.x.value + d.x.value;
        p.y.value = start.y.value + d.y.value;
        p.z.value = start.z.value + d.z.value;
        FUN_004b7f90(surface, *(void**)((char*)anim + idx * 8 + 0x28),
                     p.x.whole - view->scroll_x + 0x80,
                     p.z.whole - (p.y.whole >> 1) - view->scroll_y + 0x20);
        idx = (idx + 1) % anim->count;
    }
}
