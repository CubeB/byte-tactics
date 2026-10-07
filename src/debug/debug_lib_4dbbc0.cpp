// Decompiled by deepseek-v4.1, Sonnet 5.5, deepseek-v4.1-flash and space-bunny-free. Names are provisional.
//
// The debug allocator's free-block map, std::map<unsigned int, int> with a pool
// allocator: its insert and the tree's _Insert. DAT_00528a54 is the tree's _Nil
// node, head->left is begin() (the leftmost node), head->parent is the root and
// head->right is the rightmost node.

#include <yvals.h>
#include <new.h>

// The map's value_type: the block's base offset and its length, 8 bytes.
struct Pair_004dbbc0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Node_004dbbc0 {
    Node_004dbbc0* left;               // +0x0
    Node_004dbbc0* parent;             // +0x4
    Node_004dbbc0* right;              // +0x8
    Pair_004dbbc0 value;               // +0xc
    int color;                         // +0x14 (0 = red, 1 = black)
};

extern Node_004dbbc0* DAT_00528a54;   // the tree's _Nil node

struct Kfn_004dbbc0 {
    const unsigned int& operator()(const Pair_004dbbc0& x) const { return x.offset; }
};

// A method that ignores `this`: its caller sets ecx to the tree, which sits at
// the allocator's +0, and pushes the node size.
class Class_004ddd70 {
public:
    void* FUN_004ddd70(unsigned int n);
};

// The tree's key ordering, unsigned (as in 0x4db000).
struct Less_004dbbc0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

// The tree's iterator: one pointer, with constructors, so it is returned
// through a hidden pointer.
class Class_004dd2a0 {
public:
    Node_004dbbc0* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dbbc0* p) : ptr(p) {}

    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    Node_004dbbc0* Mynode() const { return ptr; }

    void FUN_004dd2a0();
};

class Class_004ddbe0 {
public:
    Class_004dd2a0 first;
    bool second;

    Class_004ddbe0();
    Class_004ddbe0(const Class_004dd2a0& f, const bool& s);
    Class_004ddbe0(const Class_004ddbe0& o);

    Class_004ddbe0* FUN_004ddbe0(const Class_004dd2a0& f, const bool& s);
};

inline Class_004ddbe0::Class_004ddbe0() {}
// The byte is copied before the iterator on purpose. The implicit memberwise
// copy gives MSVC 5 the first free register for `first`, so the return slot is
// filled with ecx/dl where the original uses edx/cl; reversing the two
// assignments puts the byte in cl and the iterator in edx at all four return
// sites, which is what the original does. The two-argument constructor below
// keeps its initialiser list, and with it the stores stay in member order.
inline Class_004ddbe0::Class_004ddbe0(const Class_004ddbe0& o)
{
    second = o.second;
    first = o.first;
}
inline Class_004ddbe0::Class_004ddbe0(const Class_004dd2a0& f, const bool& s)
    : first(f), second(s) {}

class Class_004dce60 {
public:
    unsigned char allocator;           // +0x0
    Less_004dbbc0 key_compare;         // +0x1
    Node_004dbbc0* head;               // +0x4
    bool multi;                        // +0x8
    int size;                          // +0xc

    static Node_004dbbc0*& Left(Node_004dbbc0* p) { return p->left; }
    static Node_004dbbc0*& Right(Node_004dbbc0* p) { return p->right; }
    static const unsigned int& Key(Node_004dbbc0* p) { return Kfn_004dbbc0()(p->value); }
    Node_004dbbc0*& Root() { return head->parent; }
    Node_004dbbc0*& Lmost() { return Left(head); }
    Class_004dd2a0 begin() { return Class_004dd2a0(Lmost()); }

    // Left rotation of x, shaped like std::_Tree<...>::_Lrotate.
    void Lrotate(Node_004dbbc0* x)
    {
        std::_Lockit lock;
        Node_004dbbc0* y = x->right;
        x->right = y->left;
        if (y->left != DAT_00528a54)
            y->left->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
        y->left = x;
        x->parent = y;
    }

    // Right rotation of x, shaped like std::_Tree<...>::_Rrotate.
    void Rrotate(Node_004dbbc0* x)
    {
        std::_Lockit lock;
        Node_004dbbc0* y = x->left;
        x->left = y->right;
        if (y->right != DAT_00528a54)
            y->right->parent = x;
        y->parent = x->parent;
        if (x == head->parent)
            head->parent = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
        y->right = x;
        x->parent = y;
    }

    Class_004dd2a0 FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                const Pair_004dbbc0* v);

    Class_004ddbe0 TreeInsert(const Pair_004dbbc0& V);
    Class_004ddbe0 FUN_004dbbc0(const Pair_004dbbc0& V);
};

