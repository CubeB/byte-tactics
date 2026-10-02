// Decompiled by Space Bunny Free, finished by deepseek-v4.1-flash, finished by LongCat 2.5 Preview Free, verified by GPT-6.1-sol, finished by space-bunny-free, finished by mimo-v2.6-pro. Names are provisional.
// space-bunny-free pass (50 min timebox, 1 real check.py run on the file, 34
// scratch variants scored free through check.py's own compile and compare
// with build/scratch/425480/score.py, which prints the growth branch's
// instructions): the saved 534-byte / 81.1 percent do-while base still stands,
// source unchanged, and nothing beat it. Everything outside the growth branch
// matches instruction for instruction, so the whole residual is the third
// _Ucopy's derived source pointer, and this pass pins down exactly why.
// The frame map, re-derived from the exe's own bytes this pass (the earlier
// passes' reading of [esp+0x24] as a _X slot is wrong, so do not build on it):
// sub esp,8 plus four pushes, so esp = S-24 after the prologue where [S] is the
// return address and the three args are [S+4] _P, [S+8] _M, [S+12] _X. At
// esp = S-24 the two locals are [esp+0x10] (this) and [esp+0x14] (_N), both in
// the dead pushed-register area; [esp+0x1c] is _P and [esp+0x24] is _X. Between
// the operator new call and the following add esp,4 esp is S-28, so there
// [esp+0x20] is _P and `mov [esp+0x24], eax` spills _S into _M's dead argument
// slot, not into a _X slot. The original and this file both do exactly that,
// which is why the first 12 instructions of the branch match.
// What the residual is, precisely: the third copy's source start is the same
// value in both, _P, reached by two different left-leaning sums.
//   original:  sub ecx,edx / add ecx,eax / sub ecx,edi   leaves [_P, _Q, dest, _M4]
//   ours:      lea eax,[ecx+edi] / sub eax,edx / sub eax,esi  leaves [dest, _P, _Q, _M4]
// (ecx holds _P on entry to both.) The leaf order decides everything else: the
// original's leftmost leaf is _P, whose register is dead after the use, so the
// emitter accumulates in place and _P keeps ecx; ours starts at the destination
// induction variable, whose register is busy for the whole loop, so the emitter
// takes a fresh register and _P is left holding the third register of the pool.
// The pool order ecx, esi, edi is the same in both, so that one choice rotates
// every other temporary by one slot: ours assigns the first copy's load temp
// ecx, the fill's _X esi and _P edi, the original assigns _P ecx, that temp esi
// and _X edi. The original's _Last cache in esi survives for the same reason
// its derivation does, and ours has to reload [ebp+8] into edx every pass.
// New measurements this pass, all through the same scorer:
//   * source-first do-while (const_iterator _s declared before iterator _d):
//     524 bytes / 55.2 percent, increments in either order identical; the
//     pre-tested source-first for and while are 531 / 72.4, both the
//     this-in-ebx family the older passes recorded, so dest-first do-while
//     remains the best of the four structures.
//   * `_P` itself as the loop variable (no local copy), do-while 522 / 60.3,
//     for 529 / 59.9, while 529 / 59.9: _P really does land in ecx, as the older
//     passes recorded, but the loop needs no derivation at all (the source IV
//     is _P itself), which loses the original's three instruction chain, and
//     the whole function rotates to this in edi and _M in ebp. So P=ecx and
//     the derivation are not reachable together from this source.
//   * a byte-identical 534 / 81.1 plateau for the third copy's spelling:
//     `_P + 0` and `const _Ty* _s = _P` as the source, `_M + _Q` and
//     `&_Q[_M]` as the destination, the increments in either order, only one
//     of the two pointers incremented in the body, and an extra nested scope.
//   * `const iterator _S` and `const iterator _Q` are also 534 / 81.1, so
//     mutability of the two block locals is not a lever either.
//   * `_Ucopy` with the destination parameter first: 532 / 58.2; with the end
//     parameter first: 526 / 54.4. Permuting _Ufill's parameters does not even
//     compile unless all three call sites move with it.
//   * ONE spelling found this pass that changes the third loop's instruction
//     sequence at all: `_Ucopy(begin(), _P, _S)` instead of `_Ucopy(_First,
//     _P, _S)`, which gives 537 bytes (the original's size) with
//     `lea ecx,[edx+edi] / sub ecx,eax / sub ecx,esi` there, but the rest of
//     the branch falls apart (62.0 percent). Recorded because it is the only
//     sign that the leaf order is not simply pinned for this file; it is not
//     a usable base.
// Best lead for the next attempt: the leaf order of the derived source, which
// no third-copy spelling reaches. The one thing that has never been tried and
// is not a spelling of the loop is making the destination induction variable
// NOT a separate register, so that the emitter's leftmost leaf cannot be the
// busy one: a source written as an explicit affine function of the destination
// (`_P + (_d - _Q - _M)` as the element to copy) rather than as a second
// pointer walked beside it. That was not tried this pass.
// space-bunny-free pass (short timebox, 1 real check.py run, 36 scratch
// variants scored free through check.py's own compile and compare): the saved
// 534-byte / 81.1 percent do-while base still stands, and nothing this pass
// beat it. Source unchanged, so the file holds the best version.
// New evidence, all scored this pass (growth-branch shapes reported as
// P=<reg of the post-new[] _P reload>, fill=<reg of the fill value pointer>):
//   * the first and third copies under two DIFFERENT helper names (_Ucopy_i for
//     the first, _Ucopy for the third, the spelling 0x46cc10.cpp needs to make
//     that file's caller match), helpers with plain `iterator` parameters,
//     `static` helpers, `value_type` in the parameter list, raw `class _Ty *`
//     typedefs, the fill's value parameter moved first, explicit placement new
//     in the third copy, a value local in each copy, a local reference to _X,
//     a local copy of _P, _P as the do-while loop variable, an extra scope
//     around the block and `_d += 1, _s += 1` all leave the P=edi shape and
//     534 bytes / 81.1 percent.
//   * the only spellings that move the reload register at all put it in a
//     register the rest of the function cannot use: `_P` as the do-while loop
//     variable gives P=ecx with the family collapsed to 522 bytes / 60.3
//     percent, `construct(--_d, *_s)` gives P=esi at 521 bytes / 55.3 percent,
//     and the header's own `_Ucopy(_P, _Last, _Q + _M)` (helper or plain
//     `iterator` parameters) gives P=ebx at 546 bytes / 58.0 percent. So every
//     reachable shape pairs the wanted ecx with the wrong allocation
//     elsewhere, which is the same wall the earlier passes recorded.
//   * caching _Last in a local declared before the block is 538 bytes /
//     59.8 percent (family flip again), the pre-tested for and if-around-do
//     forms are 541 bytes / 80.5 percent, so the kept do-while is still the
//     best structure.
// Still differs (unchanged): the whole register assignment of the growth
// branch. Original: _P in ecx from the reload after operator new, first copy's
// temp esi, fill's value pointer edi and count esi, third copy with _Last
// cached in esi, _M*4 in edi, destination in eax and the source derived in
// place into ecx (`sub ecx, edx / add ecx, eax / sub ecx, edi`). Ours: _P in
// edi, first copy's temp ecx, fill's value pointer esi and count ecx, third
// copy with _M*4 in esi, destination in ecx, the source derived into eax
// (`lea eax, [ecx + edi] / sub eax, edx / sub eax, esi`) and _Last reloaded
// into edx every iteration. Everything outside the growth branch matches
// instruction for instruction.
// Best lead for the next attempt: the priority order MSVC 5 gives the first
// copy's load temp and the fill's value pointer over _P at the reload is what
// picks edi. 0x425210's original, which is 99.6 percent matched in this repo,
// reaches the same code with _P in a register reused from a spilled `this`
// (ebx) and the source derived fresh by `lea eax, [ebx + ecx]`; 0x425480's
// original keeps `this` in ebp and derives the source in place. A spelling
// that spills a long-lived value in the growth branch so that _P inherits a
// volatile register is the untried route; so far every added local is removed
// before numbering.
// deepseek-v4.1-flash pass (60 min, 12 real check.py runs, ~180 scratch
// variants scored through check.py's own compile and compare): no new best,
// the 534-byte / 81.1 percent do-while stands. The residual is the register
// the growth branch loads _P into after the operator new call: original
// `mov ecx,[esp+0x20]`, ours `mov edi,[esp+0x20]`; everything else in the
// growth branch follows from that one choice. New measurements:
// - TU-state padding does NOT flip this function. 900 consecutive counts of
//   unused extern ints (0..900) and 100 counts each of function prototypes,
//   class declarations, static ints and empty function definitions (0..400
//   step 40) all leave the load in edi; extern ints 900..6000 and prototypes
//   0..6000 (step 60) also leave it. Unlike 0x425210, where padding moves the
//   shape, this rotation is pinned for this source.
// - the original TU's real sibling instantiations (vector<Class_004c2ea0*>
//   ::_Destroy, vector<unsigned short>::size/_Destroy, forced by member
//   pointers and the Access trick, alone and in exe emission order) do not
//   move it either: a bare unsigned-short instantiation is 534B/81.1 with the
//   load in edi, adding size or _Destroy is 535B/80.9. The real <vector>
//   header instead flips the whole function to the other family (this in
//   ebx), 547B/57.9 percent, so the hand-written class is what holds family B.
// - std::uninitialized_copy and std::copy for the third copy (58.0 and 71.7
//   percent) flip the family like the header's own _Ucopy call does.
// - <windows.h>, <string>, <stdexcept> and <stdio.h> before <memory> are
//   534-535B / 80.9-81.1 percent with the load in edi.
// - hoisting the third copy's destination (_R = _Q + _M before the _Ufill),
//   advancing _Q by _M, all seven manual first-copy spellings (including the
//   seven 534-byte ones) and a manual fill all leave the load in edi at or
//   below 81.1 percent.
// - BT_TOOLCHAIN=msvc5-rtm gives the same 534 bytes / 81.1 percent, so the
//   compiler build is not the lever. 0x4732e0 is byte-identical to 0x425480
//   apart from the two call displacements (6 bytes), so both are the same
//   COMDAT code from the same source and state.
// Best lead for the next attempt: in family B the declaration order of the
// third copy's locals DOES choose the load's register class. Source-first
// (`const_iterator _s = _P;` before `iterator _d = _Q + _M;`, any loop kind)
// puts the load in edx, a volatile register (531B/72.4 percent), while
// destination-first puts it in edi. The original wants ecx with the
// destination-first first copy (bound in ecx, dest in edx), so a spelling
// that keeps the first copy's dest in edx while giving the load ecx is what
// is still missing; a live-range or use-count change for _P that survives
// optimization is the other route (docs/field-notes.md part 4 item 2).
// mimo-v2.6-pro pass (60 min timebox, ~70 scored scratch variants, no score
// change; kept the saved 534-byte / 81.1% do-while). New evidence:
// - Translation-unit state is a live lever here: emitting a second template
//   instantiation in the same file (std::vector<unsigned short>::insert or
//   vector<int>::insert, at any of four positions) flips the third copy's
//   source derivation from `lea eax, [ecx + edi]` to `mov eax, ecx / add eax,
//   edi`, 535 bytes / 80.9%. So the file hash moves at least one byte of this
//   function, but only between the same two buckets.
// - A dead-declaration spray (30 files, 1-12 items each: functions, classes,
//   enums, typedefs, externs, unions, namespaces, templates, virtual classes,
//   at three positions) produced only those two buckets: 534 / 81.1 and
//   535 / 80.9. No variant flipped the register rotation.
// - _Ufill and first _Ucopy spellings move bytes but never the rotation:
//   `0 < _N` to `_N != 0` or a while form is 534 / 80.6 (only jbe to je),
//   manual fill loops collapse the family (532 / 55.5), a manual first copy
//   is 534 / 80.6, post-increment constructs 524 / 52.5, and source-first
//   increments, `_d += _M` splits, end() bounds and _M aliases are all byte
//   identical to the kept form.
// - The exe's two register variants of vector<T>::insert compared side by
//   side: 0x425210's (this in ebx, _M in ebp, _P reloaded into ebx) third
//   loop keeps dest in ecx, source in eax from `lea eax, [ebx + ecx]`,
//   _Last cached in esi and _M in edi; ours is that loop shape with _P in
//   edi (so the lea reads [ecx + edi]) and _Last reloaded into edx each
//   pass. The wanted 0x425480/0x4732e0/0x40d020 variant (this in ebp, _P in
//   ecx merged into the source induction variable, _Last cached in esi) is
//   the other family: every _P-into-ecx spelling flips this back to ebx, so
//   the two cannot be combined from this source. docs/field-notes.md lists
//   0x425480 on the callee-saved register rotation wall and says the exe
//   holds both register variants from different translation units.
// Still differs (unchanged): everything after the operator new call in the
// growth branch. Ours: _P in edi, first copy temp ecx, _Ufill _X esi and
// count ecx, third copy M4 in esi, dest in ecx, source derived in eax and
// _Last reloaded into edx every iteration. Original: _P in ecx, temps and
// fill registers one step around (esi / edi / esi), third copy M4 in edi,
// dest in eax, source merged into ecx (`sub ecx, edx / add ecx, eax /
// sub ecx, edi`), _Last cached in esi.
// Ninth pass (deepseek-v4.1-flash, #3897): a live `const_iterator _P0 = _P;`
// copy used as the third copy's source inside the do-while, and a
// `size_type _N0 = _M;` alias for the _Ufill count, stay byte-identical at
// 534 bytes / 81.1 percent, so the growth branch's _P-into-edi pick (the
// original keeps _P in ecx) is not liveness or value-number steered.
// Moving `_End = _S + _N;` before `_Destroy` regresses to 74.3 percent.
// Best shape unchanged.
// Eighth pass (deepseek-v4.1-flash, 5 scratch scores on top of the saved
// 534-byte / 81.1 percent do-while base): swapping _Ucopy's increments to
// `++_F, ++_P`, swapping _Ufill's to `++_F, --_N`, the pre-tested source-first
// `while (_s != _Last)` form, and sizeof-neutral edits of the growth branch all
// leave the build byte-identical at 534 bytes / 81.1 percent. Rewriting the
// capacity ternary as `(_M < size() ? _M : size())` regresses to 79.3 percent,
// so the ternary polarity is load bearing and the best base stands. Residual
// is unchanged: the head guard, the growth branch's _P (edi here, ecx in the
// original) and the per-iteration `mov edx, [ebp + 8]` _Last reload.
// space-bunny-free pass (900 s, 3 real check runs, all scored variants free
// through --sym): the 534-byte do-while base still stands at 81.1%, 534 of 537
// bytes, and no variant beat it. Everything outside the growth branch matches
// instruction for instruction, so the whole remaining difference is ONE
// allocator decision inside that branch: the original keeps _P in ecx from
// right after the operator new[] call to the third _Ucopy, and every one of
// the loop1 copy temp (esi, not ecx), the _Ufill registers (_X in edi, the
// counter in esi, not esi/ecx) and the third loop's _Last cache (esi, not a
// reload into edx) follows from that single choice. Ours picks edi for _P
// even though ecx is free there (ecx still holds _Last from the prologue until
// the capacity ternary at 0x4254d5, and dies after it).
// Measured this pass, all with the same class and only the third _Ucopy's
// spelling changed (nothing else in the file moves, and these are the only
// three score buckets the whole space produces):
//   for (; _s != _Last; ++_d, ++_s) with _d declared first   541 bytes 80.5%
//   the same with end() instead of _Last                     541 bytes 80.5%
//   the same with the increments written `_d += 1, _s += 1`  541 bytes 80.5%
//   do { ... } while (_s != end())                           534 bytes 81.1% (kept)
//   const_iterator _l = _Last; cached, declared first        546 bytes 58.0%
//   const_iterator _l = _Last; cached, declared last         546 bytes 58.0%
//   _Ucopy(_P, _Last, _Q + _M) helper (inlined)              546 bytes 58.0%
//   _s declared first, for loop, _d in the for-init           531 bytes 72.4%
//   _P itself as the loop variable, for loop                  529 bytes 59.9%
//   do construct(_d++, *_s++) while (_s != _Last)            524 bytes 52.5%
// The pre-tested form is the right STRUCTURE (the original pre-tests, and its
// `sub ecx,edx / add ecx,eax / sub ecx,edi` derives the source start in place
// from _P's own register, which is why its esi cache survives), but reaching it
// needs _P in ecx, and no loop spelling here moves _P out of edi. The
// accessor route of the guide's item 28 (begin()/end() through the growth
// branch) changes nothing: end() scores exactly what _Last scores, so the
// member reads are not what decides the rotation.
// #2343 retry by GPT-6.1-sol: five checks kept the prior 81.1% best, no MATCH.
// Reverse comparison and split destination initialization made no difference;
// explicit while(1)/break scored 79.7%. Growth branch loop guard, register
// allocation, and _Last reload remain different.
// std::vector<Class_004c2ea0*>::insert(iterator, size_type, const T&), MSVC
// 5's <vector> written out (as 0x425210.cpp does) with _Ucopy, _Ufill, fill
// and copy_backward inlined. 0x4222e0 is the only caller (the push_back).
//
// Partial (for-loop form: 80.5%, 541 of 537 bytes), up from 57.9% with the
// real <vector>.
// What changed: the third _Ucopy of the growth branch is written as a loop in
// the body with the destination declared BEFORE the source,
//   { iterator _d = _Q + _M; const_iterator _s = _P; for (; _s != _Last; ...) }
// instead of the header's inlined _Ucopy(_P, _Last, _Q + _M). That flips the
// whole register assignment of the function to the original's (this in ebp,
// count in ebx), so the "known wall" of the this/count swap is not a wall: it
// follows the declaration order of the inlined copy's destination and source
// (helper _Ucopy(dest, src, end) gives the same flip, 71.6%; src before dest
// gives this in ebx). The same lever moves 0x4732e0 and 0x40d020, which have
// the same swap, and does nothing for 0x425210 (see that file).
//
// Still different: (1) the loop bound. The original caches _Last in a register
// (`mov esi, [ebp+8]; cmp ecx, esi`); here the loop re-reads `mov edx,
// [ebp+8]` each pass (+4 bytes) because _Last is compared straight from the
// member. Every way of copying it into a local (`const_iterator _e = _Last;`
// before, inside or after the block, in any declaration order with _d and _s,
// for-init, const, or as a helper parameter in any position) puts the
// registers back to this-in-ebx (58.0%) or drops the unfolded source
// (71.6%). (2) In the growth branch the original keeps P in ecx and spills S to
// [esp+0x24]; ours keeps P in esi/edi. (3) The third loop's source start is
// `sub ecx, edx; add ecx, eax; sub ecx, edi` in the original, `lea eax,
// [ecx+edi]; sub eax, edx; sub eax, esi` here. The fast branches (second and
// third) already match instruction for instruction.
// Tried without effect: dead locals of every kind in front of the loop (they
// are removed before numbering), 1200 random placements of dead copies, the
// four _Ucopy sites in all 256 combinations of inline/helper/manual loop,
// _Ufill and _Destroy with permuted parameters, for/while/count loops, and
// dozens of dead declarations before the class.
// deepseek-v4.1-flash pass: rewrote the growth branch's third _Ucopy as a
// do-while over the destination-first locals (was a for loop). That drops the
// redundant `mov esi,[ebp+8]` and the pre-loop `cmp/je`, so ours is 534 bytes
// against the original's 537 (81.1%, up from 80.5%). The matched-instruction
// count is unchanged (107 context lines in both diffs); the gain is only three
// fewer extra instructions. The original's loop is pre-tested with _Last
// cached in esi, so the faithful for-loop form (80.5%) remains the better
// structural start and is what the notes above this block describe. Those
// findings were measured by Space Bunny Free; I confirmed the first _Ucopy
// spelling has no effect and that no header set (headers.py, 128 sets) moves
// the score.
// LongCat 2.5 Preview Free pass: re-tested the two open questions in this
// (do-while, dest-first) base. The result stands at 534 bytes / 81.1%; 30+
// scored variants this pass, none better:
// - every pre-tested form (for/while/if-around-do-while, increments in either
//   order, _d/_s in for-inits, explicit `if (_s != _Last) do..while`) folds
//   to the same 541-byte shape (80.5%): it emits the +7 byte pre-test
//   (`mov <r>,[ebp+8]; cmp <rP>,<r>; je`) but then the source derivation
//   (`lea eax,[ecx+edi]; sub eax,edx; sub eax,esi`) lands in eax, the same
//   register the _Last cache was loaded into, so the loop-end test must
//   reload `mov edx,[ebp+8]` (+3) and the init is 1 byte larger: 45 vs the
//   original's 41. The original keeps _Last in esi, M4 in edi, dest in eax
//   and builds the source from _P itself (ecx, merged as the induction var,
//   `sub ecx,edx; add ecx,eax; sub ecx,edi`), so its cache survives.
// - the _Last-cache local (const_iterator _l = _Last, in all declaration
//   orders, do-while and for) re-tested in this base: still reverts the
//   whole allocation (this to ebx, count to ebp, _P to ebx, 538-546 bytes at
//   58-60%). The do-while + _l shape is 538 bytes (one over the original)
//   with the cache surviving in edi, which proves the cache-clobber in our
//   form is solely the register rotation, not the loop shape itself.
// - `register`, manual first-copy and fill loops, &_Q[_M] and _M+_Q
//   spellings, _P as the loop variable itself, hoisted _d/_s at function
//   scope, the destination-first helper _Ucopy(_Q+_M, _P, _Last), source-
//   first declare order, and post-increment-in-call constructs: all at or
//   below 81.1%.
// So the 3 missing bytes and the register rotation (ours: _P in edi, dest in
// ecx, src in eax, M4 in esi, _Last reloaded; original: _P/src in ecx, dest
// in eax, M4 in edi, _Last cached in esi) are one allocator decision that no
// source lever in this file reaches, the same translation-unit-state wall the
// sibling family records (0x4732e0, 0x40d020, and the one-byte walls of
// 0x425210 / 0x44ec30 / 0x46e640). The 534-byte do-while is the best shape;
// keep it.
// GPT-6.1-sol verified the saved source with check.py: 81.1%, no MATCH.
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
			{ iterator _d = _Q + _M; const_iterator _s = _P; do { allocator.construct(_d, *_s); ++_d; ++_s; } while (_s != _Last); }
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


class Class_004c2ea0 { public: int field_0; };
typedef std::vector<Class_004c2ea0*> V;
typedef void (V::*F)(V::iterator, V::size_type, Class_004c2ea0* const&);
// GPT-6.1-sol final pass: five checker runs total (including baseline and final verification); reverse compare and separate destination increment stayed 81.1%, while (1)/break fell to 79.7%. Best remains the saved 534-byte do-while (81.1%); the head guard, _Last cache and growth-branch register assignment still differ.
// space-bunny-free: source unchanged from the saved base, only this comment
// block is new. Verified with check.py after the edit: 534 bytes, 81.1%, no
// MATCH.
// FUNCTION: 0x425480 ?insert@?$vector@PAVClass_004c2ea0@@V?$allocator@PAVClass_004c2ea0@@@std@@@std@@QAEXPAPAVClass_004c2ea0@@IABQAV3@@Z
F g = &V::insert;
