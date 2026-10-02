// Decompiled by Opus, edited by deepseek-v4.1, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// MATCHES (161 bytes). The original keeps the field_1fe load first and
// accumulates in edi only while the 32-bit multiply chain is signed, yet it
// converts the product to float as unsigned (fild qword with a zero high
// word). Every explicit unsigned conversion of the 32-bit product makes MSVC 5
// reassociate the chain and evaluate the /5 quotient first (or, with a 16-bit
// narrowing cast, keeps the order but pays an "and edx, 0xffff"). The fix is
// to build the 64-bit numerator by hand: store the signed 32-bit product in
// the low word of an __int64 and zero the high word. The 32-bit expression
// stays signed, so the original order and registers survive, and the __int64
// conversion is the unsigned fild qword the original has. A union with an
// int lo / int hi pair works; MSVC folds the two stores into the original
// "mov [esp+8], edi" plus the hoisted "mov [esp+0xc], 0". Verified MATCH with
// both the union and the equivalent "*(int*)&p = ...; *((int*)&p+1) = 0;".
// claude-opus-5-5 (#4428): unchanged; a 10-minute permuter run (about 500 candidates) found nothing.
// Also tried (all compile to the division-first order, 159 bytes): no cast, a
// separate int or unsigned local for field_1fe or for the sub-product, compound
// *= statements, inline Level()/Rank() helpers, (unsigned)/(long)/(int) casts and
// `/ 5u`. Only a 16-bit cast keeps field_1fe first: (short) gives 94.4% (movsx).
// Codex / GPT-6 retest in #13:
// signed intermediate products, a scale helper and a widened
// multiplier did not preserve both load order and unsigned float conversion.
// Retain the original best partial, including its redundant 16-bit mask.
//
// deepseek-v4.1 in #1995: 0x437a50 is a byte-identical twin of this function
// (same unit offsets, same 10.0f / 300.0f constants, only the _ftol call and
// the jg target differ) and it has NO "and edx, 0xffff", so a mask-free source
// does exist. A cast-free product of the same four factors gives 52.9%: MSVC5
// computes the division into edx first and loads field_1fe afterwards, so the
// accumulator is edx, not edi. Every narrowing cast tried, (unsigned short),
// (short), (unsigned char), restores the original A-first order and the exact
// original registers, but always appends "and edx, 0xffff" (or the matching
// mask) and turns the file into 167 bytes vs 161. MSVC5 does not elide that
// mask even when the value range is obvious: a micro-test whose operand is an
// unsigned short parameter still emits it, so no cast spelling removes it.
// Still differs by that one instruction (and the jg target it shifts,
// 0x4386e9 vs 0x4386ef).
//
// deepseek-v4.1 retest in #1995: the "twin at 0x437a50" note above is wrong.
// 0x437a50 sits inside Class_00437a30 (a container grow routine) and the whole
// exe has exactly one "fcomp dword [0x4fd2a4]" site, at 0x438665, so there is
// no second copy of this function to copy a mask-free shape from. New variants
// tried this session (signed int field_1fa with an unsigned third parameter,
// the same with only the last factor cast, (unsigned int) around the whole
// product, a q local of int/unsigned int declared before the product, an
// assignment of q inside the product, and moving the (unsigned short) cast to
// the C factor) all reproduce one of two shapes: 96.6% with the mask, or the
// 52.9% shape where MSVC hoists the magic division by 5 above the field_1fe
// load (accumulator edx instead of edi, b->type homed in edi instead of esi).
// Every 16-bit narrowing of the quotient forces the original order and always
// pays the 6-byte and edx,0xffff, so the order and the mask appear to be the
// same front-end decision and this file stays at 96.6%.
// Tried and rejected: all 24 factor orders of (field_1fe, (field_b8+5)/5,
// field_1fa, n) and two-part groupings, an unsigned int numerator local
// (declared first, declared after the float, assigned later, ++-style `*=`
// chain), the operands read through extra locals and pointers, widened casts
// and redundant casts on either side, +0 / *1 / |0 / 0u pads, 5u and
// (unsigned short)5 divisors, float and double conversions of the product,
// and an unsigned short local for the division result.
//
// deepseek-v4.1-flash retest in #3010 (this session). The original's integer
// block is exactly "A -> edx, save A -> edi, division -> edx, A * q -> edi,
// * field_1fa, * n", so the source has the field_1fe load *before* the
// division, which in the default compiler state only a code-emitting operand
// produces. New evidence:
//   * statement structure: `int p = field_1fe; p = p * ((b8+5)/5);
//     p = p * field_1fa; p = p * n;` with a *signed* final division
//     ((int)p / (v*300.0f)) compiles to 77.6%: the integer block above,
//     byte for byte, with the original's esi/edi/edx allocation. It differs
//     only in the frame (no sub esp,8, no mov [esp+0xc],0) and in
//     "fild dword" instead of "fild qword". Making the final conversion
//     unsigned (any of: (unsigned int)p / (v*300.0f), an unsigned int u = p
//     copy, a (float)/(double) cast, unsigned __int64, a union, an array
//     element, an inlined helper, an unsigned int chain, references) collapses
//     the statements back to the 52.9% shape in every case (about 40
//     spellings). So the unsigned conversion and the preserved statement order
//     are mutually exclusive in this compiler state.
//   * bitfield left operand: declaring field_1fe as `unsigned int : 16` gives
//     92.0% and the original's exact order and registers (A is evaluated
//     first, accumulator edi); it differs only in the A load:
//     "mov edi,[ecx+0x1fe]; and edi,0xffff" instead of the original's
//     "xor edx,edx; mov dx,[ecx+0x1fe]; mov edi,edx" (one byte longer, 162).
//     `unsigned short : 16` emits no mask and does NOT flip the order (52.9%),
//     so the flip comes from the operand needing an extra instruction, not
//     from the bitfield node. The bitfield mask is unavoidable for that type.
//   * compiler state: 128 header sets, N = 0..600 dummy extern ints, N = 0..300
//     dummy structs and the full UnitType/Unit declarations from 0x402640 all
//     leave the no-cast expression at 77.3% (161 bytes, but the chain
//     reassociated to F*A, q, n and b->type in ecx), and drop the cast version
//     to 74.2%. No state tried flips the no-cast expression to the original's
//     A-first chain.
// Best lead for the next attempt: find a source whose *left* operand needs a
// register during evaluation but emits no instruction (the z16 bitfield flip
// without the mask), or a compiler state that preserves the statement order
// with the unsigned conversion (the s05/y01 block above is the proof that the
// order is reachable; only the conversion differs).
//
// What still differs (96.6%): one extra "and edx, 0xffff" before
// "imul edi, edx", which shifts the jg target (0x4386ef vs 0x4386e9).
//
// space-bunny-free in #4680. File unchanged: still 96.6%, still 167 vs 161
// bytes, still exactly one extra 6-byte instruction. About 500 further scratch
// variants scored with build/scratch/0x438650/h.py (a probe that compiles a
// body and reports the score, the A/b8/type/accumulator registers, whether the
// 0xffff mask is there and whether the conversion is fild dword or fild qword).
//
// New facts, all measured:
//
// 1. THE MASK AND THE OPERAND ORDER ARE ONE DECISION, now confirmed from both
//    sides. Earlier notes recorded only that every mask-free spelling reorders.
//    The reverse is now proved too: every spelling that keeps the original's
//    order (A first, the original's esi/edi/edx/ecx allocation, fild qword,
//    "mov [esp+0xc],0", push edi, and the original's exact 161-byte size) pays
//    the mask. Concretely, `A * (unsigned short)((a->field_b8 + 5) / 5) * F * n`
//    and `A * Q5(a->field_b8) * F * n` with a 16-bit-parameter helper both
//    compile to the original's operand order AND its exact register allocation
//    (A -> edx, b8 -> ecx, a->type -> ecx, accumulator -> edi, fild qword
//    [esp+8]) at 96.6% / 167 bytes, differing by the and alone. Nothing tried
//    separates them. The mask is the widening of a 16-bit-typed quotient; a
//    32-bit-typed quotient is never masked but always reorders.
//
// 2. A 16-bit-PARAMETER inline helper is the cheapest way to buy the original's
//    order: `static inline unsigned short Q5(unsigned short b) { return
//    (unsigned short)((b + 5) / 5); }` called as `Q5(a->field_b8)` gives
//    96.6% with A=edx, b8=ecx, type=ecx, mul=edi and fild qword, from a source
//    that reads as ordinary code. The same helper returning 32-bit is 52.9%
//    (order flipped), so the 16-bit-ness is doing the work, not the helper.
//    It still pays the mask, so it is not in the file, but it is the shortest
//    proof that item 1 is real.
//
// 3. A mask-free shape that gets 161 bytes and the original's accumulator does
//    exist, so the 6 bytes are not simply the price of the whole schedule.
//    `int s = a->field_b8 + 5; int t = 0; if (t) { s = 0; } int p = A * (s/5)
//    * F * (unsigned int)n; r = (int)((double)(unsigned int)p / (v*300.0f));`
//    is 79.5% at exactly 161 bytes, no mask, fild qword, push edi, and the
//    accumulator in edi. It is lower than 96.6% so it is not in the file. Its
//    one fault is the a->type pointer landing in edx instead of ecx and A in ax
//    instead of edx, because the sum statement runs first. Hoisting
//    `UnitType* at = a->type` and using it for BOTH field_1fe and field_1fa
//    moves A into edx and reaches 84.1% at 161 bytes, but then a->type sits in
//    esi and the second +0x92 load moves. Roughly 60 spellings of the sum
//    split, the pointer hoists, the barrier, the conversion and the statement
//    order were swept on these two families (79.5% and 84.1%); none reached
//    96.6%, let alone MATCH.
//
// 4. Field widths re-measured on both families (item 24 of the brief). The file's
//    widths are the original's and are also the best: field_1fe and field_b8
//    unsigned short, field_1fa unsigned int. field_1fe as unsigned int drops
//    the cast family to 69.0% and the split family to 69.8% (Qfirst); as short,
//    92.0% and 86.0%; field_b8 as unsigned int, 48.8% and 75.9%; as short,
//    90.9% and 75.9%; as unsigned char, 94.4% and 75.0%; field_1fa as int,
//    76.7% and 79.5% (fild dword, so the conversion goes signed); as unsigned
//    short, 58.1% and 48.3%. None beats 96.6%.
//
// 5. The barrier's own type and statement form are irrelevant; what matters is
//    only that a statement separates the sum from the product. 30 barrier forms
//    (int, unsigned short, short, char, unsigned char, long, float, double,
//    unsigned int constants, +=, *= 1, /1, %1, <<0, &-1, do/while(0),
//    for(;;), while(0), goto, if/else, two barriers) all compile to the same
//    62.8% shape from the unsplit product and the same 79.5% shape from the
//    sum-split one. The forms that emit no code at all (do/while(0), for,
//    switch(0), goto) are optimised away and collapse the whole function to
//    73 bytes, so the barrier has to be a real `if` or a real store.
//
// 6. Compiler state re-tested against the cast expression: N = 0..120 dummy
//    `extern int` declarations change the score (96.6% up to N=10, 58.1% at
//    15..70, 74.2% at 75, 56.8% at 80..120) but the 0xffff mask is present in
//    every one of them, and no N restores the original's registers. All 256
//    header sets (tools/headers.py) top out at 96.6%.
//
// 7. Two permuter runs on this file found nothing above 96.6%: seed 7 (1615
//    candidates, 13 min) and seed 23 (4206 candidates, 28 did not compile, 50
//    duplicates, 13 min). That agrees with the hand search. tools/headers.py
//    over all 256 header sets also tops out at 96.6%.
//
// Still-open lead, narrower than the one above: the 79.5% family needs the
// a->type load to be emitted BEFORE the field_b8 + 5 group (so that A lands in
// edx and a->type in ecx) while the sum stays in its own statement (which is
// what stops the reassociation). Those two requirements fight in every spelling
// tried: hoisting a->type into a local fixes the order but moves the pointer to
// esi. A source that evaluates the product's left operand first and the sum
// second, without a narrowing cast anywhere, is what is missing.
//
// space-bunny-free, 30-minute checkpoint on the issue-4462 worktree (file
// unchanged, still 96.6%, about 100 scratch variants scored with a probe that
// compiles each one and reports whether the field_1fe load precedes the magic
// multiply, whether the 0xffff mask is there and whether the conversion is
// fild dword or fild qword). What is now settled, beyond the earlier notes:
//
// 1. The swap is caused by the UNSIGNED numerator conversion, not by the chain
//    type, the statement form, a helper or the position of the cast. A signed
//    chain with a signed conversion gives the original's order (77.6%, the
//    integer block byte for byte); the same chain with only `(unsigned)` added
//    around the whole product gives the flipped order (52.9%). Every unsigned
//    spelling flips it: the single expression, `p = p*q; p = p*f; p = p*n;`,
//    `p *= q; ...`, the quotient or the whole product or the division inside a
//    `static inline` helper, an `__int64` local holding the product (the trick
//    the MATCHed 0x48b3f0 uses for its own fild qword), a `double`/`float`
//    local, `(unsigned)` on each factor in turn, `unsigned` parameters for n
//    and field_1fa, and every no-op cast ((int), (unsigned), (long),
//    (unsigned int), (short) on the divisor). So the mask-free spelling of the
//    unsigned conversion does not exist in this compiler's space, and the
//    16-bit cast in this file is not a redundant leftover: it is the only
//    thing that buys the original's operand order.
// 2. The cast has to be on the QUOTIENT. `(unsigned short)A * Q * F * n`
//    emits no mask at all (the field is already 16 bits) but gives the flipped
//    order. MSVC evaluates the narrowed operand second, so the narrowed
//    operand must be the right one of `field_1fe * quotient`.
// 3. A third shape exists and nobody had recorded it: `(unsigned)(A*Q*F*n)`
//    alone (56.5%) makes MSVC fold the /5 through the multiply and compute
//    `((b8+5) * A) / 5`, with the magic multiply running on the product. That
//    is a real strength reduction MSVC5 only does for unsigned, and it is why
//    the original (whose magic multiply runs on `b8+5` alone) cannot be a
//    plain unsigned chain. But the original's fild qword with a constant 0 in
//    the high word is exactly the unsigned conversion, so the two facts still
//    contradict each other; that contradiction is what is left to solve.
// 4. The unsigned 52.9% shape is always: field_1fe loaded second, into ecx,
//    after the quotient is in edx, and `imul edx, ecx`. The original is
//    always: field_1fe first, into edx, `mov edi,edx`, then `imul edi,edx`.
// 5. Argument mapping confirmed: the second parameter is loaded first and its
//    type pointer is homed in esi (field_18a at +0x18a and field_1fa at
//    +0x1fa); the first parameter is loaded second, at [esp+0x14] after the
//    frame, and supplies field_1fe through its type and field_b8 directly.
//    Swapping the two in the source costs 4 points (92.1% and 75.3%) even
//    though the field offsets are identical, so the source must touch the
//    second parameter first.
//
// Best lead left: the original's order is the signed one and its conversion is
// the unsigned one, so the missing piece is a source whose numerator is
// unsigned without MSVC5 folding the /5 into the product and without the
// commutative canonicalisation putting the quotient first, most likely a
// compiler-state difference in the original file (what the permuter hunts) or
// a spelled-out helper with a 16-bit *parameter* whose mask the caller never
// sees.
// A 12-minute permute.py run on this file (--jobs 4, about 25 minutes wall
// clock) finished with an empty best.diff: nothing it tried beat 96.6%, which
// agrees with the hand search above, where the only two reachable shapes are
// 96.6% (narrowing cast, mask and all) and 52.9% (mask free, quotient first).
// A second run (seed 11, 9 minutes, 2851 candidates) also found nothing.
//
// space-bunny-free in #4577. What blocks the swap is now pinned down: it is a
// CONVERSION NODE on one of the two operands of the first multiply, and every
// cast that emits no code is folded away before the pass that reorders runs.
//   * Tried and folded (all 52.9%, i.e. the quotient goes first): the comma,
//     the double negation, `+ 0u`, `* 1u`, `<< 0`, `/ 1`, `& 0xffffffffu`,
//     `(unsigned)`, `(int)`, `(long)`, `(unsigned short)`, an enum cast and a
//     class conversion operator on field_1fe, and an inline Identity()
//     wrapper around the whole product.
//   * Casts that survive block it: `(unsigned short)` on the quotient (this
//     file, 96.6%, paying the 6-byte `and edx, 0xffff`) or a 32-bit
//     `unsigned int field_1fe : 16` bitfield (92.0%: A goes straight into
//     edi with `and edi, 0xffff`). The same cast on the parenthesised `Q * F`
//     does NOT block it (49.4%), so the barrier has to sit directly on an
//     operand of the multiply that gets swapped.
//   * MSVC 5 does know one range fact: `(unsigned short)(x / 5)` with x a
//     16-bit value emits no mask, because a 16-bit value divided by 5 still
//     fits in 16 bits. So `(unsigned short)((unsigned short)(b8 + 5) / 5)`
//     only moves the mask onto the sum (92.1%), and that mask costs the
//     `xor ecx, ecx` as well: MSVC does the add in 16 bits (`add cx, 5`) and
//     has to re-extend afterwards.
//   * A THIRD shape, and the only mask-free way to get `fild qword` with the
//     original's operand order: an intervening statement blocks the type
//     propagation from the conversion back into the chain. `int p = A * Q *
//     F * n; int t = 0; if (t) { p = 0; } int r = (int)((double)(unsigned
//     int)p / (v * 300.0f));` gives the field_1fe load first, `fild qword`
//     and no mask at all (62.8%). The dead store emits no code, and a dead
//     store to an unrelated variable between the two statements works too.
//     What it does not give is the original's register allocation: MSVC
//     hoists `mov ecx, edx` to just after the field_1fe load and accumulates
//     in ecx, while the original keeps A in edx, puts b8 in ecx, accumulates
//     in edi (and that is what fixes `push edi`, `imul edi, [esp + 0x1c]` and
//     the fild operand's offsets). Nothing tried moved that: the statement
//     forms, groupings, factor orders, the blocker on p or on another
//     variable, two blockers, the float statement's placement, int/unsigned/
//     unsigned short locals, the 32-bit bitfield, N = 0..6 uncalled static
//     inline helpers and all 128 header sets (headers.py) all stay at 62.8%.
//     Two more facts about that shape, both worth knowing: the dead store also
//     works when it hits an unrelated variable between the two statements, and
//     once it is there the operand order is field_1fe first no matter which
//     factor is written first (`Q * A * F * n`, `A * F * Q * n` and
//     `F * A * Q * n` all compile to the same 62.8% code), so in that shape
//     the order is not decided by the expression tree at all. A 10-minute
//     permute.py run seeded 21 on it tried 4685 candidates and found nothing.
//     So the free barrier and the original's schedule are two separate
//     problems, and this pass found no way to buy one with the other.
//   * Re-confirmed and now measured one by one: with field_1fa unsigned
//     instead of the cast, MSVC also folds the /5 through the multiply
//     (`((b8 + 5) * A) / 5`, 56.5%), while `(unsigned)` around the product
//     alone only reorders (52.9%); all 24 factor orders, `p = p * q`
//     accumulations, `(unsigned)` on each factor in turn, `__int64` and
//     `unsigned __int64` casts, a `__int64` local, double/float locals,
//     helpers whose parameter is `unsigned`, unions, arrays and references
//     all reorder. A signed chain still reproduces the original's integer
//     block byte for byte (77.6%) with `fild dword`, so inside one expression
//     the original's order and its `fild qword` really are mutually
//     exclusive; the only way out is the free barrier above. The harness used
//     to search all this (generate a variant, compile it, report the score and
//     whether the field_1fe load, the 0xffff mask and fild dword/qword are
//     there) is build/scratch/0x438650/h.py with gen2.py beside it, about
//     0.7 s per variant.

