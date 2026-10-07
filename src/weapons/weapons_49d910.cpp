// Decompiled by space-bunny-free. Names are provisional.
//
// Aim angles from a unit's AimFrom piece to a point: writes the heading
// difference and the pitch through two out-pointers and returns 1.
#include <math.h>

struct Vec3 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x66];
    short heading;                    // +0x66
};
#pragma pack(pop)

union Fixed { int value; struct { unsigned short fraction; short whole; }; };

void __stdcall GetAimFromPosition(Unit* obj, Vec3* out, unsigned char weapon);
int __cdecl FUN_004b715a(int x, int z);

// FUNCTION: 0x49d910
int __stdcall CalcAimAngles(Unit* unit, Unit* target, short* out_heading, short* out_pitch, int weapon, Vec3* point)
{
    Vec3 p;
    GetAimFromPosition(unit, &p, weapon);
    int dx = p.x - point->x;
    // Read through the 16.16 union and initialised before the heading: loads only the high word.
    Fixed dy;
    dy.value = p.y - point->y;
    int dz = p.z - point->z;
    *out_heading = (short)(FUN_004b715a(dx, dz) - unit->heading);
    *out_pitch = (short)FUN_004b715a(-dy.whole, (short)((int)_hypot((double)dx, (double)dz) >> 16));
    return 1;
}
