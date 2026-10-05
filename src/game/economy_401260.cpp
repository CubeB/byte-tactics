// Decompiled by Opus. Names are provisional.
// Metal counterpart of 0x401220.
#include <math.h>

struct Store_00401260 {
    char unknown_0[0x98];
    float metal;                       // +0x98
};

class UnitResources {
public:
    char unknown_0[0x1c];
    float metalUsed;                   // +0x1c
    char unknown_20[0x30 - 0x20];
    Store_00401260* store;             // +0x30

    int SpendMetal(float amount);
};

// FUNCTION: 0x401260
int UnitResources::SpendMetal(float amount)
{
    if (store->metal >= amount) {
        store->metal -= amount;
        metalUsed += amount;
        return 1;
    }
    return 0;
}
