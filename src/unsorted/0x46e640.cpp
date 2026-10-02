// Decompiled by space-bunny-free, deepseek-v4.1-flash and GPT-6.1-sol, finished by space-bunny-free, edited by deepseek-v4.1. Names are provisional.
// space-bunny-free pass (2026-10-02, #4672, tool-assisted): 99.6% re-confirmed
// (546 of 546 bytes, check.py), still only the swapped SIB byte at 0x46e708. The
// best source below is unchanged from main, because nothing I tried moved it.
// What this pass adds is a cheap pre-screener and a map of the space, for the
// siblings stuck on the same byte (0x408f30, 0x425210, 0x44ec30, 0x476210 are
// all std::vector<T>::insert at 99.6% with the same one-byte SIB, so whatever
// fixes this fixes the family).
//   * TOOL (copy this, it is 0.5 s a variant against 7 s for check.py, and it
//     needs no objdump): build/scratch/46e640/fast.py <variant.cpp> compiles
//     with the check.py flags plus /Fa, pulls the ?insert body out of the
//     listing, canonicalises it (hex to h, jump labels to l, symbols and stack
//     slots stripped) and diffs it against ctx.py's instruction list, which
//     build/scratch/46e640/ref.txt holds. It reports the count of differing
//     lines, so "225 instrs 2diff, leaeax,[ecx+ebx] vs leaeax,[ebx+ecx]" is the
//     baseline. batch.py runs a JSON list of variants ({"label", "subs":
//     [[old,new]], "after": marker, "text": filler}) four at a time and prints
//     the same one line each; sweep.py does kinds x counts of TU filler.
//     Beware: cl's listing has CRLF, the symbol line ends in "ENDP" (not
//     "ENDEF"), and gcc-style re.S makes the greedy `.*\n` swallow the file.
//   * The dead-statement levers from the guide DO move the association here,
//     but only into the other (547-byte) shape, never to the wanted SIB: a
//     self-conditional phi (`_P = _P ? _P : _P;`), a dead local
//     (`iterator _Z = _Q;`), a dead store in a folded branch (`int t = 0;
//     if (t) _Q = _P;`) each turn `lea eax,[ecx+ebx]` into
//     `mov eax,ecx; add eax,ebx` (226 instrs), so they push the third copy out
//     of the lea regime altogether. The same is true of one or two units of TU
//     filler; three or more are back to the 546-byte lea.
//   * 840 TU-filler variants (14 kinds: extern/static/dead function/dead
//     function with a loop/typedef/struct with a ctor/global+function/
//     template struct/array-local/double function/branchy function/class with
//     a member function/throw/switch, counts 1 to 60 each) produce exactly two
//     shapes and never the wanted byte: 430 builds are 546 bytes with
//     `lea eax,[ecx+ebx]` and 410 are 547 with `mov eax,ecx; add eax,ebx`.
//     So the filler axis is not just unsampled, it is saturated on this shape.
//   * The 546 form needs a *free* register for the result, which is why it is a
//     lea and not mov+add: the emitter only has a choice of base and index
//     when the destination is a third register. So the byte is decided by the
//     order of the two operands in the IV optimiser's synthesised add, and the
//     dead-statement levers change that add's *shape* rather than its order.
//   * Reading MSVC 5's own VECTOR (toolchain/msvc5-sp3/INCLUDE/VECTOR) settles
//     the source question: its insert(iterator, size_type, const _Ty&) and its
//     _Destroy/_Ucopy/_Ufill are character for character what this clone has,
//     so the clone is the real header and the byte is not a source typo. It
//     also means TA's build used a <vector> whose class state differs from this
//     toolchain's copy, since the real header here compiles to the 547 shape.
//   * A regularity that holds across every build I made, and the most useful
//     thing here for the next person: in EVERY 546-byte three-instruction
//     build, the synthesised add is <basic IV> + <the offset's positive term>,
//     i.e. the SIB base is always the loop's basic induction variable (the
//     destination _Q + _M*4, in ecx) and the index is always the offset term
//     (the _P parameter, in ebx). About 30 further perturbations all agree:
//     the third copy's argument spellings (_M + _Q, &_Q[_M], _P + 0, a one-line
//     Base() helper on either side, a cast on _M); a second _Ucopy helper used
//     only by the third call so that the loop's *init order* is (dest, source,
//     limit) or (dest, limit, source) - both wreck the function (221 instrs,
//     226 diffs), and (source, dest, limit) keeps 546 with the same byte; the
//     while form, ++_F before ++_P, and _L != _F; inline and __forceinline on
//     _Ucopy; the tail statement order; swapping _Ufill with the third copy
//     (which changes the synthesised pair to `lea eax,[ecx+esi]`, still
//     basic-IV first); a second `template class std::vector<int>;` and dead
//     functions after the class instead of before it; a friend declaration.
//     So the offset-first form is a THIRD association that no spelling of this
//     template reaches in this translation-unit state: the reachable set is
//     {basic-IV-first lea, mov+add}. Do not spend more time on spellings.
//   * This is one bug in the shared template, so the fix has to be found once
//     and will fix the siblings. All five are the same three-argument
//     std::vector<T>::insert at 99.6%, 546 of 546 bytes, with the same single
//     swapped SIB byte at the same place in the third _Ucopy: 0x408f30
//     (vector<Unit*>), 0x425210 (vector<short>), 0x44ec30
//     (vector<UElem_0044ec30>, not a free _Ucopy as I first assumed),
//     0x476210 (vector<UElem_00476210>) and this one. The pre-screener in
//     build/scratch/46e640/ is per-function: point it at any of them by
//     changing the PROC symbol it greps for and ref.txt at ctx.py's output.
//   * The framing the older notes above get backwards, which matters: the
//     original is NOT the odd one out. All six sites in the exe with this
//     synthesised-lea shape put the HIGHER-numbered register in the base slot
//     (ebx over ecx), and 0x46e708 is one of them, so the original agrees with
//     the other five. It is our minimal reproduction that always puts the
//     LOWER register in the base, in all four insert siblings at once. So the
//     byte is a property of how little translation unit the class is compiled
//     in, and every one of us has been trying to reproduce a big TU with
//     padding, which does not work because the reachable shape set has only
//     two members (basic-IV-first lea, mov+add).
//   * Total measured this pass: about 3200 builds on twelve axes (filler count
//     1 to 1600 of extern declarations and 1 to 700 each of dead functions and
//     dead functions with a loop, 14 filler kinds x 60 counts, dead statements
//     and phis, argument spellings, one-line helper routing, loop init order,
//     loop forms, comparison and ternary order, inlining hints, member order
//     in the class, #pragma pack 1/2/4/8, a second template class
//     instantiation, other code in the TU that *uses* the class, and 60
//     cumulative/reverse additions of the real header's 61 public members).
//     Plus tools/permute.py on a scratch copy of this file: 13655 candidates,
//     0 compile failures, 99.6% -> 99.6%, empty best.diff. Not one build
//     produced `lea eax,[ebx+ecx]`. Every 225-instruction build is the
//     basic-IV-first lea and every 226-instruction build is the mov+add. The
//     one axis left is a genuinely large translation unit, which padding does
//     not imitate.
//   * Ruled out by measurement, do not retry: the RTM compiler
//     (BT_TOOLCHAIN=msvc5-rtm) emits the same `lea eax,[ecx+ebx]`, so this is
//     not an SP3 patch difference; and the real header included and fully
//     instantiated (`#include <vector>` plus `template class std::vector<int>;`
//     plus the address taken) compiles to 179 instructions against the
//     original's 225, so it is the 547 shape with much more of it.
//   * A warning about the sibprobe rule in build/scratch/sibprobe: its
//     "the SIB base is the first-loaded value" finding is about a *store*
//     (mov byte ptr [a+b+disp],cl) where the emitter picks base and index. This
//     residual is a synthesised lea in a loop, whose operand order is the IV
//     optimiser's term order, and in every build here it is basic-IV-first
//     whatever the load order is. Applying the sibprobe rule here sends you
//     looking for a load to move and there is none to move.
// Best source kept below unchanged.
// space-bunny-free pass (2026-10-02, #4510): 99.6% re-confirmed, 546 of 546
// bytes, the one swapped SIB byte at 0x46e708 is still the only difference, and
// this pass adds the measurements below rather than a fix. Two new facts narrow
// what the byte is, and one closes off a lever the older notes left open.
//   * An exe-wide scan of .text for this shape (build/scratch/46e640/scan2.py:
//     `lea r32,[b+i]; sub r32,x; sub r32,y` with scale 1 and both base and index
//     general registers, no displacement) finds exactly SIX copies in the whole
//     executable: 0x408ff8 (0x408f30), 0x4252d1 (0x425210), 0x44ecf8
//     (0x44ec30), 0x46e708 (this one), 0x475d01 (0x475bd0) and 0x4762f0
//     (0x476210). All six put the HIGHER-numbered register in the base slot
//     (ebx over ecx three times, esi over edx, edi over edx), so the wanted
//     byte is what this compiler always produced for this shape and the
//     [ecx+ebx] order never appears in the original at all. 0x475bd0 is not in
//     the stuck list the guide keeps for this template, so it is the one of the
//     six worth a look next if the SIB is ever cracked.
//   * The lea emitter keeps the tree's operand order, it does not canonicalise
//     by register number, so the order here is the order the loop optimiser
//     built the sum in: our own matching `lea ecx, [edx+edi]` at 0x46e701 (the
//     third copy's destination, written `_Q + _M`) and `lea edx, [esi+edi]` at
//     0x46e79a (`_P + _M`) both carry the source expression's first operand in
//     the base, exactly as written. Note that those two are the opposite rule
//     to the six above (edx 2 over edi 7, esi 6 over edi 7), so no register
//     numbering rule can produce both. What differs between the builds is only
//     which of the two operands of that synthesised sum comes first.
//   * `template class std::vector<int>;` on the clone, which forces every
//     member of the class out of line at once and so changes the state of the
//     translation unit far more than any dummy-declaration padding the older
//     passes swept, is 546 bytes with this same single SIB diff. Together with
//     the padding sweeps already recorded below, that closes the "compiler
//     state" lever for this function: the wanted byte needs a different SOURCE
//     shape, not a differently sized translation unit.
//   * Flat at 546/99.6 with the same diff: _Ucopy with a non-const `iterator`
//     source parameter, the `++_F, ++_P` increment order, `size()` returning
//     `difference_type`, and `insert` declared after the protected helpers in
//     the class (all four leave the class layout and the loop alone).
//   * Regressions, so the statement order of the grow branch is right as
//     written: _Ufill and the third _Ucopy swapped 77.5%, _Destroy and
//     deallocate swapped 93.3%, the three pointer resets moved before them
//     45.8%, `iterator _Q = _S;` with the first _Ucopy's result discarded 65.5%
//     (the real header's `_Tmp` has to come from the return value), a _Ucopy
//     body that reads the source into a local before the store 46.3%, and an
//     extra `allocator.construct(_Q + _M, _X)` 69.8%.
// Best source kept below unchanged.
// space-bunny-free pass (2026-10-01, this run): 99.6% re-confirmed, 546 of 546
// bytes, the one swapped SIB byte at 0x46e708 is still the only difference.
// Four new measurements, all scored with check.py --sym on scratch copies:
//   * The opposite direction from the bisect notes above: ADDING the real
//     <vector>'s missing members to this clone, one at a time (reverse_iterator
//     typedefs, rbegin/rend, all four constructors, the 2-arg insert with its
//     `_Ty()` default, at/operator[]/front/back, push_back, pop_back, resize,
//     reserve, both erases, clear, the comparisons, swap, _Xran and the
//     iterator-range insert), 19 builds: every one is either 546/99.6 with the
//     same single SIB diff or 547/89.6. No member flips the SIB.
//   * All six orders of the three protected helpers (_Destroy, _Ucopy, _Ufill)
//     in the clone: 546/99.6 with the same SIB in all six, so the class member
//     order is not the lever either.
//   * `const iterator _Q`, a split `_Q; _Q = _Ucopy(...)`, the negated
//     `if (!(_End - _Last >= _M))` and nested inner `if` forms, the _Ucopy
//     while form, `_F = _F + 1, _P = _P + 1` increments, dest-first _Ucopy
//     parameters, and `_M + _Q` for the third copy: 99.6%, same SIB, except
//     the two that cost the lea (a destination local `iterator _R = _Q + _M`
//     and a `const _Ty&` temp for `*_F` in _Ucopy), which are 547/89.6.
//   * tools/permute.py 0x46e640 (12 min, 4 jobs, seed default): 6134
//     candidates, 0 compile failures, no improvement at all. best.diff is
//     empty, so the file below is unchanged. It did confirm the wall from the
//     other side: 2261 `swap_commutative` rewrites, 1441 `flip_compare`,
//     1258 `negate_if`, 942 `incdec`, 875 `loop_form`, 864 `include` and 651
//     `compound_assign` all scored equal or worse and none reached MATCH, so
//     the operand order is not reachable by re-associating the source either.
// Codegen probes (build/scratch/0x46e640/w): MSVC 5 emits `add reg,reg`, never
// a two-register `lea`, for every hand-written `p + n`, `(int)p + (int)q` and
// `p + (q - r)` shape at file scope, so the only `lea` with two pointer
// operands in this function is the one the loop optimiser synthesises for the
// third _Ucopy's derived source start, and it cannot be reproduced outside
// this loop. That is consistent with 0x408f30, whose identical wall is the
// same residual: the clone reaches the 546-byte three-instruction form but
// MSVC 5 sorts the commutative pair into the SIB base by register number
// (ecx, 1, before ebx, 3) while the original's own translation unit did not.
// deepseek-v4.1 pass (2409): re-verified 546 bytes / 99.6%, still only the
// swapped SIB byte at 0x46e708 (original lea eax,[ebx+ecx], ours
// lea eax,[ecx+ebx]). New measurements, all scored with check.py --sym on
// scratch copies of this exact file, none reached MATCH:
//   * dense padding scan, extern int declarations, every count from 1 to 100
//     and 300 to 420 (the earlier passes stepped by 8): every build is either
//     546/99.6 (same SIB diff) or 547/89.6. The counts do not fall into two
//     clean regimes as the older note says: 313 to 317 alternate between the
//     shapes one count at a time, so the dependence is hash-like, not
//     monotone, and no count in these ranges gives shape A.
//   * 12 padding kinds (extern int, static int, typedef, struct, enum,
//     function body, extern "C", namespace, class, typedef of vector<int>,
//     global int, mixed) crossed with counts 1 to 32: 384 builds, all either
//     99.6 or 89.6, same single-byte diff. 240 further builds with random
//     mixes of those kinds at random counts and at two insertion points
//     (before namespace std and before the typedef): same result.
//   * windows.h, and stdio/stdlib/string/math/time headers, in five
//     positions each: 99.6, the SIB does not flip. The guide's windows.h
//     SIB note does not apply to this lea.
//   * 42 exotic spellings of the third _Ucopy call (source as _P+0,
//     _Last-(_Last-_P), (_P-_Last)+_Last, _First+(_P-_First), _P+(_Q-_Q),
//     dest as _Q+_M, &_Q[_M], _Q+_M+0, _M+_Q, casts on either argument, the
//     result stored back into _Q, and loop forms with a forced temp):
//     anything that keeps 546 bytes keeps exactly this diff.
//   * class layout knob that does matter: moving _Ufill's definition before
//     _Ucopy's flips the build to 547/89.6 without touching the lea, which
//     confirms shape choice is a whole-TU/class-order hash, not a property
//     of the copy expression.
// So the remaining byte is a compiler-state artifact: shape A is not
// reachable from any source spelling of this function in a TU whose only
// content is this template. Best source kept below unchanged.
// GPT-6.1-sol review: fresh check.py run confirms the best source is 99.6%;
// the sole mismatch is the swapped base/index register order in the LEA at
// 0x46e708. Prior notes below record extensive unsuccessful source-level probes.
// std::vector<int>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, with _Ucopy, _Ufill, _Destroy, fill and copy_backward all
// inlined. Its one caller, 0x46d6c0, walks the 0x5c-byte entries of a
// Class_0046e000 list and pushes, for each of the entry's two std::vector<int>
// members (at +0x4 and +0x14, so _First at entry+8 and +0x18), the value of
// a packet field at +0x6 or +0xa, the count 1, and the member's _Last.
//
// The template is reproduced here rather than included from <vector>, because
// the codegen of the third inlined _Ucopy in the grow path depends on the
// rest of the class and on the include set: the real header (which also
// instantiates rbegin/rend, hence reverse_iterator) emits 547 bytes and a
// four-instruction source pointer, this one emits 546 bytes and the lea the
// original has. Still one byte differs: at 0x46e708 the original has
// `lea eax, [ebx + ecx]` (the source pointer built from the source pointer
// first) and this one has the same lea with the two registers swapped, so
// MSVC 5 built the loop's source start value in the order (dest, source)
// where the original built it (source, dest). Spelling the source start any
// other way in the source, moving a statement, changing the loop's increment
// order, and adding a live local (which demotes this from ebx to ebp) all
// move it further away.
//
// What the byte is, and what is left to try. The third _Ucopy copies
// [_P, _Last) to _Q + _M. MSVC 5 makes the destination the loop's basic
// induction variable (its start is the one-instruction lea ecx,[edx+edi])
// and re-derives the source from it, so the source start is the linear form
// `_P + destIV - _S - _M*4`, emitted as lea, sub, sub. Only the order of the
// two terms in that sum is in question, and the sum is built by the loop
// optimiser, not by the source: everything below was measured and none of it
// moves the byte.
//   * The grow branch on its own, as a free function over the same class,
//     reproduces `lea eax,[ecx+ebx]` exactly (build/scratch/0x46e640/micro),
//     so the choice is made inside this loop and not by the branches, the
//     spill of `this` or anything else in the function.
//   * Spelling: _Q + _M as _M + _Q, &_Q[_M], (_Q + _M), _Q + _M*1,
//     _Q + (int)_M, _Q + (long)_M; _P as _P + 0, *(&_P), &_P[0], (int*)_P,
//     &_Last[0]; a local for either start (that kills the lea and costs a
//     byte, 547); a static __inline accessor for either (no effect at all).
//   * The loop: ++_P,++_F either way round, _P += 1, increments in the body,
//     a while form, _F != _L either way round, _F < _L, a separate IV
//     local, dest-first parameter order, a static _Ucopy, raw int* parameters.
//   * The class: the real header's members bisected one group at a time
//     (build/scratch/0x46e640/vec.cpp), the _Ufill loop's three shapes, and
//     spelling size() once into a local. All 546-byte builds agree on the
//     swapped SIB byte.
//   * Includes: all 128 sets of the seven C headers, all 768 sets of
//     headers.py --cpp, and by hand the real header's own set
//     (<climits> <memory> <stdexcept> <xutility>, 547 bytes) plus
//     <stdexcept>, <climits>, <algorithm>, <xmemory>, <new>, <exception>,
//     <utility>, <typeinfo> and <string> in every position. Nothing matches.
//   * The one lever that does work is global state that has nothing to do
//     with this source: the *number of functions defined in the translation
//     unit* before insert is compiled flips the third _Ucopy between two
//     association shapes. Below one threshold (e.g. one dead static
//     function, or <stdexcept>, or a second std::vector<T> instantiation)
//     it is 547 bytes: `mov eax,ecx; sub eax,edx; add eax,ebx; sub eax,edi`,
//     a different association. Between one and about fourteen units it is
//     546 bytes with the swapped SIB byte, and above fourteen it is 547
//     again. A dead loop-bodied function, fourteen chained inline calls and
//     fourteen nested loops all put it in the 546 regime, so the regimes are
//     countable but the source-anchored one is not among the reachable
//     values. The original's association therefore needs a compiler state
//     this file cannot reproduce, most likely its own big translation unit.
// Claude Sonnet 5.5 pass (#601): the compiler-state probe (N unused `extern int
// dummyK;` lines after the includes, K = 8 to 400 step 8, scored with check.py
// --sym, not committed) is NOT flat here, unlike the other two functions in this
// issue: 546 bytes and 99.6 percent for N = 8 to 56 and again for N = 320 to 400,
// 547 bytes and 89.6 percent for N = 64 to 312. So the third _Ucopy's code does
// depend on how many declarations precede it, in a periodic way, but no N gives
// MATCH (the swapped SIB byte stays). plain headers.py, 128 sets: best 99.6,
// nothing matches, as the notes above say.
// space-bunny-free pass (1875): the 0x4732e0 recipe does NOT reach the SIB
// byte here, and this is now measured, not guessed.
//   * The growth branch's third copy written as an explicit loop in insert,
//     dest declared first, (iterator _D = _Q + _M; const_iterator _C = _P;
//     for (; _C != _Last; ++_D, ++_C) allocator.construct(_D, *_C);), keeps the
//     identical tree (lea ecx,[_Q+_M*4]; lea eax,[dest+_P]; sub; sub; then the
//     two adds and the cmp in the same order) with only the registers renamed,
//     and demotes this from ebx to ebp (_M takes ebx): 541 bytes, 62.8%. So on
//     this function that lever moves the register allocation, not the lea
//     operand order, which is why it took 0x4732e0 from 57.9% to 81.1% and
//     leaves 0x46e640 alone.
//   * A dest-first _Ucopy(iterator _P, const_iterator _F, const_iterator _L)
//     with all four call sites rewritten to match: the same ebx/ebp demotion,
//     526 bytes, 53.0%.
//   * The authentic MSVC 5 STL spellings of the same body (the size_type cast
//     in the first if, begin()/end() in _Ucopy and _Destroy,
//     _Q = _Ucopy(_P, end(), _Q + _M);, deallocate(begin(), end() - begin()))
//     cost 10 bytes, 536, 68.8%: the begin()/end() pair drops two instructions
//     from the grow branch, so the real <vector> header really is not what
//     produced this 546-byte build, and only _First/_Last with a discarded
//     _Ucopy result reproduce it.
#include <memory>
#include <xutility>

