// Decompiled by Opus. Names are provisional.

struct Vec3_004b6df0 {
    int x;
    int y;
    int z;
};

void __cdecl FUN_004b7173(int angle, int* yz);

// FUNCTION: 0x4b6df0
void __stdcall RotateAboutX(Vec3_004b6df0* in, Vec3_004b6df0* out, int angle)
{
    int yz[2];
    yz[0] = in->y;
    yz[1] = in->z;
    FUN_004b7173(angle, yz);
    out->x = in->x;
    out->y = yz[0];
    out->z = yz[1];
}
