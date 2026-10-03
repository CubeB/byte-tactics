// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash and
// Space Bunny Free. Names are provisional.
// Space Bunny Free pass, 60.1% -> 66.4% (777 -> 765 bytes). Measured one at a
// time on the file this pass started from: the byte-sized loop counter alone
// takes 60.1% to 61.5% (and only pays off in combination), the named receiver
// for FUN_004da8d0 takes it to 63.4%, and `res`'s declaration position plus the
// pair built in registers for FUN_004db000 take it to 66.4%.
//   * THE LOOP COUNTER IS A BYTE. `char wraps = 0;` instead of `int wraps = 0;`
//     is what unlocks the original's register allocation. With an int counter
//     MSVC wants a callee-saved register for the constant 0 before the size
//     test (`xor ebp,ebp / cmp esi,ebp`), which takes ebp away from `want` and
//     cascades: `need` ends up memory-only, `want` goes to ebx, and the second
//     scratch register is edx everywhere. With a char counter that hoisted 0
//     lands in esi instead, so `need` gets ebx and `want` gets ebp exactly as
//     the original has them, and `mov dword ptr [esp + 0x44],esi` (0x4dad78)
//     and `push esi` (0x4dada9) come out of the just-zeroed register as they do
//     there. Only a ONE byte type does this: `unsigned char` is worse (distance
//     2024 against 1795) and `short`, `unsigned short`, `int`, `unsigned int`,
//     `long` and `unsigned long` all fold `wraps` back to the literal 0 and
//     hoist it into ebp again. The counter never gets past 2, and the
//     original's `cmp esi,2 / jge` and `inc esi` are what a char counter gives.
//   * NAME THE RECEIVER when the object expression is itself a call: MSVC
//     evaluates a by-value argument before the object expression, so
//     `FUN_004da8d0()->FUN_004dc680(&ins, &rec);` computes the two addresses
//     before calling FUN_004da8d0, where the original calls it first. Written as
//     `Class_004dc680* mgr = FUN_004da8d0(); mgr->FUN_004dc680(&ins, &rec);` it
//     matches, worth 1.9 points on its own. The same for the free-block
//     hand-back: `Class_004db000* mm = (Class_004db000*)FUN_004db610();`.
//   * That hand-back builds its PAIR IN REGISTERS, not in p's slot. The
//     original pushes want and base straight out of registers (0x4daf1c and
//     0x4daf1d) with no copy into p, so the argument has to be a temporary
//     built at the call: `mm->FUN_004db000(mkpair(base, want))` gets those two
//     pushes, while `p.offset = base; p.length = want; mm->FUN_004db000(p);`
//     costs MSVC the two stores at 0x4daf1f and 0x4daf23 that the original does
//     not have (65.7% against 66.4%, 775 against 765 bytes). Declaring
//     FUN_004db000 as two scalars gets the same bytes but spells the callee's
//     signature differently from the files that own it (see NAMING below).
//   * `unsigned int res = 0;` belongs between `unsigned int size = n;` and
//     `if (size == 0) size = 1;`, which is where the original's
//     `mov dword ptr [esp + 0x24],0` sits, before the test; declared after the
//     class locals the store lands after `xor esi,esi` instead and the score
//     falls to 64.2%.
// Also measured this pass: the second insert's pair is stored offset first
// (`p.offset = want + base;` then `p.length = key + len - base - want;`), which
// is the original's store order at 0x4daec5/0x4daec9 (the score is the same
// either way, so take the one the original has), and every spelling of that
// length expression (`key - base + len - want`, `len - base + key - want`,
// `key + len - (base + want)`) compiles to the same object.
//
// NAMING, fixed this pass (the checker compares names, not parameter types, so
// these only show up once the bytes match or when the tree is linked):
//   * data/symbols.csv calls 0x4dbbc0 `Class_004dce60::FUN_004dbbc0`, not a
//     member of Class_004db610, so it now has its own Class_004dce60 and is
//     called through `((Class_004dce60*)map)`. Byte-identical either way, but
//     the old spelling would be flagged once the code matches.
//   * The map's value_type pair is the class 0x4db000.cpp and 0x4db1c0.cpp call
//     `Pair_004db000`, so this file's `Pair_004dacf0` is renamed to match; that
//     makes FUN_004db000, FUN_004dbbc0 and FUN_004dc620 mangle exactly as those
//     files spell them (`?FUN_004db000@Class_004db000@@QAEXUPair_004db000@@@Z`)
//     instead of raising undefined symbols in tools/linkcheck.py.
//   * `FUN_004db1c0` returns `unsigned int`, as the file that defines it
//     (0x4db1c0.cpp) declares, rather than `Node_004dacf0*`; the call site's
//     code is unchanged and its cast goes away.
// The remaining file-local names in extern parameter lists (Ins_004dacf0,
// Class_004dbe10, Class_004d8820) are the usual one-class-several-names case
// docs/consolidation.md describes; tools/linkcheck.py counts them.
//
// WHAT STILL DIFFERS, and what I tried:
//   * ONE REGISTER: the loop counter. The original keeps `wraps` in esi (`inc
//     esi` at 0x4dae1d, `cmp esi,2` at 0x4dae33); with a char counter MSVC packs
//     it into bl, because ebx is free once `need` dies at the guard, so we get
//     `xor bl,bl / inc bl / cmp bl,2` (2 bytes more than the original). Nothing
//     I tried moves it out of bl: the declaration at each of the eleven
//     positions in the locals block (all eleven give the same object), at the top
//     of the function and assigned in place, before the size calls, before
//     `lock`, `wraps = wraps + 1`, `if (wraps > 1)`, `(unsigned)wraps >= 2`,
//     `wraps >= (char)2`, `!(wraps < 2)`, `(char)wraps >= 2`, and the three
//     splittable locals merged into one declaration each (n2/b/cur,
//     base/len/key).
//   * That in turn is why one constant 0 survives in esi: the two uses inside
//     the loop (the second erase's flag and `DAT_005289d4 = 0`) come out as
//     `push esi` and `mov dword ptr [0x5289d4],esi`, with an `xor esi,esi` at
//     the loop head and two more where the fits test clobbers esi, where the
//     original has the immediates `push 0` and `mov dword ptr [0x5289d4],0`.
//     With the byte counter MSVC hoists a constant into a register once the
//     source has four or more literal-zero uses, and the one inside the loop is
//     the one that triggers it: replacing `DAT_005289d4 = 0`, the record ctor's
//     null, `res = 0` or the second erase's flag with a nonzero value drops one
//     of the three `xor esi,esi` but not the register, and only removing the
//     loop's store removes it. No spelling of the other five takes them out of
//     the constant table: `= wraps`, `(char)wraps`, `(unsigned char)wraps`,
//     `(short)wraps`, `q.offset - q.offset`, `q.length += 0`, `q.length *= 1`,
//     `q.length = 0u`, an inline `unsigned int zero(void) { return 0; }`
//     helper, a helper building the lookup pair, and one `unsigned int z = 0;`
//     used for all of them.
//   * THE FRAME SLOTS. The original's low band is want, n2, cur, b, len, res,
//     lock, need; ours is b, n2, cur, want, res, lock, need(erase3's temp).
//     n2, cur, p, q, ins and rec are already right, so only the eight 4-byte
//     slots are permuted, and `want`/`b` are the pair that is swapped. `len`
//     has no home in ours, so the original's `mov [esp+0x20],ebp` before
//     FUN_004dbd00 and its reload at 0x4daea3 have no counterpart here (that
//     spill is what frees ebp for `base + want` at 0x4daed8). MSVC ignores the
//     declaration order for this band (eleven positions, and moving need/want
//     to the top of the function, all give the same object), so it is allocator
//     state again, not statement order.
//   * After the loop `key` and `base` are in the opposite registers (original:
//     esi = key, ebx = base; ours: ebx = key, esi = base), the erase3
//     out-parameter takes a slot of its own instead of aliasing p's, and the
//     second insert sets `mov ecx,edi` after the pushes where the original sets
//     it before them. The clamp spellings are already right (`if (key > base)`
//     beats `if (base < key)`, 1795 against 1800) and the length expression
//     reassociates on its own.
//   * The byte count lines up with exactly those: 780 against our 765 = the two
//     `push 0` and `mov dword ptr [0x5289d4],0` immediates (8 bytes more than
//     our register forms) + `xor bl,bl` (2) - the missing len spill and reload
//     (6).
// Fact worth keeping: with the counter as a byte, MSVC keeps a zero in esi for
// the whole first block, which is why `q.length = 0` and the first erase's `0`
// argument match the original even though the source still says a literal 0.
// Dead ends this pass, all byte-identical to the file below or worse: every
// spelling of the loop test and of both clamps, `int`/`unsigned int` for need,
// want, len, key, base, size, res and pad, the pointer-typed second parameter of
// FUN_004dbe10/FUN_004dbd80, `bool atend` declared before the loop,
// `cur.ptr == map->head` instead of the value comparison, hoisting the fits
// test's length into a named local, the whole search block inside
// `if (map->count > wraps)` with the fits work in the loop body, a `next_block`
// helper for the wrap block, and FUN_004dbd00 declared as
// `void (Pair*, Class_004dbe10)` (2021 against 1825) or `void (Class_004dbe10)`
// (5418).
// The permuter ran 40 minutes on the 60.1% file and 50 on this one without
// finding anything (tools/permute.py --stack need,lock,res,want,b), and
// tools/headers.py found no header set that does better.
//
// ---- earlier passes, kept for the frame map and the dead ends ----
//
// Space Bunny Free pass, best 60.1% (777 bytes), up from 55.6%. Four changes do
// it, three of them the guide's "sete dl; test dl, dl means the result of a
// comparison was stored in a bool local first" pattern: a comparison result put
// in a named `bool` local moves this function's whole allocation, and the
// permuter's finer distance falls 5190 -> 3328. In order of what each is worth:
//   * the loop's end test is now
//       bool atend = cur == (Class_004dbe10(map->head));
//       if (atend) {
//     instead of `if (cur == (Class_004dbe10(map->head))) {` (distance 3472,
//     and it also fixes three unrelated differences: the `res + n` address goes
//     in eax where the original has it, the two record-constructor argument
//     loads use ecx and edx instead of edx and eax, and the `xor ebp,ebp` zero
//     register is gone from the commit path);
//   * `Class_004dbe10::Neq` stores its result in a local first,
//         bool ne = !(a == b);
//         return ne;
//     instead of `return !(a == b);` (distance 3398). The name of the local is
//     irrelevant; `bool same = (a == b); return !same;` is NOT the same code
//     (it costs 3 bytes and scores distance 4004), so it is the `!(a == b)`
//     expression that has to be the initialiser;
//   * the two field reads after the loop are in the other order,
//         len = cur.ptr->length;
//         key = cur.ptr->key;
//     (distance 3333), which is the order the original emits them in;
//   * the second of the two `base = key` clamps is spelled `if (key > base)`
//     rather than `if (base < key)` (distance 3328). The first one, the
//     `base == 0` test and the `base + want > key + len` test do NOT want that
//     spelling, so only this one of the three.
// Trying the same trick on the other comparisons (the fits test,
// `length >= want`, the want/need guard, the empty-map test, `res == 0`, the
// counter tail, the pad branch and both inserts) is worse, whether the bool is
// declared at the point of use or at the top of the function: it costs 7
// bytes there. The second insert's pair is written
// `p.length = key + len - base - want; p.offset = want + base;`, the field
// order the original's store order implies, and every earlier declaration
// permutation still compiles to the same object.
// A note on scoring: check.py's ratio counts our own jump TARGETS, so a
// one-byte shift in our code moves several `j??` lines and the ratio swings
// 1 to 1.5 points with no change in the code's structure. The permuter's
// `fine_score` (tools/permute.py) does not have that problem; this pass was
// driven by it. Earlier this pass the guard `map->count < wraps + 1` (MSVC
// folds `wraps + 1` to 1 there) scored 57.1% against 55.6% for
// `count <= wraps`; once the bool local is in both spellings give the same
// bytes, so the plain `<=` is back in the file.
// The frame map, re-derived from the operand bytes and worth keeping (E = esp
// after `sub esp,0x68` and the four pushes, so E = esp0-0x78, the four saved
// registers sit at E+0x00..E+0x0c, the 0x58 bytes of locals at E+0x10..E+0x67,
// arg1 at E+0x7c and arg2 at E+0x80):
//   original  E+0x10 want  E+0x14 n2  E+0x18 cur  E+0x1c b  E+0x20 len
//             E+0x24 res   E+0x28 lock E+0x2c need E+0x30 p  E+0x38 q
//             E+0x40 ins   E+0x48 rec (0x0x30 bytes, ends exactly at the retaddr)
// so the original has a `len` home at E+0x20 and `need` at E+0x2c. That map
// was read off the 55.6% build, which put `b` at E+0x10, `want` at E+0x1c,
// `need` at E+0x20 and the FUN_004dbd00 return temp at E+0x2c; the bool locals
// moved the slots again, so treat the ORIGINAL row as the fact and the ours
// row as one allocation state among several. The original's three erase calls
// (FUN_004dbe10,
// FUN_004dbd80 and FUN_004dbd00) all pass E+0x30, the slot of the named pair
// `p`; ours gives the first two E+0x30 and FUN_004dbd00 a temp of its own at
// E+0x2c, which is exactly the dword `need` is missing from. The original's
// `mov [E+0x20],ebp` between the two pushes of the FUN_004dbd00 call is the
// spill of `len`, which only happens when `want` holds ebp (ebp is taken by
// `len` at 0x4dae42, then by base+want at 0x4daea7), so the slot map, the
// missing len spill and the register rotation are all one allocation state.
// The tail (pad, both FUN_004d82c0 calls, the record ctor, the counters and
// both epilogues) is byte-identical apart from the ctor's `push ebp` for the
// null argument, where the original pushes the immediate 0; at 55.6% the same
// tail also had `lea ecx,[ebx+edi]` for `res + n` and edx/eax instead of
// ecx/edx for the two ctor argument loads, and the bool locals fixed both.
// New facts this pass: (1) the second insert's length is spelled
// `key - base + len - want` in the original (`sub esi,ebx; add esi,eax;
// mov eax,edx; sub esi,eax`), not `key + len - base - want`, but MSVC
// reassociates both spellings here, so the source order is not the lever;
// (2) the `xor ebp,ebp` zero survives every spelling of the zero uses:
// `q.length = wraps`, the erase's second argument as `wraps`, an `AtLeast1`
// helper for the size fixup, an `IsZero` helper for the `res == 0` tests,
// `res = n - n`, `res` as int/LPVOID/unsigned, `wraps` as int/unsigned/long/
// unsigned short, `map->count` as int, the guard as `count <= wraps`,
// `wraps >= count`, `!(count > wraps)`, `wraps + 1 > count`,
// `count >= wraps + 1`, `count == 0` and an empty if/else with the goto; the
// zero register is still there in every one. (3) Also flat or worse: declaring
// the whole first block (q, n2, b, cur) in a nested scope as 0x4db1c0 is
// written, `while (1)` for the walk loop, `cur = b` / `b = n2` struct copies,
// an `end` local for base+want (56.8% alone, 56.4% with the new guard), a
// temporary Pair for the FUN_004db000 call and for both inserts, Pair
// constructors, Pair parameters by value or by const reference, the erase
// callees declared by-value-returning (55.2%) or as `void` (49.6%), FUN_004dbd00
// as an explicit out-parameter (49.3%), the combined `||` guard (43.0%),
// `res` uninitialised (43.3%), all-at-the-top declarations (44.0%), and every
// permutation of n2/b/cur. `tools/permute.py` from the 55.6% file climbed to
// 55.9% (790 bytes) with helpers and do/while(0) wrappers that are not
// committable.
// The remaining blocker is still the single register allocation at the top of
// the function: the original materialises no constant zero at all (it stores
// `mov [E+0x24],0`, compares with `test esi,esi`, pushes immediates and uses
// esi, the just-zeroed wraps, for q.length and the erase argument), while this
// compile puts the constant in ebp and pushes want out to ebx. Give want ebp and
// the whole rest of the function follows.
// claude-sonnet-5-5 pass (55.6% unchanged, 777 bytes). New facts: (1) wraps is a SIGNED int in the
// original (`cmp esi,2 / jge`, not jae) and map->count is unsigned (`jbe`); fixed here, score flat but
// that diff line is gone. (2) The `xor ebp,ebp` zero register that steals ebp from `want` is NOT
// caused by res/size/q.length/the push 0s: changing the loop store `DAT_005289d4 = 0` to `= 5` makes the
// top-of-function zero vanish (the constant 5 is then hoisted instead, `mov ebp,5` in the loop
// preheader). So MSVC hoists any constant stored inside the free-block walk loop into a free callee-saved
// register, and the original evidently had no free register there or did not see a constant. Tried
// without effect (all 55.6 or lower): the 24 orderings of the four statements in the wrap block, the
// loop as do/while with the found path nested inside (as matched neighbour 0x4db1c0 is written), goto
// loop, `*(void**)&DAT = 0`, DAT as void*, static inline setters, extra uses of need/want in the loop,
// res as a one-member struct with/without a ctor, res first/after Enter/after size, wraps first, want
// declared first, all locals declared uninitialised at the top, a CRITICAL_SECTION* local, 55 random
// combinations of these. The lever is whatever stops the loop-constant hoist.
// deepseek-v4.1-flash (seventh run, 10-minute box): 55.6% (777 bytes) unchanged. Tested:
// `unsigned int res = 0;` moved next to wraps (before the Class objects) was byte-flat at 55.6,
// and as the first local before lock dropped to 51.5, so the res-zero placement is not the
// lever; both reverted.
// deepseek-v4.1-flash (sixth run, 10-minute box): 55.6% (777 bytes) unchanged.
// Tested: hoisting `unsigned int wraps = 0;` next to `unsigned int size = n;` to
// reproduce the original's early `mov dword ptr [esp+0x24], 0` dropped the score to
// 42.5 (frame/slot shuffle), reverted. Still differs at the top: original keeps
// need in EBX (spilled 0x2c) and want in EBP (spilled 0x10) with the zero spilled
// at 0x24; ours ranks them differently and reloads from different slots.
// Started by space-bunny-free, continued by deepseek-v4.1-flash and GPT-6; deepseek-v4.1 retry.
// deepseek-v4.1-flash retry 3 (best 55.6, unchanged, 5 check.py runs): no source shape tried
// this pass removes the `xor ebp,ebp` zero register, so the ebp lock-out is confirmed to come from
// the allocator rather than from spelling. Results: `unsigned int res;` at the top plus a separate
// `res = 0;` statement after the size fixup 45.9 (prologue reshuffles, lock into eax); `res`
// declared between `size = n;` and the size test 51.5 (lock moves to ebx); the FUN_004dbe10 second
// argument spelled as `wraps` instead of the literal 0, 55.6 with an unchanged diff; `res` retyped
// as `LPVOID` with the matching casts, 55.6 with an unchanged diff; the `want < need` guard flipped
// to `need > want`, 55.6 unchanged. In every variant the compiler emits `xor ebp,ebp` before the
// size test and reuses ebp for res's home store, both zero pushes and the record ctor's 5th
// argument, exactly the register the original spends on `want`. The original instead stores every
// zero as an immediate and uses esi (wraps, freshly zeroed) for the pair length and the be10
// argument. Next idea for a retry: give every literal-0 use on the path to the record ctor a
// competing already-live home (esi) so no zero register is wanted at all; the earlier pass tried
// this only partially (52.2). Same allocator class as 0x4a6ae0 and 0x4866d0.

