// Decompiled by Opus. Names are provisional.
// As in 0x4012a0, the header include decides whether MSVC loads the float
// parameter or the field first.
#include <math.h>

struct Store_00401220 {
    char unknown_0[0x8c];
    float metal;                       // +0x8c
};

class Class_00401220 {
public:
    char unknown_0[0x4];
    float x0;                          // +0x04
    char unknown_8[0x30 - 0x8];
    Store_00401220* store;             // +0x30

    int FUN_00401220(float amount);
};

// FUNCTION: 0x401220
int Class_00401220::FUN_00401220(float amount)
{
    if (store->metal >= amount) {
        store->metal -= amount;
        x0 += amount;
        return 1;
    }
    return 0;
}
