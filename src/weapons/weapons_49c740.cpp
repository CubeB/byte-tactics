// Decompiled by space-bunny-free, Opus, deepseek-v4.1-flash, Claude Sonnet 5.5,
// deepseek-v4.1, Claude Opus 5.5, GPT-5.6-Terra, LongCat 2.5 Preview Free, GPT-6
// and GPT-6.1-sol. Names are provisional.
//
// The second part of the weapons module (0x49c740 to 0x49e630): the projectile
// initialiser and the four firing routines (line of sight, V-launch, ballistic
// and dropped), the turret aiming path with its angle helpers, the remote fire
// packet and its projectile scan, the weapon fire handlers, the unit's weapon
// slots and the loop that updates them, and the weapon name table.

struct Vec3 {
    int x;
    int y;
    int z;
};

typedef Vec3 Vec3_0049e1a0;

union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

class CobScript {
public:
    int StartScriptWithArgs(char* name, void* param_2, int param_3, int param_4,
                            int param_5, int param_6, int param_7, int param_8);
    int StartScript(const char* name, void* param_2, int param_3);
};

struct Player {
    char unknown_0[4];
    int id;                            // +0x4
};

struct Gun_0049c9c0 {
    short angle;
    char unknown_2[2];
};

#pragma pack(push, 1)

struct Shot_0049c740 {
    char unknown_0[0xf4];
    unsigned short sound;              // +0xf4
};

struct Unit;

// One of a unit's three weapon slots: 49c740's shot pointer at +0x0 is
// 49e070's attached unit; the rest is 49e070's view.
struct Slot {
    union {
        Shot_0049c740* shot;           // +0x0
        Unit* attached;                // +0x0
    };
    int field_4;                       // +0x4
    short field_8;                     // +0x8
    char unknown_a[4];
    unsigned char field_e;             // +0xe
    unsigned char flags;               // +0xf
    char unknown_10[0xc];
};

struct Type_0049d270 {
    char unknown_0[0x20];
    int param;                         // +0x20
};

struct Entry_0049d270 {                // 0x1c bytes
    char unknown_0[0xc];
    Type_0049d270* type;               // +0xc
    char unknown_10[0x16 - 0x10];
    short f_16;                        // +0x16
    short f_18;                        // +0x18
    char unknown_1a[0x1c - 0x1a];
};

struct Point_0049e1a0 {
    short x;
    short z;
};

struct Flags_0049e1a0 {
    unsigned int b0 : 1, b1 : 1, b2_3 : 2, b4 : 1, b5_18 : 14, b19 : 1, b20_25 : 6, b26 : 1,
        b27 : 1, b28 : 1, b29_31 : 3;
};

// The unit a slot points at. Its class vtable sits at +0x60.
struct Target_0049e1a0 {
    char unknown_00[0x60];
    int(__stdcall* f60)(Unit*, Point_0049e1a0*, Unit*, Vec3_0049e1a0*);
    char unknown_64[0x68 - 0x64];
    int f_68;
    char unknown_6c[0xc0 - 0x6c];
    float f_c0;
    float f_c4;
    float f_c8;
    char unknown_cc[0xe4 - 0xcc];
    unsigned short f_e4;
    char unknown_e6[0x111 - 0xe6];
    Flags_0049e1a0 f_111;
};

struct Entry_0049e1a0 {        // 0x1c bytes
    Point_0049e1a0 point;      // +0x00
    char* name;                // +0x04
    int f_8;                   // +0x08
    Target_0049e1a0* attached; // +0x0c
    char unknown_10[0x14 - 0x10];
    unsigned short f_14; // +0x14, the slot's tick counter
    short f_16;          // +0x16
    short f_18;          // +0x18
    unsigned char f_1a;  // +0x1a
    unsigned char flags; // +0x1b
};

struct Store_0049e1a0 {
    char unknown_0[0x8c];
    float metal; // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy; // +0x98
};

class UnitResources {
public:
    char unknown_0[0x4];
    float x0;
    char unknown_8[0x1c - 0x8];
    float y0;
    char unknown_20[0x30 - 0x20];
    Store_0049e1a0* store; // +0x30
    int SpendEnergyAndMetal(float dx, float dy);
};

struct Owner_0049dd60 {
    char unknown_0[0x20];
    int id;                            // +0x20
};

struct UnitType_0049d000 {
    char unknown_0[0x20];
    int range;                         // +0x20
};

union Flags_0049d580 {
    struct {
        unsigned int f0 : 1;          // bit 0
        unsigned int f1 : 1;          // bit 1
        unsigned int f2_29 : 28;
        unsigned int f30 : 1;         // bit 30
        unsigned int f31 : 1;
    } b;
    unsigned int value;               // +0x111
};

struct Weapon_0049d580 {
    char unknown_0[0x68];
    int speed;                        // +0x68
    char unknown_6c[0xc8 - 0x6c];
    float pitch;                      // +0xc8
    char unknown_cc[0x104 - 0xcc];
    short f_104;                      // +0x104
    char unknown_106[0x10a - 0x106];
    unsigned char team;               // +0x10a
    char unknown_10b[0x111 - 0x10b];
    Flags_0049d580 flags;             // +0x111
};

struct UnitDef_0049d580 {
    char unknown_0[0x1fa];
    unsigned int divisor;             // +0x1fa
};

struct UnitType_0049e070 {
    char unknown_0[0x1ee];
    Unit* attached[3];                // +0x1ee
};

struct UnitType_0049e1a0 {
    char unknown_0[0x1fa];
    unsigned int f_1fa;
};

union Fbb_0049d580 {
    unsigned short value;
    struct {
        unsigned short b0 : 1, b1 : 1, b2 : 1, b3 : 1;
        unsigned short b4 : 1, b5 : 1, b6 : 1, b7 : 1;
        unsigned short b8 : 1, b9 : 1, b10 : 1, b11 : 1;
        unsigned short b12 : 1, b13 : 1, b14 : 1, b15 : 1;
    } f;
};

union Fba_0049e1a0 {
    unsigned short w;
    unsigned char b[2];
};

// The unit, as the weapon code sees it. The views disagree about the bytes at
// +0x4 (49e1a0's three weapon entries, 49d270's one, 49d580's fire state and
// 49c740's and 49e070's weapon slots all overlap there), about +0x0 (49d000's
// type, 49d270's type and 49dd60's owner), about +0x92 (49e070's and 49e1a0's
// type and 49d580's def), about +0xbc (49e1a0's resources, 49e070's field_e4
// and 49c920's and 49c9c0's f_dc and f_e6) and about the flag word (+0x110 in 49c740, 49d880 and 49d270, +0x111 in
// 49c920 and 49c9c0); the rest keep one name. Size 0x118, the unit array's
// stride.
struct Unit {
    union {                            // +0x0
        UnitType_0049d000* type;       // 49d000's unit type
        Type_0049d270* def;            // 49d270's type
        Owner_0049dd60* owner;         // 49dd60's owner
    };
    union {                            // +0x4
        Entry_0049e1a0 entries[3];     // 49e1a0's weapon entries, stride 0x1c
        Entry_0049d270 entry[1];       // 49d270's entry, indexed by the packet
        struct {                       // 49d580's fire state
            char unknown_4[4];
            int f_8;                   // +0x8
            Weapon_0049d580* f_c;      // +0xc
            char unknown_10[6];
            short f_16;                // +0x16
            short f_18;                // +0x18
            char unknown_1a;
            unsigned char f_1b;        // +0x1b
        };
        struct {                       // 49c740's and 49e070's weapon slots
            char unknown_4b[0xc];
            Slot slots[3];             // +0x10, stride 0x1c
        };
        struct {                       // 49c9c0's gun array
            char unknown_4c[0x16];
            Gun_0049c9c0 f_1a[19];     // +0x1a, indexed as f_1a[i * 7]
        };
        struct {                       // 49cc20's aim directions
            char unknown_4d[0x16];
            short aim_0049cc20[4][7][2]; // +0x1a, 0x1c apart
        };
        struct {                       // 49cde0's aim directions
            char unknown_4e[0x16];
            int aim_0049cde0[4][7];    // +0x1a, 0x1c apart
        };
        struct {                       // the aim heading at +0x66
            char unknown_4f[0x62];
            union {
                short heading;
                short angle;           // 49d000's spelling
                unsigned short f_66;   // 49d270's spelling
            };
        };
        struct {                       // 49d9c0's pitch at +0x68
            char unknown_50[0x64];
            union {
                short pitch;
                unsigned int f_68;     // 49c920's and 49c9c0's spelling
            };
        };
        struct {                       // the aim position at +0x6a
            char unknown_51[0x66];
            Vec3 pos;
        };
    };
    char unknown_8a[0x92 - 0x8a];
    union {                            // +0x92
        UnitType_0049e070* utype;      // 49e070's unit type
        UnitType_0049e1a0* mtype;      // 49e1a0's unit type
        UnitDef_0049d580* f_92;        // 49d580's unit definition
    };
    Player* player;                    // +0x96
    union {                            // +0x9a
        CobScript* anims;
        CobScript* script;
    };
    char unknown_9e[0xa8 - 0x9e];
    union {                            // +0xa8
        short f_a8;                    // 49d580's
        unsigned short f_a8u;          // 49d270's
        short field_a8;                // 49d9c0's
        short id;                      // 49d1e0's unit id
    };
    char unknown_aa[0xb0 - 0xaa];
    int field_b0;                      // +0xb0, 49c740's
    char unknown_b4[0xb8 - 0xb4];
    unsigned short f_b8;               // +0xb8, the aiming inaccuracy
    union {                            // +0xba
        Fbb_0049d580 f_bb;
        Fba_0049e1a0 f_ba;
    };
    union {                            // +0xbc
        UnitResources f_bc;            // 49e1a0's resources
        struct {
            char unknown_bc[0xdc - 0xbc];
            int f_dc;                  // +0xdc, 49c920's and 49c9c0's
            char unknown_e0[0xe4 - 0xe0];
            unsigned short field_e4;   // +0xe4, 49e070's reload time
            unsigned short f_e6;       // +0xe6, 49c920's and 49c9c0's
            char unknown_e8[0xf0 - 0xe8];
        };
    };
    char unknown_f0[0xff - 0xf0];
    unsigned char field_ff;            // +0xff, the owner's player colour (49c740)
    char unknown_100[0x108 - 0x100];
    short f_108;                       // +0x108
    unsigned char team;                // +0x10a
    char unknown_10b[0x110 - 0x10b];
    union {                            // +0x110
        unsigned int flags;
        struct {                       // 49d270's bitfield view
            unsigned int b0_27 : 28;
            unsigned int b28 : 1;
            unsigned int b29 : 1;
            unsigned int b30 : 1;
            unsigned int b31 : 1;
        };
        struct {                       // 49c920's and 49c9c0's view
            unsigned char unknown_110;
            unsigned int flags_111;    // +0x111
            char unknown_115[3];
        };
    };
};

