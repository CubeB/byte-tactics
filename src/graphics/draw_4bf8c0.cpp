// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Draws the outline of `r` (top, right, bottom, left edges in that order) into
// `surface`, or into the screen when `surface` is 0 (locked with
// LockScreen, unlocked with UnlockScreen). Each edge is a segment drawer of the
// same shape as DrawLine (clip with ClipLine, fill with BlitLine).
//
// Suspected original bug: the surface != 0 path returns an uninitialised
// `result`.

struct Surface {
    int unknown_0[2];
    int pitch;                         // +0x8
    unsigned char* pixels;             // +0xc
    int unknown_10[8];                 // 0x30 bytes, the lock descriptor
};

struct Rect_004bf8c0 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

int __stdcall LockScreen(Surface* out);
int __stdcall UnlockScreen(Surface* s);
int __stdcall ClipLine(Surface* dst, int* x0, int* y0, int* x1, int* y1);
void __cdecl BlitLine(Surface* dst, int x0, int y0, int x1, int y1, int color);
int __stdcall DrawLine(Surface* surface, int x0, int y0, int x1, int y1,
                           int color);

// FUNCTION: 0x4bf8c0
int __stdcall DrawRectangle(Surface* surface, Rect_004bf8c0* r, int color)
{
    int result;
    if (surface == 0) {
        Surface screen;
        result = LockScreen(&screen);
        if (result != 0) {
            {
                // Declared first, then assigned y1, x1, y0, x0 as separate statements.
                int x0, y0, x1, y1; y1 = r->top; x1 = r->right; y0 = r->top; x0 = r->left;
                // Kept as in the original: the lock arm tests the address of `screen`.
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->top; x0 = r->right;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->bottom; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            {
                int x0, y0, x1, y1; y1 = r->bottom; x1 = r->left; y0 = r->top; x0 = r->left;
                if (&screen == 0) {
                    Surface inner;
                    if (LockScreen(&inner)) {
                        if (ClipLine(&inner, &x0, &y0, &x1, &y1))
                            BlitLine(&inner, x0, y0, x1, y1, color);
                        UnlockScreen(&inner);
                    }
                } else {
                    if (ClipLine(&screen, &x0, &y0, &x1, &y1))
                        BlitLine(&screen, x0, y0, x1, y1, color);
                }
            }
            UnlockScreen(&screen);
        }
    } else {
        {
            int x0, y0, x1, y1; y1 = r->top; x1 = r->right; y0 = r->top; x0 = r->left;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                BlitLine(surface, x0, y0, x1, y1, color);
        }
        {
            int x0, y0, x1, y1; y1 = r->bottom; x1 = r->right; y0 = r->top; x0 = r->right;
            if (ClipLine(surface, &x0, &y0, &x1, &y1))
                BlitLine(surface, x0, y0, x1, y1, color);
        }
        DrawLine(surface, r->left, r->bottom, r->right, r->bottom, color);
        DrawLine(surface, r->left, r->top, r->left, r->bottom, color);
    }
    return result;
}
