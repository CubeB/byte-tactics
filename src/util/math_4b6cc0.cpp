// Decompiled by Opus. Names are provisional.

struct Vec3_004b6cc0 {
    int x;
    int y;
    int z;
};

void __cdecl FUN_004b7173(short angle, int* xy);

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
