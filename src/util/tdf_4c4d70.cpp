// Decompiled by Space Bunny Free. Names are provisional.
// MSVC 5's std::vector<Elem>::insert(iterator, size_type, const T&), emitted
// out of line for a vector of 4-byte elements: the exe's only caller (0x4c40b2,
// the .TDF parser) passes end(), 1 and the address of the element, so this is
// the append used after a new object is built (0x4c3e40). The body is the
// template with _Ucopy, _Ufill, copy_backward and uninitialized_fill_n inlined:
// the fast branch (0x4c4eb3 onwards) shifts the tail right and the slow branch
// (0x4c4d94 onwards) allocates with operator new[] and copies the three runs.
// The three "test edx, edx; jne; xor eax, eax" blocks are the null guard MSVC 5
// puts in this vector's size() (the same one the out-of-line size() at 0x472d30
// shows, see 0x471160.cpp), inlined three times by the growth arithmetic
// new_size = (size > n ? 2 * size : size + n).
//
// The element type is a guess and only the size matters for the code: the three
// (finish - start) >> 2 shifts fix it at 4 bytes, and any trivially copyable
// 4-byte element type gives these bytes. The pointer reading in the caller's
// neighbourhood (it stores the pointer a new Class_004c3e40 returned, and
// 0x4c3e40 itself has a vector at +0x4) makes Class_004c3e40* the natural pick.
// The name follows 0x4732e0.cpp, which is the same template instantiated on
// vector<Class_00471cc0*>: taking the member's address is what makes the
// compiler emit the instantiation out of line, as the original file did, and
// the member pointer has to return void, or VC5 resolves the two-argument
// insert overload instead. Unlike that 0x4732e0 (which the plain file misses
// only because it puts this in ebp and the count in ebx), this copy is the
// register assignment a plain file produces: this in ebx, the count in ebp.
#include <vector>

class Class_004c3e40 {                 // the element, only its size is used
public:
    int field_0;
};

typedef std::vector<Class_004c3e40*> Vec_004c4d70;
typedef void (Vec_004c4d70::*InsertFn_004c4d70)(Vec_004c4d70::iterator, Vec_004c4d70::size_type, Class_004c3e40* const&);

// FUNCTION: 0x4c4d70 ?insert@?$vector@PAVClass_004c3e40@@V?$allocator@PAVClass_004c3e40@@@std@@@std@@QAEXPAPAVClass_004c3e40@@IABQAV3@@Z
InsertFn_004c4d70 g_insert_004c4d70 = &Vec_004c4d70::insert;
