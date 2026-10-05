// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, refined by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Space Bunny Free. Names are provisional.
// Space Bunny Free 2026-10-02 (final pass): 99.5 -> MATCH, and what it took, in
// order of how much each one was worth:
// (A) The owner reload, which every earlier pass chased with aliasing tricks
// and the wrong answer turned out to be one line: the scale blocks write their
// slot through a LOCAL POINTER (`float* slot = &unit->field_bc; ... *slot =
// f;`). MSVC 5 then cannot prove that store disjoint from `unit->field_ec`,
// so it re-reads the owner in the second block (`mov eax,[esi+0xec]`) exactly
// as the original does, and because the pointer folds straight back to a
// constant offset the slot accesses still emit `fld [esi+0xbc]` / `fstp
// [esi+0xbc]`. Both blocks use the pointer, which keeps the two blocks
// structurally identical. This replaced the ScaleW union the earlier passes
// needed: with the union MSVC 5 also reloaded, but every union member sits at
// offset 0, so both owner loads came out at +0xbc instead of +0xec (99.5%).
// What does NOT defeat the forwarding: two separately spelled inlined helpers,
// one per slot, taking the unit (the second copy reuses the first's load), a
// nested struct or arrays inside the union, and a fresh pointer or char* alias
// for the owner read.
// (B) The last hunk, at 0x4650d0. The original hoists `mov eax,[g_game]` ABOVE
// the `jne 0x465881`, so both successors share the reload and the jump lands
// past the one inside `countdown_extra`; ours put the load after the branch, so
// the jump target was 0x46587c, five bytes short. Every if/else vs early-goto
// respelling at that `if` was byte-identical, and the fix is one step further
// out: the else of `if (g_game->list->FUN_00490230() == 0)` carries its OWN copy
// of the countdown block instead of `goto countdown_extra`. MSVC 5 then
// tail-merges the two copies itself and keeps the load in the branch.
// (C) The string at +0x87f was truncated: the original's is "You are placed in
// watch mode because you are hosting AI players which are still alive.  If you
// exit, they will be terminated." (125 characters), not "You are placed in
// watch mode".
// (D) The scale constants are named (see the declaration below). This is only
// about the width of each .rdata object, not about the code: MSVC 5 gives a
// float LITERAL an 8-byte slot, and with 100.0f spelled as a literal it is the
// FIRST .rdata object, so the checker compared its 4 padding bytes against the
// original's next constant (another function's 12700.0f) and called the
// function 100.0 percent with a bad reference. Naming the constant makes it a
// 4-byte object, and the declaration order puts it last so nothing follows it.
// Also settled earlier today: the byte count is exact (2392 = 2392) and the
// duplicated player guard of 0x464fe1..0x465024 is solved, so every note below
// about "38 bytes missing before 0x4655a6" is stale.
// Space Bunny Free 2026-10-02: 85.1 -> 99.5 percent, ours now 2392 bytes, the
// original's size. THREE LEVERS, all of them load-bearing:
// (1) The owner reload. SOLVED LATER, see (A) above: a plain reading of
// `unit->owner` in both blocks lets MSVC keep the pointer in EAX across the
// first block's switch and forward it to the second (`mov edx,[eax] / test
// edx,edx`, five bytes short, and the first switch's discriminant lands in ECX
// instead of EAX). The union below was the price of forcing the reload and is
// gone again.
// (2) The loop. The original keeps BOTH the entry guard (`cmp bl,0xa / mov
// [esp+0x10],bl / jae 0x4655a6`) and a latch test (`inc bl / cmp bl,0xa / mov
// [esp+0x10],bl / jb body`), and the guard's failure branches to the LATCH, not
// to the exit. A `for` with the two-return `loopCond` helper keeps the guard
// but MSVC threads the latch test away (`inc bl / mov / jmp head`); a
// do-while keeps the latch test but MSVC folds the guard away. The shape that
// gives both is a `for (;;)` whose first statement is
// `if (!loopCond(bl)) goto next_bl;` and whose last statement is
// `if (!more(++bl)) break;`, with every `continue` turned into `goto next_bl`
// and `more` a SECOND, separately spelled inlined helper
// (`if (i < 0xa) return 1; return 0;`): two different expressions at the two
// test sites, so neither can be folded into the other, and the guard's `goto
// next_bl` is what makes the head branch to the latch. `bl` has to be declared
// before the guard and `pi` has to be declared before the first `goto`, or
// MSVC rejects the jump.
// (3) The subscreen setup. Declaring the step values in the order
// `hh`, `hits`, `zacc`, `outer`, `hw` (instead of `hw`, `hh`, ...) puts MSVC's
// `shl edi,0x10` after `mov ebx,eax` where the original has it.
// Still open (99.5%, 2392 bytes, byte count exact):
// tools/permute.py was run twice on this file (15 min each, 548 candidates) and
// never beat 85.1% on the pre-union source, so the union and the `for (;;)`
// loop shape are hand findings, not permuter ones.
// (a) The two owner loads read +0xbc where the original reads +0xec, the price
// of lever (1). The offsets CAN be right: giving the owner a second one-slot
// union of its own at +0xec (`unit->ow.owner`, build/scratch/0x464f80/v40.cpp)
// emits `mov eax,[esi+0xec]` in both blocks exactly as the original does, but
// then MSVC has two disjoint union objects, forwards the pointer again and the
// score drops back to 85.7. Every other spelling tried either keeps the reload
// with the wrong offset or keeps the offset and loses the reload: a nested
// struct inside the union (v24.cpp, v36.cpp) and an array of the union
// (v23.cpp) give the right offsets and no reload; arrays INSIDE the union
// (v39.cpp, `unit->w.slot[6]` / `unit->field_ec[12]`) also give the right
// offsets and no reload; casts through `(char*)unit + 0xec` fold back to the
// same expression and are byte identical, as are fresh locals for the unit
// pointer (`Unit* u2 = unit;`), a helper returning the owner, a
// differently-typed view of the unit, and reading the owner through a pointer
// parameter. So on this compiler either the reload or the offset, never both:
// the next worker should look for what makes MSVC's local-value table drop
// `unit->owner` between the two blocks (pressure or a tracking limit), not for
// another aliasing trick.
// (b) At the first `Class_0048ff40::FUN_00490230` call the original hoists
// `mov eax,[g_game]` between `test eax,eax` and `jne`, so both successors share
// it and its `jne` lands past the reload inside `countdown_extra`; ours puts the
// load after the `jne` and `countdown_extra` reloads it. Same instruction
// multiset, pure scheduling. Tried: if/else instead of an early `goto`, an
// early `goto` instead of if/else, a named int for the call result.
// mimo-v2.6-pro 2026-10-01 (session 2, timeboxed): 80.8 -> 85.1 percent,
// 2370 -> 2386 bytes. BREAKTHROUGH: the duplicate player guard no longer CSEs.
// The trick is to spell the WHOLE second guard group through a fresh pointer
// `PlayerInfo_00464f80* pi2 = &g_game->players[bl];` in its own scope. A fresh
// `&g_game->players[bl]` gets a new value number, so MSVC cannot forward the
// first group's `pi->type` / `pi->field_146` loads and re-emits
// `mov al,[edi+0x73]` and `cmp byte [edi+0x146],0xa` (the two reloads that were
// missing). Crucially this spelling ALSO routes the second reads through EDI
// (`[edi+0x73]`, `[edi+0x146]`) exactly like the original, and does NOT swap
// esi/edi (unlike the pi2-for-type-only spelling of earlier passes which sent
// the second field read through `[ecx+0x1ca9]`). The first active test stays
// the array form `g_game->players[bl].active` (gives `mov eax,[edx+ecx*2+
// 0x1b63] / test / lea edi`), and the second active test is `pi2->active`
// (`cmp dword [edi],0`). See build/scratch/0x464f80/r2.cpp (this file) vs
// base.cpp / r1.cpp. r1 (pi2 only for type) sent field_146_2 through ecx;
// r2 (pi2 for the whole second group) is the good one.
// STILL OPEN (all compiler-state register allocation, 2386 vs 2392 = 6 bytes):
// (1) The first owner block's switch discriminant is in ECX here
// (`mov ecx,[edx+0x37eee] / sub ecx,0 / dec ecx`) but EAX in the original
// (`mov eax,[...] / sub eax,0 / dec eax`). Because block1 keeps `unit->field_ec`
// in EAX and the switch reuses ECX here, EAX (owner) survives into block2, so
// block2 CACHES owner->active (`mov edx,[eax] / test edx,edx`) instead of
// RELOADING the owner pointer (`mov eax,[esi+0xec] / cmp [eax],0`) like the
// original. So the switch register is the single root cause of BOTH the missing
// 6-byte `mov eax,[esi+0xec]` AND block2's active test form. Micro-variants
// (`int sv = g_game->field_37eee; switch(sv)`, `+0`, named active/own locals)
// all vanish to identical 85.1 output; the eax-vs-ecx choice is a register
// allocator coin flip I could not steer. Forcing block2 to reload owner without
// fixing block1's switch needs an invalidating store between the blocks (there
// is none: `unit->field_bc=f` is a different field of the same struct).
// (2) Loop head still spills before the test (`mov [esp+0x10],bl / cmp bl,0xa`)
// where the original tests first (`cmp bl,0xa / mov [esp+0x10],bl`), and the
// original keeps BOTH a head test and a bottom test (shared failure exit: head
// `cmp/mov/jae INC`, tail `inc bl/cmp bl,0xa/mov/jb body`) while ours has one
// head test and a tail `inc/mov/jmp head`. Loop-shape variants scored lower.
// (3) `shl edi,0x10` scheduled after `sub eax,ebp` here, before it in original.
// (4) The watch_check / dialog region: g_game+0x519 reloads pick edx/ecx/eax
// differently and `mov ebp,[ebx+4]` (w = dlg->field_4) scheduling differs.
// (5) watch_check player-index lea/mov order (the FUN_00456850 result math).
// More switch/owner attempts this session, all byte-identical to r2 (85.1,
// 2386): inline getSw() helper returning field_37eee (block1-only and
// both-blocks), nested if instead of &&, own/own2 fresh locals for owner,
// block2 owner via *(Player**)((char*)unit+0xec) and *(int*)((char*)unit->field_ec),
// pre-computed int sw before the if (83.7), switch -> if/else chain (84.2),
// int sv = field_37eee; switch(sv) both blocks (83.9). None flip block1's
// switch to EAX. A do-while loop shape (if (loopCond) { do {...} while
// (loopCond(bl)) }) collapses to 66.9 because continue skips the bl++ (C
// continue in do-while jumps to the condition, not the increment), so the
// for-shape is required for continue -> increment; keeping BOTH the head and
// tail tests needs the guide-1101 inline-member-helper trick which the earlier
// passes could not get to keep the entry test. Next worker: the single highest
// value lever is still block1's switch discriminant register (ECX -> EAX);
// everything in the owner region and the 6-byte deficit cascades from it.
// mimo-v2.6-pro 2026-10-01 (retry, timeboxed): 80.2 -> 80.8 percent,
// 2386 -> 2370 bytes. SOLVED: the two byte `or`-RMW sites (the dl load-modify-
// store on `flags_3923b |= 0x10` and `pi->data->flags_9b |= 0x40`). The direct
// `or byte ptr [m], K` form is a 1-bit `unsigned short` bitfield set (see the
// guide's "or byte ptr [m], K straight to memory" fact and 0x4917d0): the
// flags word at +0x3923b is now `union { unsigned short w; struct { :2, bit2,
// bit3, bit4, bit5, bit6, :9 } b; }` with `w |= 4` kept for the `or word [m],
// bp` sites (4 lives in ebp and its low byte is unaddressable, which is why
// that OR is word-sized) and `b.bit4/bit5/bit6 = 1` for the byte ORs; the unit
// byte at +0x9b is `union { unsigned char flags_9b; struct { :6, bit6b,
// bit7b, :8 } fb; }` with `fb.bit6b = 1` for the write and the plain
// `flags_9b & 0x40/0x80` reads kept (a bitfield read `if (bf)` compiles to
// shr/test, not `test byte`). Plain `unsigned char |=` goes through a register
// whenever a second OR to the same location follows anywhere later in the
// function, and bitfield sets on `unsigned char` storage do too; only the
// `unsigned short` bitfield spelling is direct every time (micro-tests
// build/scratch/0x464f80/t_or*.cpp).
// Loop shape, tried and no better than the current helper-for (80.8):
// if (loopCond) do {...} while (bl++, loopCond(bl)) 80.5 (guard folds),
// plain for (bl < 10) 80.7 (head test dropped, rotated tail only),
// if (loopCond) do {...} while (bl++, bl < 10) 80.7, if (bl < 10) do {...}
// while (bl++, loopCond(bl)) 80.5 (t_or4/t_loop*.cpp micro-tests show a
// two-return helper gives test-at-top + jmp back, a single-return helper or a
// plain bound trips the countdown pass in isolation). The original's head
// `cmp/mov/jae INC` (failure merged with the increment block, the "shared
// failure exit") plus bottom `inc/cmp/mov/jb body` is the rotated loop with a
// kept redundant entry test; no spelling found that keeps both tests without
// folding one away or tripping the trip-count pass.
// Duplicate player guard (0x464fe1..0x465024) still CSE'd: the 0x458810
// recipe (spell the second copy through `g_game->players[bl].` instead of
// `pi->`, build/scratch/0x464f80/g2.cpp) makes the frontend keep the tests but
// the second load then goes through `[eax + 0x1bd6]` instead of
// `[edi + 0x73]` and the whole register file rotates (al -> cl, lea split),
// 78.5%. pi2 (fresh `&g_game->players[bl]`) reproduces the group byte-for-byte
// but swaps esi/edi globally (previous passes, 75.6-79.5).
// Still open beyond those: the `shl edi, 0x10` scheduling in the subscreen
// setup, the `mov eax,[g_game]` hoisted before the FUN_00490230 jne, the
// switch value in eax vs ecx (first field_37eee block) and edx vs ecx (second
// g_game reload), the second owner block re-loading `unit->field_ec` from
// `[esi + 0xec]` instead of caching it, and the watch_check player-index
// computation's lea/mov order.
// deepseek-v4.1-flash 2026-10-01 (retry 6, timeboxed): no gain, stays 80.2 /
// 2386 bytes. This session's probes were flat or negative: swapping the
// countdown_extra 0x10/0x20 byte ors 80.0, `*(unsigned char*)&flags_3923b
// |= 0x10` byte-identical at 80.2. The dl load-modify-store on the 0x3923b
// byte write and the CSE'd duplicate player guard remain open as before.

