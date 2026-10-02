// Decompiled by deepseek-v4.1-flash, finished by Space Bunny Free, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free. Names are provisional.
// SPACE-BUNNY-FREE, fifth pass (issue 4691): still 96.5%, 200 of 200 bytes, no
// MATCH. About 200 further variants this pass, all byte-identical or worse, and
// two new results that are worth more than the score.
//
// 1. THE COPY IS NEEDED IN *BOTH* ARMS, FOR THE SAME REASON, and that is why
//    one copy serves both here. In the depth arm `mov bl,[esp+0x20]` clobbers
//    the low byte of ebx, so the count must be out of ebx first. In the fill
//    arm the sequence is `mov al,color / lea ecx,[ebx] / mov bl,al / ... /
//    mov bh,bl`, and `mov bl,al` clobbers ebx there too, so the count must be
//    out of ebx before it as well. The fill arm's copy is NOT forced by memset
//    consuming its count register; it is forced by the same colour clobber.
//    So ebx is the count's home in both arms, each arm takes its own copy out
//    of it, and the original has exactly the two copies it should. Ours has
//    one copy, hoisted to the common dominator of the two arms, with the fill
//    arm taking its copy from that instead of from ebx. The slope/colour load
//    order and the `lea`-versus-`mov` spelling ride along with the hoist, which
//    is why the three residual instructions move as one.
//
// 2. `lea reg,[reg]` IS NOT A COPY SPELLING MSVC 5 PRODUCES FOR THIS SHAPE.
//    Both `lea` sites in the exe that copy a register mean something else:
//    0x40a51b (inside the matched 0x40a260) materialises a SECOND NAME for a
//    value purely to compare it against the first, `lea eax,[ecx]` followed by
//    `cmp eax,ecx / je`, which is the self-assignment-guard shape; and
//    0x4d41f3 (zlib `__tr_stored_block`) is a loop countdown whose source
//    register is reloaded inside the loop body. Neither is a plain copy. A
//    108-variant lab (count as int/unsigned/long/unsigned long/short/unsigned
//    short, three loop forms, five memset spellings, the colour through a
//    local, the colour as an int) produced ZERO `lea reg,[reg]`: every copy
//    came out as `mov dst,src`. So `lea` here is not reachable from a source
//    that merely copies the count, and the two symptoms really are one.
//
// 3. The self-conditional pin is DEAD for this function. `n = n ? n : n;` on
//    the count, on p, d, z, off, start, slope, x2, color, surf and span, at
//    the top of the function, at the top of each arm, inside the depth loop
//    body and inside the fill arm, alone and in all six pairs, plus the plain
//    self-assignment `n = n;` at the same four points, plus a pin with a real
//    merge (`if (n > 0) n = n; else n = n;`) at three points, plus a pin
//    combined with a fill-arm temp: 66 variants, every one of them byte
//    identical to this file. The pin creates a phi, but MSVC 5 folds it away
//    before the copy that decides the count's home, so it cannot stop the
//    propagation. The address-taken form (`const int* np = &n;` feeding the
//    memset) is 73.1%, which is the same promotion-to-a-register result the
//    earlier passes measured. Conclusion for the next pass: a pin is not a
//    lever on this compiler, so do not spend time on it again.
//
// 4. Also closed this pass, against the CURRENT head (some of these were only
//    ever measured against the older 84.2% shape): a second count name for
//    the loop only, as int/unsigned/long/short, in the arm and as a `for`
//    counter (54.4 to 57.8%, so the head still cannot take a fifth local); a
//    second count name for the fill arm only (96.5, same residual); both arms
//    with their own name (54.4%); the fill arm's count through a `const int&`
//    (96.5, same residual); the colour through a local in the arm (96.5, and
//    `bl` is still chosen, so a colour local does not stop the clobber);
//    dropping `off` (78.5%), dropping `z` (65.9%), dropping `start` (85.7%),
//    the count declared before the guard (61.1%), the guard on an `unsigned`
//    local (63.7%), the fill arm's count read from the fields (64.4%), an
//    early `return` after the fill arm (75.1%), thirteen memset count
//    spellings times two with and without a pin (all 96.5%), and the count's
//    declared type crossed with seven memset casts, 48 combinations, of which
//    the six signed ones are 96.5 and every unsigned one is 89.4%.
//
// 5. Translation-unit state, the one lever with a measured effect here, is
//    exhausted in this family too. The fourth pass found that the count block
//    only reaches the original's `imul eax,[row]` when the file contains an
//    inline function (1 to 6 uncalled ones give 96.5%, 8 give 88.9%), so the
//    count of inline functions in the TU visibly moves this function's
//    allocation. Swept again against the current head, 348 TU states: 15
//    helper shapes (identity int, identity pointer, `a + 1`, void, char, one
//    with a loop, two ints, one taking a pointer, one taking a function
//    pointer, one calling memset, a static variable, a template, a class with
//    a member, a static const, one taking an array) at 0 to 12 copies each,
//    one of every shape at once, 66 two-shape mixes over the 6-to-12 window,
//    and 18 exotic declarations (double, float, two doubles, __int64,
//    unsigned __int64, a class with virtuals and a destructor, a throw()
//    specifier, a union, a bitfield struct, a static variable, a non-inline
//    static function, an enum, an int& parameter, a nested class, an
//    operator+, an array parameter, a char* parameter) at 1, 2 and 3 copies,
//    plus one placed inside the struct and one on a function-local static, and
//    `register` on the count. Result: 217 of the 274 helper-count variants and
//    35 of the exotic ones at 96.5%, and the rest at 93.4% or lower. The 93.4%
//    band is just the total-inline-count degradation the fourth pass described,
//    re-measured at a different point now that the used `Pitch()` member is
//    itself an inline function (six or more uncalled helpers cost 2.7 points;
//    12 still give 96.5%, 20 gave 81.2% in the older file). Nothing in any TU
//    state tips the count copy out of the dominator.
//
// 6. New tooling, reusable and left in build/scratch/0x4c06e0/. `differ.py`
//    scores a compiled `/Fa` listing against the original instruction stream
//    with the stack slots matched BY NAME (the listing's `_slope$[esp+12]`
//    and the exe's `[esp+0x18]` are the same slot, and the listing prints
//    displacements and shift counts in hex with no suffix, which is the one
//    thing that makes a listing hard to diff by eye). It reproduces this
//    file's residual exactly and reads 96.1% where check.py reads 96.5%, the
//    gap being the duplicate jump labels it does not fold, so a 100% on it is
//    a candidate to confirm with check.py rather than a result. `sweep.py`
//    compiles a whole batch in parallel, ten at a time, and prints the diff
//    for anything at or above the base: about 0.4 s per variant against 0.3 s
//    for a full check.py run, but with ten in flight, which is what makes a
//    350-variant sweep cost a couple of minutes. `lab.py` plus `genlab.py`
//    compile a cut-down function and report only the copies, which is how the
//    zero-`lea` result above was measured. Note the three traps: MSVC's
//    listing displacements are decimal-looking but hexadecimal (`[ecx+24]` is
//    0x18); a function that is never called is dropped from the listing
//    entirely, so a lab function must be extern; and `build/obj/` is
//    case-insensitive under Wine, so the listing directory is lower-cased.
//
// 7. Two permuter runs, both from seeds other than the best file, per the
//    lesson from 0x47e5c0 that a worse shape can reach a byte-identical match
//    where the best shape does not. From the arm-extracted seed (the fill arm
//    as an inline `FillRow`, 96.5% to start): 5905 candidates in 11 minutes,
//    no improvement. From the separate-loop-counter seed (`int i = n` in the
//    depth arm, 47.3% to start, 196 bytes), the worst shape anyone has kept
//    for this function: 11645 candidates in 15 minutes, climbing to 83.0% in
//    the first 70 seconds and then flat, so that basin does not contain the
//    96.5% file either and the two shapes are not connected by any of the
//    rewrites the permuter tries. This file's best shape has now been the
//    permuter's starting point five times over, and the two alternative seeds
//    are saved at build/scratch/0x4c06e0/seed_arm.cpp and w2_loopvar_i.cpp
//    for a future pass that wants to start lower.
//
// 8. One thing checked and found sound, recorded because it looks wrong at
//    first: the exe's first two loads are `mov eax,[esp+0xc]` and
//    `mov ecx,[esp+8]`, which look like the first and second parameters, but
//    for __stdcall MSVC 5 frames parameters in DECLARATION order from 8
//    upwards (a four-parameter test compiles to _p1$=8, _p2$=12, _p3$=16,
//    _p4$=20, and the return address is at [esp]), so [esp+0xc] is the third
//    parameter and [esp+8] the second. That is exactly what this file
//    declares, and the roles check out: the third parameter is dereferenced at
//    +0x10 and +0x14 for bits and depth, the second at +0, +4, +0x18 and
//    +0x1c for x1, x2, z1 and z2, and after the four pushes `imul
//    eax,[esp+0x14]`, `mov eax,[esp+0x18]` and `mov bl,[esp+0x20]` are the
//    first parameter, the slope's home and the fourth parameter. No bug in the
//    original here.
//
// The bottom line is unchanged from the fourth pass: 96.5% is a local optimum
// of this source family. What this pass adds is the reason the fill arm shares
// the depth arm's copy (the colour clobbers ebx in both arms), which closes
// the "memset consumes the count register" explanation, the measurement that
// MSVC 5 will not spell a count copy as `lea` here, which removes the other
// half of the residual as a source-shape question, and the closure of the
// translation-unit lever over 348 states.
//
// SPACE-BUNNY-FREE, fourth pass (issue 4582): best stays 96.5% (200 of 200
// bytes, size exact) after about 306 scoring runs and a 15-minute permuter run
// (9887 candidates, no match). The result of this pass is a mechanism, not a
// score: the three residual instructions are ONE effect, not three.
//   * A minimal cut-down lab (only the depth loop uses the count) shows MSVC 5
//     emits NO COPY AT ALL - the count's own register becomes the loop counter -
//     and puts the slope load before the colour load, which is the original's
//     order. The copy appears only when the fill arm also wants the count, and
//     then it is hoisted to the common dominator, which also drags the colour
//     load in front of the slope load. So copy placement, `lea` versus `mov`
//     and the slope/colour order are one effect, caused by the fill arm's use
//     being copy-propagated into the loop's induction copy. Fixing the
//     colour/slope order alone is therefore not possible.
//   * Two escapes killed by experiment: a pointer-typed count gives
//     byte-identical code (so `lea` is not a type effect), and taking the
//     count's address (`int* np = &n; memset(..., *np)`) is also identical, so
//     MSVC promotes it straight back out of the addressable state.
//   * The wall: every way of giving the fill arm its own count temp costs the
//     head. An inlined depth-loop helper taking the count by value gives 33.7%
//     (the head reallocates around the helper's parameters); a pointer-typed
//     counter MSVC cannot propagate gives 47.3%; a top-level `int i` set from
//     `n` in the arm is propagated away and is byte-identical to this file.
//   * Closed this pass, all 96.5% (identical bytes) or worse: 14 invisible-IR
//     variants (6 loop-body/arm/guard self-assignments, 8 dead stores against z,
//     slope, start, p, d, color, n in three positions); 21 TU-state shapes
//     including COMBINATIONS of inline functions and a mini clone of this
//     function (the clone and non-inline shapes give the 88.9% no-extra-inline
//     state, the only TU effect found, and it is backwards); pointer-typed /
//     address-taken / `int&` / recomputed counts; the fill arm as six inline
//     helper shapes; `Bits()`, `Depth()`, `Width()`, `Z1()`/`Z2()` accessors;
//     the count block's locals moved into the arm; the p advance duplicated in
//     both arms; `if (d)`, `if (d == 0)`, `if (!d)`, inverted guards; and a
//     144-variant combination sweep (five declaration orders x two offset
//     spellings x three loop forms x three memset spellings x n as
//     int/unsigned) where 96.5% is the ceiling.
//   * Also recorded: what the only two `lea reg,[reg]` sites in the exe have in
//     common - the destination is a temp an implicit operation modifies (the
//     reloaded-iterator comparison in the matched 0x40a260, and zlib's
//     `tr_stored_block` counter whose source is clobbered inside the loop).
//     Neither is reachable from this source shape, which fits the copy being
//     MSVC's shared count rather than a hand-written copy.
//
// Space Bunny Free pass (issue #4498): 84.2 -> 96.5 percent, 200 of 200 bytes,
// still no MATCH. The count block is now byte-identical, including the two
// instructions nine earlier passes could not get: the surface pointer is
// reloaded into ebp, the 16-bit pitch is read off it twice, the count's x1
// stays in edx, and the offset is `imul eax,[row]; add edx,eax`.
//
// Three things were needed, and only the first is a spelling the source can
// plausibly have:
//
// 1. The count block writes the offset *twice* in two different spellings: a
//    shared local for the depth pointer and the full expression again for the
//    colour pointer, with a local for the start of the span:
//        int start = span->x1;
//        int off = start + row * surf->pitch;
//        p = p + (start + row * surf->pitch);
//        if (d != 0) { d = d + off; ... }
//    `p += off` instead of the repeated expression gives 84.2 percent, and
//    dropping the `start` local (reading span->x1 inline) gives 72.9 percent,
//    so both are load-bearing. What fixes the register colouring is that x1 is
//    a *local* of the count block, read before the count, so the allocator no
//    longer coalesces the count's x1 with the dead head x1 and gives the
//    callee-saved register to the surface pointer instead.
//
// 2. One inline function in the translation unit, of any kind, called or not.
//    With the count block above and no extra function the file scores 88.9
//    percent and 202 bytes: the row product goes to ebp (`mov ebp,[row]; imul
//    ebp,eax; add edx,ebp`) instead of eax. Adding a single `unsigned short
//    Pitch() { return pitch; }` member to Surface_004c06e0 and using it for the
//    two pitch reads is enough to get the original's `imul eax,[esp+0x14]`. A
//    free `static inline unsigned short Pitch(Surface*)` works the same, and so
//    does an inline function that is never called at all, so this is compiler
//    state and not the helper's body: 1 to 6 and 12 uncalled inline functions
//    give 96.5 percent, 8 and 19 give 88.9, 20 gives 81.2, while a `static`
//    function *declaration* or an `extern` variable changes nothing. The member
//    is used twice, so nothing dead is left in the file.
//
// 3. Nothing else: no `int x1 = span->x1;` is needed, and a width local `w`, a
//    `lim` local for the clamp, a `z1` local for the division numerator, `n`
//    declared inside the block or at the top, the arms in either order, the
//    `start + row * surf->pitch` or `surf->pitch * row + start` order, and
//    `p`/`d` declared in either order all still give 96.5 percent.
//
// Still differs, 3.5 percent, three instructions in the loop preheader:
//    original: mov eax,[esp+0x18]     the slope, loaded first
//              lea ebp,[ebx]         the counter copy, inside the arm
//              mov bl,[esp+0x20]     the colour byte, after the copy
//    ours:     mov ebp,ebx           the counter copy, hoisted above add edi,edx
//              mov bl,[esp+0x20]     the colour byte, hoisted above the slope
//    and the fill arm copies the count with `mov ecx,ebp` where the original
//    uses `lea ecx,[ebx]`. MSVC 5 merges the loop's induction copy with the
//    count the fill arm needs, so one copy serves both arms and is hoisted to
//    the common dominator. Every attempt to confine it to the depth arm adds a
//    local, and one extra local reallocates the whole function (the head puts
//    span in esi and surf in ebx, 47 percent), so the lever has to be a
//    spelling that keeps the local count at eight. Tried and worse: a separate
//    `int i = n` counter anywhere (47.3), two locals with the same value so the
//    loop and the fill arm use different ones (57.6 to 61.5), the count
//    recomputed from the fields or from `start` in the fill arm (51.5 and
//    61.5), `while (n-- > 0)` (86.5), `while (n) { ... --n; }` and
//    `for (; n > 0; --n)` (57.3 each), the arms in the other order (74.1), a
//    `goto` between the arms (74.1), the colour byte or the slope copied into a
//    local in the arm (unchanged, folded away), `register` on any of the eight
//    locals (unchanged), and `volatile` on the count (76.7). A permuter run
//    from this file (3771 candidates, 5 minutes) found nothing better either,
//    and neither did all 128 header sets of headers.py (flat at 96.5) or
//    padding declarations: N `extern int`, `extern void __cdecl f(void)`,
//    `extern int __cdecl f(int,int)`, `static int` or `typedef int` lines for
//    N = 0 to 24 all stay at 96.5 percent or drop to 88.9 (the extern form
//    flips at N = 16, the others between 7 and 20).
//    Also tried against this residual, all 96.5 percent or worse: a dead store
//    inside a statically folded branch (`int t = 0; if (t) n = 0;` or
//    `if (t) off = 0;` or `if (t) p = 0;` or a dead store through a dead
//    pointer) in front of the arms, inside the depth arm, in the fill arm and
//    after the clamp; a ternary that folds away (`0 ? a : a` on the start local,
//    `a > b ? a - b : a - b` on the count, and a guard wrapped in a folded
//    conditional); and reading the same field through a second `Span*` or
//    `Surface*` local, which unlike an extra `int` local does not break the
//    head (96.5 percent either way). The last three instructions look
//    unreachable from the source: they are one copy that MSVC 5 merges with the
//    count the fill arm needs, and nothing in the source can stop that merge
//    without adding a local, which reallocates the whole function.
//
// Space Bunny Free pass (issue #4552): still 96.5 percent, 200 of 200 bytes, no
// MATCH. About 1400 further variants plus a third permuter run (5681
// candidates), all flat at 96.5 or worse. They are worth recording because they
// close off whole families:
// - Translation-unit state, the lever that got this file to 96.5, is flat. N
//   uncalled inline functions of two shapes (identity and `a + i`) for N = 1 to
//   8: 96.5 up to five, 88.9 from six on. N never makes the count copy appear
//   in the right place. So are 15 kinds of unrelated declaration (extra struct,
//   class with a member function, template, static variable, non-inline
//   function, inline function with a loop / taking a pointer / taking two ints /
//   returning void / returning char, macro, enum, union, typedef) and the
//   pitch accessor as a member, a const member or a free function, used at any
//   mix of its four call sites (the two clamp reads and the two offset reads).
// - The count and the loop: `n` as int / unsigned / unsigned int / long /
//   unsigned long, six orderings of the four locals, the offset and the start
//   local as unsigned, eleven spellings of the memset count (`n`, `(size_t)n`,
//   `(unsigned)n`, `n + 0`, `n * 1`, `n & 0x7fffffff`, `(long)n`, `*(int*)&n`,
//   a folded ternary, the fields again, `x2 - start`), three colour spellings
//   and nine loop forms (do/while with `--n`, `n -= 1`, `n-- != 0`, `n-- > 0`,
//   `while (n) { ... --n; }`, `for (; n > 0; --n)`, `for (int i = n; ...)`, a
//   separate `int i` inside the arm, `while (--n >= 0)`), every combination of
//   those three axes: 96.5 percent is the ceiling, 87.7 the common second.
// - The depth arm's body: the shift as `z >> 16`, `z >> 0x10`, `z / 65536`,
//   `(unsigned)z >> 16`, `(z >> 16) & 0xff`, the test as `*d <= zi`, `zi >= *d`,
//   `!(zi < *d)`, the stores in both orders, the four orderings of `p++; d++;`
//   and `z += slope;`, the colour through a local, `zi` through an int temporary,
//   the test and the stores as one `&&` expression, and `if (d)` for the arm
//   test: all 96.5 or worse (reversing the two stores costs two bytes, 87.6).
// - The head and clip: `span->z1 -= x1 * slope`, `z1 - slope * x1`, `0 > x1`,
//   the clip as `(int)Pitch() - 1 < x2`, the clamp chained through x2, no
//   `x2 = span->x2` reload, `x2` initialised after `slope`, no `x2` local at
//   all, p and d declared in either order, and the five orderings of the four
//   count-block locals: all 96.5 or worse.
// - Moving code across the arms, the one thing not yet in the file: the fill
//   arm as an inline `FillRow(p, n, color)` that calls memset (96.5, and the
//   same three instructions, so it is not a way in), the depth arm's loop as an
//   inline helper taking the three pointers by reference (33.5 to 47.3, the
//   by-pointer-value form 33.7), a top-level `unsigned char c = color` used by
//   the loop (87.1) or by the loop and the memset (96.5, same residual), a
//   top-level `int c4 = *(int*)&color` (96.5, same residual), the colour
//   through `(int)color`, `(int)(char)color` and `*(int*)&color + 0` (92.4,
//   92.9, 96.5), the arm test on `surf->depth` (57.0), `d == 0` first (74.1),
//   and the count's guard hoisted into its own local (96.5, same residual).
//   So even adding a ninth or tenth local leaves the three instructions alone,
//   which says the count copy is not a register-pressure artefact: the head
//   can reallocate around it without moving it.
// Two facts about the residual that a lab file settles, and they say what is
// left to try. First, the original's two copies are `lea` and ours are `mov`,
// and the whole exe has only five pure `lea reg,[reg]` copies (0x403f5c is
// data, not code): two are ours and the other two, 0x40a51b and 0x4d41f3, both
// copy a pointer. Second, a pointer is not the rule: a lab function whose count
// is a pointer-typed variable (`unsigned char* n = (unsigned char*)(x2 - x1)`,
// with a separate pointer induction variable and `memset(p, c, (unsigned)n)`)
// still copies it with `mov esi,ecx`, and so does the pointer-difference form
// with both span fields typed as pointers. So `lea` here is not a matter of the
// count's type, and the pointer-difference reconstruction of the source does not
// pay. Whatever it is, it is set by the same allocator state that moved the copy
// out of the arm, so the two symptoms have one cause and the cause is not any
// spelling of the source tried above.
//
// What the 84.2 percent passes established, kept here because it is what makes
// the 96.5 percent version legible: the matched sibling 0x4c0b10 has the same
// count block shape (surface pointer in ebp, the 16-bit pitch read off it
// twice, x1 in edx, the count in ebx), so the target shape does compile in this
// family. What decides it is register *pressure*, not spelling: in 0x4b10 the
// start value is spilled to a stack home right after the count because that
// loop needs all six registers, and a value with a stack home is given a
// scratch register, which frees ebp for the surface pointer.
//
// Scored and tied at 84.2 with the old count block (identical bytes, about 70
// shapes): a `Surface*` local for the pitch reads assigned after the clip
// (`s = surf`, declared-then-assigned, used in the clamp only, in the offset
// only), a `unsigned short* pp = &surf->pitch` read as `*pp`, a reference
// `Surface& s = *surf`, an inline `Pitch(surf)` getter, inline `Off(surf,row,x)`
// and `Cnt(span)` helpers (alone and together), `p = &p[off]`, `p = p + off`,
// `(int)row *`, `unsigned`/`long` off, `surf->pitch * row` and
// `span->x1 + ...` offset orders, all six orders of the off/z/n declarations,
// `n` computed before the guard or after the clamp, a named width local
// (`int w = x2 - x1` and `int w = span->x2 - span->x1`), a named dz local, a
// `z1` local, a dead local, the clamp without the reload of x2, the clamp
// chained as `x2 = span->x2 = ...`, the guard as `span->x2 > span->x1` (83.0
// percent, 200 bytes), the clip testing the x1 local, the offset split into
// two adds, the offset written out twice, both parameters as references, the
// guard and the count through a second `Span*` local, `int&` references to the
// fields, the pitch read as `*(unsigned short*)surf`, `unsigned short&` to the
// fields, `if (d)` instead of `if (d != 0)`, reversed arms, an early `return`
// after the loop arm, both struct definition orders, `#pragma pack(2)`, and
// every one of the 128 header sets headers.py can try (flat at 84.2). Worse,
// and each for a reason worth knowing: a pointer-returning `static inline`
// identity (MSVC 5 does not inline it, so the call plus its `push ecx` stack
// reservation rewrites the prologue, 223 bytes), a `pitch` local (75.3 and
// 71.9), a separate loop counter `int i = n` inside the depth arm (46.8:
// adding a fifth local reallocates the *head*, span moves to esi and surf to
// ebx), the same counter before the arms (51.7), a `for` counter (48.8),
// `while (n-- > 0)` (66.3), a `start` local in the old count block (77.5 and
// 73.3, 202 to 204 bytes), a second `p`/`d` pointer declared inside the count
// block (36.9 and 48.8), reading `surf->depth` twice (63.5), the count from the
// clamped x2 local (67.4), two locals with the same value (52.9), the memset
// count recomputed from the fields (52.9), and 20 or more uncalled
// `static inline` helpers in the file, which is a real compiler-state
// threshold: 1 to 19 of them are 84.2 percent and 20 or more give 72.9 percent
// and 194 bytes (a head change, not a count-block change).
//
// Draws a full horizontal span: every pixel from x1 to x2 gets the colour
// when it passes the depth test (the depth buffer keeps the integer part of
// the 16.16 depth), or the row is filled unconditionally when the surface has
// no depth buffer. Sibling of 0x4c0a90, which plots only the two end points,
// and of 0x4c0b10, which walks two ramps.
//
// Still differs (74.1%, up from 61.5%). The head, the clip and the loop body
// are now instruction for instruction identical to the original. What is left
// is one 3-cycle of registers in the span-count block:
//
//   original: mov ebp,[esp+0x1c] / mov ax,[ebp]  (surf in ebp, pitch off it)
//             mov edx,[ecx] / mov ebx,[ecx+4] / sub ebx,edx   (n in ebx)
//             xor eax,eax / mov ax,[ebp] / imul eax,[esp+0x14]
//             add edx,eax / add edi,edx
//   ours:     mov ebx,[esp+0x1c] / mov ax,[ebx]
//             mov ebp,[ecx] / xor edx,edx / mov dx,[ebx] / sub ebx,ebp
//             imul edx,[esp+0x14] / lea eax,[edx+ebp] / add edi,eax
//
// The original re-reads the pitch off a `surf` pointer it keeps in ebp, and
// computes the byte offset with `add edx, eax` on the x1 already sitting in
// edx; ours keeps the pitch in a `pitch` int, hoists the second pitch read
// up to the first, and reaches the same sum through a `lea` into eax. So the
// original's `surf` is a live callee-saved local in the count block, and our
// `int pitch = surf->pitch;` is what loses it. Every spelling tried for that
// block (an `int pitch` local, a `Surface*` local, a `start` local, the width
// as `x2 - x1` or `span->x2 - span->x1`, the offset as one statement or
// `row * pitch` then `+= span->x1`, the loop as do/while or for, an unsigned
// char pitch, and extracting the whole draw block into a static inline helper)
// stays at 74.1% or below; 74.1% is reached by many shapes and is a ceiling
// for this source family.
//
// A third pass added four more: the pitch read into a local first with the
// clamp against that local (69.0%), the same as a ternary with the clamp
// folded (69.0%), a `unsigned short` local matching the 16-bit load the
// original performs (70.2%), and an unused surface pointer local (74.1%, the
// same bytes). So the remainder really is just which register the surface
// pointer lands in, with the span's x2 field moving between edx and ebp to
// match it, and 74.1% is a ceiling for this source family.
//
// What did move it up, from 61.5%:
// - `int x1 = span->x1; int x2 = span->x2;` and dividing by `(x2 - x1)`. The
//   two named locals are what move the span pointer into ecx and the depth
//   pointer into esi, which fixes the whole prologue and the loop's pointer
//   registers (63.9%).
// - The pitch clamp written on the `x2` local (`if (x2 > pitch - 1) { ... }`
//   followed by `x2 = span->x2;`) and then an `int pitch = surf->pitch;`
//   after it (74.1%). The clamp on the local is what keeps n in ebx through
//   the count block instead of spilling it.
//
// A fourth pass (#1213, deepseek-v4.1-flash) scored four more dead ends, all
// below 74.1%: a `start` local with the offset reading `surf->pitch` inline
// (70.2%, 199 bytes, surf lands in eax and start in ebx), the same with the
// offset as `row * surf->pitch + start` (70.6%), reusing the top-level `x1`
// local for the count after the clamp (70.6%), and the shared `off` local
// (70.6%). So 74.1% still stands.
//
// The head shape is the same as 0x4c0b10's, which is also stuck on an
// ebx/ebp choice at the same two field loads.

