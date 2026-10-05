// Decompiled by space-bunny-free. Names are provisional.
// A hand-written walk over a std::map<unsigned int, int>: the argument is the
// map's value_type (a base offset plus a block length) passed by value, and its
// address is what the insert at 0x4dce60 copies into the new node. Two
// "block covers offset" tests come before the node is erased; then the tree is
// searched for the offset and the pair inserted if nothing is there.
// DAT_00528a54 is the tree's _Nil node, head->left is begin() and
// head->parent is the root. Same shape as 0x4db450 and 0x4db7d0.
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
#include <yvals.h>

struct Node_004db000 {
    Node_004db000* left;               // +0x0
    Node_004db000* parent;             // +0x4
    Node_004db000* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The map's value_type: the block's base offset and its length, 8 bytes.
struct Pair_004db000 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

extern Node_004db000* DAT_00528a54;

// The tree's iterator: one pointer. FUN_004dd2a0 is its operator--.
class Class_004dd2a0 {
public:
    Node_004db000* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db000* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
};

// The tree's own methods, all called on the map itself. FUN_004dc130 is
// erase(), FUN_004dce60 is an insert() that returns through a hidden pointer
// (its return type has a constructor), so its result arrives in eax as the
// address of the caller's temporary.
class Class_004dd250 {
public:
    Node_004db000* FUN_004dd250(const unsigned int& kv);
};

class Class_004dc130 {
public:
    Class_004dd2a0 FUN_004dc130(Class_004dd2a0 it);
};

class Class_004dce60 {
public:
    Class_004dd2a0 FUN_004dce60(Node_004db000* x, Node_004db000* y,
                                 Pair_004db000* v);
};

// A pair-like helper: an iterator and a byte, built by a member function that
// takes both by reference.
class Class_004ddbe0 {
public:
    Class_004dd2a0 field_0;                    // +0x0
    unsigned char field_4;                     // +0x4

    Class_004ddbe0() {}
    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& first, unsigned char& second);
};

struct Less_004db000 {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

class FreeBlockMap {
public:
    Less_004db000 key_compare;                  // +0x0
    Node_004db000* head;                        // +0x4
    unsigned char rebuild;                      // +0x8
    int size;                                   // +0xc
    int unknown_10[20];

    Class_004dd2a0 Begin() { return Class_004dd2a0(head->left); }
    Class_004dd2a0 End() { return Class_004dd2a0(head); }
    // The original tests this as a value (sete; neg; sbb; inc; test), which
    // MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }

    void AddFreeBlock(Pair_004db000 p);
};

// FUNCTION: 0x4db000
void FreeBlockMap::AddFreeBlock(Pair_004db000 p)
{
    Class_004dd2a0 it;
    Class_004dd2a0 it2;
    Class_004ddbe0 result;
    unsigned char inserted;

    Class_004dd2a0 n(((Class_004dd250*)this)->FUN_004dd250(p.offset));
    it = n;
    if (n == Begin()) {
        it = End();
    } else {
        it.FUN_004dd2a0();
    }
    if (Neq(n, End())) {
        if (n.ptr->key == p.length + p.offset) {
            p.length = p.length + n.ptr->length;
            ((Class_004dc130*)this)->FUN_004dc130(n);
        }
    }
    if (Neq(it, End())) {
        Node_004db000* m = it.ptr;
        if (m->length + m->key == p.offset) {
            p.length = p.length + m->length;
            p.offset = m->key;
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
            less = p.offset < x->key;
            x = less ? x->left : x->right;
        }
    }

    if (rebuild) {
        Class_004dd2a0 t(((Class_004dce60*)this)->FUN_004dce60(x, y, &p));
        return;
    }
    it2.ptr = y;
    if (less) {
        if (Class_004dd2a0(y) == Begin()) {
            inserted = 1;
            result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
            goto done;
        }
        it2.FUN_004dd2a0();
    }
    if (key_compare(it2.ptr->key, p.offset)) {
        inserted = 1;
        result.FUN_004ddbe0(((Class_004dce60*)this)->FUN_004dce60(x, y, &p), inserted);
    } else {
        inserted = 0;
        result.FUN_004ddbe0(it2, inserted);
    }
done: ;
}
