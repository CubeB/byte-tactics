// Decompiled by Opus, deepseek-v4.1-flash, GPT-5.6-Terra, claude-sonnet,
// Space Bunny Free, GPT-6.1-sol, deepseek-v4.1, space-bunny-free,
// mimo-v2.6-pro, LongCat 2.5 Preview Free, Claude Opus 5.5, Haiku,
// Claude Sonnet 5.5 and GPT-6. Names are provisional.
// The weapons module (0x499a30 to 0x49e630), Cavedog's weapons.cpp: the
// projectile array, the weapon fire and hit packets, weapon damage and area
// damage, the ballistic launch-angle solver, the weapon reach tests, the
// projectile initialiser and the four firing routines, the turret aiming path
// with its angle helpers, the remote fire packet and its projectile scan, the
// weapon fire handlers, the unit's weapon slots and the loop that updates
// them, and the weapon name table. The module's parts joined in address
// order; 0x49a120 (a gap region), 0x49b090, 0x49b720 and 0x49be60 keep their
// own files: their register plans follow their old files' symbol ids.

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

// 499a30's fixed-point position view (Unit below holds it beside Vec3).
union Fixed_0049abb0 {
    int value;                                      // 16.16 fixed point
    struct {
        unsigned short fraction;
        short whole;
    } parts;
};

struct Vec3_0049abb0 {
    int x;
    Fixed_0049abb0 y;
    int z;
};

// 499a30's views are folded in under the union's other members: its def at
// +0xc is f_c (its f10a and f111 are team and flags.value), its weapons[3]
// are slots[3], its f16/f18/f1b are f_16/f_18/f_1b, its owner is playerIndex,
// its armour is f_b8, its state is flags, its type is utype and its pos is
// pos_0049abb0.
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
            union {
                Vec3 pos;              // 49e1a0's and 49d9c0's spelling
                Vec3_0049abb0 pos_0049abb0; // 499cd0's, 49aa80's, 49abb0's and 49b000's
            };
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
    int workTime;                      // +0xb0, 49c740's
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
    unsigned char playerIndex;            // +0xff, the owner's player colour (49c740)
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
    int spawnTick;                     // +0x42
    char unknown_46[0x4a - 0x46];
    int nextSmokeTick;                 // +0x4a
    int targetUnit;                    // +0x4e
    Unit* owner;                       // +0x52, the unit that fired it
    int interceptedProjectile;         // +0x56
    char unknown_5a[0x60 - 0x5a];
    short active;                      // +0x60
    short piece;                       // +0x62, which barrel it came from
    char unknown_64[0x66 - 0x64];
    unsigned char player;              // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;              // +0x69
    // Written as an inline method the load of u->playerIndex comes before the
    // store of owner, as the original has it; in the enclosing function it
    // sinks below it.
    void SetOwner(Unit* u) { player = u->playerIndex; owner = u; }
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
// are one union, as are the two tracked-projectile pointers, 499a30's selected
// and 49c740's tracked.
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
    int gravity;                       // +0x14263
    char unknown_14267[0x1427f - 0x14267];
    unsigned char seaLevel;            // +0x1427f
    char unknown_14280;
    unsigned char viewFlags;           // +0x14281
    char unknown_14282[0x142f3 - 0x14282];
    Unit* trackedUnit;                 // +0x142f3
    union {
        Proj_0049c740* trackedProj;    // +0x142f7
        Projectile_0049c880* tracked;
        void* selected;                // 499a30's spelling
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
    char unknown_38a4b[0x391e9 - 0x38a4b];
    void* net;                         // +0x391e9
};

#pragma pack(pop)

#include <string.h>

#pragma pack(push, 1)

// 499a30's and 49b720's position view: Vec3 (see the top of the file).
typedef Vec3 Vec3_0049b720;

struct Unit;
struct Spot_0049a120;
struct FeatureDef_0049a120;
struct MapFeature_0049b090;
struct Cell_0049a120;

#pragma pack(pop)

extern Game* g_game;

void* __cdecl FUN_004d83b0(char* name, unsigned int size);

// Allocates and clears the 0x7d64-byte weapon array, then resets its count.
// FUNCTION: 0x499a30
void AllocWeaponArray(void)
{
    g_game->projectiles = FUN_004d83b0("WEAPON ARRAY", 0x7d64);
    memset(g_game->projectiles, 0, 0x7d64);
    g_game->projectileCount = 0;
}

void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x499a80
void FreeWeaponArray(void)
{
    FUN_004d85a0(g_game->projectiles);
    g_game->projectiles = 0;
}

#pragma pack(push, 1)

struct Vec3_00499ab0 {
    int x;
    int y;
    int z;
};

struct Kind_00499ab0 {
    char unknown_0[4];
    int f4;                          // +0x4, the player / "who"
};

struct Obj_00499ab0 {
    char unknown_0[0x96];
    Kind_00499ab0* kind;             // +0x96
    char unknown_9a[0xa8 - 0x9a];
    unsigned short team;             // +0xa8
};

struct Packet_00499ab0 {
    unsigned char type;              // +0x0
    Vec3_00499ab0 a;                 // +0x1
    Vec3_00499ab0 b;                 // +0xd
    unsigned char f19;               // +0x19
    unsigned char flag : 1;          // +0x1a, bit 0
    unsigned short f1b;              // +0x1b
    unsigned short f1d;              // +0x1d
    unsigned short f1f;              // +0x1f
    unsigned short f21;              // +0x21
    unsigned char f23;               // +0x23
};

#pragma pack(pop)

int __stdcall BroadcastPacket(int player, void* data, int size);

// Builds the 0x24 byte "unit status" network message (type 0xd) and hands it
// to BroadcastPacket. Only sent while g_game's bit 0 flag is set. Sibling of
// 0x499ba0, which sends the same type and size.
//
// The byte at packet +0x1a is a 1 bit bitfield that is only ever read and
// write back by the assignment at the end, so its upper seven bits are never
// initialised here; see the note in the bug list.
// FUNCTION: 0x499ab0
void __stdcall SendWeaponFirePacket(Unit* unit, Obj_00499ab0* source,
                            Obj_00499ab0* target, Vec3_00499ab0* a,
                            Vec3_00499ab0* b)
{
    Packet_00499ab0 packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.a = *a;
        packet.b = *b;
        packet.f19 = unit->f_c->team;
        packet.f23 = (unit->f_1b >> 2) & 3;
        packet.f21 = !source ? 0 : source->team;
        packet.f1f = !target ? 0 : target->team;
        packet.f1b = unit->f_16;
        packet.f1d = unit->f_18;
        packet.flag = unit->f_c->flags.value >> 30;
        BroadcastPacket(source->kind->f4, &packet, 0x24);
    }
}

struct Vec3_00499ba0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Packet_00499ba0 {
    unsigned char type;                // +0x0
    Vec3_00499ba0 a;                   // +0x1
    Vec3_00499ba0 b;                   // +0xd
    char field_19;                     // +0x19
    char unknown_1a[0x24 - 0x1a];
};
#pragma pack(pop)

