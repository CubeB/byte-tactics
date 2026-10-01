// Decompiled by Opus, finished by deepseek-v4.1-flash, verified by GPT-6.1-sol. Names are provisional.
// Retry (deepseek-v4.1-flash, issue 3061): best remains 86.5% (118 bytes both).
// The two residuals are one coupled allocator state. The inline
// `row * surf->pitch` gives the original pitch head (`xor ebx,ebx; mov bx,[ecx]`)
// but mirrors ecx/edx globally (surf->edx, w->ecx), 49.5%. All 24 declaration
// orders of z/p/x1/w with that body top out at 56.6%. Only an
// `unsigned short pitch` local pins surf=ecx, and it forces the `and ecx,0xffff`
// head plus a CSE of `off+x1` instead of the original's preserved `off` with
// `lea ebx,[ecx+edi]`. `unsigned int`/`int` pitch reallocates surf to edi
// (67.3%), <windows.h> drops to 47.6%, and headers.py is flat at 86.5%.
// GPT-6.1-sol retry in #2874: five checks kept the 86.5% best. A y local was
// unchanged; an inline offset helper fell to 49.5%. Pitch multiply and depth
// pointer setup still differ in register and memory-operand selection.
// GPT-6.1-sol retest in #1718: baseline verified at 86.5 percent.
// Inline surface pitch arithmetic and splitting the signed row product from the x1 add both reduce the match (49.5 and 56.6 percent); restored the best source.
// GPT-6.1-sol refinement pass: best remains 86.5 percent after 8 checks,
// including the starting confirmation. Splitting the depth offset into two
// additions scored 80.0; moving its assignment into the no-depth branch scored
// 33.6; delaying the depth load until the branch scored 35.0. Zero-then-assign
// short pitch and an inline pitch helper left the baseline at 86.5; an unsigned
// int pitch local scored 35.0. Restored the best source. No check printed MATCH.

