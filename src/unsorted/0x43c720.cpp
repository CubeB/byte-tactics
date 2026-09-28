// Decompiled by deepseek-v4.1-flash. Names are provisional.
// std::_Sort (the recursive quicksort step of std::sort WITH PRED) from MSVC
// 5's <algorithm>, instantiated for 25-byte records with a __stdcall
// comparison callback. _Median and _Unguarded_partition are inlined here;
// the rest of the family is 0x43c940/0x43c990 (insertion sort), 0x43cb20
// (_Unguarded_partition) and 0x43c6b0 (lower_bound).
#include <algorithm>

#pragma pack(push, 1)
struct Elem_0043c720 {
    char data[0x19];
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043c720)(const Elem_0043c720&, const Elem_0043c720&);

// FUNCTION: 0x43c720
void __stdcall FUN_0043c720(Elem_0043c720* _F, Elem_0043c720* _L,
                            Pred_0043c720 _P, Elem_0043c720*)
{
    for (; std::_SORT_MAX < _L - _F; ) {
        Elem_0043c720* _M = std::_Unguarded_partition(_F, _L,
            std::_Median(*_F, *(_F + (_L - _F) / 2), *(_L - 1), _P), _P);
        if (_L - _M <= _M - _F)
            FUN_0043c720(_M, _L, _P, (Elem_0043c720*)0), _L = _M;
        else
            FUN_0043c720(_F, _M, _P, (Elem_0043c720*)0), _F = _M;
    }
}
