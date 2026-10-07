// Decompiled by Opus. Names are provisional.
// A lazily created singleton: a vector-like container (an empty allocator,
// copied from an uninitialised default-argument temporary, then four zeroed
// dwords) allocated with a class operator new that calls GlobalAlloc.
#include <windows.h>
#include <memory>

template <class T, class A = std::allocator<T> >
class Container_004da9f0 {
public:
    explicit Container_004da9f0(const A& al = A())
        : allocator(al), field_4(0), field_8(0), field_c(0), field_10(0) {}
    static void* operator new(size_t size) { return GlobalAlloc(0, size); }

    A allocator;       // +0x0
    int field_4;       // +0x4
    int field_8;       // +0x8
    int field_c;       // +0xc
    int field_10;      // +0x10
};

extern Container_004da9f0<int>* DAT_00528a48;

// FUNCTION: 0x4da9f0
Container_004da9f0<int>* GetFreedBlockRing()
{
    if (DAT_00528a48 == 0) {
        DAT_00528a48 = new Container_004da9f0<int>;
    }
    return DAT_00528a48;
}
