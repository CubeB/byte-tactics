// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Fills a rectangle of `surface` with a solid `color` (FUN_004ccdea does the
// fill). When `surface` is null the screen is locked with FUN_004c5e70,
// drawn on and unlocked with FUN_004c5fa0. The rectangle is copied to a local
// first, because FUN_004bf620 clips it in place. Sibling of 0x4bec70 and
// 0x4bed70.

struct Rect_004bf6f0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface_004bf6f0 {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall FUN_004c5e70(Surface_004bf6f0* out);
int __stdcall FUN_004c5fa0(Surface_004bf6f0* s);
int __stdcall FUN_004bf620(Surface_004bf6f0* s, Rect_004bf6f0* r);
void __cdecl FUN_004ccdea(Surface_004bf6f0* s, Rect_004bf6f0* r, int color);

// FUNCTION: 0x4bf6f0
int __stdcall FUN_004bf6f0(Surface_004bf6f0* surface, Rect_004bf6f0* rect, int color)
{
    Rect_004bf6f0 r = *rect;
    int result;
    if (surface == 0) {
        Surface_004bf6f0 screen;
        result = FUN_004c5e70(&screen);
        if (result != 0) {
            result = FUN_004bf620(&screen, &r) != 0;
            if (result)
                FUN_004ccdea(&screen, &r, color);
            FUN_004c5fa0(&screen);
        }
    } else {
        result = FUN_004bf620(surface, &r) != 0;
        if (result)
            FUN_004ccdea(surface, &r, color);
    }
    return result;
}
