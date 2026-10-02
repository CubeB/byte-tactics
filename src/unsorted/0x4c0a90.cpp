// Decompiled by Opus, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash. Names are provisional.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// Status: 96.2% (118 of 118 bytes, ours is already the original's size).
// What is left is exactly two instructions' worth of order, both in the
// prologue: the original loads surf->depth (`mov eax,[ecx+0x14]`) BEFORE
// `push esi` and loads span->x2 (`mov edx,[ebp+4]`) BEFORE surf->bits
// (`mov esi,[ecx+0x10]`); ours loads bits, then x2, then depth after the
// `jle`. Everything from `xor ebx,ebx` to the last `ret 0x10` matches, with
// the same registers (surf=ecx, span=ebp, w=edx, p=esi, x1=edi, z=eax,
// product=ebx), the same `mov ecx,[row]/imul ecx,ebx/lea ebx,[ecx+edi]`,
// the same `add ecx,edi/add eax,ecx` z advance and the same branch targets.
//
// Two shapes, and they cannot be combined (this is the whole remaining gap):
// A. depth pointer assigned inside `if (w > 0)` (this file): the original's
//    colouring and code body, wrong load order, 96.2%.
// B. depth pointer initialised above the branch (`unsigned char* z =
//    surf->depth;`), which is required to get the load to the top: the first
//    13 instructions then match one for one, and with the offset written
//    inline (`p += surf->pitch * row + x1;` then `z += surf->pitch * row +
//    span->x1;`, the way the MATCHED src/unsorted/0x4c7a20.cpp writes the
//    same struct) the product block and the z advance match too. But the
//    allocator then gives ecx to w and edx to the surf argument, so the whole
//    function is a perfect ecx<->edx mirror of the original and the score is
//    56.6%.
//
// What moves the ecx/edx choice: whether x1 is still live at the branch.
// In B, x1 has to survive the `jle` because it is added to the row offset a
// second time after the test (`add ecx,edi/add eax,ecx` recomputes off+x1
// rather than reusing ebx, which is what the original does), and then the
// argument lands in edx. Every spelling that makes x1 die before the branch
// (folding it into the offset, `int off = row * surf->pitch + x1;` with
// `p += off; z += off;`) puts surf back in ecx, but the z advance collapses
// into the addressing mode (`mov bl,[eax+ecx]`) and the function comes out 116
// bytes, best 85.7%. So the search is for a source with x1 live across the
// branch AND surf in ecx; no declaration order, assignment order, type,
// cast, comparison or operand-order spelling moves it.
//
// Measured and rejected (all with the real checker, via the compile-only
// scorer in build/scratch/0x4c0a90/):
//   256 header combinations (headers.py): 96.2% for every one, including
//     <windows.h>, <stdlib.h>, <string.h>, <memory.h>.
//   permute.py, 15 min, 3918 candidates: 96.2% -> 96.2%, nothing.
//   All 24 declaration orders x {depth inside, depth above}: the shape-A ones
//     score 90.6-96.2% (six orders tie at 96.2%) and never fix the load order;
//     every shape-B one is 56.6%.
//   All 24 assignment orders of the same four locals (declare, then assign):
//     56.6% for all 24.
//   Shape B with the offset inline/named, x1 folded or separate, spelled with
//     the local or the field, `row * pitch` or `pitch * row`, with a cast,
//     unsigned, in a named local, or folded: 45.7-85.7%, never above 96.2%.
//   The test spelled `span->x2 > x1`, `x1 < span->x2`, `0 < span->x2 - x1`
//     or the difference as a common subexpression: 34.6-80.0% (these produce
//     114-129 bytes, so the original really does use a named w).
//   `w = span->x2; ... w -= x1;` (so x2 can be read before bits) and `int x2`
//     as its own local declared first, in both shapes: 78.1-94.3%, and the
//     bits load is still scheduled first.
//   Two-step w, pitch in its own local, an inlined `test_depth()` helper by
//     value, colour as `char`, one reused depth variable, `(unsigned char*)0`
//     instead of `0`: all 56.6%.
//
// Two spellings that must be kept (both cost real bytes when changed):
//   * `z += off + span->x1;` has to read the field. Written with the x1 local
//     the compiler folds the offset into the addressing mode and the whole
//     shape changes (114 bytes instead of 118).
//   * `int off = row * surf->pitch;` computed once, or the inline form; both
//     keep `mov ecx,[row]/imul ecx,ebx`, while `off` including x1 gives the
//     3-instruction `mov ecx,ebx/imul ecx,[row]/add ecx,edi`.
//
// Reusable tooling left in build/scratch/0x4c0a90/: multi.sh compiles a file of
// many variants and scores every function in it at once (about 3 s for 48
// variants instead of 60 s per check.py run), probe.sh prints one variant's
// /Fa listing, sc.sh scores a single scratch file, gen_perm.py,
// gen_assign.py and gen_both.py write the declaration/assignment order sweeps.
//
// DeepSeek V4.1 Flash pass (issue 4814, 3-minute permuter box): best stays
// 96.2%, 118 of 118 bytes. The 3-minute permuter run from this file (2205
// candidates) found nothing, and a run from a clean shape-B seed climbed only
// 56.6 -> 80.0 into implausible source (self-assignments and an empty inline
// helper), so it is not a lead.
//
// New results that map the wall. The prologue needs an entry-block use of
// surf->depth; a load inside `if (w > 0)` cannot be hoisted across the jle, so
// only a shape-B source (z initialised above the branch) reproduces the
// original's depth/x2/bits/x1 load order at all. Shape B with no helper
// variable still colours surf=edx, w=ecx (the mirror); the three instructions
// before `xor ebx,ebx` are the only difference, and its own pitch block already
// matches (`xor ebx,ebx / mov bx,[surf] / mov ecx,[row] / imul ecx,ebx /
// lea ebx,[ecx+edi]`) with surf/w swapped. The whole residual is therefore the
// one coloring decision surf=ecx, w=edx, and the load order is free once that
// is fixed.
//
// One source shape does pin surf=ecx while keeping the entry-block depth load:
// a separate early boolean use of the pointer, `int hd = surf->depth != 0;`
// before the if with `z = surf->depth;` inside. It scores 82.9% (133 bytes):
// the prologue and the whole pitch block then match exactly, but the boolean is
// spilled to a stack slot (`setne dl` / `[esp+0x1c]`) and the arm test becomes
// `test ebx,ebx` instead of `test eax,eax`. Every boolean spelling tried (int,
// long, char, unsigned char, bool, with and without casts, and the condition
// mixed with `z != 0`) is 47.3 to 83.2%, none better than 96.2. So the early
// use forces surf into ecx but costs the pointer register the test needs.
//
// A named pitch local also pins surf=ecx but re-colours the pitch temp. Inside
// the arm, `short pitch = surf->pitch;` with the offsets reading it is 84.6%
// (114 bytes): prologue and tail match, the pitch block is `movsx ecx,[ecx] /
// imul ecx,[row] / lea ebx,[edi+ecx]` instead of the five-instruction original.
// `unsigned short pitch` is 83.8% (120 bytes, `mov cx,[ecx] / and ecx,0xffff`),
// `int pitch` 78.1% (116 bytes, surf moves to ebx and the temp to ecx). A named
// `off` for the product does not separate the two (still 84.6 at best). The
// original's pitch lives in ebx precisely because it is a compiler CSE temp,
// not a source local: naming it lets the allocator reuse dead ecx.
//
// Also swept and flat at 96.2 or worse for this function: shape A with 0-96
// `extern int` declarations and 1-32 uncalled inline functions (no TU state
// hoists the depth load, unlike its sibling 0x4c06e0); shape B with 19 kinds of
// unused declaration at 1-32 copies each (only 116/119-byte recolourings, none
// above 78.1); all 256 header sets on the shape-B named-off form (56.6%) and on
// the inline-offset form (78.1% ceiling). So the only unsolved point is still
// the compiler's surf/w colouring tie, now with the two shapes that each solve
// one half of it recorded above.

