// Decompiled by space-bunny-free, deepseek-v4.1-flash, deepseek-v4.1, Sonnet 5.5 and Claude Opus 5.5. Names are provisional.
//
// The debug allocator's free-block set, a std::map<unsigned int, unsigned int>
// from a block's base offset to its length, walked by hand. DAT_00528a54 is the
// tree's _Nil node, head->left is begin() and head->parent is the root.

#include <windows.h>
#include <string>
#include <yvals.h>
// Only for their symbol ids: TakeFreeBlock matches only in a window of the
// symbol count (see its comment).
#include <map>
#include <list>
#include <time.h>

struct Pair_004db000 {
    unsigned int offset;               // +0x0
    unsigned int length;               // +0x4
    Pair_004db000() {}
    Pair_004db000(unsigned int o, unsigned int l) : offset(o), length(l) {}
};

struct Node_004db000 {
    Node_004db000* left;               // +0x0
    Node_004db000* parent;             // +0x4
    Node_004db000* right;              // +0x8
    Pair_004db000 value;               // +0xc
    int color;                         // +0x14
};

// The out-of-line _Min, taken on its own opaque node type.
struct Node_004dd1b0 {
    Node_004dd1b0* left;               // +0x0
};

Node_004dd1b0* __cdecl FUN_004dd1b0(Node_004dd1b0* p);

extern Node_004db000* DAT_00528a54;    // the tree's _Nil node
extern unsigned int DAT_005289d4;      // the last offset handed out
extern unsigned int DAT_00528a00;      // how often the search wrapped

// The tree's iterator: one pointer. FUN_004dd2a0 is its _Dec().
class Class_004dd2a0 {
public:
    Node_004db000* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    bool operator!=(const Class_004dd2a0& o) const { return !(*this == o); }
    Pair_004db000& operator*() const { return ptr->value; }
    Pair_004db000* operator->() const { return &ptr->value; }
    Class_004dd2a0& operator++() { Inc(); return *this; }
    Class_004dd2a0 operator++(int) { Class_004dd2a0 tmp = *this; ++*this; return tmp; }
    Class_004dd2a0& operator--() { FUN_004dd2a0(); return *this; }
    Class_004dd2a0 operator--(int) { Class_004dd2a0 tmp = *this; --*this; return tmp; }
    void FUN_004dd2a0();
    void Inc()
    {
        std::_Lockit lock;
        if (ptr->right != DAT_00528a54)
            ptr = (Node_004db000*)FUN_004dd1b0((Node_004dd1b0*)ptr->right);
        else {
            Node_004db000* p;
            while (ptr == (p = ptr->parent)->right)
                ptr = p;
            if (ptr->right != p)
                ptr = p;
        }
    }
};

// A pair-like helper: an iterator and a byte, built by a member function that
// takes both by reference.
class Class_004ddbe0 {
public:
    Class_004dd2a0 first;
    unsigned char second;
    Class_004ddbe0() {}
    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& first, unsigned char& second);
};

class Class_004dd250 { public: Node_004db000* FUN_004dd250(const Pair_004db000& k); };
class Class_004dc130 { public: Class_004dd2a0 FUN_004dc130(Class_004dd2a0 it); };
class Class_004dbec0 { public: Class_004ddbe0 FUN_004dbec0(const Pair_004db000& v); };

// FUN_004dce60 is an insert() that returns through a hidden pointer (its
// return type has a constructor), so its result arrives in eax as the address
// of the caller's temporary.
class Class_004dce60 {
public:
    Class_004dd2a0 FUN_004dce60(Node_004db000* x, Node_004db000* y,
                                 Pair_004db000* v);
};

struct Less_004db000 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class FreeBlockMap {
public:
    Less_004db000 key_compare;         // +0x0
    Node_004db000* head;               // +0x4
    unsigned char rebuild;             // +0x8
    unsigned int count;                // +0xc
    unsigned int total;                // +0x10

