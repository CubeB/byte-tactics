// Decompiled by Opus. Names are provisional.
// Energy counterpart of 0x401220.
#include <math.h>

struct Store_00401260 {
    char unknown_0[0x98];
    float energy;                      // +0x98
};

class Class_00401260 {
public:
    char unknown_0[0x1c];
    float y0;                          // +0x1c
    char unknown_20[0x30 - 0x20];
    Store_00401260* store;             // +0x30

    int SpendEnergy(float amount);
};

// FUNCTION: 0x401260
int Class_00401260::SpendEnergy(float amount)
{
    if (store->energy >= amount) {
        store->energy -= amount;
        y0 += amount;
        return 1;
    }
    return 0;
}
