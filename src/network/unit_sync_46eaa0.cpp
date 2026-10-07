// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// std::vector<Class_0046eaa0>::_Destroy(first, last) from MSVC 5's <vector>,
// called from the inlined erase at 0x46db82 (0x46dad0) with ecx set to the
// vector. It runs each 0x5c-byte element's implicit destructor, which frees
// the element's four std::vector members last-first.
#include <vector>

#pragma pack(push, 2)
struct Elem_0046faf0 {
    int a;                             // +0x0
    int b;                             // +0x4
    int c;                             // +0x8
    short d;                           // +0xc
};
#pragma pack(pop)

void __stdcall FUN_00470030(int);

namespace std {
template<> inline void allocator<Elem_0046faf0>::destroy(Elem_0046faf0* p)
{
    // Direct call: an inline std::_Destroy overload would be one inline
    // level too deep.
    FUN_00470030((int)p);
}
}

struct Elem_004702a0 {
    int unknown_0;
};

struct PacketSequencer {               // operator= is 0x470560
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    // Nested struct with its own out-of-line operator=: sets the inline depth
    // that keeps list_c's _Destroy out of line and list_d's inlined.
    std::vector<Elem_0046faf0> list_c; // +0x0c
    std::vector<Elem_0046faf0> list_d; // +0x1c (operator= 0x4707a0)
};

struct Class_0046eaa0 {                // operator= is 0x470040
    int field_0;                       // +0x00
    std::vector<Elem_004702a0> list_a; // +0x04
    std::vector<Elem_004702a0> list_b; // +0x14
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    PacketSequencer sub;               // +0x30
};

typedef std::vector<Class_0046eaa0> Vec_0046eaa0;
typedef void (Vec_0046eaa0::*DestroyFn_0046eaa0)(Vec_0046eaa0::iterator, Vec_0046eaa0::iterator);

struct Access_0046eaa0 : Vec_0046eaa0 {
    static DestroyFn_0046eaa0 fn;
};

// FUNCTION: 0x46eaa0 ?_Destroy@?$vector@UClass_0046eaa0@@V?$allocator@UClass_0046eaa0@@@std@@@std@@IAEXPAUClass_0046eaa0@@0@Z
DestroyFn_0046eaa0 Access_0046eaa0::fn = &Access_0046eaa0::_Destroy;