// deepseek-v4.1-flash 2026-10-01 (retry 5, timeboxed): 79.7 -> 80.2 percent,
// 2385 -> 2386 bytes. Declaring the FUN_00488b10 result as
// `unsigned int typeId = FUN_00488b10(...) & 0xffff;` instead of
// `unsigned short typeId = FUN_00488b10(...);` deletes the raw-eax spill to
// [esp+0x30] plus the mov ecx,eax / mov eax,ecx pair and clears that hunk.
// Still open: the shl edi,0x10 (screen_hh) schedule in the subscreen setup
// (original shifts hh after `mov ebx,eax`, ours before `sub eax,ebp`), and the
// branch-displacement hunks that follow from the remaining size gap.

// Decompiled by deepseek-v4.1-flash, finished by GPT-6, edited by deepseek-v4.1, refined by GPT-6.1-sol. Names are provisional.
// deepseek-v4.1-flash 2026-10-01 (retry 4, timeboxed): no new gains, stays at
// the 79.7% / 2385-byte best. Open sites unchanged: the duplicate player guard
// still CSEs, the typeId copy is `mov ecx,eax` (original `mov cx,ax`), and the
// three byte `or`s to flags_3923b/0x9b still go through dl instead of a direct
// `or byte ptr [mem],imm`; the dl spelling is used for plain `|=` writes in
// both the union member and the plain unsigned char member, so it is not
// union-specific.

