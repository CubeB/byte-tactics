// Decompiled by Opus. Names are provisional.
// The "unit type killed" defeat condition (see 0x48f900): when a unit of the
// named type dies, counts it down and marks the condition satisfied once
// none are left.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048f8c0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit_0048f8c0 {
    char unknown_0[0x92];
    Info_0048f8c0* info;               // +0x92
};
#pragma pack(pop)

class Class_0048f8c0 {
public:
    char unknown_0[4];
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8
    char name[0x20];                   // +0xc
    int numLeftToKill;                 // +0x2c

    void FUN_0048f8c0(Unit_0048f8c0* unit);
};

// FUNCTION: 0x48f8c0
void Class_0048f8c0::FUN_0048f8c0(Unit_0048f8c0* unit)
{
    if (_strcmpi(name, unit->info->name) == 0) {
        if (--numLeftToKill <= 0) {
            satisfied = 1;
        }
    }
}
