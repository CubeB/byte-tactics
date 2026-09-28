// Decompiled by space-bunny-free. Names are provisional.
// std::vector<unsigned short>::insert(iterator, size_type, const T&) from
// MSVC 5's <vector>, with _Ucopy, _Ufill, fill and copy_backward all
// inlined. 0x424c00 calls it from the first inlined resize() of its feature
// type remap table, next to size() (0x4251f0) and erase() (0x425430).
// Taking the member's address makes the compiler emit the template
// instantiation out of line.
//
// Partial (89.6%, 540 of 544 bytes): every instruction of the template
// matches, including the register allocation (this in ebx, _M in ebp, which
// is the opposite of 0x40d020's original). One four-instruction sum in the
// reallocation branch differs, the source start of the third _Ucopy,
// `_Ucopy(_P, _Last, _Q + _M)`, where the values all cancel to _P:
//
//   original:  lea eax, [ebx + ecx]   ; _P + (_Q + 2 * _M)
//              sub eax, edx          ; - _Q
//              sub eax, edi          ; - 2 * _M
//   ours:      mov eax, ecx          ; _Q + 2 * _M
//              sub eax, edx          ; = 2 * _M
//              add eax, ebx          ; + _P
//              sub eax, edi          ; = _P
//
// The original keeps (base + index) - _Q - 2 * _M, ours folds (dest - _Q)
// first and then adds _P. The expression is fixed in <vector>, so the only
// free variable is the compiler's state, and nothing moves it: the element
// type (short, and a 2-byte class with copy construction, both give
// byte-identical code), emitting through a derived class as 0x4251e0 does
// for _Destroy, emitting the real siblings 0x4251f0 and 0x425430 in the same
// file, and all 128 header sets of tools/headers.py. Same family as the
// partials 0x40d020 (57.0%) and 0x40d290 (92.3%), whose notes record the
// same problem with two more spellings of this sum.
#include <vector>

typedef std::vector<unsigned short> Vec_00425210;
typedef void (Vec_00425210::*InsertFn_00425210)(
    Vec_00425210::iterator, Vec_00425210::size_type, const unsigned short&);

// FUNCTION: 0x425210 ?insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z
InsertFn_00425210 g_insert_00425210 = &Vec_00425210::insert;
