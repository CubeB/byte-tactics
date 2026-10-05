// Decompiled by Opus. Names are provisional.
// The header include matters, as in 0x4011c0: without it MSVC loads the
// field before the parameter in energyUsed += energy.
#include <math.h>

struct Store_004012a0 {
    char unknown_0[0x8c];
    float energy;                      // +0x8c
    char unknown_90[0x98 - 0x90];
    float metal;                       // +0x98
};

class UnitResources {
public:
    char unknown_0[0x4];
    float energyUsed;                  // +0x04
    char unknown_8[0x1c - 0x8];
    float metalUsed;                   // +0x1c
    char unknown_20[0x30 - 0x20];
    Store_004012a0* store;             // +0x30
    int SpendEnergyAndMetal(float energy, float metal);
};

// FUNCTION: 0x4012a0
int UnitResources::SpendEnergyAndMetal(float energy, float metal)
{
    if (store->energy >= energy && store->metal >= metal) {
        store->energy -= energy;
        energyUsed += energy;
        if (store->metal >= metal) {
            store->metal -= metal;
            metalUsed += metal;
        }
        return 1;
    }
    return 0;
}
