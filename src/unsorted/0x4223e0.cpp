// Decompiled by Opus, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by Claude Opus 5.5. re-verified by GPT-6. Names are provisional.
// Deletes every entry of the global vector at DAT_00511fb4 (see 0x422460),
// then the vector itself. 0x424c00 inlines this whole function (its copy
// calls the vector's _Destroy out of line, 0x4251e0).
//
// Claude Opus 5.5 (#4416): 63.5% -> 65.9%. The epilogue is the inlined
// `delete DAT_00511fb4` (scalar deleting destructor -> ~vector -> _Destroy,
// whose dead `_F` home is the `push ecx` slot; a hand-written vector loses
// that slot). Two structural facts about the original's epilogue, both read
// off the bytes and also true of 0x424c00's inlined copy (0x424f08):
//   (1) the null test is on the loop's DAT register (`test eax,eax`), and the
//       pointer passed to the final operator delete is a separate copy made
//       before the branch (`mov edi,eax`). A plain `delete` (or `delete v`)
//       copies first and tests the copy (`mov esi,eax; test esi,esi`).
//   (2) the destructor's own `this` never gets a register: &_First and &_Last
//       are materialised from eax (`lea esi,[eax+4]`, `lea ebx,[eax+8]`) and
//       only the single-use _End store goes through edi.
// The spelling below (copy into v, test the global, destroy through the
// global, free v) is the only one found that reproduces (1), and with it the
// prologue's `push ebx`, the edi copy and `push edi`. What still differs:
// the destructor's `this` is a variable here (esi, `[esi+4]` etc.) instead of
// the two address registers, and the zero constant takes ebx (`xor ebx,ebx`,
// `cmp esi,ebx`) where the original keeps every 0 an immediate. The zero is
// the second-order effect: in the original ebx holds &_Last, so there is no
// free register left for it.
//
// Measured flat (Claude Opus 5.5, all with check.py's compiler): `delete`
// with a local copy, an `if` guard, a SafeDelete template, a static inline
// helper, an accessor, `delete &*DAT`, a reference to the vector or to the
// pointer, the global as void* with casts, the vector as a member or base of
// a wrapper struct (with or without its own destructor), element types void*,
// int, char*, const T*, the loop as iterator/while/index forms; the real
// <vector> against a line-for-line clone of its destructor chain with
// reference parameters for _Destroy or deallocate; _Destroy out of line as
// 0x424c00 calls it (no lea either, so the lea is not a _Destroy artefact);
// /Gi, /Ob1, /Op, /Oa, /Ow, /G5, /G6, /GX, /Gy and msvc5-rtm; 0 to 240
// unused prototypes in front; the preceding function 0x4222e0 and an
// out-of-line _Destroy defined in the same file; all 8 combinations of
// {v, DAT} for the test, the destructor's object and the freed pointer.
// `*(float*)&DAT_00511fb4 = 0.0f;` (a separate zero constant node) gives
// 68.2% on this spelling by freeing ebx, but it is not plausible source and
// still lacks the two leas. Permuter runs from the 64.3% spelling
// (`v = DAT; if (DAT) { v->~vector(); operator delete(v); }` + float zero,
// 9263 candidates) and from this file (9357 candidates) found nothing, and
// tools/headers.py --cpp (1536 header sets) is flat at 65.9%.
//
// Earlier notes, kept because they still hold: the real <vector> is required,
// its two-argument allocator.deallocate is the only spelling that produces the
// `push ecx` local (a hand-written class with the same layout loses it and
// scores 59.5%); MSVC 5's xmemory has `deallocate(void*, size_type)` ignore the
// size, which is why `_First` itself is the value pushed for `operator delete`.
// MSVC 5 gives a zero constant a register at 4 or more uses; every
// integer-typed spelling of the final store shares the destructor's zero node.
#include <vector>

class Class_004c2ea0 {
public:
    void* data;                        // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004c2ea0();
    ~Class_004c2ea0();
};

typedef std::vector<Class_004c2ea0*> FeatureList;

extern FeatureList* DAT_00511fb4;

// FUNCTION: 0x4223e0
void FUN_004223e0()
{
    for (Class_004c2ea0** p = DAT_00511fb4->begin(); p < DAT_00511fb4->end(); p++)
        delete *p;
    FeatureList* v = DAT_00511fb4;
    if (DAT_00511fb4) {
        DAT_00511fb4->~FeatureList();
        operator delete(v);
    }
    DAT_00511fb4 = 0;
}