// deepseek-v4.1-flash retry 2 (best 55.6, unchanged): swept the `res` declaration position
// (right after size, after map, after wraps) all stay 55.6; before lock drops to 51.5 because
// lock moves to ebx. The blocker is unchanged: `res = 0` compiles to `xor ebp,ebp` and ebp is
// then reused for the `map->count` compare, so `want` cannot take ebp; the original stores the
// 0s as immediates and compares against the just-zeroed esi (wraps).
// NOT A MATCH: measured 52.2 percent, ours 778 bytes against the original 780, frame 0x68 (correct,
// and every callee/data reference resolves at the same place). The body is instruction for
// instruction right except for register allocation:
//   * the original homes lock/need/want/wraps in edi/ebx/ebp/esi and keeps res (E+0x24) in memory
//     only; this build homes lock and map in ebx, want in edi, res in ebp and need in memory at
//     E+0x20, and the loop counter wraps shares ebp as the zero source.
//   * consequences of that one difference: `cmp dword ptr [map+0xc],esi / jbe` becomes
//     `cmp dword ptr [map+0xc],ebp / jbe`; `push 0` for the record ctor's last argument becomes
//     `push ebp`; the pad branch's `lea eax,[ebx+edi]` becomes `lea ecx,[ebx+edi]`; the b iterator
//     home slides from E+0x1c to E+0x10 (n2 stays at E+0x14); the early `mov dword ptr [esp+0x24],0`
//     becomes `xor ebp,ebp / mov [esp+0x24],ebp`.
//   * tried without effect: removing the `= 0` initialisers on base/len/key, splitting one Pair into
//     the two the original has, swapping the iterator declarations, `wraps` before `map`, the
//     `!(a == b)` Neq form, and changing the local declaration order. Every attempt leaves the
//     ebx/ebp/edi assignment as above, so this looks like MSVC5 allocator state the local source
//     does not steer (same class as 0x4a6ae0 and 0x4866d0).
// GPT-6 retry: repaired shared page commitment for reused blocks, zeroed base on
// failed reservation, fixed the 48-byte record layout and iterator hidden-return ABI, and corrected
// matched callee owners. Earlier notes below describe the superseded reconstruction. Progress over the 49.7 percent version came from removing the `= 0`
// initialisers on base/len/key (the original does not zero them, and the extra zero store was the
// spilled home that both pushed the frame to 0x6c and took the register allocation away from
// map/wraps) and from splitting the one Pair into the two the original has
// (p at E+0x30, the lower_bound query q at E+0x38), plus masking n before the
// negate in pad.
// The frame is still 0x6c where the original is 0x68: our locals relative to E
// match the original exactly (res 0x24, lock 0x28, need 0x2c, p 0x30, q 0x38,
// ins 0x40, rec 0x48) but E sits 4 bytes lower, so the extra dword is reserved
// above the record, most likely a distinct hidden-return slot for the
// FUN_004dbd00(cur) erase call (the original aliases it onto E+0x30, the p
// slot). Changing the local declaration order changes nothing.
// Worked: want/need split as a Pair; no `= 0` on base/len/key; base loaded from
// DAT_005289d4 before the `if (base == 0)`; pad = (0 - (n & 0xfff)) & 0xfff.
// Still differs: frame 0x6c; the record ctor's last argument is pushed as a
// register (ebp) instead of the immediate 0; a lea lands in ecx where the
// original uses eax in the commit path.
// The original's frame (E = esp after `sub esp,0x68` and the four pushes):
//   0x10 want        0x14 n2 (lower_bound out)   0x18 cur    0x1c b, then
//   reused as prev  0x20 len   0x24 res   0x28 the CRITICAL_SECTION pointer,
//   then `need` (both use that one home)   0x2c NOT referenced at all, a
//   genuine hole   0x30/0x34 the pair, which is ALSO the hidden-return slot of
//   FUN_004dbe10, FUN_004dbd80 and FUN_004dbd00 (all three pass &slot first
//   and 0 second)   0x38/0x3c the lower_bound query pair   0x40 the insert
//   result, reused for the second map's insert   0x48 the 48-byte record, whose
//   tail overlaps the four saved registers (0x4d8849 stores the 4th ctor
//   argument to [esi+0x2c] = E+0x74 = saved edi, which the epilogue pops and
//   discards, so the original gets away with it).
// What worked (all scored free with `check.py --sym`):
//   * `FUN_004da780()` returns the CRITICAL_SECTION pointer itself, and the
//     size argument is NOT written back: `unsigned int size = n; if (!size)
//     size = 1;`. That gives the original's `test esi,esi / jne / mov esi,1`
//     with esi live. Assigning to the parameter instead (v0, 48.5%) produced
//     `mov [esp+0x80],1`, a store into the argument slot, and turned the
//     `test` into a `cmp` against the register holding the zero.
//   * Pair_004db000 with NO constructors at all: a user-provided default
//     constructor zero-initialises the pair at the top of the function, and
//     the original has no such stores (v0 had three `mov [esp+X],ebp`).
//   * `if (cur == (Class_004dbe10(map->head)))`: the original's head test is
//     `mov ebx,[edi+4] / xor edx,edx / cmp eax,ebx / sete dl / test dl,dl /
//     je`, i.e. the End() iterator is a VALUE compared through operator==.
//     Comparing `cur.ptr == map->head` loads the head straight into the cmp
//     and drops the sete/test (49.7 -> 47.7 when reverted).
// Tried and did NOT work:
//   * declaring `wraps` before `map` to swap the esi/edi roles the original has
//     (map in edi, wraps in esi): no effect at all, 47.7 either way.
//   * the other change folded into the same run as the head test (both
//     together): 47.9, worse than the head test alone, so the two do not
//     compose.
// Still differs:
//   * `need` and `want` are in the opposite callee-saved registers (the
//     original has need in ebx and want in ebp, this file has need in ebp and
//     want in ebx, and therefore the `cmp` is the other way round).
//   * the original stores `need` into the lock's home at 0x28 and compares in
//     registers; this file gives `need` a home of its own, one dword high.
//   * the original keeps `map->count <= wraps` as `cmp [edi+0xc],esi / jbe`
//     and does not fold the known zero; this file folds it to `test eax,eax`.
//   * the trailing half (the two insert calls, VirtualAlloc, the record and
//     its ctor) has not been revisited since v0.
// deepseek-v4.1-flash retry: spelling `if (!size)` as `if (size < 1)` is the
// only change that scored better (52.2 percent, 778 bytes) because MSVC folds
// the `size = 1` fixup into the test (`cmp esi,1`). tools/headers.py (128 sets)
// and a dummy-declaration sweep of N = 0..400 all stay flat at 51.1/51.5, and
// every res/need/want declaration-order permutation keeps lock in ebx and the
// zero from `res = 0` in ebp, so the edi/ebx/ebp rotation above still stands.
// Carve a block out of the reservation allocator: enter the lock, round the
// request up (need) and double it (want), then walk the free-block map for a
// block of at least want bytes, splitting it around the allocation point and
// committing the pages with VirtualAlloc. Same std::map idiom as 0x4db000,
// 0x4db450 and 0x4db1c0: the block is the map's value_type, a base and a length.
// deepseek-v4.1-flash pass 2 (best 55.6 with `#include <memory.h>` added, 55.2
// without; tools/headers.py picks <windows.h> <memory.h> as the closest set and
// the 55.6 state is the same one 28 to 52 unused declarations reach): moved
// `unsigned int res = 0;` from the
// top of the locals down to just after `cur` and spelled the size fixup
// `if (size == 0) size = 1;` (which restores the original's `test esi,esi /
// jne / mov esi,1`; `if (size < 1)` had folded it to `cmp esi,1 / jae` and was
// what the earlier 52.2 version used). Those two moves took lock/map from ebx
// to edi and pushed the code from 52.2 to 55.2 percent.
// What is left is ONE register choice and its cascade: MSVC materialises the
// constant 0 in ebp at the size test (`xor ebp,ebp / cmp esi,ebp`) and then
// reuses ebp for `DAT_005289d4 = 0`, `q.length = 0` and the two zero pushes.
// The original never keeps 0 in a register: it stores the 0s as immediates and
// uses esi (wraps, just zeroed) for `q.length` and the FUN_004dbe10 argument.
// Because ebp is held by that constant here, `want` is pushed out to ebx, and
// with ebx occupied the find path's iterator temporaries land in edx/ecx where
// the original uses ebx and the head test's flag lands in cl where the
// original uses dl. Give ebp to `want` and the whole function should snap into
// place; the original's callee-saved homes are edi = lock then map, esi = size
// then wraps, ebp = want, with ebx left for temporaries and `need` and `res`
// memory only.
// Tried this pass, all scored with check.py --sym:
//   * demoting `res` by reading it once into a separate variable (`blk`,
//     `void* blk`, or a plain copy): 48.0, the compiler coalesces the copy and
//     keeps res in ebp; `res` declared at the very top again (w1): 51.1.
//   * `if (n == 0)`, `if (!size)`, `size = n ? n : 1`: all still materialise
//     the ebp zero (55.2/53.7/55.2).
//   * replacing the loop's `DAT_005289d4 = 0` with `= wraps`, and/or
//     `q.length = wraps`, to remove the constant: 52.2 and worse; the constant
//     still appears.
//   * swapping the need/want declarations, declaring need+want before size,
//     declaring res after wraps, after ins, after b, after cur: 53.7 to 55.2.
//   * a dummy-declaration sweep of N = 0..400 in steps of 4 flips between
//     exactly two states, 55.6 and 46.8, with no intermediate value, so the
//     source shape is the only thing left to change.
//   * defining the real preceding function 0x4dabb0 above this one (the guide's
//     state technique) gives 55.6, the same state as the dummy sweep, so it
//     buys 0.4 points and is not worth the extra code here.
// The zero-register theory to try next: make `res = 0` not a foldable
// constant, or get the size test to emit `test` before any zero exists. Every
// spelling of the fixup tried so far emits the cmp.
#include <windows.h>
#include <memory.h>

