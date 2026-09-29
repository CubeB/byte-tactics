// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// 59.4 percent, 341 of 370 bytes. The file below is the previous model's work
// plus the supervisor's; the worker that improved it from 43.2 percent stopped
// before writing notes, so this is the supervisor's reading of the remaining
// diff and is not a finished analysis.
//
// The whole function is one inlined copy loop over a container of records, with
// a 0x18-byte frame. Three groups of difference remain:
// 1. A struct passed by value is built on the stack, and the original loads and
//    stores its two fields in the opposite order:
//        original: mov edx,[ecx+8] ; mov ecx,[ecx+4] ; mov [esp+0x14],ecx
//                  lea ecx,[esp+0x14] ; mov [esp+0x1c],edx
//        ours:     mov edx,[ecx+4] ; mov ecx,[ecx+8] ; mov [esp+0x18],ecx
//                  lea ecx,[esp+0x14] ; mov [esp+0x18],edx
//    The original also leaves 0x18 unused between the two fields, so its
//    by-value struct is 12 bytes with padding while ours packs the fields
//    adjacently from 0x18. This is the guide's "inline helpers taking structs
//    by value" case: arguments are evaluated right to left, so the field
//    declaration order in the source decides which copy is loaded first. Worth
//    trying next: declare the by-value struct with its fields in the order the
//    original stores them, and check whether the padding at 0x18 comes from
//    `#pragma pack(2)` on that struct or from a naturally aligned 4-byte field.
// 2. An ebx/ebp swap through the rest of the loop: the original holds the
//    second container pointer in ebx and uses ebp for the loop's running value,
//    ours does the opposite. That is a register-priority difference and is
//    probably downstream of (1) rather than independent of it.
// 3. 29 bytes are still missing overall, so something is missing outright
//    rather than merely misordered. Compare the two `push` sequences around the
//    `call` that takes the stack struct, and check whether the original makes a
//    call we inline or vice versa.
// Claude Sonnet 5.5 pass (#589), not applied (the file below still scores best,
// 59.4 percent): rebuilding this body from 0x4b7620's documented idioms (the
// NameLess functor call, `!(a == b)` through NameNe, the named
// Class_004b7b00::FUN_004b7b00 insert, HandlerSlot zeroing, index divided before
// the call) inside `for (; rec->name; rec++)` gives 373 bytes (original 370, this
// file 341) but only 42.7 percent, because the alignment shifts. The search
// half then follows the original closely; what differs is what 0x4b7620's notes
// list (the `_Last` reload as a memory operand, the temporary's clear, the
// division sunk below the insert), plus the frame: the original is `sub esp,0x18`
// with `rec` re-read from [esp+0x2c] inside the loop and ebx as `_Last`, ours is
// `sub esp,0x1c; push ebx` with rec in ebx. Reading `rec->fn`/`rec->mask` at the
// end instead of into locals first: 361 bytes, 50.6. Zeroing the two words in
// the element constructor's initialiser list, with locals or without: 46.8 and
// 46.6. So the record loop is the 0x4b7620 walls repeated, and the leftover
// there (the temporary and the index division) is the place to attack first.
#include <string.h>
#include <vector>

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

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

struct Elem_004b75d0 {
    Class_004c91a0 name;               // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    Elem_004b75d0(const Class_004c91a0& n) : name(n), value1(0), value2(0) {}
    ~Elem_004b75d0() { name.FUN_004c9390(); }
};

extern std::vector<Elem_004b75d0> DAT_0051fc99;

struct Rec_004b7760 {
    char* name;                        // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8
};

static inline bool Same_004b7760(const char* a, const char* b)
{
    return strcmp(a, b) == 0;
}

// FUNCTION: 0x4b7760
void __stdcall FUN_004b7760(Rec_004b7760* rec)
{
    for (; rec->name; rec++) {
        int v1 = rec->value1;
        int v2 = rec->value2;
        Class_004c91b0 name(rec->name);
        char* key = name.data;
        Elem_004b75d0* first = DAT_0051fc99.begin();
        Elem_004b75d0* last = DAT_0051fc99.end();
        while (first != last) {
            Elem_004b75d0* mid = first + (last - first) / 2;
            if (_strcmpi(mid->name.data, key) < 0)
                first = mid + 1;
            else
                last = mid;
        }
        if (first == DAT_0051fc99.end() || !Same_004b7760(first->name.data, key)) {
            Elem_004b75d0 e(name);
            int index = first - DAT_0051fc99.begin();
            DAT_0051fc99.insert(first, e);
            first = DAT_0051fc99.begin() + index;        }
        int* slot = &first->value1;
        *slot = v1;
        slot[1] = v2;    }
}
