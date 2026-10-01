// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash and space-bunny-free, edited by deepseek-v4.1, finished by Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// The allocator's alloc(): look for a free block of `bytes` in the free-block
// map (a std::map<unsigned int, Pair_004db000>, the map's value_type being a
// block's base offset plus its length), erase it, and return the two leftovers
// around the request as new free blocks. If the map is empty, or two passes over
// it find nothing big enough, reserve more address space with VirtualAlloc and
// try again. Same std::map idiom, and the same two-pass search, as 0x4db450 and
// 0x4db000; DAT_005289d4 is the offset of the last block handed out (the
// allocator tries to keep allocating from it), DAT_00528a00 counts the wraps
// around the map, and DAT_00528a54 is the tree's _Nil node.
//
// NOT MATCHING: 80.5 percent, 646 of 646 bytes (Sonnet 5.5, #3275 retry; was
// 76.7 percent). Only one cause is left: the original keeps `this` in ebp and
// `bytes` in ebx (and treats `this` as memory-only in the success block,
// reloading it from [esp+0x10] at every call), we keep them the other way
// round, so ebx/ebp, the success block's `bytes` reloads and the lea/add
// order of `base + len` all differ. All the stack-frame and control-flow
// differences are gone. What fixed them, for the next person:
//  * The reservation code is an inline member returning bool (`Grow` here,
//    `return true` after the insert, `return false` when the halving gives
//    up), and the caller is `if (Grow(bytes)) return FUN_004db1c0(bytes);
//    return 0;`. MSVC then keeps the dead `xor al,al; test al,al; je` arm and
//    its second `return FUN_004db1c0(bytes)` at 0x4db420 by itself, so no
//    uninitialised flag is needed (the old `bool ok;` version read a frame
//    slot instead of using `xor al,al`).
//  * The frame is hdr-style: the erase result is a DISCARDED by-value
//    temporary (`Class_004dd2a0 FUN_004dc130(Class_004dd2a0)`, as in 0x4db450),
//    and the pairs handed to FUN_004dbec0 are temporaries too
//    (`Pair_004db000(base, mark - base)`, a const reference parameter). Both
//    land in one shared temp slot at [esp+0x14] with the lookup key at 0x1c
//    and the result at 0x24, like the original. The insert's result is the
//    8 byte pair<iterator, bool> (`Res_004dbec0`), declared at function scope.
//    With `Class_004dd2a0 res`/`out` as named 4 byte locals the slots came out
//    as out 0x20, res 0x1c, pair 0x18, k 0x24.
//  * With all-temporary pairs and no named `res` MSVC turns the tail recursion
//    into a jump (589 bytes); the named `res` (address taken) prevents that.
// deepseek-v4.1-flash retry (#3363), still 80.5 percent and 646 bytes: the
// register pick is a pure allocator tie-break, not statement order. Flat: 1..4
// dummy `static void dummyN(void) {}` definitions before the function (TU state
// does not move it), `Class_004db000* self = this;` for the two FUN_004dbec0
// calls plus `total += len` (copy-propagates), and the same alias for the
// FUN_004dd250/FUN_004dc130 sites. Giving the parameter's live range an explicit
// end (self = this immediately before the erase call, so `this` dies there)
// grows the body to 650 bytes and drops to 70.1. Note that in ours the success
// block's `mov ebp,[esi+0x10]` clobbers the only `bytes` home, which is exactly
// why the extra `mov edx,[esp+0x30]` reloads and the `sub eax,ebx` appear; flip
// the two homes and every hunk in the diff closes at once.
// Tried this pass and flat or worse for the this/bytes tie: `bytes <= len`,
// `bytes + mark`, `end` as an expression (`mark + bytes` thrice, 650 bytes and
// 75 percent: the length becomes `base + len - mark - bytes` with a different
// operand order than the original's `base - bytes + len - mark`), `const`
// bytes, `size != 0`, unsigned `tries`, a `bytes` copy `n`, swapping the
// declaration order of res and tries, moving `total += len` after the insert,
// dropping `atend`, and a loop that `break`s with the success block after it
// (78.5 percent, 642 bytes).
// deepseek-v4.1-flash retry (#3345), all still 80.5 percent and 646 bytes with
// the same this/bytes swap: `headers.py` found no fixing set (all 80.5);
// 0,4,..400 unused `extern int` declarations before the function (flat, so it
// is source shape, not compiler state); `self`/`&self` aliases for `this`; a
// `bytes` reference and a `bytes*` pointer alias; a named `unsigned int nb =
// bytes;` local fed before the erase call and used after it (the technique-3
// way to keep a value live across a call, but MSVC still reloads the
// parameter); recursive-call and `mark + bytes` inline wrappers that add a use
// without changing the bytes; dead `bytes` self-copies; `atend` as `int` and
// as a `== ? true : false` ternary; and all 24 declaration orders of
// base/len/mark/end in the success block. The tie is between `this` and
// `bytes` for the one register that survives the erase call, and none of these
// flips it; next try changing which value the source keeps across the loop
// (not the number of uses), or find a genuine sibling with the same allocator
// state.
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
    Pair_004db000() {}
    Pair_004db000(unsigned int o, int l) : offset(o), length(l) {}
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
    Class_004dd2a0 FUN_004dc130(Class_004dd2a0 it);
};

