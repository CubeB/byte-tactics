// Decompiled by space-bunny-free. Names are provisional.
// The allocator's alloc(): look for a free block of `bytes` in the free-block
// map (a std::map<unsigned int, Pair_004db000>, the map's value_type being a
// block's base offset plus its length), erase it, and return the two leftovers
// around the request as new free blocks. If the map is empty, or two passes over
// it find nothing big enough, reserve more address space with VirtualAlloc and
// try again. Same std::map idiom, and the same two-pass search, as 0x4db450 and
// 0x4db000 in this worktree; DAT_005289d4 is the offset of the last block handed
// out (the allocator tries to keep allocating from it), DAT_00528a00 counts the
// wraps around the map, and DAT_00528a54 is the tree's _Nil node.
//
// NOT MATCHING yet (58.9%, 600 of 646 bytes). What still differs, measured
// against the original at 0x4db1c0:
//   * 0x4db41a: the original keeps a dead `xor al,al; test al,al; je` and an
//     unreachable arm that retries with `return FUN_004db1c0(bytes)`. The flag
//     is a compile-time 0 here, so MSVC folds our `if (ok)` away and the retry
//     block (0x4db420-0x4db437, 24 bytes) is missing. No spelling of a local
//     bool, an uninitialised one, a comparison (`!= 0`, `== 1`), a ternary or
//     an inlined helper returning 0 reproduced the dead test.
//   * 0x4db390 and 0x4db3a8: the original hoists the VirtualAlloc and
//     VirtualFree import addresses into ebp and ebx and calls through the
//     registers; we call `dword ptr [0x4fc1bc]`. Ours keeps `bytes` in ebp
//     where the original keeps it in ebx, so no callee-saved register is free
//     for the import addresses.
//   * 0x4db3e0: the original's reservation loop is rotated, with a second copy
//     of the VirtualAlloc sequence in the latch and the back edge on the test;
//     `for (;;)`, `for (init;; incr)`, `for (init; cond; incr)`, `while` and
//     `do {} while (1)` all come out unrotated here.
//   * 0x4db1c0: the original keeps `this` in ebp and `bytes` in ebx; we keep
//     them the other way round, and the query pair lands in [esp+0x24] instead
//     of [esp+0x1c].
#include <windows.h>
#include <yvals.h>

// The tree node: the map's key (a block's base offset) and length in _Myspace.
struct Node_004db000 {
    Node_004db000* left;               // +0x0
    Node_004db000* parent;             // +0x4
    Node_004db000* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The out-of-line _Min, taken on its own opaque node type.
struct Node_004dd1b0 {
    Node_004dd1b0* left;               // +0x0
};

extern Node_004db000* DAT_00528a54;    // the tree's _Nil node
extern unsigned int DAT_005289d4;      // the last offset handed out
extern int DAT_00528a00;               // how often the search wrapped

// The map's value_type: a block's base offset and its length.
struct Pair_004db000 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

// The tree's iterator: one pointer. FUN_004dd2a0 is its operator--.
class Class_004dd2a0 {
public:
    Node_004db000* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
};

// The tree's own methods, all called on the map itself.
class Class_004dd250 {
public:
    Node_004db000* FUN_004dd250(const unsigned int& kv);
};

class Class_004dc130 {
public:
    void FUN_004dc130(Class_004dd2a0* out, Class_004dd2a0 it);
};

class Class_004dbec0 {
public:
    void FUN_004dbec0(Class_004dd2a0* out, Pair_004db000* p);
};

Node_004dd1b0* FUN_004dd1b0(Node_004dd1b0* p);

// The tree's operator++, with _Min left out of line. Its _Lockit takes over the
// stack slot the iterator local used, which is how the original gets everything
// in there at [esp+0x14].
static inline Node_004db000* IncNode(Node_004db000* cur)
{
    std::_Lockit lock;
    if (cur->right != DAT_00528a54) {
        cur = (Node_004db000*)FUN_004dd1b0((Node_004dd1b0*)cur->right);
    } else {
        Node_004db000* p;
        while (cur == (p = cur->parent)->right)
            cur = p;
        if (cur->right != p)
            cur = p;
    }
    return cur;
}

class Class_004db000 {
public:
    char unknown_0[4];                  // +0x0 the key_compare
    Node_004db000* head;               // +0x4
    unsigned char rebuild;              // +0x8
    unsigned int size;                  // +0xc the number of free blocks
    int total;                          // +0x10 the address space reserved

