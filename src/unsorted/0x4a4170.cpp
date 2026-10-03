// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, and GPT-6.1-sol, edited by deepseek-v4.1, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// claude-opus-5-5 (issue 4160): still 86.5%, code unchanged, but the
// residual is now pinned down to ONE missing zero use, with a body that is
// otherwise byte-exact. Start the next attempt from this sketch of the focus
// block (build/scratch/0x4a4170/d3.cpp in the 4160 worktree), not from the
// body below:
//     if (!FUN_004ab5b0(obj, 3)) { obj->focus = -1; obj->field_78 = 0; }
//     int old;
//     if (obj->field_78) {
//         old = e->off;
//         if (e->flags & 1)
//             e->off = obj->field_94 - obj->saved.x + p.x;
//         else
//             e->off = obj->field_94 - obj->saved.y + p.y;
//     } else {
//         old = e->off;
//         if (e->flags & 1) {
//             if (p.x < r2[0]) e->off--; else if (p.x > r2[2]) e->off++;
//         } else {
//             if (p.y < r2[1]) e->off--; else if (p.y > r2[3]) e->off++;
//         }
//     }
//     ...clamp, `if (e->off == old) return;`, holder, calls as below.
// That is the natural source: `e->off--` gives the original's 16-bit
// `mov ax,[ebx+0x140]` / `movsx ecx,ax` without the `(short)v` cast, the
// per-path stores tail-merge into the original's single store AND skip it
// when nothing changed (the original's jle lands past the store; the shared
// `e->off = v;` below stores unconditionally, which the original does not),
// and obj->field_94 read in both drag branches is hoisted as the original
// does. On its own it scores 74.4% / 729 bytes because the constant 0 does
// not get a register.
// THE MEASURED FACT: add one dummy zero store anywhere between the clamp and
// the FUN_004a2580 call (`e->field_157 = 0;`, a short or a pointer field
// works too) and the whole function is byte-identical to the original apart
// from that one store: xor edx,edx in both arms of the ab5b0 test, the
// memory-form compares, `mov al` in the drag arm, edi/esi in the clamp, the
// immediates in the tail. So MSVC's decision to keep 0 in a register for the
// region between the ab5b0 call and the FUN_004a2580 call is one zero store
// short. Measured on this body: a dummy zero COMPARE does not tip it, a byte
// store does not, a dummy store before the call or after FUN_004a2580 does
// not, and earlier in the region (the !ok arm, the drag arm) only partly.
// Tried and folded before the decision (all byte-identical to 74.4%): dead
// inits of old (`int old = 0`), redundant repeated zero stores, inline
// helpers for the clamp, the dirty flag and the redraw (with and without
// their own null tests), bool/int helpers returning the change, `!= 0` and
// `== FALSE` spellings, switch on field_78, union and pointer aliases of
// old, off, field_78 and holder, the preceding function 0x4a3ef0 defined
// unannotated above (it is matched now), and /Gi. Naming the call result
// (`int ok = FUN_004ab5b0(obj, 3); if (!ok)`) gets the zero register on this
// body but spends it on the call test (`xor edx,edx; cmp eax,edx`), 77.7% at
// 716 bytes. In 0x4a4d70 and 0x4a3ef0 the same family of residual (a zero
// register the original has and ours lacks) fell to an inline helper around
// an existing call (GetGlyph around FUN_004b7f30), so look for a helper this
// function's source used around one of its calls or fields.
// Not a bug, withdrawn below: `mov ax,[ebp+0x94]; sub ax,[ebp+0x7c]; add
// eax,esi` only feeds the 16-bit store of e->off, so the high half of eax
// never matters; that is MSVC narrowing a short assignment.
// DeepSeek V4.1 Flash (issue 4879): still 86.5%, 714 bytes. Ran the permuter
// for 3 min (3112 candidates) and it was flat at 86.5%. Compile-only sweeps of
// the zero value (plain/phi locals of every width, an inline helper returning
// 0, a named call result, pointer-typed field_78, holder-test and tail
// spellings) either fold back to the eax store or hoist the zero before the
// call test; none produced the original's two per-arm `xor edx, edx` with a
// separate `test eax, eax`. Isolated the flip with a reduced model: the full
// tail (the `if (obj->holder) obj->holder->field_14 = 1;` conditional store
// plus both tail calls) is what makes MSVC pick the eax/immediate form; a tail
// that early-returns on `holder == 0` keeps a register zero in edx/ecx. The
// residual is still the single edx-zero decision described below.
// Retry (deepseek-v4.1-flash, issue 4076, short box): two further scored
// negatives, both flat at 714 bytes / 80.9 pct and byte-identical to the
// baseline shape: an `int z;` phi (z = 0 in both arms, store in the if arm,
// read after the merge by the drag test, the clamp and the holder test) and an
// `int zero = 0;` with a redundant `zero = 0;` redefinition in the otherwise
// empty else arm. Both rematerialise as `mov [ebp+0x78], eax` plus immediate
// compares, so a two-arm constant phi does not give the value a register home.
// Retry (deepseek-v4.1-flash, issue 3781): no new variant run; the ctx
// disassembly confirms the original materialises the zero twice at the merge
// (0x4a420d and 0x4a421b xor edx,edx, then cmp [ebp+0x78],edx), while ours
// keeps one edi zero, so the residual stays the edx-zero versus edi-zero
// allocation documented below.
// deepseek-v4.1-flash retry (issue 3704, 10 min, 3 scored variants): best stays 80.9 pct.
// Naming the FUN_004ab5b0 result and capturing p.y in an int py local are both
// flat at 80.9 / 714 bytes, so neither moves the edx zero versus p.y-in-edi
// tie; the tail diff hunks are unchanged.
// Retry (deepseek-v4.1-flash, issue 3366, 900s): still 80.9%, no MATCH. All
// literal-zero spellings fold and stay byte-identical to the baseline: an
// `int zero = 0` declared at the top of the focus block or after the
// FUN_004a23b0 call and used for the field_78 compare, the `e->off < 0` clamp
// and the holder compare scores 80.9 (also as short/unsigned short). The one
// shape that does produce the wanted live edx zero is
// `int r = FUN_004ab5b0(obj, 3); if (r == 0)`: then the clamp becomes the
// original `cmp word[ebx+0x140], dx` / `mov word[ebx+0x140], dx` and the holder
// becomes `cmp eax, edx`, but the call test itself turns into
// `xor edx,edx; cmp eax,edx` instead of `test eax,eax` (709 bytes, 75.1), so
// the zero has to originate at the merge, not at the call test. Reconstructing
// the original single shared store (an `int`/`short` newOff assigned in each
// arm, `e->off = newOff` once after) is worse: 714 bytes 73.6 (int) and
// 713 bytes 65.4 (short/unsigned short). Moving `old = e->off` into each arm,
// with or without a zero local, is 707-708 bytes and 73.8-74.0. A
// `static inline` clamp helper whose zero is a plain constant folds as well
// (709 bytes, 70.8). Nothing new is left to try that does not either fold the
// zero or break the call test; the blocking decision is the live edx zero at
// the FUN_004ab5b0 join described below.
// GPT-6.1-sol retry (#3140): baseline remains best at 80.9% after five checks.
// deepseek-v4.1-flash (#3888 retry): still 80.9% / 714 bytes over two scored runs.
// The explicit spellings `if (obj->field_78 != 0)` and `if (obj->holder != 0)` in
// the focus block are byte-identical to the implicit forms, so the compare
// spelling does not steer the constant-0-in-edx decision.
// A two-arm zero local was canonicalized to the baseline. A local max clamp
// scored 69.1%, and copying field_78 into a local dragging flag scored 50.9%.
// Keep the direct field test and original clamp. No MATCH observed.
// Retry (deepseek-v4.1-flash, issue 3076): still 80.9%. The residual: the
// original keeps a literal 0 live in edx across the `focus == index` block
// (per-arm `xor edx,edx`, `cmp [ebp+0x78],edx`, `cmp word [ebx+0x140],dx`,
// `cmp eax,edx`); ours rematerialises it as immediates off the known-zero call
// result. New negatives this retry all worse: an explicit per-arm phi still
// folds to `mov [field_78],eax` (v1, 80.9); `e->flags & 0x10` zero 47.4;
// `index - obj->focus` / `obj->focus - index` are not folded but add bytes and
// never zero edx (61.7/54.7). Naming the call result puts the zero in edx
// (`mov [field_78],edx` matches) but moves the zero to the call test and changes
// the field test (709 bytes, 75.1); the two halves cannot be separated from
// source.
// BUG: at 0x4a4237 (`mov ax,[ebp+0x94]; sub ax,[ebp+0x7c]; add eax,esi`) the
// offset difference is computed at 16-bit width (`ax`) and then added as a
// 32-bit value to `esi` without sign-extending `ax`, so the high half of eax is
// whatever it held. Same in the parallel arm at 0x4a423f.
// deepseek-v4.1-flash (#2961 retry): still 80.9%. The residual is the single
// edx zero in the `focus == index` block: the original keeps constant 0 in edx for
// five uses with two per-arm `xor edx,edx`; ours rematerialises it as
// `mov [ebp+0x78],eax` plus test/immediates and cascades the tail allocation.
// ~17 shapes (phi-0 locals of several types, zero from a proven-zero field,
// store/assign reorder, reload) all value-number back to the constant or score
// worse. Suspected bug (stands from the file note): `mov ax,[ebp+0x94];
// sub ax,[ebp+0x7c]; add eax,esi` narrows to 16 bits then adds a 32-bit value
// without sign-extending ax, so the high half is garbage.
// deepseek-v4.1-flash retry (#2905), two quick scratch checks, no change: 80.9%.
// Still the single edx-zero decision in the focus==index block. Declaring
// FUN_004ab5b0 bool (Ghidra hints bool) makes it worse (69.9%); naming the
// call result reintroduces the xor/cmp at the call test (75.1%). No new shape
// found that keeps a live zero in edx; best source is the 80.9% one below.
// deepseek-v4.1 retry (#2430), five further checks, no change: 80.9%, same two
// hunks. The remaining diff is one backend decision (the constant 0 keeps a
// register, edx, across the focus block) and every "real variable" reading of
// it folds to the same immediates:
//   - `int z = e->field_157;` used for all five zero uses (store to field_78,
//     its compare, the off clamp cmp+store, the holder compare): after the
//     `if (field_157) return;` guard MSVC proves z is 0 and folds it, 80.9%.
//   - `int z = 0;` declared before the FUN_004a23b0 call, so its live range
//     crosses a call and the backend would have to rematerialise it after:
//     still folded to immediates, 80.9%.
//   - the per-arm phi (`if (...) { z = 0; ... } else { z = 0; }`): the two
//     constant defs value-number to one constant and fold, 80.9%.
// Note the counts: the original gives the constant 0 a register for five uses,
// ours has only two unavoidable ones (the clamp's cmp and store); the store to
// field_78 becomes `mov [ebp+0x78], eax` off the known-zero call result, and
// both `!= 0` tests become load+`test`. Those extra register uses are the
// difference, and they disappear because the compiler already has a zero in
// eax. A shape that keeps a live value in eax at 0x4a420d, or adds a third
// immediate-zero use inside the block, was not found. Checked in #2430 and also
// worse: naming the FUN_004ab5b0 result (`int r = ...; if (!r)`) shrinks the
// function to 709 bytes and 75.1%, and the `!= 0` spelling of the drag test is
// byte-identical. The second hunk (tail) follows from the same decision: with no
// live zero, MSVC gives the two `field_78 = 0` stores in the FUN_004ab510 arms a
// register zero (edi), which kills p.y and forces `mov edi, [esp+0x34]` before
// `cmp edi, [esp+0x24]`; the original keeps p.y in edi and stores immediates.
// Retry #1758: GPT-6.1-sol confirmed 80.9% after four checks; no MATCH. The focus==index block still changes zero rematerialization, register allocation and branch layout.
// GPT-6.1-sol refinement: six checks found no improvement. A local zero copied
// from field_78 or derived as focus+1 folds to the same immediate; focus-index
// and off-old produce larger, worse code (49.0% and 51.8%). An int* alias for
// field_78 is byte-identical to baseline at 80.9%. Keep the 80.9% source below.
// GPT-6 retry: retained 80.9%. Full-width/partial-width zero value variants
// did not recover the original edx zero; detailed previous notes remain below.
// deepseek-v4.1-flash retry: no change to the code, still 80.9% and NOT a match.
// New facts about the remaining diff, so the next attempt does not repeat them:
// an N-declaration sweep (0 to 400 unused `extern int`) leaves the score flat
// at 80.9%, so this is source shape, not compiler state. Operand-order
// respellings change nothing (`0 == obj->field_78`, `0 > e->off`,
// `0 != obj->holder`, `obj->field_78 == 0` all give byte-identical output).
// A `int zero = 0;` declared at the top of the focus block and used for the
// field_78 store, the `e->off < 0` clamp and the holder test still folds the
// store back to the call result in `eax`, so the zero never survives. `short v`
// in the wheel arm, a per-arm `old`, a single shared `off` with one
// `e->off = off` after the if/else, `e->off--`/`e->off++`, `switch`-style
// conditional stores, an `int f94`, and a `!= 1` spelling of the call test all
// score below 80.9% (between 47% and 74%), several of them by breaking the
// shared store at 0x4a427b. What is still unexplained: the original's
// `xor edx, edx` in BOTH arms of the FUN_004ab5b0 test means the 0 is a phi at
// the merge, i.e. it is live on both paths, yet every literal-zero spelling
// the front end sees is folded or rematerialised. A real variable that is 0 on
// both paths and read after the merge is the only shape left.
// deepseek-v4.1-flash retry (issue 1202): tools/headers.py with all 128 header
// sets (and the file's own structs) is also flat at 80.9%, so the edx zero is
// not a compiler-state or header effect. Rechecked the 0x4a3ef0 lesson by
// declaring `int zero = 0;` right after the FUN_004a23b0 call (not at the top
// of the focus block): still 80.9%, MSVC folds it to `mov [ebp+0x78], eax` and
// `cmp word [ebx+0x140], 0` exactly as before. No change to the code.
// Sonnet 5.5 retry (#1080): no change to the code. /Gz and /Gr give the same 80.9%. The original zero in edx is not reachable with an `int zero = 0` local, a local assigned 0 in both arms, or named locals for the call results, the drag flag, the holder and the offset (about 150 variants): MSVC folds every one back to immediates. In 0x4a3ef0 a zero local that is reassigned LATER (`int lines = 0;` then `lines = ...` in one arm), declared after the last call before the block, did create the zero register, so look for a real variable in this block that starts at 0 and is reassigned on some path.
// Not a match yet, 80.9%. The whole prologue, the entry-address computation,
// the 24-byte point copy, the FUN_004a23b0 call, both FUN_004ab510 tail
// blocks (right-button arm down to `mov [ebp+0x94], dx`) and the whole
// FUN_004ab510/FUN_0049fc50/FUN_004ab690 tail match byte for byte. What is
// left is inside the `obj->focus == index` block, and all of it comes from one
// decision: the original keeps the literal 0 in a register (edx) for the whole
// block, so it spends `xor edx, edx` in both arms of the FUN_004ab5b0 test
// and then uses `cmp [ebp+0x78], edx` / `mov word [ebx+0x140], dx` /
// `cmp eax, edx`. This file lets MSVC rematerialise the 0 as an immediate, so
// it loads `mov eax, [ebp+0x78]; test eax, eax` instead. That costs two
// instructions at the top of the block and then cascades:
//   - edx is free, so MSVC picks `dl` for the flags byte where the original
//     uses `al` (and a memory operand for the same byte in the wheel arm),
//   - the clamp block picks esi/edx where the original picks edi/esi,
//   - in the tail MSVC then needs a zero register, takes edi, kills p.y and
//     reloads it from [esp+0x34] and r1[1] into eax.
// Forcing the constant into a register needs one more literal-0 use in that
// block (an `int zero = 0` local, tried, changes nothing) or one more live
// value in eax; neither was found.
// The one place the original's arithmetic is questionable: `mov ax,
// word [ebp+0x94]; sub ax, word [ebp+0x7c]; add eax, esi` narrows the
// difference to 16 bits and then adds a 32-bit value without sign extension,
// so the high half of the result is whatever eax happened to hold. Reproduced
// here, but it looks like an original bug.
// Shapes that scored worse and are worth knowing about: one merged `v` with a
// single `e->off = v` after the if/else moves `index` into ebx and spills `e`
// (40.8%); a `?:` for the drag arm keeps the shared store but widens the
// subtract to 32 bits (61.6%); duplicating both arms so MSVC cross-jumps the
// tails does not cross-jump at all (49.0%).
// WITHDRAWN, and this is the largest single lesson in the file: the note here
// used to read "declaring the old offset uninitialised and assigning it in each
// arm, or hoisting `f94` out of the drag arm, both cost the shared store (73.5%
// and 73.7%)". The first of those was measured against a body that no longer
// exists. Spelling `old` as a two-arm phi (`int old;` declared bare, `old =
// e->off` in the drag arm and `old = v` in the wheel arm) keeps the single
// shared `e->off = v` store and is worth **+3.8 points, 80.9% to 84.7%, at an
// unchanged 714 bytes**. Nothing about the store changed; the gain is entirely
// register allocation, because giving the phi a frame slot frees EDX as the
// "materialised zero" register and so lets the tail use immediate forms.
// "A rejected sweep entry is valid only for the body it was measured on."

