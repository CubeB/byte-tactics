// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by mimo-v2.6-pro, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// Claude Opus 5.5 (#4919, 2026-10-03): still 99.8%, file unchanged except this
// note. /Gi is not the lever here. With `// FLAGS: /Gi` the first resize's
// erase is no longer inlined (an out-of-line call where the original has the
// byte-copy loop), 1659 bytes, 70.1%. Freeing inline budget in a header clone
// of <vector> (one to three of resize's size() calls written out, end()/begin()
// in resize or erase written out) gets the erase back inline but leaves its
// copy and _Destroy as calls (1679 bytes, 70 to 71%), and in every /Gi build
// the store at 0x4099f6 is still `[ecx + esi]`, so /Gi does not flip this SIB
// either. Neighbours in this TU also fall under /Gi (0x409160 69.3%, 0x409520
// 86.4%, 0x4095d0 92.7%; 0x409470 and the erase 0x40cfb0 still match), even
// though the insert 0x40cca0 after it needs /Gi. Default flags, flat at
// 99.8%: `int i` declared at the top of the function, and with it or without
// it one expansion freed later in the loop (either HasField1ce() call
// written out, `vec_7d.begin()[i]`, `vec_65.begin() + i`,
// `vec_8d.begin()[i] = ...` at the store).
// GPT-6 retry: a protected _First accessor through a vector-derived view leaves
// the store SIB unchanged, so the direct vector subscript remains best.
// DeepSeek V4.1 Flash session: still 1678 bytes and 99.8%, the single store SIB
// byte at 0x4099f6 (want `mov byte ptr [esi + ecx], al`, ours
// `mov byte ptr [ecx + esi], al`). The diagnosis is now firm: this is the
// commutative-register canonicalisation wall of 0x408f30 (0x476210), MSVC 5
// puts the LOWER-numbered of two commutative register operands in the SIB base
// (here ecx=1 over esi=6, and eax=0 over edx in the fixed read), and the
// original translation unit did not sort THIS pair. The reference-to-a-local
// pointer trick (the one that fixes the read) does bypass the sort (vA gives
// the wanted [esi + ecx]) but schedules the pointer load six instructions early
// into the clamp (1675 bytes, 86.8%); a value temp puts the load back in place
// but rotates the allocator (v2: clamp result to ecx, pointer to eax, index to
// edx, 1676 bytes, 83.8%), because the value that was in eax is displaced by
// the pointer. New negatives this pass (all check.py --sym, everything else
// byte-identical unless noted): index made a Lod via a reference to a local
// copy (`int k=i; int& rk=k; vec_8d[rk]`), an `int k[1]` array slot and an
// `int* pk=&k; vec_8d[*pk]` are all neutral at 99.8% (the store keeps base=ecx);
// dead copy-back assignments (`int k=i; i=k;` and the `k=k` self-assign form,
// the 0x461b10 lever) are neutral whether placed just before the store or at
// the loop top; a TU-order sweep with 1 to 8 dummy __stdcall functions before
// the function (the 0x4bc370 per-TU function-order lever) leaves the byte
// swapped; concatenating the real sibling 0x408100 above or below this file
// (with its g_game renamed) also leaves it swapped (99.8% / 99.4%). The value
// temp forms (d20/d21/d22) and reusing a dead slot do not get the clamp into
// eax. Remaining work list is the one byte: 0x4099f6 `[esi + ecx]` vs
// `[ecx + esi]`.
// mimo-v2.6-pro pass (build/scratch/0x409730/): still 1678 bytes, 99.8%, the
// one remaining diff is the store SIB at 0x4099f6 (want `mov byte ptr [esi +
// ecx], al`, ours `[ecx + esi]`). This pass mapped WHY the fix is hard: the
// SIB base slot always goes to the operand that is a MEM node at tree level.
// The read fix (rq8, reference to a pointer) makes the pointer operand a MEM
// load and that is why it flipped; a plain pointer local or the vector
// subscript propagates to a leaf+leaf ADD which the front end canonicalises to
// base=index. Confirmed with three probes: `int& ri = i` at the store flips
// nothing (index MEM node lands in the base, so base=i again, 99.8% neutral),
// `i[rp8]` reversed subscript is byte-identical to `rp8[i]` (the ADD is
// canonicalised before allocation), and a reference bound straight at the
// vector begin slot (`unsigned char*& rp8 = *(unsigned char**)((char*)&vec_8d
// + 4)`) folds back to the plain leaf form (99.8% neutral).
// The two MEM-pointer families are now fully characterised:
// - def after the clamp (temp split with t/b/v8, or the -0 comma
//   `clamp - (p8 = vec_8d.begin(), 0)` in every subscript spelling): the load
//   lands in the right place (right before the store) and the SIB flips to the
//   wanted [esi + ecx] roles (base=pointer), but the register allocation
//   rotates one step (value to ecx/cl, this to esi, i to edx, ptr to eax),
//   1676 bytes, 83.8%. The rotation comes from the MEM node, not the temp:
//   `t = clamp; rp8[i] = t`, `rp8[i] = (t = clamp, p8 = begin(), t)`, the
//   value as `a` or `b`, and the def in the subscript comma all rotate the
//   same way.
// - def before the clamp (statement `p8 = vec_8d.begin(); rp8[i] = clamp`,
//   including the t-slot reuse `(unsigned char*&)t = vec_8d.begin()` which
//   adds no stack slot): the store region allocates exactly like the original
//   (movsx ecx, al; sum in eax; mov byte ptr [esi + ecx], al IS correct) but
//   the def loads schedule between the clamp stages and the tail re-colours
//   (e kept in ebx instead of spilled to [esp+0x1c], the flags word to eax
//   plus a spill where the original keeps it in ebx), 1675 bytes, 86.8%.
//   The tail swap is caused by the pointer MEM node itself: the index MEM
//   probe (int& ri) does not re-colour the tail, the pointer MEM always does.
// Also flat this pass: the unused-prototype decl-count sweep 250-5750 with the
// post-read-fix source (all exactly 99.8% with the same one SIB diff; the
// earlier 0-6000 sweep predates the read fix), and the windows.h min/max
// double-evaluation spelling is not the lever. Last bounded probes: MEM nodes
// on the value side (`int& ra = a` in the clamp) still rotate (83.8%);
// sequencing `a = 1` inside the value comma between clamp and def still
// rotates; `p8 = &vec_8d[0]` and both-refs `rp8[ri]` still rotate; reusing the
// dead t slot for the pointer (`(unsigned char*&)t = vec_8d.begin()`) adds no
// slot but the tail still re-colours (86.8%), so the tail swap tracks the
// pointer MEM node itself and not the frame layout.
// What is left to try next: keep the MEM pointer (only it flips the SIB) but
// undo the allocation rotation, e.g. by finding a MEM-node spelling of the
// pointer whose def is scheduled late like the original (the original loads
// [edx+0x91] after the clamp branches with the value already in eax), or by
// finding what makes MSVC spill e and keep the flags word in ebx when the
// pointer MEM node is present.
// deepseek-v4.1-flash pass: 99.8%, READ SIB FIXED. The read access at 0x409b53
// now matches (movsx eax, byte ptr [eax + edx]) because it is written as a
// block-local pointer plus a reference to that pointer:
//   if (guard) { unsigned char* q8 = vec_8d.begin(); unsigned char*& rq8 = q8;
//                x += (char)rq8[i] / 2; }
// The reference blocks the front end's copy propagation of the pointer into the
// subscript (a plain `q8[i]` is propagated and stays swapped), so the SIB base
// becomes the pointer variable instead of the loop counter, and because the
// definition sits in the if-body block the load `mov eax,[eax+0x91]` keeps its
// original position and the rest of the 1678-byte schedule is untouched. The
// same construct keeps its instruction stream in a 6-line micro (member vector
// at +0x94, store and signed-byte read: `unsigned char*& rp = p; rp[i]` gives
// base=pointer, `p[i]` / `((unsigned char*&)p)[i]` / `(void)&p` give base=index).
// STILL DIFFERS: only the store SIB at 0x4099f6 (want `mov byte ptr [esi + ecx],
// al`, ours `[ecx + esi]`). Every store-side use of the same trick flips that
// SIB but also costs the schedule, because the pointer definition has to be a
// statement before the store: `p8 = vec_8d.begin(); rp8[i] = max(...)` puts the
// pointer load at the end of the previous block (1675 bytes, 86.6%), and
// splitting the clamp into its own statement (`int v8 = max(...); p8 = ...;
// rp8[i] = v8;`, in every scope tried: block, loop body, function scope, with
// the value fed by a comma or by the named temp) re-colours the region
// (movsx edx,al instead of movsx ecx,al, then `this` to esi, `i` to edx, the
// flags word from ebx to eax), 1676 bytes, 83.6%; the double-clamp comma
// `rp8[i] = (max(...), p8 = vec_8d.begin(), (unsigned char)max(...))` folds to
// one clamp but still hoists the pointer load before the clamp branches (1675
// bytes, 86.6%), and `rp8[(p8 = vec_8d.begin(), i)] = ...` re-colours too.
// deepseek-v4.1-flash 10-minute retry (this session): re-confirmed 1678 bytes
// and 99.6%, exactly the two SIB base/index bytes (0x4099f6 wants [esi + ecx],
// 0x409b53 wants [eax + edx]; ours [ecx + esi] and [edx + eax]). One new probe
// this pass: a loop-top `unsigned char* p8 = vec_8d.begin();` used at both byte
// sites (`p8[i] = ...`, `x += (char)p8[i] / 2`) scores 1685 bytes and 51.2%, so
// keeping the pointer local alive for one whole loop body wrecks the schedule
// even harder than the function-scope reference form; the earlier notes below
// still close the store-side and read-side routes.
//
// deepseek-v4.1-flash 10-minute pass (build/scratch/0x409730/v1-v10.cpp), the
// two SIB base/index bytes (0x4099f6 `[esi + ecx]` vs ours `[ecx + esi]`,
// 0x409b53 `[eax + edx]` vs ours `[edx + eax]`) are untouched, 1678 bytes and
// 99.6% every time. New negatives: the address ADD probe, forcing the operand
// order through integer arithmetic, is neutral in all four spellings
// (`(unsigned char*)((int)i + (int)vec_8d.begin())` at the store, at the read,
// at both, and the control `(int)vec_8d.begin() + (int)i`), so the front end
// canonicalises the commutative ADD and the tree order is not reachable from
// the source; `i[vec_8d.begin()]` (store only, read only, both) is likewise
// byte-identical at 99.6%, confirming that the source operand order does not
// reach the SIB. Routing either access through a `static` inline helper on a
// raw pointer (`StoreByte(unsigned char*, int, unsigned char)`,
// `LoadByte(unsigned char*, int)`) costs the inline budget: 1683 bytes and
// 83.9% for the store alone, the read alone and both, exactly like the
// reference-form helpers before, so the parameter node does not survive the
// expansion into a real variable. Together with the earlier store-comma result
// this closes the last two routes to a variable pointer node.
// STILL 99.6% (deepseek-v4.1-flash decomp-worker pass): the same two SIB
// base/index bytes remain (0x4099f6 wants [esi + ecx], 0x409b53 wants
// [eax + edx]; ours [ecx + esi] and [edx + eax]), size 1678 exact. New
// negative results, all scored with check.py --sym (build/scratch/0x409730/):
// temp-first comma pointer at the store (`int v8 = max(...); p8 =
// vec_8d.begin(), p8[i] = (unsigned char)v8`) is propagated back and neutral,
// and so is the clamp-first single-statement comma
// (`v8 = max(...), p8 = vec_8d.begin(), p8[i] = v8`): the store flip needs the
// pointer load sequenced BEFORE the clamp, which is exactly the T9 hoist, so
// the flip and the hoist cannot be separated by reordering the statement.
// A second comma pointer at the read (`q8 = vec_8d.begin(), x += (char)q8[i] /
// 2`, and the value-position form `x += (char)(q8 = vec_8d.begin(), q8[i]) /
// 2`) does not cancel the T9 hoist (combined still 1675 bytes, 86.6%) and is
// neutral alone; the address-comma form `*(p8 = vec_8d.begin(), p8 + i) = ...`
// is neutral. Renumbering the inlined operator[] temps via the neighbouring
// accesses (`Elem_0040cfb0* e = vec_65.begin() + i`, `n = (short)*(vec_7d.begin()
// + i)`, both, and combined with the clamp-first comma) is byte-identical;
// splitting the loop declaration (`int i; for (i = 1; ...)`) and dead
// `int z8 = (char)vec_8d[i]` / `vec_8d.size()` expansions at the sites all tip
// the first resize's inlined copy loop (1683 bytes, 83.9%), so this function's
// inline budget sits exactly on a boundary and dead-code numbering probes are
// not free. No spelling tried flips the read SIB at all.
//
// STILL 99.6% (deepseek-v4.1-flash final timebox pass): the two SIB base/index
// bytes at 0x4099f6 (want [esi + ecx], ours [ecx + esi]) and 0x409b53 (want
// [eax + edx], ours [edx + eax]) are still the only diffs; size 1678 exact.
// New negative results this pass (all scored with check.py --sym,
// build/scratch/0x409730/run.py): store as `*(&vec_8d[i])` neutral; read
// through `unsigned char& r8 = vec_8d[i]` in a block neutral (propagated back,
// contradicting the earlier note that the reference form flips); `int idx8 = i`
// copies at either site neutral; comma pointer `p8 = vec_8d.begin(), x += (char)
// p8[i] / 2` at the READ site byte-identical (neutral), while the same trick at
// the STORE site (T9) DOES flip the store SIB to the original [esi + ecx] but
// hoists `mov edx,[esp+0x20]` / `mov esi,[edx+0x91]` above the clamp block,
// 1675 bytes 86.6%; hoisting `int i` to function top 1683 bytes 83.9% still
// swapped; std::vector<signed char> 98.0% still swapped. So the store flip
// always costs the hoist (pointer def floats early once it is a real variable
// node), and no read-side spelling tried flips 0x409b53 at all.
// deepseek-v4.1-flash 10-minute retry (this session): still exactly the two SIB
// bytes at 0x4099f6 (want [esi + ecx]) and 0x409b53 (want [eax + edx]), 1678
// bytes, 99.6%. One new negative: a value local at the store site
// (`char v8 = (char)max(-100, min(100, a)); vec_8d[i] = v8;`) is 1677 bytes and
// 85.1%, so the store SIB flip still cannot be bought without a real pointer
// node and its early hoist. No spelling tried this pass moved either byte.
//
// char>&,int,unsigned char)` helper used only at the store site stays 1678
// bytes and 99.6% with both bytes still swapped; a static `LoadByte(...)` at
// the read site and an `AtByte(...)` reference helper at both sites both blow
// the inline budget (1683 bytes, 83.9%); `std::vector<char> vec_8d` with the
// read as bare `vec_8d[i] / 2` is 98.0%. So neither a helper call boundary nor
// the element type flips the encoding, consistent with the compiler-state
// conclusion below.
//
// STILL 99.6% (deepseek-v4.1-flash timebox pass): nothing this pass moved the
// two SIB base/index bytes (0x4099f6 wants [esi + ecx], 0x409b53 wants
// [eax + edx]; ours [ecx + esi] and [edx + eax]). What this pass tried, all
// scored with check.py --sym (build/scratch/0x409730/), all 1678 bytes with
// the identical two SIB hunks: extended the unused-prototype decl-count sweep
// into the 3000-6000 range (the 0x4b6c30 note in docs/agent-guide.md says
// 2700-5400 unused prototypes can flip base/index): proto3200 98.0%,
// proto3600 99.6%, proto4000 97.6%, proto4400 97.6%, proto4800 99.6%,
// proto5200 98.0%, proto5600 99.2%, proto6000 97.6%, so decl count only moves
// the score bands and the two SIB bytes never flip in any band; and all six
// permutations of the <windows.h>/<math.h>/<vector> include order (99.6% each,
// so header order is neutral even though header sets are not).
//
// deepseek-v4.1 10-minute pass: eleven more variants, every one of them still
// exactly 99.6% with the identical two SIB diffs (the schedule and the size are
// untouched, 1678 bytes): reversing the source operand order of the address ADD
// at both sites (`i[vec_8d.begin()]`, the store as `*(i + vec_8d.begin())`,
// `(&vec_8d[0])[i]`), `this->vec_8d[i]`, the explicit call `vec_8d.operator[](i)`,
// a cast chain through `void*` at both sites, `(&vec_8d.front())[i]`, the read
// with a non-leaf index `vec_8d[(unsigned char)i]` (1685 bytes, 92.9%,
// adds the movzx, SIBs still swapped), reversing the read's outer add to
// `x = (char)vec_8d[i] / 2 + x;`, and a fresh single-use pointer temporary for
// each site (`unsigned char* p8 = vec_8d.begin(); p8[i] = t2;` in its own block,
// value in a temp first). That last one is the informative one: when the pointer
// temporary is initialized after the value is computed, MSVC propagates the load
// back to the use site and the SIB roles stay swapped, so the store-only flip the
// earlier pass saw came from the pointer load being hoisted above the whole
// block, not from the "variable" node as such. tools/headers.py re-run: all 128
// sets, closest 99.6% (`<windows.h> <math.h>`). The two SIB bytes remain the
// whole work list: 0x4099f6 wants `[esi + ecx]`, 0x409b53 wants `[eax + edx]`.
//
// Additional pass (deepseek-v4.1): the read site rewritten as a braced block with
// `unsigned char* p8 = vec_8d.begin(); x += (char)p8[i] / 2;` recompiles to the
// identical 1678 bytes with the identical two SIB diffs (the single-use local is
// propagated back into the subscript), an explicit `(unsigned char)` cast around
// the clamped store value is neutral at 99.6%, and an `unsigned int` loop index
// is much worse (1683 bytes, 83.9%, the loop guard turns into a 64-bit compare).
// The two SIB base/index bytes remain the whole work list.
//
// (deepseek-v4.1-flash): key negative finding. The two swapped accesses are NOT
// a subscript-form problem and NOT a vector-container problem. A named pointer
// local `unsigned char* q = vec_8d.begin(); q[i]` DOES produce the original's
// base order for both sites, but only when it is the ONLY use in the loop. Adding
// the other site back keeps the original base: store via exact-width pointer on
// plain form like `*(&vec_8d[i])`, and `unsigned char* q = vec_8d.begin(); p[i]`
// on the store form. At the read, `unsigned char& r = vec_8d[i]` (an element
// reference) also yields the original base order. HOWEVER every construct that
// fixes one of the two accesses makes the pointer local survive into the vast
// mid-function region (it spills to a stack slot and its reload shifts a dozen
// unrelated instructions, 1679-1690 bytes, 52-85%), whereas the original has no
// such live pointer there. The free scratch scoring (`check.py --sym`) makes
// this cheap to test: the variants are in build/scratch/0x409730/exp/. Remaining
// diff is still exactly the two SIB base/index bytes.
//
// Still 99.6% (space-bunny-free pass): the code is the same 1678 bytes and every
// instruction matches except these two hunks, both the SIB base/index order of a
// byte access to vec_8d (a different SIB byte, not a different instruction):
//   0x4099f6  orig `mov byte ptr [esi + ecx], al`   ours `mov byte ptr [ecx + esi], al`
//             (the store of the clamped rating: base should be the vector pointer)
//   0x409b53  orig `movsx eax, byte ptr [eax + edx]`  ours `movsx eax, byte ptr [edx + eax]`
//             (the `(char)vec_8d[i] / 2` read: base should be the vector pointer)
// What this pass added (all scored with check.py --sym, all still 99.6% or worse):
// - The trigger is NOT the access form but the surrounding function. A 6-line
//   minimal member reproduces both orders: `v[i]` encodes base=index var, index=
//   pointer, while a NAMED POINTER LOCAL (`unsigned char* q = v.begin(); q[i]`)
//   encodes base=pointer, index=index var, for both the byte store and the byte
//   read feeding a signed /2, with no headers involved. But inserting exactly
//   that local pointer into this function (v1-v3, 9 variants: local in a block,
//   value split into a temp first, `char*` cast, unsigned pointer with and
//   without the `(char)` cast, `&v[0]`, `v.begin()`, the read split into
//   `int c` first) leaves both SIB bytes swapped; only the reference form
//   `unsigned char& r = vec_8d[i]; r = ...` changes the code at all, and it is
//   much worse (1679 bytes, 84.6%). So MSVC5's swap decision here is made on the
//   full expression/register-pressure state, not on the subscript.
// - `unsigned char& r = vec_8d[i]` forces the address into a register first
//   (two extra movs, the pointer kept in ebp) and is never right for a
//   single-use subscript.
// - Second space-bunny-free pass, header set is already optimal: adding
//   <string>, <list>, <map>, <set>, <deque>, <algorithm>, <iostream> or
//   <xstring> (before or after <vector>) scores 99.2 / 98.0 / 97.6 / 99.6, never
//   100, so the missing header is not the cause here. `<memory>` and `<new>`
//   are neutral at 99.6%, so they are also free to add.
// - Ten more access forms, all with the identical two-line diff: a named
//   `unsigned char* p8 = vec_8d.begin()` at the top of the loop body is a
//   disaster (51.0%, and 48.8% with the stored value split into a temp), even
//   used at only ONE of the two sites (the read alone 99.6%, the store alone
//   86.6% because the block reshuffles a dozen unrelated instructions).
//   Neutral at 99.6% with the same diff: `vec_8d[(unsigned)i]`,
//   `*(vec_8d.begin() + i)`, an `int k = i` copy for the store, and reading
//   `(char)vec_8d[i]` into a local first. Worse: an `unsigned char` value temp
//   before the store (85.1%), a `unsigned char& Rating(int)` member used at
//   both sites (83.9%, the accessor changes the inline budget), and making
//   vec_8d a `std::vector<char>` so the read needs no cast (98.0%). Binding
//   the vector itself to a reference inside the loop body, `v8[i]`, is 73.7%.
// - So the swap is decided before the subscript is even formed: the register
//   roles are already identical (pointer and index in the same two registers),
//   the definition order of the two registers is inconsistent between the two
//   sites, and the register NUMBERS are inconsistent too, which rules out both
//   definition order and register number as the rule. It has to be the
//   operand-tree order or some per-function state the front end carries.
// - A named pointer local hoists `mov <ptr>, [this+0x91]` to the top of the block
//   when its initializer is written before the value expression (1675 bytes,
//   86.6%); writing the value into a temp first puts it back in place.
//
// deepseek-v4.1 near-miss pass (all scored with check.py --sym, every one of them
// 99.6% or worse, the two SIB bytes unchanged unless noted): the access FORM is
// irrelevant here. Byte-identical 1678-byte results, i.e. both SIBs still swapped,
// for `i * 1` / `1 * i` as the index, `*&vec_8d[i]`, an inline cast pointer
// `((unsigned char*)vec_8d.begin())[i]`, the pointer-to-array forms
// `(*(unsigned char (*)[1])vec_8d.begin())[i]` and
// `((unsigned char (*)[1])vec_8d.begin())[i][0]`, and a 1-byte-struct element
// (`vec_8d[i].value`, 98.0%, which behaves like the char-vector result). Moving the
// /2 inside the cast, `x += (char)(vec_8d[i] / 2)`, changes the read to
// `mov al, [edx+eax]; movsx edx, al` (1677 bytes, 94.1%): the value tree changes,
// the SIB order does not. A `char` cast on the clamped store value is neutral.
// New finding: a pointer whose tree node is a real VARIABLE (assigned by a comma
// expression in the same statement, `p8 = vec_8d.begin(), p8[i] = ...`) does flip
// the store SIB to the original `[esi + ecx]`, but the extra variable reshuffles
// the schedule (1675 bytes, 86.6%, the pointer load hoists and a register moves
// from eax to ebx), and the read site needs braces plus the same trick for the
// 86.6% copy; there is no variant that flips both bytes while keeping the 1678-byte
// schedule. Remaining work list is unchanged: 0x4099f6 `[esi + ecx]` vs
// `[ecx + esi]` and 0x409b53 `[eax + edx]` vs `[edx + eax]`.
//
// GPT-6 retry: rating access, clamp, half-rating and pointer getter helpers, plus
// all 768 header sets, did not improve 99.6%. Remaining differences are still the
// two SIB base/index encodings; accessor wrappers can disturb STL inline budgeting.
// Recomputes a player's per-unit-type tables (the object built by 0x409160):
// resizes the tables at +0x8d and +0x65 to the unit type count, then for each
// unit type rates it into vec_8d[i] and the three bytes of vec_65[i].
//
// Best so far 99.6%: the code is the same length and every instruction
// matches except the base/index order of two byte accesses to vec_8d:
// the original has `mov [esi + ecx], al` (store) and `movsx eax, byte ptr
// [eax + edx]` (the `(char)vec_8d[i] / 2` read), ours encodes [ecx + esi] and
// [edx + eax]. No header set (tools/headers.py plus <ddraw.h>, <string>,
// <map>, <list> and others), access form (begin()[i], *(begin() + i),
// unsigned index), full class layout, or defining 0x409160 and 0x409470
// above this function in the same file changed it. The weapon sum's
// division order also flips with the number of declarations in the file
// (it goes wrong with the full class layout), so both are probably compiler
// state from the rest of the original file.
//
// Retry (deepseek-v4.1-flash) left both bytes unchanged: headers.py --cpp
// (768 sets) all 99.6%, N unused externs and N prototypes swept wide (0-3000)
// produce only two score bands and the same two SIB lines, and rewriting the
// accesses as begin()[i], *(begin()+i), operator[](i), data(), element struct,
// (signed char) / (int) casts and reference bindings all leave the identical
// two-line diff. A minimal function with a member vector reproduces the
// swapped order only when a byte read feeds a signed /2, so the trigger is in
// the expression's value path, not the access itself.
//
// Things that were needed to get here:
// - MSVC 5's inline budget decides which STL calls stay out of line (the
//   first resize() inlines erase() and its _Destroy, the second calls insert
//   and erase out of line). It only matched with the inline Def methods
//   HasField1ce() and Bonus1ce() below, whose inlined calls use up the budget
//   the way the original's did.
// - The last byte is one windows.h min/max expression; the sum
//   `(float)(f18a * -0.02f) + (bonus ? 25 : 0)` is shared between the macro's
//   repeated evaluations (MSVC spills it to [esp+0x14]), while Bonus1ce()
//   is re-evaluated each time. Without the (float) cast MSVC folds the
//   -0.02 into a subtraction.
// - The two flag bits at +0x241 are read from one local copy of the bitfield
//   word (`mov ecx, ebx; shr ecx, 0xb; test cl, 1`).
//
// space-bunny-free pass (build/scratch/0x409730/): still 1678 bytes and 99.8%,
// and the only diff is the store SIB byte at 0x4099f6 (want
// `mov byte ptr [esi + ecx], al`, ours `[ecx + esi]`); the read SIB at 0x409b53
// is fixed and stayed fixed all pass. 15 hand probes plus a full 15-minute
// permute run (684 candidates, 36 of which did not compile, best.diff empty)
// scored 99.8% or worse, so this file is unchanged from the pass above.
// The store-side MEM pointer and the 1678-byte schedule are now confirmed
// mutually exclusive here, with the cost of each route measured:
// - reference-bound pointer in a block, clamp still in the assignment: the SIB
//   flips to the wanted [esi + ecx] and everything from the store onwards still
//   matches, but `mov edx, [esp + 0x20]` and `mov esi, [edx + 0x91]` schedule
//   six instructions early, between the two halves of the clamp (1675 bytes,
//   86.8%). The same with the char add pulled into the block (86.8%), with a
//   no-op statement between the def and the store (86.8%), or with `*(rp8 + i)`
//   instead of `rp8[i]` (86.8%): all four byte-identical to each other, so the
//   hoist comes from the block, not from the spelling of the access.
// - value split first, then the block: the pointer load now lands exactly where
//   the original has it, right before the store, and the SIB is right, but the
//   allocation rotates (value to ecx/cl, pointer to eax, index to edx),
//   1676 bytes, 83.8%. Narrowing the temp does not bring the value back to al:
//   unsigned char 1672 bytes 82.1%, char 1672 bytes 82.1%, short 1676 bytes
//   83.8%, unsigned char at loop scope 1672 bytes 82.1%, and &vec_8d[0] in
//   place of begin() 1672 bytes 82.1%. So the rotation is the temp, not its
//   width and not its scope.
// - one loop-top pointer with a reference, used at both byte sites: 1685 bytes,
//   51.2%, so the pointer cannot be shared between them.
// - one comma-assigned pointer at the store: 1683 bytes, 84.1%, and it also
//   pushes an extra argument into the second resize's out-of-line insert.
// - a `std::vector<unsigned char>&` block reference: 1681 bytes, 77.2%.
// - `(*this).vec_8d[i]` is byte-identical at 99.8%, so the implicit `this` is
//   not the lever.
// Reading: at 0x409b49/0x409b4f the two operands are both loads from memory
// (a memreg each), so the SIB order there is decided by the front-end operand
// order alone, and the reference-bound pointer at the read is what restores it
// without costing the schedule, because the def sits in the guard's if-body a
// few instructions above its only use. That is the shape the store would need:
// a pointer node whose load is scheduled last. Nothing in the source language
// reaches it, since the pointer has to be a variable and a variable's def is
// always the earliest point of its live range.
// Scratch harness for the next pass, about 15 s a variant:
// build/scratch/0x409730/gen.py, gen2.py and gen3.py build the variants above
// from v0-baseline.cpp, score2.sh scores them with check.py --sym, and the full
// diffs are in the same directory.
#include <windows.h>
#include <math.h>
#include <vector>
struct Unit {
    int unknown_0;
};

