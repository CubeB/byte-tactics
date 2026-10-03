// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by deepseek-v4.1-flash, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by Space Bunny Free, finished by Claude Opus 5.5. Names are provisional.
//
// Claude Opus 5.5 pass (#5015): 78.9% RETAINED under the checker's flags,
// body unchanged, BUT THE ORIGINAL SOURCE IS FOUND: IT MATCHES WITH /Gi.
// The plain real header plus one ordinary use of the vector's operator= is
// byte-identical, references included, when compiled with /Gi (incremental
// compilation) added to the flags:
//     uv run tools/check.py 0x476490 <file> --flags "/O2 /Ob2 /MT /Gz /Gi"
//   prints MATCH (632 of 632 bytes) for
//     #include <vector>
//     struct Elem_00476490 { int dwords[8]; };
//     typedef std::vector<Elem_00476490> Vec_00476490;
//     void __stdcall Assign_00476490(Vec_00476490* to, const Vec_00476490* from)
//     { *to = *from; }
//     (the InsertFn_00476490 typedef and the annotated line at the end of
//     this file, unchanged)
// A `resize(n)` use instead of operator= also MATCHes; reserve, copy ctor,
// erase, destructor or no other use give 0x476210's family (60.8%). Under the
// default flags that file is 60.7%, below this file, so it is not committed;
// switch to it if the orchestrator adopts /Gi for this TU. It also settles
// the long-running puzzle below: this function and 0x476210 are the stock
// <vector> insert on two identically laid out 32-byte records
// (Record_004750b0 in 0x4751c0.cpp, Record_00474cd0 in 0x474df0.cpp), and
// 0x476210 MATCHes under /Gi with a reserve() use (which its caller 0x474df0
// really makes). So the deallocate-before-_Destroy order and the hand-written
// destroy loop below are a workaround that lands between the two families,
// not the original text. See 0x475ef0.cpp for the mechanism and for the
// other inserts of this TU that the same recipe matches.
//
// Space Bunny Free pass (#4896, 2026-10-03): still 78.9 percent, 646 of 632
// bytes, and this time the sweep was cheap: build/scratch/476490/h.py and
// sweep.py drive check.py's own compile/compare machinery, so a variant costs
// about half a second and roughly 830 variants were scored. Everything below
// re-measured flat, so the older notes stand:
//   the N-unused-declaration test at STEP 1 for N = 0..600 (the older passes
//     only did step 8): 53 values give 646 bytes, 548 give 648, none better;
//   tools/headers.py over all 256 header sets, and again with --cpp over
//     1536 (each C++ header crossed with each set): flat at 78.9;
//   the real <vector> with <windows.h>, <stdexcept>, <memory>, <xmemory>,
//     <xstring>, <string>, <iostream>, <algorithm> on top: 60.7 percent,
//     637 bytes, the this-in-EDI family, every set;
//   a clone of the header's own class body (lines 16-246 of INCLUDE/VECTOR,
//     _Xran dropped, this file's reordered insert): 78.8 percent, 648 bytes;
//   14 element types (char[32], 8 ints, float[8], double[4], short[16],
//     unsigned short[16], a union, nested structs, a base class): all 78.9,
//     the ones that are not 32 bytes or not trivially copyable fall away;
//   10 _Ucopy, 5 _Ufill and 7 _Destroy loop shapes, every combination of the
//     three hand-written in-place copies, _Ufill with the count first,
//     _Destroy with (last, first), insert defined out of class with and
//     without inline, the allocator pointer typedefs, the six tail store
//     orders, the _N/_End/_Last/deallocate expression spellings, named copies
//     of _P/_M/_X, _Q + _M vs _M + _Q vs &_Q[_M], a caller that resizes, an
//     explicit instantiation of this or of vector<int>, a public wrapper with
//     its address taken, and 1..11 dummy declarations: all flat at 78.9.
// New measurement worth keeping: the register the third _Ucopy reloads _P
// into DOES move with the class body (with the header's full body it is
// `mov esi,[esp+0x20]`, with the minimal clone `mov eax,[esp+0x20]`), so the
// reload's register is not fixed either, but no class body puts it in EDX or
// hoists it out of the post-call block.
// The families in the exe, for whoever reads this next: this function keeps
// `this` in ECX (never moved to a callee-saved register, and the four pushes
// are interleaved with the head's arithmetic), which is the family this clone
// already reaches. Every other insert moves `this` into EDI/ESI/EBX/EBP and
// reloads _P into that same register right after the allocation call: that is
// 0x476210 (matched to 99.6 by its own clone, this family is 78.8 with the
// real header), 0x433b20 and 0x4dd8c0 (both MATCHED, and both reproduce with
// the real header), 0x4758c0, 0x475bd0, 0x475ef0. So the OTHER family is
// reachable from source and this one is not, which is the same conclusion the
// twin test reaches (this shape is 632 bytes and occurs once in the exe).
// Two permuter runs (25 min seed 4242: 3997 candidates; 20 min seed 99) both
// left the score at 1125.
//
// DeepSeek V4.1 Flash pass (#4851): 78.9 percent RETAINED, 646 of 632 bytes.
// No variant beat it in ~200 check.py runs, and tools/permute.py ran 2730
// mutations for 3 minutes without leaving 78.9 (score 1125 -> 1125). Re-tested
// and confirmed flat: every _Ucopy/_Ufill loop shape and increment order (66
// combinations), the _Ufill postfix/local/do-while forms, the third-copy
// source as a pointer difference, the three tail-store orders, cast and
// accessor spellings, _P/_M/_X local copies, and data-member padding. The real
// <vector> header body (637 bytes, instr count 246 vs 245) is confirmed at
// 60.7 percent, this->EDI family, and headers.py crossed with --cpp over all
// 1536 header sets never moves it. Everything still hinges on the one decision
// the notes below name: _P in EDX (with `this` in the ECX family) versus _P
// rematerialised from [esp+0x20] (this build). The clone's 4 extra instructions
// are exactly the two prefix-loop `mov ecx,[esp+0x20]` reloads and the two tail
// `mov [esp+0x20],...` spills.
//
// TWIN TEST, Space Bunny Free pass (#4147): CLOSED, the shape is unreachable.
// The guide's twin test has four outcomes and this is outcome (1). All 29
// `insert@?$vector` instantiations the linker names in TotalA.exe are, by size:
//   matched  449 467 547 547 622 622 649 649 649 755 773 785
//   partial  477 532 537 537 544 546 546 546 632 636 639 779 781 791 794
//            798 936
// 632 occurs exactly ONCE in the whole exe: this function. Only two functions
// of any kind are 632 bytes and the other one (0x45ead0,
// `?FUN_0045ead0@@YGXPAUGadget_0045ead0@@@Z`) is an unrelated free function.
// Masking every branch and call displacement, this original has NO
// byte-identical twin anywhere in .text (0 hits in 1,026,560 bytes). So no
// matched compilation of this shape exists to copy and the residual is not
// reachable, the same verdict 0x46e640 got for its 546-byte shape.
//   build/scratch/476490/twin.py does the census and the byte search.
//
// DIFFERENTIAL TWIN, the useful part of this pass. Compiling this clone for
// 0x433b20's element type (Elem_00434020, two unsigned shorts, a 4-byte
// trivial POD, MATCHED in the exe at 547 bytes) scores 86.3% against that
// original, and the real toolchain header scores byte-exact. So the clone's
// `_Ucopy` does NOT linearise the way the real <vector> does, and it is the
// same two defects this file's own residual has:
//   third copy  original `mov eax,ecx / sub eax,edx / add eax,ebx / sub eax,edi`
//               clone    `lea eax,[ecx+ebx] / sub eax,edx / sub eax,edi`
//   tail        the original has the dead pre-delete spill `mov [esp+0x28],eax`
//               and keeps size()'s load in the taken branch; the clone spills
//               _First and _Last into the dead _P slot and hoists the load.
// That corrects an older note in this file, which rejected the real header
// because "with <stdexcept> MSVC 5 builds the third copy's source pointer as
// mov eax,edx / sub eax,ebx / add eax,edi / sub eax,ecx where the original
// wants the single-instruction form". The original does NOT want the
// single-instruction form. In BOTH 0x476490 (0x47656e: `lea eax,[ebx+ecx]`
// then `sub edx,ebx / add edx,eax / sub edx,ecx`) and 0x433b20 the wanted
// form is the four-instruction `mov/sub/add/sub` one, and the header produces
// it. The older note had the wanted shape backwards.
//
// The two spellings are however mutually exclusive with the register family,
// which is why no rewrite of this file has reached it:
//   real <vector>, header order (`_Destroy` then deallocate): 246 instructions,
//     the original's exact count, `_P` held in a register through the whole
//     reallocating branch, the four-step third copy, and the dead pre-delete
//     spill, but `this` gets callee-saved EDI (`mov edi,ecx` after the four
//     pushes) where the original leaves `this` in ECX and spills it to
//     [esp+0x10]. check.py 60.7%.
//   this clone, deallocate before `_Destroy`: the right ECX family, but `_P`
//     is spilled and reloaded, and the third copy is the `lea` form.
// The `_Destroy`/deallocate ORDER alone decides which family a 32-byte element
// gets: all 7 orderings of {destroy-call, destroy-loop, deallocate, the three
// tail stores} measured, best of the alternatives 95 differing lines against
// this file's 67. The header's whole member set is not the lever: a clone built
// from the toolchain's own vector<T,A> class body (lines 14-247 of
// INCLUDE/VECTOR, `build/scratch/476490/mkclone.sh`) with this file's insert
// body scores 78.8%, 648 bytes with check.py, so the extra ~40 members and the
// missing <stdexcept> change nothing here; they only change the 4-byte twin's
// third copy, which is why the real header fixes 0x433b20 and not this.
//
// Measured this pass, all inert or worse (instruction-diff count against the
// original, this file's own build is 67 differing lines of 249; build/scratch/
// 476490/h.py, 0.5 s per compile against check.py's 60 s):
//   all 120 declaration orders of the five member functions: 67 differing
//     lines and 249 instructions every single one, so declaration order is
//     inert here (as on 0x448c70, 0x4b5070 and 0x459c70).
//   +<windows.h>, <stdexcept>, <map>, <list>, <xstring>, <iostream>, <string>,
//   +<iterator>, <new>, <memory.h>, <xmemory>, <stdlib.h>, <assert.h>,
//   +<time.h>, <math.h>, and #pragma pack(push,8) around the class: 67 every
//     one, no effect at all on the codegen.
//   element as a union, char[32], eight separate ints, float[8], with a default
//     constructor, with an empty copy constructor: 67 (the empty copy
//     constructor drops to 188).
//   named `iterator _R = _Q + _M`, `const T& _Xr = _X`, a local `_Y = _X`,
//     a `const_iterator _p = _P` alias used as both the prefix bound and the
//     suffix source, `(size_type)` and `(iterator)` casts, `allocate(_N, 0)`,
//     `_S + (_N)`, `_S + _M + size()`, `_S + (size() + _M)`, a pointer to the
//     local _M, the `_Ufill` body as `++_F,--_N`, as a while and as a do/while,
//     the fill and the third copy written out by hand: 67.
//   the `_Ucopy(_First,_P,_S)` loop with no cached end: 74. The header's
//     `_Destroy(_First,_Last)` member call after the deallocate: 74.
// So the 14-byte overshoot really is the one register-allocation decision the
// older notes named, and no source text reaches it.
//
// deepseek-v4.1-flash (#3287): the real-<vector> recipe of the matched sibling
// 0x43c3a0 (explicit instantiation, 32-byte trivial element) scores 60.7%,
// 637 bytes, against this clone's 78.9%, 646; the clone stays the best form.
// deepseek-v4.1-flash retry (10 min): retained 78.9 percent, 646/632 bytes.
// tools/headers.py over all 128 header sets is flat at 78.9, and the 0..400
// extern-int dummy-declaration sweep (step 8) never beats 78.9 either, so the
// residual is compiler-state, not headers or symbol count. No
// deepseek-v4.1-flash (#3665 retry, 10 min): still 78.9 percent, 646 of 632
// bytes. Tried with check.py: adding each of <stdexcept>, <string>, <xstring>,
// <iostream> and <iterator> after the includes (all 78.8 percent, 648 bytes, so
// the original's four-instruction third-copy source form is not reachable
// through a header), an extra explicit insert instantiation for int and for a
// 32-byte struct placed before this one (both 78.8, 648), inlining the prefix
// _Ucopy into the _Ufill call (36.2), one declaration for _S and _Q (78.9), a
// "_M + _Q" and an "&_Q[_M]" third-copy destination (78.9), a const_iterator
// alias of _P used as both the prefix bound and the suffix source (78.9), a
// named count copy inside _Ufill and a while-form _Ucopy body (78.9 each), the
// allocator pointer/const_pointer typedefs from the 0x476210 clone (78.9), and
// a cached "_Sz = size()" local (70.8, 616 bytes: it collapses the three
// separate size() expansions, so the original really does expand size() three
// times). The wall is unchanged: the prefix-loop bound lands in ECX with a
// per-turn reload instead of staying in EDX.