struct Proj_0049c740 {
    Shot_0049c740* shot;               // +0x00
    Vec3 pos;                          // +0x04
    Vec3 pos2;                         // +0x10
    char unknown_1c[0x28 - 0x1c];
    Vec3 target;                       // +0x28
    char unknown_34[0x42 - 0x34];
    int field_42;                      // +0x42
    char unknown_46[0x4a - 0x46];
    int field_4a;                      // +0x4a, the current frame number
    int field_4e;                      // +0x4e
    Unit* owner;                       // +0x52, the unit that fired it
    int field_56;                      // +0x56
    char unknown_5a[0x60 - 0x5a];
    short active;                      // +0x60
    short piece;                       // +0x62, which barrel it came from
    char unknown_64[0x66 - 0x64];
    unsigned char player;              // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;              // +0x69
    // Written as an inline method the load of u->field_ff comes before the
    // store of owner, as the original has it; in the enclosing function it
    // sinks below it.
    void SetOwner(Unit* u) { player = u->field_ff; owner = u; }
};

struct DefFlags_0049d270 {
    unsigned int b0 : 1;
    unsigned int b1 : 1;
    unsigned int b2 : 2;
    unsigned int b4 : 1;
    unsigned int b5 : 1;
    unsigned int b6 : 2;
    unsigned int b8 : 1;
    unsigned int b9 : 11;
    unsigned int b20 : 1;
    unsigned int b21 : 11;
};

struct Def_0049d270 {                  // 0x115 bytes
    char unknown_0[0x111];
    DefFlags_0049d270 flags;           // +0x111
};

struct Entry_0049e5b0 {
    char name[0x115];
};

struct UnitType_0049c880 {
    char unknown_0[0xfe];
    short value;                       // +0xfe
};

struct Unit_0049c880;

#pragma pack(push, 1)
// Must end at +0x6b with no trailing padding: the loop stride follows it.
struct Projectile_0049c880 {
    UnitType_0049c880* type;           // +0x0
    Vec3 pos;                          // +0x4
    char unknown_10[0x52 - 0x10];
    Unit_0049c880* owner;              // +0x52, the unit that fired it
    char unknown_56[0x60 - 0x56];
    short active;                      // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69, 16-bit: the byte form changes
                                       // the register allocation of 0x49c880
};
#pragma pack(pop)

// The game state, as the weapon code sees it. The time word keeps one name per
// view (now, frame, field_38a47, teamColor); 49d270's defs and 49e5b0's entries
// are one union, as are the two tracked-projectile pointers.
struct Game {
    char unknown_0[0x2a42];
    char localPlayer;                  // +0x2a42
    char unknown_2a43;
    union {
        unsigned char flags;           // +0x2a44
        unsigned short flags_2a44;
    };
    char unknown_2a46[0x2cf3 - 0x2a46];
    union {
        Def_0049d270 defs[0x100];      // +0x2cf3
        Entry_0049e5b0 entries[0x100];
    };
    union {
        int projectileCount;           // +0x141f3
        int projCount;
        int projectile_count;
    };
    void* projectiles;                 // +0x141f7
    char unknown_141fb[0x14263 - 0x141fb];
    int field_14263;                   // +0x14263
    char unknown_14267[0x142f3 - 0x14267];
    Unit* trackedUnit;                 // +0x142f3
    union {
        Proj_0049c740* trackedProj;    // +0x142f7
        Projectile_0049c880* tracked;
    };
    char unknown_142fb[0x1433f - 0x142fb];
    Vec3 trackedPos;                   // +0x1433f
    short trackedValue;                // +0x1434b
    char unknown_1434d[0x14357 - 0x1434d];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    union {
        int field_38a47;               // +0x38a47
        int frame;
        unsigned int now;
        int teamColor;
    };
};

#pragma pack(pop)

extern Game* g_game;

int __stdcall QueryWeaponPiece(Unit* unit, unsigned char weapon);
void __stdcall PlaySoundAt(int sound, Vec3* pos, int param_3);

// FUNCTION: 0x49c740
void __stdcall InitProjectile(Proj_0049c740* proj, Shot_0049c740* shot, Vec3* pos,
                           Vec3* aim, int field_5, Unit* unit)
{
    unsigned char i;
    proj->shot = shot;
    proj->pos = *pos;
    proj->pos2 = *pos;
    if (aim)
        proj->target = *aim;
    proj->field_42 = field_5;
    proj->flags &= ~1;
    proj->active = 0;
    // Second clear via an unsigned short local, stored last: `&=` folds into one mask.
    unsigned short f = proj->flags;
    // field_4a is stored before the mask clear: puts g_game in edx.
    proj->field_4a = g_game->field_38a47;
    proj->field_56 = 0;
    proj->field_4e = 0;
    proj->flags = f & ~0x30;
    if (unit) {
        proj->SetOwner(unit);
        if ((unit->flags & 0x20000000) && g_game->trackedUnit == unit)
            g_game->trackedProj = proj;
        for (i = 0; i < 3; i++)
            if (unit->slots[i].shot == shot)
                break;
        proj->piece = (short)QueryWeaponPiece(unit, i);
        unit->field_b0 = g_game->field_38a47 + 0x258;
    } else {
        proj->player = 0xa;
        proj->owner = 0;
    }
    PlaySoundAt(shot->sound, pos, 0);
}

void CompactProjectiles();

// Same body as the matched 0x499e50, which the original inlined here.
static inline void Untrack_0049c880(Projectile_0049c880* proj)
{
    if (proj == g_game->tracked) {
        g_game->trackedPos = g_game->tracked->pos;
        g_game->trackedValue = proj->type->value;
        g_game->tracked = 0;
    }
    proj->flags |= 2;
}

// FUNCTION: 0x49c880
void __stdcall RemoveUnitProjectiles(Unit_0049c880* owner)
{
    Projectile_0049c880* proj = (Projectile_0049c880*)g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (proj->active != 0 && proj->owner == owner) {
            Untrack_0049c880(proj);
            CompactProjectiles();
        }
    }
}

#pragma pack(push, 1)
struct Object_0049c920 {
    Unit* unit;                        // +0x0
    char unknown_4[0x46 - 0x4];
    unsigned int time;                 // +0x46
};
#pragma pack(pop)

