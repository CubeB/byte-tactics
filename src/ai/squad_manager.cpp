// Decompiled by Opus, GPT-6, Claude Opus 5.5 and Sonnet. Names are provisional.
// SquadManager: one per computer player, the owner of the SquadTimer family
// (src/ai/squad_timer.cpp). It runs the squads' timers, assigns units to
// squads and retargets their weapons.

// Any header (here <windows.h>) fixes the base/index order in the
// RetargetWeapon that RetargetWeapons inlines.
#include <windows.h>

#pragma pack(push, 1)
struct Target_00406f50 {
    char unknown_0[0xbb];
    unsigned short bit0_4 : 5;         // +0xbb
    unsigned short flag5 : 1;          // 0x20
    unsigned short flag6 : 1;          // 0x40
    unsigned short bit7_15 : 9;
};

struct Obj_00406f50 {
    char unknown_0[0x52];
    Target_00406f50* target;           // +0x52
};

struct WeaponDef {
    char unknown_0[0x111];
    unsigned int unknown_bits0 : 7;    // +0x111
    unsigned int flag7 : 1;
    unsigned int flag8 : 1;
    unsigned int unknown_bits9 : 17;
    unsigned int flag26 : 1;
    unsigned int unknown_bits27 : 3;
    unsigned int flag30 : 1;
    unsigned int unknown_bit31 : 1;
};

struct Def {
    char unknown_0[0x1c0];
    short height;                      // +0x1c0
    char unknown_1c2[0x231 - 0x1c2];
    unsigned int* weaponCategories[3]; // +0x231
    char unknown_23d[0x241 - 0x23d];
    unsigned int unused : 6;           // +0x241
    unsigned int builder : 1;
    unsigned int unused7 : 4;
    unsigned int flying : 1;
    unsigned int unused12 : 20;
    unsigned int unused245 : 12;       // +0x245
    unsigned int special : 1;
    unsigned int unused13 : 19;
};
#pragma pack(pop)

struct Unit;
struct Player;

struct Weapon {                        // 0x1c bytes
    WeaponDef* def;                    // +0x0
    char unknown_4[0xb];
    unsigned char flags;               // +0xf
    char unknown_10[0xc];
};

struct Group_00408cb0 {                // 0x20 bytes
    char unknown_0[0x20];
};

#pragma pack(push, 1)
struct Unit {                          // 0x118 bytes
    char unknown_0[0x10];
    Weapon weapons[3];                 // +0x10
    char unknown_64[0x92 - 0x64];
    Def* def;                          // +0x92
    Player* owner;                     // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short category;           // +0xa6
    char unknown_a8[0xac - 0xa8];
    int group;                         // +0xac
    char unknown_b0[0x104 - 0xb0];
    float progress;                    // +0x104
    char unknown_108[0x10e - 0x108];
    unsigned char field_10e;           // +0x10e
    char unknown_10f;
    unsigned int flags;                // +0x110
    char unknown_114[4];
};

struct Player {
    int unknown_0;                     // +0x0
    char unknown_4[0x67 - 0x4];
    Unit* firstUnit;                   // +0x67
    Unit* lastUnit;                    // +0x6b
    char unknown_6f[0x73 - 0x6f];
    char state;                        // +0x73
    char unknown_74[0x78 - 0x74];
    Group_00408cb0* groups;            // +0x78
    char unknown_7c[0x108 - 0x7c];
    unsigned char allied[0x3e];        // +0x108
    unsigned char index;               // +0x146
};

struct Game {
    char unknown_0[0x14223];
    int baseX;                         // +0x14223
    int baseY;                         // +0x14227
    char unknown_1422b[0x37ee6 - 0x1422b];
    unsigned short field_37ee6;        // +0x37ee6
    char unknown_37ee8[0x38a47 - 0x37ee8];
    unsigned int now;                  // +0x38a47
};
#pragma pack(pop)

extern Game* g_game;

void __stdcall SetUnitSquad(Unit* unit, int squad);
Unit* __stdcall GetWeaponTargetUnit(Unit* unit, int weapon);
int* __stdcall FindTargetableProjectile(Unit* unit, unsigned int weapon);
int __stdcall FindWeaponTarget(Unit* unit, unsigned int weapon, int param_3);
void __stdcall SetWeaponTargetUnit(Unit* unit, int param_2, unsigned int weapon);
void __stdcall SetWeaponTargetPos(Unit* unit, int* param_2, unsigned int weapon);
void __stdcall ClearWeaponTarget(Unit* unit, unsigned int weapon);

static inline int Contains(unsigned int* bits, unsigned short index)
{
    return bits[index >> 5] & (1 << (index & 31));
}

