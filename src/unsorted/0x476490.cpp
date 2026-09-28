// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL (60.7%). std::vector<Record_004750b0>::insert(iterator, size_type,
// const T&) from MSVC 5's <vector> (VECTOR line 150), with _Ucopy, _Ufill,
// fill and copy_backward inlined (the element is trivially copyable, so the
// copies become rep movsd loops). The only caller is Class_004750b0's effect
// append at 0x4751c0, which pushes end(), 1 and the address of a stack record.
// The element type is the 32-byte Record_004750b0 (pointer, Vec3, four ints);
// its exact layout only matters for its size, 0x20.
//
// What still differs: the original spills `this` to [esp+0x10] and reloads it
// (it keeps _M in esi, _S in ebx, _P in edx and uses edi as scratch), while
// this build caches `this` in edi from entry (`mov edi, ecx` /
// `mov [esp+0x10], edi`) and so loads _End/_Last through edi instead of ecx.
// Everything else follows from that one choice: the two are 632 bytes against
// 637 bytes, and the rest of the diff is the resulting ebx/ebp/esi/edi
// permutation (which value gets a callee-saved register for the _Ufill loop,
// and _P being reloaded from its stack slot instead of living in edx). The
// branch structure, the pointer arithmetic, the calls to operator new
// (0x4b4f10) and operator delete (0x4b4f20) and both rep movsd copy loops are
// identical.
//
// Attempts that did not change it: taking the member's address versus
// explicitly instantiating the class; a named 32-byte struct of ints, char[32],
// long long[4], double[4] or the real Record_004750b0; a hand-written
// non-template method with the header's body (that one puts `this` in esi and
// needs only two locals); every header combination tools/headers.py tries,
// including --cpp (768 sets, all 60.7%); the msvc5-rtm compiler. An element
// type with a user copy constructor or operator= does make the compiler spill
// `this`, but it only does so when it emits an out-of-line call for the copy,
// which the original does not have. These registers look like compiler state
// from the rest of the original translation unit, the same wall recorded for
// 0x425480 and 0x4732e0 (both 57.9%).
#include <vector>

struct Vec3_00476490 {
    int x;
    int y;
    int z;
};

struct Elem_00476490 {              // Record_004750b0, 0x20 bytes
    void* data;                     // +0x00
    Vec3_00476490 pos;              // +0x04
    int limit;                      // +0x10
    int count;                      // +0x14
    int period;                     // +0x18
    int timer;                      // +0x1c
};

typedef std::vector<Elem_00476490> Vec_00476490;
typedef void (Vec_00476490::*InsertFn_00476490)(
    Vec_00476490::iterator, Vec_00476490::size_type, const Elem_00476490&);

// FUNCTION: 0x476490 ?insert@?$vector@UElem_00476490@@V?$allocator@UElem_00476490@@@std@@@std@@QAEXPAUElem_00476490@@IABU3@@Z
InsertFn_00476490 g_insert_00476490 = &Vec_00476490::insert;
