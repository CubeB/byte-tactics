// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free and GPT-6. Names are provisional.
// MATCH: the screen Y projection uses the passed position Z component.

#pragma pack(push, 1)

struct ShortPair_485a40 {
    short x;                           // +0x0
    short y;                           // +0x2
};

union TypeFlags_485a40 {               // the type's word at +0x241
    unsigned int all;
    struct {
        unsigned int movOrder : 2;     // bits 0-1
        unsigned int fireOrder : 2;    // bits 2-3
        unsigned int canAttack : 1;    // bit 4
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;           // bit 7
        unsigned int b8 : 1;
        unsigned int b9 : 1;           // bit 9
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int hi : 16;          // bits 16-31
    } bits;
};

struct UnitType_485a40 {
    char unknown_0[0x14a];
    ShortPair_485a40 offset;           // +0x14a
    char unknown_14e[0x1fa - 0x14e];
    short field_1fa;                   // +0x1fa
    char unknown_1fc[0x210 - 0x1fc];
    unsigned short field_210;          // +0x210
    char unknown_212[0x22e - 0x212];
    unsigned char field_22e;           // +0x22e
    unsigned char field_22f;           // +0x22f
    char unknown_230[0x241 - 0x230];
    TypeFlags_485a40 field_241;        // +0x241
    char unknown_245[0x249 - 0x245];
};

struct Player_485a40 {
    char unknown_0[0x146];
    unsigned char field_146;           // +0x146
};

class UnitResources {
public:
    int unknown[12];
    void* player;
    void Reset(unsigned char playerIndex);
};

class Class_0047cb00 {
public:
    char unknown_0[6];
    void* head;
    void FUN_0047cb00(void* node);
};

class UnitMotion;

struct Pos_485a40 {
    int x, y, z;
};

union Flags_485a40 {
    unsigned int all;
    struct {
        unsigned int b0 : 1;
        unsigned int b1 : 1;
        unsigned int b2 : 1;
        unsigned int b3 : 1;
        unsigned int b4 : 1;
        unsigned int b5 : 1;
        unsigned int b6 : 1;
        unsigned int b7 : 1;
        unsigned int b8 : 1;
        unsigned int b9 : 1;
        unsigned int b10 : 1;
        unsigned int b11 : 1;
        unsigned int b12 : 1;
        unsigned int b13 : 1;
        unsigned int b14 : 1;
        unsigned int b15 : 1;
        unsigned int b16 : 1;
        unsigned int b17 : 1;
        unsigned int mode2 : 2;        // bits 18-19
        unsigned int mode : 2;         // bits 20-21
        unsigned int f22_23 : 2;       // bits 22-23
        unsigned int f24_25 : 2;       // bits 24-25
        unsigned int f26_27 : 2;       // bits 26-27
        unsigned int b28 : 1;
        unsigned int b29 : 1;
        unsigned int b30 : 1;
        unsigned int b31 : 1;
    } bits;
};

union Flags114_485a40 {
    unsigned int all;
    struct {
        unsigned int b0 : 1;
    } bits;
};

struct Unit {
    UnitMotion* obj;                   // +0x0
    char unknown_4[0x64 - 0x4];
    short field_64;                    // +0x64
    unsigned short field_66;           // +0x66
    short field_68;                    // +0x68
    Pos_485a40 pos;                    // +0x6a
    ShortPair_485a40 screen;           // +0x76
    short field_7a;                    // +0x7a
    short field_7c;                    // +0x7c
    ShortPair_485a40 offset;           // +0x7e
    Class_0047cb00* list;              // +0x82
    Unit* owner;                       // +0x86
    Unit* first;                       // +0x8a
    Unit* next;                        // +0x8e
    UnitType_485a40* type;             // +0x92
    Player_485a40* player;             // +0x96
    char unknown_9a[0xa6 - 0x9a];
    short id;                          // +0xa6
    char unknown_a8[0xaa - 0xa8];
    short field_aa;                    // +0xaa
    char unknown_ac[0xb0 - 0xac];
    int field_b0;                      // +0xb0
    char unknown_b4[0xb8 - 0xb4];
    short field_b8;                    // +0xb8
    short field_ba;                    // +0xba
    UnitResources playerRef;           // +0xbc
    int field_f0;                      // +0xf0
    unsigned char field_f4;            // +0xf4
    char unknown_f5[1];
    unsigned char field_f6;            // +0xf6
    unsigned char field_f7;            // +0xf7
    unsigned char field_f8;            // +0xf8
    unsigned char field_f9;            // +0xf9
    unsigned char field_fa;            // +0xfa
    int field_fb;                      // +0xfb
    unsigned char field_ff;            // +0xff
    int field_100;                     // +0x100
    float field_104;                   // +0x104
    short field_108;                   // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char field_10e;           // +0x10e
    struct {
        unsigned char lo : 4;          // +0x10f
        unsigned char hi : 4;
    } field_10f;
    Flags_485a40 flags;                // +0x110
    Flags114_485a40 field_114;         // +0x114
    void ReleaseWeapons(unsigned char index);
};

