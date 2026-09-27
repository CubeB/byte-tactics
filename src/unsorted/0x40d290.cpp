// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<unsigned char>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward all
// inlined. 0x409160 calls it from the inlined resize() of the vector at
// +0x9d (next to its erase, 0x40d470). Taking the member's address makes the
// compiler emit the template instantiation out of line.
//
// Partial (92.3%): only the order of three-operand sums differs, all in
// template code whose source is fixed. The original computes
// `_End = _S + _N` as N + S, `_Last = _S + size() + _M` as (S + size) + M,
// and the _Ufill count `_M - (_Last - _P)` as (M - Last) + P; ours gives
// S + N, (S + M) + size and (M + P) - Last. The element type (any 1-byte
// type, with or without copy operations), explicit member specialisations
// of the same body with the terms rewritten, instantiating through
// push_back/resize/the real 0x409160 caller, preceding functions, /Gz, /Zp1,
// the RTM compiler and 200 random header sets never change these three.
// A fourth sum (the source start of the third _Ucopy) flips with compiler
// state every 256 declarations: <windows.h> plus <ddraw.h> (or <math.h>)
// gives the original's order there. Same family as 0x408f30 and 0x40d020.
#include <windows.h>
#include <ddraw.h>
#include <vector>

typedef std::vector<unsigned char> Vec_0040d290;
typedef void (Vec_0040d290::*InsertFn_0040d290)(
    Vec_0040d290::iterator, Vec_0040d290::size_type, const unsigned char&);

// FUNCTION: 0x40d290 ?insert@?$vector@EV?$allocator@E@std@@@std@@QAEXPAEIABE@Z
InsertFn_0040d290 g_insert_0040d290 = &Vec_0040d290::insert;