// deepseek-v4.1-flash retry 2026-10-01: re-spelling the second type/field_146 reads as *(unsigned char*)((char*)pi + 0x73/0x146) does not defeat the CSE (79.7%, 2385 bytes, same 14 hunks), so the original reload at 0x46500a needs a source shape that recomputes the player pointer, not a different lvalue spelling.
// PARTIAL: 79.6% (was 72.2%). The frame is now the original 0x34 and the slot
// order matches (byte idx 0x10, player 0x14, hits 0x18, cell 0x1c, inner 0x20,
// outer 0x24, 9999 0x28, typeOff 0x2c, typeId 0x30, self 0x34, pos 0x38/0x3c/
// 0x40); the old extra slot came from the screen_hw step being spilled, so the
// step values are now shifted (hw<<16, hh<<16) before the loops and live in
// edi/ebp. Ours is still 21 bytes shorter and every branch target is shifted.
// Pass 2 (deepseek-v4.1): the missing loop head guard is now emitted: the
// counter must be tested through a single-use inlined helper
// (`static int loopCond(unsigned char i){ if (i >= 0xa) return 0; return 1; }`
// as the for condition), exactly as the 0x48ad30 fact on SHARED.md says; that
// blocks the trip-count pass. 79.6 -> 79.7, ours 2376 bytes. The helper's
// guard store is still scheduled before the cmp instead of after it.
// Still open: the duplicated player guard (0x464fe1..0x465024) is still CSE'd
// into one copy, so 38 bytes before 0x4655a6 are missing and every later
// branch target stays shifted; the typeId copy is `mov ecx,eax` where the
// original uses `mov cx,ax`; and two byte flags writes go through dl.
// Pass 3 (deepseek-v4.1, 4 check runs on scratch variants): the duplicate
// guard IS reproducible: give the second guard group its own player pointer
// (`PlayerInfo_00464f80* pi2 = &g_game->players[bl];` used for the active /
// type / field_146 tests). Module-wide CSE then cannot fold the loads, so
// `cmp dword ptr [edi],0` and `mov al, byte ptr [edi+0x73]` come back
// (build/scratch/0x464f80/v2.cpp and v4.cpp). The cost: MSVC then swaps the
// edi/esi roles (index in edi, player in esi) and emits one extra type cmp
// chain, at 2417 bytes / 79.5%, still under this file's 79.7. The plain
// respellings (g_game->players[bl] inline, casts, signed char temp) all stay
// CSE'd at 2376 bytes / 79.7.
// Pass 4 (deepseek-v4.1, 4 more check runs on variants): the duplicate guard
// group is the whole 31-byte front deficit (original group2 spans 0x465001 to
// 0x465029; ours has one shared type chain). Three respellings tried and all
// CSE'd to byte-identical 2376-byte output: (a) reading the second group
// through `char* pb = (char*)pi` with raw int/byte accesses, (b) a single-use
// `static int guard2(PlayerInfo*)` helper returning 1/0, called as
// `if (!guard2(pi)) goto next_bl;`, (c) same as (a) but re-taking
// `pi = &g_game->players[bl]` before the second group: this one does emit 2401
// bytes but drops to 75.6%, so it stays out. The loop shape is the same story:
// the original's head guard and every `continue` share one address (0x4655a6,
// the rotated increment block `inc bl / cmp bl,0xa / mov [esp+0x10],bl /
// jb 0x464fab`), while ours has the continue target 0xb bytes before the
// head-exit target. Still open: that merge, the `mov cx,ax` vs `mov ecx,eax`
// typeId copy (ours also spills the raw call result to [esp+0x30], +4 bytes),
// and the `or byte ptr [eax+0x3923b],0x10` that ours writes through dl.
// Pass 5 (deepseek-v4.1): that dl write is NOT caused by the `(unsigned char*)`
// cast: declaring flags_3923b as `union { unsigned short w; unsigned char b; }`
// and using `.b` for the byte wise writes is byte-identical (2376 / 79.7%),
// so the load-modify-store there is a scheduler choice, not a type-alias one.
// Pass 6 (deepseek-v4.1, 9 check runs on scratch variants): the *front* of
// the original's second guard IS reproducible. Writing the FIRST active test
// as `g_game->players[bl].active` (not through `pi`, which is declared right
// after it) makes MSVC emit the original load-then-lea sequence and keeps the
// second `cmp dword ptr [edi],0 / je` (it cannot prove the two expressions
// equal), so ours is now 2385 bytes with that 4-byte pair matching. MSVC
// still forwards the byte `al` from the first type test and deletes the
// second type and field_146 chains, so 18 of the 22 remaining bytes are still
// missing there. Forcing `al` interlopers (an int copy of active, a char*
// alias, a union member at +0x73, fresh `pi2 = &g_game->players[bl]`) either
// stays byte-identical or rotates edi/esi, so the reload is a register
// allocation choice, not a source alias one. The loop is also non-rotated in
// the original (head `cmp bl,0xa / mov [esp+0x10],bl / jae 0x4655a6` plus a
// tail `inc bl / cmp / mov / jb 0x464fab`): our for-loop emits only a rotated
// tail test that jumps to the store (`jb 0x464fa2`), and rewriting it as an
// `if (loopCond(bl)) do { ... } while (loopCond(++bl));` folds the entry test
// to true and drops it. Do not chase either further without a compiler-state
// lever.
// Pass 7 (deepseek-v4.1, 4 check runs): the pi2 shape is the ONLY guard lever found.
// Variant v4b (`PlayerInfo_00464f80* pi2 = &g_game->players[bl];` for the whole second
// guard group) reproduces the duplicated guard byte-for-byte and lands at 2401 bytes,
// but it swaps the loop pointers globally: index->edi, player->esi (75.6%). Passing the
// index through `unsigned char b2 = bl; &g_game->players[b2]` changes nothing (same
// 2401 / esi-edi swap), and byte-typed reads (`char t2 = ((char*)pi)[0x73];`,
// `char t2 = *(char*)&pi->type;`) still CSE against the first group's load, so MSVC
// keys that CSE on the POINTER VALUE, not the load's type or address.
// The original is thus likely pi-plus-fresh-pointer in the Cavedog source, with a
// register assignment we cannot steer from these respellings.
// Pass 8 (GPT-6.1-sol): changing the 0x488b10 declaration among unsigned short,
// int, and short, introducing owner-pointer locals, and moving the loop bound to
// an explicit top-of-loop break all retained 79.7%. The best remains 2385 bytes.
// Previous note: Still differs: the loop head test (cmp bl,0xa / jae taken to
// the increment)
// is dropped as provably true even as a while loop, the duplicated player
// guards (0x464fe1..0x465024) are CSE'd into one copy, the typeId copy is
// `mov ecx,eax` where the original uses `mov cx,ax`, and two byte flags writes
// go through dl (mov dl,[eax+0x39x]; or dl,imm; mov [eax+0x39x],dl) where the
// original uses a direct `or byte ptr [eax+0x39x], imm`.
#include <windows.h>
#include <string.h>

