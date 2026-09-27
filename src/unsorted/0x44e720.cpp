// Decompiled by Opus. Names are provisional.
// Sets a flag bit in the word at +0x8 (a 1-bit unsigned short bitfield, which
// MSVC sets with `or byte ptr` straight to memory) and stores a short.

struct Class_0044e720 {
    char unknown_0[8];
    unsigned short unknown_bits : 6;   // +0x8
    unsigned short flag : 1;           // +0x8 bit 6
    unsigned short unknown_rest : 9;
    char unknown_a[4];
    short value;                       // +0xe

    void FUN_0044e720(short v);
};

// FUNCTION: 0x44e720
void Class_0044e720::FUN_0044e720(short v)
{
    flag = 1;
    value = v;
}