namespace std {
template<class _Ty, class _A = allocator<_Ty> >
class vector {
public:
	typedef vector<_Ty, _A> _Myt;
	typedef _A allocator_type;
	typedef _A::size_type size_type;
	typedef _A::difference_type difference_type;
	typedef _A::pointer _Tptr;
	typedef _A::const_pointer _Ctptr;
	typedef _A::reference reference;
	typedef _A::const_reference const_reference;
	typedef _A::value_type value_type;
	typedef _Tptr iterator;
	typedef _Ctptr const_iterator;

	size_type size() const
		{return (_First == 0 ? 0 : _Last - _First); }
	size_type capacity() const
		{return (_First == 0 ? 0 : _End - _First); }
	iterator begin()
		{return (_First); }
	iterator end()
		{return (_Last); }
	void insert(iterator _P, size_type _M, const _Ty& _X)
		{if (_End - _Last < _M)
			{size_type _N = size() + (_M < size() ? size() : _M);
			iterator _S = allocator.allocate(_N, (void *)0);
			iterator _Q = _Ucopy(_First, _P, _S);
			_Ufill(_Q, _M, _X);
			_Ucopy(_P, _Last, _Q + _M);
			_Destroy(_First, _Last);
			allocator.deallocate(_First, _End - _First);
			_End = _S + _N;
			_Last = _S + size() + _M;
			_First = _S; }
		else if (_Last - _P < _M)
			{_Ucopy(_P, _Last, _P + _M);
			_Ufill(_Last, _M - (_Last - _P), _X);
			fill(_P, _Last, _X);
			_Last += _M; }
		else if (0 < _M)
			{_Ucopy(_Last - _M, _Last, _Last);
			copy_backward(_P, _Last - _M, _Last);
			fill(_P, _P + _M, _X);
			_Last += _M; }}
protected:
	void _Destroy(iterator _F, iterator _L)
		{for (; _F != _L; ++_F)
			allocator.destroy(_F); }
	iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P)
		{for (; _F != _L; ++_P, ++_F)
			allocator.construct(_P, *_F);
		return (_P); }
	void _Ufill(iterator _F, size_type _N, const _Ty& _X)
		{for (; 0 < _N; --_N, ++_F)
			allocator.construct(_F, _X); }
	_A allocator;
	iterator _First, _Last, _End;
};
}

typedef std::vector<int> Vec_0046e640;
typedef void (Vec_0046e640::*InsertFn_0046e640)(
    Vec_0046e640::iterator, Vec_0046e640::size_type, int const&);

// FUNCTION: 0x46e640 ?insert@?$vector@HV?$allocator@H@std@@@std@@QAEXPAHIABH@Z
InsertFn_0046e640 g_insert_0046e640 = &Vec_0046e640::insert;
