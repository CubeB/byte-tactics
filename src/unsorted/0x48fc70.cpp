// Decompiled by Opus. Names are provisional.
// Slot 0 of the "any unit passes Z" defeat condition (vtable 0x4fd770,
// visitor vtable 0x4fd768 holding 0x48fc40; state saved by 0x48fcd0): until
// the condition is met, visits every live unit through the visitor and
// returns whether it is met.

struct Unit_0048fc70 {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048fc70 {
public:
    virtual int Visit(Unit_0048fc70* unit) = 0;
};

struct UnitList_0048fc70 {
    Unit_0048fc70* first;              // +0x0
    Unit_0048fc70* last;               // +0x4 (inclusive)

    void ForEach(UnitVisitor_0048fc70* visitor)
    {
        for (Unit_0048fc70* unit = first; unit <= last; unit++) {
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
struct Game_0048fc70 {
    char unknown_0[0x1d15];
    UnitList_0048fc70 units;           // +0x1d15
};
#pragma pack(pop)

extern Game_0048fc70* g_game;

class Condition_0048fc70 {
public:
    virtual int FUN_0048fc70();
    int done;                          // +0x04
    int announced;                     // +0x08
};

class Class_0048fc70 : public Condition_0048fc70, public UnitVisitor_0048fc70 {
public:
    int FUN_0048fc70();
};

// FUNCTION: 0x48fc70
int Class_0048fc70::FUN_0048fc70()
{
    if (done == 0) {
        g_game->units.ForEach(this);
    }
    return done;
}
