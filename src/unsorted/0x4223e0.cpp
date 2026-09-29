// Decompiled by Opus, finished by deepseek-v4.1-flash. Names are provisional.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself.
//
// Best so far (62.1%): the loop and the delete guard match exactly. What still
// differs is the inlined ~vector epilogue: the original materialises &_First
// and &_Last into esi/ebx before the deallocate call and stores immediates
// (lea esi,[eax+4]; lea ebx,[eax+8]; mov [esi],0; mov [ebx],0; [edi+0xc],0),
// while this version keeps a zero register in ebx and stores through
// [this+4]/[this+8] (which also turns the delete null test into cmp reg,reg).
// Without the trailing DAT_00511fb4 = 0 the zero register disappears and the
// stores become immediates, so the extra zero use is what makes MSVC coalesce.
// Keeping the trailing store but spelling the delete as
// `DAT_00511fb4->~vector(); operator delete(DAT_00511fb4);` scores 62.8% but
// loses the original's `test eax,eax; mov edi,eax` delete guard, so this file
// keeps the `delete` form (62.1%); no phrasing tried (local pointer, while
// loop, explicit destructor, custom vector class) produced both the guard and
// the lea/immediate epilogue. Scratch variants vA..vQ2 in
// build/scratch/0x4223e0. A custom vector class cannot reproduce the `push
// ecx` local: it comes from the real <vector>'s two-argument
// allocator.deallocate spill, so the real header is required.
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
    delete DAT_00511fb4;
    DAT_00511fb4 = 0;
}