// FUNCTION: 0x49c920
void __stdcall FUN_0049c920(Object_0049c920* obj)
{
    Unit* unit = obj->unit;
    if (unit->f_68 != 0 && !(unit->flags_111 & 0x8000000)) {
        obj->time = (unit->f_dc << 16) / unit->f_68 + g_game->now;
        return;
    }
    obj->time = g_game->now + unit->f_e6;
}

#pragma pack(push, 1)
struct Obj_0049c980 {
    char unknown_0[0x3a];
    int value;                         // +0x3a
};

struct Src_0049c980 {
    char unknown_0[0x68];
    int f_68;                          // +0x68
    int f_6c;                          // +0x6c
    int f_70;                          // +0x70
};
#pragma pack(pop)

// FUNCTION: 0x49c980
void __stdcall FUN_0049c980(Obj_0049c980* obj, Src_0049c980* src)
{
    if (src->f_6c) {
        obj->value = src->f_6c;
        return;
    }
    if (src->f_70 == 0) {
        obj->value = src->f_68;
        return;
    }
    obj->value = 0;
}

#include <math.h>

int __cdecl FUN_004b715a(int x, int z);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __stdcall FUN_004729d0(Vec3* p, short index);

extern char* DAT_00509678[4];

#pragma pack(push, 1)
struct Shot_0049c9c0 {
    char unknown_0[0x68];
    unsigned int f_68;                // +0x68
    int f_6c;                          // +0x6c
    int f_70;                          // +0x70
    char unknown_74[0xea - 0x74];
    unsigned short f_ea;               // +0xea
    char unknown_ec[0x111 - 0xec];
    unsigned int f_lower : 9;
    unsigned int f_bit9 : 1;           // bit 9 of +0x111
};

struct Proj_0049c9c0 {
    Unit* unit;                        // +0x00
    char unknown_4[0x1c - 4];
    int f_1c;                          // +0x1c
    int f_20;                          // +0x20
    int f_24;                          // +0x24
    char unknown_28[0x36 - 0x28];
    short f_36;                        // +0x36
    short f_38;                        // +0x38
    int f_3a;                          // +0x3a
    int f_3e;                          // +0x3e
    int f_42;                          // +0x42
    int f_46;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int f_4e;                          // +0x4e
    char unknown_52[0x60 - 0x52];
    short f_60;                        // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69
};
#pragma pack(pop)

struct Fire_0049c9c0 {
    char unknown_0[0xc];
    Shot_0049c9c0* shot;               // +0xc
    char unknown_10[0x1b - 0x10];
    unsigned char f_1b;                // +0x1b
};

void __stdcall InitProjectile(Proj_0049c9c0* proj, Shot_0049c9c0* shot,
                           Vec3* pos, Vec3* aim, int frame,
                           Unit* unit);

// FUNCTION: 0x49c9c0
int __stdcall FireLineOfSightProjectile(Fire_0049c9c0* fire, Unit* unit,
                           Vec3* p3, Vec3* p4, int param_5)
{
    Proj_0049c9c0* proj = 0;
    int i = g_game->projectileCount;
    if (i < 0x12c) {
        proj = (Proj_0049c9c0*)g_game->projectiles + i;
        g_game->projectileCount = i + 1;
        proj->flags &= ~2;
        proj->f_4e = 0;
    }
    if (!proj)
        return 0;

    InitProjectile(proj, fire->shot, p3, p4, g_game->frame, unit);

    int dx = p3->x - p4->x;
    // 4-byte union: only the high half is read, as a 16-bit load from a frame slot.
    union { int value; short halves[2]; } dy;
    dy.value = p3->y - p4->y;
    int dz = p3->z - p4->z;
    short a1 = FUN_004b715a(dx, dz);
    // Unsigned: the >> 16 below is a logical shift.
    unsigned int dist = (int)_hypot(dx, dz);
    proj->f_3e = (int)dist;
    short a2 = FUN_004b715a(-(int)dy.halves[1], (short)(dist >> 16));
    proj->f_36 = a1;
    proj->f_38 = a2;

    // The same helper as 0x49c980, inlined here.
    if (fire->shot->f_6c) {
        proj->f_3a = fire->shot->f_6c;
    } else if (!fire->shot->f_70) {
        proj->f_3a = fire->shot->f_68;
    } else {
        proj->f_3a = 0;
    }

    proj->f_20 = FUN_004b70ef(a2, proj->f_3a);
    int t = FUN_004b7123(a2, proj->f_3a);
    proj->f_1c = -FUN_004b70ef(a1, t);
    proj->f_24 = -FUN_004b7123(a1, t);

    // The same helper as 0x49c920, inlined here.
    Unit* u = proj->unit;
    if (u->f_68 != 0 && !(u->flags_111 & 0x8000000))
        proj->f_46 = (u->f_dc << 16) / u->f_68 + g_game->frame;
    else
        proj->f_46 = g_game->frame + u->f_e6;
    proj->f_4e = param_5;

    proj->f_60 = fire->shot->f_ea;
    unit->anims->StartScript(DAT_00509678[(fire->f_1b >> 2) & 3], 0, 0);
    short angle = unit->f_1a[((fire->f_1b >> 2) & 3) * 7].angle - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    ((CobScript*)unit->anims)->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);

    if (fire->shot->f_bit9)
        FUN_004729d0(p3, 9);
    return 1;
}

#include <string.h>

#pragma pack(push, 1)
struct FlagsBits_0049cc20 {
    unsigned int low : 9;
    unsigned int bit9 : 1;
    unsigned int high : 22;
};

// Union of int and bitfield views: bit 27 is tested as an int, bit 9 as a bitfield.
union Flags_0049cc20 {
    unsigned int all;
    FlagsBits_0049cc20 bits;
};

struct UnitType_0049cc20 {
    char unknown_0[0x68];
    int f_68;
    int f_6c;
    int f_70;
    char unknown_74[0xdc - 0x74];
    int f_dc;
    char unknown_e0[0xe6 - 0xe0];
    unsigned short f_e6;
    char unknown_e8[0xea - 0xe8];
    unsigned short f_ea;
    char unknown_ec[0x111 - 0xec];
    Flags_0049cc20 flags;              // +0x111
};

struct Shot_0049cc20 {
    char unknown_0[0x8];
    int field_8;
    UnitType_0049cc20* def;            // +0xc, the unit type that fired
    char unknown_10[0x1b - 0x10];
    unsigned char field_1b;            // +0x1b
};

struct Proj_0049cc20 {
    UnitType_0049cc20* unit;           // +0x0
    char unknown_4[0x1c - 0x4];
    Vec3 dir;                          // +0x1c
    char unknown_28[0x36 - 0x28];
    short angle;                       // +0x36
    short pitch0;                      // +0x38
    int field_3a;
    char unknown_3e[0x46 - 0x3e];
    int time;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int field_4e;
    char unknown_52[0x56 - 0x52];
    int field_56;
    char unknown_5a[0x60 - 0x5a];
    short active;                      // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69
};
#pragma pack(pop)

void __stdcall InitProjectile(Proj_0049cc20* proj, UnitType_0049cc20* shot, Vec3* pos,
                            Vec3* aim, int field_5, Unit* unit);

// FUNCTION: 0x49cc20
int __stdcall FireVLaunchProjectile(Shot_0049cc20* shot, Unit* unit, Vec3* pos,
                           Vec3* aim, int param_5, int param_6)
{
    Proj_0049cc20* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049cc20*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (!proj)
        return 0;

    InitProjectile(proj, shot->def, pos, aim, g_game->field_38a47, unit);
    proj->angle = 0;
    proj->pitch0 = 0x4000;
    shot->field_8 = 0;
    if (shot->def->f_6c) {
        proj->field_3a = shot->def->f_6c;
    } else if (shot->def->f_70 == 0) {
        proj->field_3a = shot->def->f_68;
    } else {
        proj->field_3a = 0;
    }
    memset(&proj->dir, 0, 12);
    UnitType_0049cc20* u = proj->unit;
    if (u->f_68 != 0 && !(u->flags.all & 0x8000000)) {
        proj->time = (u->f_dc << 16) / (unsigned int)u->f_68 + g_game->field_38a47;
    } else {
        proj->time = g_game->field_38a47 + u->f_e6;
    }
    proj->field_4e = param_5;
    proj->field_56 = param_6;
    proj->active = shot->def->f_ea;
    ((CobScript*)unit->anims)->StartScript(DAT_00509678[(shot->field_1b >> 2) & 3], 0, 0);
    short angle = unit->aim_0049cc20[(shot->field_1b >> 2) & 3][0][0] - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
    if (shot->def->flags.bits.bit9)
        FUN_004729d0(pos, 9);
    return 1;
}

