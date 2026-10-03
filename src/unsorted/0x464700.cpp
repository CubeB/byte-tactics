// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, edited by deepseek-v4.1, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by space-bunny-free, finished by DeepSeek V4.1 Flash, verified by GPT-6. Names are provisional.
// Codex GPT-6 retry for #5191 (2026-10-03): current main remains 98.3%.
// Moving the ff8 tick store below the six clears scores 96.6%; assigning
// it from ff4 there scores 87.8%. The original schedule remains best.
// GPT-6 retry: the current source remains 98.3%. Existing notes cover the
// pointer, local, reference and scheduling variants for the remaining block.
// A new probe took the address of an early tick local and stored through that
// pointer after the six clears. This forced the value to memory, but still
// flipped the register pool and scored 66.4%, confirming that spilling the
// local does not avoid its live-range cost.

// space-bunny-free retry 2 (still 98.3%, 473 of 473 bytes, 4 checker runs,
// about 450 in-process compiles). The big new result is that the pool flip is
// a WEIGHT RACE on `w` and that the race can be won, only with bytes:
//   c1 (the two-local body above) plus THREE extra stores of w, e.g.
//   `p->f9c = w; p->fa0 = w; p->fa4 = w;` just before the f88 line,
// gives zero=ebp, w=ebx, the four shorts from bp, `cmp eax, ebp` at 21 and
// `mov [esi+0xf8], TICK` at 22 - the pool and the whole first-block schedule
// right, and the first 14 instructions byte identical. Those three stores
// cost 18 bytes, so 491 against 473, and a greedy resync with jump targets
// wildcarded (build/scratch/464700/align.py) shows what else is left: the
// third tick still in ecx (twice), the team byte still `mov dl` and the
// buffer load still `mov eax, [esi + 0x7c]` where the original wants edx and
// ecx. So the two locals cost four register choices, of which the weight race
// below is only one. Three extra stores of `h`, or three extra
// zero stores, or three stores to the SAME field do NOT move the pool, and in
// the 98.3% body three extra stores of w change nothing while three extra
// zero stores FLIP it: so ebx goes to whichever of {constant 0, w} the
// allocator weights higher, and the two locals in the first block land on the
// same side as three extra zero stores.
// Twenty spellings of "three more references to w that fold away" all reach
// the allocator with nothing to show: `w ? w : w`, `w & w`, `w | w`, `w - w`,
// `w * 0`, `w >> 0`, `w * 1`, `~w & w`, `w / 1`, `w + 0 + 0`, `w = w`,
// `(void)w`, `unsigned z = w - w;`, identical duplicate stores, an identical
// `if/else` in both arms and a nested `?:`. All are deleted before the
// allocator, so no spelling of a folded use can buy the weight.
// A 1800-variant sweep of the two-local body WITH the three extra w stores in
// place (tick and ref type x declaration order x four guard spellings x three
// store spellings x three dummy locals) never once puts the third tick in
// edx: it is ecx or eax in every one, so the tick register is a second,
// independent casualty of the same two locals, not a tie-break that the pool
// fix would also fix. A 960-variant sweep of the same body without the extra
// stores never once puts the constant 0 back in ebp.
// A recorded conclusion, refined: it is not "a value live across a store" that
// flips the pool, it is a NAMED LOCAL. `p->ff8 = g_game->ticks + (p->fac = 0,
// ..., p->fd4 = 0, 0);` (and the same with `* (..., 1)`) keeps zero in ebp, the
// tick in edx, w in ebx and the shorts from bp at 473 bytes, with an
// expression temporary live across the 0x90 store - 96.6%, 13 miss. So the
// blocker is narrower than it looks: MSVC 5 will not hoist that expression's
// load above its own side effects (it emits the six stores first and the loads
// at 19 and 21), and the ONLY construct measured to put a load at the top of
// this block is a local's definition point - value local, pointer local, or a
// reference/pointer to g_game, all of which cost the block a register and flip
// the pool. `int* pt = &g_game->ticks;` does put `mov ecx, [g_game]` at 12 but
// pays a `lea eax, [ecx + 0x38a47]` and flips the pool too. Also: the six
// clears in a plain `static inline void six(Player*)` helper are NOT a
// scheduling barrier - the helper plus the stamp after it is byte for byte the
// flat 96.6% shape, so the 97.5% the older note reports for its sub-object
// method must come from the method form, not from the call.
// Also measured flat, on the two-local body: all 1536 sets of
// tools/headers.py --cpp (66.4% for every set, so the header set is not what
// is holding the pool), and the same sweep on THIS file's 98.3% body gives
// 98.3% for all 1536 sets, so the header set decides nothing here either.
// Transplanting the neighbouring function 0x4644d0
// into the file (it does move the compiler state - the 98.3% body drops to
// 94.1% - but the pool stays flipped), an `int z = 0` named zero, unsigned and
// long w/h, `unsigned w, h`, the multiply spelled (w * h), `delete p->buffer`,
// extra parenthesisation, the four short stores' spelling, and every use of a
// local pointer to the ff8 field.

