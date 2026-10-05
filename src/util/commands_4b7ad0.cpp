// Decompiled by Opus. Names are provisional.
// Empties the file-local global vector created by 0x4b75a0 (destroyed by
// 0x4b75d0): the inlined vector::clear() runs each element's destructor.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Elem_004b75d0 {
    Class_004c9390 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    ~Elem_004b75d0() { name.ReleaseRef(); }
};

static std::vector<Elem_004b75d0> DAT_0051fc99;

// FUNCTION: 0x4b7ad0
void FUN_004b7ad0()
{
    DAT_0051fc99.clear();
}