// gain from an empty element destructor (neutral at 646), stock MSVC6-style
// _S + (_P - _First) fill/suffix destinations (40.0 percent, 678 bytes), the
// 0x476210 file text with the element type swapped and _Destroy moved after
// the deallocate (75.1 percent, 642 bytes), or the empty-destructor plus
// _Destroy after deallocate (78.9 percent, 646 bytes). The wall is unchanged:
// the allocator keeps _P in the argument slot and reloads it into ecx, while
// the original preloads it into edx after the operator new call.
// deepseek-v4.1 retry: still 78.9%, 646 of 632 bytes. Four more variants, each
// scored with check.py: a const_iterator local copy of _P used as the prefix
// bound and the suffix source, an iterator local copy of _P, passing the fill
// source as `_Xp[0]` through a `const T* _Xp = &_X`, and a `_P - _First`
// distance local. All four compile to exactly the same 646 bytes, so MSVC
// folds the locals away and the EDX-versus-memory decision for _P is taken
// before those spellings matter. Note for the next attempt: the original's
// third copy is the four-instruction `sub edx,ebx / add edx,eax / sub edx,ecx`
// form (with `mov edx,[esp+0x24]` as the pre-load) that the full <vector>
// header produced in the 0x476210 retry, not the `lea` form the clone here
// emits, which is independent evidence that _P really is pre-loaded into EDX
// in the original.
// GPT-6 retry: 78.9%, 646 of 632 bytes; pointer and buffer constness did not change the saved register family or spilled insertion pointer.
// GPT-6.1-sol refinement: verified the 78.9% best with check.py. Moving the
// suffix _Ucopy before _Ufill changes the emitted control-flow layout and drops
// to 33.3% (640 bytes); restored the original order. The remaining allocator
// mismatch is still _P being reloaded from the argument slot instead of kept
// in EDX through the reallocating copies.
// std::vector<Elem_00476490>::insert(Elem_00476490* _P, size_type _M,
// const Elem_00476490& _X), the game's reallocating insert.
//
// NOT MATCHING: 78.9 percent, 646 bytes against 632 (space-bunny-free retry
// for #1190; the 78.9 itself is from the Sonnet 5.5 retry for #1081 below).
//
// Retry finding: writing _Destroy out as an inline loop with the end cached
// (`iterator _e = _Last;`) after the deallocate took 75.1 to 78.9 and made the
// whole tail (delete call, size() recompute, three pointer stores) match except
// the original's dead spill of _First into the _X argument slot. A scripted
// sweep of about 700 more variants (helper parameter orders and loop shapes at
// every _Ucopy/_Ufill site, statement orders, local copies of _P/_M/_X, size
// expression spellings, manual third-copy loops) never moved it further: the
// one remaining cause is that the original keeps _P in edx (loaded right after
// the operator new call, before _First) so the fill counter must be ebp with
// &_X reloaded, while this build reloads _P into ecx after each rep movsd.
// Older notes follow.
//
// The whole 14-byte difference is one allocator decision, and it is worth
// naming precisely: the original loads the insert argument _P into EDX right
// after the operator new call (`mov edx, [esp+0x24]`, before it spills _S) and
// keeps it in EDX to the end of the suffix copy, where the source pointer is
// built in place (`sub edx,ebx / add edx,eax / sub edx,ecx` from _P). This
// build keeps _P in its argument slot, and everything else follows from that:
//   10 bytes  two extra `mov ecx, [esp+0x20]` reloads of _P, one in the
//             prefix copy's preheader and one per turn of its loop,
//   9 bytes   two allocator spills in the tail (`mov [esp+0x20],ecx` and
//             `mov [esp+0x20],eax`, both into the now-dead _P slot) because
//             size() has to land in ECX here and in EAX in the original,
//   -5 bytes  the original's dead `mov [esp+0x2c], eax` (the _First argument
//             of operator delete spilled over the &_X slot), which this build
//             elides.
// Fixing the reload and the two spills without the dead store is the whole job.
//
// What this pass added, all scored for free with check.py --sym, none of it
// moving the number (each is a dead end, do not repeat):
//   - The clone of 0x476210.cpp's file, which is itself 99.6% (one SIB byte),
//     scores 60.8% and 636 bytes here. It is a different register family
//     (`mov edi, ecx` after the four pushes, this in EDI, _P reloaded into EDI
//     each turn), so the two functions really are one source with two
//     allocations, as the guide's "register family" note says. Dropping its
//     default constructor changes nothing (60.8%, 636 bytes), so the family is
//     not decided by the ctor.
//   - Writing the third inlined _Ucopy by hand as a loop with the destination
//     first (the guide's 0x425480 trick) gives 38.8% and 652 bytes, source
//     first 37.9% and 663: for 0x476490 the stock `_Ucopy(_F, _L, _P)` spelling
//     is the right family and the guide's trick is exactly wrong here.
//   - The same with the helper's own parameter order reversed (dest first at
//     all four sites): 36.6%, 649 bytes.
//   - `_Destroy(_First,_Last)` written out as a call before the deallocate is
//     the 0x44ec30/0x46cc10 form and drops to 60.6%; the same call after the
//     deallocate gives 75.1%. Only the inline loop with the cached end
//     `_e = _Last`, after the deallocate, reaches this family at all.
//   - `iterator _p = _P;` used through the whole reallocating branch: 78.9%,
//     646 bytes, identical, so a named local is not what puts _P in a register.
//   - The prefix copy written out by hand, the fill written out by hand, `int`
//     instead of `size_type` for _N, and the three pointer stores reordered:
//     78.8 / 78.9 / 78.9 / 57.0.
//   - Deleting the trivially-destructible _Destroy loop outright is the only
//     way to reach the original's exact 632 bytes here, but it drops to 63.0
//     percent and flips the family (`mov edi, ecx` after the pushes, this in
//     EDI, _N added as `add eax, edx` rather than `lea edi, [edx + eax]`), so
//     the loop is a source-level register-allocation lever whose body the
//     compiler elides; it is not emitted, it is what keeps `this` in ECX.
//     Moving it before the deallocate (the stock header order) gives 60.6.
//
// Previous state: 74.9 percent, 644 bytes against 632. The byte count is 12 too
// high, so the shape is still wrong somewhere, not just a register order.
//
// The class below is a hand-written clone of the primary `std::vector` template
// rather than the real one from <vector>. The template parameter names and the
// default allocator argument are spelled identically, so the member still
// mangles as
//   ?insert@?$vector@UElem_00476490@@V?$allocator@UElem_00476490@@@std@@@std@@
//    @QAEXPAUElem_00476490@@IABU3@@Z
// which check.py confirms. The reason for the clone is that <vector> drags in
// <stdexcept>, and with that header present MSVC 5 builds the third copy's
// source pointer as `mov eax,edx / sub eax,ebx / add eax,edi / sub eax,ecx`
// where the original wants the single-instruction form. Both sibling functions
// in this issue (0x475ef0 and 0x476210) are the same template and the same
// include is the same lever there.
//
// Written as an explicit SPECIALISATION of the real std::vector, which also
// preserves the symbol, this scores 29.6 percent (637 bytes): MSVC gives `this`
// a callee-saved register (`mov edi, ecx` after the four pushes) and loads
// _End and _Last through edi, where the original leaves `this` in ecx, spills it
// to [esp+0x10] and reloads it. That one difference permutes ebx/ebp/esi/edi
// for the whole function. The clone avoids it.
//
// The statement order inside the reallocating branch is deliberate: the
// `allocator.deallocate(_First, _End - _First)` comes BEFORE `_Destroy`, which
// is not the stock <vector> order. That reorder alone was worth 60.7 to 74.9
// percent, because it is what flips the entry shape above. It is
// behaviourally identical here because Elem_00476490 is trivially
// destructible, so _Destroy is a no-op.
//
// The element type is NOT the problem. int[8], char[32], eight separate ints,
// pointer+Vec3+four ints, long long[4], double[4], a nested array, a bitfield,
// and in-class memcpy copy constructor and operator= all compiled to the same
// bytes. That was established against the stock header and is the main reason
// the previous pass's element-type sweep came up empty.
//
// What still differs, all of it inside the reallocating branch:
//   - _P is parked in EDX in the original, from the first instruction after the
//     operator new call to the end of the suffix copy, so the prefix-copy loop
//     never reloads its bound. Here _P is spilled and reloaded, which forces
//     the different frame slots and a different source expression in the third
//     copy.
//   - The _Ufill loop's counter is EBP in the original, with &_X reloaded from
//     [esp+0x28] on each turn. Here &_X is hoisted into ebp and EDX counts.
//   - The third copy's source pointer form (see above).
//   - The epilogue's size() recompute lands in ECX where the original uses EAX.
//   - A dead `mov [esp+0x2c], eax` store that the original does not have.
#include <memory>
#include <algorithm>
#include <string.h>
#include <climits>

