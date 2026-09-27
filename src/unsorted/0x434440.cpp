// Decompiled by Opus. Names are provisional.
// std::_Destroy(Elem_00434360*) from MSVC 5's <xmemory>, where Elem_00434360
// is a struct holding one std::vector<Elem_00434020>: runs the element's
// inlined destructor, which is the held vector's. Byte-identical to
// allocator<Elem_00434360>::destroy (0x434400) but called without ecx and
// ending in `ret 4`, so this file was compiled with __stdcall as the default;
// written as an explicit __stdcall function. Its caller is the destroy loop
// of 0x434360.
#include <vector>

struct Elem_00434020 {
    unsigned short a;                  // +0x0
    unsigned short b;                  // +0x2
};

struct Elem_00434360 {
    std::vector<Elem_00434020> v;      // +0x0
};

// FUNCTION: 0x434440
void __stdcall FUN_00434440(Elem_00434360* p)
{
    std::_Destroy(p);
}
