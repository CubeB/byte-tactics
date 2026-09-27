// Decompiled by space-bunny-free. Names are provisional.
//
// Aim angles from a unit's AimFrom piece to a point: writes the heading
// difference and the pitch through two out-pointers and returns 1.
//
// Two details decide the code shape. FUN_0043e2e0 fills a local Vec3, so the
// three differences are named locals; the y difference is read back through a
// 16.16 fixed-point union, which is what makes the compiler load only the high
// word (`movsx ecx, word [slot+2]`) where a plain `>> 16` shifts the whole dword
// in a register instead. The y difference must be initialised before the
// heading statement, so that its only use is a reload from the stack and
// `point` is already dead at the first call; initialising it after the heading
// store keeps `point` live across that call and moves all three of `unit`,
// `point` and `dy` into callee-saved registers.
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

void __stdcall FUN_0043e2e0(Unit* obj, Vec3* out, unsigned char weapon);
int __cdecl FUN_004b715a(int x, int z);

// FUNCTION: 0x49d910
int __stdcall FUN_0049d910(Unit* unit, Unit* target, short* out_heading, short* out_pitch, int weapon, Vec3* point)
{
    Vec3 p;
    FUN_0043e2e0(unit, &p, weapon);
    int dx = p.x - point->x;
    Fixed dy;
    dy.value = p.y - point->y;
    int dz = p.z - point->z;
    *out_heading = (short)(FUN_004b715a(dx, dz) - unit->heading);
    *out_pitch = (short)FUN_004b715a(-dy.whole, (short)((int)_hypot((double)dx, (double)dz) >> 16));
    return 1;
}
