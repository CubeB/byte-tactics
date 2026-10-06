// Decompiled by Opus. Names are provisional.
// The compiler-generated initialiser of a file-local global std::vector
// (its atexit destructor is 0x488a00, see 0x488a00.cpp; same shape as
// map_list.cpp). The element is 8 bytes and its destructor releases the
// reference-counted string at +0 through ReleaseRef.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct UnitCategory {
    Class_004c9390 name;               // +0x0
    int value;                         // +0x4

    ~UnitCategory() { name.ReleaseRef(); }
};

// FUNCTION: 0x4889d0 _$E5
static std::vector<UnitCategory> DAT_0051e6b0;