#pragma pack(push, 1)
// Bitfields, not masks: `& 0x800000` / `& 0x200` compile to a single test.
struct Flags_0049cde0 {
    unsigned int a : 9;
    unsigned int f9 : 1;               // bit 9
    unsigned int b : 13;
    unsigned int f23 : 1;              // bit 23
    unsigned int c : 8;
};

struct UnitType_0049cde0 {
    char unknown_0[0x68];
    int f_68;                          // the fixed point length
    char unknown_6c[0xe6 - 0x6c];
    unsigned short f_e6;               // +0xe6, the fallback flight time
    char unknown_e8[0xea - 0xe8];
    unsigned short f_ea;               // +0xea
    char unknown_ec[0x111 - 0xec];
    Flags_0049cde0 flags;             // +0x111
};

struct Shot_0049cde0 {
    char unknown_0[0xc];
    UnitType_0049cde0* def;            // +0xc, the unit type that fired
    unsigned int field_10;             // +0x10
    char unknown_14[0x16 - 0x14];
    short heading;                     // +0x16
    short pitch;                       // +0x18
    char unknown_1a[0x1b - 0x1a];
    unsigned char field_1b;            // +0x1b
};

struct Proj_0049cde0 {
    char unknown_0[0x1c];
    int dirx;                          // +0x1c
    int diry;                          // +0x20
    int dirz;                          // +0x24
    char unknown_28[0x36 - 0x28];
    short angle;                       // +0x36
    short pitch0;                      // +0x38
    char unknown_3a[0x46 - 0x3a];
    int time;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int field_4e;
    char unknown_52[0x60 - 0x52];
    short active;                      // +0x60
    char unknown_62[0x69 - 0x62];
    unsigned short flags;              // +0x69
};
#pragma pack(pop)

void __stdcall InitProjectile(Proj_0049cde0* proj, UnitType_0049cde0* shot, Vec3* pos,
                            Vec3* aim, int field_5, Unit* unit);

// FUNCTION: 0x49cde0
int __stdcall FireBallisticProjectile(Shot_0049cde0* shot, Unit* unit, Vec3* pos,
                           Vec3* aim, int param_5)
{
    Proj_0049cde0* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049cde0*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (proj) {
        InitProjectile(proj, shot->def, pos, 0, g_game->field_38a47, unit);
        proj->angle = shot->heading;
        proj->pitch0 = shot->pitch;
        // The whole product is its own statement: inside the assignment the
        // division and multiply sink after the call.
        int q = (shot->field_10 / shot->def->f_68) * g_game->field_14263;
        proj->diry = FUN_004b70ef(shot->pitch, shot->def->f_68) - q;
        int scale = FUN_004b7123(shot->pitch, shot->def->f_68);
        proj->dirx = -FUN_004b70ef(shot->heading, scale);
        proj->dirz = -FUN_004b7123(shot->heading, scale);
        if (shot->def->flags.f23) {
            proj->time = (int)_hypot((double)(pos->x - aim->x), (double)(pos->z - aim->z)) / scale
                       + g_game->field_38a47;
        } else {
            proj->time = g_game->field_38a47 + shot->def->f_e6;
        }
        // active before field_4e: the reverse of the natural order is the original's.
        proj->active = shot->def->f_ea;
        proj->field_4e = param_5;
        ((CobScript*)unit->anims)->StartScript(DAT_00509678[(shot->field_1b >> 2) & 3], 0, 0);
        short angle = unit->aim_0049cde0[(shot->field_1b >> 2) & 3][0] - unit->heading;
        int a = -FUN_004b70ef(angle, 800);
        int b = -FUN_004b7123(angle, 800);
        unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
        if (shot->def->flags.f9)
            FUN_004729d0(pos, 9);
        return 1;
    }
    return 0;
}

#pragma pack(push, 1)
struct Shot_0049d000 {
    char unknown_0[0xc];
    void* weapon;                     // +0xc
};

struct Proj_0049d000 {
    char unknown_0[0x1c];
    int dirX;                         // +0x1c
    int field_20;
    int dirZ;                         // +0x24
    char unknown_28[0x36 - 0x28];
    short angle;                      // +0x36
    char unknown_38[0x3a - 0x38];
    int field_3a;
    char unknown_3e[0x4e - 0x3e];
    int field_4e;
    char unknown_52[0x69 - 0x52];
    unsigned short flags;             // +0x69
    // Only inside an inline method does MSVC 5 put the angle load before the
    // +0x3a store, as the original does; inline it in the caller and it sinks.
    void Setup(Unit* u) { angle = u->angle; field_3a = 0; field_20 = 0; }
};
#pragma pack(pop)

void __stdcall InitProjectile(Proj_0049d000*, void*, void*, int, int, Unit*);

// FUNCTION: 0x49d000
int __stdcall FUN_0049d000(Shot_0049d000* shot, Unit* unit, Vec3* pos)
{
    Proj_0049d000* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049d000*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (proj) {
        InitProjectile(proj, shot->weapon, pos, 0, g_game->field_38a47, unit);
        proj->Setup(unit);
        proj->dirX = -FUN_004b70ef(proj->angle, unit->type->range);
        proj->dirZ = -FUN_004b7123(proj->angle, unit->type->range);
        return 1;
    }
    return 0;
}

struct Flags_0049d0c0 {
    unsigned int bit0 : 1;
    unsigned int bit1 : 1;
    unsigned int unknown_2 : 18;
    unsigned int bit20 : 1;
};

#pragma pack(push, 1)
struct Info_0049d0c0 {
    char unknown_0[0x111];
    Flags_0049d0c0 flags;              // +0x111
};
#pragma pack(pop)

struct Object_0049d0c0 {
    char unknown_0[0xc];
    Info_0049d0c0* info;               // +0xc
};

int __stdcall FireLineOfSightProjectile(Object_0049d0c0* obj, int a, int b, int c, int d);
int __stdcall FireBallisticProjectile(Object_0049d0c0* obj, int a, int b, int c, int d);

// The last two arguments are passed to both callees in swapped order.
// FUNCTION: 0x49d0c0
int __stdcall FUN_0049d0c0(Object_0049d0c0* obj, int a, int b, int d, int c)
{
    int result = 0;
    if (obj->info->flags.bit0 || obj->info->flags.bit20)
        result = FireLineOfSightProjectile(obj, a, b, c, d);
    else if (obj->info->flags.bit1)
        result = FireBallisticProjectile(obj, a, b, c, d);
    return result;
}

#pragma pack(push, 1)
struct Flags_0049d120 {
    unsigned int unknown_0 : 29;
    unsigned int hitscan : 1;        // +0x111
    unsigned int unknown_30 : 2;
};

struct Def_0049d120 {
    char unknown_0[0xe0];
    int radius;                      // +0xe0
    char unknown_e4[0x111 - 0xe4];
    Flags_0049d120 flags;            // +0x111
};

struct Entry_0049d120 {              // 0x1c bytes
    char unknown_0[0xc];
    Def_0049d120* def;               // +0xc
    char unknown_10[0x1a - 0x10];
    unsigned char active;            // +0x1a
    char unknown_1b;
};

struct Table_0049d120 {
    char unknown_0[4];
    // Only three entries are declared: the index is masked to a byte, but the
    // fields below have to land on 0x6a and 0xff, which fixes the array size.
    Entry_0049d120 entries[3];       // +0x4
    char unknown_58[0x6a - 0x58];
    Vec3 pos;                        // +0x6a
    char unknown_76[0xff - 0x76];
    char player;                     // +0xff
};

struct Proj_0049d120 {               // 0x6b bytes
    Def_0049d120* def;               // +0x0
    char unknown_4[0x28 - 0x4];
    Vec3 pos;                        // +0x28
    char unknown_34[0x56 - 0x34];
    Proj_0049d120* ref;              // +0x56
    char unknown_5a[0x66 - 0x5a];
    char player;                     // +0x66
    char unknown_67[0x6b - 0x67];
};
#pragma pack(pop)

