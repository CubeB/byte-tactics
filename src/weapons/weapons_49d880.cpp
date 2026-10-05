// Decompiled by deepseek-v4.1-flash. Names are provisional.
#include <stdlib.h>

struct AimType_0049d880 {
    char unknown_0[0x106];
    unsigned short field_106;          // +0x106
    unsigned short field_108;          // +0x108
};

struct Aim_0049d880 {
    char unknown_0[0xc];
    AimType_0049d880* type;            // +0xc
    char unknown_10[0x16 - 0x10];
    short field_16;                    // +0x16
    short field_18;                    // +0x18
};

struct Unit {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

// True when both of the aim object's angles are within tolerance of the
// corresponding unit angles. With no tolerance in the type, a moved unit
// (flags 0xc) gets a generous one, a still unit a tighter one.
// The tolerance for the second angle is written as an if/else re-reading the
// field, not as `b ? b : a`: the ternary gives the two tolerances the other
// way round in ecx/esi and the bytes no longer match.
// FUNCTION: 0x49d880
int __stdcall AimWithinTolerance(Unit* unit, Aim_0049d880* aim, short angle1, short angle2)
{
    unsigned short a = aim->type->field_106;
    int x;
    int y;
    if (a == 0) {
        if (unit->flags & 0xc) {
            x = 2000;
            y = x;
        } else {
            x = 150;
            y = x;
        }
    } else {
        x = a;
        if (aim->type->field_108)
            y = aim->type->field_108;
        else
            y = a;
    }
    if (abs((short)(aim->field_16 - angle1)) <= x
        && abs((short)(aim->field_18 - angle2)) <= y)
        return 1;
    return 0;
}
