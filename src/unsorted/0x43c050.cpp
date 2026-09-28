// Decompiled by Space Bunny Free. Names are provisional.
#include <string.h>

#pragma pack(push, 1)
struct Elem_0043c050 {
    char unknown_0[0x15];
    char* name;
};
#pragma pack(pop)

typedef unsigned int size_type_0043c050;
typedef int (__stdcall* Pred_0043c050)(const Elem_0043c050& a, const Elem_0043c050& b);

void __stdcall FUN_0043c990(Elem_0043c050* f, Elem_0043c050* l,
                            Pred_0043c050 p, void*);
void __stdcall FUN_0043c720(Elem_0043c050* f, Elem_0043c050* l,
                            Pred_0043c050 p, void*);
Elem_0043c050* __stdcall FUN_0043ca70(Elem_0043c050* dest, Elem_0043c050 a,
                                      Elem_0043c050 b, Elem_0043c050 c,
                                      Pred_0043c050 p);
Elem_0043c050* __stdcall FUN_0043cb20(Elem_0043c050* f, Elem_0043c050* l,
                                       Elem_0043c050 pivot, Pred_0043c050 p);

class Class_0043c360 {
public:
    char unknown_0[0x4];
    int field_4;
    int field_8;
    int FUN_0043c360(void);
};

struct Access_0043c050 {
    char unknown_0[0x4];
    Elem_0043c050* first;
    Elem_0043c050* last;
    Elem_0043c050* end_of_storage;
    void _Destroy(Elem_0043c050* f, Elem_0043c050* l);
    void insert(Elem_0043c050* p, size_type_0043c050 n, const Elem_0043c050& x);
};

extern Access_0043c050 DAT_00512340;
extern Elem_0043c050 DAT_004fd288;

void FUN_00406bf0();
void FUN_00415b20();
void FUN_00406f00();
void FUN_00403180();

int __stdcall LessByName(const Elem_0043c050& a, const Elem_0043c050& b)
{
    return _strcmpi(a.name, b.name) < 0 ? 1 : 0;
}

static void __stdcall InsertRest(Elem_0043c050* g, Elem_0043c050* l)
{
    for (; g != l; ++g) {
        Elem_0043c050 value = *g;
        for (Elem_0043c050* m = g; LessByName(value, *--m); g = m)
            *g = *m;
        *g = value;
    }
}

static void __stdcall CopyElems(Elem_0043c050* f, Elem_0043c050* l,
                                Elem_0043c050* d)
{
    for (; f != l; ++f, ++d)
        if (d) *d = *f;
}


// PARTIAL, 82.0% (734 of 753 bytes). Layout analysis, from the disassembly:
//
// Frame is `sub esp, 0x24` (36) and holds exactly three objects: a 25-byte
// record buffer at frame+0x8 (seen as `lea edi, [esp+0x18]` at 0x43c2b7, where
// four arguments are still pushed, so the buffer is at frame+8, not frame+0x18),
// a 4-byte slot at frame+0x10 written with `ebp` at 0x43c186 and read back at
// 0x43c2aa, and a 4-byte slot at frame+0x14 written with `esi` at 0x43c18f. The
// one 25-byte buffer is shared between the `_Median` destination and the
// insertion-sort value. This file instead emits `sub esp, 0x3c` (60) with the
// record buffer at -56 and `n`/`l` sharing the 4-byte slot at -60, so the
// missing object is the sort's `first` in a slot of its own. Getting `first`
// to spill is the single blocker: the two pointers are live across the whole
// quicksort loop and MSVC 5 is content to keep both in registers.
//
// Measured negatives:
//  - The real `<algorithm>` is WRONG here and this is worth recording, because
//    the sibling 0x43c3a0 needed the real `<vector>`. `#include <algorithm>`
//    plus `std::sort(v.first, v.last, LessByName)` emits 1376 bytes against the
//    original's 753 (51.3%): MSVC 5 inlines `_Sort`, `_Insertion_sort`,
//    `_Unguarded_partition` and `_Median` at the single call site, and the
//    original has all four out of line at 0x43c720, 0x43c990, 0x43cb20 and
//    0x43ca70. So the hand-written externs for those four are load-bearing and
//    must stay. The out-of-line copies are what the original's translation unit
//    got, and forcing them by hand is the only way to reproduce the shape.
//  - `docs/consolidation.md` says this translation unit was compiled `/Gz /Zp1`
//    and that when files are regrouped it "should get /Gz and the real std::
//    templates instead". Tested directly: compiling the real-`<algorithm>`
//    variant with `--flags /Gz` gives 365 bytes against the original's 753
//    (8.9%). `/Gz` does change the helpers exactly as documented, the emitted
//    symbols become `?_Sort@std@@YGX...` and `?_Unguarded_partition@std@@YGX...`
//    and so on, but MSVC 5 then outlines `_Sort_0` and `_Unguarded_insert` into
//    separate functions where the original inlines both into this caller. So
//    the grouping recommendation in consolidation.md is right about `/Gz` and
//    about the real templates, and is not by itself sufficient: the inline
//    versus outline decision still has to be reproduced and no switch here
//    reproduces it. Recording that so the regrouping phase does not assume
//    `/Gz` plus the real headers is the whole answer.
//  - Merging the two record buffers by passing `&value` into the insert helper
//    gives the right offsets for the value but drops the `first` slot: 75% and
//    a 0x20 frame, one dword short of 0x24. So the buffer is not the problem
//    either; the missing dword really is `first`.
//
// FUNCTION: 0x43c050
void FUN_0043c050()
{
    Access_0043c050& v = DAT_00512340;
    size_type_0043c050 n =
        (size_type_0043c050)(v.first == 0 ? 0 : v.last - v.first) + 1;
    if ((size_type_0043c050)(v.first == 0 ? 0 : v.end_of_storage - v.first) < n) {
        int count = (int)n;
        if (count < 0)
            count = 0;
        Elem_0043c050* s = new Elem_0043c050[count];
        CopyElems(v.first, v.last, s);
        v._Destroy(v.first, v.last);
        delete[] v.first;
        v.end_of_storage = s + n;
        v.last = s + ((Class_0043c360*)&v)->FUN_0043c360();
        v.first = s;
    }
    for (Elem_0043c050* r = &DAT_004fd288; r != &DAT_004fd288 + 1; ++r)
        v.insert(v.last, 1, *r);
    Elem_0043c050* f = v.first;
    Elem_0043c050* l = v.last;
    Elem_0043c050 value;
    Pred_0043c050 p = LessByName;
    if (l - f <= 16)
        FUN_0043c990(f, l, p, (void*)0);
    else {
        do {
            Elem_0043c050* m = FUN_0043cb20(f, l,
                *FUN_0043ca70(&value, *f, *(f + (l - f) / 2), *(l - 1), p), p);
            if (l - m <= m - f)
                FUN_0043c720(m, l, p, (void*)0), l = m;
            else
                FUN_0043c720(f, m, p, (void*)0), f = m;
        } while (16 < l - f);
        FUN_0043c990(f, f + 16, p, (void*)0);
        InsertRest(f + 16, l);
    }
    FUN_00406bf0();
    FUN_00415b20();
    FUN_00406f00();
    FUN_00403180();
}
