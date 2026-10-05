// Decompiled by deepseek-v4.1-flash. Names are provisional.
// 585-byte GUI list entry (same class as 0x432fb0.cpp). This is MSVC 5's
// <algorithm> _Sort instantiated for the 585-byte element with a __stdcall
// comparison callback, so the real header supplies _Median and
// _Unguarded_partition (both inlined here). The 4th parameter is the
// template's unused _Ty* tag, passed as 0 and never read.
// <windows.h> must come first: it flips the evaluation order of the final
// element-count comparison to match the original.
#include <windows.h>
#include <algorithm>

#pragma pack(push, 1)
class Class_0042b370 {
public:
    char unknown_0[0x249];
    Class_0042b370& operator=(const Class_0042b370& src);
};
#pragma pack(pop)

typedef int (__stdcall* Compare)(const Class_0042b370&, const Class_0042b370&);

// FUNCTION: 0x432d40
void __stdcall FUN_00432d40(Class_0042b370* first, Class_0042b370* last,
                            Compare comp, int unused)
{
    for (; std::_SORT_MAX < last - first; ) {
        Class_0042b370* _M = std::_Unguarded_partition(first, last,
            std::_Median(Class_0042b370(*first),
                Class_0042b370(*(first + (last - first) / 2)),
                Class_0042b370(*(last - 1)), comp), comp);
        if (last - _M <= _M - first)
            FUN_00432d40(_M, last, comp, 0), last = _M;
        else
            FUN_00432d40(first, _M, comp, 0), first = _M;
    }
}
