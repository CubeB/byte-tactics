// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 2982): best stays v1 (tick store at
// statement 3). The v4 shape puts the ff8 store/cmp pair after the six clears
// with the tick load hoisted (first block positionally exact), but rotates the
// whole register pool (zero ebx vs ebp, tick ecx vs edx, w ebp vs ebx). Neither
// dummy-extern sweeps (prepack/prefunc, N up to 200), register/type/pointer/
// reference tick temps, uninitialized-declaration orders, nor local hoists moved
// it, so it is a compiler-state pool tie.
// GPT-6.1-sol retry: best remains 98.3% (473 bytes). An equivalent `if (p->ref != 0) {} else` preserved the same diff; the conditional-expression form `p->ref = p->ref ? p->ref : new PlayerRef` scored 61.3%; moving the third tick store after the first six clears scored 96.6%. Restored the 98.3% source.
// Further refinement: moving the allocation guard after the six clears scored
// 68.9%; an empty `p->ref == 0` guard scored 97.5%. Restored 98.3%; no MATCH.
//
// deepseek-v4.1-flash retry (still 98.3%): the one hunk is the position of the
// `cmp eax, ebp` / `mov [esi + 0xf8], edx` pair. Moving `p->ff8 = g_game->ticks;`
// after the six zero stores gives 96.6 with the pair in the right slot but the
// scheduler emits every load just in time (the ref load sinks, the third tick's
// two loads split around the cmp, the 0x90 store hops above the ff8 store);
// nesting the same store in an expression (`p->f8c = (p->ff8 = g_game->ticks, 0);`)
// gives byte-for-byte the same 96.6. So the pair follows the ff8 statement's
// source position, and the original's source must both load the tick early and
// store it late, which needs a temporary. Every temp rotates the callee-saved
// pool (zero leaves ebp for ebx), which is the 66.4/70.6 family already noted.
// GPT-6.1-sol retry (5 checker invocations): baseline 98.3%. An empty `if (p->ref == 0) {}` inserted after the first six clears lowers this to 97.5% and moves the ref load below the block; duplicating an identical clear in both `if (p->ref)` arms drops to 63.3% and rotates the zero register. Restored the 98.3% best.
// Retry notes (Sonnet 5.5, still 98.3%): scripted searches that all failed to
// beat this file: every single move of each statement in the first block (the
// stores must keep the original's order anyway), the tick statement at every
// position among the 24 stores, chained zero assignments, wrapping every run
// of 1 to 11 adjacent statements (and every pair of runs up to 6 long) in a
// static inline function, inline helpers with the tick as a by-value, by-
// reference or pointer parameter, an inline "ensure the ref" helper, a
// "Stamp" helper for the three tick stores, an explicit "int z = 0" for the
// zero, defining the preceding function 0x4644d0 above this one, and all 128
// header sets (also with the C++ headers). Every variant that hoists the tick
// load above the six zero stores (a local, a parameter) also moves the zero
// from ebp to ebx (66%), the flip described below; none of the register
// spellings tried (w and h declaration order and style, multiply order, store
// order, extra locals) undoes it.
// Per player slot init: stamps the current tick into three fields, clears 22
// dwords and six shorts, allocates the 0x34-byte PlayerRef and the squads table,
// sizes and clears the map-cell buffer at (width/2) * (height/2) rounded up to
// eight, and gives an inactive or non-network (type 3) player a Class_00408cb0.
//
// The 22 zero stores come out in the order the source writes them (0xac, 0xb4,
// 0xbc, 0xc4, 0xcc, 0xd4, then 0x8c..0xc8, then 0xe8, 0xe4, 0xd0, 0xd8), so the
// statements are written in exactly that order rather than in address order.
// MSVC 5 never reorders stores, so that order is the original source's.
//
// MSVC 5 gives the first *declared* of two uninitialised locals ebx and the
// second edi, so the width/2 temporary is declared first even though the
// height/2 temporary is assigned first. That is what puts height/2 in edi and
// width/2 in ebx across the operator delete call, as the original has; with
// initialised locals (int h = ...; int w = ...) the allocation comes out the
// other way round.
//
// The last test is `!p->active || p->type != 3`, not `p->active && p->type != 3`:
// the original's first branch is `je` into the allocation and the second `je`
// over it, which is the `||` with both tests left as they are.
//
// STILL DIFFERS: only the first block, 14 instructions (96.6%). The original
// runs one basic block from 0x46470c to the `jne` at 0x4647cb and inside it
//   - loads g_game, p->ref and the tick for the +0xf8 store, all three at the
//     top (0x46472d-0x464739),
//   - then the six zero stores, the `cmp eax, ebp` at 0x464763, the +0xf8 store,
//     sixteen zero stores, four zero stores, and finally the `jne`.
// So the original's condition is evaluated *before* the eighteen instructions
// that follow it, and the tick load sits at the top of the block. Here the load
// and the `cmp` stay at their source positions (the `cmp` ends up at the end of
// the block, next to the `jne`), and the scheduler hoists p->ref's load two
// stores up and the +0x90 store one store up.
//
// What was tried and does not work (all give the plain "load at its store"
// order): a `static inline` helper for the ticks, a member getter, the whole
// body through a `Player&`, plain and C++ references to the +0xf8 field, casts
// of the address, six zeros as a loop, an array, a pointer walk, a `do{}while(0)`
// and bare nested blocks, and the tick statement before the six zeros.
//
// What does move the tick load to the top is a *local initialised there*:
//   int t = g_game->ticks;          // third statement
//   ... six zeros ...
//   p->ff8 = t;
// With that, plus `PlayerRef* ref = p->ref;` declared just before it, MSVC
// emits the three loads at the top in exactly the original's order and puts
// the `cmp` at 0x464763 - i.e. the whole block matches instruction for
// instruction except for two register choices: the tick lands in ecx (the
// g_game register is reused) instead of edx, and the constant 0 lands in ebx
// (`xor ebx, ebx`, all 22 stores from ebx) instead of ebp. Any extra local
// flips the zero from ebp to ebx and the width/2 local from ebx to ebp; dummy
// locals, declaration order, const, unsigned, long, and moving the width/height
// declaration around all leave it at ebx. Without the extra locals the zero is
// in ebp and the loads are wrong, so the two cannot be had at once with the
// shapes tried here. Scratch variants are in build/scratch/0x464700/ (v1 = this
// body's first block, u2 = the two-local form, i1 = this file).
//
// The two things the previous attempt could not fix are fixed now, both by the
// same trick (see the references at the memset): the phi of
// `p->buffer = size ? operator new(size) : 0` stays in eax and the memset
// reloads [esi+0x7c] into edi, and the size load lands after the phi store.
//
// 98.3 percent, up from 96.6, and the byte count matches (473). The one
// remaining hunk is two instructions. The original evaluates its `p->ref` null
// test early but sinks the `jne` all the way down to just before the
// allocation, so the `cmp` sits high and the flags survive the 23 zero stores:
//     mov edx, [ecx + 0x38a47]      ; third g_game->ticks, for the +0xf8 store
//     <six zero stores>
//     cmp eax, ebp                  ; the ref null test
//     mov [esi + 0xf8], edx
//     <seventeen zero stores>
//     jne <past the allocation>
// Both values live only in volatiles across that run (eax for the ref, edx for
// the tick), and the six zeros get ebp, which is the callee-saved register
// the original keeps for the constant 0 for the whole function.
//
// What this file does instead hoists the `p->ref` load correctly but puts the
// cmp and the +0xf8 store above the six zeros, so the pair sits on the wrong
// side of them. Getting the tick's LOAD hoisted above the zeros while leaving
// its STORE after them needs a temporary, and that is exactly what breaks it:
// every temp tried rotates the callee-saved pool and demotes the constant 0
// from ebp to ebx, which is worth far more than the two instructions gained.
// Confirmed for `int t = g_game->ticks;` used at the +0xf8 store (66.4 percent,
// zero now in ebx and the store sunk up with the load), for the same temp with
// `int w, h;` hoisted to the top of the function so declaration order could not
// be the cause (66.4 percent, identical rotation), and previously for
// `PlayerRef* rref = p->ref;` (61.3 percent, the ref load hoists correctly and
// the zero still moves to ebx).
//
// The useful lead: at 96.6 percent, with `p->ff8 = g_game->ticks;` in its
// natural place after the six zeros, the STORE and the cmp are already on the
// right side of them and only the tick's load is late. So the target shape is
// the 96.6 percent body with just that one load hoisted, and the obstacle is
// purely that MSVC 5 will not hoist a load without also giving the value a
// home that rotates the pool. A source form that makes the load cheap to hoist
// without introducing a named temporary is what is still needed.
//
// Retry notes (space-bunny-free, still 98.3%, 473 of 473 bytes). The whole
// difference is two instructions, and it is a block ORDER difference, class
// (d): the `cmp eax, ebp` and `mov [esi + 0xf8], edx` pair. They are adjacent
// to each other in the original and in this file, and in both they sit
// immediately before the store of the third tick, six statements earlier than
// the original when the tick is the third source statement. So the pair is
// scheduled with the store of the third tick, not with its own `if` (whose
// source position is 25): the `jne` is 18 instructions below the cmp, so the
// flags are live across the whole run of stores, and the cmp is not emitted at
// its source position in either order.
//
// Measured here, all 473 bytes, all scratch in build/scratch/0x464700/:
// v1 = this file, 98.3. v2 = `p->ff8 = g_game->ticks;` moved to after the six
// zero stores, 96.6: the pair then sits exactly where the original has it, but
// the ref load (`mov eax, [esi + 0xec]`) sinks two stores into the zero run,
// the third tick's two loads are emitted at the ff8 statement and split around
// the cmp (`mov ecx, [g_game]`, `cmp`, `mov edx, [ecx + 0x38a47]`), and the
// 0x90 store is hoisted one slot. So with the statement at its original
// position MSVC emits every load just in time.
// v4 = v2 plus `PlayerRef* ref = p->ref;` and `int t = g_game->ticks;` after
// the ff4 store, 66.4: the first block is then instruction for instruction the
// original's, in order and position, with only the two register differences
// the earlier note lists (`xor ebx, ebx`, all 22 stores and the cmp from ebx,
// the third tick in ecx as `mov ecx, [ecx + 0x38a47]`), and the whole tail
// follows the zero: width/2 in ebp, height/2 in edi, `mov dl` for the team,
// `bx` for the four shorts.
// v5 = v2 + the tick local only, 66.4, third tick in eax. v6 = v2 + the ref
// local only, 70.6: the ref load hoists to the top in exactly the right slot
// and the cmp is right, but the tick load stays late, the ff8 store lands one
// slot after the 0x8c store, and the zero is ebx. v11 = v4 with `int t`
// declared before `ref`, 65.5. v12 = v4 with `PlayerRef*& ref`, 59.9 at 471
// bytes. v13 = v4 with the six zeros chained (`p->fac = p->fb4 = ... = 0`),
// 66.4, byte for byte the same code as v4, so the chain changes nothing here.
// v14 = v4 with `int h, w;` in place of `int w, h;`, 64.7. v15 = v4 with the
// `int w, h;` declaration hoisted to the top of the function, 66.4, same as v4.
//
// So the two requirements are mutually exclusive in every shape tried: v2 is
// the only one whose registers are both right (constant 0 in ebp, third tick
// in edx) and its schedule is wrong, and v4 is the only one whose schedule is
// right and its registers are both wrong. The coupling is the register pool:
// the allocator hands out (zero, width/2) as (ebp, ebx) only in the shape with
// no enregistered local in the block, and as (ebx, ebp) as soon as one local
// appears, whatever the local is (v5, v6, v11, v13, v15 all rotate it). The
// unfixed question for whoever tries next is whether the pool can be pinned
// some other way, or whether a hoisted value can be produced without an
// enregistered local at all. Everything else in the function already matches.
//
// deepseek-v4.1 retry (still 98.3%, runs: 12). New negative results, all 473
// bytes and all scratch in build/scratch/0x464700/: (A) `p->ff8 = g_game->ticks
// + (six zero stores, 0)`, (B) the ff8 statement moved after the six stores,
// (C) `p->ff8 = (six zero stores, g_game->ticks)`, (D) the same with the store
// slotted between fac and fb4, (G) `p->ff8 = (g_game->ticks, six stores,
// g_game->ticks)` (the discarded first comma operand is dropped by the front
// end, so this is B), (I) the same with the operands of the `+` reversed. Every
// one of them gives 96.6 with an identical shape: the ff8 STORE lands in the
// original's slot, but the ticks load and the cmp stay down at the statement
// (ac, b4, ref load, bc, c4, cc, d4, g_game load, cmp, ticks load, 0x90 store,
// ff8 store), i.e. MSVC will not hoist a load above the may-aliasing stores
// and the store always travels with the statement that contains the load. So
// the store can be placed either with the loads (v1, this file) or after the
// six clears (A/B/C/G/I/96.6), never split from them, without a named local.
// (E) `int z = 0;` plus `int t = g_game->ticks;` for the six clears and the
// store, and (E3) the same with `z` declared after the two tick loads, both
// 66.4: any local rotates the pool (`xor ebx, ebx` instead of `xor ebp, ebp`),
// so the 98.3 percent body stays the best.
//
// deepseek-v4.1 retry (2nd pass, still 98.3%, 18 checker runs). The first-block
// statement layout is what pins the pool: it is NOT only extra locals, any move
// of the `p->ref` guard does it too. New negative results, all 473 bytes:
// guard moved before the tick store with the store after the six clears (s3)
// 68.9, guard between the two tick loads and the six clears (s5) 67.2, tick
// store then guard then six clears (s4) 68.9, s3 with `int h, w` 67.2, s3 with
// `int w = g_game->width / 2; int h = ...` 66.4, s3 with `int w, h;` hoisted to
// the top of the function 68.9, s3 with the six clears pair-chained
// (`p->fac = p->fb4 = 0;` x3) 68.9, and the six clears plus the tick store as
// one comma expression 96.6 (store in the original's slot, load still late).
// Every reorder flips the constant 0 from ebp to ebx and width/2 from ebx to
// ebp, and the whole tail follows, so the pool cannot be pinned from the first
// block while the schedule is wrong. The remaining hunk is unchanged: the
// original has `cmp eax, ebp` and `mov [esi + 0xf8], edx` after the six clears,
// this file has them before. Scratch: s2-s6, s3a-s3d in build/scratch/0x464700/.