struct Span_004c0a90 {
    int x1;                            // +0x0
    int x2;                            // +0x4
    char unknown_8[0x18 - 0x8];
    int z1;                            // +0x18 (16.16)
    int z2;                            // +0x1c (16.16)
};

struct Surface_004c0a90 {
    unsigned short pitch;              // +0x0
    char unknown_2[0x10 - 0x2];
    unsigned char* bits;               // +0x10
    unsigned char* depth;              // +0x14
};

// FUNCTION: 0x4c0a90
void __stdcall FUN_004c0a90(int row, Span_004c0a90* span, Surface_004c0a90* surf, unsigned char color)
{
    unsigned char* z;
    unsigned char* p = surf->bits;
    int x1 = span->x1;
    int w = span->x2 - x1;
    if (w > 0) {
        z = surf->depth;
        int off = row * surf->pitch;
        p += off + x1;
        if (z != 0) {
            z += off + span->x1;
            unsigned char z1 = span->z1 >> 16;
            if (*z <= z1) {
                *p = color;
                *z = z1;
            }
            z += w;
            p += w;
            unsigned char z2 = span->z2 >> 16;
            if (*z <= z2) {
                *p = color;
                *z = z2;
            }
        } else {
            *p = color;
            p[w] = color;
        }
    }
}