    Class_004dd2a0 Begin() { return Class_004dd2a0(head->left); }
    Class_004dd2a0 End() { return Class_004dd2a0(head); }
    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }

    void FUN_004db000(Pair_004db000 p);

    unsigned int FUN_004db1c0(unsigned int bytes);
};

// FUNCTION: 0x4db1c0
unsigned int Class_004db000::FUN_004db1c0(unsigned int bytes)
{
    Class_004dd2a0 res;
    bool ok = false;
    int tries = 0;
    if (size > 0) {
        Pair_004db000 k;
        Class_004dd2a0 cur;

        k.offset = DAT_005289d4;
        k.length = 0;
        // The tree's lower_bound-style search for a free block at the offset
        // the last allocation came from.
        cur.ptr = ((Class_004dd250*)this)->FUN_004dd250(k.offset);
        if (Neq(cur, Begin())) {
            // The block before it may hold the request too, if it starts at or
            // below the last offset and reaches past the end of the request.
            Class_004dd2a0 it(cur);
            it.FUN_004dd2a0();
            if (DAT_005289d4 >= it.ptr->key
                && DAT_005289d4 + bytes <= it.ptr->key + it.ptr->length)
                cur = it;
        }

        do {
            // Hitting the end of the map wraps back to the first block; two
            // passes are all the search gets.
            bool atend = cur.ptr == head;
            if (atend) {
                cur.ptr = head->left;
                DAT_00528a00++;
                DAT_005289d4 = 0;
                tries++;
            }
            if (cur.ptr->length >= bytes) {
                Class_004dd2a0 out;
                unsigned int base = cur.ptr->key;
                unsigned int len = cur.ptr->length;
                unsigned int mark = DAT_005289d4;
                unsigned int end;
                Pair_004db000 p;

                // Drop the block from the map, then hand out `bytes` from it,
                // preferring the offset the last allocation used so the free
                // space stays together.
                ((Class_004dc130*)this)->FUN_004dc130(&out, cur);
                if (mark == 0) {
                    mark = base;
                    DAT_005289d4 = mark;
                }
                if (mark < base || mark + bytes > base + len)
                    mark = base;
                if (mark > base) {
                    p.offset = base;
                    p.length = mark - base;
                    ((Class_004dbec0*)this)->FUN_004dbec0(&res, &p);
                }
                end = mark + bytes;
                if (end < base + len) {
                    p.offset = end;
                    p.length = base + len - end;
                    ((Class_004dbec0*)this)->FUN_004dbec0(&res, &p);
                }
                DAT_005289d4 = end;
                return mark;
            }
            cur.ptr = IncNode(cur.ptr);
        } while (tries < 2);
    }

    {
        // Nothing free: reserve a block of at least `bytes`, rounded up to 8k
        // and doubled so the block has room to grow, halving it while the
        // reservation lands above 2Gb.
        unsigned int len = 0x10000000;
        unsigned int base;
        unsigned int dsize;

        if (2 * bytes > len && bytes < 0x40000000u)
            len = ((bytes + 0x1fff) & 0xffffe000) * 2;
        if (bytes > len)
            len = (bytes + 0x1fff) & 0xffffe000;
        for (;;) {
            dsize = len + 0x2000;
            base = (unsigned int)VirtualAlloc(0, dsize, 0x2000, PAGE_READWRITE);
            if (base != 0 && base + len <= 0x80000000u)
                break;
            if (base != 0)
                VirtualFree((void*)base, dsize, MEM_RELEASE);
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000 || len < bytes) {
                if (ok)
                    return FUN_004db1c0(bytes);
                return 0;
            }
        }
        total += len;
        {
            Pair_004db000 q;
            q.offset = base;
            q.length = len;
            FUN_004db000(q);
        }
        return FUN_004db1c0(bytes);
    }
}
