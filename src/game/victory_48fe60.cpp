// Decompiled by Opus. Names are provisional.

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (see 0x48ff40.cpp).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual int FUN_0048ea00();          // IsSatisfied
    virtual void FUN_0048ea10(Unit* unit);   // Slot1
    virtual void FUN_0048ea20(Unit* unit);   // Slot2
    virtual void FUN_0048ea30(Unit* unit);   // Slot3
    virtual void FUN_0048f840(Class_004b4560* file) = 0;   // Save
    virtual void FUN_0048f880(Class_004b4560* file) = 0;   // Load
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

// The mission's victory and defeat conditions (same layout as Class_0048ff40).
class Class_0048fe60 {
public:
    Condition_0048ff40* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048ff40* defeat[16];      // +0x44
    int defeatCount;                     // +0x84

    void FUN_0048fe60(Class_004b4560* file);
};

// Loads every condition (the save counterpart is 0x48fdf0).
// FUNCTION: 0x48fe60
void Class_0048fe60::FUN_0048fe60(Class_004b4560* file)
{
    if (g_game->field_391e9->FUN_00435100() == 1) {
        int i;
        for (i = 0; i < victoryCount; i++) {
            victory[i]->FUN_0048f880(file);
        }
        for (i = 0; i < defeatCount; i++) {
            defeat[i]->FUN_0048f880(file);
        }
    }
}