// Space Bunny Free (2026-10-01): this closes the ecx/edx question every earlier
// round was stuck on. The original copies the pair into the return slot with the
// byte in cl and the dword in edx at all four exits; the implicit memberwise
// copy always gives the dword ecx and the byte dl. The lever is the order of
// the two member writes in a user-defined COPY constructor: writing `second`
// before `first` (as a constructor body, not as an initialiser list, which
// MSVC 5 sorts back into declaration order) makes MSVC give cl to the byte and
// edx to the dword. That one change fixes all four sites at once, including the
// three true exits built by the two-argument constructor further down, which
// keeps its initialiser list and therefore its ascending stores. Writing both
// constructors that way (the shape that reaches 96.9% on its own) puts the byte
// store first at every site, so only the copy constructor wants reversing.
// Also tried on the way, all worse or neutral: reversing the initialiser list,
// `second` as unsigned char or char, as a nested one-byte class, `first` as a
// base class, the constructors' parameters by value (a real MSVC temporary:
// ascending stores and cl for the byte, but the return slot lands in edx, the
// dword in eax, and the function grows a `mov eax,edx` because a
// struct-returning function must leave the buffer pointer in eax), a named bool
// local at each return, a temporary byte computed before the two assignments
// (that merges the three tails into one shared block), and a 49-way sweep of
// copy-constructor form x two-argument-constructor form.
// deepseek-v4.1-flash (#3301 retry, 2026-10-01): still 87.4%, 311/311. New
// evidence on the residual: the original's pair copy is MSVC's reverse-order
// memberwise class copy (byte first: `mov cl,[src+4]; mov edx,[src];
// mov [dst],edx; mov [dst+4],cl`). That is also the code MSVC 5 emits for a
// class copy after an out-of-line copy-ctor call, for field-by-field stores
// into a fresh local, and for a two-arg construction whose source is opaque
// (a pointer deref, e.g. `return A(p->x, p->y)`); the two-arg ctor inlined
// from a known local (the real <map> source here) always comes out in
// forward order (dword first, ecx/dl). So the original's ctor arguments are
// evaluated right-to-left (byte live range starts first: it takes cl, and the
// dword then has to take edx), which MSVC 5 only does for a real call, never
// for the inlined member-init list. Every source form found so far that
// produces the reverse-order copy adds an 8-byte local and grows the frame
// from 0x10 to 0x18. Tried this round, all 87.4% or worse: a 40-combination
// sweep of return form x ctor parameter form x bool/unsigned char; pointer
// parameters; a second layout-identical pair type for the tree's result with
// a converting ctor; pair derived from the iterator; out-of-line two-arg and
// copy-ctor definitions; const-reference and direct-init locals; explicit
// member locals in both orders; static_cast/reinterpret_cast returns; helper
// inline functions; 1..31 dummy types or globals before the function;
// headers.py --cpp (768 header sets, best 87.4%). Best lead for the next
// attempt: force a reverse-order class copy from _Ans (E-8) to the return
// slot without a second 8-byte local; the false exit (`mov cl,[E-4];
// mov edx,[E-8]`) and the true exits (`mov edx,[eax]; mov cl,1`) both need
// that same right-to-left lowering. A minimal model shows the choice is made
// by the surrounding IR, not by the copy: an out-of-line writer that takes
// the destination as a pointer argument (`void f(P* out, ...); P t;
// f(&t, x); return P(t.f, t.s);`) gives exactly edx/cl, while the same writer
// as a member call with `this` = destination (`t.fill(It(x), false); return
// t;`) gives ecx/dl with a byte-identical copy sequence. The original's ctor
// call is a member call (this = E-8, two stack args, ret 8), so this file's
// ecx/dl follows from that; what made the original pick edx/cl is still open.
// deepseek-v4.1-flash (#3118 retry): still 87.4%, 311/311. Re-tried the pair
// return as: ctor body assignment, second as int, first/second by value one at
// a time, init-list order swapped, a redundant iterator self-assignment
// (`it = ans.first; ans.first = it;`) and bool self-assignment in the wrapper,
// an explicit copy ctor on the iterator, `return ans;`, and explicit
// field-by-field stores into a fresh out local (worse, 83.7). All stayed at
// 87.4% or fell, so this is the compilation-state tie already recorded below.
// deepseek-v4.1-flash (#2937 retry): still 87.4%. The only residual is a global
// ecx<->edx role swap in the pair return copy (original dword->edx/byte->cl, ours
// dword->ecx/byte->dl), consistent at all four exits. `second` as unsigned char/
// bool/by-value/ref ctor, ctor-body assignment, `return ans;`, `return TreeInsert(V);`,
// a trivial iterator and canonical pair members all stayed at 87.4% or fell; the
// prior worker already swept headers.py (128 sets) and the 0..400 dummy decls. This
// is a compilation-state tie, not a source bug.
// This is std::map<unsigned int, int, less, PoolAlloc>::insert(const
// value_type&) from MSVC 5's <map> (lines 87-89), not the tree's own insert:
//     _Pairib insert(const value_type& _X)
//         {_Imp::_Pairib _Ans = _Tr.insert(_X);
//          return (_Pairib(_Ans.first, _Ans.second)); }
// with <xtree>'s _Tree::insert (TreeInsert below, lines 211-232) inlined into
// it. Writing the tree's insert alone (the earlier attempt, 66%) cannot
// reproduce the `mov cl, 1` tails: they are the `_Ans.second` copy of a
// constant `true`, forwarded through the local `_Ans`. The local is also what
// gives the original its 0x10 byte frame, and _Insert (0x4dce60, which the
// real template reproduces byte for byte) is the out-of-line call at three
// sites because the inlined insert is nested one level down.
//
// What still differs (all one register-assignment choice, repeated at the four
// exits): the original copies `_Ans` into the return slot with `first` in edx
// and the byte in cl (`mov edx,[eax]; mov eax,[ret]; mov cl,1; mov [eax],edx;
// mov [eax+4],cl`, and at the false exit loads the byte before the dword); we
// get first in ecx and the byte in dl. Tried without effect: ctor params by
// value / `const unsigned char&` / `int`, an implicit copy ctor, `return ans;`,
// a named `out` local, `second(s), first(f)` initialiser order, a second
// constructor with no bool, headers.py (128 header sets) and 0-400 unused
// declarations in front.
// The real template, compiled with a pool allocator, gives the same flag
// registers (cl for the begin() test, eax for the head) and `mov dl, 1`; the
// original calls the out-of-line pair constructor (0x4ddbe0) at the false exit
// where the real template inlines it, so the original's inliner had run out of
// budget by then. The hand-written classes here stand in for the template's so
// that callee names match symbols.csv (Class_004dce60 = _Insert, Class_004dd2a0
// = iterator::_Dec, Class_004ddbe0 = pair<iterator, bool>'s constructor).
inline Class_004ddbe0 Class_004dce60::TreeInsert(const Pair_004dbbc0& V)
{
    Node_004dbbc0* X = Root();
    Node_004dbbc0* Y = head;
    bool Ans = true;
    {
        std::_Lockit Lk;
        while (X != DAT_00528a54) {
            Y = X;
            Ans = key_compare(Kfn_004dbbc0()(V), Key(X));
            X = Ans ? Left(X) : Right(X);
        }
    }
    if (multi)
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004dd2a0 P = Class_004dd2a0(Y);
    if (!Ans)
        ;
    else if (P == begin())
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    else
        P.FUN_004dd2a0();
    if (key_compare(Key(P.Mynode()), Kfn_004dbbc0()(V)))
        return Class_004ddbe0(FUN_004dce60(X, Y, &V), true);
    Class_004ddbe0 res;
    res.FUN_004ddbe0(P, false);
    return res;
}

