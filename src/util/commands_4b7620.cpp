// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by deepseek-v4.1-flash. Names are provisional.
// MATCH, 319/319 bytes (deepseek-v4.1-flash). Registers one named handler in
// the file-local sorted handler table (created by 0x4b75a0, destroyed by
// 0x4b7ad0). It binary-searches the table case-insensitively with _strcmpi for
// the name, and if the exact-case name is not present (a plain strcmp of the
// element tells) inserts a new 12-byte element, then stores the handler pointer
// and its mask into the element. Called by 0x406f00 with "plan", "weight" and
// "limit". This is the out-of-line form of the inner body of 0x4b7760.
//
// What fixed the last 20 bytes: the handler pair must be materialised as a
// local at the *top* of the function, before the key object:
//     Pair_004b7620 p((int)fn, mask);
//     Class_004c91b0 key(name);
// and the tail writes it as one struct copy, `*(Pair_004b7620*)slot = p;`.
// Every tail spelling that built the pair at the tail (a temporary, a by-value
// parameter, a helper, field stores) made MSVC hoist the second argument load
// above the first store (ecx/edx or eax/ecx), and every spelling that stored
// the two parameters with separate statements hoisted both loads (ecx/edx).
// Declaring the pair first keeps it alive across the whole search and makes the
// copy read the two argument slots one at a time into eax with the key
// destructor's `lea ecx, [esp+0x20]` scheduled between the load and the store,
// exactly as the original does. The element temporary is still built from
// Pair_004b7620(0, 0), which is what gives the two separate zero registers
// (xor edi,edi / xor ebx,ebx) sunk among the insert call's argument pushes.
//
// Load-bearing details kept from earlier passes (see 0x4b7760.cpp for the
// declaration-counter work): the whole preamble must stay counter-equivalent
// to the original's so the file-local vector mangles as DAT_0051fc99$S4554;
// std::vector<Class_004b7e30> lifts the index division above the insert; the
// NameLess functor spelling puts the lower_bound result in edx; the `int* slot`
// pointer gives `lea esi, [eax+edx*4+4]` with stores [esi]/[esi+4].
#include <string.h>
#include <vector>

class Class_004c91a0 {
public:
    char* data;                        // +0x0
    Class_004c91a0(const Class_004c91a0& other);
};

struct Pair_004b7620;

typedef int Fwd_004b7620;

class Class_004b7e30 {
public:
    Class_004c91a0 handle;             // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    Class_004b7e30(const Class_004c91a0& h, Pair_004b7620 pp);
    ~Class_004b7e30();
};

static std::vector<Class_004b7e30> DAT_0051fc99;

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

static inline bool operator==(const Class_004c91a0& a, const Class_004c91a0& b)
{
    return strcmp(a.data, b.data) == 0;
}

class Class_004c9390 {
public:
    void ReleaseRef();
};

struct Pair_004b7620 {
    int fn;
    int mask;
    Pair_004b7620(int f, int m) : fn(f), mask(m) {}
};

Class_004b7e30::Class_004b7e30(const Class_004c91a0& h, Pair_004b7620 pp) : handle(h)
{
    *(Pair_004b7620*)&value1 = pp;
}
Class_004b7e30::~Class_004b7e30() { ((Class_004c9390*)&handle)->ReleaseRef(); }

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

struct NameLess_004b7620 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

struct NameNe_004b7620 {
    bool operator()(const Class_004c91a0& a, const Class_004c91a0& b) const
    {
        return !(a == b);
    }
};

typedef void (__stdcall *Handler_004b7620)(void*);

// FUNCTION: 0x4b7620
void __stdcall FUN_004b7620(const char* name, Handler_004b7620 fn, int mask)
{
    Pair_004b7620 p((int)fn, mask);
    Class_004c91b0 key(name);
    Class_004b7e30* first = DAT_0051fc99.begin();
    Class_004b7e30* last = DAT_0051fc99.end();
    NameLess_004b7620 less;
    const char* k = key.data;
    while (first != last) {
        Class_004b7e30* mid = first + (last - first) / 2;
        if (less(mid->handle.data, k))
            first = mid + 1;
        else
            last = mid;
    }
    int* slot;
    if (first == DAT_0051fc99.end() || NameNe_004b7620()(first->handle, key)) {
        Class_004b7e30 e(key, Pair_004b7620(0, 0));
        int index = first - DAT_0051fc99.begin();
        DAT_0051fc99.insert(first, e);
        slot = &(DAT_0051fc99.begin() + index)->value1;
    } else {
        slot = &first->value1;
    }
    *(Pair_004b7620*)slot = p;
}
