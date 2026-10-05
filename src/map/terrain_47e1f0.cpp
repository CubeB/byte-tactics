// Decompiled by space-bunny-free. Names are provisional.

// Tests whether a unit whose footprint is field_4 x field_6 cells can sit at
// cell (x, y): its own rectangle plus the four strips that touch it (row above,
// column right, row below, column left), each one cell wider or taller so the
// corners are covered too. FUN_0047dfc0 returns 3 for a rectangle that is
// completely free, 1 when something is in the way and 0 when the rectangle
// leaves the map, so the first failure is passed straight back to the caller
// while any later failure is reported as a plain 1.
class MovementClass {
public:
    int* field_0;                      // +0x0
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned int width;                // +0x10
    unsigned int height;               // +0x14
    unsigned int* data;                // +0x18
};

int __stdcall FUN_0047dfc0(MovementClass* obj, int x, int y, int w, int h);

// The two footprint fields are copied into locals before the first call: that
// keeps them in registers across the five calls instead of loading each field
// again, and it is what puts the `h + 1` and `x - 1` values of the third and
// second calls into the two stack slots the last call reads back.
// FUNCTION: 0x47e1f0
unsigned int __stdcall FUN_0047e1f0(MovementClass* obj, int x, int y)
{
    int h = obj->field_6;
    int w = obj->field_4;
    unsigned int r = FUN_0047dfc0(obj, x, y, w, h);
    if (r <= 1)
        return r;
    if (FUN_0047dfc0(obj, x - 1, y - 1, w + 1, 1) != 3)
        return 1;
    if (FUN_0047dfc0(obj, x + w, y - 1, 1, h + 1) != 3)
        return 1;
    if (FUN_0047dfc0(obj, x, y + h, w + 1, 1) != 3)
        return 1;
    return FUN_0047dfc0(obj, x - 1, y, 1, h + 1) == 3 ? 3 : 1;
}
