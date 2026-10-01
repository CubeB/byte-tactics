// Decompiled by deepseek-v4.1, finished by Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 87.4% (311 of 311 bytes; every instruction is in place).
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
#include <yvals.h>

struct Pair_004dbbc0 {
    unsigned int offset;
    int length;
};

struct Node_004dbbc0 {
    Node_004dbbc0* left;
    Node_004dbbc0* parent;
    Node_004dbbc0* right;
    Pair_004dbbc0 value;
    int color;
};

extern Node_004dbbc0* DAT_00528a54;

struct Kfn_004dbbc0 {
    const unsigned int& operator()(const Pair_004dbbc0& x) const { return x.offset; }
};

struct Less_004dbbc0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

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
inline Class_004ddbe0::Class_004ddbe0(const Class_004ddbe0& o) : first(o.first), second(o.second) {}
inline Class_004ddbe0::Class_004ddbe0(const Class_004dd2a0& f, const bool& s)
    : first(f), second(s) {}

class Class_004dce60 {
public:
    unsigned char allocator;
    Less_004dbbc0 key_compare;
    Node_004dbbc0* head;
    bool multi;
    int size;

    static Node_004dbbc0*& Left(Node_004dbbc0* p) { return p->left; }
    static Node_004dbbc0*& Right(Node_004dbbc0* p) { return p->right; }
    static const unsigned int& Key(Node_004dbbc0* p) { return Kfn_004dbbc0()(p->value); }
    Node_004dbbc0*& Root() { return head->parent; }
    Node_004dbbc0*& Lmost() { return Left(head); }
    Class_004dd2a0 begin() { return Class_004dd2a0(Lmost()); }

    Class_004dd2a0 FUN_004dce60(Node_004dbbc0* x, Node_004dbbc0* y,
                                const Pair_004dbbc0* v);

    Class_004ddbe0 TreeInsert(const Pair_004dbbc0& V);
    Class_004ddbe0 FUN_004dbbc0(const Pair_004dbbc0& V);
};

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