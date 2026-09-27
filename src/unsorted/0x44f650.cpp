// Decompiled by Opus. Names are provisional.
// Converts up to `count` short 2D points into 16.16 fixed-point 3D vectors
// (x, 0, y), repeating the last point once the list runs out.

struct Point_0044f650 {
    short x;                           // +0x0
    short y;                           // +0x2
};

struct Vec3_0044f650 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

class Class_0044f650 {
public:
    char unknown_0[0xc];
    Point_0044f650 points[3];          // +0xc
    int count;                         // +0x18

    void FUN_0044f650(Vec3_0044f650* out, int unused, int n);
};

// FUNCTION: 0x44f650
void Class_0044f650::FUN_0044f650(Vec3_0044f650* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x = points[j].x << 16;
        out[i].y = 0;
        out[i].z = points[j].y << 16;
    }
}
