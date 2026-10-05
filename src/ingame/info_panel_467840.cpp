// Decompiled by space-bunny-free. Names are provisional.
// Sets flag 0x200 on a unit that lies below the sea level line and flag 0x100
// on one that reaches it, when it is close enough in the x/z plane to this
// object. Same unit flag field as the neighbours 0x467960 and 0x467980.

#pragma pack(push, 1)
struct Game_00467840 {
    char unknown_0[0x2a43];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x1427f - 0x2a44];
    unsigned char seaLevel;            // +0x1427f
};
#pragma pack(pop)

#pragma pack(push, 1)
struct UnitDef_00467840 {
    char unknown_0[0x16e];
    int field_16e;                     // +0x16e
    char unknown_172[0x241 - 0x172];
    struct {
        unsigned int bit0_7 : 8;       // +0x241
        unsigned int bit8 : 1;         // tested here
        unsigned int bit9_31 : 23;
    } field_241;
};

struct Unit {
    char unknown_0[0x6a];
    int x;                             // +0x6a
    int y;                             // +0x6e
    int z;                             // +0x72
    char unknown_76[0x92 - 0x76];
    UnitDef_00467840* def;             // +0x92
    char unknown_96[0xa6 - 0x96];
    unsigned short field_a6;           // +0xa6
    char unknown_a8[0xff - 0xa8];
    unsigned char field_ff;            // +0xff
    char unknown_100[0x110 - 0x100];
    unsigned int flags;                // +0x110
};

struct Vec3_00467840 {
    int x, y, z;
};

class Class_00467840 {
public:
    char unknown_0[4];
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    Vec3_00467840 pos;                 // +0xc
    void FUN_00467840(Unit* unit);
};
#pragma pack(pop)

extern Game_00467840* g_game;

// FUNCTION: 0x467840
void Class_00467840::FUN_00467840(Unit* unit)
{
    if (unit->field_a6 == 0 || unit->field_ff == g_game->playerIndex) {
        return;
    }

    UnitDef_00467840* def = unit->def;
    if (def->field_241.bit8) {
        return;
    }

    int dz = unit->z - this->pos.z;
    int dx = unit->x - this->pos.x;
    int dist = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);

    if (unit->y <= ((int)g_game->seaLevel << 16) && dist < this->field_8) {
        unit->flags |= 0x200;
    }

    if (def->field_16e + unit->y >= ((int)g_game->seaLevel << 16) && dist < this->field_4) {
        unit->flags |= 0x100;
    }
}
