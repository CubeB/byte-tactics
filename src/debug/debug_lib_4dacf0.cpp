// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash and
// Space Bunny Free, finished by claude-opus-5-5. Names are provisional.
// Commits a block for a pool allocation: rounds the request into need/want,
// takes `want` bytes of reserved address space from the free-block set (the
// allocator's TakeFreeBlock, inlined here), commits `need` of it with
// VirtualAlloc, pads the block and records it in the second set.
//
// MATCH (claude-opus-5-5, #5052; was 79.8% after #5034, 66.4% before that).
//  * The search is TakeFreeBlock's body inlined one level deep, which is why
//    the set's own members (lower_bound 0x4dc620, begin 0x4dbeb0, the
//    postfix -- and ++ 0x4dbe10/0x4dbd80, erase 0x4dbd00, insert 0x4dbbc0)
//    are called out of line here, and the "grow and retry" tail is a call to
//    0x4db450 plus the recursive call to 0x4db1c0. It needs `inline`, since
//    MSVC 5 does not inline the recursive member on its own, and it is
//    defined here without a FUNCTION line because 0x4db1c0.cpp owns that
//    address.
//  * `int tries = 0;` sits just before the loop, not at the top. At the top,
//    MSVC counts the zero of tries and of the size() test together with the
//    loop's `DAT_005289d4 = 0` and keeps a constant 0 in ebx for the whole
//    search. Declared before the loop, the zero for size(), k.length and
//    the --(0) is the register tries later lives in (esi), and the loop's
//    zeros are immediates, as in the original.
//  * The found block copies the free block's value (`Pair b = *cur;`), so
//    its offset and length are fields of one 8-byte local: the length's
//    spill slot is 0x20, next to the begin()/`it` slot at 0x1c, which is
//    what puts res, lock and need at 0x24/0x28/0x2c. `lb != begin()` uses
//    the temporary, so it shares 0x1c with `it` and the loop's begin().
//  * The clamp reads DAT_005289d4 directly, the way the prev-block test
//    does, and only then picks `mark`. Writing it with `mark` (`mark = DAT;
//    if (mark < b.offset || ...) mark = b.offset;`) gives the same
//    instructions but one more use of mark, and that use alone made MSVC
//    give `this` ebx and mark edi (lock and need swap with them).
//  * The second free block's length is `b.offset - mark + b.length - bytes`
//    (sub, add, mov, sub); `b.offset + b.length - end` is 3 bytes shorter.
#include <windows.h>
#include <set>

extern unsigned int DAT_005289d4; // offset the last block was handed out at
extern unsigned int DAT_00528a00; // how often the search wrapped around
extern unsigned int DAT_005289f0; // bytes committed
extern unsigned int DAT_005289d0; // high-water mark of DAT_005289f0
extern unsigned int DAT_00528a04;

struct Pair_004db000 {
    unsigned int offset; // +0x0
    unsigned int length; // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

struct Node_004dacf0 {
    Node_004dacf0* left;   // +0x0
    Node_004dacf0* parent; // +0x4
    Node_004dacf0* right;  // +0x8
    Pair_004db000 value;   // +0xc  the free block: offset, length
    int color;             // +0x14
};

class Class_004dbe10 {
public:
    Node_004dacf0* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dbe10& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
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
class Class_004db450 { public: bool GrowReservation(unsigned int); };
class Class_004dc680 { public: Class_004ddbe0 FUN_004dc680(const Class_004d8820& v); };

class FreeBlockMap {
public:
    char unknown_0[4];     // +0x0
    Node_004dacf0* head;   // +0x4
    unsigned char rebuild; // +0x8
    unsigned int count;    // +0xc
    unsigned int total;    // +0x10

