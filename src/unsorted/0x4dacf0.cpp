// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash and
// Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
// Commits a block for a pool allocation: rounds the request into need/want,
// takes `want` bytes of reserved address space from the free-block set (the
// allocator's FUN_004db1c0, inlined here), commits `need` of it with
// VirtualAlloc, pads the block and records it in the second set.
//
// NOT A MATCH: 79.8%, 780 of 780 bytes, frame 0x68 (claude-opus-5-5, #5034;
// the earlier files reached 66.4% with the search hand-copied into this
// function). What the rebuild found:
//  * The search is FUN_004db1c0's body inlined one level deep, which is why
//    the set's own members (lower_bound 0x4dc620, begin 0x4dbeb0, the
//    postfix -- and ++ 0x4dbe10/0x4dbd80, erase 0x4dbd00, insert 0x4dbbc0)
//    are called out of line here, and the "grow and retry" tail is a call to
//    0x4db450 plus the recursive call to 0x4db1c0. Written as an inline
//    member it gives the original's frame and the want/need/lock/size
//    registers of the prologue; it needs `inline`, since MSVC 5 does not
//    inline the recursive member on its own. It is defined here without a
//    FUNCTION line because 0x4db1c0.cpp owns that address.
//  * Iterator slots: the lower_bound result (0x14) and the loop iterator
//    (0x18) are two variables, and `it` is block scoped so that it shares
//    0x1c with the begin() temporaries, as the original does. Comparing with
//    a named `first = begin()` (not the temporary) is worth 5 points.
//  * `if (DAT_005289d4 == 0) DAT_005289d4 = base; mark = DAT_005289d4;` gives
//    the store from mark's register; `mark = base; DAT = mark;` stores base.
//  * The second free block's length is `base - mark + len - bytes`, which
//    MSVC keeps in the original's order (sub, add, mov, sub).
//  * `#include <set>` after <windows.h> is worth 3.6 points (compiler state;
//    tools/headers.py --cpp also finds <list> and <stdio.h> + <ddraw.h>).
// WHAT STILL DIFFERS: one decision. MSVC hoists the constant 0 into ebx for
// the search loop (for `DAT_005289d4 = 0` and the `push 0` of cur++), so the
// pre-loop zero uses read ebx where the original reads esi (tries == 0), the
// loop's end() test loads head into ecx instead of ebx, and the alloc-failure
// path reuses ebx instead of `xor ebx, ebx`. Any constant stored in that loop
// is hoisted (a test with `= 5` hoists the 5), and no declaration count,
// header set or /Gi changes it, so it is source shape. The one shape found
// that stops it is the prev-block in a nested inline helper taking the
// lower_bound iterator by value (`cur = Prev(lb, bytes)`): the loop then
// matches exactly (head in ebx, immediate stores), but the helper's copy of
// lb lands in ebx and `bytes` loses ebp in that block (72.4%). The next lever
// to look for is what else occupies ebx across the prev block in the original
// (there it only holds temporaries: begin().ptr, then key + length).
// Also flat: the wrap block as a helper, `while(1)`/`for(;;)`/goto loops, the
// found block outside the loop, every spelling of the clamp, by-value
// operator== and !=, a Neq(a, b) helper like 0x4db000.cpp's, real <map>
// iterators, the loop counter as char/unsigned, and int/unsigned globals.
#include <windows.h>
#include <set>

extern unsigned int DAT_005289d4; // offset the last block was handed out at
extern unsigned int DAT_00528a00; // how often the search wrapped around
extern unsigned int DAT_005289f0; // bytes committed
extern unsigned int DAT_005289d0; // high-water mark of DAT_005289f0
extern unsigned int DAT_00528a04;

struct Node_004dacf0 {
    Node_004dacf0* left;   // +0x0
    Node_004dacf0* parent; // +0x4
    Node_004dacf0* right;  // +0x8
    unsigned int key;      // +0xc  block offset
    unsigned int length;   // +0x10 block length
    int color;             // +0x14
};

struct Pair_004db000 {
    unsigned int offset; // +0x0
    unsigned int length; // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

class Class_004dbe10 {
public:
    Node_004dacf0* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dbe10& o) const { return !(*this == o); }
    Node_004dacf0* operator->() const { return ptr; }
    Class_004dbe10 FUN_004dbe10(int); // operator--(int)
    Class_004dbe10 FUN_004dbd80(int); // operator++(int)
};

class Class_004ddbe0 {
public:
    Class_004dbe10 first;
    unsigned char second;
    Class_004ddbe0() {}
};

class Class_004d8820 {
public:
    unsigned int base;    // +0x0
    unsigned int size;    // +0x4
    unsigned int count;   // +0x8
    char unknown_c[0x20]; // +0xc
    unsigned int tag;     // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c, unsigned int d, const char* e);
};

