// Decompiled by space-bunny-free. Names are provisional.
// Draws one character of text: ClipLine clamps the position into the
// surface and BlitLine draws the glyph, taking both the clamped and the
// original coordinates. When `surface` is null the screen is locked with
// LockScreen instead, and if that lock fails a second surface is tried.
// UnlockScreen then unlocks whatever LockScreen left in `screen`, even on
// the fallback path where the lock failed.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl BlitLine(Surface* dst, int x0, int y0, int x1, int y1, int ch);

// FUNCTION: 0x4bee60
int __stdcall DrawPixel(Surface* surface, int x, int y, int ch)
{
    int ret;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            // Declared in this order: it decides each local's stack slot.
            int y1 = y, x1 = x, y0 = y, x0 = x;
            if (&screen == 0) {
                Surface other;
                if (LockScreen(&other)) {
                    if (ClipLine(&other, &x0, &y0, &x1, &y1))
                        BlitLine(&other, x0, y0, x1, y1, ch);
                    UnlockScreen(&other);
                }
            } else {
                if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                    BlitLine(&screen, x0, y0, x1, y1, ch);
            }
            UnlockScreen(&screen);
        }
    } else {
        int y1 = y, x1 = x, y0 = y, x0 = x;
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLine(surface, x0, y0, x1, y1, ch);
        ret = 1;
    }
    return ret;
}
