// Decompiled by Sonnet. Names are provisional.

struct Elem48_475880
{
    int dwords[12]; // 0x30 bytes
};

// FUNCTION: 0x475880
Elem48_475880* __stdcall FUN_00475880(Elem48_475880* first, Elem48_475880* last, Elem48_475880* dest)
{
    for (; first != last; ++first, ++dest) {
        if (dest)
            *dest = *first;
    }
    return dest;
}