// deepseek-v4.1 retry (issue 2461): no improvement, still 80.9%. Measured fact:
// naming the FUN_004ab5b0 result in an int local before the test
// (`int ok = FUN_004ab5b0(obj, 3); if (ok == 0)`) DOES materialize the wanted
// 32-bit zero in EDX and reproduces the original per-arm `xor edx, edx` plus
// `mov [ebp+0x78], edx`, but it also turns the call test itself into
// `xor edx, edx` / `cmp eax, edx` instead of `test eax, eax`, which cost 5
// bytes and dropped the score to 75.1%, so the two halves looked inseparable.
// THEY ARE NOT SEPARABLE-FOR-ALL, only on this body: see the entry below.
// A `union { int full; short word; }` zero (the 0x4a3ef0 trick) gets a memory
// home here (725 bytes, 73.7%). Re-confirmed dead ends: plain int/short/pointer
// zero locals, phi-shaped zero locals, named-zero comparisons, and reference
// aliases of field_78 all stay byte-identical at 80.9%.
// deepseek-v4.1-flash retry (issue 3392): still 80.9%, three further negatives.
// `int z;` declared once and assigned 0 only inside the if body, then used for the
// field_78 store and the `e->off < z` clamp, is 717 bytes at 73.7% (the missing
// else-path def breaks the join). Spelling the arm-1 store as `obj->focus + 1`
// (focus was just set to -1) is byte-identical to the baseline (80.9%), and so is
// an `unsigned` return type on FUN_004ab5b0 (80.9%).

