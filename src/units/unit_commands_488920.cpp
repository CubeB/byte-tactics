// Decompiled by Opus. Names are provisional.
// std::sort's _Unguarded_insert for a vector of item pointers ordered by
// ComparePlayers, from a file compiled with __stdcall as the default
// (compare 0x488960).

struct Item_00485940;

typedef int (__stdcall* Pred_00488920)(Item_00485940*, Item_00485940*);

// FUNCTION: 0x488920
void __stdcall FUN_00488920(Item_00485940** last, Item_00485940* value, Pred_00488920 pred)
{
    for (Item_00485940** m = last; pred(value, *--m); last = m)
        *last = *m;
    *last = value;
}
