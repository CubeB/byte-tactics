// Decompiled by Opus. Names are provisional.
// std::copy for 4-byte elements, compiled with __stdcall as the default
// convention (same shape as 0x44eef0). Its one caller copies one vector's
// elements into another.

// FUNCTION: 0x4702d0
int* __stdcall FUN_004702d0(int* first, int* last, int* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