struct Unit;
struct Player_00464f80;

struct Class_0040eb70 { void FUN_0040eb70(); };
struct Class_00408c40 { void FUN_00408c40(); };
struct Class_00435100 {
    char unknown_0[0xd44];
    int field_d44;                     // +0xd44
    int FUN_00435100();
};
struct Class_0048ff40 { int FUN_00490230(); };
struct Class_00490360 { int FUN_00490360(); };
class Class_0048b090 { public: void FUN_0048b090(int which, int on); };

#pragma pack(push, 1)

// The player-controlled object (g_game+0x1b8a+0x14b*n), stored in
// PlayerInfo.data at +0x27.
struct Player_00464f80 {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char control;             // +0x73
    char unknown_74[0x95 - 0x74];
    unsigned char field_95;            // +0x95
    char unknown_96[0x9b - 0x96];
    union {
        unsigned char flags_9b;        // +0x9b
        struct {
            unsigned short padb : 6;
            unsigned short bit6b : 1;
            unsigned short bit7b : 1;
            unsigned short restb : 8;
        } fb;
    };
    char unknown_9d[0xa1 - 0x9d];
    unsigned short field_a1;           // +0xa1
    unsigned short field_a3;           // +0xa3
    char unknown_a5[0xbc - 0xa5];
    float field_bc;                    // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_00464f80* field_ec;         // +0xec
};