struct Elem_00476490 { int dwords[8]; };

namespace std {
template<class T, class A = allocator<T> >
class vector {
public:
    typedef A allocator_type;
    typedef typename A::size_type size_type;
    typedef T* iterator;
    typedef const T* const_iterator;

    void insert(iterator _P, size_type _M, const T& _X)
    {
        if (_End - _Last < _M) {
            size_type _N = size() + (_M < size() ? size() : _M);
            iterator _S = allocator.allocate(_N, (void *)0);
            iterator _Q = _Ucopy(_First, _P, _S);
            _Ufill(_Q, _M, _X);
            _Ucopy(_P, _Last, _Q + _M);
            allocator.deallocate(_First, _End - _First);
            {
                iterator _d = _First;
                iterator _e = _Last;
                for (; _d != _e; ++_d)
                    allocator.destroy(_d);
            }
            _End = _S + _N;
            _Last = _S + size() + _M;
            _First = _S;
        } else if (_Last - _P < _M) {
            _Ucopy(_P, _Last, _P + _M);
            _Ufill(_Last, _M - (_Last - _P), _X);
            fill(_P, _Last, _X);
            _Last += _M;
        } else if (0 < _M) {
            _Ucopy(_Last - _M, _Last, _Last);
            copy_backward(_P, _Last - _M, _Last);
            fill(_P, _P + _M, _X);
            _Last += _M;
        }
    }
    size_type size() const { return (_First == 0 ? 0 : _Last - _First); }

private:
    void _Destroy(iterator _F, iterator _L)
        { for (; _F != _L; ++_F) allocator.destroy(_F); }
    iterator _Ucopy(const_iterator _F, const_iterator _L, iterator _P)
        { for (; _F != _L; ++_P, ++_F) allocator.construct(_P, *_F); return (_P); }
    void _Ufill(iterator _F, size_type _N, const T& _X)
        { for (; 0 < _N; --_N, ++_F) allocator.construct(_F, _X); }

    allocator_type allocator;
    iterator _First, _Last, _End;
};
}

typedef std::vector<Elem_00476490> Vec_00476490;
typedef void (Vec_00476490::*InsertFn_00476490)(
    Vec_00476490::iterator, Vec_00476490::size_type, const Elem_00476490&);

// FUNCTION: 0x476490 ?insert@?$vector@UElem_00476490@@V?$allocator@UElem_00476490@@@std@@@std@@QAEXPAUElem_00476490@@IABU3@@Z
InsertFn_00476490 g_insert_00476490 = &Vec_00476490::insert;