// space-bunny-free retry (still 98.3%, 473 of 473 bytes, 2 checker runs).
// Harness: build/scratch/464700/{h,lib,lib2}.py score in process on discrete
// binary features (where the cmp/ff8 store land, which register the constant
// 0 and the third tick get, how many instructions miss), not on the ratio.
// Four measurements, all re-verified against /Fa-free disassembly of our own
// object (h.py feats()):
// (1) THE TARGET SCHEDULE IS REACHABLE, AND IT IS EXACTLY TWO LOCALS:
//   PlayerRef* ref = p->ref;   // third statement
//   int t = g_game->ticks;     // fourth statement
//   <the six zero stores>
//   p->ff8 = t;                // where the store goes
//   <the sixteen zero stores>
//   if (!ref) { p->ref = new PlayerRef; }
// comes out instruction for instruction as the original's first block, with
// the three loads at 12/13/14, the six zeros at 15-20, `cmp eax, ZERO` at 21,
// `mov [esi+0xf8], TICK` at 22 and the `jne` at 39. Every one of the 473 bytes
// matches except that ONE register choice is wrong: the constant 0 lands in ebx
// instead of ebp, so all 22 stores, the 4 shorts and the 2 cmps read ebx, w
// takes ebp, the team byte comes in `mov dl` not `mov al`, and the third tick
// takes ecx (reusing the g_game register) instead of edx. 43 of 119
// instructions differ, and all 43 are that one swap.
// (2) THE POOL FLIP IS ABOUT REGISTER PRESSURE, NOT ABOUT HAVING A LOCAL.
// An enregistered local whose live range crosses even ONE store flips it
// (`int t = g_game->ticks; p->fac = 0; p->ff8 = t;` -> zero in ebx, 66.4),
// while the same local with a live range crossing no store is byte for byte
// this file's 98.3% body, and a local that is only an ADDRESS keeps the pool:
// `int* pf = &p->ff8; p->fac = 0; *pf = g_game->ticks;` gives 97.5 (10 of 119
// instructions miss) with zero in ebp, the tick in edx, w in ebx and the shorts
// from bp. An unused local is deleted by the front end and also keeps the pool
// (95.8%). So the flip is "the block needs one more register", and a pointer
// that folds to a constant offset costs none.
// (3) THE ff8 STORE'S SLOT FOLLOWS THE TREE POSITION OF ITS STATEMENT.
// Moving the stamp through the six zero stores one at a time (k = how many
// come first) puts the store at 16, 18, 18, 20, 21, 22, 23. At k = 5, i.e.
// `p->fcc = 0; p->ff8 = g_game->ticks; p->fd4 = 0;`, the store lands at 22 -
// the original's exact slot - with every register right; only the three loads
// stay late (95.0%, 12 miss). So the original's source has its three loads in
// a statement before the six zeros and the store in a statement after them,
// and MSVC will not hoist a load above the stores itself: at k = 6 (b1) the
// loads stay below the six zeros, at k = 0 they are above them but the cmp and
// the store come up with them (this file). Neither end reaches the original.
// (4) A header sweep cannot fix it: all 1536 sets of tools/headers.py --cpp
// on the two-variable body give exactly 66.4%, none moves the pool.
//
// Mechanism, stated as precisely as the evidence allows: MSVC 5 materialises
// the load of a plain non-address-taken local at its DEFINITION point, so the
// only source construct that puts all three loads at the top of the block and
// the store 8 instructions below them is a value live across the six zero
// stores - and any such value costs the block a register, which is what costs
// the constant 0 its ebp. Loads that MSVC schedules itself never land at the
// top of this block (measured in (3)), so the original's three loads are
// either definition-point loads (and the pool then cannot be ebp) or a
// scheduler result no statement order reproduces. The register allocator's
// choice between ebp and ebx for the zero is not steerable by declaration
// order, type, scope, guard spelling, an `int z = 0` named zero, extra dead
// locals, sibling blocks, a folded extra use of w or h, or the header set -
// all measured flat at 66.4%.

