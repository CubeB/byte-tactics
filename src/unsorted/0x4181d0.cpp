// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
//
// Best version so far: 62.3%.  Semantics and the stack frame match (sub esp, 8,
// mid[1] at [esp+0x14], the spilled col[0] at [esp+0x20]).  What still differs
// is MSVC 5's register allocation and instruction scheduling in the top block:
// the original keeps the top-left x sum in eax with the operands loaded in the
// order p[0], p[4], p[6], p[2], while this source is scheduled as
// ((p[2] + p[0]) + p[4]) + p[6] and the colour pointer is hoisted into edi at
// the head of the prologue instead of after the corner sums.  The four call
// argument blocks also reload the corner values into different registers.
// The exact source that produced the original schedule is not reproduced here.

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
