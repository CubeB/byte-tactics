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
// Only for its symbol ids: TakeFreeBlock matches only in a window of the
// symbol count (see its comment).
#include <malloc.h>

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

Node_004dd1b0* __cdecl FindLeftmost(Node_004dd1b0* p);

extern Node_004db000* DAT_00528a54;    // the tree's _Nil node
extern unsigned int g_lastAllocOffset;  // the last offset handed out
extern unsigned int g_freeBlockWraps;  // how often the search wrapped

// The tree's iterator: one pointer. PrevNode is its _Dec().
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
    Class_004dd2a0& operator--() { PrevNode(); return *this; }
    Class_004dd2a0 operator--(int) { Class_004dd2a0 tmp = *this; --*this; return tmp; }
    void PrevNode();
    void Inc()
    {
        std::_Lockit lock;
        if (ptr->right != DAT_00528a54)
            ptr = (Node_004db000*)FindLeftmost((Node_004dd1b0*)ptr->right);
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
    // const reference: the hidden return pointer of insert is what gets pushed.
    Class_004ddbe0* Assign(const Class_004dd2a0& first, unsigned char& second);
};

class Class_004dd250 { public: Node_004db000* LowerBound(const Pair_004db000& k); };
class Class_004dc130 { public: Class_004dd2a0 Erase(Class_004dd2a0 it); };
class Class_004dbec0 { public: Class_004ddbe0 Insert(const Pair_004db000& v); };

// Insert is an insert() that returns through a hidden pointer (its
// return type has a constructor), so its result arrives in eax as the address
// of the caller's temporary.
class Class_004dce60 {
public:
    Class_004dd2a0 Insert(Node_004db000* x, Node_004db000* y,
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
        return Class_004dd2a0(((Class_004dd250*)this)->LowerBound(k));
    }

    void AddFreeBlock(Pair_004db000 p);
    unsigned int TakeFreeBlock(unsigned int bytes);

    // 0x4db450: reserve more address space and add it to the free blocks.
    bool Grow(unsigned int size)
    {
        // len declared before base: the other order swaps the operands of base + len.
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

// 0x4db000 AddFreeBlock is defined in src/debug/debug_lib.cpp; this file
// keeps only TakeFreeBlock, which matches in its own symbol context.

// The allocator's alloc(): look for a free block of `bytes` in the free-block
// set, preferring the block the last allocation came from (g_lastAllocOffset),
// take it out, hand back the leftovers on either side as new free blocks, and
// if two passes over the set find nothing, reserve more address space (Grow,
// the out-of-line copy of which is 0x4db450) and try again. g_freeBlockWraps
// counts the wraps around the set and DAT_00528a54 is the tree's _Nil node.
// FUNCTION: 0x4db1c0
unsigned int FreeBlockMap::TakeFreeBlock(unsigned int bytes)
{
    // Function scope, not inside the if: otherwise the recursive return becomes a jump.
    Pair_004db000 k;
    if (size() > 0) {
        k.offset = g_lastAllocOffset;
        k.length = 0;
        // STL-style iterator operators (--, ++, ->) throughout: their inlining shapes the code.
        Class_004dd2a0 lb = upper_bound(k);
        if (lb != begin()) {
            Class_004dd2a0 it = lb;
            it--;
            if (g_lastAllocOffset >= it->offset && g_lastAllocOffset + bytes <= it->offset + it->length)
                lb = it;
        }
        Class_004dd2a0 cur = lb;
        int tries = 0;
        do {
            if (cur == end()) {
                cur = begin();
                g_lastAllocOffset = 0;
                g_freeBlockWraps++;
                tries++;
            }
            if (cur->length >= bytes) {
                // len declared before base but base read first: sets operand and load order.
                unsigned int len, base;
                base = cur->offset;
                len = cur->length;
                ((Class_004dc130*)this)->Erase(cur);
                if (g_lastAllocOffset == 0)
                    g_lastAllocOffset = base;
                unsigned int mark;
                if (g_lastAllocOffset >= base && g_lastAllocOffset + bytes <= base + len)
                    mark = g_lastAllocOffset;
                else
                    mark = base;
                if (mark > base)
                    ((Class_004dbec0*)this)->Insert(Pair_004db000(base, mark - base));
                unsigned int end = mark + bytes;
                if (end < base + len)
                    ((Class_004dbec0*)this)->Insert(Pair_004db000(end, base - mark + len - bytes));
                g_lastAllocOffset = end;
                return mark;
            }
            cur++;
        } while (tries < 2);
    }
    if (Grow(bytes))
        return TakeFreeBlock(bytes);
    return 0;
}
