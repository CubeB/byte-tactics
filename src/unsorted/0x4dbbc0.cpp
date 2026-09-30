// Decompiled by deepseek-v4.1. Names are provisional.
//
// Still differs at 66.1% (311 vs 280 bytes): every byte through the lock
// destructor and the whole loop matches; the differences start at the
// _Multi test.
//  * the three `return Class_004ddbe0(FUN_004dce60(...), true);` sites fold the
//    bool to `mov byte ptr [eax + 4], 1`, while the original materialises it as
//    `mov cl, 1` / `mov byte ptr [eax + 4], cl`. Tried ctor by value, by
//    const bool&, an unsigned char second, member-init list, body assignment,
//    in-class and out-of-class definitions, and a `bool t = true;` local at
//    each site (that one drops to 51.5% and shrinks the frame), all fold or
//    miss in some other way.
//  * because our Multi site and begin() site compile to byte-identical
//    argument setups (V in ecx, the _Insert return buffer in edx), MSVC merged
//    their call+copy tails behind a `jmp`; the original's sites hold V in
//    ecx / edx / eax respectively, so its three copies stay separate. The
//    original's extra `mov cl, 1` live range is the likely reason its
//    allocator coloured the sites differently, so fixing the first point may
//    also un-merge these.
//  * remaining register choices follow from that: begin() test in al
//    (original cl), final key compare zeroes eax (original ecx), last site
//    copy-back in ecx/dl (original edx/cl), and the last site's lea order.
//  * the natural `if (ans) { ... }` spelling above scores 66.1 against the
//    literal `if (!ans) ; else if (...) return ...; else ...;` of the header
//    (60.0), which the previous attempt used; both produce the same merge.
//
// std::_Tree<unsigned int, pair<unsigned int const, int>, ...>::insert(const
// value_type&) from MSVC 5's <xtree> (lines 211-232), for the allocator's
// free-block map whose _Nil sentinel is DAT_00528a54. Hand-written here
// because the tree's _Insert (0x4dce60), iterator::_Dec (0x4dd2a0) and the
// pair<iterator,bool> constructor (0x4ddbe0) are already matched under
// provisional Class_ names, which the real template instantiation would not
// reference. The tree object has <xtree>'s layout: allocator at +0,
// key_compare at +1, head at +4, _Multi at +8, _Size at +0xc.
#include <yvals.h>

struct Node_004dbbc0 {
    Node_004dbbc0* left;               // +0x0
    Node_004dbbc0* parent;             // +0x4
    Node_004dbbc0* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

extern Node_004dbbc0* DAT_00528a54;    // the tree's _Nil sentinel

struct Pair_004dbbc0 {                 // the map's value_type
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Less_004dbbc0 {                 // the key ordering
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004dd2a0 {                 // the tree's iterator
public:
    Node_004dbbc0* ptr;                // +0x0

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dbbc0* p) : ptr(p) {}

    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }

    void FUN_004dd2a0();               // iterator::operator-- (_Dec)
};

class Class_004ddbe0 {                 // pair<iterator, bool>
public:
    Class_004dd2a0 first;              // +0x0
    bool second;                       // +0x4

    Class_004ddbe0();
    Class_004ddbe0(const Class_004dd2a0& f, const bool& s);
    Class_004ddbe0(const Class_004ddbe0& o);

    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& f, const bool& s);
};

inline Class_004ddbe0::Class_004ddbe0() {}
inline Class_004ddbe0::Class_004ddbe0(const Class_004dd2a0& f, const bool& s)
    : first(f), second(s) {}
inline Class_004ddbe0::Class_004ddbe0(const Class_004ddbe0& o)
    : first(o.first), second(o.second) {}

class Class_004dce60 {                 // the tree (std::map-shaped)
public:
    unsigned char allocator;           // +0x0 (empty pool allocator)
    Less_004dbbc0 key_compare;         // +0x1 (empty key ordering)
    Node_004dbbc0* head;               // +0x4 (_Head)
    bool multi;                        // +0x8 (_Multi)
    int size;                          // +0xc (_Size)

    Class_004dd2a0 FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                const Pair_004dbbc0* v);

    Class_004ddbe0 FUN_004dbbc0(const Pair_004dbbc0& V);
};

// FUNCTION: 0x4dbbc0
Class_004ddbe0 Class_004dce60::FUN_004dbbc0(const Pair_004dbbc0& V)
{
    Node_004dbbc0* Y = head;
    bool ans = true;
    Node_004dbbc0* X = head->parent;
    {
        std::_Lockit lock;
        while (X != DAT_00528a54) {
            Y = X;
            ans = key_compare(V.offset, X->key);
            X = ans ? X->left : X->right;
        }
    }
    if (multi)
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004dd2a0 P = Class_004dd2a0(Y);
    if (ans) {
        if (P == Class_004dd2a0(head->left))
            return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
        P.FUN_004dd2a0();
    }
    bool lt = key_compare(P.ptr->key, V.offset);
    if (lt)
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004ddbe0 res;
    res.FUN_004ddbe0(P, false);
    return res;
}
