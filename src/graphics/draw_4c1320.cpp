// Decompiled by Opus. Names are provisional.
// Returns 1 if (px, py) lies strictly inside the convex polygon pts[0..n-1]
// (every edge has the point on the same side), 0 otherwise or if n < 3.
// Without a header such as <string.h>, MSVC computes the two products in the
// other order (found with tools/headers.py).
#include <string.h>

struct Point_004c1320 {
    int x;
    int y;
};

// FUNCTION: 0x4c1320
int __stdcall PointInPolygon(Point_004c1320* pts, int n, int px, int py)
{
    if (n < 3)
        return 0;
    for (int i = 0; i < n; i++) {
        Point_004c1320* a = &pts[i];
        Point_004c1320* b = &pts[(i + 1) % n];
        if ((b->y - a->y) * (px - a->x) <= (b->x - a->x) * (py - a->y))
            return 0;
    }
    return 1;
}
