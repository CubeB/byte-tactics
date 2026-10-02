// Decompiled by Opus, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself.
//
// 30-min checkpoint (Space Bunny Free): best 63.5% (the version in the file).
// check.py runs this session: 17, no new best since the file was written.
// The first half is byte-identical (the loop, the reload of DAT_00511fb4 inside
// the delete branch and the inlined deallocate spill `push ecx;
// mov [esp+0x10],ecx`). What still differs is only the register allocation of
// the inlined ~vector epilogue. Two separate causes, both now pinned down:
//
// (1) THE ZERO CONSTANT. MSVC 5 gives a zero constant a register at 4 or more
//     uses (measured in build/scratch/0x4223e0/c_const*.cpp and c_vec*.cpp:
//     1 or 2 uses stay immediates, 3 uses get a register except in the `delete`
//     shape, where the null test keeps its TST form (`test esi,esi`, no
//     constant use) and the three member stores stay immediates; 4 uses always
//     get a register). The original has only three stores of zero, so its
//     `DAT_00511fb4 = 0` must be a *different* constant node from the
//     destructor's `_First = _Last = _End = 0`. Every integer-typed spelling of
//     that store shares the node (g = 0, 0L, 0u, '\0', *(int**)&g = 0,
//     *(unsigned*)&g = 0, g = (T*)(long)0), so all of them put the zero in a
//     register. WORKING SPELLING FOR THE NODE SPLIT: store a *float* zero,
//     `*(float*)&DAT_00511fb4 = 0.0f;`, which is a separate constant node and
//     still assembles to `mov dword ptr [0x511fb4],0`. With `delete v;` and
//     that store (build/scratch/0x4223e0/z1_float.cpp) all four stores are
//     immediates, the loop's null test is `test esi,esi` as in the original and
//     the global store lands after the pops; it scores 61.9%, so the file keeps
//     the 63.5% spelling. It is the right lead: fix (2) and it should match.
//
// (2) THE VECTOR POINTER'S REGISTER. The original keeps the vector in eax and
//     copies it into edi (`test eax,eax; mov edi,eax; je`), so eax is volatile
//     across `call operator delete` and the two member addresses have to be
//     materialised first (`lea esi,[eax+4]`, `lea ebx,[eax+8]`, then
//     `mov [esi],0`, `mov [ebx],0`, `mov [edi+0xc],0`), which also makes ebx a
//     fourth saved register. Every spelling here instead re-homes the pointer
//     into esi at the top of the block (`mov esi, eax`), and since esi is
//     callee-saved and live across both calls the addresses stay folded into
//     the addressing mode (`mov [esi+4],0`, `mov [esi+8],0`,
//     `mov [esi+0xc],0`): 121 bytes against the original's 127, missing
//     `mov edi,eax`, the two `lea`s and the push/pop of ebx. Tried, none
//     changes it: `delete v`, `delete DAT_00511fb4`, an explicit
//     `v->~vector(); ::operator delete(v);`, an `if (v)` guard, a `static
//     inline` helper taking the pointer, a hand-written vector class with the
//     same layout and destructor, the loop spelled eight ways (for/while/
//     do-while, `!= end`, a named element temp, an explicit `if (*p)`), the
//     vector local declared before the loop, a dead store in a statically
//     folded branch (`int t = 0; if (t) v = 0;` and the same on the global), an
//     `inline Identity(v) { return v; }` wrapper on the epilogue value and on
//     the loop's begin/end, and a permuter run of 12 minutes from the float
//     spelling (build/permute/0x4223e0, no gain from 61.9%). The only spelling
//     that does materialise a member address is reading `v->begin()` in the
//     loop, which puts the pointer in ebx across the loop and changes the
//     loop's bytes.
//
// Earlier notes, kept because they still hold: the real <vector> is required,
// its two-argument allocator.deallocate is the only spelling that produces the
// `push ecx` local (a hand-written class with the same layout loses it and
// scores 59.5%); MSVC 5's xmemory has `deallocate(void*, size_type)` ignore the
// size, which is why `_First` itself is the value pushed for `operator delete`.
// Dropping the final `DAT_00511fb4 = 0` reaches 64.2% but omits an instruction
// the original has, so it is not a solution. BT_TOOLCHAIN=msvc5-rtm gives 62.1%.
#include <vector>

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

extern std::vector<Class_004c2ea0*>* DAT_00511fb4;

// FUNCTION: 0x4223e0
void FUN_004223e0()
{
    for (Class_004c2ea0** p = DAT_00511fb4->begin(); p < DAT_00511fb4->end(); p++)
        delete *p;
    std::vector<Class_004c2ea0*>* v = DAT_00511fb4;
    v->~vector();
    ::operator delete(v);
    DAT_00511fb4 = 0;
}
