// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, edited by
// deepseek-v4.1. Names are provisional.
// The red-black tree insert behind std::map<unsigned int, Pair>, the same
// std::map idiom as 0x4db000 and 0x4db450. DAT_00528a54 is the tree's _Nil
// node, head->left is begin() and head->parent is the root. The value is
// placement-new'd into the new node, which is where the null test on the
// destination address comes from.
//
// NOT MATCHING: 95.6 percent, and the byte count is already exact (621 against
// 621). Everything matches byte for byte except 9 instructions, all in the else
// block, and all one decision: which of the two address-taken locals gets eax
// and which gets ecx. The reconstruction below is otherwise complete, so read
// this as "one register tie-break left", not as "far away".
//
// The diff, precisely. The original holds the tail iterator in ecx and `p` in
// eax; this file holds the iterator in eax and `p` in ecx:
//     orig 0x4dc0b0  mov ecx, edi            ours  mov eax, edi
//           0x4dc0b4  mov [esp+0x14], ecx           mov [esp+0x14], eax
//           0x4dc0df  mov ecx, [esp+0x14]           mov eax, [esp+0x14]
//           0x4dc0e3  mov eax, [esp+0x28]           mov ecx, [esp+0x28]
//           0x4dc0f4  push eax                      push ecx
//           0x4dc119  mov eax,[esp+0x24] / mov [eax],ecx
//                                                  mov ecx,[esp+0x24] / mov [ecx],eax
// The stack homes are the same in both (the reloads are from the same
// [esp+0x14] and [esp+0x28]), and the values pushed and stored are the same
// values, so this is purely the allocator's 2-colouring tie-break and not a
// difference in meaning.
//
// Ruled out for it, each compiled and scored by me on top of the earlier pass:
// swapping the two stores in the rebuild branch that falls through to this
// block (95.1), writing those two stores as one whole-struct copy through a
// temporary (76.3), binding the comparison to a named bool local first (89.8),
// and negating and swapping the comparison's operands, which is semantically
// the same test, `!key_compare(p->offset, it.ptr->key)` (94.5). The earlier
// pass had already ruled out `it.ptr = y` against a constructor and `it = y`,
// `it == Begin()` in three other spellings, `(&it)->FUN_004dd2a0()`,
// `out->field_0.ptr = it.ptr`, passing `w` or `q` or `q = w` as the rotation
// argument, and all five declaration orderings of node, it, y, less and x. One
// of those orderings ought to have moved the node's stack slot and produced
// identical code instead, which says the allocator is not ordering by
// declaration here at all.
//
// The wanted shape, for whoever picks this up: at the entry to the else block
// the iterator temp must be ecx and the `p` reload must be eax, and that choice
// has to be made before the `test bl, bl` branch. Since perturbing the
// preceding block did not move it, the likely lever is the allocator state
// entering the block, not the text inside it.
//
// Checked by deepseek-v4.1-flash against the real compiler source (this is
// _Tree::insert, toolchain/msvc5-sp3/INCLUDE/XTREE lines 211-232), which
// confirms the shape above is the source. Re-scored with check.py --sym, all
// keeping `it` in eax: the literal STL `iterator _P = iterator(_Y);` form
// (88.4, and it collapses the frame to three homes because _P is block scoped),
// `it = Class_004dd2a0(y)` and a named temp plus copy (95.6), comparing
// `Class_004dd2a0(y) == Begin()` instead of `it == Begin()` (95.6), an early
// `return` in the rebuild branch in place of the if/else (95.6), and the literal
// `if (!_Ans) ; else if (...) ... else ...` spelling (95.6). This is the one
// allocator 2-colouring and no spelling of the else block moves it.
//
// One more pass (space-bunny-free) added no fix and three flat results, so
// do not spend them again:
//  - tools/headers.py 0x4dbec0: all 128 header sets score exactly 95.6%. The
//    header set does not mirror the two scratch registers here, unlike the
//    0x41bde0 case in the guide;
//  - the guide's "compiler state" sweep, N unused `extern int dummyI;` in front
//    for N = 0 to 400 in steps of 4 (101 compiles, all 95.6%): completely
//    inert, so the remaining difference is not reachable by changing what the
//    compiler knows before this function;
//  - fresh spellings, all worse or inert: the iterator ctor-initialised inside
//    the else arm, `Class_004dd2a0 it(y);` (88.4%, the local loses its stack
//    home and the frame drops to three), the begin() test written as a direct
//    member compare, `it.ptr == head->left` (89.1%, and 88.2% together with
//    the next one, so the `operator==` call and the separate value load are
//    load bearing), the final test with the operands swapped,
//    `p->offset > it.ptr->key` (93.8%), the begin() result bound to a named
//    bool (89.8%), and the insert arm as an early `return` with the else arm as
//    a bare block (95.6%, inert, like the if/else it replaces).
//
// What the nine instructions are, reduced to one bit: the register is chosen
// for the *local*, at its birth, not for a later temporary. The original
// assigns the address-taken iterator's enregistered copy to ecx, this file to
// eax, and the other eight instructions are all consequences (the reload of p
// takes whichever is left, and `out` follows p into the closing store). So the
// one thing to move is the local's register, and the two spellings that change
// how that local is born (ctor init, raw member compare) both break it, which
// leaves the field store plus the `operator==` call as the only shape that
// holds the local together at all.
//
// Second pass (deepseek-v4.1) confirms that reading and rules out four more
// spellings, all byte-identical at 95.6 percent (621 bytes): the compare's
// first operand read through an inline `_Mynode()` accessor, the iterator
// declared inside the else arm, the birth written as `it = Class_004dd2a0(y)`,
// and the whole else arm wrapped in a block with the iterator outside it. A
// by-value inline compare helper is byte-identical too but keeps the same
// colouring. A helper around the insert call that takes the iterator as an
// unused parameter is 91.5 (650 bytes, the extra layer is not folded), and
// `--it` through an inlined `operator--` is 95.6 and byte-identical. The
// matched sibling 0x4db000 has the same idiom and reloads its iterator into
// ecx, but its birth is a plain `mov [esp+0x18], edi` with no temp copy, so
// the 0x4dbec0 birth does go through a value copy and only the choice of that
// copy's register is left.
//
// Third pass (deepseek-v4.1) ruled out five more spellings, none of which moves
// the birth register: the final compare as a raw `it.ptr->key < p->offset`
// (94.0 percent, 615 bytes, worse: it drops the Less functor and 6 bytes), the
// birth as `Class_004dd2a0 t(y); it = t;`, the tail store as
// `out->field_0 = Class_004dd2a0(it);`, an explicit `else` around the
// `it.FUN_004dd2a0();` call, and the birth cast through `(Node_004dbec0*)(void*)`
// (the last four byte-identical at 95.6 percent). Together with the sweeps
// above (128 header sets, 101 dummy externs) this says the tie-break is not
// reachable from declarations, headers, context or any spelling of the two
// statements that surround it, and the file is left at its best score.
//
// Fourth pass (deepseek-v4.1) added nine more inert spellings, all byte
// identical at 95.6 percent (621 bytes): `out` aliased to a named pointer
// local either at the top of the else arm or only in the closing arm, the
// compared key bound to a local, the node pointer bound to a local before
// the compare, `y` copied through a named node temp before the birth, `p`
// copied to a named local at the top of the else arm, the closing store as
// `*(Class_004dd2a0*)&out->field_0 = it;`, as `out->field_0 = it.ptr;` and as
// `out->field_0 = Class_004dd2a0(it);`, a named `out` alias in the rebuild
// branch, and the loop key read through a `const unsigned int&`. Binding the
// insert result to a named iterator is 87.6 percent (652 bytes, the temporary
// gains its own home). So the birth register is not steerable by any operand,
// temporary, alias or scope in either arm; the colouring is a function-wide
// allocator decision that this source cannot reach.
//
// Fifth pass (deepseek-v4.1) confirms the note above: 260 further check runs,
// none of them moved the birth register. Singles that stay byte-identical at
// 95.6 percent: `key_compare` in the loop instead of the raw `<`, `++size` and
// `size++`, `x = head->parent`, `y = this->head`, top-level `const` on either
// parameter, a by-value `Less::operator()` signature, the tail store as a comma
// expression, a fused `while (q != head->parent && q->parent->color == 0)`
// guard, the loop's x update as an if/else, the node/it declarations swapped,
// a const-qualified Begin and `(*it.ptr).key` / `(*p).offset` operand spellings.
// Moving the `it` declaration into the else arm drops to 88.4 percent (the frame
// re-homes), and every two- and three-way combination of the inert singles
// (175 builds) is byte-identical at 95.6 percent too. So the one remaining
// difference really is a function-wide allocator 2-colouring: the original
// keeps the iterator temp in ecx and the `p` reload in eax, this source gives
// the temp eax and the reload ecx, and no reachable spelling flips it.
//
// Two things this function does that are worth writing down, both confirmed
// here and neither obvious from the disassembly:
//  - The null test on the destination address, `lea eax,[edx+0xc]; cmp eax,ebx;
//    je`, is placement new: `new ((void*)&node->key) Pair_004dbec0(*p)`. The
//    same idiom is already used at 0x4c5d60.
//  - A constant lives in a register when its variable is dead. The rebuild
//    path re-tests the key with `key_compare(p->offset, y->key)` rather than
//    reusing the loop's `less` bool; once `less` is dead its register (bl) is
//    finished with, and MSVC materialises the constant 0 in it, so every
//    colour test becomes `cmp dword ptr [reg+0x14], ebx` (a memory-operand
//    compare) instead of `mov reg,[mem]; test reg,reg`. Writing that third
//    condition as a fresh comparison is what unlocked the zero register.
//  - `head->+4` is the root and `head->+0` is begin(). The empty-tree case
//    writes y->left (which is head->left), then head->parent (the root), then
//    head->right. The fixup is only needed on the header's two extremes, which
//    is correct and not a bug: the new node is a leaf under y, so y's parent's
//    pointer to y is unchanged.
#include <yvals.h>
#include <new.h>