extern unsigned int DAT_005289d4;
extern unsigned int DAT_00528a00;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_005289d0;
extern unsigned int DAT_00528a04;

struct Node_004dacf0 {
    Node_004dacf0* left;   // +0x0
    Node_004dacf0* parent; // +0x4
    Node_004dacf0* right;  // +0x8
    unsigned int key;      // +0xc
    int length;            // +0x10
    int color;             // +0x14
};

class Class_004dbe10 {
  public:
    Node_004dacf0* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    Class_004dbe10* FUN_004dbe10(Class_004dbe10*, int);
    Class_004dbe10* FUN_004dbd80(Class_004dbe10*, int);

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dbe10 a, Class_004dbe10 b)
    {
        bool ne = !(a == b);
        return ne;
    }
};

struct Pair_004db000 {
    unsigned int offset; // +0x0
    int length;          // +0x4
};

struct Ins_004dacf0 {
    Node_004dacf0* ptr;
    unsigned char inserted;
};

class Class_004db610 {
  public:
    char unknown_0[4];
    Node_004dacf0* head; // +0x4
    char unknown_8[4];
    unsigned int count; // +0xc
    int total; // +0x10
    char unknown_14[20];

};

class Class_004dce60 {
  public:
    void FUN_004dbbc0(Ins_004dacf0* out, Pair_004db000* v);
};

