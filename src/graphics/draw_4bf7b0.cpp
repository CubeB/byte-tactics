// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Outlines a rectangle (four one-pixel edges), drawing into `surface`, or into
// the screen (locked with FUN_004c5e70 and unlocked with FUN_004c5fa0) when
// `surface` is null. Returns the surface that was drawn on, so a failed lock
// returns 0 without ever unlocking. Called from 0x4a16f0 with a rect and a
// palette level.

struct Surface_004bf7b0 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

Surface_004bf7b0* __stdcall FUN_004c5e70(Surface_004bf7b0* out);
int __stdcall FUN_004c5fa0(Surface_004bf7b0* s);
void __stdcall FUN_004bec70(Surface_004bf7b0* surface, int x0, int y0, int x1,
                            int y1, int color);

// FUNCTION: 0x4bf7b0
Surface_004bf7b0* __stdcall FUN_004bf7b0(Surface_004bf7b0* surface, int* rect,
                                         int color)
{
    Surface_004bf7b0* ret = 0;
    if (surface == 0) {
        Surface_004bf7b0 screen;
        ret = FUN_004c5e70(&screen);
        if (ret) {
            FUN_004bec70(&screen, rect[0], rect[1], rect[2], rect[1], color);
            FUN_004bec70(&screen, rect[2], rect[1], rect[2], rect[3], color);
            FUN_004bec70(&screen, rect[0], rect[3], rect[2], rect[3], color);
            FUN_004bec70(&screen, rect[0], rect[1], rect[0], rect[3], color);
            FUN_004c5fa0(&screen);
        }
    } else {
        FUN_004bec70(surface, rect[0], rect[1], rect[2], rect[1], color);
        FUN_004bec70(surface, rect[2], rect[1], rect[2], rect[3], color);
        FUN_004bec70(surface, rect[0], rect[3], rect[2], rect[3], color);
        FUN_004bec70(surface, rect[0], rect[1], rect[0], rect[3], color);
    }
    return ret;
}