// The map's value_type: the key's block base offset and its length.
struct Pair_004dbec0 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

struct Node_004dbec0 {
    Node_004dbec0* left;               // +0x0
    Node_004dbec0* parent;             // +0x4
    Node_004dbec0* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14 (0 = red)
};

extern Node_004dbec0* DAT_00528a54;

// The tree's iterator: one pointer. FUN_004dd2a0 is its operator--.
class Class_004dd2a0 {
public:
    Node_004dbec0* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004dbec0* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
};

// The out parameter: the resulting iterator and whether it was inserted.
struct Class_004ddbe0 {
    Class_004dd2a0 field_0;            // +0x0
    unsigned char field_4;             // +0x4
};

// The tree's own methods. FUN_004dce60 returns an iterator through a hidden
// pointer, so its result arrives in eax as the address of the caller's
// temporary. FUN_004ddc00 is _Buynode, FUN_004dd150 _Lrotate and
// FUN_004dd1f0 _Rrotate, all called on the map itself.
class Class_004dce60 {
public:
    Class_004dd2a0 FUN_004dce60(Node_004dbec0* x, Node_004dbec0* y,
                                 Pair_004dbec0* v);
};

class Class_004ddc00 {
public:
    Node_004dbec0* FUN_004ddc00(Node_004dbec0* parent, int color);
};