class Class_004dc620 { public: Class_004dbe10 FUN_004dc620(const Pair_004db000& k); };
class Class_004dbeb0 { public: Class_004dbe10 FUN_004dbeb0(); };
class Class_004dbd00 { public: Class_004dbe10 FUN_004dbd00(Class_004dbe10 it); };
class Class_004dce60 { public: Class_004ddbe0 FUN_004dbbc0(const Pair_004db000& v); };
class Class_004db450 { public: bool FUN_004db450(unsigned int); };
class Class_004dc680 { public: Class_004ddbe0 FUN_004dc680(const Class_004d8820& v); };

class Class_004db000 {
public:
    char unknown_0[4];     // +0x0
    Node_004dacf0* head;   // +0x4
    unsigned char rebuild; // +0x8
    unsigned int count;    // +0xc
    unsigned int total;    // +0x10

    Class_004dbe10 begin() { return ((Class_004dbeb0*)this)->FUN_004dbeb0(); }
    Class_004dbe10 end() { return Class_004dbe10(head); }
    unsigned int size() const { return count; }
    unsigned int FUN_004db1c0(unsigned int bytes);
    void FUN_004db000(Pair_004db000);
};

CRITICAL_SECTION* FUN_004da780();
unsigned int __cdecl FUN_004da8a0(unsigned int size);
unsigned int __cdecl FUN_004da8c0(unsigned int size);
Class_004db000* FUN_004db610();
Class_004dc680* FUN_004da8d0();
void __cdecl FUN_004da7d0(unsigned int size);
char FUN_004db760();
int FUN_004db7c0();
void __cdecl FUN_004d82c0(void* at, int value, unsigned int count);

// The allocator's alloc() (0x4db1c0, see the notes above): find a free block
// of `bytes`, preferring the one the last allocation came from, and hand back
// the leftovers around the request as new free blocks.
inline unsigned int Class_004db000::FUN_004db1c0(unsigned int bytes)
{
    int tries = 0;
    if (size() > 0) {
        Pair_004db000 k;
        k.offset = DAT_005289d4;
        k.length = 0;
        Class_004dbe10 lb = ((Class_004dc620*)this)->FUN_004dc620(k);
        Class_004dbe10 first = begin();
        if (lb != first) {
            Class_004dbe10 it = lb;
            it.FUN_004dbe10(0);
            if (DAT_005289d4 >= it->key && DAT_005289d4 + bytes <= it->key + it->length)
                lb = it;
        }
        Class_004dbe10 cur = lb;
        do {
            if (cur == end()) {
                cur = begin();
                DAT_00528a00++;
                DAT_005289d4 = 0;
                tries++;
            }
            if (cur->length >= bytes) {
                unsigned int len = cur->length;
                unsigned int base = cur->key;
                ((Class_004dbd00*)this)->FUN_004dbd00(cur);
                if (DAT_005289d4 == 0)
                    DAT_005289d4 = base;
                unsigned int mark = DAT_005289d4;
                if (mark < base || mark + bytes > base + len)
                    mark = base;
                if (mark > base)
                    ((Class_004dce60*)this)->FUN_004dbbc0(Pair_004db000(base, mark - base));
                unsigned int end = mark + bytes;
                if (end < base + len)
                    ((Class_004dce60*)this)->FUN_004dbbc0(Pair_004db000(end, base - mark + len - bytes));
                DAT_005289d4 = end;
                return mark;
            }
            cur.FUN_004dbd80(0);
        } while (tries < 2);
    }
    if (((Class_004db450*)this)->FUN_004db450(bytes))
        return FUN_004db1c0(bytes);
    return 0;
}

// FUNCTION: 0x4dacf0
unsigned int __cdecl FUN_004dacf0(unsigned int n, unsigned int arg2)
{
    CRITICAL_SECTION* lock = FUN_004da780();
    EnterCriticalSection(lock);
    unsigned int size = n;
    unsigned int res = 0;
    if (size == 0)
        size = 1;
    unsigned int need = FUN_004da8c0(size);
    unsigned int want = FUN_004da8a0(size);
    if (want < need) {
        LeaveCriticalSection(lock);
        return 0;
    }
    unsigned int base = FUN_004db610()->FUN_004db1c0(want);
    if (base != 0) {
        res = (unsigned int)VirtualAlloc((void*)base, need, MEM_COMMIT, PAGE_READWRITE);
        if (res == 0) {
            Class_004db000* pool = FUN_004db610();
            pool->FUN_004db000(Pair_004db000(base, want));
        }
    }
    if (res == 0) {
        LeaveCriticalSection(lock);
        return 0;
    }
    unsigned int pad = (0 - (n & 0xfff)) & 0xfff;
    if (FUN_004db760()) {
        FUN_004d82c0((void*)res, FUN_004db7c0(), pad);
        res += pad;
    } else {
        FUN_004d82c0((void*)(res + n), FUN_004db7c0(), pad);
    }
    Class_004d8820 rec(res, n, DAT_00528a04, arg2, 0);
    Class_004dc680* blocks = FUN_004da8d0();
    blocks->FUN_004dc680(rec);
    FUN_004da7d0(n);
    DAT_005289f0 += (n + 0xfff) & 0xfffff000;
    if (DAT_005289f0 > DAT_005289d0)
        DAT_005289d0 = DAT_005289f0;
    LeaveCriticalSection(lock);
    return res;
}