class Class_004db450 {
  public:
    bool FUN_004db450(unsigned int);
};
class Class_004db000 {
  public:
    unsigned int FUN_004db1c0(unsigned int);
    void FUN_004db000(Pair_004db000);
};
class Class_004dbd00 {
  public:
    Class_004dbe10 FUN_004dbd00(Class_004dbe10);
};
class Class_004dbeb0 {
  public:
    Class_004dbe10* FUN_004dbeb0(Class_004dbe10*);
};
class Class_004dc620 {
  public:
    Class_004dbe10* FUN_004dc620(Class_004dbe10*, Pair_004db000*);
};

class Class_004d8820 {
  public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c, unsigned int d, const char* e);
};

class Class_004dc680 {
  public:
    void FUN_004dc680(Ins_004dacf0* out, Class_004d8820* v);
};

class CritSec_004da780 {
  public:
    CRITICAL_SECTION cs;
};

CritSec_004da780* FUN_004da780();
unsigned int __cdecl FUN_004da8a0(unsigned int size);
unsigned int __cdecl FUN_004da8c0(unsigned int size);
Class_004db610* FUN_004db610();
Class_004dc680* FUN_004da8d0();
void __cdecl FUN_004da7d0(unsigned int size);
char FUN_004db760();
int FUN_004db7c0();
void __cdecl FUN_004d82c0(void* at, int value, unsigned int count);

