// Decompiled by Haiku. Names are provisional.
// std::vector<Class_004c2ea0*>::_Destroy(first, last) from MSVC 5's <vector>:
// empty, since the element type is trivial. _Destroy is protected, so a
// derived class takes its address to make the compiler emit it out of line.
// Its caller, 0x424c00, inlines FreeFeatureFileList (delete every feature in the
// global vector DAT_00511fb4, then the vector) and calls this with ecx set
// to that vector (renamed in #335 from the free __stdcall FUN_004251e0).
// The same vector's insert is 0x425480 (called from 0x4222e0).
#include <vector>

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

typedef std::vector<Class_004c2ea0*> Vec_004251e0;
typedef void (Vec_004251e0::*DestroyFn_004251e0)(Vec_004251e0::iterator, Vec_004251e0::iterator);

struct Access_004251e0 : Vec_004251e0 {
    static DestroyFn_004251e0 fn;
};

// FUNCTION: 0x4251e0 ?_Destroy@?$vector@PAVClass_004c2ea0@@V?$allocator@PAVClass_004c2ea0@@@std@@@std@@IAEXPAPAVClass_004c2ea0@@0@Z
DestroyFn_004251e0 Access_004251e0::fn = &Access_004251e0::_Destroy;
