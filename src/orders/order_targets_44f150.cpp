// Decompiled by Opus. Names are provisional.
// Class_0044f010's override of slot 3 (vtable 0x4fd458, see 0x44f450.cpp):
// fills n positions from the path points, repeating the last point past the
// end of the path.

struct Point_0044f150 {
    short x;
    short y;
};

struct Vec3_004907e0 {
    int x;
    int y;
    int z;
};

class Class_0044f010 {
public:
    char unknown_4[0xc - 0x4];
    Point_0044f150 points[20];         // +0x0c
    int count;                         // +0x5c

    virtual void FUN_0044ef40(Vec3_004907e0* out, int unused, int n);  // slot 3
};

// FUNCTION: 0x44f150
void Class_0044f010::FUN_0044ef40(Vec3_004907e0* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x = points[j].x << 16;
        out[i].y = 0;
        out[i].z = points[j].y << 16;
    }
}
