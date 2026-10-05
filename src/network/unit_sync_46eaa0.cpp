// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// std::vector<Class_0046eaa0>::_Destroy(first, last) from MSVC 5's <vector>,
// called from the inlined erase at 0x46db82 (0x46dad0) with ecx set to the
// vector. It runs each 0x5c-byte element's implicit destructor, which frees
// the element's four std::vector members last-first.
// The element's layout comes from its operator= (0x470040): list_a and list_b
// use the vector<Elem_004702a0> helpers (0x470250, 0x470270, 0x470290,
// 0x4702a0), the three dwords at +0x24 are copied one by one, and the member
// at +0x30 is a struct with its own out-of-line operator= (0x470560), which
// inlines list_c's operator= (calling 0x46faf0, 0x46e870 and the 14-byte
// std::copy 0x470a40) and calls list_d's out of line (0x4707a0, 14-byte
// elements too). That nesting is what the inline budget needs: list_c's
// _Destroy stays out of line (0x46e870) while list_d's is inlined.
// FUN_00470030 is std::_Destroy for the 14-byte element, compiled with
// __stdcall as the default (/Gz, see docs/consolidation.md), which /Ob2 did
// not inline at this depth. The explicit specialisation of
// allocator<Elem_0046faf0>::destroy below calls it directly, standing in for
// the /Gz template (an inline std::_Destroy overload would be one inline
// level too deep here and stay a call).
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