struct Elem_0040cfb0 {
    char a;
    char b;
    char c;
};

struct Elem_0040d4f0 {
    char value;
};

struct Elem_0040d550 {
    int unknown_0;
};

struct Point16 {
    short x;
    short y;
};

struct Elem_0040cc40 {
    Point16 pos;                       // +0x0
    float key;                         // +0x4
    Elem_0040cc40() {}
    Elem_0040cc40(const Elem_0040cc40& o) : pos(o.pos), key(o.key) {}
    bool operator<(const Elem_0040cc40& o) const { return key < o.key; }
};

#pragma pack(push, 1)
struct Weapon_00409730 {
    char unknown_0[0xd4];
    unsigned short field_d4;           // +0xd4
    char unknown_d6[0xdc - 0xd6];
    int field_dc;                      // +0xdc
    char unknown_e0[0x10a - 0xe0];
    char field_10a;                    // +0x10a
};

struct Flags241_00409730 {
    unsigned int bits_0 : 6;
    unsigned int flag_6 : 1;           // bit 6
    unsigned int bits_7 : 4;
    unsigned int flag_11 : 1;          // bit 11
    unsigned int bits_12 : 12;
    unsigned int flag_24 : 1;          // bit 24
    unsigned int bits_25 : 7;
};

struct Def_00409730 {
    int HasField1ce() { return field_1ce != 0.0f; }
    int Bonus1ce() { if (field_1ce != 0.0f) return 100; return 0; }
    char unknown_0[0x186];
    float field_186;                   // +0x186
    float field_18a;                   // +0x18a
    char unknown_18e[0x1c0 - 0x18e];
    short field_1c0;                   // +0x1c0
    float field_1c2;                   // +0x1c2
    char unknown_1c6[0x1ce - 0x1c6];
    float field_1ce;                   // +0x1ce
    float field_1d2;                   // +0x1d2
    char unknown_1d6[0x1ee - 0x1d6];
    Weapon_00409730* weapons[3];       // +0x1ee
    char unknown_1fa[0x204 - 0x1fa];
    short field_204;                   // +0x204
    short field_206;                   // +0x206
    char unknown_208[0x22d - 0x208];
    char field_22d;                    // +0x22d
    char unknown_22e[0x241 - 0x22e];
    Flags241_00409730 flags_241;       // +0x241
    unsigned int bits_245_0 : 4;
    unsigned int flag_245_4 : 1;       // bit 4
    unsigned int bits_245_5 : 3;
    unsigned int flag_245_8 : 1;       // bit 8
    unsigned int bits_245_9 : 23;
};

