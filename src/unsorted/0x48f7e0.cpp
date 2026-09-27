// Decompiled by Opus. Names are provisional.
// Slot 0 of the defeat condition with vtable 0x4fd800 (state saved by
// 0x48f840, "DefeatCondition_AllUnitsKilled"): assumes the condition is met
// and offers every live unit to the visitor at +0xc, which can clear it.

struct Unit_0048f7e0 {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitCallback_0048f7e0 {
public:
    virtual int Visit(Unit_0048f7e0* unit) = 0;
};

struct UnitRange_0048f7e0 {
    Unit_0048f7e0* begin;              // +0x0
    Unit_0048f7e0* end;                // +0x4 (last unit, inclusive)

    void ForEach(UnitCallback_0048f7e0* callback)
    {
        for (Unit_0048f7e0* u = begin; u <= end; u++) {
            if (u->field_a6 != 0) {
                int result = callback->Visit(u);
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

class Base_0048f7e0 {
public:
    virtual void Unknown_0();
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8
};

class Class_0048f7e0 : public Base_0048f7e0, public UnitCallback_0048f7e0 {
public:
    int FUN_0048f7e0();
};

// FUNCTION: 0x48f7e0
int Class_0048f7e0::FUN_0048f7e0()
{
    satisfied = 1;
    g_game->units.ForEach(this);
    return satisfied;
}
