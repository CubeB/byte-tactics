// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Best version so far: 62.3% (notes below updated by Claude Opus 5.5 in #103).
// Semantics and the stack frame match (sub esp, 8, mid[1] at [esp+0x14], the
// spilled col[0] at [esp+0x20]).
//
// What still differs: the operand order of the three four-term sums. The
// original builds x as ((p[0] + p[4]) + p[6]) + p[2], y as
// ((p[3] + p[1]) + p[7]) + p[5] and the height sum as
// ((col[0] + col[2]) + col[3]) + col[1], and keeps p[2] alive in a register
// for the first call. Every source form tried pairs p[0] with p[2] first in
// the x sum. MSVC 5 ignores the source order of these terms entirely, and in
// a scratch file five identical copies of one sum function come out in three
// different orders, so the order is compiler state (what was compiled before
// this function), not source shape. Tried without success: a Pt struct array,
// Pt/array/scalar mid, every order of the three statements, expression vs
// "+=" sums, Avg4/Sum4 inline helpers, mid returned by value, locals for
// p[1].x/col[0]/col[1], "2 +" first, const pointers, each with all 128 sets
// of tools/headers.py. The original file probably defined FUN_00417f60 (the
// callee, directly before this function) first; decompiling it into this file
// is the next thing to try.
//
// Also: the original's "xor ecx, ecx; mov cl, [edi+2]; shl ecx, 8" comes
// from "col[i] * 256"; the "<< 8" below compiles to "mov ch, [edi+2]" but
// scores higher here only because the diff aligns better.
//
// The callee draws contour lines across a triangle (its third coordinate is a
// height: >> 8, minus a sea level byte, into a colour table), so col holds the
// four corner heights of a map cell and this draws the cell as four triangles
// around its centre.

// Gouraud-shaded quad fill: FUN_00417f60 fills one shaded triangle, so the quad
// is drawn as four triangles fanning from the average of the four corners.
extern void __stdcall FUN_00417f60(int* dev, int x1, int y1, int c1,
                                   int x2, int y2, int c2,
                                   int cx, int cy, int cc);

// FUNCTION: 0x4181d0
void __stdcall FUN_004181d0(int* dev, int* p, unsigned char* col)
{
    int mid[2];
    int a = p[0];
    a += p[4];
    a += p[6];
    a += p[2];
    mid[0] = (a + 2) / 4;
    int b = p[3];
    b += p[1];
    b += p[7];
    b += p[5];
    mid[1] = (b + 2) / 4;
    int c0 = col[0];
    int cc = (c0 + col[1] + col[2] + col[3]) << 6;
    FUN_00417f60(dev, p[0], p[1], c0 << 8, p[2], p[3], col[1] << 8, mid[0], mid[1], cc);
    FUN_00417f60(dev, p[2], p[3], col[1] << 8, p[4], p[5], col[2] << 8, mid[0], mid[1], cc);
    FUN_00417f60(dev, p[4], p[5], col[2] << 8, p[6], p[7], col[3] << 8, mid[0], mid[1], cc);
    FUN_00417f60(dev, p[6], p[7], col[3] << 8, p[0], p[1], c0 << 8, mid[0], mid[1], cc);
}
