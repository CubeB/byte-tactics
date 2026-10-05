// Decompiled by Opus. Names are provisional.

struct Unit {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitCallback_0048edb0 {
public:
    virtual int Visit(Unit* unit) = 0;
};

struct UnitRange_0048edb0 {
    Unit* begin;                       // +0x0
    Unit* end;                         // +0x4 (last unit, inclusive)

    void ForEach(UnitCallback_0048edb0* callback)
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
    UnitRange_0048edb0 units;          // +0x1bca
};
#pragma pack(pop)

extern Game* g_game;

short __stdcall FindUnitTypeId(char* param_1);

class Base_0048edb0 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    int field_4;                       // +0x4
    int field_8;                       // +0x8
};

class VictoryBuildUnitType : public Base_0048edb0, public UnitCallback_0048edb0 {
public:
    char field_10[0x20];               // +0x10
    short field_30;                    // +0x30

    virtual int IsSatisfied();
};

// FUNCTION: 0x48edb0
int VictoryBuildUnitType::IsSatisfied()
{
    if (field_4 != 0) {
        return 1;
    }
    if (field_30 == 0) {
        field_30 = FindUnitTypeId(field_10);
    }
    g_game->units.ForEach(this);
    return field_4;
}
