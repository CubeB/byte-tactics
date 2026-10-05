// Decompiled by Opus. Names are provisional.
// Any header include flips the operand order of the first `bit + bits`.
#include <string.h>

// Reads bit fields from an array of dwords, lowest bits first.
class Class_00415dc0 {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int FUN_00415dc0(int bits);
};

// FUNCTION: 0x415dc0
int Class_00415dc0::FUN_00415dc0(int bits)
{
    if (bit + bits < 32) {
        int r = (data[index] >> bit) & ((1 << bits) - 1);
        bit += bits;
        return r;
    }
    if (bit == 0) {
        return data[index++];
    }
    int low = 32 - bit;
    int r = (data[index] >> bit) & ((1 << low) - 1);
    bit = bits - low;
    index++;
    r |= (data[index] & ((1 << bit) - 1)) << low;
    return r;
}
