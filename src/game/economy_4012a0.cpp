// Decompiled by Opus. Names are provisional.
// The header include matters, as in 0x4011c0: without it MSVC loads the
// field before the parameter in x0 += dx.
#include <math.h>

struct Store_004012a0 {
    char unknown_0[0x8c];
    float metal;                       // +0x8c
    char unknown_90[0x98 - 0x90];
    float energy;                      // +0x98
};

class Class_004012a0 {
public:
    char unknown_0[0x4];
    float x0;                          // +0x04
    char unknown_8[0x1c - 0x8];
    float y0;                          // +0x1c
    char unknown_20[0x30 - 0x20];
    Store_004012a0* store;             // +0x30
    int SpendMetalAndEnergy(float dx, float dy);
};

// FUNCTION: 0x4012a0
int Class_004012a0::SpendMetalAndEnergy(float dx, float dy)
{
    if (store->metal >= dx && store->energy >= dy) {
        store->metal -= dx;
        x0 += dx;
        if (store->energy >= dy) {
            store->energy -= dy;
            y0 += dy;
        }
        return 1;
    }
    return 0;
}
