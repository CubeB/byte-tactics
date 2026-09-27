// Decompiled by Opus. Names are provisional.
// Builds the horizontal vector (-f1(angle, scale), 0, -f2(angle, scale))
// from the fixed-point trig helpers.

struct Vec3_004103a0 {
    int x;
    int y;
    int z;
};

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x4103a0
Vec3_004103a0 __stdcall FUN_004103a0(short angle, int scale)
{
    Vec3_004103a0 v;
    v.x = -FUN_004b70ef(angle, scale);
    v.y = 0;
    v.z = -FUN_004b7123(angle, scale);
    return v;
}
