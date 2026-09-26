// Decompiled by Opus. Names are provisional.

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

void __stdcall FUN_0040d740(Entry_40d670* first, int hole, int top, Entry_40d670 val);

// FUNCTION: 0x40d670
void __stdcall FUN_0040d670(Entry_40d670* first, int hole, int len, Entry_40d670 val)
{
    int top = hole;
    int k = 2 * hole + 2;

    while (k < len) {
        if (first[k] < first[k - 1])
            k--;
        first[hole] = first[k];
        hole = k;
        k = 2 * k + 2;
    }
    if (k == len) {
        first[hole] = first[k - 1];
        hole = k - 1;
    }
    FUN_0040d740(first, hole, top, val);
}
