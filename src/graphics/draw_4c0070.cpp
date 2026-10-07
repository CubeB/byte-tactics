// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Draws the outline of a circle of `radius` around (xc, yc) as 32 segments,
// sweeping the angle from 0x800 to 0x10000 in steps of 0x800. The first point
// is (xc + radius, yc), where the sine/cosine tables are 1 and 0, and each
// following point comes from FUN_004b7123 (x) and FUN_004b70ef (y). The
// per-segment draw lives in an inlined helper: it is that helper's parameters
// that get the four coordinate slots, while the loop keeps the carried point
// in registers, which is what produces the slot shuffle at the top of the
// body. When `surface` is null the helper locks the screen with LockScreen
// and unlocks it with UnlockScreen, once per segment; a failed lock draws
// nothing but the loop still runs to completion.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __cdecl FUN_004b70ef(int angle, int distance);
int __cdecl FUN_004b7123(int angle, int distance);
int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* a, int* b, int* c, int* d);
void __cdecl BlitLine(Surface* dst, int a, int b, int c, int d, int color);

static inline void Draw_004c0070(Surface* surface, int x0, int y0,
                                 int x1, int y1, int color)
{
    if (surface == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLine(&screen, x0, y0, x1, y1, color);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLine(surface, x0, y0, x1, y1, color);
    }
}

// FUNCTION: 0x4c0070
void __stdcall DrawCircle(Surface* surface, int xc, int yc, int radius,
                            int color)
{
    int x0 = xc + radius;
    int y0 = yc;
    for (int angle = 0x800; angle <= 0x10000; angle += 0x800) {
        int x1 = FUN_004b7123(angle, radius) + xc;
        int y1 = FUN_004b70ef(angle, radius) + yc;
        Draw_004c0070(surface, x0, y0, x1, y1, color);
        x0 = x1;
        y0 = y1;
    }
}
