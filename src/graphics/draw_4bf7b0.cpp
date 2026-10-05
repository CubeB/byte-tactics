// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Outlines a rectangle (four one-pixel edges), drawing into `surface`, or into
// the screen (locked with LockScreen and unlocked with UnlockScreen) when
// `surface` is null. Returns the surface that was drawn on, so a failed lock
// returns 0 without ever unlocking. Called from 0x4a16f0 with a rect and a
// palette level.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

Surface* __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
void __stdcall DrawLitLine(Surface* surface, int x0, int y0, int x1,
                            int y1, int color);

// FUNCTION: 0x4bf7b0
Surface* __stdcall DrawLitRectangle(Surface* surface, int* rect,
                                         int color)
{
    Surface* ret = 0;
    if (surface == 0) {
        Surface screen;
        ret = LockScreen(&screen);
        if (ret) {
            DrawLitLine(&screen, rect[0], rect[1], rect[2], rect[1], color);
            DrawLitLine(&screen, rect[2], rect[1], rect[2], rect[3], color);
            DrawLitLine(&screen, rect[0], rect[3], rect[2], rect[3], color);
            DrawLitLine(&screen, rect[0], rect[1], rect[0], rect[3], color);
            UnlockScreen(&screen);
        }
    } else {
        DrawLitLine(surface, rect[0], rect[1], rect[2], rect[1], color);
        DrawLitLine(surface, rect[2], rect[1], rect[2], rect[3], color);
        DrawLitLine(surface, rect[0], rect[3], rect[2], rect[3], color);
        DrawLitLine(surface, rect[0], rect[1], rect[0], rect[3], color);
    }
    return ret;
}
