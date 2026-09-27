// Decompiled by Opus. Names are provisional.
// Same shape as the STL's vector::_Ufill for 48-byte elements (placement
// copies of one value into raw storage), compiled with __stdcall as the
// default convention; its _Ucopy is 0x475880.

struct Elem48_00476710 {
    int dwords[12];                    // 0x30 bytes
};

// FUNCTION: 0x476710
void __stdcall FUN_00476710(Elem48_00476710* first, unsigned int n, const Elem48_00476710& value)
{
    for (; 0 < n; --n, ++first) {
        if (first)
            *first = value;
    }
}
