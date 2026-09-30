// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL 80.1%, 799 of 798 bytes. The insertion pointer and fill
// counter occupy different registers/stack homes in the reallocating branch.
// 768 header combinations and allocator construct specializations, including
// static cdecl/stdcall/fastcall forms, did not improve the saved implementation.
// The copy assignment's base must remain a struct to preserve its U mangling.
//
// The previous copy-on-itself diagnosis was incorrect. In the prefix loop,
// ebx advances from the new buffer start to its prefix finish (0x46f881).
// esi = ebx + inserted bytes is therefore the correct suffix destination.
// edi is the old-buffer source: the transformations at 0x46f8ca cancel back
// to its prior value. At 0x46f8d4/0x46f8d5 the constructor receives edi as
// its source argument and esi in ecx as its destination. The suffix is
// constructed in the new buffer, not copied onto itself.
#include <vector>

struct Class_0046eaa0 {                // 0x5c bytes, the base subobject
public:
    char unknown_0[0x5c];

    Class_0046eaa0& operator=(const Class_0046eaa0& rhs);
};

class Class_0046ded0 : public Class_0046eaa0 {   // 0x5c bytes, no members
public:
    Class_0046ded0(const Class_0046ded0& other);
    ~Class_0046ded0();

    Class_0046ded0& operator=(const Class_0046ded0& rhs)
    {
        return (Class_0046ded0&)Class_0046eaa0::operator=(rhs);
    }
};

// One indirect call, so insert is emitted as its own out-of-line copy.
typedef void (std::vector<Class_0046ded0>::*InsertFn_0046f7a0)(Class_0046ded0*,
                                        std::vector<Class_0046ded0>::size_type,
                                        const Class_0046ded0&);

void __cdecl FUN_0046f7b0(std::vector<Class_0046ded0>* v, Class_0046ded0* p,
                  std::vector<Class_0046ded0>::size_type n, const Class_0046ded0& x)
{
    InsertFn_0046f7a0 f = &std::vector<Class_0046ded0>::insert;
    (v->*f)(p, n, x);
}

// FUNCTION: 0x46f7a0 ?insert@?$vector@VClass_0046ded0@@V?$allocator@VClass_0046ded0@@@std@@@std@@QAEXPAVClass_0046ded0@@IABV3@@Z
