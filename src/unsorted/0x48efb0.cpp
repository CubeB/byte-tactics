// Decompiled by Opus. Names are provisional.
#include <string.h>

struct Unit_48efb0 {
    char unknown_0[0xa6];
    short field_a6;                  // +0xa6
    char unknown_a8[0x118 - 0xa8];
};

// Interface at +0xc of the object: slot 0 is called for each unit.
class UnitVisitor_48efb0 {
public:
    virtual int Visit(Unit_48efb0* unit) = 0;
};

struct UnitList_48efb0 {
    Unit_48efb0* first;              // +0x0
    Unit_48efb0* last;               // +0x4 (inclusive)

    void ForEach(UnitVisitor_48efb0* visitor)
    {
        for (Unit_48efb0* unit = first; unit <= last; unit++) {
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
struct Game_48efb0 {
    char unknown_0[0x1d15];
    UnitList_48efb0 units;           // +0x1d15
};

struct Info_48efb0 {
    char unknown_0[0x20];
    char name[0x20];                 // +0x20
};

struct Player_48efb0 {
    char unknown_0[0x92];
    Info_48efb0* info;               // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;              // +0xff
};
#pragma pack(pop)

// GLOBAL: 0x511de8
extern Game_48efb0* g_game;

short __stdcall FUN_00488b10(char* name);
void __stdcall FUN_0047f1a0(char* str, int flag);

class Condition_48efb0 {
public:
    virtual void FUN_0048efb0(Player_48efb0* player) = 0;
    int done;                        // +0x04
    int announced;                   // +0x08
};

#pragma pack(push, 2)
class Class_0048efb0 : public Condition_48efb0, public UnitVisitor_48efb0 {
public:
    char name[0x20];                 // +0x10
    short id;                        // +0x30
    int count;                       // +0x32
    void FUN_0048efb0(Player_48efb0* player);
    int Visit(Unit_48efb0* unit);
};
#pragma pack(pop)

// FUNCTION: 0x48efb0
void Class_0048efb0::FUN_0048efb0(Player_48efb0* player)
{
    if (done == 0 && player->kind == 1 && _strcmpi(name, player->info->name) == 0) {
        id = FUN_00488b10(name);
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
