// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The original declares the tag as a struct here, so forward-declare it
// before the header: the key of the first declaration decorates the name.
struct UnitDef;
#include "../units/unit_def.h"

struct Sub_004095d0 {
    char unknown_0[0xd4];
    unsigned short damage;         // +0xd4
    char unknown_d6[6];
    int range;                     // +0xdc
    char unknown_e0[0x2a];
    char id;                       // +0x10a
};

// The flags at +0x245 read as the original's bitfield; the header keeps the
// plain word.
#pragma pack(push, 1)
struct UnitDefFlags_004095d0 {
    char unknown_0[0x245];
    unsigned int bit0_3 : 4;
    unsigned int flag : 1;         // bit 4
    unsigned int bit5_31 : 27;
};
#pragma pack(pop)

#define MIN(a, b) (((a) > (b)) ? (b) : (a))

extern float __stdcall GetEnergyUse(UnitDef* p);

// FUNCTION: 0x4095d0
int __stdcall RateUnitType(UnitDef* p)
{
    int result = 1;
    if (p->extractsMetal != 0.0f)
        result = 0xb;
    if (p->makesMetal != 0)
        result += 10;
    if (GetEnergyUse(p) < 0.0f)
        result += 10;
    result = (int)((int)(result - p->metalCost * -0.01f) - p->energyCost * -0.002f);
    int extra = 1;
    if (((UnitDefFlags_004095d0*)p)->flag)
        extra = 0xb;
    Sub_004095d0** pp = (Sub_004095d0**)p->weapons;
    for (int i = 3; i != 0; i--) {
        Sub_004095d0* s = *pp;
        if (s->id != 0)
            extra = extra + s->damage / 40 + s->range / 100 + 5;
        pp++;
    }
    result += (signed char)((MIN(extra, 100) < -100) ? -100 : MIN(extra, 100));
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}