struct Game_00409730 {
    char unknown_0[0x1425f];
    int field_1425f;                   // +0x1425f
    char unknown_14263[0x1434f - 0x14263];
    unsigned short field_1434f;        // +0x1434f
    char unknown_14351[0x1438f - 0x14351];
    int count;                         // +0x1438f
    char unknown_14393[0x1439b - 0x14393];
    Def_00409730* defs;                // +0x1439b
    char unknown_1439f[0x37ec8 - 0x1439f];
    int field_37ec8;                   // +0x37ec8
};

struct Player_00409730 {
    char unknown_0[0x144];
    unsigned short field_144;          // +0x144
};

struct UnitList_00409730 {
    std::vector<Unit*> units;
};

class Class_00409730 {
public:
    Player_00409730* player;           // +0x00
    unsigned char index;               // +0x04
    UnitList_00409730 list_5;          // +0x05
    UnitList_00409730 list_15;         // +0x15
    UnitList_00409730 list_25;         // +0x25
    int pos_35[3];                     // +0x35
    int pos_41[3];                     // +0x41
    std::vector<Elem_0040cc40> vec_4d; // +0x4d
    short centerX;                     // +0x5d
    short centerY;                     // +0x5f
    int field_61;                      // +0x61
    std::vector<Elem_0040cfb0> vec_65; // +0x65
    int field_75;                      // +0x75
    int field_79;                      // +0x79
    std::vector<short> vec_7d;  // +0x7d
    std::vector<unsigned char> vec_8d; // +0x8d
    std::vector<unsigned char> vec_9d; // +0x9d

