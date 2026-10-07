// Decompiled by Claude Opus 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<short>::insert(iterator, size_type, const _Ty&). 0x409160 calls
// it from the inlined resize() of the vector at +0x7d (with size() 0x40d000
// and erase() 0x40d240).
#include <vector>

typedef std::vector<short> Vec_0040d020;
typedef void (Vec_0040d020::*InsertFn_0040d020)(
    Vec_0040d020::iterator, Vec_0040d020::size_type, const short&);

// An assignment on this vector type, standing in for the original TU's own;
// the insert's bytes need the TU to instantiate operator=.
void __stdcall Assign_0040d020(Vec_0040d020* dest, const Vec_0040d020* src)
{
    *dest = *src;
}

// FUNCTION: 0x40d020 ?insert@?$vector@FV?$allocator@F@std@@@std@@QAEXPAFIABF@Z
InsertFn_0040d020 g_insert_0040d020 = &Vec_0040d020::insert;
