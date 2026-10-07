// Decompiled by space-bunny-free. Names are provisional.

// Draws into `surface`, or into the screen (locked with LockScreen and
// unlocked with UnlockScreen) when `surface` is null. ClipLine clips the
// rectangle and BlitLine fills it. Returns the lock result on the screen
// path, so a failed lock returns 0 without ever unlocking.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* a, int* b, int* c, int* d);
void __cdecl BlitLine(Surface* dst, int a, int b, int c, int d, int e);

// FUNCTION: 0x4be950
int __stdcall DrawLine(Surface* surface, int x0, int y0, int x1, int y1,
                           int color)
{
    int ret;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLine(&screen, x0, y0, x1, y1, color);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLine(surface, x0, y0, x1, y1, color);
        ret = 1;
    }
    return ret;
}
