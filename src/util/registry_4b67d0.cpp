// Decompiled by Opus. Names are provisional.

struct Rect_004b67d0 {
    int x1;                            // +0x0
    int y1;                            // +0x4
    int x2;                            // +0x8
    int y2;                            // +0xc
};

// Returns 1 when rectangles a and b overlap.
// FUNCTION: 0x4b67d0
int __stdcall FUN_004b67d0(Rect_004b67d0* a, Rect_004b67d0* b)
{
    if (a->x1 > b->x2) {
        return 0;
    }
    if (a->x2 < b->x1) {
        return 0;
    }
    if (a->y1 > b->y2) {
        return 0;
    }
    return a->y2 >= b->y1 ? 1 : 0;
}