// claude-sonnet-5-5 retry (still 98.3%, no full check run beat it; permuter 10
// min, 140 candidates, nothing). Why the pool flips with a tick local, found by
// scoring micro variants: the flip is a weight race, not a property of the
// local. With a tick temp live across the six clears the constant 0 outranks
// the w/h locals (zero ebx, w ebp). It flips back to the original's pool (zero
// ebp, w ebx) once w has about two more reads (test: two extra `p->fXX = w;`
// stores before the f88 line) or when fewer than about 15 zero stores exist.
// Declaring h/w, a size local, in-place `h = (h*w+7)&~7`, explicit `int z = 0`
// in any declaration order, dead initialisers (`int w = 0`) and repeating the
// area expression all leave the flip. A volatile ticks field and a comma/lvalue
// form `(clears, *p).ff8 = g_game->ticks` give 96.6 (loads stay lazy). Even the
// unflipped temp shapes put the tick in eax/ecx and change the tail (`mov dl`),
// so the original has no tick/ref local; the open question is still how the
// third tick load is hoisted above the six clears without one.
//
// mimo-v2.6-pro retry (still 98.3%, best unchanged). New evidence, all 473
// bytes, scratch in build/scratch/0x464700/: (1) v2 with the guard AFTER the
// ff8 statement (A/B shape family) shows top-level statement stores DO sink:
// the 16 clears after the if sink past the operator new call AND past the
// Reset argument loads and `push eax`, landing just before the Reset call.
// The sink target is therefore the next real call, which is past the slot the
// original keeps the ff8 store in, so plain statement-shape sinking cannot
// explain the original. (2) The six clears as one reverse chained assignment
// (`p->fd4 = ... = p->fac = 0`, nested stores) with the ff8 statement at
// position 3 gives 68.9, and all 22 clears chained in one statement 62.2:
// nesting the clears does not delay the ff8 store. (3) `int t =
// g_game->ticks;` at position 3 with `p->ff8 = t;` after the clears and the
// guard at the end (v1 + tick temp only, the shape the earlier notes never
// tried) gives 59.9: the tick lands in eax, the ref load sinks to the guard,
// and the pool still flips. The same with a ref temp (H) 65.5, with unsigned
// t (I) 59.9, with `int w, h;` hoisted above the clears (J) 59.9. (4) The
// chain forms `p->ff8 = g_game->ticks + (p->fac = 0) + ... + (p->fd4 = 0)` and
// the reversed operand order both give 68.9 with the loads still just in time.
// Lead for whoever tries next: the guide's rotation note (0x402da0) says MSVC
// 5 hands scratch registers out in rotation and one temporary more or fewer
// earlier on shifts them; the original's three tick loads are exactly the
// rotation (eax,ecx), (edx,eax), (ecx,edx), so the tick value sits in edx as
// the 6th temporary of the block. The v1 shape already reproduces that
// rotation; only the ff8 store's slot is wrong, so the remaining question is
// unchanged: split the third load from its store without a named local.
// GPT-6.1-sol retry in #3179: 6 checker invocations, best remains 98.3%. Empty guard scored 97.5%; tick-local probe tied at 98.3%. Remaining mismatch is p->ref comparison and p->ff8 store ordering around the first six zero stores.
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


