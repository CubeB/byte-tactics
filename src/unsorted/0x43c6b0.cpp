// Decompiled by Opus. Names are provisional.
// std::_Lower_bound(first, last, value, pred, (int*)0) over a sorted table of
// 25-byte entries (the order-type table at 0x512344..0x512348), instantiated
// in a file compiled with __stdcall as the default; the one caller (0x43a513)
// passes the case-insensitive name compare 0x43a940.

#pragma pack(push, 1)
struct Entry_0043c6b0 {
    char unknown_0[0x15];
    char* name;                        // +0x15
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043c6b0)(const Entry_0043c6b0& entry, char* name);

// FUNCTION: 0x43c6b0
Entry_0043c6b0* __stdcall FUN_0043c6b0(Entry_0043c6b0* first, Entry_0043c6b0* last,
                                       char* const& value, Pred_0043c6b0 pred, int*)
{
    int n = 0;
    n += last - first;
    for (; 0 < n; ) {
        int n2 = n / 2;
        Entry_0043c6b0* m = first;
        m += n2;
        if (pred(*m, value))
            first = ++m, n -= n2 + 1;
        else
            n = n2;
    }
    return first;
}
