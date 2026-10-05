// Decompiled by Claude Opus 5.5. Names are provisional.
// A method of the timer owner (constructor 0x408cb0, which names the class
// Class_00408cb0; this one keeps the name already established for it). Walks
// the player's units round-robin through the cursor at +0x39, a slice of them
// per call, and for each finished unit that has a weapon without a valid
// target, retargets that weapon with FUN_00408920 (inlined).
//
// Notes: the weapon loop counter is a byte (MSVC then counts down from 3);
// the bit-8 test needed the (unsigned char) cast to extract with shr; the
// target filter is one `target = 0` whose block MSVC splits per spill state;
// any header (here <windows.h>) fixes the base/index order in the inlined
// FUN_00408920.
#include <windows.h>

#pragma pack(push, 1)
struct WeaponDef_004089a0 {
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

struct UnitDef_004089a0 {
    char unknown_0[0x231];
    unsigned int* weaponCategories[3]; // +0x231
};
#pragma pack(pop)

struct Unit;

struct Weapon_004089a0 {               // 0x1c bytes
    WeaponDef_004089a0* def;           // +0x0
    char unknown_4[0xb];
    unsigned char flags;               // +0xf
    char unknown_10[0xc];
};

#pragma pack(push, 1)
struct Player_004089a0 {
    char unknown_0[0x67];
    Unit* firstUnit;                   // +0x67
    Unit* lastUnit;                    // +0x6b
    char unknown_6f[0x108 - 0x6f];
    unsigned char allied[0x3e];        // +0x108
    unsigned char index;               // +0x146
};

struct Unit {                          // 0x118 bytes
    char unknown_0[0x10];
    Weapon_004089a0 weapons[3];        // +0x10
    char unknown_64[0x92 - 0x64];
    UnitDef_004089a0* def;             // +0x92
    Player_004089a0* owner;            // +0x96
    char unknown_9a[0xa6 - 0x9a];
    unsigned short category;           // +0xa6
    char unknown_a8[0x104 - 0xa8];
    float progress;                    // +0x104
    char unknown_108[0x10e - 0x108];
    unsigned char field_10e;           // +0x10e
    char unknown_10f;
    unsigned int flags;                // +0x110
    char unknown_114[4];
};

struct Game {
    char unknown_0[0x37ee6];
    unsigned short field_37ee6;        // +0x37ee6
};

class Class_004089a0 {                 // 0x3d bytes, laid out in 0x408cb0.cpp
public:
    Player_004089a0* player;           // +0x0
    char unknown_4[0x39 - 0x4];
    Unit* cursor;                      // +0x39

    void FUN_004089a0(int force);
};
#pragma pack(pop)

extern Game* g_game;

Unit* __stdcall FUN_0048a190(Unit* unit, int weapon);
int* __stdcall FUN_0049d120(Unit* unit, unsigned int weapon);
int __stdcall FUN_0040b7b0(Unit* unit, unsigned int weapon, int param_3);
void __stdcall FUN_0048a060(Unit* unit, int param_2, unsigned int weapon);
void __stdcall FUN_0048a0a0(Unit* unit, int* param_2, unsigned int weapon);
void __stdcall FUN_0048a0f0(Unit* unit, unsigned int weapon);

static inline int Contains(unsigned int* bits, unsigned short index)
{
    return bits[index >> 5] & (1 << (index & 31));
}

// Matched in 0x408920.cpp; defined in the same file, and inlined below.
void __stdcall FUN_00408920(Unit* unit, unsigned int weapon)
{
    if (unit->weapons[weapon].def->flag30) {
        int* p = FUN_0049d120(unit, weapon);
        if (p)
            FUN_0048a0a0(unit, p + 1, weapon);
        else
            FUN_0048a0f0(unit, weapon);
    } else if ((unit->flags & 0x300000) == 0x200000) {
        int r = FUN_0040b7b0(unit, weapon, 1);
        if (r)
            FUN_0048a060(unit, r, weapon);
        else
            FUN_0048a0f0(unit, weapon);
    }
}

// FUNCTION: 0x4089a0
void Class_004089a0::FUN_004089a0(int force)
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
                    Unit* target = FUN_0048a190(cursor, w);
                    if (target && (player->allied[target->owner->index]
                        || Contains(cursor->def->weaponCategories[w], target->category)
                        || (cursor->weapons[w].def->flag7 && (target->field_10e & 0x10))))
                        target = 0;
                    if (!target)
                        FUN_00408920(cursor, w);
                }
            }
        }
    }
}