// 30-min checkpoint (deepseek-v4.1-flash, issue 4322, still 98.3%): the hunk is
// unchanged (the cmp/[esi+0xf8] pair sits six stores too high). New evidence
// from this session: the target's tick load/store pair cannot come from a C++
// local, because ANY local or inline-helper parameter that is live across a
// store flips the callee-saved pool (zero ebx, w ebp) even though the schedule
// then comes out right (66.4% family). A local whose live range crosses no
// store (store immediately after the load) keeps ebp, so the flip needs the
// value to be live across at least one store. A `bool` parameter spills the
// value to the stack and keeps ebp but is the wrong value. Comma expressions,
// every binary operator, blocks, if(1)/do-while(0)/switch wrappers, six-clears
// chained or via reference/pointer, and all header sets leave the base bytes
// unchanged. The target schedule alone is reproducible with an inlined helper
// taking the tick by value (load, six clears, store) but the pool flips and the
// ref load/cmp land after the store. Lead for next attempt: find a construct
// that keeps the tick value in a scratch register across the six stores without
// the allocator treating it as a callee-saved candidate.
// deepseek-v4.1-flash retry (issue 3637, still 98.3, 6 checker runs): moving
// the ref guard after the six clears but before the remaining zero stores
// (ff8 store left at its old place) scores 68.9 with the zero register rotated
// to ebx; moving both the guard and the ff8 store there rotates the pool too.
// `if (p->ref == 0)` is byte-identical. The cmp/[esi+0xf8] pair placement is
// the only remaining hunk.
//
// space-bunny-free retry (still 98.3%, 473 of 473 bytes, about 50 checker
// runs). Harness try_.py and batches b1 to b21 in build/scratch/464700/.
// (1) The original's first block is reachable instruction for instruction with
// TWO variables where this file has none: `PlayerRef* ref = p->ref;` then
// `int t = g_game->ticks;` as the third statement and `p->ff8 = t;` after the
// six clears (x3, w2) give the original's order and its scratch registers for
// all three loads (g_game in ecx, ref in eax, ticks in edx) with the `cmp` and
// the +0xf8 store in the original's slots. The order of the two definitions
// picks the tick's register: ref first puts the ref in eax (right) and the
// tick in ecx, reusing the g_game register; tick first puts the tick in eax and
// the ref in ecx. (2) What still differs in that family is only the
// callee-saved pool: 0 in ebx instead of ebp, so the 22 stores, the four
// shorts and the `cmp` all come from ebx and width/2 takes ebp. Every local
// live across the six zero stores flips it, whatever the local is: function
// scope `int t;`, block scope `int t = ...`, a nested `{ }` around the six
// clears, `register`, `const`, `unsigned`, `int w, h;` hoisted to the top of
// the function, either declaration order, the six clears chained, and an
// inlined helper taking the tick by value or by reference (65.5 to 70.6). A
// local whose live range crosses no store keeps the pool: function scope
// `int t;` with `t = g_game->ticks; p->ff8 = t;` at statement 3 is byte for
// byte this file's 98.3 percent body. (3) The flip is not an alias class: the
// 22 cleared fields declared `float`, `void*`, `long` or `short` change
// nothing, alone (96.6 flat) or with the two variables (66.4).
// (4) NEW SHAPE AND THE BEST LEAD: put the six clears in an inline method of a
// member sub-object, a struct at +0xac holding fac, fb4, fbc, fc4, fcc and fd4
// with the five other ints of that range as padding, and
// `void Clear() { fac = 0; ... fd4 = 0; }`, then write `p->ff8 =
// g_game->ticks;` after the call (t1): 97.5 at 473 bytes, with the pool, all
// three scratch registers, the `cmp` and the +0xf8 store in the original's
// places, and only the three loads left to explain, sitting between the six
// clears and the `cmp` instead of above the clears. The same 97.5 when the
// +0xf8 store also goes through a method of a second sub-object type (v1b).
// The inlined method is a scheduling barrier, so this shape cannot hoist the
// loads either, but it is the closest anyone has come to the original's
// schedule with no variable in the block, and it shows the pool does not need
// a variable to stay right. The same 97.5 comes from writing the six clears
// through a local `Stamps_00464700* s = &p->stamps;` (z1, and with a reference
// y1/y2), so it is a *pointer* of a sub-object type that carries the six
// stores, not the type alone: writing them flat as `p->stamps.fac = 0;` with
// no local and no method is back to 96.6 (f1). The reverse arrangement, the
// +0xf8 store in a method of a sub-object at
// +0xf8 with the six clears left flat, is 96.6 (d1), and giving the +0xf8 field
// its own sub-object type changes nothing (c1, v3b, 97.5 and 98.3).
// (5) The guard cannot explain the schedule: putting
// `if (!p->ref) { p->ref = new PlayerRef; }` before the six clears makes MSVC
// emit the allocation there (67.2), and with a tick variable too, 65.5.
// Two cautions for the next run: a member function named `Set` makes CL.EXE
// fail with an empty log, so give such a method another name; and a permuter
// run on this file (8318 candidates, nine minutes, --jobs 2) found nothing.
// The remaining hunk is unchanged: `cmp eax, ebp` and `mov [esi + 0xf8], edx`
// six instructions below where this file puts them.
// DeepSeek V4.1 Flash retry (still 98.3%, 473 of 473 bytes; permuter 3 min,
// 1822 candidates, no gain). Two new negative results, both reproducing the
// schedule exactly with the callee-saved pool flipped (zero ebx, tick not
// edx): (a) the six clears AND the +0xf8 store in one inlined sub-object
// method taking the tick by value,
//   ((Stamps*)((char*)p + 0xac))->ClearStamp(g_game->ticks);
// gives load, six stores, ff8 store, ref load, cmp -> 66.4; (b) a by-value
// struct snapshot helper (`struct Snap { PlayerRef* ref; int t; }; Snap snap =
// Snap(p);` then `p->ff8 = snap.t;` and `if (!snap.ref)`) gives the same 66.4
// shape. So neither an inlined parameter nor a returned aggregate avoids the
// live-range-across-the-clears penalty: any value read before the clears and
// used after them makes the allocator hand the constant 0 ebx. The split load
// and store cannot be produced without such a value, so the pool tie stands.
// Scratch: build/scratch/0x464700/ (gen.py harness, t1-t8.py).
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
