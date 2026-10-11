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
// order; 0x49a120 (a gap region) and 0x49b720 keep their own files
// (docs/split-modules.md). 0x49b090 and 0x49be60 come last, out of address
// order: their register plans follow their symbol ids (docs/c2-regalloc.md).

#include "../util/vec3.h"

typedef Vec3 Vec3_0049e1a0;

union Fixed {
    int value;
    struct {
        unsigned short fraction;
        short whole;
    };
};

#include "../units/cob_script.h"

// Unused here: these headers and forward declarations take the symbol ids that keep
// WeaponCanReachPos (0x49aa80) and WeaponCanReachUnit (0x49abb0) matching (docs/c2-regalloc.md).
#include "../util/hapi_bank.h"
#include "../map/mission.h"
#include "../sound/sound.h"
struct BmpFileHeader;
struct BmpInfo;
struct BmpInfoHeader;
struct CalcedExplosion;
struct FrameTable;
struct Chunk;
class DamagedAllyCollector;
class AssaultTimer;
class EscortTimer;
class SquadScoutTimer;


#include "../network/player.h"
#include "../units/unit_def.h"
#include "unit_weapon_slot.h"

struct Gun_0049c9c0 {
    short angle;
    char unknown_2[2];
};

#pragma pack(push, 1)

struct Unit;

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

struct Entry_0049e1a0 {        // 0x1c bytes
    Point_0049e1a0 point;      // +0x00
    char* name;                // +0x04
    int f_8;                   // +0x08
    WeaponDef* attached;       // +0x0c
    char unknown_10[0x14 - 0x10];
    unsigned short f_14; // +0x14, the slot's tick counter
    short f_16;          // +0x16
    short f_18;          // +0x18
    unsigned char f_1a;  // +0x1a
    unsigned char flags; // +0x1b
};

#include "../game/unit_resources.h"

struct Owner_0049dd60 {
    char unknown_0[0x20];
    int id;                            // +0x20
};

struct UnitType_0049d000 {
    char unknown_0[0x20];
    int range;                         // +0x20
};

