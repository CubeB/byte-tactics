// Decompiled by Opus. Names are provisional.
// std::copy<unsigned short*, unsigned short*>(first, last, dest) from MSVC
// 5's <xutility>, called out of line by the inlined
// vector<unsigned short>::erase in 0x424c00 (at 0x424e86).
#include <vector>

namespace std {
// FUNCTION: 0x4256a0 ?copy@std@@YGPAGPAG00@Z
// Explicit __stdcall specialization: the original instantiated it under /Gz.
template <>
unsigned short* __stdcall copy(unsigned short* first, unsigned short* last, unsigned short* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
}
