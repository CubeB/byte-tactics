// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, finished by mimo-v2.6-pro. Names are provisional.
// Registers one named handler in the file-local sorted handler table (created
// by 0x4b75a0, destroyed by 0x4b7ad0). It binary-searches the table
// case-insensitively with _strcmpi for the name, and if the exact-case name is
// not present (a plain strcmp of the element tells) inserts a new 12-byte
// element, then stores the handler pointer and its mask into the element.
// Called by 0x406f00 with "plan", "weight" and "limit".
//
// This is the out-of-line form of the inner body of 0x4b7760 (the inlined
// record loop). Element type is Class_004b7e30, the vector is
// std::vector<Class_004b7e30> (the insert callee's mangled name says so).
//
// Retry note (GPT-6.1-sol): reading the vector's end pointer through a raw
// offset-8 alias in both the binary search and end check emitted identical
// code (74.4%). The existing source below remains the best at 304/319 bytes.
// Best variant so far, 74.4%. What still differs is all downstream of how
// MSVC 5 inlines std::vector::end(): where the original does
// `mov ebx,[DAT_0051fc99+8]` directly, ours loads it into eax and then does
// `mov ebx,eax`. That frees ebx early, so the compiler hoists the zero
// constant for the new element's fields (`xor ebx,ebx`) into the fall-through
// path before the inlined strcmp, which in turn makes the inlined strcmp use a
// memory operand (`cmp dl,[edi]`) instead of the original's `mov bl,[edi]`,
// and makes the `first == end()` test reload end into eax instead of ebx.
// A hand-rolled vector class reproduces the loop, the midpoint, the inlined
// strcmp and the NameNe sequence byte for byte, but then the insert call
// mangles as our own class instead of VClass_004b7e30::?$vector::insert, so it
// cannot be used. Feeding the index to the insert position and calling the
// element constructor field-wise were both tried and lose points.
//
// Second pass (deepseek-v4.1-flash) tried every source-order lever that usually
// moves MSVC 5's register choice, all of them compiled to the exact same 304
// bytes as the variant above: last declared before first; the k local declared
// before first/last; std::vector<...>::iterator typedefs for first/last/mid;
// a local reference to the vector; `while (first < last)`; mid declared outside
// the loop; and a static inline Find(first, last, k) helper called with
// begin()/end() (both a named k and key.data). Reassigning `last = end()` after
// the loop (so the post-loop test reads the named local) does make the compiler
// load _Last straight into ebx, but it also spills that local to a new frame
// slot and grows the prologue (69.2%). The remaining wall is that MSVC keeps
// the loop's end value in eax across the loop entry and only materialises ebx
// afterwards, which lets it hoist `xor ebx,ebx` past the inlined strcmp.
//
// Third pass (space-bunny-free) found that the two tail stores go through a
// pointer to the (fn, flags) pair, not through the element: the original emits
// `add esi, 4` on the found path and folds the +4 into the insert path's
// `lea esi, [eax + edx*4 + 4]`, and both stores are then [esi] and [esi+4].
// Spelling that as `int* slot; slot = &first->field_4;` /
// `slot = &DAT_0051fc99.begin()[index].field_4;` gets all three of those
// instructions right (and both zero stores into the right slots), but the extra
// tail phi costs a register: `first` drops from esi to edi and the strcmpi
// second pointer takes esi, so 74.4% falls to 62.7% (309 bytes). Declaring the
// slot local first makes no difference (same 62.7%). The root cause of both
// forms is the same: MSVC reuses the dead `last` register ebx for the loop-exit
// strcmp result test (`cmp eax, ebx` where the original has `test eax, eax`),
// which is what frees eax for the end() reload and makes the zero constant for
// the new element hoist up past the inlined strcmp.
//
// Fourth pass (mimo-v2.6-pro): found the shape that gives the original's
// control flow but not its registers (build/scratch/0x4b7620/v_guard.cpp,
// 66.1%, 315 of 319 bytes): guard the search as `if (first != last) {
// do {...} while (...); if (found) { slot = &first->field_4; goto done; } }`
// with the insert block after the guard and one shared tail. That reproduces
// the entry `je INSERT` (jumping straight at the insert block as the original
// does), the +4 folded into the slot register on both paths, stores
// [esi]/[esi+4] and the inlined strcmp with `mov bl, [ptr]` byte for byte; a
// plain while + if/else never threads the entry jump in this function's
// context (only in 0x4b7760's). It costs a full register rotation: first/mid
// land edi/esi and last/k land ebp/ebx where the original has esi/edi and
// ebx/ebp. Everything tried to flip that tie left the code identical or worse:
// all 6 declaration orders of first/last/k (48-variant search with named vs
// temporary functor, mid inside vs outside the loop and both if/else orders),
// inline begin()/end() accessors, iterator typedefs, `Class_004b7e30 e(key, 0,
// 0)` with typed constructor parameters (Command vs int), member structs with
// inline constructors for the two int fields, a {fn, flags} pair view for the
// tail stores, and `last = end()` before the post-loop test. The post-loop
// `mov ebx, [end]; cmp esi, ebx` also stays folded to `cmp reg, [end]` in every
// variant, and both zero stores keep coming from one `xor eax, eax` after the
// copy constructor instead of `xor edi, edi` / `xor ebx, ebx` before it with
// the stores interleaved with the insert argument pushes. v_base below is still
// the best at 74.4%; the guarded slot form is kept in scratch as the closest
// structural match for whoever tries next.
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

typedef void (__stdcall *Command_004b7620)(void*);

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

// FUNCTION: 0x4b7620
void __stdcall FUN_004b7620(const char* name, Command_004b7620 fn, int flags)
{
    Class_004c91b0 key(name);
    Class_004b7e30* first = DAT_0051fc99.begin();
    Class_004b7e30* last = DAT_0051fc99.end();
    const char* k = key.data;
    while (first != last) {
        Class_004b7e30* mid = first + (last - first) / 2;
        if (NameLess_004b7620()(mid->handle.data, k))
            first = mid + 1;
        else
            last = mid;
    }
    if (first == DAT_0051fc99.end() || NameNe_004b7620()(first->handle, key)) {
        Class_004b7e30 e(key);
        int index = first - DAT_0051fc99.begin();
        DAT_0051fc99.insert(first, e);
        first = DAT_0051fc99.begin() + index;
    }
    first->field_4 = (int)fn;
    first->field_8 = flags;
}
