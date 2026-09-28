// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::vector<Elem>::insert(iterator, size_type, const T&) from MSVC 5's
// <vector>, emitted out of line for a 32-byte trivially copyable element. The
// body is the template with _Ucopy, _Ufill, fill and copy_backward inlined:
// the free-space check splits into the reallocating branch (allocate, copy the
// prefix, fill _M copies, copy the suffix, deallocate, reset the three
// pointers) and the two in-place branches (shift the tail right when it is
// shorter than _M, or roll the last _M elements and shift the middle with
// copy_backward). Taking the member's address makes the compiler emit the
// template instantiation out of line, as in the original translation unit.
//
// Still differs: 89.6%, ours 637 bytes against the original's 636. The only
// difference is how the initial source pointer of the third copy
// (_Ucopy(_P, _Last, _Q + _M), the loop right after the fill) is computed.
// The original adds P and the destination first and then subtracts:
//     lea eax, [edi + edx]        ; _P + (_Q + _M)
//     sub eax, ebx                ; - _Q
//     sub eax, ecx                ; - _M * 32
// this build subtracts the destination base first and then adds P:
//     mov eax, edx                ; _Q + _M
//     sub eax, ebx                ; - _Q
//     add eax, edi                ; + _P
//     sub eax, ecx                ; - _M * 32
// Both give _P; the extra mov/add costs the one byte, which shifts every later
// offset in the diff. Every other instruction, the register assignment
// (this = edi, _M = esi, _Q = ebx, dest = edx), the branch structure and the
// operator new/delete calls are identical.
// This is the same wall as the exe's other instantiations of this template,
// 0x408f30 (vector<Unit*>) and 0x40cca0 (3-byte element): theirs leave the
// identical `mov/sub/add` against the original's `lea/sub`. The choice is
// compiler state from the rest of the original translation unit, not the
// source: all 768 header sets (tools/headers.py --cpp), the element type
// (int[8], char[32], short[16], float[8], double[4], pointer plus ints, eight
// scalars), #pragma pack 1/2/4/8/16, a derived access struct, an
// uninitialised variable, an explicit member specialisation with the template
// body verbatim, and hoisting the destination into a local all give the same
// 637 bytes with this one block unchanged.
#include <vector>

struct Elem_00476210 {
    int dwords[8];                     // 0x20 bytes
};

typedef std::vector<Elem_00476210> Vec_00476210;
typedef void (Vec_00476210::*InsertFn_00476210)(
    Vec_00476210::iterator, Vec_00476210::size_type, const Elem_00476210&);

// FUNCTION: 0x476210 ?insert@?$vector@UElem_00476210@@V?$allocator@UElem_00476210@@@std@@@std@@QAEXPAUElem_00476210@@IABU3@@Z
InsertFn_00476210 g_insert_00476210 = &Vec_00476210::insert;