#include "weapon_def.h"

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
// +0xc is f_c (a WeaponDef), its weapons[3]
// are slots[3], its f16/f18/f1b are f_16/f_18/f_1b, its owner is playerIndex,
// its armour is killCount, its state is flags, its type is utype and its pos is
// pos_0049abb0.
// The unit, as the weapon code sees it. The views disagree about the bytes at
// +0x4 (49e1a0's three weapon entries, 49d270's one, 49d580's fire state and
// 49c740's and 49e070's weapon slots all overlap there), about +0x0 (49d000's
// type, 49d270's type and 49dd60's owner), about +0xbc (49e1a0's resources,
// 49e070's field_e4 and 49c920's and 49c9c0's f_dc and f_e6) and about the flag word (+0x110 in 49c740, 49d880 and 49d270, +0x111 in
// 49c920 and 49c9c0); the rest keep one name. Size 0x118, the unit array's
// stride.
struct Unit {
    union {                            // +0x0
        UnitType_0049d000* type;       // 49d000's unit type
        Type_0049d270* def;            // 49d270's type
        Owner_0049dd60* owner;         // 49dd60's owner
    };
    union {                            // +0x4
        Entry_0049e1a0 weapons[3];     // 49e1a0's weapon entries, stride 0x1c
        Entry_0049d270 entry[1];       // 49d270's entry, indexed by the packet
        struct {                       // 49d580's fire state
            char unknown_4[4];
            int f_8;                   // +0x8
            WeaponDef* f_c;      // +0xc
            char unknown_10[6];
            short f_16;                // +0x16
            short f_18;                // +0x18
            char unknown_1a;
            unsigned char f_1b;        // +0x1b
        };
        UnitWeaponSlot slots[3];       // +0x4, stride 0x1c
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
    UnitDef* utype;                    // +0x92
    Player* player;                    // +0x96
    union {                            // +0x9a
        CobScript* anims;
        CobScript* script;
    };
    char unknown_9e[0xa8 - 0x9e];
    union {                            // +0xa8
        short f_a8;                    // 49d580's
        unsigned short f_a8u;          // 49d270's
        short id;                      // 49d1e0's and 49d9c0's unit id
    };
    char unknown_aa[0xb0 - 0xaa];
    int workTime;                      // +0xb0, 49c740's
    char unknown_b4[0xb8 - 0xb4];
    unsigned short killCount;          // +0xb8, the kill count, which scales the aiming inaccuracy
    union {                            // +0xba
        Fbb_0049d580 f_bb;
        Fba_0049e1a0 f_ba;
    };
    union {                            // +0xbc
        UnitResources resourceSlot;    // 49e1a0's resources
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
    short health;                      // +0x108
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
    WeaponDef* shot;                   // +0x00
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

struct Unit_0049c880;

#pragma pack(push, 1)
// Must end at +0x6b with no trailing padding: the loop stride follows it.
struct Projectile_0049c880 {
    WeaponDef* type;                   // +0x0
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

// Unused here: real functions declared for their symbol ids, which keep
// 0x49a890 and 0x49aa80 matching, and make up the ids of the bit-field views
// that weapon_def.h no longer declares (docs/c2-regalloc.md).
void DrawDashedCircle(void*, int, int, int, int, int, int);
int HAPINET_createnewgame(void*, char*, int, int, int, int, int);
int __stdcall SquashPack(Chunk*, int*, char*, int, int, int);
int HAPINET_enumconnections(void*, void*, unsigned long, void*, unsigned long, void*);
int PickRandomReclaimableResourcesInRadius(int, int, int, int, int, int);
int CreateUnit(int, int, int, int, int, int);

struct Projectile_00499eb0;
struct MapFeature_0049b090;

// The game state, as the weapon code sees it. The time word keeps one name per
// view (now, frame, gameTick, teamColor); the two tracked-projectile pointers
// are one union, 499a30's selected and 49c740's tracked.
struct Net {
    char unknown_0[0xd48];
    int noSeaLevelTrigger;
};

struct Game {
    char unknown_0[0xdcb];
    unsigned char palette[0x2a];       // +0xdcb, the line colours of the shots
    char unknown_df5[0x1b63 - 0xdf5];
    Player players[10];                // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    char localPlayer;                  // +0x2a42
    char playerIndex;
    union {
        unsigned char flags;           // +0x2a44
        unsigned short flags_2a44;
    };
    char unknown_2a46[0x2cf3 - 0x2a46];
    WeaponDef weaponDefs[0x100];       // +0x2cf3
    union {
        int projectileCount;           // +0x141f3
        int projCount;
        int projectile_count;
    };
    void* projectiles;                 // +0x141f7
    char unknown_141fb[0x14233 - 0x141fb];
    int mapWidthTiles;                 // +0x14233
    char unknown_14237[0x14253 - 0x14237];
    int featureCount;                  // +0x14253
    char unknown_14257[0x14263 - 0x14257];
    int gravity;                       // +0x14263
    char unknown_14267[0x1426f - 0x14267];
    MapFeature_0049b090* features;     // +0x1426f
    char unknown_14273[0x1427f - 0x14273];
    unsigned char seaLevel;            // +0x1427f
    char debugMode;
    unsigned short mapFlags;           // +0x14281
    char unknown_14283[0x142f3 - 0x14283];
    Unit* cameraFollowUnit;            // +0x142f3
    union {
        Proj_0049c740* cameraFollowTrackObj;  // +0x142f7
        Projectile_0049c880* tracked;
        void* selected;                // 499a30's spelling
        Projectile_00499eb0* selectedProjectile; // 499e50's and 499eb0's
    };
    char unknown_142fb[0x1431f - 0x142fb];
    int scrollX;                       // +0x1431f
    int scrollY;                       // +0x14323
    char unknown_14327[0x1433f - 0x14327];
    Vec3 cameraSnapPos;                // +0x1433f
    short cameraSnapTicks;             // +0x1434b
    char unknown_1434d[0x14357 - 0x1434d];
    Unit* units;                       // +0x14357
    char unknown_1435b[0x147bb - 0x1435b];
    void* cannonShellSeq;              // +0x147bb
    void* plasmaSmSeq;                 // +0x147bf
    void* plasmaMdSeq;                 // +0x147c3
    void* ultraShellSeq;               // +0x147c7
    void* plasmaSmSeq2;                // +0x147cb
    char unknown_147cf[0x147f3 - 0x147cf];
    void* flameStreamSeq;              // +0x147f3
    char unknown_147f7[0x1480f - 0x147f7];
    void* shadowSeq;                   // +0x1480f
    char unknown_14813[0x1ab9b - 0x14813];
    void* explosionLensFrame;          // +0x1ab9b
    char unknown_1ab9f[0x37e27 - 0x1ab9f];
    char region_37e27[0x37ecc - 0x37e27];
    Vec3 wind;                         // +0x37ecc
    char unknown_37ed8[0x38a47 - 0x37ed8];
    union {
        int gameTick;                  // +0x38a47
        int frame;
        unsigned int now;
        int teamColor;
    };
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Net* mapInfo;                      // +0x391e9
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
struct Cell;

#pragma pack(pop)

extern Game* g_game;

void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);

// Allocates and clears the 0x7d64-byte weapon array, then resets its count.
// FUNCTION: 0x499a30
void AllocWeaponArray(void)
{
    g_game->projectiles = GameAllocIgnoreTag("WEAPON ARRAY", 0x7d64);
    memset(g_game->projectiles, 0, 0x7d64);
    g_game->projectileCount = 0;
}

void __cdecl GameFreeThunk(void* p);

// FUNCTION: 0x499a80
void FreeWeaponArray(void)
{
    GameFreeThunk(g_game->projectiles);
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

// One view of the 0x24 byte weapon fire packet (type 0xd): the two partial
// senders at 0x499ba0 and 0x49df10, the four firing routines' views and the
// receiver at 0x49d270 were folded in. Where their own views were, an unused
// declaration keeps the file's symbol ids (docs/c2-regalloc.md).
struct WeaponFirePacket {
    unsigned char type;              // +0x0
    Vec3 origin;                     // +0x1
    Vec3 aimOrVel;                   // +0xd
    unsigned char weaponDefIndex;    // +0x19
    unsigned char flag : 1;          // +0x1a, bit 0
    unsigned char : 7;
    unsigned short fireHeading;      // +0x1b
    unsigned short firePitch;        // +0x1d
    unsigned short targetUnitId;     // +0x1f
    unsigned short ownerUnitId;      // +0x21
    unsigned char weaponSlotIndex;   // +0x23
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
    WeaponFirePacket packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.origin = *(Vec3*)a;
        packet.aimOrVel = *(Vec3*)b;
        packet.weaponDefIndex = unit->f_c->index;
        packet.weaponSlotIndex = (unit->f_1b >> 2) & 3;
        packet.ownerUnitId = !source ? 0 : source->team;
        packet.targetUnitId = !target ? 0 : target->team;
        packet.fireHeading = unit->f_16;
        packet.firePitch = unit->f_18;
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
// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_00499ba0(int a, int b, int c, int d, int e, int f);
#pragma pack(pop)

int __cdecl GetLocalDpid();

// FUNCTION: 0x499ba0
void __stdcall BroadcastWeaponFire(char param_1, Vec3_00499ba0* a, Vec3_00499ba0* b)
{
    WeaponFirePacket packet;
    if (g_game->flags_2a44 & 1) {
        packet.type = 0xd;
        packet.origin = *(Vec3*)a;
        packet.aimOrVel = *(Vec3*)b;
        packet.weaponDefIndex = param_1;
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

#include "../ai/squad_manager.h"

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
        attacker->player->ai->MarkOwnerNetDirtyFromDamageSplit((Obj_00406f50*)weapon, a, b);
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

struct Weapon_00499cd0 {
    WeaponDef* def;                 // +0x0
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
    WeaponDef* def = weapon->def;
    int damage = def->damage;
    Table_00499cd0* table = def->table;
    if (table) {
        int* p = Find_00499cd0(table, target->utype->unitname);
        if (p)
            damage = *p;
    }
    damage = (int)(damage * scale);
    short angle = FUN_004b715a(weapon->x - target->pos_0049abb0.x,
                               weapon->z - target->pos_0049abb0.z) - target->heading;
    Unit* attacker = weapon->attacker;
    if (attacker) {
        int armour = attacker->killCount / 5;
        if (armour > 5)
            armour = 5;
        damage = (armour * 6 + 100) * damage / 100;
    }
    Flags_00499cd0* flags = (Flags_00499cd0*)((char*)g_game + 0x37f2f);
    if (flags->flag7)
        damage *= 2;
    if (flags->flag8)
        damage /= 2;
    bool paralyzer = weapon->def->flags.b.paralyzer;
    int type = paralyzer ? 2 : 1;
    DamageUnit(attacker, target, damage, type, angle);
    return damage;
}

#pragma pack(push, 1)

struct Projectile_00499eb0 {
    WeaponDef* type;
    Vec3_0049b720 position;
    char unknown_10[0x52 - 0x10];
    Unit* unit;
    char unknown_56[0x66 - 0x56];
    unsigned char owner;
    char unknown_67[2];
    // Stays unsigned short: gives the single byte OR with 2.
    unsigned short flags;
};

#pragma pack(pop)

// FUNCTION: 0x499e50
void __stdcall UntrackProjectile(Projectile_00499eb0* projectile)
{
    if (projectile == g_game->selected) {
        g_game->cameraSnapPos = g_game->selectedProjectile->position;
        g_game->cameraSnapTicks = projectile->type->holdTime;
        g_game->selected = 0;
    }
    projectile->flags |= 2;
}

Cell* __stdcall GetMapCellAtPosition(Vec3_0049b720* position);
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
    WeaponDef* type = projectile->type;
    Vec3_0049b720* position = &projectile->position;
    unsigned char* value = (unsigned char*)GetMapCellAtPosition(position);
    if (value != 0)
        hostile = value[5] < g_game->seaLevel;
    if (!type->flags.b.noExplode) {
        if (projectile == g_game->selected) {
            g_game->cameraSnapPos = g_game->selectedProjectile->position;
            g_game->cameraSnapTicks = projectile->type->holdTime;
            g_game->selected = 0;
        }
        projectile->flags = projectile->flags | 2;
    }
    if (g_game->mapInfo->noSeaLevelTrigger && hostile && !unit) {
        if (projectile == g_game->selected) {
            g_game->cameraSnapPos = g_game->selectedProjectile->position;
            g_game->cameraSnapTicks = projectile->type->holdTime;
            g_game->selected = 0;
        }
        projectile->flags = projectile->flags | 2;
        return;
    }
    AccumulateScreenShake(type->shakeMagnitude, type->shakeMagnitude, type->shakeDuration);
    if (hostile && !unit) {
        PlaySoundAt(type->soundWater, position, 0);
        AddExplosionEffect(position, type->splash, 0, hostile);
    } else {
        PlaySoundAt(type->soundHit, position, 0);
        if (type->flags.b.endSmoke)
            EmitWhiteSmoke(position, 9);
        else
            AddExplosionEffect(position, type->explosionSeq, 0, hostile);
    }
    unsigned int player = projectile->owner;
    Player* record = &g_game->players[player];
    if (!record->active || record->type != 3) {
        if (type->areaOfEffect <= 0x10 && unit) {
            int damage = ApplyWeaponDamage(projectile, unit, 1.0f);
            Unit* source = projectile->unit;
            if (source) {
                int a = 0;
                int b = 0;
                if (projectile->owner != unit->playerIndex)
                    a = damage;
                else
                    b = damage;
                source->player->ai->MarkOwnerNetDirtyFromDamageSplit((Obj_00406f50*)projectile, a & 0xffff, b & 0xffff);
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

struct FixedVec3 {
    Fixed_0049a120 x;
    Fixed_0049a120 y;
    Fixed_0049a120 z;
};

struct CellPos_0049a120 {
    short x;
    short z;
};

#pragma pack(push, 1)
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
    WeaponDef* def;                    // +0x0
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

struct Cell {
    unsigned short unit;               // +0x0
    unsigned short unit2;              // +0x2
    unsigned char height;              // +0x4
    unsigned char high;                // +0x5, highest floor
    unsigned char low;                 // +0x6, lowest floor
    unsigned char metal;               // +0x7
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

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049a120(int a, int b, int c, int d, int e, int f);
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
    Cell* features[64];

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
    int AddFeature(Cell* cell)
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

Cell* __stdcall GetMapCell(int x, int y);
int __stdcall ApplyWeaponDamage(Weapon_0049a120* weapon, Unit_0049a120* target, float scale);
void __stdcall DetonateProjectile(Weapon_0049a120* weapon, Unit_0049a120* unit);
int __stdcall VectorLength(Vec3_0049a120* v);
Vec3_0049a120 __stdcall GetFootprintCentre(CellPos_0049a120* cell, FeatureDef_0049a120* def);
void __stdcall DamageFeature(Cell* cell, int x, int z, WeaponDef* def);
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

// Unused here: real functions declared for their symbol ids, which keep
// 0x49aa80 matching (docs/c2-regalloc.md).
void RegisterUnitOrders();
void RegisterGroundOrders();
void EnableAICommands();
void RegisterAICommands();
void ResetAIPlayers();
void RegisterVtolOrders();
void StepAllGafSequences();
void ResetNetStats();
void InitCommands();
void RefreshSelectionOrders();
int RIReport(int, int, int, int, int, int, int, int, int, int);
void EnumPlayersCallback(int, int, int, int, int);
int CheckDirectXVersion(int, int, int, int, int);
void EmitThrustParticles(int, int, int, int, short);
int AimCobStub(int, int, int, int);
void DispatchOrdersPanelPageFlags();
void ResetCameraState();
void FindLocalCommander();
void ClearCameraFollowState();
void ClampCameraPosition();
void ClampCameraTarget();
void UpdateScreenShake();
void UpdateCameraFollow();
void BeginMouseScroll();
void EndMouseScroll();
void UpdateMouseScroll();
void UpdateEdgeScroll();

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
    WeaponDef* wdef = a1->slots[a4 & 0xff].weapon;

    // z difference first, as named int locals: the original's order.
    int dz = a3->z - a2->z;
    int dx = a3->x - a2->x;
    if ((int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dz * dz) >> 32) > wdef->range * wdef->range)
        return 0;

    if (wdef->flags.b.waterWeapon)
        return 1;

    if (a2->y.parts.whole + a1->utype->field_170 <= g_game->seaLevel)
        return 0;

    if (wdef->flags.b.ballistic) {
        if (LineOfFire_0049aa80(*a2, *a3, wdef->speed, wdef->field_c8) == (short)0x8000)
            return 0;
    }
    return 1;
}

// Both includes change the register choice in the sea-level tests.
#include <stdlib.h>

#pragma pack(push, 1)

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
    WeaponDef* w = unit1->slots[weapon].weapon;

    if (w->flags.b.waterWeapon) {
        if (!(unit2->utype->flags1 & 0x80000) && unit2->pos_0049abb0.y.parts.whole > g_game->seaLevel)
            return 0;
        // Written (height >> 1) + whole: sets the add's operand order.
        if ((unit2->utype->flags1 & 0x1000) && (unit2->utype->field_170 >> 1) + unit2->pos_0049abb0.y.parts.whole > g_game->seaLevel)
            return 0;
        return Dist2_0049abb0(&unit1->pos_0049abb0, &unit2->pos_0049abb0) <= w->range * w->range;
    }

    if (unit1->pos_0049abb0.y.parts.whole + unit1->utype->field_170 <= g_game->seaLevel)
        return 0;
    if (unit2->pos_0049abb0.y.parts.whole + unit2->utype->field_170 <= g_game->seaLevel)
        return 0;
    if (w->flags.b.toAirWeapon && (unit2->flags & 3) != 2)
        return 0;
    // Nested ifs, not one && condition: the flag test codegen differs.
    if (w->flags.b.ballistic) {
        if (LineOfFire_0049abb0(unit1->pos_0049abb0, unit2->pos_0049abb0, w->speed, w->field_c8) == (short)0x8000)
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

struct Vec3_0049af90 {
    int x, y, z;
};

#pragma pack(push, 1)
struct Projectile_0049af90 {
    WeaponDef* type;                   // +0x0
    char unknown_4[0x28 - 0x4];
    Vec3_0049af90 pos;                 // +0x28
    char unknown_34[0x6b - 0x34];
};

// One view of the 0xe byte projectile detonate packet (type 0xe): the sender
// at 0x49a120, a view of which is in the gap file, holds the other.
struct ProjectileDetonatePacket {
    unsigned char type;                // +0x0
    Vec3_0049af90 aim;                 // +0x1
    unsigned char weaponIndex;         // +0xd
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
void __stdcall ApplyProjectileHitPacket(int unused, ProjectileDetonatePacket* p)
{
    Projectile_0049af90* proj = (Projectile_0049af90*)g_game->projectiles;
    for (int i = 0; i < g_game->projectileCount; i++, proj++) {
        if (SamePos(proj->pos, p->aim) && proj->type->index == p->weaponIndex) {
            DetonateProjectile(proj, 0);
            return;
        }
    }
}

#pragma pack(push, 1)
struct Projectile_0049b000 {
    WeaponDef* weapon;                 // +0x0
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
    // The self-destruct and explosion weapons of the unit's definition.
    WeaponDef* weapon = second ? (WeaponDef*)unit->utype->selfdestructas
                               : (WeaponDef*)unit->utype->explodeas;
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

struct Vec3_0049b3e0 {
    int x;
    Fixed y;
    int z;

    Vec3_0049b3e0 operator-(const Vec3_0049b3e0& o) const
    {
        Vec3_0049b3e0 r;
        r.x = x - o.x;
        r.y.value = y.value - o.y.value;
        r.z = z - o.z;
        return r;
    }
    Fixed Length() const
    {
        double fx = x;
        double fy = y.value;
        double fz = z;
        Fixed r;
        r.value = (int)sqrt(fx * fx + fy * fy + fz * fz);
        return r;
    }
};

struct Obj_0049b3e0 {
    char unknown_0[0x6a];
    Vec3_0049b3e0 pos;                 // +0x6a
    char unknown_76[0x110 - 0x76];
    unsigned int flags;                // +0x110
};

struct Proj_0049b3e0 {
    WeaponDef* weapon;                 // +0x0
    Vec3_0049b3e0 pos;                 // +0x4
    Vec3_0049b3e0 start;               // +0x10
    char unknown_1c[0x28 - 0x1c];
    Vec3_0049b3e0 target;              // +0x28
    char unknown_34[0x4e - 0x34];
    Obj_0049b3e0* targetUnit;          // +0x4e
    char unknown_52[0x56 - 0x52];
    int* interceptedProjectile;        // +0x56
};
#pragma pack(pop)

int __stdcall GetGroundHeight(Vec3_0049b3e0* pos);

// FUNCTION: 0x49b3e0
Vec3_0049b3e0* __stdcall GetProjectileAimPoint(Proj_0049b3e0* p)
{
    unsigned char flag = (unsigned char)p->weapon->flags.b.cruise;
    if (flag) {
        Fixed dist = (p->pos - p->target).Length();
        if (dist.whole > 0x400) {
            p->start = p->target;
            p->start.y.whole = 0x2bc;
            return &p->start;
        }
    } else {
        if (p->interceptedProjectile != 0)
            return (Vec3_0049b3e0*)(p->interceptedProjectile + 1);
        Obj_0049b3e0* q = p->targetUnit;
        if (q != 0 && (q->flags & 0x10000000) != 0)
            return &q->pos;
    }
    if (flag) {
        p->start = p->target;
        p->start.y.value = max(GetGroundHeight(&p->target), g_game->seaLevel) << 16;
        return &p->start;
    }
    return &p->target;
}

struct Vec3_0049b520 {
    int x, y, z;
};

struct Unit_0049b520 {
    WeaponDef* type;                   // +0x0
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
// than 27000 (about 148 degrees) away while the type's burnBlow flag (bit 23 of
// +0x111) is set, the function gives up and returns 0. Otherwise it returns 1.
// FUNCTION: 0x49b520
int __stdcall TurnUnitTowardsPoint(Unit_0049b520* unit, Vec3_0049b520* target)
{
    WeaponDef* type = unit->type;
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
    if (adiff1 > 27000 && type->flags.b.burnBlow)
        return 0;
    if (adiff1 < type->turnRate)
        unit->heading = a1;
    else if (diff1 < 0)
        unit->heading = unit->heading - type->turnRate;
    else
        unit->heading = unit->heading + type->turnRate;

    short diff2 = a2 - unit->pitch;
    short adiff2 = abs(diff2);
    if (adiff2 > 27000 && type->flags.b.burnBlow)
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
void __stdcall InitProjectile(Proj_0049c740* proj, WeaponDef* shot, Vec3* pos,
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
    proj->nextSmokeTick = g_game->gameTick;
    proj->interceptedProjectile = 0;
    proj->targetUnit = 0;
    proj->flags = f & ~0x30;
    if (unit) {
        proj->SetOwner(unit);
        if ((unit->flags & 0x20000000) && g_game->cameraFollowUnit == unit)
            g_game->cameraFollowTrackObj = proj;
        for (i = 0; i < 3; i++)
            if (unit->slots[i].weapon == shot)
                break;
        proj->piece = (short)QueryWeaponPiece(unit, i);
        unit->workTime = g_game->gameTick + 0x258;
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
        g_game->cameraSnapPos = g_game->tracked->pos;
        g_game->cameraSnapTicks = proj->type->holdTime;
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
    WeaponDef* shot;                   // +0xc
    char unknown_10[0x1b - 0x10];
    unsigned char f_1b;                // +0x1b
};

void __stdcall InitProjectile(Proj_0049c9c0* proj, WeaponDef* shot,
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
    if (fire->shot->startSpeed) {
        proj->f_3a = fire->shot->startSpeed;
    } else if (!fire->shot->acceleration) {
        proj->f_3a = fire->shot->speed;
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

    proj->f_60 = fire->shot->burst;
    unit->anims->StartScript(g_fireScriptNames[(fire->f_1b >> 2) & 3], 0, 0);
    short angle = unit->f_1a[((fire->f_1b >> 2) & 3) * 7].angle - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);

    if (fire->shot->flags.b.startSmoke)
        EmitWeaponSmoke(p3, 9);
    return 1;
}

#include <string.h>

#pragma pack(push, 1)
struct Shot_0049cc20 {
    char unknown_0[0x8];
    int aimCobDone;                    // +0x8, the slot's aimCob.nAimCobDone
    WeaponDef* def;                    // +0xc, the weapon that fired
    char unknown_10[0x1b - 0x10];
    unsigned char flags;               // +0x1b
};

struct Proj_0049cc20 {
    WeaponDef* unit;                   // +0x0
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

void __stdcall InitProjectile(Proj_0049cc20* proj, WeaponDef* shot, Vec3* pos,
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

    InitProjectile(proj, shot->def, pos, aim, g_game->gameTick, unit);
    proj->angle = 0;
    proj->pitch0 = 0x4000;
    shot->aimCobDone = 0;
    if (shot->def->startSpeed) {
        proj->speed = shot->def->startSpeed;
    } else if (shot->def->acceleration == 0) {
        proj->speed = shot->def->speed;
    } else {
        proj->speed = 0;
    }
    memset(&proj->dir, 0, 12);
    WeaponDef* u = proj->unit;
    if (u->speed != 0 && !u->flags.b.noAutoRange) {
        proj->time = (u->range << 16) / (unsigned int)u->speed + g_game->gameTick;
    } else {
        proj->time = g_game->gameTick + u->lifetime;
    }
    proj->targetUnit = param_5;
    proj->interceptedProjectile = param_6;
    proj->active = shot->def->burst;
    unit->anims->StartScript(g_fireScriptNames[(shot->flags >> 2) & 3], 0, 0);
    short angle = unit->aim_0049cc20[(shot->flags >> 2) & 3][0][0] - unit->heading;
    int a = -FUN_004b70ef(angle, 800);
    int b = -FUN_004b7123(angle, 800);
    unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
    if (shot->def->flags.b.startSmoke)
        EmitWeaponSmoke(pos, 9);
    return 1;
}

#pragma pack(push, 1)
struct Shot_0049cde0 {
    char unknown_0[0xc];
    WeaponDef* def;                    // +0xc, the weapon that fired
    unsigned int muzzleAimFromDeltaZ;  // +0x10
    char unknown_14[0x16 - 0x14];
    short heading;                     // +0x16
    short pitch;                       // +0x18
    char unknown_1a[0x1b - 0x1a];
    unsigned char flags;               // +0x1b
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

void __stdcall InitProjectile(Proj_0049cde0* proj, WeaponDef* shot, Vec3* pos,
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
        InitProjectile(proj, shot->def, pos, 0, g_game->gameTick, unit);
        proj->angle = shot->heading;
        proj->pitch0 = shot->pitch;
        // The whole product is its own statement: inside the assignment the
        // division and multiply sink after the call.
        int q = (shot->muzzleAimFromDeltaZ / shot->def->speed) * g_game->gravity;
        proj->diry = FUN_004b70ef(shot->pitch, shot->def->speed) - q;
        int scale = FUN_004b7123(shot->pitch, shot->def->speed);
        proj->dirx = -FUN_004b70ef(shot->heading, scale);
        proj->dirz = -FUN_004b7123(shot->heading, scale);
        if (shot->def->flags.b.burnBlow) {
            proj->time = (int)_hypot((double)(pos->x - aim->x), (double)(pos->z - aim->z)) / scale
                       + g_game->gameTick;
        } else {
            proj->time = g_game->gameTick + shot->def->lifetime;
        }
        // active before targetUnit: the reverse of the natural order is the original's.
        proj->active = shot->def->burst;
        proj->targetUnit = param_5;
        unit->anims->StartScript(g_fireScriptNames[(shot->flags >> 2) & 3], 0, 0);
        short angle = unit->aim_0049cde0[(shot->flags >> 2) & 3][0] - unit->heading;
        int a = -FUN_004b70ef(angle, 800);
        int b = -FUN_004b7123(angle, 800);
        unit->anims->StartScriptWithArgs("RockUnit", 0, 0, 2, b, a, 0, 0);
        if (shot->def->flags.b.startSmoke)
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
        InitProjectile(proj, shot->weapon, pos, 0, g_game->gameTick, unit);
        proj->Setup(unit);
        proj->dirX = -FUN_004b70ef(proj->angle, unit->type->range);
        proj->dirZ = -FUN_004b7123(proj->angle, unit->type->range);
        return 1;
    }
    return 0;
}

struct Object_0049d0c0 {
    char unknown_0[0xc];
    WeaponDef* info;                   // +0xc
};

int __stdcall FireLineOfSightProjectile(Object_0049d0c0* obj, int a, int b, int c, int d);
int __stdcall FireBallisticProjectile(Object_0049d0c0* obj, int a, int b, int c, int d);

// The last two arguments are passed to both callees in swapped order.
// FUNCTION: 0x49d0c0
int __stdcall FireWeaponByFlags(Object_0049d0c0* obj, int a, int b, int d, int c)
{
    int result = 0;
    if (obj->info->flags.b.lineOfSight || obj->info->flags.b.selfProp)
        result = FireLineOfSightProjectile(obj, a, b, c, d);
    else if (obj->info->flags.b.ballistic)
        result = FireBallisticProjectile(obj, a, b, c, d);
    return result;
}

#pragma pack(push, 1)
struct Entry_0049d120 {              // 0x1c bytes
    char unknown_0[0xc];
    WeaponDef* def;                  // +0xc
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
    WeaponDef* def;                  // +0x0
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
            if (proj->def->flags.b.targetable) {
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

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049d270(int a, int b, int c, int d, int e, int f);
#pragma pack(pop)

static inline int SamePos_0049d270(Vec3& a, Vec3& b)
{
    return a.x == b.x && a.z == b.z && a.y == b.y;
}

// The projectile scan of 0x49d1e0, defined here unannotated so that /Ob2
// inlines it into ApplyWeaponFirePacket as it did in the original. It has no
// callers in the exe. Written with the positive `if (ev->flag) { ... } return 0;`
// test, which also matches 0x49d1e0 on its own; the `if (!ev->flag) return 0;`
// spelling above compiles to the same standalone bytes but, inlined here,
// gives the cursor a register and spills `unit`.
Proj_0049d270* __stdcall FindRemoteProjectile_0049d270(WeaponFirePacket* ev)
{
    if (ev->flag) {
        char me = g_game->localPlayer;
        Proj_0049d270* proj = (Proj_0049d270*)g_game->projectiles;
        for (int i = 0; i < g_game->projCount; i++, proj++) {
            if (proj->player != me && SamePos_0049d270(proj->aim, ev->aimOrVel) && proj->owner->f_a8u == ev->targetUnitId)
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
void __stdcall ApplyWeaponFirePacket(int arg1, WeaponFirePacket* ev)
{
    unsigned char defIndex = ev->weaponDefIndex;
    WeaponDef* def = &g_game->weaponDefs[defIndex];
    if (def->flags.b.meteor) {
        Vec3* pos = &ev->aimOrVel;
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
    Unit* unit = ev->ownerUnitId == 0 ? 0 : &g_game->units[ev->ownerUnitId];
    if (!unit)
        return;
    if (!unit->b28)
        return;
    Entry_0049d270* entry = &unit->entry[ev->weaponSlotIndex];
    entry->f_18 = ev->firePitch;
    entry->f_16 = ev->fireHeading;
    unsigned short targetId = ev->targetUnitId;
    Unit* target = targetId == 0 ? 0 : &g_game->units[targetId];
    Proj_0049d270* found = FindRemoteProjectile_0049d270(ev);
    if (def->flags.b.ballistic) {
        FireBallisticProjectile(entry, unit, (void*)((char*)ev + 1), &ev->aimOrVel, target);
        return;
    }
    if (def->flags.b.vLaunch) {
        FireVLaunchProjectile(entry, unit, (void*)((char*)ev + 1), &ev->aimOrVel, target, found);
        return;
    }
    if (def->flags.b.lineOfSight || def->flags.b.selfProp) {
        FireLineOfSightProjectile(entry, unit, (void*)((char*)ev + 1), &ev->aimOrVel, target);
        return;
    }
    if (def->flags.b.dropped) {
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
// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049d580(int a, int b, int c, int d, int e, int f);
#pragma pack(pop)

void __stdcall GetAimFromPosition(Unit* obj, Vec3* out, unsigned char weapon);
void __stdcall GetWeaponPiecePosition(Unit* obj, Vec3* out, unsigned char weapon, int piece);
short __stdcall SolveLaunchAngle(int dx, int dy, int dz, int speed, float pitch);
// The weapon parameter must stay an unsigned char.
int __stdcall CalcAimAngles(Unit* unit, WeaponDef* target, short* out_heading,
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
        WeaponDef* def = unit->f_c;
        short heading;
        short pitch;
        int ok;
        if (def->flags.b.ballistic) {
            Vec3 p;
            GetAimFromPosition(fire, &p, unit->f_1b >> 2 & 3);
            int dx = p.x - point->x;
            int dy = p.y - point->y;
            int dz = p.z - point->z;
            heading = FUN_004b715a(dx, dz) - fire->f_66;
            pitch = SolveLaunchAngle(dx, dy, dz, def->speed, def->pitch);
            ok = (unsigned short)pitch != 0x8000;
        } else if (def->flags.b.lineOfSight) {
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
        short spread = unit->f_c->accuracy - (short)((fire->health << 11) / fire->utype->maxHealth) + 0x800;
        int parts = fire->killCount / 12;
        if (parts > 1)
            spread = (unsigned short)spread / parts;
        if (spread) {
            // Keep `range >> 1` inline, no `half` local.
            unsigned short range = spread;
            unit->f_16 += (short)(RandomInt(range) - (range >> 1));
            unit->f_18 += (short)(RandomInt(range) - (range >> 1));
        }
        int fired = 0;
        if (unit->f_c->flags.b.lineOfSight || unit->f_c->flags.b.selfProp)
            fired = FireLineOfSightProjectile(unit, fire, &gunpos, point, target);
        else if (unit->f_c->flags.b.ballistic)
            fired = FireBallisticProjectile(unit, fire, &gunpos, point, target);
        if (!fired)
            return 0;
        unit->f_8 = 0;
        unit->f_1b &= 0xfe;
        if (g_game->flags & 1) {
            WeaponFirePacket msg;
            msg.type = 0xd;
            msg.origin = gunpos;
            msg.aimOrVel = *point;
            msg.weaponDefIndex = unit->f_c->index;
            msg.weaponSlotIndex = unit->f_1b >> 2 & 3;
            if (fire == 0)
                msg.ownerUnitId = 0;
            else
                msg.ownerUnitId = fire->f_a8;
            msg.targetUnitId = target ? target->f_a8 : 0;
            msg.fireHeading = unit->f_16;
            msg.firePitch = unit->f_18;
            msg.flag = unit->f_c->flags.value >> 30;
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

struct Aim_0049d880 {
    char unknown_0[0xc];
    WeaponDef* type;                   // +0xc
    char unknown_10[0x16 - 0x10];
    short fireHeading;                 // +0x16
    short firePitch;                   // +0x18
};

// True when both of the aim object's angles are within tolerance of the
// corresponding unit angles. With no tolerance in the type, a moved unit
// (flags 0xc) gets a generous one, a still unit a tighter one.
// FUNCTION: 0x49d880
int __stdcall AimWithinTolerance(Unit* unit, Aim_0049d880* aim, short angle1, short angle2)
{
    unsigned short a = aim->type->tolerance;
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
        if (aim->type->pitchTolerance)
            y = aim->type->pitchTolerance;
        else
            y = a;
    }
    if (abs((short)(aim->fireHeading - angle1)) <= x
        && abs((short)(aim->firePitch - angle2)) <= y)
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
struct Aim_0049d9c0 {
    char unknown_0[0xc];
    WeaponDef* type;                  // +0xc
    char unknown_10[0x16 - 0x10];
    short fireHeading;                // +0x16
    short firePitch;                  // +0x18
    char unknown_1a;
    unsigned char weapon;             // +0x1b
};

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049d9c0(int a, int b, int c, int d, int e, int f);
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
    aim->fireHeading = (short)FUN_004b715a(dx, dz);
    aim->firePitch = (short)FUN_004b715a(-dy.whole,
                                       (short)((int)_hypot((double)dx, (double)dz) >> 16));
    if (AimWithinTolerance(unit, aim, unit->heading, unit->pitch)) {
        if (FireLineOfSightProjectile(aim, unit, &p, point, target)) {
            if (g_game->flags & 1) {
                WeaponFirePacket msg;
                msg.type = 0xd;
                msg.origin = unit->pos;
                msg.aimOrVel = *point;
                msg.weaponDefIndex = aim->type->index;
                msg.weaponSlotIndex = aim->weapon >> 2 & 3;
                // if/else testing the unit, not a ternary: the ternary flips the branches.
                if (unit == 0)
                    msg.ownerUnitId = 0;
                else
                    msg.ownerUnitId = unit->id;
                msg.targetUnitId = target ? target->id : 0;
                msg.fireHeading = aim->fireHeading;
                msg.firePitch = aim->firePitch;
                msg.flag = aim->type->flags.value >> 30;
                BroadcastPacket(unit->player->id, &msg, 0x24);
            }
            return 1;
        }
    }
    return 0;
}

#pragma pack(push, 1)
struct Shot_0049db70 {
    char unknown_0[8];
    int piece;
    WeaponDef* def;
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

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049db70(int a, int b, int c, int d, int e, int f);
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
        if (shot->def->flags.b.interceptor) {
            p = FindTargetableProjectile(source, (shot->weapon >> 2) & 3);
            if (!p)
                return 0;
        } else {
            p = 0;
        }
        if (FireVLaunchProjectile(shot, source, &pos, aim, target, p)) {
            if (g_game->flags_2a44 & 1) {
                WeaponFirePacket packet;
                packet.type = 0xd;
                packet.origin = pos;
                packet.aimOrVel = *aim;
                packet.weaponDefIndex = shot->def->index;
                packet.weaponSlotIndex = (shot->weapon >> 2) & 3;
                // Keep the `== 0 ? 0 : team` ternaries: they put the zero store on the fall-through.
                packet.ownerUnitId = source == 0 ? 0 : source->team;
                packet.targetUnitId = target == 0 ? 0 : target->team;
                packet.fireHeading = shot->heading;
                packet.firePitch = shot->pitch;
                packet.flag = shot->def->flags.b.interceptor;
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
struct Aim_0049dd60 {
    char unknown_0[0xc];
    WeaponDef* type;
    char unknown_10[0x16 - 0x10];
    short fireHeading;
    short firePitch;
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

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049dd60(int a, int b, int c, int d, int e, int f);
#pragma pack(pop)

void __stdcall InitProjectile(Projectile_0049dd60*, WeaponDef*, Vec3*, int, int, Unit*);

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
        InitProjectile(projectile, aim->type, &p, 0, g_game->gameTick, unit);
        projectile->heading = unit->heading;
        projectile->speed = 0;
        projectile->velY = 0;
        projectile->velX = -FUN_004b70ef(projectile->heading, unit->owner->id);
        projectile->velZ = -FUN_004b7123(projectile->heading, unit->owner->id);
        if (g_game->flags & 1) {
            WeaponFirePacket packet;
            packet.type = 0xd;
            packet.origin = p;
            packet.aimOrVel = *point;
            packet.weaponDefIndex = aim->type->index;
            packet.weaponSlotIndex = aim->weapon >> 2 & 3;
            if (unit == 0)
                packet.ownerUnitId = 0;
            else
                packet.ownerUnitId = unit->id;
            if (target == 0)
                packet.targetUnitId = 0;
            else
                packet.targetUnitId = target->id;
            packet.fireHeading = aim->fireHeading;
            packet.firePitch = aim->firePitch;
            packet.flag = aim->type->flags.value >> 30;
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

// Unused here: the symbol ids this declaration takes keep the allocation (docs/c2-regalloc.md).
void Unused_0049df10(int a, int b, int c, int d, int e, int f);
#pragma pack(pop)

int __cdecl GetLocalDpid();
void __stdcall InitProjectile(Proj_0049df10*, void*, void*, int, int, void*);

// FUNCTION: 0x49df10
int __stdcall SpawnProjectile(WeaponDef* weapon, Vec3* a, Vec3* b, int flag)
{
    Proj_0049df10* proj = 0;
    if (g_game->projCount < 300) {
        proj = &((Proj_0049df10*)g_game->projectiles)[g_game->projCount++];
        proj->flags &= ~2;
        proj->targetUnit = 0;
    }
    if (!proj)
        return 0;
    InitProjectile(proj, weapon, a, 0, g_game->gameTick, 0);
    proj->dir = *b;
    if (flag) {
        // Read into a local first: only then does MSVC hoist the byte load
        // above the g_game->flags_2a44 test, into the low byte of the register
        // that held `weapon`, which is what leaves al free for the `lea` and
        // pushes the +0xd store and the type store in their original places.
        unsigned char team = weapon->index;
        if (g_game->flags_2a44 & 1) {
            WeaponFirePacket packet;
            packet.type = 0xd;
            packet.origin = *a;
            packet.aimOrVel = *b;
            packet.weaponDefIndex = team;
            BroadcastPacket(GetLocalDpid(), &packet, 0x24);
        }
    }
    return 1;
}

typedef int (__stdcall* WeaponFireFn)(void* unit, void* aim, void* target, void* point);

int __stdcall FireTurretWeapon(void* unit, void* aim, void* target, void* point);
int __stdcall FireVLaunchWeapon(void* unit, void* aim, void* target, void* point);
int __stdcall FireDroppedWeapon(void* unit, void* aim, void* target, void* point);
int __stdcall FireLineOfSightWeapon(void* unit, void* aim, void* target, void* point);

// Picks the routine for the object's flags.
// FUNCTION: 0x49e010
void __stdcall SetWeaponFireHandler(WeaponDef* weapon)
{
    if (weapon->flags.b.turret) {
        weapon->fire = (WeaponFireFn)FireTurretWeapon;
        return;
    }
    if (weapon->flags.b.vLaunch) {
        weapon->fire = (WeaponFireFn)FireVLaunchWeapon;
        return;
    }
    if (weapon->flags.b.lineOfSight || weapon->flags.b.selfProp)
        weapon->fire = (WeaponFireFn)FireLineOfSightWeapon;
    else if (weapon->flags.b.dropped)
        weapon->fire = (WeaponFireFn)FireDroppedWeapon;
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
        UnitWeaponSlot* s = &unit->slots[frame.i];
        s->reloadTimer = 0;
        s->flags = (s->flags & 0xf2) | ((frame.i & 3) << 2);
        s->weapon = unit->utype->weapons[frame.i];
        // Read index inline with no named temporary; stockpile is zeroed after this line.
        s->flags = (s->flags & 0xfd) | (((unit->utype->weapons[frame.i]->index != 0) & 1 | 8) * 2);
        s->stockpile = 0;
        Vec3 a;
        GetWeaponPiecePosition(unit, &a, frame.i, -1);
        Vec3 b;
        GetAimFromPosition(unit, &b, frame.i);
        s->muzzleAimFromDeltaZ = (a.z - b.z) * 1.25;
        if (s->weapon->reloadTime > frame.maxTime)
            frame.maxTime = s->weapon->reloadTime;
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
int __stdcall CalcAimAngles(Unit* unit, WeaponDef* target,
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
        Entry_0049e1a0* e = &unit->weapons[i];
        unsigned char fl = e->flags;
        WeaponDef* attached = e->attached;
        if (!(fl & 2))
            continue;
        if (e->f_14 > 0)
            e->f_14--;
        if (!GetWeaponTargetPos(unit, &pos, i)) {
            e->flags &= 0xfe;
            continue;
        }
        if (attached->fire == 0)
            continue;
        if (attached->flags.b.turret) {
            if (!(e->flags & 1)) {
                WeaponDef* t = e->attached;
                unsigned short angle;
                int ok;
                if (t->flags.b.ballistic) {
                    GetAimFromPosition(unit, &aim, (unsigned char)((e->flags >> 2) & 3));
                    SubtractVec3(&aim, &pos, &dx, &dy, &dz);
                    heading = (unsigned short)(FUN_004b715a(dx, dz) - unit->heading);
                    angle = SolveLaunchAngle(dx, dy, dz, t->speed, t->pitch);
                    ok = (angle != 0x8000);
                } else if (t->flags.b.lineOfSight) {
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
                    unit->script->StartScriptWithArgs(g_aimScriptNames[(e->flags >> 2) & 3], (Callback*)&e->name, 0, 2,
                                               heading, angle, 0, 0);
                    SendScriptCallByName(unit, g_aimScriptNames[(e->flags >> 2) & 3], 2, heading, angle, 0, 0);
                    e->flags |= 1;
                }
            }
        } else {
            // Via a bool local: testing the bitfield in place shifts the register rotation.
            bool armed = attached->flags.b.vLaunch;
            if (armed && (!attached->flags.b.stockpile || e->f_1a) && !(e->flags & 1)) {
                e->f_8 = 0;
                unit->script->StartScriptWithArgs(g_aimScriptNames[(e->flags >> 2) & 3], (Callback*)&e->name, 0, 2, 0, 0,
                                           0, 0);
                SendScriptCallByName(unit, g_aimScriptNames[(e->flags >> 2) & 3], 2, 0, 0, 0, 0);
                e->flags |= 1;
            }
        }
        if (e->f_14 != 0)
            continue;
        if (WeaponCanReachPos(unit, &unit->pos, &pos, i)) {
            int can = 0;
            if (attached->flags.b.stockpile) {
                if (e->f_1a)
                    can = 1;
            } else {
                if (unit->resourceSlot.player->energy >= attached->energyPerShot &&
                    unit->resourceSlot.player->metal >= attached->metalPerShot)
                    can = 1;
            }
            if (can == 0)
                continue;
            Unit* fired = GetWeaponTargetUnit(unit, i);
            if (((WeaponFireFn)attached->fire)(unit, &e->point, fired, &pos) == 0)
                continue;
            if (attached->flags.b.stockpile) {
                e->f_1a--;
                UpdateBuildMenuIfFocusUnit(unit);
            } else {
                int n = unit->killCount / 5;
                if (n > 5)
                    n = 5;
                int q = unit->health * 20 / unit->utype->maxHealth;
                int pct = 100 - n * 6;
                e->f_14 = (short)((120 - q) * (pct * attached->reloadTime / 100) / 100);
            }
            int m = (attached->flags.b.commandFire) ? 0x800 : 0x400;
            unit->f_ba.w |= m;
            if (!attached->flags.b.stockpile)
                unit->resourceSlot.SpendEnergyAndMetal(attached->energyPerShot, attached->metalPerShot);
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
        char* entry = g_game->weaponDefs[i].name;
        if (_strcmpi(entry, name) == 0)
            return entry;
    }
    return 0;
}

// Unused here: real functions declared for their symbol ids, which keep
// 0x49b090 and 0x49be60 matching (docs/c2-regalloc.md).
void DrawTriangleContours(void*, int, int, int, int, int, int);
void DrawRangeCircle(void*, void*, void*, int, int, const char*, int);
void AddOrder(int, int, void*, void*, void*, int, int);
void IssueOrCancelOrder(unsigned char, int, void*, int, int*, int, int);
int SendScriptCall(void*, short, char, int, int, int, int);
void AddTextGadget(void*, char*, char*, int, short, int, int);
int HAPINET_updategameinfo(void*, char*, char*, int, int, int, int);
int IsFootprintVisible(void*, int, int, int, int, short);
void DrawProgressBar(void*, int, int, void*, void*, int);
void IssueOrderToSelection(void*, unsigned char, int, int*, int, int);
void WriteScreenshot(char*, char*, int, int, int, int);
void DrawTextClipped(void*, char*, int, int, int, int);
int OpenInputDialog(void*, char*, char*, char*, char*, int);
int ScanDirectory(char*, char*, char*, int, int, int);
int AccessRegistryValue(char*, char*, unsigned char*, unsigned long*, unsigned long, unsigned long);
double DotProduct(float, float, float, float, float, float);
void DrawFrameScaledBlended(void*, void*, int, int, double, double);
void DrawFrameScaled(void*, void*, int, int, double, double);
void* DrawLitLine(void*, int, int, int, int, int);
void* DrawBlendedLine(void*, int, int, int, int, int);
void OrderSquad(void*, int, unsigned char, int, void*, int, int, int);
int AddBlinkWord(void*, const char*, int, int, int, int, float, float);

// 0x49b090 and 0x49be60 sit here, after the module's other functions, and
// 0x49be60 after the TdfFile global below.
#pragma pack(push, 1)

// The feature records are an array of these, indexed as `features[f * 256]`.
struct MapFeature_0049b090 {
    char unknown_0[0xfa];
    unsigned char height;             // +0xfa
    char unknown_fb[0x100 - 0xfb];
};

union Flags_0049b090 {
    unsigned char value;
    struct {
        unsigned char b0 : 1;
        unsigned char dead : 1;
        unsigned char rest : 6;
    } bits;
};

// The world position is three ints at +0x4 and, in the same bytes, three shorts
// at +0x6, +0xa and +0xe: the map-cell coordinates. A union is the only way to
// get the two views, and it is the shorts the code reaches for the cell.
union WordPair_0049b090 {
    int i;
    struct {
        char lo[2];
        short hi;
    } s;
};

struct Proj_0049b090 {
    WeaponDef* type;                   // +0x0
    WordPair_0049b090 px;              // +0x4 (short at +0x6)
    WordPair_0049b090 py;              // +0x8 (short at +0xa)
    WordPair_0049b090 pz;              // +0xc (short at +0xe)
    char unknown_10[0x20 - 0x10];
    int velY;                          // +0x20
    char unknown_24[0x56 - 0x24];
    Proj_0049b090* intercepted;        // +0x56, the projectile this one is chasing
    short cellX;                       // +0x5a
    short cellZ;                       // +0x5c
    short radius;                      // +0x5e
    char unknown_60[0x66 - 0x60];
    unsigned char owner;               // +0x66
    char unknown_67[2];
    Flags_0049b090 flags;              // +0x69
};
#pragma pack(pop)

void __stdcall DetonateProjectile(Proj_0049b090* proj, Unit* unit);

// The projectile collision test. It looks up the map cell holding the
// projectile; with none it drops the camera follow, sets the dead flag and
// returns. Otherwise it kills a projectile that reaches the one it is chasing,
// sets the projectile's floor radius, tests the cell's two units against the
// owner and their height window, resolves the map feature (including the
// 0xfffe "read the neighbour cell" case), and applies the flag rules, the sea
// level ceiling and the net game check before the final kill.
// The 0xfffe reload path re-tests the feature id only against 0xfffb and skips
// the feature count check, so an id read from the neighbouring cell indexes
// g_game->features unchecked (an original bug).
// FUNCTION: 0x49b090
void __stdcall CheckProjectileCollision(WeaponDef* type, Proj_0049b090* proj)
{
    // No `Game* g = g_game` local: g_game is read at each use to stay in edi.
    // Named pos local, used again after the lookup: gives the original's prologue.
    Vec3* pos = (Vec3*)&proj->px;
    Cell* cell = GetMapCellAtPosition(pos);

    if (!cell) {
        if (proj == (Proj_0049b090*)g_game->cameraFollowTrackObj) {
            g_game->cameraSnapPos = *(Vec3*)&((Proj_0049b090*)g_game->cameraFollowTrackObj)->px;
            g_game->cameraSnapTicks = proj->type->holdTime;
            g_game->cameraFollowTrackObj = 0;
        }
        proj->flags.bits.dead = 1;
        return;
    }
    if (proj->intercepted) {
        int dx = pos->x - proj->intercepted->px.i;
        int dy = pos->y - proj->intercepted->py.i;
        int dz = pos->z - proj->intercepted->pz.i;
        int r = proj->type->areaOfEffect;
        int d = (int)(((__int64)dx * dx) >> 32) + (int)(((__int64)dy * dy) >> 32)
            + (int)(((__int64)dz * dz) >> 32);
        if (d < r * r)
            DetonateProjectile(proj, 0);
    }
    proj->radius = (cell->high + cell->low) / 2;
    if (cell->unit) {
        Unit* u = &g_game->units[cell->unit];
        if (u->playerIndex != proj->owner && proj->py.i < u->utype->modelMaxY + u->pos.y) {
            DetonateProjectile(proj, u);
            return;
        }
    }
    if (cell->unit2) {
        Unit* u = &g_game->units[cell->unit2];
        if (u->playerIndex != proj->owner) {
            if (proj->py.i >= u->utype->modelMinY + u->pos.y
                && proj->py.i <= u->utype->modelMaxY + u->pos.y) {
                DetonateProjectile(proj, u);
                return;
            }
        }
    }
    // Mask tests (here bit 14, unitsOnly, and bit 16, waterWeapon below), not the
    // bit-fields: the bit-field form changes the code.
    if (type->flags.value & 0x4000)
        return;
    {
        short cx = proj->px.s.hi / 16;
        short cz = proj->pz.s.hi / 16;
        unsigned short f = cell->feature;
        MapFeature_0049b090* mf;
        // The `!(f == 0xfffe)` chain shape sets the block order.
        if (f < 0xfffb) {
            if (f < g_game->featureCount)
                mf = g_game->features + f;
            else
                mf = 0;
        } else if (!(f == 0xfffe)) {
            mf = 0;
        } else {
            // This index spelling and `unsigned short f2` merge the feature tails.
            int n = cell->origin.offsetX + g_game->mapWidthTiles * cell->origin.offsetZ;
            unsigned short f2 = (cell - n)->feature;
            if (f2 >= 0xfffb) {
                mf = 0;
            } else {
                mf = g_game->features + f2;
            }
        }
        if (mf) {
            // Positive direction (sum > hi, else mf = 0): sets the compare operand order.
            if (mf->height + cell->low > proj->py.s.hi) {
                if (proj->cellX == cx && proj->cellZ == cz) {
                    mf = 0;
                } else {
                    proj->cellX = cx;
                    proj->cellZ = cz;
                }
            } else {
                mf = 0;
            }
        }
        if (mf) {
            DetonateProjectile(proj, 0);
            return;
        }
    }
    if (cell->low > proj->py.s.hi) {
        if (type->flags.b.groundBounce) {
            proj->velY = -(proj->velY >> 2);
            return;
        }
    } else if (type->flags.value & 0x10000) {
        return;
    } else if (proj->py.s.hi >= g_game->seaLevel) {
        return;
    } else if (g_game->mapInfo->noSeaLevelTrigger) {
        return;
    }
    DetonateProjectile(proj, 0);
}

#include "../util/tdf.h"

// A global object of the 12-byte class constructed by 0x4c2ea0 and destroyed
// by 0x4c2eb0. The compiler generates its initialiser (0x49e610) and the
// destructor it registers with atexit (0x49e630).
// FUNCTION: 0x49e610 _$E5
// FUNCTION: 0x49e630 _$E3
TdfFile g_tdfGlobalParser;

// 0x49be60 sits after the TdfFile global: its palette and scroll register plan
// follows the symbol ids.
#pragma pack(push, 1)
struct Sprite_0049be60 {
    char unknown_0[0x30];
    void* child;                       // +0x30
};

struct Angles_0049be60 {
    short x;
    short y;
    short z;
};

struct Proj_0049be60 {
    WeaponDef* type;                   // +0x0
    Vec3 pos;                          // +0x4
    Vec3 start;                        // +0x10
    char unknown_1c[0x34 - 0x1c];
    Angles_0049be60 angles;            // +0x34
    char unknown_3a[0x42 - 0x3a];
    int spawnTick;                     // +0x42
    int time;                          // +0x46
    char unknown_4a[0x5e - 0x4a];
    short groundHeightAvg;             // +0x5e
    short counter;                     // +0x60
    char unknown_62[0x64 - 0x62];
    short propellerSpin;               // +0x64
    char unknown_66[0x69 - 0x66];
    unsigned short flags;              // +0x69
};

struct MapSize_0049be60 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_0049be60 {
    unsigned char* data;               // +0x0
    MapSize_0049be60 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct PlayerInfo_0049be60 {
    char unknown_0[0x7c];
    ByteMap_0049be60 explored;         // +0x7c
};
#pragma pack(pop)

void* __stdcall GetGafFrame(void* gaf, int frame);
void __stdcall DrawFrame(void* dest, void* src, int x, int y);
void __stdcall DrawFrameBlended(void* dest, void* src, int x, int y);
void __stdcall DrawModel3doProjected(void* dest, Vec3* pos, void* sprite, void* rect);
int __stdcall PointInRect(void* region, int x, int y);
void __stdcall DrawLens(void* dest, void* src, int x, int y);
void __stdcall DrawLine(void* dest, int x1, int y1, int x2, int y2, unsigned int color);
int __stdcall GetGafFrameCount(void* gaf);
int __stdcall IsPointVisible(PlayerInfo_0049be60* pi, Vec3* pos);

// The projectile render pass: it walks the projectile array and draws every
// live projectile (counter 0) that the local player can see, by the shot kind
// (0 to 7). With map-flags bit 2 set the visibility test reads the player's
// explored-cell grid, otherwise it calls IsPointVisible.
//  - Kind 0's two line arms each make both DrawLine calls (colour2, then
//    colour1).
//  - Kinds 1, 3 and 6 pass a 3-short angle struct (the projectile's +0x34);
//    kind 3 passes its own `rot3`, which is never written (docs/bugs.md).
//  - Kind 7's jitter is 64-bit, `(rand() * 11) / 0x8000 - 5`, added to the
//    high shorts of pt.
// FUNCTION: 0x49be60
void __stdcall DrawProjectiles(void* surface)
{
    WeaponDef* type;
    int fr;
    Vec3 sp;
    Vec3 pt;
    Vec3 prev;
    Vec3 d;
    __int64 n64;
    int time = g_game->gameTick;
    void* frame0 = GetGafFrame(g_game->shadowSeq, 0);
    int index = 0;
    int visible;
    if (g_game->projectileCount <= 0)
        return;
    int offset = 0;
    while (1) {
        Proj_0049be60* p = (Proj_0049be60*)((char*)g_game->projectiles + offset);
        if (p->counter == 0) {
            unsigned char player = g_game->playerIndex;
            char* pb = (char*)g_game + 0x1b63 + 0x14b * player;
            Vec3* pos = &p->pos;
            if ((g_game->mapFlags & 2) == 2) {
                int col = (int)*(short*)((char*)pos + 2) >> 5;
                PlayerInfo_0049be60* pi = (PlayerInfo_0049be60*)pb;
                int row = ((int)*(short*)((char*)pos + 10)
                           - ((int)*(short*)((char*)pos + 6) >> 1)) >> 5;
                if (pi->explored.size.Contains(col, row) && pi->explored.Get(col, row))
                    visible = 1;
                else
                    visible = 0;
            } else {
                visible = IsPointVisible((PlayerInfo_0049be60*)pb, pos);
            }
            if (visible) {
                type = p->type;
                if (type->shotKind == 0) {
                    unsigned int color1 = g_game->palette[type->colour];
                    unsigned int color2 = g_game->palette[type->colour2];
                    int x1;
                    int y1;
                    int x2;
                    int y2;
                    x1 = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    y1 = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    x2 = (int)*(short*)((char*)&p->start + 2) - (short)g_game->scrollX + 0x80;
                    y2 = ((int)*(short*)((char*)&p->start + 10)
                              - ((int)*(short*)((char*)&p->start + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (type->colour2 != 0) {
                        // Both calls stay in each arm: one shared colour1 call swaps the colour slots.
                        if (abs(x1 - x2) > abs(y1 - y2)) {
                            if (x1 > x2) {
                                int t = x1; x1 = x2; x2 = t;
                                t = y1; y1 = y2; y2 = t;
                            }
                            DrawLine(surface, x1, y1 - 1, x2, y2 - 1, color2);
                            DrawLine(surface, x1, y1, x2, y2, color1);
                        } else {
                            if (y1 > y2) {
                                int t = x1; x1 = x2; x2 = t;
                                t = y1; y1 = y2; y2 = t;
                            }
                            DrawLine(surface, x1 - 1, y1, x2 + 1, y2, color2);
                            DrawLine(surface, x1, y1, x2, y2, color1);
                        }
                    } else {
                        DrawLine(surface, x1, y1, x2, y2, color1);
                    }
                } else if (type->shotKind == 1) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = 0x80 + (int)*(short*)((char*)&sp + 2);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20;
                    DrawFrameBlended(surface, frame0, sx, sy);
                    Angles_0049be60 rot = p->angles;
                    rot.y += 0x8000;
                    rot.z += 0x8000;
                    DrawModel3doProjected(surface, &sp, type->model, &rot);
                    Sprite_0049be60* s = (Sprite_0049be60*)type->model;
                    if (0 != s->child && p->time > time) {
                        if (type->flags.b.propeller) {
                            rot.x = p->propellerSpin;
                            DrawModel3doProjected(surface, &sp, s->child, &rot);
                        } else {
                            DrawModel3doProjected(surface, &sp, s->child, &rot);
                        }
                    }
                } else if (type->shotKind == 2) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    if (PointInRect(g_game->region_37e27, sx, sy) == 0)
                        return;
                    DrawLens(surface, g_game->explosionLensFrame, sx, sy);
                } else if (type->shotKind == 3) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20;
                    DrawFrameBlended(surface, frame0, sx, sy);
                    Angles_0049be60 rot3;
                    DrawModel3doProjected(surface, &sp, type->model, &rot3);
                } else if (type->shotKind == 4) {
                    if (type->colour < 0xff) {
                        void* gaf = 0;
                        sp.x = pos->x - (g_game->scrollX << 16);
                        sp.y = p->pos.y;
                        sp.z = p->pos.z - (g_game->scrollY << 16);
                        DrawFrameBlended(surface, frame0,
                                     (int)*(short*)((char*)&sp + 2) + 0x80,
                                     (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20);
                        int sy = ((int)*(short*)((char*)&sp + 10)
                                  - ((int)*(short*)((char*)&sp + 6) >> 1)) + 0x20;
                        int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                        switch (type->colour) {
                        case 0: gaf = g_game->cannonShellSeq; break;
                        case 1: gaf = g_game->plasmaSmSeq; break;
                        case 2: gaf = g_game->plasmaMdSeq; break;
                        case 3: gaf = g_game->ultraShellSeq; break;
                        case 4: gaf = g_game->plasmaSmSeq2; break;
                        }
                        if (gaf) {
                            int n = *(unsigned short*)gaf;
                            DrawFrame(surface, GetGafFrame(gaf, (time - p->spawnTick) % n), sx, sy);
                        }
                    }
                } else if (type->shotKind == 5) {
                    int sx = (int)*(short*)((char*)pos + 2) - (short)g_game->scrollX + 0x80;
                    int sy = ((int)*(short*)((char*)pos + 10)
                              - ((int)*(short*)((char*)pos + 6) >> 1))
                             - (short)g_game->scrollY + 0x20;
                    void* gaf = g_game->flameStreamSeq;
                    int n = GetGafFrameCount(gaf);
                    fr = n - ((p->time - time) * n) / (int)type->lifetime;
                    if (fr >= 0 && fr < n) {
                        DrawFrameBlended(surface, GetGafFrame(gaf, fr), sx, sy);
                    }
                } else if (type->shotKind == 6) {
                    sp.x = pos->x - (g_game->scrollX << 16);
                    sp.y = p->pos.y;
                    sp.z = p->pos.z - (g_game->scrollY << 16);
                    int sy = (int)*(short*)((char*)&sp + 10) - ((unsigned short)p->groundHeightAvg >> 1) + 0x20;
                    int sx = (int)*(short*)((char*)&sp + 2) + 0x80;
                    DrawFrameBlended(surface, frame0, sx, sy);
                    DrawModel3doProjected(surface, &sp, type->model, &p->angles);
                } else if (type->shotKind == 7) {
                    unsigned int color = g_game->palette[type->colour];
                    Vec3* start = &p->start;
                    d.x = pos->x - start->x;
                    d.y = pos->y - start->y;
                    d.z = pos->z - start->z;
                    union { int i; short s[2]; } nSeg;
                    nSeg.i = (int)(((__int64)((int)sqrt(d.x * (double)d.x + (double)d.y * d.y + (double)d.z * d.z)) << 16) / 0x50000);
                    if (0 != nSeg.i) {
                        n64 = nSeg.i;
                        d.x = (int)(((__int64)d.x << 16) / n64);
                        d.y = (int)(((__int64)d.y << 16) / n64);
                        d.z = (int)(((__int64)d.z << 16) / n64);
                        for (int outer = 0; outer < 2; outer++) {
                            pt = *start;
                            sp = *start;
                            {
                                short n = nSeg.s[1];
                                if (n > 0) {
                                    int i = n;
                                    do {
                                        // The unused z stays: it gives pt.z the extra reference
                                        // that sets the register assignment.
                                        int z = pt.z;  // unused; needed for the match
                                        prev = pt;
                                        sp.x += d.x;
                                        sp.y += d.y;
                                        sp.z += d.z;
                                        pt = sp;
                                        *(short*)((char*)&pt + 2) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        *(short*)((char*)&pt + 6) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        *(short*)((char*)&pt + 10) +=
                                            (short)((int)(((__int64)rand() * 11) / 0x8000) - 5);
                                        DrawLine(surface,
                                            (int)*(short*)(2 + (char*)&prev) - (short)g_game->scrollX + 0x80,
                                            ((int)*(short*)(10 + (char*)&prev)
                                             - ((int)*(short*)((char*)&prev + 6) >> 1))
                                            - (short)g_game->scrollY + 0x20,
                                            (int)*(short*)((char*)&pt + 2) - (short)g_game->scrollX + 0x80,
                                            0x20 + (((int)*(short*)((char*)&pt + 10)
                                             - ((int)*(short*)(6 + (char*)&pt) >> 1))
                                            - (short)g_game->scrollY),
                                            color);
                                        i = i - 1;
                                    } while (i != 0);
                                }
                            }
                        }
                    }
                }
            }
        }
        index = index + 1;
        offset += 0x6b;
        if (index >= g_game->projectileCount)
            break;
    }
}