static inline Pair_004db000 mkpair(unsigned int o, int l)
{
    Pair_004db000 v;
    v.offset = o;
    v.length = l;
    return v;
}

// FUNCTION: 0x4dacf0
unsigned int __cdecl FUN_004dacf0(unsigned int n, unsigned int arg2) {
    CritSec_004da780* lock = FUN_004da780();
    EnterCriticalSection(&lock->cs);
    unsigned int size = n;
    unsigned int res = 0;
    if (size == 0)
        size = 1;
    unsigned int need = FUN_004da8c0(size);
    unsigned int want = FUN_004da8a0(size);
    if (want < need) {
        LeaveCriticalSection(&lock->cs);
        return 0;
    }

    Class_004db610* map = FUN_004db610();
    // A byte counter, and that is load bearing: it is what decides where MSVC
    // puts the constant 0 (see the notes above). It never gets past 2.
    char wraps = 0;
    unsigned int base;
    unsigned int len;
    unsigned int key;
    Pair_004db000 p;
    Pair_004db000 q;
    Ins_004dacf0 ins;
    Class_004dbe10 n2;
    Class_004dbe10 b;
    Class_004dbe10 cur;

    if (map->count <= wraps)
        goto alloc_new;

    q.offset = DAT_005289d4;
    q.length = 0;
    ((Class_004dc620*)map)->FUN_004dc620(&n2, &q);
    ((Class_004dbeb0*)map)->FUN_004dbeb0(&b);
    if (n2.Neq(n2, b)) {
        b.ptr = n2.ptr;
        b.FUN_004dbe10((Class_004dbe10*)&p, 0);
        if (DAT_005289d4 >= b.ptr->key && want + DAT_005289d4 <= b.ptr->key + b.ptr->length)
            n2.ptr = b.ptr;
    }
    cur.ptr = n2.ptr;

    for (;;) {
        bool atend = cur == (Class_004dbe10(map->head));
        if (atend) {
            ((Class_004dbeb0*)map)->FUN_004dbeb0(&b);
            cur.ptr = b.ptr;
            DAT_005289d4 = 0;
            DAT_00528a00++;
            wraps++;
        }
        if (cur.ptr->length >= want)
            break;
        cur.FUN_004dbd80((Class_004dbe10*)&p, 0);
        if (wraps >= 2)
            goto alloc_new;
    }

    len = cur.ptr->length;
    key = cur.ptr->key;
    ((Class_004dbd00*)map)->FUN_004dbd00(cur);
    base = DAT_005289d4;
    if (base == 0) {
        base = key;
        DAT_005289d4 = base;
    }
    if (key > base)
        base = key;
    if (base + want > key + len)
        base = key;
    if (base > key) {
        p.offset = key;
        p.length = base - key;
        ((Class_004dce60*)map)->FUN_004dbbc0(&ins, &p);
    }
    if (base + want < key + len) {
        p.offset = want + base;
        p.length = key + len - base - want;
        ((Class_004dce60*)map)->FUN_004dbbc0(&ins, &p);
    }
    DAT_005289d4 = base + want;
    goto commit_block;

alloc_new:
    if (((Class_004db450*)map)->FUN_004db450(want))
        base = ((Class_004db000*)map)->FUN_004db1c0(want);
    else
        base = 0;
commit_block:
    if (base) {
        res = (unsigned int)VirtualAlloc((void*)base, need, MEM_COMMIT, PAGE_READWRITE);
        if (res == 0) {
            Class_004db000* mm = (Class_004db000*)FUN_004db610();
            mm->FUN_004db000(mkpair(base, want));
        }
    }

    if (res == 0) {
        LeaveCriticalSection(&lock->cs);
        return 0;
    }
    unsigned int pad = (0 - (n & 0xfff)) & 0xfff;
    if (FUN_004db760()) {
        FUN_004d82c0((void*)res, FUN_004db7c0(), pad);
        res += pad;
    } else {
        FUN_004d82c0((void*)(res + n), FUN_004db7c0(), pad);
    }
    {
        Class_004d8820 rec(res, n, DAT_00528a04, arg2, 0);
        Class_004dc680* mgr = FUN_004da8d0();
        mgr->FUN_004dc680(&ins, &rec);
    }
    FUN_004da7d0(n);
    DAT_005289f0 += (n + 0xfff) & 0xfffff000;
    if (DAT_005289f0 > DAT_005289d0)
        DAT_005289d0 = DAT_005289f0;
    LeaveCriticalSection(&lock->cs);
    return res;
}