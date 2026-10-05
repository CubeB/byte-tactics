// Decompiled by Opus. Names are provisional.
// Same test as 0x44d7c0, on a unit's position: 1 when the squared distance
// from the centre lies within [field_14, field_18].

struct Unit_0044d800 {
    char unknown_0[0x76];
    short x;                           // +0x76
    short y;                           // +0x78
};

class Class_0044d800 {
public:
    char unknown_0[8];
    short field_8;                     // +0x8
    short field_a;                     // +0xa
    char unknown_c[0x14 - 0xc];
    int field_14;                      // +0x14 (min distance squared)
    int field_18;                      // +0x18 (max distance squared)

    int FUN_0044d800(Unit_0044d800* unit);
};

// FUNCTION: 0x44d800
int Class_0044d800::FUN_0044d800(Unit_0044d800* unit)
{
    int dy = unit->y - field_a;
    int dx = unit->x - field_8;
    int distSq = dx * dx + dy * dy;
    return distSq <= field_18 && distSq >= field_14;
}
