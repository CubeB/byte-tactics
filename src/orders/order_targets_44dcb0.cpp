// Decompiled by Opus. Names are provisional.
// Same rectangle layout as 0x44dc60 (x1/x2 at +8/+0xc, y1/y2 at +0x10/+0x14):
// is the point (x, y) on the rectangle's border?

class Class_0044dcb0 {
public:
    char unknown_0[8];
    int x1;                  // +8
    int x2;                  // +0xc
    int y1;                  // +0x10
    int y2;                  // +0x14

    int FUN_0044dcb0(int x, int y);
};

// FUNCTION: 0x44dcb0
int Class_0044dcb0::FUN_0044dcb0(int x, int y)
{
    if ((x == x1 || x == x2) && y >= y1 && y <= y2) {
        return 1;
    }
    if ((y == y1 || y == y2) && x >= x1 && x <= x2) {
        return 1;
    }
    return 0;
}
