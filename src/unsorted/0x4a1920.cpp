// Decompiled by Sonnet. Names are provisional.

struct Rect {
    int x0;
    int y0;
    int x1;
    int y1;
};

// FUNCTION: 0x4a1920
int __stdcall FUN_004a1920(Rect* r, int px, int py)
{
    if (px >= r->x0 && px <= r->x1 && py >= r->y0 && py <= r->y1) {
        return 1;
    }
    return 0;
}