// FUNCTION: 0x49d120
Proj_0049d120* __stdcall FindTargetableProjectile(Table_0049d120* table, int index)
{
    Entry_0049d120* entry = &table->entries[index & 0xff];
    // Flag read before the radius, shift inside its initialiser: else the flag
    // load hoists into the prologue.
    if (!entry->active)
        return 0;
    int radius = entry->def->radius << 16;
    // projs is read before projCount: keeps g_game in ecx.
    Proj_0049d120* proj = (Proj_0049d120*)g_game->projectiles;
    int count = g_game->projCount;
    for (int i = 0; i < count; i++, proj++) {
        if (proj->player != table->player) {
            // Nested ifs, not one && chain: else the bitfield test folds into a mem test.
            if (proj->def->flags.hitscan) {
                if ((unsigned)(table->pos.x - proj->pos.x + radius) <= 2u * radius) {
                    if ((unsigned)(table->pos.z - proj->pos.z + radius) <= 2u * radius) {
                        int j;
                        for (j = 0; j < g_game->projCount; j++)
                            if (((Proj_0049d120*)g_game->projectiles)[j].ref == proj)
                                break;
                        if (j == g_game->projCount)
                            return proj;
                    }
                }
            }
        }
    }
    return 0;
}

#pragma pack(push, 1)
struct Projectile_0049d1e0 {
    char unknown_0[0x28];
    Vec3 pos;                          // +0x28
    char unknown_34[0x52 - 0x34];
    Unit* owner;                       // +0x52
    char unknown_56[0x66 - 0x56];
    char player;                       // +0x66
    char unknown_67[0x6b - 0x67];
};

struct Event_0049d1e0 {
    char unknown_0[0xd];
    Vec3 pos;                          // +0xd
    char unknown_19;
    unsigned char flag : 1;            // +0x1a
    char unknown_1b[0x1f - 0x1b];
    short ownerId;                     // +0x1f
};
#pragma pack(pop)

static inline int SamePos(Vec3& a, Vec3& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// FUNCTION: 0x49d1e0
Projectile_0049d1e0* __stdcall FindRemoteProjectile(Event_0049d1e0* ev)
{
    if (!ev->flag)
        return 0;
    char me = g_game->localPlayer;
    Projectile_0049d1e0* proj = (Projectile_0049d1e0*)g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (proj->player != me && SamePos(proj->pos, ev->pos) && proj->owner->id == ev->ownerId)
            return proj;
    }
    return 0;
}

#pragma pack(push, 1)
struct Proj_0049d270 {                // 0x6b bytes
    Type_0049d270* type;              // +0
    char unknown_4[0x1c - 0x4];
    Vec3 pos;                         // +0x1c
    Vec3 aim;                         // +0x28
    char unknown_34[0x36 - 0x34];
    unsigned short f_36;              // +0x36
    char unknown_38[0x3a - 0x38];
    int f_3a;                         // +0x3a
    char unknown_3e[0x4e - 0x3e];
    int f_4e;                         // +0x4e
    Unit* owner;                      // +0x52
    char unknown_56[0x66 - 0x56];
    char player;                      // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;             // +0x69
};

struct Event_0049d270 {
    char unknown_0[0xd];
    Vec3 pos;                         // +0xd
    unsigned char team;               // +0x19
    unsigned char b0 : 1;             // +0x1a
    unsigned char unknown_1a : 7;
    unsigned short f_1b;              // +0x1b
    unsigned short f_1d;              // +0x1d
    unsigned short ownerId;           // +0x1f
    unsigned short unitId;            // +0x21
    unsigned char entryIndex;         // +0x23
};
#pragma pack(pop)