//
// Fifth pass (#2034, deepseek-v4.1): baseline re-confirmed at 74.1%, then about
// 35 more source shapes swept with a script (each scored by check.py), none above
// 74.1%. Two findings that narrow the remaining gap:
// - The loop increment order is source order. The original emits inc edi; inc esi;
//   add ecx,eax (both pointers, then z += slope); the file's `p++; z += slope; d++;`
//   emits inc edi; add ecx,reg; inc esi. Writing `p++; d++; z += slope;` does move
//   the add after the two incs, but the slope is then reloaded per iteration into
//   ebp, which swaps one mismatch for another, so the score stays 74.1%.
// - The slope is spilled to [esp+0x18] in both versions. The original reloads it
//   once before the loop (`mov eax,[esp+0x18]`, then `add ecx,eax` in the body) and
//   uses eax for it; ours reloads it inside the loop (`mov ebp,[esp+0x18]` with the
//   reordered increments, `mov edx,[esp+0x18]` in the file). Explicit pre-loop
//   copies (`char c = color; int s = slope;` used in the body) are coalesced away
//   and change nothing, so the load placement is the allocator's, not the source's.
// Also scored and all at 74.1% or below: a z1 local used by the count block (65.9%),
// the same keeping span->z1 (66.3%), a counter copy / for / while loop (50 to 50.6%),
// a slope copy (74.1%), `int len` for the guard (50.3%), an offset local (64.7%),
// `memset(p, (unsigned char)color, n)` (72.5%), a char local for memset (74.1%),
// the pitch loaded inline instead of hoisted (70.2%), x2/pitch locals as unsigned
// short (68.6%), a field read in the clamp (66.3%), pointer-order and offset-order
// swaps (74.1%), and `span->z1 -= slope * span->x1;` (74.1%). Every variant that
// ties 74.1% emits identical bytes, so the file is already at a fixed point of the
// shapes tried: what is left is one register-coloring decision (surf reloaded into
// ebp with x1 in edx and the pitch in eax, versus surf in ebx here).