// Space Bunny Free (issue 4179): 84.7% to **86.5%**, still 714 bytes, 93.0%
// ignoring moved internal jump targets. The whole residual is inside
// `if (obj->focus == index) { ... }`; everything from 0x4a4170 to 0x4a420b and
// everything from 0x4a42cd to 0x4a4437 was already byte-exact and still is.
//
// WHAT THE ORIGINAL WANTS, from the bytes rather than from the diff. It keeps
// ONE constant 0 live in EDX across the entire focus block, materialised once
// per arm of the ab5b0 diamond and used four times:
//   0x4a420d xor edx,edx ; 0x4a421b xor edx,edx
//   0x4a4216 mov dword ptr [ebp+0x78], edx      (the field_78 store)
//   0x4a421d cmp dword ptr [ebp+0x78], edx      (the field_78 test, MEMORY form)
//   0x4a42a0 cmp word ptr [ebx+0x140], dx      (the lower clamp)
//   0x4a42a9 mov word ptr [ebx+0x140], dx
//   0x4a42c2 cmp eax, edx                       (the holder test)
// Because EAX is then free, the drag arm can put the flags byte in AL
// (`mov al, byte ptr [ebx+0x1b]` / `test al, 1`) instead of DL.
//
// WHY `cmp [mem], reg` APPEARS AT ALL, settled from a MATCHed twin rather than
// by sweeping. 0x43bad0 is MATCHed at 100% in this tree and carries the same
// idiom from `while (child != 0) { if (child->flags_6 == 0 || ...) child-
// >flags_6 = 0; }`:
//   0x43bad8 xor ebx,ebx                      ; the loop's literal 0 takes a home
//   0x43bae8 cmp dword ptr [esi+6], ebx      ; flags_6 == 0, MEMORY vs REGISTER
//   0x43bb09 mov dword ptr [esi+6], ebx       ; flags_6 = 0, store from it
// So `cmp [mem], reg` is not something a spelling asks for: C1 prefers
// `mov reg,[mem]; test reg,reg` UNLESS a register already holds the zero, and
// only a zero that is compared against memory AND stored forces one. That is
// why the wheel arm's `mov ax, word [ebx+0x140]` (16-bit, no extension)
// followed by `movsx ecx, ax` is the tell: `v` is an int loaded from a short,
// and `old` is that same value narrowed back to 16 bits.
//
// THE WIN. `int v = e->off; old = (short)v;` in the wheel arm. The cast is a
// no-op on the value and is load-bearing: it makes C1 emit
// `movsx eax, word [ebx+0x140]` (7 bytes, same length as the original's
// `mov ax, ...`) plus `movsx ecx, ax`, instead of a second load and a 2-byte
// `mov ecx, eax`. 84.7% to 86.5%. The same cast with the EDX-zero head gives
// `movsx ecx, ax` AND the whole 0x4a4282-on tail, but lands on 715 bytes and
// difflib realigns on the off-by-one, so it prints 76.0%. **Here a single byte
// of drift costs about ten points**, which is the byte-similarity rule of the
// guide biting: measure the byte count next to the percentage, always.
//
// The intermediate 85.7% is worth recording because the two halves are worth
// nothing apart. Naming the call result (`int ok = FUN_004ab5b0(obj, 3); if
// (!ok)`) is 73.7% on its own and 73.3% alone is the split `int v = e->off;
// old = v;`, yet together they are 85.7% at exactly 714: the naming gives the
// EDX zero, and the split stops C1 folding `v` and `old` into one register.
// Two regressed-looking changes that only pay in combination. This is the
// direct answer to the deepseek-v4.1 note above: the two halves ARE separable,
// just not on the 80.9% body.
//
// MEASURED AND REJECTED, all on the 86.5% body, generator asserted able to
// rebuild the known-good text first (216 + 108 + 126 + 432 + 21 = 903 scored
// variants, plus 7760 permuter candidates and 256 header sets):
// - Every literal-zero spelling is INERT, now for the third time on a third
//   body. Immediate, `int`/`short`/`unsigned`/`const int` local, and a file
//   scope `static const int` are all byte-identical, 6 x 3 field_78 tests x 2
//   clamps x 3 holder tests = 96 distinct texts, 0 occurrences of `xor edx,edx`
//   and 0 of `mov al`. C1 propagates any zero you can NAME back to an
//   immediate. Only a zero compared against memory while also stored gets a
//   register, and that is a control-flow decision, not a spelling.
// - Inverting the field_78 branch (`== 0` with the arms swapped) is 71.5% at
//   717 bytes on every base. The drag arm must fall through.
// - Storing `e->off` inside each wheel arm instead of once after the if/else is
//   72.2% at 738 bytes: C1 does not cross-jump the tails. The single shared
//   `e->off = v;` is right, and its consequence is that our `jle` lands on the
//   store where the original's lands past it. Those 11 jump-target lines are
//   the ones the checker ignores.
// - `short old` 72.6%/713, `short v` 73.8%/716, `unsigned short v` 74.9%/728,
//   `long v` byte-identical, `int f94` byte-identical, `Entry* const e`
//   byte-identical, `old` at function scope byte-identical.
// - Inlining `f94` into the drag arm's arithmetic IS the only spelling that
//   produces the wanted `mov al, byte ptr [ebx+0x1b]` (0 occurrences
//   otherwise), but it costs 13 bytes, 727 total, 74.5%.
// - `p.x`/`p.y` adjust order swapped 84.2%; deleting the `entries` local and
//   re-deriving it as `e - index` 64.5%; `int r1[4], r2[4]` combined, `r2`
//   declared first, and all three legal declaration-group orders are all
//   byte-identical, so DECLARATION ORDER IS INERT on this function, measured
//   once as the guide asks. `int hi = e->field_136 - 1` as a local 74.3%.
// - All 128 header sets score 86.5%, so the include lever is inert here too.
// - Two `permute` runs on two different bodies (5490 candidates on the 84.7%
//   body, 7760 on this one) found nothing.
//
// STILL OPEN, and it is one register-allocation decision: the ab5b0 diamond.
// The original spends `test eax, eax` on the call result AND materialises a
// separate per-arm zero in EDX; every shape found here spends the zero on the
// call test instead (`xor edx, edx` / `cmp eax, edx` / `jne`), because C1 will
// not keep a register zero live across a call, and there is no loop in this
// function to give one a home the way `while (child != 0)` does at 0x43bad0.
// Whoever closes this needs a way to make the constant live in EDX from
// 0x4a420d to 0x4a42c2 without spending it on 0x4a4209; a construct that forces
// a zero into a register BEFORE the call and reloads it after is the obvious
// untried shape.

