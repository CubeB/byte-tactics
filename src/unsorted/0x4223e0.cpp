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
// deepseek-v4.1-flash retry (v31..v49): the struct-with-vector-member guess
// shifts _First to +8 (this VC5 vector puts an empty allocator member at +0),
// so the global really is a plain std::vector<Class_004c2ea0*>*. None of these
// spellings removed the ebx zero or moved the vector into edi: `delete
// DAT_00511fb4;` / `delete v;` / a guard `if (v) { v->~vector(); operator
// delete(v); }` / the unguarded `v->~vector(); ::operator delete(v);` (63.5%,
// best) / `__stdcall` / a helper for the final store / int, unsigned,
// (void*)0, unsigned-cast and void*-typed global declarations / `memset(&
// DAT_00511fb4,0,4)` / store-before-delete / a named local kept live across
// the call. The delete forms all reproduce the original's `test/mov edi` guard
// but re-materialise the zero in ebx (62.1%); the unguarded explicit-destructor
// form drops the guard but keeps the same zero (63.5%). So the residual is one
// thing only: the original had the final `DAT_00511fb4 = 0` and the three
// member zero stores as distinct constant nodes that did not join into a
// register. Scratch variants v31..v49 in build/scratch/0x4223e0.
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
