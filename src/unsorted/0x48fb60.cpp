// Decompiled by Opus. Names are provisional.
// Slot 0 of the "any unit passes X" defeat condition (vtable 0x4fd790,
// visitor vtable 0x4fd788 holding 0x48fb30; state saved by 0x48fbc0): until
// the condition is met, visits every live unit and returns whether it is.

struct Unit_0048fb60 {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048fb60 {
public:
    virtual int Visit(Unit_0048fb60* unit) = 0;
};

struct UnitList_0048fb60 {
    Unit_0048fb60* first;              // +0x0
    Unit_0048fb60* last;               // +0x4 (inclusive)

    void ForEach(UnitVisitor_0048fb60* visitor)
    {
        for (Unit_0048fb60* unit = first; unit <= last; unit++) {
            if (unit->field_a6 != 0) {
                int result = visitor->Visit(unit);
                if (!result) {
                    break;
                }
            }
        }
    }
};

#pragma pack(push, 1)
struct Game_0048fb60 {
    char unknown_0[0x1d15];
    UnitList_0048fb60 units;           // +0x1d15
};
#pragma pack(pop)

extern Game_0048fb60* g_game;

class Condition_0048fb60 {
public:
    virtual int FUN_0048ea00();          // IsSatisfied
    int done;                          // +0x04
    int announced;                     // +0x08
};

class Class_0048fb60 : public Condition_0048fb60, public UnitVisitor_0048fb60 {
public:
    virtual int FUN_0048ea00();
};

// FUNCTION: 0x48fb60
int Class_0048fb60::FUN_0048ea00()
{
    if (done == 0) {
        g_game->units.ForEach(this);
    }
    return done;
}
