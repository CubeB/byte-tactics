// Decompiled by Opus. Names are provisional.

// Mission victory/defeat condition (see 0x48ff40.cpp).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    virtual int IsSatisfied() = 0;
    virtual void FUN_0048ea10();         // Slot1
    virtual void FUN_0048ea20();         // Slot2
    virtual void FUN_0048ea30();         // Slot3
    virtual void FUN_0048f840(void* file) = 0;   // Save
    virtual void Load(void* file) = 0;
};

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Game_0048fdf0 {
    char unknown_0[0x391e9];
    Class_00435100* field_391e9;         // +0x391e9
};
#pragma pack(pop)

extern Game_0048fdf0* g_game;

class Class_0048ff40 {
public:
    Condition_0048ff40* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048ff40* defeat[16];      // +0x44
    int defeatCount;                     // +0x84

    void FUN_0048fdf0(void* file);
};

// Saves every condition.
// FUNCTION: 0x48fdf0
void Class_0048ff40::FUN_0048fdf0(void* file)
{
    if (g_game->field_391e9->FUN_00435100() == 1) {
        int i;
        for (i = 0; i < victoryCount; i++) {
            victory[i]->FUN_0048f840(file);
        }
        for (i = 0; i < defeatCount; i++) {
            defeat[i]->FUN_0048f840(file);
        }
    }
}