    void FUN_00409730();
};
#pragma pack(pop)

extern Game_00409730* g_game;

float __stdcall FUN_00488f30(Def_00409730* def);

// deepseek-v4.1-flash timebox pass: two new shapes scored at the two remaining SIB
// bytes, neither helps. A function-scope `std::vector<unsigned char>& v8d = vec_8d;`
// reference local used at both sites keeps the pointer live across the whole function
// and drops to 1679 bytes, 73.7%. An explicit `(signed char)` cast at the read
// (`x += (signed char)vec_8d[i] / 2;`) is byte-identical at 99.6% (1678 bytes), so the
// read SIB is not a cast-spelling artefact either.
// FUNCTION: 0x409730
void Class_00409730::FUN_00409730()
{
    vec_8d.resize(g_game->count, 0);
    {
        Elem_0040cfb0 e;
        e.a = 0;
        e.b = 0;
        e.c = 0;
        vec_65.resize(g_game->count, e);
    }
    for (int i = 1; i < g_game->count; i++) {
        Def_00409730* def = &g_game->defs[(unsigned short)i];
        int a = 1;
        if (def->HasField1ce())
            a = 11;
        if (def->field_22d)
            a += 10;
        if (FUN_00488f30(def) < 0.0f)
            a += 10;
        int t = (int)(a - def->field_18a * -0.01f);
        a = (int)(t - def->field_186 * -0.002f);
        int b = 1;
        if (def->flag_245_4)
            b = 11;
        for (int w = 0; w < 3; w++) {
            Weapon_00409730* wp = def->weapons[w];
            if (wp->field_10a)
                b += wp->field_dc / 100 + wp->field_d4 / 40 + 5;
        }
        a += (char)max(-100, min(100, b));
        vec_8d[i] = max(-100, min(100, a));

        a = 1;
        Elem_0040cfb0* e = &vec_65[i];
        int n = (short)vec_7d[i];
        if (def->flag_245_4)
            a = 21;
        if (def->flags_241.flag_6 && n < 3)
            a += 30;
        if (FUN_00488f30(def) < 0.0f)
            a += 50;
        if (def->HasField1ce())
            a += 50;
        if (def->field_22d)
            a += 25;
        Flags241_00409730 flags = def->flags_241;
        if (flags.flag_11)
            a += 40;
        if (def->field_206)
            a += 15;
        if (def->field_204)
            a += 5;
        int x = (int)(a + min(max(def->field_1c2, 0.0f), 30.0f));
        if (n == 0)
            x *= 4;
        if (n == 1)
            x *= 2;
        if (def->field_1c0 >= 0)
            x *= 3;
        if (player->field_144 > (unsigned short)(g_game->field_1434f / 2)) {
            unsigned char* q8 = vec_8d.begin();
            unsigned char*& rq8 = q8;
            x += (char)rq8[i] / 2;
        }
        if (def->flag_245_8)
            x = 0;
        if (flags.flag_24)
            x = 0;
        if (def->field_1d2 != 0.0f && g_game->field_1425f < g_game->field_37ec8 / 2)
            x = 0;
        x = min(x, 100);
        e->a = x;
        e->c = (char)max(0.0f, min(100.0f, def->field_186 * -0.0025f - FUN_00488f30(def) * 5.0f));
        e->b = (char)max(0.0f, min(100.0f, (float)(def->field_18a * -0.02f) + (def->field_22d ? 25 : 0) + def->Bonus1ce()));
    }
}

