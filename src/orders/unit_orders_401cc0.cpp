// Decompiled by Opus. Names are provisional.
// An order handler from the same table as 0x401c20: updates two unit flag
// bits and returns 5, like its neighbour.

class Class_00438880;

#pragma pack(push, 1)
struct Unit_00401cc0 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};
#pragma pack(pop)

// FUNCTION: 0x401cc0
int __stdcall FUN_00401cc0(Unit_00401cc0* unit, Class_00438880* order, int unused)
{
    unit->flags = (unit->flags & ~0x8000) | 0x20;
    return 5;
}
