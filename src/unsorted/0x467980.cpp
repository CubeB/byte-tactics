// Decompiled by Opus. Names are provisional.

struct Unit_00467980 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

// FUNCTION: 0x467980
void __stdcall FUN_00467980(Unit_00467980* unit)
{
    unit->flags = (unit->flags & ~0x200) | 0x400;
}
