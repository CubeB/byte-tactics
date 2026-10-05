// Decompiled by Opus. Names are provisional.
// Draws a horizontal progress bar: the filled part of `rect` (moved down by
// `dy`) in one colour and, when not full, the rest in another.

struct Rect_004b0510 {
    int x1;                          // +0x0
    int y1;                          // +0x4
    int x2;                          // +0x8
    int y2;                          // +0xc
};

struct Colors_00467b60 {
    char unknown_0[4];
    unsigned char empty;             // +0x4
    char unknown_5[5];
    unsigned char full;              // +0xa
};

void __stdcall FillRectangle(void* surface, Rect_004b0510* rect, int color);

// FUNCTION: 0x467b60
void __stdcall FUN_00467b60(void* surface, int value, int max, Rect_004b0510* rect, Colors_00467b60* colors, int dy)
{
    Rect_004b0510 r = *rect;
    r.y1 += dy;
    r.y2 += dy;
    if (value < 0)
        value = 0;
    if (value > max)
        value = max;
    r.x2 = (rect->x2 - rect->x1) * value / max + r.x1;
    FillRectangle(surface, &r, colors->full);
    if (r.x2 != rect->x2) {
        r.x1 = r.x2 + 1;
        r.x2 = rect->x2;
        FillRectangle(surface, &r, colors->empty);
    }
}
