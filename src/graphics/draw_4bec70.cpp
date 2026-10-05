// Decompiled by space-bunny-free. Names are provisional.

// Sibling of 0x4bed70 and 0x4be950: draws into `surface`, or into the screen
// (locked with LockScreen and unlocked with UnlockScreen) when `surface` is
// null. The rectangle is handed to ClipLine by address, so it can clip it
// in place, and the clipped values go to FUN_004cc8df to draw. The state's
// colour table at +0xc8 is both the guard and the last argument of the draw,
// and the result is the surface that was drawn on, so a failed lock returns 0
// without ever unlocking.

struct App_004bec70 {
    char unknown_0[0xc8];
    unsigned int* palette;            // +0xc8
};

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

App_004bec70* GetDisplay();
Surface* __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* a, int* b, int* c, int* d);
void __cdecl FUN_004cc8df(Surface* dst, int a, int b, int c, int d, int e,
                          unsigned int* palette);

// FUNCTION: 0x4bec70
Surface* __stdcall DrawLitLine(Surface* surface, int x0, int y0,
                                         int x1, int y1, int color)
{
    App_004bec70* app = GetDisplay();
    if (!app->palette)
        return 0;
    Surface* ret;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                FUN_004cc8df(&screen, x0, y0, x1, y1, color, app->palette);
            UnlockScreen(&screen);
        }
    } else {
        if (ClipLine(surface, &x0, &y0, &x1, &y1))
            FUN_004cc8df(surface, x0, y0, x1, y1, color, app->palette);
        ret = (Surface*)1;
    }
    return ret;
}
