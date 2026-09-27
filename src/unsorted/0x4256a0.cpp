// Decompiled by Opus. Names are provisional.
// std::copy<unsigned short*, unsigned short*>(first, last, dest) from MSVC
// 5's <xutility>, called out of line by an inlined vector<unsigned short>
// erase in 0x424e86. It ends in `ret 0xc`, so this file of the original was
// compiled with __stdcall as the default; written here as a __stdcall
// function.

// FUNCTION: 0x4256a0
unsigned short* __stdcall FUN_004256a0(unsigned short* first, unsigned short* last, unsigned short* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
