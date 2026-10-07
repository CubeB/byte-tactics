// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by GPT-6.1-sol, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5, verified by GPT-6. Names are provisional.
// FLAGS: /Gi
// std::vector<Record_00475bd0>::insert(iterator, size_type, const _Ty&), stock
// MSVC 5 <vector> on a 0x3c-byte record, emitted out of line through a member
// pointer.
#include <vector>

struct Record_00475bd0 {
    int field_00;
    int field_04;
    int field_08;
    int field_0c;
    int field_10;
    int field_14;
    int field_18;
    int field_1c;
    int field_20;
    int field_24;
    int field_28;
    int field_2c;
    int field_30;
    int field_34;
    int field_38;
};

typedef std::vector<Record_00475bd0> Vec_00475bd0;
typedef void (Vec_00475bd0::*InsertFn_00475bd0)(
    Vec_00475bd0::iterator, Vec_00475bd0::size_type, const Record_00475bd0&);

// The caller's growth step; the insert's bytes need the TU to instantiate
// reserve.
void __stdcall Grow_00475bd0(Vec_00475bd0* records, int extra)
{
    records->reserve(extra + records->size());
}

// FUNCTION: 0x475bd0 ?insert@?$vector@URecord_00475bd0@@V?$allocator@URecord_00475bd0@@@std@@@std@@QAEXPAURecord_00475bd0@@IABU3@@Z
InsertFn_00475bd0 g_insert_00475bd0 = &Vec_00475bd0::insert;
