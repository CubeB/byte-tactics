// Decompiled by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5. Names are provisional.

// Draws a dashed circle: sweeps `angle` from `step` to 0x10000 in steps of
// step = 0x10000 / n and draws the segment from the previous point to the
// current point only when the counter `start` is odd (the caller passes 0 or
// 1, see 0x4670fd). FUN_004b7123 / FUN_004b70ef are the sine and cosine table
// lookups (angle, radius); FUN_004bea20 clips the segment and FUN_004cc7ab
// draws it. When `surface` is null the screen is locked with FUN_004c5e70 and
// unlocked with FUN_004c5fa0.
//
// PARTIAL: 62.0%, 353 of 353 bytes (was 48.3% and 329 bytes). Claude Sonnet 5.5
// pass (#694). Two things changed the frame from 0x3c to the original's 0x40:
//  - the current point is two plain register locals `x`, `y`, and the four
//    address-taken values `x0, y0, x1, y1` are declared INSIDE the `if (i & 1)`
//    block and copied from px, py, x, y there. The original stores y1 and x1 into
//    the dead argument slots of n and start (0x68 and 0x6c after the four pushes)
//    only inside that branch; the old source made x1, y1 the loop's own address-taken
//    variables, which forces them into the frame all the time.
//  - `i` lives in memory in the original (`mov al, byte ptr [esp+0x20]; test al, 1`,
//    and `mov edx, [esp+0x18]; inc edx; mov [esp+0x18], edx` at the tail). Writing the
//    increment in the condition, `if (i++ & 1)`, is the only spelling found that
//    puts it in memory (the increment at the tail keeps it in ebx, 351 bytes).
//    With that and `step` declared before px and py the file gets the right size.
// What still differs is one register swap: the original keeps px in ebx and the
// angle in ebp (`lea ebx, [edi+eax]`, `mov ebp, eax`, esi and edi as x and y),
// ours puts px in ebp and the angle in ebx, so the prologue and the loop are
// permuted. `i` and `py` are in memory in both; the original also pushes ebx, ebp
// and edi at the start and esi after the guard, and does px and py (`mov
// [esp+0x10], ecx` for py) before the division, where ours divides first.
// Scored with no change to 62.0%: every position of `angle` among the
// declarations (10 variants, 38.4% for the px, py, step order and 62.0% for step
// first), for and while forms, x and y and the four branch copies hoisted to
// function scope (49.0 with the copies hoisted), `unsigned int` and `unsigned char`
// for i, i++ at the tail in five positions (48.6 to 50.2), i as `% 2`, the
// previous point as a struct (34.6), `Surface` with 8 dwords (45.8: the original's
// frame proves it has the 12 dword struct, 0x10 for the four scalars and 0x30
// for the screen), a local copy of surface. The declaration-count probe (0 to 400
// unused externs) is flat at 48.3% for the old source.

struct Surface_004c01a0 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __cdecl FUN_004b7123(int angle, int scale);
int __cdecl FUN_004b70ef(int angle, int scale);
int __stdcall FUN_004c5e70(Surface_004c01a0* out);
int __stdcall FUN_004c5fa0(Surface_004c01a0* s);
int __stdcall FUN_004bea20(Surface_004c01a0* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface_004c01a0* dst, int x0, int y0, int x1, int y1, int color);

// FUNCTION: 0x4c01a0
void __stdcall FUN_004c01a0(Surface_004c01a0* surface, int cx, int cy, int radius,
                            int color, int n, int start)
{
    Surface_004c01a0 screen;
    int angle;
    int step = 0x10000 / n;
    int px = cx + radius;
    int py = cy;
    int i = start;
    if (step > 0x10000)
        return;
    angle = step;
    while (angle <= 0x10000) {
        int x = FUN_004b7123(angle, radius) + cx;
        int y = FUN_004b70ef(angle, radius) + cy;
        if (i++ & 1) {
            int x0 = px;
            int y0 = py;
            int x1 = x;
            int y1 = y;
            if (surface == 0) {
                if (FUN_004c5e70(&screen)) {
                    if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&screen, x0, y0, x1, y1, color);
                    FUN_004c5fa0(&screen);
                }
            } else {
                if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
                    FUN_004cc7ab(surface, x0, y0, x1, y1, color);
            }
        }
        px = x;
        py = y;
        angle += step;
    }
}
