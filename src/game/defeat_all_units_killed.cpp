// Decompiled by Opus and Sonnet. Names are provisional.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x86];
    Unit* field_86;                    // +0x86
    char unknown_8a[0xa6 - 0x8a];
    short field_a6;                    // +0xa6
    char unknown_a8[0xfb - 0xa8];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};
#pragma pack(pop)

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitCallback_0048f7e0 {
public:
    virtual int VisitUnit(Unit* unit) = 0;
};

struct UnitRange_0048f7e0 {
    Unit* begin;                       // +0x0
    Unit* end;                         // +0x4 (last unit, inclusive)

    void ForEach(UnitCallback_0048f7e0* callback)
    {
        for (Unit* u = begin; u <= end; u++) {
            if (u->field_a6 != 0) {
                int result = callback->VisitUnit(u);
                if (!result) {
                    break;
                }
            }
        }
    }
};

#pragma pack(push, 2)
struct Game {
    char unknown_0[0x1bca];
    UnitRange_0048f7e0 units;          // +0x1bca
};
#pragma pack(pop)

extern Game* g_game;

#include "../util/hapi_bank.h"

extern char DAT_005090fc[]; // "DefeatCondition_AllUnitsKilled"
extern char DAT_00508f30[]; // "Satisfied"
extern char DAT_00508f24[]; // "Celebrated"

class Base_0048f7e0 {
public:
    virtual int IsSatisfied();         // IsSatisfied
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8
};

// VisitUnit's `this` is the visitor subobject (+0xc), so satisfied sits at -8.
class DefeatAllUnitsKilled : public Base_0048f7e0, public UnitCallback_0048f7e0 {
public:
    virtual int IsSatisfied();
    virtual int VisitUnit(Unit* unit);
    virtual void SaveState(HapiBank* obj);
    virtual void LoadState(HapiBank* obj);
};

// Slot 0 of the unit visitor at +0xc of the "all units killed" defeat
// condition (vtable 0x4fd7f8, stored by the constructors inlined at 0x48e6e5
// and friends). Clears the condition when it meets a live, finished unit.
// FUNCTION: 0x48f790
int DefeatAllUnitsKilled::VisitUnit(Unit* unit)
{
    if ((unit->flags & 0x20) && unit->field_104 == 0.0f && unit->field_fb == 0
        && (unit->field_86 == 0 || (unit->field_86->flags & 0x40000000))) {
        satisfied = 0;
    }
    return satisfied;
}

// Slot 0 of the defeat condition with vtable 0x4fd800 (state saved by
// 0x48f840): assumes the condition is met and offers every live unit to the
// visitor at +0xc, which can clear it.
// FUNCTION: 0x48f7e0
int DefeatAllUnitsKilled::IsSatisfied()
{
    satisfied = 1;
    g_game->units.ForEach(this);
    return satisfied;
}

// FUNCTION: 0x48f840
void DefeatAllUnitsKilled::SaveState(HapiBank* obj)
{
    obj->OpenAccount(DAT_005090fc);
    obj->SetIntegerItem(DAT_00508f30, satisfied);
    obj->SetIntegerItem(DAT_00508f24, celebrated);
}

// Reads the "all units killed" defeat condition's state from a section;
// the writing counterpart is 0x48f840, compare 0x48ef40.
// FUNCTION: 0x48f880
void DefeatAllUnitsKilled::LoadState(HapiBank* obj)
{
    obj->OpenAccount("DefeatCondition_AllUnitsKilled");
    satisfied = obj->GetIntegerItem("Satisfied", 0);
    celebrated = obj->GetIntegerItem("Celebrated", 0);
}
