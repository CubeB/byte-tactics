// Decompiled by Opus, finished by deepseek-v4.1-flash. Names are provisional.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself.
//
// Best so far (63.5%, variant v12): the whole first half matches byte for
// byte (the loop, the reload of DAT_00511fb4 inside the delete branch and the
// inlined deallocate spill `push ecx; mov [esp+0x10],ecx`). What still
// differs is only the register allocation of the inlined ~vector epilogue:
// the original holds the deleted vector in eax (and saves it in edi), tests
// it with `test eax,eax`, computes &_First and &_Last into esi/ebx with `lea`
// and zeroes the three members with immediates. This version hoists the
// common zero into ebx (`xor ebx,ebx` before the loop, so the loop's own
// `test esi,esi` becomes `cmp esi,ebx`), holds the vector in esi and stores
// through [esi+4]/[esi+8]/[esi+0xc]. The real <vector> is required: its
// two-argument allocator.deallocate is the only spelling that produces the
// `push ecx` local (a hand-written class with the same layout loses it and
// scores 59.5%). The delete form (`delete DAT_00511fb4;`) keeps the original's
// `test eax,eax; mov edi,eax` guard but scores 62.1%; the explicit
// `v->~vector(); ::operator delete(v);` form used here scores 63.5% but
// drops that guard. Scratch variants v1..v13 in build/scratch/0x4223e0.
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
