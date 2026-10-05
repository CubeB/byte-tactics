// Decompiled by Claude Opus 5.5. Names are provisional.
// Adds build progress to a unit under construction: `amount` build points
// (negative when it is being taken apart) move the remaining fraction at
// +0x104 towards 0 (done) or 1 (nothing built), clamped to [0, 1]. Building
// charges the builder's resource store (FUN_004011c0) for the metal and
// energy share of the step and only proceeds when the store accepts it.
// Unbuilding adds the energy share to the unit's float at +0xd4 (times 0.5 or
// 0.7 for a type 2 player when g_game+0x37eee is 0 or 1) and destroys the
// unit through DamageUnit once nothing is left. Either way the unit's hit
// points follow the progress, and a finished unit goes to FUN_0041b8d0.
// Notes: min/max are the <windows.h> macros (the clamp evaluates max twice).
// The +0xd4 updates go through a `float&`: written as unit->field_d4, MSVC
// hoists the hit point load above the store and moves the `next >= 1.0f`
// compare. The unbuild path sets the hit points before the remaining
// fraction; the other order changes the registers in the build path too.

#include <windows.h>

#pragma pack(push, 1)
struct UnitType_0041ba60 {
    char unknown_0[0x186];
    float metalCost;                   // +0x186
    float energyCost;                  // +0x18a
    char unknown_18e[0x1ea - 0x18e];
    int buildTime;                     // +0x1ea
    char unknown_1ee[0x1fa - 0x1ee];
    unsigned int maxHp;                // +0x1fa
};

struct Player_0041ba60 {
    int active;                        // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                // +0x73
};

struct Unit {
    char unknown_0[0x92];
    UnitType_0041ba60* type;           // +0x92
    char unknown_96[0xbb - 0x96];
    unsigned char flags_bb;            // +0xbb
    char unknown_bc[0xd4 - 0xbc];
    float field_d4;                    // +0xd4
    char unknown_d8[0xec - 0xd8];
    Player_0041ba60* player;           // +0xec
    char unknown_f0[0x104 - 0xf0];
    float remaining;                   // +0x104
    short hp;                          // +0x108
    char unknown_10a[0x110 - 0x10a];
    unsigned int flags;                // +0x110
};

struct Game {
    char unknown_0[0x37eee];
    int difficulty;                    // +0x37eee (a guess)
};
#pragma pack(pop)

class Class_004011c0 {
public:
    int FUN_004011c0(float dx, float dy);
};

struct Builder_0041ba60 {
    char unknown_0[0xbc];
    Class_004011c0 store;              // +0xbc
};

extern Game* g_game;

void __stdcall DamageUnit(Unit* obj, Unit* unit, int n, int kind, int flag);
void __stdcall FUN_0041b8d0(Builder_0041ba60* builder, Unit* unit);

// FUNCTION: 0x41ba60
int __stdcall FUN_0041ba60(Builder_0041ba60* builder, Unit* unit, float amount)
{
    int result = 0;
    if (unit->remaining == 0.0f)
        return 0;
    if (amount >= 0.0f)
        unit->flags_bb |= 0x80;
    if (amount == 0.0f)
        return 0;
    UnitType_0041ba60* type = unit->type;
    float prev = unit->remaining;
    float next = min(max(prev - amount / type->buildTime, 0.0f), 1.0f);
    float step = prev - next;
    float metal = type->metalCost * step;
    float energy = type->energyCost * step;
    int hp = (int)(prev * type->maxHp) - (int)(next * type->maxHp);
    if (amount < 0.0f) {
        float refund = -energy;
        float& store = unit->field_d4;
        if (unit->player->active != 0 && unit->player->type == 2) {
            switch (g_game->difficulty) {
            case 0:
                store += refund * 0.5;
                break;
            case 1:
                store += refund * 0.7;
                break;
            default:
                store += refund;
                break;
            }
        } else {
            store += refund;
        }
        unit->hp = max(hp + unit->hp, 0);
        unit->remaining = next;
        unit->flags |= 0x2000;
        if (next >= 1.0f)
            DamageUnit(unit, unit, 30000, 9, 0);
    } else if (builder->store.FUN_004011c0(metal, energy)) {
        unit->hp = min(hp + unit->hp, unit->type->maxHp);
        unit->remaining = next;
        unit->flags |= 0x2000;
        result = 1;
    }
    if (unit->remaining == 0.0f)
        FUN_0041b8d0(builder, unit);
    return result;
}
