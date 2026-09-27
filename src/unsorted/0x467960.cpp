// Decompiled by Opus. Names are provisional.
// Same as FUN_00467980 but clears flag 0x100 instead of 0x200.

struct Unit_00467960 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

// FUNCTION: 0x467960
void __stdcall FUN_00467960(Unit_00467960* unit)
{
    unit->flags = (unit->flags & ~0x100) | 0x400;
}
