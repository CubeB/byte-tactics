// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Aim a unit's two turret angles at a target point. The heading (+0x36) is
// atan2(dx, dz); the pitch (+0x38) comes from the vertical difference against
// the horizontal distance. Each angle is then moved towards the wanted one by
// at most the unit type's turn rate (+0xE8), and if the wanted angle is more
// than 27000 (about 148 degrees) away while the type's flag bit 22 of +0x111 is
// set, the function gives up and returns 0. Otherwise it returns 1.
#include <math.h>
#include <stdlib.h>

struct Vec3_0049b520 {
    int x, y, z;
};

#pragma pack(push, 1)
struct UnitType_0049b520 {
    char unknown_0[0xe8];
    unsigned short turnRate;           // +0xe8
    char unknown_ea[0x111 - 0xea];
    unsigned int flags;                // +0x111
};
#pragma pack(pop)

struct Unit_0049b520 {
    UnitType_0049b520* type;           // +0x0
    int x;                             // +0x4
    int y;                             // +0x8
    int z;                             // +0xc
    char unknown_10[0x36 - 0x10];
    short heading;                     // +0x36
    short pitch;                       // +0x38
};

// 16.16 value in a 4 byte union: an 8 byte one (or short[4]) changes the frame.
union Fixed_0049b520 {
    int value;
    struct { unsigned short fraction; short whole; };
};

short __cdecl FUN_004b715a(int x, int z);

// FUNCTION: 0x49b520
int __stdcall TurnUnitTowardsPoint(Unit_0049b520* unit, Vec3_0049b520* target)
{
    UnitType_0049b520* type = unit->type;
    // Declared dx, dy, dz in this order: it decides the stack slot assignment.
    int dx = unit->x - target->x;
    Fixed_0049b520 dy;
    dy.value = unit->y - target->y;
    int dz = unit->z - target->z;
    short a1 = FUN_004b715a(dx, dz);
    int dist = (int)_hypot((double)dx, (double)dz);
    short a2 = FUN_004b715a(-dy.whole, (short)(dist >> 16));

    short diff1 = a1 - unit->heading;
    short adiff1 = abs(diff1);
    if (adiff1 > 27000 && (type->flags & 0x800000))
        return 0;
    if (adiff1 < type->turnRate)
        unit->heading = a1;
    else if (diff1 < 0)
        unit->heading = unit->heading - type->turnRate;
    else
        unit->heading = unit->heading + type->turnRate;

    short diff2 = a2 - unit->pitch;
    short adiff2 = abs(diff2);
    if (adiff2 > 27000 && (type->flags & 0x800000))
        return 0;
    if (adiff2 < type->turnRate)
        unit->pitch = a2;
    else if (diff2 < 0)
        unit->pitch = unit->pitch - type->turnRate;
    else
        unit->pitch = unit->pitch + type->turnRate;
    return 1;
}