// FUNCTION: 0x4dbbc0
Class_004ddbe0 Class_004dce60::FUN_004dbbc0(const Pair_004dbbc0& V)
{
    Class_004ddbe0 ans = TreeInsert(V);
    return Class_004ddbe0(ans.first, ans.second);
}

// The out-of-line tree insert for the allocator's free-block map, shaped like
// std::_Tree<...>::_Insert from MSVC 5's <xtree> but written out by hand.
//
// Under the outer std::_Lockit a 0x18-byte node is carved from the pool
// (0x4ddd70), the pair is placement-new'd into it, the map's size is bumped and
// the node is linked in. Then the red-black fixup walks up from the new node
// with a cursor z, colouring and rotating until z's parent is black or the root
// is reached, and finally blackens the root.
// FUNCTION: 0x4dce60
Class_004dd2a0 Class_004dce60::FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                            const Pair_004dbbc0* v)
{
    std::_Lockit lock;
    Node_004dbbc0* p = (Node_004dbbc0*)((Class_004ddd70*)this)->FUN_004ddd70(0x18);
    p->parent = y;
    p->color = 0;                      // red
    p->left = DAT_00528a54;
    p->right = DAT_00528a54;
    new ((void*)&p->value) Pair_004dbbc0(*v);
    ++size;

    // A positive disjunction through the bool comparator: keeps the left-child block as the then-part.
    if (y == head || x != DAT_00528a54 || key_compare(v->offset, y->value.offset)) {
        y->left = p;
        // The empty-tree case comes first and updates head->right, not head->left.
        if (y == head) {
            head->parent = p;          // the root
            head->right = p;           // the rightmost node
        } else if (y == head->left) {
            head->left = p;            // the leftmost node
        }
    } else {
        y->right = p;
        if (y == head->right)
            head->right = p;
    }

    Node_004dbbc0* z = p;
    // Explicit break, and z->parent->... re-read from the cursor: parent/grandparent locals cost a register.
    while (z != head->parent) {
        if (z->parent->color != 0)
            break;

        if (z->parent == z->parent->parent->left) {
            Node_004dbbc0* u = z->parent->parent->right;
            if (u->color == 0) {
                // Red uncle: recolour and carry on two levels up.
                z->parent->color = 1;
                u->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    Lrotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                Rrotate(z->parent->parent);
            }
        } else {
            Node_004dbbc0* u = z->parent->parent->left;
            if (u->color == 0) {
                z->parent->color = 1;
                u->color = 1;
                z->parent->parent->color = 0;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    Rrotate(z);
                }
                z->parent->color = 1;
                z->parent->parent->color = 0;
                Lrotate(z->parent->parent);
            }
        }
    }
    head->parent->color = 1;
    return Class_004dd2a0(p);
}
