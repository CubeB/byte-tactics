// Decompiled by Opus. Names are provisional.
// Reads the pixel at (x, y) of a surface, or of the screen (locked with
// LockScreen and unlocked with UnlockScreen) when `surface` is null.
//
// When the screen cannot be locked the colour is returned uninitialised: its
// stack home is y's slot, which is why that path returns y.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);

// FUNCTION: 0x4befe0
unsigned int __stdcall ReadPixel(Surface* surface, int x, int y)
{
    unsigned int color;
    if (surface == 0) {
        Surface screen;
        if (LockScreen(&screen)) {
            color = screen.pixels[screen.pitch * y + x];
            UnlockScreen(&screen);
        }
    } else {
        color = surface->pixels[surface->pitch * y + x];
    }
    return color;
}