    Class_004dbe10 begin() { return ((Class_004dbeb0*)this)->FUN_004dbeb0(); }
    Class_004dbe10 end() { return Class_004dbe10(head); }
    unsigned int size() const { return count; }
    unsigned int TakeFreeBlock(unsigned int bytes);
    void AddFreeBlock(Pair_004db000);
};

CRITICAL_SECTION* FUN_004da780();
unsigned int __cdecl FUN_004da8a0(unsigned int size);
unsigned int __cdecl RoundUpToPage(unsigned int size);
FreeBlockMap* GetFreeBlockSet();
Class_004dc680* GetBlockMap();
void __cdecl CountAlloc(unsigned int size);
char IsBackAlign();
int FUN_004db7c0();
void __cdecl FillPattern(void* at, int value, unsigned int count);

// The allocator's alloc() (0x4db1c0, see the notes above): find a free block
// of `bytes`, preferring the one the last allocation came from, and hand back
// the leftovers around the request as new free blocks.
inline unsigned int FreeBlockMap::TakeFreeBlock(unsigned int bytes)
{
    if (size() > 0) {
        Pair_004db000 k;
        k.offset = DAT_005289d4;
        k.length = 0;
        Class_004dbe10 lb = ((Class_004dc620*)this)->FUN_004dc620(k);
        if (lb != begin()) {
            Class_004dbe10 it = lb;
            it.FUN_004dbe10(0);
            if (DAT_005289d4 >= it->offset && DAT_005289d4 + bytes <= it->offset + it->length)
                lb = it;
        }
        Class_004dbe10 cur = lb;
        int tries = 0;
        do {
            if (cur == end()) {
                cur = begin();
                DAT_005289d4 = 0;
                DAT_00528a00++;
                tries++;
            }
            if (cur->length >= bytes) {
                Pair_004db000 b = *cur;
                ((Class_004dbd00*)this)->FUN_004dbd00(cur);
                if (DAT_005289d4 == 0)
                    DAT_005289d4 = b.offset;
                unsigned int mark;
                if (DAT_005289d4 >= b.offset && DAT_005289d4 + bytes <= b.offset + b.length)
                    mark = DAT_005289d4;
                else
                    mark = b.offset;
                if (mark > b.offset)
                    ((Class_004dce60*)this)->FUN_004dbbc0(Pair_004db000(b.offset, mark - b.offset));
                unsigned int end = mark + bytes;
                if (end < b.offset + b.length)
                    ((Class_004dce60*)this)->FUN_004dbbc0(Pair_004db000(end, b.offset - mark + b.length - bytes));
                DAT_005289d4 = end;
                return mark;
            }
            cur.FUN_004dbd80(0);
        } while (tries < 2);
    }
    if (((Class_004db450*)this)->GrowReservation(bytes))
        return TakeFreeBlock(bytes);
    return 0;
}

// FUNCTION: 0x4dacf0
unsigned int __cdecl AllocDebugBlock(unsigned int n, unsigned int arg2)
{
    CRITICAL_SECTION* lock = FUN_004da780();
    EnterCriticalSection(lock);
    unsigned int size = n;
    unsigned int res = 0;
    if (size == 0)
        size = 1;
    unsigned int need = RoundUpToPage(size);
    unsigned int want = FUN_004da8a0(size);
    if (want < need) {
        LeaveCriticalSection(lock);
        return 0;
    }
    unsigned int base = GetFreeBlockSet()->TakeFreeBlock(want);
    if (base != 0) {
        res = (unsigned int)VirtualAlloc((void*)base, need, MEM_COMMIT, PAGE_READWRITE);
        if (res == 0) {
            FreeBlockMap* pool = GetFreeBlockSet();
            pool->AddFreeBlock(Pair_004db000(base, want));
        }
    }
    if (res == 0) {
        LeaveCriticalSection(lock);
        return 0;
    }
    unsigned int pad = (0 - (n & 0xfff)) & 0xfff;
    if (IsBackAlign()) {
        FillPattern((void*)res, FUN_004db7c0(), pad);
        res += pad;
    } else {
        FillPattern((void*)(res + n), FUN_004db7c0(), pad);
    }
    Class_004d8820 rec(res, n, DAT_00528a04, arg2, 0);
    Class_004dc680* blocks = GetBlockMap();
    blocks->FUN_004dc680(rec);
    CountAlloc(n);
    DAT_005289f0 += (n + 0xfff) & 0xfffff000;
    if (DAT_005289f0 > DAT_005289d0)
        DAT_005289d0 = DAT_005289f0;
    LeaveCriticalSection(lock);
    return res;
}
