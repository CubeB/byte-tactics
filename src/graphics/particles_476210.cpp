// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. Names are provisional.
// FLAGS: /Gi
// std::vector<Elem_00476210>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on a 32-byte record, emitted out of line through a member
// pointer.
#include <vector>

struct Elem_00476210 {
    int dwords[8];                     // 0x20 bytes
};

typedef std::vector<Elem_00476210> Vec_00476210;
typedef void (Vec_00476210::*InsertFn_00476210)(
    Vec_00476210::iterator, Vec_00476210::size_type, const Elem_00476210&);

// The caller's growth step; the insert's bytes need the TU to instantiate
// reserve.
void __stdcall Grow_00476210(Vec_00476210* records, int periods)
{
    records->reserve(periods + records->size());
}

// FUNCTION: 0x476210 ?insert@?$vector@UElem_00476210@@V?$allocator@UElem_00476210@@@std@@@std@@QAEXPAUElem_00476210@@IABU3@@Z
InsertFn_00476210 g_insert_00476210 = &Vec_00476210::insert;
