// Decompiled by Opus. Names are provisional.

struct Vec3_482090 {
    int x;
    int y;
    int z;
};

struct UnitType_482090 {
    char unknown_0[0x170];
    unsigned char field_170;     // +0x170
    char unknown_171[0x202 - 0x171];
    short field_202;             // +0x202
};

#pragma pack(push, 2)
struct Unit {
    char unknown_0[0x6a];
    Vec3_482090 pos;             // +0x6a
    char unknown_76[4];
    char field_7a[0x92 - 0x7a];  // +0x7a
    UnitType_482090* type;       // +0x92
    void* field_96;              // +0x96
    char unknown_9a[0xf8 - 0x9a];
    char field_f8[4];            // +0xf8
};

struct Game_482090 {
    char unknown_0[0x1427f];
    unsigned char field_1427f;   // +0x1427f
};
#pragma pack(pop)

struct Params_482090 {
    void* field_0;               // +0
    void* field_4;               // +4
    short field_8;               // +8
    unsigned char field_a;       // +0xa
    void* field_c;               // +0xc
    Vec3_482090 pos;             // +0x10
    int unknown_1c;              // +0x1c
    int unknown_20;              // +0x20
};

extern Game_482090* g_game;

void __stdcall FUN_00481d50(Params_482090* params);

// FUNCTION: 0x482090
void __stdcall FUN_00482090(Unit* unit)
{
    Params_482090 p;
    p.field_0 = unit->field_96;
    p.field_4 = unit->field_7a;
    p.field_8 = unit->type->field_202;
    p.field_c = unit->field_f8;
    p.pos = unit->pos;
    p.field_a = unit->type->field_170;
    int minY = (g_game->field_1427f + 1) << 16;
    if (p.pos.y < minY) {
        p.pos.y = minY;
    }
    FUN_00481d50(&p);
}
