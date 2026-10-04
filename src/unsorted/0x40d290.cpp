// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash and space-bunny-free, finished by GPT-6. Names are provisional.
// Claude Opus 5.5 (#5544, 2026-10-04): still 93.3%, but the cause is now pinned
// down, and it is symbol ids, not the source. The earlier extern scans all used
// even counts or even steps, so they never tried an odd count. With N unused
// `extern int` declarations between <ddraw.h> and <vector>, this exact file
// MATCHes for N = 32307, 32309, ..., 32359 (odd N only; each even N between
// them gives the old tail). In `c2prio.py --symbols` terms that is this insert's
// own id at 65602..65654 (low 16 bits 0x42..0x76) and even: the vector<unsigned
// char> instantiation has to straddle 65536, with allocator<unsigned
// char>::allocate just below it and size() just above. The same window comes
// out with the declarations placed after <vector> (before the typedef).
// Declarations at the end of the file, which move only C2's own ids, never
// reach it, and nothing moves at the 32768 boundary. Hunk by hunk: odd N from
// about 32094 on gives the `_End` add, odd N 32245..32359 the `_Ufill` count,
// and odd N 32307..32453 the `_Last` sum. So the original TU had about 65,600
// declarations ahead of this insert's, like the prefix 0x41b2e0 and 0x471de0
// need (docs/c2-regalloc.md, "lost common header"). No plausible real header
// set gets there (52k at most), and the padding cannot be committed, so the file
// stays as it is. Also flat without /Gi: element types char, signed char and
// bool, and one use each of operator=, resize, reserve, the copy constructor,
// push_back and erase. Scratch: build/scratch/0x40d290/ (scan.py, tail.py and
// summ.py print the three tail shapes for a batch of files in seconds).
// #5526 Codex recheck: 93.3%; documented vector insert TU-state differences remain.
// Claude Opus 5.5 (#4601, 2026-10-04): still 93.3%. The Part 7 /Gi recipe
// (the one that matched 0x40d020 and 0x40cca0 next door) does NOT apply here.
// Under `// FLAGS: /Gi` this insert compiles to one fixed 68.7% (475 bytes)
// whatever else the file holds: no other use, operator=, reserve, resize, the
// copy ctor, the fill ctor, assign(n, x), range insert, insert(P, x), erase,
// the dtor, push_back, std::fill, std::copy_backward, swap, operator==, every
// pair of the first six, uses placed before or after the member pointer, the
// whole 0x409160 TU, <windows.h>/<ddraw.h>, the Elem and short inserts
// (0x40cca0, 0x40d020) instantiated first, 0..65536 externs ahead of
// <vector>, 0..4096 externs between <vector> and the instantiation, and up to
// 575 filler functions: 160+ files, all byte-identical. c2prio shows why: under
// /Gi the insert's own locals and params are numbered per function
// (0x3ffff9..0x3ffffe) and only the function symbol's TU-wide id moves, and
// here no tie depends on it. The /Gi build gets the first branch's _P web a
// priority of 51 (B26's `_F = _P` copy is charged to _P, not the _Ucopy
// temp), which puts _P in esi and the first _Ucopy's dest in ecx; the
// original has the default build's split (_P edi, dest edx, the _P web at
// priority 11). For vector<short> the operator= use moves the same web from
// 19 to 59, the direction that original needs; this one needs the low
// priority, which /Gi never gives. Without /Gi: a wide extern scan (0..65536, step 512,
// before <vector>; 0..65536, step 512, at the end of the file so only C2's own
// ids move; a 16x16 grid of both at step 4096; 31400..33000 step 8; and
// 32088..32100 step 1 around the _N/_S wrap) gives only 93.3, 92.3, 91.9,
// 90.5, 84.7, 83.8, 82.1, 71.5, 71.0, 69.6 and 66.4, never a MATCH. So this
// insert is from a default-flags TU, and the three tail hunks need TU state
// that no file reaches. Scratch generators: build/scratch/0x40d290/.
// deepseek-v4.1-flash (#3265 round): still 93.3 via the member-pointer instantiation of the stock header; diff hunks unchanged (add eax,ebx vs lea ecx,[ebx+eax] plus the [esp+0x20] register order in the two at-end tails).
// space-bunny-free (2026-10-01): still 93.3% (477 bytes), confirmed baseline.
// space-bunny-free (2026-10-02, issue 4430): still 93.3%, no improvement. ~110 new
// variants, all flat or worse; the scratch harness and the negative results are in
// build/scratch/0x40d290/ (fd.py + sw.py give a check.py-quality diff in 0.3 s, so
// the sweeps below cost no check.py runs). New this pass, all 47 differing lines
// (the same three hunks) unless noted:
//   * The dead-store-in-a-folded-branch lever that matched 0x40d900 in this issue
//     is DEAD here. 74 placements of `int t = 0; if (t) _M = 0;` and its
//     equivalents (while (t), a dead for, a dead array element, `if (_M & 0)`)
//     over six slots of the first block are bit-identical to the baseline (the
//     front end folds them before codegen there), and in the two else-if branches
//     they are much worse (61 to 146 differing lines): they break the inlining or
//     the block schedule.
//   * The _Last association is canonical, not source order: 16 spellings of
//     `_Last = _S + size() + _M` ((_S + size()) + _M, _S + (size() + _M),
//     (size() + _S) + _M, _M + _S + size(), _S + _M + size(), int and size_type
//     casts, the ternary written out, size() in a temp) all give the same
//     (_S + _M) + size(), so MSVC 5's front end flattens and re-sorts the
//     commutative chain and the source order cannot reach it. Six permutations of
//     the three tail statements are worse (52 to 82), best 83.6% as before.
//   * `_End = _S + _N` never becomes `add eax, ebx`: 20 spellings (both operand
//     orders, &_S[_N], a (char*)_N + (int)_S pun with the pointer cast to an int
//     and back, int/size_type/difference_type casts, _End pre-set to a dead _S,
//     a hoisted _E0, the sum assigned twice, a dead `if (t) _N = 0`) all give
//     `lea ecx, [ebx + eax]` with a fresh destination, never a 2-operand add. The
//     back end only uses `add dst, src` when dst is already one addend, and with
//     a free register ecx available it always takes the lea.
//   * The _Ufill count: 20 spellings, including every cast shape, the parenthesised
//     negation the original needs, all reassociate to `lea ecx, [ebx + edi]`.
//   * Compiler-state knobs re-tested flat at 47: #pragma pack(1/2/16), and
//     <climits> <memory> <xutility> <string.h> <stdio.h> <stdlib.h> <assert.h>
//     <new> <typeinfo> ahead of <windows.h> (<math.h> is worse at 55, as before).
//     Swapping the three `if` comparisons to `_M > ...` is worse (56); only
//     `else if (_M > 0)` is free.
//   * The exe's own <vector> insert text is identical in msvc5-rtm and msvc5-sp3,
//     so the RTM-header theory is dead too.
//   * The chain ORDER is a build-level codegen habit, not a source choice. Two
//     micro benchmarks in build/scratch/0x40d290/micro*.cpp settle it. For
//     integers, `unsigned f(x,y,z){return (x-y)+z;}` compiles to
//     `mov eax,z / mov ecx,y / sub eax,ecx / mov ecx,x / add eax,ecx`, i.e. the
//     rightmost term first, and the same shape comes out of an explicit `t = x-y`
//     temp (58 lines here, the temp is materialised twice). For pointers,
//     `T *p(b,m,l,q){return b + (l-q) + m;}` compiles to
//     `sub eax,ecx (the difference) / add eax,m / add eax,b`, so MSVC 5 folds a
//     commutative pointer chain into base + (difference + terms) and always
//     evaluates the pointer difference first. That shape is invariant to the
//     types and to the parenthesisation (micro5.cpp: unsigned or int difference,
//     signed or unsigned m, and (_S + diff) + m all give the same three
//     instructions), so it is a fixed canonical form. The original evaluates
//     (_S + size) + _M, i.e. base, difference, term, which is neither the source
//     order nor either order this build ever produces, so it is unreachable from
//     the source text.
//   * The add-versus-lea choice is a codegen habit too, and the micro benchmarks
//     pin down when this build switches (micro3.cpp, micro4.cpp). For
//     `v->e = b + n` with both operands dead afterwards, MSVC 5 emits
//     `mov eax,n / mov ecx,b / add eax,ecx / mov [edx+8],eax`, the original's
//     2-operand form, and picks either operand as the accumulator depending on
//     what else is live. As soon as the result has to sit in a register that
//     survives a branch, or the offset is still needed after an `if`, it switches
//     to `lea dst,[base+offset]` with a fresh destination, which is what this
//     build does for `_End = _S + _N` between the deallocate call and size()'s
//     null test. The original emits the `add` form there anyway.
// So the three hunks are still unreachable from this side of the header, and the
// residue is one compiler-state family with 0x408f30, 0x40cca0 and 0x40a7b0.
// tools/permute.py 0x40d290 --file build/scratch/0x40d290/v1_annot.cpp (the
// header's insert body copied out by hand as an explicit out-of-line
// specialisation, so the permuter can rewrite it): 10.1 min, 3422 candidates,
// 0 compile errors, 93.3% -> 93.3%, score 1114 -> 1114, no lineage.
// deepseek-v4.1-flash (#3770 round): re-confirmed 93.3% (477 bytes, exact) with the
// same four hunks; no new lever attempted here, the residue stays the realloc-tail
// association and load scheduling inside the <vector> instantiation.
// deepseek-v4.1-flash (2026-10-01, #4170 round, ~510 check.py runs): still 93.3%,
// best unchanged. Fresh negative results, all in build/scratch/0x40d290/:
//   * A full hand-written out-of-line specialization (body under source control,
//     `template<> void std::vector<unsigned char>::insert(...)` + the global
//     member pointer) is byte-identical to the header instantiation, so every
//     tail spelling was tested directly. About 40 spellings (both operand
//     orders, int/size_type/difference_type/(void*) casts, integer-only sums
//     with the result cast back, temporaries for _N/_S/_M/size(), `_End += _N`
//     forms, `_Last = _S + size(); _Last += _M;`, statement reorders, a hoisted
//     `iterator _F0 = _First;` local, spelled-out size()) either fold back to
//     this emission or lose bytes (91.0% or worse). Only a hoisted _First local
//     moves a register (the _End sum goes to edx), never the sum's shape.
//   * Replicating this TU's preceding emission order in the exe (vector<short>
//     size 0x40d000, insert 0x40d020, erase 0x40d240, _Destroy 0x40d280 ahead
//     of ours) changes nothing, nor do 0..38 dummy function definitions ahead.
//   * `extern int` filler sweep 0..2048 step 8 (and the 0x476210/0x408f30
//     hand-written namespace std clone carrier, whose filler sweep has four
//     states 87.0/91.0/92.8/93.3): only the third _Ucopy's source start moves
//     (period 256); the three tail hunks are present in every state.
//   * 50 single extra headers and the full tools/headers.py --cpp (768 sets,
//     0 failures) top out at 93.3%; the RTM compiler is identical.
// So the tail is unreachable from this file; the original TU's state decides it.