struct UnitType_00464f80 {
    char unknown_0[0x15a];
    int limit;                         // +0x15a
    char unknown_15e[0x210 - 0x15e];
    short field_210;                   // +0x210
    char unknown_212[0x22f - 0x212];
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x241 - 0x230];
    unsigned int flags;                // +0x241
    char unknown_245[0x249 - 0x245];
};

// A unit. Only the fields this function reads are named.
struct Unit {
    char unknown_0[0x92];
    UnitType_00464f80* type;           // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0xbc - 0xa8];
    float field_bc;                    // +0xbc
    char unknown_c0[0xd4 - 0xc0];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_00464f80* field_ec;         // +0xec
    char unknown_f0[0x110 - 0xf0];
    unsigned int flags_110;            // +0x110
    char unknown_114[0x118 - 0x114];
};

struct PlayerInfo_00464f80 {           // +0x1b63, stride 0x14b
    int active;                        // +0x0
    char unknown_4[0x22 - 0x4];
    char field_22;                     // +0x22
    char unknown_23[0x27 - 0x23];
    Player_00464f80* data;             // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* units;                       // +0x67
    Unit* units_end;                   // +0x6b
    char unknown_6f[0x73 - 0x6f];
    unsigned char type;                // +0x73
    Class_00408c40* field_74;          // +0x74
    char unknown_78[0xf0 - 0x78];
    int field_f0;                      // +0xf0
    char unknown_f4[0x140 - 0xf4];
    int field_140;                     // +0x140
    unsigned short field_144;          // +0x144
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Pos_00464f80 { int x, y, z; };

struct Point16 { short x, y; };

struct UnitDef_00464f80 { char unknown_0[0x249]; };

struct Struct_00496e90 {
    char unknown_0[0xdc];
    float width;                       // +0xdc
    float height;                      // +0xe0
    char unknown_e4[0x149 - 0xe4];
    unsigned short flag_149 : 1;       // +0x149
};

struct Widget_00464f80 {
    char unknown_0[0xcc];
    char field_cc[0x10];               // +0xcc
    char field_dc[0x20];               // +0xdc
};

struct Dialog_00464f80 {
    char unknown_0[4];
    Widget_00464f80* field_4;          // +0x4
    void* field_8;                     // +0x8
};

struct Game {
    char unknown_0[0x519];
    char gui[0x1b63 - 0x519];
    PlayerInfo_00464f80 players[10];   // +0x1b63
    char unknown_2851[0x2a42 - 0x1b63 - 10 * 0x14b];
    unsigned char localPlayer;         // +0x2a42
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x14207 - 0x2a44];
    Class_0040eb70* field_14207;       // +0x14207
    char unknown_1420b[0x14223 - 0x1420b];
    int screen_x;                      // +0x14223
    int screen_y;                      // +0x14227
    char unknown_1422b[0x14233 - 0x1422b];
    int screen_hw;                     // +0x14233
    int screen_hh;                     // +0x14237
    char unknown_1423b[0x1427f - 0x1423b];
    unsigned char field_1427f;         // +0x1427f
    char unknown_14280[0x14281 - 0x14280];
    unsigned short field_14281;        // +0x14281
    char unknown_14283[0x1439b - 0x14283];
    UnitType_00464f80* types;          // +0x1439b
    char unknown_1439f[0x37eee - 0x1439f];
    int field_37eee;                   // +0x37eee
    char unknown_37ef2[0x37ef6 - 0x37ef2];
    int field_37ef6;                   // +0x37ef6
    char unknown_37efa[0x37f5f - 0x37efa];
    char startPos[0x38a47 - 0x37f5f];  // +0x37f5f, 0x232-byte records
    unsigned int tick;                 // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Class_00435100* mode;              // +0x391e9
    Class_0048ff40* list;              // +0x391ed
    char unknown_391f1[0x39239 - 0x391f1];
    short field_39239;                 // +0x39239
    union {
        unsigned short w;
        struct {
            unsigned short padb2 : 2;
            unsigned short bit2 : 1;
            unsigned short bit3 : 1;
            unsigned short bit4 : 1;
            unsigned short bit5 : 1;
            unsigned short bit6 : 1;
            unsigned short rest2 : 9;
        } b;
    } flags_3923b;                     // +0x3923b
};

