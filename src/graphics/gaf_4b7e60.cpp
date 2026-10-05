// Decompiled by Opus. Names are provisional.
// Clips `rect` to `bounds`, moving `other` by the same amounts.

struct Rect_004b7e60 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

// FUNCTION: 0x4b7e60
void __stdcall FUN_004b7e60(Rect_004b7e60* other, Rect_004b7e60* rect, Rect_004b7e60* bounds)
{
    int d;
    d = rect->left - bounds->left;
    if (d < 0) {
        other->left -= d;
        rect->left -= d;
    }
    d = rect->right - bounds->right;
    if (d > 0) {
        other->right -= d;
        rect->right -= d;
    }
    d = rect->top - bounds->top;
    if (d < 0) {
        other->top -= d;
        rect->top -= d;
    }
    d = rect->bottom - bounds->bottom;
    if (d > 0) {
        other->bottom -= d;
        rect->bottom -= d;
    }
}
