// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Stays in its own file: in los_tables.cpp's symbol context its registers land
// differently (docs/c2-regalloc.md).
// 585-byte GUI list entry (same class as 0x432fb0 in los_tables.cpp). The 4th
// parameter is the template's unused _Ty* tag, passed as 0 and never read.
// Must come first: changes the evaluation order of the final count comparison.
#include <windows.h>
#include <algorithm>

#pragma pack(push, 1)
#include "../units/unit_def.h"
#pragma pack(pop)

typedef int (__stdcall* Compare)(const UnitDef&, const UnitDef&);

// FUNCTION: 0x432d40
void __stdcall SortUnitTypes(UnitDef* first, UnitDef* last,
                            Compare comp, int unused)
{
    for (; std::_SORT_MAX < last - first; ) {
        UnitDef* _M = std::_Unguarded_partition(first, last,
            std::_Median(UnitDef(*first),
                UnitDef(*(first + (last - first) / 2)),
                UnitDef(*(last - 1)), comp), comp);
        if (last - _M <= _M - first)
            SortUnitTypes(_M, last, comp, 0), last = _M;
        else
            SortUnitTypes(first, _M, comp, 0), first = _M;
    }
}
