// Decompiled by Opus. Names are provisional.
// std::copy<unsigned short*, unsigned short*>(first, last, dest) from MSVC
// 5's <xutility>, called out of line by the inlined
// vector<unsigned short>::erase in 0x424c00 (at 0x424e86). It ends in
// `ret 0xc`: this file of the original was compiled with /Gz, so the
// template was instantiated __stdcall. The staged files are built without
// /Gz, so the instantiation is written out here as an explicit __stdcall
// std::copy, which MSVC 5 names like the /Gz instantiation (it warns, C4666,
// that it differs from the template only by calling convention). Renamed in
// #335 from FUN_004256a0.
#include <vector>

namespace std {
// FUNCTION: 0x4256a0 ?copy@std@@YGPAGPAG00@Z
template <>
unsigned short* __stdcall copy(unsigned short* first, unsigned short* last, unsigned short* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
}
