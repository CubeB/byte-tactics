// Decompiled by Opus. Names are provisional.

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (see 0x48ff40.cpp).
class MissionCondition {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);     // Slot1
    virtual void OnUnitCaptured(Unit* unit);  // Slot2
    virtual void OnUnitCreated(Unit* unit);  // Slot3
    virtual void SaveState(Class_004b4560* file) = 0;      // Save
    virtual void LoadState(Class_004b4560* file) = 0;      // Load
};

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Class_00435100* field_391e9;         // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

// The mission's victory and defeat conditions (same layout as MissionConditions).
class Class_0048fe60 {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84

    void LoadConditions(Class_004b4560* file);
};

// Loads every condition (the save counterpart is 0x48fdf0).
// FUNCTION: 0x48fe60
void Class_0048fe60::LoadConditions(Class_004b4560* file)
{
    if (g_game->field_391e9->FUN_00435100() == 1) {
        int i;
        for (i = 0; i < victoryCount; i++) {
            victory[i]->LoadState(file);
        }
        for (i = 0; i < defeatCount; i++) {
            defeat[i]->LoadState(file);
        }
    }
}