static inline int SamePos_0049d270(Vec3& a, Vec3& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// The projectile scan of 0x49d1e0, defined here unannotated so that /Ob2
// inlines it into ApplyWeaponFirePacket as it did in the original. It has no
// callers in the exe. Written with the positive `if (ev->b0) { ... } return 0;`
// test, which also matches 0x49d1e0 on its own; the `if (!ev->b0) return 0;`
// spelling above compiles to the same standalone bytes but, inlined here,
// gives the cursor a register and spills `unit` (73.4%).
Proj_0049d270* __stdcall FindRemoteProjectile_0049d270(Event_0049d270* ev)
{
    if (ev->b0) {
        char me = g_game->localPlayer;
        Proj_0049d270* proj = (Proj_0049d270*)g_game->projectiles;
        for (int i = 0; i < g_game->projCount; i++, proj++) {
            if (proj->player != me && SamePos_0049d270(proj->aim, ev->pos) && proj->owner->f_a8u == ev->ownerId)
                return proj;
        }
    }
    return 0;
}

void __stdcall InitProjectile(void*, void*, void*, int, int, void*);
void __stdcall FireLineOfSightProjectile(void*, void*, void*, void*, void*);
void __stdcall FireVLaunchProjectile(void*, void*, void*, void*, void*, void*);
void __stdcall FireBallisticProjectile(void*, void*, void*, void*, void*);

// FUNCTION: 0x49d270
void __stdcall ApplyWeaponFirePacket(int arg1, Event_0049d270* ev)
{
    unsigned char team = ev->team;
    Def_0049d270* def = &g_game->defs[team];
    if (def->flags.b5) {
        Vec3* pos = &ev->pos;
        void* arg = (void*)((char*)ev + 1);
        Proj_0049d270* proj = 0;
        if (g_game->projCount < 300) {
            proj = &((Proj_0049d270*)g_game->projectiles)[g_game->projCount++];
            proj->flags &= 0xfffd;
            proj->f_4e = 0;
        }
        if (!proj)
            return;
        InitProjectile(proj, def, arg, 0, g_game->teamColor, 0);
        proj->pos = *pos;
        return;
    }
    Unit* unit = ev->unitId == 0 ? 0 : &g_game->units[ev->unitId];
    if (!unit)
        return;
    if (!unit->b28)
        return;
    Entry_0049d270* entry = &unit->entry[ev->entryIndex];
    entry->f_18 = ev->f_1d;
    entry->f_16 = ev->f_1b;
    unsigned short ownerId = ev->ownerId;
    Unit* owner = ownerId == 0 ? 0 : &g_game->units[ownerId];
    Proj_0049d270* found = FindRemoteProjectile_0049d270(ev);
    if (def->flags.b1) {
        FireBallisticProjectile(entry, unit, (void*)((char*)ev + 1), &ev->pos, owner);
        return;
    }
    if (def->flags.b4) {
        FireVLaunchProjectile(entry, unit, (void*)((char*)ev + 1), &ev->pos, owner, found);
        return;
    }
    if (def->flags.b0 || def->flags.b20) {
        FireLineOfSightProjectile(entry, unit, (void*)((char*)ev + 1), &ev->pos, owner);
        return;
    }
    if (def->flags.b8) {
        Proj_0049d270* proj = 0;
        if (g_game->projCount < 300) {
            proj = &((Proj_0049d270*)g_game->projectiles)[g_game->projCount++];
            proj->flags &= 0xfffd;
            proj->f_4e = 0;
        }
        if (!proj)
            return;
        InitProjectile(proj, entry->type, (void*)((char*)ev + 1), 0, g_game->teamColor, unit);
        proj->f_36 = unit->f_66;
        proj->f_3a = 0;
        proj->pos.y = 0;
        proj->pos.x = -FUN_004b70ef(proj->f_36, unit->def->param);
        proj->pos.z = -FUN_004b7123(proj->f_36, unit->def->param);
        return;
    }
}

#pragma pack(push, 1)
struct Packet_0049d580 {
    unsigned char type;               // +0x00
    Vec3 a;                           // +0x01, the gun's world position
    Vec3 b;                           // +0x0d, the aimed at point
    unsigned char team;               // +0x19
    unsigned char unknown_1a;         // +0x1a, never assigned
    short heading;                    // +0x1b
    short pitch;                      // +0x1d
    short target_a8;                  // +0x1f
    short unit_a8;                    // +0x21
    unsigned char weapon;             // +0x23
};
#pragma pack(pop)

void __stdcall GetAimFromPosition(Unit* obj, Vec3* out, unsigned char weapon);
void __stdcall GetWeaponPiecePosition(Unit* obj, Vec3* out, unsigned char weapon, int piece);
short __stdcall SolveLaunchAngle(int dx, int dy, int dz, int speed, float pitch);
// The weapon parameter must stay an unsigned char.
int __stdcall CalcAimAngles(Unit* unit, Weapon_0049d580* target, short* out_heading,
                           short* out_pitch, unsigned char weapon, Vec3* point);
int __stdcall AimWithinTolerance(Unit* unit, Unit* aim, short angle1, short angle2);
int __stdcall FireLineOfSightProjectile(Unit* fire, Unit* unit, Vec3* p3,
                           Vec3* point, Unit* target);
int __stdcall FireBallisticProjectile(Unit* shot, Unit* unit, Vec3* pos,
                           Vec3* aim, Unit* target);
int __stdcall RandomInt(int range);
int __stdcall BroadcastPacket(int player, void* data, int size);

// FUNCTION: 0x49d580
int __stdcall FireTurretWeapon(Unit* fire, Unit* unit,
                           Unit* target, Vec3* point)
{
    if ((unit->f_1b & 1) && unit->f_8) {
        Weapon_0049d580* def = unit->f_c;
        short heading;
        short pitch;
        int ok;
        if (def->flags.b.f1) {
            Vec3 p;
            GetAimFromPosition(fire, &p, unit->f_1b >> 2 & 3);
            int dx = p.x - point->x;
            int dy = p.y - point->y;
            int dz = p.z - point->z;
            heading = FUN_004b715a(dx, dz) - fire->f_66;
            pitch = SolveLaunchAngle(dx, dy, dz, def->speed, def->pitch);
            ok = (unsigned short)pitch != 0x8000;
        } else if (def->flags.b.f0) {
            ok = CalcAimAngles(fire, def, (short*)&heading, (short*)&pitch,
                              unit->f_1b >> 2 & 3, point);
        } else {
            ok = 0;
        }
        if (!ok) {
            unit->f_1b &= 0xfe;
            fire->f_bb.f.b12 = 1;
            return 0;
        }
        if (!AimWithinTolerance(fire, unit, heading, pitch)) {
            unit->f_1b &= 0xfe;
            return 0;
        }
        Vec3 gunpos;
        GetWeaponPiecePosition(fire, &gunpos, unit->f_1b >> 2 & 3, -1);
        unit->f_16 += fire->f_66;
        // Re-read unit->f_c here and in the tail instead of using def.
        short spread = unit->f_c->f_104 - (short)((fire->f_108 << 11) / fire->f_92->divisor) + 0x800;
        int parts = fire->f_b8 / 12;
        if (parts > 1)
            spread = (unsigned short)spread / parts;
        if (spread) {
            // Keep `range >> 1` inline, no `half` local.
            unsigned short range = spread;
            unit->f_16 += (short)(RandomInt(range) - (range >> 1));
            unit->f_18 += (short)(RandomInt(range) - (range >> 1));
        }
        int fired = 0;
        if ((unit->f_c->flags.value & 1) || (unit->f_c->flags.value & 0x100000))
            fired = FireLineOfSightProjectile(unit, fire, &gunpos, point, target);
        else if (unit->f_c->flags.b.f1)
            fired = FireBallisticProjectile(unit, fire, &gunpos, point, target);
        if (!fired)
            return 0;
        unit->f_8 = 0;
        unit->f_1b &= 0xfe;
        if (g_game->flags & 1) {
            Packet_0049d580 msg;
            msg.type = 0xd;
            msg.a = gunpos;
            msg.b = *point;
            msg.team = unit->f_c->team;
            msg.weapon = unit->f_1b >> 2 & 3;
            if (fire == 0)
                msg.unit_a8 = 0;
            else
                msg.unit_a8 = fire->f_a8;
            msg.target_a8 = target ? target->f_a8 : 0;
            msg.heading = unit->f_16;
            msg.pitch = unit->f_18;
            msg.unknown_1a = msg.unknown_1a ^ ((unit->f_c->flags.value >> 30 ^ msg.unknown_1a) & 1);
            BroadcastPacket(fire->player->id, &msg, 0x24);
        }
        return 1;
    }
    return 0;
}

// SUSPECTED ORIGINAL BUG (kept from an earlier pass, still worth reporting):
// the ballistic path (weapon flag bit 1) stores its heading into argument one's
// home slot, the shooter's pointer, and that slot is what the call to AimWithinTolerance
// at 0x49d681 then reads as its angle1 (`mov edx, dword ptr [esp + 0x58]`). The edi
// register still holds the real shooter pointer, so the first angle checked is the
// low 16 bits of a pointer. Worse, the third argument, the target unit, is read back
// at 0x49d745 from that same clobbered slot, so on this path the `target->f_a8` at
// 0x49d812 and the target handed to FireLineOfSightProjectile at 0x49d77d can both be a pointer's
// low half. `test al, 1` at 0x49d58e means the path is only taken when f_1b bit 0 is
// set, so this is the laser (flags bit 1) aiming path.

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

// True when both of the aim object's angles are within tolerance of the
// corresponding unit angles. With no tolerance in the type, a moved unit
// (flags 0xc) gets a generous one, a still unit a tighter one.
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
        // if/else re-reading the field, not a ternary: it swaps the two tolerances.
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

// FUNCTION: 0x49d910
int __stdcall CalcAimAngles(Unit* unit, Unit* target, short* out_heading, short* out_pitch, int weapon, Vec3* point)
{
    Vec3 p;
    GetAimFromPosition(unit, &p, weapon);
    int dx = p.x - point->x;
    // Read through the 16.16 union and initialised before the heading: loads only the high word.
    Fixed dy;
    dy.value = p.y - point->y;
    int dz = p.z - point->z;
    *out_heading = (short)(FUN_004b715a(dx, dz) - unit->heading);
    *out_pitch = (short)FUN_004b715a(-dy.whole, (short)((int)_hypot((double)dx, (double)dz) >> 16));
    return 1;
}

#pragma pack(push, 1)
struct Type_0049d9c0 {
    char unknown_0[0x10a];
    unsigned char team;               // +0x10a
    char unknown_10b[0x111 - 0x10b];
    unsigned int flags;               // +0x111
};

struct Aim_0049d9c0 {
    char unknown_0[0xc];
    Type_0049d9c0* type;              // +0xc
    char unknown_10[0x16 - 0x10];
    short field_16;                   // +0x16
    short field_18;                   // +0x18
    char unknown_1a;
    unsigned char weapon;             // +0x1b
};

struct Packet_0049d9c0 {
    unsigned char type;               // +0x00
    Vec3 a;                           // +0x01
    Vec3 b;                           // +0x0d
    unsigned char team;               // +0x19
    unsigned char unknown_1a;         // +0x1a
    short heading;                    // +0x1b
    short pitch;                      // +0x1d
    short target_a8;                  // +0x1f
    short unit_a8;                    // +0x21
    unsigned char weapon;             // +0x23
};
#pragma pack(pop)

int __stdcall AimWithinTolerance(Unit* unit, Aim_0049d9c0* aim, short angle1, short angle2);
int __stdcall FireLineOfSightProjectile(Aim_0049d9c0* aim, Unit* unit, Vec3* aimPos,
                           Vec3* point, Unit* target);

// FUNCTION: 0x49d9c0
int __stdcall FireLineOfSightWeapon(Unit* unit, Aim_0049d9c0* aim,
                           Unit* target, Vec3* point)
{
    Vec3 p;
    GetWeaponPiecePosition(unit, &p, aim->weapon >> 2 & 3, -1);
    int dx = p.x - point->x;
    Fixed dy;
    dy.value = p.y - point->y;
    int dz = p.z - point->z;
    aim->field_16 = (short)FUN_004b715a(dx, dz);
    aim->field_18 = (short)FUN_004b715a(-dy.whole,
                                       (short)((int)_hypot((double)dx, (double)dz) >> 16));
    if (AimWithinTolerance(unit, aim, unit->heading, unit->pitch)) {
        if (FireLineOfSightProjectile(aim, unit, &p, point, target)) {
            if (g_game->flags & 1) {
                Packet_0049d9c0 msg;
                msg.type = 0xd;
                msg.a = unit->pos;
                msg.b = *point;
                msg.team = aim->type->team;
                msg.weapon = aim->weapon >> 2 & 3;
                // if/else testing the unit, not a ternary: the ternary flips the branches.
                if (unit == 0)
                    msg.unit_a8 = 0;
                else
                    msg.unit_a8 = unit->field_a8;
                msg.target_a8 = target ? target->field_a8 : 0;
                msg.heading = aim->field_16;
                msg.pitch = aim->field_18;
                msg.unknown_1a = msg.unknown_1a ^ ((aim->type->flags >> 30 ^ msg.unknown_1a) & 1);
                BroadcastPacket(unit->player->id, &msg, 0x24);
            }
            return 1;
        }
    }
    return 0;
}

#pragma pack(push, 1)
struct Flags_0049db70 {
    unsigned int unknown_0 : 30;
    unsigned int special : 1;
    unsigned int unknown_31 : 1;
};

struct Def_0049db70 {
    char unknown_0[0x10a];
    unsigned char field_10a;
    char unknown_10b[0x111 - 0x10b];
    Flags_0049db70 flags;
};

struct Shot_0049db70 {
    char unknown_0[8];
    int piece;
    Def_0049db70* def;
    char unknown_10[6];
    short heading;
    short pitch;
    char unknown_1a;
    unsigned char weapon;
};

struct Kind_0049db70 {
    char unknown_0[4];
    int player;
};

struct Object_0049db70 {
    char unknown_0[0x96];
    Kind_0049db70* kind;
    char unknown_9a[0xa8 - 0x9a];
    short team;
};

struct Packet_0049db70 {
    unsigned char type;
    Vec3 pos;
    Vec3 aim;
    unsigned char field_19;
    unsigned char flag : 1;
    short heading;
    short pitch;
    short target_team;
    short source_team;
    unsigned char weapon;
};
#pragma pack(pop)

void __stdcall GetWeaponPiecePosition(Object_0049db70* obj, Vec3* out, unsigned char weapon, int piece);
// The weapon index must stay an unsigned char: it makes the shift use byte registers.
int* __stdcall FindTargetableProjectile(Object_0049db70* obj, unsigned char weapon);
int __stdcall FireVLaunchProjectile(Shot_0049db70* shot, Object_0049db70* source, Vec3* pos,
                           Vec3* aim, Object_0049db70* target, int* param_6);
double __cdecl _hypot(double x, double y);
long __cdecl _ftol();

// FUNCTION: 0x49db70
int __stdcall FireVLaunchWeapon(Object_0049db70* source, Shot_0049db70* shot,
                           Object_0049db70* target, Vec3* aim)
{
    Vec3 pos;
    if (shot->piece != 0) {
        GetWeaponPiecePosition(source, &pos, (shot->weapon >> 2) & 3, -1);
        int dx = pos.x - aim->x;
        // dy must be a 4-byte union read by its high half, not a plain int.
        union { int value; short halves[2]; } dy;
        dy.value = pos.y - aim->y;
        int dz = pos.z - aim->z;
        shot->heading = FUN_004b715a(dx, dz);
        int dist = (int)_hypot((double)dx, (double)dz);
        shot->pitch = FUN_004b715a(-(int)dy.halves[1], (short)(dist >> 16));
        // Uninitialised here and zeroed only in the else arm.
        int* p;
        if (shot->def->flags.special) {
            p = FindTargetableProjectile(source, (shot->weapon >> 2) & 3);
            if (!p)
                return 0;
        } else {
            p = 0;
        }
        if (FireVLaunchProjectile(shot, source, &pos, aim, target, p)) {
            if (g_game->flags_2a44 & 1) {
                Packet_0049db70 packet;
                packet.type = 0xd;
                packet.pos = pos;
                packet.aim = *aim;
                packet.field_19 = shot->def->field_10a;
                packet.weapon = (shot->weapon >> 2) & 3;
                // Keep the `== 0 ? 0 : team` ternaries: they put the zero store on the fall-through.
                packet.source_team = source == 0 ? 0 : source->team;
                packet.target_team = target == 0 ? 0 : target->team;
                packet.heading = shot->heading;
                packet.pitch = shot->pitch;
                packet.flag = shot->def->flags.special;
                BroadcastPacket(source->kind->player, &packet, 0x24);
            }
            shot->piece = 0;
            shot->weapon &= 0xfe;
            return 1;
        }
    }
    return 0;
}

#pragma pack(push, 1)
struct Type_0049dd60 {
    char unknown_0[0x10a];
    unsigned char team;
    char unknown_10b[0x111 - 0x10b];
    unsigned int flags;
};

struct Aim_0049dd60 {
    char unknown_0[0xc];
    Type_0049dd60* type;
    char unknown_10[0x16 - 0x10];
    short field_16;
    short field_18;
    char unknown_1a;
    unsigned char weapon;
};

struct Projectile_0049dd60 {
    char unknown_0[0x1c];
    int field_1c;
    int field_20;
    int field_24;
    char unknown_28[0x36 - 0x28];
    short field_36;
    char unknown_38[0x3a - 0x38];
    int field_3a;
    char unknown_3e[0x4e - 0x3e];
    int field_4e;
    char unknown_52[0x69 - 0x52];
    unsigned short flags;
};

struct Packet_0049dd60 {
    unsigned char type;
    Vec3 a;
    Vec3 b;
    unsigned char team;
    unsigned char unknown_1a;
    short heading;
    short pitch;
    short target_a8;
    short unit_a8;
    unsigned char weapon;
};
#pragma pack(pop)

void __stdcall InitProjectile(Projectile_0049dd60*, Type_0049dd60*, Vec3*, int, int, Unit*);

// FUNCTION: 0x49dd60
int __stdcall FireDroppedWeapon(Unit* unit, Aim_0049dd60* aim,
                           Unit* target, Vec3* point)
{
    Vec3 p;
    GetWeaponPiecePosition(unit, &p, aim->weapon >> 2 & 3, -1);
    Projectile_0049dd60* projectile = 0;
    if (g_game->projectile_count < 300) {
        projectile = &((Projectile_0049dd60*)g_game->projectiles)[g_game->projectile_count++];
        projectile->flags &= ~2;
        projectile->field_4e = 0;
    }
    if (projectile != 0) {
        InitProjectile(projectile, aim->type, &p, 0, g_game->field_38a47, unit);
        projectile->field_36 = unit->heading;
        projectile->field_3a = 0;
        projectile->field_20 = 0;
        projectile->field_1c = -FUN_004b70ef(projectile->field_36, unit->owner->id);
        projectile->field_24 = -FUN_004b7123(projectile->field_36, unit->owner->id);
        if (g_game->flags & 1) {
            Packet_0049dd60 packet;
            packet.type = 0xd;
            packet.a = p;
            packet.b = *point;
            packet.team = aim->type->team;
            packet.weapon = aim->weapon >> 2 & 3;
            if (unit == 0)
                packet.unit_a8 = 0;
            else
                packet.unit_a8 = unit->field_a8;
            if (target == 0)
                packet.target_a8 = 0;
            else
                packet.target_a8 = target->field_a8;
            packet.heading = aim->field_16;
            packet.pitch = aim->field_18;
            packet.unknown_1a = packet.unknown_1a ^ ((aim->type->flags >> 30 ^ packet.unknown_1a) & 1);
            BroadcastPacket(unit->player->id, &packet, 0x24);
        }
        return 1;
    }
    return 0;
}

#pragma pack(push, 1)
struct Proj_0049df10 {
    char unknown_0[0x1c];
    Vec3 dir;                 // +0x1c
    char unknown_28[0x4e - 0x28];
    int field_4e;                      // +0x4e
    char unknown_52[0x69 - 0x52];
    unsigned short flags;              // +0x69
};

struct Unit_0049df10 {
    char unknown_0[0x10a];
    unsigned char team;                // +0x10a
};

struct Packet_0049df10 {
    unsigned char type;                // +0x0
    Vec3 a;                   // +0x1
    Vec3 b;                   // +0xd
    unsigned char field_19;            // +0x19
    char unknown_1a[0x24 - 0x1a];
};
#pragma pack(pop)

int __cdecl GetLocalDpid();
void __stdcall InitProjectile(Proj_0049df10*, void*, void*, int, int, void*);

// FUNCTION: 0x49df10
int __stdcall FUN_0049df10(Unit_0049df10* unit, Vec3* a, Vec3* b, int flag)
{
    Proj_0049df10* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049df10*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->field_4e = 0;
    }
    if (!proj)
        return 0;
    InitProjectile(proj, unit, a, 0, g_game->field_38a47, 0);
    proj->dir = *b;
    if (flag) {
        // Read into a local first: only then does MSVC hoist the byte load
        // above the g_game->flags_2a44 test, into the low byte of the register
        // that held `unit`, which is what leaves al free for the `lea` and
        // pushes the +0xd store and the type store in their original places.
        unsigned char team = unit->team;
        if (g_game->flags_2a44 & 1) {
            Packet_0049df10 packet;
            packet.type = 0xd;
            packet.a = *a;
            packet.b = *b;
            packet.field_19 = team;
            BroadcastPacket(GetLocalDpid(), &packet, 0x24);
        }
    }
    return 1;
}