int __cdecl GetLocalDpid();

// FUNCTION: 0x499ba0
void __stdcall BroadcastWeaponFire(char param_1, Vec3_00499ba0* a, Vec3_00499ba0* b)
{
    Packet_00499ba0 packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.a = *a;
        packet.b = *b;
        packet.field_19 = param_1;
        BroadcastPacket(GetLocalDpid(), &packet, 0x24);
    }
}

#pragma pack(push, 2)
struct Object_00499c10 {
    char unknown_0[0x66];
    short heading;                     // +0x66
    char unknown_68[0x9a - 0x68];
    CobScript* anims;                  // +0x9a
};
#pragma pack(pop)

struct Source_00499c10 {
    char unknown_0[0x16];
    short heading;                     // +0x16
};

// Fixed-point trig helpers written in assembly.
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);

// FUNCTION: 0x499c10
void __stdcall RockUnit(Object_00499c10* obj, Source_00499c10* src)
{
    short angle = src->heading - obj->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    obj->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
}

struct Weapon_499c70;
struct Projectile_00499eb0;
struct Weapon_0049a120;

class SquadManager {
public:
    void MarkOwnerNetDirtyFromDamageSplit(Weapon_499c70* weapon, int a, int b);
    void MarkOwnerNetDirtyFromDamageSplit(Projectile_00499eb0* projectile, int a, int b);
    void MarkOwnerNetDirtyFromDamageSplit(Weapon_0049a120* weapon, int enemyDamage, int friendlyDamage);
};

struct Player_499c70 {
    char unknown_0[0x74];
    SquadManager* field_74;            // +0x74
};

#pragma pack(push, 1)
struct Weapon_499c70 {
    char unknown_0[0x52];
    Unit* attacker;                    // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
};
#pragma pack(pop)

unsigned short __stdcall ApplyWeaponDamage(Weapon_499c70* weapon, Unit* target, float scale);

// FUNCTION: 0x499c70
void __stdcall ApplyWeaponHit(Weapon_499c70* weapon, Unit* target)
{
    unsigned short damage = ApplyWeaponDamage(weapon, target, 1.0f);
    Unit* attacker = weapon->attacker;
    if (attacker) {
        unsigned short a = 0;
        unsigned short b = 0;
        if (weapon->owner != target->playerIndex)
            a = damage;
        else
            b = damage;
        ((Player_499c70*)attacker->player)->field_74->MarkOwnerNetDirtyFromDamageSplit(weapon, a, b);
    }
}

extern "C" int __cdecl _strcmpi(const char* str1, const char* str2);

struct Pair_00499cd0 {
    char* name;                     // +0x0
    int value;                      // +0x4
};

struct NameLess_00499cd0 {
    bool operator()(const char* a, const char* b) const
    {
        return _strcmpi(a, b) < 0;
    }
};

#pragma pack(push, 1)
struct Table_00499cd0 {
    char unknown_0[5];
    Pair_00499cd0* first;           // +0x5
    Pair_00499cd0* last;            // +0x9
};

struct Def_00499cd0 {
    char unknown_0[0x64];
    Table_00499cd0* table;          // +0x64
    char unknown_68[0xd4 - 0x68];
    unsigned short field_d4;        // +0xd4
    char unknown_d6[0x111 - 0xd6];
    unsigned int flags;             // +0x111
};

struct UnitDef_00499cd0 {
    char unknown_0[0x20];
    char name[1];                   // +0x20
};

struct Weapon_00499cd0 {
    Def_00499cd0* def;              // +0x0
    int x;                          // +0x4
    int y;                          // +0x8
    int z;                          // +0xc
    char unknown_10[0x52 - 0x10];
    Unit* attacker;                 // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;            // +0x66
};

struct Flags_00499cd0 {
    unsigned short bits0_6 : 7;
    unsigned short flag7 : 1;       // bit 7, 0x80
    unsigned short flag8 : 1;       // bit 8, 0x100
    unsigned short rest : 7;
};
#pragma pack(pop)

short __cdecl FUN_004b715a(int x, int z);
void __stdcall DamageUnit(Unit* source, Unit* target,
                            int amount, int type, unsigned short extra);

static inline int* Find_00499cd0(Table_00499cd0* table, char* name)
{
    NameLess_00499cd0 less;
    Pair_00499cd0* first = table->first;
    Pair_00499cd0* last = table->last;
    while (first != last) {
        Pair_00499cd0* mid = first + (last - first) / 2;
        if (less(mid->name, name))
            first = mid + 1;
        else
            last = mid;
    }
    if (first == table->last || less(name, first->name))
        return 0;
    return &first->value;
}

// Damage a weapon does to a target. The weapon's def holds a base damage
// (+0xd4) and a name-keyed damage table (+0x64); the target's unit type name
// is looked up in it (the same sorted (name, value) array and lower_bound as
// 0x4c4630). The result is scaled, boosted by the attacker's armour
// (6% per point, capped at 5 points), doubled or halved by two global flags,
// and handed to DamageUnit together with the angle from the weapon to the
// target minus the target's heading.
// FUNCTION: 0x499cd0
int __stdcall ApplyWeaponDamage(Weapon_00499cd0* weapon, Unit* target,
                           float scale)
{
    Def_00499cd0* def = weapon->def;
    int damage = def->field_d4;
    Table_00499cd0* table = def->table;
    if (table) {
        int* p = Find_00499cd0(table, ((UnitDef_00499cd0*)target->utype)->name);
        if (p)
            damage = *p;
    }
    damage = (int)(damage * scale);
    short angle = FUN_004b715a(weapon->x - target->pos_0049abb0.x,
                               weapon->z - target->pos_0049abb0.z) - target->heading;
    Unit* attacker = weapon->attacker;
    if (attacker) {
        int armour = attacker->f_b8 / 5;
        if (armour > 5)
            armour = 5;
        damage = (armour * 6 + 100) * damage / 100;
    }
    Flags_00499cd0* flags = (Flags_00499cd0*)((char*)g_game + 0x37f2f);
    if (flags->flag7)
        damage *= 2;
    if (flags->flag8)
        damage /= 2;
    bool veteran = (weapon->def->flags >> 7) & 1;
    int type = veteran ? 2 : 1;
    DamageUnit(attacker, target, damage, type, angle);
    return damage;
}

struct UnitType_499e50 {
    char unknown_0[0xfe];
    short value;                 // +0xfe
};

struct Unit_499e50 {
    UnitType_499e50* type;       // +0
    Vec3_0049b720 pos;           // +4
    char unknown_10[0x69 - 0x10];
    unsigned char flags;         // +0x69
};

// FUNCTION: 0x499e50
void __stdcall UntrackProjectile(Unit_499e50* unit)
{
    if (unit == g_game->selected) {
        g_game->trackedPos = ((Unit_499e50*)g_game->selected)->pos;
        g_game->trackedValue = unit->type->value;
        g_game->selected = 0;
    }
    unit->flags |= 2;
}

