// Decompiled by Opus. Names are provisional.
// Slot 1 of the "kill unit type" victory condition (vtable 0x4fd8b0, state
// saved by 0x48f160): called with a unit; counts the named unit type down and
// announces the victory condition when the count runs out.
#include <string.h>

#pragma pack(push, 1)
struct Info_0048f0f0 {
    char unknown_0[0x20];
    char name[0x20];                   // +0x20
};

struct Unit {
    char unknown_0[0x92];
    Info_0048f0f0* info;               // +0x92
    char unknown_96[0xff - 0x96];
    unsigned char kind;                // +0xff
};
#pragma pack(pop)

void __stdcall PlaySoundByName(char* str, int flag);

class Condition_0048f0f0 {
public:
    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);
    int done;                          // +0x04
    int announced;                     // +0x08
};

class VictoryKillUnitType : public Condition_0048f0f0 {
public:
    char name[0x20];                   // +0x0c
    int count;                         // +0x2c
    virtual void OnUnitDied(Unit* unit);
};

// FUNCTION: 0x48f0f0
void VictoryKillUnitType::OnUnitDied(Unit* unit)
{
    if (count > 0 && unit->kind == 1 && _strcmpi(name, unit->info->name) == 0) {
        if (--count <= 0) {
            done = 1;
            if (announced == 0) {
                PlaySoundByName("Victory Condition", 0);
                announced = 1;
            }
        }
    }
}