// space-bunny-free pass (build/scratch/0x409730/): the file is unchanged from
// the pass below, still 1678 bytes and 99.8%, residual is the single store SIB
// byte at 0x4099f6. New negatives this pass (all check.py, batch of 11 in
// ~60 s): at the store, an array slot for the pointer instead of a reference
// `unsigned char* pbuf[1]; pbuf[0] = vec_8d.begin(); pbuf[0][i] = ...` gives
// 1675 bytes 86.8% (same shape as the reference form), with an `int` value temp
// in front of it 1678 bytes 99.8% (still swapped, and the pbuf array costs no
// frame slot so the size is exact), two chained pointer locals 86.8%, both
// operands as array slots 86.8%, `*(unsigned char**)&p8 = p8` 86.8%, a
// function-scope `pbuf` used in a comma inside the store 1695 bytes 83.3%
// (adds a register-pressure cascade through the second resize's insert).
// Measured for the first time here: the reference form in a nested block (vb)
// does give the wanted `mov byte ptr [esi + ecx], al`, so the block, not the
// spelling, is what puts `mov edx,[esp+0x20]` / `mov esi,[edx+0x91]` two
// instructions early and rotates ebx (flags word) to eax in the tail.

// space-bunny-free pass 2 (build/scratch/0x409730/, 30 variants + a 20-minute
// permuter run from the flipped shape): still 99.8%, 1678 bytes, and the file
// is unchanged. This pass closed the store-SIB search to a single mechanism.
// Everything that gives the wanted `mov byte ptr [esi + ecx], al` is one of
// five pointer shapes, and all five behave identically (1675 bytes, 86.8%,
// two hunks of collateral damage): a reference-bound pointer
// (`unsigned char* p8 = ...; unsigned char*& rp8 = p8; rp8[i] = ...`),
// `unsigned char* pbuf[1]` with `pbuf[0][i]`, two chained pointer locals,
// `*(unsigned char**)&p8 = p8` then `((unsigned char*&)p8)[i]`, and
// `*(rp8 + i)`. The braces are irrelevant: the block and the flat form compile
// to byte-identical objects (vm and vb diffs are the same file), so what
// matters is only that the pointer is a variable. The array-slot and
// cast-reference forms add no frame slot, so the 1675 vs 1678 gap is not a
// stack slot; it is the two missing tail instructions.
// New negatives measured this pass, all 1678-1696 bytes, none flips the byte:
// inline dereference of the begin slot written straight into the subscript
// (`(*(unsigned char**)((char*)&vec_8d + 4))[i]`,
// `(*(unsigned char**)&vec_8d)[1][i]`, `**(unsigned char***)(&vec_8d)[i]`),
// so the front end folds those back to the plain leaf and the MEM-node rule
// does not apply to them; member accessors at the store only
// (`unsigned char*& Begin8() { return *(unsigned char**)&vec_8d; }` 1680 bytes
// 77.2%, `unsigned char* Begin8()` 99.8%, `unsigned char& Byte8(int)` 99.8%,
// and the Byte8 built from a raw `(char*)&vec_8d + 4 + j` 1676 bytes 91.0%),
// and the explicit-`this` spellings (`this->vec_8d[i]`,
// `*(this->vec_8d.begin() + i)`), both neutral at 99.8%.
// The one new combination worth recording: a MEM-node pointer only flips the
// SIB when the clamp is still inline in the assignment. With an `int` value
// temp in front, both the array-slot and the reference form give the exact
// 1678-byte schedule and the byte is swapped again (ve 99.8%, vo 99.8%,
// vn 83.8%): the pointer MEM node and the value temp together rotate the
// allocation one step instead of hoisting the load.
// A self-conditional phi on the pointer (`(a > 1000000) ? begin() : begin()`)
// to force the load to materialise is 74.8%, so it does not reach the store
// either. A 20-minute permuter run started from the SIB-correct shape (the
// reference form, 86.8%) climbed only to 95.9% and best.diff shows it kept
// the same hoisted `mov esi, [edx + 0x91]`, so the flipped shape is not
// reachable from that direction either. Both collateral effects of the flip
// are the same in all five shapes: the pair `mov edx,[esp+0x20]` /
// `mov esi,[edx+0x91]` is emitted two instructions into the previous statement
// instead of at 0x4099e0/0x4099f0, and ebx takes the `&vec_65[i]` temporary so
// the flags word moves from ebx to eax with an extra spill. Anyone picking
// this up: the only untried direction is a pointer whose node is a MEM node
// AND whose load is generated last, which the source language seems to make
// unreachable.
// (Note for the next worker: the vector::insert family warning in the task
// does not apply here, this function is 1678 bytes, not 546.)

