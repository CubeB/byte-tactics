// Decompiled by Haiku. Names are provisional.

struct Inner_49faf0
{
    char unknown_0[0x14];
    int value;  // +0x14
};

struct Outer_49faf0
{
    char unknown_0[0x18];
    Inner_49faf0* inner;  // +0x18
};

// FUNCTION: 0x49faf0
void __stdcall FUN_0049faf0(Outer_49faf0* param)
{
    param->inner->value = 0;
}
