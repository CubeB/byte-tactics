// Decompiled by Opus. Names are provisional.
// Returns 1 when the unit's type lists `id` in its table of shorts.

#pragma pack(push, 1)
struct UnitDef_004894f0 {
    char unknown_0[0x152];
    int count;                         // +0x152
    short* ids;                        // +0x156
};

struct Unit_004894f0 {
    char unknown_0[0x92];
    UnitDef_004894f0* def;             // +0x92
};
#pragma pack(pop)

// FUNCTION: 0x4894f0
int __stdcall UnitCanBuild(Unit_004894f0* unit, short id)
{
    for (int i = 0; i < unit->def->count; i++) {
        if (unit->def->ids[i] == id)
            return 1;
    }
    return 0;
}
