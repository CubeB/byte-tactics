// Decompiled by space-bunny-free. Names are provisional.
// NOT A MATCH: 48.5 percent, frame is 0x6c where the original is 0x68, so I
// have one dword of local too many (every slot below is therefore 4 high) and
// the whole allocation cascade follows from that. The original's slots are
// want 0x10, n2 0x14, cur 0x18, b 0x1c, len 0x20, res 0x24, lock 0x28,
// need 0x2c, the pair 0x30, the _Ubound key 0x38, the insert result 0x40 and
// the 48-byte record at 0x48, which is larger than the frame and so overlaps
// the saved registers (0x4d8849 stores the 4th ctor argument to [esi+0x2c],
// that is P+0x74, the saved edi, and the epilogue pops it into edi, which is
// dead, so the original gets away with it). Tried and did not help: making the
// record a block-local instead of a function-local (it was hoisted to the top
// of the function as a function-local initialiser); one pair instead of two
// for the _Ubound key and the split pairs.
// Carve a block out of the reservation allocator: enter the lock, round the
// request up to 8k (need) and double it (want), then walk the free-block map
// for a block of at least want bytes, splitting it around the allocation point
// and committing the pages with VirtualAlloc. Same std::map idiom as 0x4db000,
// 0x4db450 and 0x4db1c0: the block is the map's value_type, a base and a length.
#include <windows.h>

extern unsigned int DAT_005289d4;
extern unsigned int DAT_00528a00;
extern unsigned int DAT_005289f0;
extern unsigned int DAT_005289d0;
extern unsigned int DAT_00528a04;

struct Node_004dacf0 {
    Node_004dacf0* left;               // +0x0
    Node_004dacf0* parent;             // +0x4
    Node_004dacf0* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The map's iterator: one pointer. The three methods are the tree's own
// (operator--, the postfix operator++ and the set's erase).
class Class_004dd2a0 {
public:
    Node_004dacf0* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dacf0* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
    Class_004dd2a0 FUN_004dbe10(int);
    Class_004dd2a0 FUN_004dbd80(int);

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }
};

// The map's value_type: the block's base address and its length, 8 bytes.
struct Pair_004dacf0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4

    Pair_004dacf0() : offset(0), length(0) {}
    Pair_004dacf0(unsigned int o, int l) : offset(o), length(l) {}
};

// The insert helpers write an iterator and a flag through their first argument.
struct Ins_004dacf0 {
    Node_004dacf0* ptr;
    unsigned char inserted;

    Ins_004dacf0() {}
};

class Class_004dc620 {
public:
    void FUN_004dc620(Class_004dd2a0* out, Pair_004dacf0* kv);
};

class Class_004dbeb0 {
public:
    char unknown_0[4];
    Node_004dacf0* head;               // +0x4

    void FUN_004dbeb0(Class_004dd2a0* out);
};

class Class_004dbd00 {
public:
    Class_004dd2a0 FUN_004dbd00(Class_004dd2a0 it);
};

// The allocator: the free-block map, its block count and its reserved total.
class Class_004db610 {
public:
    char unknown_0[4];
    Node_004dacf0* head;               // +0x4
    char unknown_8[4];
    int count;                         // +0xc
    int total;                         // +0x10
    char unknown_14[20];

    void FUN_004dbbc0(Ins_004dacf0* out, Pair_004dacf0* v);
    bool FUN_004db450(unsigned int size);
    Node_004dacf0* FUN_004db1c0(unsigned int size);
    void FUN_004db000(Pair_004dacf0 p);

    void FUN_004dc620(Class_004dd2a0* out, Pair_004dacf0* kv);
    void FUN_004dbeb0(Class_004dd2a0* out);
    Class_004dd2a0 FUN_004dbd00(Class_004dd2a0 it);
};

// The record the second map keeps for every live block: 48 bytes.
class Class_004d8820 {
public:
    unsigned int base;                 // +0x0
    unsigned int size;                 // +0x4
    unsigned int count;                // +0x8
    char unknown_c[0x24];              // +0xc
    unsigned int tag;                  // +0x2c

    Class_004d8820(unsigned int a, unsigned int b, unsigned int c,
                   unsigned int d, unsigned int e);
};

class Class_004dc680 {
public:
    void FUN_004dc680(Ins_004dacf0* out, Class_004d8820* v);
};

