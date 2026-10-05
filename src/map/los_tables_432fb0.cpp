// Decompiled by space-bunny-free. Names are provisional.

// Element type: 0x249 bytes with a name string at +0x20 (see 0x42db60.cpp).
class UnitDef {
public:
    char unknown_0[0x249];
    UnitDef& operator=(const UnitDef& src);
};

typedef int (__stdcall* Compare)(const UnitDef&, const UnitDef&);

static UnitDef* __inline Copy_backward(UnitDef* F, UnitDef* L, UnitDef* X)
{
    while (F != L)
        *--X = *--L;
    return X;
}

static void __inline Unguarded_insert(UnitDef* L, UnitDef V, Compare P)
{
    for (UnitDef* M = L; P(V, *--M); L = M)
        *L = *M;
    *L = V;
}

// FUNCTION: 0x432fb0
void __stdcall FUN_00432fb0(UnitDef* first, UnitDef* last, Compare comp, void* tag)
{
    if (first != last)
        for (UnitDef* M = first; ++M != last; ) {
            UnitDef V = *M;
            if (!comp(V, *first))
                Unguarded_insert(M, V, comp);
            else {
                Copy_backward(first, M, M + 1);
                *first = V;
            }
        }
}
