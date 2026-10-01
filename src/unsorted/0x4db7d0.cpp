// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by deepseek-v4.1, finished by Sonnet 5.5. Names are provisional.
// The game's free() for its own heap: under the allocator lock it looks the
// block up in the live-block map, records the freed header in the debug arena,
// drops it from the live map, releases the pages it had reserved for the block
// and finally merges the released range into the free-block map with its two
// neighbours. Same std::map idiom as 0x4db450 and 0x4db000.
//
// NOT MATCHING: 83.0 percent, 625 of 624 bytes (Sonnet 5.5, #3275 retry; was
// 81.3 percent). What changed, and what still differs:
//  * The erase's out-parameter is NOT the iterator that gets compared. The
//    original stores the lower_bound result into it (`node = n`, the store at
//    0x4db95e) and then tests it against begin(); the erase result is only a
//    scratch out-parameter. Both live at [esp+0x60], the home of the dead
//    parameter `p`.
//  * That slot reuse is reproduced by wrapping everything from `node` to the
//    last use of `pair` in a nested block (declaring `node` in the plain
//    `if (p)` block puts it at [esp+0x10]). Declaring the block earlier loses
//    it again (78 percent).
//  * `p = (void*)((unsigned)p & 0xfffff000)` (in place, before the `blk == 0`
//    test) gives the original's `and edi,0xfffff000` placement; the length
//    result held in a local until after FUN_004db610 gives the late pair stores;
//    FUN_004da8c0's result in a local gives the original push order for
//    VirtualFree.
//  * The pair-merge erase (FUN_004dbd00) returning the iterator by value, with
//    the result discarded, shares one temp slot with the operator-- result.
// Remaining: (1) frame order: the original has cs at 0x20 and the shared temp
// slot at 0x1c, we have them the other way round (everything else, it1 0x10,
// n 0x14, it3 0x18, pair 0x24, is right). Declaration order, block position
// and extra temporaries did not move it. (2) The erase call: the original
// loads it1.ptr into esi, calls FUN_004da8d0, then lea/push/push; we push
// first and call FUN_004da8d0 last (named node at a frame slot gave the
// original order, node at [esp+0x60] does not). (3) the `node = n` store is
// scheduled after the push of &it3 in the original, before it here.
// Tried this pass and worse: the whole pair<iterator,bool> result as an 8
// byte `it3` (frame grows by 4), all iterators as by-value temps, a mutated
// `p` aliased as `node` (37 to 54 percent).
#include <windows.h>
#include <memory>

extern unsigned int DAT_005289f0;

// ---- the live-block map (FUN_004da8d0) -------------------------------------

// The map's value_type as the debug arena stores it: the tree node's _Color
// followed by the key and the rest of the value, 0x30 bytes in all.
struct LiveEntry {
    unsigned int color;                // +0x0
    unsigned int key;                  // +0x4
    char unknown_8[0x28];              // +0x8
};

struct LiveNode {
    LiveNode* left;                    // +0x0
    LiveNode* parent;                  // +0x4
    LiveNode* right;                   // +0x8
    LiveEntry entry;                   // +0xc
};

class Iter_004dce00 {
public:
    LiveNode* ptr;

    Iter_004dce00() {}
    Iter_004dce00(LiveNode* q) : ptr(q) {}
    bool operator==(const Iter_004dce00& o) const { return ptr == o.ptr; }
};

class Class_004dce00 {
public:
    char unknown_0[4];
    LiveNode* head;                    // +0x4

    Iter_004dce00 End() { return Iter_004dce00(head); }
    Iter_004dce00 FUN_004dce00(const unsigned int& key);
};

// The per-block header the allocator builds; its first dword is the map's key.
class Class_004d8820 {
public:
    unsigned int key;                  // +0x0
    unsigned int field_4;              // +0x4
    unsigned int field_8;              // +0x8
    char unknown_c[0x20];              // +0xc
    unsigned int field_2c;             // +0x2c

    Class_004d8820(void* a, unsigned int b, unsigned int c, unsigned int d,
                   unsigned int e);
};

// ---- the debug arena that records every freed header (FUN_004da9f0) --------

template <class T, class A = std::allocator<T> >
class Container_004da9f0 {
public:
    A allocator;                       // +0x0
    LiveEntry* field_4;                // +0x4
    unsigned int field_8;              // +0x8
    char unknown_c[4];
    unsigned int field_10;             // +0x10
};

Container_004da9f0<int>* FUN_004da9f0();

class Class_004dd8c0 {
public:
    void FUN_004dd8c0(unsigned int a, int b, LiveEntry* c);
};

// ---- the allocator's free-block map (FUN_004db610) --------------------------

struct Node_004db450 {
    Node_004db450* left;               // +0x0
    Node_004db450* parent;             // +0x4
    Node_004db450* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The map's value_type: the block's base address and its length.
struct Pair_004db450 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

// The map's iterator; FUN_004dbe10 is its operator--(int).
class Class_004dbe10 {
public:
    Node_004db450* ptr;