// Matched in ai_player_408920.cpp; defined in the same file, and inlined below.
void __stdcall RetargetWeapon(Unit* unit, unsigned int weapon)
{
    if (unit->weapons[weapon].def->flag30) {
        int* p = FindTargetableProjectile(unit, weapon);
        if (p)
            SetWeaponTargetPos(unit, p + 1, weapon);
        else
            ClearWeaponTarget(unit, weapon);
    } else if ((unit->flags & 0x300000) == 0x200000) {
        int r = FindWeaponTarget(unit, weapon, 1);
        if (r)
            SetWeaponTargetUnit(unit, r, weapon);
        else
            ClearWeaponTarget(unit, weapon);
    }
}

class SquadManager;

// Vtable 0x4fc980, constructor 0x407350, ??_G 0x407390.
class SquadTimer {
public:
    SquadManager* owner;               // +0x4
    void* field_8;                     // +0x8
    unsigned int time;                 // +0xc, of the next run
    unsigned int field_10;             // +0x10

    SquadTimer(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0
    virtual ~SquadTimer() {}                        // slot 1
};

#pragma pack(push, 1)
class SquadManager {                   // 0x3d bytes
public:
    Player* player;                    // +0x0
    unsigned char field_4;             // +0x4
    int countdown;                     // +0x5
    int field_9;                       // +0x9
    int field_d;                       // +0xd
    SquadTimer* timers[10];            // +0x11
    Unit* cursor;                      // +0x39

    SquadManager(Player* p);
    void FUN_00406f50(Obj_00406f50* obj, int a, int b);
    void AssignSquads();
    void RetargetWeapons(int force);
    void TickIfActive();
    void DeleteTimers();
};
#pragma pack(pop)

// Vtable 0x4fc988, constructor 0x407930, ??_G 0x407980.
class Class_00407930 : public SquadTimer {
public:
    int field_14;                      // +0x14
    int field_18;                      // +0x18
    int field_1c;                      // +0x1c
    int field_20;                      // +0x20
    int field_24;                      // +0x24

    Class_00407930(SquadManager* p, void* q, int a, int b);
    virtual void OnTimer();                         // slot 0, 0x4077e0
};

// Vtable 0x4fc990, constructor 0x4079a0, ??_G 0x4079d0.
class Class_004079d0 : public SquadTimer {
public:
    int field_14;                      // +0x14

    Class_004079d0(SquadManager* p, void* q, int a);
    virtual void OnTimer();                         // slot 0, 0x4079f0
};

// Vtable 0x4fc998, constructor 0x407a90, ??_G 0x407ac0.
class Class_00407a90 : public SquadTimer {
public:
    Class_00407a90(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x407ae0
};

struct Vec3_00407d40 {
    int x, y, z;

    Vec3_00407d40() {}
    Vec3_00407d40(Game* game) {
        int ax = (int)(game->baseX / 2 * 65536.0);
        *this = Vec3_00407d40(ax, 0, (int)(game->baseY / 2 * 65536.0));
    }
    Vec3_00407d40(int ax, int ay, int az) : x(ax), y(ay), z(az) {}
};

// Vtable 0x4fc9a0, constructor 0x407d40, ??_G 0x407e70.
class Class_00407d40 : public SquadTimer {
public:
    Vec3_00407d40 a;                   // +0x14
    Vec3_00407d40 b;                   // +0x20
    Vec3_00407d40 c;                   // +0x2c
    int field_38;                      // +0x38

