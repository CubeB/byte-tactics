// Decompiled by Sonnet, Opus, Claude Opus 5.5, deepseek-v4.1-flash and space-bunny-free. Names are provisional.
// Checksums, random numbers and vector maths. RandomInt is in math_4b6c30.cpp
// (it needs /Gi) and the fixed-point trigonometry in math_4b70a0.cpp (a gap
// region's hand-written assembly, which the build places apart).

#include <windows.h>
#include <stdlib.h>
#include <math.h>

extern const char g_errorCaption[];  // "Error"
extern unsigned int g_randomSeed;

// FUNCTION: 0x4b6b80
void __stdcall ShowErrorBox(const char* param_1, int unused)
{
    (void)unused;
    MessageBoxA(0, param_1, g_errorCaption, 0x40000);
}

// Four-byte checksum of a buffer: byte sum, byte xor, sum of (index ^ byte)
// and xor of (index + byte), packed low byte first.
// FUNCTION: 0x4b6ba0
int __stdcall ComputeChecksum(unsigned char* data, int len)
{
    unsigned char a = 0, b = 0, c = 0, d = 0;
    for (int i = 0; i < len; i++, data++) {
        a += *data;
        b ^= *data;
        c += i ^ *data;
        d ^= i + *data;
    }
    return (d << 24) | (c << 16) | (b << 8) | a;
}

// Seeds a random number generator (the seed is kept odd).
// FUNCTION: 0x4b6ca0
void __stdcall SeedRandom(unsigned int seed)
{
    g_randomSeed = (seed ^ 0x66e29572) | 1;
}

struct Vec3_004b6cc0 {
    int x;
    int y;
    int z;
};

// Fixed-point rotation helper written in assembly.
void __cdecl FUN_004b7173(short angle, int* xy);
void __cdecl FUN_004b7173(int angle, int* xy);

// Rotates a vector by three angles (one per axis pair).
// FUNCTION: 0x4b6cc0
void __stdcall RotateByAngles(Vec3_004b6cc0* in, Vec3_004b6cc0* out, short* angles)
{
    int a[2];
    int b[2];
    a[0] = in->x;
    a[1] = in->y;
    FUN_004b7173(angles[0], a);
    b[0] = a[1];
    b[1] = in->z;
    FUN_004b7173(angles[2], b);
    out->y = b[0];
    a[1] = b[1];
    FUN_004b7173(angles[1], a);
    out->x = a[0];
    out->z = a[1];
}

// Rotates a vector about the third axis: sibling of 0x4b6df0, which rotates
// (y, z) instead of (x, y).
// FUNCTION: 0x4b6d50
void __stdcall RotateAboutZ(Vec3_004b6cc0* in, Vec3_004b6cc0* out, int angle)
{
    int xy[2];
    xy[0] = in->x;
    xy[1] = in->y;
    FUN_004b7173(angle, xy);
    out->x = xy[0];
    out->y = xy[1];
    out->z = in->z;
}

// Rotates a vector in the x/z plane.
// FUNCTION: 0x4b6da0
void __stdcall RotateAboutY(Vec3_004b6cc0* in, Vec3_004b6cc0* out, int angle)
{
    int xz[2];
    xz[0] = in->x;
    xz[1] = in->z;
    FUN_004b7173(angle, xz);
    out->x = xz[0];
    out->y = in->y;
    out->z = xz[1];
}

// FUNCTION: 0x4b6df0
void __stdcall RotateAboutX(Vec3_004b6cc0* in, Vec3_004b6cc0* out, int angle)
{
    int yz[2];
    yz[0] = in->y;
    yz[1] = in->z;
    FUN_004b7173(angle, yz);
    out->x = in->x;
    out->y = yz[0];
    out->z = yz[1];
}

// Approximate length of (dx, dy): the larger magnitude plus a quarter of the
// smaller one.
// FUNCTION: 0x4b6e40
int __stdcall ApproxDistance(int dx, int dy)
{
    int a = abs(dx);
    int b = abs(dy);
    if (a > b)
        return (b >> 2) + a;
    return (a >> 2) + b;
}

// FUNCTION: 0x4b6e80
double __stdcall DotProduct(float param_1, float param_2, float param_3,
                               float param_4, float param_5, float param_6)
{
    double result = param_4 * param_1;
    result = result + param_5 * param_2;
    result = result + param_6 * param_3;
    return result;
}

struct Vec3f_004b6eb0 {
    float x;
    float y;
    float z;
};

// Difference to - from, passed and returned by value.
// FUNCTION: 0x4b6eb0
Vec3f_004b6eb0 __stdcall VectorFromTo(Vec3f_004b6eb0 from, Vec3f_004b6eb0 to)
{
    Vec3f_004b6eb0 r;
    r.x = to.x - from.x;
    r.y = to.y - from.y;
    r.z = to.z - from.z;
    return r;
}

// FUNCTION: 0x4b6f00
Vec3f_004b6eb0 __stdcall VectorFromToInt(Vec3_004b6cc0 from, Vec3_004b6cc0 to)
{
    Vec3f_004b6eb0 r;
    r.x = (float)(to.x - from.x);
    r.y = (float)(to.y - from.y);
    r.z = (float)(to.z - from.z);
    return r;
}

// Cross product a x b, passed and returned by value. The (float) casts on the
// differences emit nothing, but without them MSVC copies r to the return
// buffer with the stores interleaved (three registers instead of four).
// FUNCTION: 0x4b6f70
Vec3f_004b6eb0 __stdcall CrossProduct(Vec3f_004b6eb0 a, Vec3f_004b6eb0 b)
{
    float yz = a.y * b.z;
    float zy = a.z * b.y;
    float zx = a.z * b.x;
    float xz = a.x * b.z;
    float xy = a.x * b.y;
    float yx = a.y * b.x;
    Vec3f_004b6eb0 r;
    r.z = (float)(xy - yx);
    r.x = (float)(yz - zy);
    r.y = (float)(zx - xz);
    return r;
}

// Returns v scaled to unit length. The vector is passed and returned by value
// (the results are spilled into v's own stack slots before being copied out).
// FUNCTION: 0x4b6ff0
Vec3f_004b6eb0 __stdcall NormalizeVector(Vec3f_004b6eb0 v)
{
    // Squares stay in their own locals: an inline sum hoists the x87 loads.
    Vec3f_004b6eb0 sq;
    sq.x = v.x * v.x;
    sq.y = v.y * v.y;
    sq.z = v.z * v.z;
    float len = sqrt(sq.x + sq.y + sq.z);
    v.x /= len;
    v.y /= len;
    v.z /= len;
    return v;
}

// FUNCTION: 0x4b7070
float __stdcall VectorLengthFloat(float x, float y, float z)
{
    return sqrtf(x * x + y * y + z * z);
}
