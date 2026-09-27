// Decompiled by Opus. Names are provisional.
// The compiler-generated initialiser of a file-local global std::vector
// (its atexit destructor is 0x488a00, see 0x488a00.cpp; same shape as
// 0x434a30.cpp). The element is 8 bytes and its destructor releases the
// reference-counted string at +0 through FUN_004c9390.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

struct Elem_00488a00 {
    Class_004c9390 name;               // +0x0
    int value;                         // +0x4

    ~Elem_00488a00() { name.FUN_004c9390(); }
};

// FUNCTION: 0x4889d0 _$E5
static std::vector<Elem_00488a00> DAT_0051e6b0;