struct Flags_0049e010 {
    unsigned int bit0 : 1;
    unsigned int unknown_1 : 3;
    unsigned int bit4 : 1;
    unsigned int unknown_5 : 3;
    unsigned int bit8 : 1;
    unsigned int unknown_9 : 10;
    unsigned int bit19 : 1;
    unsigned int bit20 : 1;
};

struct Info_0049e010;

typedef int (__stdcall* DrawFn_0049e010)(Info_0049e010* info, int a, int b, int c);

#pragma pack(push, 1)
struct Info_0049e010 {
    char unknown_0[0x60];
    DrawFn_0049e010 draw;              // +0x60
    char unknown_64[0x111 - 0x64];
    Flags_0049e010 flags;              // +0x111
};
#pragma pack(pop)

int __stdcall FireTurretWeapon(Info_0049e010* info, int a, int b, int c);
int __stdcall FireVLaunchWeapon(Info_0049e010* info, int a, int b, int c);
int __stdcall FireDroppedWeapon(Info_0049e010* info, int a, int b, int c);
int __stdcall FireLineOfSightWeapon(Info_0049e010* info, int a, int b, int c);

// Picks the routine for the object's flags.
// FUNCTION: 0x49e010
void __stdcall SetWeaponFireHandler(Info_0049e010* info)
{
    if (info->flags.bit19) {
        info->draw = FireTurretWeapon;
        return;
    }
    if (info->flags.bit4) {
        info->draw = FireVLaunchWeapon;
        return;
    }
    if (info->flags.bit0 || info->flags.bit20)
        info->draw = FireLineOfSightWeapon;
    else if (info->flags.bit8)
        info->draw = FireDroppedWeapon;
}

