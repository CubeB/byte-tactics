// Decompiled by Opus. Names are provisional.
// Reads a signed bit field: FUN_00415dc0's value, sign-extended from `bits`.

class Class_00415dc0 {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int FUN_00415dc0(int bits);
    int FUN_00415e60(int bits);
};

// FUNCTION: 0x415e60
int Class_00415dc0::FUN_00415e60(int bits)
{
    int r = FUN_00415dc0(bits);
    if (r & (1 << (bits - 1))) {
        r |= -1 << bits;
    }
    return r;
}
