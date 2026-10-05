// Decompiled by Opus. Names are provisional.
// A file-local global std::vector: the compiler generates its initialiser
// (0x434a30) and the destructor it registers with atexit (0x434a60).
//
// The element is 8 bytes and its destructor releases the reference-counted
// string at +0 through ReleaseRef. The vector must be `static`: for an
// external global MSVC reloads _First after the destroy loop on both paths
// (into eax), while for a static it keeps _First in esi on the empty path.
#include <vector>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void ReleaseRef();
};

struct Elem_00434a60 {
    Class_004c9390 name;               // +0x0
    int value;                         // +0x4

    ~Elem_00434a60() { name.ReleaseRef(); }
};

// FUNCTION: 0x434a30 _$E5
// FUNCTION: 0x434a60 _$E3
static std::vector<Elem_00434a60> DAT_005122c0;