#pragma pack(pop)

extern Game* g_game;
extern int DAT_0051e53c;

void __stdcall FUN_0040b2c0(int player);
void __stdcall FUN_004827b0(Unit* unit);
void FUN_00466dc0();
void FUN_00467440();
void FUN_00466c20();
unsigned char __stdcall FUN_00456850();
unsigned short __stdcall FUN_00488b10(const char* name);
int __stdcall FUN_004b6c30(int range);
int __stdcall FUN_0047db70(UnitDef_00464f80* type, int a, Point16 cell, int c);
short __stdcall FUN_00421da0(Pos_00464f80* pos, int a, int b);
int __stdcall FUN_00485140(Pos_00464f80* pos);
Unit* __stdcall FUN_00485f50(unsigned char player, unsigned short typeId,
                                     Pos_00464f80 pos, int a, int b, int c);
void __stdcall FUN_00496e90(Struct_00496e90* obj, int height, int width);
void __stdcall FUN_004816a0(int on);
void __stdcall FUN_0048d630(int on);
void __stdcall FUN_00401360(PlayerInfo_00464f80* player);
void __stdcall FUN_004573d0(PlayerInfo_00464f80* player, int a, int b);
int __stdcall FUN_00457cb0();
int __stdcall FUN_00457bc0();
void __stdcall FUN_00450f90();
void* __stdcall FUN_004aa8f0(char* gui, const char* file, int flags);
void __stdcall FUN_0049fb10(char* gui, int a);
void __stdcall FUN_004a0bf0(char* gui, const char* gadget, const char* text, int a);
void __stdcall FUN_004a81e0(char* gui, int a);
const char* __stdcall FUN_004c5740(const char* text);
void __stdcall FUN_004abd90(char* gui, const char* text, int a, int b, int c);
void __stdcall FUN_00464de0(void* gadget);

static int loopCond_00464f80(unsigned char i)
{
    if (i >= 0xa)
        return 0;
    return 1;
}

// The loop's latch test. It has to be a second, separately spelled inlined
// helper: with the same expression at both test sites MSVC folds one of them
// away, and the original keeps both.
static int more_00464f80(unsigned char i)
{
    if (i < 0xa)
        return 1;
    return 0;
}

// The three scale constants, named so that each lands in .rdata as its own
// object of exactly the original's width, and in this order: MSVC 5 emits a
// float LITERAL in an 8-byte slot but a named static const float in 4, and
// literals come after statics, so with 100.0f spelled as a literal it is the
// first .rdata object and the checker reads its slot's 4 padding bytes as
// part of it (the original's next constant is another function's 12700.0f).
static const double kNegSeven = -0.7;
static const double kNegHalf = -0.5;
static const float kHundred = 100.0f;

