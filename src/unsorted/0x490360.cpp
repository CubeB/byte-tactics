// Decompiled by Space Bunny Free. Names are provisional.
#include <stdlib.h>

extern int FUN_0041d8b0();

extern unsigned int DAT_0051e6c4;

struct Unit;
class Class_004b4560;

// Mission victory/defeat condition (see 0x48ff40.cpp).
class Condition_0048ff40 {
public:
    int satisfied;                       // +0x4
    int celebrated;                      // +0x8

    Condition_0048ff40() { satisfied = celebrated = 0; }
    virtual int FUN_0048ea00();          // IsSatisfied
    virtual void FUN_0048ea10(Unit* unit);   // Slot1
    virtual void FUN_0048ea20(Unit* unit);   // Slot2
    virtual void FUN_0048ea30(Unit* unit);   // Slot3
    virtual void FUN_0048f840(Class_004b4560* file) = 0;   // Save
    virtual void FUN_0048f880(Class_004b4560* file) = 0;   // Load
};

// Secondary interface of a condition that visits units.
class Listener_0048ff40 {
public:
    virtual int FUN_0048f790(Unit* unit) = 0;
};

// DefeatCondition_AllUnitsKilled.
class Class_0048f840 : public Condition_0048ff40, public Listener_0048ff40 {
public:
    virtual int FUN_0048ea00();
    virtual void FUN_0048f840(Class_004b4560* file);   // Save
    virtual void FUN_0048f880(Class_004b4560* file);   // Load
    virtual int FUN_0048f790(Unit* unit);
};

class Class_00435100 {
public:
    int FUN_00435100();
};

#pragma pack(push, 1)
struct Player_00490360 {                // 0x14b bytes
    char unknown_0[0x144];
    short field_144;                   // +0x144
    char unknown_146[0x14b - 0x146];
};

struct Game_00490360 {
    char unknown_0[0x1b63];
    Player_00490360 players[10];        // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char field_2a42;           // +0x2a42
    char unknown_2a43[0x38a47 - 0x2a43];
    unsigned int ticks;                 // +0x38a47
    char unknown_38a4b[0x391e9 - 0x38a4b];
    Class_00435100* field_391e9;        // +0x391e9
};
#pragma pack(pop)

extern Game_00490360* g_game;

class Class_00490360 {
public:
    Condition_0048ff40* first[16];      // +0x00
    int firstCount;                     // +0x40
    Condition_0048ff40* second[16];     // +0x44
    int secondCount;                    // +0x84
    int field_88;                       // +0x88

    int FUN_00490360();
};

// The defeat-condition check, written as a helper so that MSVC 5 inlines it
// with `this` in ebx: written out in the switch it keeps the loop counter
// zeroed in esi from the function entry and reuses that register for every
// zero in the function, which the original does not do.
static inline int check_second(Class_00490360* self)
{
    if (self->secondCount == 0) {
        self->second[self->secondCount] = new Class_0048f840;
        self->secondCount++;
    }
    for (int i = 0; i < self->secondCount; i++) {
        if (self->second[i]->FUN_0048ea00())
            return 1;
    }
    return 0;
}

// FUNCTION: 0x490360
int Class_00490360::FUN_00490360()
{
    if (field_88) {
        if (FUN_0041d8b0()) {
            if (DAT_0051e6c4 == 0) {
                DAT_0051e6c4 = (int)((__int64)rand() * 0x2328 / 0x8000) + 0x2328;
            }
            if (DAT_0051e6c4 <= g_game->ticks) {
                DAT_0051e6c4 = 0;
                return 1;
            }
        }
        // The two mode cases share one body, but writing them out separately
        // is what makes MSVC lower the switch to the dec/je chain.
        switch (g_game->field_391e9->FUN_00435100()) {
        case 1:
            return check_second(this);
        case 2:
            return g_game->players[g_game->field_2a42].field_144 == 0;
        case 3:
            return g_game->players[g_game->field_2a42].field_144 == 0;
        }
    }
    return 0;
}
