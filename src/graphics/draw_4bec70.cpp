// Decompiled by space-bunny-free. Names are provisional.

// Sibling of 0x4bed70 and 0x4be950: draws into `surface`, or into the screen
// (locked with FUN_004c5e70 and unlocked with FUN_004c5fa0) when `surface` is
// null. The rectangle is handed to FUN_004bea20 by address, so it can clip it
// in place, and the clipped values go to FUN_004cc8df to draw. The state's
// colour table at +0xc8 is both the guard and the last argument of the draw,
// and the result is the surface that was drawn on, so a failed lock returns 0
// without ever unlocking.

struct App_004bec70 {
    char unknown_0[0xc8];
    unsigned int* palette;            // +0xc8
};

struct Surface_004bec70 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

App_004bec70* FUN_004b6220();
Surface_004bec70* __stdcall FUN_004c5e70(Surface_004bec70* out);
int __stdcall FUN_004c5fa0(Surface_004bec70* s);
int __stdcall FUN_004bea20(Surface_004bec70* dst, int* a, int* b, int* c, int* d);
void __cdecl FUN_004cc8df(Surface_004bec70* dst, int a, int b, int c, int d, int e,
                          unsigned int* palette);

// FUNCTION: 0x4bec70
Surface_004bec70* __stdcall FUN_004bec70(Surface_004bec70* surface, int x0, int y0,
                                         int x1, int y1, int color)
{
    App_004bec70* app = FUN_004b6220();
    if (!app->palette)
        return 0;
    Surface_004bec70* ret;
    if (surface == 0) {
        Surface_004bec70 screen;
        ret = FUN_004c5e70(&screen);
        if (ret) {
            if (FUN_004bea20(&screen, &x0, &y0, &x1, &y1))
                FUN_004cc8df(&screen, x0, y0, x1, y1, color, app->palette);
            FUN_004c5fa0(&screen);
        }
    } else {
        if (FUN_004bea20(surface, &x0, &y0, &x1, &y1))
            FUN_004cc8df(surface, x0, y0, x1, y1, color, app->palette);
        ret = (Surface_004bec70*)1;
    }
    return ret;
}