#pragma pack(push, 1)
struct Entry_004a4170 {                // 0x15b bytes, the table of 0x4a23b0
    char unknown_00[0x13];
    short x1;                          // +0x13
    short y1;                          // +0x15
    char unknown_17[0x1b - 0x17];
    unsigned char flags;               // +0x1b, bit 1 = vertical, 0x10 = dead
    char unknown_1c[0x136 - 0x1c];
    short field_136;                   // +0x136, largest usable offset
    char unknown_138[0x140 - 0x138];
    short off;                         // +0x140, the scroll offset
    char unknown_142[0x144 - 0x142];
    int (__stdcall *cb)(void*, int);   // +0x144, called when off changed
    char unknown_148[0x14a - 0x148];
    int field_14a;                     // +0x14a, cb's second argument
    char unknown_14e[0x157 - 0x14e];
    int field_157;                     // +0x157, entry is being dragged
};
#pragma pack(pop)

struct Holder_004a4170 {
    char unknown_00[4];
    Entry_004a4170* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                      // +0x14, set when off changed
};

struct Point_004a4170 {                // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Object_004a4170 {
    char unknown_00[0x18];
    Holder_004a4170* holder;           // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point_004a4170 point;              // +0x3c, the mouse, table relative
    char unknown_54[0x64 - 0x54];
    int focus;                         // +0x64, -1 when nothing has the focus
    char unknown_68[0x78 - 0x68];
    int field_78;                      // +0x78, non-zero while dragging
    Point_004a4170 saved;              // +0x7c, the mouse when the drag began
    short field_94;                    // +0x94, off when the drag began
};

