// Decompiled by Space Bunny Free, finished by space-bunny-free and GPT-6.1-sol, edited by deepseek-v4.1, retried by Sonnet 5.5. Names are provisional.
// Sonnet 5.5 retry (#2450), still 93.5%: the original is almost certainly
// std::map wrapper calls (lower_bound, begin, erase, insert) that MSVC inlines,
// around tree functions it calls out of line, which is why the iterator homes
// are shared compiler temporaries. A real std::map with a pool allocator
// (see 0x4db610) cannot be used here: /Ob2 inlines lower_bound, begin and the
// whole tree insert (638 bytes, 39%), where the original calls 0x4dc620,
// 0x4dbeb0 and 0x4dbec0. Mimicking the wrappers by hand, each as an inline
// member returning the iterator by value and calling the out-of-line tree
// function, was scored in all 16 combinations of {lower_bound, begin, erase}
// as wrapper or explicit out-pointer, with and without the `size` slot alias
// below: the best is 93.5% (this file); every wrapper combination is worse
// (44 to 88%, frame 0x18 or more, because each wrapper return adds a temp).
// Keep the alias: without it the same file scores 82.5%.
// NOT A MATCH: 93.5% (deepseek-v4.1), 444 of 444 bytes, every remaining
// difference is a stack-slot displacement. Still differs (see the end of this
// comment, and the UPDATED note below for what deepseek-v4.1 changed and why
// the old 92.9% claims about declaration order were wrong):
//   * The two map iterators get the wrong two frame slots. The original puts
//     the lower_bound result in the first frame slot ([esp+0x10]) and reuses
//     that same slot for the spilled `this`; mine puts the begin() result in
//     the first slot and gives `this` a slot of its own at [esp+0x18], one
//     dword higher. Every diff below is that one rotation: it is a single
//     allocation state, not nine independent problems.
//   * `lea edx, [eax + esi]` versus `lea edx, [esi + eax]`: the operands of
//     the commutative `base + len` are in the original's order, mine is
//     reversed. Writing the sum the other way round does not change it.
// Frame arithmetic, since it pins the target down (esp0 = esp at entry; the
// four pushes leave esp = esp0-0x24, so the five locals sit at esp0-0x14,
// -0x10, -0x0c, -0x08, -0x04 and the one stack argument at esp0+0x04):
//   - esp0-0x14  `this`'s spill at 0x4db459, reloaded at 0x4db4fc, then
//                overwritten by the _Ubound result (0x4db508/0x4db521) and
//                passed by address to the final insert (0x4db5e7). The
//                original reuses ONE slot for all four roles.
//   - esp0-0x10  the begin() out slot (0x4db525, 0x4db539), then the second
//                erase's hidden return slot (0x4db58a).
//   - esp0-0x0c  NOTHING: the one dword of this five-dword frame that the
//     original never touches (see the correction below).
//   - esp0-0x08  p.offset, esp0-0x04 p.length: `p` is one 8-byte Pair.
// So the original has FOUR homes and NO separate spill slot for `this`; this
// file has four homes plus a real spill slot at esp0-0x0c. Getting the spill to
// land on the top slot is the whole remaining problem.
// CORRECTION to the slot list above (re-derived from the operand bytes: the
// "third erase" line was wrong). 0x4db5d3's `lea ecx,[esp+0x18]` is not
// esp0-0x0c. It has the same single outstanding argument push as 0x4db58a's
// `lea eax,[esp+0x18]`, so both are the same absolute slot, esp0-0x10, the
// begin() out slot: BOTH erase temporaries share the one begin() slot, and
// esp0-0x0c is not referenced by any instruction at all. Counting the pushes
// again for every [esp+N] of the original: disp 0x10 with 4 pushes = esp0-0x14,
// 0x14 = esp0-0x10, 0x18 with 1 push = esp0-0x10, 0x1c = esp0-0x08, 0x20 =
// esp0-0x04, 0x28 with 4 pushes = esp0+0x04 (the argument, `size` then `it`),
// 0x2c with 5 pushes = esp0+0x04. The frame is therefore one dword WIDER than
// the four homes it uses, which is why `sub esp,0x14` must not be read as "five
// homes". The whole residual is one allocation state: the original lays the
// 4-byte homes out as [this-spill + _Ubound result][begin() out + both erase
// temps][slack] and reuses the spilled `this` slot for the _Ubound result,
// while this file lays them out as [begin() out + erase temp 1][_Ubound result
// + erase temp 2][this spill] and gives the spill a dword of its own.
// Tried and did NOT work (all scored by check.py --sym, all 92.9% or worse):
//   * permuting the declaration order of `n`, `it2`, `base` and `p` (all 24
//     orders, with and without the operand swap): no effect at all, so the
//     slot order here is NOT driven by declaration order;
//   * re-running six declaration orders of `n`, `it2`, `tmp`, `base`, `p` with
//     a fresh 4-byte home for the third erase's return: every one is 92.9%,
//     and with `tmp` declared at all the frame grows to `sub esp,0x18` with
//     the `this` spill given a slot of its own (82.5%). Adding the fifth home
//     therefore does NOT make MSVC reuse a dead home, it makes the frame wider,
//     which rules out "one home too few" as the cause;
//   * swapping the roles, so the _Ubound result lands where begin()'s does and
//     the other way round (92.9%): the two four-byte homes keep the order they
//     have whatever the declaration order, so they cannot be permuted from the
//     source text at all;
//   * inline `End()` / `Begin()` accessors in place of `Class_004dd2a0(head)`,
//     and `it = End()` in place of `it.ptr = head` (the idiom 0x4db000 uses):
//     no effect;
//   * passing the map's value_type as `const Pair_004db450&` instead of
//     `const unsigned int&`, and dropping the `(int*)&it2` cast: no effect;
//   * packing the two iterators into one 8-byte local, to force adjacent
//     slots: 87.0%, the pair lands at [esp+0x14] and the local at [esp+0x1c];
//   * declaring the call's two parameters in the other order (value first):
//     92.2%;
//   * making the members inherited instead of cast: the base classes shift
//     `total` to +0x14: 81.8%;
//   * a real named local for `it` instead of the parameter-slot alias: 81.8%,
//     so the alias onto the dead `size` slot is load bearing.
// Added by deepseek-v4.1-flash, both still 92.9% and 444 of 444 bytes with the
// identical nine-hunk slot rotation this file already had:
//   * declaring the lower_bound callee as `Class_004dd2a0 FUN_004dc620(const
//     unsigned int&)` and writing `Class_004dd2a0 n = ...FUN_004dc620(p.offset)`
//     compiles byte-for-byte the same object as the explicit
//     `void FUN_004dc620(Class_004dd2a0*, const unsigned int&)` out-param call,
//     so MSVC 5 elides the copy and the frame is not reachable that way either;
//   * moving the `it` reference alias to the top of the declaration list (and
//     deleting it from its old position) changes nothing, so the alias's
//     position is inert too.
// Added by space-bunny-free, all 92.9% and 444 of 444 bytes unless stated, so
// the slot order here is inert to everything a reader would try next:
//   * the two 4-byte homes are also inert to the LOCAL NAMES. Renaming `n`/`it2`
//     to aaa/zzz, to zzz/aaa and to q1/q2, declarations and code otherwise
//     untouched, gives three byte-identical objects. Together with the 24
//     declaration orders above this rules out both "declaration order" and
//     "symbol name" as the driver, so the layout is not a hash or source order
//     effect that a rename or a reorder can reach;
//   * `base` as a `char*` instead of an `unsigned int`, with
//     `(unsigned int)(base + len) <= 0x80000000u` and a plain `VirtualFree(base,
//     ...)`: still `lea edx,[esi + eax]`, so the SIB base/index choice for the
//     one `lea` difference is NOT driven by the operand being pointer
//     arithmetic rather than integer addition. This is the natural next guess
//     for that single instruction and it is wrong;
//   * the two calls declared as the real STL shapes (the lower_bound returning
//     the iterator by value into `n`: 55.4% and 438 bytes, it deletes the
//     `total += len` store; the begin() returning the iterator by value, with
//     `it2 = *f(&it2)`: 84.1% and 446 bytes, the copy is NOT elided, MSVC emits
//     the extra store and reorders `total += len` ahead of it). Both are worse
//     than the explicit out-parameter calls, so the out-parameter spelling in
//     this file is the right one, not an accident.
// Grows the allocator: reserves a block of at least `size` bytes with
// VirtualAlloc (rounded up to 8k, and doubled so the block has room to grow),
// retrying with half the size while the reservation lands above 2Gb, and then
// records the block in the free-block map, merging it with the neighbours it
// touches. Same std::map idiom as 0x4db000: the block is the map's value_type,
// a base pointer and a length, and the _Ubound result plus the decremented
// iterator are compared with the tree's head (End()).
// UPDATED by deepseek-v4.1: the residual is now EIGHT hunks at 93.5% (was nine at
// 92.9%). The fix was declaration order after all: with `unsigned int len` and
// `unsigned int base` declared BEFORE `Pair_004db450 p` and the two iterators
// declared LAST, the commutative `base + len` in the 2Gb test compiles to the
// original `lea edx,[eax + esi]` instead of `lea edx,[esi + eax]`, so the
// earlier note that declaration order is inert is WRONG (that note only ever
// permuted the four dword locals n/it2/base/p; moving `len` in front of `base`
// is what flips the SIB base/index choice). What is still wrong is only the
// home rotation: the original keeps the spilled `this` AND the _Ubound result
// in the first frame dword (esp0-0x14) and the begin() result in esp0-0x10,
// leaving esp0-0x0c unused, while this file gives esp0-0x14 to the begin()
// result, esp0-0x10 to the _Ubound result and esp0-0x0c to the `this` spill.
// Everything else (444 of 444 bytes) is identical. Tried here and inert:
// a named `self = this` local instead of the parameter (same 9 hunks), the
// _Ubound destination declared as `Node_004db450*` (worse, 85.4%, and it also
// loses the Neq idiom), n and it2 in one declaration statement (92.9%),
// n/it2 declared after len+base+p in the ORIGINAL order (93.5%, this file).
// Added by deepseek-v4.1 (all 93.5%, 444 of 444 bytes, the same eight hunks):
//   * declaration orders len/base/n/it2/p, len/base/n/p/it2, len/base/it2/n/p,
//     n/it2/len/base/p, p/n/it2/len/base, n/len/base/it2/p and
//     len/n/it2/base/p: byte-identical objects, so the slot rotation is not
//     reachable by reordering the declaration block;
//   * declaring n, it2, p and base at their point of use (a seed pattern from
//     the board) is inert too, and so is an extra dead 4-byte local;
//   * the erase and the insert declared as by-value-returning STL shapes
//     (`it2 = f(x)`): 456 bytes, 87.2%, the hidden return pointer grows the
//     frame to 0x18.
// The single root difference left: it2 lands at temp0 here and temp1 in the
// original (n and the spilled `this` follow it one slot down), so the erase
// pair temp sits at temp0/temp1 here instead of temp1/temp2. The emitted
// instruction sequence is otherwise identical, so that one allocation state is
// not steered by anything the declaration list or the call spelling can reach.
#include <windows.h>

