// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// 73.2 percent, 357 bytes against 370. This is 0x4b7620 (the out-of-line
// register) inlined into a record loop: same hand-rolled vector, same
// lower_bound by name, same inlined strcmp equality test, same
// insert-at-the-search-position, but iterating an array of
// {const char* name, handler, mask} records whose first null name ends the
// loop, and writing the record's handler slot into the table entry.
//
// What this pass changed (all four moves are load bearing, in this order):
//   * the hand-rolled Class_004b7b00 vector of 0x4b7620 replaces std::vector,
//     which is what puts _First in esi, _Last in ebx and the key in ebp;
//   * the record's handler slot is copied into a local BEFORE the key handle
//     is constructed, and MASK BEFORE FN. That is the only spelling that
//     reproduces the original's `mov edx,[ecx+8]; mov ecx,[ecx+4]; mov
//     [esp+0x14],ecx` (the high word of an 8-byte copy is loaded first, the
//     stores still go in ascending order). Declaring the copy after the
//     handle ctor makes MSVC reload rec->h past the call; a struct
//     initialiser (`HandlerSlot h = rec->h;`) makes it materialise the
//     address of h and spills that instead, and costs 6 points;
//   * the search comparison needs a NAMED bool local. Written inline,
//     `if (_strcmpi(a,b) < 0)` if-converts to a bare `jge` (48 percent);
//   * the element index is `(first - begin()) / 2`, the same idiom as the
//     search's midpoint, NOT `/ 12` and not `sizeof`. `/ 12` costs 5 points
//     and is also what makes MSVC sink the division below the insert.
//
// Still differs, in four places, all downstream of one allocator state:
//   1. the temporary element's two handler words. The original has
//      `xor edi,edi; xor ebx,ebx` before the name copy constructor and
//      `mov [esp+0x28],edi; mov [esp+0x30],ebx` after it, i.e. the zeros are
//      live across the call and live in the two callee-saved registers the
//      search loop has just vacated. This file forwards the record's
//      h.fn/h.mask into the temporary instead (2 loads where the original
//      has 2 xors). Nothing tried recovers the xors: zeroing in the element
//      constructor (`: h()`) makes MSVC drop the stores altogether, a
//      separate zeroed local assigned after the constructor still gets
//      forwarded, field-wise `e.h.fn = 0; e.h.mask = 0` is worse (63
//      percent), and a `static inline` clearer is the same as the plain one.
//      0x4b7620 hits the identical wall, so this is a shared MSVC 5
//      behaviour, not a modelling mistake here;
//   2. that index division. The original computes it between the copy
//      constructor and the insert, keeps it in edi across the call and does
//      `lea edx,[edi+edi*2]; lea esi,[eax+edx*4+4]` after reloading _First.
//      MSVC 5 always sinks it and re-derives it from `first` after the call.
//      Routing the index through a `static inline` helper, naming `begin()`,
//      moving the computation before the temporary, and computing it after
//      the call were all tried; only feeding the index to the insert call
//      itself (`FUN_004b7b00(begin()+index, 1, e)`) lifts it above the call,
//      and that then rebuilds the position argument and loses more;
//   3. the search comparison's bool lands in ecx (`xor ecx,ecx; setl cl`)
//      where the original has edx (`xor edx,edx; setl dl`). A thiscall
//      functor for the comparison does not help: its inlined result is also
//      ecx, which is what 0x4b7620 wants and this function does not;
//   4. the "past the end" test uses a memory operand
//      (`cmp esi, DAT_0051fca1`) where the original loads _Last into ebx,
//      the register the loop bound has just died in. Same as 0x4b7620's
//      point 1.
//
// Suspected original bug: none found. The `/ 2` in the midpoint and index
// expressions is a Cavedog idiom that only works because the element is 12
// bytes, and it is the same idiom in the already matched neighbours.
#include <string.h>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);

    bool operator==(const Class_004c91a0& other) const
    {
        return strcmp(data, other.data) == 0;
    }
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

struct NameLess_004b7760 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

struct NameNe_004b7760 {
    bool operator()(const Class_004c91a0& a, const Class_004c91a0& b) const
    {
        return !(a == b);
    }
};

typedef void (__stdcall *Handler_004b7900)(void*);

struct HandlerSlot_004b7900 {
    Handler_004b7900 fn;               // +0x4
    int mask;                          // +0x8
};

struct Elem_004b75d0 {
    Class_004c91a0 name;               // +0x0
    HandlerSlot_004b7900 h;            // +0x4

    Elem_004b75d0(const Class_004c91a0& n) : name(n), h() {}
    ~Elem_004b75d0() { name.FUN_004c9390(); }
};

class Class_004b7b00 {
public:
    char allocator;                    // +0x0
    Elem_004b75d0* _First;             // +0x4
    Elem_004b75d0* _Last;              // +0x8
    Elem_004b75d0* _End;               // +0xc

    Elem_004b75d0* begin() { return _First; }
    Elem_004b75d0* end() { return _Last; }
    void FUN_004b7b00(Elem_004b75d0* pos, int n, const Elem_004b75d0& x);
};

extern Class_004b7b00 DAT_0051fc99;

struct Rec_004b7760 {
    const char* name;                  // +0x0
    HandlerSlot_004b7900 h;            // +0x4
};

// FUNCTION: 0x4b7760
void __stdcall FUN_004b7760(Rec_004b7760* rec)
{
    for (; rec->name; rec++) {
        HandlerSlot_004b7900 h;
        h.mask = rec->h.mask;
        h.fn = rec->h.fn;
        Class_004c91b0 key(rec->name);
        Elem_004b75d0* first = DAT_0051fc99.begin();
        Elem_004b75d0* last = DAT_0051fc99.end();
        const char* k = key.data;
        while (first != last) {
            Elem_004b75d0* mid = first + (last - first) / 2;
            bool lt = _strcmpi(mid->name.data, k) < 0;
            if (lt)
                first = mid + 1;
            else
                last = mid;
        }
        HandlerSlot_004b7900* slot;
        if (first == DAT_0051fc99.end() || NameNe_004b7760()(first->name, key)) {
            Elem_004b75d0 e(key);
            int index = (first - DAT_0051fc99.begin()) / 2;
            DAT_0051fc99.FUN_004b7b00(first, 1, e);
            slot = &DAT_0051fc99.begin()[index].h;
        } else {
            slot = &first->h;
        }
        slot->fn = h.fn;
        slot->mask = h.mask;
    }
}
