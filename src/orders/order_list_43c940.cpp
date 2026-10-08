// Decompiled by Opus. Names are provisional.
// Same shape as the STL's _Unguarded_insert (insertion-sort inner loop) for
// 25-byte records, with a __stdcall comparison callback; the whole sort
// family around it (0x43c6b0 lower_bound, 0x43c720, ...) is __stdcall.

#pragma pack(push, 1)
struct Elem_0043c940 {
    char data[0x19];
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043c940)(const Elem_0043c940&, const Elem_0043c940&);

// FUNCTION: 0x43c940
void __stdcall InsertShiftOrderTypes(Elem_0043c940* last, Elem_0043c940 value, Pred_0043c940 pred)
{
    for (Elem_0043c940* m = last; pred(value, *--m); last = m)
        *last = *m;
    *last = value;
}
