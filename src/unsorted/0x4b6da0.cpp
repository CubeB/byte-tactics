// Decompiled by Opus. Names are provisional.

struct Vec3_004b6da0 {
    int x;
    int y;
    int z;
};

void __cdecl FUN_004b7173(int angle, int* xz);

// Rotates a vector in the x/z plane.
// FUNCTION: 0x4b6da0
void __stdcall FUN_004b6da0(Vec3_004b6da0* in, Vec3_004b6da0* out, int angle)
{
    int xz[2];
    xz[0] = in->x;
    xz[1] = in->z;
    FUN_004b7173(angle, xz);
    out->x = xz[0];
    out->y = in->y;
    out->z = xz[1];
}