#include <string.h>

class PlayerRef {
public:
    int unknown[12];
    void* player;
    void Reset(unsigned char playerIndex);
};

#pragma pack(push, 1)
class Class_00408cb0 {                 // 0x3d bytes
public:
    void* player;                      // +0x0
    unsigned char field_4;             // +0x4
    int countdown;                     // +0x5
    int field_9;                       // +0x9
    int field_d;                       // +0xd
    void* timers[10];                  // +0x11
    void* cursor;                      // +0x39
    Class_00408cb0(void* p);
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Player_00464700 {
    int active;                        // +0x00
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
    Class_00408cb0* unit;              // +0x74
    char unknown_78[0x7c - 0x78];
    void* buffer;                      // +0x7c
    int f80;                           // +0x80
    int f84;                           // +0x84
    int f88;                           // +0x88
    int f8c;                           // +0x8c
    int f90;
    int f94;
    int f98;
    int f9c;
    int fa0;
    int fa4;
    int fa8;
    int fac;
    int fb0;
    int fb4;
    int fb8;
    int fbc;
    int fc0;
    int fc4;
    int fc8;
    int fcc;
    int fd0;
    int fd4;
    int fd8;
    char unknown_dc[0xe4 - 0xdc];
    int fe4;                           // +0xe4
    int fe8;                           // +0xe8
    PlayerRef* ref;                    // +0xec
    int ff0;                           // +0xf0
    int ff4;                           // +0xf4
    int ff8;                           // +0xf8
    short ffc;                         // +0xfc
    short ffe;                         // +0xfe
    short f100;                        // +0x100
    short f102;                        // +0x102
    short f104;                        // +0x104
    short f106;                        // +0x106
    char unknown_108[0x146 - 0x108];
    unsigned char team;                // +0x146
    char unknown_147[0x149 - 0x147];
    unsigned short flags;              // +0x149
};

struct Game_00464700 {
    char unknown_0[0x14233];
    int width;                         // +0x14233
    int height;                        // +0x14237
    char unknown_1423b[0x38a47 - 0x1423b];
    int ticks;                         // +0x38a47
};
#pragma pack(pop)

extern Game_00464700* g_game;
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
void __stdcall FUN_00480190(Player_00464700* p);
void __stdcall FUN_0040b320(int player);

// FUNCTION: 0x464700
void __stdcall FUN_00464700(Player_00464700* p)
{
    p->ff0 = g_game->ticks;
    p->ff4 = g_game->ticks;
    p->ff8 = g_game->ticks;
    p->fac = 0;
    p->fb4 = 0;
    p->fbc = 0;
    p->fc4 = 0;
    p->fcc = 0;
    p->fd4 = 0;
    p->f8c = 0;
    p->f90 = 0;
    p->f94 = 0;
    p->f98 = 0;
    p->f9c = 0;
    p->fa0 = 0;
    p->fa4 = 0;
    p->fa8 = 0;
    p->fb0 = 0;
    p->fb8 = 0;
    p->fc0 = 0;
    p->fc8 = 0;
    p->fe8 = 0;
    p->fe4 = 0;
    p->fd0 = 0;
    p->fd8 = 0;
    if (!p->ref) {
        p->ref = new PlayerRef;
    }
    p->ref->Reset(p->team);
    p->flags &= 0xfffe;
    p->ffc = 0;
    p->ffe = 0;
    p->f104 = 0;
    p->f106 = 0;
    p->f102 = p->f100 = -1;
    int w, h;
    h = g_game->height / 2;
    w = g_game->width / 2;
    p->f80 = w;
    p->f84 = h;
    operator delete(p->buffer);
    p->f88 = (h * w + 7) & ~7;
    p->buffer = p->f88 ? operator new(p->f88) : 0;
    // Both references are load-bearing. With `memset(p->buffer, 0, p->f88)`
    // MSVC propagates the phi and the size into the inlined memset, which puts
    // the phi in edi and stores it from edi; the original reloads [esi+0x7c]
    // into edi and keeps the phi in eax. Reading the fields through references
    // stops that propagation, and it also stops the scheduler from hoisting
    // the size load `mov ecx, [esi+0x88]` above the phi store (with the size
    // read directly the load comes first, the original has it second).
    void*& bref = p->buffer;
    int& sz = p->f88;
    memset(bref, 0, sz);
    FUN_00480190(p);
    if (!p->active || p->type != 3) {
        p->unit = new Class_00408cb0(p);
        FUN_0040b320(p->team);
    }
}
