// Decompiled by Opus. Names are provisional.
// Heading from one position to another in the x/z plane.

struct Vec3_0048a980 {
    int x, y, z;
};

// Hand-written fixed-point atan2 in the gap at 0x4b70a0.
int __cdecl FUN_004b715a(int x, int z);

// FUNCTION: 0x48a980
int __stdcall GetHeadingBetween(Vec3_0048a980* from, Vec3_0048a980* to)
{
    return FUN_004b715a(from->x - to->x, from->z - to->z);
}
