// Decompiled by Opus. Names are provisional.
// std::_Destroy(std::vector<Elem_00434020>*) from MSVC 5's <xmemory>: runs
// the vector's inlined destructor. Byte-identical to allocator::destroy
// (0x434400) but called without ecx and ending in `ret 4`, so this file was
// compiled with __stdcall as the default; written as an explicit __stdcall
// function. Its caller is the destroy loop of 0x434360.
#include <vector>

struct Elem_00434020 {
    int value;                         // +0x0
};

typedef std::vector<Elem_00434020> Inner_00434440;

// FUNCTION: 0x434440
void __stdcall FUN_00434440(Inner_00434440* p)
{
    std::_Destroy(p);
}