// Codex / GPT-6 retest in #13:
// split row and x offsets, unsigned offsets and wider colour
// parameters did not reproduce the original repeated sum and registers.
// Plots the two end points of one span row: the pixel at each end gets the
// colour when it passes the depth test (the depth buffer keeps the integer
// part of the 16.16 depth), or unconditionally when the surface has no depth
// buffer.
//
// GPT-6.1-sol pass in #1905: best remains 86.5 percent after 7 check.py runs
// and all 128 header combinations. Remaining differences: pitch zero-extension
// uses cx/and instead of ebx/bx, and the depth pointer reuses the offset with an
// indexed load instead of materializing the matching lea and direct load.
// deepseek-v4.1-flash pass in #1337: 86.5 percent, 118 of 118 bytes (up from
// 85.7 percent, 116 bytes). The score moved by reading the pitch into an
// `unsigned short` local before the row multiply. That single change is what
// keeps `surf` in ecx through the whole function; without it every spelling of
// the offset (`int off = row * pitch`, the pitch read inline, or the offset
// expression written twice) flips surf to edx and swaps ecx/edx from the first
// instruction on, which is a hard 49.5 percent. Confirmed 86.5 with the offset
// as `row * pitch + x1` (this file), `x1 + row * pitch`, `pitch * row + x1`
// and `(row * pitch) + x1`; all are byte-identical.
//
// What still differs (two hunks):
//  * the pitch load. Original: `xor ebx,ebx / mov bx,[ecx] / mov ecx,[esp+0x14]
//    / imul ecx,ebx` (zero-extend into scratch ebx, row into ecx, register
//    multiply). Ours: `mov cx,[ecx] / and ecx,0xffff / imul ecx,[esp+0x14]`
//    (pitch into surf's ecx, row as the memory operand of imul). The original
//    head is exactly what the inline `row * surf->pitch` gave (the previous
//    file's head), but that spelling loses surf to edx. Every local pitch type
//    (`int`, `unsigned short`) keeps the registers right and takes the memory
//    operand.
//  * the offset is still computed once. Original materialises `off + x1` twice:
//    `lea ebx,[ecx+edi] / add esi,ebx` for the colour pointer (off stays in
//    ecx), then `add ecx,edi / add eax,ecx` for the depth pointer, and reads
//    `mov bl,[eax]`. Ours folds it into `add ecx,edi / add esi,ecx` then
//    `mov bl,[eax+ecx] / add eax,ecx`. Writing the offset twice is what produces
//    the lea, but only when the pitch is read inline, and that spelling flips
//    the allocation (49.5). So the lea and the correct register set have not yet
//    been reached together; the head difference and the offset difference look
//    coupled through which register holds the multiply result.
//
// Scratch evidence in build/scratch/0x4c0a90/ (v2 to v33): the two separate
// offset expressions (v1, v10, v14, v17, v18, v25, v32) all land at 49.5 with
// the swap; `int off = row*pitch` with two adds (v2, v16, v30) at 56.6; an
// `int pitch` local (v15) at 67.3; the `unsigned short pitch` local family
// (v20, v26, v28, v31) all at 86.5.
//
// 30-min checkpoint (deepseek-v4.1-flash, issue 3249). Best is now the 86.8
// percent / 121-byte probe in the body below (was 86.5 percent / 118 bytes for
// the correct-semantics shape). New evidence from ~700 scored scratch shapes
// (build/scratch/0x4c0a90/, files v*, s_*, o_*, q_*, i*, x1..x5, rnd_*):
//  * A `unsigned short pitch` local with the p offset `row*pitch + x1` and the
//    z offset `row*pitch + span->x1` (scratch s_us_a_b) fixes the entire
//    second hunk and leaves only the head, but is 120 bytes / 83.8 percent: the
//    local-form head is one byte longer than the original's.
//  * The head is the inline read: `xor ebx,ebx / mov bx,[ecx] / mov
//    ecx,[esp+0x14] / imul ecx,ebx` with surf still in ecx. Every inline
//    spelling of `row * surf->pitch` rotates surf to edx (w takes ecx, a hard
//    49.5 to 56.6). Every shape that pins surf to ecx keeps the pitch in a
//    named local and gets `mov cx,[ecx] / and ecx,0xffff / imul ecx,[mem]`
//    instead (83.8 to 86.5). The two properties have not been reached
//    together in any shape tried.
//  * Closest head-correct shape (64.8 percent, 116 bytes, scratch p3_*, k_*,
//    x1..x5, y_*): an uninitialised `int w` whose assignment sits inside the
//    if, guard `span->x2 - span->x1 > 0`. The CSE puts the width subtraction
//    before the test (as the original) and the head is right, but the pointer
//    registers rotate: p=edx, x1=esi, w=edi instead of p=esi, x1=edi, w=edx.
//  * The 86.0 percent variant (scratch q_zpxw_g3_s_w / n3): the same shape but
//    the guard reads w itself (`if (w > 0)`) before its assignment. That makes
//    the allocator produce the original's whole register set (surf=ecx, w=edx,
//    p=esi, x1=edi) and the right head, but adds a garbage `mov edx,[esp+0x10]`
//    and leaves `mov edx,[ebp+4] / sub edx,edi` inside the if (122 bytes).
//    Every spelling that defines w before the guard (initialised, assigned, in
//    the condition, redundant reassignment) rotates surf to edx again.
//  * headers.py: all 128 header sets x 5 C++ headers, on the inline and local
//    shapes with both z offsets, give 0 head-correct sets. <stdio.h>,
//    <math.h> and <ddraw.h> move surf to ebx (78.1 percent) for the inline
//    shape, which is the same ecx/ebx swap seen with an `int pitch` local.
//  * Register lead: among p, x1 and w the first one defined takes edx, the
//    second esi and the third edi (original: w=edx, p=esi, x1=edi, i.e. the
//    width is defined before the two pointers; k shape: p=edx, x1=esi,
//    w=edi). surf takes ecx only when w's definition sits inside the if; when
//    w is defined before the guard surf takes edx and w takes ecx. That is
//    the tie to break.
//  * The body below is a 86.8 percent / 121-byte structural probe (scratch
//    rr_0129 / best_probe_868.cpp), copied in because it outscores the 86.5
//    correct-semantics shape that was here before (kept as
//    build/scratch/0x4c0a90/best_correct_865.cpp). `unsigned short t;`
//    declared with the other locals and assigned INSIDE the if,
//    `t = (unsigned short)row;`, with `p += row * t + x1;` and
//    `z += row * t + span->x1;`, reproduces the original's whole register set
//    (surf=ecx, w=edx, p=esi, x1=edi) and the whole second hunk, differing
//    only in the head (`mov ebx,[esp+0x14] / mov ecx,ebx / and ecx,0xffff /
//    imul ecx,ebx` against `xor ebx,ebx / mov bx,[ecx] / mov ecx,[esp+0x14] /
//    imul ecx,ebx`). WARNING: the probe truncates row to 16 bits, so its
//    behaviour is not the original's; it is kept only as the byte-closest
//    starting point. What it proves: a 16-bit local defined inside the if pins
//    surf to ecx, and the target head needs that local replaced by the inline
//    `surf->pitch` temp (the local form `mov cx,[ecx] / and ecx,0xffff` and
//    the temp form `xor ebx,ebx / mov bx,[ecx]` are the two codegens the
//    allocator picks between). Every correct-semantics spelling with this
//    structure (t = surf->pitch) is 83.8 to 85.7 percent.
//  * Tried and inert: dead-copy tricks (`surf = s`, `p = q`, `w = t`,
//    `row = t`), pitch local at the top / uninitialised / struct-wrapped /
//    reference / pointer / cast / explicit mask, `off` and `start` locals,
//    assignment inside the if condition, redundant reassignment, extra
//    uninitialised locals, p/x1 declared uninitialised and assigned later,
//    400 random shape combinations, 0 to 64 dummy externs. All stay at 86.5
//    or below.

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
    unsigned char* z = surf->depth;
    unsigned char* p = surf->bits;
    int x1 = span->x1;
    int w = span->x2 - x1;
    unsigned short t;
    if (w > 0) {
        t = (unsigned short)row;
        p += row * t + x1;
        if (z != 0) {
            z += row * t + span->x1;
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