#pragma pack(push, 1)

struct ProjectileType_00499eb0 {
    char unknown_0[0x78];
    void* field_78;
    void* field_7c;
    char unknown_80[0xcc - 0x80];
    int field_cc;
    int field_d0;
    char unknown_d4[0xd6 - 0xd4];
    unsigned short field_d6;
    char unknown_d8[0xf6 - 0xd8];
    unsigned short sound1;
    unsigned short sound2;
    char unknown_fa[0x111 - 0xfa];
    // Bitfield struct: the bit tests need this form.
    struct {
        unsigned int bits0_9 : 10;
        unsigned int bit10 : 1;
        unsigned int bits11_21 : 11;
        unsigned int bit22 : 1;
        unsigned int bits23_31 : 9;
    } flags;
};

struct Projectile_00499eb0 {
    ProjectileType_00499eb0* type;
    Vec3_0049b720 position;
    char unknown_10[0x52 - 0x10];
    Unit* unit;
    char unknown_56[0x66 - 0x56];
    unsigned char owner;
    char unknown_67[2];
    // Stays unsigned short: gives the single byte OR with 2.
    unsigned short flags;
};

struct Net_00499eb0 {
    char unknown_0[0xd48];
    int field_d48;
};

struct Player_00499eb0 {
    int field_0;
    char unknown_4[0x73 - 4];
    unsigned char state;
    char unknown_74[0x14b - 0x74];
};

struct Holder_00499eb0 {
    char unknown_0[0x74];
    SquadManager* object;
};

#pragma pack(pop)

void* __stdcall GetMapCellAtPosition(Vec3_0049b720* position);
void __stdcall AccumulateScreenShake(int a, int b, int c);void __stdcall AddExplosionEffect(Vec3_0049b720* position, void* value, int a, int b);
void __stdcall EmitWhiteSmoke(Vec3_0049b720* position, int value);
// The sound id stays unsigned int: gives the original's zero extension.
void __stdcall PlaySoundAt(unsigned int sound, Vec3_0049b720* position, int value);
int __stdcall ApplyWeaponDamage(Projectile_00499eb0* projectile, Unit* unit, float scale);
void __stdcall ApplyAreaDamage(Projectile_00499eb0* projectile, Vec3_0049b720* position);

// FUNCTION: 0x499eb0
void __stdcall DetonateProjectile(Projectile_00499eb0* projectile, Unit* unit)
{
    int hostile = 0;
    ProjectileType_00499eb0* type = projectile->type;
    Vec3_0049b720* position = &projectile->position;
    unsigned char* value = (unsigned char*)GetMapCellAtPosition(position);
    if (value != 0)
        hostile = value[5] < g_game->seaLevel;
    if (!type->flags.bit22) {
        if (projectile == g_game->selected) {
            g_game->trackedPos = ((Projectile_00499eb0*)g_game->selected)->position;
            g_game->trackedValue = *(unsigned short*)((char*)projectile->type + 0xfe);
            g_game->selected = 0;
        }
        projectile->flags = projectile->flags | 2;
    }
    if (((Net_00499eb0*)g_game->net)->field_d48 && hostile && !unit) {
        if (projectile == g_game->selected) {
            g_game->trackedPos = ((Projectile_00499eb0*)g_game->selected)->position;
            g_game->trackedValue = *(unsigned short*)((char*)projectile->type + 0xfe);
            g_game->selected = 0;
        }
        projectile->flags = projectile->flags | 2;
        return;
    }
    AccumulateScreenShake(type->field_cc, type->field_cc, type->field_d0);
    if (hostile && !unit) {
        PlaySoundAt(type->sound2, position, 0);
        AddExplosionEffect(position, type->field_7c, 0, hostile);
    } else {
        PlaySoundAt(type->sound1, position, 0);
        if (type->flags.bit10)
            EmitWhiteSmoke(position, 9);
        else
            AddExplosionEffect(position, type->field_78, 0, hostile);
    }
    unsigned int player = projectile->owner;
    Player_00499eb0* record = (Player_00499eb0*)((char*)g_game + player * 0x14b + 0x1b63);
    if (!record->field_0 || record->state != 3) {
        if (type->field_d6 <= 0x10 && unit) {
            int damage = ApplyWeaponDamage(projectile, unit, 1.0f);
            Unit* source = projectile->unit;
            if (source) {
                int a = 0;
                int b = 0;
                if (projectile->owner != unit->playerIndex)
                    a = damage;
                else
                    b = damage;
                ((Holder_00499eb0*)source->player)->object->MarkOwnerNetDirtyFromDamageSplit(projectile, a & 0xffff, b & 0xffff);
                return;
            }
        } else {
            ApplyAreaDamage(projectile, position);
        }
    }
}

// Builds a zeroed 0x6b-byte parameter block (owner, position, kind 10) and
// hands it to ApplyAreaDamage (an ebp-framed __stdcall routine in a gap).

struct Vec3_0049a0c0 {
    int x;
    int y;
    int z;
};

#pragma pack(push, 1)
struct Params_0049a0c0 {
    void* owner;                       // +0x0
    Vec3_0049a0c0 pos;                 // +0x4
    char unknown_10[0x42];
    int ownerUnit;                     // +0x52
    char unknown_56[0x10];
    char kind;                         // +0x66
    char unknown_67[4];
};
#pragma pack(pop)

void __stdcall ApplyAreaDamage(Params_0049a0c0* params, Vec3_0049a0c0* pos);

// FUNCTION: 0x49a0c0
void __stdcall ApplyAreaDamageAt(void* owner, Vec3_0049a0c0* pos)
{
    Params_0049a0c0 p;
    memset(&p, 0, sizeof(p));
    p.ownerUnit = 0;
    p.owner = owner;
    p.kind = 10;
    p.pos = *pos;
    ApplyAreaDamage(&p, pos);
}

// 0x49a120's views. The function itself is a gap region (one of
// data/functions.csv's 29 gap rows) and stays in weapons_49a120.cpp; its
// declarations stay here for the symbol ids the functions after it match at.
#include <math.h>

struct Vec3_0049a120 {
    int x;                             // +0x0 (16.16)
    int y;                             // +0x4
    int z;                             // +0x8
};

// A 16.16 fixed-point value and its whole part.
union Fixed_0049a120 {
    int raw;
    struct {
        unsigned short frac;
        short whole;
    } part;
};

struct FixedVec3_0049a120 {
    Fixed_0049a120 x;
    Fixed_0049a120 y;
    Fixed_0049a120 z;
};

struct CellPos_0049a120 {
    short x;
    short z;
};

#pragma pack(push, 1)
struct WeaponDef_0049a120 {
    char unknown_0[0xd6];
    unsigned short radius;             // +0xd6
    float edgeDamage;                  // +0xd8
    char unknown_dc[0x10a - 0xdc];
    unsigned char kind;                // +0x10a
    char unknown_10b[0x111 - 0x10b];
    union {
        unsigned int all;
        struct {
            unsigned int bits0_29 : 30;
            unsigned int detonatesWeapons : 1;
            unsigned int bit31 : 1;
        } bits;
    } flags;                           // +0x111
};