    Class_004dd2a0 begin() { return Class_004dd2a0(head->left); }
    Class_004dd2a0 end() { return Class_004dd2a0(head); }
    unsigned int size() const { return count; }
    // The original tests this as a value (sete; neg; sbb; inc; test), which
    // MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }
    Class_004dd2a0 upper_bound(const Pair_004db000& k)
    {
        return Class_004dd2a0(((Class_004dd250*)this)->FUN_004dd250(k));
    }

    void AddFreeBlock(Pair_004db000 p);
    unsigned int TakeFreeBlock(unsigned int bytes);

    // 0x4db450: reserve more address space and add it to the free blocks.
    bool Grow(unsigned int size)
    {
        unsigned int len = 0x10000000;
        unsigned int base;

        if (2 * size > len && size < 0x40000000u)
            len = ((size + 0x1fff) & 0xffffe000) * 2;
        if (size > len)
            len = (size + 0x1fff) & 0xffffe000;
        base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                          PAGE_READWRITE);
        for (;;) {
            if (base != 0 && base + len <= 0x80000000u)
                break;
            if (base != 0)
                VirtualFree((void*)base, len + 0x2000, MEM_RELEASE);
            len = (len >> 1) & 0x7fffe000;
            if (len < 0x10000 || len < size)
                return false;
            base = (unsigned int)VirtualAlloc(0, len + 0x2000, 0x2000,
                                              PAGE_READWRITE);
        }
        total += len;
        AddFreeBlock(Pair_004db000(base, len));
        return true;
    }
};

// Add a free block, merged with its neighbours. The argument is the map's
// value_type (a base offset plus a block length) passed by value, and its
// address is what the insert at 0x4dce60 copies into the new node. Two
// "block covers offset" tests come before the node is erased; then the tree is
// searched for the offset and the pair inserted if nothing is there. Same shape
// as 0x4db450 and 0x4db7d0.
//
// The three Class_004ddbe0 calls (one per exit from the search) are the same
// call in the source, written on one function-scope object, so MSVC 5 merges
// their identical endings (lea ecx; call; epilogue) into the one at 0x4db1a4
// that the first two reach with a jmp. Two details make that happen:
//   * the insert call is passed straight to FUN_004ddbe0 as its first
//     argument, rather than through a named local. MSVC then lays the
//     argument pushes out interleaved (push &inserted, then the insert's four
//     arguments, then push eax for the returned iterator), which is what the
//     original does at 0x4db13f-0x4db154.
//   * that parameter is a const reference. The insert returns a class with a
//     constructor, so MSVC 5 passes a hidden return pointer to it and the
//     callee hands the same pointer back in eax, which becomes the pushed
//     first argument; a by-value or non-const-reference parameter does not
//     reproduce the `push eax`.
// FUNCTION: 0x4db000
void FreeBlockMap::AddFreeBlock(Pair_004db000 p)
{
    Class_004dd2a0 it;
    Class_004dd2a0 it2;
    Class_004ddbe0 result;
    unsigned char inserted;

    Class_004dd2a0 n(((Class_004dd250*)this)->FUN_004dd250(p));
    it = n;
    if (n == begin()) {
        it = end();
    } else {
        it.FUN_004dd2a0();
    }
    if (Neq(n, end())) {
        if (n->offset == p.length + p.offset) {
            p.length = p.length + n->length;
            ((Class_004dc130*)this)->FUN_004dc130(n);
        }
    }
    if (Neq(it, end())) {
        Node_004db000* m = it.ptr;
        if (m->value.length + m->value.offset == p.offset) {
            p.length = p.length + m->value.length;
            p.offset = m->value.offset;
            ((Class_004dc130*)this)->FUN_004dc130(it);
        }
    }

    Node_004db000* y = head;
    bool less = true;
    Node_004db000* x = y->parent;
    {
        std::_Lockit lock;
        while (x != DAT_00528a54) {
            y = x;
            less = p.offset < x->value.offset;
            x = less ? x->left : x->right;
        }
    }

    if (rebuild) {
        Class_004dd2a0 t(((Class_004dce60*)this)->FUN_004dce60(x, y, &p));
        return;
    }
    it2.ptr = y;
    if (less) {
        if (Class_004dd2a0(y) == begin()) {
            inserted = 1;
            result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
            goto done;
        }
        it2.FUN_004dd2a0();
    }
    if (key_compare(it2->offset, p.offset)) {
        inserted = 1;
        result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
    } else {
        inserted = 0;
        result.FUN_004ddbe0(it2, inserted);
    }
done: ;
}

