// Decompiled by Opus. Names are provisional.
// Slot 0 of the "unit type passes X" victory condition (vtable 0x4fd870,
// visitor vtable 0x4fd868 holding 0x48f370; state saved by 0x48f440): until
// the condition is met, visits every live unit and returns whether it is met.

struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048f3e0 {
public:
    virtual int Visit(Unit* unit) = 0;
};

struct UnitRange_0048f3e0 {
    Unit* begin;                       // +0x0
    Unit* end;                         // +0x4 (last unit, inclusive)

    void ForEach(UnitVisitor_0048f3e0* visitor)
    {
        for (Unit* u = begin; u <= end; u++) {
            if (u->field_a6 != 0) {
                int result = visitor->Visit(u);
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
    UnitRange_0048f3e0 units;          // +0x1bca
};
#pragma pack(pop)

extern Game* g_game;

class Condition_0048f3e0 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int done;                          // +0x04
    int announced;                     // +0x08
};

#pragma pack(push, 2)
class VictoryUnitTypePassesX : public Condition_0048f3e0, public UnitVisitor_0048f3e0 {
public:
    char name[0x20];                   // +0x10
    int field_30;                      // +0x30
    virtual int IsSatisfied();
};
#pragma pack(pop)

// FUNCTION: 0x48f3e0
int VictoryUnitTypePassesX::IsSatisfied()
{
    if (done == 0) {
        g_game->units.ForEach(this);
    }
    return done;
}
