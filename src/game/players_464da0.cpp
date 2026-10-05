// Decompiled by Opus. Names are provisional.
// Calls FUN_004827b0 for every unit in the range whose flag 0x10000000 is set.

struct Unit {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

#pragma pack(push, 1)
struct Range_00464da0 {
    char unknown_0[0x67];
    Unit* first;                       // +0x67
    Unit* last;                        // +0x6b
};
#pragma pack(pop)

void __stdcall FUN_004827b0(Unit* unit);

// FUNCTION: 0x464da0
void __stdcall FUN_00464da0(Range_00464da0* range)
{
    for (Unit* u = range->first; u <= range->last; u++) {
        if (u->flags & 0x10000000) {
            FUN_004827b0(u);
        }
    }
}