class Class_004dd150 {
public:
    int unknown_0;
    Node_004dbec0* head;               // +0x4
    void FUN_004dd150(Node_004dbec0* x);
};

class Class_004dd1f0 {
public:
    int unknown_0;
    Node_004dbec0* head;               // +0x4
    void FUN_004dd1f0(Node_004dbec0* x);
};

struct Less_004dbec0 {
    bool operator()(const unsigned int& a, const unsigned int& b) const
    {
        return a < b;
    }
};

class Class_004dbec0 {
public:
    Less_004dbec0 key_compare;         // +0x0
    Node_004dbec0* head;               // +0x4
    unsigned char rebuild;             // +0x8
    int size;                          // +0xc
    int unknown_10[20];

    Class_004dd2a0 Begin() { return Class_004dd2a0(head->left); }

    void FUN_004dbec0(Class_004ddbe0* out, Pair_004dbec0* p);
};

// FUNCTION: 0x4dbec0
void Class_004dbec0::FUN_004dbec0(Class_004ddbe0* out, Pair_004dbec0* p)
{
    Node_004dbec0* node;
    Class_004dd2a0 it;
    Node_004dbec0* y = head;
    bool less = true;
    Node_004dbec0* x = y->parent;
    {
        std::_Lockit lock;
        while (x != DAT_00528a54) {
            y = x;
            less = p->offset < x->key;
            x = less ? x->left : x->right;
        }
    }

    if (rebuild) {
        {
            std::_Lockit lock2;
            node = ((Class_004ddc00*)this)->FUN_004ddc00(y, 0);
            node->left = DAT_00528a54;
            node->right = DAT_00528a54;
            new ((void*)&node->key) Pair_004dbec0(*p);
            size = size + 1;
            if (y == head || x != DAT_00528a54
                || key_compare(p->offset, y->key)) {
                y->left = node;
                if (y == head) {
                    head->parent = node;
                    head->right = node;
                } else if (y == head->left) {
                    head->left = node;
                }
            } else {
                y->right = node;
                if (y == head->right)
                    head->right = node;
            }

            Node_004dbec0* q = node;
            while (q != head->parent) {
                if (q->parent->color != 0)
                    break;
                Node_004dbec0* g = q->parent->parent;
                if (q->parent == g->left) {
                    Node_004dbec0* z = g->right;
                    if (z->color == 0) {
                        q->parent->color = 1;
                        z->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->right) {
                            q = q->parent;
                            ((Class_004dd150*)this)->FUN_004dd150(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd1f0*)this)->FUN_004dd1f0(q->parent->parent);
                    }
                } else {
                    Node_004dbec0* z = g->left;
                    if (z->color == 0) {
                        q->parent->color = 1;
                        z->color = 1;
                        q->parent->parent->color = 0;
                        q = q->parent->parent;
                    } else {
                        if (q == q->parent->left) {
                            q = q->parent;
                            ((Class_004dd1f0*)this)->FUN_004dd1f0(q);
                        }
                        q->parent->color = 1;
                        q->parent->parent->color = 0;
                        ((Class_004dd150*)this)->FUN_004dd150(q->parent->parent);
                    }
                }
            }
            head->parent->color = 1;
        }
        out->field_0 = node;
        out->field_4 = 1;
    } else {
        it.ptr = y;
        if (less) {
            if (it == Begin()) {
                out->field_0 = ((Class_004dce60*)this)->FUN_004dce60(x, y, p);
                out->field_4 = 1;
                return;
            }
            it.FUN_004dd2a0();
        }
        if (key_compare(it.ptr->key, p->offset)) {
            out->field_0 = ((Class_004dce60*)this)->FUN_004dce60(x, y, p);
            out->field_4 = 1;
        } else {
            out->field_0 = it;
            out->field_4 = 0;
        }
    }
}
