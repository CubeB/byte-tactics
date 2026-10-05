// Decompiled by Opus. Names are provisional.

struct Unit;
class HapiBank;

// Mission victory/defeat condition (see 0x48ff40.cpp).
class MissionCondition {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual int IsSatisfied();           // IsSatisfied
    virtual void OnUnitDied(Unit* unit);     // Slot1
    virtual void OnUnitCaptured(Unit* unit);  // Slot2
    virtual void OnUnitCreated(Unit* unit);  // Slot3
    virtual void SaveState(HapiBank* file) = 0;            // Save
    virtual void LoadState(HapiBank* file) = 0;            // Load
};

class Mission {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x391e9];
    Mission* field_391e9;                // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84

    void SaveConditions(HapiBank* file);
};

// Saves every condition.
// FUNCTION: 0x48fdf0
void MissionConditions::SaveConditions(HapiBank* file)
{
    if (g_game->field_391e9->FUN_00435100() == 1) {
        int i;
        for (i = 0; i < victoryCount; i++) {
            victory[i]->SaveState(file);
        }
        for (i = 0; i < defeatCount; i++) {
            defeat[i]->SaveState(file);
        }
    }
}
