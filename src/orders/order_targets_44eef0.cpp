// Decompiled by Opus. Names are provisional.
// std::copy for 4-byte elements (two shorts), compiled with __stdcall as the
// default convention. Its one caller (0x44da00) is an inlined
// vector::erase(begin(), end()) whose _Destroy is 0x44ee60. Not a member of
// the Class_0044ef20 family next to it: it takes no `this`.

struct Point_0044eef0 {
    short x;
    short y;
};

// FUNCTION: 0x44eef0
Point_0044eef0* __stdcall FUN_0044eef0(Point_0044eef0* first, Point_0044eef0* last, Point_0044eef0* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
