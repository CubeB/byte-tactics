// Decompiled by Opus. Names are provisional.
// A file-local global std::vector: the compiler generates its initialiser
// (0x4b75a0) and the destructor it registers with atexit (0x4b75d0, see
// 0x4b75d0.cpp), the same shape as 0x434a30 with a 12-byte element.
//
// The element's destructor releases the reference-counted string at +0
// through FUN_004c9390.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

struct Elem_004b75d0 {
    Class_004c9390 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    ~Elem_004b75d0() { name.FUN_004c9390(); }
};

// FUNCTION: 0x4b75a0 _$E5
static std::vector<Elem_004b75d0> DAT_0051fc99;
