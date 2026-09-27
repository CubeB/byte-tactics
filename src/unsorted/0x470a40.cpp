// Decompiled by Opus. Names are provisional.
// std::copy for 14-byte elements, compiled with __stdcall as the default
// convention (same shape as 0x4702d0). Its one caller copies one vector's
// elements into another.

#pragma pack(push, 2)
struct Elem_00470a40 {
    int a;
    int b;
    int c;
    short d;
};
#pragma pack(pop)

// FUNCTION: 0x470a40
Elem_00470a40* __stdcall FUN_00470a40(Elem_00470a40* first, Elem_00470a40* last, Elem_00470a40* dest)
{
    for (; first != last; ++dest, ++first)
        *dest = *first;
    return dest;
}
