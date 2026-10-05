// Decompiled by space-bunny-free. Names are provisional.
// Draws one character of text: FUN_004bea20 clamps the position into the
// surface and FUN_004cc7ab draws the glyph, taking both the clamped and the
// original coordinates. When `surface` is null the screen is locked with
// FUN_004c5e70 instead, and if that lock fails a second surface is tried.
// FUN_004c5fa0 then unlocks whatever FUN_004c5e70 left in `screen`, even on
// the fallback path where the lock failed.
//
// The four clipped-coordinate locals are declared in this order because the
// order decides which stack slot each one gets: two of them are coalesced into
// argument slots that are dead by then (the `surface` slot, and the `x`/`y`
// slots whose copies become self-stores).

struct Surface_004bee60 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall FUN_004c5e70(Surface_004bee60* out);
int __stdcall FUN_004c5fa0(Surface_004bee60* s);
int __stdcall FUN_004bea20(Surface_004bee60* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl FUN_004cc7ab(Surface_004bee60* dst, int x0, int y0, int x1, int y1, int ch);

// FUNCTION: 0x4bee60
int __stdcall FUN_004bee60(Surface_004bee60* surface, int x, int y, int ch)
{
    int ret;
    if (surface == 0) {
        Surface_004bee60 screen;
        ret = FUN_004c5e70(&screen);
        if (ret) {
            int y1 = y, x1 = x, y0 = y, x0 = x;
            if (&screen == 0) {
                Surface_004bee60 other;
                if (FUN_004c5e70(&other)) {
                    if (FUN_004bea20(&other, &x0, &y0, &x1, &y1))
                        FUN_004cc7ab(&other, x0, y0, x1, y1, ch);
                    FUN_004c5fa0(&other);
                }
            } else {
                if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                    FUN_004cc7ab(&screen, x0, y0, x1, y1, ch);
            }
            FUN_004c5fa0(&screen);
        }
    } else {
        int y1 = y, x1 = x, y0 = y, x0 = x;
        if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
            FUN_004cc7ab(surface, x0, y0, x1, y1, ch);
        ret = 1;
    }
    return ret;
}
