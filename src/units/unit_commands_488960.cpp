// Decompiled by Opus. Names are provisional.
// std::sort's _Unguarded_partition for a vector of item pointers ordered by
// FUN_00485940, from a file compiled with __stdcall as the default.

struct Item_00485940;

typedef int (__stdcall* Pred_00488960)(Item_00485940*, Item_00485940*);

// FUNCTION: 0x488960
Item_00485940** __stdcall FUN_00488960(Item_00485940** first, Item_00485940** last,
                                        Item_00485940* pivot, Pred_00488960 pred)
{
    for (;; ++first) {
        for (; pred(*first, pivot); ++first)
            ;
        for (; pred(pivot, *--last);)
            ;
        if (last <= first)
            return first;
        Item_00485940* t = *first;
        *first = *last;
        *last = t;
    }
}
