// Decompiled by Space Bunny Free, finished by Claude Sonnet 5.5 and deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, re-tried by space-bunny-free. Names are provisional.
// space-bunny-free pass (#4639): MATCH, 322 of 322 bytes. The residual was the x
// pair's register operand, and it is reachable from the source after all. What
// settled it was the copy-index probe (build/scratch/0x47d970/probe4.py, one TU
// holding copies of the whole body, one spelling per group, read off the /Fa
// listing): the operand that reaches the register is decided per add node by how
// the two operands are ACCESSED, not by the source order of the `+` and not by
// the width or provenance of the Point copy. Measured at k=6 over the cross
// product of six access spellings (direct member, Point* member, short* index,
// both reversed) on each axis:
//   both sums direct member access (what this file had)      x high, y flips
//   both sums through Point* or short* access                x low, y low, 6/6
//   x through short*, y through Point*                       x low, y high, 6/6
// So each sum needs its own access path, and the combination that lands on the
// original's split pair (x low, y high) is x read through `short*` and y read
// through `Point*`. That is the four lines marked below. Everything earlier in
// this file about the second read of pos.x and about declaration state was a
// property of the search space those passes used, not a rule of the compiler.
// Earlier notes follow, kept as the record of what was ruled out.
//
// space-bunny-free pass (bisection): the answer the previous pass set out to
// find. Body unchanged, still 98.0% and 322 of 322 bytes, still the same two
// instructions; what is new is the smallest piece of this function that decides
// the fold, and it is one line.
//
// THE BISECTION (build/scratch/0x47d970/probe.py, one TU holding K copies of a
// body, parsed with tools/coff.py; "low" means the x add loads 0x76 into the
// register, which is what the original does, "high" means 0x7e):
//
//   body                                                        low / high
//   tiny      short t = pos.x + size.x; out = t;                 24 / 24  (k=48)
//   xonly     same, one store                                     24 / 24
//   sumsbound both sums + `if (xend >= w || yend >= h) return 0` 100 / 100 (k=200)
//             low at EVERY ODD copy index, exactly
//   m5/m6     sumsbound + width/height in locals                  24 / 24
//   m1        sumsbound + `Point p = obj->pos;`, p NOT in the test 0 / 48
//   sumsboundcopy  m1 + p.x/p.y in the test                        0 / 48
//   full      the function as it stands                            0 / 200
//
// So the one piece that removes the low form is `Point p = obj->pos;`, the
// four-byte copy, i.e. the dword read `mov edi, dword ptr [ecx + 0x76]`. Not
// the bounds test, not width or height in locals, not the loop, not the mask,
// not the cell or field_a8 tests, not `bit`, and not N unused locals of type
// int, Point or char* at the top of the body (N = 1..6 of each: 0 low, i.e. no
// effect at all, measured on 48 copies). Add that copy to a body whose fold is
// otherwise reachable and the fold is gone from every one of 200 copies.
//
// Then a second bisection took that apart further, and it is the real finding.
// Control: one sum, one store (e1/d1), 24 low of 48. Add ONE more read of the
// same field and the fold dies, and it is not the width, not the store, and not
// the copy: build/scratch/0x47d970/second.py, all at k = 48.
//
//   e1  sum only                                             24 low / 24 high
//   e3  + one extra read of pos.y                           24 low / 24 high
//   e5  + one extra read of size.y                          24 low / 24 high
//   e6  + one extra read of field_a8                        24 low / 24 high
//   e2  + one extra read of pos.x   (the summed field!)      0, and x=None
//   e4  + `Point p = obj->pos;`, store p.y only              0 low / 48 high
//   e8  copy first, sum second                               0 low / 48 high
//
// So the rule is: VC5 puts the LOWER displacement in the register for the x add
// only while 0x76 is read exactly once, by the add. Any second reference to
// that same 2-byte location, of any width and in any statement position, kills
// it, and reading the neighbouring fields does not. Reading pos.x twice changes
// the add's shape instead (reg+reg, hence x=None); reading it once more at
// 4-byte width leaves the add mem+mem and settles it on 0x7e.
//
// That is why the original and this file differ, and it is not a state effect
// at all. The original reads 0x76 three times: once as `mov ax, word ptr [ecx +
// 0x76]` in the add, once as `mov edi, dword ptr [ecx + 0x76]` for the Point
// copy, and once more as `movsx eax, di` off edi for the loop. Yet it chose
// 0x76. Our source also reads it three times and chooses 0x7e, so the count is
// not the whole rule either; what the probe establishes is the DIRECTION: every
// spelling tried here pushes the choice the same way (toward 0x7e), and the
// original is on the other side of that push. So the remaining search is not for
// a different declaration state, a different operand order or a different copy
// (all three are measured dead above) but for the one source shape in which the
// second read of pos.x does not drag the add's register operand to 0x7e. The
// cheapest place to look is the width and the provenance of that second read:
// the `mov edi, dword ptr [ecx + 0x76]` in the original is a copy of a Point,
// and a Point-typed copy is exactly what e4 and e8 above measure as the blocker.
//
// The detector is validated against the exe: validate.py runs probe.xform() on
// the original's own 322 bytes and prints ('low', 'high'), the shape the
// original has, so the labelling above is not a detector artefact.
//
// Third round, same conclusion from the other side. If the copy is the blocker
// and the original has the copy, then either the original's source does not
// make that read the way our source does, or the original was built with the
// body in a shape that reaches the same machine code by another route. Every
// alternative route tried this pass lands on 0x7e too (all 48 copies, all
// 0 low): width.py (w0..w4: the loop bounds off p, off two int locals, off
// obj->pos directly, copy unused for the bounds), offset2.py (o1..o6: the
// copy hoisted above the sums, sums through one local each, sums declared
// uninitialised and assigned, copy of size rather than of pos). Not one body
// with a second read of pos.x produced x=low, in about 400 compiled bodies
// across the three rounds. So do not spend another pass on declaration state,
// TU position, operand order, signedness or the copy's spelling: all measured
// dead. What is left is the second read's WIDTH and PROVENANCE in a form the
// front end orders differently, and the machine code says the read is there
// (`mov edi, dword ptr [ecx + 0x76]`), so the search has to keep the add
// mem+mem and change only how that dword read is spelled.
//
// The probe is cheap and reusable: `uv run build/scratch/0x47d970/probe.py
// k=48 <body> [<body> ...]` prints one line per body, split by x and y, and
// flags the x=low/y=high combination the original uses. Bodies are the keys of
// BODIES in that file (plus bodies.py, combo.py, bisect_copy.py, second.py,
// dword.py, width.py, offset2.py); add one to try a shape, 48 copies cost one
// compile. The searches run this pass, all in the same worktree: probe.py for
// the ladder above, interleave.py (the full body at 48 different TU offsets
// against 8 filler kinds: x never leaves high, so TU position is dead once the
// copy is there), offset.py (0..24 fillers in front: moves y, never x),
// types.py (pos and size the same type, distinct types, size as two flat
// shorts: all 0 low), signed.py (unsigned pos, unsigned size, both unsigned:
// all 0 low), combo.py (3 copy placements x 5 copy spellings x 5 sum spellings
// x 5 extras = 150 bodies, all 0 low), agg1..agg5 in bodies.py (the two ends in
// a Point, in an array, the sums after width and bit, int aliases for the loop
// bounds, short aliases of size: all 0 low or the add changes shape).
// space-bunny-free pass (#3169): same 98.0, 322 of 322 bytes, and the same two
// instruction residual. Body unchanged. What is new here is the instrument, not
// the answer: the earlier passes concluded the x operand is unreachable from the
// source, this pass shows the FORM is reachable in this compiler and then shows
// it is not reachable from THIS function.
//
// 1. The two sums are not symmetric to the code generator, and that is the only
//    asymmetry between them. `mov dx,[ecx+0x80]` is seven bytes
//    (66 8b 91 80 00 00 00) because 0x80 does not fit a signed disp8, while the
//    x pair (0x76, 0x7e) and y's other operand (0x78) are disp8 and four bytes.
//    Both orders of the y pair cost 11 bytes, so its choice is a coin flip; the x
//    pair is 8 bytes either way, so its choice looks decided by something else.
// 2. The lower-displacement-into-register form IS produced by this compiler for
//    a gap-8 pair. A one-file TU of 48 tiny functions, each `void fN(S* g) { short
//    t = g->a + g->b; outN = t; }` with the two shorts at 0x76 and 0x7e, gives
//    copies 8, 20, 32 and 44 as `mov cx,[0x76]; add cx,[0x7e]` and the rest as the
//    opposite, so the choice flips with the function's position in the TU. The
//    same TU with 60 copies of THIS function never puts 0x76 in ax (y flips in
//    blocks: copies 3-8, 15-20, 26-31, 38-43, 50-55), so what decides the x node
//    here is a property of this function, not of the declaration state alone.
// 3. Still no flip, all at 322 bytes: 400 random declaration states (22 kinds of
//    unused declaration, random counts 1..40, five insertion points, checked with
//    a four-at-a-time wcl harness that reads the .obj directly); ten single-kind
//    sweeps to N = 120; 25 Point field-name pairs; pos and size as two distinct
//    Point types in both declaration orders; the Point nested at depth 2 and 3
//    for pos, for size and for both; four flat short members instead of two
//    Points, with the Point copy through a cast; 20 spellings of the x sum and 9
//    of the y sum (explicit and unsigned casts, `+ 0` either side, a deref, an
//    Identity and an add2 helper, `+=`, a local temp, a Point copy and a Point
//    reference for size, a ternary, a multiply by 1); scoping variants (register,
//    a nested block, a comma declaration, static, goto). Every y-flip keeps the
//    total at 322 bytes and scores 305, never better.
// 4. Two leads for whoever comes next. First, the y coin flip and the x coin flip
//    are two different states of the same generator (the y one is reachable by
//    changing the declaration state, the x one is not), so the useful lever is
//    probably something that changes this function's expression-tree shape, not
//    the TU. Second, there is no near-copy to read the answer off: a scan of all
//    3843 functions in data/functions.csv for `66 03 41` (add ax, word [ecx+d])
//    preceded by a lower-displacement `66 8b 41` load finds exactly one function,
//    this one.
// 5. The permuter agrees. tools/permute.py is missing from this worktree, so it was run
//    from a copy in build/scratch/0x47d970/pk (with orig, data, src, include and
//    toolchain symlinked to this worktree, so it scores against this file): seeds 11
//    and 12, 7 minutes each, 1106 and 1102 candidates, and seed 13 for 6 minutes,
//    1293 candidates: all 98.0%, no improvement, no match.
// 30-min checkpoint (space-bunny-free, #3169): best 98.0%, 322 of 322 bytes, unchanged
// from the start of this pass, so the file's body is still the best version found. What
// still differs is the first pair of 16-bit adds, and only that: the original puts
// 0x76 in the register for the x sum and 0x80 for the y sum, this source puts 0x7e and
// 0x80. About 2500 variants were compiled this pass (all with the same flags, scored by
// reading the .obj directly, four at a time); none moved the x operand. New facts, in
// order of usefulness to the next attempt:
// 1. The x operand CAN take the low form in this compiler, so this is not a rule that
//    bans it. The evidence is build/scratch/0x47d970/mem.cpp: one TU, 48 tiny functions
//    each `void fN(S* g) { short t = g->a + g->b; outN = t; }` with the two shorts 8
//    bytes apart, where copies 8, 20, 32 and 44 compile to `mov cx,[0x76]; add cx,[0x7e]`
//    and the rest to the opposite. Same 4-byte disp8 pair, same expression, both
//    outcomes. So the deciding factor is compiler state, and it is reachable, but it is
//    not reachable from this function: 60 copies of THIS function in one TU never give
//    x = 0x76, and neither do the probe-shaped preambles tried before it (see below).
// 2. The one asymmetry between the two sums is the encoding: 0x80 does not fit a signed
//    disp8, so `mov dx,[ecx+0x80]` is seven bytes while `mov ax,[ecx+0x76]` is four.
//    Both orders of the y pair cost 11 bytes, both orders of the x pair cost 8, and only
//    the y choice is state-sensitive. Anything that makes the two x operands cost
//    different amounts may be what puts the decision on the other side.
// 3. No near-copy exists to copy the spelling from: scanning all 3843 rows of
//    data/functions.csv for `66 03 41` preceded by a lower-displacement `66 8b 41`
//    finds only this function, so the answer is not sitting in a matched sibling.
// deepseek-v4.1 pass (#2438): swept the inert-declaration state that fixed the
// sibling 0x47d820 (16 to 80 unused `extern int` lines in front of the first
// `#pragma pack`). For this function that lever reaches only the y sum:
//   N = 0..58 and 200..300     98.0, x loads [ecx+0x7e], y loads [ecx+0x80]
//   N = 59..180, 340..420, 600, 900   96.0, only y flips to
//                              `mov dx,[ecx+0x78] / add dx,[ecx+0x80]`
// The x sum is `mov ax,[ecx+0x7e] / add ax,[ecx+0x76]` in every one of those
// 51 states, and no state reaches MATCH. Also tried and unchanged at 98.0
// (all free-scored scratch, none committed): two distinct field types for pos
// and size (same layout, different leaf symbols), a wrapper struct holding
// both, and dead statements before the sums (a read of field_a8, a Point copy
// of size, an int local). Source operand order and `+=` were re-confirmed
// dead. So the declaration state moves the y add but nothing reaches the x
// add, and the body below stays the best version.
// space-bunny-free pass (#1856): settled the residual with a new instrument
// instead of more spellings. A scratch TU holding a dozen copies of this body
// (one per spelling, all in one object) compiles in a single pass, and parsing
// the .obj with tools/coff.py shows what each spelling really produces: the
// register operand of a mem+mem 16-bit add is chosen by front-end state, not by
// the source. In that one TU the first three copies put the LOWER displacement in
// the register for BOTH sums, the next six the HIGHER one for both, and no
// spelling (member access, short* index, `+=`, an explicit cast, the Point copy
// first or between the sums) ever split the two sums. The original splits them:
// x takes the lower, y the higher. The choice is per add node, so a mixed pair is
// not reachable from the source. Free-scored (check.py --sym) as well: each sum
// reversed, both reversed, and the copy moved to both positions, all 98.0% with
// the same two-instruction residual. Body unchanged from the previous passes.
// deepseek-v4.1-flash second pass (#1609): attacked only the x sum's register
// operand with levers not tried before. All scored as scratch (no committed body
// change): the seven operand orders of the two sums; a `Point&`/`Point*` bound to
// pos and to size; `short&` and `short*` bound to the individual fields; short
// and int locals holding pos.x; a static inline helper returning pos.x; static
// inline add helpers taking the object; and Point member accessors (SumX/SumY)
// called with the receiver swapped so the x sum is `pos.SumX(size)` and the y sum
// is `size.SumY(pos)`. Every one is 98.0 with the same two-instruction residual,
// except the `Point&`-to-size forms which compile x to the higher offset and flip
// y to the lower offset (96.0). So neither operand order nor any access path in
// the source can move this x operand: MSVC canonicalises the mem+mem add to the
// higher displacement regardless. This confirms the earlier conclusion, the
// residual is front-end state and the body below stays the best version.
//
// deepseek-v4.1-flash pass (#1182): re-ran the free-scored sweep (operands
// swapped, augment form, local/pointer/reference aliases of obj, statement
// order, comma declaration), the static-helper form, and tools/headers.py
// (128 sets, best <windows.h> 98.0). Every variant is 98.0 with the same
// two-instruction residual and the total size exactly right, so the file
// below stays the best version; the choice is not reachable from the source.
//
// space-bunny-free pass (#1112): re-derived every stack slot, confirmed the body is
// the right shape and that the one remaining difference is not reachable from the
// source. Details below; the body itself is unchanged from the previous passes.
//
// HISTORICAL, superseded by the MATCH note at the top: this pass stopped at
// PARTIAL: 98.0%, 322 of 322 bytes, and the total size is exactly right, so this is
// a two-instruction residual and nothing else: the first pair of 16-bit adds.
//
//   original   mov ax,[ecx+0x76]   add ax,[ecx+0x7e]    x end = pos.x then size.x
//              mov dx,[ecx+0x80]   add dx,[ecx+0x78]    y end = size.y then pos.y
//   ours       mov ax,[ecx+0x7e]   add ax,[ecx+0x76]    x end: operands swapped
//              mov dx,[ecx+0x80]   add dx,[ecx+0x78]    y end: correct
//
// The original's two adds pick OPPOSITE operands: the lower offset goes into the
// register for x and the higher one for y. No consistent canonicalizer produces
// that, and this build will not produce it either. Measured this pass, all
// free-scored with score.py, all at exactly 322 bytes:
// 1. Source order is not the lever at all. `pos.x+size.x` and `size.x+pos.x`
//    compile to the same code (verified by diffing the objects), and so do the
//    four combinations of the two sums: reversing x only, y only, both, and
//    `size.y+pos.y` for y (the spelling the original's operand order implies).
//    MSVC picks the operand with the higher displacement for the register and
//    ignores which side of the `+` it was written on. So the source above is the
//    most plausible original, and no spelling of it changes the output.
// 2. Other shapes, all 98.0 with the same residual: one declaration with a comma
//    (`short xend = ..., yend = ...`), `xend` and `yend` declared uninitialised
//    and then assigned, `xend = pos.x; xend += size.x` (and the same for y, and
//    both), the `Point p = obj->pos` copy moved between the two sums, and the two
//    sums swapped (that one is worse, 97.0: it reorders the four instructions).
// 3. What does move it is state, not shape. Deleting `#include <windows.h>` flips
//    BOTH sums to the lower-displacement-first form AND introduces a second
//    residual, `mov bl,[edi+ecx]` where the original has `mov bl,[ecx+edi]`,
//    which is the same base/index SIB swap that 0x47d820 could not shake. So the
//    include is load bearing twice over and stays; the earlier note understated
//    this by crediting it only with the y sum.
// 4. The declaration-state probe that reached a padding MATCH on 0x47d820 does
//    not work here. N unused declarations after the include, N = 4, 12, 20, 32
//    for each of `extern void __cdecl f(void);`, `extern int __cdecl f(int,int);`,
//    `static int v;`, `typedef int t;` and `extern int v;`: 98.0 everywhere except
//    two 96.0s, never MATCH, all at 322 bytes. Combined with the earlier
//    `extern int dummyK` sweep (K = 0 to 200 step 4) and headers.py's 128 sets
//    (best 98.0), there is no padding, header or spelling that reaches MATCH.
//
// Conclusion, same class as the residual on 0x47d820: front-end symbol-hash
// state, and the file below is the correct source. Do not spend a pass on the
// operand order of these two adds. (No padding MATCH was found here to decline.)
//
// Claude Sonnet 5.5 pass (#571) notes, kept because they explain the body: the
// fix that took it from 57.6 to 98.0 percent is to write the mask read with the
// post-increment inside it, `mask[n++] & bit`, instead of `n++` at the bottom of
// the loop. The original increments right after the load (`mov ecx,[n]; mov
// bl,[ecx+edi]; inc ecx; test bl,bl; mov [n],ecx`), and with the increment merged
// MSVC keeps `n` in the dead `flag` argument slot and gives esi to the cell
// pointer, as the original does; with `n++` at the bottom it puts `n` in esi and
// spills `bit`, which cascades through the whole loop. Explicit `(short)` casts
// and `int` locals for the two ends (56.3 percent, worse) were also tried.
//
// Stack map, re-derived this pass from the frame (sub esp,0x14, then four
// pushes, so the saved registers are at esp+0..0xc and the five local dwords at
// esp+0x10..0x1c, with arg1 at esp+0x28 and arg2 at esp+0x2c): S+0 xend, S+4 the
// outer row counter, S+8 first the 4-byte `Point p` copy and then the inner column
// counter, S+0xc first p.y and then the byte stride 13*width, S+0x10 yend. p.x
// stays in edi from the copy's dword load and p.y is re-read from the stack
// half of that copy, which is why `Point p = obj->pos;` must stay a 4-byte copy
// and not two separate `short` locals.
//
// Caller, for whoever names this: the only caller is the thunk 0x47dac0, which
// forwards its own two arguments unchanged (`push arg2; push arg1; call`, so the
// last push is the callee's first parameter) and, when the scan succeeds, clears
// bit 2 of the byte at obj+0x10f, sets it from `flag & 1`, sets
// obj->[0x110] |= 0x8000000, calls 0x47c790(obj) and then 0x440a40 with the
// object's point at +0x76 and its point at +0x7e pushed by value. So arg1 is the
// object being placed and arg2 is a flag, as declared here.
// deepseek-v4.1-flash (#2978): re-tested source operand order inside the
// sibling 0x47d820's MATCH declaration state (N = 16..56 unused `extern int`
// lines before the first `#pragma pack`, the range that makes 0x47d820
// byte-identical). xswap, bothswap and yonly all still compile the x sum
// high-first (`mov ax,[ecx+0x7e] / add ax,[ecx+0x76]`) and y high-first at
// N = 16..56, giving 98.0%, and only y flips to low-first at N = 64..80
// (96.0). So the x add is not source-order sensitive in the true declaration
// state either, confirming this is a front-end tie and not the source.
#include <windows.h>
#pragma pack(push, 1)

