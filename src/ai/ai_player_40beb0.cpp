// Decompiled by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
// Moves `from` towards `to` by at most maxLen: returns `to` when it is within
// maxLen, else `from` plus the difference scaled (16.16 fixed point) to length
// maxLen. Returns the vector by value (hidden return buffer).
// The difference must come from an inline `operator-` returning a struct by
// value: that keeps it in memory (the frame's second set of three dwords)
// instead of in registers. The length helper converts each component into
// its own double local; writing `(double)x * x` directly reorders the sum.
#include <math.h>

struct Vec3_0040beb0 {
    int x;
    int y;
    int z;

    int Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        return (int)sqrt(fx * fx + fy * fy + fz * fz);
    }
    void Scale(int s)                  // s is 16.16 fixed point
    {
        x = (int)(((__int64)x * s) >> 16);
        y = (int)(((__int64)y * s) >> 16);
        z = (int)(((__int64)z * s) >> 16);
    }
};

static inline Vec3_0040beb0 operator-(const Vec3_0040beb0& p, const Vec3_0040beb0& q)
{
    Vec3_0040beb0 r;
    r.x = p.x - q.x;
    r.y = p.y - q.y;
    r.z = p.z - q.z;
    return r;
}

// FUNCTION: 0x40beb0
Vec3_0040beb0 __stdcall MoveToward(const Vec3_0040beb0* from, const Vec3_0040beb0* to,
                                     int maxLen)
{
    Vec3_0040beb0 d = *to - *from;
    int len = d.Length();
    if (maxLen >= len)
        return *to;
    d.Scale((int)(((__int64)maxLen << 16) / len));
    Vec3_0040beb0 r;
    r.x = from->x + d.x;
    r.y = from->y + d.y;
    r.z = from->z + d.z;
    return r;
}
