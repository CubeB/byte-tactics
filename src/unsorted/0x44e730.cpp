// Decompiled by Opus. Names are provisional.
// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.

struct Class_0044e730 {
    char unknown_0[8];
    unsigned short unknown_bits : 4;   // +0x8
    unsigned short flag : 1;           // +0x8 bit 4
    unsigned short unknown_rest : 11;
    short value;                       // +0xa

    void FUN_0044e730(short v);
};

// FUNCTION: 0x44e730
void Class_0044e730::FUN_0044e730(short v)
{
    flag = 1;
    value = v;
}