// NEW, and it closes the loop on the "is it steerable from source" question: an
// explicit out-of-line SPECIALISATION
// `template<> void vector<unsigned char, allocator<unsigned char> >::insert(...)`
// whose body is this header's body copied out by hand, plus a global
// `&Vec_0040d290::insert` to force emission, compiles to BYTE-IDENTICAL code to
// the header instantiation (same 93.3%, same four hunks). So the body below is
// now under direct source control, and the following reassociations of the
// three sums all leave the emission exactly as it is:
//   `_End = _N + _S;` (operand order swapped)      : 93.3%, `lea ecx,[ebx+eax]`
//   `_Ufill(_Last, (size_type)((int)_M-(int)_Last)+(size_type)(int)_P, _X)` : 93.3%
//   `iterator _B = _S;` used for all three tail statements : 93.3%
//   `size_type _SZ = size(); size_type _N = _SZ + (_M < _SZ ? _SZ : _M);` : 64.0%
//   `_Last = _S + size() + _M;` with `size()` written out as
//   `_S + (_First == 0 ? 0 : _Last - _First) + _M` : 87.7% (breaks the CSE)
//   `size_type _Z = size(); _Last = _S + _Z + _M;` : 85.8% (475 bytes)
// So MSVC 5's canonicalisation of `_End = _S + _N` is not an operand-order
// choice at all, and the `_Ufill` count keeps reassociating to
// `(_M + _P) - _Last` through casts. The only way to the original's
// `(_M - _Last) + _P` would be for the front end to keep the negation inside
// the parenthesis, which this compiler never does for a size_type count:
// evidence that the original was built with a compiler whose front end
// canonicalises `_M - (_Last - _P)` the other way.
// deepseek-v4.1-flash (#3023 retry): still 93.3% (477 bytes, exact). `<vector>`
// member-address emission is optimal; an explicit out-of-line member specialisation
// emits byte-identical code, so it is not a steering lever. Residue: the realloc tail
// `_End = _S + _N` emits `lea ecx,[ebx+eax]` (S+N) instead of original `add eax,ebx`
// (N+S), and `_Last = _S + size() + _M` emits (S+M)+size instead of (S+size)+M, plus
// one jump-offset byte. Compiler-state tie.
// std::vector<unsigned char>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward all
// inlined. 0x409160 calls it from the inlined resize() of the vector at
// +0x9d (next to its erase, 0x40d470). Taking the member's address makes the
// compiler emit the template instantiation out of line.
//
// Partial (93.3%): only the ordering/scheduling of the reallocation tail and
// one sum association differ, all in template code whose source is fixed.
// After the deallocate call, the original loads the old _First first and
// computes `_End = _S + _N` as `add eax, ebx` (N + S), while ours schedules
// the sum first as `lea ecx, [ebx + eax]` (S + N) and loads _First after;
// `_Last = _S + size() + _M` is (S + size) + M in the original but
// (S + M) + size in ours. In the middle branch the _Ufill count
// `_M - (_Last - _P)` is (M - Last) + P in the original but (P + M) - Last in
// ours. The element type (any 1-byte type, with or without copy operations),
// explicit member specialisations of the same body with the terms rewritten,
// instantiating through push_back/resize/the real 0x409160 caller, preceding
// functions, /Gz, /Zp1, the RTM compiler, an explicit `template class`
// instantiation, a preceding erase instantiation and all 128 headers.py sets
// never change these. Same compiler-state family as 0x40cca0 and 0x40a7b0.
// A fourth sum (the source start of the third _Ucopy) flips with compiler
// state every 256 declarations: <windows.h> plus <ddraw.h> (or <math.h>)
// gives the original's order there. Same family as 0x408f30 and 0x40d020.
// This file is the member-address form (no manual body): the body below is
// the SP3 VECTOR header's insert(), whose reallocation tail reads
// `_End = _S + _N; _Last = _S + size() + _M;` exactly as the original. The
// residue is pure register/schedule choice for those two sums, so it is not
// expressible from this side of the header. Confirmed on 2026-09-30:
// headers.py --cpp (768 sets, 0 failures) tops out at 93.3%, a windows.h +
// math.h + ddraw.h triple is worse (92.8%), and 1/2/3/6/12 dummy preceding
// typedefs (the every-256-declarations state flip) do not move it. A
// written-out namespace std { class vector } carrier (as in 0x425210.cpp,
// which reaches 99.6% for a 2-byte element) scores 91.0% here, so for a
// 1-byte element the real <vector> stays the better carrier.
// Re-confirmed 2026-09-30 (deepseek-v4.1, 35 runs): an explicit member
// specialisation of std::vector<unsigned char>::insert in this same file
// emits byte-identical code to the header instantiation (93.3%, same four
// hunks), so the tail is steerable only through source statements; swapping
// the _End/_Last assignment order gives 83.6% (454 bytes), `_E += _N` temps
// give 85.8% (475 bytes), and the _Ufill count `_M - _Last + _P` written
// with integer casts is reassociated straight back. A dummy-typedef scan
// (0..1024, step 16) shows the compiler state toggles exactly every 256
// declarations (92.8% in phases 256..511 mod 512, 93.3% in 0..255) and that
// toggle moves only the fourth sum (the _Ucopy source start), never these
// two. So the two hunks below are unreachable from this emission context.
// Re-checked 2026-09-30 (deepseek-v4.1, session 2): the hand-written
// namespace std clone carrier of 0x408f30 (only the members insert needs) is
// worse here, 91.4% (477 bytes): it regresses the third _Ucopy's source start
// and moves the per-loop `mov ebp, [esp + 0x10]` reload out of the loop, while
// the two tail sums stay exactly as below. Every source spelling of the tail
// canonicalises to the same bytes: `_End = _N + _S`, `(_S + size()) + _M`,
// `_M + (_S + size())`, `size() + _S + _M`, `_End = _S; _End += _N;` and
// `_Last = _S + size(); _Last += _M;` all give this file's emission (93.3%),
// and `_First = _S;` moved above the sums reaches 92.8% only by letting the
// compiler substitute _S into size() (test ebx,ebx / sub eax,ebx), which is a
// different value, not the original. Compiler-state fillers do not reach the
// two sums either: typedef, struct, class, static-variable and function-
// prototype dummies at 0 to 16384 declarations, <string>/<list>/<map>/
// <algorithm>/<iostream>/<deque>/<set> added before <vector>, and 1 to 8
// extra vector<unsigned short/int/...>::insert instantiations ahead of this
// one all land on either 93.3% (4 hunks) or 92.8% (5 hunks, the extra hunk is
// the third _Ucopy's source start at the @@ -77 region). The two realloc-tail
// sums are invariant under every lever tried, here and in 0x408f30.
// Retry 2026-09-30 (deepseek-v4.1-flash): headers.py 128 include sets again
// top out at 93.3% (`<math.h>`, `<ddraw.h>`, `<windows.h> <math.h>`,
// `<windows.h> <ddraw.h>`, `<stdio.h> <math.h>`); adding <math.h> to this
// file gives the 92.8% triple, so the two extra windows.h/ddraw.h includes
// stay. The four hunks (one jump-offset hunk plus the three realloc-tail
// sums) are unchanged, so this stays a register-scheduling near-miss.
#include <windows.h>
#include <ddraw.h>
#include <vector>

typedef std::vector<unsigned char> Vec_0040d290;
typedef void (Vec_0040d290::*InsertFn_0040d290)(
    Vec_0040d290::iterator, Vec_0040d290::size_type, const unsigned char&);

// FUNCTION: 0x40d290 ?insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z
InsertFn_0040d290 g_insert_0040d290 = &Vec_0040d290::insert;
