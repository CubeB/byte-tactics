// Decompiled by Opus. Names are provisional.
// Slot 1 of the "kill all of type" victory condition (vtable 0x4fd8d0,
// visitor vtable 0x4fd8c8 holding 0x48ef80; state saved by 0x48f070).
#include <string.h>

#pragma pack(push, 1)
struct Info_48efb0 {
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_48efb0* info;               // +0x92
    char unknown_96[0xa6 - 0x96];
    short field_a6;                  // +0xa6
    char unknown_a8[0xff - 0xa8];
    unsigned char kind;              // +0xff
    char unknown_100[0x118 - 0x100];
};
#pragma pack(pop)

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_48efb0 {
public:
    virtual int Visit(Unit* unit) = 0;
};

struct UnitList_48efb0 {
    Unit* first;                     // +0x0
    Unit* last;                      // +0x4 (inclusive)

    void ForEach(UnitVisitor_48efb0* visitor)
    {
        for (Unit* unit = first; unit <= last; unit++) {
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
struct Game {
    char unknown_0[0x1d15];
    UnitList_48efb0 units;           // +0x1d15
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game* g_game;

short __stdcall FindUnitTypeId(char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_48efb0 {
public:
    virtual int IsSatisfied();       // IsSatisfied
    virtual void OnUnitDied(Unit* unit) = 0;
    int done;                        // +0x04
    int announced;                   // +0x08
};

#pragma pack(push, 2)
class VictoryKillAllOfType : public Condition_48efb0, public UnitVisitor_48efb0 {
public:
    char name[0x20];                 // +0x10
    short id;                        // +0x30
    int count;                       // +0x32
    virtual void OnUnitDied(Unit* unit);
    int Visit(Unit* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48efb0
void VictoryKillAllOfType::OnUnitDied(Unit* unit)
{
    if (done == 0 && unit->kind == 1 && _strcmpi(name, unit->info->name) == 0) {
        id = FindUnitTypeId(name);
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