// The allocator's alloc(): look for a free block of `bytes` in the free-block
// set, preferring the block the last allocation came from (DAT_005289d4),
// take it out, hand back the leftovers on either side as new free blocks, and
// if two passes over the set find nothing, reserve more address space (Grow,
// the out-of-line copy of which is 0x4db450) and try again. DAT_00528a00
// counts the wraps around the set and DAT_00528a54 is the tree's _Nil node.
//
// What matched it (it sat at 80.5 percent for ten passes with `this` and
// `bytes` in each other's registers):
//  * The body is the one 0x4dacf0 inlines (matched in #5052), written the same
//    way: STL-style iterators (`lb != begin()`, `it--`, `cur++`, `cur->length`)
//    whose operator--(int) calls _Dec (0x4dd2a0) out of line and whose
//    operator++(int) inlines _Inc with _Min (0x4dd1b0) out of line. Only the
//    callee addresses differ, since this file's copy of the set's members is
//    0x4dd250 (_Ubound), 0x4dc130 (erase) and 0x4dbec0 (insert).
//  * The lookup key `k` is declared at function scope. Inside the `if` MSVC
//    turns the recursive `return TakeFreeBlock(bytes)` into a jump (589 bytes).
//  * In Grow, `len` is declared before `base` (the other order swaps the
//    operands of `base + len`).
//  * In the found block, `len` is declared before `base` but `base` is read
//    first. The declaration order decides the operand order of the second
//    leftover's length (`base - bytes + len - mark` in `base`'s register, with
//    `base + len` from the test not reused), the read order keeps the loads in
//    place.
//  * The symbol count before it is right. With fewer symbols MSVC keeps
//    `base + len` from the test and reuses it for the length, two bytes
//    shorter; a dummy-declaration scan shows the reuse comes and goes with
//    the symbol count in a period of 512.
// FUNCTION: 0x4db1c0
unsigned int FreeBlockMap::TakeFreeBlock(unsigned int bytes)
{
    Pair_004db000 k;
    if (size() > 0) {
        k.offset = DAT_005289d4;
        k.length = 0;
        Class_004dd2a0 lb = upper_bound(k);
        if (lb != begin()) {
            Class_004dd2a0 it = lb;
            it--;
            if (DAT_005289d4 >= it->offset && DAT_005289d4 + bytes <= it->offset + it->length)
                lb = it;
        }
        Class_004dd2a0 cur = lb;
        int tries = 0;
        do {
            if (cur == end()) {
                cur = begin();
                DAT_005289d4 = 0;
                DAT_00528a00++;
                tries++;
            }
            if (cur->length >= bytes) {
                unsigned int len, base;
                base = cur->offset;
                len = cur->length;
                ((Class_004dc130*)this)->FUN_004dc130(cur);
                if (DAT_005289d4 == 0)
                    DAT_005289d4 = base;
                unsigned int mark;
                if (DAT_005289d4 >= base && DAT_005289d4 + bytes <= base + len)
                    mark = DAT_005289d4;
                else
                    mark = base;
                if (mark > base)
                    ((Class_004dbec0*)this)->FUN_004dbec0(Pair_004db000(base, mark - base));
                unsigned int end = mark + bytes;
                if (end < base + len)
                    ((Class_004dbec0*)this)->FUN_004dbec0(Pair_004db000(end, base - mark + len - bytes));
                DAT_005289d4 = end;
                return mark;
            }
            cur++;
        } while (tries < 2);
    }
    if (Grow(bytes))
        return TakeFreeBlock(bytes);
    return 0;
}