// space-bunny-free pass 3 (build/scratch/0x409730/, 45 more variants): the
// file is still unchanged, 1678 bytes and 99.8%, one wrong byte at 0x4099f6.
// The useful new result is the DIAGNOSIS of the coupling, from deleting parts
// of the loop body and reading the SIB byte out of the objects:
//   plain read + plain store   read swapped, store swapped   99.6%
//   trick read + plain store   read right,  store swapped   99.8%  (this file)
//   trick read + trick store   read right,  store right     86.8%, 61 instr diffs
//   NO read at all + plain store  store right
//   phi-indexed read + plain store  store right (1670 bytes)
// So the store's SIB slot is decided by register pressure from the whole loop
// body, and the read-side reference trick is one of the things that tips it.
// The original has both bytes right, so its read must have had less pressure
// than either of ours, which is where the next attempt should go: a read that
// puts the pointer in the base slot WITHOUT a second variable. Every cheap
// variant of the read keeps the exact 1678-byte schedule and the swapped store
// (x = x + (char)q/2, the value in a local, an extra `x += 0`, the /2 inside
// the cast, `const` on the pointer, an extra void use of it), so the read's
// value path is not the lever either.
// The store flip is now fully priced: all pointer shapes that give the wanted
// byte cost exactly the same 61 differing instructions, and it is always the
// same two effects (the `mov edx,[esp+0x20]` / `mov esi,[edx+0x91]` pair two
// instructions early, and ebx taking `&vec_65[i]` so the flags word moves to
// eax). Putting the pointer def in the value expression so it cannot be
// reordered (`((unsigned char*&)t)[i] = ((unsigned char*&)t = begin(), (uchar)clamp)`)
// still gives 1675 bytes 86.8% with that identical collateral, so the load is
// generated at the def whenever the pointer is a variable, whatever the source
// says. Dead locals at the top of the loop (int, char, unsigned char, short,
// float, one to three of them, and an unused pointer local) are all exactly
// neutral at 99.8% with the same single byte, so the frame and the register
// numbers are stable and no decl-count trick can reach this.
