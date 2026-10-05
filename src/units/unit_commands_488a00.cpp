// Decompiled by Opus. Names are provisional.
// A file-local global std::vector: the compiler generates its initialiser
// (0x4889d0) and the destructor it registers with atexit (0x488a00).
// Same shape as 0x434a30.cpp: an 8-byte element whose destructor releases
// the reference-counted string at +0 through ReleaseRef.
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

// FUNCTION: 0x488a00 _$E3
static std::vector<UnitCategory> DAT_0051e6b0;
