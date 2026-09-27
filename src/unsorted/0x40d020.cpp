// Decompiled by Claude Opus 5.5. Names are provisional.
// std::vector<unsigned short>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward all
// inlined. 0x409160 calls it from the inlined resize() of the vector at
// +0x7d (with size() 0x40d000 and erase() 0x40d240). Taking the member's
// address makes the compiler emit the template instantiation out of line.
//
// Partial (57.0%): the instructions are the same template code, but the
// registers differ throughout. The original keeps `this` in ebp and _M in
// ebx (as 0x40d290 does for unsigned char); ours puts `this` in ebx and _M
// in ebp (as the original 0x408f30 does for Unit*), so the realloc path
// loads _P into ebx instead of ecx and every temporary shifts. The sums that
// differ in 0x40d290 differ here too. Nothing moved it: the element type
// (short, unsigned short, 2-byte structs with and without copy
// operations), explicit specialisations of the same body, instantiating
// through push_back/resize/the real 0x409160 caller, preceding functions,
// 0 to 10000 extra declarations, header sets, /Gz, /Zp1, /G3-/G6 and the
// RTM compiler all give exactly this code.
#include <windows.h>
#include <ddraw.h>
#include <vector>

typedef std::vector<unsigned short> Vec_0040d020;
typedef void (Vec_0040d020::*InsertFn_0040d020)(
    Vec_0040d020::iterator, Vec_0040d020::size_type, const unsigned short&);

// FUNCTION: 0x40d020 ?insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z
InsertFn_0040d020 g_insert_0040d020 = &Vec_0040d020::insert;