// The loop counter and the reload-time maximum share one 8-byte local; the
// counter sits at +0x0 and the maximum at +0x4.
struct Frame_0049e070 {
    unsigned char i;                  // +0x0
    int maxTime;                      // +0x4
};

// The weapon parameters must stay unsigned char: the loop counter slot is passed unwidened.
// FUNCTION: 0x49e070
void __stdcall InitUnitWeaponSlots(Unit* unit)
{
    Frame_0049e070 frame = {0, 0};
    for (frame.i = 0; frame.i < 3; frame.i++) {
        Slot* s = &unit->slots[frame.i];
        s->field_8 = 0;
        s->flags = (s->flags & 0xf2) | ((frame.i & 3) << 2);
        s->attached = unit->utype->attached[frame.i];
        // Read team inline with no named temporary; field_e is zeroed after this line.
        s->flags = (s->flags & 0xfd) | (((unit->utype->attached[frame.i]->team != 0) & 1 | 8) * 2);
        s->field_e = 0;
        Vec3 a;
        GetWeaponPiecePosition(unit, &a, frame.i, -1);
        Vec3 b;
        GetAimFromPosition(unit, &b, frame.i);
        s->field_4 = (a.z - b.z) * 1.25;
        if (s->attached->field_e4 > frame.maxTime)
            frame.maxTime = s->attached->field_e4;
    }
    unit->script->StartScriptWithArgs("SetMaxReloadTime", 0, 0, 1, frame.maxTime * 1000 / 30, 0, 0, 0);
}

// The three aim script names. The original's array has three elements, with
// the string "AimTertiary" right after it, so a weapon index of 3 would read
// string bytes as a pointer.
extern char* DAT_00509688[3];

int __stdcall GetWeaponTargetPos(Unit* unit, Vec3* pos, int index);
Unit* __stdcall GetWeaponTargetUnit(Unit* obj, int index);
void __stdcall FUN_0049e570(Vec3* a, Vec3* b, int* dx, int* dy, int* dz);
int __stdcall CalcAimAngles(Unit* unit, Target_0049e1a0* target,
                           unsigned short* out_heading, unsigned short* out_pitch,
                           unsigned char weapon, Vec3* point);
int __stdcall WeaponCanReachPos(Unit* unit, Vec3* a2, Vec3* a3,
                           unsigned char a4);
int __stdcall SendScriptCallByName(Unit* obj, char* name, char field_5, int field_6, int field_a,
                           unsigned short field_e, unsigned short field_12);
void __stdcall FUN_0041c150(Unit* unit);

// FUNCTION: 0x49e1a0
void __stdcall UpdateUnitWeapons(Unit* unit) {
    unsigned short heading;
    unsigned char i;
    int dz;
    int dx;
    int dy;
    Vec3 pos;
    Vec3 aim;

    for (i = 0; i < 3; i++) {
        Entry_0049e1a0* e = &unit->entries[i];
        unsigned char fl = e->flags;
        Target_0049e1a0* attached = e->attached;
        if (!(fl & 2))
            continue;
        if (e->f_14 > 0)
            e->f_14--;
        if (!GetWeaponTargetPos(unit, &pos, i)) {
            e->flags &= 0xfe;
            continue;
        }
        if (attached->f60 == 0)
            continue;
        if (attached->f_111.b19) {
            if (!(e->flags & 1)) {
                Target_0049e1a0* t = e->attached;
                unsigned short angle;
                int ok;
                if (t->f_111.b1) {
                    GetAimFromPosition(unit, &aim, (unsigned char)((e->flags >> 2) & 3));
                    FUN_0049e570(&aim, &pos, &dx, &dy, &dz);
                    heading = (unsigned short)(FUN_004b715a(dx, dz) - unit->heading);
                    angle = SolveLaunchAngle(dx, dy, dz, t->f_68, t->f_c8);
                    ok = (angle != 0x8000);
                } else if (t->f_111.b0) {
                    ok = CalcAimAngles(unit, t, &heading, &angle, (unsigned char)(e->flags >> 2 & 3),
                                      &pos);
                } else {
                    ok = 0;
                }
                if (ok) {
                    e->f_18 = angle;
                    e->f_16 = heading;
                    e->f_8 = 0;
                    // Name read afresh for each call, no cached local: lets the two tails merge.
                    unit->script->StartScriptWithArgs(DAT_00509688[(e->flags >> 2) & 3], &e->name, 0, 2,
                                               heading, angle, 0, 0);
                    SendScriptCallByName(unit, DAT_00509688[(e->flags >> 2) & 3], 2, heading, angle, 0, 0);
                    e->flags |= 1;
                }
            }
        } else {
            // Via a bool local: testing the bitfield in place shifts the register rotation.
            bool armed = attached->f_111.b4;
            if (armed && (!attached->f_111.b28 || e->f_1a) && !(e->flags & 1)) {
                e->f_8 = 0;
                unit->script->StartScriptWithArgs(DAT_00509688[(e->flags >> 2) & 3], &e->name, 0, 2, 0, 0,
                                           0, 0);
                SendScriptCallByName(unit, DAT_00509688[(e->flags >> 2) & 3], 2, 0, 0, 0, 0);
                e->flags |= 1;
            }
        }
        if (e->f_14 != 0)
            continue;
        if (WeaponCanReachPos(unit, &unit->pos, &pos, i)) {
            int can = 0;
            if (attached->f_111.b28) {
                if (e->f_1a)
                    can = 1;
            } else {
                if (unit->f_bc.store->metal >= attached->f_c0 &&
                    unit->f_bc.store->energy >= attached->f_c4)
                    can = 1;
            }
            if (can == 0)
                continue;
            Unit* fired = GetWeaponTargetUnit(unit, i);
            if (attached->f60(unit, &e->point, fired, &pos) == 0)
                continue;
            if (attached->f_111.b28) {
                e->f_1a--;
                FUN_0041c150(unit);
            } else {
                int n = unit->f_b8 / 5;
                if (n > 5)
                    n = 5;
                int q = unit->f_108 * 20 / unit->mtype->f_1fa;
                int pct = 100 - n * 6;
                e->f_14 = (short)((120 - q) * (pct * attached->f_e4 / 100) / 100);
            }
            int m = (attached->f_111.b26) ? 0x800 : 0x400;
            unit->f_ba.w |= m;
            if (!attached->f_111.b28)
                unit->f_bc.SpendEnergyAndMetal(attached->f_c0, attached->f_c4);
        } else {
            unit->f_ba.b[1] |= 0x10;
        }
    }
}

// FUNCTION: 0x49e570
void __stdcall FUN_0049e570(int* a, int* b, int* dx, int* dy, int* dz)
{
    *dx = a[0] - b[0];
    *dy = a[1] - b[1];
    *dz = a[2] - b[2];
}

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

// FUNCTION: 0x49e5b0
char* __stdcall FindWeaponByName(char* name)
{
    if (name == 0 || strlen(name) == 0)
        return 0;
    for (int i = 0; i < 0x100; i++) {
        char* entry = g_game->entries[i].name;
        if (_strcmpi(entry, name) == 0)
            return entry;
    }
    return 0;
}

#include "../util/tdf.h"

// A global object of the 12-byte class constructed by 0x4c2ea0 and destroyed
// by 0x4c2eb0. The compiler generates its initialiser (0x49e610) and the
// destructor it registers with atexit (0x49e630).
// FUNCTION: 0x49e610 _$E4
// FUNCTION: 0x49e630 _$E2
TdfFile DAT_0051f310;