    Class_00407d40(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x407e90
};

// Vtable 0x4fc9a8, constructor 0x4085d0, ??_G 0x408600.
class Class_004085d0 : public SquadTimer {
public:
    Class_004085d0(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x408100
};

// Vtable 0x4fc9b0, constructor 0x4087e0, ??_G 0x408810.
class Class_00408810 : public SquadTimer {
public:
    Class_00408810(SquadManager* p, void* q);
    virtual void OnTimer();                         // slot 0, 0x4086d0
};

// The family constructors (matched in their own files) were defined in the
// same file, before SquadManager's.
SquadTimer::SquadTimer(SquadManager* p, void* q)
    : owner(p), field_8(q), time(0), field_10(p->field_4)
{
}

Class_00407930::Class_00407930(SquadManager* p, void* q, int a, int b)
    : SquadTimer(p, q), field_1c(b), field_20(a)
{
    field_24 = 0;
    field_18 = 6;
    field_14 = 3;
}

Class_004079d0::Class_004079d0(SquadManager* p, void* q, int a)
    : SquadTimer(p, q), field_14(a)
{
}

Class_00407a90::Class_00407a90(SquadManager* p, void* q)
    : SquadTimer(p, q)
{
}

Class_00407d40::Class_00407d40(SquadManager* p, void* q)
    : SquadTimer(p, q), a(g_game), b(g_game), c(g_game), field_38(0)
{
}

Class_004085d0::Class_004085d0(SquadManager* p, void* q)
    : SquadTimer(p, q)
{
}

Class_00408810::Class_00408810(SquadManager* p, void* q)
    : SquadTimer(p, q)
{
}

// FUNCTION: 0x406f50
void SquadManager::FUN_00406f50(Obj_00406f50* obj, int a, int b)
{
    Target_00406f50* t = obj->target;
    if (t) {
        if (a > b * 2) {
            t->flag6 = 1;
            return;
        }
        t->flag5 = 1;
    }
}

// FUNCTION: 0x408830
void SquadManager::AssignSquads()
{
    for (Unit* u = player->firstUnit; u <= player->lastUnit; ++u) {
        if(u->flags&0x20) {
            if(u->def->special) u->flags=(u->flags&~0x80000)|0x40000;
            else u->flags=(u->flags&~0x40000)|0x80000;
            u->flags=(u->flags&~0x100000)|0x200000;
            if(!u->group) {
                if(u->flags&0x20000000) {
                    if(u->flags&0x80000000) SetUnitSquad(u,5);
                    else SetUnitSquad(u,1);
                } else if(u->def->builder) SetUnitSquad(u,4);
                else if(u->def->flying) SetUnitSquad(u,8);
                else if(u->def->height>0) SetUnitSquad(u,7);
                else if(u->flags&0x80000000) SetUnitSquad(u,3);
            }
        }
    }
}

// Walks
// the player's units round-robin through the cursor at +0x39, a slice of them
// per call, and for each finished unit that has a weapon without a valid
// target, retargets that weapon with RetargetWeapon (inlined).
//
// Notes: the weapon loop counter is a byte (MSVC then counts down from 3);
// the bit-8 test needed the (unsigned char) cast to extract with shr; the
// target filter is one `target = 0` whose block MSVC splits per spill state;
// any header (here <windows.h>) fixes the base/index order in the inlined
// RetargetWeapon.
// FUNCTION: 0x4089a0
void SquadManager::RetargetWeapons(int force)
{
    for (int i = 0; i <= g_game->field_37ee6 / 30; i++) {
        if (cursor && cursor != player->lastUnit)
            cursor++;
        else
            cursor = player->firstUnit;
        if (cursor->category != 0 && cursor->progress == 0.0f && (cursor->flags & 0x80000000)
            && (cursor->flags & 0x300000) == 0x200000) {
            for (unsigned char w = 0; w < 3; w++) {
                if ((cursor->weapons[w].flags & 2) && (cursor->weapons[w].flags & 0x10)
                    && !(unsigned char)cursor->weapons[w].def->flag8
                    && (force || !cursor->weapons[w].def->flag26)) {
                    Unit* target = GetWeaponTargetUnit(cursor, w);
                    if (target && (player->allied[target->owner->index]
                        || Contains(cursor->def->weaponCategories[w], target->category)
                        || (cursor->weapons[w].def->flag7 && (target->field_10e & 0x10))))
                        target = 0;
                    if (!target)
                        RetargetWeapon(cursor, w);
                }
            }
        }
    }
}

// FUNCTION: 0x408c40
void SquadManager::TickIfActive()
{
    if (player->unknown_0 != 0 && player->state == 2) {
        if (--countdown <= 0) {
            countdown = 30;
            AssignSquads();
        }
        for (int i = 0; i < 10; i++) {
            if (timers[i] != 0 && timers[i]->time <= g_game->now) {
                timers[i]->OnTimer();
            }
        }
        RetargetWeapons(1);
        return;
    }
    RetargetWeapons(0);
}

// Constructor of SquadManager, the owner of the SquadTimer family (listed
// in src/ai/squad_timer.cpp): one per player, it creates a timer object for nine of its
// ten slots, each given the player's unit group of the same index.
//
// All family constructors are defined in this file before it, as in the
// original translation unit. /Ob2 inlines the first seven; its inlining budget
// then runs out, so the eighth (Class_00407a90) is inlined without its base
// constructor, and Class_00407d40's constructor is called out of line. Without
// the Class_00407d40 body the budget does not run out and nothing matches.
// FUNCTION: 0x408cb0
SquadManager::SquadManager(Player* p)
{
    player = p;
    field_4 = p->index;
    field_9 = 0;
    countdown = 30;
    cursor = 0;
    field_d = 0;
    for (int i = 0; i < 10; i++)
        timers[i] = 0;
    timers[1] = new Class_00408810(this, &player->groups[1]);
    timers[4] = new Class_004085d0(this, &player->groups[4]);
    timers[5] = new SquadTimer(this, &player->groups[5]);
    timers[2] = new Class_00407930(this, &player->groups[2], 3, 20000);
    timers[3] = new Class_004079d0(this, &player->groups[3], 2);
    timers[6] = new Class_00407930(this, &player->groups[6], 7, 50000);
    timers[7] = new Class_004079d0(this, &player->groups[7], 6);
    timers[8] = new Class_00407a90(this, &player->groups[8]);
    timers[9] = new Class_00407d40(this, &player->groups[9]);
}

// FUNCTION: 0x408f10
void SquadManager::DeleteTimers()
{
    for (int i = 0; i < 10; i++) {
        if (timers[i] != 0) {
            delete timers[i];
        }
    }
}
