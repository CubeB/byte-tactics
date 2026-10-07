// Decompiled by deepseek-v4.1-flash. Names are provisional.
// 585-byte GUI list entry (same class as 0x432fb0.cpp). The 4th parameter is
// the template's unused _Ty* tag, passed as 0 and never read.
// Must come first: changes the evaluation order of the final count comparison.
#include <windows.h>
#include <algorithm>

#pragma pack(push, 1)
class UnitDef {
public:
    char unknown_0[0x249];
    UnitDef& operator=(const UnitDef& src);
};
#pragma pack(pop)

typedef int (__stdcall* Compare)(const UnitDef&, const UnitDef&);

// FUNCTION: 0x432d40
void __stdcall FUN_00432d40(UnitDef* first, UnitDef* last,
                            Compare comp, int unused)
{
    for (; std::_SORT_MAX < last - first; ) {
        UnitDef* _M = std::_Unguarded_partition(first, last,
            std::_Median(UnitDef(*first),
                UnitDef(*(first + (last - first) / 2)),
                UnitDef(*(last - 1)), comp), comp);
        if (last - _M <= _M - first)
            FUN_00432d40(_M, last, comp, 0), last = _M;
        else
            FUN_00432d40(first, _M, comp, 0), first = _M;
    }
}
