// Decompiled by Opus, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself.
//
// Best so far (63.5%): the whole first half matches byte for byte (the loop,
// the reload of DAT_00511fb4 inside the delete branch and the inlined
// deallocate spill `push ecx; mov [esp+0x10],ecx`). What still differs is only
// the register allocation of the inlined ~vector epilogue: the original holds
// the deleted vector in eax (and saves it in edi), tests it with `test eax,eax`,
// computes &_First and &_Last into esi/ebx with `lea` and zeroes the three
// members with immediates. Every version here materialises the shared zero in
// ebx (`xor ebx,ebx` before the loop, so the loop's own `test esi,esi` becomes
// `cmp esi,ebx`) and holds the vector in esi, storing through [esi+4]/[esi+8]/
// [esi+0xc]. The real <vector> is required: its two-argument
// allocator.deallocate is the only spelling that produces the `push ecx` local
// (a hand-written class with the same layout loses it and scores 59.5%).
//
// deepseek-v4.1-flash retry (about 90 scratch files in build/scratch/0x4223e0):
// The residual has one cause: the constant 0 has four uses in the loop's
// region (the loop's own delete null check, the three member stores of the
// inlined ~vector and the final `DAT_00511fb4 = 0`), so MSVC 5 keeps it in
// ebx, which is callee-saved and live across the two operator delete calls.
// Evidence: dropping only the final `DAT_00511fb4 = 0` (scratch u_noglob) makes
// the loop emit `test esi,esi` and the epilogue emit the immediate member
// stores (`mov dword ptr [esi+4], 0`), scoring 64.2%; but that variant omits
// an instruction the original clearly has (0x422452), so it is not a valid
// solution and is not the file. With the store present, every source shape
// tried keeps the zero in a register. The threshold is visible in the scratch
// k3/k4 experiments: with no loop, 3 member stores + the global store still
// use immediates (k4_del1_st3) while 4 member stores do not (k4_del1_st4);
// with the loop, even one member store plus the store does not (v_1).
// Tried and rejected since v31: `delete DAT_00511fb4` / `delete v` with a named
// local / an explicit guard `if (v) { v->~vector(); operator delete(v); }` /
// explicit destructor + operator delete(v) (63.5%, best) / the global store
// first / a reference to the global / comma expressions / int, unsigned,
// void*, char-typed global declarations and `*(int*)&DAT_00511fb4 = 0` /
// `memset(&DAT_00511fb4,0,4)` / an inline helper that deletes / loop forms
// (`while`, `do/while`, `delete *p++`, named element temp, explicit `if (*p)`,
// `if ((int)*p)`, `!= NULL`, `(void*)` compare) / uninitialised locals in both
// declaration orders / static inline wrappers around the whole tail /
// hand-written vector-like classes with the same layout and destructor (they
// reproduce the shared zero but never the `push ecx` deallocate home slot),
// with or without taking the members' addresses / BT_TOOLCHAIN=msvc5-rtm
// (62.1%) / the vector pointer as a local declared before the loop (changes
// the loop's registers).
// Best lead for the next attempt: the original's eax/edi/esi/ebx epilogue
// appears only when the constant 0 is *not* in a register; the immediates
// shape of u_noglob shows the allocation that goes with it. Find a spelling
// where the final store still assembles to `mov dword ptr [0x511fb4], 0` but
// does not share the constant node with the destructor's three member stores.
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
