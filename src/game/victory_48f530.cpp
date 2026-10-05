// Decompiled by Opus. Names are provisional.
// Slot 0 of the victory condition with vtable 0x4fd850 (same shape as
// 0x48edb0): until satisfied, offers every live unit to the visitor at +0xc.

struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitCallback_0048f530 {
public:
    virtual int Visit(Unit* unit) = 0;
};

struct UnitRange_0048f530 {
    Unit* begin;                       // +0x0
    Unit* end;                         // +0x4 (last unit, inclusive)

    void ForEach(UnitCallback_0048f530* callback)
    {
        for (Unit* u = begin; u <= end; u++) {
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
    UnitRange_0048f530 units;          // +0x1bca
};
#pragma pack(pop)

extern Game* g_game;

class Base_0048f530 {
public:
    virtual int FUN_0048ea00();          // IsSatisfied
    int satisfied;                     // +0x4
    int celebrated;                    // +0x8
};

class Class_0048f530 : public Base_0048f530, public UnitCallback_0048f530 {
public:
    virtual int FUN_0048ea00();
};

// FUNCTION: 0x48f530
int Class_0048f530::FUN_0048ea00()
{
    if (satisfied == 0) {
        g_game->units.ForEach(this);
    }
    return satisfied;
}