struct Game {
    char unknown_0[0x2a43];
    unsigned char field_2a43;          // +0x2a43
    char unknown_2a44[0x1439b - 0x2a44];
    UnitType_485a40* unitTypes;        // +0x1439b
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall ResetWeaponTarget(Unit* unit, int index);
void __stdcall SetUnitSquad(Unit* unit, int param_2);
int __stdcall RandomInt(int range);

// FUNCTION: 0x485a40
void __stdcall InitUnitFromType(Unit* unit, Pos_485a40 pos, int param_5)
{
    unit->type = &g_game->unitTypes[(unsigned short)unit->id];
    unit->flags.bits.b28 = 1;
    unit->flags.bits.b29 = (unit->type->field_22f == 0);
    unit->flags.bits.b14 = 0;
    unit->offset = unit->type->offset;
    unit->flags.bits.b31 = unit->type->field_241.bits.hi;
    unit->field_114.bits.b0 = unit->type->field_241.bits.b7;
    unit->flags.bits.b30 = unit->type->field_241.bits.b9;

    if (param_5) {
        unit->field_104 = 0;
        unit->field_108 = unit->type->field_1fa;
    } else {
        unit->field_104 = 1.0f;
        unit->field_100 = 0;
        unit->field_108 = 0;
    }

    unit->field_10f.lo = 0;
    unit->flags.bits.b0 = 1;
    unit->flags.bits.b1 = 0;
    unit->flags.bits.b2 = 0;
    unit->flags.bits.b3 = 0;
    unit->flags.bits.b4 = 0;
    unit->flags.bits.b5 = 1;
    unit->flags.bits.b10 = 0;
    unit->flags.bits.b11 = 0;
    unit->flags.bits.b16 = 1;
    unit->flags.bits.b17 = 0;
    unit->field_f6 = 0;
    unit->field_f7 = 0;
    unit->field_10e = 0;
    unit->field_b0 = 0;
    unit->field_68 = 0;
    unit->pos = pos;

    ShortPair_485a40 off = unit->offset;
    ShortPair_485a40 screen;
    screen.x = (short)((pos.x - off.x * 0x80000 + 0x80000) >> 20);
    screen.y = (short)((pos.z - off.y * 0x80000 + 0x80000) >> 20);
    unit->screen = screen;

    unit->field_66 = (short)(RandomInt(unit->type->field_210)
                             + (0x8000 - unit->type->field_210 / 2));
    unit->field_64 = 0;
    unit->field_7a = 0;
    unit->field_7c = 0;
    unit->field_fa = 0;
    unit->field_fb = 0;
    unit->flags.bits.b9 = (unit->player->field_146 == g_game->field_2a43);
    unit->flags.bits.b8 = 0;

    for (int i = 0; i < 3; i++) {
        ResetWeaponTarget(unit, i);
        ((Unit*)unit)->ReleaseWeapons(i);
    }

    unit->field_ba = 0;
    unit->field_b8 = 0;
    unit->field_f0 = 0;
    unit->field_f4 = 0xa;
    unit->flags.bits.mode2 = unit->type->field_241.bits.movOrder;
    unit->flags.bits.mode = unit->type->field_241.bits.fireOrder;
    unit->flags.bits.b11 = unit->type->field_241.bits.canAttack;
    unit->flags.bits.f26_27 = 0;
    if (unit->type->field_22e > 1) {
        unit->flags.bits.f22_23 = 3;
        unit->flags.bits.f24_25 = 0;
    } else {
        unit->flags.bits.f22_23 = 0;
        unit->flags.bits.f24_25 = 0;
    }

    unit->field_f8 = 0;
    unit->playerRef.Reset(unit->field_ff);
    unit->field_f9 = 0xff;
    unit->field_aa = (short)RandomInt(0x10000);
    SetUnitSquad(unit, 0);
}
