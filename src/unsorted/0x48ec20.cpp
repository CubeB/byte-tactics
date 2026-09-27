// Decompiled by Opus. Names are provisional.
// Slot 1 of a victory condition (vtable 0x4fd928, visitor vtable 0x4fd920
// holding 0x48ec00; state saved by 0x48ecb0): counts the live units through
// the visitor and announces the victory condition when at most one is left.

struct Unit_0048ec20 {
    char unknown_0[0xa6];
    short field_a6;                    // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_0048ec20 {
public:
    virtual int Visit(Unit_0048ec20* unit) = 0;
};

struct UnitList_0048ec20 {
    Unit_0048ec20* first;              // +0x0
    Unit_0048ec20* last;               // +0x4 (inclusive)

    void ForEach(UnitVisitor_0048ec20* visitor)
    {
        for (Unit_0048ec20* unit = first; unit <= last; unit++) {
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
struct Game_0048ec20 {
    char unknown_0[0x1d15];
    UnitList_0048ec20 units;           // +0x1d15
};

struct Player_0048ec20 {
    int field_0;                       // +0x00
    char unknown_4[0xff - 0x4];
    unsigned char kind;                // +0xff
};
#pragma pack(pop)

extern Game_0048ec20* g_game;

void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_0048ec20 {
public:
    virtual void Unknown_0();
    virtual void FUN_0048ec20(Player_0048ec20* player);
    int done;                          // +0x04
    int announced;                     // +0x08
};

class Class_0048ec20 : public Condition_0048ec20, public UnitVisitor_0048ec20 {
public:
    int count;                         // +0x10
    void FUN_0048ec20(Player_0048ec20* player);
};

// FUNCTION: 0x48ec20
void Class_0048ec20::FUN_0048ec20(Player_0048ec20* player)
{
    if (player->kind == 1 && player->field_0 != 0) {
        count = 0;
        g_game->units.ForEach(this);
        if (count <= 1) {
            done = 1;
            if (announced == 0) {
                FUN_0047f1a0("Victory Condition", 0);
                announced = 1;
            }
        }
    }
}
