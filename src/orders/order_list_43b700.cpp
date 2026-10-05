// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x110];
    unsigned int unknown_bits : 20;      // +0x110
    unsigned int mode : 2;               // +0x110, bits 20-21
    unsigned int unknown_bits2 : 10;
};
#pragma pack(pop)

int __stdcall FUN_0040b7b0(Unit* unit, int a, int b);

// FUNCTION: 0x43b700
int __stdcall FUN_0043b700(Unit* unit)
{
    if (unit->mode == 2)
        return FUN_0040b7b0(unit, 0, 0);
    return 0;
}
