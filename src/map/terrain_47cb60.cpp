// Decompiled by Opus. Names are provisional.

struct Unit_0047cb60;

#pragma pack(push, 1)
struct Owner_0047cb60 {
    char unknown_0[6];
    Unit_0047cb60* first;           // +0x6
};

struct Unit_0047cb60 {
    char unknown_0[0x82];
    Owner_0047cb60* owner;          // +0x82
    int unknown_86;                 // +0x86
    char unknown_8a[4];
    Unit_0047cb60* next;            // +0x8e
};
#pragma pack(pop)

// FUNCTION: 0x47cb60
void __stdcall FUN_0047cb60(Unit_0047cb60* unit, Owner_0047cb60* owner)
{
    Owner_0047cb60* cur = unit->owner;
    if (owner != cur) {
        if (unit->unknown_86 == 0) {
            if (cur != 0) {
                Unit_0047cb60** pp = &cur->first;
                while (*pp != unit)
                    pp = &(*pp)->next;
                *pp = unit->next;
                unit->next = 0;
            }
            unit->next = owner->first;
            owner->first = unit;
        }
        unit->owner = owner;
    }
}