struct Res_004dbec0 {
    Class_004dd2a0 it;
    unsigned char ins;
};

class Class_004dbec0 {
public:
    void FUN_004dbec0(Res_004dbec0* out, const Pair_004db000& p);
};

Node_004dd1b0* __cdecl FUN_004dd1b0(Node_004dd1b0* p);

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

    bool Grow(unsigned int bytes)
    {
        unsigned int len = 0x10000000;
        unsigned int base;
        unsigned int dsize;

        if (2 * bytes > len && bytes < 0x40000000u)
            len = ((bytes + 0x1fff) & 0xffffe000) * 2;
        if (bytes > len)
            len = (bytes + 0x1fff) & 0xffffe000;
        dsize = len + 0x2000;
        base = (unsigned int)VirtualAlloc(0, dsize, 0x2000, PAGE_READWRITE);
        for (;;) {
            if (base != 0) {
                if (base + len <= 0x80000000u) {
                    Pair_004db000 q;
                    q.offset = base;
                    q.length = len;
                    total += len;
                    FUN_004db000(q);
                    return true;
                }
            }
            if (base != 0)
                VirtualFree((void*)base, dsize, MEM_RELEASE);
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000 || len < bytes)
                return false;
            dsize = len + 0x2000;
            base = (unsigned int)VirtualAlloc(0, dsize, 0x2000, PAGE_READWRITE);
        }
    }
};

// FUNCTION: 0x4db1c0
unsigned int Class_004db000::FUN_004db1c0(unsigned int bytes)
{
    Res_004dbec0 res;
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
                unsigned int base = cur.ptr->key;
                unsigned int len = cur.ptr->length;
                unsigned int mark;
                unsigned int end;

                // Drop the block from the map, then hand out `bytes` from it,
                // preferring the offset the last allocation used so the free
                // space stays together.
                ((Class_004dc130*)this)->FUN_004dc130(cur);
                mark = DAT_005289d4;
                if (mark == 0) {
                    mark = base;
                    DAT_005289d4 = mark;
                }
                if (mark < base || mark + bytes > base + len)
                    mark = base;
                if (mark > base) {
                    ((Class_004dbec0*)this)->FUN_004dbec0(&res, Pair_004db000(base, mark - base));
                }
                end = mark + bytes;
                if (end < base + len) {
                    ((Class_004dbec0*)this)->FUN_004dbec0(&res, Pair_004db000(end, base + len - end));
                }
                DAT_005289d4 = end;
                return mark;
            }
            cur.ptr = IncNode(cur.ptr);
        } while (tries < 2);
    }

    if (Grow(bytes))
        return FUN_004db1c0(bytes);
    return 0;
}