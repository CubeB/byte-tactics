// Decompiled by Opus. Names are provisional.

#include <string.h>
#include <math.h>

#pragma pack(push, 1)
struct Player {                        // 0x14b bytes
    char unknown_0[0x8c];
    float energy;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                       // +0x98
    char unknown_9c[0x14b - 0x9c];
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                // +0x1b63
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

class UnitResources {
public:
    int field_0;
    float energyUsed;                  // +0x04
    float field_08;                    // +0x08
    float field_0c;                    // +0x0c
    char unknown_10[0x1c - 0x10];
    float metalUsed;                   // +0x1c
    float field_20;                    // +0x20
    float field_24;                    // +0x24
    char unknown_28[0x30 - 0x28];
    Player* player;                    // +0x30

    void Reset(unsigned char playerIndex);
    int FUN_00401180(UnitResources* r, float amount);
    int FUN_004011c0(float dx, float dy);
    int SpendEnergy(float amount);
    int SpendMetal(float amount);
    int SpendEnergyAndMetal(float energy, float metal);
};

// Pilot functions used to validate the toolchain. Names are provisional.
// FUNCTION: 0x401070
void UnitResources::Reset(unsigned char playerIndex)
{
    memset(this, 0, sizeof(*this));
    player = &g_game->players[playerIndex];
}

// As in 0x4011c0, the header include decides whether MSVC loads the float
// parameter or the field first.
// FUNCTION: 0x401180
int UnitResources::FUN_00401180(UnitResources* r, float amount)
{
    r->energyUsed += amount;
    if (r->field_0c > 0.0f)
        return 0;
    r->field_08 += amount;
    return 1;
}

// Any header include matters here: with the larger symbol table MSVC loads
// the float parameter first (fld dx; fadd [ecx+4]) instead of the field.
// FUNCTION: 0x4011c0
int UnitResources::FUN_004011c0(float dx, float dy)
{
    energyUsed += dx;
    metalUsed += dy;
    if (field_0c <= 0.0f && field_24 <= 0.0f) {
        field_08 += dx;
        field_20 += dy;
        return 1;
    }
    return 0;
}

// As in 0x4012a0, the header include decides whether MSVC loads the float
// parameter or the field first.
// FUNCTION: 0x401220
int UnitResources::SpendEnergy(float amount)
{
    if (player->energy >= amount) {
        player->energy -= amount;
        energyUsed += amount;
        return 1;
    }
    return 0;
}

// Metal counterpart of 0x401220.
// FUNCTION: 0x401260
int UnitResources::SpendMetal(float amount)
{
    if (player->metal >= amount) {
        player->metal -= amount;
        metalUsed += amount;
        return 1;
    }
    return 0;
}

// The header include matters, as in 0x4011c0: without it MSVC loads the
// field before the parameter in energyUsed += energy.
// FUNCTION: 0x4012a0
int UnitResources::SpendEnergyAndMetal(float energy, float metal)
{
    if (player->energy >= energy && player->metal >= metal) {
        player->energy -= energy;
        energyUsed += energy;
        if (player->metal >= metal) {
            player->metal -= metal;
            metalUsed += metal;
        }
        return 1;
    }
    return 0;
}
