// Decompiled by Sonnet. Names are provisional.

struct Pair16 {
    short lo;
    short hi;
};

// FUNCTION: 0x414350
void __stdcall FUN_00414350(Pair16 a, int* out, Pair16 b)
{
    out[0] = (b.lo + a.lo * 2) << 19;
    out[2] = (b.hi + a.hi * 2) << 19;
}