// Sixth pass (#2034, deepseek-v4.1): the 0x4c0b10 MATCH recipe does not
// transfer to this function. Applying its head and tail (int w, int start,
// int count, while (count--), unsigned char color, the depth offset reading
// span->x1) scores 44.7 to 52.3% in all combinations, worse than the 74.1%
// baseline, which must keep the x1/x2 locals and the count read from the span.
// Also scored and worse: inline surf->pitch with a separate `int i = n` loop
// counter (46.8%), the same with char color (49.4%), `int n` before the guard
// with the pitch local declared after it (45.0%), the two-step offsets (48.0 to
// 50.6%), and unsigned char color (72.5%, 204 bytes, scored in the fifth pass). The
// 74.1% file is unchanged and is a local optimum: only the register colouring of
// the count block still differs (surf into ebp with x1 in edx there, versus
// surf into ebx with x1 in ebp here).

// Seventh pass (#2426, deepseek-v4.1-flash): baseline 74.1% re-confirmed. Inlining
// `surf->pitch` (dropping the `int pitch` local so the surface pointer would stay
// live into the offset, the shape the original register use implies) gives 70.2% /
// 202 bytes in both addition orders and also through an explicit `Surface* s` local
// used from the clamp onward (70.2%, 202 bytes); using `s` for the p/d init too is
// the same. So the pitch-local shape is required and the residual stays the
// ebx/ebp colouring of `surf` in the count block.

