// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Same rectangle layout as 0x44dc60/0x44dcb0 (x1/x2 at +8/+0xc, y1/y2 at
// +0x10/+0x14): approximate distance from (x, y) to the rectangle, 0 inside.
// Outside a corner it is 16 * major + 6 * minor axis distance (compare the
// 18/7 ring metric of 0x44d350/0x44d840); straight out from an edge it is
// 16 * distance; inside it is 16 * distance to the nearest edge.
// The nearest-edge part is three inlined `min` selects; written out as a
// function or as a chain of ifs the compiler picks a different register for
// the first argument, so keep the macro form.

class Class_0044dd00 {
public:
    char unknown_0[8];
    int x1;                            // +0x8
    int x2;                            // +0xc
    int y1;                            // +0x10
    int y2;                            // +0x14

    int FUN_0044dd00(int x, int y);
};

#define Min_0044dd00(a, b) ((a) < (b) ? (a) : (b))

// FUNCTION: 0x44dd00
int Class_0044dd00::FUN_0044dd00(int x, int y)
{
    int dx;
    if (x < x1) {
        dx = x1 - x;
    }
    else if (x > x2) {
        dx = x - x2;
    }
    else {
        if (y < y1)
            return (y1 - y) * 16;
        if (y > y2)
            return (y - y2) * 16;
        int mx = Min_0044dd00(x - x1, x2 - x);
        int my = Min_0044dd00(y - y1, y2 - y);
        return Min_0044dd00(mx, my) * 16;
    }

    int dy;
    if (y < y1)
        dy = y1 - y;
    else if (y > y2)
        dy = y - y2;
    else
        return dx * 16;

    if (dx > dy)
        return dx * 16 + dy * 6;
    return dy * 16 + dx * 6;
}
