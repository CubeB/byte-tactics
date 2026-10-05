// Decompiled by Opus. Names are provisional.
// push_heap's sift-up (the STL _Push_heap shape) for the heap of
// FUN_0040d670: moves parents down while they are less than val.

struct Entry_40d670 {
    Entry_40d670() {}
    Entry_40d670(const Entry_40d670& o) : value(o.value), key(o.key) {}
    int value;                       // +0x0
    float key;                       // +0x4
};

static inline bool operator<(const Entry_40d670& a, const Entry_40d670& b)
{
    return a.key < b.key;
}

// FUNCTION: 0x40d740
void __stdcall FUN_0040d740(Entry_40d670* first, int hole, int top, Entry_40d670 val)
{
    for (int idx = (hole - 1) / 2; top < hole && first[idx] < val; idx = (hole - 1) / 2) {
        first[hole] = first[idx];
        hole = idx;
    }
    first[hole] = val;
}
