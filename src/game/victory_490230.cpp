// Decompiled by deepseek-v4.1-flash. Names are provisional.
// The mission's defeat check. It does nothing unless the mission is active
// (the dword at +0x88, set to 1 by Class_0048df90), then asks the game mode
// which test to run: 1 runs the victory conditions (0x48fed0, defined inline
// here so /Ob2 inlines it, which is also what makes the constant 0 live in
// ebp), 2 runs the "every other player is dead or allied" loop (0x48ffd0)
// and 3 calls FUN_00490080.
//
// The loop counter is copied into a second unsigned char before the player
// test. That copy is what makes MSVC put the counter in the base slot of
// [ecx + edi + 0x108] (matching the original, and unlike the otherwise
// identical loop in 0x48ffd0); an unsigned int copy flips the SIB the right
// way too but makes MSVC hoist the zero-extension above the player test,
// which costs more bytes than the SIB saves. An unsigned char copy keeps the
// zero-extension where the original has it.

class Class_00435100 {
public:
    int FUN_00435100();
};

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (see 0x48e010.cpp and 0x48fed0.cpp).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048ff40() { satisfied = celebrated = 0; }
    virtual int FUN_0048ea00();                  // IsSatisfied
    virtual void FUN_0048ea10(Unit* unit);       // Slot1
    virtual void FUN_0048ea20(Unit* unit);       // Slot2
    virtual void FUN_0048ea30(Unit* unit);       // Slot3
    virtual void FUN_0048f840(Class_004b4560* file) = 0;   // Save
    virtual void FUN_0048f880(Class_004b4560* file) = 0;   // Load
};

// VictoryCondition_DestroyAllUnits.
class Class_0048eb40 : public Condition_0048ff40 {
public:
    virtual int FUN_0048ea00();
    virtual void FUN_0048f840(Class_004b4560* file);
    virtual void FUN_0048f880(Class_004b4560* file);
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
    Class_00435100* mode;                 // +0x391e9
};
#pragma pack(pop)

extern Game* g_game;

// 0x490080 is a __thiscall method in the original (the caller loads ecx with
// this), but its own file matched it as a free function, so it is declared
// here as a __fastcall free function: that still passes the first argument in
// ecx and gives the linker the established name FUN_00490080.
int __fastcall FUN_00490080(void* self);

class Class_0048ff40 {
public:
    Condition_0048ff40* victory[16];     // +0x00
    int victoryCount;                    // +0x40
    Condition_0048ff40* defeat[16];      // +0x44
    int defeatCount;                     // +0x84
    int active;                          // +0x88, set to 1 by Class_0048df90

    // 0x48fed0, written in the class so that /Ob2 inlines it here.
    inline int FUN_0048fed0()
    {
        if (victoryCount == 0) {
            victory[victoryCount] = new Class_0048eb40;
            victoryCount++;
        }
        for (int i = 0; i < victoryCount; i++) {
            if (!victory[i]->FUN_0048ea00())
                return 0;
        }
        return 1;
    }

    int FUN_00490230();
};

// FUNCTION: 0x490230
int Class_0048ff40::FUN_00490230()
{
    if (active == 0)
        return 0;
    switch (g_game->mode->FUN_00435100()) {
    case 1:
        return FUN_0048fed0();
    case 2: {
        unsigned char player = g_game->player;
        Player_00490230* me = &g_game->players[player];
        for (unsigned char i = 0; i < 10; i++) {
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
        return FUN_00490080(this);
    }
    return 0;
}
