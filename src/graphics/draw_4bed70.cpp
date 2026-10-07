// Decompiled by space-bunny-free. Names are provisional.

// Sibling of 0x4be950: draws into `surface`, or into the screen (locked with
// LockScreen and unlocked with UnlockScreen) when `surface` is null. The
// extra argument to the draw is the state's colour table at +0xc0, and the
// result is the surface that was drawn on, so a failed lock returns 0 without
// ever unlocking.

struct App_004bed70 {
    char unknown_0[0xc0];
    unsigned int* palette;            // +0xc0
};

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

App_004bed70* GetDisplay();
Surface* __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* a, int* b, int* c, int* d);
void __cdecl BlitLineRemapped(Surface* dst, int a, int b, int c, int d, int e,
                          unsigned int* palette);

// FUNCTION: 0x4bed70
Surface* __stdcall DrawBlendedLine(Surface* surface, int x0, int y0,
                                         int x1, int y1, int color)
{
    App_004bed70* app = GetDisplay();
    Surface* ret;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                BlitLineRemapped(&screen, x0, y0, x1, y1, color, app->palette);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            BlitLineRemapped(surface, x0, y0, x1, y1, color, app->palette);
        ret = (Surface*)1;
    }
    return ret;
}
