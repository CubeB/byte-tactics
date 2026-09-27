// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// BYTES MATCH 100%, but check.py reports one BAD reference and so not MATCH:
//
//   BAD +0x02b  UElem_00434020::?$allocator::destroy  0x4343f0
//     0x4343f0 is already named 'FUN_004343f0' in data/symbols.csv; use that name
//
// 0x4343f0 is an empty `ret 4` with exactly one caller (this function), which
// sets ecx to the element (the inner vector object) and pushes the item:
//
//   0x433567  push edi
//   0x433568  mov  ecx, esi
//   0x43356a  call 0x4343f0
//
// That is the calling convention of std::allocator<T>::destroy(pointer) for a
// trivially destructible T (out of line it is empty). It cannot be the free
// void __stdcall FUN_004343f0(int) that src/unsorted/0x4343f0.cpp declares:
// a free function never gets ecx set, and the only caller here sets it. The
// sibling 0x434400 (std::allocator<vector<Elem_00434020> >::destroy) is named
// by exactly this pattern in data/symbols.csv, and 0x434430 (empty, called
// WITHOUT ecx) is the free std::_Destroy overload. So the data/symbols.csv
// entry for 0x4343f0 is the wrong name and blocks the match; the bytes and all
// other references are correct.
//
// std::vector<std::vector<Elem_00434020> >::~vector() from MSVC 5's <vector>,
// out of line: each inner vector's elements are destroyed through the
// out-of-line allocator::destroy (empty for a trivial element), then its
// _First is freed and its three pointers zeroed; finally the outer _First is
// freed and zeroed. Written as a method that calls the destructor explicitly,
// the way 0x4330b0.cpp does; taking the allocator member's address makes the
// compiler emit allocator::destroy out of line.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

typedef std::vector<Elem_00434020> Inner_00434020;
typedef std::vector<Inner_00434020> Outer_00434020;
typedef void (std::allocator<Elem_00434020>::*DestroyFn_00434020)(Elem_00434020*);

DestroyFn_00434020 g_destroy_00434020 = &std::allocator<Elem_00434020>::destroy;

class Class_00433540 {
public:
    void FUN_00433540();
};

// FUNCTION: 0x433540
void Class_00433540::FUN_00433540()
{
    ((Outer_00434020*)this)->~vector();
}