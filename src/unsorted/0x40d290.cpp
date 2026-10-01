// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// space-bunny-free (2026-10-01): still 93.3% (477 bytes), confirmed baseline.
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
