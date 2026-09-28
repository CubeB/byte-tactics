// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Registers a command handler in the file-local sorted table (the global
// vector created by 0x4b75a0, cleared by 0x4b7ad0, whose out-of-line insert is
// 0x4b7b00): a lower_bound binary search by name, then overwrite the handler
// slot if the name is already present, otherwise insert a new {name, 0, 0}
// element at the search position and write the slot. The lookup counterpart is
// 0x4b7900, and 0x4b7760 is this body inlined into a record loop.
//
// Still differs (best 69.0%): MSVC 5 keeps the loop bound `last` in ebp and
// the search key in ebx, while the original keeps `last` in ebx and the key in
// ebp; it also schedules the index computation after the insert call (saving
// the old _First in edi) instead of before it, zeroes the temporary element
// with eax instead of edi/ebx, and emits the equality test as
// `test eax, eax; jne` instead of the original's
// `xor ecx, ecx; test eax, eax; sete cl; neg cl; sbb ecx, ecx; inc ecx`
// (the `int eq = strcmp(...) == 0; ... eq == 0` shape). No declaration order,
// const, helper signature or loop shape tried moved the ebx/ebp pair; the
// 3-argument helper below scored highest.
#include <string.h>

class Class_004c9390 {
public:
    char* data;                        // +0x0
    void FUN_004c9390();
};

class Class_004c91a0 : public Class_004c9390 {
public:
    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { FUN_004c9390(); }
};

typedef void (__stdcall *Handler_004b7900)(void*);

struct HandlerSlot_004b7900 {
    Handler_004b7900 fn;               // +0x4
    int mask;                          // +0x8
};

struct Elem_004b75d0 {
    Class_004c91a0 name;               // +0x0
    HandlerSlot_004b7900 h;            // +0x4

    Elem_004b75d0(const Class_004c91a0& n) : name(n)
    {
        h.fn = 0;
        h.mask = 0;
    }
    ~Elem_004b75d0() { name.FUN_004c9390(); }
};

// The global vector's layout: an empty allocator byte padded to 4, then
// _First/_Last/_End. Its out-of-line insert is 0x4b7b00.
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

static inline Elem_004b75d0* Lower_bound_004b7620(Elem_004b75d0* first,
                                                   Elem_004b75d0* last,
                                                   const char* key)
{
    while (first != last) {
        Elem_004b75d0* mid = first + (last - first) / 2;
        bool less = _strcmpi(mid->name.data, key) < 0;
        if (less)
            first = mid + 1;
        else
            last = mid;
    }
    return first;
}

// FUNCTION: 0x4b7620
void __stdcall FUN_004b7620(const char* name, Handler_004b7900 fn, int mask)
{
    Class_004c91b0 key(name);
    Elem_004b75d0* first =
        Lower_bound_004b7620(DAT_0051fc99.begin(), DAT_0051fc99.end(), key.data);
    HandlerSlot_004b7900* slot;
    if (first != DAT_0051fc99.end() && strcmp(first->name.data, key.data) == 0) {
        slot = &first->h;
    } else {
        Elem_004b75d0 e(key);
        int index = first - DAT_0051fc99.begin();
        DAT_0051fc99.FUN_004b7b00(first, 1, e);
        slot = &DAT_0051fc99.begin()[index].h;
    }
    slot->fn = fn;
    slot->mask = mask;
}
