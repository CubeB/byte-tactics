// Decompiled by space-bunny-free. Names are provisional.
// A quicksort of a range of item pointers ordered by ComparePlayers through a
// __stdcall function pointer (the same family as 0x488920 and 0x488960). It
// takes the median of the first, middle and last element, partitions the range
// in place around it, then recurses on the smaller half only and loops on the
// larger one. The fourth parameter is never read: both call sites (0x48576d
// and 0x485778, which pass the first level of this sort inline) and the
// recursive calls pass 0.
// The byte count of a range, rounded down to a whole number of pointers.

struct Item_00485940;

typedef int (__stdcall* Pred_00488810)(Item_00485940*, Item_00485940*);

// Stays a helper: the left size is only kept in eax through inlined calls.
static inline int RangeSize_00488810(Item_00485940** low, Item_00485940** high)
{
    return (int)((char*)high - (char*)low) & ~3;
}

void __stdcall FUN_00488810(Item_00485940** first, Item_00485940** last, Pred_00488810 pred, int unused);

// FUNCTION: 0x488810
void __stdcall FUN_00488810(Item_00485940** first, Item_00485940** last, Pred_00488810 pred, int unused)
{
    while (((int)((char*)last - (char*)first) & ~3) > 0x40) {
        int len = (int)((char*)last - (char*)first);
        Item_00485940* a = *first;
        Item_00485940* m = first[(len >> 2) / 2];
        Item_00485940* c = last[-1];
        Item_00485940* pivot;

        if (pred(a, m)) {
            if (pred(m, c))
                pivot = m;
            else if (pred(a, c))
                pivot = c;
            else
                pivot = a;
        } else if (pred(a, c)) {
            pivot = a;
        } else {
            pivot = pred(m, c) ? c : m;
        }

        Item_00485940** j = last;
        Item_00485940** i = first;
        for (;;) {
            while (pred(*i, pivot))
                ++i;
            while (pred(pivot, *--j))
                ;
            if (j <= i)
                break;
            Item_00485940* t = *i;
            *i = *j;
            *j = t;
            ++i;
        }

        if (RangeSize_00488810(i, last) <= RangeSize_00488810(first, i)) {
            FUN_00488810(i, last, pred, 0);
            last = i;
        } else {
            FUN_00488810(first, i, pred, 0);
            first = i;
        }
    }
}
