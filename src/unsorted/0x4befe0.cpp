// Decompiled by Opus. Names are provisional.
// Reads the pixel at (x, y) of a surface, or of the screen (locked with
// FUN_004c5e70 and unlocked with FUN_004c5fa0) when `surface` is null.
//
// When the screen cannot be locked the colour is returned uninitialised: its
// stack home is y's slot, which is why that path returns y.

struct Surface_004befe0 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall FUN_004c5e70(Surface_004befe0* out);
int __stdcall FUN_004c5fa0(Surface_004befe0* s);

// FUNCTION: 0x4befe0
unsigned int __stdcall FUN_004befe0(Surface_004befe0* surface, int x, int y)
{
    unsigned int color;
    if (surface == 0) {
        Surface_004befe0 screen;
        if (FUN_004c5e70(&screen)) {
            color = screen.pixels[screen.pitch * y + x];
            FUN_004c5fa0(&screen);
        }
    } else {
        color = surface->pixels[surface->pitch * y + x];
    }
    return color;
}