#pragma pack(push, 1)
struct UnitType_00438650 {
    char unknown_0[0x18a];
    float field_18a;                 // +0x18a
    char unknown_18e[0x1fa - 0x18e];
    unsigned int field_1fa;          // +0x1fa
    unsigned short field_1fe;        // +0x1fe
};

struct Unit_00438650 {
    char unknown_0[0x92];
    UnitType_00438650* type;         // +0x92
    char unknown_96[0xb8 - 0x96];
    unsigned short field_b8;         // +0xb8
};
#pragma pack(pop)

// The old partial (96.6%, one extra "and edx, 0xffff") came from narrowing the
// division result to unsigned short: MSVC re-sorts the multiplication chain
// when the product is converted as unsigned, and only a code-emitting 16-bit
// narrowing kept the original order. The union above avoids both.
// FUNCTION: 0x438650
int __stdcall FUN_00438650(Unit_00438650* a, Unit_00438650* b, int n)
{
    UnitType_00438650* bt = b->type;
    float v = bt->field_18a > 10.0f ? bt->field_18a : 10.0f;
    union {
        __int64 q;
        struct {
            int lo;
            int hi;
        } w;
    } p;
    p.w.lo = a->type->field_1fe * ((a->field_b8 + 5) / 5) * (int)bt->field_1fa * n;
    p.w.hi = 0;
    int r = (int)((double)p.q / (v * 300.0f));
    if (r <= 1) {
        r = 1;
    }
    return r;
}