// Decompiled by space-bunny-free. Names are provisional.

// Element type: 0x249 bytes with a name string at +0x20 (see 0x42db60.cpp).
class Class_0042b370 {
public:
    char unknown_0[0x249];
    Class_0042b370& operator=(const Class_0042b370& src);
};

typedef int (__stdcall* Compare)(const Class_0042b370&, const Class_0042b370&);

static Class_0042b370* __inline Copy_backward(Class_0042b370* F, Class_0042b370* L, Class_0042b370* X)
{
    while (F != L)
        *--X = *--L;
    return X;
}

static void __inline Unguarded_insert(Class_0042b370* L, Class_0042b370 V, Compare P)
{
    for (Class_0042b370* M = L; P(V, *--M); L = M)
        *L = *M;
    *L = V;
}

// FUNCTION: 0x432fb0
void __stdcall FUN_00432fb0(Class_0042b370* first, Class_0042b370* last, Compare comp, void* tag)
{
    if (first != last)
        for (Class_0042b370* M = first; ++M != last; ) {
            Class_0042b370 V = *M;
            if (!comp(V, *first))
                Unguarded_insert(M, V, comp);
            else {
                Copy_backward(first, M, M + 1);
                *first = V;
            }
        }
}