    Class_004dbe10() {}
    Class_004dbe10(Node_004db450* q) : ptr(q) {}
    bool operator==(const Class_004dbe10& o) const { return ptr == o.ptr; }

    Class_004dbe10 FUN_004dbe10(int);
};

class Class_004db450 {
public:
    char unknown_0[4];
    Node_004db450* head;               // +0x4
    char unknown_8[8];
    int total;                         // +0x10

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dbe10 a, Class_004dbe10 b) { return !(a == b); }
};

class Class_004dbd20 {
public:
    void FUN_004dbd20(Class_004dbe10* out, const unsigned int& kv);
};

class Class_004dbeb0 {
public:
    void FUN_004dbeb0(Class_004dbe10* out);
};

class Class_004dbd00 {
public:
    Class_004dbe10 FUN_004dbd00(void* node);
};

class Class_004dbbc0 {
public:
    void FUN_004dbbc0(Class_004dbe10* out, Pair_004db450* p);
};

class Class_004dc910 {
public:
    void FUN_004dc910(Class_004dbe10* out, void* node);
};

LPCRITICAL_SECTION FUN_004da780();
Class_004dce00* FUN_004da8d0();
Class_004db450* FUN_004db610();
char FUN_004db760();
int FUN_004db7c0();
void __cdecl FUN_004d8310(void* p, int pattern, unsigned int size);
void __cdecl FUN_004da840(int param_1);
unsigned int __cdecl FUN_004da8c0(int param_1);
int __cdecl FUN_004da8a0(int param_1);

// FUNCTION: 0x4db7d0
void __cdecl FUN_004db7d0(void* p, int flags)
{
    if (p) {
        LPCRITICAL_SECTION cs = FUN_004da780();
        EnterCriticalSection(cs);
        Class_004dce00* live = (Class_004dce00*)FUN_004da8d0();
        Class_004d8820 hdr(p, 0, 0, 0, 0);
        Iter_004dce00 it1 = live->FUN_004dce00(hdr.key);
        if (it1 == ((Class_004dce00*)FUN_004da8d0())->End()) {
            LeaveCriticalSection(cs);
            return;
        }
        unsigned int blk = it1.ptr->entry.key;
        unsigned int off = (0 - (blk & 0xfff)) & 0xfff;
        if (FUN_004db760())
            FUN_004d8310((char*)p - off, FUN_004db7c0(), off);
        else
            FUN_004d8310((char*)p + blk, FUN_004db7c0(), off);
        LiveEntry* ve = &it1.ptr->entry;
        Container_004da9f0<int>* arena = FUN_004da9f0();
        if (arena->field_10 < 0x2000) {
            ((Class_004dd8c0*)arena)->FUN_004dd8c0(arena->field_8, 1, ve);
        } else {
            arena->field_4[arena->field_10 & 0x1fff] = *ve;
        }
        arena->field_10++;
        // Nested block: puts `node` in the dead parameter p's home, see above.
        {
            Class_004dbe10 node;
            ((Class_004dc910*)FUN_004da8d0())->FUN_004dc910(&node, it1.ptr);
            FUN_004da840(blk);
            DAT_005289f0 -= (blk + 0xfff) & 0xfffff000;
            p = (void*)((unsigned int)p & 0xfffff000);
            if (blk == 0)
                blk = 1;
            unsigned int base = (unsigned int)p;
            unsigned int sz = FUN_004da8c0(blk);
            VirtualFree((void*)base, sz, MEM_DECOMMIT);
            int len = FUN_004da8a0(blk);
            Class_004db450* alloc = (Class_004db450*)FUN_004db610();
            Pair_004db450 pair;
            pair.offset = base;
            pair.length = len;
            Class_004dbe10 n;
            ((Class_004dbd20*)alloc)->FUN_004dbd20(&n, pair.offset);
            node = n;
            Class_004dbe10 it3;
            ((Class_004dbeb0*)alloc)->FUN_004dbeb0(&it3);
            if (node == it3)
                node.ptr = alloc->head;
            else
                node.FUN_004dbe10(0);
            if (alloc->Neq(n, Class_004dbe10(alloc->head))) {
                if (n.ptr->key == pair.offset + pair.length) {
                    pair.length = pair.length + n.ptr->length;
                    ((Class_004dbd00*)alloc)->FUN_004dbd00(n.ptr);
                }
        }
        if (alloc->Neq(node, Class_004dbe10(alloc->head))) {
            if (node.ptr->key + node.ptr->length == pair.offset) {
                pair.length = pair.length + node.ptr->length;
                pair.offset = node.ptr->key;
                ((Class_004dbd00*)alloc)->FUN_004dbd00(node.ptr);
            }
        }
        ((Class_004dbbc0*)alloc)->FUN_004dbbc0(&it3, &pair);
        }
        LeaveCriticalSection(cs);
    }
}