// THIS SESSION (deepseek-v4.1-flash, 10-minute box): 74.1 -> 75.3 percent, 199
// of 200 bytes. Three changes in one file: the color parameter is now
// `unsigned char` (the fill arm loses its movsx), the fill passes
// `*(int*)&color` to memset (the 0x4c0330 type-pun trick, gives the plain
// `mov al` byte load), and the loop increments are `p++; d++; z += slope;`
// which matches the original's `inc edi; inc esi; add ecx, ...` order. Still
// differs: the span-count block allocation (the original keeps surf in ebp and
// re-reads the pitch off it late with x1 in edx accumulating the offset; ours
// keeps surf in ebx and x1 in ebp with the offset materialised twice), and the
// loop counter stays in ebx instead of being copied to ebp so the color byte
// can land in bl. Inline `surf->pitch` offset shapes (v2 to v4, v6, v8) are all
// 71.6 to 72.4 percent at 211 to 215 bytes, worse.

//
// Ninth pass (deepseek-v4.1-flash, 60-minute box): no MATCH, best score moved
// 75.3 -> 76.6 percent (the file now holds that 76.6 percent shape, 215 bytes).
// A 792-shape in-process sweep of the count block (pitch local before/in/after
// the guard, int/unsigned short/unsigned int pitch, four offset spellings,
// two count spellings, three loop forms, two memset counts) found only one
// family that puts surf in ebp (the register the original keeps it in): the
// offset must read the *head* `x1` local rather than `span->x1`, which is both
// semantically wrong after the clip and reallocates the whole head (59.0
// percent, 200 bytes; it emits `imul ebp,ebx`, `sub ebp,[ecx]` there). Every
// shape that keeps the head at 75.3 percent or above puts surf in ebx or eax.
// A byte-pattern scan of the whole exe shows the original's count block
// (xor eax,eax / mov ecx,[ecx+0x18] / mov ax,[ebp] / imul eax,[esp+0x14]) is
// unique, so there is no sibling to copy from.
//
// The 76.6 percent file below is a length artefact, not a structural step: it
// reads `surf->pitch` inline in the depth offset (so surf stays in a register
// and the loop tail matches: `mov bl,[esp+0x20]`, `add ecx,eax`, `dec ebp`),
// but it spills `n` to [esp+0x1c] and computes `row * pitch` twice. The
// 199-byte base, saved at build/scratch/0x4c06e0/BEST_75.3_199bytes_base.cpp,
// differs in exactly one hunk (the count block) and is the better starting
// point: it has the head, clip, offset and memset arm byte-identical, and
// needs only the `mov ebp,[esp+0x1c]` / `mov edx,[ecx]` colouring. Restore it
// first if you prefer the shorter diff.
//
// Where the 76.6 percent version still differs: surf lands in ebx (not ebp),
// x1 in eax (not edx), the pitch in edx and `n` spilled to [esp+0x1c], the
// pitch local is read early so surf dies before the count, and the memset arm
// copies the count with `mov ecx,ebp` where the original uses `lea ecx,[ebx]`.

