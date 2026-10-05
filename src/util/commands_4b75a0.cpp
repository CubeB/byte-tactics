// Decompiled by Opus. Names are provisional.
// A file-local global std::vector: the compiler generates its initialiser
// (0x4b75a0) and the destructor it registers with atexit (0x4b75d0, see
// 0x4b75d0.cpp), the same shape as 0x434a30 with a 12-byte element.
//
// The element's destructor releases the reference-counted string at +0
// through ReleaseRef.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct CommandEntry {
    Class_004c9390 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    ~CommandEntry() { name.ReleaseRef(); }
};

// FUNCTION: 0x4b75a0 _$E5
static std::vector<CommandEntry> s_commandTable;
