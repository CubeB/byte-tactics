// Decompiled by Opus. Names are provisional.
// Same shape as the STL's uninitialized_copy (vector::_Ucopy) for 14-byte
// elements, compiled with __stdcall as the default convention.

#pragma pack(push, 2)
struct Elem14_46faf0 {
    int dwords[3];
    short word_c;
};
#pragma pack(pop)

// FUNCTION: 0x46faf0
Elem14_46faf0* __stdcall FUN_0046faf0(Elem14_46faf0* first, Elem14_46faf0* last, Elem14_46faf0* dest)
{
    for (; first != last; ++first, ++dest) {
        if (dest)
            *dest = *first;
    }
    return dest;
}
