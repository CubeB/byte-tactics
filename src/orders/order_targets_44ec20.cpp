// Decompiled by Opus. Names are provisional.
// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.

struct Class_0044ec20 {
    char unknown_0[8];
    unsigned short flag : 1;           // +0x8 bit 0
    unsigned short unknown_rest : 15;
    char unknown_a[0x24 - 0xa];
    short value;                       // +0x24

    void FUN_0044ec20(short v);
};

// FUNCTION: 0x44ec20
void Class_0044ec20::FUN_0044ec20(short v)
{
    flag = 1;
    value = v;
}
