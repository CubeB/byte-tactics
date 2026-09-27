// Decompiled by Opus. Names are provisional.
// Draws a closed polygon: a line between each pair of consecutive points,
// then one from the first point to the last.

struct Point_0046ba80 {
    int x;
    int y;
};

void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, unsigned char color);

// FUNCTION: 0x46ba80
void __stdcall FUN_0046ba80(void* surface, Point_0046ba80* points, int count, int color)
{
    Point_0046ba80* p = points;
    for (int i = count - 1; i > 0; i--) {
        FUN_004be950(surface, p[0].x, p[0].y, p[1].x, p[1].y, color);
        p++;
    }
    FUN_004be950(surface, points->x, points->y, p->x, p->y, color);
}
