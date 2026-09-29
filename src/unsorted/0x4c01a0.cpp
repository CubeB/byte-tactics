// Decompiled by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, finished by deepseek-v4.1-flash. Names are provisional.

// Draws a dashed circle: sweeps `angle` from `step` to 0x10000 in steps of
// step = 0x10000 / n and draws the segment from the previous point to the
// current point only when the counter `start` is odd (the caller passes 0 or
// 1, see 0x4670fd). FUN_004b7123 / FUN_004b70ef are the sine and cosine table
// lookups (angle, radius); FUN_004bea20 clips the segment and FUN_004cc7ab
// draws it. When `surface` is null the screen is locked with FUN_004c5e70 and
// unlocked with FUN_004c5fa0.
//
// Three things get this to byte-identical (the 62.0% version had all three
// wrong):
//  - the counter is NOT incremented in the test. The original tests `i & 1`
//    and increments `i` after the per-iteration stores at the loop tail, which
//    also puts i in a frame slot (E-0x38) instead of an argument slot.
//  - `angle` and `step` are both initialised from the division
//    (`int angle = 0x10000 / n; int step = 0x10000 / n;`); the compiler CSEs
//    the idiv and emits `mov ebp, eax; mov [step], eax; cmp ebp, 0x10000`.
//    With `int step = angle` it stores ebp instead and the layout shifts.
//  - the order of the four per-iteration copies (and of the tail statements)
//    drives MSVC 5's store scheduling and scratch-register rotation. Copying
//    y1, x1, y0, x0 (in that order) and writing the tail as px = x; py = y;
//    i++; angle += step; reproduces the original's `mov [esp+0x68], edi` /
//    `mov [esp+0x10], eax` order and the tail's `mov ecx,[step]; mov edx,[i]`.

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
    int px = cx + radius;
    int py = cy;
    int angle = 0x10000 / n;
    int step = 0x10000 / n;
    int i;
    if (angle > 0x10000)
        return;
    i = start;
    while (angle <= 0x10000) {
        int x = FUN_004b7123(angle, radius) + cx;
        int y = FUN_004b70ef(angle, radius) + cy;
        if (i & 1) {
            int y1 = y;
            int x1 = x;
            int y0 = py;
            int x0 = px;
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
        i++;
        angle += step;
    }
}
