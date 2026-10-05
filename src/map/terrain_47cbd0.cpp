// Decompiled by Opus. Names are provisional.
// Detaches a unit from its owner: RemoveUnitFromMap first, then (unless +0x86 is
// set) unlinks it from the owner's list (head at owner +0x6, link at unit
// +0x8e) and clears the owner. The reverse of 0x47cb60.

struct Unit;

#pragma pack(push, 1)
struct Owner_0047cb60 {
    char unknown_0[6];
    Unit* first;                    // +0x6
};

struct Unit {
    char unknown_0[0x82];
    Owner_0047cb60* owner;          // +0x82
    int unknown_86;                 // +0x86
    char unknown_8a[4];
    Unit* next;                     // +0x8e
};
#pragma pack(pop)

void __stdcall RemoveUnitFromMap(Unit* unit);

// FUNCTION: 0x47cbd0
void __stdcall FUN_0047cbd0(Unit* unit)
{
    RemoveUnitFromMap(unit);
    if (unit->unknown_86 == 0) {
        Owner_0047cb60* cur = unit->owner;
        if (cur != 0) {
            Unit** pp = &cur->first;
            while (*pp != unit)
                pp = &(*pp)->next;
            *pp = unit->next;
            unit->next = 0;
        }
    }
    unit->owner = 0;
}