struct Node_004db450 {
    Node_004db450* left;               // +0x0
    Node_004db450* parent;             // +0x4
    Node_004db450* right;              // +0x8
    unsigned int key;                  // +0xc
    int length;                        // +0x10
    int color;                         // +0x14
};

// The tree's iterator: one pointer. FUN_004dd2a0 is its operator--.
class Class_004dd2a0 {
public:
    Node_004db450* ptr;

    Class_004dd2a0() {}
    Class_004dd2a0(Node_004db450* q) : ptr(q) {}
    bool operator==(const Class_004dd2a0& o) const { return ptr == o.ptr; }
    void FUN_004dd2a0();
};

// The map's value_type: the block's base address and its length, 8 bytes.
struct Pair_004db450 {
    unsigned int offset;               // +0x0
    int length;                        // +0x4
};

// The map's own methods, all called on the allocator itself.
class Class_004dc620 {
public:
    void FUN_004dc620(Class_004dd2a0* out, const unsigned int& kv);
};

class Class_004dc130 {
public:
    void FUN_004dc130(Class_004dd2a0* out, Class_004dd2a0 it);
};

// begin(), out of line: it stores the tree's first node through its argument.
class Class_004dbeb0 {
public:
    int* FUN_004dbeb0(int* p);
};

class Class_004dbec0 {
public:
    void FUN_004dbec0(Class_004dd2a0* it, Pair_004db450* p);
};

