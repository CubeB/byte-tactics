// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Draws a dashed circle: sweeps `angle` from `step` to 0x10000 in steps of
// step = 0x10000 / n and draws the segment from the previous point to the
// current point only when the counter `start` is odd (the caller passes 0 or
// 1, see 0x4670fd). FUN_004b7123 / FUN_004b70ef are the sine and cosine table
// lookups (angle, radius); FUN_004bea20 clips the segment and FUN_004cc7ab
// draws it. When `surface` is null the screen is locked with FUN_004c5e70 and
// unlocked with FUN_004c5fa0.
//
// Still differs: the original keeps the previous x in ebx, the angle in ebp,
// the current x/y in esi/edi and spills the previous y and the counter to the
// stack; MSVC 5 here instead keeps the previous x/y and the counter in
// registers (edi/ebp/ebx), puts the angle in esi and spills the current x/y.
// The source shape (px/py copies into x0/y0, address-taken end point) is
// right; only the register allocator's choice differs, which
// `check.py` shows as the prologue and loop-carried stores.

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
    int x1, y1;
    int step = 0x10000 / n;
    if (step > 0x10000)
        return;
    int i = start;
    for (int angle = step; angle <= 0x10000; angle += step) {
        x1 = FUN_004b7123(angle, radius) + cx;
        y1 = FUN_004b70ef(angle, radius) + cy;
        if (i & 1) {
            int x0 = px;
            int y0 = py;
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
        px = x1;
        py = y1;
        i++;
    }
}
