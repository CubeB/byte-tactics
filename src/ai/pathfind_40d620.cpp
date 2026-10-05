// Decompiled by Opus. Names are provisional.

struct Entry_40d670 {
    Entry_40d670() {}
    Entry_40d670(const Entry_40d670& o) : value(o.value), key(o.key) {}
    int value;                       // +0x0
    float key;                       // +0x4
};

void __stdcall AdjustHeap(Entry_40d670* first, int hole, int len, Entry_40d670 val);

// make_heap over [first, last): the STL _Make_heap shape.
// FUNCTION: 0x40d620
void __stdcall MakeHeap(Entry_40d670* first, Entry_40d670* last, int*, Entry_40d670*)
{
    int n = last - first;
    for (int h = n / 2; 0 < h; ) {
        --h;
        AdjustHeap(first, h, n, Entry_40d670(first[h]));
    }
}
