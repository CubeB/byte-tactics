// Decompiled by Opus. Names are provisional.
// Same shape as the STL's _Unguarded_partition (the quicksort partition
// step) for 25-byte records with a __stdcall comparison callback, like its
// sibling 0x43c940: returns the first element of the upper partition.

#pragma pack(push, 1)
struct Elem_0043cb20 {
    char data[0x19];
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043cb20)(const Elem_0043cb20&, const Elem_0043cb20&);

// FUNCTION: 0x43cb20
Elem_0043cb20* __stdcall PartitionOrderTypes(Elem_0043cb20* first, Elem_0043cb20* last,
                                       Elem_0043cb20 pivot, Pred_0043cb20 pred)
{
    for (;; ++first) {
        for (; pred(*first, pivot); ++first)
            ;
        for (; pred(pivot, *--last);)
            ;
        if (last <= first)
            return first;
        Elem_0043cb20 tmp = *first;
        *first = *last;
        *last = tmp;
    }
}
