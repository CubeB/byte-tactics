// Decompiled by Opus. Names are provisional.
// Rotates a vector about the third axis: sibling of 0x4b6df0, which rotates
// (y, z) instead of (x, y).

struct Vec3_004b6d50 {
    int x;
    int y;
    int z;
};

// Fixed-point rotation helper written in assembly.
void __cdecl FUN_004b7173(int angle, int* xy);

// FUNCTION: 0x4b6d50
void __stdcall FUN_004b6d50(Vec3_004b6d50* in, Vec3_004b6d50* out, int angle)
{
    int xy[2];
    xy[0] = in->x;
    xy[1] = in->y;
    FUN_004b7173(angle, xy);
    out->x = xy[0];
    out->y = xy[1];
    out->z = in->z;
}