struct UnitDef_0049a120 {
    char unknown_0[0x15e];
    Vec3_0049a120 boxMin;              // +0x15e
    Vec3_0049a120 boxMax;              // +0x16a
};

struct Holder_0049a120;

struct Unit_0049a120 {
    char unknown_0[0x6a];
    Vec3_0049a120 pos;                 // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_0049a120* def;             // +0x92
    Holder_0049a120* holder;           // +0x96
    char unknown_9a[0xff - 0x9a];
    unsigned char owner;               // +0xff
    char unknown_100[0x118 - 0x100];
};

struct Weapon_0049a120 {
    WeaponDef_0049a120* def;           // +0x0
    Vec3_0049a120 pos;                 // +0x4
    char unknown_10[0x28 - 0x10];
    Vec3_0049a120 aim;                 // +0x28
    char unknown_34[0x52 - 0x34];
    Unit_0049a120* attacker;           // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
    char unknown_67[0x69 - 0x67];
    unsigned short flags;              // +0x69
};

struct Holder_0049a120 {
    char unknown_0[4];
    int playerId;                      // +0x4
    char unknown_8[0x74 - 8];
    SquadManager* object;              // +0x74
};

struct Cell_0049a120 {
    unsigned short unit;               // +0x0
    unsigned short unit2;              // +0x2
    char unknown_4[4];
    unsigned short feature;            // +0x8
    union {
        unsigned short spot;           // +0xa
        struct {
            unsigned char offsetZ;     // +0xa
            unsigned char offsetX;     // +0xb
        } origin;
    };
    unsigned char flags;               // +0xc
};

struct Spot_0049a120 {
    char unknown_0[8];
    Vec3_0049a120 pos;                 // +0x8
    char unknown_14[0x30 - 0x14];
};

struct FeatureDef_0049a120 {
    char unknown_0[0x100];
};

struct Packet_0049a120 {
    unsigned char type;
    Vec3_0049a120 pos;
    unsigned char kind;
};
#pragma pack(pop)

// <vector> comes after the game's own declarations: the symbol ids it gives
// cell, feature and g_game decide the spot and feature address arithmetic of
// the reach tests. The 0x49a120 gap file keeps the lean windows.h its own
// function was matched with.
#include <vector>

// What one explosion has already hit, so nothing takes damage twice.
struct Hits_0049a120 {
    int numUnits;
    int numFeatures;
    Unit_0049a120* units[20];
    Cell_0049a120* features[64];

    // Each returns 0 if the thing was hit already, else records it (while
    // there is room) and returns 1.
    int AddUnit(Unit_0049a120* unit)
    {
        for (int k = 0; k < numUnits; k++) {
            if (units[k] == unit)
                return 0;
        }
        if (numUnits < 20) {
            units[numUnits] = unit;
            numUnits++;
        }
        return 1;
    }
    int AddFeature(Cell_0049a120* cell)
    {
        for (int k = 0; k < numFeatures; k++) {
            if (features[k] == cell)
                return 0;
        }
        if (numFeatures < 64) {
            features[numFeatures] = cell;
            numFeatures++;
        }
        return 1;
    }
};

Cell_0049a120* __stdcall GetMapCell(int x, int y);
int __stdcall ApplyWeaponDamage(Weapon_0049a120* weapon, Unit_0049a120* target, float scale);
void __stdcall DetonateProjectile(Weapon_0049a120* weapon, Unit_0049a120* unit);
int __stdcall VectorLength(Vec3_0049a120* v);
Vec3_0049a120 __stdcall GetFootprintCentre(CellPos_0049a120* cell, FeatureDef_0049a120* def);
void __stdcall DamageFeature(Cell_0049a120* cell, int x, int z, WeaponDef_0049a120* def);
int __stdcall BroadcastPacket(int id, void* data, int size);

static inline int Length(Vec3_0049a120* v)
{
    double x = v->x;
    double y = v->y;
    double z = v->z;
    return (int)sqrt(x * x + y * y + z * z);
}

static inline Unit_0049a120* UnitFromId(unsigned short id)
{
    if (id == 0)
        return 0;
    return (Unit_0049a120*)&g_game->units[id];
}

static inline Vec3_0049a120 Sub(const Vec3_0049a120& a, const Vec3_0049a120& b)
{
    Vec3_0049a120 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

// <time.h> and <malloc.h> are here for their symbol ids: they decide
// SolveLaunchAngle's x87 schedule and the reach tests' register plans.
#include <time.h>
#include <malloc.h>

// The length of an integer vector, truncated to an int.

struct Vec3i_0049a850 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

// FUNCTION: 0x49a850
int __stdcall VectorLength(Vec3i_0049a850* v)
{
    double x = v->x;
    double y = v->y;
    double z = v->z;
    return (int)sqrt(x * x + y * y + z * z);
}

// Ballistic launch-angle solver: the two roots
// of the trajectory quadratic, each turned into a launch angle with
// acos(sqrt(root) / speed), pi/2 when a root is not positive.
#include <stdio.h>

// PI stays this literal and `use / PI` a separate statement from
// `use * 32768.0`: the constant pool depends on it.
#define PI 3.14159265358979

extern "C" double __cdecl _hypot(double x, double y);

// FUNCTION: 0x49a890
short __stdcall SolveLaunchAngle(int x, int height, int z, int speed, float angle)
{
    int g = g_game->gravity;
    int gg = g * g;
    // The redundant parentheses stay, with these exact counts: they steer the
    // x87 schedule. disc is expanded as (s2 + 2*gh) * s2 + h2 * gg.
    double distance = (((_hypot(x, z))));
    double d = distance * distance;
    double gh = (double)g * (double)height;
    double h2 = (((((double)height * (double)height))));
    double s2 = (double)speed * (double)speed;
    double sum = h2 + d;
    double disc = (((((s2 + 2*gh) * s2)) + h2 * gg)) * (d * d) - (((d * d * gg)) * sum);
    if (disc < 0.0)
        return 0x8000;
    disc = sqrt(disc);
    d = (s2 + gh) * d;
    double high = (disc + d) / (2*sum);
    double low = (d - disc) / (2*sum);
    double highAngle;
    double lowAngle;
    if (high > 0.0)
        highAngle = acos(sqrt(high) / (double)speed);
    else
        highAngle = PI / 2;
    if (low > 0.0)
        lowAngle = acos(sqrt(low) / (double)speed);
    else
        lowAngle = PI / 2;
    double use;
    if (angle < highAngle && highAngle <= PI / 4)
        use = highAngle;
    else if (lowAngle > angle && lowAngle <= PI / 4)
        use = lowAngle;
    else
        return 0x8000;
    use = use * 32768.0;
    return (short)(use / PI);
}

// 0x49aa80's file included <windows.h> and <stdio.h> here for the operand
// order in the height check; both are already included above.
#pragma pack(push, 1)

union Fixed_0049aa80 {
    int value;
    struct { unsigned short fraction; short whole; } parts;
};

struct Vec3_0049aa80 {
    int x;                             // +0x0 (16.16 fixed point)
    Fixed_0049aa80 y;                  // +0x4
    int z;                             // +0x8
};

struct WeaponDef_0049aa80 {
    char unknown_0[0x68];
    int field_68;                      // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                      // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                         // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;         // tested here (line of fire)
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;        // tested here (skip the team check)
        unsigned int bit17_31 : 15;
    } flags;                           // +0x111
};

