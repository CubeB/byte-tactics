// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The mission's defeat check. It does nothing unless the mission is active
// (the dword at +0x88, set to 1 by the constructor, 0x48df90), then asks the game mode
// which test to run: 1 runs the victory conditions (0x48fed0),
// 2 runs the "every other player is dead or allied" loop (0x48ffd0)
// and 3 calls CheckAlliedVictory.

class Mission {
public:
    int GetGameType();
};

struct Unit;
class HapiBank;

// Mission victory/defeat condition (see 0x48e010.cpp and 0x48fed0.cpp).
class MissionCondition {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    MissionCondition() { satisfied = celebrated = 0; }
    virtual int IsSatisfied();                   // IsSatisfied
    virtual void OnUnitDied(Unit* unit);         // Slot1
    virtual void OnUnitCaptured(Unit* unit);     // Slot2
    virtual void OnUnitCreated(Unit* unit);      // Slot3
    virtual void SaveState(HapiBank* file) = 0;            // Save
    virtual void LoadState(HapiBank* file) = 0;            // Load
};

// VictoryCondition_DestroyAllUnits.
class VictoryDestroyAllUnits : public MissionCondition {
public:
    virtual int IsSatisfied();
    virtual void SaveState(HapiBank* file);
    virtual void LoadState(HapiBank* file);
};

#pragma pack(push, 1)
struct Player_00490230 {                // 0x14b bytes
    char unknown_0[0x108];
    unsigned char allied[10];            // +0x108, one entry per other team
    char unknown_112[0x144 - 0x112];
    short count;                         // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game {
    char unknown_0[0x1b63];
    Player_00490230 players[10];          // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char player;                 // +0x2a42
    char unknown_2a43[0x391e9 - 0x2a43];
    Mission* mode;                        // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

// 0x490080 is a __thiscall method in the original (the caller loads ecx with
// this), but its own file matched it as a free function, so it is declared
// here as a __fastcall free function: that still passes the first argument in
// ecx and gives the linker the established name CheckAlliedVictory.
int __fastcall CheckAlliedVictory(void* self);

class MissionConditions {
public:
    MissionCondition* victory[16];       // +0x00
    int victoryCount;                    // +0x40
    MissionCondition* defeat[16];        // +0x44
    int defeatCount;                     // +0x84
    int active;                          // +0x88, set to 1 by the constructor (0x48df90)

    // 0x48fed0, written in the class so that /Ob2 inlines it here.
    inline int AllVictoryConditionsMet()
    {
        if (victoryCount == 0) {
            victory[victoryCount] = new VictoryDestroyAllUnits;
            victoryCount++;
        }
        for (int i = 0; i < victoryCount; i++) {
            if (!victory[i]->IsSatisfied())
                return 0;
        }
        return 1;
    }

    int CheckVictory();
};

// FUNCTION: 0x490230
int MissionConditions::CheckVictory()
{
    if (active == 0)
        return 0;
    switch (g_game->mode->GetGameType()) {
    case 1:
        return AllVictoryConditionsMet();
    case 2: {
        unsigned char player = g_game->player;
        Player_00490230* me = &g_game->players[player];
        for (unsigned char i = 0; i < 10; i++) {
            // Separate unsigned char copy of the counter: puts it in the base slot
            // and keeps the zero-extension after the player test.
            unsigned char j = i;
            if (i == g_game->player)
                continue;
            if (me->allied[j])
                continue;
            if (g_game->players[j].count != 0)
                return 0;
        }
        return 1;
    }
    case 3:
        return CheckAlliedVictory(this);
    }
    return 0;
}
