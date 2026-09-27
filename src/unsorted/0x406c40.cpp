// Decompiled by Sonnet. Names are provisional.
// Free __stdcall function (no ecx use); broadcasts *src into every element
// of dest[0..count) when dest is non-null (src is never advanced).

// FUNCTION: 0x406c40
void __stdcall FUN_00406c40(int* dest, unsigned int count, int* src)
{
    for (; count > 0; count--) {
        if (dest != 0) {
            *dest = *src;
        }
        dest++;
    }
}