#include <string.h>
struct Span_004c06e0 { int x1; int x2; char unknown_8[0x18 - 0x8]; int z1; int z2; };
struct Surface_004c06e0 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
    // The pitch accessor is not a claim about the original: one inline function
    // anywhere in the file is what puts the row product in eax (see the notes).
    unsigned short Pitch() { return pitch; }
};

// FUNCTION: 0x4c06e0
void __stdcall FUN_004c06e0(int row, Span_004c06e0* span, Surface_004c06e0* surf, unsigned char color)
{
    unsigned char* p = surf->bits;
    unsigned char* d = surf->depth;
    int x2 = span->x2;
    int slope = (span->z2 - span->z1) / (x2 - span->x1);
    if (span->x1 < 0) {
        span->z1 = span->z1 - span->x1 * slope;
        span->x1 = 0;
    }
    if (x2 > (int)surf->Pitch() - 1) {
        span->x2 = surf->Pitch() - 1;
        x2 = span->x2;
    }
    if (span->x2 - span->x1 > 0) {
        int start = span->x1;
        int off = start + row * surf->pitch;
        int z = span->z1;
        int n = span->x2 - span->x1;
        p = p + (start + row * surf->pitch);
        if (d != 0) {
            d = d + off;
            do {
                unsigned char zi = (unsigned char)(z >> 16);
                if (*d <= zi) { *p = color; *d = zi; }
                p++; d++; z += slope;
            } while (--n);
        } else {
            memset(p, *(int*)&color, n);
        }
    }
}
