// Decompiled by Opus. Names are provisional.
// As in 0x4012a0, the header include decides whether MSVC loads the float
// parameter or the field first.
#include <math.h>

struct Store_00401220 {
    char unknown_0[0x8c];
    float energy;                      // +0x8c
};

class UnitResources {
public:
    char unknown_0[0x4];
    float energyUsed;                  // +0x04
    char unknown_8[0x30 - 0x8];
    Store_00401220* store;             // +0x30

    int SpendEnergy(float amount);
};

// FUNCTION: 0x401220
int UnitResources::SpendEnergy(float amount)
{
    if (store->energy >= amount) {
        store->energy -= amount;
        energyUsed += amount;
        return 1;
    }
    return 0;
}
