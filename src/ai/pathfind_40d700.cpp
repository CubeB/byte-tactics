// Decompiled by Sonnet. Names are provisional.

struct Entry_40d670 {
    Entry_40d670() {}
    Entry_40d670(const Entry_40d670& o) : value(o.value), key(o.key) {}
    int value;                       // +0x0
    float key;                       // +0x4
};

void __stdcall AdjustHeap(Entry_40d670* first, int hole, int len, Entry_40d670 val);

// FUNCTION: 0x40d700
void __stdcall PopHeapFirst(Entry_40d670* first, Entry_40d670* last, Entry_40d670* dest,
                             Entry_40d670 val, void* unused)
{
    *dest = *first;
    AdjustHeap(first, 0, last - first, val);
}
