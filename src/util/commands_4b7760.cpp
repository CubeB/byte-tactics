// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// MATCH, 370/370 bytes and every reference (mimo-v2.6-pro). The last fix was
// the compiler-generated name of the file-local vector, DAT_0051fc99$S4554:
// that $S number counts every declaration the compiler processes before the
// vector, so the whole preamble must be counter-equivalent to the original's.
// 0x4b75a0/0x4b75d0/0x4b7900 reach $S4554 with a simpler preamble (a
// Class_004c9390 element) that cannot emit the element's handle copy
// constructor call this function needs, and this preamble measured 2 over.
// The fix: the extern "C" _strcmpi declaration block costs 2 in the counter
// (a function declaration is not free), and NameLess_004b7760 is the only
// thing that needs it, so declaring it after the vector (and before
// NameLess) drops the number to exactly $S4554 with the codegen untouched.
// The counter workhorses, measured with build/scratch/0x4b7760/snum.py on
// the near-identical tt/ probes: a standalone typedef costs 1 (kept here as
// the final +1 padding; without it this preamble gives $S4553), an
// elaborated `struct Pair_004b7760 pp` parameter declaration costs the same
// as the separate forward declaration it replaces, and dropping the
// Class_004c91a0 copy constructor declaration both breaks the codegen (the
// implicit copy constructor inlines) and moves the counter the wrong way.
//
// What fixed the bytes (this retry): the element temporary is constructed
// with a by-value constructor argument holding the (0, 0) pair:
//     Class_004b7e30 e(key, Pair_004b7760(0, 0));
// with `Class_004b7e30(const Class_004c91a0& h, Pair_004b7760 pp)` whose body
// copies the pair as a unit into the two int fields (a nested Pair member
// `p(pp)` is the same codegen; two field initialisers `value1(pp.fn)` are
// not). MSVC 5 materialises the Pair temporary's two int parameters as two
// separate zero registers before the inlined handle copy constructor (the
// original's `xor edi,edi / xor ebx,ebx` before `call 0x4c91a0`) and sinks
// their stores down among the insert call's argument pushes, exactly like
// `origin = Vec3(0, 0, 0)` at 0x44eb60 (docs/agent-guide.md). The same
// spelling also flips the last/k register tie to the original's _Last in ebx
// and the key pointer in ebp (88.4 percent had them swapped in every other
// spelling tried here and in the earlier retries).
//
// Load-bearing details kept from earlier passes: the outer
// `for (; rec->name; rec++)` (not while) puts the record increment after the
// key destructor; std::vector<Class_004b7e30> lifts the index division above
// the insert; the NameLess functor spelling puts the lower_bound result in
// edx; the `int* slot` pointers give `lea esi, [eax+edx*4+4]` with stores
// [esi]/[esi+4]; the element/Pair/Class_004c9390/Class_004c91b0 definitions
// sit after the vector (only pre-vector declarations move the $S number).
#include <string.h>
#include <vector>

class Class_004c91a0 {
public:
    char* data;                        // +0x0
    Class_004c91a0(const Class_004c91a0& other);
};

struct Pair_004b7760;

typedef int Fwd_004b7760;

class Class_004b7e30 {
public:
    Class_004c91a0 handle;             // +0x0
    int value1;                        // +0x4
    int value2;                        // +0x8

    Class_004b7e30(const Class_004c91a0& h, Pair_004b7760 pp);
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

struct Pair_004b7760 {
    int fn;
    int mask;
    Pair_004b7760(int f, int m) : fn(f), mask(m) {}
};

Class_004b7e30::Class_004b7e30(const Class_004c91a0& h, Pair_004b7760 pp) : handle(h)
{
    *(Pair_004b7760*)&value1 = pp;
}
Class_004b7e30::~Class_004b7e30() { ((Class_004c9390*)&handle)->ReleaseRef(); }

class Class_004c91b0 : public Class_004c91a0 {
public:
    Class_004c91b0(const char* text);
    ~Class_004c91b0() { ((Class_004c9390*)this)->ReleaseRef(); }
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

typedef void (__stdcall *Handler_004b7760)(void*);

struct Rec_004b7760 {
    const char* name;                  // +0x0
    Handler_004b7760 fn;               // +0x4
    int mask;                          // +0x8
};

// FUNCTION: 0x4b7760
void __stdcall FUN_004b7760(Rec_004b7760* rec)
{
    for (; rec->name; rec++) {
        int mask = rec->mask;
        Handler_004b7760 fn = rec->fn;
        Class_004c91b0 key(rec->name);
        Class_004b7e30* first = DAT_0051fc99.begin();
        Class_004b7e30* last = DAT_0051fc99.end();
        NameLess_004b7760 less;
        const char* k = key.data;
        while (first != last) {
            Class_004b7e30* mid = first + (last - first) / 2;
            if (less(mid->handle.data, k))
                first = mid + 1;
            else
                last = mid;
        }
        int* slot;
        if (first == DAT_0051fc99.end() || NameNe_004b7760()(first->handle, key)) {
            Class_004b7e30 e(key, Pair_004b7760(0, 0));
            int index = first - DAT_0051fc99.begin();
            DAT_0051fc99.insert(first, e);
            slot = &(DAT_0051fc99.begin() + index)->value1;
        } else {
            slot = &first->value1;
        }
        slot[0] = (int)fn;
        slot[1] = mask;
    }
}
