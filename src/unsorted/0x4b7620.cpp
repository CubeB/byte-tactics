// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Registers a command handler in the file-local sorted table (the global vector
// DAT_0051fc99, allocator byte at +0, _First/_Last/_End at +4/+8/+0xc, whose
// out-of-line insert is 0x4b7b00 and whose element copy constructor is
// 0x4b7e30): a lower_bound binary search by name, case-insensitively, then
// overwrite the handler slot if the name is already there, otherwise insert a
// new {name, 0, 0} element at the search position and write that slot. The
// lookup counterpart is 0x4b7900, and 0x4b7760 is this body inlined into a
// record loop (it has the same shape, including the same inlined strcmp).
//
// Three idioms are needed to get the register allocation and the two
// comparison sequences right, and all three are load bearing:
//   * the search comparison has to be a `thiscall` functor call
//     (NameLess_004b7620::operator(), so its result lands in ecx) rather than a
//     bool or int local: a local costs a register, MSVC then puts the loop
//     bound in ebp and the key in ebx, and the original has them the other way
//     round (first/last/key in esi/ebx/ebp);
//   * the second test has to be `!(a == b)` with `operator==` inlined, not
//     `strcmp(...) != 0`: the `!` has to sit on a call result, or MSVC folds it
//     to a single setne instead of the original's
//     `xor ecx, ecx; test eax, eax; sete cl; neg cl; sbb ecx, ecx; inc ecx;
//     test cl, cl` (0 or -1 plus 1 is MSVC's `!x` on a value it cannot fold);
//   * the temporary element's two handler words are cleared with
//     `e.h = HandlerSlot_004b7900();`. Any other zeroing (`h.fn = 0; h.mask = 0`
//     in the element constructor, a nested default constructor, a
//     `static inline` clear helper) lets MSVC hoist one `xor ebx, ebx` above
//     the inlined strcmp, which then needs ebx as its zero register and is
//     emitted in the other form (`cmp dl, [edi]` / `cmp cl, bl` instead of
//     `mov bl, [edi]` / `test cl, cl`).
//
// Still differs (68.9%, 331 bytes against 319), in four places:
//   1. the reload of _Last for the "past the end" test is a memory operand
//      (`cmp esi, DAT+8`) where the original loads it into ebx, the register
//      the loop bound just died in;
//   2. the temporary's clear goes through a stack temporary ($T...) and two
//      load/store pairs where the original has `xor edi, edi; xor ebx, ebx`
//      before the name copy constructor and two register stores after it;
//   3. MSVC 5 sinks the element-index division below the insert call and then
//      divides a second time (`begin()[(first - begin()) / 12]` becomes
//      `begin()[((first - begin()) / 12) / 12]`, which is also wrong at run
//      time); the original computes `(first - _First) / 12` once before the
//      call, keeps it in edi, and does `lea edx, [edi + edi*2]; lea esi,
//      [eax + edx*4 + 4]` after it. No phrasing tried (index before or after
//      the temporary, pointer arithmetic instead of indexing, passing
//      begin() + index as the insert position, a named begin() after the call)
//      keeps the division above the call;
//   4. the two final stores are emitted mask first, the original does fn
//      first with each argument loaded just before its own store.
#include <string.h>

// Release of the reference-counted string handle (0x4c9390).
class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

// Copy constructor of the handle (0x4c91a0), and the inlined equality test
// the second comparison in the function below goes through.
class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);

    bool operator==(const Class_004c91a0& other) const
    {
        return strcmp(data, other.data) == 0;
    }
};

// Constructor of the handle from a C string (0x4c91b0).
class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

// Case-insensitive "sorts before", the ordering the table is kept in.
struct NameLess_004b7620 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

// Case-sensitive "is a different name", tested after the search.
struct NameNe_004b7620 {
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

// One table entry: the name handle, then the handler and its mask.
struct Elem_004b75d0 {
    Class_004c91a0 name;               // +0x0
    HandlerSlot_004b7900 h;            // +0x4

    Elem_004b75d0(const Class_004c91a0& n) : name(n) {}
    ~Elem_004b75d0() { name.FUN_004c9390(); }
};

// The table: std::vector<Elem_004b75d0> with its insert emitted out of line.
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

// FUNCTION: 0x4b7620
void __stdcall FUN_004b7620(const char* name, Handler_004b7900 fn, int mask)
{
    Class_004c91b0 key(name);
    NameLess_004b7620 less;
    Elem_004b75d0* first = DAT_0051fc99.begin();
    Elem_004b75d0* last = DAT_0051fc99.end();
    const char* k = key.data;
    while (first != last) {
        Elem_004b75d0* mid = first + (last - first) / 2;
        if (less(mid->name.data, k))
            first = mid + 1;
        else
            last = mid;
    }
    HandlerSlot_004b7900* slot;
    if (first == DAT_0051fc99.end() || NameNe_004b7620()(first->name, key)) {
        Elem_004b75d0 e(key);
        e.h = HandlerSlot_004b7900();
        int index = (first - DAT_0051fc99.begin()) / 12;
        DAT_0051fc99.FUN_004b7b00(first, 1, e);
        slot = &DAT_0051fc99.begin()[index].h;
    } else {
        slot = &first->h;
    }
    slot->fn = fn;
    slot->mask = mask;
}
