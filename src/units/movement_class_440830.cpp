// Decompiled by space-bunny-free. Names are provisional.
// Writes a rectangle of 2-bit cells into the transposed bitmap whose dword at
// column x of row band (y>>4) holds 16 cells stacked down the column.
//
// The key to the original's 5 stack homes and its 0x14 frame is that the
// two-bit cell mask is written as `~(3 << shift)` in the source, with only
// the `3 << shift` part in a local (`m`) and the `~` written where the mask
// is used. That leaves an extra NOT node in the expression tree, and MSVC 5
// then keeps `~m` in ebp with a stack home: it stores it in the outer loop
// body and reloads it at the top of the inner loop, exactly like the original
//   mov [esp+0x18], ebp / jmp body / mov ebp, [esp+0x18]
// With the whole mask as one local (`mask = ~(3 << shift)`) the NOT is folded
// away, the mask is promoted straight into ebp with no home, and the frame
// drops back to 0xc with the `v << shift` scheduled first, so the masked old
// value is never live at the same time and never needs a home either. That
// version is 71.2%.

struct Point_00440830 {
    short x;
    short y;
};

class Class_00440830 {
public:
    int* field_0;                      // +0x0
    short field_4;                     // +0x4
    short field_6;                     // +0x6
    char unknown_8[0x10 - 0x8];
    unsigned int width;                // +0x10
    unsigned int height;               // +0x14
    unsigned int* data;                // +0x18

    void RefreshPassMap(Point_00440830 a, Point_00440830 b);
};

unsigned int __stdcall FUN_0047e1f0(Class_00440830* obj, int x, int y);

// FUNCTION: 0x440830
void Class_00440830::RefreshPassMap(Point_00440830 a, Point_00440830 b)
{
    int left = a.x - field_4;
    int top = a.y - field_6;
    int right = a.x + b.x + 1;
    int bottom = a.y + b.y + 1;
    if (left < 0) {
        left = 0;
    }
    if (top < 0) {
        top = 0;
    }
    if (right > width) {
        right = width;
    }
    if (bottom > height) {
        bottom = height;
    }
    if (left < right && top < bottom) {
        for (int y = top; y < bottom; y++) {
            for (int x = left; x < right; x++) {
                unsigned int v = FUN_0047e1f0(this, x, y);
                unsigned int m = 3 << ((y & 0xf) * 2);
                unsigned int* p = &data[(y >> 4) * width + x];
                *p = (*p & ~m) | (v << ((y & 0xf) * 2));
            }
        }
    }
}