class CritSec_004da780 {
public:
    CRITICAL_SECTION cs;

    CritSec_004da780() { InitializeCriticalSection(&cs); }
    ~CritSec_004da780() {}
};

CritSec_004da780* FUN_004da780();
unsigned int FUN_004da8a0(unsigned int size);
unsigned int FUN_004da8c0(unsigned int size);
Class_004db610* FUN_004db610();
Class_004dc680* FUN_004da8d0();
void FUN_004da7d0(unsigned int size);
char FUN_004db760();
int FUN_004db7c0();
void FUN_004d82c0(void* at, int value, unsigned int count);

// FUNCTION: 0x4dacf0
unsigned int FUN_004dacf0(unsigned int n, unsigned int arg2)
{
    CritSec_004da780* lock = FUN_004da780();
    EnterCriticalSection(&lock->cs);
    unsigned int res = 0;
    if (!n)
        n = 1;
    unsigned int need = FUN_004da8c0(n);
    unsigned int want = FUN_004da8a0(n);
    if (want < need) {
        LeaveCriticalSection(&lock->cs);
        return 0;
    }

    Class_004db610* map = FUN_004db610();
    unsigned int wraps = 0;
    unsigned int base = 0;
    Class_004dd2a0 n2;
    Class_004dd2a0 b;
    Class_004dd2a0 cur;
    Pair_004dacf0 p;
    Ins_004dacf0 ins;
    unsigned int len;
    unsigned int key;

    if (map->count <= wraps)
        goto alloc_new;

    p.offset = DAT_005289d4;
    p.length = 0;
    map->FUN_004dc620(&n2, &p);
    map->FUN_004dbeb0(&b);
    if (n2.Neq(n2, b)) {
        Class_004dd2a0 prev = n2;
        prev.FUN_004dbe10(0);
        if (DAT_005289d4 >= prev.ptr->key &&
            want + DAT_005289d4 <= prev.ptr->key + prev.ptr->length)
            n2 = prev;
    }
    cur = n2;

    for (;;) {
        if (cur == map->head) {
            map->FUN_004dbeb0(&b);
            cur = b;
            DAT_005289d4 = 0;
            DAT_00528a00++;
            wraps++;
        }
        if (cur.ptr->length >= want)
            break;
        cur.FUN_004dbd80(0);
        if (wraps >= 2)
            goto alloc_new;
    }

    key = cur.ptr->key;
    len = cur.ptr->length;
    map->FUN_004dbd00(cur);
    if (base == 0) {
        base = key;
        DAT_005289d4 = base;
    }
    if (base < key)
        base = key;
    if (base + want > key + len)
        base = key;
    if (base > key) {
        p.offset = key;
        p.length = base - key;
        map->FUN_004dbbc0(&ins, &p);
    }
    if (base + want < key + len) {
        p.offset = base + want;
        p.length = key + len - base - want;
        map->FUN_004dbbc0(&ins, &p);
    }
    DAT_005289d4 = base + want;
    goto use_block;

alloc_new:
    if (map->FUN_004db450(want))
        base = (unsigned int)map->FUN_004db1c0(want);
    if (base) {
        res = (unsigned int)VirtualAlloc((void*)base, need, MEM_COMMIT,
                                         PAGE_READWRITE);
        if (res == 0)
            map->FUN_004db000(Pair_004dacf0(base, want));
    }

use_block:
    if (res == 0) {
        LeaveCriticalSection(&lock->cs);
        return 0;
    }
    unsigned int pad = (0 - n) & 0xfff;
    if (FUN_004db760()) {
        FUN_004d82c0((void*)res, FUN_004db7c0(), pad);
        res += pad;
    } else {
        FUN_004d82c0((void*)(res + n), FUN_004db7c0(), pad);
    }
    {
        Class_004d8820 rec(res, n, DAT_00528a04, arg2, 0);
        FUN_004da8d0()->FUN_004dc680(&ins, &rec);
    }
    FUN_004da7d0(n);
    DAT_005289f0 += (n + 0xfff) & 0xfffff000;
    if (DAT_005289f0 > DAT_005289d0)
        DAT_005289d0 = DAT_005289f0;
    LeaveCriticalSection(&lock->cs);
    return res;
}