void __stdcall FUN_0049fc50(Object_004a4170* obj, int index);
void __stdcall FUN_004a23b0(Entry_004a4170* base, int index, int* r1, int* r2);
void __stdcall FUN_004a2580(Object_004a4170* obj, int index);
void __stdcall FUN_004a2be0(Object_004a4170* obj, int index);
int __stdcall FUN_004ab510(Object_004a4170* obj, unsigned char buttons);
int __stdcall FUN_004ab5b0(Object_004a4170* obj, unsigned int mask);
void __stdcall FUN_004ab690(Object_004a4170* obj, int param_2);

// FUNCTION: 0x4a4170
void __stdcall FUN_004a4170(Object_004a4170* obj, int index)
{
    Entry_004a4170* entries = obj->holder->entries;
    Entry_004a4170* e = &entries[index];
    if (e->flags & 0x10)
        return;
    if (e->field_157)
        return;

    Point_004a4170 p = obj->point;
    p.x -= entries->x1;
    p.y -= entries->y1;
    int r1[4];
    int r2[4];
    FUN_004a23b0(entries, index, r1, r2);

    if (obj->focus == index) {
        if (!FUN_004ab5b0(obj, 3)) {
            obj->focus = -1;
            obj->field_78 = 0;
        }
        int old;
        if (obj->field_78) {
            old = e->off;
            short f94 = obj->field_94;
            if (e->flags & 1)
                e->off = f94 - obj->saved.x + p.x;
            else
                e->off = f94 - obj->saved.y + p.y;
        } else {
            int v = e->off;
            // The narrowing cast is a no-op on the value and is here only
            // because it is load-bearing: it is what makes MSVC load the entry
            // offset once and sign-extend it into ECX, which is the original's
            // `mov ax, word [ebx+0x140]` / `movsx ecx, ax` pair.
            old = (short)v;
            if (e->flags & 1) {
                if (p.x < r2[0])
                    v--;
                else if (p.x > r2[2])
                    v++;
            } else {
                if (p.y < r2[1])
                    v--;
                else if (p.y > r2[3])
                    v++;
            }
            e->off = v;
        }
        if (e->off > e->field_136 - 1)
            e->off = e->field_136 - 1;
        if (e->off < 0)
            e->off = 0;
        if (e->off == old)
            return;
        if (obj->holder)
            obj->holder->field_14 = 1;
        FUN_004a2580(obj, index);
        FUN_004a2be0(obj, index);
        if (e->cb)
            e->cb(obj, e->field_14a);
        return;
    }

    if (obj->field_78)
        return;
    if (FUN_004ab510(obj, 1)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 1);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
        return;
    }
    if (FUN_004ab510(obj, 2)) {
        obj->field_78 = 0;
        if (p.x < r1[0] || p.x > r1[2] || p.y < r1[1] || p.y > r1[3])
            return;
        FUN_0049fc50(obj, index);
        FUN_004ab690(obj, 2);
        if (p.x < r2[0] || p.x > r2[2] || p.y < r2[1] || p.y > r2[3])
            return;
        obj->saved = p;
        obj->field_78 = 1;
        obj->field_94 = e->off;
    }
}
