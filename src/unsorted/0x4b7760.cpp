// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// Retry #2444: 83.8 percent (363/370 bytes), unchanged from prior best. Equality rewrite scored 82.4; indexed stores stayed 83.8.
// 83.8 percent, 363 bytes against 370. This is 0x4b7620's body inlined into a
// loop over {const char* name, handler, mask} records: intern the name with
// Class_004c91b0, lower_bound over the file-local std::vector<Class_004b7e30>
// with an _strcmpi functor, insert at the search position when the exact-case
// name is absent, then store the record's handler and mask into the element.
//
// The outer `for (; rec->name; rec++)` (not `while (rec->name) { ...; rec++; }`)
// is load bearing: a for-loop increment runs after the body's locals are
// destroyed, which puts the record increment after the key destructor call as
// the original does (80.8 -> 83.8 percent).
//
// The std::vector<Class_004b7e30> form (not a hand-rolled vector) is load
// bearing: it lifts the index division above the insert, because the index is
// needed again by `begin() + index` after the call. The comparison goes
// through the NameLess functor, which makes the lower_bound result land in edx
// as the original does; the inline `_strcmpi(a,b) < 0` spelling puts it in ecx.
//
// Still differs, all downstream of one register assignment:
//   1. original keeps _Last in ebx and the key pointer (key.data) in ebp; MSVC 5
//      puts _Last in ebp and the key pointer in ebx here. Tried separately:
//      moving the `k` declaration, `char*` vs `const char*`, a temporary
//      functor, declaring first/last uninitialised in both orders, and a
//      single-use inline Find helper. All stay at 83.8 percent, so it is not a
//      simple weighted use.
//   2. because ebx holds the key here it is not free at the insert, so the
//      temporary element's two handler words are zeroed with one `xor eax,eax`
//      after the copy constructor; the original has ebx free (key is in ebp)
//      and zeroes them with `xor edi,edi / xor ebx,ebx` before the call.
//   3. the element address after the insert: original `lea esi,[eax+edx*4+4]`
//      (straight at first->field_4) and stores [esi]/[esi+4]; here the element
//      base is kept and the stores are [esi+4]/[esi+8]. Same wall as 0x4b7620.
//
// Second pass (space-bunny-free) added these negative results:
//   4. MSVC 5 always folds a +4 member offset into the store displacement, in
//      every spelling: a nested `Pair pr` member with `pr.fn = ..; pr.mask = ..`,
//      `first->pr = pr` (whole struct copy), `Pair& pr = first->pr`, `Pair* pr =
//      &first->pr`, and `int* pr = &first->field_4; pr[0] = ..; pr[1] = ..` all
//      come back as `lea esi,[eax+edx*4]` + `mov [esi+4]`/`mov [esi+8]`. Only a
//      real 4-byte pointer expression (something MSVC cannot decompose into
//      base+index+disp) can hold elem+4 in esi, so the original's stores are
//      almost certainly not written as `elem->field_4 = fn; elem->field_8 = m;`
//      on a 12-byte element. The nested-struct form also reorders the two stores
//      at the top of the body (mask before fn), which the original does not do.
//   5. for the ebx/ebp wall, the sibling 0x4b7620 allocates esi=begin, ebx=end,
//      ebp=name with `name` a *parameter*; here `k` is a local copy of
//      key.data. Making the name parameter-shaped does not move it: a static
//      inline `Find(const char* k)` that loads begin/end itself, and declaring
//      `k` before first/last, both give the identical 363 bytes with last in ebp.
//      The other direction is much worse: a comparator taking the two string
//      objects (`less(mid->handle, key)`, no k) drops to 49.4 percent, and
//      #include <algorithm> does not compile at all, so std::lower_bound is out.
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

class Class_004b7e30 {
public:
    Class_004c91a0 handle;             // +0x0
    int field_4;                       // +0x4
    int field_8;                       // +0x8

    Class_004b7e30(const Class_004c91a0& h) : handle(h), field_4(0), field_8(0) {}
    ~Class_004b7e30() { handle.FUN_004c9390(); }
};

static std::vector<Class_004b7e30> DAT_0051fc99;

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
        if (first == DAT_0051fc99.end() || NameNe_004b7760()(first->handle, key)) {
            Class_004b7e30 e(key);
            int index = first - DAT_0051fc99.begin();
            DAT_0051fc99.insert(first, e);
            first = DAT_0051fc99.begin() + index;
        }
        first->field_4 = (int)fn;
        first->field_8 = mask;
    }
}
