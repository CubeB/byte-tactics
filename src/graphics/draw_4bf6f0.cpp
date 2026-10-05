// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Fills a rectangle of `surface` with a solid `color` (FUN_004ccdea does the
// fill). When `surface` is null the screen is locked with LockScreen,
// drawn on and unlocked with UnlockScreen. The rectangle is copied to a local
// first, because ClipRectangle clips it in place. Sibling of 0x4bec70 and
// 0x4bed70.

struct Rect_004bf6f0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipRectangle(Surface* s, Rect_004bf6f0* r);
void __cdecl FUN_004ccdea(Surface* s, Rect_004bf6f0* r, int color);

// FUNCTION: 0x4bf6f0
int __stdcall FillRectangle(Surface* surface, Rect_004bf6f0* rect, int color)
{
    Rect_004bf6f0 r = *rect;
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            result = ClipRectangle(&screen, &r) != 0;
            if (result)
                FUN_004ccdea(&screen, &r, color);
            UnlockScreen(&screen);
        }
    } else {
        result = ClipRectangle(surface, &r) != 0;
        if (result)
            FUN_004ccdea(surface, &r, color);
    }
    return result;
}
