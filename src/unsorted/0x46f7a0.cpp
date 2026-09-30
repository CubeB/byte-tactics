// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// GPT-6.1-sol refinement: six invocations kept 80.1%, including one compile
// failure. Three assignment-operator variants did not improve the best source.
// No MATCH was reached.
// PARTIAL 80.1%, 799 of 798 bytes, and every remaining difference is one
// register-allocation decision, described exactly below.
// 768 header combinations and allocator construct specializations, including
// static cdecl/stdcall/fastcall forms, did not improve the saved implementation.
// The copy assignment's base must remain a struct to preserve its U mangling.
//
// The single extra byte is at 0x46f94e: the original reloads the count from
// its incoming argument slot and adds (`mov eax,[esp+0x24]; add eax,edx`, 2 +
// 2 bytes) where this file keeps the count in a register and uses
// `lea eax,[ecx+edx]` (3 bytes). Both compute _Last = _S + size() + _M.
//
// Root cause, narrowed by an instruction-by-instruction alignment of both
// bodies: at the new[] call all four callee-saved registers are spoken for
// (esi and ebx walk the prefix, ebp holds the count, edi/ecx hold the
// insertion pointer), so exactly one of the two values must live in memory,
// and C1 picks opposite ones.
//   original: p is loaded once after new[] into edi (0x46f85e) and never
//     reloaded, while the inlined _Ufill counter _N gets a stack home in the
//     first argument's slot: `mov [esp+0x20], ebp` (0x46f88e), reload at
//     0x46f8a2, store back at 0x46f8aa. That slot is free because p is already
//     in edi.
//   ours: the argument slot stays p's home, reloaded at 0x46f87c inside the
//     prefix loop and again at 0x46f8aa, and the _Ufill counter is copied
//     into edi instead of the slot.
// Everything downstream follows from that: the suffix loop then walks the
// source in esi and the destination in edi, the mirror image of the original's
// 0x46f8d0 loop, and the epilogue reloads the count into ecx rather than eax.
//
// The previous copy-on-itself diagnosis was incorrect. In the prefix loop,
// ebx advances from the new buffer start to its prefix finish (0x46f881).
// esi = ebx + inserted bytes is therefore the correct suffix destination.
// edi is the old-buffer source: the transformations at 0x46f8ca cancel back
// to its prior value. At 0x46f8d4/0x46f8d5 the constructor receives edi as
// its source argument and esi in ecx as its destination. The suffix is
// constructed in the new buffer, not copied onto itself.
//
// Deleting Class_0046ded0::operator= so the implicit one (a bare call to the
// base's) is emitted instead scores far worse: the prologue then stores a
// different `this` and the whole body shifts.
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
