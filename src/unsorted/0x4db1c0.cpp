// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash and
// space-bunny-free, edited by deepseek-v4.1. Names are provisional.
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
// PASS deepseek-v4.1: 69.3% -> 74.4%, frame 614 -> 646 bytes, exactly the
// original frame, from one line: `bool ok;` left UNINITIALISED. That keeps the
// original's dead `xor al,al; test al,al; je` arm at 0x4db41a and its duplicate
// `return FUN_004db1c0(bytes)` tail (24 bytes) that `bool ok = false;` folds
// away; `char ok = 0;` folds it away too. The arm now differs only in how the
// flag is read: `mov al,[esp+0x30]` here (the uninitialised slot aliases the
// bytes parameter slot, C4700) vs `xor al,al` in the original. The remaining
// diff is unchanged and is all the one cause described below, the this/bytes
// tie (ebx vs ebp). Tried this pass and inert: `unsigned int n = bytes` used
// in the search test or the success block (still 69.3%), char flag.
// NOT MATCHING yet (was 69.3%, now 74.4% with `bool ok;`)., 614 of 646 bytes). What still differs, measured
// against the original at 0x4db1c0:
//   * 0x4db1c0 (the one big cause): the original keeps `this` in ebp and `bytes`
//     in ebx; we keep them the other way round, and everything downstream
//     follows: in the second half the original has mark in esi, base in edi,
//     bytes in ebx and length in ebp, while we have `this` in esi, mark in
//     edi, base in ebx and length in ebp with `bytes` reloaded from [esp+0x30].
//     The original's `this` is memory-only from 0x4db253 on (it reloads
//     [esp+0x10] at every call site, straight into ecx), ours is promoted back
//     into a register, and that promotion is what steals esi from `mark` and
//     pushes `bytes` out to the stack. So the fix is one construct that either
//     demotes `this` or makes it memory-resident, not a per-instruction fix.
//     The two lowest frame slots are also swapped because of it: the original
//     spills `this` at [esp+0x10] and keeps the erase's out iterator (which
//     reuses the dead `cur` slot) at [esp+0x14], while we spill `this` at
//     [esp+0x14] and put the iterator at [esp+0x10]. Fixing the register
//     should drag the whole frame back with it, since ebp is then overwritten
//     by `length` in the success block and `this` is forced to memory.
//     Tried and inert (all still 69.3%, 614 bytes): a `self = this` copy used
//     for every access (the copy is coalesced away), a local `n = bytes` with
//     every use renamed to `n` (also coalesced), and moving `cur`/`k` to
//     function scope, and an extra outer `for (;;)` around the whole body (the
//     grow-and-retry loop shape: the loop-nesting lever that moved `this` into
//     ebp at 0x40e160 does nothing here, still 69.3% with `mov ebx, ecx`
//     intact). Declaration order is inert here too.
//     Measured more precisely (space-bunny-free, second pass): the tie is
//     between `this` and the parameter `bytes` only, and it is decided at
//     0x4db1c5, where `this` is the single live value. The original then holds
//     `this` in ebp and `bytes` in ebx, so `bytes` must have been assigned
//     FIRST (it took the preferred ebx) and `this` got what was left; in this
//     file `this` is assigned first and takes ebx, so `bytes` is pushed into
//     ebp and, at 0x4db2c7 where the original recycles ebp for `len`, ours
//     loses `bytes` altogether and reloads it from [esp+0x30] in the second
//     half. Everything else in the diff (the `mov ecx, [esp+0x10]` before the
//     pushes at 0x4db2c0, the [esp+0x1c] query pair, the k slot) is a
//     consequence of that one decision, not a separate problem. The next thing
//     to try is whatever makes MSVC 5 order a parameter ahead of `this` in the
//     register allocator: a second live value with a longer range than `this`
//     in the pre-header, or a use of the parameter inside the `size > 0` test.
//   * 0x4db41a: the original keeps a dead `xor al,al; test al,al; je` and an
//     unreachable arm that retries with `return FUN_004db1c0(bytes)`. The flag
//     is a compile-time 0 here, so MSVC folds our `if (ok)` away and the retry
//     block (0x4db420-0x4db437, 24 bytes) is missing. No spelling of a local
//     bool, an uninitialised one, a comparison (`!= 0`, `== 1`), a ternary or
//     an inlined helper returning 0 reproduced the dead test.
//   * 0x4db1df: the query pair lands in [esp+0x24] where the original uses
//     [esp+0x1c], for the frame-layout reason above.
//
// The one scheduling fix found here (by deepseek-v4.1-flash, 59.9% to 69.3%):
// read `mark = DAT_005289d4` AFTER the erase call FUN_004dc130, not before it.
// The original loads `mark` into esi at 0x4db2d5, after the call; assigning it
// before the call changes the scheduler's live ranges and loses 9.4 points.
//
// The reservation loop now matches instruction for instruction, including the
// rotated shape: writing the VirtualAlloc out twice (once before the loop and
// once at its latch) is what produces the second copy at 0x4db3e0, the back
// edge onto the `if (base)` test at 0x4db3ae, and the import addresses hoisted
// into ebp and ebx. Writing the two `if (base != 0)` tests as two separate
// ifs at the same level, rather than nesting the VirtualFree inside one, is
// what keeps the redundant `test eax,eax` at 0x4db3bd; nesting it, or writing
// one `if (base != 0 && base + len <= 0x80000000)`, loses it again. Both the
// success path and the `if (ok)` retry have to be `return`s inside the loop
// for MSVC to lay the exit block out last.
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
};

// FUNCTION: 0x4db1c0
unsigned int Class_004db000::FUN_004db1c0(unsigned int bytes)
{
    Class_004dd2a0 res;
    bool ok;
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
                unsigned int mark;
                unsigned int end;
                Pair_004db000 p;

                // Drop the block from the map, then hand out `bytes` from it,
                // preferring the offset the last allocation used so the free
                // space stays together.
                ((Class_004dc130*)this)->FUN_004dc130(&out, cur);
                mark = DAT_005289d4;
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
                    return FUN_004db1c0(bytes);
                }
            }
            if (base != 0)
                VirtualFree((void*)base, dsize, MEM_RELEASE);
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000 || len < bytes) {
                if (ok)
                    return FUN_004db1c0(bytes);
                break;
            }
            dsize = len + 0x2000;
            base = (unsigned int)VirtualAlloc(0, dsize, 0x2000, PAGE_READWRITE);
        }
        return 0;
    }
}
