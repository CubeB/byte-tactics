// Decompiled by Space Bunny Free, finished by space-bunny-free and GPT-6.1-sol, edited by deepseek-v4.1, retried by Sonnet 5.5, retried by space-bunny-free. Names are provisional.
// RETRY NOTES (space-bunny-free, issue 3331, second pass, still 98.1%, no
// MATCH; 48 scratch variants scored, none better). The file below is
// unchanged from the 98.1% version. Three displacement-only hunks remain:
//   * the `this` spill (0x4db459 / 0x4db4fc) is at esp0-0x0c here and at
//     esp0-0x14 in the original, which is n's home;
//   * the first erase's out-pointer is `&n` at esp0-0x14 and the original has
//     esp0-0x10 there, which is it2's home;
//   * (the second erase's out-pointer and the insert's iterator argument are
//     already esp0-0x10 and match.)
// WHAT THE MATCHED SIBLING SAYS ABOUT THE ERASE (new, and probably the real
// shape). 0x4db000 is a MATCH and calls the same callee 0x4dc130, which it
// declares as `Class_004dd2a0 FUN_004dc130(Class_004dd2a0 it)`: by value in,
// by value out, result discarded. So the "out-pointer" declaration used here
// is a displacement-shaped stand-in for a by-value return, and the original
// almost certainly reads
//     it2 = ((Class_004dc130*)this)->FUN_004dc130(n);
//     ...
//     it2 = ((Class_004dc130*)this)->FUN_004dc130(it);
// with the dead assignments to one function-scope object, which is the same
// idiom 0x4db000 needs for its three FUN_004ddbe0 calls. The callee's hidden
// return pointer is then `&it2`, which is exactly why both erase `lea`s in
// the original are `lea reg, [esp + 0x18]` = esp0-0x10 = it2's home.
// Scored: that shape is 87.2% (K1) because MSVC 5 swaps the two iterator
// homes (n to esp0-0x10, it2 to esp0-0x14). Assigning the two erase results
// to `n` instead of `it2`, either or both, is 90.4% (L2, L3, L4); assigning
// them to `it2` and giving the insert `&n` is 87.2% (L5). With the by-value
// return and the result simply discarded, with no assignment at all (B1), the
// score is 97.4% and the two hidden-return temps land at esp0-0x0c, sharing
// the this-spill's slot, while n (esp0-0x14) and it2 (esp0-0x10) are already
// right. So B1 is the closest of the three shapes: it needs its two temps to
// overlay it2's home instead of taking the free dword.
// HOW THE FRAME IS HANDED OUT (the useful part of this pass). There is one
// 8-byte local, the value_type `p`, at esp0-0x08/esp0-0x04, i.e. at the
// bottom of the five-dword local area; the two 4-byte iterator locals are
// handed out top-down in ALLOCATE order (n = esp0-0x14, it2 = esp0-0x10,
// because n's address is first taken at the lower_bound call); and every
// temporary (the `this` spill, and in B1 the by-value return temps) is then
// given the lowest still-free 4-byte slot, esp0-0x0c. To match, the `this`
// spill and the return temps must instead OVERLAY n's and it2's homes, and
// MSVC 5 will not do that here: a temp can only take a local's home when that
// local is dead first, and n and it2 are both live across the erases in every
// spelling that reaches esp0-0x10.
// Inert this pass, all 98.1% with the same three hunks: `self` as a named
// local at the top of the body, at the tail, or as `Class* const` / `Class&`
// (MSVC folds it back into `this`, so it can never own a frame home while
// `this` is live across the reservation loop: the value is the same value, so
// the parameter stays live and the spill stays); the `it` alias declared
// first; the erase out-parameter as a `Class_004dd2a0&`; the erase value
// parameter as `Node_004db450*`; `FUN_004dbeb0` returning void instead of
// `int*`; the insert's iterator parameter as a reference; `n` and `it2`
// initialised with `Class_004dd2a0()`; `len` assigned after its declaration;
// `base = 0` in its declaration; a folded-branch dead store pinning `n` or
// `it2` (at the top of the body, just before `it = n`, and after the insert);
// `&n` / `&it2` spelled through a ternary or an extra cast; an inline no-op
// helper taking `int&`/`int&` called on a dead local, to try to make esp0-0x0c
// a real reserved home (three spellings, no effect, so a local whose address is
// only ever taken by an inlined no-op still gets no frame slot); and the
// by-value erase with a dead self-assignment `n = n;` after the insert, or with
// a dead folded store pinning `n` there, or with the insert taking `&n`
// (all 87.2%, the same swap).
// Worse this pass: a third iterator local for the first erase's out-parameter
// (79.2% in three declaration orders); splitting the begin result and the
// erase out-pointers into two separate variables so their (disjoint) live
// ranges could share a slot (83.1%); wrapping the whole tail in a nested
// block, or just `it2`, or just `n` (83.8% / 83.8% / 85.1%); splitting `p`
// into two 4-byte locals (56.4% / 57.5%, so the 8-byte value_type sitting at
// the bottom of the frame is load-bearing); the pair's `length` field first
// (91.6%); the begin() test written `it.ptr == it2.ptr` (83.9%); and the
// merge tests through `Neq` with explicit iterator copies (88.0%); and `it2`
// typed `Node_004db450*` with a cast at each of its four out-uses (83.9%).
// Confirmed still best: `FUN_004dc130(&n, n)` plus `FUN_004dc130(&it2, it)`,
// 98.1%, tied with `(&it2, &n)`.
// GPT-6.1-sol issue #3131 retry: 97.4% (444/444), eight check.py invocations
// including the worker's checks, no MATCH. `this` alias and reversed local
// order tied; 128 header sets also tied. Four stack-home displacements remain.
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
// RETRY NOTES (space-bunny-free, issue 3331, 98.1% now, no MATCH; was 97.4%).
// THE LEVER WAS THE ERASE'S OUT-PARAMETER SPELLING, and it is worth another
// pass. Declaring
//     void FUN_004dc130(Class_004dd2a0* out, Class_004dd2a0 it);
// and calling it as `FUN_004dc130(&n, n)` for the first merge and
// `FUN_004dc130(&it2, it)` for the second takes the score from 97.4% to 98.1%
// (three displacement-only hunks left, 444 of 444 bytes). So the discarded
// by-value result (which gave a hidden return pointer at a fresh temp slot) is
// NOT what the original has: it passes an out-pointer, and the two out-pointers
// are `&n` and `&it2` themselves. Note how much the spelling moves the frame:
// with `&n` and `&it2` the two four-byte homes are n at esp0-0x14, it2 at
// esp0-0x10, the this-spill at esp0-0x0c and no hole; with `&it2` for BOTH
// erases MSVC 5 swaps the two homes (it2 at esp0-0x14, n at esp0-0x10) and the
// score drops to 93.5%, and the same swap happens whatever the declaration
// order is. Combinations of {&n, &it2, &it} for the two erases, all scored:
// (&n,&it2) 98.1%, (&it2,&n) 98.1%, (&n,&n) 97.4%, (&it,&it) 97.4%, (&it,&it2)
// 93.5%, (&it2,&it) 93.5%, (&it2,&it2) 93.5%; the same again with the
// lower_bound out-parameter as a reference: (&n,&it2) 98.1%, (&it2,&it) 93.5%,
// the rest unchanged.
// The frame layout is pinned exactly and it is the SAME in the original and
// here: n at esp0-0x14, it2 at esp0-0x10, a four-byte slot at esp0-0x0c,
// p.offset at esp0-0x08 and p.length at esp0-0x04 (every [esp+N] of the
// original was re-derived from the push count at each site, so esp0-0x0c
// really is unreferenced). What is left is one allocation state: in the
// original the this-spill (0x4db459/0x4db4fc) shares esp0-0x14 with n and
// both erase out-pointers share esp0-0x10 with it2, so esp0-0x0c is a hole;
// here the this-spill takes esp0-0x0c and the first erase's out-pointer is
// `&n` at esp0-0x14, and that last one is the only hunk that is not a
// displacement of the this-spill. n and it2 already have the right slots
// (0x4db508's `lea ecx,[esp+0x10]` and 0x4db525's `lea eax,[esp+0x14]` are
// not in the diff), so this is a temp-versus-dead-local slot reuse decision,
// not a layout difference: the this-spill is a compiler temporary whose live
// range [0x4db459, 0x4db4fc] is disjoint from n's, and MSVC 5 gives it the
// one otherwise-unused frame dword instead of reusing n's dead home. Putting
// the first out-pointer at esp0-0x10 needs `&it2`, and that spelling is the
// one that swaps n and it2.
// Re-tried on the 98.1% base and all inert (same three hunks): dead stores in
// six positions, `self` as `Class* const` and `Class&`, `total = len + total`,
// `total += len` through an inline `AddTotal` member, an uncalled
// `static inline`, `n` and `it2` at their point of use, a `Node*` temporary
// for the first merge test, `p = Pair(base, len)` with a Pair constructor, a
// POD iterator, `Neq` as a free inline function, the lower_bound out-parameter
// as a reference and the insert's two parameters as references, and seven
// declaration orders of p/n/it2.
// Tried at 97.4% and inert (same four displacement-only hunks, 444 of 444
// bytes): 1, 2, 3 and 4 uncalled `static inline` functions, an `Identity`
// wrapper uncalled and called on the `it = n` copy, Key/Length/Head/MkPair
// inline helpers folded into the existing uses, dead stores in ten positions,
// `self` as `Class*`, `Class* const`, `Class&` and a point-of-use declaration
// with all four calls rewritten to it, `total = total + len`, `total = len +
// total`, the three p-store orders, `p = Pair(base, len)` with and without a
// Pair constructor, the iterator class with no default constructor, as a POD
// (with an `It()` helper for `head`), with an explicit copy constructor
// (48.3%) or `operator=`, the lower_bound and begin out-params as references
// instead of pointers, the insert's two parameters as references, `it` as a
// `Class_004dd2a0*` pointer alias instead of a reference, `Neq` as a free
// inline function, with pointer or const-reference parameters, `head` through
// a local, each merge test in its own nested scope, `p` through a reference,
// the reservation loop restructured with a `continue`, the `it` alias through
// two different casts, and the whole reservation loop moved into an inline
// `Grow` member taking the pair by reference (63.5%) or returning it (71.4%).
// Worse: an inline wrapper returning the iterator by value around lower_bound
// (56.0%), around begin (88.0%), around both (44.6%), a live extra 4-byte
// local (58.4%), a named `Node*` for the lower_bound result (51.1%), the two
// iterators in an array (87.7%), a copy of `p` for the insert (73.1%), a union
// holding `this` and the lower_bound result so they share one slot (71.6%),
// and the by-value erase's result assigned to `it2` (87.2% for both, 89.0%
// for the second only). Not tried: giving esp0-0x0c a real home, i.e. a
// named local there, which is what the 0x4db7d0 notes call "a genuine hole
// at 0x1c" on this same allocator.
// Two permuter runs on the 98.1% base (seeds 11 and 12, 6 minutes each, 1300
// and 1555 candidates) found nothing, so the residual is not reachable by
// meaning-preserving rewrites of this function and the `Neq` helper. The
// iterator pair in one named two-member struct, with the erase out-pointers
// naming its members, is 87.7% in all four combinations, so forcing the two
// homes adjacent does not help either.
// TECHNIQUE WORTH A GUIDE ENTRY (it moved this function 97.4% -> 98.1%): when
// a call's RESULT IS DISCARDED and the callee returns a class with a
// constructor, MSVC 5 materialises a hidden return pointer in a fresh frame
// temp. Declaring the same callee with an explicit out-pointer instead, and
// passing the address of a local you already have, removes that temp and moves
// the frame. The choice of WHICH local to pass is itself a strong lever: here
// `FUN_004dc130(&n, n)` and `FUN_004dc130(&it2, it)` give 98.1% while
// `FUN_004dc130(&it2, n)` and `FUN_004dc130(&it2, it)` swap the two iterator
// homes and give 93.5%, and declaration order cannot pull them back. So when
// two four-byte homes keep the wrong order, try naming each of them as an
// out-pointer argument before you try the declaration list.
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
            ((Class_004dc130*)this)->FUN_004dc130(&n, n);
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