struct UnitDef_0049aa80 {
    char unknown_0[0x170];
    short field_170;                   // +0x170
};

#pragma pack(pop)

short __stdcall SolveLaunchAngle(int dx, int dy, int dz, int a, int b);

// The subtraction of two points is a real by-value operator taking both
// operands by value and returning the point by value, and the line-of-fire test
// goes through a by-value helper, both exactly as in the matching sibling
// 0x49abb0 (whose MATCH this idiom produced). Those two boundaries are what put
// a2 in esi and a3 in edi for the whole function and keep the weapon def in
// ebx, which is the first block's register allocation (see the pass note at the
// top of the file).
inline Vec3_0049aa80 operator-(Vec3_0049aa80 a, Vec3_0049aa80 b)
{
    Vec3_0049aa80 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

static inline short LineOfFire_0049aa80(Vec3_0049aa80 to, Vec3_0049aa80 from, int s, int f)
{
    Vec3_0049aa80 d = from - to;
    return SolveLaunchAngle(d.x, d.y.value, d.z, s, f);
}

// FUNCTION: 0x49aa80
int __stdcall WeaponCanReachPos(Unit* a1, Vec3_0049aa80* a2, Vec3_0049aa80* a3, int a4)
{
    WeaponDef_0049aa80* wdef = (WeaponDef_0049aa80*)a1->slots[a4 & 0xff].shot;

    // z difference first, as named int locals: the original's order.
    int dz = a3->z - a2->z;
    int dx = a3->x - a2->x;
    if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.bit16)
        return 1;

    if (a2->y.parts.whole + ((UnitDef_0049aa80*)a1->utype)->field_170 <= g_game->seaLevel)
        return 0;

    if (wdef->flags.bit1) {
        if (LineOfFire_0049aa80(*a2, *a3, wdef->field_68, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}

// Both includes change the register choice in the sea-level tests.
#include <stdlib.h>

#pragma pack(push, 1)

struct WeaponDef_0049abb0 {
    char unknown_0[0x68];
    int field_68;                                   // +0x68
    char unknown_6c[0xc8 - 0x6c];
    int field_c8;                                 // +0xc8
    char unknown_cc[0xdc - 0xcc];
    int range;                                      // +0xdc
    char unknown_e0[0x111 - 0xe0];
    struct {
        unsigned int bit0 : 1;
        unsigned int bit1 : 1;                      // line of fire
        unsigned int bit2_15 : 14;
        unsigned int bit16 : 1;                     // skip the ground test
        unsigned int bit17 : 1;                     // target must be landed
        unsigned int bit18_31 : 14;
    } flags;                                        // +0x111
};

struct UnitDef_0049abb0 {
    char unknown_0[0x170];
    short height;                                   // +0x170
    char unknown_172[0x241 - 0x172];
    struct {
        unsigned int bit0_11 : 12;
        unsigned int bit12 : 1;                     // half height counts
        unsigned int bit13_18 : 6;
        unsigned int bit19 : 1;                     // ignore sea level
        unsigned int bit20_31 : 12;
    } flags;                                        // +0x241
};

#pragma pack(pop)

// The vector from point `a` to point `b`. Both operands and the result are by
// value, which is what makes the line-of-fire block below compile the way the
// original does: MSVC builds the two 12-byte argument copies as one setup unit
// instead of turning the first aggregate into a live pointer, so `unit2` stays in
// ebx across the call and the second distance tail needs no reload. Note the
// operands are named in the order the callers pass them (shooter, target) while
// the result is target - shooter, because the copy order, not the arithmetic, is
// what the register allocation is sensitive to here.
inline Vec3_0049abb0 operator-(Vec3_0049abb0 a, Vec3_0049abb0 b)
{
    Vec3_0049abb0 r;
    r.x = b.x - a.x;
    r.y.value = b.y.value - a.y.value;
    r.z = b.z - a.z;
    return r;
}

// Can the shooter hit the target? Both positions are taken by value, which is
// what puts the 12-byte copies in the frame while the units themselves are read
// where they are. The parameters are named for the order the caller passes them
// in, and the subtraction gives the target relative to the shooter.
static inline short LineOfFire_0049abb0(Vec3_0049abb0 to, Vec3_0049abb0 from, int s, int f)
{
    Vec3_0049abb0 d = from - to;
    return SolveLaunchAngle(d.x, d.y.value, d.z, s, f);
}

static inline int Dist2_0049abb0(Vec3_0049abb0* b, Vec3_0049abb0* a)
{
    int dz = a->z - b->z;
    int dx = a->x - b->x;
    return (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32);
}

// FUNCTION: 0x49abb0
int __stdcall WeaponCanReachUnit(Unit* unit1, Unit* unit2, unsigned char weapon)
{
    WeaponDef_0049abb0* w = (WeaponDef_0049abb0*)unit1->slots[weapon].shot;

    if (w->flags.bit16) {
        if (!((UnitDef_0049abb0*)unit2->utype)->flags.bit19 && unit2->pos_0049abb0.y.parts.whole > g_game->seaLevel)
            return 0;
        // Written (height >> 1) + whole: sets the add's operand order.
        if (((UnitDef_0049abb0*)unit2->utype)->flags.bit12 && (((UnitDef_0049abb0*)unit2->utype)->height >> 1) + unit2->pos_0049abb0.y.parts.whole > g_game->seaLevel)
            return 0;
        return Dist2_0049abb0(&unit1->pos_0049abb0, &unit2->pos_0049abb0) <= w->range * w->range;
    }

    if (unit1->pos_0049abb0.y.parts.whole + ((UnitDef_0049abb0*)unit1->utype)->height <= g_game->seaLevel)
        return 0;
    if (unit2->pos_0049abb0.y.parts.whole + ((UnitDef_0049abb0*)unit2->utype)->height <= g_game->seaLevel)
        return 0;
    if (w->flags.bit17 && (unit2->flags & 3) != 2)
        return 0;
    // Nested ifs, not one && condition: the flag test codegen differs.
    if (w->flags.bit1) {
        if (LineOfFire_0049abb0(unit1->pos_0049abb0, unit2->pos_0049abb0, w->field_68, w->field_c8) == (short)0x8000)
            return 0;
    }
    return Dist2_0049abb0(&unit1->pos_0049abb0, &unit2->pos_0049abb0) <= w->range * w->range;
}

// FUNCTION: 0x49adf0
int __stdcall GetWeaponRange(int param1, unsigned int param2)
{
    unsigned char idx = (unsigned char)param2;
    int offset = idx * 7;
    int* intermediate = (int*)((char*)param1 + offset * 4 + 0x10);
    int result = *(int*)((char*)*intermediate + 0xdc);
    return result;
}

#pragma pack(push, 1)
struct Projectile_0049ae20 {
    char unknown_0[0x56];
    Projectile_0049ae20* interceptedProjectile; // +0x56
    char unknown_5a[0x67 - 0x5a];
    short compactIndex;                // +0x67, this projectile's old index
    unsigned short flag0 : 1;
    unsigned short dead : 1;           // +0x69 bit 1
    unsigned short flagRest : 14;
};
#pragma pack(pop)

// Compacts the projectile array: entries whose flags bit 1 is set are dropped
// and the remaining ones are moved down. Each projectile's own old index is
// written to compactIndex first, so the pointers held at interceptedProjectile
// (relinked to the moved targets in the second pass) can be resolved by
// searching for that index.
// FUNCTION: 0x49ae20
void CompactProjectiles()
{
    Projectile_0049ae20* projectiles = (Projectile_0049ae20*)g_game->projectiles;
    int count = g_game->projectileCount;
    int n = 0;
    Projectile_0049ae20* dest = 0;
    int i = 0;
    short a[300];
    short b[300];

    for (i = 0; i < g_game->projectileCount; i++) {
        Projectile_0049ae20* p = &projectiles[i];
        // Read the bitfield into a bool first: gives the shr/test pair.
        bool dead = p->dead;
        p->compactIndex = (short)i;
        if (dead) {
            if (!dest) {
                dest = p;
            }
            count--;
        } else {
            if (dest) {
                if (g_game->selected == p) {
                    g_game->selected = dest;
                }
                if (p->interceptedProjectile) {
                    a[n] = (short)(dest - projectiles);
                    b[n] = (short)(p->interceptedProjectile - projectiles);
                    n++;
                }
                *dest = *p;
                dest++;
            }
        }
    }

    if (n != 0) {
        int found = 0;
        for (int j = 0; j < count; j++) {
            for (int k = 0; k < n; k++) {
                if (projectiles[j].compactIndex == b[k]) {
                    projectiles[a[k]].interceptedProjectile = &projectiles[j];
                    if (++found == n) {
                        goto done;
                    }
                }
            }
        }
    }
done:
    g_game->projectileCount = count;
}

struct UnitType_0049af90 {
    char unknown_0[0x10a];
    unsigned char id;                  // +0x10a
};

struct Vec3_0049af90 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Projectile_0049af90 {
    UnitType_0049af90* type;           // +0x0
    char unknown_4[0x28 - 0x4];
    Vec3_0049af90 pos;                 // +0x28
    char unknown_34[0x6b - 0x34];
};

struct Packet_0049af90 {
    char kind;                         // +0x0
    Vec3_0049af90 pos;                 // +0x1
    unsigned char typeId;              // +0xd
};
#pragma pack(pop)

void __stdcall DetonateProjectile(Projectile_0049af90* proj, int flag);

static inline int SamePos(const Vec3_0049af90& a, const Vec3_0049af90& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// Finds the projectile a network packet refers to (by position and type) and
// removes it. The position compare is an inlined helper taking references;
// written inline, MSVC keeps one induction pointer instead of two.
// FUNCTION: 0x49af90
void __stdcall ApplyProjectileHitPacket(int unused, Packet_0049af90* p)
{
    Projectile_0049af90* proj = (Projectile_0049af90*)g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (SamePos(proj->pos, p->pos) && proj->type->id == p->typeId) {
            DetonateProjectile(proj, 0);
            return;
        }
    }
}

struct Weapon_0049b000;

struct UnitType_0049b000 {
    char unknown_0[0x220];
    Weapon_0049b000* weapon1;          // +0x220
    Weapon_0049b000* weapon2;          // +0x224
};

#pragma pack(push, 1)
struct Projectile_0049b000 {
    Weapon_0049b000* weapon;           // +0x0
    Vec3_0049abb0 pos;                 // +0x4
    Vec3_0049abb0 start;               // +0x10
    char unknown_1c[0x4e - 0x1c];
    int targetUnit;                    // +0x4e
    int ownerUnit;                     // +0x52
    char unknown_56[0x66 - 0x56];
    unsigned char owner;               // +0x66
    char unknown_67[0x6b - 0x67];
};
#pragma pack(pop)

void __stdcall DetonateProjectile(Projectile_0049b000* proj, int flag);

// Fires one of a unit's two weapons (by type) from the unit's position,
// building the projectile on the stack; see 0x49af90 for the projectile.
// FUNCTION: 0x49b000
void __stdcall DetonateUnitWeapon(Unit* unit, int second)
{
    Weapon_0049b000* weapon = second ? ((UnitType_0049b000*)unit->utype)->weapon2
                                     : ((UnitType_0049b000*)unit->utype)->weapon1;
    if (weapon) {
        Projectile_0049b000 proj;
        proj.weapon = weapon;
        proj.pos = unit->pos_0049abb0;
        proj.start = unit->pos_0049abb0;
        proj.targetUnit = 0;
        proj.ownerUnit = 0;
        proj.owner = unit->playerIndex;
        DetonateProjectile(&proj, 0);
    }
}

#pragma pack(push, 1)

#define max(a, b) (((a) > (b)) ? (a) : (b))

// 16.16 fixed point seen as the short above the short below.
union Fix_0049b3e0 {
    int whole;
    short half[2];
};

struct Vec3_0049b3e0 {
    int x, y, z;

    Vec3_0049b3e0 operator-(const Vec3_0049b3e0& o) const
    {
        Vec3_0049b3e0 r;
        r.x = x - o.x;
        r.y = y - o.y;
        r.z = z - o.z;
        return r;
    }
    Fix_0049b3e0 Length() const
    {
        double fx = x;
        double fy = y;
        double fz = z;
        Fix_0049b3e0 r;
        r.whole = (int)sqrt(fx * fx + fy * fy + fz * fz);
        return r;
    }
};

struct Weapon_0049b3e0 {
    char unknown_0[0x111];
    unsigned int flags;                // +0x111
};

struct Obj_0049b3e0 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Proj_0049b3e0 {
    Weapon_0049b3e0* weapon;           // +0x0
    Vec3_0049b3e0 pos;                 // +0x4
    Vec3_0049b3e0 start;               // +0x10
    char unknown_1c[0x28 - 0x1c];
    Vec3_0049b3e0 target;              // +0x28
    char unknown_34[0x4e - 0x34];
    Obj_0049b3e0* field_4e;            // +0x4e
    char unknown_52[0x56 - 0x52];
    int* field_56;                     // +0x56
};
#pragma pack(pop)

int __stdcall GetGroundHeight(Vec3_0049b3e0* pos);

// FUNCTION: 0x49b3e0
Vec3_0049b3e0* __stdcall GetProjectileAimPoint(Proj_0049b3e0* p)
{
    unsigned char flag = (unsigned char)((p->weapon->flags >> 0x19) & 1);
    if (flag) {
        Fix_0049b3e0 dist = (p->pos - p->target).Length();
        if (dist.half[1] > 0x400) {
            p->start = p->target;
            ((Fix_0049b3e0*)&p->start.y)->half[1] = 0x2bc;
            return &p->start;
        }
    } else {
        if (p->field_56 != 0)
            return (Vec3_0049b3e0*)(p->field_56 + 1);
        Obj_0049b3e0* q = p->field_4e;
        if (q != 0 && (q->flags & 0x10000000) != 0)
            return (Vec3_0049b3e0*)((char*)q + 0x6a);
    }
    if (flag) {
        p->start = p->target;
        p->start.y = max(GetGroundHeight(&p->target), g_game->seaLevel) << 16;
        return &p->start;
    }
    return &p->target;
}

struct Vec3_0049b520 {
    int x, y, z;
};

#pragma pack(push, 1)
struct UnitType_0049b520 {
    char unknown_0[0xe8];
    unsigned short turnRate;           // +0xe8
    char unknown_ea[0x111 - 0xea];
    unsigned int flags;                // +0x111
};
#pragma pack(pop)

struct Unit_0049b520 {
    UnitType_0049b520* type;           // +0x0
    int x;                             // +0x4
    int y;                             // +0x8
    int z;                             // +0xc
    char unknown_10[0x36 - 0x10];
    short heading;                     // +0x36
    short pitch;                       // +0x38
};

// 16.16 value in a 4 byte union: an 8 byte one (or short[4]) changes the frame.
union Fixed_0049b520 {
    int value;
    struct { unsigned short fraction; short whole; };
};

// Aim a unit's two turret angles at a target point. The heading (+0x36) is
// atan2(dx, dz); the pitch (+0x38) comes from the vertical difference against
// the horizontal distance. Each angle is then moved towards the wanted one by
// at most the unit type's turn rate (+0xE8), and if the wanted angle is more
// than 27000 (about 148 degrees) away while the type's flag bit 22 of +0x111 is
// set, the function gives up and returns 0. Otherwise it returns 1.
// FUNCTION: 0x49b520
int __stdcall TurnUnitTowardsPoint(Unit_0049b520* unit, Vec3_0049b520* target)
{
    UnitType_0049b520* type = unit->type;
    // Declared dx, dy, dz in this order: it decides the stack slot assignment.
    int dx = unit->x - target->x;
    Fixed_0049b520 dy;
    dy.value = unit->y - target->y;
    int dz = unit->z - target->z;
    short a1 = FUN_004b715a(dx, dz);
    int dist = (int)_hypot((double)dx, (double)dz);
    short a2 = FUN_004b715a(-dy.whole, (short)(dist >> 16));

    short diff1 = a1 - unit->heading;
    short adiff1 = abs(diff1);
    if (adiff1 > 27000 && (type->flags & 0x800000))
        return 0;
    if (adiff1 < type->turnRate)
        unit->heading = a1;
    else if (diff1 < 0)
        unit->heading = unit->heading - type->turnRate;
    else
        unit->heading = unit->heading + type->turnRate;

    short diff2 = a2 - unit->pitch;
    short adiff2 = abs(diff2);
    if (adiff2 > 27000 && (type->flags & 0x800000))
        return 0;
    if (adiff2 < type->turnRate)
        unit->pitch = a2;
    else if (diff2 < 0)
        unit->pitch = unit->pitch - type->turnRate;
    else
        unit->pitch = unit->pitch + type->turnRate;
    return 1;
}

#pragma pack(push, 2)
struct Object_0049b680 {
    char unknown_0[0x1c];
    int x;                             // +0x1c
    int y;                             // +0x20
    int z;                             // +0x24
    char unknown_28[0x36 - 0x28];
    short angle1;                      // +0x36
    short angle2;                      // +0x38
    int length;                        // +0x3a
};
#pragma pack(pop)

// FUNCTION: 0x49b680
void __stdcall ComputeVelocityFromAngles(Object_0049b680* obj)
{
    obj->y = FUN_004b70ef(obj->angle2, obj->length);
    int r = FUN_004b7123(obj->angle2, obj->length);
    obj->x = -FUN_004b70ef(obj->angle1, r);
    obj->z = -FUN_004b7123(obj->angle1, r);
}

#pragma pack(push, 1)
struct Projectile_0049b6e0 {
    char unknown_0[0x4e];
    int targetUnit;                    // +0x4e
    char unknown_52[0x69 - 0x52];
    unsigned short flags;              // +0x69
};
#pragma pack(pop)

// Takes the next free entry of the 300-entry projectile array (see
// 0x499a30), resets two of its fields and returns it, or 0 when full.
// FUNCTION: 0x49b6e0
Projectile_0049b6e0* AllocProjectile()
{
    Projectile_0049b6e0* p = 0;
    if (g_game->projectileCount < 300) {
        p = &((Projectile_0049b6e0*)g_game->projectiles)[g_game->projectileCount++];
        p->flags &= ~2;
        p->targetUnit = 0;
    }
    return p;
}


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
    proj->spawnTick = field_5;
    proj->flags &= ~1;
    proj->active = 0;
    // Second clear via an unsigned short local, stored last: `&=` folds into one mask.
    unsigned short f = proj->flags;
    // nextSmokeTick is stored before the mask clear: puts g_game in edx.
    proj->nextSmokeTick = g_game->field_38a47;
    proj->interceptedProjectile = 0;
    proj->targetUnit = 0;
    proj->flags = f & ~0x30;
    if (unit) {
        proj->SetOwner(unit);
        if ((unit->flags & 0x20000000) && g_game->trackedUnit == unit)
            g_game->trackedProj = proj;
        for (i = 0; i < 3; i++)
            if (unit->slots[i].shot == shot)
                break;
        proj->piece = (short)QueryWeaponPiece(unit, i);
        unit->workTime = g_game->field_38a47 + 0x258;
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
void __stdcall ComputeProjectileTime(Object_0049c920* obj)
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
void __stdcall CopyWeaponVelocityParam(Obj_0049c980* obj, Src_0049c980* src)
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

short __cdecl FUN_004b715a(int x, int z);
int __cdecl FUN_004b70ef(short angle, int scale);
int __cdecl FUN_004b7123(short angle, int scale);
void __stdcall EmitWeaponSmoke(Vec3* p, short index);

extern char* g_fireScriptNames[4];

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
    unit->anims->StartScript(g_fireScriptNames[(fire->f_1b >> 2) & 3], 0, 0);
    short angle = unit->f_1a[((fire->f_1b >> 2) & 3) * 7].angle - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    ((CobScript*)unit->anims)->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);

    if (fire->shot->f_bit9)
        EmitWeaponSmoke(p3, 9);
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
    int speed;
    char unknown_3e[0x46 - 0x3e];
    int time;                          // +0x46
    char unknown_4a[0x4e - 0x4a];
    int targetUnit;
    char unknown_52[0x56 - 0x52];
    int interceptedProjectile;
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
        proj->targetUnit = 0;
    }
    if (!proj)
        return 0;

    InitProjectile(proj, shot->def, pos, aim, g_game->field_38a47, unit);
    proj->angle = 0;
    proj->pitch0 = 0x4000;
    shot->field_8 = 0;
    if (shot->def->f_6c) {
        proj->speed = shot->def->f_6c;
    } else if (shot->def->f_70 == 0) {
        proj->speed = shot->def->f_68;
    } else {
        proj->speed = 0;
    }
    memset(&proj->dir, 0, 12);
    UnitType_0049cc20* u = proj->unit;
    if (u->f_68 != 0 && !(u->flags.all & 0x8000000)) {
        proj->time = (u->f_dc << 16) / (unsigned int)u->f_68 + g_game->field_38a47;
    } else {
        proj->time = g_game->field_38a47 + u->f_e6;
    }
    proj->targetUnit = param_5;
    proj->interceptedProjectile = param_6;
    proj->active = shot->def->f_ea;
    ((CobScript*)unit->anims)->StartScript(g_fireScriptNames[(shot->field_1b >> 2) & 3], 0, 0);
    short angle = unit->aim_0049cc20[(shot->field_1b >> 2) & 3][0][0] - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
    if (shot->def->flags.bits.bit9)
        EmitWeaponSmoke(pos, 9);
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
    int targetUnit;
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
        proj->targetUnit = 0;
    }
    if (proj) {
        InitProjectile(proj, shot->def, pos, 0, g_game->field_38a47, unit);
        proj->angle = shot->heading;
        proj->pitch0 = shot->pitch;
        // The whole product is its own statement: inside the assignment the
        // division and multiply sink after the call.
        int q = (shot->field_10 / shot->def->f_68) * g_game->gravity;
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
        // active before targetUnit: the reverse of the natural order is the original's.
        proj->active = shot->def->f_ea;
        proj->targetUnit = param_5;
        ((CobScript*)unit->anims)->StartScript(g_fireScriptNames[(shot->field_1b >> 2) & 3], 0, 0);
        short angle = unit->aim_0049cde0[(shot->field_1b >> 2) & 3][0] - unit->heading;
        int a = -FUN_004b70ef(angle, 800);
        int b = -FUN_004b7123(angle, 800);
        unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
        if (shot->def->flags.f9)
            EmitWeaponSmoke(pos, 9);
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
    int velY;
    int dirZ;                         // +0x24
    char unknown_28[0x36 - 0x28];
    short angle;                      // +0x36
    char unknown_38[0x3a - 0x38];
    int speed;
    char unknown_3e[0x4e - 0x3e];
    int targetUnit;
    char unknown_52[0x69 - 0x52];
    unsigned short flags;             // +0x69
    // Only inside an inline method does MSVC 5 put the angle load before the
    // +0x3a store, as the original does; inline it in the caller and it sinks.
    void Setup(Unit* u) { angle = u->angle; speed = 0; velY = 0; }
};
#pragma pack(pop)

void __stdcall InitProjectile(Proj_0049d000*, void*, void*, int, int, Unit*);

// FUNCTION: 0x49d000
int __stdcall SpawnProjectileFromUnitMotion(Shot_0049d000* shot, Unit* unit, Vec3* pos)
{
    Proj_0049d000* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049d000*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->targetUnit = 0;
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
int __stdcall FireWeaponByFlags(Object_0049d0c0* obj, int a, int b, int d, int c)
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
    int velX;
    int velY;
    int velZ;
    char unknown_28[0x36 - 0x28];
    short heading;
    char unknown_38[0x3a - 0x38];
    int speed;
    char unknown_3e[0x4e - 0x3e];
    int targetUnit;
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
        projectile->targetUnit = 0;
    }
    if (projectile != 0) {
        InitProjectile(projectile, aim->type, &p, 0, g_game->field_38a47, unit);
        projectile->heading = unit->heading;
        projectile->speed = 0;
        projectile->velY = 0;
        projectile->velX = -FUN_004b70ef(projectile->heading, unit->owner->id);
        projectile->velZ = -FUN_004b7123(projectile->heading, unit->owner->id);
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
    int targetUnit;                    // +0x4e
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
int __stdcall SpawnProjectile(Unit_0049df10* unit, Vec3* a, Vec3* b, int flag)
{
    Proj_0049df10* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049df10*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->targetUnit = 0;
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
extern char* g_aimScriptNames[3];

int __stdcall GetWeaponTargetPos(Unit* unit, Vec3* pos, int index);
Unit* __stdcall GetWeaponTargetUnit(Unit* obj, int index);
void __stdcall SubtractVec3(Vec3* a, Vec3* b, int* dx, int* dy, int* dz);
int __stdcall CalcAimAngles(Unit* unit, Target_0049e1a0* target,
                           unsigned short* out_heading, unsigned short* out_pitch,
                           unsigned char weapon, Vec3* point);
int __stdcall WeaponCanReachPos(Unit* unit, Vec3* a2, Vec3* a3,
                           unsigned char a4);
int __stdcall SendScriptCallByName(Unit* obj, char* name, char field_5, int field_6, int field_a,
                           unsigned short field_e, unsigned short field_12);
void __stdcall UpdateBuildMenuIfFocusUnit(Unit* unit);

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
                    SubtractVec3(&aim, &pos, &dx, &dy, &dz);
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
                    unit->script->StartScriptWithArgs(g_aimScriptNames[(e->flags >> 2) & 3], &e->name, 0, 2,
                                               heading, angle, 0, 0);
                    SendScriptCallByName(unit, g_aimScriptNames[(e->flags >> 2) & 3], 2, heading, angle, 0, 0);
                    e->flags |= 1;
                }
            }
        } else {
            // Via a bool local: testing the bitfield in place shifts the register rotation.
            bool armed = attached->f_111.b4;
            if (armed && (!attached->f_111.b28 || e->f_1a) && !(e->flags & 1)) {
                e->f_8 = 0;
                unit->script->StartScriptWithArgs(g_aimScriptNames[(e->flags >> 2) & 3], &e->name, 0, 2, 0, 0,
                                           0, 0);
                SendScriptCallByName(unit, g_aimScriptNames[(e->flags >> 2) & 3], 2, 0, 0, 0, 0);
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
                UpdateBuildMenuIfFocusUnit(unit);
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
void __stdcall SubtractVec3(int* a, int* b, int* dx, int* dy, int* dz)
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
// FUNCTION: 0x49e610 _$E5
// FUNCTION: 0x49e630 _$E3
TdfFile DAT_0051f310;
