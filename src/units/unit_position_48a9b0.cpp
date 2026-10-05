// Decompiled by Opus. Names are provisional.
// Heading from one position to another in the x/z plane (like 0x48a980),
// or `def` when the two positions coincide in that plane.

struct Vec3_0048a9b0 {
    int x, y, z;
};

// Hand-written fixed-point atan2 in the gap at 0x4b70a0.
int __cdecl FUN_004b715a(int x, int z);

// FUNCTION: 0x48a9b0
unsigned short __stdcall GetHeadingBetweenOrDefault(Vec3_0048a9b0* from, Vec3_0048a9b0* to, unsigned short def)
{
    int dx = from->x - to->x;
    int dz = from->z - to->z;
    if (dx == 0 && dz == 0)
        return def;
    return FUN_004b715a(dx, dz);
}