// FUNCTION: 0x464f80
void __stdcall FUN_00464f80()
{
    g_game->field_14207->FUN_0040eb70();
    unsigned char bl = 0;
    for (;;) {
        if (!loopCond_00464f80(bl))
            goto next_bl;
        PlayerInfo_00464f80* pi;
        if (g_game->players[bl].active == 0)
            goto next_bl;
        pi = &g_game->players[bl];

        {
            unsigned char t = pi->type;
            if (t != 1 && t != 2 && t != 3)
                goto next_bl;
        }
        if (pi->field_146 == 0xa)
            goto next_bl;
        {
            PlayerInfo_00464f80* pi2 = &g_game->players[bl];
            if (pi2->active == 0)
                goto next_bl;
            unsigned char t2 = pi2->type;
            if (t2 != 1 && t2 != 2 && t2 != 3)
                goto next_bl;
            if (pi2->field_146 == 0xa)
                goto next_bl;
        }

        if (pi->field_74 != 0)
            pi->field_74->FUN_00408c40();

        FUN_0040b2c0(bl);

        {
            Unit* u = pi->units;
            while (u <= pi->units_end) {
                if (u->flags_110 & 0x10000000)
                    FUN_004827b0(u);
                u = (Unit*)((char*)u + 0x118);
            }
        }

        if (bl == g_game->field_2a43)
            FUN_00466dc0();

        if ((unsigned int)pi->field_f0 > g_game->tick)
            goto next_bl;
        pi->field_f0 += 0x1e;

        if (bl == g_game->localPlayer) {
            if (g_game->mode->FUN_00435100() == 1) {
                if (g_game->list->FUN_00490230() == 0) {
                    if (((Class_00490360*)g_game->list)->FUN_00490360() != 0) {
                        if (g_game->field_39239 < 0) {
                            g_game->field_39239 = 4;
                        } else {
                            g_game->field_39239--;
                            if (g_game->field_39239 < 0) {
                                g_game->flags_3923b.w |= 4;
                                g_game->flags_3923b.w &= 0xffef;
                                g_game->flags_3923b.b.bit6 = 1;
                            }
                        }
                    }
                } else {
                    // This is a second copy of the countdown_extra block, and
                    // the duplication is load-bearing: with a `goto` here MSVC
                    // 5 leaves the `mov eax,[g_game]` reload after the `jne`
                    // and the jump lands on it, where the original hoists the
                    // reload above the branch and jumps past it. Written out
                    // twice, MSVC tail-merges the copies and hoists it.
                    if (g_game->field_39239 < 0) {
                        g_game->field_39239 = 4;
                    } else {
                        g_game->field_39239--;
                        if (g_game->field_39239 < 0) {
                            g_game->flags_3923b.w |= 4;
                            g_game->flags_3923b.b.bit4 = 1;
                            g_game->flags_3923b.b.bit5 = 1;
                        }
                    }
                    goto skip508;
                }
            } else if ((pi->active == 0 ||
                        (pi->data->flags_9b & 0x40) == 0) &&
                       ((Class_00490360*)g_game->list)->FUN_00490360() != 0) {
                if (g_game->field_39239 < 0) {
                    g_game->field_39239 = 4;
                } else {
                    g_game->field_39239--;
                    if (g_game->field_39239 < 0) {
                        if (g_game->field_37ef6 == 2) {
                            Player_00464f80* self =
                                g_game->players[FUN_00456850()].data;
                            unsigned int typeId;
                            typeId = FUN_00488b10(
                                &g_game->startPos[0x232 *
                                    g_game->players[g_game->localPlayer].data->field_95]) & 0xffff;
                            int bound = 9999;
                            int typeOff = typeId * 0x249;
                            Pos_00464f80 pos;
                            do {
                                int cx = g_game->screen_x / 10;
                                int cy = g_game->screen_y / 10;
                                pos.x = (FUN_004b6c30(g_game->screen_x - 2 * cx) + cx) << 16;
                                pos.y = 0;
                                pos.z = (FUN_004b6c30(g_game->screen_y - 2 * cy) + cy) << 16;
                                int hh = g_game->screen_hh << 16;
                                int hits = 0;
                                unsigned int zacc =
                                    (unsigned int)pos.z - (unsigned int)hh;
                                int outer = 3;
                                int hw = g_game->screen_hw << 16;
                                do {
                                    unsigned int xacc =
                                        (unsigned int)pos.x - (unsigned int)hw;
                                    int inner = 3;
                                    Point16 cell;
                                    cell.y = zacc >> 20;
                                    do {
                                        cell.x = xacc >> 20;
                                        if (FUN_0047db70(
                                                (UnitDef_00464f80*)((char*)g_game->types + typeOff),
                                                0, cell, 1) != 0)
                                            hits++;
                                        xacc += hw;
                                    } while (--inner != 0);
                                    zacc += hh;
                                } while (--outer != 0);
                                if (hits >= 9 && FUN_00421da0(&pos, 0, 0) == -1) {
                                    if (g_game->mode->field_d44 == 0)
                                        break;
                                    if (FUN_00485140(&pos) >
                                        (int)g_game->field_1427f)
                                        break;
                                }
                            } while (--bound > 0);

                            {
                                Unit* unit = FUN_00485f50(
                                    g_game->localPlayer, typeId, pos, 1, 1, 0);
                                FUN_00496e90((Struct_00496e90*)pi,
                                             self->field_a3 * 100,
                                             self->field_a1 * 100);
                                {
                                    // The slot is written through a local
                                    // pointer because that is what makes MSVC 5
                                    // re-read unit->field_ec in the next block:
                                    // it cannot prove the store disjoint from
                                    // it. Written as `unit->field_bc = f` the
                                    // pointer is forwarded from the first block
                                    // instead and the second `mov eax,
                                    // [esi+0xec]` disappears.
                                    float* slot = &unit->field_bc;
                                    float f = (float)self->field_a1 * kHundred;
                                    if (unit->field_ec->active != 0 &&
                                        unit->field_ec->control == 2) {
                                        switch (g_game->field_37eee) {
                                        case 0: f = *slot - f * kNegHalf; break;
                                        case 1: f = *slot - f * kNegSeven; break;
                                        default: f = *slot + f; break;
                                        }
                                    } else {
                                        f = *slot + f;
                                    }
                                    *slot = f;
                                }
                                {
                                    float* slot = &unit->field_d4;
                                    float f = (float)self->field_a3 * kHundred;
                                    if (unit->field_ec->active != 0 &&
                                        unit->field_ec->control == 2) {
                                        switch (g_game->field_37eee) {
                                        case 0: f = *slot - f * kNegHalf; break;
                                        case 1: f = *slot - f * kNegSeven; break;
                                        default: f = *slot + f; break;
                                        }
                                    } else {
                                        f = *slot + f;
                                    }
                                    *slot = f;
                                }
                                FUN_004816a0(1);
                                FUN_0048d630(1);
                            }
                        } else {
                            goto watch_check;
                        }
                    }
                }
            } else {
                goto check230;
            }
        }

    skip508:
        if (pi->active != 0) {
            unsigned char t = pi->type;
            if ((t == 1 || t == 2 || t == 3) && pi->field_146 != 0xa) {
                if ((pi->field_144 != 0 || pi->field_140 == 0) &&
                    (t == 1 || t == 2)) {
                    if ((g_game->flags_3923b.w & 4) == 0 &&
                        g_game->field_39239 < 0) {
                        FUN_00401360(pi);
                    }
                }
            }
        }

        if (bl == g_game->field_2a43) {
            FUN_00467440();
            FUN_00466c20();
            if (g_game->mode->FUN_00435100() == 3) {
                DAT_0051e53c++;
                if ((DAT_0051e53c & 3) == 0)
                    FUN_004573d0(pi, 0, 0);
            }
        }
        goto next_bl;

    watch_check:
        if (g_game->mode->FUN_00435100() == 3 &&
            pi->field_22 == 0) {
            if ((g_game->players[FUN_00456850()].data->flags_9b & 0x80) != 0 ||
                FUN_00457bc0() > 0) {
                pi->data->fb.bit6b = 1;
                if (bl == g_game->localPlayer) {
                    g_game->field_14281 &= 0xfffe;
                    g_game->field_14281 &= 0xfffd;
                    FUN_004816a0(1);
                    FUN_00450f90();
                    if (FUN_00457bc0() == 0) {
                        Dialog_00464f80* dlg = (Dialog_00464f80*)
                            FUN_004aa8f0(g_game->gui, "YESORNO.GUI", 0x900);
                        if (dlg != 0) {
                            FUN_0049fb10(g_game->gui, 1);
                            Widget_00464f80* w = dlg->field_4;
                            FUN_004a0bf0(g_game->gui, "CHOICE1", "Yes", 0);
                            FUN_004a0bf0(g_game->gui, "CHOICE2", "No", 0);
                            FUN_004a0bf0(g_game->gui, "TITLE",
                                         "You're out!  Continue Watching?", 0);
                            strcpy(w->field_cc, "CHOICE1");
                            strcpy(w->field_dc, "CHOICE2");
                            dlg->field_8 = (void*)FUN_00464de0;
                            FUN_004a81e0(g_game->gui, 0x40);
                        }
                        goto skip508;
                    }
                    if (FUN_00457cb0() <= 0)
                        goto skip508;
                    FUN_004abd90(g_game->gui,
                                 FUN_004c5740("You are placed in watch mode because you are hosting AI players which are still alive.  If you exit, they will be terminated."),
                                 500, 1, 1);
                    g_game->flags_3923b.w &= 0xffef;
                    goto skip508;
                }
                goto skip508;
            }
        }

    flags82e:
        g_game->flags_3923b.w |= 4;
        g_game->flags_3923b.w &= 0xffef;
        if (pi->field_22 == 0)
            g_game->flags_3923b.b.bit6 = 1;
        goto skip508;

    check230:
        if (g_game->list->FUN_00490230() != 0)
            goto countdown_extra;
        goto skip508;

    countdown_extra:
        if (g_game->field_39239 < 0) {
            g_game->field_39239 = 4;
        } else {
            g_game->field_39239--;
            if (g_game->field_39239 < 0) {
                g_game->flags_3923b.w |= 4;
                g_game->flags_3923b.b.bit4 = 1;
                g_game->flags_3923b.b.bit5 = 1;
            }
        }
        goto skip508;

    next_bl:
        if (!more_00464f80(++bl))
            break;
    }

    if (g_game->mode->FUN_00435100() == 3 &&
        g_game->field_37ef6 != 2 &&
        FUN_00457cb0() == 0) {
        if (g_game->field_39239 < 0) {
            g_game->field_39239 = 4;
            return;
        }
        g_game->field_39239--;
        if (g_game->field_39239 < 0) {
            g_game->flags_3923b.w |= 4;
            g_game->flags_3923b.w &= 0xffef;
            g_game->flags_3923b.b.bit6 = 1;
        }
    }
}