struct Point_0047d970 {
    short x;
    short y;
};

struct Cell_0047d970 {
    short field_0;                      // +0x0, id of the unit owning the cell
    char unknown_2[0xd - 0x2];
};

struct Unit_0047d970 {
    char unknown_0[0x14e];
    unsigned char* mask;                // +0x14e, one byte per footprint cell
};

struct Obj_0047d970 {
    char unknown_0[0x76];
    Point_0047d970 pos;                 // +0x76
    char unknown_7a[4];
    Point_0047d970 size;                // +0x7e
    char unknown_82[0x92 - 0x82];
    Unit_0047d970* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    short field_a8;                     // +0xa8, the owner's own id
};

struct Game {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_0047d970* cells;               // +0x14287
};
#pragma pack(pop)

extern Game* g_game;

// Scans the rectangle the object covers, and fails if any cell the object's
// mask marks with `flag ? 2 : 4` belongs to a unit other than this one.
// FUNCTION: 0x47d970
int __stdcall FUN_0047d970(Obj_0047d970* obj, int flag)
{
    short* pp = &obj->pos.x;                  // x end reads pos.x through the alias
    Point_0047d970* q = &obj->pos;            // y end reads pos.y through this one
    Point_0047d970* r = &obj->size;
    short xend = pp[0] + obj->size.x;
    short yend = q->y + r->y;
    Point_0047d970 p = obj->pos;
    if (p.x < 1 || p.y < 1 || xend >= g_game->width || yend >= g_game->height)
        return 0;
    int width = g_game->width;
    unsigned char bit = flag ? 2 : 4;
    int n = 0;
    for (int y = p.y; y < yend; y++) {
        Cell_0047d970* c = g_game->cells + y * width;
        for (int x = p.x; x < xend; x++) {
            if ((obj->unit->mask[n++] & bit) && c[x].field_0 != 0 && c[x].field_0 != obj->field_a8)
                return 0;
        }
    }
    return 1;
}
