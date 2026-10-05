// Decompiled by Opus. Names are provisional.

struct Rect_004b6750 {
    int x1;                            // +0x0
    int y1;                            // +0x4
    int x2;                            // +0x8
    int y2;                            // +0xc
};

// Returns 1 when rectangle a lies entirely inside rectangle b.
// FUNCTION: 0x4b6750
int __stdcall FUN_004b6750(Rect_004b6750* a, Rect_004b6750* b)
{
    if (a->x1 < b->x1) {
        return 0;
    }
    if (a->x1 > b->x2) {
        return 0;
    }
    if (a->x2 < b->x1) {
        return 0;
    }
    if (a->x2 > b->x2) {
        return 0;
    }
    if (a->y1 < b->y1) {
        return 0;
    }
    if (a->y1 > b->y2) {
        return 0;
    }
    if (a->y2 < b->y1) {
        return 0;
    }
    if (a->y2 > b->y2) {
        return 0;
    }
    return 1;
}
