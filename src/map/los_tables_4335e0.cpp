// Decompiled by Haiku. Names are provisional.
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

struct Class_4335e0 {
public:
    char unknown_0[4];
    int field_4;

    int FUN_004335e0(short param_1);
};

// FUNCTION: 0x4335e0
int Class_4335e0::FUN_004335e0(short param_1)
{
    int f = field_4;
    int val = param_1;
    val *= 0x10;
    return val + f;
}