class Class_004db450 {
public:
    char unknown_0[4];
    Node_004db450* head;               // +0x4
    char unknown_8[8];
    int total;                         // +0x10

    // The original tests the iterators as a value (sete; neg; sbb; inc; test),
    // which MSVC 5 only does for a `!` applied to a bool-returning member.
    bool Neq(Class_004dd2a0 a, Class_004dd2a0 b) { return !(a == b); }

    bool FUN_004db450(unsigned int size);
};

// FUNCTION: 0x4db450
bool Class_004db450::FUN_004db450(unsigned int size)
{
    unsigned int len = 0x10000000;
    unsigned int base;
    Pair_004db450 p;
    Class_004dd2a0 n;
    Class_004dd2a0 it2;
    // Once the reservation loop is done the parameter is dead, and the original
    // reuses its stack slot for the map iterator.
    Class_004dd2a0& it = *(Class_004dd2a0*)&size;

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
    p.offset = base;
    p.length = len;
    ((Class_004dc620*)this)->FUN_004dc620(&n, p.offset);
    it = n;
    ((Class_004dbeb0*)this)->FUN_004dbeb0((int*)&it2);
    if (it == it2)
        it.ptr = head;
    else
        it.FUN_004dd2a0();
    if (Neq(n, Class_004dd2a0(head))) {
        if (n.ptr->key == p.offset + p.length) {
            p.length = p.length + n.ptr->length;
            ((Class_004dc130*)this)->FUN_004dc130(&it2, n);
        }
    }
    if (Neq(it, Class_004dd2a0(head))) {
        if (it.ptr->key + it.ptr->length == p.offset) {
            p.length = p.length + it.ptr->length;
            p.offset = it.ptr->key;
            ((Class_004dc130*)this)->FUN_004dc130(&it2, it);
        }
    }
    ((Class_004dbec0*)this)->FUN_004dbec0(&it2, &p);
    return true;
}