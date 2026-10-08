// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The original declares the tag as a struct here, so forward-declare it
// before the header: the key of the first declaration decorates the name.
struct UnitDef;
#include "../units/unit_def.h"

struct Sub_00409520 {
    char unknown_0[0xd4];
    unsigned short field_d4;       // +0xd4
    char unknown_d6[6];
    int field_dc;                  // +0xdc
    char unknown_e0[0x2a];
    char field_10a;                // +0x10a
};

// The flags at +0x245 read as the original's bitfield; the header keeps the
// plain word.
#pragma pack(push, 1)
struct UnitDefFlags_00409520 {
    char unknown_0[0x245];
    unsigned int bit0_3 : 4;
    unsigned int flag : 1;         // bit 4
    unsigned int bit5_31 : 27;
};
#pragma pack(pop)

#define MIN(a, b) (((a) > (b)) ? (b) : (a))

// Sums a score over the three sub-objects at +0x1ee: each live sub-object
// contributes its +0xdc value / 100 plus its +0xd4 value / 40 plus 5. The
// seed is 1, or 0xb when bit 4 of the flags at +0x245 is set. The result is
// clamped to [-100, 100].
// FUNCTION: 0x409520
int __stdcall RateWeapons(UnitDef* p)
{
    int result = 1;
    if (((UnitDefFlags_00409520*)p)->flag)
        result = 0xb;
    Sub_00409520** pp = (Sub_00409520**)p->weapons;
    for (int i = 3; i != 0; i--) {
        Sub_00409520* s = *pp;
        if (s->field_10a != 0)
            result = result + s->field_d4 / 40 + s->field_dc / 100 + 5;
        pp++;
    }
    // MIN() used twice: the compare-and-select appears twice.
    if (MIN(result, 100) < -100)
        return -100;
    return MIN(result, 100);
}
