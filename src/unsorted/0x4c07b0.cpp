// Decompiled by Opus. Names are provisional.
// Draws a closed polygon outline: a line between each pair of consecutive
// points, then one from the last point back to the first.

struct Point_004c07b0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

void __stdcall FUN_004be950(void* surface, int x1, int y1, int x2, int y2, unsigned char color);

// FUNCTION: 0x4c07b0
int __stdcall FUN_004c07b0(void* surface, Point_004c07b0* pts, int count, int color)
{
    for (int i = 0; i < count - 1; i++)
        FUN_004be950(surface, pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y, color);
    FUN_004be950(surface, pts[count - 1].x, pts[count - 1].y, pts[0].x, pts[0].y, color);
    return 1;
}
