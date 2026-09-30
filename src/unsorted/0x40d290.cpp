// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash and GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
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
#include <windows.h>
#include <ddraw.h>
#include <vector>

typedef std::vector<unsigned char> Vec_0040d290;
typedef void (Vec_0040d290::*InsertFn_0040d290)(
    Vec_0040d290::iterator, Vec_0040d290::size_type, const unsigned char&);

// FUNCTION: 0x40d290 ?insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z
InsertFn_0040d290 g_insert_0040d290 = &Vec_0040d290::insert;
