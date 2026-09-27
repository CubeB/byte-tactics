// Decompiled by Opus. Names are provisional.

struct Point_0044f150 {
    short x;
    short y;
};

struct Vec3_0044f150 {
    int x;
    int y;
    int z;
};

class Class_0044f150 {
public:
    char unknown_0[0xc];
    Point_0044f150 points[20];         // +0x0c
    int count;                         // +0x5c
    void FUN_0044f150(Vec3_0044f150* out, int unused, int n);
};

// FUNCTION: 0x44f150
void Class_0044f150::FUN_0044f150(Vec3_0044f150* out, int unused, int n)
{
    for (int i = 0; i < n; i++) {
        int j = i < count ? i : count - 1;
        out[i].x = points[j].x << 16;
        out[i].y = 0;
        out[i].z = points[j].y << 16;
    }
}
